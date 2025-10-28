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

#[no_mangle]
pub static tauraro_math_tau: f64 = 6.283185307179586;

#[no_mangle]
pub static tauraro_math_inf: f64 = f64::INFINITY;

#[no_mangle]
pub static tauraro_math_nan: f64 = f64::NAN;

// Math functions from libm (no_std compatible)
extern "C" {
    fn sqrt(x: f64) -> f64;
    fn pow(x: f64, y: f64) -> f64;
    fn sin(x: f64) -> f64;
    fn cos(x: f64) -> f64;
    fn tan(x: f64) -> f64;
    fn log(x: f64) -> f64;
    fn exp(x: f64) -> f64;
    fn asin(x: f64) -> f64;
    fn acos(x: f64) -> f64;
    fn atan(x: f64) -> f64;
    fn atan2(y: f64, x: f64) -> f64;
    fn sinh(x: f64) -> f64;
    fn cosh(x: f64) -> f64;
    fn tanh(x: f64) -> f64;
    fn asinh(x: f64) -> f64;
    fn acosh(x: f64) -> f64;
    fn atanh(x: f64) -> f64;
    fn log2(x: f64) -> f64;
    fn log10(x: f64) -> f64;
    fn log1p(x: f64) -> f64;
    fn exp2(x: f64) -> f64;
    fn expm1(x: f64) -> f64;
    fn ceil(x: f64) -> f64;
    fn floor(x: f64) -> f64;
    fn trunc(x: f64) -> f64;
    fn fabs(x: f64) -> f64;
    fn fmod(x: f64, y: f64) -> f64;
    fn remainder(x: f64, y: f64) -> f64;
    fn copysign(x: f64, y: f64) -> f64;
    fn fdim(x: f64, y: f64) -> f64;
    fn fmax(x: f64, y: f64) -> f64;
    fn fmin(x: f64, y: f64) -> f64;
    fn hypot(x: f64, y: f64) -> f64;
    fn nextafter(x: f64, y: f64) -> f64;
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

// Helper to create a float result
unsafe fn create_float_result(value: f64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Float;
        (*result).data.float_val = value;
    }
    result
}

// Helper to check if a value is valid for math operations
unsafe fn is_valid_number(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    
    match (*val).value_type {
        TauraroType::Int | TauraroType::Float => true,
        _ => false,
    }
}

// Helper to check for domain errors
unsafe fn check_domain_error(x: f64) -> bool {
    x.is_nan() || x.is_infinite()
}

// pow(x, y) - x raised to power y
#[no_mangle]
pub extern "C" fn tauraro_math_pow(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return core::ptr::null_mut();
        }

        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_valid_number(arg1) || !is_valid_number(arg2) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg1);
        let y = get_number(arg2);
        create_float_result(pow(x, y))
    }
}

// sqrt(x) - Square root
#[no_mangle]
pub extern "C" fn tauraro_math_sqrt(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be >= 0
        if x < 0.0 {
            return core::ptr::null_mut();
        }
        create_float_result(sqrt(x))
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
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be > 0
        if x <= 0.0 {
            return core::ptr::null_mut();
        }
        create_float_result(log(x))
    }
}

// log2(x) - Base-2 logarithm
#[no_mangle]
pub extern "C" fn tauraro_math_log2(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be > 0
        if x <= 0.0 {
            return core::ptr::null_mut();
        }
        create_float_result(log2(x))
    }
}

// log10(x) - Base-10 logarithm
#[no_mangle]
pub extern "C" fn tauraro_math_log10(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be > 0
        if x <= 0.0 {
            return core::ptr::null_mut();
        }
        create_float_result(log10(x))
    }
}

// log1p(x) - log(1+x)
#[no_mangle]
pub extern "C" fn tauraro_math_log1p(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be > -1
        if x <= -1.0 {
            return core::ptr::null_mut();
        }
        create_float_result(log1p(x))
    }
}

// exp2(x) - 2 raised to power x
#[no_mangle]
pub extern "C" fn tauraro_math_exp2(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(exp2(x))
    }
}

// expm1(x) - exp(x) - 1
#[no_mangle]
pub extern "C" fn tauraro_math_expm1(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(expm1(x))
    }
}

// exp(x) - e raised to power x
#[no_mangle]
pub extern "C" fn tauraro_math_exp(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(exp(x))
    }
}

