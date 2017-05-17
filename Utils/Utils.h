#ifndef _UTILS_H_
#define _UTILS_H_

/*
 * @Author: angelini.mattia 
 * @StudentCode: 502688
 * @Date: 2017-05-16 21:25:18 
 * @Last Modified by: mattia.angelini
 * @Last Modified time: 2017-05-17 17:43:40
 */

/**
 * Remove all occourrences of the given character in the string,
 * returning the resulting string.
 */
void Utils_str_remove_character(const char* str,const char c);

/**
 * Remove all the spaces in the given string,
 * returning the resulting string.
 */
void Utils_str_remove_spaces(const char* str);

/**
 * Cut the string given a character and
 * the string will be splitted in a left and right parts retuned in the relative parameters.
 * The function return a integer that can be thus 0 or 1, where 0 mean no problem, 1 vice versa.
 */
int Utils_str_split_by_first_char(const char* str,const char cutter_character,char* left_part,char* right_part);

int Utils_string_to_integer(const char* str);

/**
 * Make a lowercase of the given string, returning a new lowercase setting of the given one. 
 */
char* Utils_str_lowercase(char const *str);
#endif