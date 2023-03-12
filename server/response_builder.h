#include "../common/resp_schema.hxx"
extern "C" {
    #include "../dbms/src/main/user_interface/predicates.h"
    #include "../dbms/src/main/middleware/schema.h"
    #include "../dbms/src/main/user_interface/read_scan.h"
}
#include <string>

std::string getStringConstant(Constant& constant);
void addNextRecord(body_t& body, ScanInterface* scan, int maxCol);
int buildHeader(ScanInterface* scan, header_t* header);
void sendBody(body_t& body);
std::string bodyToString(body_t& body);
response_t getFinishResponse();