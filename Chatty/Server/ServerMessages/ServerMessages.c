//
// Created by Mattia Angelini on 03/04/2018.
//


#include <stdlib.h>

#include "ServerMessages.h"
#include "../../MemoryHelper/MemoryHelper.h"

List* NewMessageList(){
  return List_new(sizeof(message_t),Message_free,Message_cmp,Message_copy);
}

void Message_free(void* el){
  message_t* msg = (message_t*) el;
  FREE(msg->data.buf);
  FREE(msg);
}

void Message_copy(void*dst,void* src){
  message_t* source =(message_t*) src;
  message_t* destination=(message_t*)dst;
  memcpy(destination,source, sizeof(message_t));
  if(source->data.hdr.len > 0){
    destination->data.buf = calloc(source->data.hdr.len, sizeof(char));
    strncpy(destination->data.buf ,source->data.buf, source->data.hdr.len);
  }else{
    destination->data.buf = NULL;
  }
}

int Message_cmp(void* el1,void* el2){
  message_t* msg1 = (message_t*)el1;
  message_t* msg2 = (message_t*)el2;

  if(
          msg1->hdr.op != msg2->hdr.op ||
          strcmp(msg1->hdr.sender,msg2->hdr.sender) != 0 ||
          msg1->data.hdr.len != msg2->data.hdr.len ||
          strcmp(msg1->data.hdr.receiver,msg2->data.hdr.receiver) != 0 ||
          memcmp(msg1->data.buf,msg2->data.buf,msg1->data.hdr.len)
          ){
    return 1;
  }
  return 0;
}

message_t* Message_build(int operation,char* sender,char* reciver,char* buf,int size){
  message_t* msg = calloc(sizeof(message_t),1);
  setHeader(&(msg->hdr),operation,sender);
  char* buffer_copy = calloc(size,sizeof(char));
  memcpy(buffer_copy,buf,size);
  setData(&(msg->data),reciver,buffer_copy,size);//WARNING: Can generate problem with bad formatted strings
  return msg;
}
message_t*  Message_build_no_copy(int operation,char* sender,char* reciver,char* buf,int size){
  message_t* msg = calloc(sizeof(message_t),1);
  setHeader(&(msg->hdr),operation,sender);
  setData(&(msg->data),reciver,buf,size);//WARNING: Can generate problem with bad formatted strings
  return msg;
}

message_t* Message_build_simple_ack(int operation,char* sender){
  message_t* msg = calloc(sizeof(message_t),1);
  setHeader(&(msg->hdr),operation,sender);
  setData(&(msg->data),"",NULL,0);//WARNING: Can generate problem with bad formatted strings
  return msg;
}
