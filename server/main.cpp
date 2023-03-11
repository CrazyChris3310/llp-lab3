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

void printConstant(Constant& constant) {
	if (constant.type == INT) {
		printf("%-30" PRId64, constant.value.intVal);
	} else if (constant.type == FLOAT) {
		printf("%-30.2f", constant.value.floatVal);
	} else if (constant.type == BOOL) {
		printf("%-30s", constant.value.boolVal ? "true" : "false");
	} else {
		printf("%-30s", constant.value.stringVal);
	}
}

Database* database;
struct sockaddr_in local;
int ss;

void shutdown(int signum) {
	printf("Shutting down server\n");
	if (database != NULL) {
		closeDatabase(database);
	}
	close(ss);
	exit(0);
}

int main(int argc, char* argv[]) {

	if (argc != 3) {
		printf("Usage: %s <port> <db_dir>\n", argv[0]);
		return 1;
	}

	signal(SIGINT, shutdown);

	xml_schema::properties properties;
	properties.no_namespace_schema_location("../req_schema.xsd");


	Database* database;

	ss = socket(AF_INET, SOCK_STREAM, 0);

	inet_aton("127.0.0.1", &local.sin_addr);
	local.sin_port = htons(atoi(argv[1]));
	local.sin_family = AF_INET;

	int port_taken = bind(ss, (struct sockaddr*)&local, sizeof(local));
	printf("Taken port: %d\n", port_taken);

	listen(ss, 5);

	int cs = accept(ss, NULL, NULL);

	printf("Client connected\n");

	char buf[BUFFER_SIZE];
	int bytes_read = recv(cs, buf, BUFFER_SIZE, 0);
	buf[bytes_read] = 0;

	printf("Database to connect is set to database %s\n", buf);

	// std::string full_path = std::string(buf);

	database = openDatabase(buf);

	try {
		while (1) {
			int bytes_read = recv(cs, buf, BUFFER_SIZE, 0);
			buf[bytes_read] = 0;

			printf("%s\n\n", buf);

			std::istringstream iss(buf);
			std::unique_ptr<request_t> req(request(iss, 0, properties));

			QueryType type = resolveQueryType(req->type());
			if (type == CREATE_QUERY) {
				Schema* schema = parseCreateQuery(*req);
				createTable(database, schema);
				destroySchema(schema);
			} else if (type == DROP_QUERY) {
				const char* name = parseDropQuery(*req);
				dropTable(database, name);
			} else if (type == SELECT_QUERY) {
				SelectQuery* query = parseSelectQuery(*req);
				ScanInterface* scan = performSelectQuery(database, query);
				if (scan == NULL) {
					printf("Schema table doesn't exists");
					continue;
				}
				size_t colId = 0;
				const char* col;
				while ((col = getColumnNameById(scan, colId)) != NULL) {
					printf("%-15s", col);
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
				UpdateQuery* query = parseUpdateQuery(*req);
				performUpdateQuery(database, query);
				destroyUpdateQuery(query);
			} else if (type == INSERT_QUERY) {
				InsertQuery* query = parseInsertQuery(*req);
				performInsertQuery(database, query);
				destroyInsertQuery(query);
			} else if (type == DELETE_QUERY) {
				DeleteQuery* query = parseDeleteQuery(*req);
				performDeleteQuery(database, query);
				destroyDeleteQuery(query);
			}
		}
	} catch (const std::exception& e) {
		std::cout << "Error happened\n";
		std::cout << e.what() << std::endl;
	}


	closeDatabase(database);
	close(ss);

    return 0;
}