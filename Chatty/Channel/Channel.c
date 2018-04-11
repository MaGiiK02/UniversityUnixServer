//
// Created by Mattia Angelini on 27/09/2017.
//

#include <string.h>
#include "Channel.h"

bool _isChEmpty(Ch* ch){
  return (ch->next == ch->next_insert) && ch->lastOp == OP_POP;
}

bool _isChFull(Ch* ch){
  return ch->next_insert == ch->next && ch->lastOp == OP_PUSH;
}

Ch* Ch_New(unsigned int size, unsigned int elSize, ChFreeFunction freeFn){
  Ch* ch = malloc(sizeof(Ch));
  int ris = pthread_mutex_init(&(ch->mu),NULL);
  if(ris != 0 ) exit(1);
  ris = pthread_cond_init(&(ch->condWaitFull),NULL);
  if(ris != 0 ) exit(1);
  ris = pthread_cond_init(&(ch->condWaitEmpty),NULL);
  if(ris != 0 ) exit(1);
  ch->elSize = elSize;
  ch->next_insert = 0;
  ch->next = 0;
  ch->size = size;
  ch->freeFn = freeFn;
  ch->closed = false;
  ch->forcedClose = false;
  ch->lastOp = OP_POP; //it's like the last el was removed
  ch->queue = malloc(sizeof(void*)*size);
  for(int i=0; i<size; i++){
    ch->queue[i] = malloc(elSize);
  }
  return ch;
}

void Ch_Close(Ch* ch,bool forceClose){
  ch->closed = true;
  ch->forcedClose = forceClose;
  pthread_cond_broadcast(&(ch->condWaitEmpty));
}

void Ch_Free(Ch* ch){

  pthread_cond_destroy(&(ch->condWaitFull));
  pthread_cond_destroy(&(ch->condWaitEmpty));
  pthread_mutex_destroy(&(ch->mu));
  for(int i=0; i<ch->size; i++){
    ch->freeFn(ch->queue[i]);
  }
  FREE(ch->queue);
  FREE(ch);

}

int Ch_Push_S(Ch* ch,void* el){
    LOCK_MUTEX_EXIT(ch->mu)
    if(!ch->closed){
      while(_isChFull(ch)){
        pthread_cond_wait(&(ch->condWaitFull),&(ch->mu));
      }
      memcpy(ch->queue[ch->next_insert],el,ch->elSize);
      ch->next_insert = (ch->next_insert+1)%ch->size;
      ch->lastOp=OP_PUSH;
      pthread_cond_broadcast(&(ch->condWaitEmpty));
    }
    UNLOCK_MUTEX_EXIT(ch->mu)
    return 0;
}

int  Ch_Pop_S(Ch* ch,void* out_el){
  LOCK_MUTEX_EXIT(ch->mu)

  //IF empty the function wait for a push call on the same
  while(_isChEmpty(ch) && !ch->closed){
    pthread_cond_wait(&(ch->condWaitEmpty),&(ch->mu));
    //can happen that more thread are awakened at the same time
  }
  if((_isChEmpty(ch) && ch->closed) || ch->forcedClose){
      //if it's forced then: avoid to do anything and only notify all waiting threads tha the ch it's closed
      UNLOCK_MUTEX_EXIT(ch->mu)
      pthread_cond_broadcast(&(ch->condWaitEmpty)); //the first thread to enter on close starts to to notify others waiting for the queue
      return 1;
  }

  memcpy(out_el,ch->queue[ch->next], sizeof(ch->elSize));// not perfect a copy functional approach it's way more correct

  ch->next = (ch->next+1)%ch->size;
  ch->lastOp = OP_POP;
  pthread_cond_broadcast(&(ch->condWaitFull)); //tell to threads in write on socket that now can be empty and the filled
  UNLOCK_MUTEX_EXIT(ch->mu)
  return 0;
}
