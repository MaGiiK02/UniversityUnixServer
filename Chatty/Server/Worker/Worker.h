//
// Created by Mattia Angelini on 09/01/2018.
//

#ifndef SOL_2017_WORKER_H
#define SOL_2017_WORKER_H

#ifndef _POSIX_C_SOURCE
// For signals
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _XOPEN_SOURCE
// For sigignore
#define _XOPEN_SOURCE 500
#endif

#include <errno.h>
#include <pthread.h>
#include <sys/select.h>
#include <limits.h>
#include <string.h>

#include "../../Booleans/Booleans.h"
#include "../../Sync/Sync.h"
#include "../../Channel/Channel.h"
#include "../../Message/message.h"
#include "../../Connections/connections.h"
#include "../../Operations/ops.h"
#include "../ServerGlobalData.h"

int Worker_Start_new(pthread_t* thread); //return the worker pid

void* Worker_function(void *arg);

#endif //SOL_2017_WORKER_H
