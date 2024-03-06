#!/bin/bash

# Compile the C files
cc -std=c99 lexer.h lexer.c parser.h parser.c ParserGrader.c -o parsertests

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Remove the output file if it exists
    rm -f output.txt

    # Iterate over all .jack files in the current directory
    for file in *.jack
    do
        echo "--$file--" >> output.txt
        # Run the program on the file and append the last two lines of output to output.txt
        ./parsertests "$file" | tail -n 2 >> output.txt
        echo "" >> output.txt
    done
else
    echo "Compilation failed."
fi