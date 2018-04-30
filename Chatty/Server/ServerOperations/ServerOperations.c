//
// Created by Mattia Angelini on 06/03/2018.
//

#include "../User/User.h"
#include "ServerOperations.h"
#include "../ServerMessages/ServerMessages.h"
#include "../../SettingManager/SettingManager.h"
#include "../../Message/message.h"
#include "../Group/Group.h"
#include "../../HashTable/HashTableSynchronized.h"
#include "../../HashTable/HashTable.h"
#include "../../List/list.h"
#include "../../Debugger/Debugger.h"
#include "../FileSync/FileSync.h"


int Send_ack_to(int clientFd, int reply_code){
  message_hdr_t reply = { 0 }; // C99 (set all the struct to zero)
  strcpy(reply.sender,SERVER_SENDER_NAME);
  reply.op = reply_code;
  if(SockSync_send_header_SS(clientFd,&reply) != 0){
    return OP_BROKEN_CONN;
  }
  return OP_OK;
}

void _send_message_to_user(User* to_user,char* from_user, char* text, int type){
  message_t* msg = Message_build(type,from_user,to_user->name,text,strlen(text));
  if(type == TXT_MESSAGE){
    StatsIncNNotDelivered_S();
  } else if (type == FILE_MESSAGE){
    StatsIncNNotFileDelivered_S();
  }
  if(to_user->online){
    if(SockSync_send_message_SS(to_user->fd,msg)!=0) {
      User_set_offline(to_user);
    }else{
      if(type == TXT_MESSAGE){
        StatsDecNNotDelivered_S();
        StatsIncNDelivered_S();
      }else if (type == FILE_MESSAGE){
        StatsDecNNotFileDelivered_S();
        StatsIncNFileDelivered_S();
      }
    }
  }
  User_PushHistory(to_user,msg);
  Message_free(msg);
}

void _send_message_to_group(Group* target_group,char* from_user, char* text, int type){
  ListNode* node = target_group->registered_users->head;
  char* user_name;
  User* u = NULL;
  while(node != NULL){
    user_name = (char*) node->data;
    HashSync_lock_by_key(GD_ServerUsers,user_name);
    u = (User*) HashSync_get_element_pointer(GD_ServerUsers,user_name);
    //if(strcmp(u->name,from_user)!=0) { // not the sender of the message
      _send_message_to_user(u, from_user, text,type);
    //}
    HashSync_unlock_by_key(GD_ServerUsers,user_name);
    node = node->next;
  }
}

void _remove_user_from_groups(char* username){
  Group* g;
  ListNode* node;
  for (int i=0;i<GD_ServerGroup->hashTable->size;i++){
    List* l = GD_ServerGroup->hashTable->array[i];
    if(l == NULL) continue;
    HashSync_lock_by_index(GD_ServerGroup,i);
    node = l->head;
    while(node != NULL){
      g = (Group*) ((HashElement*)node->data)->value;
      if(Group_is_founder_by_name(g,username)){
        Hash_destroy_element(GD_ServerGroup->hashTable,g->group_name);
      }else {
        Group_remove_User_by_name(g, username);
      }
      node = node->next;
    }
    HashSync_unlock_by_index(GD_ServerGroup,i);
  }
}

int _dump_socket_on_file(int fdRead,char* filePath,int size){
  FILE* destination_file = FileSync_open(filePath,"w+");
  if(destination_file == NULL){
    ON_DEBUG(perror("opening dump file");)
    flushSocket(fdRead,size);
    return OP_FAIL;
  }

  if(dumpBufferOnStream(fdRead,destination_file,size)!=0){
    fflush(destination_file);
    FileSync_close(destination_file);
    return OP_FAIL;
  }

  fflush(destination_file);
  FileSync_close(destination_file);
  return OP_OK;
}

