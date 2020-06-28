#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mips_funct_asm.h"

/* Runs the instruction with R Format */
int32_t rInstruction(int opCode, int* regiInput, int shamt, int funct)
{
  int32_t machineInst;
  int rd = regiInput[0];
  int rs = regiInput[1];
  int rt = regiInput[2];
  
  machineInst = (opCode << 26) + (rs << 21) + (rt << 16) + (rd << 11) + (shamt << 6) + funct;

  return machineInst;
}

/* Runs instruction with I Format */
int32_t iInstruction(int opCode, int* regiInput, int immediate)
{
  int32_t machineInst;
  int32_t rt = regiInput[0];
  int32_t rs = regiInput[1];
  
  machineInst = ((opCode << 26) + (rs << 21) + (rt << 16)) + immediate;

  return machineInst;
}

/* Runs instruction with J Format */
int32_t jInstruction(int opCode, int address)
{
  int32_t machineInst;

  machineInst = (opCode << 26) + address;

  return machineInst;
}

/* Creates the machine instruction for the given instruction */
int32_t createMachineInstruction(char* instruction, int regiInput[], int numInput[], struct TableElt* label, int PC)
{
  /* Holds the maximum value of 16 bit ints + 1 */
  int negativeBuffer = 65536;
  /* add rd rs rt */
  if(strcmp(instruction, "add") == 0)
    {
      return rInstruction(0, regiInput, 0, 32);
    }
  /* addi rt rs immediate */
  else if(strcmp(instruction, "addi") == 0)
    {
      /* If the immediate value is negative, we will subtract
	 it from the buffer and pass that along (Two's Complement) */
      if(numInput[0] < 0)
	{
	  negativeBuffer += numInput[0];

	  return iInstruction(8, regiInput, negativeBuffer);
	}
      return iInstruction(8, regiInput, numInput[0]);
    }
  /* sub rd rs rt */
  else if(strcmp(instruction, "sub") == 0)
    {
      return rInstruction(0, regiInput, 0, 34);
    }
  /* mult rs rt */
  else if(strcmp(instruction, "mult") == 0)
    {
      return rInstruction(0, regiInput, 0, 24);
    }
  /* mfhi rd */
  else if(strcmp(instruction, "mfhi") == 0)
    {
      return rInstruction(0, regiInput, 0, 16);
    }
  /* mflo rd */
  else if(strcmp(instruction, "mflo") == 0)
    {
      return rInstruction(0, regiInput, 0 , 18);
    }
  /* and rd rs rt */
  else if(strcmp(instruction, "and") == 0)
    {
      return rInstruction(0, regiInput, 0, 36);
    }
  /* or rd rs rt */
  else if(strcmp(instruction, "or") == 0)
    {
      return rInstruction(0, regiInput, 0, 37);
    }
  /* nor rd rs rt */
  else if(strcmp(instruction, "nor") == 0)
    {
      return rInstruction(0, regiInput, 0, 39);
    }
  /* sll rd rt shamt */
  else if(strcmp(instruction, "sll") == 0)
    {
      return rInstruction(0, regiInput, numInput[0], 0);
    }
  /* srl rd rt shamt */
  else if(strcmp(instruction, "srl") == 0)
    {
      return rInstruction(0, regiInput, numInput[0], 2);
    }
  /* lw rs rt immediate */
  else if(strcmp(instruction, "lw") == 0)
    {
      return iInstruction(35, regiInput, numInput[0]);
    }
  /* sw rs rt immediate */
  else if(strcmp(instruction, "sw") == 0)
    {
      return iInstruction(43, regiInput, numInput[0]);
    }
  /* slt rd rs rt */
  else if(strcmp(instruction, "slt") == 0)
    {
      return rInstruction(0, regiInput, numInput[0], 42);
    }
  /* bne rs rt address */
  else if(strcmp(instruction, "bne") == 0)
    {
      int flippedInput[2];

      flippedInput[0] = regiInput[1];
      flippedInput[1] = regiInput[0];

      if(label -> value < (PC + 1))
	{
	  negativeBuffer += (label -> value - (PC + 1));
	  return iInstruction(4, flippedInput, negativeBuffer);
	}
      return iInstruction(4, flippedInput, (label -> value - (PC + 1)));
    }
  /* beq rs rt address */
  else if(strcmp(instruction, "beq") == 0)
    {
      int flippedInput[2];

      flippedInput[0] = regiInput[1];
      flippedInput[1] = regiInput[0];

      if(label -> value < (PC + 1))
	{
	  negativeBuffer += (label -> value - (PC + 1));
	  return iInstruction(4, flippedInput, negativeBuffer);
	}
      return iInstruction(4, flippedInput, (label -> value - (PC + 1)));
    }
  /* j address */
  else if(strcmp(instruction, "j") == 0)
    {
      return jInstruction(2, label -> value);
    }
  /* nop */
  else if(strcmp(instruction, "nop") == 0)
    {
      return 0;
    }
  else
    {
      printf("Function Not Recongnized! \n");
      return -1;
    }
}
