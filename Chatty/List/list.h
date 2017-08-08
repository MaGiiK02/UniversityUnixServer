#ifndef _LIST_H_
#define _LIST_H_

#include "../Booleans/Booleans.h"

/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @Brief: An implementation with all the basic function to navigate,
           manage and modify lists, a good structure if you have to remove or
           search few times.
 */

//in order to create a generic list i have to know how free it's values
typedef void (*FreeFunction)(void *);

typedef int (*CompareFunction)(void *,void *);

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
  FreeFunction freeFn;
  CompareFunction cmpFn;
} List;

List* List_new(int elementSize, FreeFunction freeFn, CompareFunction cmpFn);
void List_destroy(List* list);

void List_prepend(List* list, void* element);
void List_append(List* list, void* element);

int List_length(List* list);

void List_head(List* list, void** out_element,bool remove_el);
void List_get_head(List* list, void** out_element);
void List_drop_head(List* list, void** out_element);

void List_tail(List* list, void** out_element,bool remove_el);
void List_get_tail(List* list, void** out_element);
void List_drop_tail(List* list, void** out_element);

void List_remove(List* list,void* el);

bool List_find(List* list,void* key,void** out_element);

#endif /* _LIST_H_ */
