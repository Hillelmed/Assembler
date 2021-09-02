#include "encoder_command.h"

const struct  Command command_R[COMMAND_R_SIZE] = {
	{ "add", R_A, 0, 1 , 3},
	{ "sub", R_A, 0, 2 , 3},
	{ "and", R_A, 0, 3 , 3},
	{ "or", R_A, 0, 4 , 3},
	{ "nor", R_A, 0, 5 , 3},
	{ "move", R_C, 1, 1 , 2},
	{ "mvhi", R_C, 1, 2 , 2},
	{ "mvlo", R_C, 1, 3 , 2}
};

const struct  Command command_I[COMMAND_I_SIZE] = {
	{ "addi", I_A, 10, -1 , 3},
	{ "subi", I_A, 11, -1 , 3},
	{ "andi", I_A, 12, -1 , 3},
	{ "ori", I_A, 13, -1 , 3},
	{ "nori", I_A, 14, -1 , 3},
	{ "bne", I_B, 15, -1 , 3},
	{ "beq", I_B, 16, -1 , 3},
	{ "blt", I_B, 17, -1 , 3},
	{ "bgt", I_B, 18, -1 , 3},
	{ "lb", I_L, 19, -1 , 3},
	{ "sb", I_L, 20, -1 , 3},
	{ "lw", I_L, 21, -1 , 3},
	{ "sw", I_L, 22, -1 , 3},
	{ "lh", I_L, 23, -1 , 3},
	{ "sh", I_L, 24, -1 , 3}
};

const struct  Command command_J[COMMAND_J_SIZE] = {
	{ "jmp", J, 30, -1 , 1 },
	{ "la", J, 31, -1 , 1},
	{ "call", J, 32, 1 , 1},
	{ "stop", J, 63, -1 , 0}
};

const struct Command_Directive command_directive[COMMAND_DIRECTIVE_SIZE] = {
		{ ".asciz", charts },
		{ ".db", numeric_byte },
		{ ".dw", numeric_word },
		{ ".dh", numeric_h_byte }
};

const char *extension[] = {".entry",".extern"};

