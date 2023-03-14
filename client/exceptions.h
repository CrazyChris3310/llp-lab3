#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

struct MyException : std::exception {

    std::string message;

    MyException(std::string message) {
        this->message = message;
    }

    const char* what() const throw () {
        return message.c_str();
    }

};

struct UnsupportedSyntaxException : public MyException {
    UnsupportedSyntaxException(std::string message) : MyException(message) {};
};

struct ServerDisconnectedException : public MyException {
    ServerDisconnectedException(std::string message) : MyException(message) {}
};

struct IOException : public MyException {
    IOException(std::string message) : MyException(message) {}
};

struct SocketException : public MyException {
    SocketException(std::string message) : MyException(message) {}
};

struct InvalidSchemaException : public MyException {
    InvalidSchemaException(std::string message) : MyException(message) {}
};

struct DatabaseException : public MyException {
    DatabaseException(std::string message) : MyException(message) {}
};

#endif