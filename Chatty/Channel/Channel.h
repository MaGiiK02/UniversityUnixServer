//
// Created by Mattia Angelini on 27/09/2017.
// ispired by Golang Channels
//

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <pthread.h>
#include "../Sync/Sync.h"
#include "../Booleans/Booleans.h"

typedef void (*ChFreeFunction)(void *);

typedef struct {
    void** queue;
    unsigned int next;
    unsigned int next_insert;
    unsigned int size;
    bool closed;
    pthread_mutex_t mu;
    pthread_cond_t condWait;
    ChFreeFunction freeFn;
} Ch;

Ch* Ch_New(unsigned int size,ChFreeFunction freeFn);
void Ch_Close_S(Ch* ch);
void Ch_Free_S(Ch* cH);

int Ch_Push_S(Ch* ch,void* el);
int Ch_Pop_S(Ch* ch,void** out_el);

#endif //_CHANNEL_H_
