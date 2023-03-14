#pragma once

#include "../common/resp_schema.hxx"
#include "../common/req_schema.hxx"
#include "../common/rpc_schema.hxx"
#include <netinet/ip.h>
#define BUFFER_SIZE 1024*10 

class Network {
private: 
    xml_schema::namespace_infomap map;
    xml_schema::properties properties;
public:
    int sock;
    struct sockaddr_in local;
    char buf[BUFFER_SIZE];

    Network();
    void establishConnection(const char* ip, const char* port);
    void closeConnection();
    void sendMessage(message_t msg);
    void makeRPCCall(rpc_call rpc);
    std::unique_ptr<response_t> receiveResponse();

};