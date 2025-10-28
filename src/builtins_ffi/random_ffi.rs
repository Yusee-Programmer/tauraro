//! FFI wrapper for random module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

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
    fn rand() -> c_int;
    fn srand(seed: u32);
}

// Simple LCG for better randomness
static mut SEED: u64 = 123456789;

unsafe fn next_random() -> u32 {
    SEED = SEED.wrapping_mul(1103515245).wrapping_add(12345);
    (SEED / 65536) as u32 % 32768
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

// Helper function to create an integer value
unsafe fn create_int_value(value: i64) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Int;
        (*result).data.int_val = value;
    }
    result
}

// Helper function to create a None value
unsafe fn create_none_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::None;
    }
    result
}

// Helper function to check if a value is an integer
unsafe fn is_int_value(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    (*val).value_type == TauraroType::Int
}

// Helper function to get integer value
unsafe fn get_int_value(val: *mut TauraroValue) -> i64 {
    if val.is_null() || (*val).value_type != TauraroType::Int {
        return 0;
    }
    (*val).data.int_val
}

// Helper function to check if a value is a float
unsafe fn is_float_value(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    (*val).value_type == TauraroType::Float
}

// Helper function to get float value
unsafe fn get_float_value(val: *mut TauraroValue) -> f64 {
    if val.is_null() {
        return 0.0;
    }
    match (*val).value_type {
        TauraroType::Int => (*val).data.int_val as f64,
        TauraroType::Float => (*val).data.float_val,
        _ => 0.0,
    }
}

// Simple floor implementation for no_std
fn floor_f64(x: f64) -> f64 {
    let truncated = x as i64 as f64;
    if x < 0.0 && truncated != x {
        truncated - 1.0
    } else {
        truncated
    }
}

// random.random() - Returns random float [0.0, 1.0)
#[no_mangle]
pub extern "C" fn tauraro_random_random(_argc: c_int, _argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        let r = next_random();
        create_float_value((r as f64) / 32768.0)
    }
}

// random.randint(a, b) - Returns random integer in [a, b]
#[no_mangle]
pub extern "C" fn tauraro_random_randint(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_int_value(arg1) || !is_int_value(arg2) {
            return create_none_value();
        }

        let a = get_int_value(arg1);
        let b = get_int_value(arg2);

        if a > b {
            return create_none_value();
        }

        let range = (b - a + 1) as u32;
        let r = next_random() % range;
        create_int_value(a + (r as i64))
    }
}

// random.uniform(a, b) - Returns random float in [a, b]
#[no_mangle]
pub extern "C" fn tauraro_random_uniform(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        let a = get_float_value(arg1);
        let b = get_float_value(arg2);

        if a > b {
            return create_none_value();
        }

        let r = next_random();
        let random_val = (r as f64) / 32768.0;
        create_float_value(a + random_val * (b - a))
    }
}

// random.randrange(start, stop, step) - Returns random integer in range
#[no_mangle]
pub extern "C" fn tauraro_random_randrange(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argc > 3 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let start = if argc >= 1 {
            get_int_value(*argv.offset(0))
        } else {
            0
        };
        
        let stop = if argc >= 2 {
            get_int_value(*argv.offset(1))
        } else {
            start
        };
        
        let step = if argc >= 3 {
            get_int_value(*argv.offset(2))
        } else {
            1
        };

        if step == 0 {
            return create_none_value();
        }

        if step > 0 && start >= stop {
            return create_none_value();
        }

        if step < 0 && start <= stop {
            return create_none_value();
        }

        let range_size = floor_f64((stop - start) as f64 / step as f64) as i64;
        if range_size <= 0 {
            return create_none_value();
        }

        let r = next_random() % (range_size as u32);
        create_int_value(start + (r as i64) * step)
    }
}

// random.seed(x) - Initialize random seed
#[no_mangle]
pub extern "C" fn tauraro_random_seed(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        if argc > 0 && !argv.is_null() && !(*argv).is_null() {
            let arg = *argv.offset(0);
            let seed_val = match (*arg).value_type {
                TauraroType::Int => (*arg).data.int_val as u64,
                TauraroType::Float => (*arg).data.float_val as u64,
                _ => 0,
            };
            SEED = seed_val;
        }

        create_none_value()
    }
}

// random.getrandbits(k) - Returns integer with k random bits
#[no_mangle]
pub extern "C" fn tauraro_random_getrandbits(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_int_value(arg) {
            return create_none_value();
        }

        let k = get_int_value(arg) as u32;
        if k == 0 {
            return create_int_value(0);
        }

        // Generate k random bits
        let mut result: u64 = 0;
        for i in 0..k.min(64) {
            if next_random() % 2 == 1 {
                result |= 1 << i;
            }
        }
        
        create_int_value(result as i64)
    }
}

// random.choice(seq) - Choose random element from sequence
#[no_mangle]
pub extern "C" fn tauraro_random_choice(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        // In a real implementation, we would choose from the sequence
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}

// random.shuffle(seq) - Shuffle sequence in place
#[no_mangle]
pub extern "C" fn tauraro_random_shuffle(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        // In a real implementation, we would shuffle the sequence
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}
