//
// Created by Mattia Angelini on 17/01/2018.
//

/*
* @brief The definition of all the global variables of the server, more some syncronization system form them.
*/


#ifndef SOL_2017_SERVERGLOBALDATA_H
#define SOL_2017_SERVERGLOBALDATA_H

#include <sys/socket.h>

#include "../HashTable/HashTableSynchronized.h"
#include "../SettingManager/SettingManager.h"
#include "../Channel/Channel.h"
#include "../config.h"
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

extern HashTableSync* GD_ServerGroup;
extern HashTableSync* GD_ServerUsers;
extern Settings* GD_ServerSetting;
extern pthread_mutex_t GD_MU_FdSetRead;
extern fd_set GD_FdSetRead;
extern Ch* GD_WorkerCommunicationChannel;
extern pthread_t** GD_Workers;
extern pid_t GD_MainThread;
extern pthread_mutex_t GD_MU_Fd_Username;
extern char GD_Fd_Username[256][MAX_NAME_LENGTH];

void Data_set_name_for_fd_S(int fd,char* name);
void Data_clear_name_for_fd_S(int fd);
void Data_get_name_for_fd_S(int fd,char* name_out);
bool Data_put_in_readSet_S(int fd);
bool Data_remove_from_readSet_S(int fd);
bool Data_is_in_readSet_S(int fd);
void Data_copy_readSet_S(fd_set* set);


#endif //SOL_2017_SERVERGLOBALDATA_H
