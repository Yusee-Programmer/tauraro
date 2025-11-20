#!/usr/bin/env python3
import subprocess
import sys

def test_issue(name, code, expected):
    """Test a specific issue"""
    print(f"\n{'='*60}")
    print(f"TEST: {name}")
    print(f"{'='*60}")
    print(f"Code:\n{code}\n")
    
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
    
    print(f"Expected:\n{expected}\n")
    print(f"Got:\n{output}\n")
    print(f"Match: {expected in output}")
    if stderr:
        print(f"Stderr:\n{stderr}")

# Test 1: List formatting
test_issue(
    "List string formatting",
    "print(list('abc'))",
    "['a', 'b', 'c']"
)

# Test 2: Dict formatting  
test_issue(
    "Dict string formatting",
    "print(dict([('a', 1)]))",
    "{'a': 1}"
)

# Test 3: Zip formatting
test_issue(
    "Zip string formatting",
    "print(list(zip([1, 2], ['a', 'b'])))",
    "[(1, 'a'), (2, 'b')]"
)

# Test 4: Decorators
test_issue(
    "Decorators",
    "@lambda f: lambda: f() + 1\ndef get_num():\n    return 41\nprint(get_num())",
    "42"
)

# Test 5: Generators
test_issue(
    "Generators",
    "def gen():\n    yield 1\n    yield 2\nprint(list(gen()))",
    "[1, 2]"
)
