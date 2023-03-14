#include <iostream>
#include <string>
#include "../client/ast.h"
#include "../client/parser.h"
#include "../client/lexer.h"
#include "evaluate.h"
#include "response_processor.h"
#include "middleware.hpp"
#include "exceptions.h"

int parseInput(std::string& query, NodeWrapper& nodeWrapper) {
    yy_scan_string(query.c_str());
    int code = yyparse(nodeWrapper);
    yylex_destroy();
    return code;
}

void printResponse(ResultSet& rs) {
    if (rs.hasValues()) {
        if (rs.getColumnCount() > 0) {
            for (int i = 0; i < rs.getColumnCount(); ++i) {
                printf("%-20s", rs.getColumnName(i).c_str());
            }
            printf("\n");
        }
        while (rs.next()) {
            for (int i = 0; i < rs.getColumnCount(); ++i) {
                printf("%-20s", rs.getValueAt(i).c_str());
            }
            printf("\n");
        }
    } else {
        printf("No values\n");
    }
}

int main(int argc, char* argv[]) {

    // if (argc != 4) {
        // printf("Usage: %s <host> <port> <database>\n", argv[0]);
        // return 1;
    // }

    try {
        // Connection connection = getConnection(argv[1], argv[2], argv[3]);
        Connection connection = getConnection("127.0.0.1", "3434", "figurka");

        std::string buf;
        std::string line;
        std::cout << "> ";
        while(getline(std::cin, line)) {
            buf.append(line);
            buf.append("\n");
            if (line.find(';') != std::string::npos) {
                NodeWrapper nodeWrapper;
                int code = parseInput(buf, nodeWrapper);
                if (code) {
                    std::cout << "ret_code: " << code << std::endl;
                } else {
                    request_t req = toXmlRequest(nodeWrapper);
                    try {
                        ResultSet rs = connection.executeQuery(req);
                        if (nodeWrapper.queryType == SELECT_QUERY) {
                            printResponse(rs);
                        }
                    } catch (DatabaseException& e) {
                        std::cout << e.what() << std::endl;
                    }
                }
                buf.clear();
                std::cout << "> ";
            }
        }
    } catch (SocketException& e) {
        std::cout << e.what() << std::endl;
    } catch (IOException& e) {
        std::cout << e.what() << std::endl;
    } catch (ServerDisconnectedException& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}