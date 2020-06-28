#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "table.h"

/* Creates a Table */ 
struct Table* createTable(int size)
{
  int i;
  struct Table* newTable = malloc(sizeof(struct Table));
  newTable -> size = size;
  for(i = 0; i < size; i++)
    {
      newTable -> table[i] = NULL;
    }

  return newTable;
}

/* Creates a table element ot be added to the Table */
struct TableElt* createTableElt(char* label, int32_t value)
{
  struct TableElt* newHashElt = (struct TableElt*) malloc(sizeof(struct TableElt));
  newHashElt -> label = malloc(sizeof(label));
  strcpy(newHashElt -> label, label);
  newHashElt -> value = value;
  
  return newHashElt;
}

/* Inserts an element into the table */
void insert(struct TableElt* elt, struct Table* table, int index)
{  
  while(table -> table[index] != NULL)
    {
      if(index >= table -> size)
	{
	  index = 0;
	}

      index++;
    }
 
  table -> table[index] = elt;
}

/* Looks up a table element and returns its value */
struct TableElt* lookup(char* key, struct Table* table)
{
  int i = 0;
  struct TableElt* returnElt = NULL;
  struct TableElt* keyElt = createTableElt(key, 0);

  for(i = 0; i < table -> size; i++)
    {
      if(table -> table[i] == NULL)
	{
	  continue;
	}

      if(strcmp(keyElt -> label, table -> table[i] -> label) == 0)
	{
	  returnElt = table -> table[i];
	}
    }

  return returnElt;
}

/* Frees the memory used by the table */
void freeTable(struct Table* table)
{
  int i;
  int tableSize = table -> size;

  for(i = 0; i < tableSize; i++)
    {
      if(table -> table[i] != NULL)
	{
	  free(table -> table[i]);
	  table -> table[i] = NULL;
	}
    }

  free(table);
  
}
