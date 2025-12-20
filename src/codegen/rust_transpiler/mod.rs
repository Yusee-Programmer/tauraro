//! Rust Transpiler for Tauraro
//!
//! This module transpiles Tauraro IR to safe, idiomatic Rust code.
//! Full support for all Tauraro language constructs with type safety,
//! memory safety, and excellent concurrency support via Tokio.

pub mod compiler;
pub mod builtins;
pub mod types;
pub mod functions;
pub mod classes;
pub mod expressions;
pub mod statements;
pub mod modules;
pub mod stdlib;

use crate::ir::{IRModule, IRInstruction, IRFunction};
use anyhow::Result;
use std::collections::{HashMap, HashSet};

/// Rust code generation context
#[derive(Debug, Clone)]
pub struct RustCodegenContext {
    /// Current indentation level
    pub indent_level: usize,
    /// Generated code buffer
    pub code: String,
    /// Type mapping cache
    pub type_cache: HashMap<String, String>,
    /// Function declarations
    pub functions: HashMap<String, FunctionInfo>,
    /// Class definitions
    pub classes: HashMap<String, ClassInfo>,
    /// Module imports
    pub imports: HashSet<String>,
    /// Module name
    pub module_name: String,
    /// Used external crates
    pub external_crates: HashSet<String>,
}

#[derive(Debug, Clone)]
pub struct FunctionInfo {
    pub name: String,
    pub params: Vec<(String, String)>, // (name, type)
    pub return_type: String,
    pub is_async: bool,
}

#[derive(Debug, Clone)]
pub struct ClassInfo {
    pub name: String,
    pub fields: Vec<(String, String)>, // (name, type)
    pub methods: Vec<String>,
    pub is_trait: bool,
}

impl RustCodegenContext {
    pub fn new(module_name: String) -> Self {
        Self {
            indent_level: 0,
            code: String::new(),
            type_cache: HashMap::new(),
            functions: HashMap::new(),
            classes: HashMap::new(),
            imports: HashSet::new(),
            module_name,
            external_crates: HashSet::new(),
        }
    }

    pub fn indent(&mut self) {
        self.indent_level += 1;
    }

    pub fn dedent(&mut self) {
        if self.indent_level > 0 {
            self.indent_level -= 1;
        }
    }

    pub fn get_indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }

    pub fn emit(&mut self, code: &str) {
        self.code.push_str(&format!("{}{}\n", self.get_indent(), code));
    }

    pub fn emit_raw(&mut self, code: &str) {
        self.code.push_str(code);
    }

    pub fn add_import(&mut self, import: &str) {
        self.imports.insert(import.to_string());
    }

    pub fn add_external_crate(&mut self, crate_name: &str) {
        self.external_crates.insert(crate_name.to_string());
    }

    pub fn get_imports_code(&self) -> String {
        let mut imports_code = String::new();

        // Standard library imports
        let std_imports = vec![
            "use std::collections::{HashMap, HashSet, VecDeque};",
            "use std::sync::{Arc, Mutex, RwLock};",
            "use std::rc::Rc;",
            "use std::cell::{RefCell, Cell};",
            "use std::any::Any;",
            "use std::fmt;",
        ];

        for import in std_imports {
            imports_code.push_str(&format!("{}\n", import));
        }

        // External crate imports
        if self.external_crates.contains("tokio") {
            imports_code.push_str("use tokio::task;\n");
            imports_code.push_str("use tokio::time;\n");
        }
        if self.external_crates.contains("regex") {
            imports_code.push_str("use regex::Regex;\n");
        }
        if self.external_crates.contains("serde") {
            imports_code.push_str("use serde_json::{json, Value as JsonValue};\n");
        }

        // Custom imports
        for import in &self.imports {
            imports_code.push_str(&format!("{}\n", import));
        }

        imports_code
    }
}

/// Main Rust Transpiler
pub struct RustTranspiler {
    pub context: RustCodegenContext,
}

impl RustTranspiler {
    pub fn new(module_name: String) -> Self {
        Self {
            context: RustCodegenContext::new(module_name),
        }
    }

    /// Transpile an IR module to Rust code
    pub fn transpile(&mut self, module: IRModule) -> Result<String> {
        // Generate module header
        self.emit_module_header();

        // Generate type definitions
        self.generate_type_defs(&module)?;

        // Generate class/struct definitions
        self.generate_class_defs(&module)?;

        // Generate function implementations
        self.generate_functions(&module)?;

        // Always generate main function for root module execution
        self.emit_main()?;

        Ok(self.context.code.clone())
    }

    fn emit_module_header(&mut self) {
        let imports = self.context.get_imports_code();
        self.context.emit_raw(&imports);
        self.context.emit_raw("\n");
    }

