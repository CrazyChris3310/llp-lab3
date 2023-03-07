#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

struct UnsupportedSyntaxException : std::exception {
    const char* message;

    UnsupportedSyntaxException(const char* msg) {
        message = msg;
    }

    const char* what() const throw () {
        return message;
    }

};

#endif