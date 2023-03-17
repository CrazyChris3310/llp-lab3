#ifndef EVALUATE_H
#define EVALUATE_H

#include "../parser_lib/ast.h"
#include "../common/req_schema.hxx"
#include "../common/resp_schema.hxx"

request_t toXmlRequest(NodeWrapper& warapper);

#endif