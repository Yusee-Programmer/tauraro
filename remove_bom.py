# Script to remove BOM from a file
with open('tauraro_packages/duitk/__init__.tr', 'rb') as f:
    content = f.read()

# Check if file starts with UTF-8 BOM
if content.startswith(b'\xef\xbb\xbf'):
    print("UTF-8 BOM found, removing it...")
    # Remove the BOM (first 3 bytes)
    content = content[3:]
    # Write back without BOM
    with open('tauraro_packages/duitk/__init__.tr', 'wb') as f:
        f.write(content)
    print("BOM removed successfully!")
else:
    print("No UTF-8 BOM found in the file.")