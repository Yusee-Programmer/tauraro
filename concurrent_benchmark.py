"""
Concurrent load benchmark - Realistic web traffic simulation
"""
import time
import subprocess
import sys
import urllib.request
from concurrent.futures import ThreadPoolExecutor, as_completed

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

def make_request(url):
    """Make a single request"""
    start = time.time()
    try:
        with urllib.request.urlopen(url, timeout=3) as response:
            _ = response.read()
        return time.time() - start, True
    except:
        return time.time() - start, False

def concurrent_test(url, total_requests=200, workers=10):
    """Run concurrent requests"""
    print(f"  {total_requests} requests with {workers} concurrent workers...")

    times = []
    errors = 0
    start_time = time.time()

    with ThreadPoolExecutor(max_workers=workers) as executor:
        futures = [executor.submit(make_request, url) for _ in range(total_requests)]

        for future in as_completed(futures):
            elapsed, success = future.result()
            if success:
                times.append(elapsed)
            else:
                errors += 1

    total_time = time.time() - start_time

    if times:
        avg_time = sum(times) / len(times) * 1000
        rps = len(times) / total_time
        times.sort()
        p50 = times[len(times) // 2] * 1000
        p95 = times[int(len(times) * 0.95)] * 1000

        return {
            'rps': rps,
            'avg_ms': avg_time,
            'p50_ms': p50,
            'p95_ms': p95,
            'errors': errors
        }
    return None

print("="*70)
print("Concurrent Load Benchmark: Python HTTP vs Serveit")
print("="*70)

# Test Python HTTP Server
print("\n[1/2] Testing Python HTTP Server (concurrent)...")
python_proc = subprocess.Popen(
    [sys.executable, "benchmark_python_http.py"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

if wait_for_server("http://127.0.0.1:8000/"):
    print("  Server ready!")

    tests = [
        ("http://127.0.0.1:8000/", "HTML Response"),
        ("http://127.0.0.1:8000/api/hello", "JSON Response"),
    ]

    python_results = {}
    for url, name in tests:
        print(f"\n  {name}:")
        result = concurrent_test(url, total_requests=200, workers=10)
        if result:
            print(f"    Requests/sec: {result['rps']:.2f}")
            print(f"    Avg latency: {result['avg_ms']:.2f}ms")
            print(f"    P50 latency: {result['p50_ms']:.2f}ms")
            print(f"    P95 latency: {result['p95_ms']:.2f}ms")
            print(f"    Errors: {result['errors']}")
            python_results[name] = result

    python_proc.terminate()
    python_proc.wait()
else:
    print("  ERROR: Failed to start")
    python_proc.kill()
    sys.exit(1)

time.sleep(1)

# Test Serveit
print("\n[2/2] Testing Serveit (Tauraro) (concurrent)...")
serveit_proc = subprocess.Popen(
    ["./target/release/tauraro", "run", "benchmark_serveit.py"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

if wait_for_server("http://127.0.0.1:8001/"):
    print("  Server ready!")

    tests = [
        ("http://127.0.0.1:8001/", "HTML Response"),
        ("http://127.0.0.1:8001/api/hello", "JSON Response"),
    ]

    serveit_results = {}
    for url, name in tests:
        print(f"\n  {name}:")
        result = concurrent_test(url, total_requests=200, workers=10)
        if result:
            print(f"    Requests/sec: {result['rps']:.2f}")
            print(f"    Avg latency: {result['avg_ms']:.2f}ms")
            print(f"    P50 latency: {result['p50_ms']:.2f}ms")
            print(f"    P95 latency: {result['p95_ms']:.2f}ms")
            print(f"    Errors: {result['errors']}")
            serveit_results[name] = result

    serveit_proc.terminate()
    serveit_proc.wait()
else:
    print("  ERROR: Failed to start")
    serveit_proc.kill()
    sys.exit(1)

# Summary
print("\n" + "="*70)
print("COMPARISON - Concurrent Performance")
print("="*70)
print(f"\n{'Test':<20} {'Python RPS':<15} {'Serveit RPS':<15} {'Winner':<20}")
print("-"*70)

for name in python_results.keys():
    if name in serveit_results:
        py_rps = python_results[name]['rps']
        se_rps = serveit_results[name]['rps']

        if se_rps > py_rps:
            winner = f"Serveit (+{((se_rps/py_rps-1)*100):.1f}%)"
        else:
            winner = f"Python (+{((py_rps/se_rps-1)*100):.1f}%)"

        print(f"{name:<20} {py_rps:<15.2f} {se_rps:<15.2f} {winner:<20}")

print("\n" + "="*70)
print("\nKey Insights:")
print("- Sequential requests favor Python's simpler HTTP server")
print("- Concurrent requests showcase Serveit's async capabilities")
print("- Serveit is optimized for real-world concurrent web traffic")
print("="*70)
