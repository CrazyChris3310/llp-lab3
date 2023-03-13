#pragma once

#include "../common/req_schema.hxx"
#include "../common/resp_schema.hxx"
#include "../common/rpc_schema.hxx"
#include "../client/ast.h"
#include "exceptions.h"
#include "network.h"
#include <string>

class ResultSet {
private:
    int pos = 0;
    std::unique_ptr<response_t> current;
    std::unique_ptr<header_t> header;
    int bodySize = 0;

    void getNextBatch() {
        rpc_call rpc("next");
        net.makeRPCCall(rpc);

        current = net.receiveResponse();
        pos = -1;
        if (current.get()->body().present()) {
            bodySize = current.get()->body().get().row().size();
        }
    }


    int getHeader() {
        current = net.receiveResponse();

        if (current.get()->finished()) {
            if (current.get()->status() != 200) {
                throw DatabaseException(current.get()->message());
            }
            return 0;
        }
        if (current.get()->header().present()) {
            header = std::make_unique<header_t>(current.get()->header().get());
            return 0;
        }
        return 1;
    }

public:
    Network net;

    ResultSet(Network net) {
        this->net = net;
        int status = getHeader();
        current = NULL;
    }

    bool next() {
        if (current == NULL) {
            getNextBatch();
        }
        if (current.get()->finished()) {
            return false;
        }
        if (current.get()->body().present()) {
            pos += 1;
            if (pos >= bodySize) {
                getNextBatch();
                pos = 0;
                return !current.get()->finished();
            }
            return true;
        }
        return false;
    }

    std::string getColumnName(int id) {
        return header.get()->column().at(id).name();
    }

    std::string getValueAt(int id) {
        return current.get()->body().get().row().at(pos).row().at(id).value();
    }

    int getColumnCount() {
        return header.get()->col_amount();
    }

    bool hasValues() {
        if (header == NULL) {
            return false;
        }
        return true;
    }
};

class Connection {
private:
    Network net;

    Connection(Network net, const char* dbName) {
        this->net = net;
        connectTo(dbName);
    }

    void connectTo(const char* dbName) {
        message_t msg(true);
        msg.database(dbName);
        net.sendMessage(msg);
        net.receiveResponse();
    }
public:

    ResultSet executeQuery(request_t query) {
        message_t msg(false);
        msg.request(query);
        net.sendMessage(msg);
        return ResultSet(net);
    }

    friend Connection getConnection(const char* ip, const char* port, const char* dbName);
};

Connection getConnection(const char* ip, const char* port, const char* dbName) {
    Network net;
    net.establishConnection(ip, port);
    return Connection(net, dbName);
}