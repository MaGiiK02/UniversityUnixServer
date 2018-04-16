/*
 * membox Progetto del corso di LSO 2017
 *
 * Dipartimento di Informatica Università di Pisa
 * Docenti: Prencipe, Torquati
 *
 */
/**
 * @file chatty.c
 * @brief File principale del server chatterbox
 */
#define _POSIX_C_SOURCE 200809L
//For
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>


/* inserire gli altri include che servono */
#include "ServerGlobalData.h"
#include "User/User.h"
#include "SocketSync/SocketSync.h"
#include "Worker/Worker.h"
#include "Statistic/stats.h"
#include "../Debugger/Debugger.h"
#include "../SettingManager/SettingManager.h"
#include "Group/Group.h"


//Functions
int _argsGetStrFromFlag(const char* flag, int argc, char* argv[],char** out_ris);
static void _usagePrint(const char *prog_name);
int _buildSocket(char* socketPath,bool forceBind);
int _listenAndServe(int listenerSocketFd);
void _freeForCh(void* ptr);
int _bootstrapWorkers(int worker_count);
void _closeWorkers(int worker_count);
void _freeStructures();

void _installSignalHandlers(struct sigaction* saInterupt,struct sigaction* saPrint,struct sigaction*saRefresh);
static volatile sig_atomic_t got_SIGTERM = 0;
static volatile sig_atomic_t got_PRINTSTAT = 0;
static volatile sig_atomic_t got_REFRESH_FD= 0;
static void handlerClose(int sig){
    got_SIGTERM = 1;
}

static void handlerRefreshFd(int sig){
    got_REFRESH_FD = 1;
}

static void handlerPrintStats(int sig){
    got_PRINTSTAT = 1;
}

int main(int argc, char *argv[]) {

    int ris;

    char* conf_file_path = NULL;
    ris = _argsGetStrFromFlag("-f",argc,argv,&conf_file_path);
    if(ris != 0){
        fprintf(stderr,"missing config file form the application's args");
        _usagePrint(argv[0]);
        exit(1);
    }
    // DATA INIT
    printf("Initialing server......");
    GD_ServerSetting = SettingManager_load_settings_form_file(conf_file_path,true);
    if(GD_ServerSetting == NULL){
      exit(1);
    }
    GD_ServerUsers = User_NewHashTable(DEFAULT_HASH_SIZE);
    GD_ServerGroup = Group_NewHashTable(DEFAULT_HASH_SIZE);
    printf("End!\n");

    sigignore(SIGPIPE); //Pipe error managed at low level write read operations

    // INSTALL handlers Start
    struct sigaction saInterupt,saPrint,saRefresh; // in order to keep the in the stack
    _installSignalHandlers(&saInterupt,&saPrint,&saRefresh);
    // INSTALL handlers END

    //Initialize a structure that enable to avoid race in write procedures
    pthread_mutex_init(&GD_MU_FdSetRead,NULL);
    SockSync_init_socket_sync(GD_ServerSetting->maxConnections);
    GD_MainThread = getpid();
    GD_WorkerCommunicationChannel = Ch_New(GD_ServerSetting->maxConnections,sizeof(int),_freeForCh);
    if(_bootstrapWorkers(GD_ServerSetting->threadsInPool)!=0){
      perror("unable to start threads");
      exit(1);
    }

    int listenerFD = _buildSocket(GD_ServerSetting->unixPath, true);
    if(listenerFD < 1){
      perror("error in _buildSocket");
      exit(1);
    }
    if(_listenAndServe(listenerFD)!=0){
      exit(1);
    }
    Ch_Close(GD_WorkerCommunicationChannel,true);
    _closeWorkers(GD_ServerSetting->threadsInPool);
    unlink(GD_ServerSetting->unixPath);
    SockSync_free_socket_sync();
    _freeStructures();

    printf("###########################################\n");
    printf("# Closing the application without errors! #\n");
    printf("###########################################\n");
    return 0;
}

static void _usagePrint(const char *prog_name) {
    fprintf(stderr, "Il server va lanciato con il seguente comando:\n");
    fprintf(stderr, "  %s -f conffile\n", prog_name);
}

void _freeForCh(void* ptr){
    int* int_ptr = (int*)ptr;
    FREE(int_ptr)
}

void _installSignalHandlers(struct sigaction* saInterupt,struct sigaction* saPrint,struct sigaction*saRefresh){

  saInterupt->sa_handler = handlerClose;
  sigemptyset(&(saInterupt->sa_mask));
  saInterupt->sa_flags = SA_RESTART;

  if (sigaction(SIGINT, saInterupt, NULL) == -1){
    perror("error adding new signal handler");
    exit(1);
  }
  if (sigaction(SIGTERM, saInterupt, NULL) == -1){
    perror("error adding new signal handler");
    exit(1);
  }
  if (sigaction(SIGQUIT, saInterupt, NULL) == -1){
    perror("error adding new signal handler");
    exit(1);
  }

  saPrint->sa_handler = handlerPrintStats;
  sigemptyset(&(saPrint->sa_mask));
  saPrint->sa_flags = SA_RESTART;
  if (sigaction(SIGUSR1, saPrint, NULL) == -1){
    perror("error adding new signal handler");
    exit(1);
  }

  saRefresh->sa_handler = handlerRefreshFd;
  sigemptyset(&(saRefresh->sa_mask));
  saRefresh->sa_flags = SA_RESTART;
  if (sigaction(SIGUSR2, saRefresh, NULL) == -1){
    perror("error adding new signal handler");
    exit(1);
  }
}

