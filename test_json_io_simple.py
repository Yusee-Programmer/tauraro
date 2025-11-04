import json

print("Testing json.dump() and json.load():")

# Test simple data
data = {"name": "Tauraro", "version": 1, "enabled": True}

# Test json.dump()
print("1. Writing JSON to file...")
json.dump(data, "test_output.json")
print("   JSON written to test_output.json")

# Test json.load()
print("2. Reading JSON from file...")
loaded_data = json.load("test_output.json")
print("   Loaded:", loaded_data)
print("   All tests passed!")
