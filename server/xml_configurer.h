#include "../common/req_schema.hxx"
#include "../common/resp_schema.hxx"
#include "enum_resolver.h"
extern "C" {
    #include "../dbms/src/main/user_interface/query.h"
    #include "../dbms/src/main/user_interface/predicates.h"
    #include "../dbms/src/main/middleware/schema.h"
}
#include "exceptions.h"
#include <regex>

DeleteQuery* parseDeleteQuery(request_t req);
InsertQuery* parseInsertQuery(request_t req);
SelectQuery* parseSelectQuery(request_t req);
Schema* parseCreateQuery(request_t req);
const char* parseDropQuery(request_t req);