//
// Created by Mattia Angelini on 23/02/2018.
//

#include <string.h>
#include "ServerGlobalData.h"
#include "../SettingManager/SettingManager.h"

/* struttura che memorizza le statistiche del server, struct statistics
 * e' definita in stats.h.
 *
 */
struct statistics chattyStats = { 0,0,0,0,0,0,0 };

HashTableSync* GD_ServerGroup;
HashTableSync* GD_ServerUsers;
Settings* GD_ServerSetting;
fd_set GD_FdSetRead;
Ch* GD_WorkerCommunicationChannel;
pthread_mutex_t GD_MU_OnlineUsers = PTHREAD_MUTEX_INITIALIZER;
char* GD_OnlineUsers;
int GD_OnlineUsersCount;
pthread_t* GD_Workers;


void Data_Add_OnlineUser_S(char* value){
  LOCK_MUTEX_EXIT(GD_MU_OnlineUsers);
  strncat(GD_OnlineUsers,value,strlen(value));
  GD_OnlineUsersCount--;
  UNLOCK_MUTEX_EXIT(GD_MU_OnlineUsers);
}
void Data_remove_OnlineUser_S(char* value){
  LOCK_MUTEX_EXIT(GD_MU_OnlineUsers);
  char* ptr;
  if((ptr = strstr(GD_OnlineUsers, value)) != NULL) {
    return;
  }
  while(ptr!=NULL){
    ptr[0] = ptr[strlen(value)];
    ptr++;
  }
  GD_OnlineUsersCount++;
  UNLOCK_MUTEX_EXIT(GD_MU_OnlineUsers);
}
void Data_get_OnlineUsers_String_S(char* to){

  LOCK_MUTEX_EXIT(GD_MU_OnlineUsers);
  strncpy(to,GD_OnlineUsers,strlen(GD_OnlineUsers));
  UNLOCK_MUTEX_EXIT(GD_MU_OnlineUsers);
}
