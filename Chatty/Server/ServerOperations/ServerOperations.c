//
// Created by Mattia Angelini on 06/03/2018.
//

#include "../User/User.h"
#include "ServerOperations.h"
#include "../ServerMessages/ServerMessages.h"
#include "../../SettingManager/SettingManager.h"
#include "../../Message/message.h"


int Send_ack_to(int clientFd, int reply_code){
  message_hdr_t reply = { 0 }; // C99 (set all the struct to zero)
  strcpy(reply.sender,SERVER_SENDER_NAME);
  reply.op = reply_code;
  if(SockSync_send_header_SS(clientFd,&reply) != 0){
    return OP_BROKEN_CONN;
  }
  return OP_OK;
}

void _send_message_to_user(User* to_user,User* from_user, char* text){
  message_t* msg = Message_build(TXT_MESSAGE,from_user->name,from_user->name,text,strlen(text));
  StatsIncNNotDelivered_S();
  if(to_user->online){
    if(SockSync_send_message_SS(to_user->fd,msg)!=0) {
      User_set_offline(to_user);
    }else{
      StatsDecNNotDelivered_S();
      StatsIncNDelivered_S();
    }
  }
  User_PushHistory(to_user,msg);
  Message_free(msg);
}

void _send_message_to_user_S(User* to_user,User* from_user, char* text){
  HashSync_lock_by_key(GD_ServerUsers,to_user->name);
  _send_message_to_user(to_user,from_user,text);
  HashSync_unlock_by_key(GD_ServerUsers,to_user->name);
}

void _send_file_to_user(User* to_user,User* from_user, char* name){
  message_t* msg = Message_build(FILE_MESSAGE,from_user->name,from_user->name,name,strlen(name));
  StatsIncNNotFileDelivered_S();
  if(to_user->online){
    if(SockSync_send_message_SS(to_user->fd,msg)!=0){
      //error delivering the message
      User_set_offline(to_user);
    }else{
      StatsDecNNotFileDelivered_S();
      StatsIncNFileDelivered_S();
    }
  }
  User_PushHistory(to_user,msg);
  Message_free(msg);
}

void _send_file_to_user_S(User* to_user,User* from_user, char* name){
  HashSync_lock_by_key(GD_ServerUsers,to_user->name);
  _send_file_to_user(to_user,from_user,name);
  HashSync_unlock_by_key(GD_ServerUsers,to_user->name);
}


int OP_register(int clientFd,message_hdr_t* data){ //when the user is just registered is seen as online until the connection ends
  if(strcmp(data->sender,"")==0) return OP_FAIL; //names can't be empty
  if(HashSync_get_element_pointer(GD_ServerUsers,data->sender) != NULL){
    return OP_NICK_ALREADY;
  }
  User* u =User_new(data->sender,GD_ServerSetting->maxHistMsgs);
  StatsIncNUser_S();
  User_set_online(u,clientFd);
  HashSync_add_element_S(GD_ServerUsers,data->sender,(void*)u); //perform a shallow copy of u
  int result = OP_usrlist(clientFd,u);
  FREE(u);
  return result;
}

int OP_connect(int clientFd,User* clientUser){
  HashSync_lock_by_key(GD_ServerUsers,clientUser->name);
  User_set_online(clientUser,clientFd);
  HashSync_unlock_by_key(GD_ServerUsers,clientUser->name);
  return OP_usrlist(clientFd, clientUser);
}

int OP_unregister(int clientFd,User* clientUser) {
  if (clientUser->online) {
    HashSync_lock_by_key(GD_ServerUsers, clientUser->name);
    User_set_offline(clientUser);
    HashSync_unlock_by_key(GD_ServerUsers, clientUser->name);
  }

  StatsDecNUser_S();
  HashSync_destroy_element_S(GD_ServerUsers,clientUser->name);
  return Send_ack_to(clientFd,OP_OK);
}

