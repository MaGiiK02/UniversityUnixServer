/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 */

/* System Includes */
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

/* Project Includes */
#include "../Utils/Utils.h"

/*Module Includes */
#include "list.h"

/* private */
void _destroy_list_element(List* list,ListNode* node);
void _remove_list_element(List* list,ListNode* node,bool free_removed);
void _insert_first_element(List* list,void* element);

List* List_new(
  int elementSize,
  ListFreeFunction freeFn,
  ListCompareFunction cmpFn
){
  assert(elementSize > 0);
  assert(freeFn != NULL);
  assert(cmpFn != NULL);

  List* list = malloc (sizeof(List));

  list->logicalLength = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
  list->freeFn = freeFn;
  list->cmpFn = cmpFn;

  return list;
}

void List_destroy(List* list){
  ListNode* iterator;
  ListNode* tmp;
  tmp = iterator = list->head;
  while(iterator != NULL){
    iterator = iterator->next;
    list->freeFn(tmp->data);
    free(tmp);
    tmp = iterator;
  }
  free(list);
}

void List_prepend(List* list, void* element){
  assert(element != NULL);
  assert(list != NULL);

  if(list->logicalLength == 0){
    _insert_first_element(list,element);
  }else{
    ListNode* new_node = malloc(sizeof(ListNode));
    new_node->data = element;

    list->head->prev = new_node;
    new_node->next = list->head;
    list->head = new_node;
    new_node->prev = NULL;
    list->logicalLength ++;
  }
}

void List_append(List* list, void* element){
  assert(element != NULL);
  assert(list != NULL);

  if(list->logicalLength == 0){
    _insert_first_element(list,element);
  }else{
    ListNode* new_node = malloc(sizeof(ListNode));
    new_node->data = element;

    list->tail->next = new_node;
    new_node->prev = list->tail;
    new_node->next = NULL;
    list->tail = new_node;
    list->logicalLength ++;
  }
}

void _insert_first_element(List* list,void* element){
  ListNode* new_node = malloc(sizeof(ListNode));
  new_node->data = element;
  list->head = list->tail = new_node;
  list->logicalLength ++;
}

int List_length(List* list){
  assert(list != NULL);
  return list->logicalLength;
}

void List_get_head(List* list, void** out_element){
  List_head(list,out_element,false);
}

void List_drop_head(List* list, void** out_element){
  List_head(list,out_element,true);
}

void List_head(List* list, void** out_element,bool remove_el){
  assert(list != NULL);

  if(out_element != NULL){
    *out_element = list->head->data;
  }

  if(remove_el){
    _remove_list_element(list,list->head,remove_el);
  }
}

void List_get_tail(List* list, void** out_element){
  List_tail(list,out_element,false);
}

void List_drop_tail(List* list, void** out_element){
  List_tail(list,out_element,true);
}

void List_tail(List* list, void** out_element,bool remove_el){
  assert(list != NULL);

  if(out_element != NULL){
    *out_element = list->tail->data;
  }

  if(remove_el){
    _remove_list_element(list,list->tail,remove_el);
  }
}

void List_remove_element(List* list,void* el,void** out_element){
  ListNode* iterator = list->head;
  bool finded = false;

  while(iterator && !finded){
    iterator = iterator->next;
    if(list->cmpFn(iterator,el)){
      finded = true;
    }
  }
  _remove_list_element(list,iterator,false);
  *out_element = iterator;
}

void List_destroy_element(List* list,void* el){
  ListNode* iterator = list->head;
  bool finded = false;

  while(iterator && !finded){
    iterator = iterator->next;
    if(list->cmpFn(iterator,el)){
      finded = true;
    }
  }
  _remove_list_element(list,iterator,true);
}

void _destroy_list_element(List* list,ListNode* node){
  _remove_list_element(list,node,true);
}

void _remove_list_element(List* list,ListNode* node,bool free_removed){

  if(node == NULL) return;

  if(node->prev == NULL){
    ListNode* next_node = node->next;
    next_node->prev = NULL;
    list->head = next_node;
  }else if(node->next == NULL){
    ListNode * prev_node = node->prev;
    prev_node->next = NULL;
    list->tail = prev_node;
  }else{
   ListNode *prev_node = node ->prev;
   ListNode *next_node = node->next;
   next_node->prev = prev_node;
   prev_node->next = next_node;
  }

  list->logicalLength --;
  if(free_removed){
    list->freeFn(node->data);
  }
  free(node);
}

bool List_find(List* list,void* key,void** out_element){
  assert(list != NULL);
  assert(out_element == NULL);

  ListNode* current_el = list->head;
  void* el = NULL;
  bool finded = false;

  while(current_el && !finded){
    if(list->cmpFn(current_el->data,key) == 0){
      el = current_el->data;
      finded = true;
    }else{
      current_el = current_el->next;
    }
  }

  if(finded){
    *out_element = el;
    return true;
  }else{
    return false;
  }
}
