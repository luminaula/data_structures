#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void list_push_test(size_t count){
    linked_list_t *list = list_create(sizeof(int));
    for(size_t i=0;i<count;i++){
        int a = i;
        list_push_back(list,&a);
        list_push_front(list, &a);
    }
    list_free(list);
}

int main(int argc, char **argv) {
    size_t test_size = 0xfff;
    for(size_t i = 0; i<test_size; i++){
        list_push_test(i);
    }

    return EXIT_SUCCESS;
}