int OP_disconnect(int clientFd,User* clientUser){
  HashSync_lock_by_key(GD_ServerUsers,clientUser->name);
  User_set_offline(clientUser);
  HashSync_unlock_by_key(GD_ServerUsers,clientUser->name);
  return Send_ack_to(clientFd,OP_OK);
}

int OP_usrlist(int clientFd,User* clientUser){
  List* l;
  ListNode* ln;
  int count=0;
  char* user_list= malloc(0);
  for(int i=0;i<GD_ServerUsers->hashTable->size;i++){
    if((l=GD_ServerUsers->hashTable->array[i])!=NULL){
      HashSync_lock_by_index(GD_ServerUsers,i);
      ln = l->head;
      while(ln) {
        User *u = (User *) ((HashElement*)ln->data)->value;
        if (u->online) {
          user_list = realloc(user_list, (count+1) * (MAX_NAME_LENGTH + 1));
          memset(user_list+(count) * (MAX_NAME_LENGTH + 1),0,(MAX_NAME_LENGTH + 1));
          strcpy(user_list+(count * (MAX_NAME_LENGTH + 1)), u->name);
          count++;
        }
        ln=ln->next;
      }
      HashSync_unlock_by_index(GD_ServerUsers,i);
    }
  }
  message_t* msg = Message_build(OP_OK,SERVER_SENDER_NAME,clientUser->name,user_list,count*(MAX_NAME_LENGTH+1));
  int result = SockSync_send_message_SS(clientFd,msg) != 0 ? OP_BROKEN_CONN : OP_OK;
  Message_free(msg);
  FREE(user_list);
  return result;
}

int OP_posttxt(int clientFd, User* clientUser, message_data_t* data){
  if(strcmp(data->hdr.receiver,clientUser->name)==0) return OP_FAIL;
  if(data->hdr.len > GD_ServerSetting->maxMsgSize) return OP_MSG_TOOLONG;
  User* target = HashSync_get_element_pointer(GD_ServerUsers,(data->hdr.receiver));
  if(target == NULL){
    return OP_FAIL;
  }
  _send_message_to_user_S(target,clientUser,data->buf);
  return Send_ack_to(clientFd,OP_OK);
}

int OP_posttxtall(int clientFd,User* clientUser,message_data_t* data){
  if(data->hdr.len > GD_ServerSetting->maxMsgSize) return OP_MSG_TOOLONG;
  for(int i=0; i< GD_ServerUsers->hashTable->size;i++){
    List* l = GD_ServerUsers->hashTable->array[i];
    if(l == NULL) continue;
    HashSync_lock_by_index(GD_ServerUsers,i);
    // Here there are a nested look and it can be risky
    ListNode* el = l->head;
    User* tmp_usr;
    while(el != NULL) {
      tmp_usr = (User *) ((HashElement*)el->data)->value;
      if(strcmp(clientUser->name,tmp_usr->name)!=0) {
        _send_message_to_user(tmp_usr, clientUser, data->buf);//only the S version execute the locks
      }
      el=el->next;
    }
    HashSync_unlock_by_index(GD_ServerUsers,i);
  }
  return Send_ack_to(clientFd,OP_OK);
}

