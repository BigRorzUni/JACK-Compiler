
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

Token makeToken(TokenType tp, char* lx, int ln) 
{
    Token t;
    t.tp = tp;
    strcpy(t.lx, lx);
    t.ln = ln;
    return t;
}

void AddLibrarySymbols() 
{
    SymbolTable* MemoryTable = CreateSymbolTable(root);
    SymbolTable* MathTable = CreateSymbolTable(root);
    SymbolTable* SysTable = CreateSymbolTable(root);
    SymbolTable* OutputTable = CreateSymbolTable(root);
    SymbolTable* ScreenTable = CreateSymbolTable(root);
    SymbolTable* KeyboardTable = CreateSymbolTable(root);
    SymbolTable* ArrayTable = CreateSymbolTable(root);
    SymbolTable* StringTable = CreateSymbolTable(root);

    Token MemoryToken = makeToken(ID, "Memory", 0);
    Token peekToken = makeToken(ID, "peek", 0);
    Token pokeToken = makeToken(ID, "poke", 0);
    Token allocToken = makeToken(ID, "alloc", 0);
    Token deAllocToken = makeToken(ID, "deAlloc", 0);

    Token MathToken = makeToken(ID, "Math", 0);
    Token absToken = makeToken(ID, "abs", 0);
    Token multiplyToken = makeToken(ID, "multiply", 0);
    Token divideToken = makeToken(ID, "divide", 0);
    Token minToken = makeToken(ID, "min", 0);
    Token maxToken = makeToken(ID, "max", 0);
    Token sqrtToken = makeToken(ID, "sqrt", 0);


    Token SysToken = makeToken(ID, "Sys", 0);
    Token haltToken = makeToken(ID, "halt", 0);
    Token errorToken = makeToken(ID, "error", 0);
    Token waitToken = makeToken(ID, "wait", 0);

    Token OutputToken = makeToken(ID, "Output", 0);
    Token initToken = makeToken(ID, "init", 0);
    Token moveCursorToken = makeToken(ID, "moveCursor", 0);
    Token printCharToken = makeToken(ID, "printChar", 0);
    Token printIntToken = makeToken(ID, "printInt", 0);
    Token printStringToken = makeToken(ID, "printString", 0);
    Token printlnToken = makeToken(ID, "println", 0);
    Token backspaceToken = makeToken(ID, "backspace", 0);

    Token ScreenToken = makeToken(ID, "Screen", 0);
    Token clearScreenToken = makeToken(ID, "clearScreen", 0);
    Token setColorToken = makeToken(ID, "setColor", 0);
    Token drawPixelToken = makeToken(ID, "drawPixel", 0);
    Token drawLineToken = makeToken(ID, "drawLine", 0);
    Token drawRectangleToken = makeToken(ID, "drawRectangle", 0);
    Token drawCircleToken = makeToken(ID, "drawCircle", 0);

    Token KeyboardToken = makeToken(ID, "Keyboard", 0);
    Token keyPressedToken = makeToken(ID, "keyPressed", 0);
    Token readCharToken = makeToken(ID, "readChar", 0);
    Token readLineToken = makeToken(ID, "readLine", 0);
    Token readIntToken = makeToken(ID, "readInt", 0);

    Token ArrayToken = makeToken(ID, "Array", 0);
    Token newToken = makeToken(ID, "new", 0);
    Token disposeToken = makeToken(ID, "dispose", 0);

    Token StringToken = makeToken(ID, "String", 0);
    Token newStringToken = makeToken(ID, "new", 0);
    Token disposeStringToken = makeToken(ID, "dispose", 0);
    Token lengthToken = makeToken(ID, "length", 0);
    Token setCharAtToken = makeToken(ID, "setCharAt", 0);
    Token charAtToken = makeToken(ID, "charAt", 0);
    Token appendCharToken = makeToken(ID, "appendChar", 0);
    Token eraseLastCharToken = makeToken(ID, "eraseLastChar", 0);
    Token intValueToken = makeToken(ID, "intValue", 0);
    Token setIntToken = makeToken(ID, "setInt", 0);
    Token newLineToken = makeToken(ID, "newLine", 0);
    Token backspaceStringToken = makeToken(ID, "backSpace", 0);
    Token doubleQuoteToken = makeToken(ID, "doubleQuote", 0);


    InsertSymbol(root, MemoryToken, MemoryTable, CLASS, OBJECT);
    InsertSymbol(MemoryTable, peekToken, NULL, METHOD, INTEGER);
    InsertSymbol(MemoryTable, pokeToken, NULL, METHOD, VOID);
    InsertSymbol(MemoryTable, allocToken, NULL, METHOD, OBJECT);
    InsertSymbol(MemoryTable, deAllocToken, NULL, METHOD, VOID);


    InsertSymbol(root, MathToken, MathTable, CLASS, OBJECT);
    InsertSymbol(MathTable, absToken, NULL, METHOD, INTEGER);
    InsertSymbol(MathTable, multiplyToken, NULL, METHOD, INTEGER);
    InsertSymbol(MathTable, divideToken, NULL, METHOD, INTEGER);
    InsertSymbol(MathTable, minToken, NULL, METHOD, INTEGER);
    InsertSymbol(MathTable, maxToken, NULL, METHOD, INTEGER);
    InsertSymbol(MathTable, sqrtToken, NULL, METHOD, INTEGER);

    InsertSymbol(root, SysToken, SysTable, CLASS, OBJECT);
    InsertSymbol(SysTable, haltToken, NULL, METHOD, VOID);
    InsertSymbol(SysTable, errorToken, NULL, METHOD, VOID);
    InsertSymbol(SysTable, waitToken, NULL, METHOD, VOID);

    InsertSymbol(root, OutputToken, OutputTable, CLASS, OBJECT);
    InsertSymbol(OutputTable, initToken, NULL, METHOD, VOID);
    InsertSymbol(OutputTable, moveCursorToken, NULL, METHOD, VOID);
    InsertSymbol(OutputTable, printCharToken, NULL, METHOD, VOID);
    InsertSymbol(OutputTable, printIntToken, NULL, METHOD, VOID);
    InsertSymbol(OutputTable, printStringToken, NULL, METHOD, VOID);
    InsertSymbol(OutputTable, printlnToken, NULL, METHOD, VOID);
    InsertSymbol(OutputTable, backspaceToken, NULL, METHOD, VOID);

    InsertSymbol(root, ScreenToken, ScreenTable, CLASS, OBJECT);
    InsertSymbol(ScreenTable, clearScreenToken, NULL, METHOD, VOID);
    InsertSymbol(ScreenTable, setColorToken, NULL, METHOD, VOID);
    InsertSymbol(ScreenTable, drawPixelToken, NULL, METHOD, VOID);
    InsertSymbol(ScreenTable, drawLineToken, NULL, METHOD, VOID);
    InsertSymbol(ScreenTable, drawRectangleToken, NULL, METHOD, VOID);
    InsertSymbol(ScreenTable, drawCircleToken, NULL, METHOD, VOID);

    InsertSymbol(root, KeyboardToken, KeyboardTable, CLASS, OBJECT);
    InsertSymbol(KeyboardTable, keyPressedToken, NULL, METHOD, CHARACTER);
    InsertSymbol(KeyboardTable, readCharToken, NULL, METHOD, CHARACTER);
    InsertSymbol(KeyboardTable, readLineToken, NULL, METHOD, OBJECT);
    InsertSymbol(KeyboardTable, readIntToken, NULL, METHOD, INTEGER);

    InsertSymbol(root, ArrayToken, ArrayTable, CLASS, OBJECT);
    InsertSymbol(ArrayTable, newToken, NULL, METHOD, OBJECT);
    InsertSymbol(ArrayTable, disposeToken, NULL, METHOD, VOID);

    InsertSymbol(root, StringToken, StringTable, CLASS, OBJECT);
    InsertSymbol(StringTable, newStringToken, NULL, METHOD, OBJECT);
    InsertSymbol(StringTable, disposeStringToken, NULL, METHOD, VOID);
    InsertSymbol(StringTable, lengthToken, NULL, METHOD, INTEGER);
    InsertSymbol(StringTable, charAtToken, NULL, METHOD, CHARACTER);
    InsertSymbol(StringTable, setCharAtToken, NULL, METHOD, VOID);
    InsertSymbol(StringTable, appendCharToken, NULL, METHOD, OBJECT);
    InsertSymbol(StringTable, eraseLastCharToken, NULL, METHOD, VOID);
    InsertSymbol(StringTable, intValueToken, NULL, METHOD, INTEGER);
    InsertSymbol(StringTable, setIntToken, NULL, METHOD, VOID);
    InsertSymbol(StringTable, newLineToken, NULL, METHOD, CHARACTER);
    InsertSymbol(StringTable, backspaceStringToken, NULL, METHOD, CHARACTER);
    InsertSymbol(StringTable, doubleQuoteToken, NULL, METHOD, CHARACTER);
    
}

