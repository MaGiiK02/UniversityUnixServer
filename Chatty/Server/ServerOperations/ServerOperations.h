//
// Created by Mattia Angelini on 06/03/2018.
//

#ifndef SOL_2017_SERVEROPERATIONS_H
#define SOL_2017_SERVEROPERATIONS_H

/*
* @brief All the operations clients can request to the server,
*       basically all the endpoint the server calls to perform clients requestes.
*/

#include "../User/User.h"
#include "../ServerGlobalData.h"
#include "../SocketSync/SocketSync.h"


int Send_ack_to(int clientFd, int reply_code);

int OP_manage(int clientFd,User* clientUser,message_hdr_t* request,message_data_t* data);

int OP_register(int clientFd,message_hdr_t* request_data);

int OP_connect(int clientFd,User* clientUser, message_hdr_t* request);
int OP_unregister(int clientFd,User* clientUser, message_hdr_t* request);
int OP_disconnect(int clientFd,User* clientUser, message_hdr_t* request);

int OP_usrlist(int clientFd,User* clientUser);
int OP_posttxt(int clientFd,User* clientUser,message_hdr_t* request,message_data_t* request_data);
int OP_posttxtall(int clientFd, message_hdr_t* request,message_data_t* request_data);
int OP_getprevmsgs(int clientFd, User* clientUser, message_hdr_t* request);
int OP_postfile(int clientFd, message_hdr_t* hdr, message_data_t* request_data);
int OP_getfile(int clientFd,message_hdr_t* hdr,message_data_t* data);
int OP_user_online(int clientFd, User* clientUser, message_data_t* request_data);
int OP_creategroup(int clientFd, User* clientUser,message_hdr_t* hdr,message_data_t* request_data);
int OP_addgroup(int clientFd, User* clientUser, message_hdr_t* request, message_data_t* request_data);
int OP_delgroup(int clientFd, User* clientUser, message_hdr_t* request, message_data_t* request_data);
int OP_destroygroup(int clientFd, message_hdr_t* request,message_data_t* request_data);



#endif //SOL_2017_SERVEROPERATIONS_H
