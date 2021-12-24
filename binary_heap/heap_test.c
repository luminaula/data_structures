#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
int compare_int_higher(const void *a, const void *b) {
    int _a = *(const int *)a;
    int _b = *(const int *)b;

    if (_a < _b)
        return 1;
    else if (_a > _b)
        return -1;
    else {
        return 0;
    }
}


int compare_int_lower(const void *a, const void *b) {
    int _a = *(const int *)a;
    int _b = *(const int *)b;

    if (_a > _b)
        return 1;
    else if (_a < _b)
        return -1;
    else {
        return 0;
    }
}

int main(int argc, char **argv) {
    heap_t *heap = heap_create(compare_int_higher, sizeof(int));
    for (size_t i = 0; i < 64; i++) {
        heap_insert(heap, &i);
    }
    for (size_t i = 0; i < 70; i++) {
        int *b = (int*)heap_extract(heap);
        if (b) {
            printf("%d\n", *b);
            free(b);
        }
    }
    return EXIT_SUCCESS;
}