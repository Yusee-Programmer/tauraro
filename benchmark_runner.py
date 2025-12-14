#!/usr/bin/env python3
"""
Comprehensive Tauraro vs Python Benchmark Runner
Compiles Tauraro files to C executables and compares performance with Python
"""

import subprocess
import time
import os
import re

# List of benchmark files
BENCHMARKS = [
    "benchmark_01_basic_types",
    "benchmark_02_arithmetic",
    "benchmark_03_control_flow",
    "benchmark_04_functions",
]

# Colors for terminal output
class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    BOLD = '\033[1m'
    END = '\033[0m'

def compile_to_c(py_file, base_name):
    """Compile Python file to C using Tauraro"""
    print(f"{Colors.YELLOW}[1/3] Compiling {py_file} to C...{Colors.END}")

    cmd = [
        "./target/release/tauraro.exe",
        "compile",
        py_file,
        "--use-native-transpiler",
        "--backend", "c",
        "-o", base_name
    ]

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        c_file = f"{base_name}.exe.c"

        if os.path.exists(c_file):
            print(f"{Colors.GREEN}OK C code generated{Colors.END}")
            return c_file
        else:
            print(f"{Colors.RED}X C code generation failed{Colors.END}")
            if result.stderr:
                print(f"Error: {result.stderr[:200]}")
            return None
    except subprocess.TimeoutExpired:
        print(f"{Colors.RED}X Compilation timed out{Colors.END}")
        return None
    except Exception as e:
        print(f"{Colors.RED}X Error: {e}{Colors.END}")
        return None

def patch_c_code(c_file):
    """Apply temporary patches to generated C code"""
    print(f"{Colors.YELLOW}[2/3] Patching C code...{Colors.END}")

    try:
        with open(c_file, 'r') as f:
            content = f.read()

        # Add len function if missing
        if 'int tauraro_len_string' not in content:
            # Find insertion point after tauraro_float_int
            insert_pos = content.find('double tauraro_float_int(int value) {\n    return (double)value;\n}')
            if insert_pos != -1:
                insert_pos = content.find('}', insert_pos) + 1
                len_func = '\n\nint tauraro_len_string(const char* str) {\n    return str ? strlen(str) : 0;\n}\n'
                content = content[:insert_pos] + len_func + content[insert_pos:]

        # Fix recursive main() call
        content = re.sub(r'    main\(\);', '    // main() removed', content)

        with open(c_file, 'w') as f:
            f.write(content)

        print(f"{Colors.GREEN}OK Patching complete{Colors.END}")
        return True
    except Exception as e:
        print(f"{Colors.RED}X Patching failed: {e}{Colors.END}")
        return False

def compile_c_to_exe(c_file, exe_file):
    """Compile C code to native executable using GCC"""
    print(f"{Colors.YELLOW}[3/3] Compiling C to executable...{Colors.END}")

    cmd = ["gcc", c_file, "-o", exe_file, "-lm", "-O3"]

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        if result.returncode == 0:
            print(f"{Colors.GREEN}OK Compilation successful{Colors.END}")
            return True
        else:
            print(f"{Colors.RED}X GCC compilation failed{Colors.END}")
            errors = result.stderr[:500]
            if errors:
                print(f"Errors: {errors}")
            return False
    except Exception as e:
        print(f"{Colors.RED}X Error: {e}{Colors.END}")
        return False

def run_benchmark(command, timeout=120):
    """Run a benchmark and measure time"""
    try:
        start_time = time.time()
        result = subprocess.run(command, capture_output=True, text=True, timeout=timeout)
        end_time = time.time()

        if result.returncode == 0:
            return end_time - start_time, result.stdout
        else:
            return None, None
    except subprocess.TimeoutExpired:
        return None, None
    except Exception as e:
        print(f"Error running: {e}")
        return None, None

