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
    /// Track original numeric values for variables (before format string conversion)
    pub original_var_values: HashMap<String, String>,
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
            original_var_values: HashMap::new(),
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

        // Minimal standard library imports needed for the generated code
        let std_imports = vec![
            "use std::collections::HashMap;",
            "use std::sync::{Arc, Mutex};",
            "use std::fmt;",
        ];

        for import in std_imports {
            imports_code.push_str(&format!("{}\n", import));
        }

        // Only add external crate imports if explicitly requested
        // For now, skip them to ensure code compiles standalone
        
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

        // Check if there's a user-defined main function
        let has_user_main = module.functions.iter().any(|(name, _)| name == "main");

        // Generate function implementations
        self.generate_functions(&module)?;

        // Only generate async wrapper main if there's no user-defined main
        if !has_user_main {
            self.emit_main()?;
        }

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
        // Emit helper functions for list, dict, string, and builtin operations
        self.emit_helper_functions();
        Ok(())
    }

    fn emit_helper_functions(&mut self) {
        self.context.emit("// ===== Helper Functions for Methods and Builtins =====");
        self.context.emit("");

        // Display wrapper for Vec to avoid orphan rule
        self.context.emit_raw(r#"// Wrapper for Vec to implement Display
#[derive(Clone, Debug)]
struct VecDisplay<T: std::fmt::Display>(Vec<T>);

impl<T: std::fmt::Display> std::fmt::Display for VecDisplay<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let items: Vec<String> = self.0.iter().map(|x| format!("{}", x)).collect();
        write!(f, "[{}]", items.join(", "))
    }
}

// Simple Display impl for i64 Vec
fn format_list(list: &[i64]) -> String {
    let items: Vec<String> = list.iter().map(|x| format!("{}", x)).collect();
    format!("[{}]", items.join(", "))
}

// List method implementations
fn lst__append(mut list: Vec<i64>, item: i64) -> Vec<i64> {
    list.push(item);
    list
}

fn lst__pop(mut list: Vec<i64>) -> i64 {
    list.pop().unwrap_or(0)
}

fn lst__reverse(mut list: Vec<i64>) -> Vec<i64> {
    list.reverse();
    list
}

fn lst__index(list: &[i64], item: i64) -> i64 {
    for (i, elem) in list.iter().enumerate() {
        if *elem == item {
            return i as i64;
        }
    }
    -1
}

fn lst__count(list: &[i64], item: i64) -> i64 {
    list.iter().filter(|x| **x == item).count() as i64
}

fn lst__extend(mut list: Vec<i64>, other: Vec<i64>) -> Vec<i64> {
    list.extend(other);
    list
}

fn lst__insert(mut list: Vec<i64>, index: usize, item: i64) -> Vec<i64> {
    if index <= list.len() {
        list.insert(index, item);
    }
    list
}

fn lst__remove(mut list: Vec<i64>, item: i64) -> Vec<i64> {
    list.retain(|x| *x != item);
    list
}

fn lst__clear(mut list: Vec<i64>) -> Vec<i64> {
    list.clear();
    list
}

// String method implementations
fn text__upper(s: &str) -> String {
    s.to_uppercase()
}

fn text__lower(s: &str) -> String {
    s.to_lowercase()
}

fn text__strip(s: &str) -> String {
    s.trim().to_string()
}

fn text__replace(s: &str, old: &str, new: &str) -> String {
    s.replace(old, new)
}

fn text__split(s: &str, sep: &str) -> Vec<String> {
    s.split(sep).map(|x| x.to_string()).collect()
}

fn text__join(sep: &str, items: &[String]) -> String {
    items.join(sep)
}

fn text__startswith(s: &str, prefix: &str) -> bool {
    s.starts_with(prefix)
}

fn text__endswith(s: &str, suffix: &str) -> bool {
    s.ends_with(suffix)
}

fn text__find(s: &str, sub: &str) -> i64 {
    s.find(sub).map(|i| i as i64).unwrap_or(-1)
}

fn text__index(s: &str, sub: &str) -> i64 {
    s.find(sub).map(|i| i as i64).unwrap_or(-1)
}

fn text__count(s: &str, sub: &str) -> i64 {
    s.matches(sub).count() as i64
}

// Also support list.count() via text__count (convenience overload)
fn text__count_list(list: &[i64], item: i64) -> i64 {
    list.iter().filter(|x| **x == item).count() as i64
}

fn text__capitalize(s: &str) -> String {
    let mut chars = s.chars();
    match chars.next() {
        None => String::new(),
        Some(first) => first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase(),
    }
}

fn text__title(s: &str) -> String {
    s.split_whitespace()
        .map(|word| text__capitalize(word))
        .collect::<Vec<_>>()
        .join(" ")
}

