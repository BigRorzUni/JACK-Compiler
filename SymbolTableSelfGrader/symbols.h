#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>

// define your own types and function prototypes for the symbol table(s) module below
typedef enum {INT, CHAR, BOOL, IDENTIFIER} DataTypes;

typedef struct {
	char name[128];
	DataTypes type;
	int address;
} Symbol;

typedef struct SymbolTable {
    Symbol symbols[100];
    int ns;
    int addressCounter;
    struct SymbolTable* parent;
    struct SymbolTable** children;
    int childCount;
} SymbolTable;

extern SymbolTable* head;

void PushSymbolTable();
void PopSymbolTable();
void InsertSymbol(char* name, DataTypes type);
int FindSymbol(char* name);

#endif
