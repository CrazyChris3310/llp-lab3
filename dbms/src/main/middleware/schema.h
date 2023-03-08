#ifndef SCHEMA_H
#define SCHEMA_H

#include "../util/my_string.h"
#include "data_type.h"
#include <stdbool.h>
#include "../util/linked_list.h"

struct Field {
    struct String name;
    enum DataType type;
    size_t len;
    size_t offset;
};

struct Schema {
    const char* name;
    struct LinkedList* fields;
    size_t slotSize;
    int64_t startBlock;
    bool shouldClear;
};

struct Schema* createSchema(const char* name);
void destroySchema(struct Schema* schema);
void clearSchema(struct Schema* schema);

void addField(struct Schema* schema, const char* name, enum DataType type, size_t len);
void addIntField(struct Schema* schema, const char* name);
void addFloatField(struct Schema* schema, const char* name);
void addStringField(struct Schema* schema, const char* name, size_t len);
void addBooleanField(struct Schema* schema, const char* name);
struct LinkedList* getFieldList(struct Schema* schema);
struct Field* schemaGetField(struct Schema* schema, const char* field);
struct Field* schemaGetFieldById(struct Schema* schema, size_t id);
struct PossibleValue getFieldOffset(struct Schema* schema, struct String field);
struct PossibleValue getFieldLength(struct Schema* schema, struct String field);
enum DataType getFieldType(struct Schema* schema, struct String field);

#endif