print("=" * 70)
print("FFI Path Loading Test - Loading libraries by full path")
print("=" * 70)
print("")

print("Test 1: Load kernel32.dll by full path")
full_path = "C:\\Windows\\System32\\kernel32.dll"
print(f"Loading: {full_path}")
load_library(full_path)
print("✓ Loaded successfully by full path")
print("")

libs = list_libraries()
print(f"Loaded libraries: {libs}")
print("")

info = library_info(full_path)
print("Library info:")
print(f"  Name: {info['name']}")
print(f"  Path: {info['path']}")
print("")

print("Test 2: Define and call function from path-loaded library")
define_function(full_path, "GetTickCount", "uint32", [])
result = call_function(full_path, "GetTickCount", [])
print(f"✓ GetTickCount() = {result}")
print("")

print("Test 3: Load OpenGL32.dll by full path")
opengl_path = "C:\\Windows\\System32\\opengl32.dll"
print(f"Loading: {opengl_path}")
load_library(opengl_path)
print("✓ OpenGL32.dll loaded successfully")
print("")

libs = list_libraries()
print(f"Total libraries loaded: {len(libs)}")
for lib in libs:
    info = library_info(lib)
    print(f"  • {lib}")
    print(f"    Path: {info['path']}")
print("")

print("=" * 70)
print("✓ Path loading works perfectly!")
print("=" * 70)
print("")
print("You can now load libraries by:")
print("  1. Name only: load_library('kernel32.dll')")
print("  2. Full path: load_library('C:\\\\Windows\\\\System32\\\\kernel32.dll')")
print("  3. Relative path: load_library('./mylib.dll')")
