//! Import handling for C code generation
//! This module handles recursive compilation of imported Tauraro modules to C

use crate::ir::IRModule;
use crate::codegen::{CodeGenerator, CodegenOptions};
use crate::codegen::native::OutputType;
use crate::codegen::builtin_modules::BuiltinModuleCompiler;
use crate::module_system::ModuleSystem;
use crate::parser::Parser;
use crate::lexer::Lexer;
use crate::semantic::Analyzer;
use crate::ast::Program;
use crate::module_cache::ModuleCache;
use anyhow::{Result, anyhow};
use std::path::{Path, PathBuf};
use std::fs;
use std::collections::{HashMap, HashSet};
use std::process::Command;

/// Import compiler for C backend
pub struct ImportCompiler {
    module_system: ModuleSystem,
    compiled_modules: HashSet<String>,
    build_dir: PathBuf,
    builtin_compiler: BuiltinModuleCompiler, // Add this field
}

impl ImportCompiler {
    pub fn new(build_dir: PathBuf) -> Self {
        let mut module_system = ModuleSystem::new();
        // Add test_imports directory to the module search path
        module_system.add_search_path(PathBuf::from("test_imports"));
        
        // Create the builtin module compiler
        let builtin_compiler = BuiltinModuleCompiler::new().expect("Failed to create builtin module compiler");
        
        Self {
            module_system,
            compiled_modules: HashSet::new(),
            build_dir,
            builtin_compiler, // Initialize the new field
        }
    }

    /// Compile a main module and all its imports to C
    pub fn compile_with_imports(
        &mut self,
        main_file: &Path,
        output_options: &CodegenOptions,
    ) -> Result<HashMap<String, Vec<u8>>> {
        // Parse and compile the main module first to check for imports
        let main_module_name = main_file
            .file_stem()
            .and_then(|s| s.to_str())
            .unwrap_or("main")
            .to_string();

        // Compile the main module
        let main_ir = self.compile_module_file(main_file, &main_module_name)?;
        
        // Check if there are any imports in the IR
        let has_imports = self.has_imports(&main_ir);

        // Only create build directory if there are imports or if we need it for organization
        if has_imports {
            // Create build directory
            fs::create_dir_all(&self.build_dir)?;
        }

        // Detect imported built-in modules
        let imported_builtins = self.builtin_compiler.detect_imported_builtins(&main_ir);
        
        // Compile imported built-in modules
        if !imported_builtins.is_empty() {
            self.builtin_compiler.compile_imported_builtins(&imported_builtins)?;
        }

        let mut compiled_files = HashMap::new();

        // Generate C code for main module with extern declarations for built-in modules
        let main_c_code = self.generate_c_code_with_builtins(&main_ir, output_options, &imported_builtins)?;
        
        if has_imports {
            // Write to build directory if there are imports
            let main_c_path = self.build_dir.join(format!("{}.c", main_module_name));
            fs::write(&main_c_path, &main_c_code)?;
        }
        compiled_files.insert(main_module_name.clone(), main_c_code);

        // Recursively compile imported modules only if there are imports
        if has_imports {
            self.compile_imported_modules(&main_ir, output_options, &mut compiled_files)?;
        }

        Ok(compiled_files)
    }

