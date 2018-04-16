
//
// Created by Mattia Angelini on 03/01/2018.
//

#include "HashTableSynchronized.h"

int HashSync_lock_by_index(HashTableSync* hash, int i){
  int mutexIndex = i%(hash->mutexCount);
  LOCK_MUTEX_RETURN(*(hash->mutex[mutexIndex]),-1)
  return 0;
}

int HashSync_unlock_by_index(HashTableSync* hash, int i) {
  int mutexIndex = i%(hash->mutexCount);
  UNLOCK_MUTEX_RETURN(*(hash->mutex[mutexIndex]),-1)
  return 0;
}

HashTableSync* HashSync_new(long size,long elementSize,HashFreeFunction freeFn,HashCopyFunction cpyFn){
  HashTableSync* hash = malloc(sizeof(HashTableSync));
  hash->hashTable = Hash_new(size, elementSize, freeFn, cpyFn);
  hash->mutexCount = Utils_calculate_mutex_array_size(size);
  hash->mutex = malloc(sizeof(pthread_mutex_t*)*hash->mutexCount);
  for(int i = 0; i<hash->mutexCount; i++ ){
    hash->mutex[i] = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(hash->mutex[i], NULL);
  }
  return hash;
}

void HashSync_destroy(HashTableSync* hash){
  for(int i=0; i<hash->mutexCount; i++){
    pthread_mutex_destroy(hash->mutex[i]);
    FREE(hash->mutex[i])
  }
  Hash_destroy(hash->hashTable);
  FREE(hash->mutex)
  FREE(hash)
}

int HashSync_add_element_S(HashTableSync* hash,char* key,void* value){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  int res = Hash_add_element(hash->hashTable, key,value);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

void HashSync_remove_element_S(HashTableSync* hash,char* key,void* out_removed){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  Hash_remove_element(hash->hashTable, key,out_removed);
  HashSync_unlock_by_index(hash,hashValue);
}

void HashSync_destroy_element_S(HashTableSync* hash,char* key){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  Hash_destroy_element(hash->hashTable, key);
  HashSync_unlock_by_index(hash,hashValue);
}

int Hash_get_element_S(HashTableSync* hash,char* key,void* out_element){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  int res = Hash_get_element(hash->hashTable, key,out_element);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

int HashSync_update_element_S(HashTableSync* hash,char* key,void* element){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  int res = Hash_update_element(hash->hashTable, key,element);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

void HashSync_lock_by_key(HashTableSync* hash,char* key){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
}

void HashSync_unlock_by_key(HashTableSync* hash,char* key){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_unlock_by_index(hash,hashValue);
}

void* HashSync_get_element_pointer(HashTableSync* hash,char* key){
  return Hash_get_element_pointer(hash->hashTable,key);
}

void* HashSync_get_element_pointer_S(HashTableSync* hash,char* key){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  void* res = HashSync_get_element_pointer(hash,key);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

bool HashSync_is_element_present_S(HashTableSync* hash,char* key){
  int hashValue = Hash_function(hash->hashTable,key);
  HashSync_lock_by_index(hash,hashValue);
  bool res = Hash_is_element_present(hash->hashTable,key);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

