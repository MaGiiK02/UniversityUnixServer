#ifndef _SYNC_H_
#define _SYNC_H_

#include "pthread.h"
#include "stdio.h"

#define LOCK_MUTEX(mutex) \
  if(pthread_mutex_unlock(&mutex)!=0){ \
    perror("Error locking the mutex"); \
    exit(1); \
  }

#define UNLOCK_MUTEX(mutex) \
  if(pthread_mutex_unlock(&mutex)!=0){ \
    perror("Error unlocking the mutex"); \
    exit(1); \
  }

#endif
