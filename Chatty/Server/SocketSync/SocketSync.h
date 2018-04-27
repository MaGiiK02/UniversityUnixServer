//
// Created by Mattia Angelini on 21/02/2018.
//

#ifndef SOL_2017_SOCKETSYNC_H
#define SOL_2017_SOCKETSYNC_H

#ifndef __unix__
#include <ntsid.h>
#endif

#include <stdlib.h>
#include <pthread.h>

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
int SockSync_close_SS(long fd);
  


#endif //SOL_2017_SOCKETSYNC_H
