#!/usr/bin/env python3
import subprocess
import sys

def test_repl_if():
    """Test multiline if in REPL via subprocess"""
    code = "if True:\n    print('yes')"
    input_data = code + "\n\nexit()\n"
    
    print("=== TESTING MULTILINE IF ===")
    print(f"Input code:\n{repr(input_data)}")
    print()
    
    process = subprocess.Popen(
        [r'.\target\release\tauraro.exe', 'repl'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    
    stdout, stderr = process.communicate(input=input_data)
    
    print("Raw STDOUT:")
    print(repr(stdout))
    print()
    print("Raw STDERR:")
    print(repr(stderr))
    print()
    print("Formatted STDOUT:")
    print(stdout)
    print()
    print("Formatted STDERR:")
    print(stderr)

if __name__ == '__main__':
    test_repl_if()
