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

Database* database;


void executeSelect(request_t req, Network& net, std::string& message, int& code) {
	SelectQuery* query = parseSelectQuery(req);
	ScanInterface* scan = performSelectQuery(database, query);
	if (scan == NULL) {
		message = "Schema table doesn't exists";
		code = 404;
		printf("%s\n", message.c_str());
		destroySelectQuery(query);
		return;
	}

	try {
		header_t header(0);
		response_t resp(200, "OK", false);
		try {
			buildHeader(scan, &header, req.select().get()); 
			resp.header(header);
			net.sendResponse(resp);
		} catch (DatabaseException& e) {
			destroySelectQuery(query);
			code = 418;
			message = std::string("I'm teapot") + e.what();
			printf("Ya chainik\n");
			return;
		}

		bool hasBody = false;
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
				hasBody = false;
			}
			addRecord(body, scan, header);
			hasBody = true;
			count -= 1;
		}
		net.awaitRPCCall();
		if (hasBody) {
			resp = response_t(200, "OK", false);
			resp.body(body);
			net.sendResponse(resp);
		} else {
			return;
		}

		net.awaitRPCCall();
	} catch (ClientDisconnectedException& e) {
		destroySelectQuery(query);
		destroyScanner(scan);
		throw e;
	} catch (IOException& e){
		destroySelectQuery(query);
		destroyScanner(scan);
		throw e;
	} catch (InvalidSchemaException& e){
		destroySelectQuery(query);
		destroyScanner(scan);
		throw e;
	}

	destroySelectQuery(query);
	destroyScanner(scan);
}

void executeUpdate(request_t& req, Network& net, std::string& message, int& code) {
	UpdateQuery* query = parseUpdateQuery(req);
	performUpdateQuery(database, query);
	destroyUpdateQuery(query);
}

void executeInsert(request_t& req, Network& net, std::string& message, int& code) {
	InsertQuery* query = parseInsertQuery(req);
	performInsertQuery(database, query);
	destroyInsertQuery(query);
}

void executeDelete(request_t& req, Network& net, std::string& message, int& code) {
	DeleteQuery* query = parseDeleteQuery(req);
	performDeleteQuery(database, query);
	destroyDeleteQuery(query);
}

void executeCreate(request_t& req, Network& net, std::string& message, int& code) {
	Schema* schema = parseCreateQuery(req);
	createTable(database, schema);
	destroySchema(schema);
}

void executeDrop(request_t& req, Network& net, std::string& message, int& code) {
	const char* name = parseDropQuery(req);
	dropTable(database, name);
}

int processRequest(request_t req, Network& net) {
	int code = 200;
	std::string message = "OK";

	QueryType type = resolveQueryType(req.type());
	if (type == CREATE_QUERY) {
		executeCreate(req, net, message, code);
	} else if (type == DROP_QUERY) {
		executeDrop(req, net, message, code);
	} else if (type == SELECT_QUERY) {
		executeSelect(req, net, message, code);
	} else if (type == UPDATE_QUERY) {
		executeUpdate(req, net, message, code);
	} else if (type == INSERT_QUERY) {
		executeInsert(req, net, message, code);
	} else if (type == DELETE_QUERY) {
		executeDelete(req, net, message, code);
	}
	net.sendResponse(response_t(code, message, true));
	return 0;
}

void processConnect(std::string name, Network& net) {
	closeDatabase(database);
	database = openDatabase(name.c_str());
	net.sendResponse(response_t(200, "OK", true));
}

int processMessage(message_t& msg, Network& net) {
	if (msg.connect()) {
		processConnect(msg.database().get(), net);
		return 0;
	} else {
		// handle error
		if (database == NULL) {
			return 1;
		}
		return processRequest(msg.request().get(), net);
	}
}

void shutdown(int signum) {
	printf("Shutting down server\n");
	if (database != NULL) {
		closeDatabase(database);
	}
	exit(0);
}

void runClientLoop(Network& net) {
	try {
		while (true) {
			message_t msg(false);
			try {
				msg = net.receiveMessage();
			} catch (InvalidSchemaException& e) {
				std::cout << e.what() << std::endl;
				continue;
			}
			processMessage(msg, net);
		}
	} catch (ClientDisconnectedException& e) {
		std::cout << e.what() << std::endl;
	} catch (IOException& e) {
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
	} catch (SocketException& e) {
		std::cout << e.what() << std::endl;
	} catch (IOException& e) {
		std::cout << e.what() << std::endl;
	}

    return 0;
}