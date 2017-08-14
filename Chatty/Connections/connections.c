#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Error/Error.h"

#include "connections.h"

int _writeAll(long fd,char* msg,int size);
int _readAll(long fd,char* buffer,int size);

int openConnection(char* path, unsigned int ntimes, unsigned int secs){
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (is_error(socket_fd)){
    perror("Problems with the socket creation");
    return -1;
  }

  struct sockaddr_un address;
  /* avoid problems with strage system adr structure */
  memset(&address, 0, sizeof(struct sockaddr_un));
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, path);

  int connection_result = -1;
  int tries = 0;
  while(tries < ntimes && connection_result == -1){
    connection_result = connect(socket_fd, (struct sockaddr *)&address, sizeof(address));

    if(is_error(connection_result)){
      if(errno != ENOENT){
        perror("Unhandled error in the connection");
        return -1;
      }

      sleep(secs);
    }
    tries ++;
  }

  if (connection_result == -1){
    return -1;
  }
  return socket_fd;
}

int readHeader(long fd, message_hdr_t *hdr){
  assert(hdr != NULL);

  size_t buffer_size = sizeof(message_hdr_t);
  char* buffer = malloc(buffer_size);

  int read_result = read(fd,buffer,buffer_size);
  if(is_error(read_result)){
    perror("Error during socket read");
    return -1;
  }

  memcpy(hdr,buffer,buffer_size);
  free(buffer);

  return 0;
}

int readData(long fd, message_data_t *data){
  assert(data != NULL);

  size_t buffer_size = sizeof(message_data_t);
  char* buffer = malloc(buffer_size);

  int read_result = read(fd,buffer,buffer_size);
  if(is_error(read_result)){
    perror("Error during socket read");
    return -1;
  }

  memcpy(data,buffer,buffer_size);

  buffer_size = data->hdr.len;
  buffer = realloc(buffer,buffer_size);

  read_result = read(fd,buffer,buffer_size);
  if(is_error(read_result)){
    perror("Error during socket read");
    return -1;
  }

  memcpy(data->buf,buffer,buffer_size);
  free (buffer);

  return 0;
}

int readMsg(long fd, message_t *msg){
  assert(msg != NULL);

  int headerResult = readHeader(fd,&msg->hdr);
  if(is_error(headerResult)){
    perror("Error during read message header");
    return -1;
  }

  int headerData = readData(fd,&msg->data);
  if(is_error(headerData)){
    perror("Error during read message data");
    return -1;
  }

  return 0;
}

int sendRequest(long fd, message_t *msg){
    int write_result = sendHeader(fd,&msg->hdr);
    if(is_error(write_result)){
      perror("Error during msg header send");
      return -1;
    }
    return 0;
}

int sendHeader(long fd, message_hdr_t *data){
  int write_result = _writeAll(fd,(char*) data, sizeof(message_hdr_t));
  if(is_error(write_result)){
    perror("Error during msg header send");
    return -1;
  }
  return 0;
}

int sendData(long fd, message_data_t *data){
  int write_result = _writeAll(fd,(char*) data, sizeof(message_data_t));
  if(is_error(write_result)){
    perror("Error during msg header send");
    return -1;
  }

  //Write dataBuffer
  if(data->hdr.len > 0){
    write_result = _writeAll(fd,(char*) data->buf, data->hdr.len);
    if(is_error(write_result)){
      perror("Error during msg header send");
      return -1;
    }
  }

  return 0;
}

int _writeAll(long fd,char* msg,int size){
  int writed = -1;
  while(size > 0){ /* While all the message isn't write */
    writed = write((int) fd, msg, size);
    if(is_error(writed)){
        if(errno != EINTR){
          return -1;
      }
    }

    size -= writed; /* Remove the writed element from the size */
    msg += writed; /* move the buffer poiter after the last byte writed */
  }

  return 0;
}

int _readAll(long fd,char* buffer,int size){
  int readed = -1;
  while(size > 0){ /* While all the message isn't read */
    readed = read((int) fd, buffer, size);
    if(is_error(readed)){
        if(errno != EINTR){
          return -1;
      }
    }

    size -= readed; /* Remove the read element from the size */
    buffer += readed; /* move the buffer poiter after the last byte readed */
  }

  return 0;
}
