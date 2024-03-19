
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

SymbolTable* head = NULL;

void PushSymbolTable() {
    SymbolTable* newTable = malloc(sizeof(SymbolTable));
    newTable->ns = 0;
    newTable->addressCounter = 0;
    newTable->next = head;
    head = newTable;
}

void PopSymbolTable() {
    SymbolTable* oldHead = head;
    head = head->next;
    free(oldHead);
}

void InsertSymbol(char* name, DataTypes type) {
    strcpy(head->symbols[head->ns].name, name);
    head->symbols[head->ns].type = type;
    head->symbols[head->ns].address = head->addressCounter++;
    head->ns++;
}

int FindSymbol(char* name) {
    SymbolTable* currentTable = head;
    while (currentTable != NULL) {
        for (int i = 0; i < currentTable->ns; i++) {
            if (strcmp(currentTable->symbols[i].name, name) == 0) {
                return i;
            }
        }
        currentTable = currentTable->next;
    }
    return -1;
}