// asin(x) - Arc sine
#[no_mangle]
pub extern "C" fn tauraro_math_asin(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be in [-1, 1]
        if x < -1.0 || x > 1.0 {
            return core::ptr::null_mut();
        }
        create_float_result(asin(x))
    }
}

// acos(x) - Arc cosine
#[no_mangle]
pub extern "C" fn tauraro_math_acos(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be in [-1, 1]
        if x < -1.0 || x > 1.0 {
            return core::ptr::null_mut();
        }
        create_float_result(acos(x))
    }
}

// atan(x) - Arc tangent
#[no_mangle]
pub extern "C" fn tauraro_math_atan(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(atan(x))
    }
}

// atan2(y, x) - Arc tangent of y/x
#[no_mangle]
pub extern "C" fn tauraro_math_atan2(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_valid_number(arg1) || !is_valid_number(arg2) {
            return core::ptr::null_mut();
        }

        let y = get_number(arg1);
        let x = get_number(arg2);
        create_float_result(atan2(y, x))
    }
}

// sinh(x) - Hyperbolic sine
#[no_mangle]
pub extern "C" fn tauraro_math_sinh(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(sinh(x))
    }
}

// cosh(x) - Hyperbolic cosine
#[no_mangle]
pub extern "C" fn tauraro_math_cosh(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(cosh(x))
    }
}

// tanh(x) - Hyperbolic tangent
#[no_mangle]
pub extern "C" fn tauraro_math_tanh(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(tanh(x))
    }
}

// asinh(x) - Inverse hyperbolic sine
#[no_mangle]
pub extern "C" fn tauraro_math_asinh(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(asinh(x))
    }
}

// acosh(x) - Inverse hyperbolic cosine
#[no_mangle]
pub extern "C" fn tauraro_math_acosh(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be >= 1
        if x < 1.0 {
            return core::ptr::null_mut();
        }
        create_float_result(acosh(x))
    }
}

// atanh(x) - Inverse hyperbolic tangent
#[no_mangle]
pub extern "C" fn tauraro_math_atanh(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        // Domain check: x must be in (-1, 1)
        if x <= -1.0 || x >= 1.0 {
            return core::ptr::null_mut();
        }
        create_float_result(atanh(x))
    }
}

// ceil(x) - Ceiling
#[no_mangle]
pub extern "C" fn tauraro_math_ceil(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(ceil(x))
    }
}

// floor(x) - Floor
#[no_mangle]
pub extern "C" fn tauraro_math_floor(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(floor(x))
    }
}

// trunc(x) - Truncate
#[no_mangle]
pub extern "C" fn tauraro_math_trunc(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(trunc(x))
    }
}

// fabs(x) - Absolute value
#[no_mangle]
pub extern "C" fn tauraro_math_fabs(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg = *argv.offset(0);
        if !is_valid_number(arg) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg);
        create_float_result(fabs(x))
    }
}

// fmod(x, y) - Floating-point remainder
#[no_mangle]
pub extern "C" fn tauraro_math_fmod(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_valid_number(arg1) || !is_valid_number(arg2) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg1);
        let y = get_number(arg2);
        // Domain check: y must not be zero
        if y == 0.0 {
            return core::ptr::null_mut();
        }
        create_float_result(fmod(x, y))
    }
}

// remainder(x, y) - IEEE remainder
#[no_mangle]
pub extern "C" fn tauraro_math_remainder(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_valid_number(arg1) || !is_valid_number(arg2) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg1);
        let y = get_number(arg2);
        // Domain check: y must not be zero
        if y == 0.0 {
            return core::ptr::null_mut();
        }
        create_float_result(remainder(x, y))
    }
}

// copysign(x, y) - Copy sign
#[no_mangle]
pub extern "C" fn tauraro_math_copysign(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_valid_number(arg1) || !is_valid_number(arg2) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg1);
        let y = get_number(arg2);
        create_float_result(copysign(x, y))
    }
}

// nextafter(x, y) - Next representable value
#[no_mangle]
pub extern "C" fn tauraro_math_nextafter(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return core::ptr::null_mut();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_valid_number(arg1) || !is_valid_number(arg2) {
            return core::ptr::null_mut();
        }

        let x = get_number(arg1);
        let y = get_number(arg2);
        create_float_result(nextafter(x, y))
    }
}
