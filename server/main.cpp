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
extern "C" {
	#include "../dbms/src/main/user_interface/database.h"
	#include "../dbms/src/main/user_interface/query.h"
	#include "../dbms/src/main/user_interface/read_scan.h"
}


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

void processRequest(request_t req) {
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
			return;
		}
		size_t colId = 0;
		const char* col;
		while ((col = getColumnNameById(scan, colId)) != NULL) {
			printf("%-20s", col);
			colId += 1;
		}
		printf("\n");
		int maxCol = colId;

		while(next(scan)) {
			for (colId = 0; colId < maxCol; ++colId) {
				Constant cnst = getFieldById(scan, colId);
				printConstant(cnst);
			}
			printf("\n");
		}

		destroySelectQuery(query);
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
}

void processConnect(std::string name) {
	closeDatabase(database);
	database = openDatabase(name.c_str());
}

void processMessage(char* buf, xml_schema::properties& properties) {
	std::istringstream iss(buf);
	std::unique_ptr<message_t> msg(message(iss, 0, properties));

	if (msg.get()->connect()) {
		processConnect(msg.get()->database().get());
	} else {
		// handle error
		if (database == NULL) {
			return;
		}
		processRequest(msg.get()->request().get());
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

	int opt = 1;
	xml_schema::properties properties;
	properties.no_namespace_schema_location("../req_schema.xsd");

	ss = socket(AF_INET, SOCK_STREAM, 0);
	if (ss == 0) {
		perror("Socket creation failed\n");
		return 1;
	}

	if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed\n");
        return 1;
    }

	inet_aton("127.0.0.1", &local.sin_addr);
	local.sin_port = htons(atoi(argv[1]));
	local.sin_family = AF_INET;

	int port_taken = bind(ss, (struct sockaddr*)&local, sizeof(local));
	if (port_taken < 0) {
		printf("Error binding to port: %s\n", argv[1]);
	
	}

	if (listen(ss, MAX_CLIENTS) < 0) {
        perror("listen failed");
        return 1;
    }

	char buf[BUFFER_SIZE];
	while (true) {
		int cs = accept(ss, NULL, NULL);
		if (cs < 0) {
			perror("Accept failed\n");
			return 1;
		}

		printf("New client connected, socket fd is %d\n", cs);

		while (true) {
			int bytes_read = read(cs, buf, BUFFER_SIZE);
			if (bytes_read < 0) {
				perror("Error while receiving data\n");
				return 1;
			} else if (bytes_read == 0) {
				printf("Client disconnected, socket fd is %d\n", cs);
				close(cs);
				break;
			} else {
				printf("Received %d bytes from client %d:\n%s\n", bytes_read, cs, buf);
				buf[bytes_read] = 0;
				processMessage(buf, properties);
			}
		}

		closeDatabase(database);
		database = NULL;
	}

	closeDatabase(database);
	close(ss);

    return 0;
}