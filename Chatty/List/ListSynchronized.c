//
// Created by Mattia Angelini on 08/01/2018.
//

#include "ListSynchronized.h"

ListSync* ListSync_new(int elementSize, ListFreeFunction freeFn, ListCompareFunction cmpFn,ListCopyFunction cpyFn){
  ListSync* list = malloc(sizeof(ListSync));
  list->list =  List_new(elementSize,freeFn,cmpFn,cpyFn);
  pthread_mutex_init(&(list->mutex),NULL);
  return list;
}

void ListSync_destroy(ListSync* list){
  List_destroy(list->list);
  pthread_mutex_destroy(&(list->mutex));
  FREE(list)
}

void ListSync_prepend_S(ListSync* list, void* element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_prepend(list->list,element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}
void ListSync_append_S(ListSync* list, void* element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_append(list->list,element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

int ListSync_length(ListSync* list){
  return List_length(list->list);
}


void ListSync_get_head_S(ListSync* list, void* out_element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_get_head(list->list,out_element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

void ListSync_remove_head_S(ListSync* list, void* out_element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_remove_head(list->list,out_element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}
void ListSync_drop_head_S(ListSync* list){
  LOCK_MUTEX_EXIT(list->mutex);
  List_drop_head(list->list);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

void ListSync_get_tail_S(ListSync* list, void* out_element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_get_tail(list->list,out_element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}
void ListSync_remove_tail_S(ListSync* list, void* out_element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_remove_tail(list->list,out_element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}
void ListSync_drop_tail_S(ListSync* list){
  LOCK_MUTEX_EXIT(list->mutex);
  List_drop_tail(list->list);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

bool ListSync_remove_element_S(ListSync* list,void* el,void* out_element){
  LOCK_MUTEX_EXIT(list->mutex);
  bool ris = List_remove_element(list->list,el,out_element);
  UNLOCK_MUTEX_EXIT(list->mutex);
  return ris;
}
void ListSync_destroy_element_S(ListSync* list,void* el){
  LOCK_MUTEX_EXIT(list->mutex);
  List_destroy_element(list->list,el);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

void ListSync_find_S(ListSync* list,void* key,void* out_element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_find(list->list,key,out_element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

void ListSync_update_by_find_S(ListSync* list,void* key,void* element){
  LOCK_MUTEX_EXIT(list->mutex);
  List_update_by_find(list->list,key,element);
  UNLOCK_MUTEX_EXIT(list->mutex);
}

void ListSync_iterate_S(ListSync* list,ListIterateFunction iFn){
  LOCK_MUTEX_EXIT(list->mutex);
  List_iterate(list->list,iFn);
  UNLOCK_MUTEX_EXIT(list->mutex);
}
