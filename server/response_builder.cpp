#include "../common/resp_schema.hxx"
#include "enum_resolver.h"
extern "C" {
    #include "../dbms/src/main/user_interface/query.h"
    #include "../dbms/src/main/user_interface/predicates.h"
    #include "../dbms/src/main/middleware/schema.h"
    #include "../dbms/src/main/user_interface/read_scan.h"
}
#include "exceptions.h"
#include <string>

std::string getStringConstant(Constant& constant) {
	if (constant.type == INT) {
        return std::to_string(constant.value.intVal);
	} else if (constant.type == FLOAT) {
        return std::to_string(constant.value.floatVal);
	} else if (constant.type == BOOL) {
        return constant.value.boolVal ? "true" : "false";
	} else {
        return std::string(constant.value.stringVal);
	}
}


void addNextRecord(body_t& body, ScanInterface* scan, int maxCol) {
    row_t row;
    for (int colId = 0; colId < maxCol; ++colId) {
        Constant cnst = getFieldById(scan, colId);
        data_column_t dc(getStringConstant(cnst), colId);
        row.row().push_back(dc);
    }
    body.row().push_back(row);
}

int buildHeader(ScanInterface* scan, header_t* header) {
    Field* col;
    int colId = 0;
    while ((col = getColumnInfoById(scan, colId)) != NULL) {
        column_t column(colId, std::string(col->name.value), getStringValue(col->type), col->len);
        header->column().push_back(column);
        colId += 1;
    }
    header->col_amount(colId);
    return colId;
}

std::string bodyToString(body_t& body) {
    std::ostringstream oss;

    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "resp_schema.xsd";

    response_t resp(200, "OK", false);
    resp.body(body);
    response(oss, resp, map);

    return oss.str();
}

void sendBody(body_t& body) {
    response_t resp(200, "OK", false);
    resp.body(body);
}

response_t getFinishResponse() {
    return response_t(200, "OK", true);
}