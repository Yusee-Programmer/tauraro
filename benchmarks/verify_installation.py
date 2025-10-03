#!/usr/bin/env python3
"""
Verification Script
Checks that Tauraro is properly installed and benchmarks can be run
"""

import os
import sys
import subprocess
from pathlib import Path

def verify_tauraro_installation():
    """Verify that Tauraro is installed and accessible."""
    tauraro_exe = Path("../target/release/tauraro.exe")
    
    if not tauraro_exe.exists():
        print("❌ Tauraro executable not found!")
        print(f"   Expected location: {tauraro_exe.absolute()}")
        print("   Please compile Tauraro with: cargo build --release")
        return False
    
    try:
        # Test basic Tauraro execution
        result = subprocess.run(
            [str(tauraro_exe), "--help"],
            capture_output=True,
            text=True,
            timeout=10
        )
        
        if result.returncode == 0:
            print("✅ Tauraro installation verified")
            return True
        else:
            print("❌ Tauraro executable failed to run")
            print(f"   Error: {result.stderr}")
            return False
            
    except Exception as e:
        print(f"❌ Error testing Tauraro installation: {e}")
        return False

def verify_benchmark_files():
    """Verify that all benchmark files exist."""
    tauraro_dir = Path("tauraro")
    python_dir = Path("python")
    
    if not tauraro_dir.exists():
        print("❌ Tauraro benchmarks directory not found!")
        return False
        
    if not python_dir.exists():
        print("❌ Python benchmarks directory not found!")
        return False
    
    # Check for required benchmark files
    required_benchmarks = ["arithmetic", "string", "loop", "function", "sorting", "math"]
    
    missing_tauraro = []
    missing_python = []
    
    for benchmark in required_benchmarks:
        tauraro_file = tauraro_dir / f"{benchmark}_benchmark.tr"
        python_file = python_dir / f"{benchmark}_benchmark.py"
        
        if not tauraro_file.exists():
            missing_tauraro.append(tauraro_file)
            
        if not python_file.exists():
            missing_python.append(python_file)
    
    if missing_tauraro:
        print("❌ Missing Tauraro benchmark files:")
        for file in missing_tauraro:
            print(f"   {file}")
        return False
        
    if missing_python:
        print("❌ Missing Python benchmark files:")
        for file in missing_python:
            print(f"   {file}")
        return False
    
    print("✅ All benchmark files verified")
    return True

def run_simple_test():
    """Run a simple test to verify basic functionality."""
    try:
        # Create a simple test file
        test_content = """
print("Tauraro verification test")
result = 2 + 2
print("Result:", result)
print("Test completed successfully!")
"""
        
        test_file = Path("test_verification.tr")
        with open(test_file, "w") as f:
            f.write(test_content)
        
        # Run the test
        tauraro_exe = Path("../target/release/tauraro.exe")
        result = subprocess.run(
            [str(tauraro_exe), "run", str(test_file)],
            capture_output=True,
            text=True,
            timeout=10
        )
        
        # Clean up
        test_file.unlink()
        
        if result.returncode == 0 and "Result: 4" in result.stdout:
            print("✅ Tauraro basic functionality verified")
            return True
        else:
            print("❌ Tauraro basic functionality test failed")
            print(f"   stdout: {result.stdout}")
            print(f"   stderr: {result.stderr}")
            return False
            
    except Exception as e:
        print(f"❌ Error running basic functionality test: {e}")
        return False

def main():
    print("Tauraro vs Python Benchmark Verification")
    print("=" * 50)
    
    checks = [
        ("Tauraro Installation", verify_tauraro_installation),
        ("Benchmark Files", verify_benchmark_files),
        ("Basic Functionality", run_simple_test)
    ]
    
    all_passed = True
    
    for check_name, check_function in checks:
        print(f"\nChecking {check_name}...")
        if not check_function():
            all_passed = False
    
    print("\n" + "=" * 50)
    if all_passed:
        print("✅ All verification checks passed!")
        print("You can now run the benchmarks with: python run_benchmarks.py")
        return 0
    else:
        print("❌ Some verification checks failed!")
        print("Please fix the issues above before running benchmarks.")
        return 1

if __name__ == "__main__":
    sys.exit(main())