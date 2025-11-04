import os

# Test the specific encoding issue that was causing the problem
print("Testing UTF-8 encoding of DUITK __init__.tr file...")

file_path = os.path.join("tauraro_packages", "duitk", "__init__.tr")

# Try to read with UTF-8 (should work now)
try:
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    print("✓ SUCCESS: File can be read with UTF-8 encoding")
    print(f"  File size: {len(content)} characters")
    print(f"  First line: {repr(content.split(chr(10))[0])}")
except Exception as e:
    print(f"✗ FAILED: Cannot read with UTF-8: {e}")

# Try to read with UTF-16 (should fail or show different content)
try:
    with open(file_path, 'r', encoding='utf-16') as f:
        content = f.read()
    print("⚠ WARNING: File can also be read with UTF-16, may indicate encoding ambiguity")
except Exception as e:
    print("✓ CONFIRMED: File cannot be read with UTF-16 (as expected)")

# Check for BOM
with open(file_path, 'rb') as f:
    first_bytes = f.read(4)
    
print(f"  First 4 bytes (hex): {first_bytes.hex()}")
if first_bytes.startswith(b'\xef\xbb\xbf'):
    print("  ✓ UTF-8 BOM detected (correct)")
elif first_bytes.startswith(b'\xff\xfe'):
    print("  ✗ UTF-16 LE BOM detected (incorrect)")
elif first_bytes.startswith(b'\xfe\xff'):
    print("  ✗ UTF-16 BE BOM detected (incorrect)")
else:
    print("  - No BOM detected")