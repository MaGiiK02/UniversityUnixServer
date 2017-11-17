#import "User.h"

HashTable* User_NewHashTable(unsigned int size){
  return Hash_new(size,User_Free);
}

void User_Free(void* el){
  User* usr = (User*)el;
  int i;
  for(i=0; i<usr->history_size; i++){
    free(usr->msg_history[i]);
  }
  free(usr->msg_history);
  free(usr);
}

