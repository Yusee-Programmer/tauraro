# run.ps1 -- Rust vs Tauraro, zero-copy / borrow hot paths (Windows).
# Tauraro uses explicit borrow/own/lifetime annotations (ref / @value_type /
# `enum E from r`) and is built --strict, so every borrow is COMPILE-TIME PROVEN
# (and the proven ones elide ARC retain/release) — apples-to-apples with Rust:
#   str_view     - substring views    (Rust &str slice    vs Tauraro StrView value type)
#   enum_payload - borrowed payloads   (Rust enum<'a>      vs Tauraro `enum E from r`)
#   class_field  - shared struct borrow (Rust &Point       vs Tauraro `ref Point`)
#   list_sum     - borrowed iteration   (Rust &Vec<i64>    vs Tauraro `ref List[int]`)
#   dict_borrow  - dict value borrow    (Rust &HashMap+&str vs Tauraro `ref str = d.get(k)`)
#   value_dict   - inline value in dict (Rust HashMap<_,Point> vs Tauraro @value_type)
#   iface_call   - dynamic dispatch     (Rust &dyn Shape   vs Tauraro `ref Shape`)
# Each program self-times its workload and prints TIME_MS:<n>; we also sample
# peak working-set memory.

$ErrorActionPreference = "Continue"
$BENCH = $PSScriptRoot
$TAU   = Resolve-Path "$BENCH\..\..\tauraroc.exe"
$BDIR  = "$BENCH\build"
$WARN  = "-DTAURARO_NO_RT_HELPERS","-Wno-attributes","-Wno-unused-value","-Wno-string-compare","-Wno-unknown-attributes"
$cases = "str_view","enum_payload","class_field","list_sum","dict_borrow","value_dict","iface_call"

function Exe-Works($exe) {
    if (-not (Test-Path $exe)) { return $false }
    try {
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName=$exe; $psi.RedirectStandardOutput=$true; $psi.UseShellExecute=$false
        $p=[System.Diagnostics.Process]::Start($psi); $o=$p.StandardOutput.ReadToEnd(); $p.WaitForExit()
        return ($p.ExitCode -eq 0 -and $o -match "TIME_MS:")
    } catch { return $false }
}

function Build-Rust($name) {
    $exe = "$BENCH\${name}_rs.exe"
    Remove-Item -Force $exe -ErrorAction SilentlyContinue
    & rustc -O -C panic=abort "$BENCH\$name.rs" -o $exe 2>$null
    return (Test-Path $exe)
}

function Build-Tauraro($name) {
    $exe = "$BENCH\${name}_tr.exe"
    if (Test-Path $BDIR) { Remove-Item -Recurse -Force $BDIR }
    Push-Location $BENCH
    try { & $TAU "$BENCH\$name.tr" "--strict" "--emit" "c" > $null 2>&1 } finally { Pop-Location }
    if (-not (Test-Path "$BDIR\include")) { return $false }
    $cf = Get-ChildItem -Recurse $BDIR -Filter *.c | ForEach-Object { $_.FullName }
    foreach ($try in 1..6) {
        Remove-Item -Force $exe -ErrorAction SilentlyContinue
        & gcc -O2 @WARN "-I$BDIR\include" -o $exe @cf -lm -lws2_32 -mconsole 2>$null
        if (Exe-Works $exe) { return $true }
        Start-Sleep -Milliseconds 400
    }
    return $false
}

function Run($exe) {
    $best = [double]::MaxValue; $peak = 0L
    foreach ($r in 1..5) {
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName=$exe; $psi.RedirectStandardOutput=$true; $psi.UseShellExecute=$false
        $p=[System.Diagnostics.Process]::Start($psi); $pk=0L
        while (-not $p.HasExited) { try { $p.Refresh(); if ($p.PeakWorkingSet64 -gt $pk) { $pk=$p.PeakWorkingSet64 } } catch {}; Start-Sleep -Milliseconds 1 }
        $o=$p.StandardOutput.ReadToEnd(); try { $p.Refresh(); if ($p.PeakWorkingSet64 -gt $pk) { $pk=$p.PeakWorkingSet64 } } catch {}
        if ($o -match "TIME_MS:(\d+)") { $t=[double]$Matches[1]; if ($t -lt $best) { $best=$t } }
        if ($pk -gt $peak) { $peak=$pk }
    }
    return @{ TimeMs=$best; PeakKB=[math]::Round($peak/1024.0,0) }
}

$rows=@()
foreach ($c in $cases) {
    Write-Host "Building $c (rust)...";    $rok = Build-Rust $c
    Write-Host "Building $c (tauraro)..."; $tok = Build-Tauraro $c
    if ($rok) { $r = Run "$BENCH\${c}_rs.exe" } else { $r = @{TimeMs="FAIL";PeakKB="FAIL"} }
    if ($tok) { $t = Run "$BENCH\${c}_tr.exe" } else { $t = @{TimeMs="FAIL";PeakKB="FAIL"} }
    $rows += [pscustomobject]@{ Case=$c; RustMs=$r.TimeMs; RustKB=$r.PeakKB; TauMs=$t.TimeMs; TauKB=$t.PeakKB }
}

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add('# Rust vs Tauraro -- zero-copy hot paths')
$lines.Add('')
$lines.Add('Both do genuine zero-copy (borrowed slices / borrowed payloads). Tauraro is')
$lines.Add('built with --strict (the borrows are compile-time proven). Lower is better.')
$lines.Add('')
$lines.Add('| Case | Rust time (ms) | Rust peak (KB) | Tauraro time (ms) | Tauraro peak (KB) |')
$lines.Add('|------|---------------:|---------------:|------------------:|------------------:|')
foreach ($row in $rows) { $lines.Add(('| {0} | {1} | {2} | {3} | {4} |' -f $row.Case,$row.RustMs,$row.RustKB,$row.TauMs,$row.TauKB)) }
$lines | Set-Content -Encoding ascii "$BENCH\results.md"
$lines | ForEach-Object { Write-Host $_ }

Remove-Item -Force "$BENCH\*_rs.exe","$BENCH\*_tr.exe" -ErrorAction SilentlyContinue
if (Test-Path $BDIR) { Remove-Item -Recurse -Force $BDIR }
Write-Host "`nWrote $BENCH\results.md"