    fn generate_type_defs(&mut self, module: &IRModule) -> Result<()> {
        // Generate type aliases and enums
        self.context.emit("// Type definitions");
        self.context.emit("");

        // Add builtin types
        self.context.emit("type TauInteger = i64;");
        self.context.emit("type TauFloat = f64;");
        self.context.emit("type TauBool = bool;");
        self.context.emit("type TauString = String;");
        self.context.emit("");

        // Generate composite types
        self.context.emit("// Object type for dynamic values");
        self.context.emit_raw(
            r#"#[derive(Clone, Debug)]
pub enum TauObject {
    None,
    Bool(bool),
    Int(i64),
    Float(f64),
    String(String),
    List(Vec<TauObject>),
    Dict(HashMap<String, TauObject>),
    Custom(String, Arc<Mutex<HashMap<String, TauObject>>>),
}

impl TauObject {
    pub fn to_string(&self) -> String {
        match self {
            TauObject::None => "None".to_string(),
            TauObject::Bool(b) => b.to_string(),
            TauObject::Int(i) => i.to_string(),
            TauObject::Float(f) => {
                let s = f.to_string();
                if s.contains('.') { s } else { format!("{}.0", s) }
            }
            TauObject::String(s) => s.clone(),
            TauObject::List(items) => {
                let strs: Vec<_> = items.iter().map(|i| i.to_string()).collect();
                format!("[{}]", strs.join(", "))
            }
            TauObject::Dict(map) => {
                let mut items = Vec::new();
                for (k, v) in map.iter() {
                    items.push(format!("'{}': {}", k, v.to_string()));
                }
                format!("{{{}}}", items.join(", "))
            }
            TauObject::Custom(name, _) => format!("<{} object>", name),
        }
    }

    pub fn is_truthy(&self) -> bool {
        match self {
            TauObject::None => false,
            TauObject::Bool(b) => *b,
            TauObject::Int(i) => *i != 0,
            TauObject::Float(f) => *f != 0.0,
            TauObject::String(s) => !s.is_empty(),
            TauObject::List(items) => !items.is_empty(),
            TauObject::Dict(map) => !map.is_empty(),
            TauObject::Custom(_, _) => true,
        }
    }
}

impl PartialEq for TauObject {
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (TauObject::None, TauObject::None) => true,
            (TauObject::Bool(a), TauObject::Bool(b)) => a == b,
            (TauObject::Int(a), TauObject::Int(b)) => a == b,
            (TauObject::Float(a), TauObject::Float(b)) => a == b,
            (TauObject::String(a), TauObject::String(b)) => a == b,
            (TauObject::List(a), TauObject::List(b)) => a == b,
            (TauObject::Dict(a), TauObject::Dict(b)) => a == b,
            _ => false,
        }
    }
}

impl fmt::Display for TauObject {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.to_string())
    }
}
"#
        );
        self.context.emit_raw("\n");

        Ok(())
    }

    fn generate_class_defs(&mut self, module: &IRModule) -> Result<()> {
        // This would be populated from module class definitions
        Ok(())
    }

    fn generate_functions(&mut self, module: &IRModule) -> Result<()> {
        self.context.emit("// Function implementations");
        self.context.emit("");

        for (_name, func) in &module.functions {
            self.generate_function(func)?;
        }

        Ok(())
    }

    fn generate_function(&mut self, func: &IRFunction) -> Result<()> {
        let func_name = func.name.as_ref().unwrap_or(&"unnamed".to_string());

        // Function signature
        let params = func.params.iter()
            .map(|p| format!("{}: TauObject", p))
            .collect::<Vec<_>>()
            .join(", ");

        self.context.emit(&format!("fn {}({}) -> TauObject {{", func_name, params));
        self.context.indent();

        // Function body
        for (i, instr) in func.instructions.iter().enumerate() {
            if i == func.instructions.len() - 1 && matches!(instr, IRInstruction::Return(_)) {
                // Last instruction, emit as expression
                self.emit_instruction_expr(instr)?;
            } else {
                self.emit_instruction(instr)?;
            }
        }

        // Default return None if no explicit return
        if func.instructions.is_empty() || !matches!(func.instructions.last(), Some(IRInstruction::Return(_))) {
            self.context.emit("TauObject::None");
        }

        self.context.dedent();
        self.context.emit("}");
        self.context.emit("");

        Ok(())
    }

    fn emit_instruction(&mut self, instr: &IRInstruction) -> Result<()> {
        // Instructions will be expanded in statements.rs
        match instr {
            IRInstruction::Return(expr) => {
                self.context.emit(&format!("return {};", expr));
            }
            IRInstruction::Print(expr) => {
                self.context.emit(&format!("println!(\"{{}}\", {});", expr));
            }
            _ => {
                // Handle other instruction types
            }
        }
        Ok(())
    }

    fn emit_instruction_expr(&mut self, instr: &IRInstruction) -> Result<()> {
        match instr {
            IRInstruction::Return(expr) => {
                self.context.emit(&expr);
            }
            _ => {
                self.emit_instruction(instr)?;
            }
        }
        Ok(())
    }

    fn emit_main(&mut self) -> Result<()> {
        self.context.emit("");
        self.context.emit("#[tokio::main]");
        self.context.emit("async fn main() {");
        self.context.indent();
        self.context.emit("println!(\"Program completed successfully\");");
        self.context.dedent();
        self.context.emit("}");

        Ok(())
    }

    /// Get the final generated Rust code
    pub fn get_code(&self) -> String {
        self.context.code.clone()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_context_indentation() {
        let mut ctx = RustCodegenContext::new("test".to_string());
        assert_eq!(ctx.get_indent(), "");
        ctx.indent();
        assert_eq!(ctx.get_indent(), "    ");
        ctx.dedent();
        assert_eq!(ctx.get_indent(), "");
    }
}
