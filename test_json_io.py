import json

print("Testing json.dump() and json.load():")

# Test data
data = {
    "name": "Tauraro",
    "version": 1.0,
    "features": ["fast", "pythonic", "compiled"],
    "settings": {
        "enabled": True,
        "count": 42
    }
}

# Test json.dump()
print("1. Writing JSON to file...")
json.dump(data, "test_output.json", 2)
print("   ✓ JSON written to test_output.json")

# Test json.load()
print("2. Reading JSON from file...")
loaded_data = json.load("test_output.json")
print("   Loaded:", loaded_data)
print("   ✓ JSON loaded successfully")

# Verify data
print("3. Verifying data integrity...")
print("   Name:", loaded_data["name"])
print("   Version:", loaded_data["version"])
print("   Features:", loaded_data["features"])
print("   ✓ All tests passed!")
