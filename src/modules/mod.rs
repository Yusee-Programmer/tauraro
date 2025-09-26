/// Built-in modules for Tauraro
/// This module contains implementations of built-in modules similar to Python's standard library

use crate::value::Value;
use std::collections::HashMap;

pub mod os;
pub mod sys;
pub mod thread;

/// Initialize all built-in modules and return them as a HashMap
pub fn init_builtin_modules() -> HashMap<String, Value> {
    let mut modules = HashMap::new();
    
    // Add built-in modules
    modules.insert("os".to_string(), os::create_os_module());
    modules.insert("sys".to_string(), sys::create_sys_module());
    modules.insert("thread".to_string(), thread::create_thread_module());
    
    modules
}

/// Get a specific built-in module by name
pub fn get_builtin_module(name: &str) -> Option<Value> {
    match name {
        "os" => Some(os::create_os_module()),
        "sys" => Some(sys::create_sys_module()),
        "thread" => Some(thread::create_thread_module()),
        _ => None,
    }
}

/// Check if a module name is a built-in module
pub fn is_builtin_module(name: &str) -> bool {
    matches!(name, "os" | "sys" | "thread")
}

/// Get list of all built-in module names
pub fn get_builtin_module_names() -> Vec<String> {
    vec![
        "os".to_string(),
        "sys".to_string(),
        "thread".to_string(),
    ]
}