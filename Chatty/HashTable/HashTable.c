#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../Error/Error.h"

#include "HashTable.h"

int _hash_element_compare(void* hashEl,void* key){
  HashElement* h_el = (HashElement*)hashEl;
  return strcmp(h_el->key,(char*)key);
}

void _hash_element_free(void* hashEl){
    HashElement* el = (HashElement*)hashEl;
    el->freeFn(el->value);
    free(el);
    hashEl = NULL;
}

HashTable* Hash_new(long size,HashFreeFunction freeFn){
  assert(size > 0);
  assert(freeFn != NULL);

  HashTable* h = malloc(sizeof(HashTable));
  if(h == NULL){
    return NULL;
  }

  h->size = size;
  h->array = malloc(sizeof(List)*size);
  h->freeFn = freeFn;

  return h;
}

void Hash_destroy_safe(HashTable** hash){
  Hash_destroy(*hash);
  *hash = NULL;
}

void Hash_destroy(HashTable* hash){
  int i;
  for(i=0; i<hash->size; i++){
    if(hash->array[i] != NULL){
      List_destroy(hash->array[i]);
    }
  }
  free (hash);
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
  List* value_list = hash->array[index];

  if(value_list == NULL){
    value_list = hash->array[index] = List_new(sizeof(HashElement),_hash_element_free,_hash_element_compare);
  }

  HashElement* el = malloc(sizeof(HashElement));
  el->value = value;
  el->key = key;
  el->freeFn = hash->freeFn;

  List_append(value_list,el);//TO DO solo se la chiave e' diversa
  return 0;
}

void Hash_remove_element(HashTable* hash,char* key,void** out_removed){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];
  if(value_list != NULL){
    HashElement* el;
    List_remove_element(value_list,(void*)key,(void**)&el);
    *out_removed = el->value;
  }
}

void Hash_destroy_element(HashTable* hash,char* key){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list != NULL){
    List_destroy_element(value_list,key);
  } 
}

int Hash_get_element(HashTable* hash,char* key,void** out_element){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL || List_length(value_list)<=0){
    return -1;
  }
  
  HashElement* el;
  List_find(value_list,key,(void**)&el);
  if(el == NULL){
    return -1;
  }

  *out_element = el->value;
  free(el);

  return 0;
}
