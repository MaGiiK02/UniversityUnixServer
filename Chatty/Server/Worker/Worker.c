//
// Created by Mattia Angelini on 09/01/2018.
//

#include "Worker.h"
#include "../User/User.h"
#include "../SocketSync/SocketSync.h"
#include "../ServerOperations/ServerOperations.h"
#include "../ServerMessages/ServerMessages.h"
#include <signal.h>

int _manageRequest(int clientFd,message_hdr_t* request);

int Worker_Start_new(pthread_t *thread){
  return pthread_create(thread,NULL,Worker_function,NULL);
}

void* Worker_function(void *arg){

  //SETTING SIGNAL FOR THREADS
  sigset_t blockedSet;
  sigemptyset(&blockedSet);
  sigaddset(&blockedSet,SIGUSR1);
  sigaddset(&blockedSet,SIGUSR2);
  if(pthread_sigmask(SIG_BLOCK,&blockedSet,NULL)!=0){
    perror("Registering child sigmask");
    pthread_exit(NULL);
  }

  sigignore(SIGPIPE);

  int fd,ris;
  User* u;
  message_t* msg;
  message_hdr_t request;
  while(Ch_Pop_S(GD_WorkerCommunicationChannel,(void*)&fd) == 0){ //passive wait inside the pop Fn
    if(fd<=0) continue;
    memset(&request,0, sizeof(message_hdr_t));
    if(readHeader(fd,&request) <= 0){
      perror("error readHeader");
      //then i can't write back to te client and i don't even bother abut manage the request
      continue;
    }

    switch (ris=_manageRequest(fd,&request)){
      case OP_OK:
        Data_put_in_readSet_S(fd); //put back the fd in the reading set
        kill(GD_MainThread,SIGUSR2); // notify restoring of an FD
        break;
      case OP_BROKEN_CONN:
        HashSync_lock_by_key(GD_ServerUsers,request.sender);
        u = HashSync_get_element_pointer(GD_ServerUsers,request.sender);
        u->online = false;
        u->fd = -1;
        HashSync_unlock_by_key(GD_ServerUsers,request.sender);
        break;
      default:
        //If an error happen this part of code have the purpose to notify the client
        StatsIncNErrors();
        msg = Message_build_simple_ack(ris,SERVER_SENDER_NAME);
        SockSync_send_message_SS(fd,msg);

        HashSync_lock_by_key(GD_ServerUsers,request.sender);
        if((u = HashSync_get_element_pointer(GD_ServerUsers,request.sender))) {
          //if it's not null
          u->online = false;
          u->fd = -1;
        }
        HashSync_unlock_by_key(GD_ServerUsers,request.sender);

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
  if(request->op == REGISTER_OP){
    //is the only operation that don't have need about an user
    result = OP_register(clientFd,request);
  }else{
    if(readData(clientFd,&data) <= 0){
      return OP_BROKEN_CONN;
    }
    usr = HashSync_get_element_pointer(GD_ServerUsers,request->sender);
    if(usr != NULL){
      result = OP_manage(clientFd,usr,request,&data);
    }else{
      result = OP_NICK_UNKNOWN;
    }
  }
  return result;
}
