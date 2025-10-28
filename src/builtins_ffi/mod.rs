//! FFI wrappers for builtin modules
//! These modules are compiled to object files and linked with generated C code
//!
//! Each module uses #![no_std] for minimal dependencies and easy C linking

pub mod abc_ffi;
pub mod asyncio_ffi;
pub mod base64_ffi;
pub mod collections_ffi;
pub mod copy_ffi;
pub mod csv_ffi;
pub mod datetime_ffi;
pub mod exceptions_ffi;
pub mod functools_ffi;
pub mod gc_ffi;
pub mod hashlib_ffi;
pub mod httptools_ffi;
pub mod httpx_ffi;
pub mod io_ffi;
pub mod itertools_ffi;
pub mod json_ffi;
pub mod logging_ffi;
pub mod math_ffi;
pub mod memory_ffi;
pub mod os_ffi;
pub mod pickle_ffi;
pub mod random_ffi;
pub mod re_ffi;
pub mod socket_ffi;
pub mod sys_ffi;
pub mod threading_ffi;
pub mod time_ffi;
pub mod unittest_ffi;
pub mod urllib_ffi;
pub mod websockets_ffi;
