#!/usr/bin/env python3
"""Compile all FFI modules to object files."""

import subprocess
import sys
from pathlib import Path

# List of all FFI modules
modules = [
    "abc", "asyncio", "base64", "collections", "copy", "csv", "datetime",
    "exceptions", "functools", "gc", "hashlib", "httptools", "httpx", "io",
    "itertools", "json", "logging", "memory", "os", "pickle", "random", "re",
    "socket", "sys", "threading", "time", "unittest", "urllib", "websockets"
]

root_dir = Path(__file__).parent
src_dir = root_dir / "src" / "builtins_ffi"
build_dir = root_dir / "build" / "builtin"

# Create build directory
build_dir.mkdir(parents=True, exist_ok=True)

success_count = 0
failed_count = 0
failed_modules = []

for module in modules:
    src_file = src_dir / f"{module}_ffi.rs"
    obj_file = build_dir / f"{module}_ffi.o"

    if not src_file.exists():
        print(f"WARNING: {module}_ffi.rs not found, skipping...")
        continue

    print(f"Compiling {module}_ffi.rs...", end=" ")

    try:
        result = subprocess.run(
            [
                "rustc",
                "--crate-type", "staticlib",
                "--emit", "obj",
                "-C", "panic=abort",
                "-O",
                str(src_file),
                "-o", str(obj_file)
            ],
            capture_output=True,
            text=True,
            timeout=60
        )

        if result.returncode == 0:
            print(f"[SUCCESS]")
            success_count += 1
        else:
            print(f"[FAILED]")
            failed_count += 1
            failed_modules.append(module)
            # Print first error
            errors = [line for line in result.stderr.split('\n') if line.startswith('error')]
            if errors:
                print(f"   Error: {errors[0]}")

    except subprocess.TimeoutExpired:
        print(f"[TIMEOUT]")
        failed_count += 1
        failed_modules.append(module)
    except Exception as e:
        print(f"[EXCEPTION]: {e}")
        failed_count += 1
        failed_modules.append(module)

print(f"\n{'='*60}")
print(f"Compilation Summary:")
print(f"  Success: {success_count}/{len(modules)}")
print(f"  Failed:  {failed_count}/{len(modules)}")

if failed_modules:
    print(f"\nFailed modules: {', '.join(failed_modules)}")
    sys.exit(1)
else:
    print(f"\n[OK] All modules compiled successfully!")
    sys.exit(0)
