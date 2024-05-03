#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// define your own types and function prototypes for the symbol table(s) module below

typedef enum {METHOD, CLASS, CLASS_VAR, VAR, OBJECT} SymbolType;

typedef struct SymbolTable SymbolTable;

typedef struct {
	Token token;
    SymbolType sType;
	int address;
    SymbolTable* scope;
} Symbol;

struct SymbolTable {
    Symbol symbols[100];
    int ns;
    int addressCounter;
    struct SymbolTable* parent;
};



extern SymbolTable* root;

SymbolTable* CreateSymbolTable(SymbolTable* parent);
void AddChildSymbolTable(SymbolTable* parent, SymbolTable* child);

void InsertSymbol(SymbolTable* st, Token t, SymbolTable* scope, SymbolType sType);
int FindSymbol(SymbolTable* st, Token t);

void PrintSymbolTable(SymbolTable* st);

void ShallowPass(char* filename, SymbolTable* st);

#endif
