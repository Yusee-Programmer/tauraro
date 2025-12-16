#!/usr/bin/env python3
"""
Test script for shared library generation
This script will be compiled to a shared library (.so on Linux, .dll on Windows, .dylib on macOS)
"""

def add(a, b):
    """Add two numbers"""
    return a + b

def multiply(a, b):
    """Multiply two numbers"""
    return a * b

def greet(name):
    """Greet someone"""
    return f"Hello, {name}!"

# Test the functions
if __name__ == "__main__":
    print("Testing shared library functions:")
    print(f"add(5, 3) = {add(5, 3)}")
    print(f"multiply(4, 7) = {multiply(4, 7)}")
    print(f"greet('Tauraro') = {greet('Tauraro')}")
