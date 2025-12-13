//! FFI wrapper for uuid module - UUID generation
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

// Simple random generator for UUID
static mut UUID_SEED: u64 = 12345;

unsafe fn uuid_random_byte() -> u8 {
    UUID_SEED = UUID_SEED.wrapping_mul(1103515245).wrapping_add(12345);
    ((UUID_SEED >> 16) & 0xFF) as u8
}

// Generate UUID4 (random UUID)
unsafe fn generate_uuid4() -> *mut TauraroValue {
    // Initialize seed from time if needed
    if UUID_SEED == 12345 {
        let current_time = time(core::ptr::null_mut()) as u64;
        UUID_SEED = current_time;
    }

    // UUID format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    // Where x is random hex, 4 is version, y is variant (8, 9, a, or b)
    let output = malloc(37); // 36 chars + null terminator
    if output.is_null() {
        return create_string_value("");
    }

    let hex_chars = b"0123456789abcdef";
    let mut pos = 0;

    // Generate 16 random bytes
    let mut bytes = [0u8; 16];
    for i in 0..16 {
        bytes[i] = uuid_random_byte();
    }

    // Set version (4) and variant (8-b) bits
    bytes[6] = (bytes[6] & 0x0F) | 0x40; // Version 4
    bytes[8] = (bytes[8] & 0x3F) | 0x80; // Variant 10xx

    // Format as UUID string
    for i in 0..16 {
        if i == 4 || i == 6 || i == 8 || i == 10 {
            *output.add(pos) = b'-';
            pos += 1;
        }
        *output.add(pos) = hex_chars[((bytes[i] >> 4) & 0xF) as usize];
        *output.add(pos + 1) = hex_chars[(bytes[i] & 0xF) as usize];
        pos += 2;
    }
    *output.add(36) = 0; // Null terminator

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

// Helper to create object value
unsafe fn create_object_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Object;
    }
    result
}

// uuid.uuid4() - Generate random UUID
#[no_mangle]
pub extern "C" fn tauraro_uuid_uuid4(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // UUID4 takes no arguments
        if argc > 0 {
            return create_object_value();
        }

        generate_uuid4()
    }
}

// uuid.uuid1() - Generate time-based UUID (simplified as uuid4)
#[no_mangle]
pub extern "C" fn tauraro_uuid_uuid1(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // For simplicity, we'll generate a UUID4
        // A real UUID1 would use timestamp and MAC address
        generate_uuid4()
    }
}

// uuid.UUID(hex) - Create UUID from hex string (returns object)
#[no_mangle]
pub extern "C" fn tauraro_uuid_uuid_new(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc < 1 || argv.is_null() {
            return create_object_value();
        }

        let arg = *argv;
        if arg.is_null() || (*arg).value_type != TauraroType::String {
            return create_object_value();
        }

        // In a real implementation, we would parse and validate the UUID
        // For now, return the string as-is wrapped in an object
        create_object_value()
    }
}
