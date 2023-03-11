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

// handle errors
Constant parseConstant(constant_t& constant) {
    DataType dt = resolveDataType(constant.type());
    switch(dt) {
        case INT: return intConstant(atoi(constant.value().c_str()));
        case FLOAT: return floatConstant(atof(constant.value().c_str()));
        case BOOL: return boolConstant(strcmp(constant.value().c_str(), "true") == 0 ? true : false);
        case STRING: return stringConstant(constant.value().c_str());
    }
    throw UndefinedEnumValueException("Cannot resolve constant for type" + constant.type());
}

Condition* parseCondition(condition_t& condition) {
    Constant constant = parseConstant(condition.constant());
    CompareOperator op = resolveCompareOperator(condition.operator_());
    const char* name = condition.field().c_str();
    return createCondition(name, constant, op);
}

Predicate* parsePredicate(predicate_t& predicate) {
    Predicate* pred = createPredicate();
    for(predicate_t::condition_const_iterator i(predicate.condition().begin());
        i != predicate.condition().end(); ++i) {
            condition_t condit = *i;
            Condition* cond = parseCondition(condit);
            addConditionDirectly(pred, cond);
    }
    return pred;
}

DeleteQuery* parseDeleteQuery(request_t& req) {
    DeleteQuery* qeury;
    delete_t del = req.delete_().get();
    std::string str = del.from();
    Predicate* predicate = parsePredicate(del.predicate());
    return createDeleteQuery(str.c_str(), predicate);
}

InsertQuery* parseInsertQuery(request_t& req) {
    insert_t ins = req.insert().get();
    std::string str = ins.into();
    InsertQuery* query = createInsertQuery(str.c_str());
    map_t values = ins.values();
    for (map_t::entry_type& entry : values.entry()) {
        Constant constant = parseConstant(entry.value());
        const char* key = entry.key().c_str();
        addInsertionField(query, key, constant);
    }
    return query;
}

SelectQuery* parseSelectQuery(request_t& req) {
    select_t sel = req.select().get();
    Predicate* pred;
    if (sel.predicate().present()) {
        pred = parsePredicate(sel.predicate().get());
    } else {
        pred = createPredicate();
    }
    std::string table = *sel.table().begin();
    SelectQuery* query = createSelectQuery(table.c_str(), pred);

    for(select_t::table_const_iterator i(sel.table().begin() + 1);
        i != sel.table().end(); ++i) {
            joinTable(query, (*i).c_str());
    }

    return query;
}

UpdateQuery* parseUpdateQuery(request_t& req) {
    update_t upd = req.update().get();
    Predicate* pred = parsePredicate(upd.predicate());
    UpdateQuery* query = createUpdateQuery(upd.table().c_str(), pred);

    map_t values = upd.fields();

    for (map_t::entry_type& entry : values.entry()) {
        Constant cnst = parseConstant(entry.value());
        addUpdateField(query, entry.key().c_str(), cnst);
    }

    return query;
}

#define DEFAULT_STRING_LENGTH 50

//handle situation when type is not correct
Schema* parseCreateQuery(request_t& req) {
    create_t crt = req.create().get();
    Schema* schema = createSchema(crt.table().c_str());
    map_t mapa = crt.fields();
    for (map_t::entry_type entry : mapa.entry()) {
        std::string& name = entry.key();
        std::string type = entry.value().value();
        int colLength;
        if (type == "int") {
            addIntField(schema, name.c_str());
        } else if (type == "float") {
            addFloatField(schema, name.c_str());
        } else if (type == "bool") {
            addBooleanField(schema, name.c_str());
        } else if (type.substr(0, 6) == "string") {
            int len = DEFAULT_STRING_LENGTH;
            std::regex pattern("\\((\\d+)\\)");
            std::smatch match;
            if (std::regex_search(type, match, pattern)) {
                std::string numString = match[1].str();
                len = std::stoi(numString);
            }
            addStringField(schema, name.c_str(), len);
        } 
    }
    return schema;
}

const char* parseDropQuery(request_t& req) {
    drop_t drop = req.drop().get();
    return drop.table().c_str();
}