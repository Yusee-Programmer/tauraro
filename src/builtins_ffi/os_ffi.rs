//! FFI wrapper for os module - exports C-compatible functions
//! Uses #![no_std] for minimal dependencies and easy C linking

#![no_std]

use core::ffi::{c_int, c_char};
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// Wrapper type for static const pointers to make them Sync
#[repr(transparent)]
struct ConstPtr(*const u8);
unsafe impl Sync for ConstPtr {}

impl ConstPtr {
    const fn new(ptr: *const u8) -> Self {
        ConstPtr(ptr)
    }
    
    pub const fn as_ptr(&self) -> *const u8 {
        self.0
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
    fn free(ptr: *mut u8);
    fn strlen(s: *const u8) -> usize;
    fn strcpy(dest: *mut u8, src: *const u8) -> *mut u8;
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

// Helper function to check if a value is a string
unsafe fn is_string_value(val: *mut TauraroValue) -> bool {
    if val.is_null() {
        return false;
    }
    (*val).value_type == TauraroType::String
}

// Helper function to get string content
unsafe fn get_string_content(val: *mut TauraroValue) -> *const u8 {
    if val.is_null() || (*val).value_type != TauraroType::String {
        return core::ptr::null();
    }
    (*val).data.str_val
}

// Helper function to create a None value
unsafe fn create_none_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::None;
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

// Helper function to create a list value
unsafe fn create_list_value() -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::List;
        // In a real implementation, we would initialize the list properly
    }
    result
}

// Helper function to create a boolean value
unsafe fn create_bool_value(value: bool) -> *mut TauraroValue {
    let result = tauraro_value_new();
    if !result.is_null() {
        (*result).value_type = TauraroType::Bool;
        (*result).data.bool_val = value;
    }
    result
}

// Helper function to check if value is valid
unsafe fn is_valid_value(val: *mut TauraroValue) -> bool {
    !val.is_null()
}

// Helper function to check if value is an integer
unsafe fn is_int_value(val: *mut TauraroValue) -> bool {
    !val.is_null() && (*val).value_type == TauraroType::Int
}

// os.getcwd() - Get current working directory
#[no_mangle]
pub extern "C" fn tauraro_os_getcwd(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would call the OS-specific function
        // For now, we'll return a platform-appropriate placeholder string
        #[cfg(target_os = "windows")]
        {
            create_string_value("C:\\Users\\Placeholder")
        }
        #[cfg(not(target_os = "windows"))]
        {
            create_string_value("/home/placeholder")
        }
    }
}

// os.listdir(path) - List directory contents
#[no_mangle]
pub extern "C" fn tauraro_os_listdir(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc > 1 {
            return create_none_value();
        }
        
        // In a real implementation, we would read the directory
        // For now, we'll return an empty list
        create_list_value()
    }
}

// os.chdir(path) - Change current directory
#[no_mangle]
pub extern "C" fn tauraro_os_chdir(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would change the directory
        // For now, we'll return None
        create_none_value()
    }
}

// os.mkdir(path) - Create directory
#[no_mangle]
pub extern "C" fn tauraro_os_mkdir(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would create the directory
        // For now, we'll return None
        create_none_value()
    }
}

// os.makedirs(path) - Create directories recursively
#[no_mangle]
pub extern "C" fn tauraro_os_makedirs(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would create the directories
        // For now, we'll return None
        create_none_value()
    }
}

// os.rmdir(path) - Remove directory
#[no_mangle]
pub extern "C" fn tauraro_os_rmdir(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would remove the directory
        // For now, we'll return None
        create_none_value()
    }
}

// os.remove(path) - Remove file
#[no_mangle]
pub extern "C" fn tauraro_os_remove(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would remove the file
        // For now, we'll return None
        create_none_value()
    }
}

