#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

void heap_construct_test() {
    size_t heap_size = 0xff;
    int *arr = malloc(sizeof(int) * heap_size);
    for (int i = 0; i < heap_size; i++) {
        arr[i] = rand() & heap_size;
    }
    heap_t *heap = heap_construct(compare_int_lower, sizeof(int),heap_size, arr, 1);

    int *prev = heap_extract(heap);
    if (!prev) {
        return;
    }
    for (size_t i = 1; i < heap->size; i++) {
        int *val = heap_extract(heap);
        if (!val) {
            printf("Heap size error\n");
            // _Exit(EXIT_FAILURE);
        }
        if (heap->comparator(prev, val) > 0) {
            printf("Heap value error %d > %d\n", *val, *prev);
            // _Exit(EXIT_FAILURE);
        }
        *prev = *val;
        free(val);
    }
    free(prev);
    heap_free(heap);
}

void heap_sort_test(){
    int (*comparator)(const void *,const void*) = compare_int_higher;
    size_t array_size = 0xffff;
    int *arr = malloc(sizeof(int) * array_size);
    for(size_t i=0;i<array_size; i++){
        arr[i] = rand();
    }

    heap_sort(comparator,sizeof(int),arr,array_size);

    for(size_t i= 1; i<array_size; i++){
        if(comparator(&arr[i],&arr[i-1]) > 0){
            printf("Heap sort error %d > %d\n",arr[i-1],arr[i]);
        }
    }
    free(arr);
}

void heap_insert_extract_test(){
    size_t heap_size = 0xff;
    heap_t *heap = heap_create(compare_int_lower,sizeof(int));
    for(size_t i = 0; i<heap_size; i++){
        int a = rand() & heap_size;
        heap_insert(heap,&a);
    }
    int *prev = heap_extract(heap);
    while(heap->size){
        int *a = heap_extract(heap);
        if(!a){
            break;
        }
        if(heap->comparator(prev,a) > 0){
            printf("Heap insert error %d > %d\n",*prev, *a);
        }
        *prev = *a;
        free(a);
    }
    free(prev);
    heap_free(heap);
}

int main(int argc, char **argv) {
    // printf("%d\n",0 >> 1);
    srand(time(NULL));

    for (size_t i = 0; i < 1; i++) {
        // heap_sort_test();
        heap_insert_extract_test();
        heap_construct_test();
    }
    
    return EXIT_SUCCESS;
}