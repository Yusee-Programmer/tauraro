import time
import subprocess
import sys

def run_benchmark(filename, language):
    if language == "python":
        cmd = [sys.executable, filename]
    else:
        # Use 'tauraro' on Linux/Mac, 'tauraro.exe' on Windows
        import os
        exe_name = "tauraro.exe" if os.name == "nt" else "tauraro"
        cmd = [f"./target/release/{exe_name}", "run", filename]

    start = time.time()
    result = subprocess.run(cmd, capture_output=True, text=True)
    end = time.time()

    return {
        "output": result.stdout,
        "error": result.stderr,
        "time": end - start,
        "returncode": result.returncode
    }

def parse_benchmark_time(output):
    lines = output.strip().split("\n")
    for line in lines:
        if "Total time:" in line:
            parts = line.split(":")
            if len(parts) >= 2:
                time_str = parts[1].strip().split()[0]
                return float(time_str)
    return 0.0

def main():
    benchmarks = [
        "benchmarks/bench_arithmetic.py",
        "benchmarks/bench_loops.py",
        "benchmarks/bench_functions.py",
        "benchmarks/bench_data_structures.py"
    ]

    print("=" * 80)
    print("COMPREHENSIVE PYTHON VS TAURARO BENCHMARKS")
    print("=" * 80)
    print()

    results = {}

    for bench in benchmarks:
        bench_name = bench.split("/")[1].replace("bench_", "").replace(".py", "")
        print(f"\n{'=' * 80}")
        print(f"Running: {bench_name.upper()}")
        print(f"{'=' * 80}\n")

        print("--- Python ---")
        py_result = run_benchmark(bench, "python")
        print(py_result["output"])
        if py_result["error"]:
            print(f"ERROR: {py_result['error']}")

        print("\n--- Tauraro ---")
        tr_result = run_benchmark(bench, "tauraro")
        print(tr_result["output"])
        if tr_result["error"]:
            print(f"ERROR: {tr_result['error']}")

        py_time = parse_benchmark_time(py_result["output"])
        tr_time = parse_benchmark_time(tr_result["output"])

        if py_time > 0 and tr_time > 0:
            speedup = py_time / tr_time
            results[bench_name] = {
                "python": py_time,
                "tauraro": tr_time,
                "speedup": speedup
            }

    print("\n" + "=" * 80)
    print("SUMMARY")
    print("=" * 80)
    print(f"{'Benchmark':<25} {'Python (s)':<15} {'Tauraro (s)':<15} {'Speedup':<10}")
    print("-" * 80)

    total_py = 0.0
    total_tr = 0.0

    for bench_name, data in results.items():
        print(f"{bench_name:<25} {data['python']:<15.4f} {data['tauraro']:<15.4f} {data['speedup']:<10.2f}x")
        total_py += data['python']
        total_tr += data['tauraro']

    print("-" * 80)
    if total_tr > 0:
        overall_speedup = total_py / total_tr
        print(f"{'TOTAL':<25} {total_py:<15.4f} {total_tr:<15.4f} {overall_speedup:<10.2f}x")
        print("=" * 80)
        print()

        if overall_speedup >= 20:
            print(f"✅ TARGET ACHIEVED: {overall_speedup:.2f}x speedup (target: 20-50x)")
        else:
            print(f"⚠️  TARGET NOT MET: {overall_speedup:.2f}x speedup (target: 20-50x)")
            print(f"   Need {20/overall_speedup:.2f}x more optimization")

if __name__ == "__main__":
    main()
