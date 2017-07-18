/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @Date: 2017-05-16 21:25:18
 * @Last Modified by: mattia.angelini
 * @Last Modified time: 2017-05-18 16:00:48
 */

/* Header Include */
#include "Utils.h"

/* System Includes*/
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>

char* Utils_str_remove_character(const char* str,const char c){
    char *cleared_str = malloc(sizeof(char)*strlen(str));
    char *write = &cleared_str[0];
    char *read = &str[0];

    if( cleared_str == NULL )
        return NULL;

    do {
        if ( *read != c && *read != '\0' ) /* Want to controll better the string termination. */
            *write++ = *read;
    } while ( *read++ );

    *write = '\0'; /* adding the string terminator */

    return cleared_str;
}

char* Utils_str_remove_spaces(const char* str){
    return Utils_str_remove_character(str,' ');
}

int Utils_str_split_by_first_char(const char* str,const char* cutter_character,char** left_part,char** right_part){

    char *str_cpy = strdup(str);
    char *saveptr;

    *left_part = strdup(strtok_r(str_cpy, cutter_character,&saveptr));
    *right_part = strdup(strtok_r(NULL, cutter_character,&saveptr));

    if(*left_part == 0 || *right_part == 0){
        return 1;
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

char* Utils_str_lowercase(char const *str)
{
    int i;
    char* str_lower = strdup(str);

    for(i=0;i<strlen(str_lower);i++){
        *(str_lower+i) = tolower(*(str_lower+i));
    }

    return str_lower;
}

void Utils_str_clear(char *str){
    memset(str,0,strlen(str));
}

int _is_a_special_char(char const c){
    return (
        c == '\n' ||
        c == '\a' ||
        c == '\b' ||
        c == '\f' ||
        c == '\r' ||
        c == '\t' ||
        c == '\v'
    );
}

/*It's a little rendoundant naut in this way is more efficent.*/
char* Utils_str_remove_special_chars(char const *str){
    char *cleared_str = calloc(strlen(str),sizeof(char));
    int writed = 0;
    int readed = 0;

    if( str == NULL )
        return NULL;

    for(readed = 0;readed<strlen(str);readed ++){
        if ( !_is_a_special_char(str[readed]) )
            cleared_str[writed++] = str[readed];
    }
    cleared_str[writed] = '\0'; /* adding the string terminator */

    return cleared_str;
}
