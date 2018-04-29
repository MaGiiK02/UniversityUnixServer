//
// Created by Mattia Angelini on 03/01/2018.
//


/*
* @brief It's a wrapper for the HashTable module that ensure no race condidtion on data,
*       Using an partitoned MutexArray.
*/

#ifndef SOL_2017_HASHTABLESYNCRONIZED_H
#define SOL_2017_HASHTABLESYNCRONIZED_H

#include <pthread.h>
#include <math.h>

#include "HashTable.h"
#include "../Sync/Sync.h"
#include "../MemoryHelper/MemoryHelper.h"
#include "../Utils/Utils.h"

typedef struct{
    HashTable* hashTable;
    int mutexCount;
    pthread_mutex_t** mutex;
} HashTableSync;

HashTableSync* HashSync_new(long size,long elementSize,HashFreeFunction freeFn,HashCopyFunction cpyFn);

void HashSync_destroy(HashTableSync* hash);

int HashSync_add_element_S(HashTableSync* hash,char* key,void* value);

void HashSync_remove_element_S(HashTableSync* hash,char* key,void* out_removed);

void HashSync_destroy_element_S(HashTableSync* hash,char* key);

int HashSync_get_element_S(HashTableSync* hash,char* key,void* out_element);

int HashSync_update_element_S(HashTableSync* hash,char* key,void* element);

void HashSync_lock_by_key(HashTableSync* hash,char* key);

void HashSync_unlock_by_key(HashTableSync* hash,char* key);

int HashSync_lock_by_index(HashTableSync* hash, int i);

int HashSync_unlock_by_index(HashTableSync* hash, int i);

void* HashSync_get_element_pointer(HashTableSync* hash,char* key);
void* HashSync_get_element_pointer_S(HashTableSync* hash,char* key);

bool HashSync_is_element_present_S(HashTableSync* hash,char* key);

#endif //SOL_2017_HASHTABLESYNCRONIZED_H
