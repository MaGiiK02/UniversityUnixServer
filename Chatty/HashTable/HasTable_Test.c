#include <stdio.h>
#include <stdlib.h>

#include "HashTable.h"

void freeIntFunction(void* el){
  free(el);
}

int main(){
  HashTable ht = Hash_new(256, freeIntFunction);

  int* tmp = malloc(sizeof(int));
  *tmp = 8;
  Hash_add_element(ht, "Otto" , tmp);

  tmp = malloc(sizeof(int));
  *tmp = 1;
  Hash_add_element(ht, "Uno" , tmp);

  tmp = malloc(sizeof(int));
  *tmp = 3;
  Hash_add_element(ht, "Tre" , tmp);


  Hash_get_element(ht,"Otto", &tmp)
  assert(*tmp == 8);

  Hash_destroy(ht,"Tre");
  assert(Hash_get_element(ht,"Tre",NULL) != 0);

  Hash_remove_element(ht,"Uno", &tmp);
  assert(*tmp == 1);
  assert(Hash_get_element(ht,"Uno",NULL) != 0);


  Hash_destroy(&ht);
  assert(ht == NULL);

  exit(0);

}
