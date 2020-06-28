#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>

struct TableElt
{
  char* label;
  int32_t value;
};

struct Table
{
  int size;
  struct TableElt* table[512];
};

/* Creates a table element ot be added to the Table */
struct TableElt* createTableElt(char* label, int32_t value);

/* Creates a Table */ 
struct Table* createTable(int size);

/* Inserts an element into the table */
void insert(struct TableElt* elt, struct Table* table, int index);

/* Looks up a table element and returns its value */ 
struct TableElt* lookup(char* key, struct Table* table);

/* Frees the memory used by the table */
void freeTable(struct Table* table);

#endif