// os.rename(src, dst) - Rename file or directory
#[no_mangle]
pub extern "C" fn tauraro_os_rename(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_string_value(arg1) || !is_string_value(arg2) {
            return create_none_value();
        }
        
        // In a real implementation, we would rename the file or directory
        // For now, we'll return None
        create_none_value()
    }
}

// os.stat(path) - Get file status
#[no_mangle]
pub extern "C" fn tauraro_os_stat(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would get file status
        // For now, we'll return None
        create_none_value()
    }
}

// os.getpid() - Get process ID
#[no_mangle]
pub extern "C" fn tauraro_os_getpid(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would get the process ID
        // For now, we'll return 0
        create_int_value(0)
    }
}

// os.getppid() - Get parent process ID
#[no_mangle]
pub extern "C" fn tauraro_os_getppid(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // In a real implementation, we would get the parent process ID
        // For now, we'll return 0
        create_int_value(0)
    }
}

// os.system(command) - Execute system command
#[no_mangle]
pub extern "C" fn tauraro_os_system(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_int_value(-1);
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_int_value(-1);
        }
        
        // In a real implementation, we would execute the system command
        // For now, we'll return 0 (success)
        create_int_value(0)
    }
}

// os.getenv(key, default=None) - Get environment variable
#[no_mangle]
pub extern "C" fn tauraro_os_getenv(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argc > 2 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would get the environment variable
        // For now, we'll return None or the default value
        if argc == 2 {
            let default_arg = *argv.offset(1);
            return default_arg;
        }
        create_none_value()
    }
}

// os.putenv(key, value) - Set environment variable
#[no_mangle]
pub extern "C" fn tauraro_os_putenv(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_string_value(arg1) || !is_string_value(arg2) {
            return create_none_value();
        }
        
        // In a real implementation, we would set the environment variable
        // For now, we'll return None
        create_none_value()
    }
}

// os.access(path, mode) - Test for access to a path
#[no_mangle]
pub extern "C" fn tauraro_os_access(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_bool_value(false);
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_string_value(arg1) || !is_int_value(arg2) {
            return create_bool_value(false);
        }
        
        // In a real implementation, we would test for access to the path
        // For now, we'll return true
        create_bool_value(true)
    }
}

// os.chmod(path, mode) - Change file mode
#[no_mangle]
pub extern "C" fn tauraro_os_chmod(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 2 || argv.is_null() || (*argv).is_null() || (*argv.offset(1)).is_null() {
            return create_none_value();
        }
        
        let arg1 = *argv.offset(0);
        let arg2 = *argv.offset(1);
        if !is_string_value(arg1) || !is_int_value(arg2) {
            return create_none_value();
        }
        
        // In a real implementation, we would change the file mode
        // For now, we'll return None
        create_none_value()
    }
}

// os.path.join(path1, path2, ...) - Join path components
#[no_mangle]
pub extern "C" fn tauraro_path_join(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc < 1 || argv.is_null() {
            return create_none_value();
        }
        
        // In a real implementation, we would join the path components
        // For now, we'll return a placeholder string
        create_string_value("path/joined")
    }
}

// os.path.split(path) - Split path into directory and basename
#[no_mangle]
pub extern "C" fn tauraro_path_split(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would split the path
        // For now, we'll return a tuple with placeholder strings
        let result = tauraro_value_new();
        if !result.is_null() {
            (*result).value_type = TauraroType::Tuple;
            // In a real implementation, we would create a proper tuple
        }
        result
    }
}

// os.path.dirname(path) - Return directory name of path
#[no_mangle]
pub extern "C" fn tauraro_path_dirname(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would return the directory name
        // For now, we'll return a placeholder string
        create_string_value("dirname")
    }
}

// os.path.basename(path) - Return basename of path
#[no_mangle]
pub extern "C" fn tauraro_path_basename(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would return the basename
        // For now, we'll return a placeholder string
        create_string_value("basename")
    }
}