int OP_register(int clientFd,message_hdr_t* data){ //when the user is just registered is seen as online until the connection ends
  if(strcmp(data->sender,"")==0) return OP_FAIL; //names can't be empty
  if(Group_is_a_valid_groupname(data->sender)){
    //a user name can't contain Group prefix
    return OP_FAIL;
  }
  if(HashSync_get_element_pointer_S(GD_ServerUsers,data->sender) != NULL){
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

int OP_connect(int clientFd,User* clientUser, message_hdr_t* request){
  HashSync_lock_by_key(GD_ServerUsers,request->sender);
  User_set_online(clientUser,clientFd);
  HashSync_unlock_by_key(GD_ServerUsers,request->sender);
  return OP_usrlist(clientFd, clientUser);
}

int OP_unregister(int clientFd,User* clientUser, message_hdr_t* request) {
  if (clientUser->online) {
    HashSync_lock_by_key(GD_ServerUsers, request->sender);
    User_set_offline_leave_sock(clientUser);
    HashSync_unlock_by_key(GD_ServerUsers, request->sender);
  }
  _remove_user_from_groups(request->sender);
  StatsDecNUser_S();
  HashSync_destroy_element_S(GD_ServerUsers,request->sender);
  int result = Send_ack_to(clientFd,OP_OK);
  SockSync_close_SS(clientFd);
  return result;
}

int OP_disconnect(int clientFd,User* clientUser,message_hdr_t* request){
  HashSync_lock_by_key(GD_ServerUsers,request->sender);
  User_set_offline_leave_sock(clientUser);
  HashSync_unlock_by_key(GD_ServerUsers,request->sender);
  int result = Send_ack_to(clientFd,OP_OK);
  SockSync_close_SS(clientFd);
  return result;
}

int OP_usrlist(int clientFd,User* clientUser){
  List* l;
  ListNode* ln;
  int count=0;
  char* user_list= malloc(0);
  for(int i=0;i<GD_ServerUsers->hashTable->size;i++){
    HashSync_lock_by_index(GD_ServerUsers,i);
    if((l=GD_ServerUsers->hashTable->array[i])!=NULL){
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
    }
    HashSync_unlock_by_index(GD_ServerUsers,i);
  }
  message_t* msg = Message_build(OP_OK,SERVER_SENDER_NAME,clientUser->name,user_list,count*(MAX_NAME_LENGTH+1));
  int result = SockSync_send_message_SS(clientFd,msg) != 0 ? OP_BROKEN_CONN : OP_OK;
  Message_free(msg);
  FREE(user_list);
  return result;
}

int OP_posttxt(int clientFd, User* clientUser, message_hdr_t* request, message_data_t* data){

  if(strcmp(data->hdr.receiver,clientUser->name)==0){
    return OP_FAIL;
  }
  if(data->hdr.len > GD_ServerSetting->maxMsgSize) return OP_MSG_TOOLONG;
  if(Group_is_a_valid_groupname(data->hdr.receiver)){
    //Should be a group
    HashSync_lock_by_key(GD_ServerGroup, data->hdr.receiver);
    Group* g = HashSync_get_element_pointer(GD_ServerGroup, data->hdr.receiver);
    if (g == NULL) {
      HashSync_unlock_by_key(GD_ServerGroup, data->hdr.receiver);
      return OP_NICK_UNKNOWN;
    }
    if(!Group_is_registered_by_name(g,request->sender)){
      HashSync_unlock_by_key(GD_ServerGroup,(data->hdr.receiver));
      return OP_NICK_UNKNOWN;
    }
    _send_message_to_group(g, request->sender, data->buf, TXT_MESSAGE);
    HashSync_unlock_by_key(GD_ServerGroup, data->hdr.receiver);
  }else {
    //Should be a user
    HashSync_lock_by_key(GD_ServerUsers, data->hdr.receiver);
    User *target = HashSync_get_element_pointer(GD_ServerUsers, (data->hdr.receiver));
    if (target == NULL) {
      HashSync_unlock_by_key(GD_ServerUsers, data->hdr.receiver);
      return OP_FAIL;
    }
    _send_message_to_user(target, request->sender, data->buf, TXT_MESSAGE);
    HashSync_unlock_by_key(GD_ServerUsers, data->hdr.receiver);
  }
  return Send_ack_to(clientFd,OP_OK);
}

int OP_posttxtall(int clientFd,message_hdr_t* request,message_data_t* data){
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
      if(strcmp(request->sender,tmp_usr->name)!=0) {
        _send_message_to_user(tmp_usr, request->sender, data->buf,TXT_MESSAGE);//only the S version execute the locks
      }
      el=el->next;
    }
    HashSync_unlock_by_index(GD_ServerUsers,i);
  }
  return Send_ack_to(clientFd,OP_OK);
}

