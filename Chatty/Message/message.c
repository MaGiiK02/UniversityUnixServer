#import "message.h"


List* NewMessageList(){
  return List_new(sizeof(message_t),Message_free,Message_cmp);
}

void Message_free(void* el){
  message_t* msg = (message_t*) el;
  free(msg->data.buf);
  free(msg);
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