#!/bin/bash
# Fix main() return type in generated C code
# Only fixes "return temp_result;" statements inside main() function

if [ -z "$1" ]; then
    echo "Usage: $0 <c_file>"
    exit 1
fi

C_FILE="$1"

if [ ! -f "$C_FILE" ]; then
    echo "Error: File $C_FILE not found"
    exit 1
fi

# Use Python to do proper parsing - find and fix only in main() function
python3 - "$C_FILE" << 'EOF'
import sys
import re

c_file = sys.argv[1]

with open(c_file, 'r') as f:
    content = f.read()

# Find main function and fix return statements in it
in_main = False
depth = 0
fixed_lines = []
lines = content.split('\n')

for i, line in enumerate(lines):
    # Check if this is the start of main function
    if re.match(r'^\s*int\s+main\s*\(', line):
        in_main = True
        depth = 0

    # Track braces depth
    if in_main:
        depth += line.count('{')
        depth -= line.count('}')

        # Fix return temp_result in main
        if 'return temp_result;' in line and depth > 0:
            line = re.sub(r'return temp_result;', 'return 0;  // Fixed: main returns int', line)

        # Exit main when we close all braces
        if depth == 0 and '}' in line:
            in_main = False

    fixed_lines.append(line)

with open(c_file, 'w') as f:
    f.write('\n'.join(fixed_lines))

print(f"Fixed {c_file}")
EOF
