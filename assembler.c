/*
 ============================================================================
 Name        : Maman14FinalProject.c
 Author      : Hillel medioni
 Version     : 1.0
 Copyright   : My copyright notice
 Description : This Project present a virtual computer world that present
 compile for assemble lang my algorithm its like present in the guide but with benefit
 This file its the main application get assembler path from args, and start to compile
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "asmessages.h"
#include "apputil.h"
#include "encoder_command.h"
#include "symbols_table.h"

int write_files(char *file_path,
		Operation_memory_list *data_memory_list, Symbol_list *symbol_list);
int write_file_obj(char *file_path_obj, Operation_memory_list *data_memory_list);
int write_file_ent(char *file_path_ent, Symbol_list *symbol_list);
int write_file_ext(char *file_path_ext, Operation_memory_list *data_memory_list, Symbol_list *symbol_list);

int append_until_finish_data(FILE *fp,
		Operation_memory *om, int *memory_runner,int *avalibale_in_line);
int assembler_file(char *file_path, int LOG_MODE ,char *buffer,
		Symbol_list *symbol_list, Operation_memory_list *memory_order_list, Operation_memory_list *data_memory_list);
int first_compile(FILE *file, char *buffer,
		Symbol_list *symbol_list, Operation_memory_list *memory_order_list, Operation_memory_list *data_memory_list);
int second_compile(FILE *file, char *buffer,
		Symbol_list *symbol_list, Operation_memory_list *memory_order_list);

int main(int argc, char* argv[]) {
	int i = 1, LOG_MODE = 0;
	Symbol_list *symbol_list = NULL;
	Operation_memory_list *memory_order_list = NULL;
	Operation_memory_list *data_memory_list = NULL;
	char *buffer = calloc(LENGTH_MAX,sizeof(char));
	if(argc > 2)
	{
		if(strcmp(argv[i],"-log") == 0)
		{
			LOG_MODE = 1;
			i = 2;
		}
	}
	for (; i < argc; ++i) {
		symbol_list = new_list();
		memory_order_list = new_list_operation();
		data_memory_list = new_list_operation();
		assembler_file(argv[i], LOG_MODE, buffer, symbol_list, memory_order_list, data_memory_list);
	}
	free_list(symbol_list);
	free_memory_list(data_memory_list);
	free_memory_list(memory_order_list);
	free(buffer);
	return EXIT_SUCCESS;
}

/*
 ============================================================================
 This function get single file path and start to the first compile and check
 if have some errors
 LOG_MODE its present if need to print to console logs (default is not printing)
 ============================================================================
 */

int assembler_file(char *file_path, int LOG_MODE ,char *buffer, Symbol_list *symbol_list, Operation_memory_list *memory_order_list, Operation_memory_list *data_memory_list)
{
	int find_error = 0;
	FILE *file;
	if(validate_file_name(file_path))
	{
		printf("\n[ERROR] file path are not valid : %s \n",file_path);
		return 1;
	}
	file = fopen(file_path, "r");
	if (file)
	{
		find_error = first_compile(file, buffer, symbol_list,memory_order_list, data_memory_list);
		merge_order_data(memory_order_list,data_memory_list);
		arrange_symbol_data(symbol_list,data_memory_list);
		rewind(file);
		if(find_error == 1)
		{
			second_compile(file, buffer, symbol_list, memory_order_list);
		}
		else {
			find_error = second_compile(file, buffer, symbol_list, memory_order_list);
		}
		if(find_error == 1)
		{
			printf("[ERROR] Output files will not be created\n");
			return 1;
		}
		if(LOG_MODE)
		{
			printf("\n");
			print_operation_memory_list(memory_order_list);
			print_list(symbol_list);
		}
	    fclose(file);
		write_files(strremove(file_path,".as"), memory_order_list, symbol_list);
		if(LOG_MODE)
		{
		    printf("\n[DEBUG] Finish compile assembler file : %s export files : obj,ext,ent \n",file_path);
		}
	}else
	{
	    printf("\n[ERROR] assembler: the file : %s failed to read maybe not exist \n",file_path);
	    return 1;
	}
	return 0;
}

/*
 ============================================================================
 This function do the first compile on the as file and start to build the
 memory operation and data operation
 check also if have some problem
 ============================================================================
 */

