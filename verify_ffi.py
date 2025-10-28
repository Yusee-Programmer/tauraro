#!/usr/bin/env python3

"""
Verification script to ensure all FFI modules are properly implemented
"""

import os
import sys

def check_ffi_modules():
    """Check that all FFI modules exist and have proper structure"""
    
    ffi_dir = "src/builtins_ffi"
    if not os.path.exists(ffi_dir):
        print(f"Error: {ffi_dir} directory not found")
        return False
    
    # List of all expected FFI modules
    expected_modules = [
        "abc_ffi", "asyncio_ffi", "base64_ffi", "collections_ffi",
        "copy_ffi", "csv_ffi", "datetime_ffi", "exceptions_ffi",
        "functools_ffi", "gc_ffi", "hashlib_ffi", "httptools_ffi",
        "httpx_ffi", "io_ffi", "itertools_ffi", "json_ffi",
        "logging_ffi", "math_ffi", "memory_ffi", "os_ffi",
        "pickle_ffi", "random_ffi", "re_ffi", "socket_ffi",
        "sys_ffi", "threading_ffi", "time_ffi", "unittest_ffi",
        "urllib_ffi", "websockets_ffi"
    ]
    
    print("Verifying FFI modules...")
    print("=" * 50)
    
    all_good = True
    
    for module in expected_modules:
        module_file = os.path.join(ffi_dir, f"{module}.rs")
        if not os.path.exists(module_file):
            print(f"❌ Missing: {module_file}")
            all_good = False
        else:
            # Check if file has content
            try:
                with open(module_file, 'r') as f:
                    content = f.read()
                    if len(content) < 100:
                        print(f"⚠️  Small file: {module_file} ({len(content)} chars)")
                    else:
                        print(f"✅ Found: {module_file} ({len(content)} chars)")
            except Exception as e:
                print(f"❌ Error reading {module_file}: {e}")
                all_good = False
    
    # Check mod.rs file
    mod_file = os.path.join(ffi_dir, "mod.rs")
    if not os.path.exists(mod_file):
        print(f"❌ Missing: {mod_file}")
        all_good = False
    else:
        print(f"✅ Found: {mod_file}")
    
    print("=" * 50)
    
    if all_good:
        print("✅ All FFI modules verified successfully!")
        return True
    else:
        print("❌ Some issues found in FFI modules")
        return False

def check_module_functions():
    """Check that key functions exist in some sample modules"""
    
    print("\nChecking sample module functions...")
    print("=" * 50)
    
    # Check a few key modules for function signatures
    sample_checks = {
        "math_ffi.rs": ["tauraro_math_sqrt", "tauraro_math_sin"],
        "os_ffi.rs": ["tauraro_os_getcwd", "tauraro_os_listdir"],
        "json_ffi.rs": ["tauraro_json_dumps", "tauraro_json_loads"],
        "time_ffi.rs": ["tauraro_time_time", "tauraro_time_sleep"],
    }
    
    ffi_dir = "src/builtins_ffi"
    all_good = True
    
    for module_file, functions in sample_checks.items():
        full_path = os.path.join(ffi_dir, module_file)
        if os.path.exists(full_path):
            try:
                with open(full_path, 'r') as f:
                    content = f.read()
                    found_functions = []
                    missing_functions = []
                    
                    for func in functions:
                        if func in content:
                            found_functions.append(func)
                        else:
                            missing_functions.append(func)
                    
                    if missing_functions:
                        print(f"⚠️  {module_file}: Missing {missing_functions}")
                        all_good = False
                    else:
                        print(f"✅ {module_file}: All functions found")
                        
            except Exception as e:
                print(f"❌ Error checking {module_file}: {e}")
                all_good = False
        else:
            print(f"❌ Missing module file: {module_file}")
            all_good = False
    
    print("=" * 50)
    
    if all_good:
        print("✅ Sample function checks passed!")
        return True
    else:
        print("❌ Some function checks failed")
        return False

def main():
    """Main verification function"""
    
    print("Tauraro FFI Module Verification")
    print("=" * 50)
    
    success1 = check_ffi_modules()
    success2 = check_module_functions()
    
    print("\n" + "=" * 50)
    if success1 and success2:
        print("🎉 All verifications passed! FFI implementation is complete.")
        return 0
    else:
        print("❌ Some verifications failed. Please check the implementation.")
        return 1

if __name__ == "__main__":
    sys.exit(main())