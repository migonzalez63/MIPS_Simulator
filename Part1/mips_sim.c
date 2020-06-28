#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mips_functions.h"

#define TRUE 1
#define FALSE 0

/* Defines a pointer to the register and memory table we will use to simulate our CPU */
struct Table* registers;
struct Table*  memory;

/* Defines our temp register hi and lo. Will be used to store result of mult */
int hiLo[2];

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

/* Determines if the given word is a number by making sire that each character falls into their
   appropiate ASCII values. If the first character of the word is '-', then the number will be 
   negative.
   if it is a number, return true, false otherwise. */
int isNumber(char* word)
{
  int i;
  int numFlag = TRUE;
  int length = strlen(word);

  for(i = 0; i < length-1; i++)
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

/* Scans a MIPS assembly file and detects all labels in the file. Will delimit them by the ':'
   character and then take their position in the file and save that into our memory table */
void parseLabels(char* file)
{
  struct TableElt* label;
  int endFlag = TRUE;
  char* delimiter = ":";
  char fileStr[100];
  char* wordToken;
  FILE* fp;
  int len;
  int i = 0; 

  fp = fopen(file, "r");
  memset(fileStr, 0, 20);

  if(fp == NULL)
    {
      printf("Error opening file. Please try another one. \n");
      exit(1);
    }

  /* Begins reading from file */
  endFlag = fscanf(fp, "%s", fileStr);

  /* While we still have stuff to scan from the file, we will scan a word
     compare the last character to see if it is a label, make that last 
     character \0 in order to only save the name of the label, then add it
     to our memory table with its position in the file. */
  while(endFlag == TRUE)
    {
      len = strlen(fileStr);
      if(strcmp(&fileStr[len-1], delimiter) == 0)
	{
	  fileStr[len-1] = '\000';
	  label = createTableElt(fileStr, ftell(fp));
	  insert(label, memory, i++);
	}
      memset(fileStr, 0, len);
      endFlag = fscanf(fp, "%s", fileStr);
    }

  fclose(fp);
}

/* Scans through the file and begins to execute all intructions it finds. Will ignore any
   lines starting with '#', '.', and '\n' for blank lines. */
void parseInstructions(char* file)
{
  int i;
  int j;
  int skipFlag = FALSE;
  char delimiter[] = " ,\n\t";
  char fileStr[200];
  char* wordToken;
  char* instruction;
  int numInput[3] = {0, 0, 0};
  struct TableElt* label = NULL;
  struct TableElt* regiInput[3] = {NULL, NULL, NULL};
  FILE* fp;

  /* Read in the labels */
  parseLabels(file);

  fp = fopen(file, "r");
				  
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
		  regiInput[i] = lookup(wordToken, registers);
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
		  label = lookup(wordToken, memory);
		}
	      
	      wordToken = strtok(NULL, delimiter);
	    }

	  /* Executes the appropiate instructions with the appropiate inputs */
	  runInstruction(instruction, label, regiInput, numInput, registers, memory, fp, hiLo);

	  /* Resets all our values */
	  for(i = 0; i < 3; i++)
	    {
	      numInput[i] = 0;
	      regiInput[i] = NULL;
	    }
	  label = NULL;
	  instruction = NULL;
	}

      skipFlag = FALSE;
    }

  fclose(fp);
}

int main(int argc, char* argv[])
{
  int i;
  char* fileName;
  char* regiName[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3" ,"$t0", "$t1",
			 "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3",
			 "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp",
			 "$fp", "$ra"};
  struct TableElt* currentRegister;

  if(argc < 2 || argc > 2)
    {
      printf("Too many inputs!.\n");
      exit(1);
    }

  fileName = argv[1];

  /* Initilaizes our register and memory tables */
  registers = createTable(32);
  memory = createTable(512);

  /* Properly labels each register so we can acces them properly */
  for(i = 0; i < registers -> size; i++)
    {
      registers -> table[i] = createTableElt(regiName[i], 0);
    }

  /* Begins parsing intructions from file */
  parseInstructions(fileName);

  printf("Finished Executing Program. Printing Registers: \n");

  /* Prints each register name and content */
  for(i = 0; i< registers -> size; i++)
    {
      currentRegister = registers -> table[i];
      printf("%s: %08x \n", currentRegister -> label, currentRegister -> value);
    }

  /* Frees the register and memory tables */
  freeTable(registers);
  freeTable(memory);  
}
