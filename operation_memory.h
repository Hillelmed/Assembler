/*
 * operation_memory.h
 * This code present how the command encode to memory and the code manage all the command
 * here we have an data strucute struct Operation_memory_list like array list
 * have a init size and i increases the size all the time with the init** when the array is full
 * i will multiplier the size
 * the struct Operation_memory present the memory command encode to binary mc = machine code
 * the address present the address in the vm will be run it the next address is for not uses
 * the IC ID algo i choose this way to improve the code (and also change a little bit)
 */

#ifndef OPERATION_MEMORY_H_
#define OPERATION_MEMORY_H_

#define SIZE_INIT 15

typedef struct Operation_memory
{
	unsigned int address;
	long int mc;
	unsigned int next_address;
	int status : 2;
	char symbol_name[32];
} Operation_memory;

typedef struct Operation_memory_list
{
	Operation_memory **list;
	unsigned int size;
	unsigned int current_size;
	unsigned int data_size;
} Operation_memory_list;

Operation_memory *create_new_om_init();
Operation_memory *create_new_operation_memory(unsigned int address, long int mc, int status);
Operation_memory_list *new_list_operation();
Operation_memory_list *insert_operation_to_list(Operation_memory_list *om_list,Operation_memory *obj);
Operation_memory_list *insert_operation_init_to_list(Operation_memory_list *om_list, unsigned int address, long int mc, int status);
Operation_memory *get_first_opm_lack(Operation_memory_list *opm_list);
Operation_memory_list *merge_order_data(Operation_memory_list* order, Operation_memory_list* data);
void print_operation_memory_list(Operation_memory_list *om_list);
void print_operation_memory(Operation_memory *om_obj);
void copy_and_increase(Operation_memory_list *om_list);
void free_memory_list(Operation_memory_list *om_list);


#endif /* OPERATION_MEMORY_H_ */
