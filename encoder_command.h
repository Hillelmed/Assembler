/*
 * encoder_command.h
 * This code present all the encoder in assembler usage for example encode to binary
 * this code include all the command add,move the implementation exist in the .c file
 * Have 2 struct present how the command build in my code
 */

#ifndef ENCODER_COMMAND_H_
#define ENCODER_COMMAND_H_

#include <stdio.h>
#include <stdlib.h>

#include "symbols_table.h"
#include "operation_memory.h"
#include "apputil.h"
#include "asmessages.h"

#define COMMAND_R_SIZE 8
#define COMMAND_I_SIZE 15
#define COMMAND_J_SIZE 4
#define COMMAND_DIRECTIVE_SIZE 4
#define EXTENSION_SIZE 1
#define LENGTH_MAX 80


enum command_types {R_A = 0, R_C = 1, I_A = 2, I_B = 3, I_L = 4, J = 5};
enum args_types {numeric_byte = 1, numeric_h_byte = 2, numeric_word = 4, charts = 3};

typedef struct Command_Directive
{
	char *name;
	enum args_types arg_type;
}Command_Directive;

typedef struct Command
{
	char *name;
	enum command_types type;
	char opcode;
	char funct;
	unsigned int args_num;
}Command;

extern const struct Command command_R[];
extern const struct Command command_I[];
extern const struct Command command_J[];
extern const struct Command_Directive command_directive[];

extern const char *extension[];

int encode_arrange_commnd_operator(char* command_str,Symbol_list *symbol_list,
		Operation_memory_list *memory_order_list,
		Operation_memory_list *data_memory_list,
		int *IC,int *DC, char *error_message);
int arrange_data_command(char* command_str,
		Symbol_list *symbol_list,
		Operation_memory_list *data_memory_list, char *error_message);
int arrange_symbol_data(Symbol_list *symbol_list,
		Operation_memory_list *data_memory_list);
void fix_str_input(char *str);
int append_data_to_list(Command_Directive command_d,
		Operation_memory_list *opnm_list,
		char *args, char *symbol_name, int spliter, int *DC, char *error_message);

long int encode_r_command(Command command, char *args, char *error_message);
long int encode_i_command(Command command, char *args, int *status, char *error_message);
long int encode_j_command(Command command, char *args, int *status, char *error_message);
long int encode_d_command(Operation_memory_list *om_list,
		Command_Directive command_d,
		char* symbol_name, char *args ,int *DC, char *error_message);

int fill_encode_i_command(Command command, char *args, Operation_memory_list *om_list,
		Symbol_list *symbol_list, char *error_message);
int fill_encode_j_command(Command command, char *args, Operation_memory_list *om_list,
		Symbol_list *symbol_list , char *error_message);

int append_data_to_memory(Command_Directive command_d,
		Operation_memory_list *opnm_list,
		char **spliter_args,
		char *args, char *symbol_name, int *set_symbol,int *DC, int byte_size);
#endif /* ENCODER_COMMAND_H_ */
