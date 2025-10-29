#!/usr/bin/env python3
"""
Script to automatically fix Dict variant usage after changing from HashMap to Rc<RefCell<HashMap>>
"""

import re
import os
import glob

def fix_file(filepath):
    """Fix a single file"""
    print(f"Processing {filepath}...")

    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    original_content = content

    # Add imports if needed
    has_rc = 'use std::rc::Rc;' in content
    has_refcell = 'use std::cell::RefCell;' in content

    if 'Value::Dict(' in content or 'dict.iter()' in content or 'dict.len()' in content:
        # Find the imports section
        if not has_rc and not has_refcell:
            # Add both imports after 'use std::collections::HashMap;'
            content = content.replace(
                'use std::collections::HashMap;',
                'use std::collections::HashMap;\nuse std::rc::Rc;\nuse std::cell::RefCell;'
            )
        elif not has_rc:
            content = content.replace(
                'use std::collections::HashMap;',
                'use std::collections::HashMap;\nuse std::rc::Rc;'
            )
        elif not has_refcell:
            content = content.replace(
                'use std::rc::Rc;',
                'use std::rc::Rc;\nuse std::cell::RefCell;'
            )

    # Fix patterns
    # 1. Ok(Value::Dict(HashMap::new())) -> Ok(Value::Dict(Rc::new(RefCell::new(HashMap::new()))))
    content = re.sub(
        r'Value::Dict\(HashMap::new\(\)\)',
        'Value::Dict(Rc::new(RefCell::new(HashMap::new())))',
        content
    )

    # 2. Value::Dict(dict) where dict is HashMap -> Value::Dict(Rc::new(RefCell::new(dict)))
    # This is tricky, need context...
    # Only fix if it's: Ok(Value::Dict(dict)) or return Value::Dict(dict) where dict is a plain HashMap
    content = re.sub(
        r'(Ok\(Value::Dict\(|return Value::Dict\()([a-z_]+)\)\)',
        lambda m: f"{m.group(1)}Rc::new(RefCell::new({m.group(2)})))" if m.group(2) not in ['dict.clone()', 'dict.borrow()'] else m.group(0),
        content
    )

    # 3. dict.iter() -> dict.borrow().iter()
    content = re.sub(
        r'(\b[a-z_]+)\.iter\(\)',
        lambda m: f'{m.group(1)}.borrow().iter()' if 'dict' in m.group(1).lower() or 'map' == m.group(1) else m.group(0),
        content
    )

    # 4. dict.len() -> dict.borrow().len()
    content = re.sub(
        r'dict\.len\(\)',
        'dict.borrow().len()',
        content
    )

    # 5. dict.is_empty() -> dict.borrow().is_empty()
    content = re.sub(
        r'dict\.is_empty\(\)',
        'dict.borrow().is_empty()',
        content
    )

    # 6. dict.get(...) -> dict.borrow().get(...)
    content = re.sub(
        r'dict\.get\(',
        'dict.borrow().get(',
        content
    )

    # 7. dict.contains_key(...) -> dict.borrow().contains_key(...)
    content = re.sub(
        r'dict\.contains_key\(',
        'dict.borrow().contains_key(',
        content
    )

    # 8. dict.keys() -> dict.borrow().keys()
    content = re.sub(
        r'dict\.keys\(\)',
        'dict.borrow().keys()',
        content
    )

    # Write back if changed
    if content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"  Fixed {filepath}")
        return True
    return False

def main():
    # Find all .rs files in src/
    files = []
    for pattern in ['src/**/*.rs', 'src/*.rs']:
        files.extend(glob.glob(pattern, recursive=True))

    fixed_count = 0
    for filepath in files:
        if 'target' not in filepath:  # Skip target directory
            if fix_file(filepath):
                fixed_count += 1

    print(f"\nFixed {fixed_count} files")

if __name__ == '__main__':
    main()
