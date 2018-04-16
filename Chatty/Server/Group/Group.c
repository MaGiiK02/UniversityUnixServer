//
// Created by Mattia Angelini on 12/04/2018.
//

#include "Group.h"

int _compare_user_name(void* u1,void* u2){
  char* user1 = (char*) u1;
  char* user2 = (char*) u2;
  return strcmp(user1,user2);
}

void _free_user_name(void* u){
  char* name = (char*)u;
  FREE(name)
}

void _name_copy(void* trg,void* source){
  memcpy(trg,source, sizeof(char)*MAX_NAME_LENGTH);
}

Group* Group_new(char* name, User* founder){
  Group* g = malloc(sizeof(Group));
  strcpy(g->group_name,name);
  strcpy(g->founder,founder->name);
  g->registered_users = List_new(sizeof(char)*MAX_NAME_LENGTH,&_free_user_name,&_compare_user_name,&_name_copy);
  List_append(g->registered_users,founder->name);
  return g;
}


int Group_add_User(Group* g, User* u){
  if(Group_is_founder(g,u) || Group_is_registered(g,u)){
    return 1;
  }
  List_append(g->registered_users,u->name);
  return 0;
}

void Group_remove_User(Group* g, User* u){
  Group_remove_User_by_name(g,u->name);
}

void Group_remove_User_by_name(Group* g, char* username){
  List_destroy_element(g->registered_users,username);
}

bool Group_is_founder(Group* g, User* u){
  return Group_is_founder_by_name(g,u->name);
}

bool Group_is_founder_by_name(Group* g, char* username){
  return (strcmp(g->founder,username) == 0) ? true:false;
}

bool Group_is_registered(Group* g, User* u) {
  return Group_is_registered_by_name(g, u->name);
}

bool Group_is_registered_by_name(Group* g, char* name) {
  return List_find(g->registered_users,name,NULL);
}

void Group_free(void* g){
  Group* group = (Group*)g;
  List_destroy(group->registered_users);
  FREE(group)
}

void Group_copy(void* dst, void* src){
  if(dst == src) return;
  memcpy(dst,src, sizeof(Group));
}

HashTableSync* Group_NewHashTable(unsigned int size){
  return HashSync_new(size, sizeof(Group),&Group_free,&Group_copy);
}

bool Group_is_a_valid_groupname(char* name){
  int str_len = strlen(name);
  if (str_len>MAX_NAME_LENGTH) return false;
  char support_name[MAX_NAME_LENGTH+1];
  strcpy(support_name,name);
  Utils_str_lowercase(support_name,str_len);
  return strstr(support_name,"gruppo") != NULL;
}