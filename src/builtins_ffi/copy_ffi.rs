//! FFI wrapper for copy module - exports C-compatible functions
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
}

// Helper function to create a None value
unsafe fn create_none_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::None;
    }
    result
}

// Helper function to check if value is valid
unsafe fn is_valid_value(val: *mut TauraroValue) -> bool {
    !val.is_null()
}

// copy.copy(x) - Shallow copy
#[no_mangle]
pub extern "C" fn tauraro_copy_copy(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments (copy takes exactly 1 argument: x)
        if argc != 1 {
            return create_none_value();
        }
        
        // Check that argument is provided
        if argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would perform a shallow copy
        // For now, we'll return the first argument as a placeholder
        arg
    }
}

// copy.deepcopy(x[, memo]) - Deep copy
#[no_mangle]
pub extern "C" fn tauraro_copy_deepcopy(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments (deepcopy takes 1-2 arguments: x, memo)
        if argc < 1 || argc > 2 {
            return create_none_value();
        }
        
        // Check that all arguments are provided
        if argv.is_null() {
            return create_none_value();
        }
        
        // Validate all arguments
        for i in 0..argc {
            let arg = *argv.offset(i as isize);
            if !is_valid_value(arg) {
                return create_none_value();
            }
        }
        
        // In a real implementation, we would perform a deep copy
        // For now, we'll return the first argument as a placeholder
        *argv.offset(0)
    }
}

// copy.Error([message]) - Exception raised for copy-related errors
#[no_mangle]
pub extern "C" fn tauraro_copy_error_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments (Error takes 0-1 arguments: message)
        if argc > 1 {
            return create_none_value();
        }
        
        // Check that all arguments are provided (if any)
        if argc > 0 && argv.is_null() {
            return create_none_value();
        }
        
        // Validate all arguments
        for i in 0..argc {
            let arg = *argv.offset(i as isize);
            if !is_valid_value(arg) {
                return create_none_value();
            }
        }
        
        // Create an error object
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::Object;
        }
        
        // In a real implementation, we would create a copy error
        // For now, we'll just return an object
        result
    }
}