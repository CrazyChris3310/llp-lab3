#pragma once

#include <exception>
#include <string>

struct MyException : std::exception {

    std::string message;

    MyException(std::string message) {
        this->message = message;
    }

    const char* what() const throw () {
        return message.c_str();
    }

};

struct UndefinedEnumValueException : public MyException {
    UndefinedEnumValueException(std::string message) : MyException(message) {}
};

struct ClientDisconnectedException : public MyException {
    ClientDisconnectedException(std::string message) : MyException(message) {}
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