SymbolTable* CreateSymbolTable(SymbolTable* parent) 
{
    SymbolTable* newNode = malloc(sizeof(SymbolTable));
    newNode->ns = 0;
    newNode->addressCounter = 0;
    newNode->parent = parent;
    return newNode;
}


void InsertSymbol(SymbolTable* st, Token tk, SymbolTable* scope, SymbolType sType, DataType dType) 
{
    st->symbols[st->ns].token = tk;
    st->symbols[st->ns].sType = sType;
    st->symbols[st->ns].dType = dType;
    st->symbols[st->ns].scope = scope;
    st->symbols[st->ns].address = st->addressCounter++;
    st->ns++;
}

SymbolTable* GetSymbolTable(SymbolTable* st, Token tk) 
{
    if (st == NULL) 
    {
        //printf("current scope is null\n");
        return NULL;
    }
    for (int i = 0; i < st->ns; i++) 
    {
        if (strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            return st->symbols[i].scope;
        }
    }
    // If the symbol was not found in the current symbol table, search for it in the parent
    if (st->parent != NULL) 
    {
        //printf("Searching parent for symbol table\n");
        return GetSymbolTable(st->parent, tk);
    }
    return NULL;
}

SymbolTable* ExitScope(SymbolTable* st) 
{
    if(st->parent != NULL)
        return st->parent;
}

