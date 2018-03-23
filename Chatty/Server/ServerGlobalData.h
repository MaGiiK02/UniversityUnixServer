//
// Created by Mattia Angelini on 17/01/2018.
//

#ifndef SOL_2017_SERVERGLOBALDATA_H
#define SOL_2017_SERVERGLOBALDATA_H

#include <sys/socket.h>

#include "../HashTable/HashTableSynchronized.h"
#include "../SettingManager/SettingManager.h"
#include "../Channel/Channel.h"
#include "Statistic/stats.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>

#define DEFAULT_HASH_SIZE 128
#define MAX_NAME_LENGHT 32


void Data_Add_OnlineUser_S(char* value);
void Data_remove_OnlineUser_S(char* value);
void Data_get_copy_OnlineUsers_S(char* to);


#define SERVER_SENDER_NAME "ChattyServer"

#endif //SOL_2017_SERVERGLOBALDATA_H
