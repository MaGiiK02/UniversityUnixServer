#ifndef _LIST_H_
#define _LIST_H_

#include "../Booleans/Booleans.h"

/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @Brief: An implementation with all the basic functions to navigate,
           manage and modify lists, a good structure if you have to remove or
           search few times. It supports generic types of elements.
 */

//in order to create a generic list i have to know how free it's values
typedef void (*ListFreeFunction)(void *);

typedef int (*ListCompareFunction)(void *,void *);
typedef void (*ListCopyFunction)(void *,void *);
typedef int (*ListIterateFunction)(void *);

typedef struct _listNode {
   void* data;
   struct _listNode* next;
   struct _listNode* prev;
} ListNode;

typedef struct {
  int logicalLength;
  int elementSize;
  ListNode *head;
  ListNode *tail;
  ListFreeFunction freeFn;
  ListCompareFunction cmpFn;
  ListCopyFunction cpyFn;
} List;

List* List_new(int elementSize, ListFreeFunction freeFn, ListCompareFunction cmpFn,ListCopyFunction cpyFn);
void List_destroy(List* list);

void List_prepend(List* list, void* element);
void List_append(List* list, void* element);

int List_length(List* list);

void List_head(List* list, void* out_element,bool remove_el);
void List_get_head(List* list, void* out_element);
void* List_get_head_pointer(List* list);
void List_remove_head(List* list, void* out_element);
void List_drop_head(List* list);

void List_tail(List* list, void* out_element,bool remove_el);
void* List_get_tail_pointer(List* list);
void List_get_tail(List* list, void* out_element);
void List_remove_tail(List* list, void* out_element);
void List_drop_tail(List* list);

bool List_remove_element(List* list,void* el,void* out_element);
void List_destroy_element(List* list,void* el);

bool List_find(List* list,void* key,void* out_element);
void* List_find_and_get_pointer(List* list,void* key);
bool List_update_by_find(List* list,void* key,void* element);

void List_iterate(List* list,ListIterateFunction iFn);




#endif /* _LIST_H_ */
