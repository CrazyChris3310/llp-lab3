#ifndef QUERY_H
#define QUERY_H

#include "../util/linked_list.h"
#include "predicates.h"

struct SelectQuery {
    const char* from;
    struct LinkedList* joins;
    struct Predicate* predicate;  
};

struct InsertQuery {
    const char* into;
    struct LinkedList* values;
};

struct DeleteQuery {
    const char* from;
    struct Predicate* predicate;
};

struct UpdateQuery {
    const char* table;
    struct Predicate* predicate;
    struct LinkedList* new_values;
};

enum QueryType { CREATE_QUERY, UPDATE_QUERY, SELECT_QUERY, DELETE_QUERY, INSERT_QUERY, DROP_QUERY };

struct SelectQuery* createSelectQuery(const char* from, struct Predicate* predicate);
struct InsertQuery* createInsertQuery(const char* into);
struct DeleteQuery* createDeleteQuery(const char* from, struct Predicate* predicate);
struct UpdateQuery* createUpdateQuery(const char* table, struct Predicate* predicate);

void destroySelectQuery(struct SelectQuery* query);
void destroyInsertQuery(struct InsertQuery* query);
void destroyDeleteQuery(struct DeleteQuery* query);
void destroyUpdateQuery(struct UpdateQuery* query);

void addInsertionField(struct InsertQuery* query, const char* field, struct Constant value);
void addUpdateField(struct UpdateQuery* query, const char* field, struct Constant value);
void clearInsertQuery(struct InsertQuery* query);
void joinTable(struct SelectQuery* query, const char* table);

#endif