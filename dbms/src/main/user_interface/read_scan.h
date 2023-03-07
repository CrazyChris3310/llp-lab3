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

void reset(struct ScanInterface* scanner);
void destroyScanner(struct ScanInterface* scanner);

#endif