int encode_arrange_commnd_operator(char* command_str,Symbol_list *symbol_list, Operation_memory_list *memory_order_list, Operation_memory_list *data_memory_list, int *IC,int *DC, char *error_message)
{
	int status = 0 , command_encod_index, x , flag_new_symbol = 0 , len = 0;
	char tmp_command_line[LENGTH_MAX] = {0}, a = '\0';
	char *spliter_args[4] = {0};
	Operation_memory *om = NULL;
	strcpy(tmp_command_line, command_str);
	if(command_str[0] == '\t' || command_str[0] == ' ')
	{
		str_split(tmp_command_line,"\t",spliter_args);
		spliter_args[2] = spliter_args[1];
		spliter_args[1] = spliter_args[0];
		spliter_args[0] = "\0";
	}
	else
	{
		str_split(tmp_command_line,"\t",spliter_args);
		fix_str_input(spliter_args[0]);
	}
	if(str_is_not_empty(spliter_args[0]))
	{
		flag_new_symbol = 1;
	}

	fix_str_input(spliter_args[1]);

	if(str_is_not_empty(spliter_args[2]))
	{
		fix_str_input(spliter_args[2]);
	}

	command_encod_index = get_valid_memory_command(spliter_args[1]);
	if(command_encod_index != -1)
	{
		if(flag_new_symbol)
		{
			len = strlen(spliter_args[0]);
			if(len > 31)
			{
				sprintf(error_message,"Label crossed the maximum char (31) : label = %s",spliter_args[0]);
				return -1;
			}
			if(insert_to_list(symbol_list, data, spliter_args[0], 55) == NULL)
			{
				sprintf(error_message,"%s label : %s",LABEL_ALREADY_EXIST, spliter_args[0]);
				return -1;
			}
			flag_new_symbol = 0;
		}
		/**write 8 in  algo*/
		encode_d_command(data_memory_list, command_directive[command_encod_index],spliter_args[0] , spliter_args[2], DC , error_message);
	}
	else
	{
		if(strcmp(spliter_args[0], extension[0]) == 0)
		{
			/**
			 * .entry will be care in the next through on file
			 */
			return 0;
		}
		else if(strcmp(spliter_args[0], extension[1]) == 0)
		{
			if(insert_to_list(symbol_list,external,spliter_args[1],0) == NULL)
			{
				strcpy(error_message, EXTERNAL_ALREADY_EXIST);
				return -1;
			}

		} else if(strcmp(spliter_args[1], extension[0]) == 0)
		{
			printf("[WARN] the entry : %s contains label\n", spliter_args[2]);
			return 0;
		} else if(strcmp(spliter_args[1], extension[1]) == 0)
		{
			printf("[WARN] the extern : %s contains label\n", spliter_args[2]);
			return 0;
		}
		else
		{
			if(flag_new_symbol)
					{
						len = strlen(spliter_args[0]);
						if(len > 31)
						{
							sprintf(error_message," Label crossed the maximum char (31) : label = %s",spliter_args[0]);
							return -1;
						}
						if(insert_to_list(symbol_list,code,spliter_args[0],*IC) == NULL)
						{
							sprintf(error_message,"%s label : %s",LABEL_ALREADY_EXIST, spliter_args[0]);
							return -1;
						}
						flag_new_symbol = 0;
					}
					command_encod_index = get_valid_operator_location(spliter_args[1],&a);
					switch(a)
					{
						case 'R':
						{
							x = encode_r_command(command_R[command_encod_index], spliter_args[2] , error_message);
							if(x == -1 && error_message[0] != '\0')
							{
								return -1;
							}
							om = create_new_operation_memory(*IC,x, 1);
							break;
						}
						case 'I':
						{
							x = encode_i_command(command_I[command_encod_index], spliter_args[2], &status , error_message);
							if(x == -1 && error_message[0] != '\0')
							{
								return -1;
							}
							om = create_new_operation_memory(*IC,x, status);
							status = 0;
							break;
						}
						case 'J':
						{
							x = encode_j_command(command_J[command_encod_index], spliter_args[2], &status , error_message);
							if(x == -1 && error_message[0] != '\0')
							{
								return -1;
							}
							om = create_new_operation_memory(*IC,x, status);
							status = 0;
							break;
						}
						default:
							sprintf(error_message,"Command not found : %s\n", spliter_args[1]);
							return -1;
					}
					insert_operation_to_list(memory_order_list,om);
					*IC += 4;
					om->next_address = *IC;
		}
	}
	om = NULL;
	return 0;
}

int arrange_data_command(char* command_str,Symbol_list *symbol_list, Operation_memory_list *data_memory_list, char *error_message)
{
	char command_chr = '\0';
	int command_encod_index = -1;
	char *spliter_args[LENGTH_MAX] = {0};
	char tmp_command_line[LENGTH_MAX];
	strcpy(tmp_command_line, command_str);
	if(command_str[0] == '\t' || command_str[0] == ' ')
	{
		str_split(tmp_command_line,"\t",spliter_args);
		spliter_args[2] = spliter_args[1];
		spliter_args[1] = spliter_args[0];
		spliter_args[0] = "\0";
	}
	else
	{
		str_split(tmp_command_line,"\t",spliter_args);
		fix_str_input(spliter_args[0]);
	}
	fix_str_input(spliter_args[1]);
	fix_str_input(spliter_args[2]);
	if(strcmp(spliter_args[0], extension[0]) == 0)
	{
		if(append_atrb(symbol_list, spliter_args[1], entry) == NULL)
		{
			strcpy(error_message,ENTRY_NOT_INCLOUD_IN_SYMBOLS);
			return -1;
		}
		return 0;
	}
	else
	{
		command_encod_index = get_valid_operator_location(spliter_args[1],&command_chr);
		switch(command_chr)
		{
			case 'I':
			{
				return fill_encode_i_command(command_I[command_encod_index], spliter_args[2], data_memory_list, symbol_list, error_message);
			}
			case 'J':
			{
				return fill_encode_j_command(command_J[command_encod_index], spliter_args[2], data_memory_list, symbol_list, error_message);
			}
			default:
				return 0;
		}

	}
	return 0;

}

