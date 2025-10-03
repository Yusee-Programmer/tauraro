/// OS module - provides operating system interface functionality
/// Similar to Python's os module

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
// Import HPList
use crate::modules::hplist::HPList;

/// Create the os module object with all its functions and attributes
pub fn create_os_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Environment variables
    namespace.insert("environ".to_string(), create_environ());
    
    // Path operations
    namespace.insert("getcwd".to_string(), Value::NativeFunction(os_getcwd));
    namespace.insert("chdir".to_string(), Value::NativeFunction(os_chdir));
    namespace.insert("listdir".to_string(), Value::NativeFunction(os_listdir));
    namespace.insert("mkdir".to_string(), Value::NativeFunction(os_mkdir));
    namespace.insert("makedirs".to_string(), Value::NativeFunction(os_makedirs));
    namespace.insert("rmdir".to_string(), Value::NativeFunction(os_rmdir));
    namespace.insert("remove".to_string(), Value::NativeFunction(os_remove));
    namespace.insert("rename".to_string(), Value::NativeFunction(os_rename));
    namespace.insert("stat".to_string(), Value::NativeFunction(os_stat));
    
    // Path utilities
    namespace.insert("path".to_string(), create_path_module());
    
    // Process operations
    namespace.insert("getpid".to_string(), Value::NativeFunction(os_getpid));
    namespace.insert("system".to_string(), Value::NativeFunction(os_system));
    
    // Environment functions
    namespace.insert("getenv".to_string(), Value::NativeFunction(os_getenv));
    namespace.insert("putenv".to_string(), Value::NativeFunction(os_putenv));
    
    // File operations
    namespace.insert("access".to_string(), Value::NativeFunction(os_access));
    namespace.insert("chmod".to_string(), Value::NativeFunction(os_chmod));
    
    // Constants
    namespace.insert("name".to_string(), Value::Str(get_os_name()));
    namespace.insert("sep".to_string(), Value::Str(std::path::MAIN_SEPARATOR.to_string()));
    namespace.insert("pathsep".to_string(), Value::Str(if cfg!(windows) { ";" } else { ":" }.to_string()));
    namespace.insert("linesep".to_string(), Value::Str(if cfg!(windows) { "\r\n" } else { "\n" }.to_string()));
    
    // Access mode constants
    namespace.insert("F_OK".to_string(), Value::Int(0)); // File exists
    namespace.insert("R_OK".to_string(), Value::Int(4)); // Readable
    namespace.insert("W_OK".to_string(), Value::Int(2)); // Writable
    namespace.insert("X_OK".to_string(), Value::Int(1)); // Executable
    
    Value::Module("os".to_string(), namespace)
}

/// Create environment variables dictionary
fn create_environ() -> Value {
    let mut environ = HashMap::new();
    for (key, value) in env::vars() {
        environ.insert(key, Value::Str(value));
    }
    Value::Dict(environ)
}

/// Create os.path submodule
fn create_path_module() -> Value {
    let mut namespace = HashMap::new();
    
    namespace.insert("join".to_string(), Value::NativeFunction(path_join));
    namespace.insert("split".to_string(), Value::NativeFunction(path_split));
    namespace.insert("dirname".to_string(), Value::NativeFunction(path_dirname));
    namespace.insert("basename".to_string(), Value::NativeFunction(path_basename));
    namespace.insert("exists".to_string(), Value::NativeFunction(path_exists));
    namespace.insert("isfile".to_string(), Value::NativeFunction(path_isfile));
    namespace.insert("isdir".to_string(), Value::NativeFunction(path_isdir));
    namespace.insert("abspath".to_string(), Value::NativeFunction(path_abspath));
    namespace.insert("realpath".to_string(), Value::NativeFunction(path_realpath));
    namespace.insert("getsize".to_string(), Value::NativeFunction(path_getsize));
    
    Value::Module("os.path".to_string(), namespace)
}

// OS Functions Implementation

pub fn os_getcwd(_args: Vec<Value>) -> Result<Value> {
    match env::current_dir() {
        Ok(path) => Ok(Value::Str(path.to_string_lossy().to_string())),
        Err(e) => Err(anyhow::anyhow!("Failed to get current directory: {}", e)),
    }
}

pub fn os_chdir(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("chdir() takes exactly one argument"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("chdir() argument must be a string")),
    };
    
    match env::set_current_dir(path) {
        Ok(()) => Ok(Value::None),
        Err(e) => Err(anyhow::anyhow!("Failed to change directory: {}", e)),
    }
}

