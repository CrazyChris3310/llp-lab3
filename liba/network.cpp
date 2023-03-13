#include "network.h"
#include <string>
#include <sstream>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "exceptions.h"


Network::Network() {
    map[""].name = "";
    map[""].schema = "req_schema.xsd";
    properties.no_namespace_schema_location("../resp_schema.xsd");
    this->sock = -1;
}

void Network::establishConnection(const char* ip, const char* port) {
    closeConnection();

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock < 0) {
        throw SocketException("Failed to aquire socket");
    }
    struct sockaddr_in local;
    inet_aton(ip, &local.sin_addr);
    local.sin_port = htons(atoi(port));
    local.sin_family = AF_INET;

    int status = connect(sock, (struct sockaddr*)&local, sizeof(local));
    if (status < 0) {
        throw SocketException("Failed to connect to remote server");
    }
}

void Network::closeConnection() {
    if (this->sock > 0) {
        close(this->sock);
    }
    this->sock = -1;
}

void Network::sendMessage(message_t msg) {
    std::ostringstream oss;
    message(oss, msg, map);
    if (write(this->sock, oss.str().c_str(), oss.str().length()) < 0) {
        throw IOException("Failed to send data");
    }
}

void Network::makeRPCCall(rpc_call rpc) {
    std::ostringstream oss;

    xml_schema::namespace_infomap rpc_map;
    rpc_map[""].name = "";
    rpc_map[""].schema = "rpc_schema.xsd";
    rpc_call_(oss, rpc, rpc_map);
    if (write(this->sock, oss.str().c_str(), oss.str().length()) < 0) {
        throw IOException("Failed to send data");
    }
}

std::unique_ptr<response_t> Network::receiveResponse() {
    int bytes_read = recv(this->sock, buf, BUFFER_SIZE, 0);
    if (bytes_read == 0) {
        throw ServerDisconnectedException("Server has disconnected");
    }
    else if (bytes_read < 0) {
        throw IOException("Failed to receive data");
    } 
    buf[bytes_read] = 0;
    // printf("Received message:\n%s", buf);

    std::istringstream iss(buf);
    try {
        return response(iss, 0, properties);
    } catch(std::exception& e) {
        throw InvalidSchemaException(e.what());
    }
}