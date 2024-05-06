#!/bin/bash

# Compile the C files with the C99 standard
cc -std=c99 lexer.h parser.h symbols.h compiler.h lexer.c parser.c symbols.c compiler.c -o compilertest

if [ $? -eq 0 ]; then
    echo "Compilation successful"
    ./compilertest
else
    echo "Compilation failed"
fi