#!/usr/bin/env python3
import subprocess

def test_triple_quote():
    """Test triple quote strings"""
    code = """s = '''multi
line'''
print(len(s))"""
    
    print("Testing triple quote strings:")
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
    
    print(f"Output: '{output}'")
    print(f"Stderr: {stderr}")
    print(f"Expected: '10'")
    print(f"Match: {'10' in output}")

test_triple_quote()
