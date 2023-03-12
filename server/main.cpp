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
struct sockaddr_in local;
int ss;

void printConstant(Constant& constant) {
	if (constant.type == INT) {
		printf("%-20" PRId64, constant.value.intVal);
	} else if (constant.type == FLOAT) {
		printf("%-20.4f", constant.value.floatVal);
	} else if (constant.type == BOOL) {
		printf("%-20s", constant.value.boolVal ? "true" : "false");
	} else {
		printf("%-20s", constant.value.stringVal);
	}
}

int sendResponse(std::string msg, int sc) {
	int byte_written = write(sc, msg.c_str(), msg.length());
	if (byte_written <= 0) {
		perror("Error while sending data\n");
		return 1;
	}
	return 0;
}

int processRequest(request_t req, Network& net) {
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
			printf("Schema table doesn't exists\n");
			return 1;
		}

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
		destroySelectQuery(query);

		net.awaitRPCCall();
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
	net.sendResponse(getFinishResponse());
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

int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		return 1;
	}
	signal(SIGINT, shutdown);

	Network net(argv[1]);

	while (true) {
		net.acceptClient();

		while (true) {
			std::unique_ptr<message_t> msg = net.receiveMessage();
			if (msg == NULL) {
				break;
			}
			processMessage(msg, net);
		}

		closeDatabase(database);
		database = NULL;
	}

	closeDatabase(database);
	close(ss);

    return 0;
}