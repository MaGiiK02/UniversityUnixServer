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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <signal.h>

/* inserire gli altri include che servono */
#include "../HashTable/HashTable.h"
#include "../User/User.h"
#include "../Sync/Sync.h"
#include "../SettingManager/SettingManager.h"
#include "../Booleans/Booleans.h"
#include "../Operations/ops.h"

#define DEFAULT_HASH_SIZE 512

//Functions
int _argsGetStrFromFlag(const char* flag, int argc, char* argv[],char** out_ris);
static void _usagePrint(const char *prog_name);

//Signal Handler
typedef void (*SignalHadler)(int sig, siginfo_t *info, void *ucontext);
void handlerClose(int sig, siginfo_t *info, void *ucontext);
void handlerPrintStats(int sig, siginfo_t *info, void *ucontext);
void _init();

/* struttura che memorizza le statistiche del server, struct statistics
 * e' definita in stats.h.
 *
 */
struct statistics chattyStats = { 0,0,0,0,0,0,0 };

HashTable* RegisteredUsers;
Settings Setting;

int main(int argc, char *argv[]) {
    int ris;

    char* conffile_path = NULL;
    ris = argsGetStrFromFlag("-f",argc,argv,&conffile_path);
    if(ris != 0){
        fprintf(stderr,"missing config file form the application's args");
        usagePrint(argv[0]);
        exit(1);
    }
    Setting = SettingManager_load_settings_form_file(conffile_path);

    init();

    _addSignalHandler(SIGINT,handlerClose);
    _addSignalHandler(SIGTERM,handlerClose);
    _addSignalHandler(SIGQUIT,handlerClose);
    _addSignalHandler(SIGUSR1,handlerPrintStats);

    _listenAndServe(

    );

    return 0;
}

int _init(){
    RegisteredUsers = User_NewHashTable(DEFAULT_HASH_SIZE);
}

static void _usagePrint(const char *prog_name) {
    fprintf(stderr, "Il server va lanciato con il seguente comando:\n");
    fprintf(stderr, "  %s -f conffile\n", prog_name);
}

int _argsGetStrFromFlag(const char* flag, int argc, char* argv[],char** out_ris){
    int i;
    for(i=1; i<argc; i++){
        if(strcmp(arcv[i],flag) == 0){
            if(i+1>=argc){
                return -1;
            }else{
                *out_ris = arcv[i];
                return 0;
            }
        }
    }
    return 1;
}

int _listenAndServe(){

}

void _addSignalHandler(unsigned int signal,handler SignalHandler){
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(signal, &sa, NULL) == -1){
        perror("error adding new signal handler");
        exit(1);
    }
}

