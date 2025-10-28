//! FFI wrapper for sys module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// Wrapper type for static const pointers to make them Sync
#[repr(transparent)]
struct ConstPtr(*const u8);
unsafe impl Sync for ConstPtr {}

impl ConstPtr {
    const fn new(ptr: *const u8) -> Self {
        ConstPtr(ptr)
    }
    
    pub const fn as_ptr(&self) -> *const u8 {
        self.0
    }
}

// Type definitions (must match C)
#[repr(C)]
#[derive(PartialEq, Eq)]
pub enum TauraroType {
    Int = 0, Float = 1, Bool = 2, String = 3, List = 4,
    Dict = 5, Tuple = 6, Set = 7, None = 8, Object = 9,
    Function = 10, Bytes = 11, Complex = 12, Range = 13, Frozenset = 14,
}

#[repr(C)]
pub union TauraroData {
    pub int_val: i64,
    pub float_val: f64,
    pub bool_val: bool,
    pub str_val: *mut u8,
}

#[repr(C)]
pub struct TauraroValue {
    pub value_type: TauraroType,
    pub ref_count: c_int,
    pub data: TauraroData,
}

extern "C" {
    fn tauraro_value_new() -> *mut TauraroValue;
    fn malloc(size: usize) -> *mut u8;
    fn free(ptr: *mut u8);
    fn strlen(s: *const u8) -> usize;
    fn strcpy(dest: *mut u8, src: *const u8) -> *mut u8;
    fn exit(code: i32) -> !;
}

// sys.exit(code)
#[no_mangle]
pub extern "C" fn tauraro_sys_exit(argc: c_int, argv: *mut *mut TauraroValue) -> ! {
    unsafe {
        let code = if argc > 0 && !argv.is_null() {
            match (*(*argv.offset(0))).value_type {
                TauraroType::Int => (*(*argv.offset(0))).data.int_val as i32,
                _ => 0,
            }
        } else {
            0
        };
        exit(code);
    }
}

// Helper function to create a string value
unsafe fn create_string_value(s: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        // Allocate and copy the string
        let len = s.len();
        if len == 0 {
            (*result).data.str_val = malloc(1) as *mut u8;
            if !(*result).data.str_val.is_null() {
                *(*result).data.str_val = 0; // Null terminator
            }
        } else {
            (*result).data.str_val = malloc(len + 1) as *mut u8;
            if !(*result).data.str_val.is_null() {
                let src = s.as_ptr();
                for i in 0..len {
                    *(*result).data.str_val.add(i) = *src.add(i);
                }
                *(*result).data.str_val.add(len) = 0; // Null terminator
            }
        }
    }
    result
}

// Helper function to create an integer value
unsafe fn create_int_value(value: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = value;
    }
    result
}

// Helper function to create a None value
unsafe fn create_none_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::None;
    }
    result
}

// Helper function to create a list value
unsafe fn create_list_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::List;
        // In a real implementation, we would initialize the list properly
    }
    result
}

// Helper function to create a tuple value
unsafe fn create_tuple_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Tuple;
        // In a real implementation, we would initialize the tuple properly
    }
    result
}

// Helper function to create a dict value
unsafe fn create_dict_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Dict;
        // In a real implementation, we would initialize the dict properly
    }
    result
}

// Helper function to check if value is valid
unsafe fn is_valid_value(val: *mut TauraroValue) -> bool {
    !val.is_null()
}

// Helper function to check if value is an integer
unsafe fn is_int_value(val: *mut TauraroValue) -> bool {
    !val.is_null() && (*val).value_type == TauraroType::Int
}

// Helper function to check if value is a string
unsafe fn is_string_value(val: *mut TauraroValue) -> bool {
    !val.is_null() && (*val).value_type == TauraroType::String
}

// sys.getrefcount(obj) - Return the reference count of an object
#[no_mangle]
pub extern "C" fn tauraro_sys_getrefcount(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_int_value(0);
        }
        
        // In a real implementation, we would return the reference count
        // For now, we'll return a placeholder value
        create_int_value(1)
    }
}

