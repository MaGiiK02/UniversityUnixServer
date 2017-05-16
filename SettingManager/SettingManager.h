#ifndef _SETTING_MANAGER_H_
#define _SETTING_MANAGER_H_

/*
 * @Author: angelini.mattia 
 * @StudentCode: 502688
 * @Date: 2017-05-15 07:59:04 
 * @Last Modified by: mattia.angelini
 * @Last Modified time: 2017-05-16 14:52:28
 */

/* @Description: 
 *  This component provide a structure and the system to load the project settings.
 *  Settings list:
 *      -> UnixPath = The path for the AF_UNIX socket.
 *      -> MaxConnections = The max client that the server can handle.
 *      -> ThreadsInPool = The nuber of threads that the server can use.
 *      -> MaxMsgSize = The messages max size accepted from the server.
 *      -> MaxFileSize = The file max size accepted from the server.
 *      -> MaxHistMsgs = The history for each client that the server keeps in memory.
 *      -> DirName = The directory where the server will store client's files.
 *      -> StatFileName = The file where the server will store his statistics.
 */

#include "SettingManagerException.h"

/* Default settings */
#define SETTING_DEFAULT_UNIX_PATH "./tmp/chatty_socket"
#define SETTING_DEFAULT_MAX_CONNECTIONS 32
#define SETTING_DEFAULT_THREADS_IN_POOL 8
#define SETTING_DEFAULT_MAX_MSG_SIZE 512
#define SETTING_DEFAULT_MAX_FILE_SIZE 1024
#define SETTING_DEFAULT_MAX_HITS_MSG 16
#define SETTING_DEFAULT_DIR_NAME "./tmp/chatty"
#define SETTING_DEFAULT_STAT_FILE_NAME "./tmp/chatty"

/* The structure that will be used to store the application settings */
typedef struct {
    char unixPath[256];
    unsigned short int maxConnections;
    unsigned short int threadsInPool;
    unsigned short int maxMsgSize;
    unsigned short int maxFileSize;
    unsigned short int maxHistMsgs;
    char dirName[256];
    char statFileName[256];
} Settings;

/**
 * Create a Settings structure in the heap, and returns it's pointer. 
 * @throws ...
 * @return The Settings struct's pointer.
 */
 
Settings* SettingManager_new_settings_struct();

/**
 * Destroy a Settings structure from the heap given it's pointer.
 * @throws ...
 */
void SettingManager_destroy_settings_struct(Settings* settings);

/**
 * This function load in a Setting structure, all the settings defined in the file located by the prameter path,
 * if some settings can't be found, the default falue will be used. 
 * @param settingFilePath a string with the path to the settings file.
 * @throws ...
 * @return The Settings struct's pointer.
 */
Settings* SettingManager_load_settings_form_file(char settingFilePath[256]);

#endif /*!_SETTING_MANAGER_H_*/

