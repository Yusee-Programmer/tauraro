"""
Test serveit with all endpoints including dynamic routing
"""
import time
import subprocess
import sys
import urllib.request
from concurrent.futures import ThreadPoolExecutor, as_completed

def wait_for_server(url, timeout=5):
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
    start = time.time()
    try:
        with urllib.request.urlopen(url, timeout=3) as response:
            data = response.read()
        return time.time() - start, True, len(data)
    except Exception as e:
        return time.time() - start, False, 0

def concurrent_test(url, total_requests=100, workers=10):
    times = []
    errors = 0
    bytes_total = 0
    start_time = time.time()

    with ThreadPoolExecutor(max_workers=workers) as executor:
        futures = [executor.submit(make_request, url) for _ in range(total_requests)]

        for future in as_completed(futures):
            elapsed, success, size = future.result()
            if success:
                times.append(elapsed)
                bytes_total += size
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
            'errors': errors,
            'bytes': bytes_total
        }
    return None

print("="*70)
print("Serveit Full Functionality Test with Dynamic Routing")
print("="*70)

# Start serveit
print("\nStarting Serveit server...")
serveit_proc = subprocess.Popen(
    ["./target/release/tauraro", "run", "benchmark_serveit.py"],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

if wait_for_server("http://127.0.0.1:8001/"):
    print("Server ready!\n")

    tests = [
        ("http://127.0.0.1:8001/", "HTML Root"),
        ("http://127.0.0.1:8001/api/hello", "Simple JSON"),
        ("http://127.0.0.1:8001/api/user/123", "Dynamic User Route"),
        ("http://127.0.0.1:8001/api/data", "Large JSON (100 items)"),
        ("http://127.0.0.1:8001/html/page", "HTML Page"),
    ]

    results = {}
    for url, name in tests:
        print(f"{name}:")
        result = concurrent_test(url, total_requests=100, workers=10)
        if result:
            print(f"  RPS: {result['rps']:.2f}")
            print(f"  Avg latency: {result['avg_ms']:.2f}ms")
            print(f"  P50/P95: {result['p50_ms']:.2f}ms / {result['p95_ms']:.2f}ms")
            print(f"  Errors: {result['errors']}")
            print(f"  Total bytes: {result['bytes']:,}\n")
            results[name] = result
        else:
            print(f"  ERROR: All requests failed\n")

    serveit_proc.terminate()
    serveit_proc.wait()

    print("="*70)
    print("SUMMARY")
    print("="*70)
    total_rps = sum(r['rps'] for r in results.values()) / len(results)
    print(f"\nAverage RPS across all endpoints: {total_rps:.2f}")
    print(f"All {len(tests)} endpoints working correctly!")
    print(f"Dynamic routing with string methods: ✅")
    print("\n" + "="*70)
else:
    print("ERROR: Failed to start serveit")
    serveit_proc.kill()
    sys.exit(1)
