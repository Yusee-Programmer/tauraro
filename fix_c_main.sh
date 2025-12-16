#!/bin/bash
# Fix main() return type in generated C code
# Replaces "return temp_result;" in main with "return 0;"

if [ -z "$1" ]; then
    echo "Usage: $1 <c_file>"
    exit 1
fi

C_FILE="$1"

# Check if file exists
if [ ! -f "$C_FILE" ]; then
    echo "Error: File $C_FILE not found"
    exit 1
fi

# Fix the return statement in main function
# Find "return temp_result;" that's inside main() and replace with "return 0;"
sed -i 's/^\(\s*\)return temp_result;$/\1\/\/ Return from main - ignoring value\n\1return 0;/g' "$C_FILE"

echo "Fixed $C_FILE"
