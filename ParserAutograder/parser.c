#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// GLOBAL VARIABLES
Token t;
ParserInfo pi;

// GRAMMAR DECLARATIONS

ParserInfo classDeclar(); // class { memberDeclar* }
ParserInfo memberDeclar(); // classVarDeclar | subroutineDeclar
ParserInfo classVarDeclar(); // (static | field ) type varName (, varName)* ;
ParserInfo type(); // int | char | boolean | identifier
ParserInfo subroutineDeclar();
ParserInfo paramList();
ParserInfo subroutineBody();
ParserInfo statement();
ParserInfo varDeclarStatement();
ParserInfo letStatement();
ParserInfo ifStatement();
ParserInfo whileStatement();
ParserInfo doStatement();
ParserInfo subroutineCall();
ParserInfo expressionList();
ParserInfo returnStatement();
ParserInfo expression();
ParserInfo relationalExpression();
ParserInfo arithmeticExpression();
ParserInfo term();
ParserInfo factor();
ParserInfo operand();

// GRAMMAR IMPLEMENTATIONS
ParserInfo classDeclar()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "class") == 0)
		printf("class detected\n");
	else
		return (ParserInfo){classExpected, t};
	t = GetNextToken();
	if(t.tp == ID)
		printf("className detected\n");
	else
		return (ParserInfo){idExpected, t};
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
		printf("open brace detected\n");
	else
		return (ParserInfo){openBraceExpected, t};
	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{
		
		pi = memberDeclar();
		if(pi.er != none)
			return pi;
	}
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
		printf("end of class\n");
	else
		return (ParserInfo){closeBraceExpected, t};
		
	/*
	t = GetNextToken();
	if(t.tp == EOF)
		printf("EOF\n");
	else
		return (ParserInfo){syntaxError, t}; // no EOF token detected
	*/
	return (ParserInfo){none, t};
}

ParserInfo memberDeclar()
{
	t = GetNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0))
	{
		printf("classVarDeclar detected\n");
		pi = classVarDeclar();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0))
	{
	
		printf("subroutineDeclar detected\n");
		pi = subroutineDeclar();
		if(pi.er != none)
			return pi;
	}
	else
		return (ParserInfo){memberDeclarErr, t};

	return (ParserInfo){none, t};

}

ParserInfo classVarDeclar()
{
	pi = type();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	
	if(t.tp == ID)
		printf("varName or subroutineName detected\n");
	else
		return (ParserInfo){idExpected, t};

	while(t.tp != EOF && strcmp(t.lx, ";") != 0)
	{
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
		{
			t = GetNextToken();
			if(t.tp == ID)
				printf("varName detected\n");
			else
				return (ParserInfo){idExpected, t};
		}
		else if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		{
			return (ParserInfo){none, t};
			printf("end of classVarDeclar\n");
		}
		else
			return (ParserInfo){semicolonExpected, t};
	}

	return (ParserInfo){none, t};
}

ParserInfo type()
{
	t = GetNextToken();

	if(t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
	{
		printf("primitive type detected\n");
		return (ParserInfo){none, t};
	}
	else if(t.tp == ID)
	{
		printf("user-defined type detected\n");
		return (ParserInfo){none, t};
	}
	else
	{
		return (ParserInfo){syntaxError, t};
	}
}


ParserInfo subroutineDeclar()
{
	t = GetNextToken();
	printf("%s\n", t.lx);
	if(t.tp == RESWORD && strcmp(t.lx, "void") == 0)
	{
		t = GetNextToken();
		printf("void detected\n");
	}
	else if (t.tp == ID)
	{
		pi = type();
		if(pi.er != none)
			return pi;
	}
	else
		return (ParserInfo){illegalType, t};
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
	{
		printf("open paren detected\n");
		pi = paramList();
		if(pi.er != none)
			return pi;
	}
	else
		return (ParserInfo){openParenExpected, t};
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
	{
		printf("close paren detected\n");
	}
	else
		return (ParserInfo){closeParenExpected, t};
	
	pi = subroutineBody();
	if(pi.er != none)
		return pi;

	return (ParserInfo){none, t};
}

ParserInfo subroutineBody()
{
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
	{
		printf("open brace detected\n");
	}
	else
		return (ParserInfo){openBraceExpected, t};
	
	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{
		pi = statement();
		if(pi.er != none)
			return pi;
	}
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
	{
		printf("close brace detected\n");
	}
	else
		return (ParserInfo){closeBraceExpected, t};
}

ParserInfo statement()
{
	t = GetNextToken();
	return (ParserInfo){syntaxError, t};
}

ParserInfo paramList()
{
	pi = type();
	if(pi.er != none)
		return pi;
	t = GetNextToken();
	if(t.tp == ID)
		printf("paramName detected\n");
	else
		return (ParserInfo){idExpected, t};
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
	{
		t = GetNextToken();
		pi = type();
		if(pi.er != none)
			return pi;
		t = GetNextToken();
		if(t.tp == ID)
			printf("paramName detected\n");
		else
			return (ParserInfo){idExpected, t};
		t = PeekNextToken();
	}

	return (ParserInfo){none, t};
}


// MAIN FUNCTIONS

int InitParser (char* file_name)
{
	InitLexer(file_name);

	return 0;
}

ParserInfo Parse ()
{
	ParserInfo pi;

	// implement the function
	pi = classDeclar();

	return pi;
}


int StopParser ()
{
	StopLexer();

	return 0;
}

char* ParseErrorString (SyntaxErrors e)
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


#ifndef TEST_PARSER
int main ()
{
	InitParser("Ball.jack");

	ParserInfo pi = Parse();

	printf("%s\n", ParseErrorString(pi.er));

	if(pi.er != none)
		printf("[%d, \'%s\', ln: %i]\n", pi.tk.tp, pi.tk.lx, pi.tk.ln);

	return 0;
}
#endif
