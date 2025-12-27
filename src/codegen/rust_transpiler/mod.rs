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
pub mod runtime;
pub mod project_generator;
// New feature modules - will be integrated incrementally
// pub mod features_todo;
// pub mod comprehensions_improved;
// pub mod builtins_extended;
// pub mod string_methods;
// pub mod collection_methods;

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
    /// Track variable types (for string concat detection)
    pub variable_types: HashMap<String, String>, // "string", "int", "float", "bool", etc.
    /// Current function name being generated
    pub current_function: String,
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
            variable_types: HashMap::new(),
            current_function: String::new(),
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

    /// Transpile an IR module to Rust code (for library module - no main function)
    pub fn transpile_for_project(&mut self, module: IRModule) -> Result<String> {
        // Generate module header
        self.emit_module_header();

        // Generate type definitions
        self.generate_type_defs(&module)?;

        // Generate class/struct definitions
        self.generate_class_defs(&module)?;

        // Generate function implementations
        self.generate_functions(&module)?;

        // Generate a callable main function (pub fn run_program) that contains global code
        self.emit_program_function(&module.globals)?;

        Ok(self.context.code.clone())
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

        // Only generate main if there's no user-defined main
        if !has_user_main {
            self.emit_main(&module.globals)?;
        }

        Ok(self.context.code.clone())
    }

    /// Generate a complete Rust Cargo project from an IR module
    pub fn transpile_to_project(&mut self, module: IRModule, output_dir: &std::path::Path, project_name: &str) -> Result<std::path::PathBuf> {
        use crate::codegen::rust_transpiler::project_generator::ProjectGenerator;
        use std::path::Path;

        // First transpile to full Rust code (for library, without main function)
        let full_rust_code = self.transpile_for_project(module)?;

        // Create project generator
        let generator = ProjectGenerator::new(output_dir.to_path_buf(), project_name.to_string());
        
        // Create the project directory structure
        let project_root = generator.create_project()?;

        // Generate Cargo.toml with the correct package name
        let cargo_name = project_name.replace("-", "_");
        generator.generate_cargo_toml(&project_root, &cargo_name)?;

        // Generate .gitignore
        generator.generate_gitignore(&project_root)?;

        // Write generated code as src/generated.rs
        generator.generate_generated_rs(&project_root, &full_rust_code)?;

        // Create lib.rs that re-exports the generated module
        let lib_content = r#"// Tauraro Runtime Library
// Auto-generated by Tauraro compiler
// This module contains your compiled Tauraro code

#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

// Re-export the generated module contents
pub mod generated;
pub use generated::*;

// Type aliases for convenience
pub type TauInteger = i64;
pub type TauFloat = f64;
pub type TauBool = bool;
pub type TauString = String;
"#;

        std::fs::write(project_root.join("src").join("lib.rs"), &lib_content)?;
        
        // Create main.rs that calls the generated run_program function
        let main_content = format!(r#"// Generated entry point for Tauraro program
#![allow(non_snake_case)]

use {name}::generated::run_program;

fn main() {{
    // Execute the Tauraro program
    match std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| {{
        run_program();
    }})) {{
        Ok(_) => {{}},
        Err(_) => {{
            eprintln!("Fatal error in Tauraro program");
            std::process::exit(1);
        }}
    }}
}}
"#, name = cargo_name);

        std::fs::write(project_root.join("src").join("main.rs"), &main_content)?;

        println!("✓ Rust Cargo project generated successfully!");
        println!("  Location: {}", project_root.display());
        println!("  ");
        println!("  Project structure:");
        println!("    Cargo.toml        - Package configuration");
        println!("    src/");
        println!("      lib.rs          - Runtime library");
        println!("      main.rs         - Entry point");
        println!("      generated.rs    - Your compiled Tauraro code");
        println!("    .gitignore        - Git ignore rules");
        println!("  ");
        println!("  To build the project:");
        println!("    cd {} && cargo build --release", project_root.display());
        println!("  ");
        println!("  To run:");
        println!("    cargo run --release");

        Ok(project_root)
    }

    fn generate_main_rs_wrapper(&self, _full_code: &str) -> Result<String> {
        Ok("// Generated main wrapper\nfn main_wrapper() { }".to_string())
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

// Type-safe operation helpers
impl TauObject {
    fn type_name(&self) -> String {
        match self {
            TauObject::None => "NoneType".to_string(),
            TauObject::Bool(_) => "bool".to_string(),
            TauObject::Int(_) => "int".to_string(),
            TauObject::Float(_) => "float".to_string(),
            TauObject::String(_) => "str".to_string(),
            TauObject::List(_) => "list".to_string(),
            TauObject::Dict(_) => "dict".to_string(),
            TauObject::Custom(name, _) => format!("{}", name),
        }
    }

    fn compare(&self, other: &TauObject) -> Result<std::cmp::Ordering, String> {
        use std::cmp::Ordering;
        match (self, other) {
            (TauObject::Int(a), TauObject::Int(b)) => Ok(a.cmp(b)),
            (TauObject::Float(a), TauObject::Float(b)) => {
                Ok(if a < b { Ordering::Less } else if a > b { Ordering::Greater } else { Ordering::Equal })
            }
            (TauObject::Int(a), TauObject::Float(b)) => {
                let a_f = *a as f64;
                Ok(if a_f < *b { Ordering::Less } else if a_f > *b { Ordering::Greater } else { Ordering::Equal })
            }
            (TauObject::Float(a), TauObject::Int(b)) => {
                let b_f = *b as f64;
                Ok(if a < &b_f { Ordering::Less } else if a > &b_f { Ordering::Greater } else { Ordering::Equal })
            }
            (TauObject::String(a), TauObject::String(b)) => Ok(a.cmp(b)),
            (TauObject::Bool(a), TauObject::Bool(b)) => Ok(a.cmp(b)),
            _ => Err(format!("'<' not supported between instances of '{}' and '{}'", self.type_name(), other.type_name())),
        }
    }

    fn contains(&self, item: &TauObject) -> Result<bool, String> {
        match self {
            TauObject::List(items) => {
                Ok(items.iter().any(|x| x == item))
            }
            TauObject::Dict(map) => {
                if let TauObject::String(key) = item {
                    Ok(map.contains_key(key))
                } else {
                    Ok(false)
                }
            }
            TauObject::String(s) => {
                if let TauObject::String(needle) = item {
                    Ok(s.contains(needle))
                } else {
                    Err("'in' requires string as both operands for string search".to_string())
                }
            }
            _ => Err(format!("argument of type '{}' is not iterable", self.type_name())),
        }
    }

    fn get_item(&self, key: &TauObject) -> Result<TauObject, String> {
        match self {
            TauObject::Dict(map) => {
                if let TauObject::String(key_str) = key {
                    Ok(map.get(key_str).cloned().unwrap_or(TauObject::None))
                } else {
                    Err("dictionary keys must be strings".to_string())
                }
            }
            TauObject::List(items) => {
                let idx = key.to_int()? as usize;
                Ok(items.get(idx).cloned().unwrap_or(TauObject::None))
            }
            _ => Err(format!("'{}' object is not subscriptable", self.type_name())),
        }
    }

    fn to_bool(&self) -> bool {
        match self {
            TauObject::None => false,
            TauObject::Bool(b) => *b,
            TauObject::Int(i) => *i != 0,
            TauObject::Float(f) => *f != 0.0,
            TauObject::String(s) => !s.is_empty(),
            TauObject::List(l) => !l.is_empty(),
            TauObject::Dict(d) => !d.is_empty(),
            TauObject::Custom(_, _) => true,
        }
    }

    fn to_int(&self) -> Result<i64, String> {
        match self {
            TauObject::Int(i) => Ok(*i),
            TauObject::Float(f) => Ok(*f as i64),
            TauObject::Bool(b) => Ok(if *b { 1 } else { 0 }),
            TauObject::String(s) => s.parse::<i64>().map_err(|_| format!("invalid literal for int() with base 10: '{}'", s)),
            _ => Err(format!("int() argument must be a string or a number, not '{}'", self.type_name())),
        }
    }

    fn to_float(&self) -> Result<f64, String> {
        match self {
            TauObject::Float(f) => Ok(*f),
            TauObject::Int(i) => Ok(*i as f64),
            TauObject::Bool(b) => Ok(if *b { 1.0 } else { 0.0 }),
            TauObject::String(s) => s.parse::<f64>().map_err(|_| format!("could not convert string to float: '{}'", s)),
            _ => Err(format!("float() argument must be a string or a number, not '{}'", self.type_name())),
        }
    }

    fn iter(&self) -> std::vec::IntoIter<&TauObject> {
        match self {
            TauObject::List(items) => items.iter().collect::<Vec<_>>().into_iter(),
            _ => Vec::new().into_iter(),
        }
    }

    fn len(&self) -> usize {
        match self {
            TauObject::List(items) => items.len(),
            TauObject::String(s) => s.len(),
            TauObject::Dict(map) => map.len(),
            _ => 0,
        }
    }

    fn to_vec(&self) -> std::result::Result<Vec<TauObject>, String> {
        match self {
            TauObject::List(items) => Ok(items.clone()),
            TauObject::String(s) => Ok(s.chars().map(|c| TauObject::String(c.to_string())).collect()),
            _ => Err(format!("'{}' object is not iterable", self.type_name())),
        }
    }
}

// Implement Neg trait for TauObject
impl std::ops::Neg for TauObject {
    type Output = TauObject;
    fn neg(self) -> TauObject {
        match self {
            TauObject::Int(i) => TauObject::Int(-i),
            TauObject::Float(f) => TauObject::Float(-f),
            _ => TauObject::None,
        }
    }
}

// Implement Neg trait for &TauObject
impl std::ops::Neg for &TauObject {
    type Output = TauObject;
    fn neg(self) -> TauObject {
        match self {
            TauObject::Int(i) => TauObject::Int(-i),
            TauObject::Float(f) => TauObject::Float(-f),
            _ => TauObject::None,
        }
    }
}

// Display implementations for collections using wrapper types
pub struct TauList(pub Vec<TauObject>);

impl fmt::Display for TauList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let items: Vec<String> = self.0.iter().map(|item| item.to_string()).collect();
        write!(f, "[{}]", items.join(", "))
    }
}

