/*
 * @Author: mattia.angelini 
 * @Date: 2017-05-16 11:12:03 
 * @StudentCode: 502688
 * @Last Modified by: angelini.mattia
 * @Last Modified time: 2017-05-16 22:22:59
 */

/* System Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Project Includes */

/*Module Includes */
#include "SettingManager.h"


/* Functions Definition */

 
Settings* SettingManager_new_settings_struct(){
    Settings* settings = malloc(sizeof(Settings));
    settings->dirName =  malloc(sizeof(char)*256);
    settings->statFileName = malloc(sizeof(char)*256);
    settings->unixPath = malloc(sizeof(char)*256);
    return settings;
}


void SettingManager_destroy_settings_struct(Settings** settings_ptr){
    Settings* settings = *settings_ptr;
    if(settings == NULL)
        return;
  
    free(settings->dirName);
    free(settings->statFileName);
    free(settings->unixPath);
    free(settings);
    *settings_ptr = NULL;
}

void SettingManager_settings_set_unix_path (Settings* settings, char* unix_path){
    strcpy (settings->unixPath, unix_path);
}

void SettingManager_settings_get_unix_path (Settings* settings,char* out_unix_path){
    strcpy (out_unix_path, settings->unixPath);
}


void SettingManager_settings_set_dir_name (Settings* settings, char* dir_name){
    strcpy (settings->dirName, dir_name);
}

void SettingManager_settings_get_dir_name (Settings* settings,char* out_dir_name){
    strcpy (out_dir_name, settings->dirName);
}


void SettingManager_settings_set_stat_file_name (Settings* settings, char* stat_file_name){
    strcpy (settings->statFileName, stat_file_name);
}

void SettingManager_settings_get_stat_file_name (Settings* settings,char* out_stat_file_name){
    strcpy (out_stat_file_name, settings->statFileName);
}

void SettingManager_print_settings_struct(Settings* settings){
    if(settings == NULL)
        return;
        
    printf("Printing the Settings object -> %p\n",settings);
    printf("    unixPath: %s\n",settings->unixPath);
    printf("    maxConnections: %d\n",settings->maxConnections);
    printf("    threadsInPool: %d\n",settings->threadsInPool);
    printf("    maxMsgSize: %d\n",settings->threadsInPool);
    printf("    maxFileSize: %d\n",settings->maxFileSize);
    printf("    maxHistMsgs: %d\n",settings->maxHistMsgs);
    printf("    dirName: %s\n",settings->dirName);
    printf("    statFileName: %s\n",settings->statFileName);
    printf("End Settings struct.\n");    
}