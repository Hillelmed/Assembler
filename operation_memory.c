#include "operation_memory.h"
#include "apputil.h"


Operation_memory_list *new_list_operation()
{
	int i;
	Operation_memory_list *temp = malloc(sizeof *temp);
    if (!temp) {
        return NULL;
    }
    temp->list = malloc(sizeof(struct Operation_memory)*SIZE_INIT);
    for (i = 0; i < SIZE_INIT; ++i) {
    	temp->list[i] = NULL;
	}
    temp->size = SIZE_INIT;
    temp->current_size = 0;
    temp->data_size = 0;
    return temp;

}

Operation_memory *create_new_operation_memory(unsigned int address, long int mc, int status)
{
	Operation_memory *new_obj = malloc(sizeof *new_obj);
	if (!new_obj) {
        return NULL;
    }
    new_obj->address = address;
    new_obj->mc = mc;
    new_obj->status = status;
    new_obj->symbol_name[0] = '\0';
    return new_obj;
}

Operation_memory *create_new_om_init()
{
	Operation_memory *new_obj = malloc(sizeof *new_obj);
	if (!new_obj) {
        return NULL;
    }
    return new_obj;
}

Operation_memory_list *insert_operation_init_to_list(Operation_memory_list *om_list, unsigned int address, long int mc, int status)
{
	int i = 0;
	Operation_memory *new_obj = malloc(sizeof *new_obj);
	new_obj->address = address;
	new_obj->mc = mc;
	new_obj->status = status;
	if(om_list->size == 0)
	{
		om_list->list[i] = new_obj;
		om_list->current_size += 1;
		return om_list;
	}
	for (i = 0; i < om_list->size; ++i) {
		if(om_list->list[i] == NULL)
		{
			om_list->list[i] = new_obj;
			om_list->current_size += 1;
			return om_list;
		}
	}
	if(i == om_list->size)
	{
		copy_and_increase(om_list);
		om_list->list[i] = new_obj;
		om_list->current_size += 1;
	}
	return om_list;
}
Operation_memory_list *insert_operation_to_list(Operation_memory_list *om_list,Operation_memory *obj)
{
	int i = 0;
	if(om_list->size == 0)
	{
		om_list->list[i] = obj;
		om_list->current_size += 1;
		return om_list;
	}
	for (i = 0; i < om_list->size; ++i) {
		if(om_list->list[i] == NULL)
		{
			om_list->list[i] = obj;
			om_list->current_size += 1;
			return om_list;
		}
	}
	if(i == om_list->size)
	{
		copy_and_increase(om_list);
		om_list->list[i] = obj;
		om_list->current_size += 1;
	}
	return om_list;
}

Operation_memory *get_first_opm_lack(Operation_memory_list *opm_list)
{
	int i = 0;
	for (i = 0; i < opm_list->size; ++i) {
		if(opm_list->list[i] != NULL)
		{
			if(opm_list->list[i]->status == 0)
			{
				return opm_list->list[i];
			}
		}
	}
	return NULL;
}

Operation_memory_list *merge_order_data(Operation_memory_list* order, Operation_memory_list* data)
{
	int i = 0, j = 0 , diss = 0;
	for (i = 0; i < order->size; ++i) {
		if(order->list[i] != NULL)
		{
			continue;
		}else
		{
			break;
		}
	}
	data->list[j]->address = order->list[i-1]->next_address;
	data->list[j]->next_address = data->list[j]->next_address + data->list[j]->address;
	while(data->list[j] != NULL)
	{
		insert_operation_to_list(order,data->list[j]);
		j++;
		if(data->list[j] != NULL)
		{
			diss = data->list[j]->next_address - data->list[j]->address;
			data->list[j]->address = data->list[j-1]->next_address;
			data->list[j]->next_address = data->list[j]->address + diss;
		}
	}
	order->data_size = data->data_size;
	return NULL;
}

void copy_and_increase(Operation_memory_list *om_list)
{
	int i;
	Operation_memory **tmp_list = malloc(sizeof(struct Operation_memory)*(SIZE_INIT + om_list->size));
	for (i = 0; i < om_list->size; ++i)
	{
		if(om_list->list[i] != NULL)
		{
			tmp_list[i] = om_list->list[i];
		}
	}
	om_list->list = tmp_list;
	om_list->size = SIZE_INIT + om_list->size;
}

void print_operation_memory_list(Operation_memory_list *om_list)
{
	int i;
	for (i = 0; i < om_list->size; ++i) {
		if(om_list->list[i] != NULL) {
			printf("[DEBUG] Om : %d ,address : %d",i,om_list->list[i]->address);
			printf(" machine code : ");
			print_bytes(om_list->list[i]->mc);
			printf(" next address code : %d", om_list->list[i]->next_address);
			printf(" status : %d", om_list->list[i]->status);
			if(om_list->list[i]->symbol_name[0] != '\0')
			{
				printf(" symbol_name : %s", om_list->list[i]->symbol_name);
			}
		    printf("\n");
		}
	}
	printf("[DEBUG] total current size : %d\n", om_list->current_size);
	printf("[DEBUG] total current data : %d\n", om_list->data_size);

}

void print_operation_memory(Operation_memory *om_obj)
{
	printf("\nOperation Memory : address : %d",om_obj->address);
	printf(" machine code : ");
	print_bytes(om_obj->mc);
	printf("\n");

}

void free_memory_list(Operation_memory_list *om_list)
{
	int i;
	for (i = 0; i < om_list->size; ++i) {
		if(om_list->list[i] != NULL) {
			free(om_list->list[i]);
			om_list->list[i] = NULL;
		}
	}
	free(om_list->list);
	free(om_list);
}

