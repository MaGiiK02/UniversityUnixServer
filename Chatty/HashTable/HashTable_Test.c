#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "HashTable.h"
char* randstring(size_t length);
void print_hash(HashTable* ht);
void freeIntFunction(void* el){
  free((int*)el);
}
void cpyIntFunction(void* dst,void* src){
  memcpy(dst,src, sizeof(int));
}


int main(){
  HashTable* ht = Hash_new(256, sizeof(int), &freeIntFunction, &cpyIntFunction);

  assert(Hash_function(ht,"Demo_vcnweuorvn&34ert") == Hash_function(ht,"Demo_vcnweuorvn&34ert"));

  int* tmp = malloc(sizeof(int));
  *tmp = 8;
  Hash_add_element(ht, "Otto" , tmp);

  *tmp = 1;
  Hash_add_element(ht, "Uno" , tmp);

  *tmp = 3;
  Hash_add_element(ht, "Tre" , tmp);

  print_hash(ht);

  Hash_get_element(ht, "Otto", (void*) tmp);
  assert(*tmp == 8);

  Hash_destroy_element(ht,"Tre");
  assert(Hash_get_element(ht,"Tre",(void*) tmp) != 0);
  print_hash(ht);

  Hash_remove_element(ht,"Uno",  (void*) tmp);
  assert(*tmp == 1);
  assert(Hash_get_element(ht,"Uno",(void*) tmp) != 0);

  print_hash(ht);

  //stresstest
  char* name;
  for(int i=0;i< 10000;i ++){
    name = randstring(25);
    *tmp = i;
    Hash_add_element(ht, name, tmp);
    free(name);
  }
  free(tmp);

  print_hash(ht);

  Hash_destroy(ht);

  exit(0);

}

char* randstring(size_t length) {

  static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
  char *randomString = NULL;

  if (length) {
    randomString = malloc(sizeof(char) * (length +1));

    if (randomString) {
      for (int n = 0;n < length;n++) {
        int key = rand() % (int)(sizeof(charset) -1);
        randomString[n] = charset[key];
      }

      randomString[length] = '\0';
    }
  }

  return randomString;
}

void print_hash(HashTable* ht){
  List* list;
  for(int i=0; i<ht->size; i++ ) {
    list = ht->array[i];
    if (list) {
      fprintf(stdout, "%d->(",i);
      ListNode* node = list->head;
      while (node != NULL){
        if(node->next == NULL){
          fprintf(stdout, "%d",*((int*)((HashElement*)node->data)->value));
        }else{
          fprintf(stdout, "%d,",*((int*)((HashElement*)node->data)->value));
        }
        node = node->next;
      }
      fprintf(stdout,")\n");
    }
  }
}

