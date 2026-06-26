# run_zerocopy.ps1 -- ARC vs Zero-copy (borrows / lifetimes / StrView) benchmark.
#
# For each case we compile two source variants:
#   <case>_arc.tr  - idiomatic ARC (by-value, copying slices)
#   <case>_zc.tr   - zero-copy (ref borrows / StrView / borrowed payloads), built --strict
# and measure, for each:
#   TIME_MS  - self-timed workload (optimized build, no counters)
#   PeakKB   - peak working-set memory (external sampling)
#   ALLOCS   - net live heap blocks held at peak  (-DTAURARO_MEMCOUNT build)
#   STRS     - net live TrStr string objects held (-DTAURARO_MEMCOUNT build)
#
# Zero-copy wins show up as: lower TIME (elided retain/release) and/or lower
# STRS/ALLOCS/PeakKB (no data copied). --strict only ENFORCES the borrows --
# it does not change codegen, so ARC and Zero-copy differ only by the source.

$ErrorActionPreference = "Continue"
$BENCH = $PSScriptRoot
$TAU   = Resolve-Path "$BENCH\..\..\tauraroc.exe"
$BDIR  = "$BENCH\build"
$INC   = "$BDIR\include"
$WARN  = "-DTAURARO_NO_RT_HELPERS","-Wno-attributes","-Wno-unused-value","-Wno-string-compare","-Wno-unknown-attributes"

$cases = "str_view","str_pass","list_iter","class_pass","enum_payload","dict_pass","interface"

# A freshly gcc-linked exe on Windows can be intermittently corrupted by AV /
# file-locking during the multi-file compile, so it links but won't run (exit
# 127 / no output). We retry the gcc step and verify the exe actually emits
# TIME_MS before trusting it.
function Exe-Works($exe) {
    if (-not (Test-Path $exe)) { return $false }
    try {
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = $exe; $psi.RedirectStandardOutput = $true; $psi.UseShellExecute = $false
        $p = [System.Diagnostics.Process]::Start($psi)
        $out = $p.StandardOutput.ReadToEnd(); $p.WaitForExit()
        return ($p.ExitCode -eq 0 -and $out -match "TIME_MS:")
    } catch { return $false }
}

function Compile-Variant($file, $strict, $outOpt, $outMc) {
    if (Test-Path $BDIR) { Remove-Item -Recurse -Force $BDIR }
    Push-Location $BENCH   # --emit c writes ./build relative to CWD
    try {
        if ($strict) { & $TAU $file "--strict" "--emit" "c" > $null 2>&1 }
        else         { & $TAU $file "--emit" "c" > $null 2>&1 }
    } finally { Pop-Location }
    if (-not (Test-Path $INC)) { return $false }
    $cfiles = Get-ChildItem -Recurse $BDIR -Filter *.c | ForEach-Object { $_.FullName }
    foreach ($try in 1..6) {
        Remove-Item -Force $outOpt,$outMc -ErrorAction SilentlyContinue
        & gcc -O2 @WARN "-I$INC" -o $outOpt @cfiles -lm -lws2_32 -mconsole 2>$null
        & gcc -O2 -DTAURARO_MEMCOUNT @WARN "-I$INC" -o $outMc @cfiles -lm -lws2_32 -mconsole 2>$null
        if ((Exe-Works $outOpt) -and (Test-Path $outMc)) { return $true }
        Start-Sleep -Milliseconds 400
    }
    return $false
}

function Run-Timed($exe) {
    $best = [double]::MaxValue; $peak = 0L
    foreach ($r in 1..3) {
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = $exe; $psi.RedirectStandardOutput = $true; $psi.UseShellExecute = $false
        $p = [System.Diagnostics.Process]::Start($psi)
        $pk = 0L
        while (-not $p.HasExited) { try { $p.Refresh(); if ($p.PeakWorkingSet64 -gt $pk) { $pk = $p.PeakWorkingSet64 } } catch {}; Start-Sleep -Milliseconds 1 }
        $out = $p.StandardOutput.ReadToEnd()
        try { $p.Refresh(); if ($p.PeakWorkingSet64 -gt $pk) { $pk = $p.PeakWorkingSet64 } } catch {}
        if ($out -match "TIME_MS:(\d+)") { $t = [double]$Matches[1]; if ($t -lt $best) { $best = $t } }
        if ($pk -gt $peak) { $peak = $pk }
    }
    return @{ TimeMs = $best; PeakKB = [math]::Round($peak/1024.0,0) }
}

