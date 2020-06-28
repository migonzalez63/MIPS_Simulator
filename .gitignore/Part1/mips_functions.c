#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mips_functions.h"

#define TRUE 1
#define FALSE 0

/* Gives offset of array in order to not clash with memory values created
   before executing */
int offset = 20;

/* Stores temp variables hi and lo. Will be used by mult, mfhi, and mflo */
int32_t hiLo[2];
 
/* Adds the contents of two registers */
int32_t add(int32_t firstSource, int32_t secondSource)
{
  int32_t result = firstSource + secondSource;

  return result;
}

/* Adds the contents of one register with an immediate value */
int32_t addi(int32_t firstSource, int signExtImm)
{
  int32_t result = firstSource + signExtImm;

  return result;
}

/* Subtracts the contents of two registers */ 
int32_t sub(int32_t firstSource, int32_t secondSource)
{
  int32_t result = firstSource - secondSource;

  return result;
}

/* Multiplies the contents of two registers. Since we are multiplying
   two 32 bit values, it is possible we will get a 64 bit number,
   thus we must store the top 32 bits and the lower 32 bits into temp
   registers, hi and lo. 65535 represents tht lower 32 bits and 
   4294901760 represents the upper 32 bits. */
int32_t* mult(int32_t firstSource, int32_t secondSource)
{
  int64_t result = firstSource * secondSource;

  hiLo[0] = result & 65535;
  hiLo[1] = result & 4294901760;

  return hiLo;
}

/* Access the hi register */
int32_t mfhi(int32_t* hiLo)
{
  return hiLo[1];
}

/* Access the lo register */
int32_t mflo(int32_t* hiLo)
{
  return hiLo[0];
}

/* Applies the logical and operator to the content of two registers */
int32_t and(int32_t firstSource, int32_t secondSource)
{
  int32_t result = firstSource & secondSource;

  return result;
}

/* Applies the logical or operator to the content of two registers */
int32_t or(int32_t firstSource, int32_t secondSource)
{
  int32_t result = firstSource | secondSource;

  return result;
}

/* Applies the logical nor operator to the content of two registers */
int32_t nor(int32_t firstSource, int32_t secondSource)
{
  int32_t result = or(firstSource, secondSource);

  return ~result;
}

/* Shifts the contents of the a register to the left by any given number of 
   bits */
int32_t sll(int32_t firstSource, int shamt)
{
  int32_t result = firstSource << shamt;

  return result;
}

/* Shifts the contents of the a register to the right by any given number of 
   bits */
int32_t srl(int32_t firstSource, int shamt)
{
  int32_t result = firstSource >> shamt;

  return result;
}

/* Loads a value into a register */
void lw(struct TableElt* firstSource, struct Table* memory, int signExtImm)
{
  int index;
  struct TableElt* loadValue;

  /* Each word is 4 bytes long, thus we will store each word by an offset of 4*/
  index = signExtImm / 4;  

  /* Pulls value from memory */
  loadValue = memory -> table[index + offset];

  /* sets the register equal the stored value */
  firstSource -> value = loadValue -> value;

  /* frees the space in memory */
  free(loadValue);
  
}

/* Saves a register value into memory */
void sw(struct TableElt* firstSource, struct Table* memory, int signExtImm)
{
  int index;
  struct TableElt* saveValue;
  saveValue = createTableElt(firstSource -> label, firstSource -> value);

  /* Each word is 4 bytes long, thus we will store each word by an offset of 4*/
  index = signExtImm / 4;  

  /* Stores the content of register into memory */
  memory -> table[index + offset] = saveValue;
}

/* Applies the logical operator less than to two registers and returns
   the boolean result */
