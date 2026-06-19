# run_all.ps1 -- Tauraro Benchmark Suite: C vs Rust vs Tauraro (Windows)
# Self-hosted compiler only. Reports both time (TIME_MS) and peak working-set memory.

$ROOT    = Resolve-Path "$PSScriptRoot\..\.."
$BENCH   = $PSScriptRoot
$TAU_EXE = "$ROOT\tauraro\tauraroc.exe"

function Run-Bench($exe) {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName               = $exe
    $psi.RedirectStandardOutput = $true
    $psi.UseShellExecute        = $false
    $proc = [System.Diagnostics.Process]::Start($psi)

    $peakMem = 0L
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    while (-not $proc.HasExited) {
        try {
            $proc.Refresh()
            if ($proc.PeakWorkingSet64 -gt $peakMem) { $peakMem = $proc.PeakWorkingSet64 }
        } catch {}
        if ($sw.Elapsed.TotalSeconds -gt 60) {
            try { $proc.Kill() } catch {}
            break
        }
        Start-Sleep -Milliseconds 2
    }
    $out = $proc.StandardOutput.ReadToEnd()
    try {
        $proc.Refresh()
        if ($proc.PeakWorkingSet64 -gt $peakMem) { $peakMem = $proc.PeakWorkingSet64 }
    } catch {}

    $time_s = $null
    $line = $out -split "`n" | Where-Object { $_ -match "TIME_MS:(\d+)" } | Select-Object -First 1
    if ($line -match "TIME_MS:(\d+)") {
        $time_s = [double]$Matches[1] / 1000.0
    }
    return @{ Time = $time_s; PeakMemKB = [math]::Round($peakMem / 1024.0, 1) }
}

function Compile-C($src, $out) {
    # Always pass -lm; harmless on Windows, required on Linux for math functions
    $r = gcc -O3 -lm -o $out $src 2>&1
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
        Write-Warning "Build it first: run the bootstrap in tauraro/src/"
        return $false
    }
    $r = & $TAU_EXE -O3 $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "Tauraro compile failed: $r"; return $false }
    return $true
}

$benchmarks = @(
    @{ name = "1 - Integer Sum";      dir = "1_sum"         },
    @{ name = "2 - Fibonacci";        dir = "2_fibonacci"   },
    @{ name = "3 - Float Multiply";   dir = "3_float_mul"   },
    @{ name = "4 - XOR Shift PRNG";   dir = "4_xorshift"    },
    @{ name = "5 - Newton Sqrt";      dir = "5_newton"      },
    @{ name = "6 - Mandelbrot";       dir = "6_mandelbrot"  },
    @{ name = "7 - Sieve 50M";        dir = "7_sieve"       },
    @{ name = "8 - N-Body 3b";        dir = "8_nbody"       },
    @{ name = "9 - Collatz 10M";      dir = "9_collatz"     },
    @{ name = "10 - MatMul 400x400";  dir = "10_matmul"     }
)

Write-Host ""
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "   Tauraro Benchmark Suite  --  C vs Rust vs Tauraro" -ForegroundColor Cyan
Write-Host "   Compiler: $TAU_EXE" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$results = @()

