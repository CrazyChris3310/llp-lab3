#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../predicates.h"
#include "../read_scan.h"
#include "../write_scan.h"
#include "../../util/comparators.h"
#include "scanner_declarations.h"
#include "../../util/linked_list.h"

bool __joinScannerHasField(void* ptr, const char* field);
int64_t __getIntegerFromJoinScanner(void* ptr, const char* field);
float __getFloatFromJoinScanner(void* ptr, const char* field);
bool __getBoolFromJoinScanner(void* ptr, const char* field);
struct String __getStringFromJoinScanner(void* ptr, const char* field);
struct Constant __getFieldFromJoinScanner(void* ptr, const char* field);
struct Constant __getFieldFromJoinScannerById(void* ptr, size_t id);

void __setIntegerToJoinScanner(void* ptr, const char* field, int64_t value);
void __setFloatToJoinScanner(void* ptr, const char* field, float value);
void __setBoolToJoinScanner(void* ptr, const char* field, bool value);
void __setStringToJoinScanner(void* ptr, const char* field, struct String value);
void __setVarcharToJoinScanner(void* ptr, const char* field, const char* value);

static bool __joinScanNextFunction(void* ptr);

void __joinInsertion(void* ptr);
void __destroyJoinScanner(void* ptr);
static void __resetJoinScanner(void* ptr);

size_t __getColumnsCountFromJoinScanner(void* ptr);
const char* __getColumnNameByIdFormJoinScanner(void* ptr, size_t id);
struct Field* __getColumnInfoByIdFromJoinScanner(void* ptr, size_t id);\
int __getFieldIdFromJoincanner(void* ptr, const char* name);

struct JoinScanner* createJoinScanner(struct ScanInterface* left, struct ScanInterface* right) {
    struct JoinScanner* scanner = malloc(sizeof(struct JoinScanner));
    scanner->leftScanner = left;
    scanner->rightScanner = right;
    next(left);

    scanner->scanInterface.goToNextRecord = __joinScanNextFunction;
    scanner->scanInterface.insertNextRecord = __joinInsertion;

    scanner->scanInterface.hasField = __joinScannerHasField;
    scanner->scanInterface.getBool = __getBoolFromJoinScanner;
    scanner->scanInterface.getFloat = __getFloatFromJoinScanner;
    scanner->scanInterface.getInt = __getIntegerFromJoinScanner;
    scanner->scanInterface.getString = __getStringFromJoinScanner;
    scanner->scanInterface.getField = __getFieldFromJoinScanner;
    scanner->scanInterface.getFieldById = __getFieldFromJoinScannerById;

    scanner->scanInterface.setBool = __setBoolToJoinScanner;
    scanner->scanInterface.setInt = __setIntegerToJoinScanner;
    scanner->scanInterface.setFloat = __setFloatToJoinScanner;
    scanner->scanInterface.setString = __setStringToJoinScanner;
    scanner->scanInterface.setVarchar = __setVarcharToJoinScanner;

    scanner->scanInterface.destroy = __destroyJoinScanner;
    scanner->scanInterface.reset = __resetJoinScanner;
    scanner->scanInterface.deleteRecord = __joinInsertion;

    scanner->scanInterface.getColumnsCount = __getColumnsCountFromJoinScanner;
    scanner->scanInterface.getColumnNameById = __getColumnNameByIdFormJoinScanner;
    scanner->scanInterface.getColumnInfoById = __getColumnInfoByIdFromJoinScanner;
    scanner->scanInterface.getFieldId = __getFieldIdFromJoincanner;


    return scanner;
}

void __destroyJoinScanner(void* ptr) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    scanner->leftScanner->destroy(scanner->leftScanner);
    scanner->rightScanner->destroy(scanner->rightScanner);
    free(scanner);
}

void __resetJoinScanner(void* ptr) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    scanner->leftScanner->reset(scanner->leftScanner);
    next(scanner->leftScanner);
    scanner->rightScanner->reset(scanner->rightScanner);
}

