/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Compiler Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name:
Student ID:
Email:
Date Work Commenced:
*************************************************************************/

#include "compiler.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>

/*
take directory as input

for each file in directory add its name to a program wide symbol table then parse it

when parser is initialised for each file create a child symbol table for the associated file

when a function is defined create a child symbol table for it

find out how to stay in the lowest scope possible maybe a global scope variable (initialised as the class symbol table)

when a function scope is exited then change lowest scope var to its parent

when a variable is created check from the lowest to highest scope for its name
	if it is not in there then add it to the lowest scope symbol table
	if it is then return an error

when a variable is assigned check from lowest to highest for its presence
	if it is not there then error
	if it is then continue



add scaffold symbol output to parser

*/


int InitCompiler ()
{
	// make initial symbol table
	//printf("program wide symbol table created!\n");
	return 1;
}

ParserInfo compile (char* dir_name)
{
	ParserInfo p;

	DIR* dir;
    struct dirent* entry;

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("Unable to read directory");
		p.er = lexerErr;
        return p;
    }

	char *filePaths[1024]; // Array to store file paths
	int filePathIndex = 0; // Index to keep track of the next empty spot in the array

	while ((entry = readdir(dir)) != NULL) 
	{
		char *ext = strrchr(entry->d_name, '.');
		if (ext != NULL && strcmp(ext, ".jack") == 0) 
		{
			char *filePath = malloc(1024 * sizeof(char));
			snprintf(filePath, 1024, "%s/%s", dir_name, entry->d_name);
			filePaths[filePathIndex++] = filePath; // Store the file path in the array
		}
	}

	root = CreateSymbolTable(NULL);

	//printf("\n\nSHALLOW PARSING EACH FILE\n\n\n");

	// Add library files to the symbol table
	
	//p = ShallowPass("Pong/Ball.jack", root);

	
	// Parse each file
    for (int i = 0;i < filePathIndex; i++) 
	{
		

		//printf("\nSHALLOW PARSING %s\n", filePaths[i]);
		//char* check = filePaths[i];
		
		
		//fflush(stdout);
		p = ShallowPass(filePaths[i], root);

		if(p.er != none)
		{
			//printf("qhy has it stopped\n");
			break;
		}
		
		//printf("\n\n");
	}

	AddLibrarySymbols();

	//*/


    rewinddir(dir);

	//PrintSymbolTable(root);

	//printf("\n\nSHALLOW PARSING COMPLETE\n\n======================================");

	//printf("\n\nDEEP PARSING PONG/BALL.JACK\n\n\n");

	//InitParser("Pong/Ball.jack");
	//p = Parse();
	//StopParser();

	//printf("\n\nDEEP PARSING EACH FILE\n\n\n");


	for (int i = 0; i < filePathIndex; i++) 
	{
		if(p.er != none)
		{
			break;
		}
		//printf("\nDEEP PARSING %s\n", filePaths[i]);
		InitParser(filePaths[i]);
		p = Parse();
		StopParser();
    }
	
	//printf("\n\nDEEP PARSING COMPLETE\n\n");

	//PrintSymbolTable(root);

	closedir(dir);

	// Free each file path in the array
	for (int i = 0; i < filePathIndex; i++) 
	{
		free(filePaths[i]);
	}

	return p;
}

int StopCompiler ()
{


	return 1;
}

char* GetErrorString(SyntaxErrors e)
{
	{
	switch(e)
	{
		case(lexerErr):
			return "lexer error";
		case(classExpected):
			return "keyword class expected";
		case(idExpected):
			return "identifier expected";
		case(openBraceExpected):
			return "{ expected";
		case(closeBraceExpected):
			return "} expected";
		case(memberDeclarErr):
			return "class member declaration must begin with static, field, constructor , function , or method";
		case(classVarErr):
			return "class variables must begin with field or static";
		case(illegalType):
			return "a type must be int, char, boolean, or identifier";
		case(semicolonExpected):
			return "; expected";
		case(subroutineDeclarErr):
			return "subrouting declaration must begin with constructor, function, or method";
		case(openParenExpected):
			return "( expected";
		case(closeParenExpected):
			return ") expected";
		case(closeBracketExpected):
			return "] expected";
		case(equalExpected):
			return "= expected";
		case(syntaxError):
			return "any other kind of syntax error";
		case(undecIdentifier):
			return "undeclared identifier (e.g. class, subroutine, or variable)";
		case(redecIdentifier):
			return "redeclaration of identifier in the same scope";
		default:
			return "no errors :)";
	}
}
}

void OutputError(ParserInfo p)
{
	if (p.er == none)
		printf ("No errors\n");
	else
		printf ("Error in file %s line %i at or near %s: %s\n" , p.tk.fl , p.tk.ln , p.tk.lx , GetErrorString(p.er));
}


#ifndef TEST_COMPILER
int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("Please provide a directory name.\n");
        return 1;
    }
	InitCompiler ();
	//printf("--------------\n");
	ParserInfo p = compile (argv[1]);
	OutputError (p);
	StopCompiler ();
	return 1;
}
#endif
