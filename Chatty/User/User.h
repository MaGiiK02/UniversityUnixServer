#ifndef _USER_H_
#define _USER_H_

#include "../config.h"
#include "../HashTable/HashTable.h"
#include "../Message/message.h"

typedef struct _user{
  char name[MAX_NAME_LENGTH];
  message_t** msg_history;
  unsigned int history_size;
}User;

int User_PushHistory(message_t* msg);

int User_PopHistory();

/// User Hash

HashTable* User_NewHashTable(unsigned int size);

void User_Free(void* el);

#endif
