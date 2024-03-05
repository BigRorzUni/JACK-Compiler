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
		t = PeekNextToken();
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
	t = PeekNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0))
	{
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
	t = GetNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0))
	{
		printf("classVarDeclar detected\n");
	}
	else
		return (ParserInfo){classVarErr, t};

	pi = type();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	if(t.tp == ID)
		printf("identifier detected\n");
	else
		return (ParserInfo){idExpected, t};

	t = PeekNextToken();
	while(t.tp != EOF && strcmp(t.lx, ";") != 0)
	{
		t = GetNextToken();
		//printf("current token in classVarDeclar is %s\n", t.lx);
		if(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
		{
			t = GetNextToken();
			if(t.tp == ID)
				printf("varName detected\n");
			else
				return (ParserInfo){idExpected, t};
		}
		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
	{
		printf("end of classVarDeclar\n");
		return (ParserInfo){none, t};
	}
	else
		return (ParserInfo){semicolonExpected, t};

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
	if(t.tp == RESWORD && (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0))
	{
		printf("subroutineDeclar detected\n");
	}
	else
		return (ParserInfo){subroutineDeclarErr, t};

	t = GetNextToken();
	if(t.tp == RESWORD && (strcmp(t.lx, "void") == 0 || strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
	{
		t = GetNextToken();
		printf("subroutine type detected\n");
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
	}
	else
		return (ParserInfo){openParenExpected, t};
	
	t = PeekNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
	{
		printf("close paren detected\n");
		t = GetNextToken();
	}
	else
	{
		pi = paramList();
		if(pi.er != none)
			return pi;
		
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
			printf("close paren detected\n");
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
	printf("subroutineBody detected\n");
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
		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
	{
		printf("close brace detected\n");
	}
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
	{
		printf("var declar statement detected\n");
	}
	else
		return (ParserInfo){syntaxError, t};

	pi = type();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	if(t.tp == ID)
		printf("identifier detected\n");
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
				printf("varName detected\n");
			else
				return (ParserInfo){idExpected, t};
		}
		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
	{
		printf("end of classVarDeclar\n");
		return (ParserInfo){none, t};
	}
	else
		return (ParserInfo){semicolonExpected, t};
	

	return (ParserInfo){syntaxError, t};
}

ParserInfo letStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "let") == 0)
	{
		printf("let statement detected\n");
	}
	else
		return (ParserInfo){syntaxError, t};
	
	t = GetNextToken();
	if(t.tp == ID)
		printf("identifier detected\n");
	else
		return (ParserInfo){idExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "[") == 0)
	{
		pi = expression();
		if(pi.er != none)
			return pi;

		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "]") == 0)
			printf("close bracket detected\n");
		else
			return (ParserInfo){closeBracketExpected, t};
	}
	else if(t.tp == SYMBOL && strcmp(t.lx, "=") == 0)
	{
		printf("assignment detected\n");
		pi = expression();
		if(pi.er != none)
			return pi;
	}
	else
		return (ParserInfo){equalExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		printf("semicolon detected\n");
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo ifStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "if") == 0)
	{
		printf("if statement detected\n");
	}
	else
		return (ParserInfo){syntaxError, t};
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
	{
		printf("open paren detected\n");
	}
	else
		return (ParserInfo){openParenExpected, t};
	
	pi = expression();
	if(pi.er != none)
		return pi;

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
		printf("close paren detected\n");
	else
		return (ParserInfo){closeParenExpected, t};

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
	{
		printf("open brace detected\n");
	}
	else
		return (ParserInfo){openBraceExpected, t};
	
	t = PeekNextToken();
	while(t.tp != EOF && strcmp(t.lx, "}") != 0)
	{
		t = GetNextToken();

		pi = statement();
		if(pi.er != none)
			return pi;
		t = PeekNextToken();
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
		printf("close brace detected\n");
	else
		return (ParserInfo){closeBraceExpected, t};
	
	t = PeekNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "else") == 0)
	{
		t = GetNextToken();
		if(t.tp == RESWORD && strcmp(t.lx, "else") == 0)
			printf("else detected\n");
		else
			return (ParserInfo){syntaxError, t};
		
		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "{") == 0)
		{
			printf("open brace detected\n");
		}
		else
			return (ParserInfo){openBraceExpected, t};
		
		t = PeekNextToken();
		while(t.tp != EOF && strcmp(t.lx, "}") != 0)
		{
			t = GetNextToken();

			pi = statement();
			if(pi.er != none)
				return pi;
			t = PeekNextToken();
		}

		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)
			printf("close brace detected\n");
		else
			return (ParserInfo){closeBraceExpected, t};
	}

	return (ParserInfo){none, t};
}

