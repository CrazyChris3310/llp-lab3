#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <inttypes.h>
#include <stdbool.h>

enum CompareOperator { EQUAL, LESS, GREATER, NOT_EQUAL, GREATER_OR_EQUAL, LESS_OR_EQUAL };

bool compareInts(int64_t a, int64_t b, enum CompareOperator oper);
bool compareFloats(float a, float b, enum CompareOperator oper);
bool compareBools(bool a, bool b,  enum CompareOperator oper);
bool compareVarchars(const char* a, const char* b,  enum CompareOperator oper);

#endif