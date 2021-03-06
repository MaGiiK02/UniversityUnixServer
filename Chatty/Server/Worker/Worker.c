//
// Created by Mattia Angelini on 09/01/2018.
//

#include "Worker.h"
#include "../User/User.h"
#include "../SocketSync/SocketSync.h"
#include "../ServerOperations/ServerOperations.h"
#include "../ServerMessages/ServerMessages.h"
#include "../../Message/message.h"
#include "../../Debugger/Debugger.h"
#include <signal.h>

int _manageRequest(int clientFd,message_hdr_t* request);

int Worker_Start_new(pthread_t *thread){
  return pthread_create(thread,NULL,Worker_function,NULL);
}

void* Worker_function(void *arg){

  //SETTING signal block for thread
  //in order to force th system to deliver them to the main-thread
  sigset_t blockedSet;
  sigemptyset(&blockedSet);
  sigaddset(&blockedSet,SIGUSR1);
  sigaddset(&blockedSet,SIGUSR2);
  sigaddset(&blockedSet,SIGTERM);
  sigaddset(&blockedSet,SIGINT);
  sigaddset(&blockedSet,SIGQUIT);

  if(pthread_sigmask(SIG_BLOCK,&blockedSet,NULL)!=0){
    perror("Registering child sigmask");
    pthread_exit(NULL);
  }

  sigignore(SIGPIPE);

  int fd,ris;
  User* u;
  message_t* msg;
  message_hdr_t request;
  char username_support[MAX_NAME_LENGTH];
  while(Ch_Pop_S(GD_WorkerCommunicationChannel,(void*)&fd) == 0){ //passive wait inside the pop Fn
    if(fd<=0) continue;
    memset(&request,0, sizeof(message_hdr_t));
    request.op = -1;
    if(SockSync_read_header_SS(fd,&request) <= 0){
      ON_DEBUG(perror("error readHeader");)
      //then i can't write back to te client and i don't even bother about manage the request
      #ifdef MAKE_VALGRIND_HAPPY
            memset(username_support,0,MAX_NAME_LENGTH *  sizeof(char));
      #endif
      Data_get_name_for_fd_S(fd,username_support);
      SockSync_close_SS(fd);
      HashSync_lock_by_key(GD_ServerUsers,username_support);
      if ((u = HashSync_get_element_pointer(GD_ServerUsers,username_support))){
        User_set_offline_leave_sock(u);
      }
      HashSync_unlock_by_key(GD_ServerUsers,username_support);
      continue;
    }

    switch (ris=_manageRequest(fd,&request)){
      case OP_OK:
        if(request.op != UNREGISTER_OP && request.op != DISCONNECT_OP){
          Data_put_in_readSet_S(fd); //put back the fd in the reading set
          kill(GD_MainThread,SIGUSR2); // notify restoring of an FD
        }
        break;
      case OP_BROKEN_CONN:
        HashSync_lock_by_key(GD_ServerUsers,request.sender);
        if ((u = HashSync_get_element_pointer(GD_ServerUsers,request.sender))){
          User_set_offline_leave_sock(u);
        }
        HashSync_unlock_by_key(GD_ServerUsers,request.sender);
        SockSync_close_SS(fd);
        break;
      default:
        //If an error happen this part of code have the purpose to notify the client
        if(ris!= OP_FAIL) StatsIncNErrors();
        msg = Message_build_simple_ack(ris,SERVER_SENDER_NAME);
        if(SockSync_send_message_SS(fd,msg) != 0) {
          // reply error on the socket
          HashSync_lock_by_key(GD_ServerUsers, request.sender);
          if ((u = HashSync_get_element_pointer(GD_ServerUsers, request.sender))) {
            User_set_offline_leave_sock(u);
          }
          HashSync_unlock_by_key(GD_ServerUsers, request.sender);
          SockSync_close_SS(fd);
        }

        Message_free(msg);
        break;
    }
  }
  pthread_exit(NULL);
}

int _manageRequest(int clientFd,message_hdr_t* request){
  message_data_t data;
  User* usr;
  int result = 0;
  data.buf = NULL;
  if(request->op == REGISTER_OP){
    //is the only operation that don't have need about an user
    result = OP_register(clientFd,request);
  }else{
    if(SockSync_read_data_SS(clientFd,&data) <= 0){
      return OP_BROKEN_CONN;
    }
    usr = HashSync_get_element_pointer_S(GD_ServerUsers,request->sender);
    if(usr != NULL){
      result = OP_manage(clientFd,usr,request,&data);
    }else{
      result = OP_NICK_UNKNOWN;
    }
    FREE(data.buf)
  }
  return result;
}
