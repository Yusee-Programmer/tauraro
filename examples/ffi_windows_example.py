"""
FFI Windows Example - Calling Windows API functions from Tauraro

This example demonstrates how to use Tauraro's FFI to call native Windows API functions.
"""

# Load Windows kernel32.dll
print("Loading kernel32.dll...")
load_library("kernel32.dll")
load_library("user32.dll")

# Define GetTickCount function (returns milliseconds since system startup)
# DWORD GetTickCount(void)
print("\nDefining GetTickCount function...")
define_function("kernel32.dll", "GetTickCount", "uint32", [])

# Call GetTickCount
print("Calling GetTickCount...")
tick_count = call_function("kernel32.dll", "GetTickCount", [])
print(f"System uptime: {tick_count} milliseconds")
print(f"System uptime: {tick_count / 1000 / 60} minutes")

# Define Sleep function
# void Sleep(DWORD dwMilliseconds)
print("\nDefining Sleep function...")
define_function("kernel32.dll", "Sleep", "void", ["uint32"])

# Sleep for 1 second
print("Sleeping for 1000 milliseconds...")
call_function("kernel32.dll", "Sleep", [1000])
print("Done sleeping!")

# Define GetCurrentProcessId
# DWORD GetCurrentProcessId(void)
print("\nDefining GetCurrentProcessId function...")
define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])

# Get current process ID
pid = call_function("kernel32.dll", "GetCurrentProcessId", [])
print(f"Current Process ID: {pid}")

# Define MessageBoxA (if user32.dll is available)
# int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
print("\nDefining MessageBoxA function...")
define_function("user32.dll", "MessageBoxA", "int32", ["pointer", "string", "string", "uint32"])

# Show a message box (uncomment to test - this will show a GUI dialog)
# result = call_function("user32.dll", "MessageBoxA", [0, "Hello from Tauraro FFI!", "Tauraro", 0])
# print(f"MessageBox result: {result}")

# List all loaded libraries
print("\n=== Loaded Libraries ===")
libs = list_libraries()
for lib in libs:
    print(f"- {lib}")
    info = library_info(lib)
    print(f"  Path: {info['path']}")
    print(f"  Functions: {info['functions']}")

print("\nFFI Windows example completed successfully!")
