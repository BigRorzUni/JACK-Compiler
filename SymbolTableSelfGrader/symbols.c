
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

SymbolTable* CreateSymbolTableNode(SymbolTable* parent) {
    SymbolTable* newNode = malloc(sizeof(SymbolTable));
    newNode->ns = 0;
    newNode->addressCounter = 0;
    newNode->parent = parent;
    newNode->children = NULL;
    newNode->childCount = 0;
    return newNode;
}

void AddChildSymbolTable(SymbolTable* parent, SymbolTable* child) {
    parent->children = realloc(parent->children, sizeof(SymbolTable*) * (parent->childCount + 1));
    parent->children[parent->childCount] = child;
    parent->childCount++;
}

void InsertSymbol(SymbolTable* node, char* name, DataTypes type) {
    strcpy(node->symbols[node->ns].name, name);
    node->symbols[node->ns].type = type;
    node->symbols[node->ns].address = node->addressCounter++;
    node->ns++;
}

int FindSymbol(SymbolTable* node, char* name) {
    if (node == NULL) {
        return -1;
    }
    for (int i = 0; i < node->ns; i++) {
        if (strcmp(node->symbols[i].name, name) == 0) {
            return i;
        }
    }
    for (int i = 0; i < node->childCount; i++) {
        int childResult = FindSymbol(node->children[i], name);
        if (childResult != -1) {
            return childResult;
        }
    }
    return -1;
}