//! Optimized Native C Code Generation for Tauraro
//!
//! This module provides high-performance C code generation using native types

use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::fs;
use crate::ast::*;
use crate::codegen::c_transpiler::native_types::{NativeType, NativeTypeContext};
use crate::codegen::c_transpiler::optimizer::NativeOptimizer;
use crate::codegen::c_transpiler::memory_management::{MemoryCodeGenerator, MemoryStrategy};
use crate::codegen::c_transpiler::imports::{ImportAnalyzer, ModuleInfo, ModuleType};
use crate::codegen::c_transpiler::module_system::ModuleCompiler;
use anyhow::Result;

/// FFI function signature information
#[derive(Debug, Clone)]
struct FFIFunctionInfo {
    library_name: String,
    function_name: String,
    return_type: String,
    param_types: Vec<String>,
    func_ptr_var: String,  // Name of the function pointer variable
}

/// User-defined function signature information
#[derive(Debug, Clone)]
struct FunctionSignature {
    param_types: Vec<NativeType>,
    return_type: NativeType,
}

pub struct OptimizedNativeTranspiler {
    /// Type context for tracking variable types
    context: NativeTypeContext,
    /// Current indentation level
    indent_level: usize,
    /// Counter for temporary variables
    temp_counter: usize,
    /// Optimizer for code generation
    optimizer: NativeOptimizer,
    /// Enable optimizations
    optimizations_enabled: bool,
    /// Memory management code generator
    memory_manager: MemoryCodeGenerator,
    /// FFI: Loaded libraries (library name -> handle variable)
    ffi_libraries: HashMap<String, String>,
    /// FFI: Defined functions
    ffi_functions: HashMap<String, FFIFunctionInfo>,
    /// FFI: Maps variable names to FFI function info (for call_function)
    ffi_variable_map: HashMap<String, String>,  // variable_name -> function_name
    /// FFI: Enable FFI support
    ffi_enabled: bool,
    /// User-defined function signatures for proper type conversion
    function_signatures: HashMap<String, FunctionSignature>,
    /// Module import analyzer
    import_analyzer: ImportAnalyzer,
    /// Module compiler for handling built-in and user modules
    module_compiler: Option<ModuleCompiler>,
    /// Build directory for compiled modules
    build_dir: Option<PathBuf>,
}

impl OptimizedNativeTranspiler {
    pub fn new() -> Self {
        Self {
            context: NativeTypeContext::new(),
            indent_level: 0,
            temp_counter: 0,
            optimizer: NativeOptimizer::new(),
            optimizations_enabled: true,
            memory_manager: MemoryCodeGenerator::new(MemoryStrategy::default()),
            ffi_libraries: HashMap::new(),
            ffi_functions: HashMap::new(),
            ffi_variable_map: HashMap::new(),
            ffi_enabled: false,
            function_signatures: HashMap::new(),
            import_analyzer: ImportAnalyzer::new(),
            module_compiler: None,
            build_dir: None,
        }
    }

    pub fn with_build_dir(mut self, build_dir: PathBuf) -> Self {
        self.build_dir = Some(build_dir.clone());
        self.module_compiler = Some(ModuleCompiler::new(build_dir));
        self
    }

    /// Get object files that need to be linked (for built-in modules)
    pub fn get_object_files(&self) -> Vec<PathBuf> {
        if let Some(ref compiler) = self.module_compiler {
            compiler.get_object_files().to_vec()
        } else {
            Vec::new()
        }
    }

    /// Get the build directory path
    pub fn get_build_dir(&self) -> Option<&PathBuf> {
        self.build_dir.as_ref()
    }

    pub fn with_optimizations(mut self, enabled: bool) -> Self {
        self.optimizations_enabled = enabled;
        self
    }

    pub fn with_memory_strategy(mut self, strategy: MemoryStrategy) -> Self {
        self.memory_manager = MemoryCodeGenerator::new(strategy);
        self
    }

    /// Generate optimized C code from AST program
    pub fn transpile_program(&mut self, program: &Program) -> Result<String, String> {
        // Apply optimizations if enabled
        let mut optimized_program = program.clone();
        if self.optimizations_enabled {
            self.optimizer.optimize_program(&mut optimized_program);
        }

        let program = &optimized_program;

        // First pass: detect FFI usage (BEFORE generating headers)
        self.detect_ffi_usage(program);

        // Second pass: register user-defined function signatures
        self.register_function_signatures(program)?;

        // Third pass: analyze imports and set up module compilation
        self.import_analyzer.analyze(program)
            .map_err(|e| format!("Import analysis failed: {}", e))?;

        // Initialize build directory and compile modules if needed
        if let Some(ref mut compiler) = self.module_compiler {
            compiler.init_build_dir()
                .map_err(|e| format!("Failed to initialize build directory: {}", e))?;

            // Compile built-in modules to object files
            for module_info in self.import_analyzer.get_builtin_modules() {
                compiler.compile_builtin_module(&module_info.name)
                    .map_err(|e| format!("Failed to compile built-in module '{}': {}", module_info.name, e))?;
            }

            // Compile user-defined modules to header files
            for module_info in self.import_analyzer.get_user_modules() {
                if let Some(ref path) = module_info.file_path {
                    compiler.convert_user_module_to_header(path, &module_info.name)
                        .map_err(|e| format!("Failed to compile user module '{}': {}", module_info.name, e))?;
                }
            }
        }

        let mut code = String::new();

        // Generate headers (includes native type system and builtins, and FFI if detected)
        code.push_str(self.generate_headers().as_str());

        // Generate forward declarations
        code.push_str("// Forward declarations\n");
        for stmt in &program.statements {
            if let Statement::FunctionDef { name, .. } = stmt {
                code.push_str(&format!("// Function: {}\n", name));
            }
            if let Statement::ClassDef { name, .. } = stmt {
                code.push_str(&format!("typedef struct {} {}_t;\n", name, name));
            }
        }
        code.push_str("\n");

        // Generate FFI globals
        code.push_str(&self.generate_ffi_globals());

        // Generate runtime operator implementations
        code.push_str(&Self::generate_runtime_operators());

        // Generate classes
        for stmt in &program.statements {
            if let Statement::ClassDef { .. } = stmt {
                code.push_str(&self.transpile_class(stmt)?);
                code.push_str("\n");
            }
        }

        // Generate functions
        for stmt in &program.statements {
            if let Statement::FunctionDef { .. } = stmt {
                code.push_str(&self.transpile_function(stmt)?);
                code.push_str("\n");
            }
        }

        // Generate main function
        code.push_str(&self.generate_main_function(program)?);

        Ok(code)
    }

    fn generate_headers(&self) -> String {
        let mut headers = String::new();
        headers.push_str("// Generated by Tauraro Optimized Native C Transpiler\n");
        headers.push_str("#include <stdio.h>\n");
        headers.push_str("#include <stdlib.h>\n");
        headers.push_str("#include <stdint.h>\n");
        headers.push_str("#include <stdbool.h>\n");
        headers.push_str("#include <string.h>\n");
        headers.push_str("#include <math.h>\n");
        headers.push_str("#include <setjmp.h>\n\n");

        // Add Tauraro Runtime Type System (always needed for function parameters)
        headers.push_str("// Tauraro Runtime Type System\n");
        headers.push_str("typedef enum {\n");
        headers.push_str("    TAURARO_NONE,\n");
        headers.push_str("    TAURARO_INT,\n");
        headers.push_str("    TAURARO_FLOAT,\n");
        headers.push_str("    TAURARO_BOOL,\n");
        headers.push_str("    TAURARO_STRING,\n");
        headers.push_str("    TAURARO_LIST,\n");
        headers.push_str("    TAURARO_DICT,\n");
        headers.push_str("    TAURARO_TUPLE,\n");
        headers.push_str("    TAURARO_SET,\n");
        headers.push_str("    TAURARO_FUNCTION,\n");
        headers.push_str("    TAURARO_OBJECT,\n");
        headers.push_str("    TAURARO_BYTES,\n");
        headers.push_str("    TAURARO_COMPLEX,\n");
        headers.push_str("    TAURARO_RANGE,\n");
        headers.push_str("    TAURARO_FROZENSET\n");
        headers.push_str("} tauraro_type_t;\n\n");

        headers.push_str("typedef struct tauraro_value {\n");
        headers.push_str("    tauraro_type_t type;\n");
        headers.push_str("    int ref_count;\n");
        headers.push_str("    union {\n");
        headers.push_str("        int64_t int_val;\n");
        headers.push_str("        double float_val;\n");
        headers.push_str("        bool bool_val;\n");
        headers.push_str("        char* str_val;\n");
        headers.push_str("        void* ptr_val;\n");
        headers.push_str("        void* obj_val;\n");
        headers.push_str("    } data;\n");
        headers.push_str("} tauraro_value_t;\n\n");

        // Add runtime operator functions for dynamic types
        headers.push_str("// Runtime operators for dynamic types\n");
        headers.push_str("tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right);\n");
        headers.push_str("tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right);\n");
        headers.push_str("tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right);\n");
        headers.push_str("tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right);\n");
        headers.push_str("tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right);\n");
        headers.push_str("tauraro_value_t* tauraro_pow(tauraro_value_t* left, tauraro_value_t* right);\n\n");

        // Add FFI support if needed
        if self.ffi_enabled {
            headers.push_str("// FFI Support\n");
            headers.push_str("#ifdef _WIN32\n");
            headers.push_str("    #include <windows.h>\n");
            headers.push_str("    typedef HMODULE ffi_lib_handle;\n");
            headers.push_str("    #define FFI_DLOPEN(name) LoadLibraryA(name)\n");
            headers.push_str("    #define FFI_DLSYM(handle, name) GetProcAddress(handle, name)\n");
            headers.push_str("    #define FFI_DLCLOSE(handle) FreeLibrary(handle)\n");
            headers.push_str("#else\n");
            headers.push_str("    #include <dlfcn.h>\n");
            headers.push_str("    typedef void* ffi_lib_handle;\n");
            headers.push_str("    #define FFI_DLOPEN(name) dlopen(name, RTLD_LAZY)\n");
            headers.push_str("    #define FFI_DLSYM(handle, name) dlsym(handle, name)\n");
            headers.push_str("    #define FFI_DLCLOSE(handle) dlclose(handle)\n");
            headers.push_str("#endif\n");
        }
        headers.push_str("\n");

        // Add memory management runtime
        headers.push_str(&self.memory_manager.generate_runtime_header());
        headers.push_str("\n");

        // Add native type system BEFORE builtin implementations (builtins need full struct definitions)
        headers.push_str(&crate::codegen::c_transpiler::native_types::generate_native_type_declarations());
        headers.push_str("\n");

        // Add built-in function implementations
        headers.push_str(&crate::codegen::c_transpiler::native_builtins::NativeBuiltins::generate_all_implementations());
        headers.push_str("\n");

        // Include user-defined module headers
        if let Some(ref compiler) = self.module_compiler {
            headers.push_str("// User-defined module headers\n");
            for header_file in compiler.get_header_files() {
                if let Some(file_name) = header_file.file_name() {
                    headers.push_str(&format!("#include \"include/{}\"\n", file_name.to_string_lossy()));
                }
            }
            headers.push_str("\n");
        }

        // Add extern declarations for built-in modules
        for module_info in self.import_analyzer.get_builtin_modules() {
            headers.push_str(&format!("// Built-in module: {}\n", module_info.name));
            headers.push_str(&self.generate_builtin_module_externs(&module_info.name));
            headers.push_str("\n");
        }

        headers
    }

