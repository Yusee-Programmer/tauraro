//! Heap, refcount/GC, manual memory API
use crate::value::Value;
use crate::ast::Type;
use std::collections::HashMap;

/// Variable scope
#[derive(Debug, Clone)]
pub struct Scope {
    pub variables: HashMap<String, Value>,
    pub variable_types: HashMap<String, Type>, // Track declared types for strict typing
    pub parent: Option<usize>,
    pub scope_type: String, // "global", "function", "class", "block"
    // Fast lookup cache for local variables (name -> index)
    local_variable_indices: HashMap<String, usize>,
}

impl Scope {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
            variable_types: HashMap::new(),
            parent: None,
            scope_type: "module".to_string(),
            local_variable_indices: HashMap::new(),
        }
    }
    
    /// Register a local variable for fast access
    pub fn register_local(&mut self, name: &str) {
        let index = self.variables.len();
        self.local_variable_indices.insert(name.to_string(), index);
    }
    
    /// Check if a variable is a local variable
    pub fn is_local(&self, name: &str) -> bool {
        self.local_variable_indices.contains_key(name)
    }
    
    /// Get local variable index for fast access
    pub fn get_local_index(&self, name: &str) -> Option<usize> {
        self.local_variable_indices.get(name).copied()
    }
}

/// Memory management utilities
pub struct MemoryManager;

impl MemoryManager {
    /// Allocate memory for a value
    pub fn allocate(value: Value) -> Value {
        value
    }
    
    /// Deallocate memory for a value
    pub fn deallocate(_value: Value) {
        // In a real implementation, this would handle reference counting or garbage collection
    }
    
    /// Clone a value if it's not unique (for copy-on-write optimization)
    pub fn clone_if_not_unique(value: &Value) -> Value {
        value.clone()
    }
}