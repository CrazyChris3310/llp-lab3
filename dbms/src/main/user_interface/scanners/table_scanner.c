#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "../../util/my_string.h"
#include "../../file_io/file_manager.h"
#include "../../page_cacheing/cache_manager.h"
#include "../../middleware/page_record.h"
#include "scanner_declarations.h"
#include "../../user_interface/predicates.h"
#include "../../middleware/schema.h"

static bool __tableScanNextFunction(void* ptr);
static void __insertRecordIntoTableScanner(void* ptr);
static void __deleteRecordFromTableScanner(void* ptr);

void __setIntegerToTableScanner(void* ptr, const char* field, int64_t value);
void __setFloatToTableScanner(void* ptr, const char* field, float value);
void __setBoolToTableScanner(void* ptr, const char* field, bool value);
void __setStringToTableScanner(void* ptr, const char* field, struct String value);
void __setVarcharToTableScanner(void* ptr, const char* field, const char* value);

static bool __tableScannerHasField(void* ptr, const char* field);
int64_t __getIntegerFromTableScanner(void* ptr, const char* field);
float __getFloatFromTableScanner(void* ptr, const char* field);
bool __getBoolFromTableScanner(void* ptr, const char* field);
struct String __getStringFromTableScanner(void* ptr, const char* field);
struct Constant __getFieldFromTableScanner(void* ptr, const char* field);
struct Constant __getFieldFromTableScannerById(void* ptr, size_t id);

static void moveScannerToBlock(struct TableScanner* scanner, size_t blockId);
static size_t moveScannerToNewBlock(struct TableScanner* scanner, bool hasCurrent);

void __destroyTableScanner(void* ptr);
static void __resetTableScanner(void* ptr);

size_t __getColumnsCountFromTableScanner(void* ptr);
const char* __getColumnNameByIdFromTableScanner(void* ptr, size_t id);
struct Field* __getColumnInfoByIdFromTableScanner(void* ptr, size_t id);
int __getFieldIdFromTableScanner(void* ptr, const char* name);

/*
    search for the first page of given table in file and set current scanner before first record of this table
*/
struct TableScanner* createTableScanner(struct CacheManager* cm, struct Schema* schema, bool isNew, size_t blockStart) {
    struct TableScanner* scanner = malloc(sizeof(struct TableScanner));
    scanner->cacheManager = cm;
    scanner->schema = schema;
    scanner->blockId = blockStart;
    scanner->startBlock = blockStart;
    scanner->pageRecord = NULL;

    if (isNew) {
        size_t newBlockId = moveScannerToNewBlock(scanner, false);
        schema->startBlock = newBlockId;
        scanner->startBlock = newBlockId;
    } else {
        moveScannerToBlock(scanner, blockStart);
    }

    scanner->scanInterface.goToNextRecord = __tableScanNextFunction;
    scanner->scanInterface.insertNextRecord = __insertRecordIntoTableScanner;
    scanner->scanInterface.deleteRecord = __deleteRecordFromTableScanner;

    scanner->scanInterface.hasField = __tableScannerHasField;
    scanner->scanInterface.getBool = __getBoolFromTableScanner;
    scanner->scanInterface.getFloat = __getFloatFromTableScanner;
    scanner->scanInterface.getInt = __getIntegerFromTableScanner;
    scanner->scanInterface.getString = __getStringFromTableScanner;
    scanner->scanInterface.getField = __getFieldFromTableScanner;
    scanner->scanInterface.getFieldById = __getFieldFromTableScannerById;

    scanner->scanInterface.setBool = __setBoolToTableScanner;
    scanner->scanInterface.setInt = __setIntegerToTableScanner;
    scanner->scanInterface.setFloat = __setFloatToTableScanner;
    scanner->scanInterface.setString = __setStringToTableScanner;
    scanner->scanInterface.setVarchar = __setVarcharToTableScanner;

    scanner->scanInterface.destroy = __destroyTableScanner;
    scanner->scanInterface.reset = __resetTableScanner;

    scanner->scanInterface.getColumnsCount = __getColumnsCountFromTableScanner;
    scanner->scanInterface.getColumnNameById = __getColumnNameByIdFromTableScanner;
    scanner->scanInterface.getColumnInfoById = __getColumnInfoByIdFromTableScanner;
    scanner->scanInterface.getFieldId = __getFieldIdFromTableScanner;

    return scanner;
}

void __destroyTableScanner(void* ptr) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    if (scanner->schema->shouldClear) {
        destroySchema(scanner->schema);
    }
    destroyPageRecord(scanner->pageRecord);
    free(scanner);
}

size_t getCurrentBlock(struct TableScanner* scanner) {
    return scanner->blockId;
}

static bool __tableScannerHasField(void* ptr, const char* field) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return schemaGetField(scanner->schema, field) != NULL;
}

int64_t __getIntegerFromTableScanner(void* ptr, const char* field) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return getIntFromRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field});
}

float __getFloatFromTableScanner(void* ptr, const char* field) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return getFloatFromRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field});
}

bool __getBoolFromTableScanner(void* ptr, const char* field) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return getBoolFromRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field});
}

struct String __getStringFromTableScanner(void* ptr, const char* field) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return getStringFromRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field});
}

struct Constant __getFieldFromTableScanner(void* ptr, const char* field) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    void* data = getFieldFromRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field});
    struct Constant constant;
    enum DataType type = getFieldType(scanner->schema, (struct String){.lenght = strlen(field), .value = field});
    constant.type = type;
    switch (type) {
        case INT:
            memcpy(&constant.value.intVal, data, sizeof(int64_t));
            break;
        case FLOAT:
            memcpy(&constant.value.floatVal, data, sizeof(float));
            break;
        case BOOL:
            constant.value.boolVal = *(bool*)data;
            break;
        case STRING:
            constant.value.stringVal = (char*)(data) + sizeof(size_t);
            break;
        default:
            break;
    }
    return constant;
}