int first_compile(FILE *file, char *buffer, Symbol_list *symbol_list, Operation_memory_list *memory_order_list, Operation_memory_list *data_memory_list)
{

	int IC = 100, DC = 0, line = 0,status = 0 , find_error = 0;
	char error_message[MAX_LENGTH_ERROR_MESSAGE] = {0};
	while(fgets(buffer, LENGTH_MAX, file) != NULL)
	{
		line++;
		if(buffer[0] != ';' && buffer[0] != '\n' && str_is_not_empty(buffer))
		{
				status = encode_arrange_commnd_operator(buffer, symbol_list, memory_order_list, data_memory_list, &IC, &DC, error_message);
				if(status != 0)
				{
					find_error = 1;
					printf("[ERROR] First parsing failed at line : %d error message : %s\n", line, error_message);
				}
				memset(buffer, 0, sizeof(char) * LENGTH_MAX);
				memset(error_message,0 ,MAX_LENGTH_ERROR_MESSAGE);
		}
    }
	return find_error;
}

/*
 ============================================================================
 This function do the second compile and this function run on all data we
 received from the first running and now completing the missing binary code
 of the memory photo
  ============================================================================
 */

int second_compile(FILE *file, char *buffer, Symbol_list *symbol_list, Operation_memory_list *memory_order_list)
{
	int status = 0, find_error = 0, line = 0;
	char error_message[MAX_LENGTH_ERROR_MESSAGE] = {0};
	while(fgets(buffer, LENGTH_MAX, file) != NULL)
	{
		line++;
		if(buffer[0] != ';' && buffer[0] != '\n' && str_is_not_empty(buffer))
		{
				status = arrange_data_command(buffer, symbol_list, memory_order_list, error_message);
				if(status != 0)
				{
					find_error = 1;
					printf("[ERROR] Second parsing failed at line : %d error message : %s\n", line, error_message);
				}
				memset(buffer, 0, sizeof(char) * LENGTH_MAX);
		}
     }
	return find_error;
}

/*
 ============================================================================
 This function exec the writes to write the files
  ============================================================================
 */

int write_files(char *input_file_name, Operation_memory_list *data_memory_list , Symbol_list *symbol_list)
{
	   char *file_path_obj, *file_path_ext, *file_path_ent;
	   file_path_obj = malloc(strlen(input_file_name)+4);
	   file_path_ext = malloc(strlen(input_file_name)+4);
	   file_path_ent = malloc(strlen(input_file_name)+4);

	   strcpy(file_path_obj, input_file_name);
	   strcpy(file_path_ext, input_file_name);
	   strcpy(file_path_ent, input_file_name);

	   strcat(file_path_obj, ".obj");
	   strcat(file_path_ext, ".ext");
	   strcat(file_path_ent, ".ent");

	   write_file_obj(file_path_obj, data_memory_list);
	   write_file_ent(file_path_ent, symbol_list);
	   write_file_ext(file_path_ext, data_memory_list , symbol_list);
	   return 0;
}

int write_file_ext(char *file_path_ext, Operation_memory_list *data_memory_list, Symbol_list *symbol_list)
{
	FILE *fp_ext;
	int i = 0 , j = 0;
	Symbol_data *tmp_head = symbol_list->head;
	fp_ext = fopen(file_path_ext, "w");
	if(fp_ext)
	{
	   for (i = 0; i < data_memory_list->current_size; ++i) {
		   if(data_memory_list->list[i]->symbol_name[0] != '\0')
		   {
			   tmp_head = get_by_name(symbol_list,data_memory_list->list[i]->symbol_name);
			   for (j = 0; j < ATTRIBUTES_SIZE; ++j)
			   {
				   if(tmp_head->attribute[j] == 4)
				   {
					   fprintf(fp_ext, "%s 0%d\n", tmp_head->name, data_memory_list->list[i]->address);
					   break;
				   }
			   }
		   }
	   }
	   fclose(fp_ext);
	}
	return 0;
}

int write_file_ent(char *file_path_ent, Symbol_list *symbol_list)
{
	   FILE *fp_ent;
	   Symbol_data *tmp_head = symbol_list->head;
	   int i = 0;
	   fp_ent = fopen(file_path_ent, "w");
	   if(fp_ent)
	   {
		   while(tmp_head != NULL)
		   {
			   for (i = 0; i < ATTRIBUTES_SIZE; ++i)
			   {
				   if(tmp_head->attribute[i] == 3)
				   {
					   fprintf(fp_ent, "%s 0%d\n", tmp_head->name, tmp_head->value);
					   break;
				   }
			   }
			   tmp_head = tmp_head->next;
		   }
		   fclose(fp_ent);
	   }
	   tmp_head = NULL;
	   return 0;
}
int write_file_obj(char *file_path_obj, Operation_memory_list *data_memory_list)
{
	   FILE *fp_obj;
	   int dc = data_memory_list->data_size;
	   int i = 0, memory_runner = 100, avalibale_in_line = 4,size = data_memory_list->current_size - 1;
	   int ic_total = data_memory_list->list[size]->next_address  - 100 -  dc;

	   fp_obj = fopen(file_path_obj, "w");
	   if(fp_obj)
	   {
		   fprintf(fp_obj, "\t%d %d\t\n", ic_total, dc);
		   for (i = 0; i < data_memory_list->current_size ; ++i) {
			   append_until_finish_data(fp_obj, data_memory_list->list[i], &memory_runner, &avalibale_in_line);
		   }
		   fclose(fp_obj);
	   }
	   return 0;
}

