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
pthread_t* GD_Workers;
pthread_mutex_t GD_MU_FdSetRead;
pthread_mutex_t GD_MU_OnlineUsers;

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


