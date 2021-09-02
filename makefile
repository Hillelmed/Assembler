make: symbols_table.c apputil.c encoder_command.c operation_memory.c assembler.c
	gcc -ansi -pedantic -Wall apputil.c encoder_command.c operation_memory.c symbols_table.c assembler.c -o assmebler