int OP_getprevmsgs(int clientFd,User* clientUser,message_hdr_t* request){
  if(Send_ack_to(clientFd,OP_OK)!= OP_OK) return OP_BROKEN_CONN;

  HashSync_lock_by_key(GD_ServerUsers,request->sender);
  message_data_t prevMessagesData;
  int buffer[1];
  buffer[0] = clientUser->msg_history->logicalLength;
  setData(&prevMessagesData,clientUser->name,(char*)buffer, sizeof(int));
  if(SockSync_send_data_SS(clientFd,&prevMessagesData)!=0){
    HashSync_unlock_by_key(GD_ServerUsers,request->sender);
    return OP_BROKEN_CONN;
  }

  ListNode* el = clientUser->msg_history->tail; //the message are prepended to the list
  message_t* msg;
  while(el != NULL) {
    msg = (message_t*)el->data;
    if(SockSync_send_message_SS(clientFd,msg)!= 0){
      HashSync_unlock_by_key(GD_ServerUsers,request->sender);
      return OP_BROKEN_CONN;
    }
    el=el->prev ;
  }
  HashSync_unlock_by_key(GD_ServerUsers,request->sender);
  return OP_OK;
}

int OP_postfile(int clientFd, message_hdr_t* hdr,message_data_t* data){
  message_data_t file_info;
  bool is_group = Group_is_a_valid_groupname(data->hdr.receiver);
  Group* g = NULL;
  User* target = NULL;
  SockSync_lock_read_by_fd(clientFd);
  if(readDataNoBuffer(clientFd,&file_info)!=0){
    SockSync_unlock_read_by_fd(clientFd);
    return OP_BROKEN_CONN;
  }
  SockSync_unlock_read_by_fd(clientFd);

  if(file_info.hdr.len >= GD_ServerSetting->maxFileSize*KILOBYTE){
    SockSync_lock_read_by_fd(clientFd);
    flushSocket(clientFd,file_info.hdr.len); // remove all socket content
    SockSync_unlock_read_by_fd(clientFd);
    return OP_MSG_TOOLONG;
  }

  char filePath[256];
  Utils_dir_create_if_not_exist(GD_ServerSetting->dirName);
  Utils_build_path(filePath,GD_ServerSetting->dirName,data->buf);
  ON_DEBUG(
          printf("try to load the file in %s\n",filePath);
  )
  if(is_group){
    HashSync_lock_by_key(GD_ServerGroup,data->hdr.receiver);

    g = HashSync_get_element_pointer(GD_ServerGroup,data->hdr.receiver);
    if(g == NULL){
      HashSync_unlock_by_key(GD_ServerGroup,(data->hdr.receiver));
      SockSync_lock_read_by_fd(clientFd);
      flushSocket(clientFd,file_info.hdr.len); // remove all socket content
      SockSync_unlock_read_by_fd(clientFd);
      return OP_NICK_UNKNOWN;
    }

    if(!Group_is_registered_by_name(g,hdr->sender)){
      HashSync_unlock_by_key(GD_ServerGroup,(data->hdr.receiver));
      SockSync_lock_read_by_fd(clientFd);
      flushSocket(clientFd,file_info.hdr.len); // remove all socket content
      SockSync_unlock_read_by_fd(clientFd);
      return OP_NICK_UNKNOWN;
    }
    SockSync_lock_read_by_fd(clientFd);
    int dump_ris = _dump_socket_on_file(clientFd,filePath,file_info.hdr.len);
    SockSync_unlock_read_by_fd(clientFd);
    if(dump_ris!= OP_OK){
      HashSync_unlock_by_key(GD_ServerGroup,data->hdr.receiver);
      return  dump_ris;
    }

    _send_message_to_group(g,hdr->sender,filePath,FILE_MESSAGE);

    HashSync_unlock_by_key(GD_ServerGroup,data->hdr.receiver);
  }else{
    HashSync_lock_by_key(GD_ServerUsers,(data->hdr.receiver));

    target = HashSync_get_element_pointer(GD_ServerUsers,data->hdr.receiver);
    if(target == NULL){
      HashSync_unlock_by_key(GD_ServerUsers,(data->hdr.receiver));
      SockSync_lock_read_by_fd(clientFd);
      flushSocket(clientFd,file_info.hdr.len); // remove all socket content
      SockSync_unlock_read_by_fd(clientFd);
      return OP_NICK_UNKNOWN;
    }
    SockSync_lock_read_by_fd(clientFd);
    int dump_ris = _dump_socket_on_file(clientFd,filePath,file_info.hdr.len);
    SockSync_unlock_read_by_fd(clientFd);
    if(dump_ris!= OP_OK){
      HashSync_unlock_by_key(GD_ServerUsers,(data->hdr.receiver));
      return dump_ris;
    }

    _send_message_to_user(target,hdr->sender,filePath,FILE_MESSAGE);

    HashSync_unlock_by_key(GD_ServerUsers,(data->hdr.receiver));
  }
  return Send_ack_to(clientFd,OP_OK);
}

