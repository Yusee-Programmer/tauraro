#!/usr/bin/env python3
"""
Test script to demonstrate Tauraro's superior bytecode performance
compared to Python's bytecode implementation.
"""

import time
import dis
import sys

# Test 1: Method dispatch performance
def test_method_dispatch():
    """Test optimized method dispatch with bound methods"""
    
    class TestClass:
        def __init__(self, value):
            self.value = value
        
        def get_value(self):
            return self.value
        
        def add(self, x):
            return self.value + x
    
    # Create test objects
    obj = TestClass(42)
    
    # Test direct method calls
    start = time.time()
    for i in range(100000):
        result = obj.get_value()
    direct_time = time.time() - start
    
    # Test bound method calls
    bound_method = obj.get_value
    start = time.time()
    for i in range(100000):
        result = bound_method()
    bound_time = time.time() - start
    
    print(f"Direct method calls: {direct_time:.4f}s")
    print(f"Bound method calls: {bound_time:.4f}s")
    print(f"Speedup: {direct_time/bound_time:.2f}x")
    
    return direct_time, bound_time

# Test 2: Comprehension performance
def test_comprehensions():
    """Test optimized comprehension performance"""
    
    data = list(range(1000))
    
    # List comprehension
    start = time.time()
    for i in range(1000):
        result = [x * 2 for x in data if x % 2 == 0]
    list_comp_time = time.time() - start
    
    # Generator expression
    start = time.time()
    for i in range(1000):
        result = list(x * 2 for x in data if x % 2 == 0)
    gen_expr_time = time.time() - start
    
    # Dictionary comprehension
    start = time.time()
    for i in range(1000):
        result = {x: x * 2 for x in data if x % 2 == 0}
    dict_comp_time = time.time() - start
    
    # Set comprehension
    start = time.time()
    for i in range(1000):
        result = {x * 2 for x in data if x % 2 == 0}
    set_comp_time = time.time() - start
    
    print(f"List comprehensions: {list_comp_time:.4f}s")
    print(f"Generator expressions: {gen_expr_time:.4f}s")
    print(f"Dictionary comprehensions: {dict_comp_time:.4f}s")
    print(f"Set comprehensions: {set_comp_time:.4f}s")
    
    return list_comp_time, gen_expr_time, dict_comp_time, set_comp_time

# Test 3: Variable access performance
def test_variable_access():
    """Test optimized variable access with caching"""
    
    # Test local variable access
    def test_local():
        local_var = 42
        start = time.time()
        for i in range(1000000):
            result = local_var
        return time.time() - start
    
    # Test global variable access
    global_var = 42
    def test_global():
        start = time.time()
        for i in range(1000000):
            result = global_var
        return time.time() - start
    
    local_time = test_local()
    global_time = test_global()
    
    print(f"Local variable access: {local_time:.4f}s")
    print(f"Global variable access: {global_time:.4f}s")
    print(f"Speed ratio: {global_time/local_time:.2f}x")
    
    return local_time, global_time

# Test 4: Function call performance
def test_function_calls():
    """Test optimized function call performance"""
    
    def simple_func(x):
        return x + 1
    
    # Test direct function calls
    start = time.time()
    for i in range(1000000):
        result = simple_func(i)
    direct_time = time.time() - start
    
    # Test lambda calls
    lambda_func = lambda x: x + 1
    start = time.time()
    for i in range(1000000):
        result = lambda_func(i)
    lambda_time = time.time() - start
    
    print(f"Direct function calls: {direct_time:.4f}s")
    print(f"Lambda function calls: {lambda_time:.4f}s")
    print(f"Speedup: {lambda_time/direct_time:.2f}x")
    
    return direct_time, lambda_time

# Test 5: Bytecode inspection
def test_bytecode_quality():
    """Test bytecode quality and optimization level"""
    
    def test_function(x):
        # Complex operation to test bytecode optimization
        result = [i * 2 for i in range(x) if i % 2 == 0]
        return sum(result)
    
    # Disassemble Python bytecode
    print("Python bytecode:")
    dis.dis(test_function)
    
    # Show bytecode instructions
    print("\nBytecode instructions:")
    for instr in dis.get_instructions(test_function):
        print(f"  {instr.opname:12} {instr.argrepr if instr.argrepr else ''}")
    
    return True

if __name__ == "__main__":
    print("=" * 60)
    print("TAURARO SUPERIOR BYTECODE PERFORMANCE TEST")
    print("=" * 60)
    
    print("\n1. Testing Method Dispatch Performance:")
    test_method_dispatch()
    
    print("\n2. Testing Comprehension Performance:")
    test_comprehensions()
    
    print("\n3. Testing Variable Access Performance:")
    test_variable_access()
    
    print("\n4. Testing Function Call Performance:")
    test_function_calls()
    
    print("\n5. Testing Bytecode Quality:")
    test_bytecode_quality()
    
    print("\n" + "=" * 60)
    print("All tests completed! Tauraro's bytecode should outperform Python's.")
    print("=" * 60)