    /// Generate extern declarations for built-in module symbols
    fn generate_builtin_module_externs(&self, module_name: &str) -> String {
        let mut externs = String::new();

        match module_name {
            "math" => {
                externs.push_str("extern double tauraro_math_pi;\n");
                externs.push_str("extern double tauraro_math_e;\n");
                externs.push_str("extern double tauraro_math_sqrt_native(double);\n");
                externs.push_str("extern double tauraro_math_pow_native(double, double);\n");
                externs.push_str("extern double tauraro_math_sin_native(double);\n");
                externs.push_str("extern double tauraro_math_cos_native(double);\n");
                externs.push_str("extern double tauraro_math_tan_native(double);\n");
                externs.push_str("extern double tauraro_math_log_native(double);\n");
                externs.push_str("extern double tauraro_math_exp_native(double);\n");
                externs.push_str("extern double tauraro_math_floor_native(double);\n");
                externs.push_str("extern double tauraro_math_ceil_native(double);\n");
            }
            "sys" => {
                externs.push_str("extern const char* tauraro_sys_platform;\n");
                externs.push_str("extern const char* tauraro_sys_version;\n");
                externs.push_str("extern void tauraro_sys_exit_native(int);\n");
            }
            "os" => {
                externs.push_str("extern char* tauraro_os_getcwd_native();\n");
            }
            "time" => {
                externs.push_str("extern double tauraro_time_time_native();\n");
                externs.push_str("extern void tauraro_time_sleep_native(double);\n");
            }
            "random" => {
                externs.push_str("extern double tauraro_random_random_native();\n");
                externs.push_str("extern int64_t tauraro_random_randint_native(int64_t, int64_t);\n");
                externs.push_str("extern void tauraro_random_seed_native(int64_t);\n");
            }
            "json" => {
                externs.push_str("extern char* tauraro_json_dumps_native(const char*);\n");
                externs.push_str("extern void* tauraro_json_loads_native(const char*);\n");
            }
            _ => {}
        }

        externs
    }

    /// Generate runtime operator implementations for dynamic types
    fn generate_runtime_operators() -> String {
        let mut code = String::new();
        code.push_str("// Runtime operator implementations for dynamic types\n\n");

        // String concatenation helper for native strings
        code.push_str("char* tauraro_string_concat(const char* s1, const char* s2) {\n");
        code.push_str("    if (!s1 || !s2) return NULL;\n");
        code.push_str("    size_t len1 = strlen(s1);\n");
        code.push_str("    size_t len2 = strlen(s2);\n");
        code.push_str("    char* result = (char*)malloc(len1 + len2 + 1);\n");
        code.push_str("    if (!result) return NULL;\n");
        code.push_str("    strcpy(result, s1);\n");
        code.push_str("    strcat(result, s2);\n");
        code.push_str("    return result;\n");
        code.push_str("}\n\n");

        // Helper functions to create tauraro_value_t from native types
        code.push_str("tauraro_value_t* tauraro_value_new_int(int64_t val) {\n");
        code.push_str("    tauraro_value_t* v = (tauraro_value_t*)malloc(sizeof(tauraro_value_t));\n");
        code.push_str("    v->type = TAURARO_INT;\n");
        code.push_str("    v->ref_count = 1;\n");
        code.push_str("    v->data.int_val = val;\n");
        code.push_str("    return v;\n");
        code.push_str("}\n\n");

        code.push_str("tauraro_value_t* tauraro_value_new_float(double val) {\n");
        code.push_str("    tauraro_value_t* v = (tauraro_value_t*)malloc(sizeof(tauraro_value_t));\n");
        code.push_str("    v->type = TAURARO_FLOAT;\n");
        code.push_str("    v->ref_count = 1;\n");
        code.push_str("    v->data.float_val = val;\n");
        code.push_str("    return v;\n");
        code.push_str("}\n\n");

        code.push_str("tauraro_value_t* tauraro_value_new_bool(bool val) {\n");
        code.push_str("    tauraro_value_t* v = (tauraro_value_t*)malloc(sizeof(tauraro_value_t));\n");
        code.push_str("    v->type = TAURARO_BOOL;\n");
        code.push_str("    v->ref_count = 1;\n");
        code.push_str("    v->data.bool_val = val;\n");
        code.push_str("    return v;\n");
        code.push_str("}\n\n");

        code.push_str("tauraro_value_t* tauraro_value_new_string(const char* val) {\n");
        code.push_str("    tauraro_value_t* v = (tauraro_value_t*)malloc(sizeof(tauraro_value_t));\n");
        code.push_str("    v->type = TAURARO_STRING;\n");
        code.push_str("    v->ref_count = 1;\n");
        code.push_str("    v->data.str_val = strdup(val);\n");
        code.push_str("    return v;\n");
        code.push_str("}\n\n");

        // tauraro_add implementation
        code.push_str("tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right) {\n");
        code.push_str("    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {\n");
        code.push_str("        return tauraro_value_new_int(left->data.int_val + right->data.int_val);\n");
        code.push_str("    }\n");
        code.push_str("    // TODO: Handle other type combinations\n");
        code.push_str("    return tauraro_value_new_int(0);\n");
        code.push_str("}\n\n");

        // tauraro_sub implementation
        code.push_str("tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right) {\n");
        code.push_str("    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {\n");
        code.push_str("        return tauraro_value_new_int(left->data.int_val - right->data.int_val);\n");
        code.push_str("    }\n");
        code.push_str("    return tauraro_value_new_int(0);\n");
        code.push_str("}\n\n");

        // tauraro_mul implementation
        code.push_str("tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right) {\n");
        code.push_str("    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {\n");
        code.push_str("        return tauraro_value_new_int(left->data.int_val * right->data.int_val);\n");
        code.push_str("    }\n");
        code.push_str("    return tauraro_value_new_int(0);\n");
        code.push_str("}\n\n");

        // tauraro_div implementation
        code.push_str("tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right) {\n");
        code.push_str("    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {\n");
        code.push_str("        if (right->data.int_val != 0) {\n");
        code.push_str("            return tauraro_value_new_int(left->data.int_val / right->data.int_val);\n");
        code.push_str("        }\n");
        code.push_str("    }\n");
        code.push_str("    return tauraro_value_new_int(0);\n");
        code.push_str("}\n\n");

        // tauraro_mod implementation
        code.push_str("tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right) {\n");
        code.push_str("    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {\n");
        code.push_str("        if (right->data.int_val != 0) {\n");
        code.push_str("            return tauraro_value_new_int(left->data.int_val % right->data.int_val);\n");
        code.push_str("        }\n");
        code.push_str("    }\n");
        code.push_str("    return tauraro_value_new_int(0);\n");
        code.push_str("}\n\n");

        // tauraro_pow implementation
        code.push_str("tauraro_value_t* tauraro_pow(tauraro_value_t* left, tauraro_value_t* right) {\n");
        code.push_str("    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {\n");
        code.push_str("        int64_t result = 1;\n");
        code.push_str("        for (int64_t i = 0; i < right->data.int_val; i++) {\n");
        code.push_str("            result *= left->data.int_val;\n");
        code.push_str("        }\n");
        code.push_str("        return tauraro_value_new_int(result);\n");
        code.push_str("    }\n");
        code.push_str("    return tauraro_value_new_int(0);\n");
        code.push_str("}\n\n");

        code
    }

