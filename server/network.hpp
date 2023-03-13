#pragma once

#include "../common/resp_schema.hxx"
#include "../common/req_schema.hxx"
#include "../common/rpc_schema.hxx"
#include "exceptions.h"
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
        if (ss < 0) {
            throw SocketException("Unable to create socket");
        }

        if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            perror("setsockopt failed\n");
            throw SocketException("Setting socket options failed");
        }

        inet_aton("127.0.0.1", &local.sin_addr);
        local.sin_port = htons(atoi(port));
        local.sin_family = AF_INET;

        int port_taken = bind(ss, (struct sockaddr*)&local, sizeof(local));
        if (port_taken < 0) {
            throw SocketException("Error binding to port: " + std::string(port));
        }

        if (listen(ss, MAX_CLIENTS) < 0) {
            throw SocketException("Listen failed");
        }
    }

    void acceptClient() {
        cs = accept(ss, NULL, NULL);
		if (cs < 0) {
            throw IOException("Accept failed");
		}
        printf("New client connected, socket fd is %d\n", cs);
    }

    void closeConnection() {
        close(cs);
        close(ss);
    }

    void sendResponse(response_t msg) {
        std::ostringstream oss;
        response(oss, msg, map);
        if (write(cs, oss.str().c_str(), oss.str().length()) < 0) {
            throw IOException("Unable to send data to client");
        }
    }

    std::unique_ptr<message_t> receiveMessage() {
        int bytes_read = read(cs, buf, BUFFER_SIZE);
        
        if (bytes_read < 0) {
            throw IOException("Error while receiving data");
        } else if (bytes_read == 0) {
            close(cs);
            throw ClientDisconnectedException("Client " + std::to_string(cs) + " disconncted");
        }

        printf("Received %d bytes from client %d:\n%s\n", bytes_read, cs, buf);
        buf[bytes_read] = 0;
        std::istringstream iss(buf);
        try {
            return message(iss, 0, properties);
        } catch (std::exception& e) {
            throw InvalidSchemaException(e.what());
        }
    }

    std::unique_ptr<rpc_call> awaitRPCCall() {
        int bytes_read = read(cs, buf, BUFFER_SIZE);
        if (bytes_read < 0) {
            throw IOException("Error while waiting for RPC");
        } else if (bytes_read == 0) {
            close(cs);
            throw ClientDisconnectedException("Client " + std::to_string(cs) + " disconncted");
        }

        xml_schema::properties props;
        props.no_namespace_schema_location("../rpc_schema.xsd");

        printf("Received %d RPC call from client %d:\n%s\n", bytes_read, cs, buf);
        buf[bytes_read] = 0;
        std::istringstream iss(buf);
        try {
            return rpc_call_(iss, 0, props);
        } catch (std::exception& e) {
            throw InvalidSchemaException(e.what());
        }
    }

};