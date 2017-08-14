#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../Message/message.h"
#include "../Error/Error.h"
#include "connections.h"

#define SOCKET_PATH "/tmp/test-connect"

int main(){
  int server_socket;
  int tmp;
  server_socket = openConnection(SOCKET_PATH,5,10);
  if(is_error(server_socket)){
    perror("openConnect");
    exit(EXIT_FAILURE);
  }


  message_t* to_send_message = malloc(sizeof(message_t));
  setHeader(&to_send_message->hdr,100,"Il suddito dell'oscuro!");
  char msg[50] = "Le ho protato del gelato oscuro signore! <3";
  setData(&to_send_message->data, "Server l'oscuro!",msg, sizeof(msg));
  tmp = sendRequest(server_socket,to_send_message);
  if(is_error(tmp)){
    perror("sendRequest");
    exit(1);
  }
  tmp = sendData(server_socket,&to_send_message->data);
  if(is_error(tmp)){
    perror("sendData");
    exit(1);
  }


  message_t* recived_message = malloc(sizeof(message_t));
  tmp = readMsg(server_socket,recived_message);
  if(is_error(tmp)){
    perror("readMsg");
    exit(1);
  }
  printMessage(recived_message);

}
