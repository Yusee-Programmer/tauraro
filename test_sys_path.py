# Test sys.path functionality
import sys

print("=== Testing sys.path ===\n")

# Test 1: Check sys.path exists and has default paths
print("--- Test 1: Default sys.path ---")
print("sys.path:")
for path in sys.path:
    print("  -", path)
print()

# Test 2: Verify TAURARO_PATH environment variable
print("--- Test 2: TAURARO_PATH env var ---")
import os
if "TAURARO_PATH" in os.environ:
    print("TAURARO_PATH:", os.environ["TAURARO_PATH"])
else:
    print("TAURARO_PATH not set")
print()

# Test 3: Test path manipulation
print("--- Test 3: Path Manipulation ---")
print("Before append - paths:", len(sys.path))
sys.path_append("/custom/path")
print("After append - paths:", len(sys.path))

# Test 4: Check last path
print("Last path in sys.path:", sys.path[-1])

print("\n=== sys.path Tests Complete ===")
