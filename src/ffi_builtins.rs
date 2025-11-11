//! FFI Builtin Functions for Tauraro
//!
//! This module provides builtin functions for FFI operations that can be called from Tauraro code.

#[cfg(feature = "ffi")]
use crate::ffi::{FFIManager, FFIType, CallingConvention, parse_ffi_type};
use crate::value::Value;
use crate::modules::hplist::HPList;
use std::sync::{Arc, Mutex};
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use anyhow::{Result, anyhow};

#[cfg(feature = "ffi")]
lazy_static::lazy_static! {
    /// Global FFI manager shared across the runtime
    pub static ref GLOBAL_FFI_MANAGER: Arc<Mutex<FFIManager>> = Arc::new(Mutex::new(FFIManager::new()));
}

/// Load a dynamic library
///
/// Usage:
/// ```python
/// # Windows
/// load_library("kernel32.dll")
/// load_library("user32")
///
/// # Linux
/// load_library("libm.so")
/// load_library("m")  # auto-detects libm.so
///
/// # macOS
/// load_library("libSystem.dylib")
/// ```
pub fn load_library_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.is_empty() {
            return Err(anyhow!("load_library() requires at least 1 argument (library_name)"));
        }

        let library_name = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Library name must be a string")),
        };

        let mut manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        manager.load_library(&library_name)?;

        Ok(Value::None)
    }
}

/// Define a function from a loaded library
///
/// Usage:
/// ```python
/// # Define a function signature
/// define_function("kernel32.dll", "GetTickCount", "int32", [])
/// define_function("m", "sqrt", "double", ["double"])
/// define_function("m", "pow", "double", ["double", "double"])
/// ```
pub fn define_function_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.len() < 3 {
            return Err(anyhow!(
                "define_function() requires at least 3 arguments (library, function_name, return_type)"
            ));
        }

        let library_name = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Library name must be a string")),
        };

        let function_name = match &args[1] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Function name must be a string")),
        };

        let return_type_str = match &args[2] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Return type must be a string")),
        };

        let return_type = parse_ffi_type(&return_type_str)?;

        // Parse parameter types (optional 4th argument)
        let param_types = if args.len() > 3 {
            match &args[3] {
                Value::List(list) => {
                    let mut types = Vec::new();
                    for item in list {
                        match item {
                            Value::Str(s) => {
                                types.push(parse_ffi_type(s)?);
                            }
                            _ => return Err(anyhow!("Parameter types must be strings")),
                        }
                    }
                    types
                }
                _ => return Err(anyhow!("Parameter types must be a list")),
            }
        } else {
            Vec::new()
        };

        let mut manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        manager.define_function(
            &library_name,
            &function_name,
            return_type.clone(),
            param_types.clone(),
            None, // Use default C calling convention
        )?;

        // Create and return an ExternFunction value that can be called directly
        let signature = format!("{}({}) -> {}", 
            function_name, 
            param_types.iter().map(|t| format!("{:?}", t)).collect::<Vec<_>>().join(", "), 
            format!("{:?}", return_type)
        );
        
        Ok(Value::ExternFunction {
            library_name: library_name.clone(),
            name: function_name.clone(),
            signature,
            return_type,
            param_types,
        })
    }
}

/// Call an external function from a loaded library
///
/// Usage:
/// ```python
/// # Call a function
/// result = call_function("kernel32.dll", "GetTickCount", [])
/// result = call_function("m", "sqrt", [16.0])
/// result = call_function("m", "pow", [2.0, 3.0])
/// ```
pub fn call_function_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.len() < 2 {
            return Err(anyhow!(
                "call_function() requires at least 2 arguments (library, function_name)"
            ));
        }

        let library_name = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Library name must be a string")),
        };

        let function_name = match &args[1] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Function name must be a string")),
        };

        // Get function arguments (optional 3rd argument)
        let func_args = if args.len() > 2 {
            match &args[2] {
                Value::List(list) => list.as_vec().clone(),
                _ => return Err(anyhow!("Function arguments must be a list")),
            }
        } else {
            Vec::new()
        };

        let manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        let result = manager.call_external_function(&library_name, &function_name, func_args)?;

        Ok(result)
    }
}

