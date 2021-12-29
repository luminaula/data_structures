#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
int main(int argc, char** argv){
	queue_t *queue = queue_create(sizeof(int));
	for(int i=0;i<70;i++){
		queue_push(queue,&i);
	}
	for(int i=0;i<900;i++){
		int *a = queue_pop(queue);
		if(a){
			printf("%d ",*a);
			free(a);
		}
	}
	printf("\n");
	return EXIT_SUCCESS;
}