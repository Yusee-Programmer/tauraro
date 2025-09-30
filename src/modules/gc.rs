/// Garbage collection module for Tauraro
/// Provides APIs for garbage collection and memory management

use crate::value::Value;
use crate::runtime::get_global_memory_api;
use anyhow::Result;
use std::collections::HashMap;

/// Create the garbage collection module
pub fn create_gc_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Garbage collection functions
    namespace.insert("collect".to_string(), Value::BuiltinFunction("gc.collect".to_string(), builtin_gc_collect));
    namespace.insert("collect_tracing".to_string(), Value::BuiltinFunction("gc.collect_tracing".to_string(), builtin_gc_collect_tracing));
    namespace.insert("configure".to_string(), Value::BuiltinFunction("gc.configure".to_string(), builtin_gc_configure));
    
    // Garbage collection information
    namespace.insert("isenabled".to_string(), Value::BuiltinFunction("gc.isenabled".to_string(), builtin_gc_isenabled));
    namespace.insert("enable".to_string(), Value::BuiltinFunction("gc.enable".to_string(), builtin_gc_enable));
    namespace.insert("disable".to_string(), Value::BuiltinFunction("gc.disable".to_string(), builtin_gc_disable));
    
    // Garbage collection statistics
    namespace.insert("get_stats".to_string(), Value::BuiltinFunction("gc.get_stats".to_string(), builtin_gc_get_stats));
    namespace.insert("get_threshold".to_string(), Value::BuiltinFunction("gc.get_threshold".to_string(), builtin_gc_get_threshold));
    namespace.insert("set_threshold".to_string(), Value::BuiltinFunction("gc.set_threshold".to_string(), builtin_gc_set_threshold));
    
    Value::Module("gc".to_string(), namespace)
}

/// Built-in function to force garbage collection
fn builtin_gc_collect(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    global_memory_api.collect();
    Ok(Value::Int(0)) // Return number of collected objects (simplified)
}

/// Built-in function to force tracing garbage collection
fn builtin_gc_collect_tracing(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    global_memory_api.collect_tracing();
    Ok(Value::Int(0)) // Return number of collected objects (simplified)
}

/// Built-in function to configure garbage collector
fn builtin_gc_configure(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("gc.configure() takes exactly 1 argument"));
    }
    
    let global_memory_api = get_global_memory_api();
    
    match &args[0] {
        Value::Int(threshold) => {
            global_memory_api.configure_gc(*threshold as usize);
            Ok(Value::None)
        }
        Value::Dict(config) => {
            // Handle configuration dictionary
            if let Some(Value::Int(threshold)) = config.get("threshold") {
                global_memory_api.configure_gc(*threshold as usize);
            }
            Ok(Value::None)
        }
        _ => Err(anyhow::anyhow!("gc.configure() expects an integer or dictionary argument")),
    }
}

/// Built-in function to check if garbage collection is enabled
fn builtin_gc_isenabled(_args: Vec<Value>) -> Result<Value> {
    // In Tauraro, GC is always enabled but can be controlled by memory mode
    let global_memory_api = get_global_memory_api();
    let mode = global_memory_api.get_memory_mode();
    let enabled = match mode {
        crate::runtime::MemoryMode::Manual => false,
        _ => true,
    };
    Ok(Value::Bool(enabled))
}

/// Built-in function to enable garbage collection
fn builtin_gc_enable(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    global_memory_api.set_memory_mode(crate::runtime::MemoryMode::Automatic);
    Ok(Value::None)
}

/// Built-in function to disable garbage collection
fn builtin_gc_disable(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    global_memory_api.set_memory_mode(crate::runtime::MemoryMode::Manual);
    Ok(Value::None)
}

/// Built-in function to get garbage collection statistics
fn builtin_gc_get_stats(_args: Vec<Value>) -> Result<Value> {
    let global_memory_api = get_global_memory_api();
    let stats = global_memory_api.stats();
    
    // Create a dict with GC statistics
    let mut dict = HashMap::new();
    dict.insert("collections".to_string(), Value::Int(stats.collections as i64));
    dict.insert("current_allocations".to_string(), Value::Int(stats.current_allocations as i64));
    dict.insert("total_allocations".to_string(), Value::Int(stats.total_allocations as i64));
    
    Ok(Value::Dict(dict))
}

/// Built-in function to get garbage collection threshold
fn builtin_gc_get_threshold(_args: Vec<Value>) -> Result<Value> {
    // Return a simplified threshold value
    Ok(Value::Tuple(vec![
        Value::Int(700),
        Value::Int(10),
        Value::Int(10),
    ]))
}

/// Built-in function to set garbage collection threshold
fn builtin_gc_set_threshold(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("gc.set_threshold() takes 1 to 3 arguments"));
    }
    
    // In a full implementation, this would set the actual GC thresholds
    // For now, we'll just log the configuration
    println!("GC thresholds configured:");
    for (i, arg) in args.iter().enumerate() {
        if let Value::Int(threshold) = arg {
            println!("  Generation {}: {}", i, threshold);
        }
    }
    
    Ok(Value::None)
}