/// Unload a previously loaded library
///
/// Usage:
/// ```python
/// unload_library("kernel32.dll")
/// ```
pub fn unload_library_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.is_empty() {
            return Err(anyhow!("unload_library() requires 1 argument (library_name)"));
        }

        let library_name = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Library name must be a string")),
        };

        let mut manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        manager.unload_library(&library_name)?;

        Ok(Value::None)
    }
}

/// List all loaded libraries
///
/// Usage:
/// ```python
/// libs = list_libraries()
/// print(libs)  # ["kernel32.dll", "m"]
/// ```
pub fn list_libraries_builtin(_args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        let manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        let libraries = manager.list_libraries();
        let list = libraries
            .into_iter()
            .map(Value::Str)
            .collect::<Vec<_>>();

        Ok(Value::List(HPList::from_values(list)))
    }
}

/// Get information about a loaded library
///
/// Usage:
/// ```python
/// info = library_info("kernel32.dll")
/// print(info)  # {"name": "kernel32.dll", "path": "C:\\Windows\\System32\\kernel32.dll", "functions": 5}
/// ```
pub fn library_info_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.is_empty() {
            return Err(anyhow!("library_info() requires 1 argument (library_name)"));
        }

        let library_name = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Library name must be a string")),
        };

        let manager = GLOBAL_FFI_MANAGER.lock().unwrap();

        if let Some((name, path, func_count)) = manager.get_library_info(&library_name) {
            let mut info = HashMap::new();
            info.insert("name".to_string(), Value::Str(name));
            info.insert("path".to_string(), Value::Str(path.to_string_lossy().to_string()));
            info.insert("functions".to_string(), Value::Int(func_count as i64));

            Ok(Value::Dict(Rc::new(RefCell::new(info))))
        } else {
            Err(anyhow!("Library not loaded: {}", library_name))
        }
    }
}

/// Add a custom search path for libraries
///
/// Usage:
/// ```python
/// add_library_path("/usr/local/mylibs")
/// add_library_path("C:\\MyLibraries")
/// ```
pub fn add_library_path_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.is_empty() {
            return Err(anyhow!("add_library_path() requires 1 argument (path)"));
        }

        let path_str = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Path must be a string")),
        };

        let mut manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        manager.add_search_path(&path_str);

        Ok(Value::None)
    }
}

/// Allocate a buffer for FFI use (e.g., for MSG structure)
///
/// Usage:
/// ```python
/// # Allocate 48 bytes for MSG structure
/// msg_buffer = allocate_buffer(48)
/// ```
pub fn allocate_buffer_builtin(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("allocate_buffer() requires 1 argument (size in bytes)"));
    }

    let size = match &args[0] {
        Value::Int(i) if *i > 0 => *i as usize,
        _ => return Err(anyhow!("Buffer size must be a positive integer")),
    };

    // Allocate zeroed memory
    let buffer = vec![0u8; size];
    let boxed = Box::new(buffer);
    let ptr = Box::into_raw(boxed);

    // Return pointer as integer
    Ok(Value::Int(ptr as usize as i64))
}

/// Free a buffer allocated by allocate_buffer
///
/// Usage:
/// ```python
/// free_buffer(msg_buffer)
/// ```
pub fn free_buffer_builtin(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow!("free_buffer() requires 1 argument (buffer pointer)"));
    }

    let ptr = match &args[0] {
        Value::Int(i) => *i as usize as *mut Vec<u8>,
        _ => return Err(anyhow!("Buffer pointer must be an integer")),
    };

    if ptr.is_null() {
        return Err(anyhow!("Cannot free null pointer"));
    }

    // Reconstruct the Box and let it drop
    unsafe {
        let _boxed = Box::from_raw(ptr);
        // Box will be dropped here, freeing the memory
    }

    Ok(Value::None)
}

