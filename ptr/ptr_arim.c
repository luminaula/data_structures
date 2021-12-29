#include "ptr_arim.h"
#include <stdint.h>

int ptr_in_range(const void *ptr, const void *a, const void *b) {
    if (a > b) {
        const void *tmp = a;
        a = b;
        b = tmp;
    }
    if (ptr >= a && ptr <= b) {
        return 1;
    }
    return 0;
}

ptrdiff_t ptr_diff(const void *ptr, const void *ptr2){
    intptr_t pt = (intptr_t) ptr;
    intptr_t pt2 = (intptr_t) ptr2;
    ptrdiff_t diff = ptr - ptr2;
    if(diff < 0){
        return -diff;
    }
    return diff;
}