//
// Created by Mattia Angelini on 21/02/2018.
//
#include "SocketSync.h"
#include "../../Debugger/Debugger.h"

pthread_mutex_t* SOCKET_MUTEX_WRITE;
pthread_mutex_t* SOCKET_MUTEX_READ;
int SOCK_MUTEX_SIZE=0;
bool IS_SOCKET_MUTEX_INIT = false;

void SockSync_init_socket_sync(int max_concurrent_fds){
  if(!IS_SOCKET_MUTEX_INIT){
    SOCK_MUTEX_SIZE = Utils_calculate_mutex_array_size(max_concurrent_fds);
    SOCKET_MUTEX_READ = malloc(sizeof(pthread_mutex_t)*SOCK_MUTEX_SIZE);
    SOCKET_MUTEX_WRITE = malloc(sizeof(pthread_mutex_t)*SOCK_MUTEX_SIZE);
    for(int i=0;i<SOCK_MUTEX_SIZE;i++) {
      pthread_mutex_init(&(SOCKET_MUTEX_READ[i]),NULL);
      pthread_mutex_init(&(SOCKET_MUTEX_WRITE[i]),NULL);
    }
    IS_SOCKET_MUTEX_INIT = true;
  }
}
void SockSync_free_socket_sync(){
  if(IS_SOCKET_MUTEX_INIT){
    for(int i=0;i<SOCK_MUTEX_SIZE;i++) {
      pthread_mutex_destroy(&(SOCKET_MUTEX_READ[i]));
      pthread_mutex_destroy(&(SOCKET_MUTEX_WRITE[i]));
    }
    FREE (SOCKET_MUTEX_READ);
    FREE (SOCKET_MUTEX_WRITE);
    SOCK_MUTEX_SIZE = 0;
    IS_SOCKET_MUTEX_INIT = false;
  }
}

void SockSync_lock_write_by_fd(int fd){
  int index = fd%SOCK_MUTEX_SIZE;
  LOCK_MUTEX_EXIT(SOCKET_MUTEX_WRITE[index]);
}

void SockSync_unlock_write_by_fd(int fd){
  int index = fd%SOCK_MUTEX_SIZE;
  UNLOCK_MUTEX_EXIT(SOCKET_MUTEX_WRITE[index]);
}

void SockSync_lock_read_by_fd(int fd){
  int index = fd%SOCK_MUTEX_SIZE;
  LOCK_MUTEX_EXIT(SOCKET_MUTEX_READ[index]);
}

void SockSync_unlock_read_by_fd(int fd){
  int index = fd%SOCK_MUTEX_SIZE;
  UNLOCK_MUTEX_EXIT(SOCKET_MUTEX_READ[index]);
}

int SockSync_send_message_SS(long fd, message_t* msg){
  SockSync_lock_write_by_fd(fd);
  int result = sendRequest(fd,msg);
  SockSync_unlock_write_by_fd(fd);
  return result;
}

int SockSync_send_header_SS(long fd, message_hdr_t* hdr){
  SockSync_lock_write_by_fd(fd);
  int result = sendHeader(fd,hdr);
  SockSync_unlock_write_by_fd(fd);
  return result;
}

int SockSync_send_data_SS(long fd, message_data_t* data){
  SockSync_lock_write_by_fd(fd);
  int result = sendData(fd,data);
  SockSync_unlock_write_by_fd(fd);
  return result;
}

int SockSync_read_header_SS(long fd, message_hdr_t* hdr_out){
  SockSync_lock_read_by_fd(fd);
  int result = readHeader(fd,hdr_out);
  SockSync_unlock_read_by_fd(fd);
  return result;
}

int SockSync_read_data_SS(long fd, message_data_t* hdr_out){
  SockSync_lock_read_by_fd(fd);
  int result = readData(fd,hdr_out);
  SockSync_unlock_read_by_fd(fd);
  return result;
}

int SockSync_close_SS(long fd){
  SockSync_lock_write_by_fd(fd);
  SockSync_lock_read_by_fd(fd);
  ON_DEBUG(printf("CLOSING FD:[%ld]\n",fd);)
  int result = close(fd);
  SockSync_unlock_read_by_fd(fd);
  SockSync_unlock_write_by_fd(fd);
  return result;
}