int arrange_symbol_data(Symbol_list *symbol_list, Operation_memory_list *data_memory_list)
{
	int i=0;
	Symbol_data *data_tmp;
	for (i = 0; i < data_memory_list->size; ++i) {
		if(data_memory_list->list[i] != NULL)
		{
			if(data_memory_list->list[i]->symbol_name[0] != '\0')
			{
				data_tmp = get_by_name(symbol_list, data_memory_list->list[i]->symbol_name);
				data_tmp->value = data_memory_list->list[i]->address;
			}
		}else
		{
			break;
		}
	}
	return 0;
}

long int encode_d_command(Operation_memory_list *opnm_list,Command_Directive command_d,char* symbol_name , char *args , int *DC, char *error_message)
{
	int size = 0, i = 0, count_data = 0 , set_symbol = 0 , in_data = 0;
	char *spliter_args[LENGTH_MAX] = {0};
	Operation_memory *om = NULL;
	switch (command_d.arg_type) {
		case charts:
			if(args[size] != '"') {
				strcpy(error_message,".asciz command need to start with \"");
				return -1;
			}
			size = 1;
			while(args[size] != '"') {
				om = create_new_operation_memory(*DC, args[size],1);
				if(symbol_name != NULL && str_is_not_empty(symbol_name) && set_symbol == 0)
				{
					strcpy(om->symbol_name,symbol_name);
					set_symbol = 1;
				}
				insert_operation_to_list(opnm_list,om);
				size++;
				*DC += 1;
				count_data++;
				om->next_address = *DC;
			}
			om = create_new_operation_memory(*DC, '\0',1);
			insert_operation_to_list(opnm_list,om);
			*DC += 1;
			count_data++;
			om->next_address = *DC;
			opnm_list->data_size += count_data;
		break;
		case numeric_byte:
			size = str_split(args,",",spliter_args);
			if(size < 1)
			{
				return -1;
			}
			for(i = 0;i < size ; ++i) {
				remove_char(spliter_args[i],'+');
				in_data = atoi(trimwhitespace(spliter_args[i]));
				om = create_new_operation_memory(*DC, in_data & 0xFF, 1);
				if(symbol_name != NULL && str_is_not_empty(symbol_name) && set_symbol == 0)
				{
					strcpy(om->symbol_name,symbol_name);
					set_symbol = 1;
				}
				insert_operation_to_list(opnm_list,om);
				*DC += command_d.arg_type;
				count_data += command_d.arg_type;
				om->next_address = *DC;
			}
			opnm_list->data_size += count_data;
			break;
		case numeric_h_byte:
			size = str_split(args,",",spliter_args);
			if(size < 1)
			{
				return -1;
			}
			for(i = 0;i < size ; ++i) {
				remove_char(spliter_args[i],'+');
				in_data = atoi(trimwhitespace(spliter_args[i]));
				om = create_new_operation_memory(*DC, in_data & 0xFFFF, 1);
				if(symbol_name != NULL && str_is_not_empty(symbol_name) && set_symbol == 0)
				{
					strcpy(om->symbol_name,symbol_name);
					set_symbol = 1;
				}
				insert_operation_to_list(opnm_list,om);
				*DC += command_d.arg_type;
				count_data += command_d.arg_type;
				om->next_address = *DC;
			}
			opnm_list->data_size += count_data;

			break;
		case numeric_word:
			size = str_split(args,",",spliter_args);
			if(size < 1)
			{
				return -1;
			}
			for(i = 0;i < size ; ++i) {
				in_data = atoi(trimwhitespace(spliter_args[i]));
				om = create_new_operation_memory(*DC, in_data, 1);
				if(symbol_name != NULL && str_is_not_empty(symbol_name) && set_symbol == 0)
				{
					strcpy(om->symbol_name,symbol_name);
					set_symbol = 1;
				}
				insert_operation_to_list(opnm_list,om);
				*DC += command_d.arg_type;
				count_data += command_d.arg_type;
				om->next_address = *DC;
			}
			opnm_list->data_size += count_data;
			break;
		default:
			break;
	}
	return 0;
}

