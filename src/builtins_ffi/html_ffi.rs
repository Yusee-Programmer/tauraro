//! FFI wrapper for html module - exports C-compatible functions
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

// HTML escape a string
unsafe fn html_escape_string(input: *const u8) -> *mut TauraroValue {
    if input.is_null() {
        return create_string_value("");
    }

    let len = strlen(input);
    if len == 0 {
        return create_string_value("");
    }

    // Allocate buffer (worst case: each char becomes &entity; = 6 bytes)
    let output = malloc(len * 6 + 1);
    if output.is_null() {
        return create_string_value("");
    }

    let mut out_pos = 0;
    for i in 0..len {
        let ch = *input.add(i);
        match ch {
            b'<' => {
                // &lt;
                *output.add(out_pos) = b'&';
                *output.add(out_pos + 1) = b'l';
                *output.add(out_pos + 2) = b't';
                *output.add(out_pos + 3) = b';';
                out_pos += 4;
            },
            b'>' => {
                // &gt;
                *output.add(out_pos) = b'&';
                *output.add(out_pos + 1) = b'g';
                *output.add(out_pos + 2) = b't';
                *output.add(out_pos + 3) = b';';
                out_pos += 4;
            },
            b'&' => {
                // &amp;
                *output.add(out_pos) = b'&';
                *output.add(out_pos + 1) = b'a';
                *output.add(out_pos + 2) = b'm';
                *output.add(out_pos + 3) = b'p';
                *output.add(out_pos + 4) = b';';
                out_pos += 5;
            },
            b'"' => {
                // &quot;
                *output.add(out_pos) = b'&';
                *output.add(out_pos + 1) = b'q';
                *output.add(out_pos + 2) = b'u';
                *output.add(out_pos + 3) = b'o';
                *output.add(out_pos + 4) = b't';
                *output.add(out_pos + 5) = b';';
                out_pos += 6;
            },
            b'\'' => {
                // &#x27;
                *output.add(out_pos) = b'&';
                *output.add(out_pos + 1) = b'#';
                *output.add(out_pos + 2) = b'x';
                *output.add(out_pos + 3) = b'2';
                *output.add(out_pos + 4) = b'7';
                *output.add(out_pos + 5) = b';';
                out_pos += 6;
            },
            _ => {
                *output.add(out_pos) = ch;
                out_pos += 1;
            }
        }
    }
    *output.add(out_pos) = 0; // Null terminator

    // Create result value
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        (*result).data.str_val = output;
    }
    result
}

// HTML unescape a string (basic implementation)
unsafe fn html_unescape_string(input: *const u8) -> *mut TauraroValue {
    if input.is_null() {
        return create_string_value("");
    }

    let len = strlen(input);
    if len == 0 {
        return create_string_value("");
    }

    // Allocate buffer
    let output = malloc(len + 1);
    if output.is_null() {
        return create_string_value("");
    }

    let mut in_pos = 0;
    let mut out_pos = 0;
    while in_pos < len {
        let ch = *input.add(in_pos);
        if ch == b'&' {
            // Check for entities
            if in_pos + 3 < len &&
               *input.add(in_pos + 1) == b'l' &&
               *input.add(in_pos + 2) == b't' &&
               *input.add(in_pos + 3) == b';' {
                *output.add(out_pos) = b'<';
                in_pos += 4;
                out_pos += 1;
            } else if in_pos + 3 < len &&
                      *input.add(in_pos + 1) == b'g' &&
                      *input.add(in_pos + 2) == b't' &&
                      *input.add(in_pos + 3) == b';' {
                *output.add(out_pos) = b'>';
                in_pos += 4;
                out_pos += 1;
            } else if in_pos + 4 < len &&
                      *input.add(in_pos + 1) == b'a' &&
                      *input.add(in_pos + 2) == b'm' &&
                      *input.add(in_pos + 3) == b'p' &&
                      *input.add(in_pos + 4) == b';' {
                *output.add(out_pos) = b'&';
                in_pos += 5;
                out_pos += 1;
            } else if in_pos + 5 < len &&
                      *input.add(in_pos + 1) == b'q' &&
                      *input.add(in_pos + 2) == b'u' &&
                      *input.add(in_pos + 3) == b'o' &&
                      *input.add(in_pos + 4) == b't' &&
                      *input.add(in_pos + 5) == b';' {
                *output.add(out_pos) = b'"';
                in_pos += 6;
                out_pos += 1;
            } else if in_pos + 5 < len &&
                      *input.add(in_pos + 1) == b'#' &&
                      *input.add(in_pos + 2) == b'x' &&
                      *input.add(in_pos + 3) == b'2' &&
                      *input.add(in_pos + 4) == b'7' &&
                      *input.add(in_pos + 5) == b';' {
                *output.add(out_pos) = b'\'';
                in_pos += 6;
                out_pos += 1;
            } else {
                *output.add(out_pos) = ch;
                in_pos += 1;
                out_pos += 1;
            }
        } else {
            *output.add(out_pos) = ch;
            in_pos += 1;
            out_pos += 1;
        }
    }
    *output.add(out_pos) = 0; // Null terminator

    // Create result value
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::String;
        (*result).data.str_val = output;
    }
    result
}

// Helper function to create a string value
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

// html.escape(s, quote=True) - Escape HTML special characters
#[no_mangle]
pub extern "C" fn tauraro_html_escape(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc < 1 || argc > 2 {
            return create_string_value("");
        }

        // Get string argument
        let string_val = *argv;
        if string_val.is_null() || (*string_val).value_type != TauraroType::String {
            return create_string_value("");
        }

        let input_str = (*string_val).data.str_val;

        // HTML escape the string
        html_escape_string(input_str)
    }
}

// html.unescape(s) - Unescape HTML entities
#[no_mangle]
pub extern "C" fn tauraro_html_unescape(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Validate arguments
        if argc != 1 {
            return create_string_value("");
        }

        // Get string argument
        let string_val = *argv;
        if string_val.is_null() || (*string_val).value_type != TauraroType::String {
            return create_string_value("");
        }

        let input_str = (*string_val).data.str_val;

        // HTML unescape the string
        html_unescape_string(input_str)
    }
}
