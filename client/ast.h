#ifndef AST_H
#define AST_H

#include <iostream>
#include <list>

enum NodeType { FOR_NODE, ACTION_NODE, FILTER_NODE, RETURN_NODE, UPDATE_NODE, REMOVE_NODE, INSERT_NODE,
                MAP_NODE, MAP_ENTRY_NODE, CONDITION_NODE, CONDITION_UNION_NODE, CONSTANT_NODE,
                CREATE_TABLE_NODE, DROP_TABLE_NODE };

enum QueryType { CREATE_QUERY, UPDATE_QUERY, SELECT_QUERY, DELETE_QUERY, INSERT_QUERY, DROP_QUERY };

struct Node {
public:
     NodeType nodeType;

    virtual void print(int depth) = 0;
    virtual ~Node() {};
    NodeType getNodeType() {
        return this->nodeType;
    }
};

const char* getStringQueryType(QueryType type);

struct NodeWrapper {
    Node* node;
    QueryType queryType;
};

void printKeyVal(const char* key, const char* val, int depth);

struct ForNode : public Node {
    const char* variable;
    const char* tableName;
    Node* action;

    ForNode(const char* variable, const char* tableName, Node* action);
    void print(int depth) override;
    ~ForNode();
};

struct ActionNode : public Node {
    std::list<Node*> actions;

    ActionNode() { this->nodeType = ACTION_NODE; }
    void addAction(Node* action);
    void print(int depth) override;
    ~ActionNode();
};

enum DataType { INT, FLOAT, STRING, BOOL, REF };

struct Constant : public Node {
    DataType type;

    Constant(DataType type) {
        this->type = type;
        this->nodeType = CONSTANT_NODE;
    }
    virtual std::string getStrVal() { return ""; };
    std::string getStrType();
    void print(int depth) override ;
};

struct FloatConstant : public Constant {
    float value;
    
    FloatConstant(float value): Constant(FLOAT) {
        this->value = value;
    }
    std::string getStrVal() override {
        return std::to_string(this->value);
    }
};

struct IntConstant : public Constant {
    int value;

    IntConstant(int value): Constant(INT) {
        this->value = value;
    }
    std::string getStrVal() override {
        return std::to_string(this->value);
    }
};

struct BoolConstant : public Constant {
    bool value;

    BoolConstant(bool value): Constant(BOOL) {
        this->value = value;
    }
    std::string getStrVal() override {
        return this->value ? "true" : "false";
    }
};

struct StringConstant : public Constant {
    const char* value;

    StringConstant(const char* value, bool isRef = false): Constant(isRef ? REF : STRING) {
        this->value = value;
    }
    std::string getStrVal() override {
        return this->value;
    }
    ~StringConstant() {
        free((void*)value);
    }
};


enum LogicalOp { AND, OR };

struct Predicate : public Node {
    virtual ~Predicate() {}
};

enum ConstantOperation { EQ, NEQ, GT, LT, GTE, LTE, LIKE };

struct Condition : public Predicate {
    Constant* lval;
    Constant* rval;
    ConstantOperation op;

    const char* operation_str[7] = { "==", "!=", ">", "<", ">=", "<=", "like" };
    Condition(Constant* lval, Constant* rval, ConstantOperation op);
    void print(int depth) override;
    ~Condition();
};

struct ConditionUnion : public Predicate {
    LogicalOp op;
    Predicate* lval;
    Predicate* rval;

    const char* getStrOperator();

    ConditionUnion(LogicalOp op, Predicate* lval, Predicate* rval);
    void print(int depth) override;
    ~ConditionUnion();
};

struct FilterNode : public Node {
    Predicate* predicate;

    FilterNode(Predicate* predicate);
    void print(int depth) override;
    ~FilterNode();
};

struct TerminalAction : public Node {

};

struct ReturnAction : public TerminalAction {
        Node* retVal;

        ReturnAction(Node* retVal);
        void print(int depth) override;
        ~ReturnAction();
};

struct MapEntry : public Node {
        const char* key;
        Constant* value;

        MapEntry(const char* key, Constant* value);
        void print(int depth) override;
        ~MapEntry();
};

struct MapNode : public Node {
        std::list<MapEntry*> entries;
        
        MapNode() { this->nodeType = MAP_NODE; }
        void addEntry(MapEntry* entry);
        void print(int depth) override;
        ~MapNode();
};

struct UpdateAction : public TerminalAction {
        const char* variable;
        MapNode* value;
        const char* table;

        UpdateAction(const char* variable, MapNode* value, const char* table);
        void print(int depth) override;
        ~UpdateAction();
};

struct RemoveAction : public TerminalAction {
        const char* variable;
        const char* table;

        RemoveAction(const char* variable, const char* table);
        void print(int depth) override;
        ~RemoveAction();
};

struct InsertNode : public Node {
        MapNode* map;
        const char* table;

        InsertNode(MapNode* map, const char* table);
        void print(int depth) override;
        ~InsertNode();
};

struct CreateTableNode : public Node {
        const char* table;
        MapNode* fields;

        CreateTableNode(const char* table, MapNode* fields);
        void print(int depth) override;
        ~CreateTableNode();
};

struct DropTableNode : public Node {
        const char* table;

        DropTableNode(const char* table);
        void print(int depth) override;
        ~DropTableNode();
};

#endif