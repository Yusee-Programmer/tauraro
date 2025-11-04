import os
import sys

# Add the current directory to Python path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

print("Verifying DUITK package structure...")
print(f"Current directory: {os.getcwd()}")

# Check if the duitk package directory exists
duitk_path = os.path.join(os.getcwd(), "tauraro_packages", "duitk")
print(f"DUITK package path: {duitk_path}")
print(f"DUITK directory exists: {os.path.exists(duitk_path)}")

if os.path.exists(duitk_path):
    print("Contents of DUITK directory:")
    for item in os.listdir(duitk_path):
        print(f"  {item}")
    
    # Try to read the __init__.tr file
    init_file = os.path.join(duitk_path, "__init__.tr")
    if os.path.exists(init_file):
        print("\n__init__.tr file exists")
        try:
            with open(init_file, 'r', encoding='utf-8') as f:
                content = f.read()
                print(f"File size: {len(content)} characters")
                print("First few lines:")
                for i, line in enumerate(content.split('\n')[:5]):
                    print(f"  {i+1}: {line}")
            print("✓ Successfully read __init__.tr with UTF-8 encoding")
        except Exception as e:
            print(f"✗ Error reading __init__.tr: {e}")
    else:
        print("✗ __init__.tr file not found")
else:
    print("✗ DUITK package directory not found")