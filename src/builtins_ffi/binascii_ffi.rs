//! FFI wrapper for binascii module - binary/ASCII conversions
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
    fn strlen(s: *const u8) -> usize;
}

// Convert bytes to hex string
unsafe fn bytes_to_hex(data: *const u8, len: usize) -> *mut TauraroValue {
    if data.is_null() || len == 0 {
        return create_string_value("");
    }

    let hex_len = len * 2;
    let output = malloc(hex_len + 1);
    if output.is_null() {
        return create_string_value("");
    }

    let hex_chars = b"0123456789abcdef";
    for i in 0..len {
        let byte = *data.add(i);
        *output.add(i * 2) = hex_chars[((byte >> 4) & 0xF) as usize];
        *output.add(i * 2 + 1) = hex_chars[(byte & 0xF) as usize];
    }
    *output.add(hex_len) = 0;

    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        (*result).data.str_val = output;
    }
    result
}

// Convert hex string to bytes
unsafe fn hex_to_bytes(hex: *const u8) -> *mut TauraroValue {
    if hex.is_null() {
        return create_string_value("");
    }

    let hex_len = strlen(hex);
    if hex_len == 0 || hex_len % 2 != 0 {
        return create_string_value("");
    }

    let byte_len = hex_len / 2;
    let output = malloc(byte_len + 1);
    if output.is_null() {
        return create_string_value("");
    }

    for i in 0..byte_len {
        let high = hex_digit_to_int(*hex.add(i * 2));
        let low = hex_digit_to_int(*hex.add(i * 2 + 1));
        *output.add(i) = (high << 4) | low;
    }
    *output.add(byte_len) = 0;

    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        (*result).data.str_val = output;
    }
    result
}

// Helper to convert hex digit to int
unsafe fn hex_digit_to_int(ch: u8) -> u8 {
    match ch {
        b'0'..=b'9' => ch - b'0',
        b'A'..=b'F' => ch - b'A' + 10,
        b'a'..=b'f' => ch - b'a' + 10,
        _ => 0,
    }
}

// Calculate CRC32 checksum (simplified polynomial)
unsafe fn calculate_crc32(data: *const u8, len: usize) -> u32 {
    if data.is_null() || len == 0 {
        return 0;
    }

    let mut crc: u32 = 0xFFFFFFFF;
    for i in 0..len {
        let byte = *data.add(i);
        crc ^= byte as u32;
        for _ in 0..8 {
            if crc & 1 != 0 {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    !crc
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

// binascii.hexlify(data) - Convert bytes to hex
#[no_mangle]
pub extern "C" fn tauraro_binascii_hexlify(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return create_string_value("");
        }

        let arg = *argv;
        if arg.is_null() || (*arg).value_type != TauraroType::String {
            return create_string_value("");
        }

        let data = (*arg).data.str_val;
        let len = strlen(data);

        bytes_to_hex(data, len)
    }
}

// binascii.unhexlify(hexstr) - Convert hex to bytes
#[no_mangle]
pub extern "C" fn tauraro_binascii_unhexlify(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return create_string_value("");
        }

        let arg = *argv;
        if arg.is_null() || (*arg).value_type != TauraroType::String {
            return create_string_value("");
        }

        let hex_str = (*arg).data.str_val;

        hex_to_bytes(hex_str)
    }
}

// binascii.crc32(data[, crc]) - CRC32 checksum
#[no_mangle]
pub extern "C" fn tauraro_binascii_crc32(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return create_int_value(0);
        }

        let arg = *argv;
        if arg.is_null() || (*arg).value_type != TauraroType::String {
            return create_int_value(0);
        }

        let data = (*arg).data.str_val;
        let len = strlen(data);

        let crc = calculate_crc32(data, len);

        create_int_value(crc as i64)
    }
}

// binascii.b2a_hex(data) - Alias for hexlify
#[no_mangle]
pub extern "C" fn tauraro_binascii_b2a_hex(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    tauraro_binascii_hexlify(argc, argv)
}

// binascii.a2b_hex(hexstr) - Alias for unhexlify
#[no_mangle]
pub extern "C" fn tauraro_binascii_a2b_hex(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    tauraro_binascii_unhexlify(argc, argv)
}
