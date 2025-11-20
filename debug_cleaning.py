#!/usr/bin/env python3
import subprocess

def test_output_cleaning():
    """Test the output cleaning logic"""
    code = "if True:\n    print('yes')"
    input_data = code + "\n\nexit()\n"
    
    process = subprocess.Popen(
        [r'.\target\release\tauraro.exe', 'repl'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    
    stdout, stderr = process.communicate(input=input_data)
    
    print("Raw output lines:")
    lines = stdout.split('\n')
    for i, line in enumerate(lines):
        print(f"  [{i}] {repr(line)}")
    
    print("\nCleaning output...")
    output_lines = []
    in_banner = True
    
    for i, line in enumerate(lines):
        print(f"Processing line {i}: {repr(line)}, in_banner={in_banner}")
        
        # Skip banner lines (first few lines before actual output)
        if in_banner:
            if line.startswith('Tauraro'):
                print(f"  -> Skipping (banner line)")
                continue
            elif line.startswith('[Rust-based'):
                print(f"  -> Skipping (banner line)")
                continue
            elif line.startswith('Type "help'):
                print(f"  -> Skipping (banner line)")
                continue
            elif line.strip() == '':
                # End of banner
                print(f"  -> Skipping (empty banner line, marking in_banner=False)")
                in_banner = False
                continue
            else:
                in_banner = False
                print(f"  -> End of banner (no match), marking in_banner=False")
        
        # Remove prompt characters but keep the output
        if line.startswith('>>> ') or line.startswith('... '):
            print(f"  -> Skipping (prompt line)")
            pass
        else:
            print(f"  -> Adding to output")
            output_lines.append(line)
    
    cleaned_stdout = '\n'.join(output_lines).strip()
    
    print("\nCleaned output:")
    print(repr(cleaned_stdout))
    print("\nFormatted:")
    print(cleaned_stdout)
    
    print(f"\nDoes cleaned output contain 'yes'? {('yes' in cleaned_stdout)}")

if __name__ == '__main__':
    test_output_cleaning()
