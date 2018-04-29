//
// Created by Mattia Angelini on 10/09/2017.
//

/*
* @brief A module to manage the User data structure.
*/

#ifndef _USER_H_
#define _USER_H_

#include <stdlib.h>
#include <unistd.h>

#include "../../config.h"
#include "../../HashTable/HashTableSynchronized.h"
#include "../SocketSync/SocketSync.h"
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


User* User_new(char* name,int historySize);

void User_PushHistory(User* u, message_t* msg);

void User_copy(void* dst, void* src);

void User_shallow_Free(void* el);
void User_Free(void* el);

void User_set_offline_leave_sock(User* u);

void User_set_offline(User* u);
void User_set_online(User* u,int fd);


/// User Hash

HashTableSync* User_NewHashTable(unsigned int size);

#endif
