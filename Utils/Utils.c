/*
 * @Author: angelini.mattia 
 * @StudentCode: 502688
 * @Date: 2017-05-16 21:25:18 
 * @Last Modified by: mattia.angelini
 * @Last Modified time: 2017-05-17 17:41:40
 */

/* Header Include */
#include "Utils.h"

/* System Includes*/ 
#include <string.h>
#include <errno.h>

void Utils_str_remove_character(const char* str,const char c){
    char *cleared_str = strdup(str);
    char *write = cleared_str
    char *read = cleared_str;

    if( cleared_str == NULL ) 
        return NULL,

    do {
        if ( *read != c && *read != '\0' ) /* Want to controll better the string termination. */
            *write++ = *read;
    } while ( *read++ );

    *write = '\0'; /* adding the string terminator */
    
    return cleared_str;
}

void Utils_str_remove_spaces(const char* str){
    return Utils_str_remove_character(str,' ');   
}

int Utils_str_split_by_first_char(const char* str,const char cutter_character,char* left_part,char* right_part){
    char* str_cpy = strdup(str); 
    char* cutter_position = strchr(str,cutter_character);
    if(cutter_position == NULL)
        return 1;
    
    left_part = str_cpy;
    right_part = cutter_position + 1;
    *equal_char = '\0';                /*Put a string terminator in the position on the cutter_character, in order to split the string in 2 */  
 
    return 0; 
}

int Utils_string_to_integer(const char* str){
    int converted_val = strtol(str, &endptr, base);
    errno = 0;
    if (
        (
            errno == ERANGE && (
                val == LONG_MAX || 
                val == LONG_MIN
            )
        ) || (
            errno != 0 && val == 0
        )
    ){
        // TO DO set errno
        THROW;
    }

   if (endptr == str) {
        // TO DO set errno
        THROW;
    }

    return converted_val;
}

char* Utils_str_lowercase(char const *str)
{
    char* str_lower = strdup(str);
    int i;
    for(i=0;i<strlen(str_lower);i++){
        *(str_lower+i) = tolower(*(str_lower+i); 
    }

    return str_lower;
}