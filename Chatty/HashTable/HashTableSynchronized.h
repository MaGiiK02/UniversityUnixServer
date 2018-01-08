//
// Created by Mattia Angelini on 03/01/2018.
//

#ifndef SOL_2017_HASHTABLESYNCRONIZED_H
#define SOL_2017_HASHTABLESYNCRONIZED_H

#include <pthread.h>
#include <math.h>

#include "HashTable.h"
#include "../Sync/Sync.h"
#include "../MemoryHelper/MemoryHelper.h"

typedef struct{
    List** array;
    long size;
    HashCopyFunction cpyFn;
    HashFreeFunction freeFn;
    long elementSize;
    HashElement* workingElement; //used to avoid the continue malloc and free
    int mutexCount;
    pthread_mutex_t* mutex[];
} HashTableSync;

HashTableSync* HashSync_new(long size,long elementSize,HashFreeFunction freeFn,HashFreeFunction cpyFn);

void HashSync_destroy(HashTableSync* hash);

int HashSync_add_element_S(HashTableSync* hash,char* key,void* value);

void HashSync_remove_element_S(HashTableSync* hash,char* key,void* out_removed);

void HashSync_destroy_element_S(HashTableSync* hash,char* key);

int HashSync_get_element_S(HashTableSync* hash,char* key,void* out_element);

#endif //SOL_2017_HASHTABLESYNCRONIZED_H
