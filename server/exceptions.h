#include <exception>
#include <string>

struct UndefinedEnumValueException : std::exception {

    std::string message;

    UndefinedEnumValueException(std::string message) {
        this->message = message;
    }

    const char* what() const throw () {
        return message.c_str();
    }

};