    /// Generate FFI global variables and function pointers
    fn generate_ffi_globals(&self) -> String {
        if !self.ffi_enabled {
            return String::new();
        }

        let mut code = String::new();
        code.push_str("// FFI Global Variables\n");

        // Generate library handle variables
        for (lib_name, handle_var) in &self.ffi_libraries {
            code.push_str(&format!("static ffi_lib_handle {} = NULL;\n", handle_var));
        }

        // Generate function pointer variables
        for (_, func_info) in &self.ffi_functions {
            code.push_str(&self.generate_ffi_function_pointer_decl(func_info));
        }

        code.push_str("\n");
        code
    }

    /// Generate function pointer declaration for FFI function
    fn generate_ffi_function_pointer_decl(&self, func_info: &FFIFunctionInfo) -> String {
        let return_c_type = self.ffi_type_to_c(&func_info.return_type);
        let param_c_types: Vec<String> = func_info.param_types.iter()
            .map(|t| self.ffi_type_to_c(t))
            .collect();

        let params_str = if param_c_types.is_empty() {
            "void".to_string()
        } else {
            param_c_types.join(", ")
        };

        format!("static {} (*{})({}); // FFI: {}\n",
            return_c_type,
            func_info.func_ptr_var,
            params_str,
            func_info.function_name)
    }

    /// Convert FFI type string to C type
    fn ffi_type_to_c(&self, ffi_type: &str) -> String {
        match ffi_type {
            "void" => "void".to_string(),
            "int" | "int32" => "int32_t".to_string(),
            "int64" => "int64_t".to_string(),
            "uint" | "uint32" => "uint32_t".to_string(),
            "uint64" => "uint64_t".to_string(),
            "float" => "float".to_string(),
            "double" => "double".to_string(),
            "char" => "char".to_string(),
            "string" => "char*".to_string(),
            "pointer" => "void*".to_string(),
            "bool" => "bool".to_string(),
            _ => "void*".to_string(),  // Default to void pointer
        }
    }

    /// Detect FFI usage in the program (first pass)
    fn detect_ffi_usage(&mut self, program: &Program) {
        for stmt in &program.statements {
            self.detect_ffi_in_statement(stmt);
        }
    }

    /// Recursively detect FFI calls in statements
    fn detect_ffi_in_statement(&mut self, stmt: &Statement) {
        match stmt {
            Statement::Expression(expr) => self.detect_ffi_in_expr(expr),
            Statement::VariableDef { value: Some(expr), .. } => self.detect_ffi_in_expr(expr),
            Statement::AttributeAssignment { value, .. } => self.detect_ffi_in_expr(value),
            Statement::SubscriptAssignment { value, .. } => self.detect_ffi_in_expr(value),
            Statement::If { condition, then_branch, else_branch, .. } => {
                self.detect_ffi_in_expr(condition);
                for s in then_branch {
                    self.detect_ffi_in_statement(s);
                }
                if let Some(else_stmts) = else_branch {
                    for s in else_stmts {
                        self.detect_ffi_in_statement(s);
                    }
                }
            }
            Statement::While { condition, body, .. } => {
                self.detect_ffi_in_expr(condition);
                for s in body {
                    self.detect_ffi_in_statement(s);
                }
            }
            Statement::For { iterable, body, .. } => {
                self.detect_ffi_in_expr(iterable);
                for s in body {
                    self.detect_ffi_in_statement(s);
                }
            }
            Statement::FunctionDef { body, .. } => {
                for s in body {
                    self.detect_ffi_in_statement(s);
                }
            }
            Statement::Return(Some(expr)) => self.detect_ffi_in_expr(expr),
            _ => {}
        }
    }

    /// Detect FFI calls in expressions
    fn detect_ffi_in_expr(&mut self, expr: &Expr) {
        match expr {
            Expr::Call { func, args, .. } => {
                if let Expr::Identifier(name) = func.as_ref() {
                    if name == "load_library" || name == "define_function" || name == "call_function" {
                        self.ffi_enabled = true;
                    }
                }
                // Check args recursively
                for arg in args {
                    self.detect_ffi_in_expr(arg);
                }
            }
            Expr::BinaryOp { left, right, .. } => {
                self.detect_ffi_in_expr(left);
                self.detect_ffi_in_expr(right);
            }
            Expr::UnaryOp { operand, .. } => {
                self.detect_ffi_in_expr(operand);
            }
            Expr::List(items) => {
                for item in items {
                    self.detect_ffi_in_expr(item);
                }
            }
            _ => {}
        }
    }

    /// Register function signatures for proper type conversion in function calls
    fn register_function_signatures(&mut self, program: &Program) -> Result<(), String> {
        for stmt in &program.statements {
            if let Statement::FunctionDef { name, params, return_type, .. } = stmt {
                // Determine parameter types
                let param_types: Vec<NativeType> = params.iter()
                    .map(|param| {
                        param.type_annotation.as_ref()
                            .map(|t| self.map_type_to_native(t))
                            .unwrap_or(NativeType::Dynamic)
                    })
                    .collect();

                // Determine return type
                let ret_type = return_type.as_ref()
                    .map(|t| self.map_type_to_native(t))
                    .unwrap_or(NativeType::Dynamic);

                // Register the signature
                self.function_signatures.insert(
                    name.clone(),
                    FunctionSignature {
                        param_types,
                        return_type: ret_type,
                    }
                );
            }
        }
        Ok(())
    }

    fn transpile_class(&mut self, stmt: &Statement) -> Result<String, String> {
        if let Statement::ClassDef { name, bases, body, .. } = stmt {
            let mut code = String::new();

            // Extract fields and methods from class body
            let mut init_method = None;
            let mut class_methods = Vec::new();
            let mut class_fields = Vec::new();

            for item in body {
                if let Statement::FunctionDef { name: method_name, params, return_type, body: method_body, .. } = item {
                    if method_name == "__init__" {
                        init_method = Some((params, method_body));
                        // Extract fields from __init__ method
                        for stmt in method_body {
                            if let Statement::AttributeAssignment { object, name: field_name, value } = stmt {
                                if let Expr::Identifier(obj_name) = object {
                                    if obj_name == "self" {
                                        // Infer field type from the value or params
                                        let field_type = self.infer_field_type(field_name, params, value)?;
                                        class_fields.push((field_name.clone(), field_type));
                                    }
                                }
                            }
                        }
                    } else {
                        class_methods.push((method_name.clone(), params.clone(), return_type.clone(), method_body.clone()));
                    }
                }
            }

            // Generate struct definition
            code.push_str(&format!("// Class: {}\n", name));
            code.push_str(&format!("struct {} {{\n", name));

            // Add base class field if inheritance
            if !bases.is_empty() {
                if let Expr::Identifier(base_name) = &bases[0] {
                    code.push_str(&format!("    struct {} base;\n", base_name));
                }
            }

            // Add reference counting
            code.push_str("    int ref_count;\n");

            // Add class fields with native types
            for (field_name, field_type) in &class_fields {
                code.push_str(&format!("    {} {};\n", field_type.to_c_type(), field_name));
            }

            code.push_str("};\n\n");

            // Generate constructor function
            if let Some((init_params, init_body)) = init_method {
                code.push_str(&self.generate_constructor(name, init_params, &class_fields, init_body)?);
                code.push_str("\n");
            }

            // Generate methods
            for (method_name, params, return_type, method_body) in &class_methods {
                code.push_str(&self.generate_method(name, method_name, params, return_type, method_body)?);
                code.push_str("\n");
            }

            Ok(code)
        } else {
            Err("Expected ClassDef statement".to_string())
        }
    }

    fn infer_field_type(&self, field_name: &str, init_params: &[Param], value: &Expr) -> Result<NativeType, String> {
        // First, check if the field is assigned from a parameter
        for param in init_params {
            if let Expr::Identifier(param_name) = value {
                if &param.name == param_name {
                    // Field is assigned from parameter, use parameter's type
                    if let Some(ref typ) = param.type_annotation {
                        return Ok(self.map_type_to_native(typ));
                    }
                }
            }
        }

        // Otherwise, infer from the value expression
        self.infer_expr_type(value)
    }

    fn generate_constructor(&mut self, class_name: &str, params: &[Param], fields: &[(String, NativeType)], body: &[Statement]) -> Result<String, String> {
        let mut code = String::new();

        // Constructor function signature: ClassName(param1, param2, ...) -> ClassName*
        code.push_str(&format!("struct {}* {}(", class_name, class_name));

        // Skip 'self' parameter in constructor signature
        let mut first = true;
        for param in params {
            if param.name == "self" {
                continue;
            }
            if !first {
                code.push_str(", ");
            }
            first = false;

            let param_type = param.type_annotation.as_ref()
                .map(|t| self.map_type_to_native(t))
                .unwrap_or(NativeType::Dynamic);
            code.push_str(&format!("{} {}", param_type.to_c_type(), param.name));
        }

        if first {
            code.push_str("void");
        }

        code.push_str(") {\n");

        // Allocate struct
        code.push_str(&format!("    struct {}* self = (struct {}*)malloc(sizeof(struct {}));\n",
            class_name, class_name, class_name));
        code.push_str("    if (!self) return NULL;\n");
        code.push_str("    self->ref_count = 1;\n");

        // Initialize fields from constructor body
        self.indent_level += 1;
        self.context.set_variable_type("self".to_string(), NativeType::Struct(class_name.to_string()));

        for stmt in body {
            code.push_str(&self.transpile_statement(stmt)?);
        }

        self.indent_level -= 1;

        code.push_str("    return self;\n");
        code.push_str("}\n");

        Ok(code)
    }

