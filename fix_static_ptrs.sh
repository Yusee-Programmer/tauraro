#!/bin/bash
# Fix static pointer Sync issues by adding a wrapper type

for module in sys os asyncio; do
  file="src/builtins_ffi/${module}_ffi.rs"
  
  # Check if file exists
  if [ ! -f "$file" ]; then
    echo "Skipping $module - file not found"
    continue
  fi
  
  # Add the ConstPtr wrapper after the panic handler, before type definitions
  sed -i '/^#\[panic_handler\]/,/^}$/{
    /^}$/a\
\
// Wrapper type for static const pointers to make them Sync\
#[repr(transparent)]\
struct ConstPtr(*const u8);\
unsafe impl Sync for ConstPtr {}\
\
impl ConstPtr {\
    const fn new(ptr: *const u8) -> Self {\
        ConstPtr(ptr)\
    }\
    \
    pub const fn as_ptr(&self) -> *const u8 {\
        self.0\
    }\
}
  }' "$file"
  
  # Replace all `pub static ... : *const u8 = ` with `pub static ... : ConstPtr = ConstPtr::new(`
  sed -i 's/\(pub static [^:]*\): \*const u8 = \(.*\);$/\1: ConstPtr = ConstPtr::new(\2);/g' "$file"
  
  echo "Fixed $module"
done
