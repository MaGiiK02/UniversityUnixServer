//
// Created by Mattia Angelini on 27/09/2017.
// ispired by Golang Channels
//

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <pthread.h>
#include "../Sync/Sync.h"
#include "../Booleans/Booleans.h"
#include "../MemoryHelper/MemoryHelper.h"

#define OP_PUSH 0
#define OP_POP 1

typedef void (*ChFreeFunction)(void *);

typedef struct {
    void** queue;
    unsigned int next;
    unsigned int next_insert;
    unsigned int size;
    unsigned int elSize;
    int lastOp;
    bool closed;
    bool forcedClose;
    pthread_mutex_t mu;
    pthread_cond_t condWaitFull;
    pthread_cond_t condWaitEmpty;
    ChFreeFunction freeFn;
} Ch;

Ch* Ch_New(unsigned int size,unsigned int elSize,ChFreeFunction freeFn);
void Ch_Close(Ch* ch,bool forceClose);
void Ch_Free(Ch* cH);

int Ch_Push_S(Ch* ch,void* el);
int Ch_Pop_S(Ch* ch,void* out_el);

#endif //_CHANNEL_H_
