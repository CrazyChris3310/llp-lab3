#include "my_string.h"

int compareStrings(struct String a, struct String b) {
    if (a.lenght < b.lenght)
        return -1;
    else if (a.lenght > b.lenght)
        return 1;
        
    for (int i = 0; i < a.lenght; ++i) {
        if (a.value[i] < b.value[i])
            return -1;
        else if (a.value[i] > b.value[i])
            return 1;
    }
    return 0;
}

bool equals(struct String a, struct String b) {
    return compareStrings(a, b) == 0;
}