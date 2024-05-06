#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// GLOBsAL VARIABLES
Token t;
ParserInfo pi;

// GRAMMAR DECLARATIONS

ParserInfo classDeclar(); // class { memberDeclar* }
ParserInfo memberDeclar(); // classVarDeclar | subroutineDeclar
ParserInfo classVarDeclar(); // (static | field ) type varName (, varName)* ;
ParserInfo type(); // int | char | boolean | identifier
ParserInfo subroutineDeclar(); // (constructor | function | method) (void | type) subroutineName ( paramList ) subroutineBody
ParserInfo paramList(); // type identifier (, type identifier)* | empty
ParserInfo subroutineBody(); // { statement* }
ParserInfo statement(); // varDeclarStatement | letStatement | ifStatement | whileStatement | doStatement | returnStatement
ParserInfo varDeclarStatement(); // var type identifier (, identifier)* ;
ParserInfo letStatement(); // let identifier [expression]? = expression ;
ParserInfo ifStatement(); // if (expression) { statement* } (else { statement* })?
ParserInfo whileStatement(); // while (expression) { statement* }
ParserInfo doStatement(); // do subroutineCall ;
ParserInfo subroutineCall(); // identifer (. identifier)? ( expressionList )
ParserInfo expressionList(); // expression (, expression)* | empty
ParserInfo returnStatement(); // return expression? ;
ParserInfo expression(); // relationalExpression ((& | |) relationalExpression)*
ParserInfo relationalExpression(); // arithmeticExpression ((< | > | =) arithmeticExpression)*
ParserInfo arithmeticExpression(); // term ((+ | -) term)*
ParserInfo term(); // factor (( * | /) factor)*
ParserInfo factor(); // (- | ~ | empty) operand
ParserInfo operand(); // integerConstant | identifier (.identifier)? ([ expression ] | ( expressionList ))? | ( expression ) | stringConstant | true | false | null | this


// GRAMMAR IMPLEMENTATIONS
ParserInfo classDeclar()
{
	t = GetNextToken();
	if(t.tp == ERR)
		return (ParserInfo){lexerErr, t};
	if(t.tp == RESWORD && strcmp(t.lx, "class") == 0)
		;
	else
		return (ParserInfo){classExpected, t};

	t = GetNextToken();
	if(t.tp == ID)
		;
	else
		return (ParserInfo){idExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
		;
	else
		return (ParserInfo){openBraceExpected, t};

	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{	
		pi = memberDeclar();
		if(pi.er != none)
			return pi;
		t = PeekNextToken();
	}

	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
		;
	else
		return (ParserInfo){closeBraceExpected, t};
		
	return (ParserInfo){none, t};
}

ParserInfo memberDeclar()
{
	t = PeekNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0))
	{
		pi = classVarDeclar();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0))
	{
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
	t = GetNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0))
		;
	else
		return (ParserInfo){classVarErr, t};

	pi = type();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	if(t.tp == ID)
		;
	else
		return (ParserInfo){idExpected, t};

	t = PeekNextToken();
	while(t.tp != EOF && strcmp(t.lx, ";") != 0)
	{
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
		{
			t = GetNextToken();
			if(t.tp == ID)
				;
			else
				return (ParserInfo){idExpected, t};
		}

		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		;
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo type()
{
	t = GetNextToken();
	if(t.tp == ID || (t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)))
		return (ParserInfo){none, t};

	return (ParserInfo){syntaxError, t};
}

ParserInfo subroutineDeclar()
{
	t = GetNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0))
		;
	else
		return (ParserInfo){subroutineDeclarErr, t};

	t = PeekNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "void") == 0 || strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
		t = GetNextToken();
	else if (t.tp == ID)
	{
		pi = type();
		if(pi.er != none)
			return pi;
	}
	else
		return (ParserInfo){illegalType, t};
	
	t = GetNextToken();
	if(t.tp == ID)
		;
	else
		return (ParserInfo){idExpected, t};
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
		;
	else
		return (ParserInfo){openParenExpected, t};
	
	t = PeekNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
		t = GetNextToken();
	else
	{
		if(t.tp == ID | (t.tp == RESWORD && strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
		{
			pi = paramList();
			if(pi.er != none)
				return pi;
		}
		
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
			;
		else
			return (ParserInfo){closeParenExpected, t};
	}
	
	pi = subroutineBody();
	if(pi.er != none)
		return pi;

	return (ParserInfo){none, t};
}

ParserInfo subroutineBody()
{
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
		;
	else
		return (ParserInfo){openBraceExpected, t};
	
	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{
		pi = statement();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
		;
	else
		return (ParserInfo){closeBraceExpected, t};
	
	return (ParserInfo){none, t};
}

ParserInfo statement()
{
	t = PeekNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "var") == 0)
	{
		pi = varDeclarStatement();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && strcmp(t.lx, "let") == 0)
	{
		pi = letStatement();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && strcmp(t.lx, "if") == 0)
	{
		pi = ifStatement();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && strcmp(t.lx, "while") == 0)
	{
		pi = whileStatement();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && strcmp(t.lx, "do") == 0)
	{
		pi = doStatement();
		if(pi.er != none)
			return pi;
	}
	else if(t.tp == RESWORD && strcmp(t.lx, "return") == 0)
	{
		pi = returnStatement();
		if(pi.er != none)
			return pi;
	}

	return (ParserInfo){none, t};
}

