#ifndef _SYNC_H_
#define _SYNC_H_

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define LOCK_MUTEX_EXIT(mutex) \
  if(pthread_mutex_unlock(&mutex)!=0){ \
    perror("Error locking the mutex"); \
    exit(1); \
  }

#define UNLOCK_MUTEX_EXIT(mutex) \
  if(pthread_mutex_unlock(&mutex)!=0){ \
    perror("Error unlocking the mutex"); \
    exit(1); \
  }

#define LOCK_MUTEX_RETURN(mutex,ret_val) \
  if(pthread_mutex_unlock(&mutex)!=0){ \
    perror("Error locking the mutex"); \
    return ret_val; \
  }

#define UNLOCK_MUTEX_RETURN(mutex,ret_val) \
  if(pthread_mutex_unlock(&mutex)!=0){ \
    perror("Error unlocking the mutex"); \
    return ret_val; \
  }

#endif
