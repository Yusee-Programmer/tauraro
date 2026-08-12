# run_all_llvm.ps1 -- Tauraro Benchmark Suite, LLVM BACKEND (Windows)
# Compiles + runs every benchmark with `tauraroc --backend llvm` and compares against
# gcc -O3 C, rustc -O3, and the DEFAULT Tauraro C backend, so the LLVM speedup is visible.
# Reports time (TIME_MS) and peak working-set memory. Writes benchmarks/results_llvm.md.
#
# The LLVM driver produces a running executable in one shot (it builds runtime.o + invokes
# clang/llc and links the right platform libs itself). Requires clang or llc (>= 15) on PATH.

$ROOT    = Resolve-Path "$PSScriptRoot\..\.."
$BENCH   = $PSScriptRoot
$TAU_EXE = "$ROOT\tauraro\tauraroc.exe"

$haveClang = [bool](Get-Command clang -ErrorAction SilentlyContinue)
$haveLlc   = [bool](Get-Command llc   -ErrorAction SilentlyContinue)
if (-not $haveClang -and -not $haveLlc) {
    Write-Error "Neither clang nor llc found -- the LLVM backend cannot link. Install LLVM >= 15."
    exit 1
}

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
        if ($sw.Elapsed.TotalSeconds -gt 60) { try { $proc.Kill() } catch {}; break }
        Start-Sleep -Milliseconds 2
    }
    $out = $proc.StandardOutput.ReadToEnd()
    try {
        $proc.Refresh()
        if ($proc.PeakWorkingSet64 -gt $peakMem) { $peakMem = $proc.PeakWorkingSet64 }
    } catch {}

    $time_s = $null
    $line = $out -split "`n" | Where-Object { $_ -match "TIME_MS:(\d+)" } | Select-Object -First 1
    if ($line -match "TIME_MS:(\d+)") { $time_s = [double]$Matches[1] / 1000.0 }
    return @{ Time = $time_s; PeakMemKB = [math]::Round($peakMem / 1024.0, 1) }
}

