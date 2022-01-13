#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t hash_t;


typedef struct hash_table_t{
	size_t bucket_count;
	size_t key_size;
	size_t value_size;
	uint32_t (*hash_func)(const void *key,size_t size);
	void **buckets;
}hash_table_t;

hash_table_t *create_hash_table(size_t key_size, size_t value_size);

#endif