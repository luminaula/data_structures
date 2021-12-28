#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int g_test_failure = 0;

int compare_int_higher(const void *a, const void *b) {
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

int compare_int_lower(const void *a, const void *b) {
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

void heap_construct_test(size_t size) {
    int *arr = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() & size;
    }
    heap_t heap = heap_construct(compare_int_lower, sizeof(int), size, arr, 1);

    int *prev = heap_extract(heap);
    if (!prev) {
        return;
    }
    for (size_t i = 0; i < heap.size; i++) {
        int *val = heap_extract(heap);
        if (!val) {
            fprintf(stderr, "Heap size error\n");
            g_test_failure = 1;
            exit(EXIT_FAILURE);
        }
        if (heap.comparator(val, prev) > 0) {
            fprintf(stderr, "Heap value error %d > %d\n", *val, *prev);
            g_test_failure = 1;
            exit(EXIT_FAILURE);
        }
        *prev = *val;
        free(val);
    }
    free(prev);
    heap_free(heap);
}

void heap_sort_test(size_t size) {
    int (*comparator)(const void *, const void *) = compare_int_lower;
    int *arr = malloc(sizeof(int) * size);
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }

    heap_sort(comparator, sizeof(int), arr, size);

    for (size_t i = 1; i < size; i++) {
        if (comparator(&arr[i - 1], &arr[i]) > 0) {
            g_test_failure = 1;
            fprintf(stderr, "Heap sort error index %lu %d > %d\n", i, arr[i - 1], arr[i]);
            exit(EXIT_FAILURE);
        }
    }
    free(arr);
}

void heap_insert_extract_test(size_t size) {
    heap_t heap = heap_create(compare_int_lower, sizeof(int));
    for (size_t i = 0; i < size; i++) {
        int a = rand() & size;
        heap_insert(heap, &a);
    }
    int *prev = heap_extract(heap);
    while (heap.size) {
        int *a = heap_extract(heap);
        if (!a) {
            fprintf(stderr, "Heap extract error\n");
            g_test_failure = 1;
            exit(EXIT_FAILURE);
        }
        if (heap.comparator(a, prev) > 0) {
            fprintf(stderr, "Heap value error %d > %d\n", *prev, *a);
            g_test_failure = 1;
            exit(EXIT_FAILURE);
        }
        *prev = *a;
        free(a);
    }
    free(prev);
    heap_free(heap);
}

void at_exit_message() { printf("Heap tests : %s\n", g_test_failure ? "Failed" : "Success"); }

int main(int argc, char **argv) {
    srand(time(NULL));
    size_t test_set_size = 0xfff;
    atexit(at_exit_message);

    for (size_t i = 1; i < test_set_size; i++) {
        heap_sort_test(test_set_size);
        heap_insert_extract_test(test_set_size);
        heap_construct_test(test_set_size);
        heap_sort_test(i);
        heap_insert_extract_test(i);
        heap_construct_test(i);

    }

    return g_test_failure;
}