// sys.getsizeof(obj) - Return the size of an object in bytes
#[no_mangle]
pub extern "C" fn tauraro_sys_getsizeof(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_int_value(0);
        }
        
        // In a real implementation, we would return the size of the object
        // For now, we'll return a placeholder value
        create_int_value(16)
    }
}

// sys.intern(string) - Intern a string
#[no_mangle]
pub extern "C" fn tauraro_sys_intern(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        // In a real implementation, we would intern the string
        // For now, we'll just return the string as-is
        arg
    }
}

// sys.path_append(path) - Append path to sys.path
#[no_mangle]
pub extern "C" fn tauraro_sys_path_append(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would append the path to sys.path
        // For now, we'll return None
        create_none_value()
    }
}

// sys.path_insert(index, path) - Insert path at index in sys.path
#[no_mangle]
pub extern "C" fn tauraro_sys_path_insert(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let index_arg = *argv.offset(0);
        let path_arg = *argv.offset(1);
        
        if !is_int_value(index_arg) || !is_string_value(path_arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would insert the path at the index in sys.path
        // For now, we'll return None
        create_none_value()
    }
}

// sys.path_remove(path) - Remove path from sys.path
#[no_mangle]
pub extern "C" fn tauraro_sys_path_remove(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would remove the path from sys.path
        // For now, we'll return None
        create_none_value()
    }
}

// Constants
// sys.platform - Returns platform string
#[no_mangle]
pub static tauraro_sys_platform: ConstPtr = ConstPtr::new({
    #[cfg(target_os = "windows")]
    { b"win32\0".as_ptr() }
    #[cfg(target_os = "linux")]
    { b"linux\0".as_ptr() }
    #[cfg(target_os = "macos")]
    { b"darwin\0".as_ptr() }
    #[cfg(not(any(target_os = "windows", target_os = "linux", target_os = "macos")))]
    { b"unknown\0".as_ptr() }
});

// sys.version
#[no_mangle]
pub static tauraro_sys_version: ConstPtr = ConstPtr::new(b"Tauraro 0.2.0\0".as_ptr());

// sys.maxsize - Maximum value a variable of integer type can take
#[no_mangle]
pub static tauraro_sys_maxsize: i64 = 9223372036854775807; // i64::MAX

// sys.byteorder - Native byte order
#[no_mangle]
pub static tauraro_sys_byteorder: ConstPtr = ConstPtr::new({
    #[cfg(target_endian = "little")]
    { b"little\0".as_ptr() }
    #[cfg(target_endian = "big")]
    { b"big\0".as_ptr() }
});

// sys.copyright
#[no_mangle]
pub static tauraro_sys_copyright: ConstPtr = ConstPtr::new(b"Copyright (c) 2024 Tauraro Project\0".as_ptr());

// sys.api_version
#[no_mangle]
pub static tauraro_sys_api_version: i64 = 1;

// sys.dont_write_bytecode
#[no_mangle]
pub static tauraro_sys_dont_write_bytecode: bool = false;

// sys.argv - Command line arguments
// This would be initialized at startup in a real implementation
#[no_mangle]
pub static tauraro_sys_argv: ConstPtr = ConstPtr::new(b"[\"tauraro\"]\0".as_ptr());

// sys.executable - Path to the executable
#[no_mangle]
pub static tauraro_sys_executable: ConstPtr = ConstPtr::new(b"tauraro\0".as_ptr());

// sys.stdin - Standard input
#[no_mangle]
pub static tauraro_sys_stdin: ConstPtr = ConstPtr::new(b"<stdin>\0".as_ptr());

// sys.stdout - Standard output
#[no_mangle]
pub static tauraro_sys_stdout: ConstPtr = ConstPtr::new(b"<stdout>\0".as_ptr());

// sys.stderr - Standard error
#[no_mangle]
pub static tauraro_sys_stderr: ConstPtr = ConstPtr::new(b"<stderr>\0".as_ptr());