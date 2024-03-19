
/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Symbol Tables Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name:
Student ID:
Email:
Date Work Commenced:
*************************************************************************/

#include "symbols.h"

Symbol St[100];  // symbol table
int ns; // number of symbols in the table
int addressCounter;

void InsertSymbol(char* name, DataTypes type)
{
	strcpy (St[ns].name , name);
	St[ns].type = type;
	St[ns].address = addressCounter++;
	ns++;
}

int FindSymbol(char* name)
{
	for (int i = 0; i < ns; i++)
	{
		if (!strcmp(St[i].name, name))
		{
			return i; //Return idnex
		}

	}
	return -1; //Index was not found
}

void InitSymbolTable()
{
	ns = 0;
	addressCounter = 0;
}

const char* DataTypeToString(DataTypes t)
{
	if (t == banana)
		return "banana";
	else if (t == bapple)
		return "bapple";
	else if (t == gelato)
		return "gelato";
	else return "unknown data type";
}
void PrintAllSymbols()
{
	printf("\n\nThe symbols of this program:\n");
	for (int i = 0; i < ns; i++)
		printf("%s: %s, %i\n", St[i].name, DataTypeToString(St[i].type), St[i].address);
}

int GetAddress(int r) // return the memory adress of the symbol given by its index r withing the symbol table
{
	return St[r].address;
}