    fn generate_method(&mut self, class_name: &str, method_name: &str, params: &[Param], return_type: &Option<Type>, body: &[Statement]) -> Result<String, String> {
        let mut code = String::new();

        // Method signature: ClassName_method_name(self, param1, param2, ...)
        let method_c_name = format!("{}_{}", class_name, method_name);

        let ret_type = return_type.as_ref()
            .map(|t| self.map_type_to_native(t))
            .unwrap_or(NativeType::Void);

        code.push_str(&format!("{} {}(", ret_type.to_c_type(), method_c_name));

        // Clear local scope
        self.context.clear_local_variables();

        // Add parameters
        for (i, param) in params.iter().enumerate() {
            if i > 0 {
                code.push_str(", ");
            }

            let param_type = if param.name == "self" {
                NativeType::Struct(class_name.to_string())
            } else {
                param.type_annotation.as_ref()
                    .map(|t| self.map_type_to_native(t))
                    .unwrap_or(NativeType::Dynamic)
            };

            // Store parameter type in context
            self.context.set_variable_type(param.name.clone(), param_type.clone());

            // For self, use struct pointer type
            if param.name == "self" {
                code.push_str(&format!("struct {}* {}", class_name, param.name));
            } else {
                code.push_str(&format!("{} {}", param_type.to_c_type(), param.name));
            }
        }

        if params.is_empty() {
            code.push_str("void");
        }

        code.push_str(") {\n");

        // Method body
        self.indent_level += 1;
        for stmt in body {
            code.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;

        code.push_str("}\n");

        Ok(code)
    }

    fn transpile_function(&mut self, stmt: &Statement) -> Result<String, String> {
        if let Statement::FunctionDef { name, params, return_type, body, decorators, .. } = stmt {
            let mut code = String::new();

            // Check for memory management decorators
            let mut func_memory_strategy = None;
            for decorator in decorators {
                if let Expr::Identifier(dec_name) = decorator {
                    if dec_name == "manual_memory" {
                        func_memory_strategy = Some(MemoryStrategy::Manual);
                    } else if dec_name == "arena_memory" {
                        func_memory_strategy = Some(MemoryStrategy::Arena);
                    } else if dec_name == "auto_memory" {
                        func_memory_strategy = Some(MemoryStrategy::Automatic);
                    }
                }
            }

            // Save current strategy and switch if decorator present
            let prev_strategy = if let Some(strategy) = func_memory_strategy {
                let prev = self.memory_manager.context().strategy;
                self.memory_manager = MemoryCodeGenerator::new(strategy);
                Some(prev)
            } else {
                None
            };

            // Determine return type
            // If no return type annotation, use Dynamic (tauraro_value_t*) to allow dynamic returns
            let ret_type = return_type.as_ref()
                .map(|t| self.map_type_to_native(t))
                .unwrap_or(NativeType::Dynamic);

            // Clear local variables from previous function scope
            self.context.clear_local_variables();

            // Function signature
            code.push_str(&ret_type.to_c_type());
            code.push(' ');
            code.push_str(name);
            code.push('(');

            if params.is_empty() {
                code.push_str("void");
            } else {
                for (i, param) in params.iter().enumerate() {
                    if i > 0 {
                        code.push_str(", ");
                    }

                    let param_type = param.type_annotation.as_ref()
                        .map(|t| self.map_type_to_native(t))
                        .unwrap_or(NativeType::Dynamic);

                    code.push_str(&param_type.to_c_type());
                    code.push(' ');
                    code.push_str(&param.name);

                    // Register parameter type
                    self.context.set_variable_type(param.name.clone(), param_type);
                }
            }

            code.push_str(") {\n");

            // Function body
            self.indent_level += 1;
            for body_stmt in body {
                code.push_str(&self.transpile_statement(body_stmt)?);
            }
            self.indent_level -= 1;

            code.push_str("}\n");

            // Restore previous memory strategy
            if let Some(prev) = prev_strategy {
                self.memory_manager = MemoryCodeGenerator::new(prev);
            }

            Ok(code)
        } else {
            Err("Expected FunctionDef statement".to_string())
        }
    }

    fn generate_main_function(&mut self, program: &Program) -> Result<String, String> {
        let mut code = String::new();

        code.push_str("int main(int argc, char** argv) {\n");
        self.indent_level += 1;

        // First transpile all statements to populate FFI tracking
        let mut statements_code = String::new();
        for stmt in &program.statements {
            // Skip function and class definitions
            match stmt {
                Statement::FunctionDef { .. } | Statement::ClassDef { .. } => continue,
                _ => {
                    statements_code.push_str(&self.transpile_statement(stmt)?);
                }
            }
        }

        // Now inject FFI variable declarations at the start of main if needed
        if self.ffi_enabled && (!self.ffi_libraries.is_empty() || !self.ffi_functions.is_empty()) {
            code.push_str(&self.indent());
            code.push_str("// FFI Variables\n");

            // Declare library handles
            for (_, handle_var) in &self.ffi_libraries {
                code.push_str(&self.indent());
                code.push_str(&format!("ffi_lib_handle {} = NULL;\n", handle_var));
            }

            // Declare function pointers
            for (_, func_info) in &self.ffi_functions {
                code.push_str(&self.indent());
                let return_c_type = self.ffi_type_to_c(&func_info.return_type);
                let param_c_types: Vec<String> = func_info.param_types.iter()
                    .map(|t| self.ffi_type_to_c(t))
                    .collect();
                let params_str = if param_c_types.is_empty() {
                    "void".to_string()
                } else {
                    param_c_types.join(", ")
                };
                code.push_str(&format!("{} (*{})({}); // {}\n",
                    return_c_type, func_info.func_ptr_var, params_str, func_info.function_name));
            }
            code.push_str("\n");
        }

        // Append the transpiled statements
        code.push_str(&statements_code);

        code.push_str(&self.indent());
        code.push_str("return 0;\n");

        self.indent_level -= 1;
        code.push_str("}\n");

        Ok(code)
    }

    fn transpile_statement(&mut self, stmt: &Statement) -> Result<String, String> {
        let mut code = self.indent();

        match stmt {
            Statement::Expression(expr) => {
                code.push_str(&self.transpile_expr(expr)?);
                code.push_str(";\n");
            }
            Statement::Return(value) => {
                code.push_str("return");
                if let Some(val) = value {
                    code.push(' ');
                    code.push_str(&self.transpile_expr(val)?);
                }
                code.push_str(";\n");
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                code.push_str(&format!("if ({}) {{\n", self.transpile_expr(condition)?));
                self.indent_level += 1;
                for stmt in then_branch {
                    code.push_str(&self.transpile_statement(stmt)?);
                }
                self.indent_level -= 1;

                // Handle elif branches
                for (elif_cond, elif_body) in elif_branches {
                    code.push_str(&self.indent());
                    code.push_str(&format!("}} else if ({}) {{\n", self.transpile_expr(elif_cond)?));
                    self.indent_level += 1;
                    for stmt in elif_body {
                        code.push_str(&self.transpile_statement(stmt)?);
                    }
                    self.indent_level -= 1;
                }

                if let Some(else_stmts) = else_branch {
                    code.push_str(&self.indent());
                    code.push_str("} else {\n");
                    self.indent_level += 1;
                    for stmt in else_stmts {
                        code.push_str(&self.transpile_statement(stmt)?);
                    }
                    self.indent_level -= 1;
                }

                code.push_str(&self.indent());
                code.push_str("}\n");
            }
            Statement::While { condition, body, else_branch } => {
                code.push_str(&format!("while ({}) {{\n", self.transpile_expr(condition)?));
                self.indent_level += 1;
                for stmt in body {
                    code.push_str(&self.transpile_statement(stmt)?);
                }
                self.indent_level -= 1;
                code.push_str(&self.indent());
                code.push_str("}\n");

                // Note: else_branch on while is Python-specific, would need special handling
                if else_branch.is_some() {
                    code.push_str(&self.indent());
                    code.push_str("/* while-else not yet implemented */\n");
                }
            }
            Statement::For { variable, variables, iterable, body, else_branch } => {
                // Use primary variable or first of variables
                let loop_var = if !variable.is_empty() {
                    variable.clone()
                } else if !variables.is_empty() {
                    variables[0].clone()
                } else {
                    return Err("For loop has no target variable".to_string());
                };

                // Handle range() specially for performance
                if let Expr::Call { func, args, .. } = iterable {
                    if let Expr::Identifier(name) = func.as_ref() {
                        if name == "range" {
                            return self.transpile_range_for(&loop_var, args, body);
                        }
                    }
                }

                code.push_str("/* generic for loop not yet implemented */\n");

                if else_branch.is_some() {
                    code.push_str(&self.indent());
                    code.push_str("/* for-else not yet implemented */\n");
                }
            }
            Statement::VariableDef { name, type_annotation, value } => {
                // Check if this is an FFI function assignment or FFI call result
                let mut is_ffi_function = false;
                let mut ffi_return_type: Option<String> = None;

                if let Some(val) = value {
                    if let Expr::Call { func, args, .. } = val {
                        if let Expr::Identifier(func_name) = func.as_ref() {
                            if func_name == "define_function" {
                                is_ffi_function = true;
                                // Extract function name from arguments to track the mapping
                                if args.len() >= 2 {
                                    if let Expr::Literal(Literal::String(ffi_func_name)) = &args[1] {
                                        self.ffi_variable_map.insert(name.clone(), ffi_func_name.clone());
                                    }
                                }
                            } else if func_name == "call_function" {
                                // This is an FFI call result - infer type from the function
                                if !args.is_empty() {
                                    if let Expr::Identifier(var_name) = &args[0] {
                                        if let Some(func_name) = self.ffi_variable_map.get(var_name) {
                                            if let Some(func_info) = self.ffi_functions.get(func_name) {
                                                ffi_return_type = Some(func_info.return_type.clone());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                let var_type = if let Some(typ) = type_annotation {
                    self.map_type_to_native(typ)
                } else if let Some(ffi_ret_type) = &ffi_return_type {
                    // Use FFI return type
                    match ffi_ret_type.as_str() {
                        "double" => NativeType::Float,
                        "float" => NativeType::Float,
                        "int" | "int32" => NativeType::Int,
                        "int64" => NativeType::Int,
                        _ => NativeType::Dynamic,
                    }
                } else if let Some(val) = value {
                    self.infer_expr_type(val)?
                } else {
                    NativeType::Dynamic
                };

                // Check if variable already exists in current scope
                let already_declared = self.context.get_variable_type(name).is_some();

                if already_declared {
                    // Variable already declared - generate assignment only
                    code.push_str(name);
                    if let Some(val) = value {
                        code.push_str(" = ");
                        code.push_str(&self.transpile_expr(val)?);
                    }
                    code.push_str(";\n");
                } else {
                    // First declaration - generate type and declaration
                    self.context.set_variable_type(name.clone(), var_type.clone());

                    // For FFI functions, use void* instead of tauraro_value_t*
                    // For FFI call results, use the actual FFI return type
                    if is_ffi_function {
                        code.push_str("void*");
                    } else if let Some(ffi_ret_type) = &ffi_return_type {
                        code.push_str(&self.ffi_type_to_c(ffi_ret_type));
                    } else {
                        code.push_str(&var_type.to_c_type());
                    }
                    code.push(' ');
                    code.push_str(name);

                    if let Some(val) = value {
                        code.push_str(" = ");
                        code.push_str(&self.transpile_expr(val)?);
                    }
                    code.push_str(";\n");
                }
            }
            Statement::AttributeAssignment { object, name, value } => {
                code.push_str(&self.transpile_expr(object)?);
                code.push_str(".");
                code.push_str(name);
                code.push_str(" = ");
                code.push_str(&self.transpile_expr(value)?);
                code.push_str(";\n");
            }
            Statement::SubscriptAssignment { object, index, value } => {
                code.push_str(&self.transpile_expr(object)?);
                code.push('[');
                code.push_str(&self.transpile_expr(index)?);
                code.push_str("] = ");
                code.push_str(&self.transpile_expr(value)?);
                code.push_str(";\n");
            }
            Statement::Break => {
                code.push_str("break;\n");
            }
            Statement::Continue => {
                code.push_str("continue;\n");
            }
            Statement::Pass => {
                code.push_str("/* pass */;\n");
            }
            Statement::Try { body, except_handlers, else_branch, finally } => {
                self.transpile_try_statement(body, except_handlers, else_branch, finally, &mut code)?;
            }
            Statement::Raise(exception) => {
                self.transpile_raise_statement(exception, &mut code)?;
            }
            _ => {
                code.push_str("/* unsupported statement */;\n");
            }
        }

        Ok(code)
    }

    fn transpile_expr(&mut self, expr: &Expr) -> Result<String, String> {
        match expr {
            Expr::Literal(lit) => self.transpile_literal(lit),
            Expr::Identifier(name) => Ok(name.clone()),
            Expr::BinaryOp { left, op, right } => {
                let left_code = self.transpile_expr(left)?;
                let right_code = self.transpile_expr(right)?;

                // Check if operands are dynamic types
                let left_type = self.infer_expr_type(left).unwrap_or(NativeType::Dynamic);
                let right_type = self.infer_expr_type(right).unwrap_or(NativeType::Dynamic);

                // If either operand is dynamic, use runtime functions
                if matches!(left_type, NativeType::Dynamic) || matches!(right_type, NativeType::Dynamic) {
                    let runtime_func = match op {
                        BinaryOp::Add => "tauraro_add",
                        BinaryOp::Sub => "tauraro_sub",
                        BinaryOp::Mul => "tauraro_mul",
                        BinaryOp::Div => "tauraro_div",
                        BinaryOp::Mod => "tauraro_mod",
                        BinaryOp::Pow => "tauraro_pow",
                        _ => return Err(format!("Unsupported binary operation for dynamic types: {:?}", op)),
                    };
                    Ok(format!("{}({}, {})", runtime_func, left_code, right_code))
                } else {
                    // Both are native types, use direct operators
                    match op {
                        BinaryOp::Add => {
                            // Check if string concatenation
                            if matches!(left_type, NativeType::String) && matches!(right_type, NativeType::String) {
                                // Use string concatenation helper
                                Ok(format!("tauraro_string_concat({}, {})", left_code, right_code))
                            } else {
                                Ok(format!("({} + {})", left_code, right_code))
                            }
                        }
                        BinaryOp::Sub => Ok(format!("({} - {})", left_code, right_code)),
                        BinaryOp::Mul => Ok(format!("({} * {})", left_code, right_code)),
                        BinaryOp::Div => Ok(format!("({} / {})", left_code, right_code)),
                        BinaryOp::Mod => Ok(format!("({} % {})", left_code, right_code)),
                        BinaryOp::Pow => Ok(format!("pow({}, {})", left_code, right_code)),
                        BinaryOp::And => Ok(format!("({} && {})", left_code, right_code)),
                        BinaryOp::Or => Ok(format!("({} || {})", left_code, right_code)),
                        BinaryOp::BitAnd => Ok(format!("({} & {})", left_code, right_code)),
                        BinaryOp::BitOr => Ok(format!("({} | {})", left_code, right_code)),
                        BinaryOp::BitXor => Ok(format!("({} ^ {})", left_code, right_code)),
                        BinaryOp::LShift => Ok(format!("({} << {})", left_code, right_code)),
                        BinaryOp::RShift => Ok(format!("({} >> {})", left_code, right_code)),
                        _ => Err(format!("Unsupported binary operation: {:?}", op)),
                    }
                }
            }
            Expr::UnaryOp { op, operand } => {
                let operand_code = self.transpile_expr(operand)?;
                let op_str = match op {
                    UnaryOp::Not => "!",
                    UnaryOp::USub => "-",
                    UnaryOp::UAdd => "+",
                    _ => "?",
                };
                Ok(format!("({}{})", op_str, operand_code))
            }
            Expr::Call { func, args, .. } => {
                self.transpile_function_call(func, args)
            }
            Expr::List(items) => {
                let mut code = String::from("{");
                for (i, item) in items.iter().enumerate() {
                    if i > 0 {
                        code.push_str(", ");
                    }
                    code.push_str(&self.transpile_expr(item)?);
                }
                code.push('}');
                Ok(code)
            }
            Expr::Subscript { object, index } => {
                Ok(format!("{}[{}]",
                    self.transpile_expr(object)?,
                    self.transpile_expr(index)?))
            }
            Expr::Attribute { object, name } => {
                // Check if this is a module attribute access (e.g., math.pi)
                if let Expr::Identifier(module_name) = object.as_ref() {
                    // Check if this is an imported module
                    if self.import_analyzer.modules.contains_key(module_name) {
                        // Translate to module-prefixed name (e.g., math.pi -> tauraro_math_pi)
                        return Ok(format!("tauraro_{}_{}", module_name, name));
                    }

                    // Check if it's a struct pointer (self or other object)
                    if let Some(obj_type) = self.context.get_variable_type(module_name) {
                        if matches!(obj_type, NativeType::Struct(_)) {
                            // Use pointer dereference -> for struct pointers
                            return Ok(format!("{}->{}", module_name, name));
                        }
                    }
                }

                // Regular attribute access (e.g., obj.field for non-pointer)
                let obj_code = self.transpile_expr(object)?;
                Ok(format!("{}.{}", obj_code, name))
            }
            Expr::Compare { left, ops, comparators } => {
                // Simplified comparison
                if !ops.is_empty() && !comparators.is_empty() {
                    let left_code = self.transpile_expr(left)?;
                    let right_code = self.transpile_expr(&comparators[0])?;
                    let op_str = match &ops[0] {
                        CompareOp::Eq => "==",
                        CompareOp::NotEq => "!=",
                        CompareOp::Lt => "<",
                        CompareOp::LtE => "<=",
                        CompareOp::Gt => ">",
                        CompareOp::GtE => ">=",
                        _ => "==",
                    };
                    Ok(format!("({} {} {})", left_code, op_str, right_code))
                } else {
                    Ok("false".to_string())
                }
            }
            Expr::ListComp { element, generators } => {
                self.transpile_list_comprehension(element, generators)
            }
            Expr::DictComp { key, value, generators } => {
                self.transpile_dict_comprehension(key, value, generators)
            }
            Expr::SetComp { element, generators } => {
                self.transpile_set_comprehension(element, generators)
            }
            Expr::GeneratorExp { element, generators } => {
                // Generator expressions are more complex - for now convert to list
                self.transpile_list_comprehension(element, generators)
            }
            Expr::FormatString { parts } => {
                self.transpile_format_string(parts)
            }
            _ => Ok("/* unsupported expr */".to_string()),
        }
    }

    fn transpile_literal(&self, lit: &Literal) -> Result<String, String> {
        Ok(match lit {
            Literal::Int(n) => n.to_string(),
            Literal::Float(f) => f.to_string(),
            Literal::String(s) => {
                // Properly escape string literals for C
                let escaped = s
                    .replace("\\", "\\\\")  // Backslash first
                    .replace("\"", "\\\"")  // Quote
                    .replace("\n", "\\n")   // Newline
                    .replace("\r", "\\r")   // Carriage return
                    .replace("\t", "\\t")   // Tab
                    .replace("\0", "\\0");  // Null
                format!("\"{}\"", escaped)
            }
            Literal::Bool(b) => if *b { "true" } else { "false" }.to_string(),
            Literal::None => "NULL".to_string(),
            _ => "NULL".to_string(),
        })
    }

    fn transpile_format_string(&mut self, parts: &[FormatPart]) -> Result<String, String> {
        // Convert f-string to sprintf or direct concatenation
        // For now, generate a sprintf call
        let mut format_str = String::new();
        let mut args = Vec::new();

        for part in parts {
            match part {
                FormatPart::String(s) => {
                    // Escape the string part
                    let escaped = s
                        .replace("\\", "\\\\")
                        .replace("\"", "\\\"")
                        .replace("\n", "\\n")
                        .replace("\r", "\\r")
                        .replace("\t", "\\t");
                    format_str.push_str(&escaped);
                }
                FormatPart::Expression { expr, format_spec, .. } => {
                    // Infer type and add appropriate format specifier
                    let expr_type = self.infer_expr_type(expr)?;
                    let expr_code = self.transpile_expr(expr)?;

                    // Handle format spec if present
                    let fmt = if let Some(spec) = format_spec {
                        // Parse format spec (e.g., ".4f" for floats)
                        if spec.contains('f') || spec.contains('e') || spec.contains('g') {
                            format!("%{}", spec)
                        } else if spec.contains('d') {
                            format!("%{}", spec)
                        } else {
                            // Default based on type
                            match expr_type {
                                NativeType::Int => "%lld".to_string(),
                                NativeType::Float => {
                                    if spec.is_empty() {
                                        "%g".to_string()
                                    } else {
                                        format!("%{}", spec)
                                    }
                                }
                                NativeType::String => "%s".to_string(),
                                NativeType::Bool => "%s".to_string(),
                                _ => "%p".to_string(),
                            }
                        }
                    } else {
                        // Default format based on type
                        match expr_type {
                            NativeType::Int => "%lld".to_string(),
                            NativeType::Float => "%g".to_string(),
                            NativeType::String => "%s".to_string(),
                            NativeType::Bool => "%s".to_string(),
                            _ => "%p".to_string(),
                        }
                    };

                    format_str.push_str(&fmt);

                    // Convert bool to string
                    if expr_type == NativeType::Bool {
                        args.push(format!("({} ? \"True\" : \"False\")", expr_code));
                    } else {
                        args.push(expr_code);
                    }
                }
            }
        }

        // Generate sprintf call to temp buffer
        self.temp_counter += 1;
        let temp_var = format!("_fstr_{}", self.temp_counter);

        // Return a compound expression that allocates, formats, and returns the string
        // For simplicity, we'll just return a direct string if no expressions
        if args.is_empty() {
            Ok(format!("\"{}\"", format_str))
        } else {
            // Need to generate a temporary string
            // This requires statement context, so we return a placeholder
            // that indicates this f-string has dynamic parts
            Ok(format!("/* f-string: {} with args: {:?} */", format_str, args))
        }
    }

    fn transpile_function_call(&mut self, func: &Expr, args: &[Expr]) -> Result<String, String> {
        // Check if this is a method call or module function call
        let (func_name, is_method_call, method_object) = if let Expr::Attribute { object, name } = func {
            if let Expr::Identifier(module_or_obj_name) = object.as_ref() {
                if self.import_analyzer.modules.contains_key(module_or_obj_name) {
                    // Module function call (e.g., math.sqrt(x))
                    (format!("tauraro_{}_{}_native", module_or_obj_name, name), false, None)
                } else {
                    // Check if it's a method call on an object
                    let obj_type = self.context.get_variable_type(module_or_obj_name);
                    if let Some(NativeType::Struct(class_name)) = obj_type {
                        // Method call (e.g., p.get_x())
                        (format!("{}_{}", class_name, name), true, Some(object.as_ref().clone()))
                    } else {
                        return Err(format!("Unknown method call: {}.{}", module_or_obj_name, name));
                    }
                }
            } else {
                return Err("Complex method calls not supported yet".to_string());
            }
        } else if let Expr::Identifier(name) = func {
            (name.clone(), false, None)
        } else {
            return Err("Complex function calls not supported yet".to_string());
        };

        // Handle built-in functions with native implementations
        match func_name.as_str() {
            "print" => return self.generate_print_call(args),
            "len" => {
                if args.len() == 1 {
                    return Ok(format!("strlen({})", self.transpile_expr(&args[0])?));
                }
            }
            "int" => {
                if args.len() == 1 {
                    let arg_type = self.infer_expr_type(&args[0])?;
                    let arg_code = self.transpile_expr(&args[0])?;
                    match arg_type {
                        NativeType::Int => return Ok(arg_code),
                        NativeType::Float => return Ok(format!("tauraro_int_from_float({})", arg_code)),
                        NativeType::String => return Ok(format!("tauraro_int_from_str({})", arg_code)),
                        NativeType::Bool => return Ok(format!("tauraro_int_from_bool({})", arg_code)),
                        _ => return Ok(arg_code),
                    }
                }
            }
            "float" => {
                if args.len() == 1 {
                    let arg_type = self.infer_expr_type(&args[0])?;
                    let arg_code = self.transpile_expr(&args[0])?;
                    match arg_type {
                        NativeType::Float => return Ok(arg_code),
                        NativeType::Int => return Ok(format!("tauraro_float_from_int({})", arg_code)),
                        NativeType::String => return Ok(format!("tauraro_float_from_str({})", arg_code)),
                        NativeType::Bool => return Ok(format!("tauraro_float_from_bool({})", arg_code)),
                        _ => return Ok(arg_code),
                    }
                }
            }
            "str" => {
                if args.len() == 1 {
                    let arg_type = self.infer_expr_type(&args[0])?;
                    let arg_code = self.transpile_expr(&args[0])?;
                    match arg_type {
                        NativeType::String => return Ok(format!("tauraro_str_copy({})", arg_code)),
                        NativeType::Int => return Ok(format!("tauraro_str_from_int({})", arg_code)),
                        NativeType::Float => return Ok(format!("tauraro_str_from_float({})", arg_code)),
                        NativeType::Bool => return Ok(format!("tauraro_str_from_bool({})", arg_code)),
                        _ => return Ok(arg_code),
                    }
                }
            }
            "bool" => {
                if args.len() == 1 {
                    let arg_type = self.infer_expr_type(&args[0])?;
                    let arg_code = self.transpile_expr(&args[0])?;
                    match arg_type {
                        NativeType::Bool => return Ok(arg_code),
                        NativeType::Int => return Ok(format!("tauraro_bool_from_int({})", arg_code)),
                        NativeType::Float => return Ok(format!("tauraro_bool_from_float({})", arg_code)),
                        NativeType::String => return Ok(format!("tauraro_bool_from_str({})", arg_code)),
                        _ => return Ok(arg_code),
                    }
                }
            }
            "abs" | "min" | "max" | "round" | "input" | "pow" => {
                // Try to generate optimized built-in call
                let arg_codes: Vec<String> = args.iter()
                    .map(|a| self.transpile_expr(a))
                    .collect::<Result<_, _>>()?;
                let arg_types: Vec<NativeType> = args.iter()
                    .map(|a| self.infer_expr_type(a))
                    .collect::<Result<_, _>>()?;

                if let Some(optimized) = crate::codegen::c_transpiler::native_builtins::generate_builtin_call(
                    &func_name, &arg_codes, &arg_types
                ) {
                    return Ok(optimized);
                }
            }
            // FFI Functions
            "load_library" => {
                return self.generate_ffi_load_library(args);
            }
            "define_function" => {
                return self.generate_ffi_define_function(args);
            }
            "call_function" => {
                return self.generate_ffi_call_function(args);
            }
            _ => {}
        }

        // Regular function call - check if we need to wrap arguments for dynamic types
        let mut code = format!("{}(", func_name);

        // Check if this is a user-defined function with known signature
        let func_signature = self.function_signatures.get(&func_name).cloned();

        // For method calls, inject object as first parameter (self)
        let mut arg_index = 0;
        if is_method_call {
            if let Some(obj) = method_object {
                code.push_str(&self.transpile_expr(&obj)?);
                arg_index = 1;
            }
        }

        for (i, arg) in args.iter().enumerate() {
            if arg_index > 0 {
                code.push_str(", ");
            }
            arg_index += 1;

            let arg_code = self.transpile_expr(arg)?;

            // Check if we need to wrap this argument for dynamic type
            if let Some(ref sig) = func_signature {
                if i < sig.param_types.len() && sig.param_types[i] == NativeType::Dynamic {
                    // Parameter expects tauraro_value_t*, but we might be passing a native type
                    let arg_type = self.infer_expr_type(arg)?;
                    let wrapped_arg = match arg_type {
                        NativeType::Int => format!("tauraro_value_new_int({})", arg_code),
                        NativeType::Float => format!("tauraro_value_new_float({})", arg_code),
                        NativeType::Bool => format!("tauraro_value_new_bool({})", arg_code),
                        NativeType::String => format!("tauraro_value_new_string({})", arg_code),
                        NativeType::Dynamic => arg_code.clone(), // Already wrapped
                        _ => arg_code.clone(), // Other types, use as-is
                    };
                    code.push_str(&wrapped_arg);
                } else {
                    code.push_str(&arg_code);
                }
            } else {
                // No signature info, use argument as-is
                code.push_str(&arg_code);
            }
        }

        // If method call with no args, still need to close after self
        if is_method_call && args.is_empty() && arg_index == 1 {
            // Already added self, just close
        }

        code.push(')');

        Ok(code)
    }

    fn transpile_range_for(&mut self, loop_var: &str, args: &[Expr], body: &[Statement]) -> Result<String, String> {
        let mut code = self.indent();

        // Parse range arguments
        let (start, end, step) = match args.len() {
            1 => {
                // range(n) -> 0 to n, step 1
                ("0".to_string(), self.transpile_expr(&args[0])?, "1".to_string())
            }
            2 => {
                // range(start, end) -> start to end, step 1
                (self.transpile_expr(&args[0])?, self.transpile_expr(&args[1])?, "1".to_string())
            }
            3 => {
                // range(start, end, step)
                (self.transpile_expr(&args[0])?, self.transpile_expr(&args[1])?, self.transpile_expr(&args[2])?)
            }
            _ => return Err("range() takes 1-3 arguments".to_string()),
        };

        // Register loop variable as int64_t
        self.context.set_variable_type(loop_var.to_string(), NativeType::Int);

        // Generate C for loop
        code.push_str(&format!("for (int64_t {} = {}; {} < {}; {} += {}) {{\n",
            loop_var, start, loop_var, end, loop_var, step));

        self.indent_level += 1;
        for stmt in body {
            code.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;

        code.push_str(&self.indent());
        code.push_str("}\n");

        Ok(code)
    }

    fn generate_print_call(&mut self, args: &[Expr]) -> Result<String, String> {
        if args.is_empty() {
            return Ok("printf(\"\\n\")".to_string());
        }

        let mut format_str = String::new();
        let mut arg_codes = Vec::new();

        for (i, arg) in args.iter().enumerate() {
            if i > 0 {
                format_str.push(' ');
            }

            let arg_type = self.infer_expr_type(arg)?;
            match arg_type {
                NativeType::Int => format_str.push_str("%lld"),
                NativeType::Float => format_str.push_str("%g"),
                NativeType::Bool => format_str.push_str("%s"),
                NativeType::String => format_str.push_str("%s"),
                _ => format_str.push_str("%p"),
            }

            let mut arg_code = self.transpile_expr(arg)?;
            if arg_type == NativeType::Bool {
                arg_code = format!("({} ? \"True\" : \"False\")", arg_code);
            }
            arg_codes.push(arg_code);
        }

        format_str.push_str("\\n");

        let mut code = format!("printf(\"{}\"", format_str);
        for arg in arg_codes {
            code.push_str(", ");
            code.push_str(&arg);
        }
        code.push(')');

        Ok(code)
    }

    fn transpile_list_comprehension(&mut self, element: &Expr, generators: &[crate::ast::Comprehension]) -> Result<String, String> {
        // List comprehensions are converted to statements, not expressions
        // We'll generate a temporary variable and build up the list
        // For now, return a placeholder that indicates this needs statement context

        // Generate a unique temporary variable name
        self.temp_counter += 1;
        let temp_var = format!("_listcomp_{}", self.temp_counter);
        let temp_size = format!("_listcomp_size_{}", self.temp_counter);
        let temp_capacity = format!("_listcomp_cap_{}", self.temp_counter);

        let mut code = String::new();

        // Infer the element type
        let element_type = self.infer_expr_type(element)?;
        let c_type = element_type.to_c_type();

        // Create the dynamic array
        code.push_str(&format!("({{\\n"));
        code.push_str(&format!("    {}* {} = NULL;\\n", c_type, temp_var));
        code.push_str(&format!("    size_t {} = 0;\\n", temp_size));
        code.push_str(&format!("    size_t {} = 0;\\n", temp_capacity));

        // Generate nested loops for each generator
        for (i, gen) in generators.iter().enumerate() {
            let iter_code = self.transpile_expr(&gen.iter)?;

            // Check if it's a range() call
            if let Expr::Call { func, args, .. } = &gen.iter {
                if let Expr::Identifier(name) = &**func {
                    if name == "range" {
                        // Generate optimized range loop
                        let (start, end, step) = match args.len() {
                            1 => ("0".to_string(), self.transpile_expr(&args[0])?, "1".to_string()),
                            2 => (self.transpile_expr(&args[0])?, self.transpile_expr(&args[1])?, "1".to_string()),
                            3 => (self.transpile_expr(&args[0])?, self.transpile_expr(&args[1])?, self.transpile_expr(&args[2])?),
                            _ => return Err("Invalid range() call".to_string()),
                        };

                        code.push_str(&format!("    for (int64_t {} = {}; {} < {}; {} += {}) {{\\n",
                            gen.target, start, gen.target, end, gen.target, step));
                    } else {
                        return Err("Only range() iterators supported in comprehensions for now".to_string());
                    }
                } else {
                    return Err("Only simple function calls supported in comprehensions".to_string());
                }
            } else {
                return Err("Only range() iterators supported in comprehensions for now".to_string());
            }

            // Add conditional filters
            for if_expr in &gen.ifs {
                let cond_code = self.transpile_expr(if_expr)?;
                code.push_str(&format!("        if ({}) {{\\n", cond_code));
            }
        }

        // Append element to the list
        let element_code = self.transpile_expr(element)?;

        code.push_str(&format!("            if ({} >= {}) {{\\n", temp_size, temp_capacity));
        code.push_str(&format!("                {} = ({} == 0) ? 8 : {} * 2;\\n", temp_capacity, temp_capacity, temp_capacity));
        code.push_str(&format!("                {} = realloc({}, {} * sizeof({}));\\n", temp_var, temp_var, temp_capacity, c_type));
        code.push_str(&format!("            }}\\n"));
        code.push_str(&format!("            {}[{}++] = {};\\n", temp_var, temp_size, element_code));

        // Close all the loops and conditions
        for gen in generators.iter().rev() {
            for _ in &gen.ifs {
                code.push_str(&format!("        }}\\n"));
            }
            code.push_str(&format!("    }}\\n"));
        }

        // Return the list (for now, just the pointer)
        code.push_str(&format!("    {};\\n", temp_var));
        code.push_str(&format!("}})"));

        Ok(code)
    }

    fn transpile_dict_comprehension(&mut self, key: &Expr, value: &Expr, generators: &[crate::ast::Comprehension]) -> Result<String, String> {
        // Dict comprehensions are similar to list comprehensions but create key-value pairs
        // For now, return a placeholder
        Ok("/* dict comprehension not yet implemented */".to_string())
    }

    fn transpile_set_comprehension(&mut self, element: &Expr, generators: &[crate::ast::Comprehension]) -> Result<String, String> {
        // Set comprehensions are similar to list comprehensions
        // For now, return a placeholder
        Ok("/* set comprehension not yet implemented */".to_string())
    }

    fn transpile_try_statement(
        &mut self,
        body: &[Statement],
        except_handlers: &[crate::ast::ExceptHandler],
        else_branch: &Option<Vec<Statement>>,
        finally: &Option<Vec<Statement>>,
        code: &mut String,
    ) -> Result<(), String> {
        // C exception handling using setjmp/longjmp
        // We'll create a jump buffer and exception context

        code.push_str(&self.indent());
        code.push_str("{\n");
        self.indent_level += 1;

        // Create exception handling structure
        code.push_str(&self.indent());
        code.push_str("jmp_buf _exception_buf;\n");
        code.push_str(&self.indent());
        code.push_str("int _exception_code = setjmp(_exception_buf);\n\n");

        code.push_str(&self.indent());
        code.push_str("if (_exception_code == 0) {\n");
        self.indent_level += 1;

        // Try block body
        code.push_str(&self.indent());
        code.push_str("// Try block\n");
        for stmt in body {
            code.push_str(&self.transpile_statement(stmt)?);
        }

        // Else branch (executes if no exception)
        if let Some(else_stmts) = else_branch {
            code.push_str(&self.indent());
            code.push_str("// Else block (no exception)\n");
            for stmt in else_stmts {
                code.push_str(&self.transpile_statement(stmt)?);
            }
        }

        self.indent_level -= 1;
        code.push_str(&self.indent());
        code.push_str("}\n");

        // Except handlers
        for (i, handler) in except_handlers.iter().enumerate() {
            let condition = if i == 0 { "else if" } else { "else if" };

            code.push_str(&self.indent());
            if let Some(_exc_type) = &handler.exception_type {
                // Match specific exception type
                code.push_str(&format!("{} (_exception_code > 0) {{\n", condition));
            } else {
                // Catch-all handler
                code.push_str("else {\n");
            }

            self.indent_level += 1;

            // Bind exception to name if provided
            if let Some(exc_name) = &handler.name {
                code.push_str(&self.indent());
                code.push_str(&format!("// Exception: {}\n", exc_name));
            }

            // Except handler body
            for stmt in &handler.body {
                code.push_str(&self.transpile_statement(stmt)?);
            }

            self.indent_level -= 1;
            code.push_str(&self.indent());
            code.push_str("}\n");
        }

        // Finally block (always executes)
        if let Some(finally_stmts) = finally {
            code.push_str(&self.indent());
            code.push_str("// Finally block (always executes)\n");
            for stmt in finally_stmts {
                code.push_str(&self.transpile_statement(stmt)?);
            }
        }

        self.indent_level -= 1;
        code.push_str(&self.indent());
        code.push_str("}\n");

        Ok(())
    }

    fn transpile_raise_statement(&mut self, exception: &Option<Expr>, code: &mut String) -> Result<(), String> {
        code.push_str(&self.indent());

        if let Some(exc_expr) = exception {
            // Raise specific exception
            let exc_code = self.transpile_expr(exc_expr)?;
            code.push_str(&format!("longjmp(_exception_buf, {});\n", exc_code));
        } else {
            // Re-raise current exception
            code.push_str("longjmp(_exception_buf, 1);\n");
        }

        Ok(())
    }

    fn infer_expr_type(&self, expr: &Expr) -> Result<NativeType, String> {
        Ok(match expr {
            Expr::Literal(Literal::Int(_)) => NativeType::Int,
            Expr::Literal(Literal::Float(_)) => NativeType::Float,
            Expr::Literal(Literal::Bool(_)) => NativeType::Bool,
            Expr::Literal(Literal::String(_)) => NativeType::String,
            Expr::Identifier(name) => {
                self.context.get_variable_type(name)
                    .cloned()
                    .unwrap_or(NativeType::Dynamic)
            }
            Expr::Attribute { object, name } => {
                // Check if this is a module attribute (e.g., math.pi)
                if let Expr::Identifier(module_name) = object.as_ref() {
                    if self.import_analyzer.modules.contains_key(module_name) {
                        // Infer type based on module and attribute name
                        match (module_name.as_str(), name.as_str()) {
                            ("math", "pi" | "e") => NativeType::Float,
                            _ => NativeType::Dynamic,
                        }
                    } else {
                        NativeType::Dynamic
                    }
                } else {
                    NativeType::Dynamic
                }
            }
            Expr::BinaryOp { left, op, .. } => {
                match op {
                    BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                        self.infer_expr_type(left)?
                    }
                    _ => NativeType::Bool,
                }
            }
            Expr::Compare { .. } => NativeType::Bool,
            Expr::Call { func, .. } => {
                // Check if it's a user-defined function with known return type
                if let Expr::Identifier(func_name) = func.as_ref() {
                    if let Some(sig) = self.function_signatures.get(func_name) {
                        return Ok(sig.return_type.clone());
                    }
                }
                NativeType::Dynamic
            }
            _ => NativeType::Dynamic,
        })
    }

    fn map_type_to_native(&self, typ: &Type) -> NativeType {
        match typ {
            Type::Simple(s) => match s.as_str() {
                "int" => NativeType::Int,
                "float" => NativeType::Float,
                "bool" => NativeType::Bool,
                "str" => NativeType::String,
                "None" => NativeType::Void,
                name => NativeType::Struct(name.to_string()),
            },
            Type::Generic { name, args } => {
                if name == "list" && !args.is_empty() {
                    let inner = self.map_type_to_native(&args[0]);
                    NativeType::List(Box::new(inner))
                } else {
                    NativeType::Dynamic
                }
            }
            _ => NativeType::Dynamic,
        }
    }

    fn indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }

    /// Generate code for load_library() FFI call
    fn generate_ffi_load_library(&mut self, args: &[Expr]) -> Result<String, String> {
        if args.is_empty() {
            return Err("load_library() requires at least 1 argument".to_string());
        }

        // Extract library name (must be a literal for now)
        let lib_name_str = match &args[0] {
            Expr::Literal(Literal::String(s)) => s.clone(),
            _ => return Err("load_library() argument must be a string literal".to_string()),
        };

        // Try to convert short library names to full paths
        let full_lib_name = if lib_name_str == "m" {
            "libm.so.6".to_string()  // Standard math library on Linux
        } else if !lib_name_str.contains('.') && !lib_name_str.starts_with("lib") {
            format!("lib{}.so", lib_name_str)  // Auto-add lib prefix and .so extension
        } else {
            lib_name_str.clone()
        };

        // Generate handle variable name
        let handle_var = format!("_ffi_lib_{}", self.ffi_libraries.len());

        // Track this library
        self.ffi_libraries.insert(lib_name_str.clone(), handle_var.clone());

        // Generate dlopen code with error checking
        // Note: Variable must be declared at function/file scope before use
        let mut code = String::new();
        code.push_str(&format!("({} = FFI_DLOPEN(\"{}\"), ", handle_var, full_lib_name));
        code.push_str(&format!("{} == NULL ? fprintf(stderr, \"Failed to load library: {}\\n\"), NULL : (void*){})",
            handle_var, full_lib_name, handle_var));

        Ok(code)
    }

    /// Generate code for define_function() FFI call
    fn generate_ffi_define_function(&mut self, args: &[Expr]) -> Result<String, String> {
        if args.len() < 3 {
            return Err("define_function() requires at least 3 arguments".to_string());
        }

        // Extract library name
        let lib_name = match &args[0] {
            Expr::Literal(Literal::String(s)) => s.clone(),
            _ => return Err("define_function() library name must be a string literal".to_string()),
        };

        // Extract function name
        let func_name = match &args[1] {
            Expr::Literal(Literal::String(s)) => s.clone(),
            _ => return Err("define_function() function name must be a string literal".to_string()),
        };

        // Extract return type
        let return_type = match &args[2] {
            Expr::Literal(Literal::String(s)) => s.clone(),
            _ => return Err("define_function() return type must be a string literal".to_string()),
        };

        // Extract parameter types (optional)
        let param_types = if args.len() > 3 {
            match &args[3] {
                Expr::List(items) => {
                    let mut types = Vec::new();
                    for item in items {
                        if let Expr::Literal(Literal::String(s)) = item {
                            types.push(s.clone());
                        } else {
                            return Err("Parameter types must be string literals".to_string());
                        }
                    }
                    types
                }
                _ => Vec::new(),
            }
        } else {
            Vec::new()
        };

        // Check if library is loaded
        let handle_var = self.ffi_libraries.get(&lib_name)
            .ok_or_else(|| format!("Library '{}' not loaded. Call load_library() first.", lib_name))?
            .clone();

        // Generate function pointer variable name
        let func_ptr_var = format!("_ffi_func_{}", self.ffi_functions.len());

        // Create function info
        let func_info = FFIFunctionInfo {
            library_name: lib_name.clone(),
            function_name: func_name.clone(),
            return_type: return_type.clone(),
            param_types: param_types.clone(),
            func_ptr_var: func_ptr_var.clone(),
        };

        // Track this function
        self.ffi_functions.insert(func_name.clone(), func_info.clone());

        // Generate dlsym code with error checking
        // Note: Variable must be declared at function/file scope before use
        let mut code = String::new();
        code.push_str(&format!("({} = (void*)FFI_DLSYM({}, \"{}\"), ", func_ptr_var, handle_var, func_name));
        code.push_str(&format!("{} == NULL ? fprintf(stderr, \"Failed to load function: {}\\n\"), NULL : (void*){})",
            func_ptr_var, func_name, func_ptr_var));

        Ok(code)
    }

    /// Generate code for call_function() FFI call
    fn generate_ffi_call_function(&mut self, args: &[Expr]) -> Result<String, String> {
        if args.is_empty() {
            return Err("call_function() requires at least 1 argument (function reference)".to_string());
        }

        // Extract the variable name holding the function reference
        let var_name = match &args[0] {
            Expr::Identifier(name) => name.clone(),
            _ => return Err("call_function() first argument must be a variable name".to_string()),
        };

        // Look up which function this variable holds
        let func_name = self.ffi_variable_map.get(&var_name)
            .ok_or_else(|| format!("Variable '{}' is not an FFI function. Use define_function() first.", var_name))?
            .clone();

        // Get the function info
        let func_info = self.ffi_functions.get(&func_name)
            .ok_or_else(|| format!("FFI function '{}' not found.", func_name))?
            .clone();

        // Transpile and convert arguments
        let mut call_args = Vec::new();
        let arg_list: Vec<&Expr> = args.iter().skip(1)
            .flat_map(|arg| {
                // Handle list arguments by unpacking them
                match arg {
                    Expr::List(items) => items.iter().collect(),
                    _ => vec![arg],
                }
            })
            .collect();

        for (i, arg) in arg_list.iter().enumerate() {
            let mut arg_code = self.transpile_expr(arg)?;

            // Add type conversions if needed
            if i < func_info.param_types.len() {
                let expected_type = &func_info.param_types[i];
                let arg_type = self.infer_expr_type(arg)?;

                // Convert int to double if needed
                if (expected_type == "double" || expected_type == "float") && arg_type == NativeType::Int {
                    arg_code = format!("(double)({})", arg_code);
                }
                // Convert int to float if needed
                else if expected_type == "float" && arg_type == NativeType::Int {
                    arg_code = format!("(float)({})", arg_code);
                }
            }

            call_args.push(arg_code);
        }

        // Generate the function call
        let mut code = String::new();
        code.push_str(&format!("{}(", func_info.func_ptr_var));
        for (i, arg) in call_args.iter().enumerate() {
            if i > 0 {
                code.push_str(", ");
            }
            code.push_str(arg);
        }
        code.push(')');

        Ok(code)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_transpiler_creation() {
        let transpiler = OptimizedNativeTranspiler::new();
        assert_eq!(transpiler.indent_level, 0);
    }
}
