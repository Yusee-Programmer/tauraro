/// SYS module - provides system-specific parameters and functions
/// Similar to Python's sys module

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::env;
use std::sync::{Arc, Mutex};
use std::cell::RefCell;
use std::path::PathBuf;

// Thread-local storage for additional module system paths
thread_local! {
    static MODULE_SYSTEM_PATHS: RefCell<Vec<PathBuf>> = RefCell::new(Vec::new());
}

/// Global sys.path storage for manipulation functions
static mut SYS_PATH: Option<Arc<Mutex<Vec<Value>>>> = None;

/// Create the sys module object with all its functions and attributes
pub fn create_sys_module() -> Value {
    let mut namespace = HashMap::new();
    
    // System information
    namespace.insert("version".to_string(), Value::Str(format!("Tauraro {}", env!("CARGO_PKG_VERSION"))));
    namespace.insert("version_info".to_string(), create_version_info());
    namespace.insert("platform".to_string(), Value::Str(get_platform()));
    
    // Path information - use a special function that returns current sys.path
    namespace.insert("path".to_string(), get_current_sys_path());
    namespace.insert("executable".to_string(), get_executable());
    
    // Standard streams (simplified)
    namespace.insert("stdin".to_string(), Value::Str("<stdin>".to_string()));
    namespace.insert("stdout".to_string(), Value::Str("<stdout>".to_string()));
    namespace.insert("stderr".to_string(), Value::Str("<stderr>".to_string()));
    
    // System limits and configuration
    namespace.insert("maxsize".to_string(), Value::Int(i64::MAX));
    namespace.insert("byteorder".to_string(), Value::Str(if cfg!(target_endian = "little") { "little" } else { "big" }.to_string()));
    
    // Functions
    namespace.insert("exit".to_string(), Value::NativeFunction(sys_exit));
    namespace.insert("getrefcount".to_string(), Value::NativeFunction(sys_getrefcount));
    namespace.insert("getsizeof".to_string(), Value::NativeFunction(sys_getsizeof));
    namespace.insert("intern".to_string(), Value::NativeFunction(sys_intern));
    
    // Path manipulation functions
    namespace.insert("path_append".to_string(), Value::NativeFunction(sys_path_append));
    namespace.insert("path_insert".to_string(), Value::NativeFunction(sys_path_insert));
    namespace.insert("path_remove".to_string(), Value::NativeFunction(sys_path_remove));
    
    // Module information
    namespace.insert("modules".to_string(), create_modules_dict());
    namespace.insert("builtin_module_names".to_string(), create_builtin_modules());
    
    // Interpreter information
    namespace.insert("copyright".to_string(), Value::Str("Copyright (c) 2024 Tauraro Project".to_string()));
    namespace.insert("api_version".to_string(), Value::Int(1));
    namespace.insert("dont_write_bytecode".to_string(), Value::Bool(false));
    
    // Command line arguments (empty for now)
    namespace.insert("argv".to_string(), Value::List(vec![Value::Str("tauraro".to_string())]));
    
    Value::Module("sys".to_string(), namespace)
}

/// Create version_info tuple
fn create_version_info() -> Value {
    let version = env!("CARGO_PKG_VERSION");
    let parts: Vec<&str> = version.split('.').collect();
    
    let major = parts.get(0).unwrap_or(&"0").parse::<i64>().unwrap_or(0);
    let minor = parts.get(1).unwrap_or(&"0").parse::<i64>().unwrap_or(0);
    let micro = parts.get(2).unwrap_or(&"0").parse::<i64>().unwrap_or(0);
    
    Value::Tuple(vec![
        Value::Int(major),
        Value::Int(minor),
        Value::Int(micro),
        Value::Str("final".to_string()),
        Value::Int(0),
    ])
}

/// Get platform string
fn get_platform() -> String {
    if cfg!(target_os = "windows") {
        "win32".to_string()
    } else if cfg!(target_os = "macos") {
        "darwin".to_string()
    } else if cfg!(target_os = "linux") {
        "linux".to_string()
    } else {
        "unknown".to_string()
    }
}

/// Get current sys.path (returns a copy of the shared storage)
pub fn get_current_sys_path() -> Value {
    unsafe {
        if let Some(ref sys_path) = SYS_PATH {
            if let Ok(path_list) = sys_path.lock() {
                return Value::List(path_list.clone());
            }
        }
    }
    
    // Fallback: create initial sys.path if not initialized
    create_sys_path()
}

