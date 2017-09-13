#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "HashTable.h"

void freeIntFunction(void* el){
  free(el);
}

int main(){
  HashTable* ht = Hash_new(256, freeIntFunction);

  assert(Hash_function(ht,"Demo_vcnweuorvn&34ert") == Hash_function(ht,"Demo_vcnweuorvn&34ert"));

  int* tmp = malloc(sizeof(int));
  *tmp = 8;
  Hash_add_element(ht, "Otto" , tmp);

  tmp = malloc(sizeof(int));
  *tmp = 1;
  Hash_add_element(ht, "Uno" , tmp);

  tmp = malloc(sizeof(int));
  *tmp = 3;
  Hash_add_element(ht, "Tre" , tmp);


  Hash_get_element(ht,"Otto", (void**) &tmp);
  assert(*tmp == 8);

  Hash_destroy_element(ht,"Tre");
  assert(Hash_get_element(ht,"Tre",(void**)&tmp) != 0);

  Hash_remove_element(ht,"Uno",  (void**) &tmp);
  assert(*tmp == 1);
  assert(Hash_get_element(ht,"Uno",(void**)&tmp) != 0);


  Hash_destroy_safe(&ht);
  assert(ht == NULL);

  exit(0);

}
