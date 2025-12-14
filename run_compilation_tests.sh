#!/bin/bash
echo "====================================="
echo "Testing C Transpiler with Lazy Code Generation"
echo "====================================="
echo ""

# Test 1: Minimal (no built-ins)
echo "TEST 1: Minimal script (no built-ins)"
./target/release/tauraro.exe compile test_minimal.py --backend c --use-native-transpiler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    lines=$(wc -l < test_minimal.c 2>/dev/null || echo "0")
    echo "✓ SUCCESS - Generated $lines lines"
    grep -q "tauraro_print" test_minimal.c && echo "  ✗ Contains print functions (should not)" || echo "  ✓ No unused print functions"
    grep -q "tauraro_str" test_minimal.c && echo "  ✗ Contains str functions (should not)" || echo "  ✓ No unused str functions"
else
    echo "✗ FAILED to compile"
fi
echo ""

# Test 2: With print only
echo "TEST 2: Script with print only"
./target/release/tauraro.exe compile test_with_print.py --backend c --use-native-transpiler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    lines=$(wc -l < test_with_print.c 2>/dev/null || echo "0")
    echo "✓ SUCCESS - Generated $lines lines"
    grep -q "tauraro_print" test_with_print.c && echo "  ✓ Contains print functions" || echo "  ✗ Missing print functions"
    grep -q "tauraro_str" test_with_print.c && echo "  ✗ Contains unused str functions" || echo "  ✓ No unused str functions"
else
    echo "✗ FAILED to compile"
fi
echo ""

# Test 3: Advanced features (all built-ins)
echo "TEST 3: Advanced features (print, str, int, float, len)"
./target/release/tauraro.exe compile test_advanced_features.py --backend c --use-native-transpiler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    lines=$(wc -l < test_advanced_features.c 2>/dev/null || echo "0")
    echo "✓ SUCCESS - Generated $lines lines"
    grep -q "tauraro_print" test_advanced_features.c && echo "  ✓ Contains print functions" || echo "  ✗ Missing print"
    grep -q "tauraro_str" test_advanced_features.c && echo "  ✓ Contains str functions" || echo "  ✗ Missing str"
    grep -q "tauraro_int" test_advanced_features.c && echo "  ✓ Contains int functions" || echo "  ✗ Missing int"
    grep -q "tauraro_float" test_advanced_features.c && echo "  ✓ Contains float functions" || echo "  ✗ Missing float"
    grep -q "tauraro_len" test_advanced_features.c && echo "  ✓ Contains len functions" || echo "  ✓ Missing len"
else
    echo "✗ FAILED to compile"
fi
echo ""

# Test 4: OOP
echo "TEST 4: OOP with classes and inheritance"
./target/release/tauraro.exe compile test_oop_simple.py --backend c --use-native-transpiler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    lines=$(wc -l < test_oop_simple.c 2>/dev/null || echo "0")
    echo "✓ SUCCESS - Generated $lines lines"
    grep -q "struct Animal" test_oop_simple.c && echo "  ✓ Generated Animal struct" || echo "  ✗ Missing Animal struct"
    grep -q "struct Dog" test_oop_simple.c && echo "  ✓ Generated Dog struct" || echo "  ✗ Missing Dog struct"
else
    echo "✗ FAILED to compile"
fi
echo ""

# Test 5: Comprehensive (modules and built-ins)
echo "TEST 5: Comprehensive modules test"
./target/release/tauraro.exe compile test_use_builtins.py --backend c --use-native-transpiler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    lines=$(wc -l < test_use_builtins.c 2>/dev/null || echo "0")
    echo "✓ SUCCESS - Generated $lines lines"
    [ -f "build/headers/test_builtins_module.h" ] && echo "  ✓ Generated module header" || echo "  ✗ Missing module header"
else
    echo "✗ FAILED to compile"
fi
echo ""

echo "====================================="
echo "Summary: All compilation tests completed"
echo "====================================="
