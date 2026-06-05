$ErrorActionPreference = "Stop"

# Bootstrap binary — set by CI via BOOTSTRAP_BIN, or fall back to tauraroc.exe on PATH.
$BOOTSTRAP = if ($env:BOOTSTRAP_BIN) { $env:BOOTSTRAP_BIN } else { "tauraroc.exe" }

if (-not (Test-Path $BOOTSTRAP) -and -not (Get-Command $BOOTSTRAP -ErrorAction SilentlyContinue)) {
    Write-Error "ERROR: bootstrap binary not found: $BOOTSTRAP`nSet BOOTSTRAP_BIN to the path of a tauraroc.exe, or put tauraroc.exe on PATH."
    exit 1
}

# -o tauraroc.exe (no path separator) now places the binary in CWD directly: .\tauraroc.exe
Write-Host "==> Compiling src/main.tr -> .\tauraroc.exe"
& $BOOTSTRAP src/main.tr -o tauraroc.exe --static

if (-not (Test-Path ".\tauraroc.exe")) {
    Write-Error "ERROR: tauraroc.exe not produced — compilation failed"
    exit 1
}

Write-Host "==> Done: .\tauraroc.exe"
