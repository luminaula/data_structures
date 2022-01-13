#include "ptr_arim.h"
#include <stdint.h>
#include <string.h>
#include "hash_table.h"
#include <stdlib.h>
#include "murmur3.h"

//todo todo

uint32_t murmur3_hash(const void *key,size_t size){
	static uint32_t seed = 0xEA7BEA57;
	uint32_t out;
	MurmurHash3_x86_32(key,size,seed,&out);
	return out;
}

hash_table_t *create_hash_table(size_t key_size, size_t value_size){
	hash_table_t *hash_table = malloc(sizeof(hash_table_t));
	hash_table->buckets = calloc(1,sizeof(void*));
	hash_table->key_size = key_size;
	hash_table->value_size = value_size;
	hash_table->hash_func = murmur3_hash;
	return hash_table;
}

void hash_table_free(hash_table_t *hash_table){
	if(!hash_table){
		return;
	}
	if(hash_table->buckets){
		for(size_t i=0;i<hash_table->bucket_count; i++){
			if(hash_table->buckets[i]){
				free(hash_table->buckets[i]);
			}
		}
	}
	free(hash_table);
}

void hash_table_insert(hash_table_t *hash_table,void *key, void *value){
	size_t entry_size =sizeof(hash_t) + hash_table->key_size + hash_table->value_size + sizeof(void*);
	void *key_value_pair = malloc(entry_size);
	hash_t hash = hash_table->hash_func(key,hash_table->key_size);
	
	void *write_ptr = key_value_pair;

	memcpy(write_ptr,&hash,sizeof(hash_t));
	PTR_ADVANCE(write_ptr,sizeof(hash));

	memcpy(write_ptr,key,hash_table->key_size);
	PTR_ADVANCE(write_ptr,hash_table->key_size);

	memcpy(write_ptr,value,hash_table->value_size);
	PTR_ADVANCE(write_ptr,hash_table->value_size);

	*(uintptr_t*)PTR_OFFSET(key_value_pair,entry_size - sizeof(void*)) = 0;

	return;
}