ParserInfo whileStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "while") == 0)
	{
		printf("while statement detected\n");
	}
	else
		return (ParserInfo){syntaxError, t};

	t = GetNextToken();
	return (ParserInfo){syntaxError, t};
}

ParserInfo doStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "do") == 0)
	{
		printf("do statement detected\n");
	}
	else
		return (ParserInfo){syntaxError, t};

	pi = subroutineCall();
	if(pi.er != none)
		return pi;
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		printf("semicolon detected\n");
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo subroutineCall()
{
	t = GetNextToken();
	if(t.tp == ID)
		printf("subroutineName detected\n");
	else
		return (ParserInfo){idExpected, t};
	
	t = PeekNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ".") == 0)
	{
		t = GetNextToken();

		t = GetNextToken();
		
		if(t.tp == ID)
			printf(". subroutine detected\n");
		else
			return (ParserInfo){idExpected, t};
	}
	
	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
	{
		printf("open paren detected\n");
	}
	else
		return (ParserInfo){openParenExpected, t};


	t = PeekNextToken();
	printf("current token is %s\n", t.lx);
	if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
	{
		t = GetNextToken();
		printf("close paren detected\n");
	}
	else
	{
		pi = expressionList();
		if(pi.er != none)
			return pi;
		
		t = GetNextToken();
		if(t .tp == SYMBOL && strcmp(t.lx, ")") == 0)
			printf("close paren detected\n");
		else
			return (ParserInfo){closeParenExpected, t};
	}

	return (ParserInfo){none, t};
}

ParserInfo paramList()
{
	printf("paramlist detected\n");

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
		printf("current token in param list is %s\n", t.lx);

		t = GetNextToken();
		if(t.tp == ID)
			printf("paramName detected\n");
		else
			return (ParserInfo){idExpected, t};

		t = PeekNextToken();
	}

	printf("end of paramList\n");

	return (ParserInfo){none, t};
}

ParserInfo expressionList()
{
	printf("expressionList detected\n");

	pi = expression();
	if(pi.er != none)
		return pi;
	
	t = PeekNextToken();
	while(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
	{
		t = GetNextToken();
		printf("current token in expression list is %s\n", t.lx);

		pi = expression();
		if(pi.er != none)
			return pi;

		t = PeekNextToken();
	}

	printf("end of expressionList\n");
	
	return (ParserInfo){none, t};
}

ParserInfo returnStatement()
{
	t = GetNextToken();
	if(t.tp == RESWORD && strcmp(t.lx, "return") == 0)
		printf("return statement detected at line %i\n", t.ln);
	else
		return (ParserInfo){syntaxError, t};
	
	t = PeekNextToken();
	if(strcmp(t.lx, ";") != 0)
	{
		pi = expression();
		if(pi.er != none)
			return pi;
	}

	t = GetNextToken();
	if(t.tp == SYMBOL && strcmp(t.lx, ";") == 0)
		printf("semicolon detected\n");
	else
		return (ParserInfo){semicolonExpected, t};

	return (ParserInfo){none, t};
}

ParserInfo expression()
{
	printf("expression detected\n");

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
	printf("relational expression detected\n");
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
	}
	return (ParserInfo){none, t};
}