ParserInfo varDeclarStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "var") == 0)
		;
	else
		return (ParserInfo){syntaxError, t};

	pi = type();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	if(t.tp == ID)
		;
	else
		return (ParserInfo){idExpected, t};
	
	t = PeekNextToken();
	while(t.tp != EOF && strcmp(t.lx, ";") != 0)
	{
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
		{
			t = GetNextToken();
			if(t.tp == ID)
				;
			else
				return (ParserInfo){idExpected, t};
		}

		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		return (ParserInfo){none, t};
	else
		return (ParserInfo){semicolonExpected, t};
	

	return (ParserInfo){none, t};
}

ParserInfo letStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "let") == 0)
		;
	else
		return (ParserInfo){syntaxError, t};
	
	t = GetNextToken();
	if(t.tp == ID)
		;
	else
		return (ParserInfo){idExpected, t};

	t = PeekNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "[") == 0)
	{
		t = GetNextToken();

		pi = expression();
		if(pi.er != none)
			return pi;

		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "]") == 0)
			;
		else
			return (ParserInfo){closeBracketExpected, t};
	}
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "=") == 0)
	{
		pi = expression();
		if(pi.er != none)
			return pi;
	}
	else
		return (ParserInfo){equalExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		;
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo ifStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "if") == 0)
		;
	else
		return (ParserInfo){syntaxError, t};
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
		;
	else
		return (ParserInfo){openParenExpected, t};
	
	pi = expression();
	if(pi.er != none)
		return pi;

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
		;
	else
		return (ParserInfo){closeParenExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
		;
	else
		return (ParserInfo){openBraceExpected, t};
	
	t = PeekNextToken();
	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{
		pi = statement();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
		;
	else
		return (ParserInfo){closeBraceExpected, t};
	
	t = PeekNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "else") == 0)
	{
		t = GetNextToken();
		if(t.tp == RESWORD && strcmp(t.lx, "else") == 0)
			;
		else
			return (ParserInfo){syntaxError, t};
		
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
			;
		else
			return (ParserInfo){openBraceExpected, t};
		
		t = PeekNextToken();
		while(t.tp != EOF && strcmp(t.lx, "}") != 0)
		{
			pi = statement();
			if(pi.er != none)
				return pi;
			
			t = PeekNextToken();
		}

		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
			;
		else
			return (ParserInfo){closeBraceExpected, t};
	}

	return (ParserInfo){none, t};
}

ParserInfo whileStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "while") == 0)
		;
	else
		return (ParserInfo){syntaxError, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
		;
	else
		return (ParserInfo){openParenExpected, t};
	
	pi = expression();
	if(pi.er != none)
		return pi;

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
		;
	else
		return (ParserInfo){closeParenExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
		;
	else
		return (ParserInfo){openBraceExpected, t};
	
	t = PeekNextToken();
	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{
		pi = statement();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
		;
	else
		return (ParserInfo){closeBraceExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo doStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "do") == 0)
		;
	else
		return (ParserInfo){syntaxError, t};

	pi = subroutineCall();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		;
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo subroutineCall()
{
	t = GetNextToken();
	if(t.tp == ID)
		;
	else
		return (ParserInfo){idExpected, t};
	
	t = PeekNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ".") == 0)
	{
		t = GetNextToken();

		t = GetNextToken();
		if(t.tp == ID)
			;
		else
			return (ParserInfo){idExpected, t};
	}
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
		;
	else
		return (ParserInfo){openParenExpected, t};

	t = PeekNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
		t = GetNextToken();
	else
	{
		pi = expressionList();
		if(pi.er != none)
			return pi;
		
		t = GetNextToken();
		if(t .tp == SYMBOL && strcmp(t.lx, ")") == 0)
			;
		else
			return (ParserInfo){closeParenExpected, t};
	}

	return (ParserInfo){none, t};
}

ParserInfo paramList()
{
	pi = type();
	if(pi.er != none)
		return pi;

	t = GetNextToken();
	if(t.tp == ID)
		;
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
			;
		else
			return (ParserInfo){idExpected, t};

		t = PeekNextToken();
	}

	return (ParserInfo){none, t};
}

