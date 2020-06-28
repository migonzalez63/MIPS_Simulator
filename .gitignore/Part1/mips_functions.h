#ifndef MIPS_FUNCTIONS_H
#define MIPS_FUNCTIONS_H

#include "table.h"

/* Adds the contents of two registers */
int32_t add(int32_t firstSource, int32_t secondSource);

/* Adds the contents of one register with an immediate value */
int32_t addi(int32_t firstSource, int signExtImm);

/* Subtracts the contents of two registers */ 
int32_t sub(int32_t firsSource, int32_t secondSource);

/* Multiplies the contents of two registers. Since we are multiplying
   two 32 bit values, it is possible we will get a 64 bit number,
   thus we must store the top 32 bits and the lower 32 bits into temp
   registers, hi and lo. 65535 represents tht lower 32 bits and 
   4294901760 represents the upper 32 bits. */
int32_t* mult(int32_t firstSource, int32_t secondSource);

/* Access the hi register */
int32_t mfhi(int32_t* hiLo);

/* Access the lo register */
int32_t mflo(int32_t* hiLo);

/* Applies the logical and operator to the content of two registers */
int32_t and(int32_t firstSource, int32_t secondSource);

/* Applies the logical or operator to the content of two registers */
int32_t or(int32_t firstSource, int32_t secondSource);

/* Applies the logical nor operator to the content of two registers */
int32_t nor(int32_t firstSource, int32_t secondSource);

/* Shifts the contents of the a register to the left by any given number of 
   bits */
int32_t sll(int32_t firstSource, int shamt);

/* Shifts the contents of the a register to the right by any given number of 
   bits */
int32_t srl(int32_t firstSource, int shamt);

/* Loads a value into a register */
void lw(struct TableElt* firstSource, struct Table* memory, int signExtImm);

/* Saves a register value into memory */
void sw(struct TableElt* firstSource, struct Table* memory, int signExtImm);

/* Applies the logical operator less than to two registers and returns
   the boolean result */
int32_t slt(int32_t firstSource, int32_t secondSource);

/* Compares the contents of two registers and jumps to the given label 
   if the are NOT equal to each other */
int32_t bne(int32_t firstSource, int32_t secondSource, struct TableElt* label);

/* Compares the contents of two registers and jumps to the given label 
   if the are equal to each other */ 
int32_t beq(int32_t firstSource, int32_t secondSource, struct TableElt* label);

/* Jumps to the given label location on file */
int32_t j(struct TableElt* label);

/* Burns one clock cycle */
void nop();

/* Compares the given instruction to see what it should execute and depending on the intructions, we will 
   inintialize the proper inputs. */
void runInstruction(char* instruction, struct TableElt* label, struct TableElt* regiInput[], int numInput[], struct Table* registers, struct Table* memory, FILE* fp, int32_t* hiLo);  

#endif
