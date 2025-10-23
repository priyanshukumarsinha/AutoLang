#!/bin/bash

# Usage check
if [ $# -lt 2 ]; then
    echo "Usage: $0 <filename> <-s|-p|-t>"
    echo "  -s : Display Symbol Table"
    echo "  -p : Display Parse Tree"
    echo "  -t : Run Type Checker"
    exit 1
fi

FILE="$1"
FLAG="$2"

# Check if the file exists
if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' not found!"
    exit 1
fi

# Validate flag
if [[ "$FLAG" != "-s" && "$FLAG" != "-p" && "$FLAG" != "-t" ]]; then
    echo "Error: Invalid option '$FLAG'. Use -s for symbol table or -p for parse tree."
    exit 1
fi

# Clear terminal
clear

# Clean previous build
echo "Cleaning previous build..."
make clean

# Build the project
echo "Building the parser..."
make

# Run the parser with the given file and flag
echo "Running parser on '$FILE' with option '$FLAG'..."
./build/autolangparser "$FILE" "$FLAG"
