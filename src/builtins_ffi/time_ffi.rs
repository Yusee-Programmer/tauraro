//! FFI wrapper for time module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::c_int;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[repr(C)]
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
}

// Platform-specific time functions
#[cfg(target_os = "windows")]
extern "C" {
    fn GetTickCount64() -> u64;
}

#[cfg(unix)]
extern "C" {
    fn time(tloc: *mut i64) -> i64;
    fn usleep(usec: u32) -> c_int;
}

#[cfg(target_os = "windows")]
extern "C" {
    fn Sleep(dwMilliseconds: u32);
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

// time.time() - Returns current time in seconds since epoch
#[no_mangle]
pub extern "C" fn tauraro_time_time(_argc: c_int, _argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        #[cfg(unix)]
        {
            let mut t: i64 = 0;
            time(&mut t as *mut i64);
            create_float_value(t as f64)
        }

        #[cfg(target_os = "windows")]
        {
            let ticks = GetTickCount64();
            create_float_value((ticks as f64) / 1000.0)
        }

        #[cfg(not(any(unix, target_os = "windows")))]
        {
            // Fallback for other platforms
            create_float_value(0.0)
        }
    }
}

// time.sleep(seconds) - Sleep for given seconds
#[no_mangle]
pub extern "C" fn tauraro_time_sleep(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }

        let arg = *argv.offset(0);
        let seconds = match (*arg).value_type {
            TauraroType::Float => (*arg).data.float_val,
            TauraroType::Int => (*arg).data.int_val as f64,
            _ => return create_none_value(),
        };

        // Ensure non-negative sleep time
        if seconds < 0.0 {
            return create_none_value();
        }

        #[cfg(unix)]
        {
            usleep((seconds * 1_000_000.0) as u32);
        }

        #[cfg(target_os = "windows")]
        {
            Sleep((seconds * 1000.0) as u32);
        }

        #[cfg(not(any(unix, target_os = "windows")))]
        {
            // Fallback for other platforms - busy wait
            let start = tauraro_time_time(0, core::ptr::null_mut());
            loop {
                let current = tauraro_time_time(0, core::ptr::null_mut());
                if !start.is_null() && !current.is_null() {
                    let elapsed = (*current).data.float_val - (*start).data.float_val;
                    if elapsed >= seconds {
                        break;
                    }
                }
            }
        }

        create_none_value()
    }
}

// time.perf_counter() - High-resolution performance counter
#[no_mangle]
pub extern "C" fn tauraro_time_perf_counter(_argc: c_int, _argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would use high-resolution timers
        // For now, we'll use the same implementation as time.time()
        tauraro_time_time(0, core::ptr::null_mut())
    }
}

// time.process_time() - Process time
#[no_mangle]
pub extern "C" fn tauraro_time_process_time(_argc: c_int, _argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would measure process CPU time
        // For now, we'll use the same implementation as time.time()
        tauraro_time_time(0, core::ptr::null_mut())
    }
}

// time.monotonic() - Monotonic clock
#[no_mangle]
pub extern "C" fn tauraro_time_monotonic(_argc: c_int, _argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would use monotonic clock
        // For now, we'll use the same implementation as time.time()
        tauraro_time_time(0, core::ptr::null_mut())
    }
}

// time.strftime(format, timestamp) - Format time
#[no_mangle]
pub extern "C" fn tauraro_time_strftime(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() || (*argv).is_null() {
            return create_string_value("");
        }
        
        // In a real implementation, we would format the time
        // For now, we'll return a placeholder string
        create_string_value("Thu Jan  1 00:00:00 1970")
    }
}

// time.ctime(timestamp) - Convert time to string
#[no_mangle]
pub extern "C" fn tauraro_time_ctime(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would convert time to string
        // For now, we'll return a placeholder string
        create_string_value("Thu Jan  1 00:00:00 1970")
    }
}

// time.gmtime(timestamp) - Convert to UTC time struct
#[no_mangle]
pub extern "C" fn tauraro_time_gmtime(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a time struct
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}

// time.localtime(timestamp) - Convert to local time struct
#[no_mangle]
pub extern "C" fn tauraro_time_localtime(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would create a time struct
        // For now, we'll return None as a placeholder
        create_none_value()
    }
}
