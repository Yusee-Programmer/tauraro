//! Import System for C Transpiler
//!
//! This module handles detecting, analyzing, and compiling imports when transpiling to C.
//! It supports:
//! - User-defined modules: Compiled to C and generated as .h header files
//! - Builtin modules: Compiled to object files and linked

use crate::ast::{Program, Statement};
use crate::ir::{IRModule, IRInstruction};
use anyhow::{Result, anyhow};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};

/// Information about an imported module
#[derive(Debug, Clone)]
pub struct ModuleInfo {
    pub name: String,
    pub module_type: ModuleType,
    pub file_path: Option<PathBuf>,
    pub aliases: Vec<String>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ModuleType {
    Builtin,
    UserDefined,
}

/// Import analyzer that scans AST for import statements
pub struct ImportAnalyzer {
    pub modules: HashMap<String, ModuleInfo>,
    pub search_paths: Vec<PathBuf>,
}

impl ImportAnalyzer {
    pub fn new() -> Self {
        Self {
            modules: HashMap::new(),
            search_paths: vec![
                PathBuf::from("."),
                PathBuf::from("tauraro_packages"),
                PathBuf::from("lib"),
            ],
        }
    }

    /// Analyze program AST and collect all imports
    pub fn analyze(&mut self, program: &Program) -> Result<()> {
        for stmt in &program.statements {
            self.analyze_statement(stmt)?;
        }
        Ok(())
    }

    /// Analyze IR module and collect all imports
    pub fn analyze_ir(&mut self, module: &IRModule) -> Result<()> {
        // Process global instructions for imports
        for instruction in &module.globals {
            match instruction {
                IRInstruction::Import { module: module_name } => {
                    self.add_import(module_name, None)?;
                }
                IRInstruction::ImportFrom { module: module_name, names: _ } => {
                    self.add_import(module_name, None)?;
                }
                _ => {}
            }
        }
        Ok(())
    }

    fn analyze_statement(&mut self, stmt: &Statement) -> Result<()> {
        match stmt {
            Statement::Import { module, alias } => {
                self.add_import(module, alias.clone())?;
            }
            Statement::FromImport { module, names } => {
                self.add_import(module, None)?;
                // Track which names are imported for optimization
            }
            // Recursively check nested statements
            Statement::If { then_branch, elif_branches, else_branch, .. } => {
                for s in then_branch {
                    self.analyze_statement(s)?;
                }
                for (_, body) in elif_branches {
                    for s in body {
                        self.analyze_statement(s)?;
                    }
                }
                if let Some(else_b) = else_branch {
                    for s in else_b {
                        self.analyze_statement(s)?;
                    }
                }
            }
            Statement::While { body, .. } | Statement::For { body, .. } => {
                for s in body {
                    self.analyze_statement(s)?;
                }
            }
            Statement::FunctionDef { body, .. } => {
                for s in body {
                    self.analyze_statement(s)?;
                }
            }
            Statement::ClassDef { body, .. } => {
                for s in body {
                    self.analyze_statement(s)?;
                }
            }
            Statement::Try { body: try_body, except_handlers, else_branch, finally, .. } => {
                for s in try_body {
                    self.analyze_statement(s)?;
                }
                for handler in except_handlers {
                    for s in &handler.body {
                        self.analyze_statement(s)?;
                    }
                }
                if let Some(else_b) = else_branch {
                    for s in else_b {
                        self.analyze_statement(s)?;
                    }
                }
                if let Some(finally_b) = finally {
                    for s in finally_b {
                        self.analyze_statement(s)?;
                    }
                }
            }
            Statement::With { body, .. } => {
                for s in body {
                    self.analyze_statement(s)?;
                }
            }
            _ => {}
        }
        Ok(())
    }

