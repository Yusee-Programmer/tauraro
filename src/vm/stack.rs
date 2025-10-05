//! Operand stack + helpers
use crate::value::Value;

/// Stack frame for function calls with traceback information
#[derive(Debug, Clone)]
pub struct StackFrame {
    pub function_name: String,
    pub return_address: usize,
    pub scope_index: usize,
    /// Line number where this frame was created
    pub line_number: usize,
    /// File name where this frame was created
    pub file_name: String,
}

/// Operand stack for the VM
#[derive(Debug)]
pub struct OperandStack {
    stack: Vec<Value>,
}

impl OperandStack {
    pub fn new() -> Self {
        Self {
            stack: Vec::new(),
        }
    }
    
    /// Push a value onto the stack
    pub fn push(&mut self, value: Value) {
        self.stack.push(value);
    }
    
    /// Pop a value from the stack
    pub fn pop(&mut self) -> Option<Value> {
        self.stack.pop()
    }
    
    /// Peek at the top value without removing it
    pub fn peek(&self) -> Option<&Value> {
        self.stack.last()
    }
    
    /// Get the size of the stack
    pub fn len(&self) -> usize {
        self.stack.len()
    }
    
    /// Check if the stack is empty
    pub fn is_empty(&self) -> bool {
        self.stack.is_empty()
    }
    
    /// Duplicate the top value
    pub fn dup(&mut self) {
        if let Some(top) = self.stack.last().cloned() {
            self.stack.push(top);
        }
    }
    
    /// Rotate the top two values
    pub fn rot_two(&mut self) {
        if self.stack.len() >= 2 {
            let len = self.stack.len();
            self.stack.swap(len - 1, len - 2);
        }
    }
    
    /// Rotate the top three values
    pub fn rot_three(&mut self) {
        if self.stack.len() >= 3 {
            let len = self.stack.len();
            self.stack.swap(len - 1, len - 3);
        }
    }
}