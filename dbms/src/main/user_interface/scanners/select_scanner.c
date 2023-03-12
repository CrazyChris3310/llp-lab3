#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface/predicates.h"
#include "user_interface/read_scan.h"
#include "user_interface/write_scan.h"
#include "util/comparators.h"
#include "user_interface/scanners/scanner_declarations.h"
#include "util/linked_list.h"

int64_t __getIntegerFromSelectScanner(void* ptr, const char* field);
float __getFloatFromSelectScanner(void* ptr, const char* field);
bool __getBoolFromSelectScanner(void* ptr, const char* field);
struct String __getStringFromSelectScanner(void* ptr, const char* field);
struct Constant __getFieldFromSelectScanner(void* ptr, const char* field);
struct Constant __getFieldFromSelectScannerById(void* ptr, size_t id);

void __setIntegerToSelectScanner(void* ptr, const char* field, int64_t value);
void __setFloatToSelectScanner(void* ptr, const char* field, float value);
void __setBoolToSelectScanner(void* ptr, const char* field, bool value);
void __setStringToSelectScanner(void* ptr, const char* field, struct String value);
void __setVarcharToSelectScanner(void* ptr, const char* field, const char* value);

static bool __selectScanNextFunction(void* ptr);
static void __insertRecordIntoSelectScanner(void* ptr);
static void __deleteRecordFromSelectScanner(void* ptr);

void __destroySelectScanner(void* ptr);
static void __resetSelectScanner(void* ptr);

size_t __getColumnsCountFromSelectScanner(void* ptr);
const char* __getColumnNameByIdFromSelectScanner(void* ptr, size_t id);
struct Field* __getColumnInfoByIdFromSelectScanner(void* ptr, size_t id);

struct SelectScanner* createSelectScanner(struct ScanInterface* scan, struct Predicate predicate) {
    struct SelectScanner* scanner = malloc(sizeof(struct SelectScanner));
    scanner->predicate = predicate;
    scanner->tableScanner = scan;

    scanner->scanInterface.goToNextRecord = __selectScanNextFunction;
    scanner->scanInterface.insertNextRecord = __insertRecordIntoSelectScanner;

    scanner->scanInterface.getBool = __getBoolFromSelectScanner;
    scanner->scanInterface.getFloat = __getFloatFromSelectScanner;
    scanner->scanInterface.getInt = __getIntegerFromSelectScanner;
    scanner->scanInterface.getString = __getStringFromSelectScanner;
    scanner->scanInterface.getField = __getFieldFromSelectScanner;
    scanner->scanInterface.getFieldById = __getFieldFromSelectScannerById;

    scanner->scanInterface.setBool = __setBoolToSelectScanner;
    scanner->scanInterface.setInt = __setIntegerToSelectScanner;
    scanner->scanInterface.setFloat = __setFloatToSelectScanner;
    scanner->scanInterface.setString = __setStringToSelectScanner;
    scanner->scanInterface.setVarchar = __setVarcharToSelectScanner;

    scanner->scanInterface.destroy = __destroySelectScanner;
    scanner->scanInterface.reset = __resetSelectScanner;
    scanner->scanInterface.deleteRecord = __deleteRecordFromSelectScanner;

    scanner->scanInterface.getColumnsCount = __getColumnsCountFromSelectScanner;
    scanner->scanInterface.getColumnNameById = __getColumnNameByIdFromSelectScanner;
    scanner->scanInterface.getColumnInfoById = __getColumnInfoByIdFromSelectScanner;

    return scanner;
}

void __destroySelectScanner(void* ptr) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    scanner->tableScanner->destroy(scanner->tableScanner);
    free(scanner);
}

static bool conditionIsSatisfied(struct SelectScanner* scanner, struct Condition condition) {
    struct Constant constant = getField((struct ScanInterface*)scanner, condition.fieldName);
    switch (constant.type) {
        case INT:
            return compareInts(constant.value.intVal, condition.constant.value.intVal, condition.oper);
        case FLOAT:
            return compareFloats(constant.value.floatVal, condition.constant.value.floatVal, condition.oper);
        case BOOL:
            return compareBools(constant.value.boolVal, condition.constant.value.boolVal, condition.oper);
        case STRING:
            return compareVarchars(constant.value.stringVal, condition.constant.value.stringVal, condition.oper);
    }
    return true;
}

static bool predicateIsSatisfied(struct SelectScanner* scanner, struct Predicate predicate) {
    struct ListIterator* iterator = createListIterator(predicate.conditions);
    bool result = true;
    while (iteratorHasNext(iterator)) {
        struct Condition* condition = iteratorNext(iterator);
        if (!conditionIsSatisfied(scanner, *condition)) {
            result = false;
            break;
        }
    }
    freeListIterator(iterator);
    return result;
}

int64_t __getIntegerFromSelectScanner(void* ptr, const char* field) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getInt(scanner->tableScanner, field);
}

float __getFloatFromSelectScanner(void* ptr, const char* field) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getFloat(scanner->tableScanner, field);
}

bool __getBoolFromSelectScanner(void* ptr, const char* field) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getBool(scanner->tableScanner, field);
}

struct String __getStringFromSelectScanner(void* ptr, const char* field) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getString(scanner->tableScanner, field);
}

struct Constant __getFieldFromSelectScanner(void* ptr, const char* field) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getField(scanner->tableScanner, field);
}

struct Constant __getFieldFromSelectScannerById(void* ptr, size_t id) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getFieldById(scanner->tableScanner, id);
}

void __setIntegerToSelectScanner(void* ptr, const char* field, int64_t value) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    setInt(scanner->tableScanner, field, value);
}

void __setFloatToSelectScanner(void* ptr, const char* field, float value) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    setFloat(scanner->tableScanner, field, value);
}

void __setBoolToSelectScanner(void* ptr, const char* field, bool value) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    setBool(scanner->tableScanner, field, value);
}

void __setStringToSelectScanner(void* ptr, const char* field, struct String value) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    setString(scanner->tableScanner, field, value);
}

void __setVarcharToSelectScanner(void* ptr, const char* field, const char* value) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    setVarchar(scanner->tableScanner, field, value);
}

static bool __selectScanNextFunction(void* ptr) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;

    while (scanner->tableScanner->goToNextRecord(scanner->tableScanner)) {
        if (predicateIsSatisfied(scanner, scanner->predicate)) {
            return true;
        }
    }
    return false;
}

static void __insertRecordIntoSelectScanner(void* ptr) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    insert(scanner->tableScanner);
}

static void __resetSelectScanner(void* ptr) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    scanner->tableScanner->reset(scanner->tableScanner);
}

static void __deleteRecordFromSelectScanner(void* ptr) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    scanner->tableScanner->deleteRecord(scanner->tableScanner);
}

size_t __getColumnsCountFromSelectScanner(void* ptr) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    return getColumnsCount(scanner->tableScanner);
}

const char* __getColumnNameByIdFromSelectScanner(void* ptr, size_t id) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    if (id >= __getColumnsCountFromSelectScanner(scanner)) {
        return NULL;
    }
    return getColumnNameById(scanner->tableScanner, id);
}

struct Field* __getColumnInfoByIdFromSelectScanner(void* ptr, size_t id) {
    struct SelectScanner* scanner = (struct SelectScanner*)ptr;
    if (id >= __getColumnsCountFromSelectScanner(scanner)) {
        return NULL;
    }
    return getColumnInfoById(scanner->tableScanner, id);
}