    fn add_import(&mut self, module_name: &str, alias: Option<String>) -> Result<()> {
        // Skip if already analyzed
        if self.modules.contains_key(module_name) {
            if let Some(alias) = alias {
                if let Some(module_info) = self.modules.get_mut(module_name) {
                    if !module_info.aliases.contains(&alias) {
                        module_info.aliases.push(alias);
                    }
                }
            }
            return Ok(());
        }

        // Determine if builtin or user-defined
        let (module_type, file_path) = if self.is_builtin_module(module_name) {
            (ModuleType::Builtin, None)
        } else {
            // Search for user module
            let path = self.find_module_file(module_name)?;
            (ModuleType::UserDefined, Some(path))
        };

        let mut aliases = Vec::new();
        if let Some(alias) = alias {
            aliases.push(alias);
        }

        self.modules.insert(
            module_name.to_string(),
            ModuleInfo {
                name: module_name.to_string(),
                module_type,
                file_path,
                aliases,
            },
        );

        Ok(())
    }

    /// Check if a module is a builtin module
    fn is_builtin_module(&self, name: &str) -> bool {
        // List of Tauraro builtin modules
        matches!(
            name,
            "math" | "sys" | "os" | "time" | "random" | "json" | "re" |
            "datetime" | "collections" | "itertools" | "functools" |
            "io" | "threading" | "asyncio" | "urllib" | "http" | "httpx" |
            "websockets" | "unittest" | "typing" | "pathlib"
        )
    }

    /// Find module file in search paths
    fn find_module_file(&self, module_name: &str) -> Result<PathBuf> {
        let extensions = ["py", "tr", "tau", "tauraro"];

        for search_path in &self.search_paths {
            // Try module file
            for ext in &extensions {
                let module_path = search_path.join(format!("{}.{}", module_name, ext));
                if module_path.exists() {
                    return Ok(module_path);
                }
            }

            // Try package __init__ file
            for ext in &extensions {
                let package_path = search_path
                    .join(module_name)
                    .join(format!("__init__.{}", ext));
                if package_path.exists() {
                    return Ok(package_path);
                }
            }
        }

        Err(anyhow!("Module '{}' not found in search paths", module_name))
    }

    /// Get all user-defined modules that need to be compiled
    pub fn get_user_modules(&self) -> Vec<&ModuleInfo> {
        self.modules
            .values()
            .filter(|m| m.module_type == ModuleType::UserDefined)
            .collect()
    }

    /// Get all builtin modules that need to be linked
    pub fn get_builtin_modules(&self) -> Vec<&ModuleInfo> {
        self.modules
            .values()
            .filter(|m| m.module_type == ModuleType::Builtin)
            .collect()
    }
}

/// Module compiler that handles recursive compilation of user modules
pub struct ModuleCompiler {
    compiled_modules: HashSet<String>,
    output_dir: PathBuf,
}

impl ModuleCompiler {
    pub fn new(output_dir: PathBuf) -> Self {
        Self {
            compiled_modules: HashSet::new(),
            output_dir,
        }
    }

    /// Compile a user module to header-only file
    pub fn compile_module(&mut self, module_info: &ModuleInfo) -> Result<(PathBuf, PathBuf)> {
        if self.compiled_modules.contains(&module_info.name) {
            // Already compiled, return path
            let h_path = self.output_dir.join(format!("{}.h", module_info.name));
            return Ok((h_path.clone(), h_path));
        }

        let file_path = module_info.file_path.as_ref().ok_or_else(|| {
            anyhow!("User module '{}' has no file path", module_info.name)
        })?;

        // Parse and compile the module
        let source = std::fs::read_to_string(file_path)?;

        // Lexical analysis
        use crate::lexer::Lexer;
        let tokens: Vec<_> = Lexer::new(&source)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in module '{}': {}", module_info.name, e))?;

        // Parsing
        use crate::parser::Parser;
        let mut parser = Parser::new(tokens);
        let ast = parser.parse()
            .map_err(|e| anyhow!("Parser error in module '{}': {}", module_info.name, e))?;

        // Semantic analysis (with default strict_types = false)
        use crate::semantic::Analyzer;
        let semantic_ast = Analyzer::new(false).analyze(ast)
            .map_err(|errors| anyhow!("Semantic errors in module '{}': {:?}", module_info.name, errors))?;

        // Generate IR
        use crate::ir::Generator;
        let ir_module = Generator::new().generate(semantic_ast)?;

        // Generate header-only file with both declarations and implementations
        let header_code = self.generate_header_only(&module_info.name, &ir_module)?;

        // Write header file
        let h_path = self.output_dir.join(format!("{}.h", module_info.name));
        std::fs::write(&h_path, header_code)?;

        self.compiled_modules.insert(module_info.name.clone());

        Ok((h_path.clone(), h_path))
    }

