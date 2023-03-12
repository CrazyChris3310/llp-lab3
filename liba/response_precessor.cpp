#include <sstream>
#include "../common/resp_schema.hxx"


response_t parseResponse(char* xml) {
    std::istringstream iss(xml);
    xml_schema::properties properties;
	properties.no_namespace_schema_location("../resp_chema.xsd");
    std::unique_ptr<response_t> resp(response(iss, 0, properties));

    return *resp;
}