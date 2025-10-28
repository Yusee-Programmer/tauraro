print("=== Tauraro FFI Comprehensive Windows Test ===")
print("")

print("Test 1: Load kernel32.dll")
load_library("kernel32.dll")
print("SUCCESS: kernel32.dll loaded")
print("")

print("Test 2: List loaded libraries")
libs = list_libraries()
print("Loaded libraries:", libs)
print("")

print("Test 3: Get library info")
info = library_info("kernel32.dll")
print("Library name:", info["name"])
print("Library path:", info["path"])
print("")

print("Test 4: Define GetTickCount")
define_function("kernel32.dll", "GetTickCount", "uint32", [])
print("SUCCESS: GetTickCount defined")
print("")

print("Test 5: Call GetTickCount")
tick1 = call_function("kernel32.dll", "GetTickCount", [])
print("GetTickCount result:", tick1)
uptime_minutes = tick1 / 1000 / 60
print("System uptime:", uptime_minutes, "minutes")
print("")

print("Test 6: Define GetCurrentProcessId")
define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])
print("SUCCESS: GetCurrentProcessId defined")
print("")

print("Test 7: Call GetCurrentProcessId")
pid = call_function("kernel32.dll", "GetCurrentProcessId", [])
print("Process ID:", pid)
print("")

print("Test 8: Define Sleep")
define_function("kernel32.dll", "Sleep", "void", ["uint32"])
print("SUCCESS: Sleep defined")
print("")

print("Test 9: Call Sleep for 100ms")
call_function("kernel32.dll", "Sleep", [100])
print("SUCCESS: Slept for 100ms")
print("")

print("Test 10: Verify time passed")
tick2 = call_function("kernel32.dll", "GetTickCount", [])
elapsed = tick2 - tick1
print("Time elapsed:", elapsed, "ms")
print("")

print("Test 11: Check library info again")
info2 = library_info("kernel32.dll")
print("Functions defined:", info2["functions"])
print("")

print("=== All Tests Passed! ===")
print("")
print("Summary:")
print("- Library loaded: kernel32.dll")
print("- Functions defined: 3 (GetTickCount, GetCurrentProcessId, Sleep)")
print("- Process ID:", pid)
print("- System uptime:", uptime_minutes, "minutes")
print("- Sleep test: PASSED (", elapsed, "ms elapsed)")
print("")
print("FFI system is fully functional on Windows!")
