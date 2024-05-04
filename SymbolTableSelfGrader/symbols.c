
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


void InsertSymbol(SymbolTable* st, Token tk, SymbolTable* scope, SymbolType sType, DataType dType) 
{
    st->symbols[st->ns].token = tk;
    st->symbols[st->ns].sType = sType;
    st->symbols[st->ns].dType = dType;
    st->symbols[st->ns].scope = scope;
    st->symbols[st->ns].address = st->addressCounter++;
    st->ns++;
}

SymbolTable* GetSymbolTable(SymbolTable* st, Token tk) 
{
    if (st == NULL) 
    {
        return NULL;
    }
    for (int i = 0; i < st->ns; i++) 
    {
        if (strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            return st->symbols[i].scope;
        }
    }
    // If the symbol was not found in the current symbol table, search for it in the parent
    if (st->parent != NULL) 
    {
        return GetSymbolTable(st->parent, tk);
    }
    return NULL;
}

SymbolTable* ExitScope(SymbolTable* st) 
{
    if(st->parent != NULL)
        return st->parent;
}

int FindSymbol(SymbolTable* st, Token tk) 
{
    if (st == NULL) 
    {
        printf("Symbol table error when trying to find symbol %s at %d\n", tk.lx, tk.ln);
        return -1;
    }

    //printf("searching table for %s\n", tk.lx);
    for (int i = 0; i < st->ns; i++) 
    {
        //printf("comparing %s to %s\n", st->symbols[i].token.lx, tk.lx);
        if (strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            //printf("found symbol %s at %d in the table\n", tk.lx, i);
            return 1;
        }
    }
    // If the symbol was not found in the current symbol table, search for it in the parent
    if (st->parent != NULL) 
    {
        printf("searching parent table\n");
        return FindSymbol(st->parent, tk);
    }
    return 0;
}

char* SymbolTypeString(SymbolType sType) 
{
    switch (sType) 
    {
        case METHOD: return "METHOD";
        case CLASS: return "CLASS";
        case CLASS_VAR: return "CLASS_VAR";
        case VAR: return "VAR";
        case ARG: return "ARG";
    }
    return "UNKNOWN";
}

char* DataTypeString(DataType dType) 
{
    switch (dType) 
    {
        case INTEGER: return "INT";
        case CHARACTER: return "CHAR";
        case BOOLEAN: return "BOOL";
        case OBJECT: return "OBJECT";
        case VOID: return "VOID";
    }
    return "UNKNOWN";
}

int TokenToDataType(Token tk)
{
    if(tk.tp == RESWORD)
    {
        if(strcmp(tk.lx, "int") == 0)
            return INTEGER;
        else if(strcmp(tk.lx, "char") == 0)
            return CHARACTER;
        else if(strcmp(tk.lx, "boolean") == 0)
            return BOOLEAN;
        else if(strcmp(tk.lx, "void") == 0)
            return VOID;
    }
    
    if(tk.tp == ID)
        return OBJECT;

    return -1;
}

void PrintSymbolTable(SymbolTable* st) 
{
    if (st == NULL) 
    {
        printf("Symbol table is NULL\n");
        return;
    }

    // First, print the current symbol table
    printf("SYMBOL TABLE:\n");
    printf("num symbols: %d\n", st->ns);
    printf("addressCounter: %d\n", st->addressCounter);
    printf("-----------------------------\n");
    
    for (int i = 0; i < st->ns; i++) 
    {
        printf("Symbol: %d, ", i);
        printf("token lx: %s, ", st->symbols[i].token.lx);
        printf("symbol type: %s, ", SymbolTypeString(st->symbols[i].sType));
        printf("data type: %s, ", DataTypeString(st->symbols[i].dType));
        printf("address: %d, ", st->symbols[i].address);
        if(st->symbols[i].scope != NULL)
        {
            printf("scope: TRUE\n");
            //printf("scope: TRUE\n****************************\n");
            //PrintSymbolTable(st->symbols[i].scope);
            //printf("END OF SCOPE TABLE\n****************************\n");
        }
        else
            printf("scope: FALSE\n");
    }
    printf("-----------------------------\n");
}

void ShallowPass(char* filename, SymbolTable* st) 
{
    InitLexer(filename);
    
    Token t;
    int type;
    SymbolTable* classTable = NULL;

    while(t.tp != EOFile) 
    {
        t = GetNextToken();

        if (t.tp == RESWORD) 
        {
            if (strcmp(t.lx, "class") == 0) 
            {
                t = GetNextToken();
                classTable = CreateSymbolTable(st);
                InsertSymbol(st, t, classTable, CLASS, OBJECT);
                //printf("class table added for %s\n", t.lx);
            }
            else if(strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0)
            {
                t = GetNextToken();
                if(t.tp == RESWORD && (strcmp(t.lx, "void") == 0 || strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
                    type = TokenToDataType(t);
                
                t = GetNextToken();
                if (t.tp == ID)
                {
                    SymbolTable* methodTable = CreateSymbolTable(classTable);
                    InsertSymbol(classTable, t, methodTable, METHOD, type);
                    //printf("method table added for %s\n", t.lx);
                }
            }
            else if(strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0)
            {
                t = GetNextToken();
                if(t.tp == ID || (t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)))
                    type = TokenToDataType(t);
                
                t = GetNextToken();
                if(t.tp == ID)
                {
                    InsertSymbol(classTable, t, NULL, CLASS_VAR, type);
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
                                InsertSymbol(classTable, t, NULL, CLASS_VAR, type);
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
}