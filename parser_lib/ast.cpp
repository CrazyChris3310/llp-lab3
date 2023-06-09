#include <iostream>
#include <string>
#include <stdlib.h>
#include "ast.h"

void printKeyVal(const char* key, const char* val, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << key << ": " << val << std::endl;
}

const char* getStringNodeType(NodeType type) {
    switch (type) {
        case FOR_NODE:
            return "for";
        case ACTION_NODE:
            return "action";
        case FILTER_NODE:   
            return "filter";
        case RETURN_NODE:   
            return "return";   
        case UPDATE_NODE:
            return "update";
        case REMOVE_NODE:
            return "remove";
        case INSERT_NODE:
            return "insert";
        case MAP_NODE:
            return "map";
        case MAP_ENTRY_NODE:
            return "map_entry"; 
        case CONDITION_NODE:  
            return "condition";
        case CONDITION_UNION_NODE: 
            return "condition_union";
        case CONSTANT_NODE: 
            return "constant";
        case CREATE_TABLE_NODE:
            return "create_table";
        case DROP_TABLE_NODE:
            return "drop_table";
        case RET_ALL_NODE:
            return "return_all_node";
        default:
            return "unknown";
    }
}

const char* getStringQueryType(QueryType type) {
    switch(type) {
        case CREATE_QUERY:
            return "CREATE_QUERY";
        case UPDATE_QUERY:
            return "UPDATE_QUERY";
        case SELECT_QUERY:
            return "SELECT_QUERY";
        case DELETE_QUERY:
            return "DELETE_QUERY";
        case INSERT_QUERY:
            return "INSERT_QUERY";
        case DROP_QUERY:
            return "DROP_QUERY";
        default:
            return "unkonw query type";
    }
}

// ------------------------------------------ ForNode ------------------------------------------

ForNode::ForNode(const char* variable, const char* tableName, Node* action) {
    this->variable = variable;
    this->tableName = tableName;
    this->action = action;
    this->nodeType = FOR_NODE;
}

void ForNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("variable", this->variable, depth);
    printKeyVal("table", this->tableName, depth);
    printKeyVal("actions", "", depth);
    if (this->action != nullptr) {
        this->action->print(depth + 1);
    }
}

ForNode::~ForNode() {
    delete this->action;
    free((void*)this->variable);
    free((void*)this->tableName);
}

void ReturnAllNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("value", "ALL", depth);
}

// ------------------------------------------ ActionNode ------------------------------------------

void ActionNode::addAction(Node* action) { 
    this->actions.push_back(action); 
}

void ActionNode::print(int depth) {
    for (auto arg : this->actions) {
        printKeyVal("action", "", depth);
        arg->print(depth + 1);
    }
}

ActionNode::~ActionNode() {
    for (auto action : this->actions) {
        delete action;
    }
}

// ------------------------------------------ Constant ------------------------------------------

std::string Constant::getStrType() {
    switch (this->type) {
        case INT:
            return "int";
        case FLOAT:
            return "float";
        case STRING:
            return "string";
        case BOOL:
            return "bool";
        case REF:
            return "ref";
        default:
            return "unknown";
    }
}

void Constant::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("type", this->getStrType().c_str(), depth);
    printKeyVal("value", this->getStrVal().c_str(), depth);
}

// ------------------------------------------ Condition ------------------------------------------

Condition::Condition(Constant* lval, Constant* rval, ConstantOperation op): lval(lval), rval(rval) {
    this->op = op;
    this->nodeType = CONDITION_NODE;
}

void Condition::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("Operation", operation_str[this->op], depth);
    printKeyVal("Left", "", depth);
    this->lval->print(depth + 1);
    printKeyVal("Right", "", depth);
    this->rval->print(depth + 1);
}

Condition::~Condition() {
    delete this->lval;
    delete this->rval;
}

// ------------------------------------------ ConditionUnion ------------------------------------------

const char* ConditionUnion::getStrOperator() {
    switch (this->op) {
        case AND:
            return "and";
        case OR:
            return "or";
        default:
            return "unknown";
    }
};

ConditionUnion::ConditionUnion(LogicalOp op, Predicate* lval, Predicate* rval) {
    this->op = op;
    this->lval = lval;
    this->rval = rval;
    this->nodeType = CONDITION_UNION_NODE;
}

