/// Memory management module for Tauraro
/// Provides APIs for automatic, manual, and hybrid memory management

use crate::value::Value;
use crate::runtime::{MemoryMode, get_global_memory_api};
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

/// Create the memory management module
pub fn create_memory_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Memory allocation functions
    namespace.insert("auto".to_string(), Value::BuiltinFunction("memory.auto".to_string(), builtin_memory_auto));
    namespace.insert("manual".to_string(), Value::BuiltinFunction("memory.manual".to_string(), builtin_memory_manual));
    namespace.insert("hybrid".to_string(), Value::BuiltinFunction("memory.hybrid".to_string(), builtin_memory_hybrid));
    
    // Memory mode functions
    namespace.insert("set_mode".to_string(), Value::BuiltinFunction("memory.set_mode".to_string(), builtin_memory_set_mode));
    namespace.insert("get_mode".to_string(), Value::BuiltinFunction("memory.get_mode".to_string(), builtin_memory_get_mode));
    
    // Memory statistics and information
    namespace.insert("stats".to_string(), Value::BuiltinFunction("memory.stats".to_string(), builtin_memory_stats));
    namespace.insert("usage".to_string(), Value::BuiltinFunction("memory.usage".to_string(), builtin_memory_usage));
    
    // Memory management utilities
    namespace.insert("sizeof".to_string(), Value::BuiltinFunction("memory.sizeof".to_string(), builtin_memory_sizeof));
    
    Value::Module("memory".to_string(), namespace)
}

/// Built-in function for automatic memory allocation
fn builtin_memory_auto(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("memory.auto() takes exactly 1 argument"));
    }
    
    let global_memory_api = get_global_memory_api();
    let managed_ptr = global_memory_api.auto(args[0].clone());
    
    Ok(Value::TypedValue {
        value: Box::new(managed_ptr.get().clone()),
        type_info: crate::ast::Type::Any,
    })
}

/// Built-in function for manual memory allocation
fn builtin_memory_manual(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("memory.manual() takes exactly 1 argument"));
    }
    
    let global_memory_api = get_global_memory_api();
    let managed_ptr = global_memory_api.manual(args[0].clone());
    
    Ok(Value::TypedValue {
        value: Box::new(managed_ptr.get().clone()),
        type_info: crate::ast::Type::Any,
    })
}

/// Built-in function for hybrid memory allocation
fn builtin_memory_hybrid(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("memory.hybrid() takes exactly 1 argument"));
    }
    
    let global_memory_api = get_global_memory_api();
    let managed_ptr = global_memory_api.hybrid(args[0].clone());
    
    Ok(Value::TypedValue {
        value: Box::new(managed_ptr.get().clone()),
        type_info: crate::ast::Type::Any,
    })
}

/// Built-in function to set memory management mode
fn builtin_memory_set_mode(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("memory.set_mode() takes exactly 1 argument"));
    }
    
    let global_memory_api = get_global_memory_api();
    
    match &args[0] {
        Value::Str(mode_str) => {
            let mode = match mode_str.as_str() {
                "automatic" => MemoryMode::Automatic,
                "manual" => MemoryMode::Manual,
                "arena" => MemoryMode::Arena,
                "hybrid" => MemoryMode::Hybrid,
                _ => return Err(anyhow::anyhow!("Invalid memory mode: {}", mode_str)),
            };
            global_memory_api.set_memory_mode(mode);
            Ok(Value::None)
        }
        _ => Err(anyhow::anyhow!("memory.set_mode() expects a string argument")),
    }
}

/// Built-in function to get current memory management mode
fn builtin_memory_get_mode(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    let mode = global_memory_api.get_memory_mode();
    
    let mode_str = match mode {
        MemoryMode::Automatic => "automatic",
        MemoryMode::Manual => "manual",
        MemoryMode::Arena => "arena",
        MemoryMode::Hybrid => "hybrid",
    };
    
    Ok(Value::Str(mode_str.to_string()))
}

/// Built-in function to get memory statistics
fn builtin_memory_stats(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    let stats = global_memory_api.stats();
    
    // Create a dict with statistics
    let mut dict = HashMap::new();
    dict.insert("total_allocations".to_string(), Value::Int(stats.total_allocations as i64));
    dict.insert("current_allocations".to_string(), Value::Int(stats.current_allocations as i64));
    dict.insert("total_bytes".to_string(), Value::Int(stats.total_bytes as i64));
    dict.insert("collections".to_string(), Value::Int(stats.collections as i64));
    dict.insert("manual_allocations".to_string(), Value::Int(stats.manual_allocations as i64));
    dict.insert("auto_allocations".to_string(), Value::Int(stats.auto_allocations as i64));
    
    Ok(Value::Dict(Rc::new(RefCell::new(dict))))
}

/// Built-in function to get memory usage
fn builtin_memory_usage(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    let usage = global_memory_api.memory_usage();
    Ok(Value::Str(usage))
}

/// Built-in function to get the size of a value in bytes
fn builtin_memory_sizeof(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("memory.sizeof() takes exactly 1 argument"));
    }
    
    let size = match &args[0] {
        Value::Int(_) => std::mem::size_of::<i64>(),
        Value::Float(_) => std::mem::size_of::<f64>(),
        Value::Bool(_) => std::mem::size_of::<bool>(),
        Value::Str(s) => s.len(),
        Value::List(items) => {
            std::mem::size_of::<Vec<Value>>() + 
            items.iter().map(|v| {
                match v {
                    Value::Int(_) => std::mem::size_of::<i64>(),
                    Value::Float(_) => std::mem::size_of::<f64>(),
                    Value::Bool(_) => std::mem::size_of::<bool>(),
                    Value::Str(s) => s.len(),
                    _ => std::mem::size_of_val(&v),
                }
            }).sum::<usize>()
        },
        Value::Dict(dict) => {
            std::mem::size_of::<HashMap<String, Value>>() + 
            dict.borrow().iter().map(|(k, v)| {
                k.len() + match v {
                    Value::Int(_) => std::mem::size_of::<i64>(),
                    Value::Float(_) => std::mem::size_of::<f64>(),
                    Value::Bool(_) => std::mem::size_of::<bool>(),
                    Value::Str(s) => s.len(),
                    _ => std::mem::size_of_val(&v),
                }
            }).sum::<usize>()
        },
        Value::Tuple(items) => {
            std::mem::size_of::<Vec<Value>>() + 
            items.iter().map(|v| {
                match v {
                    Value::Int(_) => std::mem::size_of::<i64>(),
                    Value::Float(_) => std::mem::size_of::<f64>(),
                    Value::Bool(_) => std::mem::size_of::<bool>(),
                    Value::Str(s) => s.len(),
                    _ => std::mem::size_of_val(&v),
                }
            }).sum::<usize>()
        },
        Value::Set(items) => {
            std::mem::size_of::<Vec<Value>>() + 
            items.iter().map(|v| {
                match v {
                    Value::Int(_) => std::mem::size_of::<i64>(),
                    Value::Float(_) => std::mem::size_of::<f64>(),
                    Value::Bool(_) => std::mem::size_of::<bool>(),
                    Value::Str(s) => s.len(),
                    _ => std::mem::size_of_val(&v),
                }
            }).sum::<usize>()
        },
        _ => std::mem::size_of_val(&args[0]),
    };
    
    Ok(Value::Int(size as i64))
}