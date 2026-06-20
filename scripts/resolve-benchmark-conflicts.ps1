# Helper script to resolve benchmarks/results.md conflicts
# Usage: .\scripts\resolve-benchmark-conflicts.ps1 [-RemoteBranch] [origin/master | upstream/master]
# Example: .\scripts\resolve-benchmark-conflicts.ps1 -RemoteBranch upstream/master

param(
    [string]$RemoteBranch = "upstream/master"
)

Write-Host "Tauraro Benchmark Conflict Resolver" -ForegroundColor Cyan
Write-Host "===================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Resolving conflicts with: $RemoteBranch" -ForegroundColor Yellow
Write-Host "Strategy: Keep your version of benchmarks/results.md" -ForegroundColor Yellow
Write-Host ""

# Check if we're already up to date with the branch
Write-Host "Fetching latest from $RemoteBranch..." -ForegroundColor Gray
git fetch origin 2>&1 | Out-Null
git fetch upstream 2>&1 | Out-Null

# Test if merge would have conflicts
Write-Host "Checking for potential conflicts..." -ForegroundColor Gray
$mergeTest = git merge-base --is-ancestor HEAD $RemoteBranch
$isAncestor = $?

if ($isAncestor) {
    Write-Host "✅ Already up to date with $RemoteBranch - no conflicts!" -ForegroundColor Green
    Write-Host ""
    exit 0
}

Write-Host "🔄 Rebasing against $RemoteBranch with 'ours' strategy..." -ForegroundColor Yellow
Write-Host ""

# Attempt rebase with -X ours strategy
$ErrorActionPreference = "Continue"
git rebase -X ours $RemoteBranch
$rebaseExitCode = $LASTEXITCODE

if ($rebaseExitCode -eq 0) {
    Write-Host ""
    Write-Host "✅ Conflicts resolved successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Summary:" -ForegroundColor Green
    git log --oneline -5
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Green
    Write-Host "  1. Review the changes: git log -10"
    Write-Host "  2. Push: git push origin master --force-with-lease"
    Write-Host ""
}
else {
    Write-Host ""
    Write-Host "❌ Rebase encountered an error (exit code: $rebaseExitCode)" -ForegroundColor Red
    Write-Host "Manually resolve remaining conflicts and run:" -ForegroundColor Yellow
    Write-Host "  git rebase --continue"
    Write-Host ""
    Write-Host "To abort and start over:" -ForegroundColor Yellow
    Write-Host "  git rebase --abort"
    Write-Host ""
    exit 1
}
