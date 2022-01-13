#ifndef PTR_ARIM_H
#define PTR_ARIM_H

#include <stdint.h>
#include <stddef.h>

#define PTR_OFFSET(ptr, n) (((char *)(ptr)) + (n))
#define PTR_ADVANCE(ptr, n) (ptr = PTR_OFFSET(ptr, n))

int ptr_in_range(const void *ptr, const void *a, const void *b);
ptrdiff_t ptr_diff(const void *ptr, const void *ptr2);

#endif