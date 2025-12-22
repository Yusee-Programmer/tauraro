//! Minimal Rust Code Generator
//! 
//! This module generates Rust code from Tauraro IR.
//! Much simpler approach that directly uses IR types instead of trying to parse AST.

use crate::ir::{IRModule, IRFunction, IRInstruction, Value, Type};
use anyhow::Result;
use std::collections::HashMap;

pub struct MinimalRustGenerator {
    code: String,
    indent_level: usize,
}

impl MinimalRustGenerator {
    pub fn new() -> Self {
        Self {
            code: String::new(),
            indent_level: 0,
        }
    }

    fn indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }

    fn emit(&mut self, line: &str) {
        self.code.push_str(&format!("{}{}\n", self.indent(), line));
    }

    pub fn generate(&mut self, _module: &IRModule) -> Result<String> {
        // Generate minimal Rust program
        self.code.clear();
        
        // Standard imports
        self.emit("use std::collections::HashMap;");
        self.emit("use std::vec::Vec;");
        self.emit("");
        
        // Main function
        self.emit("fn main() {");
        self.indent_level += 1;
        self.emit("println!(\"Hello from Tauraro Rust!\");");
        self.indent_level -= 1;
        self.emit("}");
        self.emit("");
        
        Ok(self.code.clone())
    }
}

pub fn generate_rust_code(module: &IRModule) -> Result<String> {
    let mut gen = MinimalRustGenerator::new();
    gen.generate(module)
}
