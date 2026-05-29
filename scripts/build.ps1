$ErrorActionPreference = "Stop"

# Bootstrap binary — set by CI via BOOTSTRAP_BIN, or fall back to tauraroc.exe on PATH.
$BOOTSTRAP = if ($env:BOOTSTRAP_BIN) { $env:BOOTSTRAP_BIN } else { "tauraroc.exe" }

if (-not (Test-Path $BOOTSTRAP) -and -not (Get-Command $BOOTSTRAP -ErrorAction SilentlyContinue)) {
    Write-Error "ERROR: bootstrap binary not found: $BOOTSTRAP`nSet BOOTSTRAP_BIN to the path of a tauraroc.exe, or put tauraroc.exe on PATH."
    exit 1
}

Write-Host "==> Compiling tauraro/src/main.tr -> tauraro\src\build\tauraroc.exe"
& $BOOTSTRAP tauraro/src/main.tr -o tauraroc.exe --static

if (-not (Test-Path ".\tauraro\src\build\tauraroc.exe")) {
    Write-Error "ERROR: tauraroc.exe not produced — compilation failed"
    exit 1
}

Write-Host "==> Done: tauraro\src\build\tauraroc.exe"
