//
// Created by Mattia Angelini on 21/02/2018.
//
#include "FileSync.h"
#include "../../Debugger/Debugger.h"

pthread_mutex_t* FILE_MUTEX;
int FILE_MUTEX_SIZE=0;
bool IS_FILE_MUTEX_INIT = false;

void FileSync_init_socket_sync(int max_concurrent_file){
  if(!IS_FILE_MUTEX_INIT){
    FILE_MUTEX_SIZE = Utils_calculate_mutex_array_size(max_concurrent_file);
    FILE_MUTEX = malloc(sizeof(pthread_mutex_t)*FILE_MUTEX_SIZE);
    for(int i=0;i<FILE_MUTEX_SIZE;i++) {
      pthread_mutex_init(&(FILE_MUTEX[i]),NULL);
    }
    IS_FILE_MUTEX_INIT = true;
  }
}
void FileSync_free_socket_sync(){
  if(IS_FILE_MUTEX_INIT){
    for(int i=0;i<FILE_MUTEX_SIZE;i++) {
      pthread_mutex_destroy(&(FILE_MUTEX[i]));
    }
    FREE (FILE_MUTEX);
    FILE_MUTEX_SIZE = 0;
    IS_FILE_MUTEX_INIT = false;
  }
}

void _lock_by_fd(int fd){
  int index = fd%FILE_MUTEX_SIZE;
  LOCK_MUTEX_EXIT(FILE_MUTEX[index]);
}

void _unlock_by_fd(int fd){
  int index = fd%FILE_MUTEX_SIZE;
  UNLOCK_MUTEX_EXIT(FILE_MUTEX[index]);
}

FILE* FileSync_open(char* path,char* mode){
  FILE* f = fopen(path,mode);
  if(f == NULL) return NULL;
  _lock_by_fd(fileno(f));
  return f;
}
void FileSync_close(FILE *f){
  if(f == NULL) return;
  int fd = fileno(f);
  fclose(f);
  _unlock_by_fd(fd);
}
