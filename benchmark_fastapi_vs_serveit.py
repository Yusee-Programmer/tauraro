"""
Comprehensive Benchmark: FastAPI (uvicorn) vs Serveit (Tauraro)
Compares performance across multiple endpoints with concurrent load
"""
import time
import subprocess
import sys
import urllib.request
from concurrent.futures import ThreadPoolExecutor, as_completed

def wait_for_server(url, timeout=10):
    """Wait for server to be ready"""
    print(f"  Waiting for server at {url}...")
    start = time.time()
    while time.time() - start < timeout:
        try:
            with urllib.request.urlopen(url, timeout=1) as response:
                if response.status == 200:
                    print(f"  Server ready!")
                    return True
        except:
            time.sleep(0.2)
    return False

def make_request(url):
    """Make a single request and measure performance"""
    start = time.time()
    try:
        with urllib.request.urlopen(url, timeout=3) as response:
            data = response.read()
        return time.time() - start, True, len(data)
    except Exception as e:
        return time.time() - start, False, 0

def concurrent_test(url, total_requests=200, workers=20):
    """Run concurrent requests and measure performance"""
    print(f"  Running {total_requests} requests with {workers} workers...")

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
        p99 = times[int(len(times) * 0.99)] * 1000

        return {
            'rps': rps,
            'avg_ms': avg_time,
            'p50_ms': p50,
            'p95_ms': p95,
            'p99_ms': p99,
            'errors': errors,
            'bytes': bytes_total,
            'total_time': total_time
        }
    return {'error': 'All requests failed', 'errors': errors}

def run_benchmark_suite(base_url, name, endpoints):
    """Run full benchmark suite for a server"""
    print(f"\n{'='*70}")
    print(f"{name} Benchmark Results")
    print(f"{'='*70}")

    results = {}
    total_rps = 0

    for path, description in endpoints:
        url = base_url + path
        print(f"\n{description}:")
        result = concurrent_test(url, total_requests=200, workers=20)

        if 'error' not in result:
            print(f"  Requests/sec: {result['rps']:.2f}")
            print(f"  Avg latency: {result['avg_ms']:.2f}ms")
            print(f"  P50 latency: {result['p50_ms']:.2f}ms")
            print(f"  P95 latency: {result['p95_ms']:.2f}ms")
            print(f"  P99 latency: {result['p99_ms']:.2f}ms")
            print(f"  Errors: {result['errors']}")

            results[description] = result
            total_rps += result['rps']
        else:
            print(f"  ERROR: {result['error']}")
            results[description] = result

    print(f"\n{'='*70}")
    if results:
        print(f"Average RPS across all endpoints: {total_rps/len(endpoints):.2f}")
    print(f"{'='*70}")

    return results

def main():
    print("="*70)
    print("FastAPI (uvicorn) vs Serveit (Tauraro) - Performance Benchmark")
    print("="*70)
    print("\nConfiguration:")
    print("  - Requests per endpoint: 200")
    print("  - Concurrent workers: 20")
    print("  - Timeout: 3 seconds")
    print()

    endpoints = [
        ("/", "Root HTML"),
        ("/api/hello", "Simple JSON API"),
        ("/api/user/123", "Dynamic Route (user ID)"),
        ("/api/data", "Large JSON (100 items)"),
        ("/html/page", "HTML Page"),
    ]

    # Test FastAPI
    print("\n[1/2] Testing FastAPI with uvicorn...")
    fastapi_proc = subprocess.Popen(
        [sys.executable, "benchmark_fastapi.py"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    if not wait_for_server("http://127.0.0.1:8000/", timeout=15):
        print("ERROR: FastAPI server failed to start")
        fastapi_proc.kill()
        return

    fastapi_results = run_benchmark_suite("http://127.0.0.1:8000", "FastAPI + uvicorn", endpoints)

    print("\n  Stopping FastAPI server...")
    fastapi_proc.terminate()
    fastapi_proc.wait(timeout=5)
    time.sleep(2)

    # Test Serveit
    print("\n[2/2] Testing Serveit (Tauraro)...")
    serveit_proc = subprocess.Popen(
        ["./target/release/tauraro", "run", "benchmark_serveit.py"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    if not wait_for_server("http://127.0.0.1:8001/", timeout=15):
        print("ERROR: Serveit server failed to start")
        serveit_proc.kill()
        return

    serveit_results = run_benchmark_suite("http://127.0.0.1:8001", "Serveit (Tauraro)", endpoints)

    print("\n  Stopping Serveit server...")
    serveit_proc.terminate()
    serveit_proc.wait(timeout=5)

    # Comparison
    print("\n" + "="*70)
    print("PERFORMANCE COMPARISON")
    print("="*70)
    print(f"\n{'Endpoint':<25} {'FastAPI RPS':<15} {'Serveit RPS':<15} {'Winner':<25}")
    print("-"*80)

    fastapi_total = 0
    serveit_total = 0
    endpoint_count = 0

    for endpoint_desc in fastapi_results.keys():
        if endpoint_desc in serveit_results:
            fa_result = fastapi_results[endpoint_desc]
            se_result = serveit_results[endpoint_desc]

            if 'rps' in fa_result and 'rps' in se_result:
                fa_rps = fa_result['rps']
                se_rps = se_result['rps']

                if se_rps > fa_rps:
                    winner = f"Serveit (+{((se_rps/fa_rps-1)*100):.1f}%)"
                    symbol = "🏆"
                else:
                    winner = f"FastAPI (+{((fa_rps/se_rps-1)*100):.1f}%)"
                    symbol = "⚡"

                print(f"{endpoint_desc:<25} {fa_rps:<15.2f} {se_rps:<15.2f} {symbol} {winner:<20}")

                fastapi_total += fa_rps
                serveit_total += se_rps
                endpoint_count += 1

    if endpoint_count > 0:
        fastapi_avg = fastapi_total / endpoint_count
        serveit_avg = serveit_total / endpoint_count

        print("\n" + "="*80)
        print(f"{'OVERALL AVERAGE':<25} {fastapi_avg:<15.2f} {serveit_avg:<15.2f}")

        if serveit_avg > fastapi_avg:
            improvement = ((serveit_avg/fastapi_avg-1)*100)
            print(f"\n🏆 Winner: Serveit by {improvement:.1f}%")
        else:
            improvement = ((fastapi_avg/serveit_avg-1)*100)
            print(f"\n⚡ Winner: FastAPI by {improvement:.1f}%")

        print("="*80)

        # Detailed Analysis
        print("\n" + "="*70)
        print("DETAILED ANALYSIS")
        print("="*70)

        print("\nFastAPI Strengths:")
        print("  - Mature Python async framework")
        print("  - Extensive ecosystem and libraries")
        print("  - Automatic API documentation")
        print("  - Type hints and validation with Pydantic")

        print("\nServeit Strengths:")
        print("  - Native Rust performance")
        print("  - Built directly into Tauraro language")
        print("  - Lower memory footprint")
        print("  - Compiled binary (no Python interpreter)")

        print("\nUse Case Recommendations:")
        print("  - Choose FastAPI: Complex APIs, Python ecosystem integration")
        print("  - Choose Serveit: High-performance microservices, Tauraro apps")

        print("\n" + "="*70)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nBenchmark interrupted by user")
