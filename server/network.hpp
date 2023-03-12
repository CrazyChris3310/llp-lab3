#pragma once

#include "../common/resp_schema.hxx"
#include "../common/req_schema.hxx"
#include "../common/rpc_schema.hxx"
#include <netinet/ip.h>
#include <string>
#include <sstream>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024*10 
#define MAX_CLIENTS 1

class Network {
private: 
    xml_schema::namespace_infomap map;
    xml_schema::properties properties;
    int ss;
    int cs;
    struct sockaddr_in local;
    char buf[BUFFER_SIZE];
public:

    Network(const char* port) {
        map[""].name = "";
        map[""].schema = "resp_schema.xsd";
        properties.no_namespace_schema_location("../req_schema.xsd");

        int opt = 1;
        ss = socket(AF_INET, SOCK_STREAM, 0);
        if (ss == 0) {
            perror("Socket creation failed\n");
        }

        if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            perror("setsockopt failed\n");
        }

        inet_aton("127.0.0.1", &local.sin_addr);
        local.sin_port = htons(atoi(port));
        local.sin_family = AF_INET;

        int port_taken = bind(ss, (struct sockaddr*)&local, sizeof(local));
        if (port_taken < 0) {
            printf("Error binding to port: %s\n", port);
        }

        if (listen(ss, MAX_CLIENTS) < 0) {
            perror("listen failed");
        }
    }

    int acceptClient() {
        cs = accept(ss, NULL, NULL);
		if (cs < 0) {
			perror("Accept failed\n");
			return 1;
		}
        printf("New client connected, socket fd is %d\n", cs);
        return 0;
    }

    void closeConnection() {
        close(cs);
        close(ss);
    }

    void sendResponse(response_t msg) {
        std::ostringstream oss;
        response(oss, msg, map);
        send(cs, oss.str().c_str(), oss.str().length(), 0);
    }

    std::unique_ptr<message_t> receiveMessage() {
        int bytes_read = recv(cs, buf, BUFFER_SIZE, 0);
        
        if (bytes_read < 0) {
            perror("Error while receiving data\n");
            return NULL;
        } else if (bytes_read == 0) {
            printf("Client disconnected, socket fd is %d\n", cs);
            close(cs);
            return NULL;
        }

        printf("Received %d bytes from client %d:\n%s\n", bytes_read, cs, buf);
        buf[bytes_read] = 0;
        std::istringstream iss(buf);
        return message(iss, 0, properties);
    }

    std::unique_ptr<rpc_call> awaitRPCCall() {
        int bytes_read = recv(cs, buf, BUFFER_SIZE, 0);
        if (bytes_read < 0) {
            perror("Error while receiving data\n");
            return NULL;
        } else if (bytes_read == 0) {
            printf("Client disconnected, socket fd is %d\n", cs);
            close(cs);
            return NULL;
        }

        xml_schema::properties props;
        props.no_namespace_schema_location("../rpc_schema.xsd");

        printf("Received %d RPC call from client %d:\n%s\n", bytes_read, cs, buf);
        buf[bytes_read] = 0;
        std::istringstream iss(buf);
        return rpc_call_(iss, 0, props);
    }

};