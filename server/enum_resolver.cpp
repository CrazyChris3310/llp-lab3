#include "exceptions.h"
#include "enum_resolver.h"
#include <string>

// may not work, should use strcmp()
DataType resolveDataType(std::string str) {
    if (str == "int") {
        return INT;
    } else if (str == "float") {
        return FLOAT;
    } else if (str == "bool") {
        return BOOL;
    } else if (str == "string") {
        return STRING;
    } else if (str == "ref") {
        return REF;
    }

    throw UndefinedEnumValueException("DataType has no value " + str);
}

std::string getStringValue(DataType type) {
    if (type == INT) {
        return "int";
    } else if (type == FLOAT) {
        return "float";
    } else if (type == BOOL) {
        return "bool";
    } else if (type == STRING) {
        return "string";
    } else if (type == REF) {
        return "ref";
    } else {
        return "UNKNOWN";
    }
} 

//hande errors...
QueryType resolveQueryType(std::string type) {
    if (type == "CREATE_QUERY")
        return CREATE_QUERY;
    else if (type == "DROP_QUERY")
        return DROP_QUERY;
    else if (type == "SELECT_QUERY")
        return SELECT_QUERY;
    else if (type == "INSERT_QUERY")
        return INSERT_QUERY;
    else if (type == "UPDATE_QUERY") 
        return UPDATE_QUERY;
    else if (type == "DELETE_QUERY")
        return DELETE_QUERY;

    throw UndefinedEnumValueException("QueryType has no value " + type);
}

std::string getStringValue(QueryType type) {
    if (type == CREATE_QUERY)
        return "CREATE_QUERY";
    else if (type == DROP_QUERY)
        return "DROP_QUERY";
    else if (type == SELECT_QUERY)
        return "SELECT_QUERY";
    else if (type == INSERT_QUERY)
        return "INSERT_QUERY";
    else if (type == UPDATE_QUERY)
        return "UPDATE_QUERY";
    else if (type == DELETE_QUERY)
        return "DELETE_QUERY";
    else
        return "UNKNOWN";
}

CompareOperator resolveCompareOperator(std::string str) {
    if (str == "GT")
        return GREATER;
    else if (str == "LT")
        return LESS;
    else if (str == "EQ")
        return EQUAL;
    else if (str == "NEQ")
        return NOT_EQUAL;
    else if (str == "GTE") 
        return GREATER_OR_EQUAL;
    else if (str == "LTE")
        return LESS_OR_EQUAL;
    
    throw UndefinedEnumValueException("DataType has no value " + str + "\n");
}

std::string getStringValue(CompareOperator oper) {
    if (oper == GREATER)
        return "GT";
    else if (oper == LESS)
        return "LT";
    else if (oper == EQUAL)
        return "EQ";
    else if (oper == NOT_EQUAL)
        return "NEQ";
    else if (oper == GREATER_OR_EQUAL) 
        return "GTE";
    else if (oper == LESS_OR_EQUAL)
        return "LTE";
    else
        return "UNKNOWN";
}