int OP_getfile(int clientFd,message_hdr_t* hdr,message_data_t* data){
    // READ FILE IF EXIST sed to user with an ack ok!
    FILE* f;
    if(!(f=FileSync_open(data->buf,"r"))){
      return OP_NO_SUCH_FILE;
    }

    long size = Utils_file_size(f);
    char* buffer = malloc(size);
    if(fread(buffer,size,1,f)<=0){
      fflush(f);
      FileSync_close(f);
      perror("error reading the file in OP_getfile");
      return OP_FAIL;
    }
    fflush(f);
    FileSync_close(f);

    message_t* msg = Message_build_no_copy(OP_OK,SERVER_SENDER_NAME,hdr->sender,NULL,0); //evito la copia di tutto il file
    msg->data.hdr.len = size;
    msg->data.buf = buffer;
    if(SockSync_send_message_SS(clientFd,msg) !=0 ){
      Message_free(msg);
      return OP_BROKEN_CONN;
    }
    Message_free(msg); //fa anche la free del buffer

    return OP_OK;
}

int OP_user_online(int clientFd,User* clientUser,message_data_t* request_data){
  int buffer[1] = {0};
  buffer[0] = HashSync_is_element_present_S(GD_ServerUsers,request_data->buf);
  message_t* msg = Message_build(OP_OK,SERVER_SENDER_NAME,clientUser->name,(char*) buffer,sizeof(buffer));
  if(SockSync_send_message_SS(clientFd,msg)<=0){
    Message_free(msg);
    return OP_BROKEN_CONN;
  }
  Message_free(msg);
  return OP_OK;
}

