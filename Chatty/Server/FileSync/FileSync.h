//
// Created by Mattia Angelini on 21/02/2018.
//


/*
* @brief A module used to provide syncronization FILES.
        It use a partitioned MutexArray, like the SyncHashTable wrapper using the FDs like index for the array to perform locks.
*/

#ifndef SOL_2017_FILESYNC_H
#define SOL_2017_FILESYNC_H


#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#ifndef __unix__
#include <ntsid.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "../../Booleans/Booleans.h"
#include "../../Message/message.h"
#include "../../Utils/Utils.h"
#include "../../Sync/Sync.h"
#include "../../Connections/connections.h"

void FileSync_init_socket_sync(int max_concurrent_fds);
void FileSync_free_socket_sync();

FILE* FileSync_open(char* path,char* mode);
void FileSync_close(FILE *f);

#endif //SOL_2017_FILESYNC_H
