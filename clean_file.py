# Script to clean special characters from the file
with open('tauraro_packages/duitk/__init__.tr', 'r', encoding='utf-8') as f:
    content = f.read()

# Replace special characters with ASCII equivalents
cleaned = content.replace('✓', '[OK]').replace('✗', '[X]')

# Write back the cleaned content
with open('tauraro_packages/duitk/__init__.tr', 'w', encoding='utf-8') as f:
    f.write(cleaned)

print("File cleaned successfully!")