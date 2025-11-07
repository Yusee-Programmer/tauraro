"""
Benchmark runner for serveit vs FastAPI
"""
import time
import subprocess
import sys
import signal
from concurrent.futures import ThreadPoolExecutor, as_completed
import urllib.request
import urllib.error

def make_request(url):
    """Make a single HTTP request and return the time taken"""
    start = time.time()
    try:
        with urllib.request.urlopen(url, timeout=5) as response:
            _ = response.read()
            status = response.status
        elapsed = time.time() - start
        return elapsed, status, None
    except Exception as e:
        elapsed = time.time() - start
        return elapsed, 0, str(e)

def benchmark_endpoint(url, num_requests=1000, concurrent=50):
    """Benchmark a single endpoint with concurrent requests"""
    print(f"\n  Testing {url}")
    print(f"  Requests: {num_requests}, Concurrency: {concurrent}")

    times = []
    errors = 0
    status_codes = {}

    start_time = time.time()

    with ThreadPoolExecutor(max_workers=concurrent) as executor:
        futures = [executor.submit(make_request, url) for _ in range(num_requests)]

        for future in as_completed(futures):
            elapsed, status, error = future.result()
            if error:
                errors += 1
            else:
                times.append(elapsed)
                status_codes[status] = status_codes.get(status, 0) + 1

    total_time = time.time() - start_time

    if times:
        times.sort()
        avg_time = sum(times) / len(times)
        min_time = min(times)
        max_time = max(times)
        p50 = times[len(times) // 2]
        p95 = times[int(len(times) * 0.95)]
        p99 = times[int(len(times) * 0.99)]

        rps = len(times) / total_time

        return {
            'total_time': total_time,
            'requests': num_requests,
            'successful': len(times),
            'errors': errors,
            'rps': rps,
            'avg_ms': avg_time * 1000,
            'min_ms': min_time * 1000,
            'max_ms': max_time * 1000,
            'p50_ms': p50 * 1000,
            'p95_ms': p95 * 1000,
            'p99_ms': p99 * 1000,
        }
    else:
        return {'error': 'All requests failed', 'errors': errors}

def wait_for_server(url, timeout=10):
    """Wait for server to be ready"""
    print(f"Waiting for server at {url}...")
    start = time.time()
    while time.time() - start < timeout:
        try:
            with urllib.request.urlopen(url, timeout=1) as response:
                if response.status == 200:
                    print(f"Server ready!")
                    return True
        except:
            time.sleep(0.1)
    return False

def run_benchmark_suite(base_url, name):
    """Run full benchmark suite for a server"""
    print(f"\n{'='*60}")
    print(f"{name} Benchmark Results")
    print(f"{'='*60}")

    endpoints = [
        ('/', 'Root HTML'),
        ('/api/hello', 'Simple JSON'),
        ('/api/user/123', 'Dynamic Route'),
        ('/api/data', 'Large JSON (100 items)'),
        ('/html/page', 'HTML Page'),
    ]

    results = {}
    total_rps = 0

    for path, description in endpoints:
        url = base_url + path
        print(f"\n{description}:")
        result = benchmark_endpoint(url, num_requests=500, concurrent=25)

        if 'error' not in result:
            print(f"  Requests/sec: {result['rps']:.2f}")
            print(f"  Avg latency: {result['avg_ms']:.2f}ms")
            print(f"  P95 latency: {result['p95_ms']:.2f}ms")
            print(f"  P99 latency: {result['p99_ms']:.2f}ms")
            print(f"  Min/Max: {result['min_ms']:.2f}ms / {result['max_ms']:.2f}ms")
            print(f"  Errors: {result['errors']}")

            results[description] = result
            total_rps += result['rps']
        else:
            print(f"  ERROR: {result['error']}")
            results[description] = result

    print(f"\n{'='*60}")
    print(f"Total average RPS across all endpoints: {total_rps/len(endpoints):.2f}")
    print(f"{'='*60}")

    return results

def main():
    print("="*60)
    print("Serveit vs Python HTTP Server Benchmark")
    print("="*60)

    # Start Python HTTP server
    print("\n[1/4] Starting Python HTTP server...")
    python_proc = subprocess.Popen(
        [sys.executable, "benchmark_python_http.py"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    if not wait_for_server("http://127.0.0.1:8000/", timeout=10):
        print("ERROR: Python HTTP server failed to start")
        python_proc.kill()
        return

    # Run Python HTTP benchmarks
    print("\n[2/4] Running Python HTTP benchmarks...")
    python_results = run_benchmark_suite("http://127.0.0.1:8000", "Python HTTP Server")

    # Stop Python HTTP server
    print("\n[3/4] Stopping Python HTTP server...")
    python_proc.terminate()
    python_proc.wait(timeout=5)
    time.sleep(1)

    # Start Serveit server
    print("\n[4/4] Starting Serveit server...")
    serveit_proc = subprocess.Popen(
        ["./target/release/tauraro", "run", "benchmark_serveit.py"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    if not wait_for_server("http://127.0.0.1:8001/", timeout=10):
        print("ERROR: Serveit server failed to start")
        serveit_proc.kill()
        return

    # Run Serveit benchmarks
    print("\n[5/5] Running Serveit benchmarks...")
    serveit_results = run_benchmark_suite("http://127.0.0.1:8001", "Serveit (Tauraro)")

    # Stop Serveit server
    print("\nStopping Serveit server...")
    serveit_proc.terminate()
    serveit_proc.wait(timeout=5)

    # Compare results
    print("\n" + "="*60)
    print("COMPARISON SUMMARY")
    print("="*60)
    print(f"\n{'Endpoint':<25} {'Python RPS':<15} {'Serveit RPS':<15} {'Winner':<15}")
    print("-"*70)

    for endpoint in python_results.keys():
        if endpoint in serveit_results:
            py_result = python_results[endpoint]
            se_result = serveit_results[endpoint]

            if 'rps' in py_result and 'rps' in se_result:
                py_rps = py_result['rps']
                se_rps = se_result['rps']

                if se_rps > py_rps:
                    winner = f"Serveit (+{((se_rps/py_rps-1)*100):.1f}%)"
                else:
                    winner = f"Python (+{((py_rps/se_rps-1)*100):.1f}%)"

                print(f"{endpoint:<25} {py_rps:<15.2f} {se_rps:<15.2f} {winner:<15}")

    print("\n" + "="*60)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nBenchmark interrupted by user")
