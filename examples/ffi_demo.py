# FFI Demo - Loading and calling system libraries

# On Windows, load kernel32.dll and call GetTickCount
# On Unix-like systems, load libc and call time functions

import os
import sys

def test_ffi():
    print("Testing FFI functionality...")
    
    try:
        if sys.platform == "win32":
            # Windows example
            print("Loading kernel32.dll...")
            load_library("kernel32.dll")
            
            # Define GetTickCount function
            print("Defining GetTickCount function...")
            define_function("kernel32.dll", "GetTickCount", "int32", [])
            
            # Call the function
            print("Calling GetTickCount...")
            tick_count = call_function("kernel32.dll", "GetTickCount", [])
            print(f"Tick count: {tick_count}")
            
        else:
            # Unix-like example - try to load libc
            print("Loading libc...")
            try:
                load_library("libc.so.6")  # Linux
                libc_name = "libc.so.6"
            except:
                try:
                    load_library("libc.dylib")  # macOS
                    libc_name = "libc.dylib"
                except:
                    load_library("c")  # Generic
                    libc_name = "c"
            
            # Define time function
            print("Defining time function...")
            define_function(libc_name, "time", "int64", ["pointer"])
            
            # Call the function
            print("Calling time function...")
            current_time = call_function(libc_name, "time", [None])
            print(f"Current time: {current_time}")
            
        # Test library management functions
        print("\nTesting library management...")
        libs = list_libraries()
        print(f"Loaded libraries: {libs}")
        
        if libs:
            info = library_info(libs[0])
            print(f"Library info: {info}")
            
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    test_ffi()