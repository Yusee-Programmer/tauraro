# WebViewTK Test Runner
# Run all tests sequentially with delays

Write-Host "WebViewTK Test Suite Runner" -ForegroundColor Cyan
Write-Host "=============================" -ForegroundColor Cyan
Write-Host ""

# Build first
Write-Host "Building with webviewtk feature..." -ForegroundColor Yellow
cargo build --features webviewtk
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}
Write-Host "Build successful!" -ForegroundColor Green
Write-Host ""

# Get all test files
$testFiles = Get-ChildItem "examples\webviewtk_tests\*.tr" | Sort-Object Name

Write-Host "Found $($testFiles.Count) tests" -ForegroundColor Green
Write-Host ""

# Run each test
foreach ($test in $testFiles) {
    Write-Host "Running: $($test.Name)" -ForegroundColor Cyan
    Write-Host "Press Ctrl+C to skip to next test, or close window when done viewing" -ForegroundColor Gray
    
    .\target\debug\tauraro.exe run $test.FullName
    
    Write-Host "Test completed: $($test.Name)" -ForegroundColor Green
    Write-Host ""
    Start-Sleep -Seconds 1
}

Write-Host "All tests completed!" -ForegroundColor Green
