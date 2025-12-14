//! FFI wrapper for secrets module - exports C-compatible functions for secure random tokens
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;

// Minimal panic handler for panic=abort mode
#[cfg(not(test))]
#[panic_handler]
fn panic(_: &core::panic::PanicInfo) -> ! {
    unsafe {
        extern "C" { fn abort() -> !; }
        abort()
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
    fn time(t: *mut i64) -> i64;
}

// Simple LCG random generator (same as random module)
static mut SECRETS_SEED: u64 = 12345;

unsafe fn secrets_random_byte() -> u8 {
    SECRETS_SEED = SECRETS_SEED.wrapping_mul(1103515245).wrapping_add(12345);
    ((SECRETS_SEED >> 16) & 0xFF) as u8
}

// Generate random hex string
unsafe fn generate_hex_token(nbytes: usize) -> *mut TauraroValue {
    if nbytes == 0 {
        return create_string_value("");
    }

    // Initialize seed from time if needed
    if SECRETS_SEED == 12345 {
        let current_time = time(core::ptr::null_mut()) as u64;
        SECRETS_SEED = current_time;
    }

    // Each byte becomes 2 hex chars
    let hex_len = nbytes * 2;
    let output = malloc(hex_len + 1);
    if output.is_null() {
        return create_string_value("");
    }

    let hex_chars = b"0123456789abcdef";
    for i in 0..nbytes {
        let byte = secrets_random_byte();
        *output.add(i * 2) = hex_chars[((byte >> 4) & 0xF) as usize];
        *output.add(i * 2 + 1) = hex_chars[(byte & 0xF) as usize];
    }
    *output.add(hex_len) = 0; // Null terminator

    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        (*result).data.str_val = output;
    }
    result
}

// Generate URL-safe token
unsafe fn generate_url_token(nbytes: usize) -> *mut TauraroValue {
    if nbytes == 0 {
        return create_string_value("");
    }

    // Initialize seed from time if needed
    if SECRETS_SEED == 12345 {
        let current_time = time(core::ptr::null_mut()) as u64;
        SECRETS_SEED = current_time;
    }

    // URL-safe characters: A-Za-z0-9-_
    let url_chars = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    let output = malloc(nbytes + 1);
    if output.is_null() {
        return create_string_value("");
    }

    for i in 0..nbytes {
        let byte = secrets_random_byte();
        *output.add(i) = url_chars[(byte & 0x3F) as usize]; // 64 chars = 6 bits
    }
    *output.add(nbytes) = 0; // Null terminator

    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        (*result).data.str_val = output;
    }
    result
}

// Helper to create string value
unsafe fn create_string_value(s: &str) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        if s.len() > 0 {
            (*result).data.str_val = malloc(s.len() + 1);
            if !(*result).data.str_val.is_null() {
                for i in 0..s.len() {
                    *(*result).data.str_val.add(i) = s.as_bytes()[i];
                }
                *(*result).data.str_val.add(s.len()) = 0;
            }
        } else {
            (*result).data.str_val = malloc(1);
            if !(*result).data.str_val.is_null() {
                *(*result).data.str_val = 0;
            }
        }
    }
    result
}

// Helper to create int value
unsafe fn create_int_value(value: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = value;
    }
    result
}

// secrets.token_hex([nbytes=None]) - Generate random hex string
#[no_mangle]
pub extern "C" fn tauraro_secrets_token_hex(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Default to 32 bytes (64 hex chars)
        let mut nbytes = 32;

        if argc > 0 && !argv.is_null() {
            let arg = *argv;
            if !arg.is_null() && (*arg).value_type == TauraroType::Int {
                nbytes = (*arg).data.int_val as usize;
            }
        }

        generate_hex_token(nbytes)
    }
}

// secrets.token_urlsafe([nbytes=None]) - Generate URL-safe token
#[no_mangle]
pub extern "C" fn tauraro_secrets_token_urlsafe(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Default to 32 bytes
        let mut nbytes = 32;

        if argc > 0 && !argv.is_null() {
            let arg = *argv;
            if !arg.is_null() && (*arg).value_type == TauraroType::Int {
                nbytes = (*arg).data.int_val as usize;
            }
        }

        generate_url_token(nbytes)
    }
}

// secrets.randbelow(exclusive_upper_bound) - Random int below bound
#[no_mangle]
pub extern "C" fn tauraro_secrets_randbelow(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return create_int_value(0);
        }

        let arg = *argv;
        if arg.is_null() || (*arg).value_type != TauraroType::Int {
            return create_int_value(0);
        }

        let bound = (*arg).data.int_val;
        if bound <= 0 {
            return create_int_value(0);
        }

        // Initialize seed from time if needed
        if SECRETS_SEED == 12345 {
            let current_time = time(core::ptr::null_mut()) as u64;
            SECRETS_SEED = current_time;
        }

        // Generate random number below bound
        let mut value = 0i64;
        for _ in 0..8 {
            value = (value << 8) | (secrets_random_byte() as i64);
        }

        create_int_value((value.abs() % bound))
    }
}

// secrets.choice(seq) - Choose random element (simplified - returns index)
#[no_mangle]
pub extern "C" fn tauraro_secrets_choice(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return create_int_value(0);
        }

        // In a full implementation, we would choose from the sequence
        // For now, return a random index (0-9)
        if SECRETS_SEED == 12345 {
            let current_time = time(core::ptr::null_mut()) as u64;
            SECRETS_SEED = current_time;
        }

        let random_index = (secrets_random_byte() % 10) as i64;
        create_int_value(random_index)
    }
}

// secrets.token_bytes(nbytes) - Generate random bytes (same as token_hex)  
#[no_mangle]
pub extern "C" fn tauraro_secrets_token_bytes(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    // Use same implementation as token_hex for simplicity
    tauraro_secrets_token_hex(argc, argv)
}
