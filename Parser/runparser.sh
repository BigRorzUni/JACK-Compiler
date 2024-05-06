#!/bin/bash

# Compile the C files
cc -std=c99 lexer.h lexer.c parser.h parser.c ParserGrader.c -o parsertests

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Run the program
    ./parsertests "Ball.jack"
else
    echo "Compilation failed."
fi