void AssignScopeToInstance(SymbolTable* st, Token tk, SymbolTable* scope) 
{
    if (st == NULL) 
    {
        //printf("Symbol table error when trying to assign scope to %s\n", tk.lx);
        return;
    }
    for(int i = 0; i < st->ns; i++) 
    {
        if(strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            st->symbols[i].scope = scope;
            return;
        }
    }

    
}

int CheckScopeForSymbol(SymbolTable* st, Token tk) 
{
    if (st == NULL) 
    {
        //printf("Symbol table error when trying to find symbol %s at %d\n", tk.lx, tk.ln);
        return -1;
    }

    for (int i = 0; i < st->ns; i++) 
    {
        if (strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            return 1;
        }
    }
    return 0;
}

int FindSymbol(SymbolTable* st, Token tk) 
{
    if (st == NULL) 
    {
        //printf("Symbol table error when trying to find symbol %s at %d\n", tk.lx, tk.ln);
        return -1;
    }

    //printf("searching table for %s\n", tk.lx);
    for (int i = 0; i < st->ns; i++) 
    {
        //printf("comparing %s to %s\n", st->symbols[i].token.lx, tk.lx);
        if (strcmp(st->symbols[i].token.lx, tk.lx) == 0) 
        {
            //printf("found symbol %s at %d in the table\n", tk.lx, i);
            return 1;
        }
    }
    // If the symbol was not found in the current symbol table, search for it in the parent
    if (st->parent != NULL) 
    {
        //printf("searching parent table\n");
        return FindSymbol(st->parent, tk);
    }
    return 0;
}

char* SymbolTypeString(SymbolType sType) 
{
    switch (sType) 
    {
        case METHOD: return "METHOD";
        case CLASS: return "CLASS";
        case CLASS_VAR: return "CLASS_VAR";
        case VAR: return "VAR";
        case ARG: return "ARG";
    }
    return "UNKNOWN";
}

char* DataTypeString(DataType dType) 
{
    switch (dType) 
    {
        case INTEGER: return "INT";
        case CHARACTER: return "CHAR";
        case BOOLEAN: return "BOOL";
        case OBJECT: return "OBJECT";
        case VOID: return "VOID";
    }
    return "UNKNOWN";
}

