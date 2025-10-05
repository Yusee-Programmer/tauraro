//! Call frame, function activation records
use crate::value::Value;
use std::collections::HashMap;

/// Frame object inspired by CPython's PyFrameObject
/// Properly manages local variables and execution context
#[derive(Debug, Clone)]
pub struct ExecutionFrame {
    /// Local variables (like CPython's f_locals)
    pub locals: HashMap<String, Value>,
    /// Global variables reference
    pub globals: HashMap<String, Value>,
    /// Built-in variables
    pub builtins: HashMap<String, Value>,
    /// Code being executed
    pub code_name: String,
    /// Parent frame for nested calls
    pub parent: Option<Box<ExecutionFrame>>,
}

impl ExecutionFrame {
    pub fn new(code_name: String) -> Self {
        Self {
            locals: HashMap::new(),
            globals: HashMap::new(),
            builtins: HashMap::new(),
            code_name,
            parent: None,
        }
    }
    
    /// Set variable with proper scoping (like CPython's STORE_FAST/STORE_NAME)
    pub fn set_local(&mut self, name: &str, value: Value) {
        self.locals.insert(name.to_string(), value);
    }
    
    /// Get variable with proper lookup order (like CPython's LOAD_FAST/LOAD_NAME)
    pub fn get_variable(&self, name: &str) -> Option<Value> {
        // LEGB rule: Local, Enclosing, Global, Built-in
        if let Some(value) = self.locals.get(name) {
            return Some(value.clone());
        }
        if let Some(value) = self.globals.get(name) {
            return Some(value.clone());
        }
        if let Some(value) = self.builtins.get(name) {
            return Some(value.clone());
        }
        None
    }
}