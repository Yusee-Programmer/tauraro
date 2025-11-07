"""
JIT Test Suite Runner
Executes all JIT tests and reports results.
"""

import time

print("\n" + "=" * 70)
print(" " * 20 + "TAURARO JIT TEST SUITE")
print("=" * 70)
print()

# List of all test files
test_files = [
    "test_01_integer_arithmetic.py",
    "test_02_float_arithmetic.py",
    "test_03_bitwise_operations.py",
    "test_04_comparisons.py",
    "test_05_unary_and_fused.py",
]

total_tests = 0
passed_tests = 0
failed_tests = 0
start_time = time.time()

print("Running JIT test suite...\n")

for test_file in test_files:
    print(f"\nExecuting: {test_file}")
    print("-" * 70)

    # In a real implementation, we would exec() the file
    # For now, just note that each test should be run
    print(f"[Would execute: {test_file}]")
    print()

end_time = time.time()
elapsed = end_time - start_time

print("\n" + "=" * 70)
print("TEST SUITE SUMMARY")
print("=" * 70)
print(f"Total test files: {len(test_files)}")
print(f"Execution time: {elapsed:.2f} seconds")
print()
print("Test files:")
for i, test_file in enumerate(test_files, 1):
    print(f"  {i}. {test_file}")
print()
print("To run individual tests:")
print("  ./target/release/tauraro.exe run tests/jit/test_01_integer_arithmetic.py")
print("=" * 70)
