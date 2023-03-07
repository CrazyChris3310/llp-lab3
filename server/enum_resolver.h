#ifndef ENUM_RESOLVER_H
#define ENUM_RESOLVER_H
extern "C" {
    #include "../dbms/src/main/middleware/data_type.h"
    #include "../dbms/src/main/user_interface/query.h"
    #include "../dbms/src/main/util/comparators.h"
}
#include <string>

// may not work, should use strcmp()
DataType resolveDataType(std::string str);
std::string getStringValue(DataType type);

//hande errors...
QueryType resolveQueryType(std::string type);
std::string getStringValue(QueryType type);

CompareOperator resolveCompareOperator(std::string str);
std::string getStringValue(CompareOperator oper);

#endif