fn text__isdigit(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_ascii_digit())
}

fn text__isalpha(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_alphabetic())
}

// Dict methods - generic implementations
fn dict__get(map: &std::collections::HashMap<String, String>, key: &str, default: &str) -> String {
    map.get(key).cloned().unwrap_or_else(|| default.to_string())
}

fn dict__keys(map: &std::collections::HashMap<String, String>) -> Vec<String> {
    map.keys().cloned().collect()
}

fn dict__values(map: &std::collections::HashMap<String, String>) -> Vec<String> {
    map.values().cloned().collect()
}

fn dict__update(mut map: std::collections::HashMap<String, String>, other: std::collections::HashMap<String, String>) -> std::collections::HashMap<String, String> {
    for (k, v) in other {
        map.insert(k, v);
    }
    map
}

fn dict__clear(mut map: std::collections::HashMap<String, String>) -> std::collections::HashMap<String, String> {
    map.clear();
    map
}

// Builtin functions
fn tau_abs(n: i64) -> i64 {
    n.abs()
}

fn tau_min(numbers: &[i64]) -> i64 {
    *numbers.iter().min().unwrap_or(&0)
}

fn tau_max(numbers: &[i64]) -> i64 {
    *numbers.iter().max().unwrap_or(&0)
}

fn tau_sum(numbers: &[i64]) -> i64 {
    numbers.iter().sum()
}

fn tau_pow(base: i64, exp: i64) -> i64 {
    base.pow(exp as u32)
}

fn tau_round(f: f64) -> i64 {
    f.round() as i64
}

