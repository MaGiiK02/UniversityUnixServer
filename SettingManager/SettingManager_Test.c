/*
 * @Author: angelini.mattia 
 * @StudentCode: 502688
 * @Date: 2017-05-15 07:54:03 
 * @Last Modified by: angelini.mattia
 * @Last Modified time: 2017-05-16 22:28:05
 */

/* @Description: 
 *  This file is used to test the SettingManager component of the project,
 *  is a simple test that controll given a defined setting file if it is correctly loaded.
 */

#include <stdio.h>

#include "SettingManager.h"


int main(){
    char str[256];
    Settings* s = SettingManager_new_settings_struct();
    SettingManager_print_settings_struct(s);

    SettingManager_settings_set_dir_name(s,"Prova!!");
    SettingManager_print_settings_struct(s); 

    SettingManager_settings_get_dir_name(s,str);
    printf("Readed string : %s \n",str);

    SettingManager_destroy_settings_struct(&s);
    SettingManager_print_settings_struct(s);

    return 0;
}
 