ParserInfo arithmeticExpression()
{
	printf("arithmetic expression detected\n");
	pi = term();
	if(pi.er != none)
		return pi;
	
	printf("term before operator is %s\n", t.lx);

	t = PeekNextToken();
	while(t.tp == SYMBOL && (strcmp(t.lx, "+") == 0 || strcmp(t.lx, "-") == 0))
	{
		t = GetNextToken();
		printf("arithmetic operator %s detected\n", t.lx);

		pi = term();
		printf("term after operator is %s\n", t.lx);
		if(pi.er != none)
			return pi;
	}

	return (ParserInfo){none, t};
}

ParserInfo term()
{
	printf("term detected\n");

	pi = factor();
	if(pi.er != none)
		return pi;	

	return (ParserInfo){none, t};
}

ParserInfo factor()
{
	printf("factor detected\n");

	t = PeekNextToken();
	if(t.tp == SYMBOL && (strcmp(t.lx, "-") == 0 || strcmp(t.lx, "~") == 0))
	{
		printf("unary operator detected\n");
		t = GetNextToken();
	}

	pi = operand();
	if(pi.er != none)
		return pi;
	
	return (ParserInfo){none, t};
}

ParserInfo operand()
{
	printf("operand detected\n");

	t = PeekNextToken();
	printf("token: %s of type %i\n", t.lx, t.tp);
	if(t.tp == INT)
	{
		t = GetNextToken();
		printf("integer constant detected\n");
	}
	else if(t.tp == ID)
	{
		t = GetNextToken();
		printf("identifier");

		t = PeekNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ".") == 0)
		{
			t = GetNextToken();

			t = GetNextToken();
			if(t.tp == ID)
				printf(".identifier detected\n");
			else
				return (ParserInfo){idExpected, t};
			
			t = PeekNextToken();
			if(t.tp == SYMBOL && strcmp(t.lx, "[") == 0)
			{
				printf("open sqr bracket detected\n");
				t = GetNextToken();

				pi = expression();
				if(pi.er != none)
					return pi;

				t = GetNextToken();
				if(t.tp == SYMBOL && strcmp(t.lx, "]") == 0)
					printf("close sqr bracket detected\n");
				else
					return (ParserInfo){closeBracketExpected, t};
			}
			else if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
			{
				printf("open paren detected\n");
				t = GetNextToken();

				pi = expressionList();
				if(pi.er != none)
					return pi;

				t = GetNextToken();
				if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)
					printf("close paren detected\n");
				else
					return (ParserInfo){closeParenExpected, t};
			}
		}
		else
			printf(" detected\n");
	}
	else if(t.tp == SYMBOL && strcmp(t.lx, "(") == 0)
	{
		t = GetNextToken();
		printf("open paren detected\n");

		pi = expression();
		if(pi.er != none)
			return pi;

		t = GetNextToken();
		if(t.tp == SYMBOL && strcmp(t.lx, ")") == 0)

			printf("close paren detected\n");
		else
			return (ParserInfo){closeParenExpected, t};
	}
	else if(t.tp == STRING)
	{
		t = GetNextToken();
		printf("string detected");
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "true") == 0)
	{
		t = GetNextToken();
		printf("true detected\n");
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "false") == 0)
	{
		t = GetNextToken();
		printf("false detected\n");
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "null") == 0)
	{
		t = GetNextToken();
		printf("null detected\n");
	}
	else if (t.tp == RESWORD && strcmp(t.lx, "this") == 0)
	{
		t = GetNextToken();
		printf("this detected\n");
	}
	else
		return (ParserInfo){syntaxError, t};
	
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

	printf("ERROR: %s\n", ParseErrorString(pi.er));

	if(pi.er != none)
		printf("[%d, \'%s\', ln: %i]\n", pi.tk.tp, pi.tk.lx, pi.tk.ln);

	return 0;
}
#endif
