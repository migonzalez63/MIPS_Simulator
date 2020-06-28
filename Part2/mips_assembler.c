#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mips_funct_asm.h"

#define TRUE 1
#define FALSE 0

/* Defines a pointer to a label table to hold the addressed of labels and 
   the 32 bit machine instruction */
struct Table* labels;
struct Table* machineInstr;

/* Program counter. Used to identify addressed of instrucitons */
int PC;

/* Determines if a given word is an instruction by comparing it to all possible instructions we
   can execute.
   If it is an instruction, returns tru, otherwise false. */
int isInstruction(char* word)
{
  char* instructions[18] = {"add", "addi", "sub", "mult", "mfhi", "mflo", "and", "or",
			   "nor", "sll", "srl", "lw", "sw", "slt", "bne", "beq", "j", "nop"};

  for(int i = 0; i < 18; i++)
    {
      if(strcmp(instructions[i], word) == 0)
	{
	  return TRUE;
	}
    }

  return FALSE;
}

/* Determines if the given word is a register by comparing the first character of the word with '$',
   since all registers can be identifies by that character.
   If it is a register, returns true, false otherwise. */
int isRegister(char* word)
{
  char regiDelim = '$';

  if(word[0] == regiDelim)
    {
      return TRUE;
    }

  return FALSE;
}

/* Takes a register and returns its index */
int regiToNum(char* word)
{
  int i;
  char* regiName[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3" ,"$t0", "$t1",
			"$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3",
			"$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp",
			"$fp", "$ra"};

  for(i = 0; i < 32; i++)
    {
      if(strcmp(regiName[i], word) == 0)
	{
	  return i;
	}
    }

  return 0;
}

/* Determines if the given word is a number by making sire that each character falls into their
   appropiate ASCII values. If the first character of the word is '-', then the number will be 
   negative.
   if it is a number, return true, false otherwise. */
int isNumber(char* word)
{
  int i;
  int numFlag = TRUE;
  int length = strlen(word);

  for(i = 0; i < length; i++)
    {
      if(word[i] < 48 || word[i] > 57)
	{
	  numFlag = FALSE;
	}
    }

  if(word[0] == '-')
    {
      numFlag = TRUE;
    }

  return numFlag;
}

/* Identifies if a given word is a hex number */
int isHex(char* word)
{
  char delim = 'x';

  if(word[1] == delim)
    {
      return TRUE;
    }

  return FALSE;
}

/* Converts a string into a number. First will determine if the number is negative, then
   it will convert the number and return it with its appropiate sign */
int numConverter(char* word)
{
  int i;
  int number;
  int negFlag = FALSE;

  if(word[0] == '-')
    {
      number = atoi(&word[1]);
      return number * (-1);
    }

  number = atoi(word);

  return number;
}


/* Changes the machine instruction to a 32 bit binary number */
void decimalToBinary(int32_t dec)
{
  int j;
  // array to store binary number
  int binaryNum[32] = {0};
  int buffer;
  
  // counter for binary array
  int i = 0;

  printf("Binary: ");
  
  if(dec <= 0)
    {
      dec = 0;
      printf("%032d\n", dec);
      return;
    }
  while (dec > 0)
    {
      // storing remainder in binary array
      binaryNum[i] = dec % 2;
      dec = dec / 2;
      i++;
    }
  
  for(j = 31; j >= 0; j--)
    {
      printf("%d", binaryNum[j]);
    }

  printf("\n");
}

/* Scans a MIPS assembly file and detects all labels in the file. Will delimit them by the ':'
   character and then take their position in the file and save that into our memory table */
void parseLabels(char* file)
{
  int i = 0;
  struct TableElt* label;
  int skipFlag = FALSE;
  int endFlag = TRUE;
  char delimiter[] = " ,\n\t";
  char fileStr[100];
  char* wordToken;
  FILE* fp;
  int len;

  fp = fopen(file, "r");
  memset(fileStr, 0, 100);

  if(fp == NULL)
    {
      printf("Error opening file. Please try another one. \n");
      exit(1);
    }

  PC = 0;

  /* Reads in each line and if it contains a label, it will save the address
     to it */
  while(fgets(fileStr, 100, fp) != NULL)
    {
      if(fileStr[0] == '\n' || fileStr[0] == '#' || fileStr[0] == '.')
	{
	  skipFlag = TRUE;
	}

      if(skipFlag == FALSE)
	{
      	  wordToken = strtok(fileStr, delimiter);

	  while(wordToken != NULL)
	    {
	      len = strlen(wordToken);
	      
	      if(strcmp(&wordToken[len-1], ":") == 0)
		{
		  wordToken[len-1] = '\000';
		  label = createTableElt(wordToken, PC);
		  insert(label, labels, i++);
		}
	      memset(fileStr, 0, len);
	      wordToken = strtok(NULL, delimiter);
	    }

	  PC++;
	}
      
      skipFlag = FALSE;
    }
  
  fclose(fp);
}