long int encode_r_command(Command command, char *args, char *error_message)
{
	long int x = 0, funct_mrg = command.funct << 6, opcode_mrg = command.opcode << 26;
	long int mrg_args = -1;
	int size = 0, rt = 0, rs = 0, rd = 0;
	char *spliter_args[LENGTH_MAX] = {0};
	size = str_split(args,",",spliter_args);
	if(size != command.args_num)
	{
		if(size < command.args_num)
		{
			strcpy(error_message,MISSING_PARAMETER);
		}else
		{
			strcpy(error_message,TO_MANY_PARAMETER);
		}
		return -1;
	}
	switch (command.type) {
		case R_A:
			trimwhitespace(spliter_args[0]);
			trimwhitespace(spliter_args[1]);
			trimwhitespace(spliter_args[2]);
			remove_char(spliter_args[0],'$');
			remove_char(spliter_args[1],'$');
			remove_char(spliter_args[2],'$');

			rs = atoi(spliter_args[0]);
			rt = atoi(spliter_args[1]);
			rd = atoi(spliter_args[2]);
			if(rs > 31 || rs < 0 || rt > 31 || rt < 0 ||rd > 31 || rd < 0)
			{
				strcpy(error_message,REGISTRY_OUT_OF_BOUND);
				return -1;
			}
			rs = rs << 21;
			rt = rt << 16;
			rd = rd << 11;
			mrg_args = rs | rt | rd;
			break;
		case R_C:
			trimwhitespace(spliter_args[0]);
			trimwhitespace(spliter_args[1]);
			remove_char(spliter_args[0],'$');
			remove_char(spliter_args[1],'$');
			rd = atoi(spliter_args[0]);
			rs = atoi(spliter_args[1]);
			if(rs > 31 || rs < 0 ||rd > 31 || rd < 0)
			{
				strcpy(error_message,REGISTRY_OUT_OF_BOUND);
				return -1;
			}
			rd = rd << 21;
			rs = rs << 11;
			mrg_args = rs | rd;
			break;
		default:
			strcpy(error_message,UNDEFINED_COMMAND);
			return -1;
			break;
	}
	if(mrg_args != -1)
	{
		x = x | opcode_mrg | mrg_args | funct_mrg;
	}
	return x;
}

long int encode_i_command(Command command, char *args, int *status, char *error_message)
{
	int size = 0;
	long int x = 0, opcode_mrg = command.opcode << 26 , rt = 0, rs = 0, mrg_args = 0;
	short immed = 0;
	char *spliter_args[LENGTH_MAX] = {0};
	size = str_split(args,",",spliter_args);
	if(size != command.args_num)
	{
		if(size < command.args_num)
		{
			strcpy(error_message,MISSING_PARAMETER);
		}else
		{
			strcpy(error_message,TO_MANY_PARAMETER);
		}
		return -1;
	}
	switch (command.type) {
		case I_B:
			remove_char(spliter_args[0],'$');
			remove_char(spliter_args[1],'$');
			remove_char(spliter_args[2],'$');
			rs = atoi(spliter_args[0]);
			rt = atoi(spliter_args[1]);
			if(rs > 31 || rs < 0 ||rs > 31 || rs < 0)
			{
				strcpy(error_message,REGISTRY_OUT_OF_BOUND);
				return -1;
			}
			immed = 0;
			mrg_args = (rt << 16) | (rs << 21);
			mrg_args |= immed;
			*status = 0;
			/**check when i have the address on specific label and put him spliter_args[2] */
			break;
		case I_A:
		case I_L:
	        if (!is_number(spliter_args[1]))
	        {
	            printf ("Entered input is not a number\n");
	            return -1;
	        }
			remove_char(spliter_args[0],'$');
			remove_char(spliter_args[1],'$');
			remove_char(spliter_args[2],'$');
			rs = atoi(spliter_args[0]);
			immed = atoi(spliter_args[1]);
			rt = atoi(spliter_args[2]);
			if(rs > 31 || rs < 0 ||rs > 31 || rs < 0)
			{
				strcpy(error_message,REGISTRY_OUT_OF_BOUND);
				return -1;
			}
			mrg_args = (rs << 21) | (rt << 16) | (immed & 0xFFFF);
			*status = 1;
			break;
		default:
			break;
	}
	if(mrg_args != 0)
	{
		x = x | opcode_mrg | mrg_args;
	}
	return x;
}