int TokenToDataType(Token tk)
{
    if(tk.tp == RESWORD)
    {
        if(strcmp(tk.lx, "int") == 0)
            return INTEGER;
        else if(strcmp(tk.lx, "char") == 0)
            return CHARACTER;
        else if(strcmp(tk.lx, "boolean") == 0)
            return BOOLEAN;
        else if(strcmp(tk.lx, "void") == 0)
            return VOID;
    }
    
    if(tk.tp == ID)
        return OBJECT;

    return -1;
}

void PrintSymbolTable(SymbolTable* st) 
{
    if (st == NULL) 
    {
        //printf("Symbol table is NULL\n");
        return;
    }

    // First, print the current symbol table
    printf("SYMBOL TABLE:\n");
    printf("num symbols: %d\n", st->ns);
    printf("addressCounter: %d\n", st->addressCounter);
    printf("-----------------------------\n");
    
    for (int i = 0; i < st->ns; i++) 
    {
        printf("Symbol: %d, ", i);
        printf("token lx: %s, ", st->symbols[i].token.lx);
        printf("symbol type: %s, ", SymbolTypeString(st->symbols[i].sType));
        printf("data type: %s, ", DataTypeString(st->symbols[i].dType));
        printf("address: %d, ", st->symbols[i].address);
        if(st->symbols[i].scope != NULL)
        {
            printf("scope: TRUE\n");
            //printf("scope: TRUE\n****************************\n");
            //PrintSymbolTable(st->symbols[i].scope);
            //printf("END OF SCOPE TABLE\n****************************\n");
        }
        else
            printf("scope: FALSE\n");
    }
    printf("-----------------------------\n");
}

ParserInfo ShallowPass(char* filename, SymbolTable* st) 
{
    InitLexer(filename);

    //printf("Parsing %s\n", filename);
    
    Token t = {0};
    int type;
    SymbolTable* classTable = NULL;

    while(t.tp != EOFile) 
    {
        t = GetNextToken();

        if (t.tp == RESWORD) 
        {
            if (strcmp(t.lx, "class") == 0) 
            {
                t = GetNextToken();
                classTable = CreateSymbolTable(st);
                InsertSymbol(st, t, classTable, CLASS, OBJECT);
                //printf("class table added for %s\n", t.lx);
            }
            else if(strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0)
            {
                t = GetNextToken();
                if(t.tp == RESWORD && (strcmp(t.lx, "void") == 0 || strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0))
                    type = TokenToDataType(t);
                
                t = GetNextToken();
                if (t.tp == ID)
                {
                    SymbolTable* methodTable = CreateSymbolTable(classTable);
                    InsertSymbol(classTable, t, methodTable, METHOD, type);
                    //printf("method table added for %s\n", t.lx);
                }
            }
            else if(strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0)
            {
                t = GetNextToken();
                if(t.tp == ID || (t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)))
                    type = TokenToDataType(t);
                
                t = GetNextToken();
                if(t.tp == ID)
                {
                    int symbolExists = FindSymbol(classTable, t);
                    if(symbolExists == 1)
                    {
                        //printf("Error: redeclaration of identifier %s\n", t.lx);
                        return (ParserInfo){redecIdentifier, t};
                    }
                    InsertSymbol(classTable, t, NULL, CLASS_VAR, type);
                    //printf("class variable %s has been added\n", t.lx);

                    t = PeekNextToken();
                    while(t.tp != EOF && strcmp(t.lx, ";") != 0)
                    {
                        t = GetNextToken();
                        if(t.tp == SYMBOL && strcmp(t.lx, ",") == 0)
                        {
                            t = GetNextToken();
                            if(t.tp == ID)
                            {
                                int symbolExists = FindSymbol(classTable, t);
                                if(symbolExists == 1)
                                {
                                    return (ParserInfo){redecIdentifier, t};
                                }
                                InsertSymbol(classTable, t, NULL, CLASS_VAR, type);
                                //printf("class variable %s has been added\n", t.lx);
                            }
                        }

                        t = PeekNextToken();
                    }
                }
            }
        }
    }

    StopLexer();

    return (ParserInfo){none, t};
}