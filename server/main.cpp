#include <iostream>
#include <sstream>
#include <signal.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../common/req_schema.hxx"
#include "xml_configurer.h"
#include "enum_resolver.h"
#include "response_builder.h"
extern "C" {
	#include "../dbms/src/main/user_interface/database.h"
	#include "../dbms/src/main/user_interface/query.h"
	#include "../dbms/src/main/user_interface/read_scan.h"
}
#include "network.hpp"

#define BUFFER_SIZE 1024*10
#define MAX_CLIENTS 1


Database* database;

int processRequest(request_t req, Network& net) {
	int code = 200;
	std::string message = "OK";

	header_t header(0);
	QueryType type = resolveQueryType(req.type());
	if (type == CREATE_QUERY) {
		Schema* schema = parseCreateQuery(req);
		createTable(database, schema);
		destroySchema(schema);
	} else if (type == DROP_QUERY) {
		const char* name = parseDropQuery(req);
		dropTable(database, name);
	} else if (type == SELECT_QUERY) {
		SelectQuery* query = parseSelectQuery(req);
		ScanInterface* scan = performSelectQuery(database, query);
		if (scan == NULL) {
			message = "Schema table doesn't exists";
			code = 404;
			printf("%s\n", message.c_str());
		}

		try {
			int maxCol = buildHeader(scan, &header);
			response_t resp(200, "OK", false);
			resp.header(header);
			net.sendResponse(resp);

			int status;
			body_t body;
			int count = 10;
			while(next(scan)) {
				if (count == 0) {
					net.awaitRPCCall();
					resp = response_t(200, "OK", false);
					resp.body(body);
					net.sendResponse(resp);
					body = body_t();
					count = 10;
				}
				addNextRecord(body, scan, maxCol);
				count -= 1;
			}
			net.awaitRPCCall();
			resp = response_t(200, "OK", false);
			resp.body(body);
			net.sendResponse(resp);

			net.awaitRPCCall();
		} catch (ClientDisconnectedException& e){
			destroySelectQuery(query);
			destroyScanner(scan);
			throw e;
		}

		destroySelectQuery(query);
		destroyScanner(scan);
	} else if (type == UPDATE_QUERY) {
		UpdateQuery* query = parseUpdateQuery(req);
		performUpdateQuery(database, query);
		destroyUpdateQuery(query);
	} else if (type == INSERT_QUERY) {
		InsertQuery* query = parseInsertQuery(req);
		performInsertQuery(database, query);
		destroyInsertQuery(query);
	} else if (type == DELETE_QUERY) {
		DeleteQuery* query = parseDeleteQuery(req);
		performDeleteQuery(database, query);
		destroyDeleteQuery(query);
	}
	net.sendResponse(response_t(code, message, true));
	return 0;
}

void processConnect(std::string name, Network& net) {
	closeDatabase(database);
	database = openDatabase(name.c_str());
	net.sendResponse(response_t(200, "OK", true));
}

int processMessage(std::unique_ptr<message_t>& msg, Network& net) {
	if (msg.get()->connect()) {
		processConnect(msg.get()->database().get(), net);
		return 0;
	} else {
		// handle error
		if (database == NULL) {
			return 1;
		}
		return processRequest(msg.get()->request().get(), net);
	}
}

void shutdown(int signum) {
	printf("Shutting down server\n");
	if (database != NULL) {
		closeDatabase(database);
	}
	close(ss);
	exit(0);
}

void runClientLoop(Network& net) {
	try {
		while (true) {
			std::unique_ptr<message_t> msg;
			try {
				msg = net.receiveMessage();
			} catch (InvalidSchemaException& e) {
				std::cout << e.what() << std::endl;
				continue;
			}
			if (msg == NULL) {
				break;
			}
			processMessage(msg, net);
		}
	} catch (ClientDisconnectedException& e) {
		std::cout << e.what() << std::endl;
	}
	closeDatabase(database);
	database = NULL;
}

int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		return 1;
	}
	signal(SIGINT, shutdown);

	try {
		Network net(argv[1]);

		while (true) {
			net.acceptClient();
			runClientLoop(net);
		}

		closeDatabase(database);
		close(ss);
	} catch (SocketException& e) {
		std::cout << e.what() << std::endl;
	} catch (IOException& e) {
		std::cout << e.what() << std::endl;
	}

    return 0;
}