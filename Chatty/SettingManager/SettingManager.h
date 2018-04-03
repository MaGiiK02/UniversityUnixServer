#ifndef _SETTING_MANAGER_H_
#define _SETTING_MANAGER_H_

/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @Brief: This file contains the struct of the setting as well as,
 *          the functions to work with the setting, such as the load setting
 *          and some safe setter and getter for the structure.
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

/* Project Include */
#include "../Booleans/Booleans.h"


#define SETTING_FIELD_NAME_UNIX_PATH "unixpath"
#define SETTING_FIELD_NAME_MAX_CONNECTIONS "maxconnections"
#define SETTING_FIELD_NAME_THREADS_IN_POOL "threadsinpool"
#define SETTING_FIELD_NAME_MAX_MSG_SIZE "maxmsgsize"
#define SETTING_FIELD_NAME_MAX_FILE_SIZE "maxfilesize"
#define SETTING_FIELD_NAME_MAX_HITS_MSG "maxhistmsgs"
#define SETTING_FIELD_NAME_DIR_NAME "dirname"
#define SETTING_FIELD_NAME_STAT_FILE_NAME "statfilename"

#define SETTING_DEFAULT_UNIX_PATH "./tmp/chatty_socket\0"
#define SETTING_DEFAULT_MAX_CONNECTIONS 32
#define SETTING_DEFAULT_THREADS_IN_POOL 8
#define SETTING_DEFAULT_MAX_MSG_SIZE 512
#define SETTING_DEFAULT_MAX_FILE_SIZE 1024
#define SETTING_DEFAULT_MAX_HITS_MSG 16
#define SETTING_DEFAULT_DIR_NAME "./tmp/chatty\0"
#define SETTING_DEFAULT_STAT_FILE_NAME "./tmp/chatty\0"

/* The structure that will be used to store the application settings */
typedef struct {
    char* unixPath;
    unsigned short int maxConnections;
    unsigned short int threadsInPool;
    unsigned short int maxMsgSize;
    unsigned short int maxFileSize;
    unsigned short int maxHistMsgs;
    char* dirName;
    char* statFileName;
} Settings;

/**
 * Create a Settings structure in the heap, and returns it's pointer.
 * @throws ...
 * @return The Settings struct's pointer.
 */
Settings* SettingManager_new_settings_struct();

/**
 * Create a Settings structure in the heap, filling it with the settings default values, and returning it's pointer.
 * @throws ...
 * @return The Settings struct's pointer.
 */
Settings* SettingManager_new_default_settings_struct();

/**
 * Destroy a Settings structure from the heap given it's pointer.
 * NOTE : The function assumes that the inner pointer (strings) can be removed!
 */
void SettingManager_destroy_settings_struct(Settings** settings);

/**
 * Sefe setter for the unixPath in the settings structure, it performs the copy of the value from the parameter.
 */
void SettingManager_settings_set_unix_path (Settings *settings, const char *unix_path);

/**
 * Sefe getter for the unixPath in the settings structure, it performs the copy of the value in the out parameter.
 */
void SettingManager_settings_get_unix_path (Settings *settings,char *out_unix_path);

/**
 * Sefe setter for the dirName in the settings structure, it performs the copy of the value from the parameter.
 */
void SettingManager_settings_set_dir_name (Settings *settings, const char *dir_name);

/**
 * Sefe getter for the dirName in the settings structure, it performs the copy of the value in the out parameter.
 */
void SettingManager_settings_get_dir_name (Settings *settings,char *out_unix_path);

/**
 * Sefe setter for the statFileName in the settings structure, it performs the copy of the value from the parameter.
 */
void SettingManager_settings_set_stat_file_name (Settings *settings, const char *stat_file_name);

/**
 * Sefe getter for the statFileName in the settings structure, it performs the copy of the value in the out parameter.
 */
void SettingManager_settings_get_stat_file_name (Settings *settings,char *out_stat_file_name);

/**
 * Print in the stdout the give Sttings struct
 */
void SettingManager_print_settings_struct(Settings* settings);

/**
 * This function load in a Setting structure, all the settings defined in the file located by the prameter path,
 * if some settings can't be found, the default falue will be used.
 * @param settingFilePath a string with the path to the settings file.
 * @throws ...
 * @return The Settings struct's pointer.
 */
Settings* SettingManager_load_settings_form_file(const char * settingFilePath,bool useDefault);

#endif /*!_SETTING_MANAGER_H_*/
