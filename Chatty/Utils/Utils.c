/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 */

/* Header Include */
#include "Utils.h"

/* System Includes*/
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>

void Utils_str_remove_spaces(char* str){
    Utils_str_remove_character(str,' ');
}

void Utils_str_remove_character(char* str,const char c){
    char space[1] = {c};
    Utils_str_remove_characters(str,space);
}

void Utils_str_remove_special_chars(char* str){
    char special_chars[8] = {'\n', '\a', '\b', '\f', '\r', '\t', '\v',0};
    Utils_str_remove_characters(str,special_chars);
}

void Utils_str_remove_characters(char* str,char* to_be_removed){
    if( str == NULL || to_be_removed == NULL)
        return;

    char* writer = str;
    char* reader = str;

    while( *reader != '\0' && reader != NULL){
        if (strchr(to_be_removed,*reader) == NULL){ //check if the current character is in the to remove list
            *writer = *reader; // copy of the char
            writer++;
        }
        reader++;
    }
    *writer = '\0';
}

int Utils_str_split_by_first_char(char* str,const char* cutter_character,char** left_part,char** right_part){

    *left_part = strtok(str, cutter_character);
    *right_part = strtok(NULL,cutter_character);

    if(*left_part == 0 || *right_part == 0){
        return -1;
    }

    return 0;
}

int Utils_string_to_integer(const char* str){
    char* endptr = NULL;
    int converted_val = strtol(str, &endptr, 10);

    errno = 0; /*To avoid to get older error*/
    /* Error checking based on the strtol specfication http://man7.org/linux/man-pages/man3/strtol.3.html*/
    if (
        (
            errno == ERANGE && (
                converted_val == LONG_MAX ||
                converted_val == LONG_MIN
            )
        ) || (
            errno != 0 && converted_val == 0
        )
    ){
        // TO DO set errno
    }

   if (endptr == str) {
        // TO DO set errno
    }

    return converted_val;
}

void Utils_str_lowercase(char* str,int size)
{
    char* current_char = str;

    while(current_char++ && size > 0 ){
        *(current_char) = tolower(*current_char);
        size --;
    }
}

int Utils_str_compare_case_insensitive(const char* str_1,const char* str_2){

    int equals = 0;

    while( (*str_1 != '\0') && (*str_2 != '\0') && (equals == 0) ) {
       equals = tolower(*str_1) - tolower(*str_2);
       str_1++;
       str_2++;
    }

    return equals;
}

void Utils_str_clear(char* str){
    memset(str,0,strlen(str));
}

int Utils_calculate_mutex_array_size(long size){
    return ceil(log2((double)size)+1);
}

int Utils_build_path(char* target,char* basepath,char* fileName){
  return sprintf(target,"%s/%s",basepath,fileName);
}

void Utils_dir_create_if_not_exist(char* path){
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        mkdir(path, 0700);
    }
}

long Utils_file_size(FILE* f){
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}