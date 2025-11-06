//! Type Inference for C Transpiler
//!
//! This module performs type inference on IR to generate optimized C code.
//! It detects variables with known types (especially integers) and generates
//! specialized code that uses native C types instead of heap-allocated values.

use crate::ir::{IRInstruction, IRModule};
use crate::value::Value;
use std::collections::{HashMap, HashSet};

#[derive(Debug, Clone, PartialEq)]
pub enum InferredType {
    Int,
    Float,
    Bool,
    String,
    Dynamic, // Unknown or mixed type
}

/// Type inference context that tracks variable types
pub struct TypeInferenceContext {
    /// Maps variable names to their inferred types
    pub var_types: HashMap<String, InferredType>,
    /// Variables that can be optimized (used consistently as single type)
    pub optimizable_vars: HashSet<String>,
}

impl TypeInferenceContext {
    pub fn new() -> Self {
        Self {
            var_types: HashMap::new(),
            optimizable_vars: HashSet::new(),
        }
    }

    /// Analyze an IR module and infer types for all variables
    pub fn analyze_module(&mut self, module: &IRModule) {
        // First pass: collect initial types from LoadConst (recursively)
        for instr in &module.globals {
            self.analyze_instruction_recursive(instr);
        }

        // Second pass: mark variables as optimizable if they maintain single type
        self.identify_optimizable_vars(&module.globals);
    }

    /// Analyze a single instruction and update type information
    fn analyze_instruction(&mut self, instr: &IRInstruction) {
        match instr {
            IRInstruction::LoadConst { value, result } => {
                let inferred_type = match value {
                    Value::Int(_) => InferredType::Int,
                    Value::Float(_) => InferredType::Float,
                    Value::Bool(_) => InferredType::Bool,
                    Value::Str(_) => InferredType::String,
                    _ => InferredType::Dynamic,
                };
                self.set_var_type(result, inferred_type);
            }
            IRInstruction::BinaryOp { op, left, right, result } => {
                // Infer result type based on operands
                let left_type = self.get_var_type(left);
                let right_type = self.get_var_type(right);

                let result_type = match (left_type, right_type) {
                    (InferredType::Int, InferredType::Int) => InferredType::Int,
                    (InferredType::Float, _) | (_, InferredType::Float) => InferredType::Float,
                    (InferredType::String, InferredType::String) => InferredType::String,
                    _ => InferredType::Dynamic,
                };

                self.set_var_type(result, result_type);
            }
            IRInstruction::StoreGlobal { name, value } |
            IRInstruction::StoreLocal { name, value } => {
                // Variable takes the type of assigned value
                if let Some(value_type) = self.var_types.get(value) {
                    self.set_var_type(name, value_type.clone());
                }
            }
            IRInstruction::LoadGlobal { name, result } |
            IRInstruction::LoadLocal { name, result } => {
                // Result gets type of loaded variable
                if let Some(name_type) = self.var_types.get(name) {
                    self.set_var_type(result, name_type.clone());
                }
            }
            IRInstruction::Call { func, result, .. } => {
                // Handle known builtin functions
                if let Some(result_var) = result {
                    let result_type = match func.as_str() {
                        "range" => InferredType::Dynamic, // range returns iterator
                        "len" => InferredType::Int,
                        "str" => InferredType::String,
                        "int" => InferredType::Int,
                        "float" => InferredType::Float,
                        "bool" => InferredType::Bool,
                        _ => InferredType::Dynamic,
                    };
                    self.set_var_type(result_var, result_type);
                }
            }
            IRInstruction::For { variable, .. } => {
                // Loop variable in range() is always int
                self.set_var_type(variable, InferredType::Int);
            }
            _ => {
                // Other instructions don't affect type inference yet
            }
        }
    }

    /// Recursively analyze nested instructions (like loop bodies)
    fn analyze_instruction_recursive(&mut self, instr: &IRInstruction) {
        self.analyze_instruction(instr);

        match instr {
            IRInstruction::For { body, .. } |
            IRInstruction::While { body, .. } => {
                for body_instr in body {
                    self.analyze_instruction_recursive(body_instr);
                }
            }
            IRInstruction::If { then_body, elif_branches, else_body, .. } => {
                for body_instr in then_body {
                    self.analyze_instruction_recursive(body_instr);
                }
                for (_, elif_body) in elif_branches {
                    for body_instr in elif_body {
                        self.analyze_instruction_recursive(body_instr);
                    }
                }
                if let Some(else_body) = else_body {
                    for body_instr in else_body {
                        self.analyze_instruction_recursive(body_instr);
                    }
                }
            }
            _ => {}
        }
    }

    /// Identify variables that can be optimized (used consistently as single type)
    fn identify_optimizable_vars(&mut self, instructions: &[IRInstruction]) {
        // Track variable type consistency
        let mut var_type_changes: HashMap<String, Vec<InferredType>> = HashMap::new();

        for instr in instructions {
            self.track_type_changes(instr, &mut var_type_changes);
        }

        // Mark variables that maintain consistent type (especially Int)
        for (var, types) in &var_type_changes {
            if types.iter().all(|t| matches!(t, InferredType::Int)) {
                self.optimizable_vars.insert(var.clone());
            }
        }

        // Also mark variables that only have Int type and no type changes (constants, temporaries)
        for (var, typ) in &self.var_types {
            if matches!(typ, InferredType::Int) && !var_type_changes.contains_key(var) {
                self.optimizable_vars.insert(var.clone());
            }
        }
    }

    /// Track type changes for a variable across all instructions
    fn track_type_changes(&self, instr: &IRInstruction, changes: &mut HashMap<String, Vec<InferredType>>) {
        match instr {
            IRInstruction::StoreGlobal { name, value } |
            IRInstruction::StoreLocal { name, value } => {
                if let Some(value_type) = self.var_types.get(value) {
                    changes.entry(name.clone())
                        .or_insert_with(Vec::new)
                        .push(value_type.clone());
                }
            }
            IRInstruction::For { body, .. } |
            IRInstruction::While { body, .. } => {
                for body_instr in body {
                    self.track_type_changes(body_instr, changes);
                }
            }
            IRInstruction::If { then_body, elif_branches, else_body, .. } => {
                for body_instr in then_body {
                    self.track_type_changes(body_instr, changes);
                }
                for (_, elif_body) in elif_branches {
                    for body_instr in elif_body {
                        self.track_type_changes(body_instr, changes);
                    }
                }
                if let Some(else_body) = else_body {
                    for body_instr in else_body {
                        self.track_type_changes(body_instr, changes);
                    }
                }
            }
            _ => {}
        }
    }

    fn set_var_type(&mut self, var: &str, var_type: InferredType) {
        self.var_types.insert(var.to_string(), var_type);
    }

    fn get_var_type(&self, var: &str) -> InferredType {
        self.var_types.get(var).cloned().unwrap_or(InferredType::Dynamic)
    }

    /// Check if a variable should use optimized C code (native int64_t)
    pub fn is_optimizable_int(&self, var: &str) -> bool {
        self.optimizable_vars.contains(var) &&
        matches!(self.get_var_type(var), InferredType::Int)
    }

    /// Get the C type declaration for a variable
    pub fn get_c_type(&self, var: &str) -> &str {
        if self.is_optimizable_int(var) {
            "int64_t"
        } else {
            match self.get_var_type(var) {
                InferredType::Int => "int64_t",
                InferredType::Float => "double",
                InferredType::Bool => "bool",
                _ => "tauraro_value_t*",
            }
        }
    }
}