int append_until_finish_data(FILE *fp, Operation_memory *om, int *memory_runner,int *avalibale_in_line)
{
	unsigned char b1,b2,b3,b4;
	unsigned long data;
	int distance_write_byte = om->next_address - om->address;
	switch (distance_write_byte) {
		case 4:
			switch (*avalibale_in_line) {
				case 4:
					   data = om->mc;
					   b1 = (unsigned char)(data & 0xff);
					   b2 = (unsigned char)(data >> 8) & 0xff;
					   b3 = (unsigned char)(data >> 16) & 0xff;
					   b4 = (unsigned char)(data >> 24);
					   fprintf(fp,"0%d %02X %02X %02X %02X\n",*memory_runner, b1, b2, b3, b4);
					   *memory_runner += 4;
					break;
				case 3:
					   data = om->mc;
					   b1 = (unsigned char)(data & 0xff);
					   b2 = (unsigned char)(data >> 8) & 0xff;
					   b3 = (unsigned char)(data >> 16) & 0xff;
					   b4 = (unsigned char)(data >> 24);
					   fprintf(fp,"%02X %02X %02X\n0%d %02X ", b1, b2, b3, *memory_runner, b4);
					   *avalibale_in_line = 3;
					   *memory_runner += 4;
					break;
				case 2:
					   data = om->mc;
					   b1 = (unsigned char)(data & 0xff);
					   b2 = (unsigned char)(data >> 8) & 0xff;
					   b3 = (unsigned char)(data >> 16) & 0xff;
					   b4 = (unsigned char)(data >> 24);
					   fprintf(fp,"%02X %02X\n0%d %02X %02X ", b1, b2, *memory_runner, b3, b4);
					   *avalibale_in_line = 2;
					   *memory_runner += 4;

					break;
				case 1:
					   data = om->mc;
					   b1 = (unsigned char)(data & 0xff);
					   b2 = (unsigned char)(data >> 8) & 0xff;
					   b3 = (unsigned char)(data >> 16) & 0xff;
					   b4 = (unsigned char)(data >> 24);
					   fprintf(fp,"%02X\n0%d %02X %02X %02X ", b1,*memory_runner, b2, b3, b4);
					   *avalibale_in_line = 1;
					   *memory_runner += 4;
					break;
				default:
					break;
			}
			break;
		case 2:
			switch (*avalibale_in_line) {
			case 4:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   b2 = (unsigned char)(data >> 8) & 0xff;
				   fprintf(fp,"0%d %02X %02X ", *memory_runner, b1, b2);
				   *avalibale_in_line = 2;
				   *memory_runner += 4;
				break;
			case 3:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   b2 = (unsigned char)(data >> 8) & 0xff;
				   fprintf(fp,"%02X %02X ", b1, b2);
				   *avalibale_in_line = 1;
				break;
			case 2:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   b2 = (unsigned char)(data >> 8) & 0xff;
				   fprintf(fp,"%02X %02X\n", b1, b2);
				   *avalibale_in_line = 4;
				break;
			case 1:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   b2 = (unsigned char)(data >> 8) & 0xff;
				   fprintf(fp,"%02X\n0%d %02X ", b1, *memory_runner , b2);
				   *avalibale_in_line = 3;
				   *memory_runner += 4;
				break;
			default:
				break;
		}
			break;
		case 1:
			switch (*avalibale_in_line) {
			case 4:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   fprintf(fp,"0%d %02X ",*memory_runner, b1);
				   *memory_runner += 4;
				   *avalibale_in_line = 3;
				break;
			case 3:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   fprintf(fp,"%02X ", b1);
				   *avalibale_in_line = 2;
				break;
			case 2:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   fprintf(fp,"%02X ", b1);
				   *avalibale_in_line = 1;
				break;
			case 1:
				   data = om->mc;
				   b1 = (unsigned char)(data & 0xff);
				   fprintf(fp,"%02X\n", b1);
				   *avalibale_in_line = 4;
				break;
			default:
				break;
		}
			break;
		default:
			break;
	}
	return 0;
}

