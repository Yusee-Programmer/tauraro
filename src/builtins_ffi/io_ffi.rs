//! FFI wrapper for io module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
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

// Helper function to check if a value is a string
unsafe fn is_string_value(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    (*val).value_type == TauraroType::String
}

// Helper function to get string content
unsafe fn get_string_content(val: *mut TauraroValue) -> *const u8 {
    if val.is_null() || (*val).value_type != TauraroType::String {
        return core::ptr::null();
    }
    (*val).data.str_val
}

// Helper function to create a None value
unsafe fn create_none_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::None;
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

// Helper function to create a boolean value
unsafe fn create_bool_value(value: bool) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bool;
        (*result).data.bool_val = value;
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

// Helper function to create an object value
unsafe fn create_object_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Object;
        // In a real implementation, we would initialize the object properly
    }
    result
}

// io.open(file, mode) - Open a file
#[no_mangle]
pub extern "C" fn tauraro_io_open(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let file_arg = *argv.offset(0);
        if !is_string_value(file_arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would open a file
        // For now, we'll return a placeholder object
        create_object_value()
    }
}

// io.TextIOWrapper() - Create text I/O wrapper
#[no_mangle]
pub extern "C" fn tauraro_io_textiowrapper_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a text I/O wrapper
        // For now, we'll return a placeholder object
        create_object_value()
    }
}

// io.StringIO() - Create string I/O
#[no_mangle]
pub extern "C" fn tauraro_io_stringio_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a string I/O object
        // For now, we'll return a placeholder object
        create_object_value()
    }
}

// io.BytesIO() - Create bytes I/O
#[no_mangle]
pub extern "C" fn tauraro_io_bytesio_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a bytes I/O object
        // For now, we'll return a placeholder object
        create_object_value()
    }
}

// File object read(size) - Read from file
#[no_mangle]
pub extern "C" fn tauraro_io_read(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would read from a file
        // For now, we'll return an empty string as a placeholder
        create_string_value("")
    }
}

// File object write(data) - Write to file
#[no_mangle]
pub extern "C" fn tauraro_io_write(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_int_value(0);
        }
        
        // In a real implementation, we would write to a file
        // For now, we'll return 0 as a placeholder
        create_int_value(0)
    }
}

// File object readline(size) - Read line from file
#[no_mangle]
pub extern "C" fn tauraro_io_readline(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would read a line from a file
        // For now, we'll return an empty string as a placeholder
        create_string_value("")
    }
}

// File object readlines(hint) - Read all lines from file
#[no_mangle]
pub extern "C" fn tauraro_io_readlines(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would read all lines from a file
        // For now, we'll return an empty list as a placeholder
        create_list_value()
    }
}

// File object seek(offset, whence) - Seek to position in file
#[no_mangle]
pub extern "C" fn tauraro_io_seek(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_int_value(0);
        }
        
        // In a real implementation, we would seek to a position in a file
        // For now, we'll return 0 as a placeholder
        create_int_value(0)
    }
}

// File object tell() - Get current position in file
#[no_mangle]
pub extern "C" fn tauraro_io_tell(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would get the current position in a file
        // For now, we'll return 0 as a placeholder
        create_int_value(0)
    }
}

// File object flush() - Flush file buffer
#[no_mangle]
pub extern "C" fn tauraro_io_flush(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would flush the file buffer
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}

// File object close() - Close file
#[no_mangle]
pub extern "C" fn tauraro_io_close(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would close a file
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}

// File object __enter__() - Context manager enter
#[no_mangle]
pub extern "C" fn tauraro_io_enter(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        // Return self
        *argv.offset(0)
    }
}

// File object __exit__(exc_type, exc_val, exc_tb) - Context manager exit
#[no_mangle]
pub extern "C" fn tauraro_io_exit(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would handle context manager exit
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}