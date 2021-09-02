/*
 * apputil.h
 * This code present the utils usage in code for example to remove char from *char
 *  all the duplication uses function cut to here
 */

#ifndef APPUTIL_H_
#define APPUTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "encoder_command.h"

char *strremove(char *str, char *sub);
int validate_file_name(char *file_path);
int str_split(char *str, char *spliter, char **args);
int get_valid_operator_location(char *arg,char *dest_group);
int get_valid_memory_command(char *arg);
char* replace_char(char* str, char find, char replace);
char *trimwhitespace(char *str);
int str_is_not_empty(char *str);
int is_number(char *str);
int str_is_not_empty_len(char *str, int length);
void remove_char(char *s,char chr);
void print_bytes(long int x);
#endif /* APPUTIL_H_ */
