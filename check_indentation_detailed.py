# Script to check indentation in detail
with open('tauraro_packages/duitk/__init__.tr', 'r', encoding='utf-8') as f:
    lines = f.readlines()

print(f"Total lines: {len(lines)}")

# Check for indentation issues
for i, line in enumerate(lines[:50]):  # Check first 50 lines
    # Show line number and first 30 characters
    line_display = repr(line[:30])
    if len(line) > 30:
        line_display += "..."
    
    # Analyze leading whitespace
    leading = line[:len(line) - len(line.lstrip())]
    
    if leading:
        # Count spaces and tabs
        space_count = leading.count(' ')
        tab_count = leading.count('\t')
        
        if tab_count > 0:
            print(f"{i+1:3}: TAB INDENTATION ({tab_count} tabs, {space_count} spaces) - {line_display}")
        else:
            if space_count % 4 != 0:
                print(f"{i+1:3}: UNUSUAL SPACING ({space_count} spaces) - {line_display}")
            else:
                print(f"{i+1:3}: {space_count} spaces - {line_display}")
    else:
        print(f"{i+1:3}: no indent - {line_display}")