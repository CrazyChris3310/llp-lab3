#include <math.h>
#include <string.h>
#include "comparators.h"

bool compareInts(int64_t a, int64_t b,  enum CompareOperator oper) {
    switch (oper) {
        case EQUAL:
            return a == b;
        case NOT_EQUAL:
            return a != b;
        case GREATER:
            return a > b;
        case GREATER_OR_EQUAL:
            return a >= b;
        case LESS:
            return a < b;
        case LESS_OR_EQUAL:
            return a <= b;
    }
    return false;
}

const float accuracy = 1e-5;

bool compareFloats(float a, float b,  enum CompareOperator oper) {
    switch (oper) {
        case EQUAL:
            return fabsf(a - b) < accuracy;
        case GREATER:
            return a > b;
        case GREATER_OR_EQUAL:
            return a >= b;
        case LESS:
            return a < b;
        case LESS_OR_EQUAL:
            return a <= b;
        default:
            return fabsf(a - b) >= accuracy;
    }
}

bool compareBools(bool a, bool b,  enum CompareOperator oper) {
    switch (oper) {
        case EQUAL:
            return a == b;
        case NOT_EQUAL:
            return a != b;
        default:
            return false;
    }
}

bool compareVarchars(const char* a, const char* b,  enum CompareOperator oper) {
    int res = strcmp(a, b);
    switch (oper) {
        case EQUAL:
            return res == 0;
        case NOT_EQUAL:
            return res != 0;
        case GREATER:
            return res > 0;
        case GREATER_OR_EQUAL:
            return res >= 0;
        case LESS:
            return res < 0;
        case LESS_OR_EQUAL:
            return res <= 0;
    }
    return false;
}