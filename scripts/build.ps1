$ErrorActionPreference = "Stop"

# Bootstrap binary — set by CI via BOOTSTRAP_BIN, or fall back to tauraroc.exe on PATH.
$BOOTSTRAP = if ($env:BOOTSTRAP_BIN) { $env:BOOTSTRAP_BIN } else { "tauraroc.exe" }

if (-not (Test-Path $BOOTSTRAP) -and -not (Get-Command $BOOTSTRAP -ErrorAction SilentlyContinue)) {
    Write-Error "ERROR: bootstrap binary not found: $BOOTSTRAP`nSet BOOTSTRAP_BIN to the path of a tauraroc.exe, or put tauraroc.exe on PATH."
    exit 1
}

Write-Host "==> Compiling src/main.tr -> .\tauraroc.exe"
& $BOOTSTRAP src/main.tr -o tauraroc.exe --static

# New bootstrap (v0.0.4+): binary lands in CWD as .\tauraroc.exe
# Old bootstrap (<=v0.0.3): binary lands in src\build\tauraroc.exe
# Normalise: move from old location to CWD if needed.
if (-not (Test-Path ".\tauraroc.exe") -and (Test-Path ".\src\build\tauraroc.exe")) {
    Write-Host "==> Moving src\build\tauraroc.exe -> .\tauraroc.exe (old bootstrap compat)"
    Move-Item ".\src\build\tauraroc.exe" ".\tauraroc.exe"
}

if (-not (Test-Path ".\tauraroc.exe")) {
    Write-Error "ERROR: tauraroc.exe not produced — compilation failed"
    exit 1
}

Write-Host "==> Done: .\tauraroc.exe"
