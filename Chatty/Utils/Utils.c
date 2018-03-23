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

void Utils_str_remove_spaces(char* str){
    Utils_str_remove_character(str,' ');
}

void Utils_str_remove_character(char* str,const char c){
    char space[1] = {c};
    Utils_str_remove_characters(str,space);
}

void Utils_str_remove_special_chars(char* str){
    char special_chars[7] = {'\n', '\a', '\b', '\f', '\r', '\t', '\v'};
    Utils_str_remove_characters(str,special_chars);
}

void Utils_str_remove_characters(char* str,const char *to_be_removed){

    if( str == NULL )
        return;

    char* writer = str;
    char* reader = str;

    while( *reader != '\0'){
        if (strchr(to_be_removed,*reader) == NULL){
            *writer = *reader;
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

void Utils_str_lowercase(char* str)
{
    char* current_char = str;

    while(current_char++){
        *(current_char) = tolower(*current_char);
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
    return ceil(sqrt((double)size)); // Maybe a logarithmic function it's better
}