# Self-hosted tauraroc writes intermediates + the exe to a CWD-relative
# `build/` dir. Anchor CWD to $BENCH so that dir is always $BENCH\build and
# matches the path we measure below -- otherwise (e.g. invoked from the repo
# root) we'd compile into <cwd>\build but MEASURE a stale $BENCH\build\bench.exe
# left over from a previous run, reporting bogus (often huge) numbers.
Push-Location $BENCH
try {

foreach ($b in $benchmarks) {
    $dir = Join-Path $BENCH $b.dir
    Write-Host "Compiling $($b.name)..." -ForegroundColor Yellow

    $c_ok  = Compile-C        "$dir\bench.c"  "$dir\bench_c.exe"
    $rs_ok = Compile-Rust     "$dir\bench.rs" "$dir\bench_rs.exe"
    $tr_ok = Compile-Tauraro  "$dir\bench.tr"

    Write-Host "  Running..." -ForegroundColor DarkGray
    $c_res  = if ($c_ok)  { Run-Bench "$dir\bench_c.exe"  } else { @{ Time = $null; PeakMemKB = $null } }
    $rs_res = if ($rs_ok) { Run-Bench "$dir\bench_rs.exe" } else { @{ Time = $null; PeakMemKB = $null } }
    # Self-hosted tauraroc places the exe in <benchmarks>/build/bench.exe (shared, overwritten each iteration)
    $tr_exe = "$BENCH\build\bench.exe"
    $tr_res = if ($tr_ok -and (Test-Path $tr_exe)) { Run-Bench $tr_exe } else { @{ Time = $null; PeakMemKB = $null } }

    $c_time  = $c_res.Time
    $rs_time = $rs_res.Time
    $tr_time = $tr_res.Time

    $tau_c_ratio  = if ($null -ne $c_time  -and $null -ne $tr_time -and $c_time  -gt 0) { [math]::Round($tr_time / $c_time,  2) } else { $null }
    $tau_rs_ratio = if ($null -ne $rs_time -and $null -ne $tr_time -and $rs_time -gt 0) { [math]::Round($tr_time / $rs_time, 2) } else { $null }

    $results += [PSCustomObject]@{
        Benchmark = $b.name
        C_sec     = if ($null -ne $c_time)  { [math]::Round($c_time,  3) } else { "FAIL" }
        Rust_sec  = if ($null -ne $rs_time) { [math]::Round($rs_time, 3) } else { "FAIL" }
        Tau_sec   = if ($null -ne $tr_time) { [math]::Round($tr_time, 3) } else { "FAIL" }
        TauOverC  = if ($null -ne $tau_c_ratio)  { "${tau_c_ratio}x" }  else { "--" }
        TauOverRs = if ($null -ne $tau_rs_ratio) { "${tau_rs_ratio}x" } else { "--" }
        C_memKB   = if ($null -ne $c_res.PeakMemKB)  { $c_res.PeakMemKB }  else { "FAIL" }
        Rs_memKB  = if ($null -ne $rs_res.PeakMemKB) { $rs_res.PeakMemKB } else { "FAIL" }
        Tau_memKB = if ($null -ne $tr_res.PeakMemKB) { $tr_res.PeakMemKB } else { "FAIL" }
        _ratio    = $tau_c_ratio
    }
    Write-Host "  Done: C=$($results[-1].C_sec)s ($($results[-1].C_memKB) KB)  Rust=$($results[-1].Rust_sec)s ($($results[-1].Rs_memKB) KB)  Tauraro=$($results[-1].Tau_sec)s ($($results[-1].Tau_memKB) KB)" -ForegroundColor Gray
    Write-Host ""
}

} finally { Pop-Location }

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  RESULTS  (seconds -- lower is faster)" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$fmt = "{0,-24} {1,8} {2,8} {3,10} {4,9} {5,9}"
Write-Host ($fmt -f "Benchmark", "C(s)", "Rust(s)", "Tauraro(s)", "Tau/C", "Tau/Rust") -ForegroundColor White
Write-Host ($fmt -f "------------------------", "-------", "-------", "---------", "--------", "--------") -ForegroundColor DarkGray

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
Write-Host "  Tau/C    = Tauraro / C time    (< 1.00x = Tauraro faster than C)" -ForegroundColor DarkGray
Write-Host "  Tau/Rust = Tauraro / Rust time (< 1.00x = Tauraro faster than Rust)" -ForegroundColor DarkGray
Write-Host ""

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  PEAK MEMORY  (KB -- lower is more efficient)" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$mfmt = "{0,-24} {1,10} {2,10} {3,10} {4,9} {5,9}"
Write-Host ($mfmt -f "Benchmark", "C(KB)", "Rust(KB)", "Tau(KB)", "Tau/C", "Tau/Rust") -ForegroundColor White
Write-Host ($mfmt -f "------------------------", "---------", "---------", "---------", "--------", "--------") -ForegroundColor DarkGray

