/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 */

/* @Description:
 *  This file is used to test the generic list component of the project.
 */

#include <stdio.h>

#include "list.h"
#include <assert.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>

typedef struct _test_struct{
  int* valPointer;
  int val;
} TestStruct;

int str_cmp(void* el1,void* el2){
  return strcmp((char*)el1,(char*)el2);
}

void str_free(void* el){
  free(el);
}

int int_cmp(void* el1,void* el2){
  return el1 - el2;
}

void int_free(void* el){
  free(el);
}

int struct_cmp(void* el1,void* el2){
  TestStruct* s1;
  TestStruct* s2;
  s1 = (TestStruct*) el1;
  s2 = (TestStruct*) el2;
  return (((*(s1->valPointer)) * s1->val)^2) - (((*(s2->valPointer))*s2->val)^2);
}

void struct_free(void* el){
  TestStruct* s;
  s = (TestStruct*)el;
  free(s->valPointer);
  free(s);
}

int main(int argc, char *argv[]){
  List* int_list = List_new(sizeof(int),&int_free,&int_cmp);
  int* temp = malloc (sizeof(int));
  *temp = 12;
  List_append(int_list,(void*)temp);
  assert(List_length(int_list) == 1);

  List_get_head(int_list,&temp);
  assert(*temp == 12);
  List_get_tail(int_list,&temp);
  assert(*temp == 12);

  temp = malloc (sizeof(int));
  *temp = 4;
  List_append(int_list,(void*)temp);
  assert(List_length(int_list) == 2);

  List_get_head(int_list,&temp);
  assert(*temp == 12);
  List_get_tail(int_list,&temp);
  assert(*temp == 4);

  temp = malloc (sizeof(int));
  *temp = 1;
  List_prepend(int_list,(void*)temp);
  assert(List_length(int_list) == 3);

  List_get_head(int_list,&temp);
  assert(*temp == 1);
  List_get_tail(int_list,&temp);
  assert(*temp == 4);

  List_drop_tail(int_list,NULL);
  List_get_tail(int_list,&temp);
  assert(*temp == 12);
  assert(List_length(int_list) == 2);
  List_get_head(int_list,&temp);
  assert(*temp == 1);

  List_drop_head(int_list,NULL);
  List_get_tail(int_list,&temp);
  assert(*temp == 12);
  assert(List_length(int_list) == 1);
  List_get_head(int_list,&temp);
  assert(*temp == 12);

  List_destroy(int_list);

}
