#include <iostream>
#include <string>
#include "../client/ast.h"
#include "../client/parser.h"
#include "../client/lexer.h"
#include "evaluate.h"
#include <sstream>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int parseInput(std::string& query, NodeWrapper& nodeWrapper) {
    yy_scan_string(query.c_str());
    int code = yyparse(nodeWrapper);
    yylex_destroy();
    return code;
}

std::string toXmlString(NodeWrapper& wrapper) {
    std::ostringstream oss;
    request_t req = toXmlRequest(wrapper);

    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "req_schema.xsd";

    request(oss, req, map);

    return oss.str();
}


void sendData(int sock, std::string buf) {
    int bytes_sent = send(sock, buf.c_str(), buf.length(), 0);
    if (bytes_sent != buf.length()) {
        std::cout << "WARNING! Not everything was sent to server\n";
        std::cout << "Only " << bytes_sent << " bytes were delivered\n";
    }
}


int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Usage: %s <host> <port> <database>\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    inet_aton(argv[1], &local.sin_addr);
	local.sin_port = htons(atoi(argv[2]));
	local.sin_family = AF_INET;

    int status = connect(sock, (struct sockaddr*)&local, sizeof(local));
    if (status < 0) {
        std::cout << "Error during connection\n";
        return 1;
    }

    sendData(sock, argv[3]);

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
                sendData(sock, toXmlString(nodeWrapper));
            }
            buf.clear();
            std::cout << "> ";
        }
    }

    return 0;
}