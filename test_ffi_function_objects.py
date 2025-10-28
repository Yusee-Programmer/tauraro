print("=" * 70)
print("FFI Function Objects Demo - Call FFI functions like native functions!")
print("=" * 70)
print("")

print("Step 1: Load the library")
load_library("kernel32.dll")
print("✓ kernel32.dll loaded")
print("")

print("Step 2: Define functions and get callable function objects")
print("  Creating GetTickCount function...")
GetTickCount = define_function("kernel32.dll", "GetTickCount", "uint32", [])
print("  ✓ GetTickCount is now a Tauraro function object")

print("  Creating GetCurrentProcessId function...")
GetCurrentProcessId = define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])
print("  ✓ GetCurrentProcessId is now a Tauraro function object")

print("  Creating Sleep function...")
Sleep = define_function("kernel32.dll", "Sleep", "void", ["uint32"])
print("  ✓ Sleep is now a Tauraro function object")
print("")

print("=" * 70)
print("Step 3: Call the functions directly like normal Tauraro functions!")
print("=" * 70)
print("")

print("Calling GetTickCount()...")
uptime = GetTickCount()
print(f"  System uptime: {uptime} milliseconds")
print(f"  That's {uptime / 1000 / 60} minutes")
print("")

print("Calling GetCurrentProcessId()...")
pid = GetCurrentProcessId()
print(f"  Process ID: {pid}")
print("")

print("Calling Sleep(500) - sleeping for 500ms...")
start_time = GetTickCount()
Sleep(500)
end_time = GetTickCount()
elapsed = end_time - start_time
print(f"  Slept for {elapsed}ms")
print("")

print("=" * 70)
print("✓ SUCCESS! FFI functions work like normal Tauraro functions!")
print("=" * 70)
print("")
print("Benefits of this approach:")
print("  • Natural Python-like syntax")
print("  • Functions are first-class objects")
print("  • Can be passed as arguments")
print("  • Can be stored in variables")
print("  • Can be returned from functions")
print("")

print("Example of passing FFI functions as arguments:")
print("")
print("def measure_time(func):")
print("    start = GetTickCount()")
print("    func()")
print("    end = GetTickCount()")
print("    return end - start")
print("")
print("# Pass Sleep as an argument")
print("time_taken = measure_time(lambda: Sleep(250))")
print("")

# Actually demonstrate it
def measure_time(func):
    start = GetTickCount()
    func()
    end = GetTickCount()
    return end - start

time_taken = measure_time(lambda: Sleep(250))
print(f"✓ Measured sleep time: {time_taken}ms")
print("")

print("=" * 70)
print("FFI functions are now fully integrated with Tauraro!")
print("=" * 70)
