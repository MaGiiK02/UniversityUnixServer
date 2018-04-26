//
// Created by Mattia Angelini on 21/02/2018.
//
#include "SocketSync.h"

pthread_mutex_t* SOCKET_MUTEX;
int SOCK_MUTEX_SIZE=0;
bool IS_SOCKET_MUTEX_INIT = false;

void SockSync_init_socket_sync(int max_concurrent_fds){
  if(!IS_SOCKET_MUTEX_INIT){
    SOCK_MUTEX_SIZE = Utils_calculate_mutex_array_size(max_concurrent_fds);
    SOCKET_MUTEX = malloc(sizeof(pthread_mutex_t)*SOCK_MUTEX_SIZE);
    for(int i=0;i<SOCK_MUTEX_SIZE;i++) {
      pthread_mutex_init(&(SOCKET_MUTEX[i]),NULL);
    }
    IS_SOCKET_MUTEX_INIT = true;
  }
}
void SockSync_free_socket_sync(){
  if(IS_SOCKET_MUTEX_INIT){
    for(int i=0;i<SOCK_MUTEX_SIZE;i++) {
      pthread_mutex_destroy(&(SOCKET_MUTEX[i]));
    }
    FREE (SOCKET_MUTEX);
    SOCK_MUTEX_SIZE = 0;
    IS_SOCKET_MUTEX_INIT = false;
  }
}

void _lock_by_fd(int fd){
  int index = fd%SOCK_MUTEX_SIZE;
  LOCK_MUTEX_EXIT(SOCKET_MUTEX[index]);
}

void _unlock_by_fd(int fd){
  int index = fd%SOCK_MUTEX_SIZE;
  UNLOCK_MUTEX_EXIT(SOCKET_MUTEX[index]);
}

int SockSync_send_message_SS(long fd, message_t* msg){
  _lock_by_fd(fd);
  int result = sendRequest(fd,msg);
  _unlock_by_fd(fd);
  return result;
}

int SockSync_send_header_SS(long fd, message_hdr_t* hdr){
  _lock_by_fd(fd);
  int result = sendHeader(fd,hdr);
  _unlock_by_fd(fd);
  return result;
}

int SockSync_send_data_SS(long fd, message_data_t* data){
  _lock_by_fd(fd);
  int result = sendData(fd,data);
  _unlock_by_fd(fd);
  return result;
}

int SockSync_close_SS(long fd){
  _lock_by_fd(fd);
  int result = close(fd);
  _unlock_by_fd(fd);
  return result;
}