    /// Check if the IR module has any import statements
    fn has_imports(&self, ir_module: &IRModule) -> bool {
        for (_, function) in &ir_module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    match instruction {
                        crate::ir::IRInstruction::Import { .. } => return true,
                        crate::ir::IRInstruction::ImportFrom { .. } => return true,
                        _ => {}
                    }
                }
            }
        }
        false
    }

    /// Compile a single module file to IR
    fn compile_module_file(&self, file_path: &Path, module_name: &str) -> Result<IRModule> {
        let source = fs::read_to_string(file_path)?;
        
        // Lexical analysis
        let tokens = Lexer::new(&source)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in {}: {:?}", file_path.display(), e))?;

        // Parsing
        let mut parser = Parser::new(tokens);
        let ast = parser.parse()
            .map_err(|e| anyhow!("Parser error in {}: {:?}", file_path.display(), e))?;

        // Semantic analysis with module context
        let semantic_ast = self.analyze_with_module_context(ast, module_name)?;

        // Generate IR
        let mut ir_generator = crate::ir::Generator::new();
        let ir_module = ir_generator.generate(semantic_ast)
            .map_err(|e| anyhow!("IR generation error in {}: {}", file_path.display(), e))?;

        Ok(ir_module)
    }

    /// Analyze AST with proper module context
    fn analyze_with_module_context(&self, ast: Program, module_name: &str) -> Result<Program> {
        // Semantic analysis
        let semantic_ast = Analyzer::new(false) // strict_types = false for now
            .analyze(ast)
            .map_err(|errors| anyhow!("Semantic errors in module '{}': {:?}", module_name, errors))?;
            
        Ok(semantic_ast)
    }

    /// Generate C code from IR module
    fn generate_c_code(&self, ir_module: &IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        let generator = crate::codegen::c_abi::CCodeGenerator::new();
        generator.generate(ir_module.clone(), options)
    }

    /// Generate C code from IR module with built-in module extern declarations
    fn generate_c_code_with_builtins(&self, ir_module: &IRModule, options: &CodegenOptions, imported_builtins: &HashSet<String>) -> Result<Vec<u8>> {
        let generator = crate::codegen::c_abi::CCodeGenerator::new();
        
        // Generate the base C code
        let mut c_code = generator.generate(ir_module.clone(), options)?;
        
        // Generate extern declarations for built-in modules
        if !imported_builtins.is_empty() {
            let extern_declarations = self.builtin_compiler.generate_extern_declarations(imported_builtins)?;
            
            // Convert to string and insert extern declarations at the top
            let c_code_str = String::from_utf8(c_code)?;
            
            // Find the position to insert extern declarations (after includes)
            let insert_pos = if let Some(pos) = c_code_str.find("\n\n") {
                pos + 2
            } else {
                0
            };
            
            // Insert extern declarations
            let mut new_c_code = c_code_str.clone();
            new_c_code.insert_str(insert_pos, &extern_declarations);
            
            // Convert back to bytes
            c_code = new_c_code.into_bytes();
        }
        
        Ok(c_code)
    }

    /// Recursively compile all imported modules
    fn compile_imported_modules(
        &mut self,
        ir_module: &IRModule,
        options: &CodegenOptions,
        compiled_files: &mut HashMap<String, Vec<u8>>,
    ) -> Result<()> {
        // Look for import instructions in the IR
        for (_, function) in &ir_module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    match instruction {
                        crate::ir::IRInstruction::Import { module, alias: _ } => {
                            self.compile_module_if_needed(module, options, compiled_files)?;
                        }
                        crate::ir::IRInstruction::ImportFrom { module, names: _ } => {
                            self.compile_module_if_needed(module, options, compiled_files)?;
                        }
                        _ => {}
                    }
                }
            }
        }

        Ok(())
    }

    /// Compile a module if it hasn't been compiled yet
    fn compile_module_if_needed(
        &mut self,
        module_name: &str,
        options: &CodegenOptions,
        compiled_files: &mut HashMap<String, Vec<u8>>,
    ) -> Result<()> {
        // Skip if already compiled
        if self.compiled_modules.contains(module_name) {
            return Ok(());
        }

        // Check if this is a built-in module
        if self.module_system.module_cache().should_cache_module(module_name) {
            // For built-in modules, we now handle them in the main compilation process
            // Mark as compiled to prevent circular dependencies
            self.compiled_modules.insert(module_name.to_string());
            return Ok(());
        }

        // Mark as compiled to prevent circular dependencies
        self.compiled_modules.insert(module_name.to_string());

        // Resolve module path
        let module_path = self.module_system.resolve_module_path(module_name)
            .map_err(|e| anyhow!("Failed to resolve module '{}': {}", module_name, e))?;

        // Compile the module
        let ir_module = self.compile_module_file(&module_path, module_name)?;
        
        // Determine output path (handle packages) - only generate header files now
        let output_path = if module_name.contains('.') {
            // It's a package module like "extra.utils"
            let parts: Vec<&str> = module_name.split('.').collect();
            let package_dir = self.build_dir.join(parts[..parts.len() - 1].join("/"));
            fs::create_dir_all(&package_dir)?;
            package_dir.join(format!("{}.h", parts[parts.len() - 1]))
        } else {
            // It's a top-level module
            self.build_dir.join(format!("{}.h", module_name))
        };

        // Generate header file with function implementations
        let mut header = String::new();
        let generator = crate::codegen::c_abi::CCodeGenerator::new();
        
        // Header guard - replace dots with underscores for valid C preprocessor syntax
        let guard_name = format!("{}_H", module_name.to_uppercase().replace(".", "_"));
        header.push_str(&format!("#ifndef {}\n", guard_name));
        header.push_str(&format!("#define {}\n\n", guard_name));
        
        // Include standard headers
        header.push_str("#include <stdio.h>\n");
        header.push_str("#include <stdlib.h>\n");
        header.push_str("#include <stdint.h>\n");
        header.push_str("#include <stdbool.h>\n");
        header.push_str("#include <string.h>\n\n");
        
        // String operations
        header.push_str("// String operations\n");
        header.push_str("char* tauraro_str_concat(const char* s1, const char* s2) {\n");
        header.push_str("    if (s1 == NULL || s2 == NULL) return NULL;\n");
        header.push_str("    size_t len1 = strlen(s1);\n");
        header.push_str("    size_t len2 = strlen(s2);\n");
        header.push_str("    char* result = (char*)malloc(len1 + len2 + 1);\n");
        header.push_str("    strcpy(result, s1);\n");
        header.push_str("    strcat(result, s2);\n");
        header.push_str("    return result;\n");
        header.push_str("}\n\n");
        
        // Function implementations
        for (name, function) in &ir_module.functions {
            // Skip main and other special functions that shouldn't be in headers
            if name == "main" || name == "tauraro_main" {
                continue;
            }
            
            if !function.is_extern {
                let return_type = generator.ir_type_to_c(&function.return_type);
                let param_types: Vec<String> = function.params.iter()
                    .map(|param| generator.ir_type_to_c(&param.ty))
                    .map(|s| s.to_string())
                    .collect();
                let params: Vec<String> = function.params.iter()
                    .zip(param_types.iter())
                    .map(|(param, param_type)| format!("{} {}", param_type, param.name))
                    .collect();
                let params_str: String = if params.is_empty() {
                    "void".to_string()
                } else {
                    params.join(", ")
                };
                
                // Generate function signature
                header.push_str(&format!("static inline {} {}({}) {{\n", return_type, name, params_str));
                
                // Generate function body from IR
                header.push_str("    // Function implementation from IR\n");
                
                // Add local variable declarations
                let mut declared_vars = std::collections::HashSet::new();
                
                // Generate actual implementation based on IR blocks
                for block in &function.blocks {
                    for instruction in &block.instructions {
                        match instruction {
                            crate::ir::IRInstruction::FormatString { dest, parts } => {
                                // Declare the destination variable if not already declared
                                if !declared_vars.contains(dest) {
                                    header.push_str(&format!("    char* {};\n", dest));
                                    declared_vars.insert(dest.clone());
                                }
                                
                                header.push_str(&format!("    // Format string: {}\n", dest));
                                header.push_str(&format!("    size_t len = 0;\n"));
                                
                                // Calculate total length needed
                                for part in parts {
                                    match part {
                                        crate::ir::FormatPartIR::String(s) => {
                                            header.push_str(&format!("    len += {};\n", s.len()));
                                        }
                                        crate::ir::FormatPartIR::Expression { expr: _, format_spec: _, conversion: _ } => {
                                            header.push_str("    len += 32; // Space for expression\n");
                                        }
                                    }
                                }
                                
                                header.push_str(&format!("    {} = (char*)malloc(len + 1);\n", dest));
                                header.push_str(&format!("    {}[0] = '\\0';\n", dest));
                                
                                // Build the string
                                for part in parts {
                                    match part {
                                        crate::ir::FormatPartIR::String(s) => {
                                            if !s.is_empty() {
                                                // Escape quotes in the string
                                                let escaped = s.replace("\"", "\\\"");
                                                header.push_str(&format!("    strcat({}, \"{}\")", dest, escaped));
                                                header.push_str(";\n");
                                            }
                                        }
                                        crate::ir::FormatPartIR::Expression { expr, format_spec: _, conversion: _ } => {
                                            match expr {
                                                crate::ir::IRValue::Variable(var_name) => {
                                                    // Check if the variable is a string parameter
                                                    let is_string_param = function.params.iter()
                                                        .find(|param| &param.name == var_name)
                                                        .map(|param| matches!(param.ty, crate::ir::IRType::Pointer(_)))
                                                        .unwrap_or(false);
                                                    
                                                    if is_string_param {
                                                        // It's a string parameter
                                                        header.push_str(&format!("    strcat({}, (char*){})", dest, var_name));
                                                        header.push_str(";\n");
                                                    } else {
                                                        // Assume it's a number
                                                        header.push_str(&format!("    {{ char temp[32]; sprintf(temp, \"%lld\", (long long){}); strcat({}, temp); }}\n", var_name, dest));
                                                    }
                                                }
                                                crate::ir::IRValue::ImmediateString(s) => {
                                                    // Escape quotes in the string
                                                    let escaped = s.replace("\"", "\\\"");
                                                    header.push_str(&format!("    strcat({}, \"{}\")", dest, escaped));
                                                    header.push_str(";\n");
                                                }
                                                crate::ir::IRValue::ImmediateInt(i) => {
                                                    header.push_str(&format!("    {{ char temp[32]; sprintf(temp, \"%lld\", (long long){}); strcat({}, temp); }}\n", i, dest));
                                                }
                                                crate::ir::IRValue::ImmediateFloat(f) => {
                                                    header.push_str(&format!("    {{ char temp[32]; sprintf(temp, \"%f\", {}); strcat({}, temp); }}\n", f, dest));
                                                }
                                                crate::ir::IRValue::ImmediateBool(b) => {
                                                    header.push_str(&format!("    {{ char temp[32]; sprintf(temp, \"%s\", {} ? \"true\" : \"false\"); strcat({}, temp); }}\n", b, dest));
                                                }
                                                _ => {
                                                    header.push_str(&format!("    strcat({}, \"<expr>\")", dest));
                                                    header.push_str(";\n");
                                                }
                                            }
                                        }
                                    }
                                }
                                
                                header.push_str(&format!("    return {};\n", dest));
                            }
                            crate::ir::IRInstruction::Ret { value } => {
                                if let Some(val) = value {
                                    match val {
                                        crate::ir::IRValue::Variable(var_name) => {
                                            header.push_str(&format!("    return {};\n", var_name));
                                        }
                                        crate::ir::IRValue::ImmediateString(s) => {
                                            // For string returns, we need to allocate and return a copy
                                            header.push_str(&format!("    char* result = (char*)malloc(strlen(\"{}\") + 1);\n", s));
                                            header.push_str(&format!("    strcpy(result, \"{}\")", s));
                                            header.push_str(";\n");
                                            header.push_str("    return result;\n");
                                        }
                                        crate::ir::IRValue::ImmediateInt(i) => {
                                            header.push_str(&format!("    return {};\n", i));
                                        }
                                        crate::ir::IRValue::ImmediateFloat(f) => {
                                            header.push_str(&format!("    return {};\n", f));
                                        }
                                        crate::ir::IRValue::ImmediateBool(b) => {
                                            header.push_str(&format!("    return {};\n", if *b { "1" } else { "0" }));
                                        }
                                        crate::ir::IRValue::Null => {
                                            header.push_str("    return NULL;\n");
                                        }
                                        _ => {
                                            header.push_str("    // Return statement\n");
                                        }
                                    }
                                } else {
                                    header.push_str("    return;\n");
                                }
                            }
                            crate::ir::IRInstruction::Add { dest, left, right } => {
                                // Declare the destination variable if not already declared
                                if !declared_vars.contains(dest) {
                                    // Try to infer the type from the operands
                                    let var_type = if let (crate::ir::IRValue::Variable(left_var), crate::ir::IRValue::Variable(right_var)) = (left, right) {
                                        // Check if both variables are string parameters
                                        let left_is_string = function.params.iter()
                                            .find(|param| &param.name == left_var)
                                            .map(|param| matches!(param.ty, crate::ir::IRType::Pointer(_)))
                                            .unwrap_or(false);
                                        let right_is_string = function.params.iter()
                                            .find(|param| &param.name == right_var)
                                            .map(|param| matches!(param.ty, crate::ir::IRType::Pointer(_)))
                                            .unwrap_or(false);
                                        
                                        if left_is_string && right_is_string {
                                            "char*"
                                        } else {
                                            "int64_t"
                                        }
                                    } else {
                                        "int64_t" // Default to int64_t
                                    };
                                    
                                    header.push_str(&format!("    {} {};\n", var_type, dest));
                                    declared_vars.insert(dest.clone());
                                }
                                
                                match (left, right) {
                                    (crate::ir::IRValue::Variable(left_var), crate::ir::IRValue::Variable(right_var)) => {
                                        // Check if both variables are string parameters for string concatenation
                                        let left_is_string = function.params.iter()
                                            .find(|param| &param.name == left_var)
                                            .map(|param| matches!(param.ty, crate::ir::IRType::Pointer(_)))
                                            .unwrap_or(false);
                                        let right_is_string = function.params.iter()
                                            .find(|param| &param.name == right_var)
                                            .map(|param| matches!(param.ty, crate::ir::IRType::Pointer(_)))
                                            .unwrap_or(false);
                                        
                                        if left_is_string && right_is_string {
                                            // String concatenation
                                            header.push_str(&format!("    {} = tauraro_str_concat({}, {});\n", dest, left_var, right_var));
                                        } else {
                                            // Numeric addition
                                            header.push_str(&format!("    {} = {} + {};\n", dest, left_var, right_var));
                                        }
                                    }
                                    (crate::ir::IRValue::Variable(var), crate::ir::IRValue::ImmediateInt(i)) |
                                    (crate::ir::IRValue::ImmediateInt(i), crate::ir::IRValue::Variable(var)) => {
                                        header.push_str(&format!("    {} = {} + {};\n", dest, var, i));
                                    }
                                    (crate::ir::IRValue::ImmediateInt(left_i), crate::ir::IRValue::ImmediateInt(right_i)) => {
                                        header.push_str(&format!("    {} = {} + {};\n", dest, left_i, right_i));
                                    }
                                    _ => {
                                        header.push_str(&format!("    {} = 0; // Addition\n", dest));
                                    }
                                }
                            }
                            crate::ir::IRInstruction::Mul { dest, left, right } => {
                                // Declare the destination variable if not already declared
                                if !declared_vars.contains(dest) {
                                    header.push_str(&format!("    int64_t {};\n", dest));
                                    declared_vars.insert(dest.clone());
                                }
                                
                                match (left, right) {
                                    (crate::ir::IRValue::Variable(left_var), crate::ir::IRValue::Variable(right_var)) => {
                                        header.push_str(&format!("    {} = {} * {};\n", dest, left_var, right_var));
                                    }
                                    _ => {
                                        header.push_str(&format!("    {} = 0; // Multiplication\n", dest));
                                    }
                                }
                            }
                            crate::ir::IRInstruction::Call { dest, func, args } => {
                                // Handle function calls
                                if let Some(dest_var) = dest {
                                    // Declare the destination variable if not already declared
                                    if !declared_vars.contains(dest_var) {
                                        // Try to infer return type from the function being called
                                        let return_type = if func == "strlen" {
                                            "size_t"
                                        } else if func.starts_with("is_") {
                                            "bool"
                                        } else {
                                            "int64_t" // Default
                                        };
                                        
                                        header.push_str(&format!("    {} {};\n", return_type, dest_var));
                                        declared_vars.insert(dest_var.clone());
                                    }
                                    
                                    // Generate function call
                                    let arg_strs: Vec<String> = args.iter().map(|arg| {
                                        match arg {
                                            crate::ir::IRValue::Variable(name) => name.clone(),
                                            crate::ir::IRValue::ImmediateInt(i) => i.to_string(),
                                            crate::ir::IRValue::ImmediateFloat(f) => f.to_string(),
                                            crate::ir::IRValue::ImmediateString(s) => format!("\"{}\"", s.replace("\"", "\\\"")),
                                            crate::ir::IRValue::ImmediateBool(b) => if *b { "1" } else { "0" }.to_string(),
                                            crate::ir::IRValue::Null => "NULL".to_string(),
                                            _ => "0".to_string(), // Default
                                        }
                                    }).collect();
                                    
                                    let args_str = arg_strs.join(", ");
                                    header.push_str(&format!("    {} = {}({});\n", dest_var, func, args_str));
                                } else {
                                    // Function call without return value
                                    let arg_strs: Vec<String> = args.iter().map(|arg| {
                                        match arg {
                                            crate::ir::IRValue::Variable(name) => name.clone(),
                                            crate::ir::IRValue::ImmediateInt(i) => i.to_string(),
                                            crate::ir::IRValue::ImmediateFloat(f) => f.to_string(),
                                            crate::ir::IRValue::ImmediateString(s) => format!("\"{}\"", s.replace("\"", "\\\"")),
                                            crate::ir::IRValue::ImmediateBool(b) => if *b { "1" } else { "0" }.to_string(),
                                            crate::ir::IRValue::Null => "NULL".to_string(),
                                            _ => "0".to_string(), // Default
                                        }
                                    }).collect();
                                    
                                    let args_str = arg_strs.join(", ");
                                    header.push_str(&format!("    {}({});\n", func, args_str));
                                }
                            }
                            crate::ir::IRInstruction::Alloca { dest, ty } => {
                                // Declare variable with appropriate type
                                if !declared_vars.contains(dest) {
                                    let c_type = generator.ir_type_to_c(ty);
                                    header.push_str(&format!("    {} {};\n", c_type, dest));
                                    declared_vars.insert(dest.clone());
                                }
                            }
                            crate::ir::IRInstruction::Store { value, ptr } => {
                                match value {
                                    crate::ir::IRValue::Variable(var_name) => {
                                        header.push_str(&format!("    {} = {};\n", ptr, var_name));
                                    }
                                    crate::ir::IRValue::ImmediateInt(i) => {
                                        header.push_str(&format!("    {} = {};\n", ptr, i));
                                    }
                                    crate::ir::IRValue::ImmediateFloat(f) => {
                                        header.push_str(&format!("    {} = {};\n", ptr, f));
                                    }
                                    crate::ir::IRValue::ImmediateString(s) => {
                                        header.push_str(&format!("    {} = \"{}\";\n", ptr, s.replace("\"", "\\\"")));
                                    }
                                    crate::ir::IRValue::ImmediateBool(b) => {
                                        header.push_str(&format!("    {} = {};\n", ptr, if *b { "1" } else { "0" }));
                                    }
                                    crate::ir::IRValue::Null => {
                                        header.push_str(&format!("    {} = NULL;\n", ptr));
                                    }
                                    _ => {
                                        header.push_str(&format!("    {} = 0; // Store\n", ptr));
                                    }
                                }
                            }
                            _ => {
                                header.push_str("    // Other instruction\n");
                            }
                        }
                    }
                }
                
                // Add default return if no return was generated
                if function.blocks.is_empty() || !function.blocks.iter().any(|b| b.instructions.iter().any(|i| matches!(i, crate::ir::IRInstruction::Ret { .. }))) {
                    if return_type == "void" {
                        header.push_str("    // Function implementation would go here\n");
                    } else if return_type.contains("int") {
                        header.push_str("    return 0; // Default return value\n");
                    } else if return_type.contains("*") {
                        header.push_str("    return NULL; // Default return value\n");
                    } else {
                        header.push_str("    // Function implementation would go here\n");
                        header.push_str("    return 0; // Default return value\n");
                    }
                }
                
                header.push_str("}\n\n");
            }
        }
        
        // Global variable declarations
        for global in &ir_module.globals {
            let c_type = generator.ir_type_to_c(&global.ty);
            header.push_str(&format!("extern {} {};\n", c_type, global.name));
        }
        
        header.push_str("\n#endif // ");
        header.push_str(&guard_name);
        header.push_str("\n");
        
        // Write the header file
        fs::write(&output_path, header)?;
        // Note: We don't store compiled_files anymore since we're not generating .c files

        // Recursively compile imports in this module
        self.compile_imported_modules(&ir_module, options, compiled_files)?;

        Ok(())
    }

    /// Compile all modules to native executables
    pub fn compile_to_native(
        &self,
        compiled_files: &HashMap<String, Vec<u8>>,
        main_module_name: &str,
        output_options: &CodegenOptions,
    ) -> Result<PathBuf, Box<dyn std::error::Error>> {
        use crate::codegen::native::{NativeCompiler, OutputType};

        let compiler = NativeCompiler::new();
        let output_type = if output_options.export_symbols {
            OutputType::SharedLibrary
        } else {
            OutputType::Executable
        };

        // Check if we have imports (which would mean we created a build directory)
        let has_imports = compiled_files.len() > 1 || 
            (compiled_files.len() == 1 && compiled_files.contains_key(main_module_name) && {
                // Check if the main module file exists in the build directory
                let main_c_file = self.build_dir.join(format!("{}.c", main_module_name));
                main_c_file.exists()
            });

        // Determine the main C file path
        let main_c_file = if has_imports {
            // Use the file in the build directory
            self.build_dir.join(format!("{}.c", main_module_name))
        } else {
            // For single files without imports, use a temporary file
            let temp_dir = std::env::temp_dir();
            let temp_file = temp_dir.join(format!("{}.c", main_module_name));
            
            // Write the C code to the temporary file
            if let Some(c_code) = compiled_files.get(main_module_name) {
                fs::write(&temp_file, c_code)?;
            }
            
            temp_file
        };

        let output_name = if output_options.export_symbols {
            format!("{}.dll", main_module_name) // Windows DLL
        } else {
            format!("{}.exe", main_module_name) // Windows executable
        };
        
        // Determine output path
        let output_path = if has_imports {
            self.build_dir.join(&output_name)
        } else {
            // For single files, put output in current directory
            PathBuf::from(&output_name)
        };
        
        compiler.compile_c_to_native(
            &main_c_file,
            Some(&output_path),
            output_type,
            output_options.export_symbols,
        ).map_err(|e| anyhow!("Native compilation failed: {}", e))?;

        // Clean up temporary file if we created one
        if !has_imports {
            let _ = fs::remove_file(&main_c_file);
        }

        Ok(output_path)
    }

    /// Compile a built-in module to an object file and cache it
    fn compile_builtin_module_to_cache(&mut self, module_name: &str) -> Result<()> {
        // Check if module is already cached and up to date
        if self.is_builtin_module_cached(module_name)? {
            return Ok(());
        }
        
        // Get the object file path in cache
        let obj_path = self.module_system.module_cache().get_module_obj_path(module_name);
        
        // Compile the built-in module to object file and cache it
        self.compile_builtin_module_to_obj(module_name, &obj_path)?;
        
        Ok(())
    }
    
    /// Check if a built-in module is already cached and up to date
    fn is_builtin_module_cached(&self, module_name: &str) -> Result<bool> {
        let module_source_path = PathBuf::from("src").join("modules").join(format!("{}.rs", module_name));
        let obj_path = self.module_system.module_cache().get_module_obj_path(module_name);
        
        // If object file doesn't exist, it's not cached
        if !obj_path.exists() {
            return Ok(false);
        }
        
        // Check if source file is newer than object file
        let source_metadata = fs::metadata(&module_source_path)?;
        let obj_metadata = fs::metadata(&obj_path)?;
        
        let source_modified = source_metadata.modified()?;
        let obj_modified = obj_metadata.modified()?;
        
        // If source is newer, cache is outdated
        if source_modified > obj_modified {
            return Ok(false);
        }
        
        // Also check if any dependencies have changed
        // For now, we'll just check the main Cargo.toml file as a simple dependency check
        let cargo_toml_path = PathBuf::from("Cargo.toml");
        if cargo_toml_path.exists() {
            let cargo_metadata = fs::metadata(&cargo_toml_path)?;
            let cargo_modified = cargo_metadata.modified()?;
            
            // If Cargo.toml is newer than the object file, rebuild
            if cargo_modified > obj_modified {
                return Ok(false);
            }
        }
        
        Ok(true)
    }
    
    /// Compile a built-in module to an object file
    fn compile_builtin_module_to_obj(&self, module_name: &str, obj_path: &Path) -> Result<()> {
        // Create the cache directory if it doesn't exist
        if let Some(parent) = obj_path.parent() {
            fs::create_dir_all(parent)?;
        }
        
        // For built-in modules that are part of the Tauraro crate, we need to generate
        // a standalone C file that exports the functions and constants
        let c_code = self.generate_builtin_module_c_code(module_name)?;
        
        // Write C code to a temporary file
        let temp_dir = std::env::temp_dir();
        let c_file_path = temp_dir.join(format!("{}_module.c", module_name));
        fs::write(&c_file_path, c_code)?;
        
        // Use the native compiler to detect available C compiler and compile to object file
        let compiler = crate::codegen::native::NativeCompiler::new();
        let c_compiler = compiler.detect_c_compiler()
            .map_err(|e| anyhow!("Failed to detect C compiler: {}", e))?;
        
        let output = Command::new(&c_compiler)
            .arg("-c")  // Compile only, don't link
            .arg(&c_file_path)
            .arg("-o")
            .arg(obj_path)
            .output()
            .map_err(|e| anyhow!("Failed to execute C compiler: {}", e))?;
            
        // Clean up temporary file
        let _ = fs::remove_file(&c_file_path);
        
        if !output.status.success() {
            let stderr = String::from_utf8_lossy(&output.stderr);
            return Err(anyhow!("Failed to compile built-in module to object file: {}", stderr));
        }
        
        Ok(())
    }
    
    /// Generate C code for a built-in module that exports functions and constants
    fn generate_builtin_module_c_code(&self, module_name: &str) -> Result<String> {
        let mut c_code = String::new();
        
        // Standard includes
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <stdint.h>\n");
        c_code.push_str("#include <stdbool.h>\n");
        c_code.push_str("#include <math.h>\n");
        c_code.push_str("#include <string.h>\n\n");
        
        // Module-specific code
        match module_name {
            "math" => {
                // Math module constants
                c_code.push_str("// Math module constants\n");
                c_code.push_str("double TAURARO_MATH_PI = 3.141592653589793;\n");
                c_code.push_str("double TAURARO_MATH_E = 2.718281828459045;\n");
                c_code.push_str("double TAURARO_MATH_TAU = 6.283185307179586;\n\n");
                
                // Math module functions
                c_code.push_str("// Math module functions\n");
                c_code.push_str("double tauraro_math_sin(double x) { return sin(x); }\n");
                c_code.push_str("double tauraro_math_cos(double x) { return cos(x); }\n");
                c_code.push_str("double tauraro_math_sqrt(double x) { return sqrt(x); }\n");
                c_code.push_str("double tauraro_math_tan(double x) { return tan(x); }\n");
                c_code.push_str("double tauraro_math_asin(double x) { return asin(x); }\n");
                c_code.push_str("double tauraro_math_acos(double x) { return acos(x); }\n");
                c_code.push_str("double tauraro_math_atan(double x) { return atan(x); }\n");
                c_code.push_str("double tauraro_math_atan2(double y, double x) { return atan2(y, x); }\n");
                c_code.push_str("double tauraro_math_sinh(double x) { return sinh(x); }\n");
                c_code.push_str("double tauraro_math_cosh(double x) { return cosh(x); }\n");
                c_code.push_str("double tauraro_math_tanh(double x) { return tanh(x); }\n");
                c_code.push_str("double tauraro_math_pow(double x, double y) { return pow(x, y); }\n");
                c_code.push_str("double tauraro_math_exp(double x) { return exp(x); }\n");
                c_code.push_str("double tauraro_math_log(double x) { return log(x); }\n");
                c_code.push_str("double tauraro_math_log2(double x) { return log2(x); }\n");
                c_code.push_str("double tauraro_math_log10(double x) { return log10(x); }\n");
                c_code.push_str("double tauraro_math_ceil(double x) { return ceil(x); }\n");
                c_code.push_str("double tauraro_math_floor(double x) { return floor(x); }\n");
                c_code.push_str("double tauraro_math_fabs(double x) { return fabs(x); }\n");
                c_code.push_str("double tauraro_math_fmod(double x, double y) { return fmod(x, y); }\n");
            },
            "os" => {
                // OS module constants and functions
                c_code.push_str("// OS module constants\n");
                c_code.push_str("#ifdef _WIN32\n");
                c_code.push_str("const char* TAURARO_OS_NAME = \"Windows\";\n");
                c_code.push_str("#elif __APPLE__\n");
                c_code.push_str("const char* TAURARO_OS_NAME = \"macOS\";\n");
                c_code.push_str("#elif __linux__\n");
                c_code.push_str("const char* TAURARO_OS_NAME = \"Linux\";\n");
                c_code.push_str("#else\n");
                c_code.push_str("const char* TAURARO_OS_NAME = \"Unknown\";\n");
                c_code.push_str("#endif\n\n");
                
                c_code.push_str("// OS module functions\n");
                c_code.push_str("const char* tauraro_os_getenv(const char* name) { return getenv(name); }\n");
                c_code.push_str("int tauraro_os_system(const char* command) { return system(command); }\n");
            },
            "sys" => {
                // Sys module constants and functions
                c_code.push_str("// Sys module constants\n");
                c_code.push_str("#ifdef _WIN32\n");
                c_code.push_str("const char* TAURARO_SYS_PLATFORM = \"Windows\";\n");
                c_code.push_str("#elif __APPLE__\n");
                c_code.push_str("const char* TAURARO_SYS_PLATFORM = \"Darwin\";\n");
                c_code.push_str("#elif __linux__\n");
                c_code.push_str("const char* TAURARO_SYS_PLATFORM = \"Linux\";\n");
                c_code.push_str("#else\n");
                c_code.push_str("const char* TAURARO_SYS_PLATFORM = \"Unknown\";\n");
                c_code.push_str("#endif\n\n");
                
                c_code.push_str("const char* TAURARO_SYS_VERSION = \"Tauraro 1.0\";\n\n");
                
                c_code.push_str("// Sys module functions\n");
                c_code.push_str("void tauraro_sys_exit(int status) { exit(status); }\n");
            },
            _ => {
                // Generic module - implementations will be linked from object files
                c_code.push_str("// Generic built-in module - implementations will be linked from object files\n");
            }
        }
        
        // Add a module initialization function
        c_code.push_str("\n// Module initialization\n");
        c_code.push_str(&format!("void tauraro_{}_init() {{\n", module_name));
        c_code.push_str("    // Module initialization code would go here\n");
        c_code.push_str("}\n");
        
        Ok(c_code)
    }

    /// Compile all modules to native with specific options
    pub fn compile_to_native_with_options(
        &self,
        compiled_files: &HashMap<String, Vec<u8>>,
        main_module_name: &str,
        output_options: &CodegenOptions,
        output_type: OutputType,
        target: &str,
    ) -> Result<PathBuf, Box<dyn std::error::Error>> {
        use crate::codegen::native::{NativeCompiler, TargetPlatform};

        let compiler = NativeCompiler::new();
        let platform = TargetPlatform::detect();

        // Check if we have imports (which would mean we created a build directory)
        let has_imports = compiled_files.len() > 1 || 
            (compiled_files.len() == 1 && compiled_files.contains_key(main_module_name) && {
                // Check if the main module file exists in the build directory
                let main_c_file = self.build_dir.join(format!("{}.c", main_module_name));
                main_c_file.exists()
            });

        // Collect all C files that need to be compiled
        let mut c_files = Vec::new();
        
        if has_imports {
            // For files with imports, we only have the main C file now
            // All imported modules are in header files with implementations
            let main_c_file = self.build_dir.join(format!("{}.c", main_module_name));
            c_files.push(main_c_file);
        } else {
            // For single files without imports, use a temporary file
            let temp_dir = std::env::temp_dir();
            let temp_file = temp_dir.join(format!("{}.c", main_module_name));
            
            // Write the C code to the temporary file
            if let Some(c_code) = compiled_files.get(main_module_name) {
                fs::write(&temp_file, c_code)?;
            }
            
            c_files.push(temp_file);
        }
        
        // Convert to array of path references
        let c_file_refs: Vec<&Path> = c_files.iter().map(|p| p.as_path()).collect();
        
        // Determine output name based on output type and target
        let output_name = match &output_type {
            OutputType::Executable => {
                if target == "android" {
                    format!("{}.apk", main_module_name)
                } else {
                    let ext = platform.executable_extension();
                    if ext.is_empty() {
                        main_module_name.to_string()
                    } else {
                        format!("{}.{}", main_module_name, ext)
                    }
                }
            },
            OutputType::SharedLibrary => {
                let ext = if !target.is_empty() && target != "native" {
                    target.to_string()
                } else {
                    platform.library_extension().to_string()
                };
                format!("{}.{}", main_module_name, ext)
            },
            OutputType::StaticLibrary => {
                format!("{}.a", main_module_name)
            },
            OutputType::ObjectFile => {
                let ext = platform.object_extension();
                format!("{}.{}", main_module_name, ext)
            },
        };
        
        // Determine output path
        let output_path = if has_imports {
            self.build_dir.join(&output_name)
        } else {
            // For single files, put output in current directory
            PathBuf::from(&output_name)
        };
        
        // Convert built-in object files to path references
        let obj_file_refs: Vec<PathBuf> = self.builtin_compiler.get_object_files();
        let obj_file_refs: Vec<&Path> = obj_file_refs.iter().map(|p| p.as_path()).collect();
        
        compiler.compile_multiple_c_to_native(
            &c_file_refs,
            &obj_file_refs,
            Some(&output_path),
            output_type,
            output_options.export_symbols,
        ).map_err(|e| anyhow!("Native compilation failed: {}", e))?;

        // Clean up temporary file if we created one
        if !has_imports {
            if let Some(temp_file) = c_files.first() {
                let _ = fs::remove_file(temp_file);
            }
        }

        Ok(output_path)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::path::PathBuf;

    #[test]
    fn test_import_compiler_creation() {
        let build_dir = PathBuf::from("test_build");
        let compiler = ImportCompiler::new(build_dir);
        assert_eq!(compiler.compiled_modules.len(), 0);
    }
}