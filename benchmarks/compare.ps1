$py_code = @"
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

print(fib(20))
"@

$tr_code = @"
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

print(fib(20))
"@

Write-Host "TAURARO vs PYTHON BENCHMARK" -ForegroundColor Green
Write-Host "=" * 60

Write-Host "`nTesting Fibonacci(20)...`n" -ForegroundColor Yellow

Write-Host "Python:" -ForegroundColor Cyan
$py_start = Get-Date
$py_output = python -c $py_code
$py_time = ((Get-Date) - $py_start).TotalSeconds
Write-Host "Output: $py_output"
Write-Host "Time: $py_time seconds"

Write-Host "`nTauraro:" -ForegroundColor Cyan
$tr_start = Get-Date
$tr_output = & "c:\Users\Yusee Habibu\Downloads\tauraro\target\release\tauraro.exe" run -c $tr_code 2>&1 | Select-String "[0-9]+"
$tr_time = ((Get-Date) - $tr_start).TotalSeconds
Write-Host "Output: $tr_output"
Write-Host "Time: $tr_time seconds"

Write-Host "`n" + "=" * 60
$speedup = $py_time / $tr_time
Write-Host "Speedup: $speedup x" -ForegroundColor Green
