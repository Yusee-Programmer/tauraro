# Run the regression test suite under tests/lang/ and tests/regression/.
#
# Each test file uses std/test's TestRunner and is run with `tauraroc --run`.
# A test file passes iff:
#   - the compile+run exits 0, AND
#   - stdout does NOT contain the word "FAILED"
#
# Usage: scripts/run_tests.ps1 [path/to/single_test.tr]

param(
    [string[]]$Files
)

$ErrorActionPreference = "Continue"

$TAURAROC = if ($env:BOOTSTRAP_BIN) { $env:BOOTSTRAP_BIN } else { ".\tauraroc.exe" }
if (-not (Test-Path $TAURAROC) -and -not (Get-Command $TAURAROC -ErrorAction SilentlyContinue)) {
    if (Test-Path ".\tauraroc") {
        $TAURAROC = ".\tauraroc"
    } else {
        Write-Error "ERROR: tauraroc binary not found: $TAURAROC`nSet BOOTSTRAP_BIN, or build .\tauraroc.exe / .\tauraroc first."
        exit 1
    }
}

if (-not $Files -or $Files.Count -eq 0) {
    $Files = @(Get-ChildItem -Path tests/lang, tests/regression -Filter *.tr -Recurse -ErrorAction SilentlyContinue |
        Sort-Object FullName | ForEach-Object { $_.FullName })
}

$total = 0
$failed = 0
$failedFiles = @()

foreach ($f in $Files) {
    $total++
    Write-Host "==> $f"
    $out = & $TAURAROC --run $f 2>&1 | Out-String
    $status = $LASTEXITCODE
    Write-Host $out
    if ($status -ne 0 -or $out -match "FAILED") {
        $failed++
        $failedFiles += $f
    }
}

# --- Formatter idempotency check ------------------------------------------
# `tauraroc fmt` must be idempotent: fmt(fmt(x)) == fmt(x).
foreach ($FSAMPLE in @("examples/02_operators.tr", "examples/03_control_flow.tr")) {
    if (-not (Test-Path $FSAMPLE)) { continue }
    $total++
    Write-Host "==> fmt idempotency: $FSAMPLE"
    $f1 = & $TAURAROC fmt $FSAMPLE 2>$null | Out-String
    $f1file = [System.IO.Path]::GetTempFileName()
    Set-Content -Path $f1file -Value $f1 -NoNewline -Encoding utf8
    $f2 = & $TAURAROC fmt $f1file 2>$null | Out-String
    Remove-Item -Force $f1file -ErrorAction SilentlyContinue
    if ($f1 -ne $f2) {
        Write-Host "  NOT IDEMPOTENT"
        $failed++
        $failedFiles += "fmt:$FSAMPLE"
    }
}

Write-Host ""
Write-Host "==================================="
Write-Host "Test files: $total, failed: $failed"
if ($failed -gt 0) {
    Write-Host "Failed files:"
    foreach ($f in $failedFiles) {
        Write-Host "  - $f"
    }
    exit 1
}
Write-Host "All test files passed."
