# Script to check indentation in the file
with open('tauraro_packages/duitk/__init__.tr', 'r', encoding='utf-8') as f:
    lines = f.readlines()

print(f"Total lines: {len(lines)}")

# Check first 30 lines for indentation issues
for i, line in enumerate(lines[:30]):
    # Show line number and first 50 characters
    line_display = repr(line[:50])
    if len(line) > 50:
        line_display += "..."
    
    # Check for mixed tabs and spaces
    leading_whitespace = line[:len(line) - len(line.lstrip())]
    has_tabs = '\t' in leading_whitespace
    has_spaces = ' ' in leading_whitespace
    
    if has_tabs and has_spaces:
        print(f"{i+1:3}: MIXED INDENTATION - {line_display}")
    else:
        print(f"{i+1:3}: {line_display}")