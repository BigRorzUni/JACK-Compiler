#!/bin/bash

# Compile the C files
cc -std=c99 lexer.h lexer.c parser.h parser.c ParserGrader.c -o parsertests

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program:"
    echo "comment define test in "#define TEST" to run without tests"
    # Run the program
    ./parsertests
else
    echo "Compilation failed."
fi