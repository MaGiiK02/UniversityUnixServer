/*
 * @Author: mattia.angelini 
 * @Date: 2017-05-16 11:12:03 
 * @StudentCode: 502688
 * @Last Modified by: mattia.angelini
 * @Last Modified time: 2017-05-18 16:29:47
 */

/* System Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Project Includes */
#include "../Utils/Utils.h"

/*Module Includes */
#include "SettingManager.h"


/* Private functions */


/*
 * Set a Setting struct field buy the field name aand a string value, casted to the correct field type
 */
void _settings_set_value_by_field_name(Settings* settings,const char* field_name,const char* value){
    if (strcmp(Utils_str_lowercase(field_name), SETTING_FIELD_NAME_UNIX_PATH) == 0){
        SettingManager_settings_set_unix_path(settings,value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_MAX_CONNECTIONS) == 0){
        settings->maxConnections = Utils_string_to_integer(value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_THREADS_IN_POOL) == 0){
        settings->threadsInPool = Utils_string_to_integer(value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_MAX_MSG_SIZE) == 0){
        settings->maxMsgSize = Utils_string_to_integer(value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_MAX_FILE_SIZE) == 0){
        settings->maxFileSize = Utils_string_to_integer(value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_MAX_HITS_MSG) == 0){
        settings->maxHistMsgs = Utils_string_to_integer(value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_DIR_NAME) == 0){
        SettingManager_settings_set_dir_name(settings,value);

    } else if ( strcmp(Utils_str_lowercase(field_name),SETTING_FIELD_NAME_STAT_FILE_NAME) == 0){
        SettingManager_settings_set_stat_file_name(settings,value);
        
    }
}


/*
 * Load all the settings from a given file pointer, if some setting can't be retreived then the default is used.
 */
Settings* _load_settings_from_file_ptr(FILE* fptr){

    Settings* settings = SettingManager_new_settings_struct();

    char* file_line = malloc( 512 * sizeof(char) );
    char* setting_value = NULL;
    char* setting_name = NULL;


    fgets(file_line,512,fptr);
    // TODO Handle row longer than 512 characters
    while(file_line){
        file_line = Utils_str_remove_spaces(file_line);
        if( *file_line != '#' ){
            /*The line isn't a comment*/
            /*Yes, I know that strtok exists*/
            if(Utils_str_split_by_first_char(file_line,'=',setting_name,setting_value) == 0 ){ /* If no problem occoured in the splitting, the function return a 0 code */
                _settings_set_value_by_field_name(settings,setting_name,setting_value);
            }
        }
        fgets(file_line,512,fptr);
    }                    
    return settings;
}

/* Header's functions definition */
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

void SettingManager_settings_set_unix_path (Settings* settings,const char* unix_path){
    strcpy (settings->unixPath, unix_path);
}

void SettingManager_settings_get_unix_path (Settings* settings,char* out_unix_path){
    strcpy (out_unix_path, settings->unixPath);
}


void SettingManager_settings_set_dir_name (Settings* settings,const char* dir_name){
    strcpy (settings->dirName, dir_name);
}

void SettingManager_settings_get_dir_name (Settings* settings,char* out_dir_name){
    strcpy (out_dir_name, settings->dirName);
}


void SettingManager_settings_set_stat_file_name (Settings* settings,const char* stat_file_name){
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

Settings* SettingManager_load_settings_form_file(const char* settingFilePath){
    Settings* s = NULL;
    FILE* fp = fopen(settingFilePath,"r");     /*Open the file to the given path for reading*/
    s = _load_settings_from_file_ptr(fp);      /*Read the file loading setting in the settings struct*/
    fclose(fp);                                /*Close the file*/
    return s;
}



