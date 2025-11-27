# Tauraro vs Python Benchmark Runner
# Run this script from the tauraro root directory
# Usage: .\benchmarks\run_benchmarks.ps1

$ErrorActionPreference = "Stop"

# Colors for output
function Write-Color($text, $color) {
    Write-Host $text -ForegroundColor $color
}

Write-Color "`n========================================================" "Cyan"
Write-Color "   TAURARO vs PYTHON BENCHMARK SUITE (1 Billion Iterations)" "Cyan"
Write-Color "========================================================`n" "Cyan"

# Check if tauraro compiler exists
$tauraro = ".\target\release\tauraro.exe"
if (-not (Test-Path $tauraro)) {
    Write-Color "Building Tauraro compiler..." "Yellow"
    cargo build --release
}

# Benchmark list
$benchmarks = @(
    @{ name = "Simple Loop"; file = "bench_loop" },
    @{ name = "Arithmetic"; file = "bench_arithmetic" },
    @{ name = "Function Calls"; file = "bench_function_calls" },
    @{ name = "Conditionals"; file = "bench_conditionals" },
    @{ name = "Nested Loops"; file = "bench_nested_loops" },
    @{ name = "Bitwise Ops"; file = "bench_bitwise" }
)

# Results storage
$results = @()

foreach ($bench in $benchmarks) {
    $name = $bench.name
    $file = $bench.file
    $trFile = "benchmarks\$file.tr"
    $pyFile = "benchmarks\$file.py"
    $cFile = "benchmarks\$file.c"
    $exeFile = "benchmarks\$file.exe"
    
    Write-Color "`n--- Benchmark: $name ---" "Green"
    
    # Compile Tauraro to C
    Write-Host "  Compiling $file.tr to C..."
    & $tauraro compile $trFile --backend c -o $cFile 2>$null
    
    # Compile C with GCC -O3
    Write-Host "  Compiling C with GCC -O3..."
    gcc -O3 -o $exeFile $cFile -lm 2>$null
    
    # Run Tauraro benchmark
    Write-Host "  Running Tauraro..."
    $tauTime = (Measure-Command { & ".\$exeFile" 2>$null }).TotalSeconds
    Write-Host "    Tauraro: $([math]::Round($tauTime, 2))s" -ForegroundColor Cyan
    
    # Run Python benchmark
    Write-Host "  Running Python..."
    $pyTime = (Measure-Command { python $pyFile 2>$null }).TotalSeconds
    Write-Host "    Python:  $([math]::Round($pyTime, 2))s" -ForegroundColor Yellow
    
    # Calculate speedup
    $speedup = [math]::Round($pyTime / $tauTime, 1)
    Write-Host "    Speedup: ${speedup}x" -ForegroundColor Magenta
    
    $results += @{
        Name = $name
        Tauraro = [math]::Round($tauTime, 2)
        Python = [math]::Round($pyTime, 2)
        Speedup = $speedup
    }
    
    # Cleanup
    Remove-Item $cFile -ErrorAction SilentlyContinue
    Remove-Item $exeFile -ErrorAction SilentlyContinue
}

# Print results table
Write-Color "`n`n========================================================" "Cyan"
Write-Color "                    RESULTS TABLE" "Cyan"
Write-Color "========================================================" "Cyan"
Write-Color "| Benchmark         | Tauraro (s) | Python (s) | Speedup |" "White"
Write-Color "|-------------------|-------------|------------|---------|" "White"

foreach ($r in $results) {
    $nameStr = $r.Name.PadRight(17)
    $tauStr = "$($r.Tauraro)".PadLeft(11)
    $pyStr = "$($r.Python)".PadLeft(10)
    $speedStr = "$($r.Speedup)x".PadLeft(7)
    Write-Host "| $nameStr | $tauStr | $pyStr | $speedStr |"
}

Write-Color "|-------------------|-------------|------------|---------|" "White"

# Calculate averages
$totalTau = 0
$totalPy = 0
foreach ($r in $results) {
    $totalTau += $r.Tauraro
    $totalPy += $r.Python
}
$avgTau = [math]::Round($totalTau / $results.Count, 2)
$avgPy = [math]::Round($totalPy / $results.Count, 2)
$avgSpeedup = [math]::Round($avgPy / $avgTau, 1)

$avgNameStr = "AVERAGE".PadRight(17)
$avgTauStr = "$avgTau".PadLeft(11)
$avgPyStr = "$avgPy".PadLeft(10)
$avgSpeedStr = "${avgSpeedup}x".PadLeft(7)
Write-Color "| $avgNameStr | $avgTauStr | $avgPyStr | $avgSpeedStr |" "Green"
Write-Color "========================================================`n" "Cyan"

Write-Color "Tauraro compiled code is approximately ${avgSpeedup}x faster than Python!" "Green"
Write-Host ""
