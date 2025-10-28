//! FFI wrapper for abc module - exports C-compatible functions
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

// Helper function to create a string value
unsafe fn create_string_value(value: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        // In a real implementation, we would allocate and copy the string
        // For now, we'll just set a placeholder
    }
    result
}

// abc.ABCMeta() - Create ABCMeta metaclass
#[no_mangle]
pub extern "C" fn tauraro_abc_abcmeta_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would create an ABCMeta metaclass instance
        // For now, we'll return the argument as a placeholder (similar to the regular implementation)
        arg
    }
}

// abc.ABC() - Create ABC base class
#[no_mangle]
pub extern "C" fn tauraro_abc_abc_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Create a simple object to represent ABC class
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::Object;
        }
        result
    }
}

// abc.abstractmethod(funcobj) - Decorator for abstract methods
#[no_mangle]
pub extern "C" fn tauraro_abc_abstractmethod(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would mark the function as abstract
        // For now, we'll return the function as a placeholder (similar to the regular implementation)
        arg
    }
}

// Additional helper functions that might be needed for ABC functionality

// Check if a class is an abstract base class
#[no_mangle]
pub extern "C" fn tauraro_abc_isabstract(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_bool_value(false);
        }
        
        let arg = *argv.offset(0);
        if !is_valid_value(arg) {
            return create_bool_value(false);
        }
        
        // In a real implementation, we would check if the class has abstract methods
        // For now, we'll return false as a placeholder
        create_bool_value(false)
    }
}

// Register a virtual subclass
#[no_mangle]
pub extern "C" fn tauraro_abc_register(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let cls = *argv.offset(0);
        let subclass = *argv.offset(1);
        
        if !is_valid_value(cls) || !is_valid_value(subclass) {
            return create_none_value();
        }
        
        // In a real implementation, we would register the subclass
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}