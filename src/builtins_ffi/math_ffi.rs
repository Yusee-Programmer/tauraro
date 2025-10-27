//! FFI wrapper for math module - exports C-compatible functions
//! This module is compiled to an object file and linked with generated C code

use std::os::raw::c_int;

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
    pub ptr_val: *mut std::ffi::c_void,
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
pub static tauraro_math_pi: f64 = std::f64::consts::PI;

#[no_mangle]
pub static tauraro_math_e: f64 = std::f64::consts::E;

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
        if argc < 1 || argv.is_null() {
            eprintln!("Error: sqrt() requires 1 argument");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.sqrt();
        }

        result
    }
}

// pow(x, y) - x raised to power y
#[no_mangle]
pub extern "C" fn tauraro_math_pow(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() {
            eprintln!("Error: pow() requires 2 arguments");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let y = get_number(*argv.offset(1));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.powf(y);
        }

        result
    }
}

// sin(x) - Sine
#[no_mangle]
pub extern "C" fn tauraro_math_sin(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            eprintln!("Error: sin() requires 1 argument");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.sin();
        }

        result
    }
}

// cos(x) - Cosine
#[no_mangle]
pub extern "C" fn tauraro_math_cos(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            eprintln!("Error: cos() requires 1 argument");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.cos();
        }

        result
    }
}

// tan(x) - Tangent
#[no_mangle]
pub extern "C" fn tauraro_math_tan(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            eprintln!("Error: tan() requires 1 argument");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.tan();
        }

        result
    }
}

// log(x) - Natural logarithm
#[no_mangle]
pub extern "C" fn tauraro_math_log(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            eprintln!("Error: log() requires 1 argument");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.ln();
        }

        result
    }
}

// exp(x) - e raised to power x
#[no_mangle]
pub extern "C" fn tauraro_math_exp(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            eprintln!("Error: exp() requires 1 argument");
            std::process::exit(1);
        }

        let x = get_number(*argv.offset(0));
        let result = tauraro_value_new();

        if !result.is_null() {
            (*result).value_type = TauraroType::Float;
            (*result).data.float_val = x.exp();
        }

        result
    }
}
