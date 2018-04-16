#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../Error/Error.h"
#include "../MemoryHelper/MemoryHelper.h"
#include "../Debugger/Debugger.h"

#include "HashTable.h"


int _hash_element_compare(void* hashEl,void* key){
  HashElement* h_el = (HashElement*)hashEl;
  return strcmp(h_el->key,(char*)key);
}

void _hash_element_free(void* hashEl){
  if(hashEl == NULL) return;
  HashElement* el = (HashElement*)hashEl;
  if(el->value == NULL) return;
  el->freeFn(el->value);
  FREE(el)
}

void _hash_element_copy(void* dst, void* src){
  HashElement* destination = (HashElement*) dst;
  HashElement* source = (HashElement*) src;
  if(src == dst ) return ;
  memcpy(destination,source,sizeof(HashElement));
  source->cpyFn(destination->value,source->value);
}

HashTable* Hash_new(long size,long elementSize, HashFreeFunction freeFn,HashCopyFunction cpyFn){
  assert(size > 0);
  assert(freeFn != NULL);

  HashTable* h = malloc(sizeof(HashTable));
  if(h == NULL){
    return NULL;
  }

  h->size = size;
  h->array = malloc(sizeof(List*)*size);
  memset(h->array,0,sizeof(List*)*size);
  h->elementSize = elementSize;
  h->freeFn = freeFn;
  h->cpyFn = cpyFn;

  return h;
}

void Hash_destroy(HashTable* hash){
  int i;
  for(i=0; i<hash->size; i++){
    if(hash->array[i] != NULL){
      List_destroy(hash->array[i]);
    }
  }
  //the inner pointed values are already freed in List_destroy called before
  FREE(hash->array)
  FREE(hash)
}

int Hash_function(HashTable* hash,char* key){
  unsigned long int hashval = 0;
	int i = 0;

	while(i < strlen(key)) {
		hashval += key[i];
		i++;
	}

	return hashval % hash->size;
}

int Hash_add_element(HashTable* hash,char* key,void* value){
  int index = Hash_function(hash,key);
  Log(("Hash Value For %s is %d\n",key,index));
  List* value_list = hash->array[index];

  if(value_list == NULL){
    value_list = hash->array[index] = List_new(sizeof(HashElement),&_hash_element_free,&_hash_element_compare,&_hash_element_copy);
  } else if(List_find(value_list,(void*)key,NULL)) {
    return 1;//Key already exist
  }

  HashElement el = {0};
  el.value = malloc(hash->elementSize);
  hash->cpyFn(el.value,value);
  strcpy(el.key,key);
  el.freeFn = hash->freeFn;
  el.cpyFn = hash->cpyFn;
  List_append(value_list,&el);
  return 0;
}

void Hash_remove_element(HashTable* hash,char* key,void* out_removed){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];
  if(value_list != NULL){
    HashElement el = {0};
    if(List_remove_element(value_list,(void*)key,(void*)&el)){
      hash->cpyFn(out_removed,el.value);
    }
  }
}

void Hash_destroy_element(HashTable* hash,char* key){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list != NULL){
    List_destroy_element(value_list,key);
  } 
}

int Hash_get_element(HashTable* hash,char* key,void* out_element){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL || List_length(value_list)<=0){
    return -1;
  }
  
  HashElement el = {0};
  if(!List_find(value_list,key,(void*)&el)){
    return -1; // some ide can give unreachable code but it's not the case
  }

  hash->cpyFn(out_element, el.value);

  return 0;
}

void* Hash_get_element_pointer(HashTable* hash,char* key){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL || List_length(value_list)<=0){
    return NULL;
  }

  HashElement el ={0};
  if(!List_find(value_list,key,(void*)&el)){
    return NULL; // some ide can give unreachable code but it's not the case
  }

  return el.value;
}

bool Hash_is_element_present(HashTable* hash,char* key){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL){
    return false;
  }
  return List_find(value_list,(void*)key,NULL);
}

int Hash_update_element(HashTable* hash,char* key,void* element){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL || List_length(value_list)<=0){
    return -1;
  }

  HashElement el = {0};
  if(!List_find(value_list,key,(void*)&el)){
    return -1; // some ide can give unreachable code but it's not the case
  }

  hash->cpyFn(el.value,element);

  return 0;
}