    /// Generate header-only file with both declarations and implementations
    fn generate_header_only(&self, module_name: &str, ir_module: &IRModule) -> Result<String> {
        let mut header = String::new();

        // Header guard
        let guard = format!("TAURARO_{}_H", module_name.to_uppercase());
        header.push_str(&format!("#ifndef {}\n", guard));
        header.push_str(&format!("#define {}\n\n", guard));

        // Include dependencies
        header.push_str("#include <stdio.h>\n");
        header.push_str("#include <stdlib.h>\n");
        header.push_str("#include <string.h>\n");
        header.push_str("#include <stdbool.h>\n");
        header.push_str("#include <stdint.h>\n");
        header.push_str("#include <math.h>\n");
        header.push_str("#include <ctype.h>\n\n");

        // Add type definitions (only if not already included)
        header.push_str("#ifndef TAURARO_TYPES_DEFINED\n");
        header.push_str("#define TAURARO_TYPES_DEFINED\n\n");

        use crate::codegen::c_transpiler::types;
        header.push_str(&types::generate_type_definitions());
        header.push_str("\n#endif // TAURARO_TYPES_DEFINED\n\n");

        // Add OOP structures (only if not already included)
        header.push_str("#ifndef TAURARO_OOP_DEFINED\n");
        header.push_str("#define TAURARO_OOP_DEFINED\n\n");

        use crate::codegen::c_transpiler::oop;
        header.push_str(&oop::generate_oop_structures());
        header.push_str("\n#endif // TAURARO_OOP_DEFINED\n\n");

        // Add runtime function declarations (only if not already declared)
        header.push_str("#ifndef TAURARO_RUNTIME_DECLARED\n");
        header.push_str("#define TAURARO_RUNTIME_DECLARED\n\n");
        header.push_str("// Runtime support functions\n");
        header.push_str("tauraro_value_t* tauraro_value_new(void);\n");
        header.push_str("tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right);\n");
        header.push_str("tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right);\n");
        header.push_str("tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right);\n");
        header.push_str("tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right);\n");
        header.push_str("tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right);\n");
        header.push_str("tauraro_value_t* tauraro_print(int argc, tauraro_value_t** argv);\n");
        header.push_str("\n#endif // TAURARO_RUNTIME_DECLARED\n\n");

        // Generate global variable definitions and comments
        header.push_str(&format!("// Module: {} - Global variables and comments\n", module_name));
        for instruction in &ir_module.globals {
            match instruction {
                IRInstruction::Comment(text) => {
                    // Generate C comment
                    header.push_str(&format!("// {}\n", text));
                }
                IRInstruction::StoreGlobal { name, .. } | IRInstruction::StoreTypedGlobal { name, .. } => {
                    let prefixed_name = format!("{}_{}", module_name, name);
                    header.push_str(&format!("tauraro_value_t* {} = NULL;\n", prefixed_name));
                }
                _ => {}
            }
        }
        header.push_str("\n");

        // Generate function implementations
        use crate::codegen::c_transpiler::functions;
        if !ir_module.functions.is_empty() {
            header.push_str(&format!("// Module: {} - Function implementations\n\n", module_name));
            for (func_name, function) in &ir_module.functions {
                // Generate function with module prefix
                let prefixed_name = format!("{}_{}", module_name, func_name);

                // Create a modified function with prefixed name
                let mut prefixed_function = function.clone();
                prefixed_function.name = prefixed_name.clone();

                header.push_str(&functions::generate_function(&prefixed_function)?);
                header.push_str("\n\n");
            }
        }

        header.push_str(&format!("#endif // {}\n", guard));

        Ok(header)
    }
}