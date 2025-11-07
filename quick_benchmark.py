"""
Quick benchmark comparison - Sequential requests
"""
import time
import subprocess
import sys
import urllib.request

def wait_for_server(url, timeout=5):
    """Wait for server to be ready"""
    start = time.time()
    while time.time() - start < timeout:
        try:
            with urllib.request.urlopen(url, timeout=1) as response:
                if response.status == 200:
                    return True
        except:
            time.sleep(0.1)
    return False

def measure_requests(url, count=100):
    """Measure sequential request performance"""
    times = []
    errors = 0

    print(f"  Making {count} sequential requests to {url}...")

    start_time = time.time()
    for i in range(count):
        req_start = time.time()
        try:
            with urllib.request.urlopen(url, timeout=2) as response:
                _ = response.read()
            times.append(time.time() - req_start)
        except Exception as e:
            errors += 1

    total_time = time.time() - start_time

    if times:
        avg_time = sum(times) / len(times) * 1000  # Convert to ms
        rps = len(times) / total_time
        return {
            'rps': rps,
            'avg_ms': avg_time,
            'errors': errors,
            'total_time': total_time
        }
    return None

print("="*60)
print("Quick Benchmark: Python HTTP vs Serveit")
print("="*60)

# Test Python HTTP Server
print("\n[1/2] Testing Python HTTP Server...")
python_proc = subprocess.Popen(
    [sys.executable, "benchmark_python_http.py"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

if wait_for_server("http://127.0.0.1:8000/"):
    print("  Server ready!")

    endpoints = [
        ("http://127.0.0.1:8000/", "Root HTML"),
        ("http://127.0.0.1:8000/api/hello", "JSON API"),
    ]

    python_results = {}
    for url, name in endpoints:
        print(f"\n  {name}:")
        result = measure_requests(url, count=200)
        if result:
            print(f"    RPS: {result['rps']:.2f}")
            print(f"    Avg latency: {result['avg_ms']:.2f}ms")
            print(f"    Errors: {result['errors']}")
            python_results[name] = result

    python_proc.terminate()
    python_proc.wait()
else:
    print("  ERROR: Server failed to start")
    python_proc.kill()
    sys.exit(1)

time.sleep(1)

# Test Serveit
print("\n[2/2] Testing Serveit (Tauraro)...")
serveit_proc = subprocess.Popen(
    ["./target/release/tauraro", "run", "benchmark_serveit.py"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

if wait_for_server("http://127.0.0.1:8001/"):
    print("  Server ready!")

    endpoints = [
        ("http://127.0.0.1:8001/", "Root HTML"),
        ("http://127.0.0.1:8001/api/hello", "JSON API"),
    ]

    serveit_results = {}
    for url, name in endpoints:
        print(f"\n  {name}:")
        result = measure_requests(url, count=200)
        if result:
            print(f"    RPS: {result['rps']:.2f}")
            print(f"    Avg latency: {result['avg_ms']:.2f}ms")
            print(f"    Errors: {result['errors']}")
            serveit_results[name] = result

    serveit_proc.terminate()
    serveit_proc.wait()
else:
    print("  ERROR: Server failed to start")
    serveit_proc.kill()
    sys.exit(1)

# Summary
print("\n" + "="*60)
print("RESULTS SUMMARY")
print("="*60)
print(f"\n{'Test':<20} {'Python RPS':<15} {'Serveit RPS':<15} {'Improvement':<15}")
print("-"*65)

for name in python_results.keys():
    if name in serveit_results:
        py_rps = python_results[name]['rps']
        se_rps = serveit_results[name]['rps']
        improvement = ((se_rps / py_rps - 1) * 100) if py_rps > 0 else 0

        print(f"{name:<20} {py_rps:<15.2f} {se_rps:<15.2f} {improvement:+.1f}%")

print("\n" + "="*60)
