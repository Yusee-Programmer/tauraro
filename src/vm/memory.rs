//! Memory management for the virtual machine

use crate::value::Value;
use std::collections::HashMap;

pub struct Scope {
    pub variables: HashMap<String, Value>,
    pub parent: Option<Box<Scope>>,
}

impl Scope {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
            parent: None,
        }
    }
    
    pub fn new_with_parent(parent: Scope) -> Self {
        Self {
            variables: HashMap::new(),
            parent: Some(Box::new(parent)),
        }
    }
}

impl Default for Scope {
    fn default() -> Self {
        Self::new()
    }
}