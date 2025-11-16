# Test all WebViewTK examples
$tauraro = "..\..\target\debug\tauraro.exe"
$tests = @(
    "01_hello_world.tr",
    "02_container_styles.tr",
    "03_column_layout.tr",
    "04_row_layout.tr",
    "05_nested_layouts.tr",
    "06_edge_insets.tr",
    "07_alignment_test.tr",
    "08_text_styles.tr",
    "09_sized_box.tr",
    "10_padding_widget.tr",
    "11_complex_card.tr",
    "12_dashboard_layout.tr"
)

Write-Host "Testing all WebViewTK examples..." -ForegroundColor Cyan
Write-Host "Press Ctrl+C to skip to next test" -ForegroundColor Yellow
Write-Host ""

foreach ($test in $tests) {
    Write-Host "Running: $test" -ForegroundColor Green
    & $tauraro run $test
    Start-Sleep -Milliseconds 500
    Write-Host ""
}

Write-Host "All tests completed!" -ForegroundColor Green
