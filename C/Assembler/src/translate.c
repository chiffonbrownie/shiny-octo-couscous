#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are 
   valid, since that will be checked in part two.

   
   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {
	/* Future codes for pseudo instructions fisrt pass processing*/
		write_inst_string(output, name, args, num_args);
		return 1;
	
}

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li and blt pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS. 

   Writes the instruction in hexadecimal format to OUTPUT during pass #2. 
   
   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros. 

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write 
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error. 
*/

int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr,
	SymbolTable* symtbl, SymbolTable* reltbl) {
		
    /* R-Type Format instructions */
    if (strcmp(name, "add") == 0)        return write_rtype (0x33, 0x00, 0x00, output, args, num_args);
    else if (strcmp(name, "sub") == 0)   return write_rtype (0x33, 0x00, 0x20, output, args, num_args);
    else if (strcmp(name, "or") == 0)    return write_rtype (0x33, 0x06, 0x00, output, args, num_args);
    /* YOUR CODE HERE  for the remaining instructions*/
    // sll
    else if (strcmp(name, "sll") == 0)   return write_rtype (0x33, 0x01, 0x00, output, args, num_args);
    // srl
    else if (strcmp(name, "srl") == 0)   return write_rtype (0x33, 0x05, 0x00, output, args, num_args);
    // sra
    else if (strcmp(name, "sra") == 0)   return write_rtype (0x33, 0x05, 0x20, output, args, num_args);


    /* B-Type Format instructions */
    else if (strcmp(name, "beq") == 0)   return write_branch (0x63, 0x00, output, args, num_args, addr, symtbl);
    /* YOUR CODE HERE  for the remaining instructions*/
    // bne
    else if (strcmp(name, "bne") == 0)   return write_branch (0x63, 0x63, output, args, num_args, addr, symtbl);
    // blt
    else if (strcmp(name, "blt") == 0)   return write_branch (0x63, 0x04, output, args, num_args, addr, symtbl);


    /* JALR-Type Format instructions */
        /* Add your code for jalr*/
    // jalr
	else if (strcmp(name, "jalr") == 0)  return write_jalr (0x67, 0x00, output,args, num_args, addr, reltbl);
    /* JAL-Type Format instructions */
	    /* Add your code the for jal*/
    // jal
    else if (strcmp(name, "jal") == 0)   return write_jal (0x6F, output, args, num_args, addr, reltbl);
	
	   
    /* I-Type Format instructions */
        /* Add your code for the for addi*/
    // addi
    else if (strcmp(name, "addi") == 0)  return write_imm (0x13, 0x00, output, args, num_args);
    // andi
    else if (strcmp(name, "andi") == 0)  return write_imm (0x13, 0x07, output, args, num_args);
    // ori
    else if (strcmp(name, "ori") == 0)   return write_imm (0x13, 0x06, output, args, num_args);
    // lb
    else if (strcmp(name, "sb") == 0)   return write_mem (0x23, 0x00, output, args, num_args);
    // lw
    else if (strcmp(name, "sw") == 0)   return write_mem (0x23, 0x02, output, args, num_args);

    /* Shift I-Type Format instructions */
	    /* Add your code for the for slli,  srli, and srai*/
    // slli
    else if (strcmp(name, "slli") == 0)  return write_shift_im (0x13, 0x01, 0x00, output, args, num_args);
    // srli
    else if (strcmp(name, "srli") == 0)  return write_shift_im (0x13, 0x05, 0x00, output, args, num_args);
    // srai
    else if (strcmp(name, "srai") == 0)  return write_shift_im (0x13, 0x05, 0x20, output, args, num_args);
   
	
    /* S-Type Format instructions */
	  /* Add your code for the for sb and sw */
	// sb
    else if (strcmp(name, "lb") == 0)   return read_mem (0x03, 0x00, output, args, num_args);
    // sw
    else if (strcmp(name, "lw") == 0)   return read_mem (0x03, 0x02, output, args, num_args);


    else                                 return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.
   Validation is done on all user inputs
 */
int write_rtype(uint8_t opcode,  uint8_t funct3, uint8_t funct7, FILE* output, char** args, size_t num_args) {

    /* Validate # of arguments */
    if (num_args != 3) {
        return -1;
    }
	
    int rd = translate_reg(args[0]);
    int rs1 = translate_reg(args[1]);
    int rs2 = translate_reg(args[2]);

    /* Validate register numbers */
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        return -1;
    } 

    uint32_t instruction = (funct7 << 25) + (rs2 << 20) + (rs1 << 15) + (funct3 << 12) + (rd << 7) + opcode;
    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing most B-type instructions. You should use
   translate_reg() to parse registers, get_addr_for_symbol() to parse labels, 
   and write_inst_hex() to write to OUTPUT. 
   Both are defined in translate_utils.h.
   Validation is done on all user inputs
 */
int write_branch(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args, 
	uint32_t addr, SymbolTable* symtbl) {

    /* Validate # of arguments */
    if (num_args != 3) {
        return -1;
    }

    int rs1 = translate_reg(args[0]);
    int rs2 = translate_reg(args[1]);
    char* label = args[2];

    int64_t add = get_addr_for_symbol(symtbl, label);
    
    /* Validate register numbers and address */
    if (add == -1 || rs1 == -1 || rs2 == -1) {
        return -1;
    }

    /*
       It is not necessary to substract 4 because in this assembler application
       the program counter is increased in 4 after calling translate_inst
       uint16_t imm = add - addr - 4;
    */ 
    int64_t imm = (add - addr);

    // Check whether imm is in the right range (12 bits: between -4096 and 4095)
    if (imm > 4095 || imm < -4096) {
        return -1;
    }

    uint32_t bit12 = ((imm & 0x1000) >> 12) << 31;
    uint32_t bit11 = ((imm & 0x0800) >> 11) << 7;
    uint32_t bits10to5 = ((imm & 0x07E0) >> 5) << 25;
    uint32_t bits4to1 = ((imm & 0x001E) >> 1) << 8;

    uint32_t instruction = opcode + bit11 + bits4to1 + (funct3 << 12) + (rs1 << 15) + (rs2 << 20) + bits10to5 + bit12;
    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing most UJ-type instructions. You should use
   translate_reg() to parse registers, get_addr_for_symbol() to parse symbol
   and write_inst_hex() to write to OUTPUT. 
   Both are defined in translate_utils.h.
   Validation is done on all user inputs. Field as opcode does
   not need to be validated because it has been verified during
   program debugging.

   jal sentence is not position independent. Although the immediate included
   in the sentence is an offset to be added (arithmetically) to the current
   PC to form an absolute address and there is a limit on the "jump"
   (21 bits offset), the linker could allocate the instruction or the function
   where jal jumps in a different block of the virtual memory address, so
   the linker, eventually, will recalculate the offset of jal
 */
int write_jal(uint8_t opcode, FILE* output, char** args, size_t num_args, 
	uint32_t addr,    SymbolTable* reltbl ) {

    /* Validate # of arguments */
    if (num_args != 2) {
        return -1;
    }
	
    int rd = translate_reg(args[0]);
    char* label = args[1];
    int add = add_to_table(reltbl, label, addr);
    /* Validate register number and whether the label has succedded when
       added to the relocation table */
    if (add == -1 || rd == -1) {
        return -1;
    }
    int imm = add - addr;

    /* Check whether imm is in the right range 
       (21 bits: between -1,048,576 and 1,048,575) */
    if (imm > 1048575 || imm < -1048576) {
        return -1;
    }
	
    uint32_t bits19to12 = (imm & 0xFF000);
    uint32_t bits10to1 = ((imm & 0x007FF) >> 1) << 21;
    uint32_t bit11 =  ((imm & 0x00800) >> 11) << 20;
    uint32_t bit20 =  ((imm & 0x100000) >> 20) << 31;

    uint32_t instruction = opcode + (rd << 7) + bits19to12 + bit11 + bits10to1 + bit20 ;
    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing jalr I-type instructions. You should use
   translate_reg() to parse registers, add_to_table() and write_inst_hex()
   to write to OUTPUT. Both are defined in translate_utils.h.
   Validation is done on all user inputs. Field as opcode does
   not need to be validated because it has been verified during
   program debugging.

   jalr sentence is not position independent. This is because although a sentence
   like lui or auipc will define the 20 most significant bits, jalr includes
   the 12 less significant ones and, eventually, the jumping address can
   be relocated by the linker, so linker needs to know this fact in order to
   recalculate the address to be included in auipc or lui and the offset
   to include in jalr.
 */
int write_jalr(uint8_t opcode, uint8_t funct3, FILE* output, char** args, size_t num_args, 
	uint32_t addr, SymbolTable* reltbl) {

    /* Validate # of arguments */
    if (num_args != 3) {
        return -1;
    }
	
    int rd = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    char* label = args[2];
    int add = add_to_table(reltbl, label, addr);
    /* Validate registers number and whether the label has succedded when
       added to the relocation table */
    if (add == -1 || rd == -1 || rs == -1) {
        return -1;
    }

    int imm = add - addr;
	
    // Check whether imm is in the right range (12 bits: between -4096 and 4095)
    if (imm > 2047 || imm < -2048) {
        return -1;
    }

    uint32_t instruction = opcode +  (rd << 7) + (funct3 << 12) + (rs << 15)  + ((imm & 0x0FFF) << 20);
    write_inst_hex(output, instruction);
    return 0;
}


/* A helper function for writing immediate instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.
   Validation is done on all user inputs. Fields as opcode and funct3 do
   not need to be validated because they have been verified during
   program debugging.
 */
// int write_imm (uint8_t opcode,  uint8_t funct3, FILE* output, char** args, size_t num_args) {
//     /* Add your code */

//     /* Validate # of arguments */
//     if (num_args != 3) {
//         return -1;
//     }
//     // translate and initiate
//     int rd = translate_reg(args[0]);
//     int rs1 = translate_reg(args[1]);
//     int check_imm = translate_num(output,atoi(args[2]),-4096,4095);
//     uint16_t imm =atoi(args[2]);

//     printf("--Write Imm\n");
    
//     // check if everything is valid
//     if (rd == -1 || rs1 == -1 || check_imm == -1) {
//         return -1;
//     }

//     printf("--opcode: %d, funct3: %d, num_args: %ld\n",opcode,funct3,num_args);
//     printf("--arg0: %s, arg1: %s. arg2: %s\n",args[0],args[1],args[2]);
//     printf("--rd: %d, rs1: %d, imm: %d\n",rd,rs1,imm);
    
//     // create instruction
//     // write to hex
    
//     uint32_t instruction = opcode + (rd << 7) + (funct3 << 12) (rs1 << 15) ((imm & 0x0FFF) << 20); ////////
// 	write_inst_hex(output, instruction);
// 	return 0;
// }

int write_imm (uint8_t opcode,  uint8_t funct3, FILE* output, char** args, size_t num_args) {
   long int imm;
    /* Validate # of arguments */
    if (num_args != 3) {
        return -1;
    }
    // translate and initiate
    int rd = translate_reg(args[0]);
    int rs1 = translate_reg(args[1]);
    /* imm is a 12 bits signed number (so range between -2048 and 2047) */
    int num = translate_num(&imm, args[2], -2048, 2047);
   
    // check if everything is valid
    if (rd == -1 || rs1 == -1 || num == -1) {
        return -1;
    }
   
    uint32_t instruction = (imm << 20) + (rs1 << 15) +  (funct3 << 12) + (rd << 7) + opcode ;
    write_inst_hex(output, instruction);
    return 0;
}


/* A function for writing shift immediate instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.
   Validation is done on all user inputs. Fields as opcode, funct3 and funct7
   do not need to be validated because they have been verified during
   program debugging.
 */
int write_shift_im (uint8_t opcode,  uint8_t funct3, uint8_t funct7, FILE* output, char** args, size_t num_args) {
    /* Add your code */
    long int shamt;
    //long int imm;

    /* Validate # of arguments */
    if (num_args != 3) {
        return -1;
    }

    int rd = translate_reg(args[0]);
    int rs1 = translate_reg(args[1]);
    int shamt_num = translate_num(&shamt,args[2],-16,15);
    //int num = translate_num(&imm,((atoi(args[2]) >> 5) & ((1 << 7)-1)),-64,63);

    if (rd == -1 || rs1 == -1 || shamt_num == -1) {
        return -1;
    }

    uint32_t instruction = (funct7 << 25) + (shamt << 20) + (rs1 << 15) + (funct3 << 12) + (rd << 7) + opcode ;
    write_inst_hex(output, instruction);
	return 0;
}


/* A function for writing immediate instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.
   Validation is done on all user inputs. Fields as opcode and funct3 do
   not need to be validated because they have been verified during
   program debugging.
 */
int read_mem (uint8_t opcode,  uint8_t funct3, FILE* output, char** args, size_t num_args) {
    /* Add your code */
    long int imm;

    if (num_args != 3) {
        return -1;
    }

    int rd = translate_reg(args[0]);
    int rs1 = translate_reg(args[2]);
    int error = translate_num(&imm, args[1], -1048576, 1048575);


    if (rd == -1 || error == -1 || rs1 == -1) {
        return -1;
    }
   
    uint32_t instruction = (imm << 20) + (rs1 << 15) +  (funct3 << 12) + (rd << 7) + opcode ;
    write_inst_hex(output, instruction);
    return 0;
}


/* A helper function for writing most S-type instructions. You should use
   translate_reg() to parse registers, translate_num() to parse 
   numerical arguments and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.
   Validation is done on all user inputs. Fields as opcode and funct3 do
   not need to be validated because they have been verified during
   program debugging.
 */
int write_mem (uint8_t opcode,  uint8_t funct3, FILE* output, char** args, size_t num_args) {

    /* Add your code */
    long int imm;

    if (num_args != 3) {
        return -1;
    }

    int rs2 = translate_reg(args[0]);
    int rs1 = translate_reg(args[2]);
    int error = translate_num(&imm, args[1], -1048576, 1048575);

    if (rs2 == -1 || error == -1 || rs1 == -1) {
        return -1;
    }

    long int imm_first_five = imm & 31; // last 5 bits 11111
    long int imm_last_seven = imm >> 5; // first 7 bits

    uint32_t instruction = (imm_last_seven << 25) + (rs2 << 20) + (rs1 << 15) + (funct3 << 12) + (imm_first_five << 7) + opcode ;
    write_inst_hex(output, instruction);
    return 0;
}
