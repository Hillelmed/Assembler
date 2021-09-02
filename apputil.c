/*
 * apputil.c
 *
 *  Created on: May 23, 2021
 *      Author: user
 */

#include "apputil.h"


char *strremove(char *str, char *sub)
{
    int len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

int str_split(char *str, char *spliter, char **args)
{
	int i = 0;
	char *arg = strtok(str, spliter);
	while(arg != NULL)
	{
		args[i] = arg;
		arg = strtok(NULL, spliter);
		i++;
	}
	return i;
}


int str_is_not_empty_len(char *str, int length)
{
	int i = 0;
	if(str == '\0') {
		return 0;
	}
	if(length == -1) {
		while(str[i] != '\0')
		{
			if(str[i] == ' ' || str[i] == '\n'  || str[i] =='\t')
					{
						i++;
						continue;
					}else {
						return 1;
					}
		}
	}
	for (i = 0; i < length; ++i) {
		if(str[i] == ' ' || str[i] == '\n'  || str[i] =='\t')
		{
			continue;
		}else {
			return 1;
		}
	}
	return 0;
}

int str_is_not_empty(char *str)
{
	return str_is_not_empty_len(str,-1);
}


int get_valid_memory_command(char *arg) {
	int i = 0;
	for (i = 0; i < COMMAND_DIRECTIVE_SIZE; ++i) {
		if (strcmp(arg, command_directive[i].name) == 0) {
		    return i;
		}
	}
	return -1;
}

int get_valid_operator_location(char *arg,char* dest_group)
{
	int i;

	for (i = 0; i < COMMAND_R_SIZE; ++i) {
		if (strcmp(arg, command_R[i].name) == 0) {
			*dest_group = 'R';
		    return i;
		}
	}
	for (i = 0; i < COMMAND_I_SIZE; ++i) {
		if (strcmp(arg, command_I[i].name) == 0) {
			*dest_group = 'I';
		    return i;
		}
	}

	for (i = 0; i < COMMAND_J_SIZE; ++i) {
		if (strcmp(arg, command_J[i].name) == 0) {
			*dest_group = 'J';
		    return i;
		}
	}

	return -1;
}

char* replace_char(char* str, char find, char replace)
{
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}


char *trimwhitespace(char *str)
{
    int count = 0;
    int i;
    for (i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];

    str[count] = '\0';

  return str;
}

int is_number(char *str)
{
	int i = 0;
	if(str[i] == '-'){
		i++;
	}
    for (; str[i]!= '\0'; i++)
    {
        if (isdigit(str[i]) == 0)
              return 0;
    }
    return 1;
}

void remove_char(char *s,char chr)
{
	int i,j = 0;
	for(i = 0; s[i] != '\0'; i++)
	{
		if ( s[i] != chr )
		{
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
}


int validate_file_name(char *file_path)
{
	char *end_path = strrchr(file_path, '.');
	if(end_path != NULL)
	{
		return(strcmp(end_path, ".as"));
	}
	return 1;
}


void print_bytes(long int x)
{
    int i = 0, tmp;
    for (i = 31; i >= 0; i--)
    {
		tmp = (x & (1u << i) ? 1 : 0);
		printf("%d",tmp);
    }
}



