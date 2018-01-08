//
// Created by Mattia Angelini on 08/01/2018.
//

#ifndef SOL_2017_LISTSYNCHRONIZED_H
#define SOL_2017_LISTSYNCHRONIZED_H

#include <pthread.h>
#include "list.h"
#include "../Sync/Sync.h"
#include "../MemoryHelper/MemoryHelper.h"

typedef struct {
    int logicalLength;
    int elementSize;
    ListNode *head;
    ListNode *tail;
    ListFreeFunction freeFn;
    ListCompareFunction cmpFn;
    ListCopyFunction cpyFn;
    pthread_mutex_t* mutex;
} ListSync;

ListSync* ListSync_new(int elementSize, ListFreeFunction freeFn, ListCompareFunction cmpFn,ListCopyFunction cpyFn);
void ListSync_destroy(ListSync* list);

void ListSync_prepend_S(ListSync* list, void* element);
void ListSync_append_S(ListSync* list, void* element);

int ListSync_length(ListSync* list);

void ListSync_head_S(ListSync* list, void* out_element,bool remove_el);
void ListSync_get_head_S(ListSync* list, void* out_element);
void ListSync_remove_head_S(ListSync* list, void* out_element);
void ListSync_drop_head_S(ListSync* list);

void ListSync_tail_S(ListSync* list, void* out_element,bool remove_el);
void ListSync_get_tail_S(ListSync* list, void* out_element);
void ListSync_remove_tail_S(ListSync* list, void* out_element);
void ListSync_drop_tail_S(ListSync* list);

bool ListSync_remove_element_S(ListSync* list,void* el,void* out_element);
void ListSync_destroy_element_S(ListSync* list,void* el);

bool ListSync_find_S(ListSync* list,void* key,void* out_element);

#endif //SOL_2017_LISTSYNCHRONIZED_H
