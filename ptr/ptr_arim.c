#include "ptr_arim.h"

int ptr_in_range(const void *ptr,const void *a, const void *b ){
	if(a > b){
		void *tmp = a;
		a = b;
		b = tmp;
	}
	if(ptr >= a && ptr <= b){
		return 1;
	}
	return 0;
}
