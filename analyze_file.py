# Script to analyze the file for potential issues
with open('tauraro_packages/duitk/__init__.tr', 'r', encoding='utf-8') as f:
    lines = f.readlines()

print(f"Total lines: {len(lines)}")

# Look for potential issues
for i, line in enumerate(lines):
    # Check for lines with only whitespace
    if line.strip() == '' and line != '\n':
        print(f"Line {i+1}: Only whitespace - {repr(line)}")
    
    # Check for mixed line endings
    if '\r\n' in line and '\n' in line:
        print(f"Line {i+1}: Mixed line endings")
    
    # Check for non-standard characters
    try:
        line.encode('ascii')
    except UnicodeEncodeError:
        print(f"Line {i+1}: Non-ASCII characters - {repr(line[:50])}")
        
    # Check for problematic indentation
    if line.startswith(' '):
        # Count leading spaces
        space_count = 0
        for char in line:
            if char == ' ':
                space_count += 1
            else:
                break
        if space_count % 4 != 0 and space_count != 1:
            print(f"Line {i+1}: Unusual space indentation ({space_count} spaces) - {repr(line[:30])}")

print("Analysis complete.")