/// Create sys.path list with cross-platform default paths
fn create_sys_path() -> Value {
    let mut path_list = Vec::new();
    
    // Add current directory (always first)
    path_list.push(Value::Str(".".to_string()));
    
    // Add built-in package directories
    path_list.push(Value::Str("tauraro_packages".to_string()));
    path_list.push(Value::Str("tauraro_packages/externals".to_string()));
    
    // Add platform-specific standard library paths
    if cfg!(target_os = "windows") {
        path_list.push(Value::Str("C:\\tauraro\\lib".to_string()));
        path_list.push(Value::Str("C:\\tauraro\\lib\\site-packages".to_string()));
    } else {
        path_list.push(Value::Str("/usr/local/lib/tauraro".to_string()));
        path_list.push(Value::Str("/usr/lib/tauraro".to_string()));
    }
    
    // Add paths from TAURARO_PATH environment variable
    if let Ok(tauraro_path) = env::var("TAURARO_PATH") {
        for path in tauraro_path.split(if cfg!(target_os = "windows") { ';' } else { ':' }) {
            let trimmed = path.trim();
            if !trimmed.is_empty() {
                path_list.push(Value::Str(trimmed.to_string()));
            }
        }
    }
    
    // Initialize global sys.path storage
    unsafe {
        SYS_PATH = Some(Arc::new(Mutex::new(path_list.clone())));
    }
    
    Value::List(path_list)
}

/// Get executable path
fn get_executable() -> Value {
    match env::current_exe() {
        Ok(path) => Value::Str(path.to_string_lossy().to_string()),
        Err(_) => Value::Str("tauraro".to_string()),
    }
}

/// Create modules dictionary (simplified)
fn create_modules_dict() -> Value {
    let mut modules = HashMap::new();
    
    // Add built-in modules
    modules.insert("sys".to_string(), Value::Str("<module 'sys' (built-in)>".to_string()));
    modules.insert("os".to_string(), crate::modules::os::create_os_module());
    modules.insert("thread".to_string(), crate::modules::thread::create_thread_module());
    modules.insert("builtins".to_string(), Value::Str("<module 'builtins' (built-in)>".to_string()));
    
    Value::Dict(modules)
}

/// Create builtin module names tuple
fn create_builtin_modules() -> Value {
    Value::Tuple(vec![
        Value::Str("sys".to_string()),
        Value::Str("os".to_string()),
        Value::Str("thread".to_string()),
        Value::Str("builtins".to_string()),
    ])
}

// SYS Functions Implementation

pub fn sys_exit(args: Vec<Value>) -> Result<Value> {
    let exit_code = if args.is_empty() {
        0
    } else {
        match &args[0] {
            Value::Int(i) => *i as i32,
            Value::None => 0,
            _ => return Err(anyhow::anyhow!("exit() argument must be an integer or None")),
        }
    };
    
    std::process::exit(exit_code);
}

pub fn sys_getrefcount(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("getrefcount() takes exactly one argument"));
    }
    
    // In Rust, we don't have reference counting like Python
    // Return a placeholder value
    Ok(Value::Int(1))
}

pub fn sys_getsizeof(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("getsizeof() takes exactly one argument"));
    }
    
    let size = match &args[0] {
        Value::None => 8,
        Value::Bool(_) => 1,
        Value::Int(_) => 8,
        Value::Float(_) => 8,
        Value::Str(s) => s.len() as i64 + 24, // String overhead
        Value::List(l) => (l.len() as i64 * 8) + 24, // List overhead
        Value::Dict(d) => (d.len() as i64 * 16) + 32, // Dict overhead
        Value::Tuple(t) => (t.len() as i64 * 8) + 16, // Tuple overhead
        _ => 64, // Default size for complex objects
    };
    
    Ok(Value::Int(size))
}

pub fn sys_intern(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("intern() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Str(s) => Ok(Value::Str(s.clone())), // In Rust, strings are already interned in a sense
        _ => Err(anyhow::anyhow!("intern() argument must be a string")),
    }
}

// Additional sys module utilities

