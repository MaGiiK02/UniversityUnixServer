#ifndef _HASH_H_
#define _HASH_H_

/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @Brief: The hash structure (that will be implememented with list when collision happens)
 */


#include "../List/list.h"

typedef void (*HashFreeFunction)(void*);
typedef void (*HashCopyFunction)(void*, void*);

typedef struct{
  char* key;
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
    HashElement* workingElement; //used to avoid the continue malloc and free
} HashTable;

HashTable* Hash_new(long size,long elementSize,HashFreeFunction freeFn,HashFreeFunction cpyFn);

void Hash_destroy(HashTable* hash);

int Hash_function(HashTable* hash,char* key);

int Hash_add_element(HashTable* hash,char* key,void* value);

void Hash_remove_element(HashTable* hash,char* key,void* out_removed);

void Hash_destroy_element(HashTable* hash,char* key);

int Hash_get_element(HashTable* hash,char* key,void* out_element);

int Hash_update_element(HashTable* hash,char* key,void* element);

#endif