function Run-Counts($exe) {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $exe; $psi.RedirectStandardOutput = $true; $psi.UseShellExecute = $false
    $p = [System.Diagnostics.Process]::Start($psi)
    $out = $p.StandardOutput.ReadToEnd(); $p.WaitForExit()
    $a = $null; $s = $null
    if ($out -match "ALLOCS:(-?\d+)") { $a = [long]$Matches[1] }
    if ($out -match "STRS:(-?\d+)")   { $s = [long]$Matches[1] }
    return @{ Allocs = $a; Strs = $s }
}

$rows = @()
foreach ($c in $cases) {
    foreach ($v in "arc","zc") {
        $src = "$BENCH\${c}_${v}.tr"
        if (-not (Test-Path $src)) { continue }
        Write-Host "Building $c ($v)..."
        $opt = "$BENCH\${c}_${v}_opt.exe"; $mc = "$BENCH\${c}_${v}_mc.exe"
        if (-not (Compile-Variant $src ($v -eq "zc") $opt $mc)) { Write-Host "  build FAILED"; continue }
        $t = Run-Timed $opt; $n = Run-Counts $mc
        $rows += [pscustomobject]@{ Case=$c; Variant=$v; TimeMs=$t.TimeMs; PeakKB=$t.PeakKB; Allocs=$n.Allocs; Strs=$n.Strs }
    }
}

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add('# Zero-copy vs ARC -- benchmark results')
$lines.Add('')
$lines.Add('ARC variant = by-value / copying. Zero-copy variant = ref borrows / StrView /')
$lines.Add('borrowed payloads, compiled with --strict. Lower is better in every column.')
$lines.Add('')
$lines.Add('| Case | Variant | Time (ms) | Peak (KB) | Live allocs | Live strs |')
$lines.Add('|------|---------|----------:|----------:|------------:|----------:|')
foreach ($row in $rows) {
    $lines.Add(('| {0} | {1} | {2} | {3} | {4} | {5} |' -f $row.Case, $row.Variant, $row.TimeMs, $row.PeakKB, $row.Allocs, $row.Strs))
}
$lines.Add('')
$lines.Add('## Ratio (ARC / Zero-copy) -- higher means zero-copy wins more')
$lines.Add('')
$lines.Add('| Case | Speedup | Mem ratio | Strs ratio |')
$lines.Add('|------|--------:|----------:|-----------:|')
foreach ($c in $cases) {
    $a = $rows | Where-Object { $_.Case -eq $c -and $_.Variant -eq 'arc' } | Select-Object -First 1
    $z = $rows | Where-Object { $_.Case -eq $c -and $_.Variant -eq 'zc' }  | Select-Object -First 1
    if ($null -eq $a -or $null -eq $z) { continue }
    $sp = if ($z.TimeMs -gt 0) { [math]::Round($a.TimeMs / $z.TimeMs, 2) } else { '-' }
    $mr = if ($z.PeakKB -gt 0) { [math]::Round($a.PeakKB / $z.PeakKB, 2) } else { '-' }
    $sr = if ($z.Strs -gt 0)   { [math]::Round([double]$a.Strs / $z.Strs, 2) } else { '-' }
    $lines.Add(('| {0} | {1}x | {2}x | {3}x |' -f $c, $sp, $mr, $sr))
}

$lines | Set-Content -Encoding ascii "$BENCH\results.md"
$lines | ForEach-Object { Write-Host $_ }

Remove-Item -Force "$BENCH\*_opt.exe","$BENCH\*_mc.exe" -ErrorAction SilentlyContinue
if (Test-Path $BDIR) { Remove-Item -Recurse -Force $BDIR }
Write-Host ""
Write-Host "Wrote $BENCH\results.md"