/// Get system information
pub fn get_system_info() -> HashMap<String, Value> {
    let mut info = HashMap::new();
    
    info.insert("arch".to_string(), Value::Str(env::consts::ARCH.to_string()));
    info.insert("os".to_string(), Value::Str(env::consts::OS.to_string()));
    info.insert("family".to_string(), Value::Str(env::consts::FAMILY.to_string()));
    info.insert("dll_extension".to_string(), Value::Str(env::consts::DLL_EXTENSION.to_string()));
    info.insert("dll_prefix".to_string(), Value::Str(env::consts::DLL_PREFIX.to_string()));
    info.insert("dll_suffix".to_string(), Value::Str(env::consts::DLL_SUFFIX.to_string()));
    info.insert("exe_extension".to_string(), Value::Str(env::consts::EXE_EXTENSION.to_string()));
    info.insert("exe_suffix".to_string(), Value::Str(env::consts::EXE_SUFFIX.to_string()));
    
    info
}

/// Set command line arguments
pub fn set_argv(args: Vec<String>) -> Value {
    let argv: Vec<Value> = args.into_iter().map(Value::Str).collect();
    Value::List(argv)
}

/// Add path to sys.path
pub fn add_to_path(path: String, sys_path: &mut Vec<Value>) {
    let path_value = Value::Str(path);
    if !sys_path.contains(&path_value) {
        sys_path.push(path_value);
    }
}

/// Get environment variables as a dictionary
pub fn get_environ() -> Value {
    let mut environ = HashMap::new();
    for (key, value) in env::vars() {
        environ.insert(key, Value::Str(value));
    }
    Value::Dict(environ)
}

/// Check if running in interactive mode
pub fn is_interactive() -> bool {
    // For now, assume non-interactive
    // This could be enhanced to detect actual interactive mode
    false
}

/// Get the current recursion limit (conceptual)
pub fn get_recursion_limit() -> i64 {
    1000 // Default recursion limit
}

/// sys.path_append function - adds a path to sys.path and module system
fn sys_path_append(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path_append() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s.clone(),
        _ => return Err(anyhow::anyhow!("path_append() argument must be a string")),
    };
    
    unsafe {
        if let Some(ref sys_path) = SYS_PATH {
            if let Ok(mut path_list) = sys_path.lock() {
                path_list.push(Value::Str(path_str.clone()));
            }
        }
    }
    
    // Also notify the module system about the new path
    use std::path::PathBuf;
    MODULE_SYSTEM_PATHS.with(|paths| {
        if let Ok(mut paths) = paths.try_borrow_mut() {
            paths.push(PathBuf::from(path_str));
        }
    });
    
    Ok(Value::None)
}

/// sys.path.insert(index, path) - Insert a directory at the given index in sys.path
pub fn sys_path_insert(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("path_insert() takes exactly two arguments"));
    }
    
    let index = match &args[0] {
        Value::Int(i) => *i as usize,
        _ => return Err(anyhow::anyhow!("path_insert() first argument must be an integer")),
    };
    
    match &args[1] {
        Value::Str(path) => {
            unsafe {
                if let Some(ref sys_path) = SYS_PATH {
                    if let Ok(mut path_list) = sys_path.lock() {
                        if index <= path_list.len() {
                            path_list.insert(index, Value::Str(path.clone()));
                        } else {
                            path_list.push(Value::Str(path.clone()));
                        }
                    }
                }
            }
            
            // Also notify the module system about the new path
            use std::path::PathBuf;
            MODULE_SYSTEM_PATHS.with(|paths| {
                if let Ok(mut paths) = paths.try_borrow_mut() {
                    if index <= paths.len() {
                        paths.insert(index, PathBuf::from(path.clone()));
                    } else {
                        paths.push(PathBuf::from(path.clone()));
                    }
                }
            });
            
            Ok(Value::None)
        }
        _ => Err(anyhow::anyhow!("path_insert() second argument must be a string")),
    }
}

/// sys.path.remove(path) - Remove the first occurrence of path from sys.path
pub fn sys_path_remove(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path_remove() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Str(path) => {
            unsafe {
                if let Some(ref sys_path) = SYS_PATH {
                    if let Ok(mut path_list) = sys_path.lock() {
                        if let Some(pos) = path_list.iter().position(|x| {
                            if let Value::Str(s) = x {
                                s == path
                            } else {
                                false
                            }
                        }) {
                            path_list.remove(pos);
                        }
                    }
                }
            }
            
            // Also notify the module system about the path removal
            use std::path::PathBuf;
            MODULE_SYSTEM_PATHS.with(|paths| {
                if let Ok(mut paths) = paths.try_borrow_mut() {
                    paths.retain(|p| p != &PathBuf::from(path.clone()));
                }
            });
            
            Ok(Value::None)
        }
        _ => Err(anyhow::anyhow!("path_remove() argument must be a string")),
    }
}
