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


int InitCompiler ()
{
	return 1;
}

ParserInfo compile (char* dir_name)
{
	ParserInfo p;

	// write your code below


	p.er = none;
	return p;
}

int StopCompiler ()
{


	return 1;
}

char* ErrorString(SyntaxErrors e)
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
		default:
			return "no errors :)";
	}
}
}

void PrintError(ParserInfo p)
{
	if (p.er == none)
		printf ("No errors\n");
	else
		printf ("Error in file %s line %i at or near %s: %s\n" , p.tk.fl , p.tk.ln , p.tk.lx , ErrorString(p.er));
}


#ifndef TEST_COMPILER
int main ()
{
	InitCompiler ();
	ParserInfo p = compile ("Pong");
	PrintError (p);
	StopCompiler ();
	return 1;
}
#endif