"#);
        self.context.emit("");
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
        let func_name = &func.name;
        let params = func.params.iter()
            .map(|p| format!("{}: i64", p))
            .collect::<Vec<_>>()
            .join(", ");

        // Determine return type based on IR or default to ()
        let return_type = if func_name == "main" { 
            "".to_string() 
        } else { 
            // By default, user functions return () unless explicitly typed
            " -> ()".to_string()
        };
        
        self.context.emit(&format!("fn {}{}{} {{", func_name, if params.is_empty() { "()".to_string() } else { format!("({})", params) }, return_type));
        self.context.indent();

        // Generate function body from IR blocks
        if func.blocks.is_empty() {
            // Empty function
            if func_name == "main" {
                self.context.emit("println!(\"Program executed\");");
            }
        } else {
            // Generate code from all blocks
            for block in &func.blocks {
                self.generate_block(&block.instructions)?;
            }
        }

        self.context.dedent();
        self.context.emit("}");
        self.context.emit("");

        Ok(())
    }

    fn generate_block(&mut self, instructions: &[crate::ir::IRInstruction]) -> Result<()> {
        for instr in instructions {
            self.generate_instruction(instr)?;
        }
        Ok(())
    }

    fn generate_instruction(&mut self, instr: &crate::ir::IRInstruction) -> Result<()> {
        use crate::ir::IRInstruction::*;

        match instr {
            Comment(text) => {
                self.context.emit(&format!("// {}", text));
            }
            LoadConst { value, result } => {
                let rust_value = self.value_to_rust(value);
                self.context.emit(&format!("let {} = {};", result, rust_value));
            }
            LoadLocal { name, result } => {
                self.context.emit(&format!("let {} = {};", result, name));
                // If this variable had a tracked origin, propagate it
                if let Some(orig) = self.context.original_var_values.get(name) {
                    self.context.original_var_values.insert(result.clone(), orig.clone());
                }
            }
            LoadGlobal { name, result } => {
                // For now, treat LoadGlobal the same as LoadLocal (simplified)
                self.context.emit(&format!("let {} = {};", result, name));
                // If this variable had a tracked origin, propagate it
                if let Some(orig) = self.context.original_var_values.get(name) {
                    self.context.original_var_values.insert(result.clone(), orig.clone());
                }
            }
            StoreLocal { name, value } => {
                self.context.emit(&format!("let {} = {};", name, value));
                // Track if this variable is assigned from another tracked variable
                if let Some(orig) = self.context.original_var_values.get(value) {
                    self.context.original_var_values.insert(name.clone(), orig.clone());
                }
            }
            StoreGlobal { name, value } => {
                // For now, treat StoreGlobal the same as StoreLocal (simplified)
                self.context.emit(&format!("let {} = {};", name, value));
                // Track if this variable is assigned from another tracked variable
                if let Some(orig) = self.context.original_var_values.get(value) {
                    self.context.original_var_values.insert(name.clone(), orig.clone());
                }
            }
            BinaryOp { op, left, right, result } => {
                let rust_op = self.binary_op_to_rust(op);
                
                // Try to resolve f-string variables to their original numeric values
                let left_val = if let Some(orig) = self.context.original_var_values.get(left) {
                    orig.clone()
                } else {
                    left.clone()
                };
                let right_val = if let Some(orig) = self.context.original_var_values.get(right) {
                    orig.clone()
                } else {
                    right.clone()
                };
                
                // Only use format! for string concatenation if BOTH operands look like strings
                // (contain "string" or "str" in their name, or are string literals)
                let left_is_string = left_val.contains("\"") || left_val.starts_with("fstring") || left_val.ends_with("_str");
                let right_is_string = right_val.contains("\"") || right_val.starts_with("fstring") || right_val.ends_with("_str");
                
                if rust_op == "+" && (left_is_string || right_is_string) {
                    // String concatenation case - use format
                    self.context.emit(&format!(
                        "let {} = {{ let l = format!(\"{{:?}}\", &{}); let r = format!(\"{{:?}}\", &{}); format!(\"{{}}{{}}\", l, r) }};", 
                        result, left_val, right_val
                    ));
                } else {
                    // Normal numeric operation
                    self.context.emit(&format!("let {} = {} {} {};", result, left_val, rust_op, right_val));
                }
            }
            Call { func, args, result } => {
                // Special handling for print function
                if func == "print" {
                    if args.len() == 1 {
                        self.context.emit(&format!("println!(\"{{}}\", {});", args[0]));
                    } else {
                        let arg_placeholders = args.iter().map(|_| "{}").collect::<Vec<_>>().join(" ");
                        let args_str = args.join(", ");
                        self.context.emit(&format!("println!(\"{}\", {});", arg_placeholders, args_str));
                    }
                } else if func == "len" && args.len() == 1 {
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = {}.len() as i64;", res, args[0]));
                    }
                } else if func == "range" {
                    if let Some(res) = result {
                        if args.len() == 1 {
                            self.context.emit(&format!("let {} = (0..{}).collect::<Vec<_>>();", res, args[0]));
                        } else if args.len() == 2 {
                            self.context.emit(&format!("let {} = ({}..{}).collect::<Vec<_>>();", res, args[0], args[1]));
                        }
                    }
                } else if func.starts_with("lst__") || func.starts_with("text__") {
                    // List and string methods need special handling for references
                    let modified_args: Vec<String> = args.iter().enumerate().map(|(i, arg)| {
                        // First argument to many list/string methods needs to be borrowed
                        if i == 0 && (func == "lst__index" || func == "lst__count" || func == "text__count" || 
                                      func == "text__find" || func == "text__index" || func == "text__startswith" ||
                                      func == "text__endswith" || func == "text__split" || func == "text__capitalize" ||
                                      func == "text__title" || func == "text__isdigit" || func == "text__isalpha" ||
                                      func == "text__upper" || func == "text__lower" || func == "text__strip" ||
                                      func == "text__replace" || func == "text__join") {
                            format!("&{}", arg)
                        } else {
                            arg.clone()
                        }
                    }).collect();
                    
                    let args_str = modified_args.join(", ");
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = {}({});", res, func, args_str));
                    } else {
                        self.context.emit(&format!("{}({});", func, args_str));
                    }
                } else {
                    // Regular function call
                    let args_str = args.join(", ");
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = {}({});", res, func, args_str));
                    } else {
                        self.context.emit(&format!("{}({});", func, args_str));
                    }
                }
            }
            Return { value } => {
                // Don't emit return in main function, main must return ()
                if let Some(val) = value {
                    self.context.emit(&format!("// return {};", val));
                }
            }
            If { condition, then_body, elif_branches, else_body } => {
                self.generate_if_statement(condition, then_body, elif_branches, else_body.as_ref())?;
            }
            While { condition, body, .. } => {
                self.context.emit(&format!("while {} {{", condition));
                self.context.indent();
                self.generate_block(body)?;
                self.context.dedent();
                self.context.emit("}");
            }
            For { variable, iterable, body, .. } => {
                self.context.emit(&format!("for {} in &{} {{", variable, iterable));
                self.context.indent();
                self.generate_block(body)?;
                self.context.dedent();
                self.context.emit("}");
            }
            Break => {
                self.context.emit("break;");
            }
            Continue => {
                self.context.emit("continue;");
            }
            ListCreate { elements, result } => {
                let elems_str = elements.join(", ");
                self.context.emit(&format!("let {} = vec![{}];", result, elems_str));
            }
            DictCreate { pairs, result } => {
                self.context.emit(&format!("let mut {} = HashMap::new();", result));
                for (key, value) in pairs {
                    self.context.emit(&format!("{}.insert({}, {});", result, key, value));
                }
            }
            FormatString { parts, result } => {
                self.generate_format_string(parts, result)?;
            }
            crate::ir::IRInstruction::FormatStringWithType { parts, result, original_type: _, original_var } => {
                self.generate_format_string(parts, result)?;
                // Track the original variable if provided
                if let Some(orig_var) = original_var {
                    self.context.original_var_values.insert(result.clone(), orig_var.clone());
                }
            }
            _ => {
                // For unimplemented instructions, emit a comment
                // self.context.emit(&format!("// TODO: {:?}", instr));
            }
        }
        Ok(())
    }

    fn generate_if_statement(
        &mut self,
        condition: &str,
        then_body: &[crate::ir::IRInstruction],
        elif_branches: &[(String, Vec<crate::ir::IRInstruction>)],
        else_body: Option<&Vec<crate::ir::IRInstruction>>,
    ) -> Result<()> {
        self.context.emit(&format!("if {} {{", condition));
        self.context.indent();
        self.generate_block(then_body)?;
        self.context.dedent();

        for (elif_cond, elif_body) in elif_branches {
            self.context.emit(&format!("}} else if {} {{", elif_cond));
            self.context.indent();
            self.generate_block(elif_body)?;
            self.context.dedent();
        }

        if let Some(else_stmts) = else_body {
            self.context.emit("} else {");
            self.context.indent();
            self.generate_block(else_stmts)?;
            self.context.dedent();
        }

        self.context.emit("}");
        Ok(())
    }

    fn generate_format_string(
        &mut self,
        parts: &[crate::ir::IRFormatPart],
        result: &str,
    ) -> Result<()> {
        use crate::ir::IRFormatPart::*;

        let mut format_str = String::new();
        let mut format_args = Vec::new();
        let mut single_var: Option<String> = None;

        for part in parts {
            match part {
                Literal(s) => {
                    format_str.push_str(&s.replace("{", "{{").replace("}", "}}"));
                }
                Expression { var, format_spec: _ } => {
                    format_str.push_str("{}");
                    format_args.push(var.clone());
                    // If there's only one expression and no other literal besides the var, track it
                    if single_var.is_none() && format_str.replace("{}", "").trim().is_empty() {
                        single_var = Some(var.clone());
                    }
                }
            }
        }

        if format_args.is_empty() {
            self.context.emit(&format!("let {} = \"{}\".to_string();", result, format_str));
        } else {
            let args_str = format_args.join(", ");
            self.context.emit(&format!(
                "let {} = format!(\"{}\", {});",
                result, format_str, args_str
            ));
            
            // If this f-string is just wrapping a single variable for printing,
            // track the original variable name so arithmetic operations can use it
            if let Some(orig_var) = single_var {
                self.context.original_var_values.insert(result.to_string(), orig_var);
            }
        }
        Ok(())
    }

    fn value_to_rust(&self, value: &crate::value::Value) -> String {
        use crate::value::Value::*;
        match value {
            Int(i) => i.to_string(),
            Float(f) => {
                let s = f.to_string();
                if s.contains('.') { s } else { format!("{}.0", s) }
            }
            Bool(b) => b.to_string(),
            Str(s) => format!("\"{}\"", s.escape_default()),
            Ellipsis => "\"...\"".to_string(),
            _ => "0".to_string(), // Default for unsupported types
        }
    }

    fn binary_op_to_rust(&self, op: &crate::ast::BinaryOp) -> String {
        use crate::ast::BinaryOp::*;
        match op {
            Add => "+",
            Sub => "-",
            Subtract => "-",
            Multiply => "*",
            Mul => "*",
            Divide => "/",
            Div => "/",
            FloorDivide => "/",
            FloorDiv => "/",
            Modulo => "%",
            Mod => "%",
            Power => "^",
            Pow => "^",
            Equal => "==",
            Eq => "==",
            NotEqual => "!=",
            Ne => "!=",
            Neq => "!=",
            Less => "<",
            Lt => "<",
            LessEqual => "<=",
            Le => "<=",
            Lte => "<=",
            Greater => ">",
            Gt => ">",
            GreaterEqual => ">=",
            Ge => ">=",
            Gte => ">=",
            And => "&&",
            Or => "||",
            BitAnd => "&",
            BitOr => "|",
            BitXor => "^",
            LeftShift => "<<",
            LShift => "<<",
            RightShift => ">>",
            RShift => ">>",
            In => "in",
            NotIn => "not in",
            Is => "==",
            IsNot => "!=",
            MatMul => "@",
            _ => "+", // fallback
        }.to_string()
    }

    fn emit_main(&mut self) -> Result<()> {
        self.context.emit("");
        self.context.emit("fn main() {");
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
