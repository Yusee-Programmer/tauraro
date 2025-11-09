//! Usage Analyzer - Tracks which builtins and functions are actually used
//!
//! This module analyzes the IR to determine which built-in functions,
//! module functions, and features are actually used, so we only generate
//! the necessary C code (no redundant code).

use crate::ir::{IRModule, IRInstruction};
use std::collections::HashSet;

#[derive(Debug, Clone)]
pub struct UsageInfo {
    /// Built-in functions that are called (print, len, range, etc.)
    pub used_builtins: HashSet<String>,

    /// Module functions that are called (math.sqrt, sys.exit, etc.)
    pub used_module_functions: HashSet<String>,

    /// Features used (FFI, OOP, memory management, etc.)
    pub used_features: HashSet<String>,

    /// User-defined functions that are called
    pub used_user_functions: HashSet<String>,
}

impl UsageInfo {
    pub fn new() -> Self {
        Self {
            used_builtins: HashSet::new(),
            used_module_functions: HashSet::new(),
            used_features: HashSet::new(),
            used_user_functions: HashSet::new(),
        }
    }

    /// Analyze an IR module to determine what's actually used
    pub fn analyze(&mut self, module: &IRModule) {
        // Analyze global instructions
        for instruction in &module.globals {
            self.analyze_instruction(instruction);
        }

        // Analyze all functions
        for (_name, func) in &module.functions {
            for block in &func.blocks {
                for instruction in &block.instructions {
                    self.analyze_instruction(instruction);
                }
            }
        }
    }

    fn analyze_instruction(&mut self, instruction: &IRInstruction) {
        match instruction {
            IRInstruction::Call { func, args, .. } => {
                // Check if it's a builtin function call
                if self.is_builtin(func) {
                    self.used_builtins.insert(func.clone());
                } else if func.contains('.') {
                    // Module function like math.sqrt
                    self.used_module_functions.insert(func.clone());
                } else {
                    // User-defined function
                    self.used_user_functions.insert(func.clone());
                }

                // Mark that args are used (for future optimizations)
                for _ in args {
                    // Could track argument usage here
                }
            }

            IRInstruction::Import { .. } => {
                self.used_features.insert("imports".to_string());
            }

            IRInstruction::ImportFrom { .. } => {
                self.used_features.insert("imports".to_string());
            }

            IRInstruction::ObjectCreate { .. } => {
                self.used_features.insert("oop".to_string());
            }

            IRInstruction::ObjectGetAttr { .. } | IRInstruction::ObjectSetAttr { .. } => {
                self.used_features.insert("oop".to_string());
            }

            IRInstruction::Try { .. } => {
                self.used_features.insert("exceptions".to_string());
            }

            // Recursively analyze nested instructions in control flow
            IRInstruction::If { then_body, elif_branches, else_body, .. } => {
                for instr in then_body {
                    self.analyze_instruction(instr);
                }
                for (_cond, body) in elif_branches {
                    for instr in body {
                        self.analyze_instruction(instr);
                    }
                }
                if let Some(else_b) = else_body {
                    for instr in else_b {
                        self.analyze_instruction(instr);
                    }
                }
            }

            IRInstruction::While { body, condition_instructions, .. } => {
                for instr in condition_instructions {
                    self.analyze_instruction(instr);
                }
                for instr in body {
                    self.analyze_instruction(instr);
                }
            }

            IRInstruction::For { body, .. } => {
                for instr in body {
                    self.analyze_instruction(instr);
                }
            }

            IRInstruction::Try { body, handlers, else_body, finally_body } => {
                for instr in body {
                    self.analyze_instruction(instr);
                }
                for (_exc_type, _var, handler_body) in handlers {
                    for instr in handler_body {
                        self.analyze_instruction(instr);
                    }
                }
                if let Some(else_b) = else_body {
                    for instr in else_b {
                        self.analyze_instruction(instr);
                    }
                }
                if let Some(finally_b) = finally_body {
                    for instr in finally_b {
                        self.analyze_instruction(instr);
                    }
                }
            }

            _ => {
                // Other instruction types don't need special tracking
            }
        }
    }

    /// Check if a function name is a built-in
    fn is_builtin(&self, name: &str) -> bool {
        matches!(
            name,
            "print" | "len" | "range" | "int" | "float" | "str" | "bool" |
            "list" | "dict" | "tuple" | "set" | "type" | "isinstance" |
            "input" | "open" | "abs" | "min" | "max" | "sum" | "sorted" |
            "enumerate" | "zip" | "map" | "filter" | "any" | "all" |
            "pow" | "round" | "ord" | "chr" | "hex" | "oct" | "bin"
        )
    }
}
