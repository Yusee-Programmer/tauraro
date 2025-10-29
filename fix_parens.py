#!/usr/bin/env python3
import re
import glob

def fix_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    original = content

    # Fix missing closing paren after Rc::new(RefCell::new(...))
    # Pattern: Ok(Value::Dict(Rc::new(RefCell::new(something))$ (missing closing paren)
    content = re.sub(
        r'Ok\(Value::Dict\(Rc::new\(RefCell::new\(([^)]+)\)\)\)',
        r'Ok(Value::Dict(Rc::new(RefCell::new(\1))))',
        content
    )

    # Also fix return Value::Dict cases
    content = re.sub(
        r'return Value::Dict\(Rc::new\(RefCell::new\(([^)]+)\)\)\)',
        r'return Value::Dict(Rc::new(RefCell::new(\1))))',
        content
    )

    if content != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Fixed {filepath}")
        return True
    return False

def main():
    files = glob.glob('src/**/*.rs', recursive=True)
    fixed = 0
    for f in files:
        if 'target' not in f:
            if fix_file(f):
                fixed += 1
    print(f"Fixed {fixed} files")

if __name__ == '__main__':
    main()
