#include <iostream>
#include <string>
#include "../client/ast.h"
#include "../client/parser.h"
#include "../client/lexer.h"
#include "evaluate.h"
#include <sstream>

int parseInput(std::string& query, NodeWrapper& nodeWrapper) {
    yy_scan_string(query.c_str());
    int code = yyparse(nodeWrapper);
    yylex_destroy();
    return code;
}

void print(NodeWrapper& wrapper) {
    std::ostringstream oss;
    request_t req = toXmlRequest(wrapper);

    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "req_schema.xsd";

    request(oss, req, map);

    std::cout << oss.str() << std::endl;
}

int main(int argc, char* argv[]) {

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
                // nodeWrapper.node->print(0);
                // delete nodeWrapper.node;
                print(nodeWrapper);
            }
            buf.clear();
            std::cout << "> ";
        }
    }

    return 0;
}