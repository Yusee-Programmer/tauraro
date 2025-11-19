# Comprehensive Benchmark Runner - Tauraro vs Python

$python_exe = "python"
$tauraro_exe = "c:\Users\Yusee Habibu\Downloads\tauraro\target\release\tauraro.exe"
$benchmark_dir = "c:\Users\Yusee Habibu\Downloads\tauraro\benchmarks"
$python_script = "$benchmark_dir\comprehensive_benchmark.py"
$tauraro_script = "$benchmark_dir\comprehensive_benchmark.tr"

Write-Host "╔════════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║     COMPREHENSIVE BENCHMARK: TAURARO vs PYTHON                   ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan

# Run Python benchmarks
Write-Host "`n[1/2] Running Python benchmarks..." -ForegroundColor Yellow
$python_start = Get-Date
$python_output = & $python_exe $python_script 2>&1
$python_total_time = ((Get-Date) - $python_start).TotalSeconds

# Parse Python output
Write-Host "`nPython Results:" -ForegroundColor Green
Write-Host $python_output | Select-Object -Last 20

# Run Tauraro benchmarks
Write-Host "`n[2/2] Running Tauraro benchmarks..." -ForegroundColor Yellow
$tauraro_start = Get-Date
$tauraro_output = & $tauraro_exe run $tauraro_script 2>&1
$tauraro_total_time = ((Get-Date) - $tauraro_start).TotalSeconds

# Parse Tauraro output
Write-Host "`nTauraro Results:" -ForegroundColor Green
Write-Host $tauraro_output | Select-Object -Last 20

# Summary
Write-Host "`n╔════════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║                      BENCHMARK SUMMARY                             ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan

Write-Host "`nTotal Execution Times:" -ForegroundColor Yellow
Write-Host "  Python:  $($python_total_time.ToString('F2'))s" -ForegroundColor Green
Write-Host "  Tauraro: $($tauraro_total_time.ToString('F2'))s" -ForegroundColor Cyan

$speedup = $python_total_time / $tauraro_total_time
if ($speedup -gt 1) {
    Write-Host "  Speedup: ${speedup}x faster in Python (Tauraro is $(1/$speedup)x slower)" -ForegroundColor Red
} else {
    Write-Host "  Speedup: $($speedup)x (Tauraro is $($speedup)x faster!)" -ForegroundColor Green
}

Write-Host "`nDetailed Breakdown:" -ForegroundColor Yellow

# Extract individual benchmark times
$python_lines = $python_output -split "`n" | where { $_ -match ": \d+\.\d+s" }
$tauraro_lines = $tauraro_output -split "`n" | where { $_ -match ": \d+\.\d+s" }

Write-Host "`n{'Benchmark',-40} {'Python',15} {'Tauraro',15} {'Speedup',10}" -ForegroundColor Cyan
Write-Host "─" * 85 -ForegroundColor Cyan

for ($i = 0; $i -lt [Math]::Min($python_lines.Count, $tauraro_lines.Count); $i++) {
    $p_match = $python_lines[$i] | Select-String -Pattern "(.+?):\s+(\d+\.\d+)s"
    $t_match = $tauraro_lines[$i] | Select-String -Pattern "(.+?):\s+(\d+\.\d+)s"
    
    if ($p_match -and $t_match) {
        $name = $p_match.Matches[0].Groups[1].Value.Trim()
        $p_time = [double]$p_match.Matches[0].Groups[2].Value
        $t_time = [double]$t_match.Matches[0].Groups[2].Value
        $bench_speedup = $p_time / $t_time
        
        $speedup_color = if ($bench_speedup -lt 1) { "Red" } else { "Green" }
        $speedup_text = if ($bench_speedup -lt 1) { "$(1/$bench_speedup)x slower" } else { "${bench_speedup}x faster" }
        
        Write-Host ('{0,-40} {1,15:F4}s {2,15:F4}s {3,10}' -f $name, $p_time, $t_time, $speedup_text) -ForegroundColor $speedup_color
    }
}

Write-Host ""
