#include "query.h"
#include <stdlib.h>

void emptyFreeFunction(void* ptr){}

struct SelectQuery* createSelectQuery(const char* from, struct Predicate* predicate) {
    struct SelectQuery* query = malloc(sizeof(struct SelectQuery));
    query->joins = createClearableLinkedList(emptyFreeFunction);
    query->from = from;
    query->predicate = predicate;
    return query;
}

void joinTable(struct SelectQuery* query, const char* table) {
    addNode(query->joins, (char*)table);
}

void destroySelectQuery(struct SelectQuery* query) {
    if (query == NULL) {
        return;
    }
    destroyPredicate(query->predicate);
    freeLinkedList(query->joins);
    free(query);
}

/*
    * Creates an insert query with the given table name and values.
    * 
    * @param into The name of the table to insert into.
    * @param values List of Condition structs containing pair of column name and value.
    * @return The insert query.
*/
struct InsertQuery* createInsertQuery(const char* into) {
    struct InsertQuery* query = malloc(sizeof(struct InsertQuery));
    query->into = into;
    query->values = createLinkedList();
    return query;
}

void clearInsertQuery(struct InsertQuery* query) {
    if (query != NULL) {
        clearList(query->values);
    }
}

void destroyInsertQuery(struct InsertQuery* query) {
    freeLinkedList(query->values);
    free(query);
}

void addInsertionField(struct InsertQuery* query, const char* field, struct Constant value) {
    struct Condition* condition = createCondition(field, value, EQUAL);
    addNode(query->values, condition);
}

struct DeleteQuery* createDeleteQuery(const char* from, struct Predicate* predicate) {
    struct DeleteQuery* query = malloc(sizeof(struct DeleteQuery));
    query->from = from;
    query->predicate = predicate;
    return query;
}

void destroyDeleteQuery(struct DeleteQuery* query) {
    destroyPredicate(query->predicate);
    free(query);
}

struct UpdateQuery* createUpdateQuery(const char* table, struct Predicate* predicate) {
    struct UpdateQuery* query = malloc(sizeof(struct UpdateQuery));
    query->new_values = createLinkedList();
    query->table = table;
    query->predicate = predicate;
    return query;
}

void addUpdateField(struct UpdateQuery* query, const char* field, struct Constant value) {
    struct Condition* condition = createCondition(field, value, EQUAL);
    addNode(query->new_values, condition);
}

void destroyUpdateQuery(struct UpdateQuery* query) {
    destroyPredicate(query->predicate);
    freeLinkedList(query->new_values);
    free(query);
}