#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// define your own types and function prototypes for the symbol table(s) module below

typedef enum {METHOD, CLASS, CLASS_VAR, VAR, ARG} SymbolType;

typedef enum {INTEGER, CHARACTER, BOOLEAN, OBJECT, VOID} DataType;

typedef struct SymbolTable SymbolTable;

typedef struct {
	Token token;
    SymbolType sType;
    DataType dType;
	int address;
    SymbolTable* scope;
} Symbol;

struct SymbolTable {
    Symbol symbols[100];
    int ns;
    int addressCounter;
    SymbolTable* parent;
};



extern SymbolTable* root;

SymbolTable* CreateSymbolTable(SymbolTable* parent);
SymbolTable* GetSymbolTable(SymbolTable* st, Token tk);
SymbolTable* ExitScope(SymbolTable* st);

void InsertSymbol(SymbolTable* st, Token t, SymbolTable* scope, SymbolType sType, DataType dType);
int FindSymbol(SymbolTable* st, Token t);
void AddLibrarySymbols();
int CheckScopeForSymbol(SymbolTable* st, Token t);

void AssignScopeToInstance(SymbolTable* st, Token t, SymbolTable* scope);

int TokenToDataType(Token t);

void PrintSymbolTable(SymbolTable* st);

ParserInfo ShallowPass(char* filename, SymbolTable* st);

#endif
