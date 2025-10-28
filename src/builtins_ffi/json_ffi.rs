//! FFI wrapper for json module - exports C-compatible functions
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
    fn strdup(s: *const u8) -> *mut u8;
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

// Helper function to create a float value
unsafe fn create_float_value(value: f64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Float;
        (*result).data.float_val = value;
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

// json.loads(json_string) - Parse JSON string
#[no_mangle]
pub extern "C" fn tauraro_json_loads(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        let str_ptr = get_string_content(arg);
        if str_ptr.is_null() {
            return create_none_value();
        }
        
        // In a real implementation, we would parse the JSON string
        // For now, we'll return a simple object as a placeholder
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::Dict;
            // Placeholder for parsed JSON object
        }
        result
    }
}

// json.dumps(obj) - Serialize object to JSON string
#[no_mangle]
pub extern "C" fn tauraro_json_dumps(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_string_value("null");
        }
        
        let arg = *argv.offset(0);
        
        // In a real implementation, we would serialize the object to JSON
        // For now, we'll return a placeholder string based on the object type
        match (*arg).value_type {
            TauraroType::None => create_string_value("null"),
            TauraroType::Bool => {
                if (*arg).data.bool_val {
                    create_string_value("true")
                } else {
                    create_string_value("false")
                }
            },
            TauraroType::Int => {
                // In a real implementation, we would format the integer properly
                create_string_value("0")
            },
            TauraroType::Float => {
                // In a real implementation, we would format the float properly
                create_string_value("0.0")
            },
            TauraroType::String => {
                let str_ptr = get_string_content(arg);
                if !str_ptr.is_null() {
                    // In a real implementation, we would properly escape the string
                    create_string_value("\"placeholder\"")
                } else {
                    create_string_value("\"\"")
                }
            },
            _ => create_string_value("{}")
        }
    }
}

// json.load(file) - Load JSON from file
#[no_mangle]
pub extern "C" fn tauraro_json_load(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        // In a real implementation, we would load JSON from a file
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}

// json.dump(obj, file) - Dump JSON to file
#[no_mangle]
pub extern "C" fn tauraro_json_dump(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        // In a real implementation, we would dump JSON to a file
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}

// json.JSONEncoder() - Create JSON encoder
#[no_mangle]
pub extern "C" fn tauraro_json_jsonencoder_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a JSON encoder object
        // For now, we'll return a placeholder object
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::Object;
        }
        result
    }
}

// json.JSONDecoder() - Create JSON decoder
#[no_mangle]
pub extern "C" fn tauraro_json_jsondecoder_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a JSON decoder object
        // For now, we'll return a placeholder object
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::Object;
        }
        result
    }
}