/// Get the address of a function in a loaded library
/// This is useful for passing function pointers to callbacks
///
/// Usage:
/// ```python
/// # Get the address of gtk_main_quit
/// quit_addr = get_function_address("libgtk-3-0.dll", "gtk_main_quit")
/// # Use it as a callback function pointer
/// g_signal_connect_swapped(window, "destroy", quit_addr, 0)
/// ```
pub fn get_function_address_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.len() < 2 {
            return Err(anyhow!("get_function_address() requires 2 arguments (library_name, function_name)"));
        }

        let library_name = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Library name must be a string")),
        };

        let function_name = match &args[1] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("Function name must be a string")),
        };

        let manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        let address = manager.get_function_address(&library_name, &function_name)?;

        Ok(Value::Int(address as i64))
    }
}

/// Connect GTK window destroy signal to gtk_main_quit
/// This is a convenience function for GTK applications
pub fn gtk_connect_destroy_builtin(args: Vec<Value>) -> Result<Value> {
    #[cfg(not(feature = "ffi"))]
    {
        return Err(anyhow!("FFI feature is not enabled"));
    }

    #[cfg(feature = "ffi")]
    {
        if args.len() < 3 {
            return Err(anyhow!("gtk_connect_destroy() requires 3 arguments (widget_ptr, gtk_lib_name, gobject_lib_name)"));
        }

        let widget_ptr = match &args[0] {
            Value::Int(i) => *i,
            _ => return Err(anyhow!("Widget pointer must be an integer")),
        };

        let gtk_lib_name = match &args[1] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("GTK library name must be a string")),
        };

        let gobject_lib_name = match &args[2] {
            Value::Str(s) => s.clone(),
            _ => return Err(anyhow!("GObject library name must be a string")),
        };

        // Get the address of gtk_main_quit
        let manager = GLOBAL_FFI_MANAGER.lock().unwrap();
        let quit_addr = manager.get_function_address(&gtk_lib_name, "gtk_main_quit")?;

        // Call g_signal_connect_data to connect the destroy signal
        let signal_name = "destroy";
        let result = manager.call_external_function(
            &gobject_lib_name,
            "g_signal_connect_data",
            vec![
                Value::Int(widget_ptr),
                Value::Str(signal_name.to_string()),
                Value::Int(quit_addr as i64),
                Value::Int(0), // data
                Value::Int(0), // destroy_data
                Value::Int(0), // connect_flags
            ],
        )?;

        Ok(result)
    }
}

/// Initialize FFI builtins and return a HashMap of function names to Value::BuiltinFunction
pub fn init_ffi_builtins() -> HashMap<String, Value> {
    let mut builtins = HashMap::new();

    builtins.insert(
        "load_library".to_string(),
        Value::BuiltinFunction("load_library".to_string(), load_library_builtin),
    );

    builtins.insert(
        "define_function".to_string(),
        Value::BuiltinFunction("define_function".to_string(), define_function_builtin),
    );

    builtins.insert(
        "call_function".to_string(),
        Value::BuiltinFunction("call_function".to_string(), call_function_builtin),
    );

    builtins.insert(
        "unload_library".to_string(),
        Value::BuiltinFunction("unload_library".to_string(), unload_library_builtin),
    );

    builtins.insert(
        "list_libraries".to_string(),
        Value::BuiltinFunction("list_libraries".to_string(), list_libraries_builtin),
    );

    builtins.insert(
        "library_info".to_string(),
        Value::BuiltinFunction("library_info".to_string(), library_info_builtin),
    );

    builtins.insert(
        "add_library_path".to_string(),
        Value::BuiltinFunction("add_library_path".to_string(), add_library_path_builtin),
    );

    builtins.insert(
        "allocate_buffer".to_string(),
        Value::BuiltinFunction("allocate_buffer".to_string(), allocate_buffer_builtin),
    );

    builtins.insert(
        "free_buffer".to_string(),
        Value::BuiltinFunction("free_buffer".to_string(), free_buffer_builtin),
    );

    builtins.insert(
        "get_function_address".to_string(),
        Value::BuiltinFunction("get_function_address".to_string(), get_function_address_builtin),
    );

    builtins.insert(
        "gtk_connect_destroy".to_string(),
        Value::BuiltinFunction("gtk_connect_destroy".to_string(), gtk_connect_destroy_builtin),
    );

    builtins
}
