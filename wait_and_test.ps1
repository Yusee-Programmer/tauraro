$timeout = 0
while ((Test-Path "C:\Users\Yusee Habibu\Downloads\tauraro\target\release\tauraro.exe") -eq $false -and $timeout -lt 60) {
    Start-Sleep -Seconds 5
    $timeout += 5
    Write-Host "Waiting for build... ($timeout seconds elapsed)"
}

if (Test-Path "C:\Users\Yusee Habibu\Downloads\tauraro\target\release\tauraro.exe") {
    Write-Host "Build complete! Running test..."
    cd "C:\Users\Yusee Habibu\Downloads\tauraro"
    & ".\target\release\tauraro.exe" run test_type_3arg.tr
} else {
    Write-Host "Build timeout after $timeout seconds"
}
