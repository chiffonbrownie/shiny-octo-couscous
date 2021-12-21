
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
	write_to_log("Error: allocation failed\n");
	exit(1);
}

void addr_alignment_incorrect() {
	write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
	write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE* output, uint32_t addr, const char* name) {
	fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
	/* YOUR CODE HERE */
	SymbolTable *retval;

	retval = (SymbolTable*) malloc(1 * sizeof(SymbolTable));
	
	if (retval == NULL) {
		allocation_failed();
	}

	retval->mode = mode;
	retval->len = 0;
	retval->tbl = (Symbol*) malloc(retval->len * sizeof(Symbol));
	if(retval->tbl == NULL) {
		free(retval);
		allocation_failed();
	}
	return retval;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
	
	for (int i = 0; i < table->len; i++) {
		free(table->tbl[i].name);
	}
	free(table->tbl);
	free(table);
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added. 

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists  
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed().

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
	
	if ((addr & 0x3) != 0) {
		addr_alignment_incorrect();
		return -1;
	}

	if (table->mode == SYMTBL_UNIQUE_NAME) {
		for (int i = 0; i < table->len; i++) {
			if (strcmp(table->tbl[i].name, name) == 0) {
				name_already_exists(name);
				return -1;
			}
		}
	}

	table->len++;
	table->tbl = (Symbol*) realloc(table->tbl, table->len * sizeof(Symbol));
	if(table->tbl == NULL) {
		allocation_failed();
	}

	table->tbl[table->len - 1].name = malloc(1 * sizeof(char));
	if(table->tbl[table->len - 1].name == NULL) {
		allocation_failed();
	}
	strcpy(table->tbl[table->len - 1].name, name);

	table->tbl[table->len - 1].addr = addr;
	return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
	
	for (int i = 0; i < table->len; i++) {
		if (strcmp(table->tbl[i].name, name) == 0) {
			return table->tbl[i].addr;
		}
	}
	return -1;   
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
	
	/* YOUR CODE HERE */
	printf("%s\n", "hi");
	for (int i = 0; i < table->len; i++) {
		write_symbol(output, table->tbl[i].addr, table->tbl[i].name);
	} 
}