def main():
    print("=" * 70)
    print(f"{Colors.BOLD}TAURARO vs PYTHON - COMPREHENSIVE BENCHMARKS{Colors.END}")
    print("=" * 70)
    print()

    results = []

    for benchmark in BENCHMARKS:
        print()
        print("-" * 70)
        print(f"{Colors.BLUE}{Colors.BOLD}Benchmark: {benchmark}{Colors.END}")
        print("-" * 70)

        py_file = f"{benchmark}.py"
        exe_file = f"{benchmark}.exe"

        # Step 1: Compile to C
        c_file = compile_to_c(py_file, benchmark)
        if not c_file:
            results.append({
                'name': benchmark,
                'status': 'COMPILE_FAILED',
                'py_time': None,
                'tau_time': None,
                'speedup': None
            })
            continue

        # Step 2: Patch C code
        if not patch_c_code(c_file):
            results.append({
                'name': benchmark,
                'status': 'PATCH_FAILED',
                'py_time': None,
                'tau_time': None,
                'speedup': None
            })
            continue

        # Step 3: Compile to executable
        if not compile_c_to_exe(c_file, exe_file):
            results.append({
                'name': benchmark,
                'status': 'GCC_FAILED',
                'py_time': None,
                'tau_time': None,
                'speedup': None
            })
            continue

        print()
        # Step 4: Run benchmarks
        print(f"{Colors.YELLOW}Running benchmarks...{Colors.END}")

        # Run Python
        print(f"  {Colors.BLUE}Python...{Colors.END}", end=' ', flush=True)
        py_time, py_output = run_benchmark(["python", py_file])
        if py_time is not None:
            print(f"{Colors.GREEN}OK {py_time:.3f}s{Colors.END}")
        else:
            print(f"{Colors.RED}X Failed{Colors.END}")
            results.append({
                'name': benchmark,
                'status': 'PY_RUN_FAILED',
                'py_time': None,
                'tau_time': None,
                'speedup': None
            })
            continue

        # Run Tauraro
        print(f"  {Colors.BLUE}Tauraro...{Colors.END}", end=' ', flush=True)
        tau_time, tau_output = run_benchmark([f"./{exe_file}"])
        if tau_time is not None:
            print(f"{Colors.GREEN}OK {tau_time:.3f}s{Colors.END}")
        else:
            print(f"{Colors.RED}X Failed{Colors.END}")
            results.append({
                'name': benchmark,
                'status': 'TAU_RUN_FAILED',
                'py_time': py_time,
                'tau_time': None,
                'speedup': None
            })
            continue

        # Calculate speedup
        speedup = py_time / tau_time if tau_time > 0 else 0

        # Verify outputs
        output_match = (py_output.strip() == tau_output.strip())

        results.append({
            'name': benchmark,
            'status': 'SUCCESS',
            'py_time': py_time,
            'tau_time': tau_time,
            'speedup': speedup,
            'match': output_match
        })

        # Display results
        print()
        print(f"{Colors.BOLD}Results:{Colors.END}")
        print(f"  Python:  {py_time:.3f}s")
        print(f"  Tauraro: {tau_time:.3f}s")
        print(f"  Speedup: {Colors.GREEN}{Colors.BOLD}{speedup:.2f}x{Colors.END}")
        if output_match:
            print(f"  Output:  {Colors.GREEN}OK Match{Colors.END}")
        else:
            print(f"  Output:  {Colors.YELLOW}! Differ{Colors.END}")

    # Final Summary
    print()
    print()
    print("=" * 70)
    print(f"{Colors.BOLD}FINAL SUMMARY{Colors.END}")
    print("=" * 70)
    print()
    print(f"{'Benchmark':<35} {'Python (s)':>12} {'Tauraro (s)':>12} {'Speedup':>10} {'Status':>10}")
    print("-" * 80)

    total_speedup = 0
    successful_count = 0

    for r in results:
        name = r['name'][:34]
        if r['status'] == 'SUCCESS':
            py_t = f"{r['py_time']:.3f}"
            tau_t = f"{r['tau_time']:.3f}"
            speedup = f"{r['speedup']:.2f}x"
            status = f"{Colors.GREEN}OK{Colors.END}"
            total_speedup += r['speedup']
            successful_count += 1
        else:
            py_t = "N/A"
            tau_t = "N/A"
            speedup = "N/A"
            status = f"{Colors.RED}X{Colors.END}"

        print(f"{name:<35} {py_t:>12} {tau_t:>12} {speedup:>10} {status:>10}")

    print("=" * 80)

    if successful_count > 0:
        avg_speedup = total_speedup / successful_count
        print()
        print(f"{Colors.GREEN}{Colors.BOLD}Average Speedup: {avg_speedup:.2f}x{Colors.END}")
        print(f"{Colors.GREEN}Successful: {successful_count}/{len(BENCHMARKS)}{Colors.END}")
    else:
        print()
        print(f"{Colors.RED}No successful benchmarks{Colors.END}")

    print()

if __name__ == "__main__":
    main()
