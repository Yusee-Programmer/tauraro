//! Execution frame for the virtual machine

use crate::value::Value;
use std::collections::HashMap;

pub struct ExecutionFrame {
    pub locals: HashMap<String, Value>,
    pub globals: HashMap<String, Value>,
    pub pc: usize,
}

impl ExecutionFrame {
    pub fn new() -> Self {
        Self {
            locals: HashMap::new(),
            globals: HashMap::new(),
            pc: 0,
        }
    }
}

impl Default for ExecutionFrame {
    fn default() -> Self {
        Self::new()
    }
}