long int encode_j_command(Command command, char *args, int *status, char *error_message)
{
	int size = 0;
	long int x = 0, opcode_mrg = command.opcode << 26 , mrg_args = 0;
	long int address = 0;
	long int reg = 0;
	char *spliter_args[LENGTH_MAX] = {0};
	size = str_split(args,",",spliter_args);
	if(size != command.args_num)
	{
		if(size < command.args_num)
		{
			strcpy(error_message,MISSING_PARAMETER);
		}else
		{
			strcpy(error_message,TO_MANY_PARAMETER);
		}
		return -1;
	}
	if(strcmp(command.name,"jmp") == 0)
	{
		if(strstr(spliter_args[0],"$") != NULL)
		{
			reg = 1;
			reg = reg << 25;
			remove_char(spliter_args[0],'$');
			address = atoi(spliter_args[0]);
			if(address > 31 || address < 0)
			{
				strcpy(error_message,REGISTRY_OUT_OF_BOUND);
				return -1;
			}
			mrg_args = reg | address;
			*status = 1;
		}
		else
		{
			reg = 0;
			reg = reg << 25;
			mrg_args = reg;
			/**check when i have the address on specific label and put him */
		}
	} else if (strcmp(command.name,"la") == 0 || strcmp(command.name,"call") == 0)
	{
		reg = 0;
		/**check when i have the address on specific label and put him */
	} else if(strcmp(command.name,"stop") == 0)
	{
		*status = 1;
		/**Stop statmant */
	}else
	{
		/**Error not found J command */
		return -1;
	}
	x = x | opcode_mrg | (reg << 25) | mrg_args;
	return x;
}

int fill_encode_j_command(Command command, char *args, Operation_memory_list *om_list, Symbol_list *symbol_list, char *error_message)
{
	int size = 0, result = 0;
	Operation_memory *om = NULL;
	Symbol_data *sd;
	char *spliter_args[LENGTH_MAX] = {0};
	size = str_split(args,",",spliter_args);
	if(size != command.args_num)
	{
		return -1;
	}
	if(command.opcode == 63 || (command.opcode == 30 && strstr(spliter_args[0],"$") != NULL))
	{
		/**Not relevant for this code*/
		return 0;
	}
	om = get_first_opm_lack(om_list);
	sd = get_by_name(symbol_list,args);
	if(sd == NULL)
	{
		strcpy(error_message, COMMAND_USE_MISSING_SYMBOL);
		return -1;
	}
	if(om != NULL)
	{
		result = (sd->value & 0xFFFFFF);
		om->mc = om->mc | result;
		om->status = 1;
		strcpy(om->symbol_name,sd->name);
	}
	return 0;
}

int fill_encode_i_command(Command command, char *args, Operation_memory_list *om_list, Symbol_list *symbol_list, char *error_message)
{
	int i = 0, size = 0, result = 0;
	Operation_memory *om;
	Symbol_data *sd;
	char *spliter_args[LENGTH_MAX] = {0};
	if(command.type == I_B)
	{
		size = str_split(args,",",spliter_args);
		if(size != command.args_num)
		{
			return -1;
		}
		om = get_first_opm_lack(om_list);
		sd = get_by_name(symbol_list, spliter_args[2]);
		if(om != NULL && sd != NULL)
		{
			for (i = 0; i < ATTRIBUTES_SIZE; ++i)
			{
				if(sd->attribute[i] == 4)
				{
					strcpy(error_message,I_COMMAND_USE_EXTERNAL);
					return -1;
				}
			}
			result = ((sd->value - om->address) & 0xFFFF);
			om->mc = om->mc | result;
			om->status = 1;
		}
		return 0;
	}else
	{
		/*not relevant command*/
		return 0;
	}
}


void fix_str_input(char *str)
{
	if(str == '\0' || str == 0 || str == NULL)
	{
		return;
	}
	trimwhitespace(str);
	remove_char(str,':');
	remove_char(str,'\n');
	remove_char(str,'\t');
}

