#!/bin/bash

# Compile the C files
cc -std=c99 lexer.h parser.h symbols.h compiler.h lexer.c parser.c symbols.c compiler.c SymbolGrader.c -o alltests

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Remove the output file if it exists
    rm -f output.txt

    # Iterate over all .jack files in the current directory
    for dir in */; do
        echo "--$dir--" >> output.txt
        ./alltests "$dir" | tail -n 1 >> output.txt
        echo "" >> output.txt
    done
else
    echo "Compilation failed."
fi