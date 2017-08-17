#include "hash.h"


int _hash_element_compare(void* hashEl,void* key){
  HashElement* h_el1 = (HashElement*)hashEl;
  return strcmp(h_el1->key,(char*)key);
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

  Hash h = malloc(sizeof(Hash));
  if(h == NULL){
    return NULL;
  }

  h->size = size;
  h->array = malloc(sizeof(List)*size);
  h->freeFn = freeFn;

  return h;
}

void Hash_destroy(HashTable* hash){
  int i;
  for(i=0; i++; i<hash.size){
    if(hash.array[i] != NULL){
      List_destroy((List*)hash.array[i]);
    }
  }
  free (hash);
  hash = NULL;
}

int Hash_function(HashTable* hash,char* key){
  unsigned long int hashval;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hash->size;
}

int Hash_add_element(HashTable* hash,char* key,void* value){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL){
    value_list = List_new(sizeof(HashElement),_hash_element_free,_hash_element_compare);
  }

  HashElement el = malloc(sizeof(HashElement));
  el->value = value;
  el->key = key;

  List_append(value_list,el);
}

int Hash_remove_element(HashTable* hash,char* key,void** out_removed){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL){
    return -1;
  }

  return List_remove_element(value_list,key,out_removed);

}

int Hash_destroy_element(HashTable* hash,char* key){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL){
    return -1;
  }

  return List_destroy_element(value_list,key);
}

int Hash_get_element(HashTable* hash,char* key,void** out_element){
  int index = Hash_function(hash,key);
  List* value_list = hash->array[index];

  if(value_list == NULL){
    return -1;
  }

  int result = List_find(value_list,key,out_element);
  if(result){
    return -1;
  }

  return 0;
}
