//
// Created by Mattia Angelini on 21/02/2018.
//


/*
* @brief A module used to provide syncronization on sockets FDs.
        It use a partitioned MutexArray, like the SyncHashTable wrapper using the FDs like index for the array to perform locks.
*/

#ifndef SOL_2017_SOCKETSYNC_H
#define SOL_2017_SOCKETSYNC_H

#ifndef __unix__
#include <ntsid.h>
#endif

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../../Booleans/Booleans.h"
#include "../../Message/message.h"
#include "../../Utils/Utils.h"
#include "../../Sync/Sync.h"
#include "../../Connections/connections.h"

void SockSync_init_socket_sync(int max_concurrent_fds);
void SockSync_free_socket_sync();

//Send a message to an FD avoiding a concurrent write on it
int SockSync_send_message_SS(long fd,message_t* msg);
int SockSync_send_header_SS(long fd, message_hdr_t* hdr);
int SockSync_send_data_SS(long fd, message_data_t* data);

int SockSync_read_header_SS(long fd, message_hdr_t* hdr_out);

int SockSync_read_data_SS(long fd, message_data_t* hdr_out);

int SockSync_close_SS(long fd);

void SockSync_lock_write_by_fd(int fd);
void SockSync_unlock_write_by_fd(int fd);
void SockSync_lock_read_by_fd(int fd);
void SockSync_unlock_read_by_fd(int fd);

  


#endif //SOL_2017_SOCKETSYNC_H
