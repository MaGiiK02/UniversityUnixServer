#ifndef _HASH_H_
#define _HASH_H_

/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @Brief: The hash structure (that will be implememented with list when collision happens)
 */
#ifndef HASH_KEY_SIZE
#define HASH_KEY_SIZE  32
#endif

#include "../List/list.h"

typedef void (*HashFreeFunction)(void*);
typedef void (*HashCopyFunction)(void* , void*);

typedef struct{
  char key[HASH_KEY_SIZE];
  void* value;
  HashFreeFunction freeFn;
  HashCopyFunction cpyFn;
} HashElement;

typedef struct{
    List** array;
    long size;
    HashCopyFunction cpyFn;
    HashFreeFunction freeFn;
    long elementSize;
} HashTable;

HashTable* Hash_new(long size,long elementSize,HashFreeFunction freeFn,HashCopyFunction cpyFn);

void Hash_destroy(HashTable* hash);

int Hash_function(HashTable* hash,char* key);

int Hash_add_element(HashTable* hash,char* key,void* value);

void Hash_remove_element(HashTable* hash,char* key,void* out_removed);

void Hash_destroy_element(HashTable* hash,char* key);

int Hash_get_element(HashTable* hash,char* key,void* out_element);

void* Hash_get_element_pointer(HashTable* hash,char* key);

int Hash_update_element(HashTable* hash,char* key,void* element);

bool Hash_is_element_present(HashTable* hash,char* key);

#endif