int OP_creategroup(int clientFd,User* clientUser,message_hdr_t* request,message_data_t* request_data){
  if(!Group_is_a_valid_groupname(request_data->hdr.receiver)){
    return OP_FAIL;
  }

  if(HashSync_is_element_present_S(GD_ServerGroup,request_data->hdr.receiver)){
    return OP_NICK_ALREADY;
  }

  HashSync_lock_by_key(GD_ServerUsers,request->sender);
  Group* g = Group_new(request_data->hdr.receiver,clientUser);
  HashSync_unlock_by_key(GD_ServerUsers,request->sender);
  HashSync_add_element_S(GD_ServerGroup,request_data->hdr.receiver,g);
  return Send_ack_to(clientFd,OP_OK);
}

int OP_addgroup(int clientFd,User* clientUser, message_hdr_t* request,message_data_t* request_data){
  Group* g = HashSync_get_element_pointer_S(GD_ServerGroup,request_data->hdr.receiver);
  if(g == NULL){
    return OP_NICK_UNKNOWN;
  }

  HashSync_lock_by_key(GD_ServerGroup,request_data->hdr.receiver);
  HashSync_lock_by_key(GD_ServerUsers,request->sender);
  Group_add_User(g,clientUser);
  HashSync_unlock_by_key(GD_ServerUsers,request->sender);
  HashSync_unlock_by_key(GD_ServerGroup,request_data->hdr.receiver);

  return Send_ack_to(clientFd,OP_OK);
}

int OP_delgroup(int clientFd,User* clientUser, message_hdr_t* request,message_data_t* request_data){
  Group* g = HashSync_get_element_pointer_S(GD_ServerGroup,request_data->hdr.receiver);
  if(g == NULL){
    return OP_NICK_UNKNOWN;
  }

  HashSync_lock_by_key(GD_ServerGroup,request_data->hdr.receiver);
  HashSync_lock_by_key(GD_ServerUsers,request->sender);
  Group_remove_User(g,clientUser);
  HashSync_unlock_by_key(GD_ServerUsers,request->sender);
  HashSync_unlock_by_key(GD_ServerGroup,request_data->hdr.receiver);

  return Send_ack_to(clientFd,OP_OK);
}

int OP_destroygroup(int clientFd, message_hdr_t* request,message_data_t* request_data){
  Group* g = HashSync_get_element_pointer_S(GD_ServerGroup,request_data->hdr.receiver);
  if(g == NULL){
    return OP_NICK_UNKNOWN;
  }

  if(strcmp(g->founder,request->sender)!=0){
    return OP_FAIL;
  }
  HashSync_destroy_element_S(GD_ServerGroup,request_data->hdr.receiver);

  return Send_ack_to(clientFd,OP_OK);
}

int OP_manage(int clientFd,User* clientUser,message_hdr_t* request,message_data_t* data){
  switch (request->op) {
    case UNREGISTER_OP:
      return OP_unregister(clientFd, clientUser,request);
    case CONNECT_OP:
      return OP_connect(clientFd,clientUser,request);
    case DISCONNECT_OP:
      return OP_disconnect(clientFd,clientUser,request);
    case USRLIST_OP:
      return OP_usrlist(clientFd,clientUser);
    case POSTTXT_OP:
      return OP_posttxt(clientFd,clientUser,request,data);
    case POSTTXTALL_OP:
      return OP_posttxtall(clientFd, request,data);
    case GETPREVMSGS_OP:
      return OP_getprevmsgs(clientFd, clientUser,request);
    case POSTFILE_OP:
      return OP_postfile(clientFd, request, data);
    case GETFILE_OP:
      return OP_getfile(clientFd , request, data);
    case USERONLINE_OP:
      return  OP_user_online(clientFd, clientUser,data);
    case CREATEGROUP_OP:
      return OP_creategroup(clientFd,clientUser,request,data);
    case ADDGROUP_OP:
      return OP_addgroup(clientFd,clientUser,request,data);
    case DELGROUP_OP:
      return OP_delgroup(clientFd,clientUser,request,data);
    case DESTROYGROUP_OP:
      return OP_destroygroup(clientFd,request,data);
    default:
      return OP_FAIL;
  }
}