struct Constant __getFieldFromTableScannerById(void* ptr, size_t id) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    struct Field* field = schemaGetFieldById(scanner->schema, id);
    return __getFieldFromTableScanner(ptr, field->name.value);
}

void __setIntegerToTableScanner(void* ptr, const char* field, int64_t value) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    setIntToRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field}, value);
}

void __setFloatToTableScanner(void* ptr, const char* field, float value) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    setFloatToRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field}, value);
}

void __setBoolToTableScanner(void* ptr, const char* field, bool value) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    setBoolToRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field}, value);
}

void __setStringToTableScanner(void* ptr, const char* field, struct String value) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    setStringToRecord(scanner->pageRecord, (struct String){.lenght = strlen(field), .value = field}, value);
}

void __setVarcharToTableScanner(void* ptr, const char* field, const char* value) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    setStringToRecord(scanner->pageRecord, 
                    (struct String){.lenght = strlen(field), .value = field},
                    (struct String){.lenght = strlen(value), .value = value});
}

static void moveScannerToBlock(struct TableScanner* scanner, size_t blockId) {
    if (scanner->pageRecord != NULL) {
        destroyPageRecord(scanner->pageRecord);
    }
    scanner->pageRecord = createPageRecord(scanner->cacheManager, scanner->schema, blockId);
    scanner->blockId = blockId;
}

static void moveScannerToNextBlock(struct TableScanner* scanner) {
    if (scanner->pageRecord != NULL) {
        destroyPageRecord(scanner->pageRecord);
    }
    struct CachedPage* page = requestCachedPage(scanner->cacheManager, scanner->blockId);

    if (page->page->header->nextPage.exists) {
        scanner->pageRecord = createPageRecord(scanner->cacheManager, scanner->schema, page->page->header->nextPage.value);
        scanner->blockId = page->page->header->nextPage.value;
    } else {
        scanner->pageRecord = NULL;
        scanner->blockId = 0;
    }

    releaseCachedPage(scanner->cacheManager, page);
}

static size_t moveScannerToNewBlock(struct TableScanner* scanner, bool hasCurrent) {
    struct PageHeader header;
    header.nextPage = (struct PossibleValue){ .exists = false };

    size_t blockId;
    struct FileHeader* fileHeader = &scanner->cacheManager->fileManager->header;
    if (!fileHeader->freePages.exists) {
        blockId = addNewBlock(scanner->cacheManager->fileManager, &header);
    } else {
        blockId = fileHeader->freePages.value;
        struct CachedPage* freePage = requestCachedPage(scanner->cacheManager, blockId);
        fileHeader->freePages = freePage->page->header->nextPage;
        freePage->page->header->nextPage = (struct PossibleValue){ .exists = false };
        releaseCachedPage(scanner->cacheManager, freePage);
    }

    if (scanner->pageRecord != NULL) {
        destroyPageRecord(scanner->pageRecord);
    }

    if (hasCurrent) {
        struct CachedPage* cachedPage = requestCachedPage(scanner->cacheManager, scanner->blockId);
        cachedPage->page->header->nextPage = (struct PossibleValue){ .exists = true, .value = blockId };
        cachedPage->page->isDirty = true;
        releaseCachedPage(scanner->cacheManager, cachedPage);
    }

    scanner->pageRecord = createPageRecord(scanner->cacheManager, scanner->schema, blockId);
    clearAllRecords(scanner->pageRecord);
    scanner->blockId = blockId;
    return blockId;
}

static bool isLastBlock(struct TableScanner* scanner) {
    struct CachedPage* page = requestCachedPage(scanner->cacheManager, scanner->blockId);
    bool isLast = !page->page->header->nextPage.exists;
    releaseCachedPage(scanner->cacheManager, page);
    return isLast;
}

static bool __tableScanNextFunction(void* ptr) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    bool found = goToNextRecord(scanner->pageRecord);
    while (!found) {
        if (isLastBlock(scanner)) {
            return false;
        }
        moveScannerToNextBlock(scanner);
        found = goToNextRecord(scanner->pageRecord);
    }
    return true;
}

static void __insertRecordIntoTableScanner(void* ptr) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    bool found = insertNextRecord(scanner->pageRecord);
    while (!found) {
        if (isLastBlock(scanner)) {
            moveScannerToNewBlock(scanner, true);
        } else {
            moveScannerToNextBlock(scanner);
        }
        found = insertNextRecord(scanner->pageRecord);
    }
}

static void __resetTableScanner(void* ptr) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    moveScannerToBlock(scanner, scanner->startBlock);
}

static void __deleteRecordFromTableScanner(void* ptr) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    deletePageRecord(scanner->pageRecord);
}

size_t __getColumnsCountFromTableScanner(void* ptr) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return getListSize(scanner->schema->fields);
}

const char* __getColumnNameByIdFromTableScanner(void* ptr, size_t id) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    if (id >= __getColumnsCountFromTableScanner(scanner)) {
        return NULL;
    }
    return schemaGetFieldById(scanner->schema, id)->name.value;
}

struct Field* __getColumnInfoByIdFromTableScanner(void* ptr, size_t id) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    if (id >= __getColumnsCountFromTableScanner(scanner)) {
        return NULL;
    }
    return schemaGetFieldById(scanner->schema, id);
}

int __getFieldIdFromTableScanner(void* ptr, const char* name) {
    struct TableScanner* scanner = (struct TableScanner*)ptr;
    return schemaGetFieldId(scanner->schema, name);
}