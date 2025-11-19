# Benchmark runner for Tauraro vs Python

$tauraro_exe = "c:\Users\Yusee Habibu\Downloads\tauraro\target\release\tauraro.exe"
$benchmarks = @(
    @{ name = "Fibonacci"; py = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks\benchmark.py"; tr = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks\benchmark_fib.tr" },
    @{ name = "Primes"; py = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks\benchmark.py"; tr = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks\benchmark_primes.tr" },
    @{ name = "Matrix"; py = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks\benchmark.py"; tr = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks\benchmark_matrix.tr" }
)

$results = @{}

foreach ($benchmark in $benchmarks) {
    Write-Host "`n=== Running $($benchmark.name) Benchmark ===" -ForegroundColor Green
    
    # Run Python benchmark
    Write-Host "Running Python..." -ForegroundColor Yellow
    $py_start = [datetime]::Now
    & python $($benchmark.py)
    $py_time = ([datetime]::Now - $py_start).TotalSeconds
    Write-Host "Python Time: ${py_time}s" -ForegroundColor Cyan
    
    # Run Tauraro benchmark
    Write-Host "Running Tauraro..." -ForegroundColor Yellow
    $tr_start = [datetime]::Now
    & $tauraro_exe $($benchmark.tr)
    $tr_time = ([datetime]::Now - $tr_start).TotalSeconds
    Write-Host "Tauraro Time: ${tr_time}s" -ForegroundColor Cyan
    
    $speedup = $py_time / $tr_time
    Write-Host "$($benchmark.name) Speedup: ${speedup}x" -ForegroundColor White
    
    $results[$benchmark.name] = @{
        python = $py_time
        tauraro = $tr_time
        speedup = $speedup
    }
}

Write-Host "`n=== BENCHMARK SUMMARY ===" -ForegroundColor Green
foreach ($key in $results.Keys) {
    $r = $results[$key]
    Write-Host "$key`: Python: $($r.python)s | Tauraro: $($r.tauraro)s | Speedup: $($r.speedup)x"
}
