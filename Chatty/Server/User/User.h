#ifndef _USER_H_
#define _USER_H_

#include <stdlib.h>

#include "../../config.h"
#include "../../HashTable/HashTableSynchronized.h"
#include "../../Message/message.h"
#include "../../MemoryHelper/MemoryHelper.h"
#include "../../Connections/connections.h"
#include "../../Booleans/Booleans.h"
#include "../../Message/message.h"
#include "../../Sync/Sync.h"
#include "../../Utils/Utils.h"
#include "../Statistic/stats.h"

typedef struct _user{
  char name[MAX_NAME_LENGTH];
  List* msg_history;
  unsigned int max_history_size;
  bool online;
  int fd;
}User;

void User_init_socket_sync(int max_user_count);
void User_free_socket_sync();

User* User_new(char* name,int historySize);

void User_destroy(User* u);

void User_PushHistory(User* u, message_t* msg);

int User_send_history(User* u);

/// User Hash

HashTableSync* User_NewHashTable(unsigned int size);

void User_copy(void* dst, void* src);

void User_Free(void* el);

void User_set_offline_leave_sock(User* u);

void User_set_offline(User* u);
void User_set_online(User* u,int fd);
#endif