int32_t slt(int32_t firstSource, int32_t secondSource)
{
  if(firstSource < secondSource)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

/* Compares the contents of two registers and jumps to the given label 
   if the are NOT equal to each other */
int32_t bne(int32_t firstSource, int32_t secondSource, struct TableElt* label)
{
   if(firstSource != secondSource)
    {
      return label -> value;
    }
  else
    {
      return 0;
    }
}

/* Compares the contents of two registers and jumps to the given label 
   if the are equal to each other */ 
int32_t beq(int32_t firstSource, int32_t secondSource, struct TableElt* label)
{
  if(firstSource == secondSource)
    {
      return label -> value;
    }
  else
    {
      return 0;
    }
}

/* jumps to the given label */
int32_t j(struct TableElt* label)
{
  return label -> value;
}

/* Burns one clock cycle */
void nop()
{
  sll(0, 0);
}

/* Compares the given instruction to see what it should execute and depending on the intructions, we will 
   inintialize the proper inputs. */
void runInstruction(char* instruction, struct TableElt* label, struct TableElt* regiInput[], int numInput[], struct Table* registers, struct Table* memory, FILE*fp, int32_t* hiLo)
{
  int i;
  int jump;
  int shamt;
  int signExtImm;
  struct TableElt* saveValue;
  struct TableElt* destination;
  struct TableElt* firstSource;
  struct TableElt* secondSource;

  if(strcmp(instruction, "add") == 0)
    {
       destination = regiInput[0];
       firstSource = regiInput[1];
       secondSource = regiInput[2];

       destination -> value = add(firstSource -> value,
				       secondSource -> value);
    }
  else if(strcmp(instruction, "addi") == 0)
    {
       destination = regiInput[0];
       firstSource = regiInput[1];
       signExtImm = numInput[0];

       destination -> value = addi(firstSource -> value,
				       signExtImm);
    }
  else if(strcmp(instruction, "sub") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      secondSource = regiInput[2];

      destination -> value = sub(firstSource -> value,
				       secondSource -> value);
    }
  else if(strcmp(instruction, "mult") == 0)
    {
      firstSource = regiInput[0];
      secondSource = regiInput[1];

      hiLo = mult(firstSource -> value, secondSource -> value);
    }
  else if(strcmp(instruction, "mfhi") == 0)
    {
      regiInput[0] -> value = mfhi(hiLo);
    }
  else if(strcmp(instruction, "mflo") == 0)
    {
      regiInput[0] -> value = mflo(hiLo);
    }
  else if(strcmp(instruction, "and") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      secondSource = regiInput[2];

      destination -> value = and(firstSource -> value,
				       secondSource -> value);
    }
  else if(strcmp(instruction, "or") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      secondSource = regiInput[2];

      destination -> value = or(firstSource -> value,
				       secondSource -> value);
    }
  else if(strcmp(instruction, "nor") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      secondSource = regiInput[2];

      destination -> value = nor(firstSource -> value,
				       secondSource -> value);
    }
  else if(strcmp(instruction, "sll") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      shamt = numInput[0];

      destination -> value = sll(firstSource -> value,
				   shamt);
    }
  else if(strcmp(instruction, "srl") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      shamt = numInput[0];

      destination -> value = srl(firstSource -> value,
				   shamt);
    }
  else if(strcmp(instruction, "lw") == 0)
    {
      firstSource = regiInput[0];
      signExtImm = numInput[0];

      lw(firstSource, memory, signExtImm);
    }
  else if(strcmp(instruction, "sw") == 0)
    {
      firstSource = regiInput[0];
      signExtImm = numInput[0];

      sw(firstSource, memory, signExtImm);
    }
   else if(strcmp(instruction, "slt") == 0)
    {
      destination = regiInput[0];
      firstSource = regiInput[1];
      secondSource = regiInput[2];

      destination -> value = slt(firstSource -> value,
				       secondSource -> value);
    }
   else if(strcmp(instruction, "bne") == 0)
    {
      firstSource = regiInput[0];
      secondSource = regiInput[1];

      
      jump = bne(firstSource -> value, secondSource -> value, label);

      if(jump == 0)
	{
	  return;
	}
      else
	{
	  fseek(fp, jump, SEEK_SET);
	}
    }
   else if(strcmp(instruction, "beq") == 0)
    {
      firstSource = regiInput[0];
      secondSource = regiInput[1];
      
      jump = beq(firstSource -> value, secondSource -> value, label);

      if(jump == 0)
	{
	  return;
	}
      else
	{
	  fseek(fp, jump, SEEK_SET);
	}
    }
   else if(strcmp(instruction, "j") == 0)
    {
      jump = j(label);

      if(jump == 0)
	{
	  return;
	}
      else
	{
	  fseek(fp, jump, SEEK_SET);
	}
    }
   else if(strcmp(instruction, "nop") == 0)
     {
       nop();
       return;
     }
   else
     {
       printf("Instruction Not Recognized!");
       return;
     }
}

