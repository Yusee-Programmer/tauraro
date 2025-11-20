#!/usr/bin/env python3
import subprocess

def test_decorator_simple():
    """Test a simpler decorator pattern"""
    code = """
def my_decorator(f):
    def wrapper():
        return f() + 1
    return wrapper

@my_decorator
def get_num():
    return 41

print(get_num())
"""
    
    print("Testing decorator with named function wrapper:")
    input_data = code + "\n\nexit()\n"
    
    process = subprocess.Popen(
        [r'.\target\release\tauraro.exe', 'repl'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    
    stdout, stderr = process.communicate(input=input_data)
    
    # Clean output
    lines = stdout.split('\n')
    output_lines = []
    in_banner = True
    
    for line in lines:
        if in_banner:
            if line.startswith('Tauraro') or line.startswith('[Rust-based') or line.startswith('Type "help'):
                continue
            elif line.strip() == '':
                in_banner = False
                continue
            else:
                in_banner = False
        
        if not (line.startswith('>>> ') or line.startswith('... ')):
            output_lines.append(line)
    
    output = '\n'.join(output_lines).strip()
    
    print(f"Output: {output}")
    print(f"Stderr: {stderr}")
    print(f"Success: {'42' in output}")

test_decorator_simple()
