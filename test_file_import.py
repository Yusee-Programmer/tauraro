# Test importing user-defined modules
print("=== Testing File-Based Imports ===\n")

# Test 1: Try to import mymodule (should fail for now, but we'll fix it)
print("--- Test 1: Import User Module ---")
try:
    import mymodule
    print("mymodule imported successfully!")
    print("mymodule.PI:", mymodule.PI)
    print("mymodule.greet('World'):", mymodule.greet('World'))
except Exception as e:
    print("Failed to import mymodule:", str(e))

print("\n=== File Import Tests Complete ===")
