# Soundness corpus runner (§1b/§1c of the safety roadmap) - Windows/PowerShell.
#
# Two suites under tests/soundness/:
#   reject/  - programs that MUST be rejected under --strict. Each file carries a
#              `# EXPECT: [CODE]` header naming the diagnostic that must fire. The
#              runner asserts the compile fails AND that exact code is emitted.
#   accept/  - safe programs that MUST compile under --strict, build, and run
#              (exit 0).
#
# Usage:  scripts/run_soundness.ps1

$ErrorActionPreference = "Continue"

# PREFER the freshly-built .\tauraroc.exe (current source) over the bootstrap
# stage0 seed (BOOTSTRAP_BIN): the soundness corpus validates the SHIPPED compiler.
$TAURAROC = $null
if ($env:TAURAROC) { $TAURAROC = $env:TAURAROC }
elseif (Test-Path ".\tauraroc.exe") { $TAURAROC = ".\tauraroc.exe" }
elseif (Test-Path ".\tauraroc") { $TAURAROC = ".\tauraroc" }
elseif ($env:BOOTSTRAP_BIN -and (Test-Path $env:BOOTSTRAP_BIN)) { $TAURAROC = $env:BOOTSTRAP_BIN }
else { $TAURAROC = ".\tauraroc.exe" }
if (-not (Test-Path $TAURAROC) -and -not (Get-Command $TAURAROC -ErrorAction SilentlyContinue)) {
    Write-Error "ERROR: tauraroc binary not found: $TAURAROC"
    exit 1
}
Write-Host "(compiler under test: $TAURAROC)"
$CC = if ($env:CC) { $env:CC } else { "gcc" }
$WARN = @("-Wno-string-compare","-Wno-comment","-Wno-attributes","-Wno-unused-value")
$LIBS = @("-lm","-lws2_32","-mconsole")

$rejPass = 0; $rejFail = 0; $accPass = 0; $accFail = 0

Write-Host "== REJECT corpus (must fail under --strict with the expected code) =="
foreach ($src in (Get-ChildItem -Path tests/soundness/reject -Filter *.tr -ErrorAction SilentlyContinue | Sort-Object Name)) {
    $name = $src.BaseName
    $m = [regex]::Match((Get-Content $src.FullName -Raw), '\[[A-Z]-[0-9]+\]')
    if (-not $m.Success) { Write-Host "FAIL  $name (missing '# EXPECT: [CODE]' header)"; $rejFail++; continue }
    $want = $m.Value
    $out = & $TAURAROC $src.FullName --strict --check 2>&1 | Out-String
    $rc = $LASTEXITCODE
    if ($rc -eq 0) { Write-Host "FAIL  $name (compiled clean; expected $want)"; $rejFail++; continue }
    if ($out -match [regex]::Escape($want)) { Write-Host "PASS  $name ($want)"; $rejPass++ }
    else { Write-Host "FAIL  $name (failed, but not with $want)"; $rejFail++ }
}

Write-Host ""
Write-Host "== ACCEPT corpus (safe; must compile under --strict, build, run, exit 0) =="
foreach ($src in (Get-ChildItem -Path tests/soundness/accept -Filter *.tr -ErrorAction SilentlyContinue | Sort-Object Name)) {
    $name = $src.BaseName
    $chk = & $TAURAROC $src.FullName --strict --check 2>&1 | Out-String
    if ($LASTEXITCODE -ne 0) { Write-Host "FAIL  $name (--strict rejected a SAFE program)"; $accFail++; continue }
    if (Test-Path build) { Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue }
    & $TAURAROC $src.FullName --strict --emit c 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Host "FAIL  $name (emit)"; $accFail++; continue }
    $cfiles = Get-ChildItem -Path build -Filter *.c -Recurse | ForEach-Object { $_.FullName }
    & $CC -O1 -DTAURARO_NO_RT_HELPERS $WARN -I build/include -o "build/$name.exe" $cfiles $LIBS 2>&1 | Out-Null
    if (-not (Test-Path "build/$name.exe")) { Write-Host "FAIL  $name (C compile)"; $accFail++; continue }
    & "build/$name.exe" | Out-Null
    if ($LASTEXITCODE -eq 0) { Write-Host "PASS  $name"; $accPass++ } else { Write-Host "FAIL  $name (runtime exit != 0)"; $accFail++ }
}
if (Test-Path build) { Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue }

Write-Host ""
Write-Host "================================================"
Write-Host "reject: $rejPass passed, $rejFail failed    accept: $accPass passed, $accFail failed"
if (($rejFail + $accFail) -eq 0) {
    Write-Host "SOUNDNESS CORPUS: all clean."
    exit 0
} else {
    Write-Host "SOUNDNESS CORPUS: FAILURES DETECTED."
    exit 1
}
