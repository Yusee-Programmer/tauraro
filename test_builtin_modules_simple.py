# Test script for builtin modules (simplified for VM)
import math
import sys
import os
import json
import random
import time

# Math module test
print("=== Math module test ===")
print("Pi:", math.pi)
print("Square root of 16:", math.sqrt(16))
print("Sin of pi/2:", math.sin(math.pi / 2))
print("Cos of 0:", math.cos(0))

# Sys module test
print("\n=== Sys module test ===")
print("Platform:", sys.platform)
print("Version:", sys.version)

# OS module test
print("\n=== OS module test ===")
print("Current directory:", os.getcwd())

# JSON module test
print("\n=== JSON module test ===")
data = {"name": "Tauraro", "version": "0.2.0"}
json_str = json.dumps(data)
print("JSON string:", json_str)
# Note: loads might not be fully implemented yet
# parsed_data = json.loads(json_str)
# print("Parsed data:", parsed_data)

# Random module test
print("\n=== Random module test ===")
print("Random number:", random.random())
print("Random integer (1-10):", random.randint(1, 10))

# Time module test
print("\n=== Time module test ===")
start = time.time()
print("Current time:", start)

print("\n=== All basic tests completed! ===")
