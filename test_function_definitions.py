# Test if function definitions are working

print("Testing function definitions...")

# Try to define and call a simple function
try:
    print("Defining GetModuleHandleA...")
    define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])
    print("Function defined successfully!")
    
    print("Calling GetModuleHandleA...")
    result = call_function("kernel32.dll", "GetModuleHandleA", [0])
    print(f"Result: {result}")
    
except Exception as e:
    print(f"Error: {e}")
    import traceback
    traceback.print_exc()

print("Test completed.")