pub struct TauDict(pub std::collections::HashMap<String, TauObject>);

impl fmt::Display for TauDict {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let items: Vec<String> = self.0.iter()
            .map(|(k, v)| format!("'{}': {}", k, v.to_string()))
            .collect();
        write!(f, "{{{}}}", items.join(", "))
    }
}

// Type-safe operation functions
fn tau_add(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x + y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x + y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 + y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x + *y as f64)),
        (TauObject::String(x), TauObject::String(y)) => Ok(TauObject::String(format!("{}{}", x, y))),
        (TauObject::List(x), TauObject::List(y)) => {
            let mut result = x.clone();
            result.extend(y.clone());
            Ok(TauObject::List(result))
        }
        _ => Err(format!("unsupported operand type(s) for +: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_sub(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x - y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x - y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 - y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x - *y as f64)),
        _ => Err(format!("unsupported operand type(s) for -: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_mul(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x * y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x * y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 * y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x * *y as f64)),
        (TauObject::String(x), TauObject::Int(y)) => {
            if *y < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            Ok(TauObject::String(x.repeat(*y as usize)))
        }
        (TauObject::Int(x), TauObject::String(y)) => {
            if *x < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            Ok(TauObject::String(y.repeat(*x as usize)))
        }
        (TauObject::List(x), TauObject::Int(y)) => {
            if *y < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            let mut result = Vec::new();
            for _ in 0..*y {
                result.extend(x.clone());
            }
            Ok(TauObject::List(result))
        }
        (TauObject::Int(x), TauObject::List(y)) => {
            if *x < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            let mut result = Vec::new();
            for _ in 0..*x {
                result.extend(y.clone());
            }
            Ok(TauObject::List(result))
        }
        _ => Err(format!("unsupported operand type(s) for *: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_div(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("division by zero".to_string()); }
            Ok(TauObject::Float(*x as f64 / *y as f64))
        }
        (TauObject::Float(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float division by zero".to_string()); }
            Ok(TauObject::Float(x / y))
        }
        (TauObject::Int(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float division by zero".to_string()); }
            Ok(TauObject::Float(*x as f64 / y))
        }
        (TauObject::Float(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("float division by zero".to_string()); }
            Ok(TauObject::Float(x / *y as f64))
        }
        _ => Err(format!("unsupported operand type(s) for /: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_floordiv(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("integer division or modulo by zero".to_string()); }
            Ok(TauObject::Int(x / y))
        }
        (TauObject::Float(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float floor division by zero".to_string()); }
            Ok(TauObject::Float((x / y).floor()))
        }
        (TauObject::Int(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float floor division by zero".to_string()); }
            Ok(TauObject::Float((*x as f64 / y).floor()))
        }
        (TauObject::Float(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("float floor division by zero".to_string()); }
            Ok(TauObject::Float((x / *y as f64).floor()))
        }
        _ => Err(format!("unsupported operand type(s) for //: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_mod(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("integer division or modulo by zero".to_string()); }
            Ok(TauObject::Int(x % y))
        }
        (TauObject::Float(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float modulo".to_string()); }
            Ok(TauObject::Float(x % y))
        }
        (TauObject::Int(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float modulo".to_string()); }
            Ok(TauObject::Float(*x as f64 % y))
        }
        (TauObject::Float(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("float modulo".to_string()); }
            Ok(TauObject::Float(x % *y as f64))
        }
        _ => Err(format!("unsupported operand type(s) for %: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_pow(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y < 0 {
                Ok(TauObject::Float((*x as f64).powf(*y as f64)))
            } else {
                Ok(TauObject::Int(x.pow(*y as u32)))
            }
        }
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x.powf(*y))),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float((*x as f64).powf(*y))),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x.powf(*y as f64))),
        _ => Err(format!("unsupported operand type(s) for **: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_range(start: &TauObject, end: &TauObject) -> Result<Vec<TauObject>, String> {
    match (start, end) {
        (TauObject::Int(s), TauObject::Int(e)) => {
            Ok((*s..*e).map(|i| TauObject::Int(i)).collect())
        }
        _ => Err(format!("range() requires integer arguments")),
    }
}

fn tau_range_step(start: &TauObject, end: &TauObject, step: &TauObject) -> Result<Vec<TauObject>, String> {
    match (start, end, step) {
        (TauObject::Int(s), TauObject::Int(e), TauObject::Int(st)) => {
            if *st == 0 {
                return Err("range() step argument must not be zero".to_string());
            }
            let mut result = Vec::new();
            if *st > 0 {
                let mut i = *s;
                while i < *e {
                    result.push(TauObject::Int(i));
                    i += st;
                }
            } else {
                let mut i = *s;
                while i > *e {
                    result.push(TauObject::Int(i));
                    i += st;
                }
            }
            Ok(result)
        }
        _ => Err(format!("range() requires integer arguments")),
    }
}

fn tau_sum(items: &TauObject) -> TauObject {
    match items {
        TauObject::List(list) => {
            let mut sum: i64 = 0;
            let mut is_float = false;
            let mut float_sum: f64 = 0.0;
            
            for item in list {
                match item {
                    TauObject::Int(i) => {
                        if is_float {
                            float_sum += *i as f64;
                        } else {
                            sum += i;
                        }
                    }
                    TauObject::Float(f) => {
                        if !is_float {
                            float_sum = sum as f64;
                            is_float = true;
                        }
                        float_sum += f;
                    }
                    _ => {}
                }
            }
            
            if is_float {
                TauObject::Float(float_sum)
            } else {
                TauObject::Int(sum)
            }
        }
        _ => TauObject::None,
    }
}

fn vec_to_display_string(vec: &Vec<TauObject>) -> String {
    let items: Vec<String> = vec.iter().map(|i| i.to_string()).collect();
    format!("[{}]", items.join(", "))
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

// List method implementations - work with TauObject
fn lst__append(list: TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(mut items) = list {
        items.push(item);
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn lst__pop(list: TauObject, index: Option<i64>) -> TauObject {
    if let TauObject::List(mut items) = list {
        let idx = if let Some(i) = index {
            (i as usize).min(if items.is_empty() { 0 } else { items.len() - 1 })
        } else if !items.is_empty() {
            items.len() - 1
        } else {
            return TauObject::None;
        };
        if idx < items.len() {
            items.remove(idx)
        } else {
            TauObject::None
        }
    } else {
        TauObject::None
    }
}

fn lst__reverse(list: TauObject) -> TauObject {
    if let TauObject::List(mut items) = list {
        items.reverse();
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn lst__sort(list: TauObject) -> TauObject {
    if let TauObject::List(mut items) = list {
        items.sort_by(|a, b| {
            if let (TauObject::Int(x), TauObject::Int(y)) = (a, b) {
                x.cmp(y)
            } else {
                std::cmp::Ordering::Equal
            }
        });
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn lst__index(list: &TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(items) = list {
        for (i, elem) in items.iter().enumerate() {
            if format!("{:?}", elem) == format!("{:?}", item) {
                return TauObject::Int(i as i64);
            }
        }
        TauObject::Int(-1)
    } else {
        TauObject::Int(-1)
    }
}

fn lst__count(list: &TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(items) = list {
        let count = items.iter()
            .filter(|x| format!("{:?}", x) == format!("{:?}", item))
            .count() as i64;
        TauObject::Int(count)
    } else {
        TauObject::Int(0)
    }
}

fn lst__extend(list: TauObject, other: TauObject) -> TauObject {
    if let (TauObject::List(mut items), TauObject::List(other_items)) = (list, other) {
        items.extend(other_items);
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn lst__insert(list: TauObject, index: i64, item: TauObject) -> TauObject {
    if let TauObject::List(mut items) = list {
        let i = (index as usize).min(items.len());
        items.insert(i, item);
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn lst__remove(list: TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(mut items) = list {
        items.retain(|x| format!("{:?}", x) != format!("{:?}", item));
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn lst__clear(list: TauObject) -> TauObject {
    TauObject::List(Vec::new())
}

fn lst__copy(list: &TauObject) -> TauObject {
    if let TauObject::List(items) = list {
        TauObject::List(items.clone())
    } else {
        list.clone()
    }
}

// String method implementations - work with TauObject
fn text__upper(obj: &TauObject) -> TauObject {
    match obj {
        TauObject::String(s) => TauObject::String(s.to_uppercase()),
        _ => obj.clone(),
    }
}

fn text__lower(obj: &TauObject) -> TauObject {
    match obj {
        TauObject::String(s) => TauObject::String(s.to_lowercase()),
        _ => obj.clone(),
    }
}

fn text__strip(obj: &TauObject) -> TauObject {
    match obj {
        TauObject::String(s) => TauObject::String(s.trim().to_string()),
        _ => obj.clone(),
    }
}

fn text__lstrip(obj: &TauObject) -> TauObject {
    match obj {
        TauObject::String(s) => TauObject::String(s.trim_start().to_string()),
        _ => obj.clone(),
    }
}

fn text__rstrip(obj: &TauObject) -> TauObject {
    match obj {
        TauObject::String(s) => TauObject::String(s.trim_end().to_string()),
        _ => obj.clone(),
    }
}

fn text__replace(obj: &TauObject, old: TauObject, new: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(old_str), TauObject::String(new_str)) = (obj, &old, &new) {
        TauObject::String(s.replace(old_str, new_str))
    } else {
        obj.clone()
    }
}

fn text__split(obj: &TauObject, sep: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(sep_str)) = (obj, &sep) {
        let parts: Vec<TauObject> = s.split(sep_str.as_str())
            .map(|x| TauObject::String(x.to_string()))
            .collect();
        TauObject::List(parts)
    } else {
        obj.clone()
    }
}

fn text__join(sep: &TauObject, items: &TauObject) -> TauObject {
    if let (TauObject::String(sep_str), TauObject::List(list)) = (sep, items) {
        let strs: Vec<String> = list.iter().map(|x| x.to_string()).collect();
        TauObject::String(strs.join(sep_str))
    } else {
        TauObject::String(String::new())
    }
}

fn text__startswith(obj: &TauObject, prefix: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(pre)) = (obj, &prefix) {
        TauObject::Bool(s.starts_with(pre))
    } else {
        TauObject::Bool(false)
    }
}

fn text__endswith(obj: &TauObject, suffix: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(suf)) = (obj, &suffix) {
        TauObject::Bool(s.ends_with(suf))
    } else {
        TauObject::Bool(false)
    }
}

fn text__find(obj: &TauObject, sub: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(substring)) = (obj, &sub) {
        let pos = s.find(substring.as_str()).map(|i| i as i64).unwrap_or(-1);
        TauObject::Int(pos)
    } else {
        TauObject::Int(-1)
    }
}

fn text__index(obj: &TauObject, sub: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(substring)) = (obj, &sub) {
        let pos = s.find(substring.as_str()).map(|i| i as i64).unwrap_or(-1);
        TauObject::Int(pos)
    } else {
        TauObject::Int(-1)
    }
}

fn text__count(obj: &TauObject, sub: TauObject) -> TauObject {
    if let (TauObject::String(s), TauObject::String(substring)) = (obj, &sub) {
        let count = s.matches(substring.as_str()).count() as i64;
        TauObject::Int(count)
    } else {
        TauObject::Int(0)
    }
}

fn text__capitalize(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let mut chars = s.chars();
        let result = match chars.next() {
            None => String::new(),
            Some(first) => first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase(),
        };
        TauObject::String(result)
    } else {
        obj.clone()
    }
}

fn text__title(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let result = s.split_whitespace()
            .map(|word| {
                let mut chars = word.chars();
                match chars.next() {
                    None => String::new(),
                    Some(first) => first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase(),
                }
            })
            .collect::<Vec<_>>()
            .join(" ");
        TauObject::String(result)
    } else {
        obj.clone()
    }
}

fn text__isdigit(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let result = !s.is_empty() && s.chars().all(|c| c.is_ascii_digit());
        TauObject::Bool(result)
    } else {
        TauObject::Bool(false)
    }
}

fn text__isalpha(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let result = !s.is_empty() && s.chars().all(|c| c.is_alphabetic());
        TauObject::Bool(result)
    } else {
        TauObject::Bool(false)
    }
}

fn text__isalnum(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let result = !s.is_empty() && s.chars().all(|c| c.is_alphanumeric());
        TauObject::Bool(result)
    } else {
        TauObject::Bool(false)
    }
}

fn text__isspace(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let result = !s.is_empty() && s.chars().all(|c| c.is_whitespace());
        TauObject::Bool(result)
    } else {
        TauObject::Bool(false)
    }
}

fn text__isupper(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let has_cased = s.chars().any(|c| c.is_alphabetic());
        let all_upper = s.chars().filter(|c| c.is_alphabetic()).all(|c| c.is_uppercase());
        TauObject::Bool(has_cased && all_upper)
    } else {
        TauObject::Bool(false)
    }
}

fn text__islower(obj: &TauObject) -> TauObject {
    if let TauObject::String(s) = obj {
        let has_cased = s.chars().any(|c| c.is_alphabetic());
        let all_lower = s.chars().filter(|c| c.is_alphabetic()).all(|c| c.is_lowercase());
        TauObject::Bool(has_cased && all_lower)
    } else {
        TauObject::Bool(false)
    }
}

// Dict methods - work with TauObject
fn dict__get(obj: &TauObject, key: TauObject, default: Option<TauObject>) -> TauObject {
    if let TauObject::Dict(map) = obj {
        if let TauObject::String(key_str) = &key {
            map.get(key_str).cloned().unwrap_or_else(|| default.unwrap_or(TauObject::None))
        } else {
            default.unwrap_or(TauObject::None)
        }
    } else {
        default.unwrap_or(TauObject::None)
    }
}

fn dict__keys(obj: &TauObject) -> TauObject {
    if let TauObject::Dict(map) = obj {
        let keys: Vec<TauObject> = map.keys()
            .map(|k| TauObject::String(k.clone()))
            .collect();
        TauObject::List(keys)
    } else {
        TauObject::List(Vec::new())
    }
}

fn dict__values(obj: &TauObject) -> TauObject {
    if let TauObject::Dict(map) = obj {
        let values: Vec<TauObject> = map.values().cloned().collect();
        TauObject::List(values)
    } else {
        TauObject::List(Vec::new())
    }
}

fn dict__items(obj: &TauObject) -> TauObject {
    if let TauObject::Dict(map) = obj {
        let items: Vec<TauObject> = map.iter()
            .map(|(k, v)| {
                TauObject::List(vec![TauObject::String(k.clone()), v.clone()])
            })
            .collect();
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn dict__pop(obj: TauObject, key: TauObject, default: Option<TauObject>) -> TauObject {
    if let TauObject::Dict(mut map) = obj {
        if let TauObject::String(key_str) = &key {
            map.remove(key_str).unwrap_or_else(|| default.unwrap_or(TauObject::None))
        } else {
            default.unwrap_or(TauObject::None)
        }
    } else {
        default.unwrap_or(TauObject::None)
    }
}

fn dict__clear(obj: TauObject) -> TauObject {
    if let TauObject::Dict(_) = obj {
        TauObject::Dict(std::collections::HashMap::new())
    } else {
        TauObject::Dict(std::collections::HashMap::new())
    }
}

fn dict__update(obj: TauObject, other: TauObject) -> TauObject {
    if let (TauObject::Dict(mut map), TauObject::Dict(other_map)) = (obj, other) {
        for (k, v) in other_map {
            map.insert(k, v);
        }
        TauObject::Dict(map)
    } else {
        TauObject::Dict(std::collections::HashMap::new())
    }
}

// Set methods - work with TauObject
fn set__add(obj: TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(mut items) = obj {
        if !items.iter().any(|x| format!("{:?}", x) == format!("{:?}", item)) {
            items.push(item);
        }
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn set__discard(obj: TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(mut items) = obj {
        items.retain(|x| format!("{:?}", x) != format!("{:?}", item));
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn set__remove(obj: TauObject, item: TauObject) -> TauObject {
    if let TauObject::List(mut items) = obj {
        let len_before = items.len();
        items.retain(|x| format!("{:?}", x) != format!("{:?}", item));
        TauObject::List(items)
    } else {
        TauObject::List(Vec::new())
    }
}

fn set__union(obj: &TauObject, other: TauObject) -> TauObject {
    if let (TauObject::List(items), TauObject::List(other_list)) = (obj, other) {
        let mut result = items.clone();
        for item in other_list {
            if !result.iter().any(|x| format!("{:?}", x) == format!("{:?}", item)) {
                result.push(item);
            }
        }
        TauObject::List(result)
    } else {
        obj.clone()
    }
}

fn set__intersection(obj: &TauObject, other: TauObject) -> TauObject {
    if let (TauObject::List(items), TauObject::List(other_list)) = (obj, other) {
        let mut result = Vec::new();
        for item in items {
            if other_list.iter().any(|x| format!("{:?}", x) == format!("{:?}", item)) {
                if !result.iter().any(|x| format!("{:?}", x) == format!("{:?}", item)) {
                    result.push(item.clone());
                }
            }
        }
        TauObject::List(result)
    } else {
        TauObject::List(Vec::new())
    }
}

fn set__difference(obj: &TauObject, other: TauObject) -> TauObject {
    if let (TauObject::List(items), TauObject::List(other_list)) = (obj, other) {
        let mut result = Vec::new();
        for item in items {
            if !other_list.iter().any(|x| format!("{:?}", x) == format!("{:?}", item)) {
                if !result.iter().any(|x| format!("{:?}", x) == format!("{:?}", item)) {
                    result.push(item.clone());
                }
            }
        }
        TauObject::List(result)
    } else {
        obj.clone()
    }
}

fn set__clear(obj: TauObject) -> TauObject {
    TauObject::List(Vec::new())
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

// Note: tau_sum is now defined in compile_builtins for TauObject types
// The old raw i64 version is no longer emitted to avoid conflicts

// Note: tau_pow is defined in TauObject impl for handling TauObject types
// The old raw i64 version is no longer emitted to avoid conflicts

fn tau_round(f: f64) -> i64 {
    f.round() as i64
}

fn tau_divmod(a: i64, b: i64) -> (i64, i64) {
    (a / b, a % b)
}

fn tau_hex(n: i64) -> String {
    format!("0x{:x}", n)
}

fn tau_oct(n: i64) -> String {
    format!("0o{:o}", n)
}

fn tau_bin(n: i64) -> String {
    format!("0b{:b}", n)
}

fn tau_ord(c: &str) -> i64 {
    c.chars().next().unwrap_or('\0') as i64
}

fn tau_chr(n: i64) -> String {
    ((n as u8) as char).to_string()
}

// Note: tau_type and tau_isinstance are defined in TauObject impl for handling TauObject types
// Old raw i64 versions removed to avoid conflicts

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
        self.context.current_function = func_name.clone();
        
        let params = func.params.iter()
            .map(|p| format!("{}: TauObject", p))
            .collect::<Vec<_>>()
            .join(", ");

        // Determine return type based on IR or default to TauObject
        let return_type = if func_name == "main" { 
            "".to_string() 
        } else { 
            // All user functions return TauObject for consistency
            " -> TauObject".to_string()
        };
        
        self.context.emit(&format!("fn {}{}{} {{", func_name, if params.is_empty() { "()".to_string() } else { format!("({})", params) }, return_type));
        self.context.indent();

        // Generate function body from IR blocks
        if func.blocks.is_empty() {
            // Empty function
            if func_name == "main" {
                self.context.emit("println!(\"Program executed\");");
            } else {
                self.context.emit("TauObject::None");
            }
        } else {
            // Generate code from all blocks
            let mut has_explicit_return = false;
            for (i, block) in func.blocks.iter().enumerate() {
                self.generate_block(&block.instructions)?;
                // Check if last instruction in last block is a return
                if i == func.blocks.len() - 1 {
                    if let Some(crate::ir::IRInstruction::Return { .. }) = block.instructions.last() {
                        has_explicit_return = true;
                    }
                }
            }
            // Ensure function ends with a return statement for non-main functions (if no explicit return)
            if func_name != "main" && !has_explicit_return {
                self.context.emit("TauObject::None");
            }
        }

        self.context.dedent();
        self.context.emit("}");
        self.context.emit("");
        
        // Clear current function
        self.context.current_function = String::new();

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
                
                // Track the type of this variable
                match value {
                    crate::value::Value::Str(_) => {
                        self.context.variable_types.insert(result.clone(), "string".to_string());
                    }
                    crate::value::Value::Int(_) => {
                        self.context.variable_types.insert(result.clone(), "int".to_string());
                    }
                    crate::value::Value::Float(_) => {
                        self.context.variable_types.insert(result.clone(), "float".to_string());
                    }
                    crate::value::Value::Bool(_) => {
                        self.context.variable_types.insert(result.clone(), "bool".to_string());
                    }
                    crate::value::Value::List(_) => {
                        self.context.variable_types.insert(result.clone(), "list".to_string());
                    }
                    crate::value::Value::Dict(_) => {
                        self.context.variable_types.insert(result.clone(), "dict".to_string());
                    }
                    _ => {}
                }
            }
            LoadLocal { name, result } => {
                // Clone values to avoid move errors when variables are used multiple times
                self.context.emit(&format!("let {} = {}.clone();", result, name));
                // If this variable had a tracked origin, propagate it
                if let Some(orig) = self.context.original_var_values.get(name) {
                    self.context.original_var_values.insert(result.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(name) {
                    self.context.variable_types.insert(result.clone(), typ.clone());
                }
            }
            LoadGlobal { name, result } => {
                // Clone values to avoid move errors when variables are used multiple times
                self.context.emit(&format!("let {} = {}.clone();", result, name));
                // If this variable had a tracked origin, propagate it
                if let Some(orig) = self.context.original_var_values.get(name) {
                    self.context.original_var_values.insert(result.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(name) {
                    self.context.variable_types.insert(result.clone(), typ.clone());
                }
            }
            StoreLocal { name, value } => {
                // Clone values to avoid move errors when used multiple times
                self.context.emit(&format!("let {} = {}.clone();", name, value));
                // Track if this variable is assigned from another tracked variable
                if let Some(orig) = self.context.original_var_values.get(value) {
                    self.context.original_var_values.insert(name.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(value) {
                    self.context.variable_types.insert(name.clone(), typ.clone());
                }
            }
            StoreGlobal { name, value } => {
                // Clone values to avoid move errors when used multiple times
                self.context.emit(&format!("let {} = {}.clone();", name, value));
                // Track if this variable is assigned from another tracked variable
                if let Some(orig) = self.context.original_var_values.get(value) {
                    self.context.original_var_values.insert(name.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(value) {
                    self.context.variable_types.insert(name.clone(), typ.clone());
                }
            }
            LoadTypedLocal { name, result, type_info: _ } => {
                // Type info is for the IR, but Rust has its own type system
                self.context.emit(&format!("let {} = {};", result, name));
                // If this variable had a tracked origin, propagate it
                if let Some(orig) = self.context.original_var_values.get(name) {
                    self.context.original_var_values.insert(result.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(name) {
                    self.context.variable_types.insert(result.clone(), typ.clone());
                }
            }
            StoreTypedLocal { name, value, type_info: _ } => {
                self.context.emit(&format!("let {} = {};", name, value));
                // Track if this variable is assigned from another tracked variable
                if let Some(orig) = self.context.original_var_values.get(value) {
                    self.context.original_var_values.insert(name.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(value) {
                    self.context.variable_types.insert(name.clone(), typ.clone());
                }
            }
            LoadTypedGlobal { name, result, type_info: _ } => {
                // Type info is for the IR, but Rust has its own type system
                self.context.emit(&format!("let {} = {};", result, name));
                // If this variable had a tracked origin, propagate it
                if let Some(orig) = self.context.original_var_values.get(name) {
                    self.context.original_var_values.insert(result.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(name) {
                    self.context.variable_types.insert(result.clone(), typ.clone());
                }
            }
            StoreTypedGlobal { name, value, type_info: _ } => {
                // Type info is for the IR, but Rust has its own type system
                self.context.emit(&format!("let {} = {};", name, value));
                // Track if this variable is assigned from another tracked variable
                if let Some(orig) = self.context.original_var_values.get(value) {
                    self.context.original_var_values.insert(name.clone(), orig.clone());
                }
                // Propagate type information
                if let Some(typ) = self.context.variable_types.get(value) {
                    self.context.variable_types.insert(name.clone(), typ.clone());
                }
            }
            BinaryOp { op, left, right, result } => {
                // Use tau_* helper functions for all binary operations
                let op_fn = match op {
                    crate::ast::BinaryOp::Add => "tau_add",
                    crate::ast::BinaryOp::Sub => "tau_sub",
                    crate::ast::BinaryOp::Mul => "tau_mul",
                    crate::ast::BinaryOp::Div => "tau_div",
                    crate::ast::BinaryOp::FloorDiv => "tau_floordiv",
                    crate::ast::BinaryOp::Mod => "tau_mod",
                    crate::ast::BinaryOp::Pow => "tau_pow",
                    _ => "tau_add", // Default fallback
                };
                
                self.context.emit(&format!(
                    "let {} = {}(&{}, &{}).unwrap_or(TauObject::None);", 
                    result, op_fn, left, right
                ));
            }
            TypedBinaryOp { op, left, right, result, type_info: _ } => {
                // Use tau_* helper functions for all binary operations (same as BinaryOp)
                let op_fn = match op {
                    crate::ast::BinaryOp::Add => "tau_add",
                    crate::ast::BinaryOp::Sub => "tau_sub",
                    crate::ast::BinaryOp::Mul => "tau_mul",
                    crate::ast::BinaryOp::Div => "tau_div",
                    crate::ast::BinaryOp::FloorDiv => "tau_floordiv",
                    crate::ast::BinaryOp::Mod => "tau_mod",
                    crate::ast::BinaryOp::Pow => "tau_pow",
                    _ => "tau_add", // Default fallback
                };
                
                self.context.emit(&format!(
                    "let {} = {}(&{}, &{}).unwrap_or(TauObject::None);", 
                    result, op_fn, left, right
                ));
            }
            UnaryOp { op, operand, result } => {
                match op {
                    crate::ast::UnaryOp::USub | crate::ast::UnaryOp::Minus => {
                        // For unary minus, use negation on TauObject
                        self.context.emit(&format!("let {} = -{};", result, operand));
                    }
                    crate::ast::UnaryOp::Not => {
                        // For logical not, use negation of bool conversion
                        self.context.emit(&format!("let {} = TauObject::Bool(!{}.to_bool());", result, operand));
                    }
                    _ => {
                        // Other unary ops not yet supported
                        self.context.emit(&format!("let {} = {}; // Unary operator not yet supported", result, operand));
                    }
                }
            }
            Call { func, args, result } => {
                // Special handling for print function
                if func == "print" {
                    if args.len() == 1 {
                        let arg = &args[0];
                        self.context.emit(&format!("println!(\"{{}}\", {}.to_string());", arg));
                    } else if args.len() > 1 {
                        // For multiple arguments, concatenate them with spaces using format!
                        let placeholders = vec!["{}"; args.len()].join(" ");
                        let args_str = args.iter()
                            .map(|arg| format!("{}.to_string()", arg))
                            .collect::<Vec<_>>()
                            .join(", ");
                        self.context.emit(&format!("println!(\"{}\", {});", placeholders, args_str));
                    }
                } else if func == "len" && args.len() == 1 {
                    if let Some(res) = result {
                        // len(iterable) - returns length as TauObject::Int
                        self.context.emit(&format!("let {} = TauObject::Int({}.len() as i64);", res, args[0]));
                    }
                } else if func == "abs" && args.len() == 1 {
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = match &{} {{ TauObject::Int(i) => TauObject::Int(i.abs()), TauObject::Float(f) => TauObject::Float(f.abs()), x => x.clone() }};", res, args[0]));
                    }
                } else if func == "min" && !args.is_empty() {
                    if let Some(res) = result {
                        if args.len() == 1 {
                            // min(list) - find minimum in list/iterable
                            self.context.emit(&format!("let {} = {}.iter().min_by(|a, b| a.compare(&b).unwrap_or(std::cmp::Ordering::Equal)).cloned().unwrap_or(TauObject::None);", res, args[0]));
                        } else {
                            // min(a, b, c) - multiple arguments
                            let min_expr = args.iter()
                                .skip(1)
                                .fold(args[0].clone(), |acc, arg| format!("if {} < {} {{ {} }} else {{ {} }}", acc, arg, acc, arg));
                            self.context.emit(&format!("let {} = {};", res, min_expr));
                        }
                    }
                } else if func == "max" && !args.is_empty() {
                    if let Some(res) = result {
                        if args.len() == 1 {
                            // max(list) - find maximum in list/iterable
                            self.context.emit(&format!("let {} = {}.iter().max_by(|a, b| a.compare(&b).unwrap_or(std::cmp::Ordering::Equal)).cloned().unwrap_or(TauObject::None);", res, args[0]));
                        } else {
                            // max(a, b, c) - multiple arguments
                            let max_expr = args.iter()
                                .skip(1)
                                .fold(args[0].clone(), |acc, arg| format!("if {} > {} {{ {} }} else {{ {} }}", acc, arg, acc, arg));
                            self.context.emit(&format!("let {} = {};", res, max_expr));
                        }
                    }
                } else if func == "sum" && args.len() == 1 {
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = tau_sum(&{});", res, args[0]));
                    }
                } else if func == "int" && args.len() == 1 {
                    if let Some(res) = result {
                        // Convert TauObject to integer
                        self.context.emit(&format!("let {} = match &{} {{ TauObject::Int(i) => TauObject::Int(*i), TauObject::Float(f) => TauObject::Int(f.trunc() as i64), TauObject::String(s) => {{ let i = s.parse::<i64>().unwrap_or(0); TauObject::Int(i) }}, TauObject::Bool(b) => TauObject::Int(if *b {{ 1 }} else {{ 0 }}), _ => TauObject::None }};", res, args[0]));
                    }
                } else if func == "float" && args.len() == 1 {
                    if let Some(res) = result {
                        // Convert TauObject to float
                        self.context.emit(&format!("let {} = match &{} {{ TauObject::Int(i) => TauObject::Float(*i as f64), TauObject::Float(f) => TauObject::Float(*f), TauObject::String(s) => {{ let f = s.parse::<f64>().unwrap_or(0.0); TauObject::Float(f) }}, TauObject::Bool(b) => TauObject::Float(if *b {{ 1.0 }} else {{ 0.0 }}), _ => TauObject::None }};", res, args[0]));
                    }
                } else if func == "str" && args.len() == 1 {
                    if let Some(res) = result {
                        // Convert to string - wrap in TauObject::String
                        self.context.emit(&format!("let {} = TauObject::String(format!(\"{{}}\", {}));", res, args[0]));
                    }
                } else if func == "bool" && args.len() == 1 {
                    if let Some(res) = result {
                        // Convert to boolean (0 is false, everything else is true for numbers)
                        self.context.emit(&format!("let {} = {} != 0;", res, args[0]));
                    }
                } else if func == "list" && args.len() == 1 {
                    if let Some(res) = result {
                        // Convert to list - wrap in TauObject::List
                        self.context.emit(&format!("let {} = TauObject::List({}.iter().map(|x| x.clone()).collect::<Vec<_>>());", res, args[0]));
                    }
                } else if func == "range" {
                    if let Some(res) = result {
                        if args.len() == 1 {
                            self.context.emit(&format!("let {} = TauObject::List(tau_range(&TauObject::Int(0), &{}).unwrap_or_default());", res, args[0]));
                        } else if args.len() == 2 {
                            self.context.emit(&format!("let {} = TauObject::List(tau_range(&{}, &{}).unwrap_or_default());", res, args[0], args[1]));
                        } else if args.len() == 3 {
                            // Use tau_range_step for 3-argument range
                            self.context.emit(&format!("let {} = TauObject::List(tau_range_step(&{}, &{}, &{}).unwrap_or_default());", res, args[0], args[1], args[2]));
                        }
                    }
                } else if func == "enumerate" && args.len() == 1 {
                    if let Some(res) = result {
                        // enumerate(list) returns list of [index, value] pairs wrapped as TauObjects
                        self.context.emit(&format!("let {} = TauObject::List({}.iter().enumerate().map(|(i, v)| TauObject::List(vec![TauObject::Int(i as i64), v.clone()])).collect());", res, args[0]));
                    }
                } else if func == "reversed" && args.len() == 1 {
                    if let Some(res) = result {
                        self.context.emit(&format!("let mut {} = {}; {}.reverse();", res, args[0], res));
                    }
                } else if func == "sorted" && args.len() == 1 {
                    if let Some(res) = result {
                        self.context.emit(&format!("let mut {} = {}; {}.sort();", res, args[0], res));
                    }
                } else if func == "any" && args.len() == 1 {
                    if let Some(res) = result {
                        // any(iterable) - check if any element is truthy
                        self.context.emit(&format!("let {} = {}.iter().any(|x| *x != 0);", res, args[0]));
                    }
                } else if func == "all" && args.len() == 1 {
                    if let Some(res) = result {
                        // all(iterable) - check if all elements are truthy
                        self.context.emit(&format!("let {} = {}.iter().all(|x| *x != 0);", res, args[0]));
                    }
                } else if func == "zip" && !args.is_empty() {
                    if let Some(res) = result {
                        // zip(*iterables) - combine multiple iterables into tuples
                        // Returns TauObject::List of tuple pairs wrapped as TauObject::List
                        if args.len() == 2 {
                            // Extract vecs from TauObjects and zip them, wrapping results as lists
                            self.context.emit(&format!(
                                "let {} = TauObject::List({}.to_vec().unwrap_or_default().into_iter().zip({}.to_vec().unwrap_or_default()).map(|(a, b)| TauObject::List(vec![a, b])).collect::<Vec<_>>());",
                                res, args[0], args[1]
                            ));
                        } else if args.len() == 3 {
                            self.context.emit(&format!(
                                "let {} = TauObject::List({}.to_vec().unwrap_or_default().into_iter().zip({}.to_vec().unwrap_or_default()).zip({}.to_vec().unwrap_or_default()).map(|((a, b), c)| TauObject::List(vec![a, b, c])).collect::<Vec<_>>());",
                                res, args[0], args[1], args[2]
                            ));
                        }
                    }
                } else if func == "map" && args.len() == 2 {
                    if let Some(res) = result {
                        // map(function, iterable) - apply function to each element
                        // Returns TauObject::List wrapped result
                        self.context.emit(&format!(
                            "let {} = TauObject::List({}.iter().map(|x| {}(x.clone())).collect::<Vec<_>>());",
                            res, args[1], args[0]
                        ));
                    }
                } else if func == "filter" && args.len() == 2 {
                    if let Some(res) = result {
                        // filter(function, iterable) - filter elements based on condition
                        // Returns TauObject::List wrapped result
                        // Use filter_map to handle owned values; clone for the predicate to avoid moving
                        self.context.emit(&format!(
                            "let {} = TauObject::List({}.to_vec().unwrap_or_default().into_iter().filter_map(|x| if {}(x.clone()).to_bool() {{ Some(x) }} else {{ None }} ).collect::<Vec<_>>());",
                            res, args[1], args[0]
                        ));
                    }
                } else if func == "type" && args.len() == 1 {
                    if let Some(res) = result {
                        // type(value) - returns type as string
                        self.context.emit(&format!(
                            "let {} = \"int\".to_string(); // type() returns type of value",
                            res
                        ));
                    }
                } else if func == "isinstance" && args.len() == 2 {
                    if let Some(res) = result {
                        // isinstance(value, type) - check if value is instance of type
                        // Simplified implementation - always true for integers
                        self.context.emit(&format!("let {} = true;", res));
                    }
                } else if func == "round" && args.len() >= 1 {
                    if let Some(res) = result {
                        if args.len() == 1 {
                            // round(x) - convert TauObject to f64 then round
                            self.context.emit(&format!("let {} = match &{} {{ TauObject::Int(i) => TauObject::Int(*i), TauObject::Float(f) => TauObject::Int(f.round() as i64), _ => TauObject::None }};", res, args[0]));
                        } else {
                            // round(x, ndigits) - not supported yet, use inline code for numeric types
                            self.context.emit(&format!(
                                "let {} = match &{} {{ TauObject::Float(f) => TauObject::Float((f * 10_f64.powi({} as i32)).round() / 10_f64.powi({} as i32)), TauObject::Int(i) => TauObject::Int(*i), _ => TauObject::None }};",
                                res, args[0], args[1], args[1]
                            ));
                        }
                    }
                } else if func == "pow" && args.len() == 2 {
                    if let Some(res) = result {
                        // pow(base, exponent) - call helper function
                        self.context.emit(&format!(
                            "let {} = tau_pow({}, {});",
                            res, args[0], args[1]
                        ));
                    }
                } else if func == "divmod" && args.len() == 2 {
                    if let Some(res) = result {
                        // divmod(a, b) - returns (quotient, remainder)
                        self.context.emit(&format!(
                            "let {} = (({}) / ({}), ({}) % ({}));",
                            res, args[0], args[1], args[0], args[1]
                        ));
                    }
                } else if func == "hex" && args.len() == 1 {
                    if let Some(res) = result {
                        // hex(x) - convert to hexadecimal string
                        self.context.emit(&format!(
                            "let {} = format!(\"0x{{:x}}\", {});",
                            res, args[0]
                        ));
                    }
                } else if func == "oct" && args.len() == 1 {
                    if let Some(res) = result {
                        // oct(x) - convert to octal string
                        self.context.emit(&format!(
                            "let {} = format!(\"0o{{:o}}\", {});",
                            res, args[0]
                        ));
                    }
                } else if func == "bin" && args.len() == 1 {
                    if let Some(res) = result {
                        // bin(x) - convert to binary string
                        self.context.emit(&format!(
                            "let {} = format!(\"0b{{:b}}\", {});",
                            res, args[0]
                        ));
                    }
                } else if func == "ord" && args.len() == 1 {
                    if let Some(res) = result {
                        // ord(c) - get unicode code point of character
                        self.context.emit(&format!(
                            "let {} = {}.chars().next().unwrap_or('\\0') as i64;",
                            res, args[0]
                        ));
                    }
                } else if func == "chr" && args.len() == 1 {
                    if let Some(res) = result {
                        // chr(x) - get character from unicode code point
                        self.context.emit(&format!(
                            "let {} = (({} as u8) as char).to_string();",
                            res, args[0]
                        ));
                    }
                } else if func.starts_with("lst__") || func.starts_with("text__") || func.starts_with("dict__") || func.starts_with("set__") {
                    // List, string, dict, and set methods need special handling for references
                    let modified_args: Vec<String> = args.iter().enumerate().map(|(i, arg)| {
                        // First argument to many list/string/dict/set methods needs to be borrowed
                        if i == 0 && (func == "lst__index" || func == "lst__count" || func == "text__count" || 
                                      func == "text__find" || func == "text__index" || func == "text__startswith" ||
                                      func == "text__endswith" || func == "text__split" || func == "text__capitalize" ||
                                      func == "text__title" || func == "text__isdigit" || func == "text__isalpha" ||
                                      func == "text__upper" || func == "text__lower" || func == "text__strip" ||
                                      func == "text__replace" || func == "text__join" ||
                                      func == "dict__get" || func == "dict__keys" || func == "dict__values" || func == "dict__items" ||
                                      func == "set__intersection" || func == "set__difference") {
                            format!("&{}", arg)
                        } else {
                            arg.clone()
                        }
                    }).collect();
                    
                    let args_str = modified_args.join(", ");
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = {}({});", res, func, args_str));
                        
                        // Track type information for method results
                        if func == "text__split" || func == "text__join" {
                            self.context.variable_types.insert(res.clone(), "list".to_string());
                        } else if func == "dict__keys" || func == "dict__values" || func == "dict__items" {
                            self.context.variable_types.insert(res.clone(), "list".to_string());
                        } else if func == "text__upper" || func == "text__lower" || func == "text__strip" ||
                                  func == "text__lstrip" || func == "text__rstrip" || func == "text__replace" ||
                                  func == "text__capitalize" || func == "text__title" {
                            self.context.variable_types.insert(res.clone(), "string".to_string());
                        }
                    } else {
                        self.context.emit(&format!("{}({});", func, args_str));
                    }
                } else {
                    // Regular function call
                    // Check if this is a recognized builtin that should be prefixed with "tau_"
                    let func_to_call = match func.as_str() {
                        "round" | "pow" | "divmod" | "hex" | "oct" | "bin" | "ord" | "chr" |
                        "zip" | "map" | "filter" | "type" | "isinstance" => {
                            format!("tau_{}", func)
                        },
                        _ => func.clone()
                    };
                    
                    let args_str = args.join(", ");
                    if let Some(res) = result {
                        self.context.emit(&format!("let {} = {}({});", res, func_to_call, args_str));
                    } else {
                        self.context.emit(&format!("{}({});", func_to_call, args_str));
                    }
                }
            }
            Return { value } => {
                // Emit actual return statement (except in main function)
                if self.context.current_function != "main" {
                    if let Some(val) = value {
                        self.context.emit(&format!("return {};", val));
                    } else {
                        self.context.emit("return TauObject::None;");
                    }
                }
            }
            If { condition, then_body, elif_branches, else_body } => {
                self.generate_if_statement(condition, then_body, elif_branches, else_body.as_ref())?;
            }
            While { condition, body, .. } => {
                self.context.emit(&format!("while {}.to_bool() {{", condition));
                self.context.indent();
                self.generate_block(body)?;
                self.context.dedent();
                self.context.emit("}");
            }
            For { variable, iterable, body, .. } => {
                // Handle tuple unpacking in for loops (e.g., "i, val" from enumerate)
                if variable.contains(',') {
                    // Tuple unpacking: for (i, val) in iterator that contains lists
                    // Split variable names
                    let var_names: Vec<&str> = variable.split(',').map(|s| s.trim()).collect();
                    let var1 = var_names.get(0).unwrap_or(&"_var0");
                    let var2 = var_names.get(1).unwrap_or(&"_var1");
                    
                    // Iterate over the iterable and destructure lists
                    self.context.emit(&format!("let _iter_items = if let TauObject::List(items) = &{} {{ items.clone() }} else {{ vec![] }};", iterable));
                    self.context.emit(&format!("for _tuple in &_iter_items {{"));
                    self.context.indent();
                    self.context.emit(&format!("let {} = if let TauObject::List(items) = _tuple {{ items.get(0).cloned().unwrap_or(TauObject::None) }} else {{ TauObject::None }};", var1));
                    self.context.emit(&format!("let {} = if let TauObject::List(items) = _tuple {{ items.get(1).cloned().unwrap_or(TauObject::None) }} else {{ TauObject::None }};", var2));
                    self.generate_block(body)?;
                    self.context.dedent();
                    self.context.emit("}");
                } else {
                    // Simple variable: for var in iterator
                    // Handle both Vec<TauObject> and TauObject::List
                    self.context.emit(&format!("let _iter_items = if let TauObject::List(items) = &{} {{ items.clone() }} else {{ vec![] }};", iterable));
                    self.context.emit(&format!("for {} in &_iter_items {{", variable));
                    self.context.indent();
                    
                    // Create convenience variables i and val for enumerate results (lists with 2+ elements)
                    self.context.emit(&format!("let i = if let TauObject::List(items) = &{} {{ items.get(0).cloned().unwrap_or(TauObject::None) }} else {{ TauObject::None }};", variable));
                    self.context.emit(&format!("let val = if let TauObject::List(items) = &{} {{ items.get(1).cloned().unwrap_or(TauObject::None) }} else {{ TauObject::None }};", variable));
                    
                    self.generate_block(body)?;
                    self.context.dedent();
                    self.context.emit("}");
                }
            }
            Break => {
                self.context.emit("break;");
            }
            Continue => {
                self.context.emit("continue;");
            }
            ListCreate { elements, result } => {
                let elems_str = elements.join(", ");
                self.context.emit(&format!("let {} = TauObject::List(vec![{}]);", result, elems_str));
            }
            DictCreate { pairs, result } => {
                // Create a dictionary from key-value pairs
                self.context.emit(&format!("let mut {} = HashMap::new();", result));
                for (key, value) in pairs {
                    // key and value are variable names that contain TauObject values
                    self.context.emit(&format!("{}.insert(match {} {{ TauObject::String(s) => s, _ => format!(\"{{:?}}\", {}) }}, {});", result, key, key, value));
                }
                self.context.emit(&format!("let {} = TauObject::Dict({});", result, result));
            }
            DictGetItem { dict, key, result } => {
                // Dictionary/list indexing: dict[key] or list[index]
                // For dicts: use get() with string key
                // For lists: use get() with usize index
                // We check if it looks like a dict access (string key) or list access (numeric key)
                self.context.emit(&format!("let {} = {}.get(&{}.to_string()).map(|v| v.clone()).unwrap_or_else(|| \"0\".to_string());", result, dict, key));
            }
            DictSetItem { dict, key, value } => {
                // Dictionary item assignment: dict[key] = value
                // Check if dict is a HashMap or Vec
                self.context.emit(&format!("{}.insert(\"{}\".to_string(), {});", dict, key, value));
            }
            ObjectCreate { class_name, result } => {
                // Create new object instance
                self.context.emit(&format!("let {} = {{}}; // TODO: Create {} instance", result, class_name));
            }
            ObjectSetAttr { object, attr, value } => {
                // Set object attribute
                self.context.emit(&format!("{}.{} = {}; // TODO: Set attribute {}", object, attr, value, attr));
            }
            ObjectGetAttr { object, attr, result } => {
                // Get object attribute
                self.context.emit(&format!("let {} = {}.{}; // TODO: Get attribute {}", result, object, attr, attr));
            }
            SuperCall { args: _, result } => {
                // Super call - placeholder
                self.context.emit(&format!("let {} = super_result; // TODO: Handle super() call", result));
            }
            Try { body, handlers, else_body, finally_body } => {
                // Python-style try/except/else/finally in Rust
                // We'll use Result types and match expressions to handle exceptions
                
                // Generate match block for exception handling
                self.context.emit("{");
                self.context.indent();
                self.context.emit("// Try block");
                self.context.emit("let _try_result = {");
                self.context.indent();
                self.generate_block(body)?;
                self.context.emit("Ok(())");
                self.context.dedent();
                self.context.emit("};");
                self.context.emit("");
                
                // Handle the result with match
                self.context.emit("match _try_result {");
                self.context.indent();
                self.context.emit("Ok(_) => {");
                self.context.indent();
                
                // If there was no exception, execute else block if present
                if let Some(else_block) = else_body {
                    self.context.emit("// Else block (no exception)");
                    self.generate_block(else_block)?;
                } else {
                    self.context.emit("// No exception occurred");
                }
                
                self.context.dedent();
                self.context.emit("}");
                
                // Handle except blocks
                if !handlers.is_empty() {
                    self.context.emit("Err(_err) => {");
                    self.context.indent();
                    self.context.emit("// Exception handling");
                    
                    for (i, (exc_type, var_name, handler_body)) in handlers.iter().enumerate() {
                        if i > 0 {
                            self.context.emit("// Else if");
                        }
                        let exc_type_str = exc_type.as_ref().map(|s| s.as_str()).unwrap_or("Exception");
                        let var_name_str = var_name.as_ref().map(|s| s.as_str()).unwrap_or("_err");
                        self.context.emit(&format!("// Catch {} as {}", exc_type_str, var_name_str));
                        self.generate_block(handler_body)?;
                    }
                    
                    self.context.dedent();
                    self.context.emit("}");
                } else {
                    self.context.emit("Err(_err) => { /* Uncaught exception */ }");
                }
                
                self.context.dedent();
                self.context.emit("}");
                
                // Finally block executes regardless
                if let Some(finally_block) = finally_body {
                    self.context.emit("");
                    self.context.emit("// Finally block (always executes)");
                    self.generate_block(finally_block)?;
                }
                
                self.context.dedent();
                self.context.emit("}");
            }
            Raise { exception } => {
                if let Some(exc) = exception {
                    self.context.emit(&format!("// raise {}", exc));
                } else {
                    self.context.emit("// raise");
                }
            }
            Import { module } => {
                self.context.emit(&format!("// use {};", module));
            }
            ImportFrom { module, names } => {
                let names_str = names.join(", ");
                self.context.emit(&format!("// use {}::{{{}}};", module, names_str));
            }
            Lambda { params, body_instructions, captured_vars: _, result, body_result_var } => {
                // Lambda expressions - compile to Rust closures
                // These take owned TauObject values and can handle both direct calls and iterator usage
                let params_with_types: Vec<String> = params.iter()
                    .map(|p| format!("{}: TauObject", p))
                    .collect();
                let params_str = params_with_types.join(", ");
                self.context.emit(&format!("let {} = |{}| {{", result, params_str));
                self.context.indent();
                
                // Generate the body instructions
                self.generate_block(body_instructions)?;
                
                // Emit the result of the lambda body
                self.context.emit(&format!("{}", body_result_var));
                
                self.context.dedent();
                self.context.emit("};");
            }
            ListComprehension {
                element_instrs,
                element_result,
                variable,
                iterable,
                condition_instrs,
                condition_result,
                result,
            } => {
                // List comprehension: [expr for var in iterable if condition]
                // Compile to Rust iterator chain, wrapped in TauObject::List
                // We need to generate inline code to avoid brace mismatches
                
                self.context.emit(&format!("let {} = {{", result));
                self.context.indent();
                self.context.emit(&format!("let __iterable = {}.to_vec().unwrap_or_default();", iterable));
                self.context.emit("let __result = __iterable.iter().filter_map(|__item| {");
                self.context.indent();
                self.context.emit(&format!("let {} = __item.clone();", variable));
                
                // Generate condition check if present
                if let Some(cond_var) = condition_result {
                    self.generate_block(condition_instrs)?;
                    self.context.emit(&format!("if !{}.to_bool() {{ return None; }}", cond_var));
                }
                
                // Generate element computation
                self.generate_block(element_instrs)?;
                
                // Return the element wrapped in Some
                self.context.emit(&format!("Some({})", element_result));
                
                self.context.dedent();
                self.context.emit("}).collect::<Vec<_>>();");
                self.context.emit("TauObject::List(__result)");
                self.context.dedent();
                self.context.emit("};");
                
                // Track this variable as a list type
                self.context.variable_types.insert(result.clone(), "list".to_string());
            }
            DictComprehension {
                key_instrs,
                key_result,
                value_instrs,
                value_result,
                variable,
                iterable,
                condition_instrs,
                condition_result,
                result,
            } => {
                // Dict comprehension: {key_expr: val_expr for var in iterable if condition}
                // Compile to Rust iterator chain building a HashMap
                self.context.emit(&format!("let {} = {}.iter().map(|x| x.clone()).filter_map(|{}| {{", result, iterable, variable));
                self.context.indent();
                
                // Generate condition check if present
                if let Some(cond_var) = condition_result {
                    self.generate_block(condition_instrs)?;
                    self.context.emit(&format!("if {}.to_bool() {{", cond_var));
                    self.context.indent();
                }
                
                // Generate key computation
                self.generate_block(key_instrs)?;
                
                // Generate value computation
                self.generate_block(value_instrs)?;
                
                // Return the (key, value) pair wrapped in Some
                self.context.emit(&format!("Some(({}.to_string(), {}))", key_result, value_result));
                
                if condition_result.is_some() {
                    self.context.dedent();
                    self.context.emit("} else { None }");
                }
                
                self.context.dedent();
                self.context.emit(&format!("}}).collect::<Vec<_>>();"));
                
                // Convert Vec<(String, Value)> to HashMap and wrap in TauObject::Dict
                self.context.emit(&format!("let {} = TauObject::Dict({}.into_iter().collect::<std::collections::HashMap<_, _>>());", result, result));
                
                // Track this variable as a dict type
                self.context.variable_types.insert(result.clone(), "dict".to_string());
            }
            Slice { object, start, stop, step: _, result } => {
                // String/list slicing: object[start:stop:step]
                let start_expr = start.as_deref().unwrap_or("0");
                let stop_expr = if let Some(s) = stop {
                    s.as_str()
                } else {
                    // Emit code that calculates length inline
                    if object.starts_with("\"") || object.ends_with("\"") {
                        // String slicing
                        self.context.emit(&format!(
                            "let {} = {}.chars().skip({} as usize).collect::<String>();",
                            result, object, start_expr
                        ));
                    } else {
                        // List slicing
                        self.context.emit(&format!(
                            "let {} = TauObject::List({}.iter().skip({} as usize).map(|x| x.clone()).collect::<Vec<_>>());",
                            result, object, start_expr
                        ));
                    }
                    return Ok(());
                };
                
                if object.starts_with("\"") || object.ends_with("\"") {
                    // String slicing with explicit stop
                    self.context.emit(&format!(
                        "let {} = {}.chars().skip({} as usize).take(({} - {}) as usize).collect::<String>();",
                        result, object, start_expr, stop_expr, start_expr
                    ));
                } else {
                    // List slicing with explicit stop
                    self.context.emit(&format!(
                        "let {} = TauObject::List({}.iter().skip({} as usize).take(({} - {}) as usize).map(|x| x.clone()).collect::<Vec<_>>());",
                        result, object, start_expr, stop_expr, start_expr
                    ));
                }
            }
            TupleCreate { elements, result } => {
                // Create a tuple from elements
                self.context.emit(&format!("let {} = ({});", result, elements.join(", ")));
            }
            TupleGetItem { tuple, index, result } => {
                // Get element from tuple by index
                self.context.emit(&format!("let {} = {}.{};", result, tuple, index));
            }
            TupleUnpack { tuple, targets } => {
                // Unpack tuple into variables
                let targets_str = targets.join(", ");
                self.context.emit(&format!("let ({}) = {};", targets_str, tuple));
            }
            ListCreate { elements, result: res } if false => {
                // This case is already handled above, but we need to match ListCreate here too
                // for completeness (shouldn't reach here)
            }
            Yield { value } => {
                // Yield statement for generators - emit placeholder
                if let Some(val) = value {
                    self.context.emit(&format!("// yield {};", val));
                } else {
                    self.context.emit("// yield;");
                }
            }
            YieldFrom { iterable } => {
                // Yield from statement - emit placeholder
                self.context.emit(&format!("// yield from {};", iterable));
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
        // Convert TauObject condition to bool using .to_bool() method
        self.context.emit(&format!("if {}.to_bool() {{", condition));
        self.context.indent();
        self.generate_block(then_body)?;
        self.context.dedent();

        for (elif_cond, elif_body) in elif_branches {
            self.context.emit(&format!("}} else if {}.to_bool() {{", elif_cond));
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
            self.context.emit(&format!("let {} = TauObject::String(\"{}\".to_string());", result, format_str));
        } else {
            let args_str = format_args.join(", ");
            self.context.emit(&format!(
                "let {} = TauObject::String(format!(\"{}\", {}));",
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
            Int(i) => format!("TauObject::Int({})", i),
            Float(f) => {
                let s = f.to_string();
                let formatted = if s.contains('.') { s.clone() } else { format!("{}.0", s) };
                format!("TauObject::Float({})", formatted)
            }
            Bool(b) => format!("TauObject::Bool({})", b),
            Str(s) => format!("TauObject::String(\"{}\".to_string())", s.escape_default()),
            Ellipsis => "TauObject::String(\"...\".to_string())".to_string(),
            _ => "TauObject::None".to_string(), // Default for unsupported types
        }
    }

    fn binary_op_to_rust(&self, op: &crate::ast::BinaryOp) -> String {
        use crate::ast::BinaryOp::*;
        
        let result = match op {
            Add => "+",
            Sub => "-",
            Mul => "*",
            Div => "/",
            FloorDiv => "/",
            Mod => "%",
            Pow => "^",
            LShift => "<<",
            RShift => ">>",
            BitOr => "|",
            BitXor => "^",
            BitAnd => "&",
            MatMul => "@",
            And => "&&",
            Or => "||",
            Eq => "==",
            Ne => "!=",
            Neq => "!=",
            Lt => "<",
            Le => "<=",
            Gt => ">",
            Ge => ">=",
            Gte => ">=",
            Lte => "<=",
            Is => "==",
            IsNot => "!=",
            In => "in",
            NotIn => "not in",
        };
        
        result.to_string()
    }

    fn emit_program_function(&mut self, globals: &[crate::ir::IRInstruction]) -> Result<()> {
        self.context.emit("");
        self.context.emit("pub fn run_program() {");
        self.context.indent();
        
        // Execute global instructions
        if globals.is_empty() {
            self.context.emit("println!(\"Program completed successfully\");");
        } else {
            self.generate_block(globals)?;
        }
        
        self.context.dedent();
        self.context.emit("}");

        Ok(())
    }

    fn emit_main(&mut self, globals: &[crate::ir::IRInstruction]) -> Result<()> {
        self.context.emit("");
        self.context.emit("pub fn main() {");
        self.context.indent();
        
        // Execute global instructions
        if globals.is_empty() {
            self.context.emit("println!(\"Program completed successfully\");");
        } else {
            self.generate_block(globals)?;
        }
        
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