function Compile-C($src, $out) {
    $r = gcc -O3 -lm -o $out $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "C compile failed: $r"; return $false }
    return $true
}
function Compile-Rust($src, $out) {
    $r = rustc -C opt-level=3 -C target-cpu=native -o $out $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "Rust compile failed: $r"; return $false }
    return $true
}
function Compile-TauraroC($src) {
    if (-not (Test-Path $TAU_EXE)) { Write-Warning "Compiler not found: $TAU_EXE"; return $false }
    $r = & $TAU_EXE -O3 $src 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "Tauraro-C compile failed: $r"; return $false }
    return $true
}
# LLVM backend -- produces the exe directly at $out (the driver links platform libs itself).
function Compile-TauraroLlvm($src, $out) {
    if (-not (Test-Path $TAU_EXE)) { Write-Warning "Compiler not found: $TAU_EXE"; return $false }
    $r = & $TAU_EXE --backend llvm -O3 $src -o $out 2>&1
    if ($LASTEXITCODE -ne 0) { Write-Warning "Tauraro-LLVM compile failed: $r"; return $false }
    return $true
}
# The C and LLVM backends BOTH write to a CWD-relative build\ dir; on Windows a just-run
# bench.exe (or the LLVM driver's log) can hold a handle, so the next compile fails with
# "Permission denied" / "resource busy". Wipe build\ (retrying past the transient lock)
# before every Tauraro compile so each starts from a clean, unlocked directory.
function Clean-Build {
    $bd = Join-Path $BENCH "build"; $n = 0
    while ((Test-Path $bd) -and ($n -lt 50)) {
        Remove-Item $bd -Recurse -Force -ErrorAction SilentlyContinue
        if (Test-Path $bd) { Start-Sleep -Milliseconds 100 }
        $n++
    }
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
Write-Host "   Tauraro Benchmark Suite  --  LLVM backend vs C / Rust / Tau-C" -ForegroundColor Cyan
Write-Host "   Compiler: $TAU_EXE" -ForegroundColor Cyan
if ($haveClang) { Write-Host "   LLVM: $((& clang --version | Select-Object -First 1))" -ForegroundColor Cyan }
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$results = @()

# Anchor CWD to $BENCH so the C-backend's CWD-relative build\ dir is $BENCH\build.
Push-Location $BENCH
try {

foreach ($b in $benchmarks) {
    $dir = Join-Path $BENCH $b.dir
    Write-Host "Compiling $($b.name)..." -ForegroundColor Yellow

    $c_ok  = Compile-C       "$dir\bench.c"  "$dir\bench_c.exe"
    $rs_ok = Compile-Rust    "$dir\bench.rs" "$dir\bench_rs.exe"
    Clean-Build
    $tc_ok = Compile-TauraroC "$dir\bench.tr"
    # Measure the C-backend exe BEFORE the LLVM compile reuses build\.
    $tc_exe = "$BENCH\build\bench.exe"
    $tc_res = if ($tc_ok -and (Test-Path $tc_exe)) { Run-Bench $tc_exe } else { @{ Time = $null; PeakMemKB = $null } }

    # LLVM backend -> its own exe path (avoids clobbering the C-backend build\).
    Clean-Build
    $ll_out = "$dir\bench_llvm.exe"
    $ll_ok  = Compile-TauraroLlvm "$dir\bench.tr" $ll_out
    $ll_exe = if (Test-Path $ll_out) { $ll_out } elseif (Test-Path "$dir\bench_llvm") { "$dir\bench_llvm" } else { $null }

    Write-Host "  Running..." -ForegroundColor DarkGray
    $c_res  = if ($c_ok)  { Run-Bench "$dir\bench_c.exe"  } else { @{ Time = $null; PeakMemKB = $null } }
    $rs_res = if ($rs_ok) { Run-Bench "$dir\bench_rs.exe" } else { @{ Time = $null; PeakMemKB = $null } }
    $ll_res = if ($ll_ok -and $ll_exe) { Run-Bench $ll_exe } else { @{ Time = $null; PeakMemKB = $null } }

    $c_time = $c_res.Time; $rs_time = $rs_res.Time; $tc_time = $tc_res.Time; $ll_time = $ll_res.Time

    # Ratios are all relative to the LLVM backend (the subject of this script).
    $ll_c  = if ($null -ne $c_time  -and $null -ne $ll_time -and $c_time  -gt 0) { [math]::Round($ll_time / $c_time,  2) } else { $null }
    $ll_rs = if ($null -ne $rs_time -and $null -ne $ll_time -and $rs_time -gt 0) { [math]::Round($ll_time / $rs_time, 2) } else { $null }
    $ll_tc = if ($null -ne $tc_time -and $null -ne $ll_time -and $tc_time -gt 0) { [math]::Round($ll_time / $tc_time, 2) } else { $null }

    $results += [PSCustomObject]@{
        Benchmark = $b.name
        C_sec     = if ($null -ne $c_time)  { [math]::Round($c_time,  3) } else { "FAIL" }
        Rust_sec  = if ($null -ne $rs_time) { [math]::Round($rs_time, 3) } else { "FAIL" }
        TauC_sec  = if ($null -ne $tc_time) { [math]::Round($tc_time, 3) } else { "FAIL" }
        Llvm_sec  = if ($null -ne $ll_time) { [math]::Round($ll_time, 3) } else { "FAIL" }
        LLoverC   = if ($null -ne $ll_c)  { "${ll_c}x" }  else { "--" }
        LLoverRs  = if ($null -ne $ll_rs) { "${ll_rs}x" } else { "--" }
        LLoverTC  = if ($null -ne $ll_tc) { "${ll_tc}x" } else { "--" }
        C_memKB   = if ($null -ne $c_res.PeakMemKB)  { $c_res.PeakMemKB }  else { "FAIL" }
        Rs_memKB  = if ($null -ne $rs_res.PeakMemKB) { $rs_res.PeakMemKB } else { "FAIL" }
        TC_memKB  = if ($null -ne $tc_res.PeakMemKB) { $tc_res.PeakMemKB } else { "FAIL" }
        LL_memKB  = if ($null -ne $ll_res.PeakMemKB) { $ll_res.PeakMemKB } else { "FAIL" }
        _ratio    = $ll_c
    }
    Write-Host "  Done: C=$($results[-1].C_sec)s  Rust=$($results[-1].Rust_sec)s  Tau-C=$($results[-1].TauC_sec)s  Tau-LLVM=$($results[-1].Llvm_sec)s" -ForegroundColor Gray
    Write-Host ""
}

} finally { Pop-Location }

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  RESULTS  (seconds -- lower is faster; ratios vs the LLVM backend)" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$fmt = "{0,-22} {1,7} {2,7} {3,7} {4,9} {5,7} {6,8} {7,8}"
Write-Host ($fmt -f "Benchmark", "C", "Rust", "Tau-C", "Tau-LLVM", "LL/C", "LL/Rust", "LL/TauC") -ForegroundColor White
Write-Host ($fmt -f "----------------------", "------", "------", "------", "--------", "------", "-------", "-------") -ForegroundColor DarkGray

foreach ($r in $results) {
    $line = $fmt -f $r.Benchmark, $r.C_sec, $r.Rust_sec, $r.TauC_sec, $r.Llvm_sec, $r.LLoverC, $r.LLoverRs, $r.LLoverTC
    $ratio = $r._ratio
    if ($null -ne $ratio -and $ratio -le 1.05) { Write-Host $line -ForegroundColor Green }
    elseif ($null -ne $ratio -and $ratio -le 1.20) { Write-Host $line -ForegroundColor Yellow }
    else { Write-Host $line -ForegroundColor White }
}

Write-Host ""
Write-Host "  LL/C = Tauraro-LLVM / C, LL/Rust vs rustc, LL/TauC vs the C backend (< 1.00x = faster)" -ForegroundColor DarkGray
Write-Host ""

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  PEAK MEMORY  (KB -- lower is leaner)" -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host ""

$mfmt = "{0,-22} {1,9} {2,9} {3,9} {4,11}"
Write-Host ($mfmt -f "Benchmark", "C(KB)", "Rust(KB)", "TauC(KB)", "TauLLVM(KB)") -ForegroundColor White
Write-Host ($mfmt -f "----------------------", "--------", "--------", "--------", "----------") -ForegroundColor DarkGray
foreach ($r in $results) {
    Write-Host ($mfmt -f $r.Benchmark, $r.C_memKB, $r.Rs_memKB, $r.TC_memKB, $r.LL_memKB) -ForegroundColor White
}
Write-Host ""

# ── Markdown report -> benchmarks/results_llvm.md ───────────────────────────────
$RESULTS_MD = Join-Path $BENCH "results_llvm.md"
$clangVer = if ($haveClang) { (& clang --version 2>$null | Select-Object -First 1) } else { $null }
$gccVer   = (& gcc --version 2>$null | Select-Object -First 1)
$rustVer  = (& rustc --version 2>$null | Select-Object -First 1)

$md = New-Object System.Collections.Generic.List[string]
$md.Add("# Tauraro Benchmark Results -- LLVM backend")
$md.Add("")
$md.Add("Auto-generated by ``benchmarks/run_all_llvm.ps1``. Lower is better in every column.")
$md.Add("Ratios (``LL/*``) are relative to the **LLVM backend** (< 1.00x = LLVM faster/leaner).")
$md.Add("")
$md.Add("- **OS:** Windows $([System.Environment]::OSVersion.Version)")
$md.Add("- **Date (UTC):** $((Get-Date).ToUniversalTime().ToString('yyyy-MM-dd HH:mm:ss'))")
$md.Add("- **Compiler:** ``$TAU_EXE``")
if ($clangVer) { $md.Add("- **LLVM:** $clangVer") }
if ($gccVer)   { $md.Add("- **C:** $gccVer") }
if ($rustVer)  { $md.Add("- **Rust:** $rustVer") }
$md.Add("")
$md.Add("## Wall time (seconds)")
$md.Add("")
$md.Add("| Benchmark | C (s) | Rust (s) | Tau-C (s) | Tau-LLVM (s) | LL/C | LL/Rust | LL/TauC |")
$md.Add("|-----------|------:|---------:|----------:|-------------:|-----:|--------:|--------:|")
foreach ($r in $results) {
    $md.Add("| $($r.Benchmark) | $($r.C_sec) | $($r.Rust_sec) | $($r.TauC_sec) | $($r.Llvm_sec) | $($r.LLoverC) | $($r.LLoverRs) | $($r.LLoverTC) |")
}
$md.Add("")
$md.Add("## Peak resident memory (KB)")
$md.Add("")
$md.Add("| Benchmark | C (KB) | Rust (KB) | Tau-C (KB) | Tau-LLVM (KB) |")
$md.Add("|-----------|-------:|----------:|-----------:|--------------:|")
foreach ($r in $results) {
    $md.Add("| $($r.Benchmark) | $($r.C_memKB) | $($r.Rs_memKB) | $($r.TC_memKB) | $($r.LL_memKB) |")
}
$md.Add("")
$md.Add("_``Tau-C`` is the default C backend; ``Tau-LLVM`` is ``--backend llvm``. ``LL/TauC`` < 1.00x means the LLVM backend is faster than the C backend._")

($md -join "`n") | Out-File -FilePath $RESULTS_MD -Encoding utf8
Write-Host "Wrote Markdown report: $RESULTS_MD" -ForegroundColor Green
Write-Host ""

# Exit 0 so a non-zero $LASTEXITCODE from the last gcc/rustc isn't mistaken for failure.
exit 0