void ConditionUnion::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("Operation", getStrOperator(), depth);
    printKeyVal("Left", "", depth);
    this->lval->print(depth + 1);
    printKeyVal("Right", "", depth);
    this->rval->print(depth + 1);
}

ConditionUnion::~ConditionUnion() {
    delete this->lval;
    delete this->rval;
}

// ------------------------------------------ FilterNode ------------------------------------------

FilterNode::FilterNode(Predicate* predicate) {
    this->predicate = predicate;
    this->nodeType = FILTER_NODE;
}

void FilterNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("predicate", "", depth);
    this->predicate->print(depth + 1);
}

FilterNode::~FilterNode() {
    delete this->predicate;
}

// ------------------------------------------ ReturnAction ------------------------------------------

ReturnAction::ReturnAction(Node* retVal) {
    this->retVal = retVal;
    this->nodeType = RETURN_NODE;
}

void ReturnAction::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("return_val", "", depth);
    this->retVal->print(depth + 1);
}

ReturnAction::~ReturnAction() {
    delete this->retVal;
}

// ------------------------------------------ UpdateAction ------------------------------------------

UpdateAction::UpdateAction(const char* variable, MapNode* value, const char* table) {
    this->variable = variable;  
    this->value = value;
    this->table = table;
    this->nodeType = UPDATE_NODE;
}

void UpdateAction::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("variable", this->variable, depth + 1);
    printKeyVal("table", this->table, depth + 1);
    this->value->print(depth + 1);
}

UpdateAction::~UpdateAction() {
    delete this->value;
    free((void*)this->variable);
    free((void*)this->table);
}

// ------------------------------------------ RemoveAction ------------------------------------------

RemoveAction::RemoveAction(const char* variable, const char* table) {
    this->variable = variable;
    this->table = table;
    this->nodeType = REMOVE_NODE;
}

void RemoveAction::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("variable", this->variable, depth + 1);
    printKeyVal("table", this->table, depth + 1);
}

RemoveAction::~RemoveAction() {
    free((void*)this->variable);
    free((void*)this->table);
}

// ------------------------------------------ MapEntry ------------------------------------------

MapEntry::MapEntry(const char* key, Constant* value) {
    this->key = key;
    this->value = value;
    this->nodeType = MAP_ENTRY_NODE;
}

void MapEntry::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("key", this->key, depth);
    printKeyVal("value", "", depth);
    this->value->print(depth + 1);
}

MapEntry::~MapEntry() {
    delete this->value;
    free((void*)this->key);
}

// ------------------------------------------ MapNode ------------------------------------------

void MapNode::addEntry(MapEntry* entry) {
    this->entries.push_back(entry);
}

void MapNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("entries", "", depth);
    for (auto entry : this->entries) {
        printKeyVal("entry", "", depth + 1);
        entry->print(depth + 2);
    }
}

MapNode::~MapNode() {
    for (auto entry : this->entries) {
        delete entry;
    }
}

// ------------------------------------------ InsertNode ------------------------------------------

InsertNode::InsertNode(MapNode* map, const char* table) {
    this->map = map;
    this->table = table;
    this->nodeType = INSERT_NODE;
}

void InsertNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("table", this->table, depth );
    printKeyVal("values", "", depth);
    this->map->print(depth + 1);
}

InsertNode::~InsertNode() {
    delete this->map;
    free((void*)this->table);
}

// ------------------------------------------ CreateTableNode ------------------------------------------

CreateTableNode::CreateTableNode(const char* table, MapNode* fields) {
    this->fields = fields;
    this->table = table;
    this->nodeType = CREATE_TABLE_NODE;
}

void CreateTableNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("table", this->table, depth);
    printKeyVal("fields", "", depth);
    this->fields->print(depth + 1);
}

CreateTableNode::~CreateTableNode() {
    delete this->fields;
    free((void*)this->table);
}

// ------------------------------------------ DropTableNode ------------------------------------------

DropTableNode::DropTableNode(const char* table) {
    this->table = table;
    this->nodeType = DROP_TABLE_NODE;
}

void DropTableNode::print(int depth) {
    printKeyVal("node_type", getStringNodeType(getNodeType()), depth);
    printKeyVal("table", this->table, depth);
}

DropTableNode::~DropTableNode() {
    free((void*)this->table);
}