#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// define your own types and function prototypes for the symbol table(s) module below
//typedef enum {INTEGER, CHARACTER, BOOLEAN, IDENTIFIER} DataTypes;

typedef enum {METHOD, CLASS, VARIABLE, TYPE} SymbolKinds;

typedef struct {
	char name[128];
    SymbolKinds sKind;
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

extern SymbolTable* root;

SymbolTable* CreateSymbolTable(SymbolTable* parent);
void AddChildSymbolTable(SymbolTable* parent, SymbolTable* child);

void InsertSymbol(SymbolTable* st, char* name, SymbolKinds sKind);
int FindSymbol(SymbolTable* st, char* name);

void PrintSymbolTable(SymbolTable* st);

#endif
