#include "../common/resp_schema.hxx"
#include "../common/req_schema.hxx"
extern "C" {
    #include "../dbms/src/main/user_interface/predicates.h"
    #include "../dbms/src/main/middleware/schema.h"
    #include "../dbms/src/main/user_interface/read_scan.h"
}
#include <string>

std::string getStringConstant(Constant& constant);
void addNextRecord(body_t& body, ScanInterface* scan, int maxCol);
void addRecord(body_t& body, ScanInterface* scan, header_t& header);
int buildHeader(ScanInterface* scan, header_t* header, select_t& sel);
void sendBody(body_t& body);
std::string bodyToString(body_t& body);
response_t getFinishResponse();