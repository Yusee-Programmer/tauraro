use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_double, c_int};

// String manipulation functions
#[no_mangle]
pub extern "C" fn reverse_string(input: *const c_char) -> *mut c_char {
    if input.is_null() {
        return std::ptr::null_mut();
    }
    
    unsafe {
        let c_str = CStr::from_ptr(input);
        if let Ok(str_slice) = c_str.to_str() {
            let reversed: String = str_slice.chars().rev().collect();
            if let Ok(c_string) = CString::new(reversed) {
                return c_string.into_raw();
            }
        }
    }
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn string_length(input: *const c_char) -> c_int {
    if input.is_null() {
        return -1;
    }
    
    unsafe {
        let c_str = CStr::from_ptr(input);
        if let Ok(str_slice) = c_str.to_str() {
            return str_slice.len() as c_int;
        }
    }
    -1
}

#[no_mangle]
pub extern "C" fn concatenate_strings(str1: *const c_char, str2: *const c_char) -> *mut c_char {
    if str1.is_null() || str2.is_null() {
        return std::ptr::null_mut();
    }
    
    unsafe {
        let c_str1 = CStr::from_ptr(str1);
        let c_str2 = CStr::from_ptr(str2);
        
        if let (Ok(s1), Ok(s2)) = (c_str1.to_str(), c_str2.to_str()) {
            let concatenated = format!("{}{}", s1, s2);
            if let Ok(c_string) = CString::new(concatenated) {
                return c_string.into_raw();
            }
        }
    }
    std::ptr::null_mut()
}

// Array operations
#[no_mangle]
pub extern "C" fn sum_array(arr: *const c_int, len: c_int) -> c_int {
    if arr.is_null() || len <= 0 {
        return 0;
    }
    
    unsafe {
        let slice = std::slice::from_raw_parts(arr, len as usize);
        slice.iter().sum()
    }
}

#[no_mangle]
pub extern "C" fn average_array(arr: *const c_int, len: c_int) -> c_double {
    if arr.is_null() || len <= 0 {
        return 0.0;
    }
    
    unsafe {
        let slice = std::slice::from_raw_parts(arr, len as usize);
        let sum: c_int = slice.iter().sum();
        sum as c_double / len as c_double
    }
}

#[no_mangle]
pub extern "C" fn find_max(arr: *const c_int, len: c_int) -> c_int {
    if arr.is_null() || len <= 0 {
        return 0;
    }
    
    unsafe {
        let slice = std::slice::from_raw_parts(arr, len as usize);
        *slice.iter().max().unwrap_or(&0)
    }
}

// Mathematical functions
#[no_mangle]
pub extern "C" fn power_function(base: c_double, exponent: c_double) -> c_double {
    base.powf(exponent)
}

#[no_mangle]
pub extern "C" fn square_root(value: c_double) -> c_double {
    if value < 0.0 {
        return -1.0; // Error indicator
    }
    value.sqrt()
}

#[no_mangle]
pub extern "C" fn factorial(n: c_int) -> c_int {
    if n < 0 {
        return -1; // Error indicator
    }
    if n <= 1 {
        return 1;
    }
    
    let mut result = 1;
    for i in 2..=n {
        result *= i;
    }
    result
}

#[no_mangle]
pub extern "C" fn fibonacci(n: c_int) -> c_int {
    if n < 0 {
        return -1; // Error indicator
    }
    if n <= 1 {
        return n;
    }
    
    let mut a = 0;
    let mut b = 1;
    for _ in 2..=n {
        let temp = a + b;
        a = b;
        b = temp;
    }
    b
}

// Utility functions
#[no_mangle]
pub extern "C" fn is_prime(n: c_int) -> c_int {
    if n < 2 {
        return 0; // false
    }
    if n == 2 {
        return 1; // true
    }
    if n % 2 == 0 {
        return 0; // false
    }
    
    let sqrt_n = (n as f64).sqrt() as c_int;
    for i in (3..=sqrt_n).step_by(2) {
        if n % i == 0 {
            return 0; // false
        }
    }
    1 // true
}

#[no_mangle]
pub extern "C" fn celsius_to_fahrenheit(celsius: c_double) -> c_double {
    celsius * 9.0 / 5.0 + 32.0
}

#[no_mangle]
pub extern "C" fn fahrenheit_to_celsius(fahrenheit: c_double) -> c_double {
    (fahrenheit - 32.0) * 5.0 / 9.0
}

// Memory management
#[no_mangle]
pub extern "C" fn free_string(ptr: *mut c_char) {
    if !ptr.is_null() {
        unsafe {
            let _ = CString::from_raw(ptr);
        }
    }
}

// Random number generation (simple implementation)
static mut SEED: u32 = 1;

#[no_mangle]
pub extern "C" fn random_int(min: c_int, max: c_int) -> c_int {
    if min >= max {
        return min;
    }
    
    unsafe {
        SEED = SEED.wrapping_mul(1103515245).wrapping_add(12345);
        let range = (max - min) as u32;
        min + ((SEED / 65536) % range) as c_int
    }
}

#[no_mangle]
pub extern "C" fn random_double() -> c_double {
    unsafe {
        SEED = SEED.wrapping_mul(1103515245).wrapping_add(12345);
        (SEED as c_double) / (u32::MAX as c_double)
    }
}

// Date/time utilities (simplified)
#[no_mangle]
pub extern "C" fn get_current_year() -> c_int {
    2024 // Simplified implementation
}

#[no_mangle]
pub extern "C" fn get_library_version() -> *mut c_char {
    let version = "1.0.0";
    if let Ok(c_string) = CString::new(version) {
        c_string.into_raw()
    } else {
        std::ptr::null_mut()
    }
}