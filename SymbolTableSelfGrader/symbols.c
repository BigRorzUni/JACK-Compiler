
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
    newNode->children = NULL;
    newNode->childCount = 0;
    return newNode;
}

void AddChildSymbolTable(SymbolTable* parent, SymbolTable* child) 
{
    SymbolTable** newChildren = malloc(sizeof(SymbolTable*) * (parent->childCount + 1));
    for (int i = 0; i < parent->childCount; i++) 
    {
        newChildren[i] = parent->children[i];
    }
    newChildren[parent->childCount] = child;
    free(parent->children);
    parent->children = newChildren;
    parent->childCount++;
}

void InsertSymbol(SymbolTable* st, char* name, SymbolKinds sKind) 
{
    strcpy(st->symbols[st->ns].name, name);
    st->symbols[st->ns].sKind = sKind;
    st->symbols[st->ns].address = st->addressCounter++;
    st->ns++;
}

int FindSymbol(SymbolTable* st, char* name) 
{
    if (st == NULL) {
        return -1;
    }
    for (int i = 0; i < st->ns; i++) {
        if (strcmp(st->symbols[i].name, name) == 0) 
        {
            return i;
        }
    }
    for (int i = 0; i < st->childCount; i++) 
    {
        int childResult = FindSymbol(st->children[i], name);
        if (childResult != -1) {
            return childResult;
        }
    }
    return -1;
}

char* SymbolKindString(SymbolKinds sKind) 
{
    switch (sKind) 
    {
        case METHOD: return "METHOD";
        case CLASS: return "CLASS";
        case VARIABLE: return "VARIABLE";
        case TYPE: return "TYPE";
    }
    return "UNKNOWN";
}


void PrintSymbolTable(SymbolTable* st) 
{
    if (st == NULL) 
    {
        return;
    }

    printf("SYMBOL TABLE:\n");
    printf("num symbols: %d\n", st->ns);
    printf("addressCounter: %d\n", st->addressCounter);
    printf("childCount: %d\n", st->childCount);
    

    for (int i = 0; i < st->ns; i++) 
    {
        printf("-----------------------\n");
        printf("Symbol %d:\n", i);
        printf("name: %s\n", st->symbols[i].name);
        printf("symbol kind: %s\n", SymbolKindString(st->symbols[i].sKind));
        printf("address: %d\n", st->symbols[i].address);
    }

    for (int i = 0; i < st->childCount; i++) 
    {
        printf("\n***********************\n\n");
        PrintSymbolTable(st->children[i]);
    }
}