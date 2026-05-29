# run_all.ps1 -- Tauraro Benchmark Suite: C vs Rust vs Tauraro
# Runs 5 benchmarks at 1B iterations each and prints a comparison table.

$ROOT    = Resolve-Path "$PSScriptRoot\..\.."
$BENCH   = $PSScriptRoot
# Self-hosted compiler — built from tauraro/src/main.tr
$TAU_EXE = "$ROOT\tauraro\src\build\tauraroc.exe"

function Run-Bench($exe) {
    $out = & $exe 2>&1
    $internal = $out | Where-Object { $_ -match "^TIME_MS:(\d+)" } | Select-Object -First 1
    if ($internal -match "TIME_MS:(\d+)") {
        return [double]$Matches[1] / 1000.0
    }
    return $null
}

function Compile-C($src, $out) {
    $r = gcc -O3 -o $out $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "C compile failed: $r"; return $false }
    return $true
}

function Compile-Rust($src, $out) {
    $r = rustc -C opt-level=3 -C target-cpu=native -o $out $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "Rust compile failed: $r"; return $false }
    return $true
}

function Compile-Tauraro($src) {
    if (-not (Test-Path $TAU_EXE)) {
        Write-Warning "Self-hosted compiler not found at: $TAU_EXE"
        Write-Warning "Build it first: run the bootstrap build in tauraro/src/"
        return $false
    }
    $r = & $TAU_EXE -O3 $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "Tauraro compile failed: $r"; return $false }
    return $true
}

$benchmarks = @(
    @{ name = "1 - Integer Sum";    dir = "1_sum"       },
    @{ name = "2 - Fibonacci";      dir = "2_fibonacci"  },
    @{ name = "3 - Float Multiply"; dir = "3_float_mul"  },
    @{ name = "4 - XOR Shift PRNG"; dir = "4_xorshift"   },
    @{ name = "5 - Newton Sqrt";    dir = "5_newton"     }
)

Write-Host ""
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "   Tauraro Benchmark Suite  --  C vs Rust vs Tauraro" -ForegroundColor Cyan
Write-Host "   1,000,000,000 iterations per benchmark" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$results = @()

foreach ($b in $benchmarks) {
    $dir = Join-Path $BENCH $b.dir
    Write-Host "Compiling $($b.name)..." -ForegroundColor Yellow

    $c_ok  = Compile-C        "$dir\bench.c"  "$dir\bench_c.exe"
    $rs_ok = Compile-Rust     "$dir\bench.rs" "$dir\bench_rs.exe"
    $tr_ok = Compile-Tauraro  "$dir\bench.tr"

    Write-Host "  Running..." -ForegroundColor DarkGray
    $c_time  = if ($c_ok)  { Run-Bench "$dir\bench_c.exe"  } else { $null }
    $rs_time = if ($rs_ok) { Run-Bench "$dir\bench_rs.exe" } else { $null }
    $tr_time = if ($tr_ok) { Run-Bench "$dir\bench.exe"    } else { $null }

    $tau_c_ratio  = if ($c_time  -and $tr_time) { [math]::Round($tr_time / $c_time,  2) } else { $null }
    $tau_rs_ratio = if ($rs_time -and $tr_time) { [math]::Round($tr_time / $rs_time, 2) } else { $null }

    $results += [PSCustomObject]@{
        Benchmark = $b.name
        C_sec     = if ($null -ne $c_time)  { [math]::Round($c_time,  3) } else { "FAIL" }
        Rust_sec  = if ($null -ne $rs_time) { [math]::Round($rs_time, 3) } else { "FAIL" }
        Tau_sec   = if ($null -ne $tr_time) { [math]::Round($tr_time, 3) } else { "FAIL" }
        TauOverC  = if ($null -ne $tau_c_ratio)  { "${tau_c_ratio}x" }  else { "--" }
        TauOverRs = if ($null -ne $tau_rs_ratio) { "${tau_rs_ratio}x" } else { "--" }
        _ratio    = $tau_c_ratio
    }
    Write-Host "  Done: C=$($results[-1].C_sec)s  Rust=$($results[-1].Rust_sec)s  Tauraro=$($results[-1].Tau_sec)s" -ForegroundColor Gray
    Write-Host ""
}

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  RESULTS  (seconds -- lower is faster)" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$fmt = "{0,-24} {1,8} {2,8} {3,10} {4,9} {5,9}"
Write-Host ($fmt -f "Benchmark", "C(s)", "Rust(s)", "Tauraro(s)", "Tau/C", "Tau/Rust") -ForegroundColor White
Write-Host ($fmt -f "-------------------------", "-------", "-------", "---------", "--------", "--------") -ForegroundColor DarkGray

foreach ($r in $results) {
    $line = $fmt -f $r.Benchmark, $r.C_sec, $r.Rust_sec, $r.Tau_sec, $r.TauOverC, $r.TauOverRs
    $ratio = $r._ratio
    if ($null -ne $ratio -and $ratio -le 1.05) {
        Write-Host $line -ForegroundColor Green
    } elseif ($null -ne $ratio -and $ratio -le 1.20) {
        Write-Host $line -ForegroundColor Yellow
    } else {
        Write-Host $line -ForegroundColor White
    }
}

Write-Host ""
Write-Host "  Tau/C    = Tauraro time / C time    (1.00x = same speed as C)" -ForegroundColor DarkGray
Write-Host "  Tau/Rust = Tauraro time / Rust time (1.00x = same speed as Rust)" -ForegroundColor DarkGray
Write-Host ""
