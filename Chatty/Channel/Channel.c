//
// Created by Mattia Angelini on 27/09/2017.
//

#include "Channel.h"

bool _isChEmpty(Ch* ch);

Ch* Ch_New(unsigned int size,ChFreeFunction freeFn){
  Ch* ch = malloc(sizeof(Ch));
  ch->queue = malloc(sizeof(void)*size);
  int ris = pthread_mutex_init(&(ch->mu),NULL);
  if(ris != 0 ) exit(1);
  ris = pthread_cond_init(&(ch->condWait),NULL);
  if(ris != 0 ) exit(1);
  ch->next_insert = 0;
  ch->next = 0;
  ch->size = size;
  ch->freeFn = freeFn;
  ch->closed = false;
  return ch;
}

void Ch_Close_S(Ch* ch){
  LOCK_MUTEX_EXIT(ch->mu)
  ch->closed = true;
  UNLOCK_MUTEX_EXIT(ch->mu)
}

void Ch_Free_S(Ch* ch){
  LOCK_MUTEX_EXIT(ch->mu)
  for(int i = 0; i<ch->size; i++){
    if(ch->queue[i]!= NULL){
      ch->freeFn(ch->queue[i]);
    }
  }
  //ENSURE THREAD TO CLOSE {
    ch->next_insert=ch->next_insert;
    ch->closed = true;
    UNLOCK_MUTEX_EXIT(ch->mu)
    pthread_cond_broadcast(&(ch->condWait));
    wait(10); // to be really sure about it
    LOCK_MUTEX_EXIT(ch->mu)
  //}
  free(ch->queue);
  UNLOCK_MUTEX_EXIT(ch->mu)
  free(ch);

}

int Ch_Push_S(Ch* ch,void* el){
    LOCK_MUTEX_EXIT(ch->mu)
    if(!ch->closed){
      ch->queue[ch->next_insert] = el;
      ch->next_insert = (ch->next_insert+1)%ch->size;
      UNLOCK_MUTEX_EXIT(ch->condWait)
    }
    UNLOCK_MUTEX_EXIT(ch->mu)
}

int  Ch_Pop_S(Ch* ch,void** out_el){
  LOCK_MUTEX_EXIT(ch->mu)
  if(_isChEmpty(ch)){
    UNLOCK_MUTEX_EXIT(ch->mu)
    UNLOCK_MUTEX_EXIT(ch->condWait)
    LOCK_MUTEX_EXIT(ch->mu)
  }
  
  //IF empty the function wait for a push call on the same
  while(_isChEmpty(ch) && !ch->closed){
    pthread_cond_wait(&(ch->condWait),&(ch->mu));
  }
  if(_isChEmpty(ch) && ch->closed){
      free(ch->queue);
      UNLOCK_MUTEX_EXIT(ch->mu)
      free(ch);
      return 1;
  }

  *out_el= ch->queue[ch->next];
  ch->queue[ch->next] = NULL;
  ch->next = (ch->next_insert+1)%ch->size;

  if(_isChEmpty(ch) && ch->closed){
      free(ch->queue);
    UNLOCK_MUTEX_EXIT(ch->mu)
    free(ch);
    return 1;
  }
  return 0;

}

bool _isChEmpty(Ch* ch){
  return ch->next == ch->next_insert || ch->next == (ch->next_insert-1)%ch->size;
}