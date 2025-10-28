print("FFI Basic Test")
print("Testing load_library...")

load_library("kernel32.dll")
print("Library loaded")

libs = list_libraries()
print("Loaded libraries:")
print(libs)

info = library_info("kernel32.dll")
print("Library info:")
print(info)

define_function("kernel32.dll", "GetTickCount", "uint32", [])
print("GetTickCount defined")

result = call_function("kernel32.dll", "GetTickCount", [])
print("GetTickCount result:")
print(result)

print("Test completed!")
