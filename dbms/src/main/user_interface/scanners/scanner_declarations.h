#ifndef SCANNERS_DECLARATIONS_H
#define SCANNERS_DECLARATIONS_H

#include <inttypes.h>
#include "../predicates.h"
#include "../write_scan.h"
#include "../../util/linked_list.h"

struct ScanInterface {
    void(*reset)(void* ptr);

    bool(*goToNextRecord)(void* ptr);
    void(*insertNextRecord)(void* ptr);
    void(*deleteRecord)(void* ptr);

    bool(*hasField)(void* ptr, const char* field);
    int64_t(*getInt)(void* ptr, const char* field);
    struct String(*getString)(void* ptr, const char* filed);
    float(*getFloat)(void* ptr, const char* field);
    bool(*getBool)(void* ptr, const char* field);
    struct Constant(*getField)(void* ptr, const char* field);
    struct Constant(*getFieldById)(void* ptr, size_t id);

    void(*setInt)(void* ptr, const char* field, int64_t value);
    void(*setString)(void* ptr, const char* field, struct String value);
    void(*setBool)(void* ptr, const char* field, bool value);
    void(*setFloat)(void* ptr, const char* field, float value);
    void(*setVarchar)(void* ptr, const char* field, const char* value);

    size_t(*getColumnsCount)(void* ptr);
    const char*(*getColumnNameById)(void* ptr, size_t id);
    struct Field*(*getColumnInfoById)(void* ptr, size_t id);
    int(*getFieldId)(void* ptr, const char* name);

    void(*destroy)(void* ptr);
};

struct TableScanner {
    struct ScanInterface scanInterface;

    struct Schema* schema;
    struct CacheManager* cacheManager;
    struct PageRecord* pageRecord;
    size_t blockId;
    size_t startBlock;
};

struct SelectScanner {
    struct ScanInterface scanInterface;

    struct ScanInterface* tableScanner;
    struct Predicate predicate;
};

struct JoinScanner {
    struct ScanInterface scanInterface;

    struct ScanInterface* leftScanner;
    struct ScanInterface* rightScanner;
};

struct ProjectScanner {
    struct ScanInterface scanInterface;

    struct ScanInterface* innerScanner;
    struct LinkedList* columns;
};

#endif 