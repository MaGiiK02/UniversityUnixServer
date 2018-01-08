/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 */

/* @Description:
 *  This file is used to test the SettingManager component of the project,
 *  is a simple test that control given a defined setting file if it is correctly loaded.
 */

#include <stdio.h>

#include "SettingManager.h"


int main(int argc, char *argv[]){

  char str[256];
  Settings* s = SettingManager_new_settings_struct();
  SettingManager_print_settings_struct(s);

  SettingManager_settings_set_dir_name(s,"Prova!!");
  SettingManager_print_settings_struct(s);

  SettingManager_settings_get_dir_name(s,str);
  printf("Readed string : %s \n",str);

  SettingManager_destroy_settings_struct(&s);
  SettingManager_print_settings_struct(s);

  printf("File path: %s \n",argv[1]);

  s = SettingManager_load_settings_form_file(argv[1]);
  SettingManager_print_settings_struct(s);
  SettingManager_destroy_settings_struct(&s);

  return 0;
}
