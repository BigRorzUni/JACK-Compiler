
/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Symbol Tables Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Rory Edmonds
Student ID: 201508877
Email: sc22re@leeds.ac.uk
Date Work Commenced: 19/03/2024
*************************************************************************/

#include "symbols.h"

SymbolTable* root = NULL;

SymbolTable* CreateSymbolTable(SymbolTable* parent) 
{
    SymbolTable* newNode = malloc(sizeof(SymbolTable));
    newNode->ns = 0;
    newNode->addressCounter = 0;
    newNode->parent = parent;
    return newNode;
}


void InsertSymbol(SymbolTable* st, Token tk, SymbolTable* scope, SymbolType sType) 
{
    st->symbols[st->ns].token = tk;
    st->symbols[st->ns].sType = sType;
    st->symbols[st->ns].scope = scope;
    st->symbols[st->ns].address = st->addressCounter++;
    st->ns++;
}

int FindSymbol(SymbolTable* st, Token tk) 
{
    if (st == NULL) {
        return -1;
    }
    for (int i = 0; i < st->ns; i++) {
        if (strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            return i;
        }
    }
    // If the symbol was not found in the current symbol table, search for it in the parent
    if (st->parent != NULL) 
    {
        int parentResult = FindSymbol(st->parent, tk);
        if (parentResult != -1) {
            return parentResult;
        }
    }
    return -1;
}

char* SymbolKindString(SymbolType sType) 
{
    switch (sType) 
    {
        case METHOD: return "METHOD";
        case CLASS: return "CLASS";
        case CLASS_VAR: return "CLASS_VAR";
        case VAR: return "VAR";
        case OBJECT: return "OBJECT";
    }
    return "UNKNOWN";
}

void PrintSymbolTable(SymbolTable* st) 
{
    if (st == NULL) 
    {
        return;
    }

    // First, print the current symbol table
    printf("SYMBOL TABLE:\n");
    printf("num symbols: %d\n", st->ns);
    printf("addressCounter: %d\n", st->addressCounter);
    
    for (int i = 0; i < st->ns; i++) 
    {
        printf("-----------------------\n");
        printf("Symbol %d:\n", i);
        printf("token lx: %s\n", st->symbols[i].token.lx);
        printf("symbol type: %s\n", SymbolKindString(st->symbols[i].sType));
        printf("address: %d\n", st->symbols[i].address);
        if(st->symbols[i].scope != NULL)
            printf("scope: TRUE\n");
        else
            printf("scope: FALSE\n");
    }

    // Then, print the parent symbol table
    /*
    if (st->parent != NULL) 
    {
        PrintSymbolTable(st->parent);
    }
    */
}

void ShallowPass(char* filename, SymbolTable* st) 
{
    InitLexer(filename);
    
    Token t;
    SymbolTable* classTable;

    while(t.tp != EOFile) 
    {
        t = GetNextToken();

        if (t.tp == RESWORD) 
        {
            if (strcmp(t.lx, "class") == 0) 
            {
                t = GetNextToken();
                classTable = CreateSymbolTable(st);
                InsertSymbol(st, t, classTable, CLASS);
                //printf("class table added for %s\n", t.lx);
            }
            else if(strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0)
            {
                t = GetNextToken();
                if(t.tp == RESWORD && (strcmp(t.lx, "void") == 0 || strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
                    ;
                
                t = GetNextToken();
                if (t.tp == ID)
                {
                    SymbolTable* methodTable = CreateSymbolTable(st);
                    InsertSymbol(classTable, t, methodTable, METHOD);
                    //printf("method table added for %s\n", t.lx);
                }
            }
            else if(strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0)
            {
                t = GetNextToken();
                if(t.tp == ID || (t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)))
                    ;
                
                t = GetNextToken();
                if(t.tp == ID)
                {
                    InsertSymbol(classTable, t, NULL, CLASS_VAR);
                    //printf("class variable %s has been added\n", t.lx);

                    t = PeekNextToken();
                    while(t.tp != EOF && strcmp(t.lx, ";") != 0)
                    {
                        t = GetNextToken();
                        if(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
                        {
                            t = GetNextToken();
                            if(t.tp == ID)
                            {
                                InsertSymbol(classTable, t, NULL, CLASS_VAR);
                                //printf("class variable %s has been added\n", t.lx);
                            }
                        }

                        t = PeekNextToken();
                    }
                }
            }
        }
    }

    StopLexer();

    PrintSymbolTable(classTable);
}