foreach ($r in $results) {
    $tau_c_mem  = if ($r.C_memKB  -is [double] -and $r.Tau_memKB -is [double] -and $r.C_memKB  -gt 0) { "$([math]::Round($r.Tau_memKB / $r.C_memKB, 2))x"  } else { "--" }
    $tau_rs_mem = if ($r.Rs_memKB -is [double] -and $r.Tau_memKB -is [double] -and $r.Rs_memKB -gt 0) { "$([math]::Round($r.Tau_memKB / $r.Rs_memKB, 2))x" } else { "--" }
    $line = $mfmt -f $r.Benchmark, $r.C_memKB, $r.Rs_memKB, $r.Tau_memKB, $tau_c_mem, $tau_rs_mem
    Write-Host $line -ForegroundColor White
}

Write-Host ""
Write-Host "  Tau/C    = Tauraro / C peak memory    (< 1.00x = Tauraro uses less memory than C)" -ForegroundColor DarkGray
Write-Host "  Tau/Rust = Tauraro / Rust peak memory (< 1.00x = Tauraro uses less memory than Rust)" -ForegroundColor DarkGray
Write-Host ""

# ── Markdown report -> benchmarks/results.md ────────────────────────────────────

$RESULTS_MD = Join-Path $BENCH "results.md"
$gccVer  = (& gcc --version 2>$null | Select-Object -First 1)
$rustVer = (& rustc --version 2>$null | Select-Object -First 1)

$md = New-Object System.Collections.Generic.List[string]
$md.Add("# Tauraro Benchmark Results")
$md.Add("")
$md.Add("Auto-generated by ``benchmarks/run_all.ps1``. Lower is better in every column.")
$md.Add("")
$md.Add("- **OS:** Windows $([System.Environment]::OSVersion.Version)")
$md.Add("- **Date (UTC):** $((Get-Date).ToUniversalTime().ToString('yyyy-MM-dd HH:mm:ss'))")
$md.Add("- **Compiler:** ``$TAU_EXE``")
if ($gccVer)  { $md.Add("- **C:** $gccVer") }
if ($rustVer) { $md.Add("- **Rust:** $rustVer") }
$md.Add("")
$md.Add("## Wall time (seconds)")
$md.Add("")
$md.Add("| Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau/C | Tau/Rust |")
$md.Add("|-----------|------:|---------:|------------:|------:|---------:|")
foreach ($r in $results) {
    $md.Add("| $($r.Benchmark) | $($r.C_sec) | $($r.Rust_sec) | $($r.Tau_sec) | $($r.TauOverC) | $($r.TauOverRs) |")
}
$md.Add("")
$md.Add("## Peak resident memory (KB)")
$md.Add("")
$md.Add("| Benchmark | C (KB) | Rust (KB) | Tauraro (KB) | Tau/C | Tau/Rust |")
$md.Add("|-----------|-------:|----------:|-------------:|------:|---------:|")
foreach ($r in $results) {
    $tau_c_mem  = if ($r.C_memKB  -is [double] -and $r.Tau_memKB -is [double] -and $r.C_memKB  -gt 0) { "$([math]::Round($r.Tau_memKB / $r.C_memKB, 2))x"  } else { "--" }
    $tau_rs_mem = if ($r.Rs_memKB -is [double] -and $r.Tau_memKB -is [double] -and $r.Rs_memKB -gt 0) { "$([math]::Round($r.Tau_memKB / $r.Rs_memKB, 2))x" } else { "--" }
    $md.Add("| $($r.Benchmark) | $($r.C_memKB) | $($r.Rs_memKB) | $($r.Tau_memKB) | $tau_c_mem | $tau_rs_mem |")
}
$md.Add("")
$md.Add("_``Tau/C`` and ``Tau/Rust`` are ratios: < 1.00x means Tauraro is faster / leaner._")

($md -join "`n") | Out-File -FilePath $RESULTS_MD -Encoding utf8
Write-Host "Wrote Markdown report: $RESULTS_MD" -ForegroundColor Green
Write-Host ""

# Reporting tool: completed successfully even if some toolchains were absent.
# Exit 0 explicitly so a non-zero $LASTEXITCODE left by the last gcc/rustc
# invocation isn't mistaken for a script failure.
exit 0