// os.path.exists(path) - Check if path exists
#[no_mangle]
pub extern "C" fn tauraro_path_exists(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_bool_value(false);
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_bool_value(false);
        }
        
        // In a real implementation, we would check if the path exists
        // For now, we'll return true
        create_bool_value(true)
    }
}

// os.path.isfile(path) - Check if path is a file
#[no_mangle]
pub extern "C" fn tauraro_path_isfile(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_bool_value(false);
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_bool_value(false);
        }
        
        // In a real implementation, we would check if the path is a file
        // For now, we'll return true
        create_bool_value(true)
    }
}

// os.path.isdir(path) - Check if path is a directory
#[no_mangle]
pub extern "C" fn tauraro_path_isdir(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_bool_value(false);
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_bool_value(false);
        }
        
        // In a real implementation, we would check if the path is a directory
        // For now, we'll return true
        create_bool_value(true)
    }
}

// os.path.abspath(path) - Return absolute path
#[no_mangle]
pub extern "C" fn tauraro_path_abspath(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would return the absolute path
        // For now, we'll return a placeholder string
        create_string_value("/absolute/path")
    }
}

// os.path.realpath(path) - Return real path
#[no_mangle]
pub extern "C" fn tauraro_path_realpath(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_none_value();
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_none_value();
        }
        
        // In a real implementation, we would return the real path
        // For now, we'll return a placeholder string
        create_string_value("/real/path")
    }
}

// os.path.getsize(path) - Return size of file
#[no_mangle]
pub extern "C" fn tauraro_path_getsize(argc: c_int, argv: *mut *mut TauraroValue) -> *mut TauraroValue {
    unsafe {
        // Check arguments
        if argc != 1 || argv.is_null() || (*argv).is_null() {
            return create_int_value(0);
        }
        
        let arg = *argv.offset(0);
        if !is_string_value(arg) {
            return create_int_value(0);
        }
        
        // In a real implementation, we would return the size of the file
        // For now, we'll return 0
        create_int_value(0)
    }
}

// Constants
// os.name - Platform name
#[no_mangle]
pub static tauraro_os_name: ConstPtr = ConstPtr::new({
    #[cfg(target_os = "windows")]
    { b"nt\0".as_ptr() }
    #[cfg(target_os = "linux")]
    { b"posix\0".as_ptr() }
    #[cfg(target_os = "macos")]
    { b"posix\0".as_ptr() }
    #[cfg(not(any(target_os = "windows", target_os = "linux", target_os = "macos")))]
    { b"unknown\0".as_ptr() }
});

// os.sep - Path separator
#[no_mangle]
pub static tauraro_os_sep: ConstPtr = ConstPtr::new({
    #[cfg(target_os = "windows")]
    { b"\\\0".as_ptr() }
    #[cfg(not(target_os = "windows"))]
    { b"/\0".as_ptr() }
});

// os.pathsep - Path list separator
#[no_mangle]
pub static tauraro_os_pathsep: ConstPtr = ConstPtr::new({
    #[cfg(target_os = "windows")]
    { b";\0".as_ptr() }
    #[cfg(not(target_os = "windows"))]
    { b":\0".as_ptr() }
});

// os.linesep - Line separator
#[no_mangle]
pub static tauraro_os_linesep: ConstPtr = ConstPtr::new({
    #[cfg(target_os = "windows")]
    { b"\r\n\0".as_ptr() }
    #[cfg(not(target_os = "windows"))]
    { b"\n\0".as_ptr() }
});

// os.F_OK - File exists
#[no_mangle]
pub static tauraro_os_F_OK: i64 = 0;

// os.R_OK - Readable
#[no_mangle]
pub static tauraro_os_R_OK: i64 = 4;

// os.W_OK - Writable
#[no_mangle]
pub static tauraro_os_W_OK: i64 = 2;

// os.X_OK - Executable
#[no_mangle]
pub static tauraro_os_X_OK: i64 = 1;