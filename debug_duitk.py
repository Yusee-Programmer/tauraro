# Debug DUITK to understand what's happening

print("Loading DUITK...")
import duitk

print("Creating application...")
app = duitk.Application("Debug App")
print(f"App created: {app}")
print(f"App hinstance: {app.hinstance}")

print("\nCreating window...")
# Let's manually test the function calls that are failing
try:
    print("Calling GetModuleHandleA...")
    hinstance = call_function("kernel32.dll", "GetModuleHandleA", [0])
    print(f"Module handle: {hinstance}")
    
    if hinstance is None:
        print("GetModuleHandleA returned None!")
        # Try to get last error
        try:
            error = call_function("kernel32.dll", "GetLastError", [])
            print(f"Last error: {error}")
        except Exception as e:
            print(f"Error getting last error: {e}")
except Exception as e:
    print(f"Error calling GetModuleHandleA: {e}")

print("\nTest completed.")