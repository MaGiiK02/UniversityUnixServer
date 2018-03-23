
//
// Created by Mattia Angelini on 03/01/2018.
//

#include "HashTableSynchronized.h"

int HashSync_lock_by_index(HashTableSync* hash, int i){
  int mutexIndex = i%(hash->mutexCount);
  LOCK_MUTEX_RETURN(hash->mutex[mutexIndex],-1)
  return 0;
}

int HashSync_unlock_by_index(HashTableSync* hash, int i) {
  int mutexIndex = i%(hash->mutexCount);
  UNLOCK_MUTEX_RETURN(hash->mutex[mutexIndex],-1)
  return 0;
}

HashTableSync* HashSync_new(long size,long elementSize,HashFreeFunction freeFn,HashFreeFunction cpyFn){
  HashTableSync* hash = (HashTableSync*)Hash_new(size, elementSize, freeFn, cpyFn);
  hash->mutexCount = Utils_calculate_mutex_array_size(hash->size);
  hash->mutex = malloc(sizeof(pthread_mutex_t*)*hash->mutexCount);
  for(int i = 0; i<hash->mutexCount; i++ ){
    pthread_mutex_init(&(hash->mutex[i]), NULL);
  }
  return hash;
}

void HashSync_destroy(HashTableSync* hash){
  int i;
  for(i=0; i<hash->size; i++){
    if(hash->array[i] != NULL){
      List_destroy(hash->array[i]);
    }
  }
  for(i=0; i<hash->mutexCount; i++){
    pthread_mutex_destroy(&(hash->mutex[i]));
  }
  //TODO FREE(hash->mutex)
  //TODO hash->freeFn(hash->workingElement->value);
  FREE(hash->workingElement) //the inner pointed values are already freed in List_destroy called before
  FREE(hash->array)
  FREE(hash)
}

int HashSync_add_element_S(HashTableSync* hash,char* key,void* value){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_lock_by_index(hash,hashValue);
  int res = Hash_add_element((HashTable*)hash, key,value);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

void HashSync_remove_element_S(HashTableSync* hash,char* key,void* out_removed){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_lock_by_index(hash,hashValue);
  Hash_remove_element((HashTable*)hash, key,out_removed);
  HashSync_unlock_by_index(hash,hashValue);
}

void HashSync_destroy_element_S(HashTableSync* hash,char* key){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_lock_by_index(hash,hashValue);
  Hash_destroy_element((HashTable*)hash, key);
  HashSync_unlock_by_index(hash,hashValue);
}

int Hash_get_element_S(HashTableSync* hash,char* key,void* out_element){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_lock_by_index(hash,hashValue);
  int res = Hash_get_element((HashTable*)hash, key,out_element);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

int HashSync_update_element_S(HashTableSync* hash,char* key,void* element){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_lock_by_index(hash,hashValue);
  int res = Hash_update_element((HashTable*)hash, key,element);
  HashSync_unlock_by_index(hash,hashValue);
  return res;
}

void HashSync_lock_by_key(HashTableSync* hash,char* key){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_lock_by_index(hash,hashValue);
}

void HashSync_unlock_by_key(HashTableSync* hash,char* key){
  int hashValue = Hash_function((HashTable*)hash,key);
  HashSync_unlock_by_index(hash,hashValue);
}
