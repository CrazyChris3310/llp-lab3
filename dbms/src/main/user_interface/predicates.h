#ifndef PREDICATES_H
#define PREDICATES_H

#include <inttypes.h>
#include <stdbool.h>
#include "../middleware/data_type.h"
#include "scanners/scanners.h"
#include "../util/comparators.h"
#include "../util/linked_list.h"

struct Constant {
    enum DataType type;
    union {
        int64_t intVal;
        float floatVal;
        bool boolVal;
        const char* stringVal;
    } value;
};

struct Condition {
    const char* fieldName;
    struct Constant constant;
    enum CompareOperator oper;
};

struct Predicate {
    struct LinkedList* conditions;
};

struct Predicate* createPredicate();
void destroyPredicate(struct Predicate* predicate);

struct Condition* createCondition(const char* fieldName, struct Constant constant, enum CompareOperator oper);
void addCondition(struct Predicate* predicate, char* fieldName, struct Constant constant, enum CompareOperator oper);
void addConditionDirectly(struct Predicate* predicate, struct Condition* condition);

struct Constant intConstant(int64_t value);
struct Constant floatConstant(float value);
struct Constant boolConstant(bool value);
struct Constant stringConstant(const char* value);
struct Constant refConstant(const char* column);

#endif