ParserInfo expressionList()
{
	pi = expression();
	if(pi.er != none)
		return pi;
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
	{
		t = GetNextToken();

		pi = expression();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}
	
	return (ParserInfo){none, t};
}

ParserInfo returnStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "return") == 0)
		;
	else
		return (ParserInfo){syntaxError, t};
	
	t = PeekNextToken();
	if(strcmp(t.lx, ";") != 0)
	{
		pi = expression();
		if(pi.er != none)
			return (ParserInfo){semicolonExpected, t};
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		;
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo expression()
{
	pi = relationalExpression();
	if(pi.er != none)
		return pi;
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && (strcmp(t.lx, "&") == 0 || strcmp(t.lx, "|") == 0))
	{
		t = GetNextToken();

		pi = relationalExpression();
		if(pi.er != none)
			return pi;
	}

	return (ParserInfo){none, t};
}

ParserInfo relationalExpression()
{
	pi = arithmeticExpression();
	if(pi.er != none)
		return pi;
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && (strcmp(t.lx, "<") == 0 || strcmp(t.lx, ">") == 0 || strcmp(t.lx, "=") == 0))
	{
		t = GetNextToken();

		pi = arithmeticExpression();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}
	return (ParserInfo){none, t};
}

ParserInfo arithmeticExpression()
{
	pi = term();
	if(pi.er != none)
		return pi;
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && (strcmp(t.lx, "+") == 0 || strcmp(t.lx, "-") == 0))
	{
		t = GetNextToken();

		pi = term();
		if(pi.er != none)
			return pi;
		
		t = PeekNextToken();
	}

	return (ParserInfo){none, t};
}

ParserInfo term()
{
	pi = factor();
	if(pi.er != none)
		return pi;
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && (strcmp(t.lx, "*") == 0 || strcmp(t.lx, "/") == 0))
	{
		t = GetNextToken();

		pi = term();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}

	return (ParserInfo){none, t};
}

ParserInfo factor()
{
	t = PeekNextToken();
	if(t.tp == SYMBOL && (strcmp(t.lx, "-") == 0 || strcmp(t.lx, "~") == 0))
		t = GetNextToken();

	pi = operand();
	if(pi.er != none)
		return pi;
	
	return (ParserInfo){none, t};
}

ParserInfo operand()
{
	t = PeekNextToken();
	if(t.tp == INT)
	{
		t = GetNextToken();

		return (ParserInfo){none, t};
	}
	else if(t.tp == ID) 
	{
		t = GetNextToken();

		t = PeekNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ".") == 0)
		{
			t = GetNextToken();

			t = GetNextToken();
			if(t.tp == ID)
				;
			else
				return (ParserInfo){idExpected, t};
		}
		else
			;
		t = PeekNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "[") == 0)
		{
			t = GetNextToken();

			pi = expression();
			if(pi.er != none)
				return pi;

			t = GetNextToken();
			if(t.tp == SYMBOL && strcmp(t.lx, "]") == 0)
				;
			else
				return (ParserInfo){closeBracketExpected, t};
		}
		else if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
		{
			t = GetNextToken();

			t = PeekNextToken();
			if(strcmp(t.lx, ")") != 0)
			{
				pi = expressionList();
				if(pi.er != none)
					return pi;
			}

			t = GetNextToken();
			if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
				;
			else
				return (ParserInfo){closeParenExpected, t};
		}
		
		return (ParserInfo){none, t};
	}
	else if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
	{
		t = GetNextToken();

		pi = expression();
		if(pi.er != none)
			return pi;

		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
			;
		else
			return (ParserInfo){closeParenExpected, t};
		
		return (ParserInfo){none, t};
	}
	else if(t.tp == STRING)
	{
		t = GetNextToken();
		return (ParserInfo){none, t};
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "true") == 0)
	{
		t = GetNextToken();
		return (ParserInfo){none, t};
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "false") == 0)
	{
		t = GetNextToken();
		return (ParserInfo){none, t};
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "null") == 0)
	{
		t = GetNextToken();
		return (ParserInfo){none, t};
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "this") == 0)
	{
		t = GetNextToken();
		return (ParserInfo){none, t};
	}

	return (ParserInfo){syntaxError, t};
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
int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("Please provide a file name.\n");
        return 1;
    }

    InitParser(argv[1]);

    ParserInfo pi = Parse();

	printf("--------------\n");
    printf(pi.er == none ? "%s", ParseErrorString(pi.er) : "ERROR: %s", ParseErrorString(pi.er));
    printf("\nlast token read = [%d, \'%s\', ln: %i]\n", pi.tk.tp, pi.tk.lx, pi.tk.ln);

    return 0;
}
#endif
