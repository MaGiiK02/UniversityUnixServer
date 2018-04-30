#include "User.h"
#include "../../List/list.h"
#include "../../Error/Error.h"
#include "../../Message/message.h"
#include "../ServerGlobalData.h"
#include "../ServerMessages/ServerMessages.h"
#include "../../Debugger/Debugger.h"


User* User_new(char* name,int historySize){
  User* u = malloc(sizeof(User));
  strcpy(u->name,name);
  u->fd = -1;
  u->online = false;
  u->max_history_size = historySize;
  u->msg_history = NewMessageList();
  return u;
}

void User_PushHistory(User* u,message_t* msg){
  if(List_length(u->msg_history)<= u->max_history_size){
    List_prepend(u->msg_history,msg);
  }else{
    List_drop_tail(u->msg_history);
    List_prepend(u->msg_history,msg);
  }
}

HashTableSync* User_NewHashTable(unsigned int size){

  return HashSync_new(size, sizeof(User),&User_Free,&User_copy);
}

void User_copy(void* dst, void* src){
  if(dst == src)return;
  memcpy(dst,src, sizeof(User));
}

void User_Free(void* el){
  User* usr = (User*)el;
  List_destroy(usr->msg_history);
  FREE(usr);
}

void User_shallow_Free(void* el){
  User* usr = (User*)el;
  FREE(usr);
}

void User_set_online(User* u,int fd){
  char tmp_name[MAX_NAME_LENGTH];
  memset(tmp_name,0,MAX_NAME_LENGTH * sizeof(char));
  ON_DEBUG(printf("CONNECTING %s[%d--->%d]\n",u->name,u->fd,fd);)
  if(u->fd > 0) {
    Data_get_name_for_fd_S(u->fd,tmp_name);
    if(strcmp(u->name,tmp_name) == 0 || strlen(tmp_name)==0){
      SockSync_close_SS(u->fd);
    }
  }
  u->fd = fd;
  Data_set_name_for_fd_S(fd,u->name);
  if(!u->online){
    u->online = true;
    StatsIncNOnline_S();
  }
}

void User_set_offline(User* u){
  if(u->fd > 0) {
    SockSync_close_SS(u->fd);
    Data_clear_name_for_fd_S(u->fd);
    u->fd = -1;
  }
  if(u->online){
    u->online = false;
    StatsDecNOnline_S();
  }
}

void User_set_offline_leave_sock(User* u){
  if(u->fd>0) {
    u->fd = -1;
  }
  if(u->online){
    u->online = false;
    StatsDecNOnline_S();
  }
}

