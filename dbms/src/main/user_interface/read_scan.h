#ifndef READ_SCAN_H
#define READ_SCAN_H

#include <inttypes.h>
#include <stdbool.h>
#include "../util/my_string.h"
#include "predicates.h"

struct ScanInterface;

bool next(struct ScanInterface* scanner);

int64_t getInt(struct ScanInterface* scanner, const char* field);
float getFloat(struct ScanInterface* scanner, const char* field);
bool getBool(struct ScanInterface* scanner, const char* field);
struct String getString(struct ScanInterface* scanner, const char* field);
struct Constant getField(struct ScanInterface* scanner, const char* field);
struct Constant getFieldById(struct ScanInterface* scanner, size_t id);

void reset(struct ScanInterface* scanner);
void destroyScanner(struct ScanInterface* scanner);

size_t getColumnsCount(struct ScanInterface* scanner);
const char* getColumnNameById(struct ScanInterface* scanner, size_t id);
struct Field* getColumnInfoById(struct ScanInterface* scanner, size_t id);
int getColumnId(struct ScanInterface* scanner, const char* name);

#endif