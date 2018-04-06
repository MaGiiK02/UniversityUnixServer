//
// Created by Mattia Angelini on 17/01/2018.
//

#ifndef SOL_2017_SERVERGLOBALDATA_H
#define SOL_2017_SERVERGLOBALDATA_H

#include <sys/socket.h>

#include "../HashTable/HashTableSynchronized.h"
#include "../SettingManager/SettingManager.h"
#include "../Channel/Channel.h"
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

extern HashTableSync* GD_ServerGroup;
extern HashTableSync* GD_ServerUsers;
extern Settings* GD_ServerSetting;
extern pthread_mutex_t GD_MU_FdSetRead;
extern fd_set GD_FdSetRead;
extern Ch* GD_WorkerCommunicationChannel;
extern pthread_mutex_t GD_MU_OnlineUsers;
extern pthread_t* GD_Workers;
extern pid_t GD_MainThread;


bool Data_put_in_readSet_S(int fd);
bool Data_remove_from_readSet_S(int fd);
bool Data_is_in_readSet_S(int fd);
void Data_copy_readSet_S(fd_set* set);


#endif //SOL_2017_SERVERGLOBALDATA_H