pub fn os_listdir(args: Vec<Value>) -> Result<Value> {
    let path = if args.is_empty() {
        ".".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow::anyhow!("listdir() argument must be a string")),
        }
    };
    
    match fs::read_dir(&path) {
        Ok(entries) => {
            let mut result = Vec::new();
            for entry in entries {
                if let Ok(entry) = entry {
                    if let Some(name) = entry.file_name().to_str() {
                        result.push(Value::Str(name.to_string()));
                    }
                }
            }
            Ok(Value::List(HPList::from_values(result)))
        }
        Err(e) => Err(anyhow::anyhow!("Failed to list directory '{}': {}", path, e)),
    }
}

pub fn os_mkdir(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("mkdir() takes exactly one argument"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("mkdir() argument must be a string")),
    };
    
    match fs::create_dir(path) {
        Ok(()) => Ok(Value::None),
        Err(e) => Err(anyhow::anyhow!("Failed to create directory '{}': {}", path, e)),
    }
}

pub fn os_makedirs(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("makedirs() takes exactly one argument"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("makedirs() argument must be a string")),
    };
    
    match fs::create_dir_all(path) {
        Ok(()) => Ok(Value::None),
        Err(e) => Err(anyhow::anyhow!("Failed to create directories '{}': {}", path, e)),
    }
}

pub fn os_rmdir(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("rmdir() takes exactly one argument"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("rmdir() argument must be a string")),
    };
    
    match fs::remove_dir(path) {
        Ok(()) => Ok(Value::None),
        Err(e) => Err(anyhow::anyhow!("Failed to remove directory '{}': {}", path, e)),
    }
}

pub fn os_remove(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("remove() takes exactly one argument"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("remove() argument must be a string")),
    };
    
    match fs::remove_file(path) {
        Ok(()) => Ok(Value::None),
        Err(e) => Err(anyhow::anyhow!("Failed to remove file '{}': {}", path, e)),
    }
}

pub fn os_rename(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("rename() takes exactly two arguments"));
    }
    
    let src = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("rename() first argument must be a string")),
    };
    
    let dst = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("rename() second argument must be a string")),
    };
    
    match fs::rename(src, dst) {
        Ok(()) => Ok(Value::None),
        Err(e) => Err(anyhow::anyhow!("Failed to rename '{}' to '{}': {}", src, dst, e)),
    }
}

pub fn os_stat(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("stat() takes exactly one argument"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("stat() argument must be a string")),
    };
    
    match fs::metadata(path) {
        Ok(metadata) => {
            let mut stat_result = HashMap::new();
            stat_result.insert("st_size".to_string(), Value::Int(metadata.len() as i64));
            stat_result.insert("st_mode".to_string(), Value::Int(if metadata.is_dir() { 16877 } else { 33188 }));
            
            if let Ok(modified) = metadata.modified() {
                if let Ok(duration) = modified.duration_since(std::time::UNIX_EPOCH) {
                    stat_result.insert("st_mtime".to_string(), Value::Float(duration.as_secs_f64()));
                }
            }
            
            Ok(Value::Dict(stat_result))
        }
        Err(e) => Err(anyhow::anyhow!("Failed to stat '{}': {}", path, e)),
    }
}

pub fn os_getpid(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(std::process::id() as i64))
}

pub fn os_system(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("system() takes exactly one argument"));
    }
    
    let command = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("system() argument must be a string")),
    };
    
    let output = if cfg!(target_os = "windows") {
        Command::new("cmd").args(["/C", command]).output()
    } else {
        Command::new("sh").args(["-c", command]).output()
    };
    
    match output {
        Ok(output) => Ok(Value::Int(output.status.code().unwrap_or(-1) as i64)),
        Err(e) => Err(anyhow::anyhow!("Failed to execute command: {}", e)),
    }
}

pub fn os_getenv(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("getenv() takes 1 or 2 arguments"));
    }
    
    let key = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("getenv() first argument must be a string")),
    };
    
    let default = if args.len() == 2 {
        args[1].clone()
    } else {
        Value::None
    };
    
    match env::var(key) {
        Ok(value) => Ok(Value::Str(value)),
        Err(_) => Ok(default),
    }
}

pub fn os_putenv(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("putenv() takes exactly two arguments"));
    }
    
    let key = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("putenv() first argument must be a string")),
    };
    
    let value = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("putenv() second argument must be a string")),
    };
    
    env::set_var(key, value);
    Ok(Value::None)
}

