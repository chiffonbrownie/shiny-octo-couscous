#ifndef ASSEMBLER_H
#define ASSEMBLER_H

int assemble(const char* in_name, const char* tmp_name, const char* out_name);

int pass_one(FILE *input, FILE* output, SymbolTable* symtbl);

int pass_two(FILE *input, FILE* output, SymbolTable* symtbl, SymbolTable* reltbl);
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args);

#endif
