# Helper script to resolve benchmarks/results.md conflicts
# Usage: .\scripts\resolve-benchmark-conflicts.ps1 [-RemoteBranch] [origin/master | upstream/master]
# Example: .\scripts\resolve-benchmark-conflicts.ps1 upstream/master

param(
    [string]$RemoteBranch = "origin/master"
)

Write-Host "Resolving conflicts with $RemoteBranch using 'ours' strategy..." -ForegroundColor Yellow
Write-Host "This will keep your version of benchmarks/results.md"
Write-Host ""

# Attempt rebase with -X ours strategy
$ErrorActionPreference = "Continue"
git rebase -X ours $RemoteBranch
$rebaseExitCode = $LASTEXITCODE

if ($rebaseExitCode -eq 0) {
    Write-Host ""
    Write-Host "✅ Conflicts resolved successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:"
    Write-Host "  1. Review the changes: git log -5"
    Write-Host "  2. Push: git push origin master --force-with-lease"
}
else {
    Write-Host ""
    Write-Host "❌ Rebase failed with -X ours strategy" -ForegroundColor Red
    Write-Host "Manually resolve remaining conflicts and run:"
    Write-Host "  git rebase --continue"
    exit 1
}
