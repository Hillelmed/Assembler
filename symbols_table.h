/*
 * symbols.h
 * This code present the symbol in the as files
 * we have four kind of symbol its call attributes like we can see
 * and we have struct Symbol_data that present a single symbol in as file (single but have many attributes)
 * Here i choose the linked list data structe to improve and stay with the order that given in file
 * struct Symbol_list have pointer to the head * and that's it
 */

#ifndef SYMBOLS_TABLE_H_
#define SYMBOLS_TABLE_H_

#define ATTRIBUTES_SIZE 4

enum attributes {none = 0 , code = 1, data = 2, entry = 3, external = 4};

typedef struct Symbol_data {
	enum attributes attribute[ATTRIBUTES_SIZE];
	char *name;
	unsigned int value;
    struct Symbol_data *next;
} Symbol_data;

typedef struct Symbol_list {
	Symbol_data *head;
} Symbol_list;


Symbol_list *new_list();
Symbol_data *create_new_symbol_data(enum attributes arttibute, char *name, unsigned int value);
Symbol_data *insert_to_list(Symbol_list *symbol_list, enum attributes arttibute, char *name, unsigned int value);
Symbol_list *append_atrb(Symbol_list *symbol_list, char *name, enum attributes arttibute);
Symbol_data *get_by_name(Symbol_list *symbol_list, char *name);
int symbol_exist(Symbol_list *symbol_list, Symbol_data *new_node);
void print_list(Symbol_list *self);
void free_list(Symbol_list *mylist);

#endif /* SYMBOLS_TABLE_H_ */