int _argsGetStrFromFlag(const char* flag, int argc, char* argv[],char** out_ris){
    int i;
    for(i=1; i<argc; i++){
        if(strcmp(argv[i],flag) == 0){
            if(i+1>=argc){
                return -1;
            }else{
                *out_ris = argv[i+1];
                return 0;
            }
        }
    }
    return 1;
}

int _bootstrapWorkers(int worker_count){
    int ris;
    GD_Workers = malloc(worker_count * sizeof(pthread_t*));
    for(int i=0;i<worker_count;i++){
      GD_Workers[i] = malloc(sizeof(pthread_t));
        ris = Worker_Start_new((GD_Workers[i]));
        if(ris !=0) return ris;
    }
    return 0;
}

void _closeWorkers(int worker_count){
  for (int i = 0; i < worker_count; i++) {
    pthread_join(*(GD_Workers[i]), NULL);
    FREE(GD_Workers[i]);
  }
  Log(("-->Freeing workers\n"));
  FREE(GD_Workers);
}

void _freeStructures(){

    Log(("-->Freeing GD_ServerGroup\n"));
    HashSync_destroy(GD_ServerGroup);

    Log(("-->Freeing GD_ServerUsers\n"));
    HashSync_destroy(GD_ServerUsers);

    Log(("-->Freeing GD_ServerSetting\n"));
    SettingManager_destroy_settings_struct(&GD_ServerSetting);

    Log(("-->Freeing GD_WorkerCommunicationChannel\n"));
    Ch_Free(GD_WorkerCommunicationChannel);

}

int _buildSocket(char* socketPath,bool forceBind){

    if (strlen(socketPath)> 108){
        errno = ENAMETOOLONG;
        perror("socketPath too long");
        return -1;
    }

    int listenerfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(listenerfd < 0) {
        perror("error in socket()");
        return -1;
    }

    struct sockaddr_un socket_address;
    socket_address.sun_family = AF_UNIX;
    strcpy (socket_address.sun_path, socketPath);

    if(forceBind) unlink(socketPath);
    if( bind(listenerfd, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0){
        perror("error in address bind");
        return -1;
    }

    return listenerfd;
}

int _listenAndServe(int listenerSocketFd) {
    struct timespec timer;
    if(listen(listenerSocketFd,GD_ServerSetting->maxConnections) <0){
        perror("error in listen");
        return -1;
    }

    sigset_t sigIgnore;
    sigemptyset(&sigIgnore);
    //sigaddset(&sigIgnore, SIGPIPE);

    int max_fd = listenerSocketFd;
    FD_ZERO(&GD_FdSetRead);
    FD_SET(listenerSocketFd, &GD_FdSetRead);

    fd_set working_rfd;
    int event = 0;
    int lastAddedFd=0;
    struct sockaddr_un client_adderss;
    int in_addr_len = 0;

    while(!got_SIGTERM){
        Data_copy_readSet_S(&working_rfd);
        max_fd = max_fd >= lastAddedFd ? max_fd : lastAddedFd; // updatefd
        timer.tv_sec = 300; //5 minutes
        timer.tv_nsec = 0;
        event = pselect(max_fd+1,&working_rfd,NULL,NULL,&timer,&sigIgnore);

        if(event <=0){
            switch(errno){
                case EINTR:
                    //A signal was recived
                    if(got_PRINTSTAT){
                        FILE* stat_file = fopen(GD_ServerSetting->statFileName, "w+");
                        printStats_S(stat_file);
                        fflush(stat_file);
                        fclose(stat_file);
                        got_PRINTSTAT = 0;
                    }
                    if(got_SIGTERM) {
                        break;
                    }
                    if(got_REFRESH_FD) got_REFRESH_FD = 0;
                    break;
            }
            continue;
        }
        for(int fd=1; fd<max_fd+1; fd++){
            if(FD_ISSET(fd,&working_rfd)) {
                if (fd == listenerSocketFd) {
                    //A new connection is asked from a client
                    lastAddedFd = accept(listenerSocketFd, (struct sockaddr *) &client_adderss, (socklen_t*)&in_addr_len);
                    if (lastAddedFd < 0) {
                        perror("error can't accept connection on accept");
                    }
                    Data_put_in_readSet_S(lastAddedFd);
                } else {
                    //it's a read write operation from a client
                    if (Data_remove_from_readSet_S(fd)) {
                        Ch_Push_S(GD_WorkerCommunicationChannel, &fd);
                    }
                }
            }
        }
    }
    return 0;
}