int OP_getprevmsgs(int clientFd,User* clientUser){
  if(Send_ack_to(clientFd,OP_OK)!= OP_OK) return OP_BROKEN_CONN;

  HashSync_lock_by_key(GD_ServerUsers,clientUser->name);
  message_data_t prevMessagesData;
  int buffer[1];
  memset(buffer,0, sizeof(int));
  memcpy(buffer,&(clientUser->msg_history->logicalLength), sizeof(int));
  setData(&prevMessagesData,clientUser->name,(char*)buffer, sizeof(int));
  if(SockSync_send_data_SS(clientFd,&prevMessagesData)!=0){
    return OP_BROKEN_CONN;
  }

  ListNode* el = clientUser->msg_history->tail; //the message are prepended to the list
  message_t* msg;
  while(el != NULL) {
    msg = (message_t*)el->data;
    if(SockSync_send_message_SS(clientFd,msg)!= 0){
      return OP_BROKEN_CONN;
    }
    el=el->prev ;
  }
  HashSync_lock_by_key(GD_ServerUsers,clientUser->name);
  return OP_OK;
}
int OP_postfile(int clientFd,User* clientUser,message_data_t* data){
  message_data_t file_info;

  if(readDataNoBuffer(clientFd,&file_info)!=0){
    return OP_BROKEN_CONN;
  }

  if(file_info.hdr.len >= GD_ServerSetting->maxFileSize*KILOBYTE){
    flushSocket(clientFd,file_info.hdr.len); // remove all socket content
    return OP_MSG_TOOLONG;
  }

  User* target = HashSync_get_element_pointer(GD_ServerUsers,(data->hdr.receiver));
  if(target == NULL){
    return OP_NICK_UNKNOWN;
  }

  char filePath[256];
  Utils_dir_create_if_not_exist(GD_ServerSetting->dirName);
  Utils_build_path(filePath,GD_ServerSetting->dirName,data->buf);
  FILE* destination_file = fopen(filePath,"w+");
  if(destination_file == NULL){
    return OP_FAIL;
  }
  if(dumpBufferOnStream(clientFd,destination_file,file_info.hdr.len)!=0){
    fflush(destination_file);
    fclose(destination_file);
    return OP_FAIL;
  }

  fflush(destination_file);
  fclose(destination_file);

  _send_file_to_user_S(target,clientUser,filePath);
  return Send_ack_to(clientFd,OP_OK);
}
int OP_getfile(int clientFd,User* clientUser,message_data_t* data){
    // READ FILE IF EXIST sed to user with an ack ok!
    FILE* f;
    if(!(f=fopen(data->buf,"r"))){
      return OP_NO_SUCH_FILE;
    }

    long size = Utils_file_size(f);
    char* buffer = malloc(size);
    if(fread(buffer,size,1,f)<=0){
      fflush(f);
      fclose(f);
      perror("error reading the file in OP_getfile");
      return OP_FAIL;
    }
    fflush(f);
    fclose(f);

    message_t* msg = Message_build_no_copy(OP_OK,SERVER_SENDER_NAME,clientUser->name,NULL,0); //evito la copia di tutto il file
    msg->data.hdr.len = size;
    msg->data.buf = buffer;
    if(SockSync_send_message_SS(clientFd,msg) !=0 ){
      Message_free(msg);
      return OP_BROKEN_CONN;
    }
    Message_free(msg); //fa anche la free del buffer

    return OP_OK;
}


int OP_creategroup(int clientFd,User* clientUser,message_data_t* data);
int OP_addgroup(int clientFd,User* clientUser,message_data_t* data);
int OP_delgroup(int clientFd,User* clientUser,message_data_t* data);

int OP_manage(int clientFd,User* clientUser,message_hdr_t* request,message_data_t* data){
  switch (request->op) {

    case UNREGISTER_OP:
      return OP_unregister(clientFd, clientUser);
    case CONNECT_OP:
      return OP_connect(clientFd,clientUser);
    case DISCONNECT_OP:
      return OP_disconnect(clientFd,clientUser);
    case USRLIST_OP:
      return OP_usrlist(clientFd,clientUser);
    case POSTTXT_OP:
      return OP_posttxt(clientFd,clientUser,data);
    case POSTTXTALL_OP:
      return OP_posttxtall(clientFd,clientUser,data);
    case GETPREVMSGS_OP:
      return OP_getprevmsgs(clientFd,clientUser);
    case POSTFILE_OP:
      return OP_postfile(clientFd,clientUser,data);
    case GETFILE_OP:
      return OP_getfile(clientFd,clientUser,data);
    case CREATEGROUP_OP:

    case ADDGROUP_OP:

    case DELGROUP_OP:
      return OP_FAIL; //Not implemented yet!
    default:
      return OP_FAIL;
  }
}
