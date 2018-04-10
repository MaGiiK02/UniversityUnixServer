//
// Created by Mattia Angelini on 03/04/2018.
//

#ifndef SOL_2017_SERVERMESSAGES_H
#define SOL_2017_SERVERMESSAGES_H


#include "../../List/list.h"
#include "../../Message/message.h"

/** LIST FUNCTIONS **/

List* NewMessageList();
void Message_free(void* el);
void Message_copy(void* dst,void* src);
int Message_cmp(void* el1,void* el2);
message_t* Message_build(int operation,char* sender,char* reciver,char* buf,int size);
message_t*  Message_build_no_copy(int operation,char* sender,char* reciver,char* buf,int size);
message_t* Message_build_simple_ack(int operation,char* sender);


#endif //SOL_2017_SERVERMESSAGES_H
