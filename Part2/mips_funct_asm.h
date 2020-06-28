#ifndef MIPS_FUNCT_ASM_H
#define MIPS_FUNCT_ASM_H

#include <stdint.h>
#include "table.h"

/* Runs instruction with R Format */
int32_t rInstruction(int opCode, int* regiInput, int shamt, int funct);

/* Runs instruction with I Format */
int32_t iInstruction(int opCode, int* regiInput, int immediate);

/* Runs instruction with J Format */
int32_t jInstruction(int opCode, int address);

/* Creates the machine instruction for the given instruction */
int32_t createMachineInstruction(char* insruction, int* regiInput, int* numInput, struct TableElt* label, int PC);
  
#endif
