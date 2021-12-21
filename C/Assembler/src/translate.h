#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdint.h>

/* IMPLEMENT ME - see documentation in translate.c */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, 
    uint32_t addr, SymbolTable* symtbl, SymbolTable* reltbl);

/* Declaring helper functions: */

int write_rtype(uint8_t opcode,  uint8_t funct3, uint8_t funct7, FILE* output, char** args, size_t num_args); /* for add, sub, and, or, sll, srl, sra instructions */


int write_shift_im(uint8_t opcode, uint8_t funct3, uint8_t funct7,FILE* output, char** args, size_t num_args); /* For immediate shift instructions slli, srli, srai */ 


int write_imm(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args); /* for addi, andi and orri instructions */
int read_mem(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args); /* for lw and lb instructions */


int write_mem(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args); /* for sw and sb instructions */
                      

int write_branch(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args, 
	uint32_t addr, SymbolTable* symtbl);  /* for bne, beq, blt, bge  instrictions */


int write_jal(uint8_t opcode, FILE* output, char** args, size_t num_args, 
	uint32_t addr, SymbolTable* symtbl);                                   /* for jal  instruction */
	
int write_jalr(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args, 
	uint32_t addr, SymbolTable* reltbl);   /* for jalr  instruction */

#endif
