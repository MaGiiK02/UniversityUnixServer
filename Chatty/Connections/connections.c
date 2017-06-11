#include <sys/types.h>
#include <sys/socket.h>

#include "connections.h"

int readHeader(long connfd, message_hdr_t *hdr){

}

int readData(long fd, message_data_t *data){

}

int readMsg(long fd, message_t *msg){

}

int sendRequest(long fd, message_t *msg){
    
    size_t missing = sizeof(message_t);
    message_t * buffer = (message_t*)msg;

    while(missing>0) /* While all the message isn't write */
        if((r = write((int) fd,buffer, missing))== -1){
            if(errno == EINTR)continue;
            return -1;
        }

        if(r == 0) return 0;

        missing -= r; /* */
        buffer +=r;
    }
    return 1;
}