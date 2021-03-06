#ifndef _UTILS_H_
#define _UTILS_H_

/*
 * @Author: angelini.mattia
 * @StudentCode: 502688
 * @brief A collection of generic purpose utils functions used in the project.
 */

#include "../Booleans/Booleans.h"
#include "../MemoryHelper/MemoryHelper.h"

#define KILOBYTE 1024

void Utils_str_remove_character(char* str,const char c);

void Utils_str_remove_characters(char* str, char *to_be_removed);

void Utils_str_remove_spaces(char* str);

/*
 * Creates a string based on the given one where the listed special characterr will be removed.
 * -> \n,\a,\b,\f,\r,\t,\v.
 */
void Utils_str_remove_special_chars(char* str);

/**
 * Cut the string given a character and
 * the string will be splitted in a left and right parts retuned in the relative parameters.
 * The function return a integer that can be thus 0 or 1, where 0 mean no problem, 1 vice versa.
 */
int Utils_str_split_by_first_char(char* str,const char* cutter_character,char** left_part,char** right_part);

int Utils_string_to_integer(const char* str);

void Utils_str_lowercase(char* str,int size);

int Utils_str_compare_case_insensitive(const char* str_1, const char* str_2);

void Utils_str_clear(char* str);

char* Utils_str_trim(char* str);

int Utils_calculate_mutex_array_size(long size);

int Utils_build_path(char* target,char* basepath,char* filename);

long Utils_file_size(FILE* f);

void Utils_dir_create_if_not_exist(char* path);

void flushSocket(int fd,int size);

#endif
