# Simple test for C compilation with builtin modules
import math
import sys
import os
import json

# Test math functions
result = math.sqrt(25)
print("Square root of 25:", result)

# Test sys functions
print("Platform:", sys.platform)

# Test os functions
current_dir = os.getcwd()
print("Current directory:", current_dir)

# Test json functions
data = {"test": "value", "number": 42}
json_str = json.dumps(data)
print("JSON string:", json_str)
parsed = json.loads(json_str)
print("Parsed JSON:", parsed)