//! Stack frame for the virtual machine

use crate::value::Value;
use std::collections::HashMap;

pub struct StackFrame {
    pub stack: Vec<Value>,
    pub locals: HashMap<String, Value>,
}

impl StackFrame {
    pub fn new() -> Self {
        Self {
            stack: Vec::new(),
            locals: HashMap::new(),
        }
    }
}

impl Default for StackFrame {
    fn default() -> Self {
        Self::new()
    }
}