/* Scans through the file and begins to execute all intructions it finds. Will ignore any
   lines starting with '#', '.', and '\n' for blank lines. */
void parseInstructions(char* file)
{
  int i;
  int j;
  int k;
  int result;
  int skipFlag = FALSE;
  char delimiter[] = " ,\n\t";
  char fileStr[200];
  char* wordToken;
  char* instruction;
  int numInput[3] = {0, 0, 0};
  int regiInput[3] = {0, 0, 0};
  struct TableElt* instrWord = NULL;
  struct TableElt* label = NULL;
  FILE* fp;

  /* Read in the labels */
  parseLabels(file);

  fp = fopen(file, "r");

  PC = 0;
				  
  label = NULL;
  instruction = NULL;

  /* Begin reading in from the file line by line */
  while(fgets(fileStr, 200, fp) != NULL)
    {
      /* Will ignore and discard line if it begins with '#', '.', and '\n',
	 since they are not instructions */
      if(fileStr[0] == '#' || fileStr[0] == '.' || fileStr[0] == '\n')
	{
	  skipFlag = TRUE;
	  memset(fileStr, 0, strlen(fileStr));
	}

      if(skipFlag == FALSE)
	{
	  i = 0;
	  j = 0;

	  /* Tokenizes each word in the line */
	  wordToken = strtok(fileStr, delimiter);

	  while(wordToken != NULL)
	    {
	      /* Checks if word is instruction */
	      if(isInstruction(wordToken) == TRUE)
		{
		  instruction = wordToken;
		}
	      /* Checks if word is Regsiter */
	      else if(isRegister(wordToken) == TRUE)
		{
		  regiInput[i] = regiToNum(wordToken);
		  i++;
		}
	      /* Checks if word is number and converts it */
	      else if(isNumber(wordToken) == TRUE)
		{
		  numInput[j] = numConverter(wordToken);
		  j++;
		}
	      /* Checks if the word is hex and converts it */
	      else if(isHex(wordToken) == TRUE)
		{
		  numInput[j] = (int) strtol(wordToken, NULL, 16);
		}
	      /* If it is not any of the previous values, then it 
		 must be a label */
	      else
		{
		  label = lookup(wordToken, labels);
		}
	      
	      wordToken = strtok(NULL, delimiter);
	    }

	  /* Executes the appropiate instructions with the appropiate inputs */
	  result = createMachineInstruction(instruction, regiInput, numInput, label, PC);
	  instrWord = createTableElt(instruction, result);

	  insert(instrWord, machineInstr, PC); 

	  /* Resets all our values */
	  for(i = 0; i < 3; i++)
	    {
	      numInput[i] = 0;
	      regiInput[i] = 0;
	    }
	  label = NULL;
	  instruction = NULL;

	  PC++;
	}

      memset(fileStr, 0, 100);
      skipFlag = FALSE;
    }

  fclose(fp);
}

int main(int argc, char* argv[])
{
  int i;
  unsigned long binaryNum;
  char* fileName;

  if(argc < 2 || argc > 2)
    {
      printf("Too many inputs!.\n");
      exit(1);
    }

  fileName = argv[1];
  
  /* Initilaizes our register and memory tables */
  labels = createTable(100);
  machineInstr = createTable(100);
  
  /* Begins parsing intructions from file */
  parseInstructions(fileName);

  i = 0;

  /* Converts the machine instruction into a binary or hex value */
  while(machineInstr -> table[i] != NULL)
    {
      /* For binary conversion */
      //decimalToBinary(machineInstr -> table[i] -> value);
      printf("%08x\n", machineInstr -> table[i] -> value);
      i++;
    }
  
  /* Frees the labels and machine instruction tables */
  freeTable(labels);
  freeTable(machineInstr);  
}
