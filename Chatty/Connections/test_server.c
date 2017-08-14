#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>



#include "../Message/message.h"
#include "../Error/Error.h"
#include "connections.h"

#define SOCKET_PATH "/tmp/test-connect"

int main(){

  int socket_fd;
  int client_fd;
  int tmp;
  struct sockaddr_un address;
  struct sockaddr client_address;


  unlink(SOCKET_PATH);

  socket_fd = socket(AF_UNIX,SOCK_STREAM,0);
  if(is_error(socket_fd)){
    perror("Unable to create the socket!");
    exit(EXIT_FAILURE);
  }

  address.sun_family = AF_UNIX;
  strcpy(address.sun_path,SOCKET_PATH);

  tmp = bind(socket_fd,(struct sockaddr *) &address,sizeof(address));
  if(is_error(tmp)){
    perror("Unable to bind");
    exit(EXIT_FAILURE);
  }

  tmp = listen(socket_fd,1);
  if(is_error(tmp)){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /*test read */
  socklen_t client_address_len;
  client_fd = accept(
    socket_fd,
    (struct sockaddr *) &client_address,
    &client_address_len);
  if(is_error(client_fd)){
    perror("Accept");
    exit(EXIT_FAILURE);
  }

  message_t* recived_message = malloc(sizeof(message_t));
  tmp = readMsg(client_fd,recived_message);

  printMessage(recived_message);

  message_t* to_send_message = malloc(sizeof(message_t));
  setHeader(&to_send_message->hdr,100,"Server l'oscuro!");
  char msg[20] = "Si gelatooo!!!";
  setData(&to_send_message->data, "Il suddito dell'oscuro!",msg, sizeof(msg));

  sendRequest(client_fd,to_send_message);
  sendData(client_fd,&(to_send_message->data));

  free(to_send_message);
  free(recived_message);

  exit(EXIT_SUCCESS);
}
