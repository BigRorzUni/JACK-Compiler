/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Rory Edmonds
Student ID: 201508877
Email: sc22re@leeds.ac.uk
Date Work Commenced: 09/02/2024
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE


typedef enum {INITIAL, WHITESPACE, INLINE_COMMENT, READ_SYMBOL, BLOCK_COMMENT, IDENTIFIER, KEYWORD, INT_CONST, STR_CONST, ERROR, END} State;
State currState;

int linenum;
char* filename;

const char* keywords[] = { "class", "constructor", "method", "function", "int", "boolean", "char", "void", "var", "static", "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"};
const char* operators = ",;=.+-*/&|~(){}[]<>";

// LINKED LIST VARIABLES
typedef struct Node {
    Token token;
    struct Node* next;
} Node;

Node* head = NULL;
Node* tail = NULL;

// CONSTANTS
#define CHUNK_SIZE 1024

// GLOBAL VARIABLES
FILE* fp;

// BUFFER
char chunk[CHUNK_SIZE];

char* buffer = NULL;     // Buffer to store the entire file
size_t bufferSize = 0;   // Size of the buffer



// FUNCTIONS

Token BuildToken(TokenType tp, char* lx, int ln, char* fl)
{
    Token t;
    t.tp = tp;
    strcpy(t.lx, lx); // for some reason this didnt work with NewLineInStr.jack
    t.ln = ln;
    strcpy(t.fl, fl);
    return t;
}

void AddToken(Token t)
{
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->token = t;
  newNode->next = NULL;

  if(head == NULL)
  {
    head = newNode;
    tail = newNode;
  }
  else
  {
    tail->next = newNode;
    tail = newNode;
  }
}

const char* getTypeString(TokenType type) 
{
  switch (type) 
  {
    case RESWORD:
        return "RESWORD";
    case ID:
        return "ID";
    case INT:
        return "INT";
    case SYMBOL:
        return "SYMBOL";
    case STRING:
        return "STRING";
    case EOFile:
        return "EOFile";
    case ERR:
        return "ERR";
  }
}

char* getErrorMsg(LexErrCodes code) 
{
  switch (code) 
  {
    case EofInCom:
      return "Error: unexpected eof in comment";
    case NewLnInStr:
      return "Error: new line in string constant";
    case EofInStr:
      return "Error: unexpected eof in string constant";
    case IllSym:
      return "Error: illegal symbol in source file";
  }
}


void TokeniseBuffer()
{
  // Iterate through buffer character by character
  char* curr = buffer;
  currState = INITIAL;

  Token t;
  linenum = 1;
  char temp[128] = "";
  int ec = 0;
  int i = 0;

  while (currState != END)
  {
    switch (currState)
    {
      case INLINE_COMMENT:
        if(*curr == EOF)
        {
          ec = EofInCom;

          currState = ERROR;

          break;
        }
        if(*curr == '\n')
        {
          currState = INITIAL;
          linenum++;
        }
        curr++;
        break;

      case BLOCK_COMMENT:
        if(*curr == EOF)
        {
          ec = EofInCom;

          currState = ERROR;

          break;
        }
        if(*curr == '*')
        {
          if(*(curr+1) == '/')
          {
            currState = INITIAL;
            curr += 2;
            continue;
          }
        }

        if(*curr == '\n')
          linenum++;

        curr++;
        break;

      case READ_SYMBOL:
          temp[0] = *curr;
          temp[1] = '\0';

          t = BuildToken(SYMBOL, temp, linenum, filename);
          AddToken(t);

          currState = INITIAL;
          curr++;

          break;
      
      case IDENTIFIER:
        if(*curr == '_' || isalpha(*curr) || isdigit(*curr))
        {
          temp[i++] = *curr;
          curr++;
          continue;
        }

        temp[i] = '\0';
    
        // Check if it is a keyword
        int isKeyword = 0;
        for(int j = 0; j < sizeof(keywords)/sizeof(keywords[0]); j++)
        {
          if(strcmp(temp, keywords[j]) == 0)
          {
            isKeyword = 1;         
            break;
          }
        }

        if(isKeyword != 0)
        {
          currState = KEYWORD;
          continue;
        }

        t = BuildToken(ID, temp, linenum, filename);

        AddToken(t);

        memset(temp, 0, sizeof(temp));
        i = 0;

        currState = INITIAL;
        break;

      case KEYWORD:
        t = BuildToken(RESWORD, temp, linenum, filename);

        AddToken(t);

        memset(temp, 0, sizeof(temp));
        i = 0;

        currState = INITIAL;
        break;

      case INT_CONST:
        if(isdigit(*curr))
        {
          temp[i++] = *curr;
          curr++;
          continue;
        }
        
        t = BuildToken(INT, temp, linenum, filename);

        AddToken(t);

        memset(temp, 0, sizeof(temp));
        i = 0;

        currState = INITIAL;
        break;
      
      case STR_CONST:
        if(*curr != '"')
        {
          
          if(*curr == '\n' )
          {
            ec = NewLnInStr;

            currState = ERROR;

            break;
          }

          if(*curr == EOF)
          {
            ec = EofInStr;

            currState = ERROR;

            break;
          }
          temp[i++] = *curr;
          
          curr++;
          continue;
        }
        else
        {
          temp[i] = '\0';

          t = BuildToken(STRING, temp, linenum, filename);
          AddToken(t);

          memset(temp, 0, sizeof(temp));
          i = 0; 

          currState = INITIAL;
          curr++;
        }

        break;
      
      case ERROR:
        t = BuildToken(ERR, getErrorMsg(ec), linenum, filename);
        t.ec = ec;

        AddToken(t);

        return;

      default:
        if(*curr == EOF)
        {
          // add EOF token
          t = BuildToken(EOFile, "EOF", linenum, filename);
          AddToken(t);

          currState = END;
          break;
        }

        if(isspace(*curr))
        {
          if(*curr == '\n')
          {
            linenum++;
            curr++;
            continue;
          }
          curr++;
          continue;
        }

        if(strchr(operators, *curr) != NULL)
        {
          if(*curr == '/')
          {
            if(*(curr+1) == '/')
            {
              currState = INLINE_COMMENT;
              curr += 2;
              continue;
            }
            else if(*(curr+1) == '*')
            {
              currState = BLOCK_COMMENT;
              curr += 2;
              continue;
            }
          }

          currState = READ_SYMBOL;
          continue;
        }

        if(isalpha(*curr) || *curr == '_')
        {
          currState = IDENTIFIER;
          continue;
        }

        if(isdigit(*curr))
        {
          currState = INT_CONST;
          continue;
        }

        if(*curr == '"')
        {
          currState = STR_CONST;
          curr++;
          continue;
        }

        // Otherwise it should be rejected
        ec = IllSym;

        currState = ERROR;

        continue;

    }    
  }
  return;
}



