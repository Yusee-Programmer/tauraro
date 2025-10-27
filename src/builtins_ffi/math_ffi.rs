//! FFI wrapper for math module - exports C-compatible functions
//! This module is compiled to an object file and linked with generated C code
//!
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::{c_int, c_void};
use core::panic::PanicInfo;

// Panic handler required for #![no_std]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// Re-export the tauraro_value_t type definition
// This must match the C definition in generated code
#[repr(C)]
pub enum TauraroType {
    Int = 0,
    Float = 1,
    Bool = 2,
    String = 3,
    List = 4,
    Dict = 5,
    Tuple = 6,
    Set = 7,
    None = 8,
    Object = 9,
    Function = 10,
    Bytes = 11,
    Complex = 12,
    Range = 13,
    Frozenset = 14,
}

#[repr(C)]
pub union TauraroData {
    pub int_val: i64,
    pub float_val: f64,
    pub bool_val: bool,
    pub str_val: *mut u8,
    pub ptr_val: *mut c_void,
}

#[repr(C)]
pub struct TauraroValue {
    pub value_type: TauraroType,
    pub ref_count: c_int,
    pub data: TauraroData,
}

// External function to allocate new value (from main C program)
extern "C" {
    fn tauraro_value_new() -> *mut TauraroValue;
}

// Math constants
#[no_mangle]
pub static tauraro_math_pi: f64 = 3.141592653589793;

#[no_mangle]
pub static tauraro_math_e: f64 = 2.718281828459045;

// Math functions from libm (no_std compatible)
extern "C" {
    fn sqrt(x: f64) -> f64;
    fn pow(x: f64, y: f64) -> f64;
    fn sin(x: f64) -> f64;
    fn cos(x: f64) -> f64;
    fn tan(x: f64) -> f64;
    fn log(x: f64) -> f64;
    fn exp(x: f64) -> f64;
}

// Helper to get numeric value from TauraroValue
unsafe fn get_number(val: *mut TauraroValue) -> f64 {
    if val.is_null() {
        return 0.0;
    }

    match (*val).value_type {
        TauraroType::Int => (*val).data.int_val as f64,
        TauraroType::Float => (*val).data.float_val,
        _ => 0.0,
    }
}

// sqrt(x) - Square root
#[no_mangle]
pub extern "C" fn tauraro_math_sqrt(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Return NULL on invalid arguments (no panic, no std)
        if argc < 1 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = sqrt(x);
        }

        result
    }
}

// pow(x, y) - x raised to power y
#[no_mangle]
pub extern "C" fn tauraro_math_pow(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let y = get_number(*argv.offset(1));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = pow(x, y);
        }

        result
    }
}

// sin(x) - Sine
#[no_mangle]
pub extern "C" fn tauraro_math_sin(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = sin(x);
        }

        result
    }
}

// cos(x) - Cosine
#[no_mangle]
pub extern "C" fn tauraro_math_cos(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = cos(x);
        }

        result
    }
}

// tan(x) - Tangent
#[no_mangle]
pub extern "C" fn tauraro_math_tan(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = tan(x);
        }

        result
    }
}

// log(x) - Natural logarithm
#[no_mangle]
pub extern "C" fn tauraro_math_log(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = log(x);
        }

        result
    }
}

// exp(x) - e raised to power x
#[no_mangle]
pub extern "C" fn tauraro_math_exp(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return core::ptr::null_mut();
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = exp(x);
        }

        result
    }
}
