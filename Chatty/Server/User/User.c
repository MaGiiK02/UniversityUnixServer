#include "User.h"
#include "../../List/list.h"
#include "../../Error/Error.h"
#include "../../Message/message.h"
#include "../ServerGlobalData.h"
#include "../ServerMessages/ServerMessages.h"


User* User_new(char* name,int historySize){
  User* u = malloc(sizeof(User));
  strcpy(u->name,name);
  u->fd = -1;
  u->online = false;
  u->max_history_size = historySize;
  u->msg_history = NewMessageList();
  return u;
}

void User_destroy(User* u){
  List_destroy(u->msg_history);
  FREE(u);
}

void User_PushHistory(User* u,message_t* msg){
  if(List_length(u->msg_history)<= u->max_history_size){
    List_prepend(u->msg_history,msg);
  }else{
    List_drop_tail(u->msg_history);
    List_prepend(u->msg_history,msg);
  }
  #ifdef DEBUG
  ListNode* n = u->msg_history->head;
  printf("USER(%s) messages list:\n",u->name);
  while(n){
    message_t* msg = (message_t*) n->data;
    printf("    ",msg->data.buf);
    n = n->next;
  }
  #endif
}

HashTableSync* User_NewHashTable(unsigned int size){

  return HashSync_new(size, sizeof(User),&User_Free,&User_copy);
}

void User_copy(void* dst, void* src){
  memcpy(dst,src, sizeof(User));
}

void User_Free(void* el){
  User* usr = (User*)el;
  List_destroy(usr->msg_history);
  FREE(usr);
}

void User_set_online(User* u,int fd){
  u->fd = fd;
  if(!u->online){
    u->online = true;
    StatsIncNOnline_S();
  }
}

void User_set_offline(User* u){
  u->fd = -1;
  if(u->online){
    u->online = false;
    StatsDecNOnline_S();
  }
}