void outputList(Node* curr)
{
  curr = head;

  if(head == NULL)
  {
    printf("List is empty\n");
    return;
  }

  while (curr != NULL) 
  {
      
      if(curr->token.tp == ERR)
        printf("[%s, %s, \'%s\', ln: %i]", getTypeString(curr->token.tp), getErrorMsg(curr->token.ec), curr->token.lx, curr->token.ln);
      else
        printf("[%s, \'%s\', ln: %i]", getTypeString(curr->token.tp), curr->token.lx, curr->token.ln);
      curr = curr->next;
      printf("\n");
  }
}


// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{

  Node* tmp = head;
  Node* next;

  while (tmp != NULL) 
  {
    next = tmp->next;
    // Free the node itself
    free(tmp);
    tmp = next;
  }

    head = NULL;
  
  filename = file_name;

  // Read from file
  fp = fopen(file_name, "r");

  if(fp == NULL)
    return 0;

  if (ferror(fp)) 
    return 0;

  fseek(fp, 0, SEEK_END);
  bufferSize = ftell(fp);
  rewind(fp);

  buffer = malloc(bufferSize + 1);
  
  int bytesRead = fread(buffer, 1, bufferSize, fp);
  buffer[bufferSize] = EOF;

  //printf("FILE: %s\n", buffer);

  TokeniseBuffer();

  return 1;
}


// Get the next token from the source file
Token GetNextToken ()
{
  Token t;
  Node* n = head;

  if(head == NULL)
  {
    t.tp = EOFile;

    return t;
  }

  t = head->token;
  head = head->next;

  free(n);

  return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;

  if(head == NULL)
  {
    t.tp = ERR;
    return t;
  }

  t = head->token;

  return t;

}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
  // Free the buffer and set it to NULL
  if(buffer != NULL) {
    free(buffer);
  }

  // Close the file
  if(fp != NULL) {
    fclose(fp);
  }

  // Free token memory
    Node* tmp = head;
    Node* next;

    while (tmp != NULL) 
    {
      next = tmp->next;
      // Free the node itself
      free(tmp);
      tmp = next;
    }

    head = NULL;

  
	return 0;
}

// do not remove the next line
#ifndef TEST
int main ()
{
  InitLexer ("Main.jack");
  printf ("Checking that your GetNextToken function does return the correct tokens\n");
  Token t = GetNextToken ();
  int i = 0;
  while (t.tp != EOFile)
  {
      //printf ("Your function returned this token:\n");
      printf("[%s, \'%s\', ln: %i]\n", getTypeString(t.tp), t.lx, t.ln);
    i++;
    t  = GetNextToken();
  }
  printf ("Calling your StopLexer function\n");
  StopLexer ();
  
	return 0;
}
// do not remove the next line
#endif
