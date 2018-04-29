//
// Created by Mattia Angelini on 12/04/2018.
//

/*
* @brief A module to manage the Group data structure.
*/

#ifndef SOL_2017_GROUPS_H
#define SOL_2017_GROUPS_H

#include "../../config.h"
#include "../../List/list.h"
#include "../User/User.h"

typedef struct _group {
    char group_name[MAX_NAME_LENGTH];
    List *registered_users;
    char founder[MAX_NAME_LENGTH];
}Group;

Group* Group_new(char* name,User* founder);

int Group_add_User(Group* g, User* u);

void Group_remove_User(Group* g, User* u);
void Group_remove_User_by_name(Group* g, char* username);

bool Group_is_founder(Group* g, User* u);
bool Group_is_founder_by_name(Group* g, char* username);

bool Group_is_registered(Group* g, User* u);
bool Group_is_registered_by_name(Group* g, char* name);

void Group_free(void* g);
void Group_copy(void* dst, void* src);

bool Group_is_a_valid_groupname(char* name);


/// User Hash

HashTableSync* Group_NewHashTable(unsigned int size);



#endif //SOL_2017_GROUPS_H
