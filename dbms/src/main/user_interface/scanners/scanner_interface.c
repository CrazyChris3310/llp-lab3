#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "util/my_string.h"
#include "file_io/file_manager.h"
#include "page_cacheing/cache_manager.h"
#include "middleware/page_record.h"
#include "scanner_declarations.h"
#include "middleware/data_type.h"

#include "user_interface/read_scan.h"
#include "user_interface/write_scan.h"
#include "user_interface/predicates.h"

int64_t getInt(struct ScanInterface* scanner, const char* field) {
    return scanner->getInt(scanner, field);
}
float getFloat(struct ScanInterface* scanner, const char* field) {
    return scanner->getFloat(scanner, field);
}
bool getBool(struct ScanInterface* scanner, const char* field) {
    return scanner->getBool(scanner, field);
}
struct String getString(struct ScanInterface* scanner, const char* field) {
    return scanner->getString(scanner, field);
}

void setInt(struct ScanInterface* scanner, const char* field, int64_t value) {
    scanner->setInt(scanner, field, value);
}

void setFloat(struct ScanInterface* scanner, const char* field, float value) {
    scanner->setFloat(scanner, field, value);
}

struct Constant getField(struct ScanInterface* scanner, const char* field) {
    return scanner->getField(scanner, field);
}

struct Constant getFieldById(struct ScanInterface* scanner, size_t id) {
    return scanner->getFieldById(scanner, id);
}

void setBool(struct ScanInterface* scanner, const char* field, bool value) {
    scanner->setBool(scanner, field, value);
}

void setString(struct ScanInterface* scanner, const char* field, struct String value) {
    scanner->setString(scanner, field, value);
}

void setVarchar(struct ScanInterface* scanner, const char* field, const char* value) {
    scanner->setVarchar(scanner, field, value);
}

void setField(struct ScanInterface* scanner, const char* field, struct Constant value) {
    if (value.type == STRING) {
        scanner->setVarchar(scanner, field, value.value.stringVal);
    } else if (value.type == INT) {
        scanner->setInt(scanner, field, value.value.intVal);
    } else if (value.type == FLOAT) {
        scanner->setFloat(scanner, field, value.value.floatVal);
    } else if (value.type == BOOL) {
        scanner->setBool(scanner, field, value.value.boolVal);
    }
}

bool next(struct ScanInterface* scanner) {
    return scanner->goToNextRecord(scanner);
}

void insert(struct ScanInterface* scanner) {
    scanner->insertNextRecord(scanner);
}

void destroyScanner(struct ScanInterface* scanner) {
    scanner->destroy(scanner);
}

void deleteRecord(struct ScanInterface* scanner) {
    scanner->deleteRecord(scanner);
}

void reset(struct ScanInterface* scanner) {
    scanner->reset(scanner);
}

bool hasField(struct ScanInterface* scanner, const char* field) {
    return scanner->hasField(scanner, field);
}

size_t getColumnsCount(struct ScanInterface* scanner) {
    return scanner->getColumnsCount(scanner);
}

const char* getColumnNameById(struct ScanInterface* scanner, size_t id) {
    return scanner->getColumnNameById(scanner, id);
}

struct Field* getColumnInfoById(struct ScanInterface* scanner, size_t id) {
    return scanner->getColumnInfoById(scanner, id);
}