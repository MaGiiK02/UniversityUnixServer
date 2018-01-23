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
#include "../MemoryHelper/MemoryHelper.h"

/*Module Includes */
#include "list.h"

/* private */
void _remove_list_element(List* list,ListNode* node);
void _insert_first_element(List* list,void* element);
bool _find_node(List* list,void* key,ListNode** out_element);


List* List_new(
  int elementSize,
  ListFreeFunction freeFn,
  ListCompareFunction cmpFn,
  ListCopyFunction  cpyFn
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
  list->cpyFn = cpyFn;

  return list;
}

void List_destroy(List* list){
  ListNode* iterator;
  ListNode* tmp;
  tmp = iterator = list->head;
  while(iterator != NULL){
    iterator = iterator->next;
    list->freeFn(tmp->data);
    FREE(tmp)
    tmp = iterator;
  }
  FREE(list)
}

void List_prepend(List* list, void* element){
  assert(element != NULL);
  assert(list != NULL);

  if(list->logicalLength == 0){
    _insert_first_element(list,element);
  }else{
    ListNode* new_node = malloc(sizeof(ListNode));
    new_node->data = malloc(list->elementSize);
    list->cpyFn(new_node->data,element);

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
    new_node->data = malloc(list->elementSize);
    memcpy(new_node->data,element,list->elementSize);

    list->tail->next = new_node;
    new_node->prev = list->tail;
    new_node->next = NULL;
    list->tail = new_node;
    list->logicalLength ++;
  }
}

void _insert_first_element(List* list,void* element){
  ListNode* new_node = malloc(sizeof(ListNode));
  new_node->prev = new_node->next = NULL;
  new_node->data = malloc(list->elementSize);
  memcpy(new_node->data,element,list->elementSize);
  list->head = list->tail = new_node;
  list->logicalLength ++;
}

int List_length(List* list){
  assert(list != NULL);
  return list->logicalLength;
}

void List_get_head(List* list, void* out_element){
  List_head(list,out_element,false);
}

void List_remove_head(List* list, void* out_element){
  List_head(list,out_element,true);
}

void List_drop_head(List* list){
  assert(list != NULL);

  _remove_list_element(list,list->head);
}

void List_head(List* list, void* out_element,bool remove_el){
  assert(list != NULL);
  list->cpyFn(out_element, list->head->data);
  if(remove_el){
    _remove_list_element(list,list->head);
  }
}

void List_get_tail(List* list, void* out_element){
  List_tail(list,out_element,false);
}

void List_remove_tail(List* list, void* out_element){
  List_tail(list,out_element,true);
}

void List_drop_tail(List* list){
  assert(list != NULL);

  _remove_list_element(list,list->tail);
}

void List_tail(List* list, void* out_element,bool remove_el){
  assert(list != NULL);
  list->cpyFn(out_element, list->tail->data);
  if(remove_el){
    _remove_list_element(list,list->tail);
  }
}

bool List_remove_element(List* list,void* el,void* out_element){
  ListNode* to_delete;

  if(_find_node(list,el,&to_delete)){
    list->cpyFn(out_element, to_delete->data);
    _remove_list_element(list,to_delete);
    return true;
  }
  return false;
}

void List_destroy_element(List* list,void* el){
  ListNode* to_delete;

  if(_find_node(list,el,&to_delete)){
    _remove_list_element(list,to_delete);
  }
}

void _remove_list_element(List* list,ListNode* node){

  if(node == NULL) return;

  if((node->prev == NULL) && (node->next == NULL)){
    list->head = list->tail = NULL;
  }else if(node->prev == NULL){
    ListNode* next_node = node->next;
    next_node->prev = NULL;
    list->head = next_node;
  }else if(node->next == NULL){    
    ListNode * prev_node = node->prev;
    prev_node->next = NULL;
    list->tail = prev_node;
  }else{
   ListNode *prev_node = node->prev;
   ListNode *next_node = node->next;
   next_node->prev = prev_node;
   prev_node->next = next_node;
  }

  list->logicalLength --;
  list->freeFn(node->data);

  FREE(node)
}

bool List_find(List* list,void* key,void* out_element){
  ListNode* node;
  bool found = _find_node(list,key,&node);
  if(found && out_element != NULL){
    list->cpyFn(out_element, node->data);
  }
  return found;
}

bool List_update_by_find(List* list,void* key,void* element){
  ListNode* node;
  bool found = _find_node(list,key,&node);
  if(found && out_element != NULL){
    list->cpyFn(node->data, element);
  }
  return found;
}

bool _find_node(List* list,void* key,ListNode** out_element){
  assert(list != NULL);

  ListNode* current_el = list->head;
  bool found = false;

  while(current_el && !found){
    if(list->cmpFn(current_el->data,key) == 0){
      *out_element = current_el;
      found = true;
    }else{
      current_el = current_el->next;
    }
  }

  return found;
}
