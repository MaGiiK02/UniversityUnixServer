//
// Created by Mattia Angelini on 23/02/2018.
//

#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include "ServerGlobalData.h"

HashTableSync* GD_ServerGroup;
HashTableSync* GD_ServerUsers;
Settings* GD_ServerSetting;
fd_set GD_FdSetRead;
Ch* GD_WorkerCommunicationChannel;
pthread_t** GD_Workers;
pthread_mutex_t GD_MU_FdSetRead;
pid_t GD_MainThread;
pthread_mutex_t GD_MU_Fd_Username;
char GD_Fd_Username[256][MAX_NAME_LENGTH];

bool Data_put_in_readSet_S(int fd){
  LOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  bool ris=false;
  if(!FD_ISSET(fd,&GD_FdSetRead)){
    FD_SET(fd,&GD_FdSetRead);
    ris=true;
  }
  UNLOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  return ris;
}

bool Data_remove_from_readSet_S(int fd){
  LOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  bool ris=false;
  if(FD_ISSET(fd,&GD_FdSetRead)){
    FD_CLR(fd,&GD_FdSetRead);
    ris = true;
  }
  UNLOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  return ris;
}

bool Data_is_in_readSet_S(int fd){
  LOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  bool ris;
  ris = FD_ISSET(fd,&GD_FdSetRead);
  UNLOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  return ris;
}

void Data_copy_readSet_S(fd_set* set){
  LOCK_MUTEX_EXIT(GD_MU_FdSetRead);
  memcpy(set,&GD_FdSetRead, sizeof(fd_set));
  UNLOCK_MUTEX_EXIT(GD_MU_FdSetRead);
}

void Data_set_name_for_fd_S(int fd,char* name){
  if(fd<0 || fd>255) return;
  LOCK_MUTEX_EXIT(GD_MU_Fd_Username);
  strcpy(GD_Fd_Username[fd],name);
  UNLOCK_MUTEX_EXIT(GD_MU_Fd_Username);
}
void Data_clear_name_for_fd_S(int fd){
  if(fd<0 || fd>255) return;
  LOCK_MUTEX_EXIT(GD_MU_Fd_Username);
  memset(GD_Fd_Username,0,MAX_NAME_LENGTH * sizeof(char));
  UNLOCK_MUTEX_EXIT(GD_MU_Fd_Username);
}

void Data_get_name_for_fd_S(int fd,char* name_out){
  if(fd<0 || fd>255){
    name_out = NULL;
    return;
  }
  LOCK_MUTEX_EXIT(GD_MU_Fd_Username);
  strcpy(name_out,GD_Fd_Username[fd]);
  UNLOCK_MUTEX_EXIT(GD_MU_Fd_Username);
}