pub fn os_access(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("access() takes exactly two arguments"));
    }
    
    let path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("access() first argument must be a string")),
    };
    
    let mode = match &args[1] {
        Value::Int(i) => *i,
        _ => return Err(anyhow::anyhow!("access() second argument must be an integer")),
    };
    
    let path_obj = Path::new(path);
    
    // F_OK (0) - file exists
    if mode == 0 {
        return Ok(Value::Bool(path_obj.exists()));
    }
    
    // For other modes, we'll do basic checks
    let exists = path_obj.exists();
    if !exists {
        return Ok(Value::Bool(false));
    }
    
    // R_OK (4) - readable (assume true if file exists)
    // W_OK (2) - writable (basic check)
    // X_OK (1) - executable (basic check)
    
    Ok(Value::Bool(true)) // Simplified implementation
}

pub fn os_chmod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("chmod() takes exactly two arguments"));
    }
    
    let _path = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("chmod() first argument must be a string")),
    };
    
    let _mode = match &args[1] {
        Value::Int(i) => *i,
        _ => return Err(anyhow::anyhow!("chmod() second argument must be an integer")),
    };
    
    // Platform-specific implementation would go here
    // For now, just return None (no-op)
    Ok(Value::None)
}

// Path module functions

pub fn path_join(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Str(String::new()));
    }
    
    let mut path = PathBuf::new();
    for arg in args {
        match arg {
            Value::Str(s) => path.push(s),
            _ => return Err(anyhow::anyhow!("path.join() arguments must be strings")),
        }
    }
    
    Ok(Value::Str(path.to_string_lossy().to_string()))
}

pub fn path_split(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.split() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.split() argument must be a string")),
    };
    
    let path = Path::new(path_str);
    let parent = path.parent().unwrap_or(Path::new("")).to_string_lossy().to_string();
    let filename = path.file_name().unwrap_or_default().to_string_lossy().to_string();
    
    Ok(Value::Tuple(vec![Value::Str(parent), Value::Str(filename)]))
}

pub fn path_dirname(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.dirname() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.dirname() argument must be a string")),
    };
    
    let path = Path::new(path_str);
    let parent = path.parent().unwrap_or(Path::new("")).to_string_lossy().to_string();
    
    Ok(Value::Str(parent))
}

pub fn path_basename(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.basename() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.basename() argument must be a string")),
    };
    
    let path = Path::new(path_str);
    let filename = path.file_name().unwrap_or_default().to_string_lossy().to_string();
    
    Ok(Value::Str(filename))
}

pub fn path_exists(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.exists() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.exists() argument must be a string")),
    };
    
    Ok(Value::Bool(Path::new(path_str).exists()))
}

pub fn path_isfile(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.isfile() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.isfile() argument must be a string")),
    };
    
    Ok(Value::Bool(Path::new(path_str).is_file()))
}

pub fn path_isdir(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.isdir() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.isdir() argument must be a string")),
    };
    
    Ok(Value::Bool(Path::new(path_str).is_dir()))
}

pub fn path_abspath(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.abspath() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.abspath() argument must be a string")),
    };
    
    match fs::canonicalize(path_str) {
        Ok(abs_path) => Ok(Value::Str(abs_path.to_string_lossy().to_string())),
        Err(_) => {
            // If canonicalize fails, try to make it absolute manually
            let current_dir = env::current_dir().unwrap_or_default();
            let abs_path = current_dir.join(path_str);
            Ok(Value::Str(abs_path.to_string_lossy().to_string()))
        }
    }
}

pub fn path_realpath(args: Vec<Value>) -> Result<Value> {
    // Same as abspath for now
    path_abspath(args)
}

pub fn path_getsize(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("path.getsize() takes exactly one argument"));
    }
    
    let path_str = match &args[0] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("path.getsize() argument must be a string")),
    };
    
    match fs::metadata(path_str) {
        Ok(metadata) => Ok(Value::Int(metadata.len() as i64)),
        Err(e) => Err(anyhow::anyhow!("Failed to get size of '{}': {}", path_str, e)),
    }
}

/// Get the operating system name
fn get_os_name() -> String {
    if cfg!(windows) {
        "nt".to_string()
    } else if cfg!(unix) {
        "posix".to_string()
    } else {
        "unknown".to_string()
    }
}