bool __joinScannerHasField(void* ptr, const char* field) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    return scanner->leftScanner->hasField(scanner->leftScanner, field) || scanner->rightScanner->hasField(scanner->rightScanner, field);
}

int64_t __getIntegerFromJoinScanner(void* ptr, const char* field) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (scanner->leftScanner->hasField(scanner->leftScanner, field)) {
        return getInt(scanner->leftScanner, field);
    } else {
        return getInt(scanner->rightScanner, field);
    }
}

float __getFloatFromJoinScanner(void* ptr, const char* field) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (scanner->leftScanner->hasField(scanner->leftScanner, field)) {
        return getFloat(scanner->leftScanner, field);
    } else {
        return getFloat(scanner->rightScanner, field);
    }
}

bool __getBoolFromJoinScanner(void* ptr, const char* field) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (scanner->leftScanner->hasField(scanner->leftScanner, field)) {
        return getBool(scanner->leftScanner, field);
    } else {
        return getBool(scanner->rightScanner, field);
    }
}

struct String __getStringFromJoinScanner(void* ptr, const char* field) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (scanner->leftScanner->hasField(scanner->leftScanner, field)) {
        return getString(scanner->leftScanner, field);
    } else {
        return getString(scanner->rightScanner, field);
    }
}

struct Constant __getFieldFromJoinScanner(void* ptr, const char* field) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (scanner->leftScanner->hasField(scanner->leftScanner, field)) {
        return getField(scanner->leftScanner, field);
    } else {
        return getField(scanner->rightScanner, field);
    }
}

struct Constant __getFieldFromJoinScannerById(void* ptr, size_t id) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    size_t leftSize = getColumnsCount(scanner->leftScanner);
    if (id < leftSize) {
        return getFieldById(scanner->leftScanner, id);
    } else {
        return getFieldById(scanner->rightScanner, id - leftSize);
    }
}

void __setIntegerToJoinScanner(void* ptr, const char* field, int64_t value) {}

void __setFloatToJoinScanner(void* ptr, const char* field, float value) {}

void __setBoolToJoinScanner(void* ptr, const char* field, bool value) {}

void __setStringToJoinScanner(void* ptr, const char* field, struct String value) {}

void __setVarcharToJoinScanner(void* ptr, const char* field, const char* value) {}

static bool __joinScanNextFunction(void* ptr) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;

    if (next(scanner->rightScanner)) {
        return true;
    } else {
        reset(scanner->rightScanner);
        return next(scanner->rightScanner) && next(scanner->leftScanner);
    }
}

void __joinInsertion(void* ptr) {}

size_t __getColumnsCountFromJoinScanner(void* ptr) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    return getColumnsCount(scanner->leftScanner) + getColumnsCount(scanner->rightScanner);
}

const char* __getColumnNameByIdFormJoinScanner(void* ptr, size_t id) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (id >= __getColumnsCountFromJoinScanner(scanner)) {
        return NULL;
    }
    size_t leftSize = getColumnsCount(scanner->leftScanner);
    if (id < leftSize) {
        return getColumnNameById(scanner->leftScanner, id);
    } else {
        return getColumnNameById(scanner->rightScanner, id - leftSize);
    }
}

struct Field* __getColumnInfoByIdFromJoinScanner(void* ptr, size_t id) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    if (id >= __getColumnsCountFromJoinScanner(scanner)) {
        return NULL;
    }
    size_t leftSize = getColumnsCount(scanner->leftScanner);
    if (id < leftSize) {
        return getColumnInfoById(scanner->leftScanner, id);
    } else {
        return getColumnInfoById(scanner->rightScanner, id - leftSize);
    }
}

int __getFieldIdFromJoincanner(void* ptr, const char* name) {
    struct JoinScanner* scanner = (struct JoinScanner*)ptr;
    int id = getColumnId(scanner->leftScanner, name);
    if (id >= 0) {
        return id;
    }
    id = getColumnId(scanner->rightScanner, name);
    if (id >= 0) {
        return getColumnsCount(scanner->leftScanner) + id;
    }
    return -1;
    
}