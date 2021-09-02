/*
 * symbols.c
 *
 *  Created on: Jul 24, 2021
 *      Author: user
 */

#include "symbols_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "asmessages.h"
#include "apputil.h"
#include "encoder_command.h"

Symbol_list *new_list()
{
	Symbol_list *temp = malloc(sizeof *temp);
    if (!temp) {
        return NULL;
    }
    temp->head = NULL;

    return temp;
}

Symbol_data *create_new_symbol_data(enum attributes arttibute, char *name, unsigned int value)
{
	Symbol_data *new_node = malloc(sizeof *new_node);
    if (!new_node) {
        return NULL;
    }
    new_node->attribute[0] = arttibute;
    new_node->attribute[1] = 0;
    new_node->attribute[2] = 0;
    new_node->attribute[3] = 0;

    if (!(new_node->name = malloc(strlen(name)))) {
        free(new_node);
        return NULL;
    }
    strcpy(new_node->name, name);
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

Symbol_data *insert_to_list(Symbol_list *symbol_list, enum attributes arttibute, char *name, unsigned int value)
{
	Symbol_data *tmp_node = create_new_symbol_data(arttibute,name,value);
	Symbol_data *tmp_head = symbol_list->head;

	if(symbol_exist(symbol_list,tmp_node)) {
		return NULL;
	}

    if (!tmp_node) {
        return NULL;
    }
    if(tmp_head == NULL) {
    	tmp_head = tmp_node;
    	symbol_list->head = tmp_head;
    	return symbol_list->head;
    }

    while(tmp_head->next != NULL) {
    	tmp_head = tmp_head->next;
    }

    tmp_head->next = tmp_node;
    return symbol_list->head;
}

Symbol_list *append_atrb(Symbol_list *symbol_list, char *name, enum attributes arttibute)
{
    Symbol_data *current = symbol_list->head;
    int i=0;
    while (current != NULL)
    {
        if(strcmp(current->name,name) == 0)
        {
        	for (i = 0; i < 5; ++i)
        	{
        		if(current->attribute[i] == 0)
        		{
        			current->attribute[i] = arttibute;
        			return symbol_list;
        		}
			}
        }
        current = current->next;
    }
    return NULL;

}

Symbol_data *get_by_name(Symbol_list *symbol_list, char *name)
{
    Symbol_data *current = symbol_list->head;

    while (current != NULL)
    {
        if(strcmp(current->name,name) == 0)
        {
        	return current;
        }
        current = current->next;
    }
    return NULL;

}

int symbol_exist(Symbol_list *symbol_list, Symbol_data *new_node)
{
    Symbol_data *current = symbol_list->head;

    while (current != NULL)
    {
        if(strcmp(current->name,new_node->name) == 0)
        {
        	return 1;
        }
        current = current->next;
    }
    return 0;
}

void print_list(Symbol_list *self)
{
	int i = 0;
    Symbol_data *current = self->head;
    while (current != NULL)
    {
        printf("\n[DEBUG] Name = %s , ",current->name);
        printf("Value = %d , ",current->value);
        for (i = 0; i < ATTRIBUTES_SIZE; ++i)
        {
        	if(current->attribute[i] != 0)
        	{
                printf(" Attribute %d = %d " , i , current->attribute[i]);
        	}
		}
        current = current->next;
    }
}

void free_list(Symbol_list *mylist)
{
	Symbol_data *current = mylist->head;
    while (current) {
    	Symbol_data *victim = current;
        current = current->next;
        free(victim->name);
        free(victim);
    }
    free(mylist);
}
