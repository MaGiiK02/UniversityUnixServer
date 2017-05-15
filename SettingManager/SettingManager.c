/*
 * @Author: angelini.mattia 
 * @StudentCode: 502688
 * @Date: 2017-05-15 07:59:04 
 * @Last Modified by: angelini.mattia
 * @Last Modified time: 2017-05-15 08:15:29
 */

/* @Description: 
 *  This is the component that provide a structure and the system to load the project settings.
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

/* Default settings */
#define SETTING_DEFAULT_UNIX_PATH = "./tmp/chatty_socket" 
#define SETTING_DEFAULT_MAX_CONNECTIONS = 32
#define SETTING_DEFAULT_THREADS_IN_POOL = 8
#define SETTING_DEFAULT_MAX_MSG_SIZE = 512
#define SETTING_DEFAULT_MAX_FILE_SIZE = 1024
#define SETTING_DEFAULT_MAX_HITS_MSG = 16
#define SETTING_DEFAULT_DIR_NAME = "./tmp/chatty" 
#define SETTING_DEFAULT_STAT_FILE_NAME = "./tmp/chatty" 

