//! FFI wrapper for gc module - exports C-compatible functions
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
    fn tauraro_value_new_with_type(t: TauraroType) -> *mut TauraroValue;
}

// Helper function to create an integer value
unsafe fn create_int_value(val: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = val;
    }
    result
}

// Helper function to create a boolean value
unsafe fn create_bool_value(val: bool) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bool;
        (*result).data.bool_val = val;
    }
    result
}

// Helper function to create a string value
unsafe fn create_string_value(s: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        // Note: In a real implementation, we would need to allocate and copy the string
        // For now, we'll just set it to null
        (*result).data.str_val = core::ptr::null_mut();
    }
    result
}

// Helper function to create a tuple value
unsafe fn create_tuple_value(items: &[*mut TauraroValue]) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Tuple;
        // Note: In a real implementation, we would need to store the items
        // For now, we'll just create an empty tuple
    }
    result
}

// Helper function to create a dict value
unsafe fn create_dict_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Dict;
        // Note: In a real implementation, we would need to store key-value pairs
        // For now, we'll just create an empty dict
    }
    result
}

// gc.collect(generation=2) - Run garbage collector
#[no_mangle]
pub extern "C" fn tauraro_gc_collect(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 1 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would run the garbage collector
        // For now, we'll return a placeholder integer (number of objects collected)
        create_int_value(0)
    }
}

// gc.enable() - Enable automatic garbage collection
#[no_mangle]
pub extern "C" fn tauraro_gc_enable(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would enable automatic garbage collection
        // For now, we'll return None as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// gc.disable() - Disable automatic garbage collection
#[no_mangle]
pub extern "C" fn tauraro_gc_disable(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would disable automatic garbage collection
        // For now, we'll return None as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// gc.isenabled() - Check if automatic garbage collection is enabled
#[no_mangle]
pub extern "C" fn tauraro_gc_isenabled(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would check if garbage collection is enabled
        // For now, we'll return True as a placeholder
        create_bool_value(true)
    }
}

// gc.get_stats() - Get garbage collector statistics
#[no_mangle]
pub extern "C" fn tauraro_gc_get_stats(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would get garbage collector statistics
        // For now, we'll return a dict with placeholder statistics
        let stats_dict = create_dict_value();
        stats_dict
    }
}

// gc.set_debug(flags) - Set garbage collector debugging flags
#[no_mangle]
pub extern "C" fn tauraro_gc_set_debug(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would set debugging flags
        // For now, we'll return None as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// gc.get_debug() - Get garbage collector debugging flags
#[no_mangle]
pub extern "C" fn tauraro_gc_get_debug(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would get debugging flags
        // For now, we'll return 0 as a placeholder
        create_int_value(0)
    }
}

// gc.set_threshold(threshold0, threshold1, threshold2) - Set garbage collection thresholds
#[no_mangle]
pub extern "C" fn tauraro_gc_set_threshold(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 || argc > 3 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would set the thresholds
        // For now, we'll return None as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// gc.get_threshold() - Get garbage collection thresholds
#[no_mangle]
pub extern "C" fn tauraro_gc_get_threshold(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would get the thresholds
        // For now, we'll return a tuple with placeholder values
        let items = [
            create_int_value(700),
            create_int_value(10),
            create_int_value(10),
        ];
        create_tuple_value(&items)
    }
}

// gc.freeze() - Freeze garbage collector
#[no_mangle]
pub extern "C" fn tauraro_gc_freeze(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would freeze the garbage collector
        // For now, we'll return None as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// gc.unfreeze() - Unfreeze garbage collector
#[no_mangle]
pub extern "C" fn tauraro_gc_unfreeze(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc > 0 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would unfreeze the garbage collector
        // For now, we'll return None as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::None;
        }
        result
    }
}

// gc.is_finalized() - Check if object has been finalized
#[no_mangle]
pub extern "C" fn tauraro_gc_is_finalized(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_int_value(-1); // Error code
        }
        
        // In a real implementation, we would check if object has been finalized
        // For now, we'll return False as a placeholder
        create_bool_value(false)
    }
}