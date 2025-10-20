#!/bin/bash

# Check if a filename is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

FILE="$1"

# Check if the file exists
if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' not found!"
    exit 1
fi

# Clear the terminal
clear

# Clean previous build
echo "Cleaning previous build..."
make clean

# Build the project
echo "Building the parser..."
make

# Run the parser with the given file
echo "Running parser on '$FILE'..."
./build/autolangparser "$FILE"
