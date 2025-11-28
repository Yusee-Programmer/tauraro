//! C Transpiler for Tauraro
//!
//! This module transpiles Tauraro IR to high-performance, optimized native C code.
//! Comprehensive support for all Tauraro language constructs with native type inference.

pub mod builtins;
pub mod compiler;
pub mod functions;
pub mod memory_management;
pub mod oop;
pub mod runtime;
pub mod types;
pub mod pure_native;

use crate::ir::{IRModule, IRInstruction, IRFunction};
use crate::value::Value;
use crate::ast::{Type, BinaryOp, Expr, Literal};
use anyhow::Result;
use std::collections::{HashMap, HashSet};

/// Information about a class for OOP support
#[derive(Debug, Clone)]
struct ClassInfo {
    name: String,
    parent: Option<String>,
    fields: Vec<(String, NativeType)>,
    methods: Vec<MethodInfo>,
    constructors: Vec<String>,
    static_methods: Vec<String>,
}

/// Method information for OOP support
#[derive(Debug, Clone)]
struct MethodInfo {
    name: String,
    params: Vec<(String, NativeType)>,
    return_type: NativeType,
    is_static: bool,
    is_virtual: bool,
}

/// Function signature information
#[derive(Debug, Clone)]
struct FunctionInfo {
    name: String,
    params: Vec<(String, NativeType)>,
    return_type: NativeType,
    is_closure: bool,
    captures: Vec<String>,
}

/// Main C Transpiler struct
/// Transpiles Tauraro IR to optimized, native C code
pub struct CTranspiler {
    /// Variable type cache for optimized code generation
    var_types: HashMap<String, NativeType>,
    /// Track which variables have been declared
    declared_vars: HashSet<String>,
    /// Map from original variable names to unique variable names
    var_name_mapping: HashMap<String, String>,
    /// Current indentation level
    indent_level: usize,
    /// Counter for generating unique temporary variable names
    temp_var_counter: usize,
    /// All variables needed in current scope (for two-pass generation)
    scope_variables: HashMap<String, NativeType>,
    /// Current function parameters (should not be redeclared in body)
    function_parameters: HashSet<String>,
    /// Class definitions and their methods
    class_definitions: HashMap<String, ClassInfo>,
    /// Function definitions and signatures
    function_definitions: HashMap<String, FunctionInfo>,
    /// Current function context for nested scopes
    current_function: Option<String>,
    /// Loop nesting level for break/continue
    loop_depth: usize,
    /// Exception handling context
    exception_handlers: Vec<String>,
    /// Import mappings
    imports: HashMap<String, String>,
    /// Track utility functions have been generated
    generated_utilities: HashSet<String>,
    /// Function parameter counts and defaults (func_name -> (param_count, Vec<(param_name, has_default)>))
    function_params: HashMap<String, (usize, Vec<(String, bool)>)>,
    /// Lambdas to generate: (function_name, params, body_instructions, captured_vars)
    lambdas_to_generate: Vec<(String, Vec<String>, Vec<IRInstruction>, Vec<String>, String)>,
    /// Track static type hints for optimization
    static_typed_vars: HashMap<String, crate::ast::Type>,
    /// Enable aggressive optimizations for typed code
    enable_native_types: bool,
}

/// Native C types for optimized transpilation
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
enum NativeType {
    Int64,      // long long - for integers
    Double,     // double - for floats
    CStr,       // const char* - for strings
    Bool,       // int (0/1) - for booleans
    Object,     // TauObject* - for class instances
    Dict,       // TauDict* - for dictionaries
    List,       // TauList* - for lists/arrays
    Function,   // TauFunction* - for functions/closures
    Iterator,   // TauIterator* - for iterators
    Exception,  // TauException* - for exceptions
    Module,     // TauModule* - for imported modules
    Generic,    // TauValue - fallback for complex types
}

impl CTranspiler {
    /// Create a new C transpiler
    pub fn new() -> Self {
        CTranspiler {
            var_types: HashMap::new(),
            declared_vars: HashSet::new(),
            var_name_mapping: HashMap::new(),
            indent_level: 0,
            temp_var_counter: 0,
            scope_variables: HashMap::new(),
            function_parameters: HashSet::new(),
            class_definitions: HashMap::new(),
            function_definitions: HashMap::new(),
            current_function: None,
            loop_depth: 0,
            exception_handlers: Vec::new(),
            imports: HashMap::new(),
            generated_utilities: HashSet::new(),
            function_params: HashMap::new(),
            lambdas_to_generate: Vec::new(),
            static_typed_vars: HashMap::new(),
            enable_native_types: true,
        }
    }

    /// Check if a name is a C reserved keyword and mangle it if needed
    fn mangle_c_keyword(&self, name: &str) -> String {
        // C reserved keywords that must be mangled
        const C_KEYWORDS: &[&str] = &[
            "auto", "break", "case", "char", "const", "continue", "default", "do",
            "double", "else", "enum", "extern", "float", "for", "goto", "if",
            "int", "long", "register", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
            // Additional common C identifiers that may conflict
            "main", "printf", "scanf", "malloc", "free", "strlen", "strcpy", "strcat",
            "strcmp", "memcpy", "memset", "exit", "abort", "NULL", "true", "false",
            "bool", "stdin", "stdout", "stderr", "FILE", "size_t", "time", "clock",
        ];
        
        if C_KEYWORDS.contains(&name) {
            format!("tau_{}", name)
        } else {
            name.to_string()
        }
    }

    /// Compile an Expr to C code that assigns to result_var
    fn expr_to_c_code(&mut self, expr: &Expr, indent_level: usize, result_var: &str) -> Result<String> {
        let ind = "    ".repeat(indent_level);
        let mut output = String::new();
        
        match expr {
            Expr::Identifier(name) => {
                let resolved = self.resolve_var_name(name);
                output.push_str(&format!("{}{} = {};\n", ind, result_var, resolved));
            },
            Expr::Literal(Literal::Int(n)) => {
                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}}};\n", ind, result_var, n));
            },
            Expr::Literal(Literal::Float(f)) => {
                output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = {}}};\n", ind, result_var, f));
            },
            Expr::Literal(Literal::String(s)) => {
                output.push_str(&format!("{}{} = (TauValue){{.type = 2, .value.s = strdup(\"{}\"), .refcount = 1}};\n", ind, result_var, s.replace("\"", "\\\"")));
            },
            Expr::Literal(Literal::Bool(b)) => {
                output.push_str(&format!("{}{} = (TauValue){{.type = 3, .value.i = {}}};\n", ind, result_var, if *b { 1 } else { 0 }));
            },
            Expr::Literal(Literal::None) => {
                output.push_str(&format!("{}{} = tauraro_none();\n", ind, result_var));
            },
            Expr::BinaryOp { left, op, right } => {
                // Compute left and right into temporaries
                let left_var = format!("_left_{}", self.temp_var_counter);
                self.temp_var_counter += 1;
                let right_var = format!("_right_{}", self.temp_var_counter);
                self.temp_var_counter += 1;
                
                output.push_str(&self.expr_to_c_code(left, indent_level, &left_var)?);
                output.push_str(&self.expr_to_c_code(right, indent_level, &right_var)?);
                
                // Handle binary operations
                match op {
                    BinaryOp::Add => {
                        output.push_str(&format!("{}{} = tauraro_add({}, {});\n", ind, result_var, left_var, right_var));
                    },
                    BinaryOp::Sub => {
                        output.push_str(&format!("{}{} = tauraro_sub({}, {});\n", ind, result_var, left_var, right_var));
                    },
                    BinaryOp::Mul => {
                        output.push_str(&format!("{}{} = tauraro_mul({}, {});\n", ind, result_var, left_var, right_var));
                    },
                    BinaryOp::Div => {
                        output.push_str(&format!("{}{} = tauraro_div({}, {});\n", ind, result_var, left_var, right_var));
                    },
                    BinaryOp::Mod => {
                        output.push_str(&format!("{}{} = tauraro_mod({}, {});\n", ind, result_var, left_var, right_var));
                    },
                    BinaryOp::Pow => {
                        output.push_str(&format!("{}{} = tauraro_pow({}, {});\n", ind, result_var, left_var, right_var));
                    },
                    _ => {
                        output.push_str(&format!("{}{} = {};\n", ind, result_var, left_var));
                    }
                }
            },
            _ => {
                // For complex expressions, just use a placeholder 0
                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = 0}};\n", ind, result_var));
            }
        }
        
        Ok(output)
    }

    /// Generate a unique variable name to prevent conflicts
    fn generate_unique_var(&mut self, base_name: &str) -> String {
        let mut counter = 0;
        let mut candidate = base_name.to_string();
        while self.declared_vars.contains(&candidate) {
            counter += 1;
            candidate = format!("{}_v{}", base_name, counter);
        }
        candidate
    }

    /// Get or create a unique variable name for the given original name
    fn get_unique_var_name(&mut self, original_name: &str) -> String {
        if let Some(unique_name) = self.var_name_mapping.get(original_name) {
            unique_name.clone()
        } else {
            let unique_name = self.generate_unique_var(original_name);
            self.var_name_mapping.insert(original_name.to_string(), unique_name.clone());
            unique_name
        }
    }

    /// Resolve variable name to unique name (read-only lookup) and mangle C keywords
    fn resolve_var_name(&self, original_name: &str) -> String {
        let name = self.var_name_mapping.get(original_name).cloned().unwrap_or_else(|| original_name.to_string());
        self.mangle_c_keyword(&name)
    }

    /// Collect all temporary variable names used in a list of instructions
    fn collect_temp_vars_from_instructions(&self, instrs: &[IRInstruction]) -> std::collections::HashSet<String> {
        let mut vars = std::collections::HashSet::new();
        for instr in instrs {
            match instr {
                IRInstruction::LoadGlobal { result, .. } => {
                    vars.insert(self.resolve_var_name(result));
                }
                IRInstruction::LoadTypedGlobal { result, .. } => {
                    vars.insert(self.resolve_var_name(result));
                }
                IRInstruction::BinaryOp { result, left, right, .. } => {
                    vars.insert(self.resolve_var_name(result));
                    vars.insert(self.resolve_var_name(left));
                    vars.insert(self.resolve_var_name(right));
                }
                IRInstruction::TypedBinaryOp { result, left, right, .. } => {
                    vars.insert(self.resolve_var_name(result));
                    vars.insert(self.resolve_var_name(left));
                    vars.insert(self.resolve_var_name(right));
                }
                IRInstruction::UnaryOp { result, .. } => {
                    vars.insert(self.resolve_var_name(result));
                }
                _ => {}
            }
        }
        vars
    }

    /// Declare a variable if not already declared
    fn ensure_var_declared(&mut self, var_name: &str, var_type: NativeType, indent: &str) -> (String, String) {
        if !self.declared_vars.contains(var_name) {
            self.declared_vars.insert(var_name.to_string());
            self.var_types.insert(var_name.to_string(), var_type);
            (format!("{}{} {};", indent, self.format_type(var_type), var_name), var_name.to_string())
        } else {
            // Check if existing type matches
            if let Some(existing_type) = self.var_types.get(var_name) {
                if *existing_type == var_type {
                    // Type matches, no need to redeclare
                    (String::new(), var_name.to_string())
                } else if *existing_type == NativeType::Generic && var_type != NativeType::Generic {
                    // Upgrading from Generic to Specific type - update the tracking
                    self.var_types.insert(var_name.to_string(), var_type);
                    // Remove from Generic list in scope if it was there
                    if let Some(scope_entry) = self.scope_variables.get_mut(var_name) {
                        *scope_entry = var_type;
                    }
                    // Now declare the specific type
                    (format!("{}{} {};", indent, self.format_type(var_type), var_name), var_name.to_string())
                } else {
                    // Type mismatch - generate a unique variable name
                    self.temp_var_counter += 1;
                    let unique_name = format!("{}_v{}", var_name, self.temp_var_counter);
                    self.declared_vars.insert(unique_name.clone());
                    self.var_types.insert(unique_name.clone(), var_type);
                    (format!("{}{} {};", indent, self.format_type(var_type), unique_name), unique_name)
                }
            } else {
                // Variable exists but no type info - assume it's fine
                (String::new(), var_name.to_string())
            }
        }
    }

    /// Transpile an IR module to optimized C code
    pub fn transpile(&self, module: &IRModule) -> Result<String> {
        let mut transpiler = self.clone();
        let mut output = String::new();
        
        // Clear lambdas from previous transpilations
        transpiler.lambdas_to_generate.clear();
        
        // Populate function parameter information for default argument handling
        for (func_name, func) in &module.functions {
            let mut params_info = Vec::new();
            for param in &func.params {
                let has_default = func.defaults.contains_key(param);
                params_info.push((param.clone(), has_default));
            }
            transpiler.function_params.insert(func_name.clone(), (func.params.len(), params_info));
            
            // Also populate function_definitions with type information for function calls
            let mut param_types = Vec::new();
            for param in &func.params {
                let param_native_type = if let Some(ast_type) = func.param_types.get(param) {
                    transpiler.ast_type_to_native(ast_type)
                } else {
                    NativeType::Generic
                };
                param_types.push((param.clone(), param_native_type));
            }
            
            let return_native_type = if let Some(ret_type) = &func.return_type {
                transpiler.ast_type_to_native(ret_type)
            } else {
                NativeType::Generic
            };
            
            let func_info = FunctionInfo {
                name: func_name.clone(),
                params: param_types,
                return_type: return_native_type,
                is_closure: false,
                captures: Vec::new(),
            };
            
            transpiler.function_definitions.insert(func_name.clone(), func_info);
        }

        // Add C header and includes
        output.push_str("#include <stdio.h>\n");
        output.push_str("#include <stdlib.h>\n");
        output.push_str("#include <string.h>\n");
        output.push_str("#include <stdbool.h>\n");
        output.push_str("#include <math.h>\n");
        output.push_str("#include <stdarg.h>\n");
        output.push_str("#include <setjmp.h>\n");
        output.push_str("#include <ctype.h>\n");
        output.push_str("#include <stdint.h>\n");
        output.push_str("\n");

        // Add type definitions
        output.push_str(&transpiler.generate_type_definitions());
        output.push_str("\n");

        // Add utility functions
        output.push_str(&transpiler.generate_utilities());
        output.push_str("\n");

        // Transpile functions (forward declarations first)
        if !module.functions.is_empty() {
            output.push_str("// Function forward declarations\n");
            for (func_name, func) in &module.functions {
                output.push_str(&transpiler.generate_function_signature(func_name, func, true)?);
                output.push_str(";\n");
            }
            output.push_str("\n");

            output.push_str("// Function definitions\n");
            for (_, func) in &module.functions {
                output.push_str(&transpiler.transpile_function(func)?);
            }
            
            // Generate wrapper functions for default arguments
            output.push_str("\n// Wrapper functions for default arguments\n");
            for (func_name, func) in &module.functions {
                if !func.defaults.is_empty() && func_name != "main" {
                    // Generate wrapper for each possible number of arguments
                    let num_params = func.params.len();
                    let num_defaults = func.defaults.len();
                    let min_args = num_params - num_defaults;
                    
                    // Generate wrappers for each arity from min_args to num_params-1
                    for provided_args in min_args..num_params {
                        output.push_str(&transpiler.generate_default_wrapper(func_name, func, provided_args)?);
                    }
                }
            }
        }

        // Add main function if not present (wrap global code in main)
        if !module.functions.contains_key("main") {
            // Reset variable tracking for main function scope
            transpiler.declared_vars.clear();
            transpiler.var_types.clear();
            transpiler.indent_level = 1;
            transpiler.temp_var_counter = 0;
            transpiler.scope_variables.clear();
            transpiler.function_parameters.clear();  // Clear function parameters for main scope

            // First pass: collect all variables from globals
            if !module.globals.is_empty() {
                transpiler.collect_variables(&module.globals);
                // Additional pass: explicitly collect all StoreLocal target names
                transpiler.collect_store_local_targets(&module.globals);
            }
            
            // Apply type information from module to override generic types
            // For main() variables, List and Dict types should be stored as Generic (TauValue)
            // since that's how they're declared in C by generate_variable_declarations
            for (var_name, var_type) in &module.type_info.variable_types {
                let native_type = transpiler.ast_type_to_native(var_type);
                // Convert List/Dict to Generic for local variable storage
                // (function parameters remain as List/Dict for native operations)
                let storage_type = match native_type {
                    NativeType::List | NativeType::Dict => NativeType::Generic,
                    other => other,
                };
                transpiler.scope_variables.insert(var_name.clone(), storage_type);
            }
            
            // Ensure common temporary variables are always declared
            transpiler.scope_variables.entry("temp".to_string()).or_insert(NativeType::Generic);
            transpiler.scope_variables.entry("temp_result".to_string()).or_insert(NativeType::Generic);
            transpiler.scope_variables.entry("temp_left".to_string()).or_insert(NativeType::Generic);
            transpiler.scope_variables.entry("temp_right".to_string()).or_insert(NativeType::Generic);

            // Initialize var_types from scope_variables
            transpiler.var_types = transpiler.scope_variables.clone();

            output.push_str("\nint main(int argc, char* argv[]) {\n");

            // Generate variable declarations
            output.push_str(&transpiler.generate_variable_declarations());
            
            // Second pass: put global code in main
            if !module.globals.is_empty() {
                for instruction in &module.globals {
                    output.push_str(&transpiler.transpile_instruction(instruction, 1)?);
                }
            }
            
            output.push_str("\n    return 0;\n");
            output.push_str("}\n");
        }

        // Generate forward declarations for all collected lambdas
        if !transpiler.lambdas_to_generate.is_empty() {
            output.push_str("\n// Lambda function forward declarations\n");
            for (lambda_name, _, _, _, _) in &transpiler.lambdas_to_generate {
                output.push_str(&format!("TauValue {}(int argc, TauValue* argv);\n", lambda_name));
            }
            output.push_str("\n");
        }

        // Generate lambda functions after main (now with forward declarations above)
        if !transpiler.lambdas_to_generate.is_empty() {
            output.push_str("// Lambda functions\n");
            for (lambda_name, params, body_instrs, _captured_vars, body_result_var) in &transpiler.lambdas_to_generate {
                output.push_str(&transpiler.generate_lambda_function(
                    lambda_name,
                    params,
                    body_instrs,
                    body_result_var
                )?);
            }
        }

        Ok(output)
    }

    fn generate_type_definitions(&self) -> String {
        let mut output = String::new();

        // Advanced Type definitions for Tauraro runtime
        output.push_str("// Advanced Type definitions for Tauraro runtime\n");
        
        // Forward declarations
        output.push_str("typedef struct TauValue TauValue;\n");
        output.push_str("typedef struct TauList TauList;\n");
        output.push_str("typedef struct TauDict TauDict;\n");
        output.push_str("typedef struct TauDictEntry TauDictEntry;\n");
        output.push_str("typedef struct TauObject TauObject;\n");
        output.push_str("typedef struct TauClass TauClass;\n");
        output.push_str("typedef struct TauFunction TauFunction;\n");
        output.push_str("typedef struct TauClosure TauClosure;\n");
        output.push_str("typedef struct TauIterator TauIterator;\n");
        output.push_str("typedef struct TauException TauException;\n");
        output.push_str("typedef struct TauModule TauModule;\n");
        output.push_str("typedef struct TauMethod TauMethod;\n");
        output.push_str("\n");

        // Enhanced value type with comprehensive support
        output.push_str("// Generic value type (for complex types)\n");
        output.push_str("struct TauValue {\n");
        output.push_str("    int type; // 0=int, 1=float, 2=string, 3=bool, 4=list, 5=dict, 6=object, 7=function, 8=exception\n");
        output.push_str("    union {\n");
        output.push_str("        long long i;\n");
        output.push_str("        double f;\n");
        output.push_str("        char* s;\n");
        output.push_str("        TauList* list;\n");
        output.push_str("        TauDict* dict;\n");
        output.push_str("        TauObject* obj;\n");
        output.push_str("        TauFunction* func;\n");
        output.push_str("        TauException* exc;\n");
        output.push_str("        void* ptr;\n");
        output.push_str("    } value;\n");
        output.push_str("    int refcount;  // Reference counting for GC\n");
        output.push_str("    struct TauValue* next; // For GC linked list\n");
        output.push_str("};\n\n");

        // Enhanced list type with dynamic growth
        output.push_str("// Dynamic list type\n");
        output.push_str("struct TauList {\n");
        output.push_str("    TauValue* items;\n");
        output.push_str("    size_t size;\n");
        output.push_str("    size_t capacity;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Enhanced dictionary with hash table
        output.push_str("// Hash table dictionary entry\n");
        output.push_str("struct TauDictEntry {\n");
        output.push_str("    char* key;\n");
        output.push_str("    TauValue value;\n");
        output.push_str("    struct TauDictEntry* next; // For collision chaining\n");
        output.push_str("};\n\n");

        output.push_str("// Hash table dictionary type\n");
        output.push_str("struct TauDict {\n");
        output.push_str("    TauDictEntry** buckets;\n");
        output.push_str("    size_t size;\n");
        output.push_str("    size_t capacity;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Class system with inheritance
        output.push_str("// Class definition with inheritance support\n");
        output.push_str("struct TauClass {\n");
        output.push_str("    char* name;\n");
        output.push_str("    struct TauClass* parent; // For inheritance\n");
        output.push_str("    TauDict* methods; // Method table\n");
        output.push_str("    TauDict* static_methods;\n");
        output.push_str("    TauDict* properties;\n");
        output.push_str("    size_t instance_size;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Enhanced object type with class reference
        output.push_str("// Object instance with class support\n");
        output.push_str("struct TauObject {\n");
        output.push_str("    TauClass* class_ref; // Reference to class definition\n");
        output.push_str("    char* class_name; // For compatibility\n");
        output.push_str("    TauDict* attributes; // Instance variables\n");
        output.push_str("    void* native_data; // For native extensions\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Function and closure support
        output.push_str("// Function type with closure support\n");
        output.push_str("typedef TauValue (*TauNativeFunc)(int argc, TauValue* argv);\n");
        output.push_str("\n");
        
        output.push_str("struct TauFunction {\n");
        output.push_str("    char* name;\n");
        output.push_str("    TauNativeFunc native_func; // For C functions\n");
        output.push_str("    struct TauClosure* closure; // For closures\n");
        output.push_str("    int param_count;\n");
        output.push_str("    char** param_names;\n");
        output.push_str("    int is_native;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        output.push_str("// Closure with captured variables\n");
        output.push_str("struct TauClosure {\n");
        output.push_str("    TauDict* captured_vars; // Captured from outer scope\n");
        output.push_str("    TauFunction* function;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Iterator support
        output.push_str("// Iterator interface\n");
        output.push_str("struct TauIterator {\n");
        output.push_str("    void* data; // Iterator-specific data\n");
        output.push_str("    TauValue (*next)(struct TauIterator*);\n");
        output.push_str("    int (*has_next)(struct TauIterator*);\n");
        output.push_str("    void (*cleanup)(struct TauIterator*);\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Exception handling
        output.push_str("// Exception type for error handling\n");
        output.push_str("struct TauException {\n");
        output.push_str("    char* type; // Exception type name\n");
        output.push_str("    char* message;\n");
        output.push_str("    char* traceback;\n");
        output.push_str("    TauValue value; // Optional associated value\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Module system
        output.push_str("// Module system support\n");
        output.push_str("struct TauModule {\n");
        output.push_str("    char* name;\n");
        output.push_str("    char* path;\n");
        output.push_str("    TauDict* globals; // Module global variables\n");
        output.push_str("    TauDict* exports; // Exported symbols\n");
        output.push_str("    int is_loaded;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Method binding
        output.push_str("// Bound method (method + instance)\n");
        output.push_str("struct TauMethod {\n");
        output.push_str("    TauObject* instance;\n");
        output.push_str("    TauFunction* function;\n");
        output.push_str("    int refcount;\n");
        output.push_str("};\n\n");

        // Runtime utility forward declarations
        output.push_str("// Runtime utility forward declarations\n");
        output.push_str("unsigned int tauraro_hash(const char* key);\n");
        output.push_str("TauDict* tauraro_create_dict();\n");
        output.push_str("void tauraro_dict_set(TauDict* dict, const char* key, TauValue value);\n");
        output.push_str("TauValue* tauraro_dict_get(TauDict* dict, const char* key);\n");
        output.push_str("TauValue tauraro_list_len(TauList* list);\n");
        output.push_str("TauList* tauraro_create_list(size_t initial_capacity);\n");
        output.push_str("void tauraro_list_append(TauList* list, TauValue item);\n");
        output.push_str("TauValue tauraro_list_get(TauList* list, long long index);\n");
        output.push_str("void tauraro_list_set(TauList* list, long long index, TauValue item);\n");
        output.push_str("TauValue tauraro_list_pop(TauList* list);\n");
        output.push_str("int tauraro_list_contains(TauList* list, TauValue item);\n");
        output.push_str("TauValue lst__append(TauValue lst, TauValue item);\n");
        output.push_str("TauValue text__upper(TauValue str);\n");
        output.push_str("TauValue text__lower(TauValue str);\n");
        output.push_str("TauValue text__strip(TauValue str);\n");
        output.push_str("TauValue text__split(TauValue str, TauValue delim);\n");
        output.push_str("TauValue text__join(TauValue delim, TauValue list);\n");
        output.push_str("TauValue text__replace(TauValue str, TauValue old_s, TauValue new_s);\n");
        output.push_str("TauValue text__startswith(TauValue str, TauValue prefix);\n");
        output.push_str("TauValue text__endswith(TauValue str, TauValue suffix);\n");
        output.push_str("TauValue text__find(TauValue str, TauValue substr);\n");
        output.push_str("TauValue text__title(TauValue str);\n");
        output.push_str("TauValue text__capitalize(TauValue str);\n");
        output.push_str("TauValue text__swapcase(TauValue str);\n");
        output.push_str("TauValue text__lstrip(TauValue str);\n");
        output.push_str("TauValue text__rstrip(TauValue str);\n");
        output.push_str("TauValue text__isdigit(TauValue str);\n");
        output.push_str("TauValue text__isalpha(TauValue str);\n");
        output.push_str("TauValue text__isalnum(TauValue str);\n");
        output.push_str("TauValue text__isspace(TauValue str);\n");
        output.push_str("TauValue text__isupper(TauValue str);\n");
        output.push_str("TauValue text__islower(TauValue str);\n");
        output.push_str("TauValue text__count(TauValue str, TauValue sub);\n");
        output.push_str("TauValue text__center(TauValue str, TauValue width);\n");
        output.push_str("TauValue text__ljust(TauValue str, TauValue width);\n");
        output.push_str("TauValue text__rjust(TauValue str, TauValue width);\n");
        output.push_str("TauValue text__zfill(TauValue str, TauValue width);\n");
        output.push_str("TauValue lst__pop(TauValue lst);\n");
        output.push_str("TauValue lst__insert(TauValue lst, TauValue index, TauValue item);\n");
        output.push_str("TauValue lst__remove(TauValue lst, TauValue item);\n");
        output.push_str("TauValue lst__extend(TauValue lst, TauValue other);\n");
        output.push_str("TauValue lst__index(TauValue lst, TauValue item);\n");
        output.push_str("TauValue lst__count(TauValue lst, TauValue item);\n");
        output.push_str("TauValue lst__reverse(TauValue lst);\n");
        output.push_str("TauValue lst__sort(TauValue lst);\n");
        output.push_str("TauValue lst__copy(TauValue lst);\n");
        output.push_str("TauValue lst__clear(TauValue lst);\n");
        output.push_str("TauValue range(TauValue end);\n");
        output.push_str("TauValue range2(TauValue start, TauValue end);\n");
        output.push_str("TauValue range3(TauValue start, TauValue end, TauValue step);\n");
        output.push_str("TauValue tauraro_abs(TauValue val);\n");
        output.push_str("TauValue tauraro_min(TauValue a, TauValue b);\n");
        output.push_str("TauValue tauraro_max(TauValue a, TauValue b);\n");
        output.push_str("TauValue tauraro_sum(TauValue list);\n");
        output.push_str("TauValue tauraro_super_call(TauObject* self, TauValue* args, int argc);\n");
        // Additional builtins
        output.push_str("TauValue tauraro_sorted(TauValue list);\n");
        output.push_str("TauValue tauraro_reversed(TauValue list);\n");
        output.push_str("TauValue tauraro_enumerate_list(TauValue list, TauValue start);\n");
        output.push_str("TauValue tauraro_zip_lists(TauValue list1, TauValue list2);\n");
        output.push_str("TauValue tauraro_any(TauValue list);\n");
        output.push_str("TauValue tauraro_all(TauValue list);\n");
        output.push_str("TauValue tauraro_type_name(TauValue val);\n");
        output.push_str("TauValue tauraro_isinstance(TauValue obj, TauValue type_str);\n");
        output.push_str("TauValue tauraro_ord(TauValue ch);\n");
        output.push_str("TauValue tauraro_chr(TauValue num);\n");
        output.push_str("TauValue tauraro_round(TauValue num, TauValue places);\n");
        output.push_str("TauValue tauraro_pow(TauValue base, TauValue exp);\n");
        output.push_str("TauValue tauraro_sqrt(TauValue num);\n");
        output.push_str("TauValue tauraro_hex(TauValue num);\n");
        output.push_str("TauValue tauraro_bin(TauValue num);\n");
        output.push_str("TauValue tauraro_oct(TauValue num);\n");
        output.push_str("TauValue tauraro_divmod(TauValue a, TauValue b);\n");
        output.push_str("TauValue tauraro_to_list(TauValue val);\n");
        output.push_str("TauValue tauraro_to_set(TauValue val);\n");
        output.push_str("TauValue tauraro_repr(TauValue val);\n");
        // String method declarations
        output.push_str("TauValue tauraro_str_upper(TauValue str);\n");
        output.push_str("TauValue tauraro_str_lower(TauValue str);\n");
        output.push_str("TauValue tauraro_str_strip(TauValue str);\n");
        output.push_str("TauValue tauraro_str_lstrip(TauValue str);\n");
        output.push_str("TauValue tauraro_str_rstrip(TauValue str);\n");
        output.push_str("TauValue tauraro_str_title(TauValue str);\n");
        output.push_str("TauValue tauraro_str_capitalize(TauValue str);\n");
        output.push_str("TauValue tauraro_str_swapcase(TauValue str);\n");
        output.push_str("TauValue tauraro_str_isdigit(TauValue str);\n");
        output.push_str("TauValue tauraro_str_isalpha(TauValue str);\n");
        output.push_str("TauValue tauraro_str_isalnum(TauValue str);\n");
        output.push_str("TauValue tauraro_str_isspace(TauValue str);\n");
        output.push_str("TauValue tauraro_str_isupper(TauValue str);\n");
        output.push_str("TauValue tauraro_str_islower(TauValue str);\n");
        output.push_str("TauValue tauraro_str_count(TauValue str, TauValue sub);\n");
        output.push_str("TauValue tauraro_str_center(TauValue str, TauValue width);\n");
        output.push_str("TauValue tauraro_str_ljust(TauValue str, TauValue width);\n");
        output.push_str("TauValue tauraro_str_rjust(TauValue str, TauValue width);\n");
        output.push_str("TauValue tauraro_str_zfill(TauValue str, TauValue width);\n");
        // List method declarations
        output.push_str("TauValue tauraro_list_pop_v(TauValue list);\n");
        output.push_str("TauValue tauraro_list_insert(TauValue list, TauValue index, TauValue item);\n");
        output.push_str("TauValue tauraro_list_remove(TauValue list, TauValue item);\n");
        output.push_str("TauValue tauraro_list_extend_v(TauValue list, TauValue other);\n");
        output.push_str("int tauraro_equals(TauValue a, TauValue b);\n");
        output.push_str("\n");

        output
    }

    fn generate_function_signature(&self, func_name: &str, func: &IRFunction, declare_only: bool) -> Result<String> {
        let mut output = String::new();

        // Determine return type based on type annotation
        let return_type = if func_name == "main" {
            "int".to_string()
        } else if let Some(ret_type) = &func.return_type {
            // Use native type if we have a return type annotation
            let native_type = self.ast_type_to_native(ret_type);
            self.get_c_type(native_type).to_string()
        } else {
            // Default to TauValue for untyped functions
            "TauValue".to_string()
        };

        // Mangle function name if it's a C keyword, but NOT "main" which is the C entry point
        let safe_func_name = if func_name == "main" {
            "main".to_string()
        } else {
            self.mangle_c_keyword(func_name)
        };

        output.push_str(&return_type);
        output.push_str(" ");
        output.push_str(&safe_func_name);
        output.push_str("(");

        if func_name == "main" {
            output.push_str("int argc, char* argv[]");
        } else {
            // Generate parameter list with proper types
            let param_strs: Vec<String> = func.params.iter()
                .map(|p| {
                    let param_type = if let Some(ast_type) = func.param_types.get(p) {
                        // Parameter has a type annotation - use native type
                        let native_type = self.ast_type_to_native(ast_type);
                        self.get_c_type(native_type).to_string()
                    } else {
                        // No type annotation - use TauValue
                        "TauValue".to_string()
                    };
                    format!("{} {}", param_type, self.mangle_c_keyword(p))
                })
                .collect();
            output.push_str(&param_strs.join(", "));
        }

        output.push_str(")");

        if !declare_only {
            output.push_str(" {\n");
        }

        Ok(output)
    }

    fn generate_default_wrapper(&self, func_name: &str, func: &IRFunction, num_args: usize) -> Result<String> {
        let mut output = String::new();
        
        // Mangle function name if it's a C keyword
        let safe_func_name = self.mangle_c_keyword(func_name);
        
        // Generate wrapper function name (e.g., greet_arity_0 for 0 arguments)
        let wrapper_name = format!("{}_arity_{}", safe_func_name, num_args);
        
        // Always use TauValue for main transpiler
        let return_type = "TauValue".to_string();
        
        output.push_str(&format!("{} {}(", return_type, wrapper_name));
        
        // Always use TauValue parameters in main transpiler
        let provided_params: Vec<String> = func.params.iter()
            .take(num_args)
            .map(|p| format!("TauValue {}", self.mangle_c_keyword(p)))
            .collect();
        output.push_str(&provided_params.join(", "));
        output.push_str(") {\n");
        
        // Generate call to actual function with defaults filled in
        output.push_str(&format!("    return {}(", safe_func_name));
        
        let mut call_args = Vec::new();
        for (i, param) in func.params.iter().enumerate() {
            if i < num_args {
                // Use provided argument
                call_args.push(param.clone());
            } else {
                // Use default value - always use TauValue wrapper
                if let Some(default_val) = func.defaults.get(param) {
                    let default_c = self.wrap_value_in_tauvalue(default_val);
                    call_args.push(default_c);
                } else {
                    // Shouldn't happen, but fall back to None
                    call_args.push("(TauValue){.type = 0, .value.i = 0}".to_string());
                }
            }
        }
        output.push_str(&call_args.join(", "));
        output.push_str(");\n");
        output.push_str("}\n\n");
        
        Ok(output)
    }
    
    /// Convert a Value to a typed C literal (for static typing)
    fn value_to_typed_c_literal(&self, value: &Value, ast_type: &Type) -> String {
        let native_type = self.ast_type_to_native(ast_type);
        match (value, native_type) {
            (Value::Int(i), NativeType::Int64) => format!("{}", i),
            (Value::Int(i), NativeType::Double) => format!("{}.0", i),
            (Value::Float(f), NativeType::Double) => format!("{}", f),
            (Value::Float(f), NativeType::Int64) => format!("{}", *f as i64),
            (Value::Bool(b), NativeType::Bool) => if *b { "1".to_string() } else { "0".to_string() },
            (Value::Str(s), NativeType::CStr) => format!("\"{}\"", s.replace("\"", "\\\"")),
            _ => self.value_to_c_literal(value), // Fall back to TauValue
        }
    }
    
    fn value_to_c_literal(&self, value: &Value) -> String {
        match value {
            Value::Int(i) => format!("(TauValue){{.type = 0, .value.i = {}}}", i),
            Value::Float(f) => format!("(TauValue){{.type = 1, .value.f = {}}}", f),
            Value::Str(s) => format!("(TauValue){{.type = 2, .value.s = strdup(\"{}\"), .refcount = 1}}", s.replace("\"", "\\\"")),
            Value::Bool(b) => format!("(TauValue){{.type = 3, .value.i = {}}}", if *b { 1 } else { 0 }),
            Value::None => "(TauValue){.type = 0, .value.i = 0}".to_string(),
            _ => "(TauValue){.type = 0, .value.i = 0}".to_string(),
        }
    }

    fn transpile_function(&mut self, func: &IRFunction) -> Result<String> {
        let mut output = String::new();

        // Reset variable tracking for this function scope
        self.declared_vars.clear();
        self.var_types.clear();
        self.var_name_mapping.clear();
        self.indent_level = 1;
        self.temp_var_counter = 0;
        self.scope_variables.clear();
        self.function_parameters.clear();
        
        // Set current function name for return type handling
        self.current_function = Some(func.name.clone());

        // Add function parameters to scope with their actual types (static typing support)
        for param in &func.params {
            // Check if we have type information for this parameter
            let param_type = if let Some(ast_type) = func.param_types.get(param) {
                self.ast_type_to_native(ast_type)
            } else {
                NativeType::Generic
            };
            self.scope_variables.insert(param.clone(), param_type);
            self.function_parameters.insert(param.clone());
            self.var_types.insert(param.clone(), param_type);
        }

        // Collect all variables needed in this function (first pass)
        let mut all_instructions = Vec::new();
        for block in &func.blocks {
            all_instructions.extend(block.instructions.clone());
        }
        self.collect_variables(&all_instructions);

        // Initialize var_types from scope_variables for runtime type checking
        // (but don't overwrite parameter types we already set)
        for (var, var_type) in &self.scope_variables {
            self.var_types.entry(var.clone()).or_insert(*var_type);
        }

        // Function signature
        output.push_str(&self.generate_function_signature(&func.name, func, false)?);

        // Generate variable declarations (don't declare parameters again)
        let var_decls = self.generate_variable_declarations();
        output.push_str(&var_decls);

        // Generate code (second pass)
        let mut last_was_return = false;
        for instruction in &all_instructions {
            let inst_str = self.transpile_instruction(instruction, self.indent_level)?;
            output.push_str(&inst_str);
            // Track if the last instruction was a return
            last_was_return = matches!(instruction, IRInstruction::Return { .. });
        }

        // Add default return for non-main functions ONLY if we didn't have an explicit return
        if func.name != "main" && !last_was_return {
            // Get the function's return type
            if let Some(ret_type) = &func.return_type {
                let native_ret_type = self.ast_type_to_native(ret_type);
                match native_ret_type {
                    NativeType::Int64 => {
                        output.push_str(&format!("{}return 0;\n", self.indent()));
                    }
                    NativeType::Double => {
                        output.push_str(&format!("{}return 0.0;\n", self.indent()));
                    }
                    NativeType::CStr => {
                        output.push_str(&format!("{}return \"\";\n", self.indent()));
                    }
                    _ => {
                        output.push_str(&format!("{}return (TauValue){{.type = 0, .value.i = 0}};\n", self.indent()));
                    }
                }
            } else {
                // Untyped function - use TauValue
                output.push_str(&format!("{}TauValue ret = {{.type = 0, .value.i = 0}};\n", self.indent()));
                output.push_str(&format!("{}return ret;\n", self.indent()));
            }
        } else if func.name == "main" && !last_was_return {
            output.push_str("    return 0;\n");
        }

        output.push_str("}\n\n");
        Ok(output)
    }

    fn transpile_instruction(&mut self, instr: &IRInstruction, indent_level: usize) -> Result<String> {
        self.indent_level = indent_level;
        let mut output = String::new();
        let ind = self.indent();

        match instr {
            IRInstruction::Comment(msg) => {
                output.push_str(&format!("{}// {}\n", ind, msg));
            }

            IRInstruction::LoadConst { value, result } => {
                let resolved_result = self.resolve_var_name(result);
                // Check both var_types and scope_variables for the result type
                let result_type = self.var_types.get(&resolved_result).copied()
                    .or_else(|| self.scope_variables.get(&resolved_result).copied())
                    .or_else(|| self.var_types.get(result).copied())
                    .or_else(|| self.scope_variables.get(result).copied())
                    .unwrap_or(NativeType::Generic);
                
                // Generate appropriate assignment based on result type
                match result_type {
                    NativeType::Int64 => {
                        // Extract int value from constant
                        let int_val = match value {
                            Value::Int(i) => *i,
                            _ => 0,
                        };
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, int_val));
                    }
                    NativeType::Double => {
                        let float_val = match value {
                            Value::Float(f) => *f,
                            Value::Int(i) => *i as f64,
                            _ => 0.0,
                        };
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, float_val));
                    }
                    _ => {
                        // Generic TauValue assignment
                        let wrapped_val = self.wrap_value_in_tauvalue(value);
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, wrapped_val));
                    }
                }
                self.var_types.insert(resolved_result.clone(), result_type);
                self.var_types.insert(result.clone(), result_type);
            }

            IRInstruction::LoadLocal { name, result } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_result = self.resolve_var_name(result);
                
                // Check the DECLARATION type first - this is how the variable is declared in C
                // This determines whether it's a raw native type or a TauValue
                let source_decl_type = self.scope_variables.get(&resolved_name).copied()
                    .or_else(|| self.scope_variables.get(name).copied());
                let source_value_type = self.var_types.get(&resolved_name).copied()
                    .or_else(|| self.var_types.get(name).copied())
                    .unwrap_or(NativeType::Generic);
                
                // If declaration is Generic (TauValue), source is TauValue regardless of value type
                let source_type = match source_decl_type {
                    Some(NativeType::Generic) => NativeType::Generic,
                    Some(other) => other,
                    None => source_value_type,
                };
                
                let target_type = self.var_types.get(&resolved_result).copied()
                    .unwrap_or(NativeType::Generic);
                
                // Handle type conversions when loading
                match (target_type, source_type) {
                    (NativeType::Int64, NativeType::Int64) => {
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Int64, NativeType::Generic) => {
                        output.push_str(&format!("{}{} = {}.value.i;\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Double, NativeType::Double) => {
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Double, NativeType::Generic) => {
                        output.push_str(&format!("{}{} = {}.value.f;\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::Int64) => {
                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::Double) => {
                        output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::List) => {
                        // Wrap TauList* in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::Dict) => {
                        // Wrap TauDict* in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::List, NativeType::List) |
                    (NativeType::Dict, NativeType::Dict) => {
                        // Same native type - direct assignment
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::List, NativeType::Generic) => {
                        // Extract TauList* from TauValue
                        output.push_str(&format!("{}{} = {}.value.list;\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Dict, NativeType::Generic) => {
                        // Extract TauDict* from TauValue
                        output.push_str(&format!("{}{} = {}.value.dict;\n", ind, resolved_result, resolved_name));
                    }
                    _ => {
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                }
                self.var_types.insert(resolved_result, target_type);
            }

            IRInstruction::StoreLocal { name, value } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_value = self.resolve_var_name(value);
                
                // AGGRESSIVE: Always check types and convert if needed
                // This ensures correctness even if type detection is imperfect
                
                // Determine target type from declarations - check scope_variables first for declared types
                let target_type = self.scope_variables.get(&resolved_name).copied()
                    .or_else(|| self.scope_variables.get(name).copied())
                    .or_else(|| self.var_types.get(&resolved_name).copied())
                    .or_else(|| self.var_types.get(name).copied())
                    .unwrap_or(NativeType::Generic);
                
                let source_type = self.scope_variables.get(&resolved_value).copied()
                    .or_else(|| self.scope_variables.get(value).copied())
                    .or_else(|| self.var_types.get(&resolved_value).copied())
                    .or_else(|| self.var_types.get(value).copied())
                    .unwrap_or(NativeType::Generic);
                
                // Generate assignment with automatic wrapping/unwrapping
                match (target_type, source_type) {
                    // Same types - direct assignment
                    (NativeType::Int64, NativeType::Int64) |
                    (NativeType::Double, NativeType::Double) |
                    (NativeType::CStr, NativeType::CStr) |
                    (NativeType::Bool, NativeType::Bool) |
                    (NativeType::Generic, NativeType::Generic) => {
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                    }
                    // Native to Generic - wrap
                    (NativeType::Generic, NativeType::Int64) => {
                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}, .refcount = 1}};\n", ind, resolved_name, resolved_value));
                    }
                    (NativeType::Generic, NativeType::Double) => {
                        output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = {}, .refcount = 1}};\n", ind, resolved_name, resolved_value));
                    }
                    (NativeType::Generic, NativeType::CStr) => {
                        output.push_str(&format!("{}{} = (TauValue){{.type = 2, .value.s = {}, .refcount = 1}};\n", ind, resolved_name, resolved_value));
                    }
                    (NativeType::Generic, NativeType::Bool) => {
                        output.push_str(&format!("{}{} = (TauValue){{.type = 3, .value.i = ({} ? 1 : 0), .refcount = 1}};\n", ind, resolved_name, resolved_value));
                    }
                    // Generic to Native - unwrap  
                    (NativeType::Int64, _) => {
                        output.push_str(&format!("{}{} = {}.value.i;\n", ind, resolved_name, resolved_value));
                    }
                    (NativeType::Double, _) => {
                        output.push_str(&format!("{}{} = {}.value.f;\n", ind, resolved_name, resolved_value));
                    }
                    (NativeType::CStr, _) => {
                        output.push_str(&format!("{}{} = {}.value.s;\n", ind, resolved_name, resolved_value));
                    }
                    (NativeType::Bool, _) => {
                        output.push_str(&format!("{}{} = {}.value.i ? 1 : 0;\n", ind, resolved_name, resolved_value));
                    }
                    // Fallback
                    _ => {
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                    }
                }
                
                self.var_types.insert(resolved_name.clone(), target_type);
                self.scope_variables.insert(resolved_name, target_type);
            }

            IRInstruction::LoadGlobal { name, result } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_name = self.resolve_var_name(name);
                
                // Check the source variable's DECLARED type (scope_variables first)
                // This is what the variable is actually declared as, not what was last assigned
                let source_type = self.scope_variables.get(&resolved_name)
                    .or_else(|| self.scope_variables.get(name))
                    .or_else(|| self.var_types.get(&resolved_name))
                    .or_else(|| self.var_types.get(name))
                    .copied()
                    .unwrap_or(NativeType::Generic);
                
                // Check the target variable's DECLARED type
                let target_type = self.scope_variables.get(&resolved_result)
                    .or_else(|| self.var_types.get(&resolved_result))
                    .copied()
                    .unwrap_or(NativeType::Generic);
                
                // Handle type conversions based on both source and target types
                match (target_type, source_type) {
                    (NativeType::Int64, NativeType::Int64) |
                    (NativeType::Double, NativeType::Double) |
                    (NativeType::List, NativeType::List) |
                    (NativeType::Dict, NativeType::Dict) => {
                        // Same type - direct assignment
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::Int64) => {
                        // Wrap native int in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::Double) => {
                        // Wrap native double in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::List) => {
                        // Wrap TauList* in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Generic, NativeType::Dict) => {
                        // Wrap TauDict* in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = {}, .refcount = 1}};\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Int64, NativeType::Generic) => {
                        // Extract int from TauValue
                        output.push_str(&format!("{}{} = {}.value.i;\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Double, NativeType::Generic) => {
                        // Extract double from TauValue
                        output.push_str(&format!("{}{} = {}.value.f;\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::List, NativeType::Generic) => {
                        // Extract TauList* from TauValue
                        output.push_str(&format!("{}{} = {}.value.list;\n", ind, resolved_result, resolved_name));
                    }
                    (NativeType::Dict, NativeType::Generic) => {
                        // Extract TauDict* from TauValue
                        output.push_str(&format!("{}{} = {}.value.dict;\n", ind, resolved_result, resolved_name));
                    }
                    _ => {
                        // Generic to Generic or other cases - direct assignment
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                }
                self.var_types.insert(resolved_result.clone(), target_type);
            }

                IRInstruction::LoadTypedGlobal { name, result, type_info } => {
                    let resolved_result = self.resolve_var_name(result);
                    let resolved_name = self.resolve_var_name(name);
                    let native_type = self.ast_type_to_native(type_info);
                    
                    // Check if source is a temp variable (TauValue) - need to extract the value
                    let is_source_tauvalue = name == "temp" || name == "temp_result" || name.starts_with("temp_") ||
                                             resolved_name == "temp" || resolved_name == "temp_result" || resolved_name.starts_with("temp_");
                    
                    if is_source_tauvalue {
                        // Source is TauValue, extract based on target type
                        match native_type {
                            NativeType::Int64 => {
                                output.push_str(&format!("{}{} = {}.value.i;\n", ind, resolved_result, resolved_name));
                            }
                            NativeType::Double => {
                                output.push_str(&format!("{}{} = {}.value.f;\n", ind, resolved_result, resolved_name));
                            }
                            NativeType::CStr => {
                                output.push_str(&format!("{}{} = {}.value.s;\n", ind, resolved_result, resolved_name));
                            }
                            NativeType::Bool => {
                                output.push_str(&format!("{}{} = {}.value.i ? 1 : 0;\n", ind, resolved_result, resolved_name));
                            }
                            _ => {
                                // For Generic/other types, direct assignment
                                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                            }
                        }
                    } else {
                        // For typed globals with native types, use direct assignment
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                    }
                    self.var_types.insert(resolved_result, native_type);
                }

            IRInstruction::StoreGlobal { name, value } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_value = self.resolve_var_name(value);
                
                // Check if target variable has a known type
                let target_type = self.var_types.get(&resolved_name)
                    .or_else(|| self.var_types.get(name))
                    .copied()
                    .unwrap_or(NativeType::Generic);
                
                // If it's a native type, store directly
                output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                self.var_types.insert(resolved_name, target_type);
            }

            IRInstruction::StoreTypedGlobal { name, value, type_info } => {
                // Store typed global - handle type information for typed variables
                let resolved_name = self.resolve_var_name(name);
                let resolved_value = self.resolve_var_name(value);
                let native_type = self.ast_type_to_native(type_info);
                
                // Get source type - but ALWAYS treat temp/temp_result as Generic since they're TauValue
                let source_type = if value == "temp" || value == "temp_result" || value.starts_with("temp_") ||
                                     resolved_value == "temp" || resolved_value == "temp_result" || resolved_value.starts_with("temp_") {
                    NativeType::Generic
                } else {
                    self.scope_variables.get(&resolved_value).copied()
                        .or_else(|| self.scope_variables.get(value).copied())
                        .or_else(|| self.var_types.get(&resolved_value).copied())
                        .or_else(|| self.var_types.get(value).copied())
                        .unwrap_or(NativeType::Generic)
                };
                
                // Handle assignment based on target type 
                match native_type {
                    NativeType::Int64 => {
                        if matches!(source_type, NativeType::Int64) {
                            output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                        } else {
                            output.push_str(&format!("{}{} = {}.value.i;\n", ind, resolved_name, resolved_value));
                        }
                    }
                    NativeType::Double => {
                        if matches!(source_type, NativeType::Double) {
                            output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                        } else {
                            output.push_str(&format!("{}{} = {}.value.f;\n", ind, resolved_name, resolved_value));
                        }
                    }
                    _ => {
                        // For other types, direct assignment
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                    }
                }
                
                self.var_types.insert(resolved_name, native_type);
                self.var_types.insert(name.clone(), native_type);
            }
            
            // Typed local variable operations - optimize for native types
                IRInstruction::LoadTypedLocal { name, result, type_info } => {
                    let resolved_name = self.resolve_var_name(name);
                    let resolved_result = self.resolve_var_name(result);
                    let native_type = self.ast_type_to_native(type_info);
                    
                    // For native types with optimization enabled, use direct native C types
                    if self.enable_native_types {
                        match native_type {
                            NativeType::Int64 => {
                                output.push_str(&format!("{}{} = (long long){};\n", ind, resolved_result, resolved_name));
                                self.var_types.insert(resolved_result.clone(), NativeType::Int64);
                                self.scope_variables.insert(resolved_result, NativeType::Int64);
                            }
                            NativeType::Double => {
                                output.push_str(&format!("{}{} = (double){};\n", ind, resolved_result, resolved_name));
                                self.var_types.insert(resolved_result.clone(), NativeType::Double);
                                self.scope_variables.insert(resolved_result, NativeType::Double);
                            }
                            NativeType::CStr => {
                                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                                self.var_types.insert(resolved_result.clone(), NativeType::CStr);
                                self.scope_variables.insert(resolved_result, NativeType::CStr);
                            }
                            NativeType::Bool => {
                                output.push_str(&format!("{}{} = (int){};\n", ind, resolved_result, resolved_name));
                                self.var_types.insert(resolved_result.clone(), NativeType::Bool);
                                self.scope_variables.insert(resolved_result, NativeType::Bool);
                            }
                            _ => {
                                // For complex types, use TauValue
                                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                                self.var_types.insert(resolved_result.clone(), native_type);
                                self.scope_variables.insert(resolved_result, native_type);
                            }
                        }
                    } else {
                        // Without optimization, always use TauValue
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                        self.var_types.insert(resolved_result.clone(), NativeType::Generic);
                        self.scope_variables.insert(resolved_result, NativeType::Generic);
                    }
                }            IRInstruction::StoreTypedLocal { name, value, type_info } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_value = self.resolve_var_name(value);
                let native_type = self.ast_type_to_native(type_info);
                
                // Track type for this variable
                self.static_typed_vars.insert(name.clone(), type_info.clone());
                self.var_types.insert(resolved_name.clone(), native_type);
                self.scope_variables.insert(resolved_name.clone(), native_type);
                
                // Handle assignment based on target type
                match native_type {
                    NativeType::Int64 => {
                        // Extract int value from TauValue or use directly if already int
                        let is_temp = resolved_value.starts_with("var_") && resolved_value.ends_with("_temp");
                        let source_type = if is_temp {
                            NativeType::Generic
                        } else {
                            self.var_types.get(&resolved_value).copied()
                                .or_else(|| self.scope_variables.get(&resolved_value).copied())
                                .unwrap_or(NativeType::Generic)
                        };
                        
                        if matches!(source_type, NativeType::Int64) {
                            output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                        } else {
                            output.push_str(&format!("{}{} = {}.value.i;\n", ind, resolved_name, resolved_value));
                        }
                    }
                    NativeType::Double => {
                        // Extract float value from TauValue or use directly if already double
                        let is_temp = resolved_value.starts_with("var_") && resolved_value.ends_with("_temp");
                        let source_type = if is_temp {
                            NativeType::Generic
                        } else {
                            self.var_types.get(&resolved_value).copied()
                                .or_else(|| self.scope_variables.get(&resolved_value).copied())
                                .unwrap_or(NativeType::Generic)
                        };
                        
                        if matches!(source_type, NativeType::Double) {
                            output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                        } else {
                            output.push_str(&format!("{}{} = {}.value.f;\n", ind, resolved_name, resolved_value));
                        }
                    }
                    _ => {
                        // For other types, direct assignment
                        output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                    }
                }
            }
            
            // Typed binary operation - optimize for native types when available
            IRInstruction::TypedBinaryOp { op, left, right, result, type_info } => {
                let resolved_left = self.resolve_var_name(left);
                let resolved_right = self.resolve_var_name(right);
                let resolved_result = self.resolve_var_name(result);
                let result_type = self.ast_type_to_native(type_info);
                
                let left_type = self.var_types.get(&resolved_left).copied()
                    .or_else(|| self.scope_variables.get(&resolved_left).copied())
                    .unwrap_or(NativeType::Generic);
                let right_type = self.var_types.get(&resolved_right).copied()
                    .or_else(|| self.scope_variables.get(&resolved_right).copied())
                    .unwrap_or(NativeType::Generic);
                
                let op_str = self.format_binary_op(op.clone());
                
                // Handle based on result type and operand types
                match result_type {
                    NativeType::Int64 => {
                        // For int result, extract operands if they're TauValue
                        let left_extract = if matches!(left_type, NativeType::Int64) {
                            resolved_left.clone()
                        } else {
                            format!("{}.value.i", resolved_left)
                        };
                        let right_extract = if matches!(right_type, NativeType::Int64) {
                            resolved_right.clone()
                        } else {
                            format!("{}.value.i", resolved_right)
                        };
                        output.push_str(&format!("{}{} = {} {} {};\n", ind, resolved_result, left_extract, op_str, right_extract));
                    }
                    NativeType::Double => {
                        // For double result, extract operands if they're TauValue
                        let left_extract = if matches!(left_type, NativeType::Double) {
                            resolved_left.clone()
                        } else {
                            format!("(double){}.value.f", resolved_left)
                        };
                        let right_extract = if matches!(right_type, NativeType::Double) {
                            resolved_right.clone()
                        } else {
                            format!("(double){}.value.f", resolved_right)
                        };
                        output.push_str(&format!("{}{} = {} {} {};\n", ind, resolved_result, left_extract, op_str, right_extract));
                    }
                    _ => {
                        // For TauValue result, wrap in TauValue
                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = (long long){}.value.i {} (long long){}.value.i, .refcount = 1}};\n", 
                            ind, resolved_result, resolved_left, op_str, resolved_right));
                    }
                }
                self.var_types.insert(resolved_result.clone(), result_type);
                self.scope_variables.insert(resolved_result, result_type);
            }

            IRInstruction::BinaryOp { op, left, right, result } => {
                let resolved_left = self.resolve_var_name(left);
                let resolved_right = self.resolve_var_name(right);
                let resolved_result = self.resolve_var_name(result);
                
                let left_type = self.var_types.get(&resolved_left).copied()
                    .or_else(|| self.var_types.get(left).copied())
                    .unwrap_or(NativeType::Generic);
                let right_type = self.var_types.get(&resolved_right).copied()
                    .or_else(|| self.var_types.get(right).copied())
                    .unwrap_or(NativeType::Generic);

                // Special handling for power operation
                if matches!(op, BinaryOp::Pow) {
                    // Power operations return double, wrapped in TauValue
                    // Use runtime type checking for proper value extraction
                    output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = pow(\n", ind, resolved_result));
                    output.push_str(&format!("{}    ({}.type == 1 ? {}.value.f : (double){}.value.i),\n", 
                        ind, resolved_left, resolved_left, resolved_left));
                    output.push_str(&format!("{}    ({}.type == 1 ? {}.value.f : (double){}.value.i))}};\n",
                        ind, resolved_right, resolved_right, resolved_right));
                    self.var_types.insert(resolved_result, NativeType::Generic);
                } else if matches!(op, BinaryOp::Add) && 
                         (matches!(left_type, NativeType::CStr) || matches!(right_type, NativeType::CStr)) {
                    // String concatenation (only for known string types)
                    self.temp_var_counter += 1;
                    let temp_var = format!("temp_concat_{}", self.temp_var_counter);
                    
                    // Allocate temporary buffer for concatenation
                    output.push_str(&format!("{}char {}[512] = {{0}};\n", ind, temp_var));
                    
                    let left_val = match left_type {
                        NativeType::CStr => resolved_left.clone(),
                        _ => format!("({}.value.s ? {}.value.s : \"\")", resolved_left, resolved_left)
                    };
                    let right_val = match right_type {
                        NativeType::CStr => resolved_right.clone(),
                        _ => format!("({}.value.s ? {}.value.s : \"\")", resolved_right, resolved_right)
                    };
                    
                    output.push_str(&format!("{}strcpy({}, {});\n", ind, temp_var, left_val));
                    output.push_str(&format!("{}strcat({}, {});\n", ind, temp_var, right_val));
                    output.push_str(&format!("{}{} = (TauValue){{.type = 2, .value.s = strdup({}), .refcount = 1}};\n", ind, resolved_result, temp_var));
                    self.var_types.insert(resolved_result, NativeType::Generic);
                } else {
                    // Regular binary operations
                    let op_str = self.format_binary_op(op.clone());
                    let inferred_result_type = self.infer_binary_op_type(left_type, right_type, op.clone());
                    
                    // Check the DECLARATION type of the result variable (scope_variables has declaration types)
                    // This determines whether we can use native types or must wrap in TauValue
                    let declared_result_type = self.scope_variables.get(&resolved_result).copied()
                        .unwrap_or(NativeType::Generic);
                    
                    // Determine the actual result type to use for code generation
                    // If declared as native type, use it; otherwise use inferred type (but will wrap in TauValue)
                    let result_type = if matches!(declared_result_type, NativeType::Int64 | NativeType::Double | NativeType::Bool) {
                        declared_result_type
                    } else {
                        inferred_result_type
                    };
                    
                    // Track if we need to wrap the result in TauValue
                    let needs_wrapping = matches!(declared_result_type, NativeType::Generic);

                    // Special case: Generic + Generic might be string or integer concatenation
                    if matches!(op, BinaryOp::Add) && matches!(left_type, NativeType::Generic) && matches!(right_type, NativeType::Generic) {
                        // Generate runtime type checking for addition
                        self.temp_var_counter += 1;
                        let temp_concat = format!("temp_concat_rt_{}", self.temp_var_counter);
                        output.push_str(&format!("{}// Runtime type checking for + operation\n", ind));
                        output.push_str(&format!("{}if (({}.type == 2 || {}.type == 2)) {{\n", ind, resolved_left, resolved_right));
                        output.push_str(&format!("{}    char {}[512] = {{0}};\n", ind, temp_concat));
                        // Handle left operand: if it's a string use it directly, if it's int convert it, otherwise empty
                        output.push_str(&format!("{}    if ({}.type == 2) {{\n", ind, resolved_left));
                        output.push_str(&format!("{}        strcpy({}, {}.value.s);\n", ind, temp_concat, resolved_left));
                        output.push_str(&format!("{}    }} else if ({}.type == 0) {{\n", ind, resolved_left));
                        output.push_str(&format!("{}        char int_buf[64];\n", ind));
                        output.push_str(&format!("{}        snprintf(int_buf, sizeof(int_buf), \"%lld\", {}.value.i);\n", ind, resolved_left));
                        output.push_str(&format!("{}        strcpy({}, int_buf);\n", ind, temp_concat));
                        output.push_str(&format!("{}    }}\n", ind));
                        // Handle right operand: concatenate to existing buffer
                        output.push_str(&format!("{}    if ({}.type == 2) {{\n", ind, resolved_right));
                        output.push_str(&format!("{}        strcat({}, {}.value.s);\n", ind, temp_concat, resolved_right));
                        output.push_str(&format!("{}    }} else if ({}.type == 0) {{\n", ind, resolved_right));
                        output.push_str(&format!("{}        char int_buf[64];\n", ind));
                        output.push_str(&format!("{}        snprintf(int_buf, sizeof(int_buf), \"%lld\", {}.value.i);\n", ind, resolved_right));
                        output.push_str(&format!("{}        strcat({}, int_buf);\n", ind, temp_concat));
                        output.push_str(&format!("{}    }}\n", ind));
                        output.push_str(&format!("{}{} = (TauValue){{.type = 2, .value.s = strdup({}), .refcount = 1}};\n", ind, resolved_result, temp_concat));
                        output.push_str(&format!("{}}} else {{\n", ind));
                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}.value.i + {}.value.i}};\n", ind, resolved_result, resolved_left, resolved_right));
                        output.push_str(&format!("{}}}\n", ind));
                    } else {
                        match result_type {
                            NativeType::Int64 => {
                                // If result doesn't need wrapping (is declared as native type),
                                // generate native operation, extracting values from TauValue operands if needed
                                if !needs_wrapping {
                                    // Direct native type operation - extract values from operands
                                    output.push_str(&format!("{}{} = {} {} {};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                } else {
                                    // Wrap in TauValue since result variable is declared as TauValue
                                    output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {} {} {}}};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                }
                            }
                            NativeType::Double => {
                                // If result doesn't need wrapping (is declared as native type),
                                // generate native operation, extracting values from TauValue operands if needed
                                if !needs_wrapping {
                                    // Direct native type operation
                                    output.push_str(&format!("{}{} = (double){} {} (double){};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                } else {
                                    // Wrap in TauValue since result variable is declared as TauValue
                                    output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = (double){} {} (double){}}};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                }
                            }
                            NativeType::Bool => {
                                // If result doesn't need wrapping (is declared as native type),
                                // generate native comparison, extracting values from TauValue if needed
                                if !needs_wrapping {
                                    // Direct native comparison - result is a native type
                                    // Extract values from operands (handles both native and TauValue operands)
                                    let left_val = self.extract_value(&resolved_left, left_type);
                                    let right_val = self.extract_value(&resolved_right, right_type);
                                    output.push_str(&format!("{}{} = {} {} {};\n",
                                        ind, resolved_result,
                                        left_val,
                                        op_str,
                                        right_val));
                                } else {
                                    // Wrap in TauValue since result variable is declared as TauValue
                                    output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {} {} {}}};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                }
                            }
                            _ => {
                                // Default case for other types - if result is native, don't wrap
                                if !needs_wrapping {
                                    output.push_str(&format!("{}{} = {} {} {};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                } else {
                                    output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {} {} {}}};\n",
                                        ind, resolved_result,
                                        self.extract_value(&resolved_left, left_type),
                                        op_str,
                                        self.extract_value(&resolved_right, right_type)));
                                }
                            }
                        }
                    }
                    // Track the type of value stored in the result variable
                    // If wrapping was needed, the variable holds TauValue (Generic), not the native type
                    if needs_wrapping {
                        self.var_types.insert(resolved_result, NativeType::Generic);
                    } else {
                        self.var_types.insert(resolved_result, result_type);
                    }
                }
            }

            IRInstruction::Call { func, args, result } => {
                let args_str = args.join(", ");
                let var_types_snapshot = self.var_types.clone();
                
                // Check if this might be a lambda/function pointer call (variable that's not a known function)
                let is_likely_lambda = !func.contains("::") && !func.contains("__") && 
                                       !matches!(func.as_str(), 
                                           "print" | "tauraro_print" | "str" | "len" | "range" | 
                                           "list" | "dict" | "tuple" | "set" | "int" | "float" | 
                                           "bool" | "abs" | "min" | "max" | "sum" | "sorted");
                
                if is_likely_lambda && var_types_snapshot.get(func).is_some() {
                    // This is a call to a variable - might be a lambda
                    if let Some(res) = result {
                        let resolved_res = self.resolve_var_name(res);
                        let resolved_func = self.resolve_var_name(func);
                        // Try to call it as a lambda function
                        output.push_str(&format!(
                            "{}// Lambda or function pointer call\n", ind
                        ));
                        output.push_str(&format!(
                            "{}{{\n", ind
                        ));
                        output.push_str(&format!(
                            "{}    TauValue (*_lambda_func)(int, TauValue*) = (TauValue (*)(int, TauValue*))((intptr_t){}.value.ptr);\n",
                            ind, resolved_func
                        ));
                        output.push_str(&format!(
                            "{}    if (_lambda_func != NULL) {{\n", ind
                        ));
                        
                        // Build arguments array for lambda call
                        if args.is_empty() {
                            output.push_str(&format!("{}        {} = _lambda_func(0, NULL);\n", ind, resolved_res));
                        } else {
                            output.push_str(&format!("{}        TauValue _lambda_args[] = {{ {} }};\n", ind, args_str));
                            output.push_str(&format!("{}        {} = _lambda_func({}, _lambda_args);\n", ind, resolved_res, args.len()));
                        }
                        
                        output.push_str(&format!("{}    }} else {{\n", ind));
                        output.push_str(&format!("{}        {} = tauraro_none();\n", ind, resolved_res));
                        output.push_str(&format!("{}    }}\n", ind));
                        output.push_str(&format!("{}}}\n", ind));
                        self.var_types.insert(resolved_res, NativeType::Generic);
                    }
                } else {
                    match func.as_str() {
                    "print" | "tauraro_print" => {
                        // Format print arguments properly
                        output.push_str(&format!("{}printf(", ind));
                        
                        let mut format_parts = Vec::new();
                        let mut arg_values = Vec::new();
                        
                        for arg in args.iter() {
                            if let Some(var_type) = var_types_snapshot.get(arg) {
                                match var_type {
                                    NativeType::Int64 => {
                                        format_parts.push("%lld".to_string());
                                        arg_values.push(arg.clone());
                                    }
                                    NativeType::Double => {
                                        format_parts.push("%f".to_string());
                                        arg_values.push(arg.clone());
                                    }
                                    NativeType::CStr => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(arg.clone());
                                    }
                                    NativeType::Bool => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("({}.value.i ? \"true\" : \"false\")", arg));
                                    }
                                    NativeType::Object => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("{}->class_name", arg));
                                    }
                                    NativeType::List => {
                                        // Check if it's declared as TauValue (Generic) or raw TauList*
                                        format_parts.push("%s".to_string());
                                        let decl_type = self.scope_variables.get(arg).copied()
                                            .unwrap_or(NativeType::List);
                                        if matches!(decl_type, NativeType::Generic) {
                                            // Already TauValue containing a list, use directly
                                            arg_values.push(format!("tauraro_str_from_value(&{}).value.s", arg));
                                        } else {
                                            // Raw TauList*, need to wrap in TauValue for string conversion
                                            arg_values.push(format!("tauraro_str_from_value(&(TauValue){{.type = 4, .value.list = {}}}).value.s", arg));
                                        }
                                    }
                                    NativeType::Dict => {
                                        // Check if it's declared as TauValue (Generic) or raw TauDict*
                                        format_parts.push("%s".to_string());
                                        let decl_type = self.scope_variables.get(arg).copied()
                                            .unwrap_or(NativeType::Dict);
                                        if matches!(decl_type, NativeType::Generic) {
                                            // Already TauValue containing a dict, use directly
                                            arg_values.push(format!("tauraro_str_from_value(&{}).value.s", arg));
                                        } else {
                                            // Raw TauDict*, need to wrap in TauValue for string conversion
                                            arg_values.push(format!("tauraro_str_from_value(&(TauValue){{.type = 5, .value.dict = {}}}).value.s", arg));
                                        }
                                    }
                                    NativeType::Function => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("\"<function>\""));
                                    }
                                    NativeType::Iterator => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("\"<iterator>\""));
                                    }
                                    NativeType::Exception => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("\"<exception>\""));
                                    }
                                    NativeType::Module => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("\"<module>\""));
                                    }
                                    NativeType::Generic => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("tauraro_str_from_value(&{}).value.s", arg));
                                    }
                                }
                            } else {
                                // Unknown type, treat as string
                                format_parts.push("%s".to_string());
                                arg_values.push(format!("tauraro_str_from_value(&{}).value.s", arg));
                            }
                        }
                        
                        let format_str = format_parts.join(" ");
                        output.push_str(&format!("\"{}\\n\"", format_str));
                        for arg in arg_values {
                            output.push_str(&format!(", {}", arg));
                        }
                        output.push_str(");\n");
                    }
                    "str" => {
                        // Handle str() conversion for different types
                        if let Some(res) = result {
                            if args.len() == 1 {
                                let arg = &args[0];
                                if let Some(var_type) = var_types_snapshot.get(arg) {
                                    match var_type {
                                        NativeType::Int64 => {
                                            output.push_str(&format!("{}{} = tauraro_str_int({});\n", ind, res, arg));
                                        }
                                        NativeType::Double => {
                                            output.push_str(&format!("{}{} = tauraro_str_double({});\n", ind, res, arg));
                                        }
                                        _ => {
                                            output.push_str(&format!("{}{} = tauraro_str_from_value(&{});\n", ind, res, arg));
                                        }
                                    }
                                } else {
                                    output.push_str(&format!("{}{} = tauraro_str_from_value(&{});\n", ind, res, arg));
                                }
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "int" => {
                        // Handle int() conversion
                        if let Some(res) = result {
                            if args.len() == 1 {
                                let arg = &args[0];
                                if let Some(var_type) = var_types_snapshot.get(arg) {
                                    match var_type {
                                        NativeType::CStr => {
                                            output.push_str(&format!("{}{} = tauraro_int_string({});\n", ind, res, arg));
                                        }
                                        _ => {
                                            output.push_str(&format!("{}{} = tauraro_int({});\n", ind, res, arg));
                                        }
                                    }
                                } else {
                                    output.push_str(&format!("{}{} = tauraro_int({});\n", ind, res, arg));
                                }
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "float" => {
                        // Handle float() conversion
                        if let Some(res) = result {
                            if args.len() == 1 {
                                let arg = &args[0];
                                if let Some(var_type) = var_types_snapshot.get(arg) {
                                    match var_type {
                                        NativeType::CStr => {
                                            output.push_str(&format!("{}{} = tauraro_float_string({});\n", ind, res, arg));
                                        }
                                        _ => {
                                            output.push_str(&format!("{}{} = tauraro_float({});\n", ind, res, arg));
                                        }
                                    }
                                } else {
                                    output.push_str(&format!("{}{} = tauraro_float({});\n", ind, res, arg));
                                }
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "len" => {
                        // Handle len() for different types
                        if let Some(res) = result {
                            if args.len() == 1 {
                                let arg = &args[0];
                                // Extract list pointer from TauValue wrapper - check type first
                                output.push_str(&format!("{}if ({}.type == 4) {{ {} = tauraro_list_len((TauList*){}.value.list); }} else {{ {} = (TauValue){{.type = 0, .value.i = 0, .refcount = 1}}; }}\n", ind, arg, res, arg, res));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "range" => {
                        // Handle range() with 1, 2, or 3 arguments
                        if let Some(res) = result {
                            match args.len() {
                                1 => output.push_str(&format!("{}{} = range({});\n", ind, res, args[0])),
                                2 => output.push_str(&format!("{}{} = range2({}, {});\n", ind, res, args[0], args[1])),
                                3 => output.push_str(&format!("{}{} = range3({}, {}, {});\n", ind, res, args[0], args[1], args[2])),
                                _ => output.push_str(&format!("{}{} = range({});\n", ind, res, args.join(", "))),
                            }
                            // range() returns TauValue containing a list, not raw TauList*
                            self.var_types.insert(res.clone(), NativeType::Generic);
                        }
                    }
                    "sum" => {
                        // Handle sum() for lists
                        if let Some(res) = result {
                            if args.len() == 1 {
                                let arg = &args[0];
                                output.push_str(&format!("{}{} = tauraro_sum({});\n", ind, res, arg));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "min" => {
                        // Handle min() with 2 arguments
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_min({}, {});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "max" => {
                        // Handle max() with 2 arguments
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_max({}, {});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "abs" => {
                        // Handle abs() for TauValue
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_abs({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "sorted" => {
                        // Handle sorted() - returns new sorted list
                        if let Some(res) = result {
                            if args.len() >= 1 {
                                output.push_str(&format!("{}{} = tauraro_sorted({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::List);
                            }
                        }
                    }
                    "reversed" => {
                        // Handle reversed() - returns new reversed list
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_reversed({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::List);
                            }
                        }
                    }
                    "enumerate" => {
                        // Handle enumerate() - returns list of (index, value) tuples
                        if let Some(res) = result {
                            if args.len() >= 1 {
                                let start = if args.len() > 1 { args[1].clone() } else { "tauraro_int(0)".to_string() };
                                output.push_str(&format!("{}{} = tauraro_enumerate_list({}, {});\n", ind, res, args[0], start));
                                self.var_types.insert(res.clone(), NativeType::List);
                            }
                        }
                    }
                    "zip" => {
                        // Handle zip() - returns list of tuples
                        if let Some(res) = result {
                            if args.len() >= 2 {
                                output.push_str(&format!("{}{} = tauraro_zip_lists({}, {});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::List);
                            }
                        }
                    }
                    "any" => {
                        // Handle any() - returns True if any element is truthy
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_any({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "all" => {
                        // Handle all() - returns True if all elements are truthy
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_all({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "type" => {
                        // Handle type() - returns type name as string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_type_name({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "isinstance" => {
                        // Handle isinstance() check
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_isinstance({}, {});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "ord" => {
                        // Handle ord() - character to ASCII value
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_ord({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "chr" => {
                        // Handle chr() - ASCII value to character
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_chr({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "round" => {
                        // Handle round() - round to nearest integer or decimal places
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_round({}, tauraro_int(0));\n", ind, res, args[0]));
                            } else if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_round({}, {});\n", ind, res, args[0], args[1]));
                            }
                            self.var_types.insert(res.clone(), NativeType::Generic);
                        }
                    }
                    "pow" => {
                        // Handle pow() - power function
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_pow({}, {});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "sqrt" => {
                        // Handle sqrt()
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_sqrt({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "input" => {
                        // Handle input() - read line from stdin
                        if let Some(res) = result {
                            if args.is_empty() {
                                output.push_str(&format!("{}{} = tauraro_input(tauraro_str(\"\"));\n", ind, res));
                            } else {
                                output.push_str(&format!("{}{} = tauraro_input({});\n", ind, res, args[0]));
                            }
                            self.var_types.insert(res.clone(), NativeType::CStr);
                        }
                    }
                    "bool" => {
                        // Handle bool() conversion
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_to_bool({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "list" => {
                        // Handle list() - convert to list
                        if let Some(res) = result {
                            if args.is_empty() {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = tauraro_create_list(8)}};\n", ind, res));
                            } else {
                                output.push_str(&format!("{}{} = tauraro_to_list({});\n", ind, res, args[0]));
                            }
                            self.var_types.insert(res.clone(), NativeType::List);
                        }
                    }
                    "dict" => {
                        // Handle dict() - create/convert to dict
                        if let Some(res) = result {
                            output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = tauraro_create_dict()}};\n", ind, res));
                            self.var_types.insert(res.clone(), NativeType::Dict);
                        }
                    }
                    "set" => {
                        // Handle set() - create set (implemented as dict with None values)
                        if let Some(res) = result {
                            if args.is_empty() {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = tauraro_create_dict()}};\n", ind, res));
                            } else {
                                output.push_str(&format!("{}{} = tauraro_to_set({});\n", ind, res, args[0]));
                            }
                            self.var_types.insert(res.clone(), NativeType::Dict);
                        }
                    }
                    "tuple" => {
                        // Handle tuple() - create tuple from elements (list in C)
                        if let Some(res) = result {
                            if args.is_empty() {
                                // Empty tuple
                                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = tauraro_create_list(0)}};\n", ind, res));
                            } else if args.len() == 1 {
                                // Single argument - could be an iterable to convert
                                output.push_str(&format!("{}{} = tauraro_to_list({});\n", ind, res, args[0]));
                            } else {
                                // Multiple arguments - create tuple from all elements
                                output.push_str(&format!("{}TauList* _tuple_{} = tauraro_create_list({});\n", ind, self.temp_var_counter, args.len()));
                                let tuple_var = format!("_tuple_{}", self.temp_var_counter);
                                self.temp_var_counter += 1;
                                for arg in args {
                                    output.push_str(&format!("{}tauraro_list_append({}, {});\n", ind, tuple_var, arg));
                                }
                                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}}};\n", ind, res, tuple_var));
                            }
                            self.var_types.insert(res.clone(), NativeType::List);
                        }
                    }
                    "hex" => {
                        // Handle hex() - convert to hex string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_hex({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "bin" => {
                        // Handle bin() - convert to binary string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_bin({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "oct" => {
                        // Handle oct() - convert to octal string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_oct({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "divmod" => {
                        // Handle divmod() - returns (quotient, remainder)
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_divmod({}, {});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::List);
                            }
                        }
                    }
                    // Memory management functions
                    "allocate" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_allocate(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "free" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_free(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "create_arena" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_create_arena(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "destroy_arena" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_destroy_arena(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "reset_arena" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_reset_arena(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "memory_stats" => {
                        if let Some(res) = result {
                            output.push_str(&format!("{}{} = tauraro_memory_stats(0, NULL);\n", ind, res));
                            self.var_types.insert(res.clone(), NativeType::Generic);
                        }
                    }
                    // System programming functions
                    "sizeof" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_sizeof(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "alignof" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_alignof(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "memcpy" => {
                        if let Some(res) = result {
                            if args.len() == 3 {
                                output.push_str(&format!("{}{} = tauraro_memcpy(3, (TauValue*[]){{&{}, &{}, &{}}});\n", ind, res, args[0], args[1], args[2]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "memset" => {
                        if let Some(res) = result {
                            if args.len() == 3 {
                                output.push_str(&format!("{}{} = tauraro_memset(3, (TauValue*[]){{&{}, &{}, &{}}});\n", ind, res, args[0], args[1], args[2]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "memmove" => {
                        if let Some(res) = result {
                            if args.len() == 3 {
                                output.push_str(&format!("{}{} = tauraro_memmove(3, (TauValue*[]){{&{}, &{}, &{}}});\n", ind, res, args[0], args[1], args[2]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "memcmp" => {
                        if let Some(res) = result {
                            if args.len() == 3 {
                                output.push_str(&format!("{}{} = tauraro_memcmp(3, (TauValue*[]){{&{}, &{}, &{}}});\n", ind, res, args[0], args[1], args[2]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "ptr_read" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_ptr_read(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                            } else if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_ptr_read(2, (TauValue*[]){{&{}, &{}}});\n", ind, res, args[0], args[1]));
                            }
                            self.var_types.insert(res.clone(), NativeType::Generic);
                        }
                    }
                    "ptr_write" => {
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_ptr_write(2, (TauValue*[]){{&{}, &{}}});\n", ind, res, args[0], args[1]));
                            } else if args.len() == 3 {
                                output.push_str(&format!("{}{} = tauraro_ptr_write(3, (TauValue*[]){{&{}, &{}, &{}}});\n", ind, res, args[0], args[1], args[2]));
                            }
                            self.var_types.insert(res.clone(), NativeType::Generic);
                        }
                    }
                    "ptr_offset" => {
                        if let Some(res) = result {
                            if args.len() == 2 {
                                output.push_str(&format!("{}{} = tauraro_ptr_offset(2, (TauValue*[]){{&{}, &{}}});\n", ind, res, args[0], args[1]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "null_ptr" => {
                        if let Some(res) = result {
                            output.push_str(&format!("{}{} = tauraro_null_ptr(0, NULL);\n", ind, res));
                            self.var_types.insert(res.clone(), NativeType::Generic);
                        }
                    }
                    "is_null" => {
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_is_null(1, (TauValue*[]){{&{}}});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    _ => {
                        // Check for polymorphic base class method calls (Shape__area, Shape__get_name, etc)
                        // Only dispatch for methods that are known to be overridden
                        let is_base_shape_call = func == "Shape__area" && !args.is_empty();
                        
                        if is_base_shape_call && result.is_some() {
                            // Generate polymorphic dispatch for area() which is overridden by Circle and Rectangle
                            let obj_arg = &args[0];
                            let res = result.as_ref().unwrap();
                            
                            output.push_str(&format!(
                                "{}if ({}.type == 6 && {}.value.obj && {}.value.obj->class_name) {{\n",
                                ind, obj_arg, obj_arg, obj_arg
                            ));
                            output.push_str(&format!(
                                "{}    if (strcmp({}.value.obj->class_name, \"Circle\") == 0) {{\n",
                                ind, obj_arg
                            ));
                            output.push_str(&format!(
                                "{}        {} = Circle__area({});\n",
                                ind, res, args_str
                            ));
                            output.push_str(&format!(
                                "{}    }} else if (strcmp({}.value.obj->class_name, \"Rectangle\") == 0) {{\n",
                                ind, obj_arg
                            ));
                            output.push_str(&format!(
                                "{}        {} = Rectangle__area({});\n",
                                ind, res, args_str
                            ));
                            output.push_str(&format!(
                                "{}    }} else {{\n",
                                ind
                            ));
                            output.push_str(&format!(
                                "{}        {} = {}({});\n",
                                ind, res, func, args_str
                            ));
                            output.push_str(&format!(
                                "{}    }}\n",
                                ind
                            ));
                            output.push_str(&format!(
                                "{}}}\n",
                                ind
                            ));
                        } else if func.contains('.') {
                            let parts: Vec<&str> = func.split('.').collect();
                            if parts.len() == 2 {
                                let object = parts[0];
                                let method = parts[1];
                                
                                if let Some(res) = result {
                                    output.push_str(&format!("{}{} = tauraro_call_method({}, \"{}\", {}, {});\n", 
                                        ind, res, object, method, args.len(), args_str));
                                    self.var_types.insert(res.clone(), NativeType::Generic);
                                } else {
                                    output.push_str(&format!("{}tauraro_call_method({}, \"{}\", {}, {});\n", 
                                        ind, object, method, args.len(), args_str));
                                }
                            }
                        } else {
                            // Regular function call
                            // Check if this is a string method call pattern (e.g., s__upper should be text__upper)
                            let mut call_func = if func.ends_with("__upper") || func.ends_with("__lower") || 
                                              func.ends_with("__strip") || func.ends_with("__split") || 
                                              func.ends_with("__join") {
                                // Replace variable prefix with "text" for string methods
                                func.split("__").nth(1).map(|method| format!("text__{}", method)).unwrap_or_else(|| func.clone())
                            } else {
                                // Mangle function name if it's a C keyword
                                self.mangle_c_keyword(func)
                            };
                            
                            // Check if we have parameter type information for this function
                            // This helps us convert arguments appropriately
                            let processed_args = if let Some(func_info) = self.function_definitions.get(func) {
                                // We have parameter type info for this function
                                args.iter().enumerate().map(|(i, arg)| {
                                    if i < func_info.params.len() {
                                        let (_, param_type) = &func_info.params[i];
                                        let arg_value_type = self.var_types.get(arg).cloned().unwrap_or(NativeType::Generic);
                                        // Also check declaration type to know if it's a native var or TauValue
                                        let arg_decl_type = self.scope_variables.get(arg).cloned().unwrap_or(NativeType::Generic);
                                        
                                        // Check if we need to unwrap or wrap
                                        // Important: If decl_type is Generic (TauValue) but value_type is List/Dict,
                                        // we need to extract .value.list/.value.dict when param expects List/Dict
                                        // Similarly, if decl_type is Generic and param expects Generic, no conversion needed
                                        match (param_type, arg_value_type, arg_decl_type) {
                                            // When param expects native type and arg is TauValue containing that type
                                            (NativeType::Int64, NativeType::Int64, NativeType::Generic) |
                                            (NativeType::Int64, NativeType::Generic, NativeType::Generic) => {
                                                // Unwrap TauValue to long long
                                                format!("{}.value.i", arg)
                                            }
                                            (NativeType::Double, NativeType::Double, NativeType::Generic) |
                                            (NativeType::Double, NativeType::Generic, NativeType::Generic) => {
                                                // Unwrap TauValue to double
                                                format!("{}.value.f", arg)
                                            }
                                            (NativeType::CStr, NativeType::CStr, NativeType::Generic) |
                                            (NativeType::CStr, NativeType::Generic, NativeType::Generic) => {
                                                // Unwrap TauValue to string
                                                format!("{}.value.s", arg)
                                            }
                                            (NativeType::List, NativeType::List, NativeType::Generic) |
                                            (NativeType::List, NativeType::Generic, NativeType::Generic) => {
                                                // Unwrap TauValue to TauList*
                                                format!("{}.value.list", arg)
                                            }
                                            (NativeType::Dict, NativeType::Dict, NativeType::Generic) |
                                            (NativeType::Dict, NativeType::Generic, NativeType::Generic) => {
                                                // Unwrap TauValue to TauDict*
                                                format!("{}.value.dict", arg)
                                            }
                                            // When param expects TauValue (Generic) but arg is native type
                                            (NativeType::Generic, NativeType::Int64, NativeType::Int64) => {
                                                // Wrap long long to TauValue
                                                format!("(TauValue){{.type = 0, .value.i = {}}}", arg)
                                            }
                                            (NativeType::Generic, NativeType::Double, NativeType::Double) => {
                                                // Wrap double to TauValue
                                                format!("(TauValue){{.type = 1, .value.f = {}}}", arg)
                                            }
                                            (NativeType::Generic, NativeType::List, NativeType::List) => {
                                                // Wrap TauList* to TauValue (only when declaration is also List)
                                                format!("(TauValue){{.type = 4, .value.list = {}}}", arg)
                                            }
                                            (NativeType::Generic, NativeType::Dict, NativeType::Dict) => {
                                                // Wrap TauDict* to TauValue (only when declaration is also Dict)
                                                format!("(TauValue){{.type = 5, .value.dict = {}}}", arg)
                                            }
                                            // When param expects TauValue and arg is already TauValue (containing list/dict)
                                            (NativeType::Generic, NativeType::List, NativeType::Generic) |
                                            (NativeType::Generic, NativeType::Dict, NativeType::Generic) |
                                            (NativeType::Generic, NativeType::Generic, NativeType::Generic) => {
                                                // Already TauValue, no conversion needed
                                                arg.clone()
                                            }
                                            _ => arg.clone() // No conversion needed for exact matches
                                        }
                                    } else {
                                        arg.clone()
                                    }
                                }).collect::<Vec<_>>().join(", ")
                            } else {
                                args_str.clone() // No conversion if we don't know parameter types
                            };
                            
                            // Check if this is a function call with fewer arguments than expected (default arguments)
                            if args.is_empty() {
                                // Check if this function has a wrapper for 0 arguments
                                // First check with mangled name, then original
                                let check_name = self.mangle_c_keyword(func);
                                if let Some((expected_count, _)) = self.function_params.get(check_name.as_str())
                                    .or_else(|| self.function_params.get(func.as_str())) {
                                    if *expected_count > 0 {
                                        // Use the arity-specific wrapper
                                        call_func = format!("{}_arity_0", call_func);
                                    }
                                }
                            } else {
                                let check_name = self.mangle_c_keyword(func);
                                if let Some((expected_count, _)) = self.function_params.get(check_name.as_str())
                                    .or_else(|| self.function_params.get(func.as_str())) {
                                    if args.len() < *expected_count {
                                        // Use the arity-specific wrapper
                                        call_func = format!("{}_arity_{}", call_func, args.len());
                                    }
                                }
                            }
                            
                            // Resolve result variable name
                            if let Some(res) = result {
                                let resolved_res = self.resolve_var_name(res);
                                
                                // Check if the function returns a native type
                                let func_return_type = self.function_definitions.get(func)
                                    .map(|f| f.return_type)
                                    .unwrap_or(NativeType::Generic);
                                
                                // Check what type the result variable is
                                let result_var_type = self.scope_variables.get(&resolved_res).copied()
                                    .or_else(|| self.var_types.get(&resolved_res).copied())
                                    .unwrap_or(NativeType::Generic);
                                
                                // If function returns native type but result is TauValue, wrap the result
                                match (func_return_type, result_var_type) {
                                    (NativeType::Int64, NativeType::Generic) => {
                                        // Wrap long long return in TauValue
                                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}({})}};\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Double, NativeType::Generic) => {
                                        // Wrap double return in TauValue
                                        output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = {}({})}};\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Bool, NativeType::Generic) => {
                                        // Wrap bool return in TauValue
                                        output.push_str(&format!("{}{} = (TauValue){{.type = 3, .value.i = {}({})}};\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::CStr, NativeType::Generic) => {
                                        // Wrap string return in TauValue
                                        output.push_str(&format!("{}{} = (TauValue){{.type = 2, .value.s = {}({})}};\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::List, NativeType::Generic) => {
                                        // Wrap TauList* return in TauValue
                                        output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}({})}};\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Dict, NativeType::Generic) => {
                                        // Wrap TauDict* return in TauValue
                                        output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = {}({})}};\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Generic, NativeType::Int64) => {
                                        // Unwrap TauValue return to long long
                                        output.push_str(&format!("{}{} = {}({}).value.i;\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Generic, NativeType::Double) => {
                                        // Unwrap TauValue return to double
                                        output.push_str(&format!("{}{} = {}({}).value.f;\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Generic, NativeType::List) => {
                                        // Unwrap TauValue return to TauList*
                                        output.push_str(&format!("{}{} = {}({}).value.list;\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    (NativeType::Generic, NativeType::Dict) => {
                                        // Unwrap TauValue return to TauDict*
                                        output.push_str(&format!("{}{} = {}({}).value.dict;\n", 
                                            ind, resolved_res, call_func, processed_args));
                                    }
                                    _ => {
                                        // Direct assignment for matching types or Generic functions
                                        output.push_str(&format!("{}{} = {}({});\n", ind, resolved_res, call_func, processed_args));
                                    }
                                }
                                
                                // Update var_types based on what we ACTUALLY stored
                                // If we wrapped the result in TauValue, the stored type is Generic
                                // If we unwrapped or did direct assignment, use the appropriate type
                                match (func_return_type, result_var_type) {
                                    // When we wrapped native return in TauValue, result is Generic (TauValue)
                                    (NativeType::Int64, NativeType::Generic) |
                                    (NativeType::Double, NativeType::Generic) |
                                    (NativeType::Bool, NativeType::Generic) |
                                    (NativeType::CStr, NativeType::Generic) |
                                    (NativeType::List, NativeType::Generic) |
                                    (NativeType::Dict, NativeType::Generic) => {
                                        self.var_types.insert(resolved_res.clone(), NativeType::Generic);
                                    }
                                    // When we unwrapped TauValue to native, result is native type
                                    (NativeType::Generic, NativeType::Int64) => {
                                        self.var_types.insert(resolved_res.clone(), NativeType::Int64);
                                    }
                                    (NativeType::Generic, NativeType::Double) => {
                                        self.var_types.insert(resolved_res.clone(), NativeType::Double);
                                    }
                                    (NativeType::Generic, NativeType::List) => {
                                        self.var_types.insert(resolved_res.clone(), NativeType::List);
                                    }
                                    (NativeType::Generic, NativeType::Dict) => {
                                        self.var_types.insert(resolved_res.clone(), NativeType::Dict);
                                    }
                                    // Direct assignment - use function return type
                                    _ => {
                                        self.var_types.insert(resolved_res.clone(), func_return_type);
                                    }
                                }
                            } else {
                                output.push_str(&format!("{}{}({});\n", ind, call_func, processed_args));
                            }
                        }
                    }
                }
                }  // Close the else block for lambda check
            }

            IRInstruction::Return { value } => {
                if let Some(val) = value {
                    let resolved_val = self.resolve_var_name(val);
                    
                    // Get the function's return type from function_definitions
                    let return_type = self.current_function.as_ref()
                        .and_then(|fname| self.function_definitions.get(fname))
                        .map(|finfo| finfo.return_type)
                        .unwrap_or(NativeType::Generic);
                    
                    // Get the value's type
                    let val_type = self.var_types.get(&resolved_val).copied()
                        .or_else(|| self.scope_variables.get(&resolved_val).copied())
                        .unwrap_or(NativeType::Generic);
                    
                    // Convert if needed
                    match (return_type, val_type) {
                        (NativeType::Int64, NativeType::Generic) => {
                            output.push_str(&format!("{}return {}.value.i;\n", ind, resolved_val));
                        }
                        (NativeType::Double, NativeType::Generic) => {
                            output.push_str(&format!("{}return {}.value.f;\n", ind, resolved_val));
                        }
                        (NativeType::CStr, NativeType::Generic) => {
                            output.push_str(&format!("{}return {}.value.s;\n", ind, resolved_val));
                        }
                        (NativeType::List, NativeType::Generic) => {
                            output.push_str(&format!("{}return {}.value.list;\n", ind, resolved_val));
                        }
                        (NativeType::Dict, NativeType::Generic) => {
                            output.push_str(&format!("{}return {}.value.dict;\n", ind, resolved_val));
                        }
                        (NativeType::Generic, NativeType::Int64) => {
                            output.push_str(&format!("{}return (TauValue){{.type = 0, .value.i = {}}};\n", ind, resolved_val));
                        }
                        (NativeType::Generic, NativeType::Double) => {
                            output.push_str(&format!("{}return (TauValue){{.type = 1, .value.f = {}}};\n", ind, resolved_val));
                        }
                        (NativeType::Generic, NativeType::List) => {
                            output.push_str(&format!("{}return (TauValue){{.type = 4, .value.list = {}}};\n", ind, resolved_val));
                        }
                        (NativeType::Generic, NativeType::Dict) => {
                            output.push_str(&format!("{}return (TauValue){{.type = 5, .value.dict = {}}};\n", ind, resolved_val));
                        }
                        _ => {
                            output.push_str(&format!("{}return {};\n", ind, resolved_val));
                        }
                    }
                } else {
                    output.push_str(&format!("{}return {{.type = 0, .value.i = 0}};\n", ind));
                }
            }

            IRInstruction::If { condition, then_body, elif_branches, else_body } => {
                // Check the type of condition variable - if it's a native type, use it directly
                let resolved_condition = self.resolve_var_name(condition);
                let condition_type = self.var_types.get(&resolved_condition).copied()
                    .or_else(|| self.scope_variables.get(&resolved_condition).copied())
                    .unwrap_or(NativeType::Generic);
                
                // Generate appropriate condition check based on variable type
                let condition_check = match condition_type {
                    NativeType::Int64 | NativeType::Bool => {
                        // Native type - use it directly (0 = false, non-zero = true)
                        format!("{}", resolved_condition)
                    }
                    NativeType::Double => {
                        // Native double - compare against 0.0
                        format!("({} != 0.0)", resolved_condition)
                    }
                    _ => {
                        // TauValue - extract boolean from TauValue
                        format!("({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))", 
                            resolved_condition, resolved_condition, resolved_condition, resolved_condition)
                    }
                };
                output.push_str(&format!("{}if ({}) {{\n", ind, condition_check));
                for instr in then_body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }

                for (elif_cond, elif_instrs) in elif_branches {
                    let resolved_elif_cond = self.resolve_var_name(elif_cond);
                    let elif_type = self.var_types.get(&resolved_elif_cond).copied()
                        .or_else(|| self.scope_variables.get(&resolved_elif_cond).copied())
                        .unwrap_or(NativeType::Generic);
                    
                    let elif_check = match elif_type {
                        NativeType::Int64 | NativeType::Bool => {
                            format!("{}", resolved_elif_cond)
                        }
                        NativeType::Double => {
                            format!("({} != 0.0)", resolved_elif_cond)
                        }
                        _ => {
                            format!("({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))", 
                                resolved_elif_cond, resolved_elif_cond, resolved_elif_cond, resolved_elif_cond)
                        }
                    };
                    output.push_str(&format!("{}}} else if ({}) {{\n", ind, elif_check));
                    for instr in elif_instrs {
                        output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                    }
                }

                if let Some(else_instrs) = else_body {
                    output.push_str(&format!("{}}} else {{\n", ind));
                    for instr in else_instrs {
                        output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                    }
                }
                output.push_str(&format!("{}}}\n", ind));
            }

            IRInstruction::While { condition, condition_instructions, body } => {
                // For while loops, we need to:
                // 1. Generate condition setup code (comparisons, etc.)
                // 2. Generate while with the condition check
                // 3. Execute body and let condition be re-evaluated on next iteration
                
                // Generate the while loop
                output.push_str(&format!("{}while (1) {{\n", ind)); // Infinite loop, condition check inside
                
                // Re-evaluate the condition at the START of each iteration
                for instr in condition_instructions {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }
                
                // Check the type of condition variable - if it's a native type, use it directly
                let resolved_condition = self.resolve_var_name(condition);
                let condition_type = self.var_types.get(&resolved_condition).copied()
                    .or_else(|| self.scope_variables.get(&resolved_condition).copied())
                    .unwrap_or(NativeType::Generic);
                
                // Generate appropriate condition check based on variable type
                let condition_check = match condition_type {
                    NativeType::Int64 | NativeType::Bool => {
                        // Native type - use it directly (0 = false, non-zero = true)
                        format!("{}", resolved_condition)
                    }
                    NativeType::Double => {
                        // Native double - compare against 0.0
                        format!("({} != 0.0)", resolved_condition)
                    }
                    _ => {
                        // TauValue - extract boolean from TauValue
                        format!("({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))", 
                            resolved_condition, resolved_condition, resolved_condition, resolved_condition)
                    }
                };
                
                // Check condition and break if false
                output.push_str(&format!("{}if (!({} )) break;\n", ind.repeat(2), condition_check));
                
                // Execute body
                for instr in body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }
                output.push_str(&format!("{}}}\n", ind));
            }

            IRInstruction::For { variable, iterable, body, .. } => {
                // For loop - iterate over list items
                output.push_str(&format!("{}// for loop over {}\n", ind, iterable));
                
                // Check if the loop variable is a typed variable
                let resolved_variable = self.resolve_var_name(variable);
                let var_type = self.var_types.get(&resolved_variable).copied()
                    .or_else(|| self.scope_variables.get(&resolved_variable).copied())
                    .unwrap_or(NativeType::Generic);
                
                // Check if the iterable is a native TauList* or a TauValue containing a list
                let resolved_iterable = self.resolve_var_name(iterable);
                let iterable_type = self.var_types.get(&resolved_iterable).copied()
                    .or_else(|| self.scope_variables.get(&resolved_iterable).copied())
                    .or_else(|| self.var_types.get(iterable).copied())
                    .or_else(|| self.scope_variables.get(iterable).copied())
                    .unwrap_or(NativeType::Generic);
                
                // Generate unique temp variables for iteration
                self.temp_var_counter += 1;
                let loop_counter = format!("_for_i_{}", self.temp_var_counter);
                
                // Handle native TauList* vs TauValue containing a list
                if matches!(iterable_type, NativeType::List) {
                    // Direct TauList* - no need to wrap/unwrap
                    output.push_str(&format!("{}TauList* _list = {};\n", ind, resolved_iterable));
                    output.push_str(&format!("{}for(int {} = 0; {} < _list->size; {}++) {{\n", ind, loop_counter, loop_counter, loop_counter));
                    
                    // Declare/assign the loop variable based on its type
                    match var_type {
                        NativeType::Int64 => {
                            output.push_str(&format!("{}    {} = _list->items[{}].value.i;\n", ind, resolved_variable, loop_counter));
                        }
                        NativeType::Double => {
                            output.push_str(&format!("{}    {} = _list->items[{}].value.f;\n", ind, resolved_variable, loop_counter));
                        }
                        _ => {
                            output.push_str(&format!("{}    TauValue {} = _list->items[{}];\n", ind, variable, loop_counter));
                        }
                    }
                    
                    // Process loop body
                    for instr in body {
                        output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                    }
                    
                    output.push_str(&format!("{}}}\n", ind));
                } else {
                    // TauValue containing a list - original behavior
                    let loop_list = format!("_for_list_{}", self.temp_var_counter);
                    
                    // Get the iterable variable
                    output.push_str(&format!("{}TauValue {} = {};\n", ind, loop_list, resolved_iterable));
                    
                    // Check if it's a list and iterate
                    output.push_str(&format!("{}if ({}.type == 4) {{\n", ind, loop_list));
                    output.push_str(&format!("{}    TauList* _list = {}.value.list;\n", ind, loop_list));
                    output.push_str(&format!("{}    for(int {} = 0; {} < _list->size; {}++) {{\n", ind, loop_counter, loop_counter, loop_counter));
                    
                    // Declare/assign the loop variable based on its type
                    match var_type {
                        NativeType::Int64 => {
                            output.push_str(&format!("{}        {} = _list->items[{}].value.i;\n", ind, resolved_variable, loop_counter));
                        }
                        NativeType::Double => {
                            output.push_str(&format!("{}        {} = _list->items[{}].value.f;\n", ind, resolved_variable, loop_counter));
                        }
                        _ => {
                            // For untyped variables, declare as TauValue
                            output.push_str(&format!("{}        TauValue {} = _list->items[{}];\n", ind, variable, loop_counter));
                        }
                    }
                    
                    // Process loop body
                    for instr in body {
                        output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                    }
                    
                    output.push_str(&format!("{}}}\n", ind));
                    output.push_str(&format!("{}}}\n", ind));
                }
            }

            IRInstruction::Break => {
                output.push_str(&format!("{}break;\n", ind));
            }

            IRInstruction::Continue => {
                output.push_str(&format!("{}continue;\n", ind));
            }

            IRInstruction::ListCreate { elements, result } => {
                output.push_str(&format!("{}// Create list with {} elements\n", ind, elements.len()));
                // Use the original result name directly - don't create unique names
                // since StoreLocal will assign this to a specific variable
                let result_var = self.resolve_var_name(result);
                // Create TauList structure
                output.push_str(&format!("{}{{ TauList* _list = malloc(sizeof(TauList));\n", ind));
                output.push_str(&format!("{}_list->size = {};\n", ind, elements.len()));
                output.push_str(&format!("{}_list->capacity = {};\n", ind, elements.len()));
                output.push_str(&format!("{}_list->items = malloc(sizeof(TauValue) * {});\n", ind, elements.len()));
                for (i, elem) in elements.iter().enumerate() {
                    output.push_str(&format!("{}_list->items[{}] = {};\n", ind, i, elem));
                }
                output.push_str(&format!("{}{}.type = 4; {}.value.list = _list; }}\n", ind, result_var, result_var));
                // The result is a TauValue containing a list, not a raw TauList*
                // So the type should remain Generic (TauValue), not NativeType::List
                self.var_types.insert(result_var, NativeType::Generic);
            }

            IRInstruction::DictCreate { pairs, result } => {
                output.push_str(&format!("{}// Create dictionary\n", ind));
                // Use the original result name directly
                let result_var = self.resolve_var_name(result);
                output.push_str(&format!("{}{{ TauDict* _dict = tauraro_create_dict();\n", ind));
                for (key, val) in pairs.iter() {
                    // Extract string from key if needed
                    output.push_str(&format!("{}tauraro_dict_set(_dict, {}.value.s, {});\n", ind, key, val));
                }
                output.push_str(&format!("{}{}.type = 5; {}.value.dict = _dict; }}\n", ind, result_var, result_var));
                // The result is a TauValue containing a dict, not a raw TauDict*
                // So the type should remain Generic (TauValue), not NativeType::Dict
                self.var_types.insert(result_var, NativeType::Generic);
            }

            IRInstruction::Try { body, handlers, else_body, finally_body } => {
                // Simple try block without exception handling
                output.push_str(&format!("{}// try block (simplified)\n", ind));
                output.push_str(&format!("{}{{\n", ind));
                for instr in body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }
                
                // Handle else clause
                if let Some(else_instrs) = else_body {
                    output.push_str(&format!("{}    // else block\n", ind));
                    for instr in else_instrs {
                        output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                    }
                }
                output.push_str(&format!("{}}}\n", ind));
                
                // Handle finally clause
                if let Some(finally_instrs) = finally_body {
                    output.push_str(&format!("{}// finally block\n", ind));
                    for instr in finally_instrs {
                        output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                    }
                }
            }



            IRInstruction::ObjectCreate { class_name, result } => {
                output.push_str(&format!("{}// Create {} object\n", ind, class_name));
                let resolved_result = self.resolve_var_name(result);
                
                // Create unique temporary variable name for the TauObject pointer
                self.temp_var_counter += 1;
                let temp_obj_var = format!("temp_obj_{}", self.temp_var_counter);
                
                // First create the object
                output.push_str(&format!("{}TauObject* {} = tauraro_create_object(\"{}\");\n", ind, temp_obj_var, class_name));
                
                // Wrap it in a TauValue
                output.push_str(&format!("{}{} = (TauValue){{.type = 6, .value.obj = {}, .refcount = 1}};\n", ind, resolved_result, temp_obj_var));
                
                self.var_types.insert(resolved_result.clone(), NativeType::Object);
            }
            
            IRInstruction::ObjectSetAttr { object, attr, value } => {
                output.push_str(&format!("{}// Set attribute {} on object {}\n", ind, attr, object));
                let resolved_object = self.resolve_var_name(object);
                let resolved_value = self.resolve_var_name(value);
                
                // Extract TauObject* from TauValue and call set_attribute
                output.push_str(&format!("{}tauraro_set_attribute({}.value.obj, \"{}\", {});\n", 
                    ind, resolved_object, attr, resolved_value));
            }
            
            IRInstruction::ObjectGetAttr { object, attr, result } => {
                output.push_str(&format!("{}// Get attribute {} from object {}\n", ind, attr, object));
                let resolved_object = self.resolve_var_name(object);
                let resolved_result = self.resolve_var_name(result);
                
                // Extract TauObject* from TauValue and call get_attribute
                output.push_str(&format!("{}{} = tauraro_get_attribute({}.value.obj, \"{}\");\n", 
                    ind, resolved_result, resolved_object, attr));
                
                self.var_types.insert(resolved_result, NativeType::Generic);
            }
            
            IRInstruction::DictSetItem { dict, key, value } => {
                output.push_str(&format!("{}// Set dictionary item {} = {}\n", ind, key, value));
                output.push_str(&format!("{}tauraro_dict_set({}, {}, {});\n", ind, dict, key, value));
            }
            
            IRInstruction::DictGetItem { dict, key, result } => {
                let resolved_dict = self.resolve_var_name(dict);
                let resolved_key = self.resolve_var_name(key);
                let resolved_result = self.resolve_var_name(result);
                
                output.push_str(&format!("{}// Get dictionary or list item\n", ind));
                output.push_str(&format!("{}if ({}.type == 5 && {}.value.dict) {{\n", ind, resolved_dict, resolved_dict));
                output.push_str(&format!("{}    TauValue* _dict_val = tauraro_dict_get({}.value.dict, {}.value.s);\n", ind, resolved_dict, resolved_key));
                output.push_str(&format!("{}    {} = _dict_val ? *_dict_val : tauraro_none();\n", ind, resolved_result));
                output.push_str(&format!("{}}} else if ({}.type == 4 && {}.value.list) {{\n", ind, resolved_dict, resolved_dict));
                output.push_str(&format!("{}    long long _idx = {}.type == 0 ? {}.value.i : 0;\n", ind, resolved_key, resolved_key));
                output.push_str(&format!("{}    if (_idx < 0) _idx = {}.value.list->size + _idx;\n", ind, resolved_dict));
                output.push_str(&format!("{}    if (_idx >= 0 && _idx < (long long){}.value.list->size) {{\n", ind, resolved_dict));
                output.push_str(&format!("{}        {} = {}.value.list->items[_idx];\n", ind, resolved_result, resolved_dict));
                output.push_str(&format!("{}    }} else {{\n", ind));
                output.push_str(&format!("{}        {} = tauraro_none();\n", ind, resolved_result));
                output.push_str(&format!("{}    }}\n", ind));
                output.push_str(&format!("{}}} else {{\n", ind));
                output.push_str(&format!("{}    {} = tauraro_none();\n", ind, resolved_result));
                output.push_str(&format!("}}\n"));
                self.var_types.insert(resolved_result, NativeType::Generic);
            }

            // Enhanced existing instruction support - extend existing Try instruction
            IRInstruction::Try { body, handlers, else_body, finally_body } => {
                output.push_str(&format!("{}// Try-except-finally block (enhanced)\n", ind));
                output.push_str(&format!("{}if (setjmp(tauraro_exception_buf) == 0) {{\n", ind));
                
                // Try body
                for try_instr in body {
                    output.push_str(&self.transpile_instruction(try_instr, indent_level + 1)?);
                }
                
                // Else body (if no exception)
                if let Some(else_body) = else_body {
                    for else_instr in else_body {
                        output.push_str(&self.transpile_instruction(else_instr, indent_level + 1)?);
                    }
                }
                
                output.push_str(&format!("{}}} else {{\n", ind));
                
                // Exception handlers
                for (exc_type, var_name, handler_body) in handlers {
                    if let Some(exc_type) = exc_type {
                        output.push_str(&format!("{}    if (tauraro_exception_matches(\"{}\")) {{\n", ind, exc_type));
                    } else {
                        output.push_str(&format!("{}    // Catch all exceptions\n", ind));
                        output.push_str(&format!("{}    {{\n", ind));
                    }
                    
                    if let Some(var_name) = var_name {
                        output.push_str(&format!("{}        TauException* {} = tauraro_current_exception;\n", ind, var_name));
                    }
                    
                    for handler_instr in handler_body {
                        output.push_str(&self.transpile_instruction(handler_instr, indent_level + 2)?);
                    }
                    output.push_str(&format!("{}    }}\n", ind));
                }
                
                output.push_str(&format!("{}}}\n", ind));
                
                // Finally block
                if let Some(finally_body) = finally_body {
                    output.push_str(&format!("{}// Finally block\n", ind));
                    for finally_instr in finally_body {
                        output.push_str(&self.transpile_instruction(finally_instr, indent_level)?);
                    }
                }
            }
            
            // Enhanced Raise instruction
            IRInstruction::Raise { exception } => {
                output.push_str(&format!("{}// Raise exception\n", ind));
                if let Some(exception) = exception {
                    output.push_str(&format!("{}tauraro_throw_exception({});\n", ind, exception));
                } else {
                    output.push_str(&format!("{}tauraro_throw_exception(tauraro_current_exception);\n", ind));
                }
                output.push_str(&format!("{}return tauraro_none(); // Early return after exception\n", ind));
            }
            
            // Enhanced Import/ImportFrom instructions with advanced module support
            IRInstruction::Import { module } => {
                output.push_str(&format!("{}// Import module: {}\n", ind, module));
                output.push_str(&format!("{}TauModule* module_{} = tauraro_import_module(\"{}\");\n", 
                    ind, module.replace(".", "_"), module));
                output.push_str(&format!("{}TauValue {} = tauraro_module_to_value(module_{});\n", 
                    ind, module.replace(".", "_"), module.replace(".", "_")));
            }
            
            IRInstruction::ImportFrom { module, names } => {
                output.push_str(&format!("{}// From import: from {} import {}\n", 
                    ind, module, names.join(", ")));
                output.push_str(&format!("{}TauModule* temp_module = tauraro_import_module(\"{}\");\n", 
                    ind, module));
                
                for name in names {
                    output.push_str(&format!("{}{} = tauraro_module_get(temp_module, \"{}\");\n", 
                        ind, name, name));
                    self.var_types.insert(name.clone(), NativeType::Generic);
                }
            }
            
            IRInstruction::UnaryOp { op, operand, result } => {
                let resolved_operand = self.resolve_var_name(operand);
                let resolved_result = self.resolve_var_name(result);
                
                let operand_type = self.var_types.get(&resolved_operand).copied()
                    .or_else(|| self.var_types.get(operand).copied())
                    .unwrap_or(NativeType::Generic);

                match op {
                    crate::ast::UnaryOp::Not => {
                        // Logical NOT operation
                        output.push_str(&format!("{}{} = (TauValue){{.type = 3, .value.i = !{}.value.i}};\n",
                            ind, resolved_result, resolved_operand));
                    }
                    crate::ast::UnaryOp::USub | crate::ast::UnaryOp::Minus => {
                        // Negation operation
                        match operand_type {
                            NativeType::Int64 => {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = -{}.value.i}};\n",
                                    ind, resolved_result, resolved_operand));
                            }
                            NativeType::Double => {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = -{}.value.f}};\n",
                                    ind, resolved_result, resolved_operand));
                            }
                            _ => {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = -{}.value.i}};\n",
                                    ind, resolved_result, resolved_operand));
                            }
                        }
                    }
                    crate::ast::UnaryOp::UAdd => {
                        // Unary plus (just copy the value)
                        output.push_str(&format!("{}{} = {};\n",
                            ind, resolved_result, resolved_operand));
                    }
                    crate::ast::UnaryOp::Invert | crate::ast::UnaryOp::BitNot => {
                        // Bitwise NOT operation
                        output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = ~{}.value.i}};\n",
                            ind, resolved_result, resolved_operand));
                    }
                }
                self.var_types.insert(resolved_result, NativeType::Generic);
            }

            // Super call for OOP inheritance
            IRInstruction::SuperCall { args, result } => {
                output.push_str(&format!("{}// super() call\n", ind));
                let resolved_result = self.resolve_var_name(result);
                
                // Generate super call - calls parent class constructor/method
                if args.is_empty() {
                    output.push_str(&format!("{}{} = tauraro_super_call(self, NULL, 0);\n", 
                        ind, resolved_result));
                } else {
                    let args_list = args.iter()
                        .map(|a| self.resolve_var_name(a))
                        .collect::<Vec<_>>()
                        .join(", ");
                    output.push_str(&format!("{}{{ TauValue _super_args[] = {{ {} }};\n", ind, args_list));
                    output.push_str(&format!("{}{} = tauraro_super_call(self, _super_args, {}); }}\n", 
                        ind, resolved_result, args.len()));
                }
                self.var_types.insert(resolved_result, NativeType::Generic);
            }

            // ==================== NEW ADVANCED FEATURES ====================
            
            // Lambda expressions - compile to function pointers
            IRInstruction::Lambda { params, body_instructions, captured_vars, result, body_result_var } => {
                let resolved_result = self.resolve_var_name(result);
                self.temp_var_counter += 1;
                let lambda_id = self.temp_var_counter;
                let lambda_func_name = format!("_lambda_impl_{}", lambda_id);
                
                output.push_str(&format!("{}// Lambda expression\n", ind));
                
                // Ensure the result variable is declared
                if !self.declared_vars.contains(&resolved_result) && !self.scope_variables.contains_key(&resolved_result) {
                    output.push_str(&format!("{}TauValue {};\n", ind, resolved_result));
                    self.declared_vars.insert(resolved_result.clone());
                }
                
                // Generate a unique function for this lambda
                // We store the function pointer in a TauValue with type=7 (pointer)
                output.push_str(&format!("{}{} = (TauValue){{.type = 7, .value.ptr = (void*)(intptr_t){}}};\n", 
                    ind, resolved_result, lambda_func_name));
                
                // Record that this variable holds a function pointer
                self.var_types.insert(resolved_result, NativeType::Function);
                
                // Store lambda info for later code generation
                // We'll generate the actual function at the end in a helper section
                self.lambdas_to_generate.push((
                    lambda_func_name.clone(),
                    params.clone(),
                    body_instructions.clone(),
                    captured_vars.clone(),
                    body_result_var.clone()
                ));
            }
            
            // List comprehension - [expr for x in iterable if condition]
            IRInstruction::ListComprehension { 
                element_instrs, element_result, variable, iterable, 
                condition_instrs, condition_result, result 
            } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_iterable = self.resolve_var_name(iterable);
                let resolved_var = self.resolve_var_name(variable);
                let resolved_elem = self.resolve_var_name(element_result);
                
                output.push_str(&format!("{}// List comprehension\n", ind));
                
                // Declare the result variable
                output.push_str(&format!("{}TauValue {};\n", ind, resolved_result));
                
                output.push_str(&format!("{}TauList* _lc_{} = tauraro_create_list(16);\n", ind, self.temp_var_counter));
                self.temp_var_counter += 1;
                let lc_list = format!("_lc_{}", self.temp_var_counter - 1);
                
                // Collect all temporary variables that will be used in the loop
                let mut all_temp_vars = std::collections::HashSet::new();
                all_temp_vars.extend(self.collect_temp_vars_from_instructions(element_instrs));
                all_temp_vars.extend(self.collect_temp_vars_from_instructions(condition_instrs));
                
                // Pre-declare loop temps before the loop
                let loop_ind = format!("{}    ", ind);
                for temp_var in &all_temp_vars {
                    if temp_var != &resolved_var && temp_var != &resolved_iterable && temp_var != &resolved_result {
                        output.push_str(&format!("{}TauValue {};\n", loop_ind, temp_var));
                    }
                }
                
                // Iterate over source
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL) {{\n", ind, resolved_iterable, resolved_iterable));
                output.push_str(&format!("{}    for (size_t _i = 0; _i < {}.value.list->size; _i++) {{\n", ind, resolved_iterable));
                output.push_str(&format!("{}        TauValue {} = {}.value.list->items[_i];\n", ind, resolved_var, resolved_iterable));
                
                // Apply condition if present
                if !condition_instrs.is_empty() {
                    if let Some(cond_result) = condition_result {
                        let cond_res = self.resolve_var_name(cond_result);
                        // Compile condition instructions
                        for instr in condition_instrs {
                            output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                        }
                        output.push_str(&format!("{}        if (!({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))) continue;\n", 
                            ind, cond_res, cond_res, cond_res, cond_res));
                    }
                }
                
                // Compute element - execute stored instructions
                for instr in element_instrs {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                }
                
                // Append to result list
                output.push_str(&format!("{}        tauraro_list_append({}, {});\n", ind, lc_list, resolved_elem));
                output.push_str(&format!("{}    }}\n", ind));
                output.push_str(&format!("{}}}\n", ind));
                
                // Store result
                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}}};\n", ind, resolved_result, lc_list));
                self.var_types.insert(resolved_result, NativeType::List);
            }
            
            // Dict comprehension - {key: value for x in iterable if condition}
            IRInstruction::DictComprehension {
                key_instrs, key_result, value_instrs, value_result,
                variable, iterable, condition_instrs, condition_result, result
            } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_iterable = self.resolve_var_name(iterable);
                let resolved_var = self.resolve_var_name(variable);
                let resolved_key = self.resolve_var_name(key_result);
                let resolved_val = self.resolve_var_name(value_result);
                
                output.push_str(&format!("{}// Dict comprehension\n", ind));
                
                // Declare the result variable
                output.push_str(&format!("{}TauValue {};\n", ind, resolved_result));
                
                output.push_str(&format!("{}TauDict* _dc_{} = tauraro_create_dict();\n", ind, self.temp_var_counter));
                self.temp_var_counter += 1;
                let dc_dict = format!("_dc_{}", self.temp_var_counter - 1);
                
                // Collect all temporary variables that will be used in the loop
                let mut all_temp_vars = std::collections::HashSet::new();
                all_temp_vars.extend(self.collect_temp_vars_from_instructions(key_instrs));
                all_temp_vars.extend(self.collect_temp_vars_from_instructions(value_instrs));
                all_temp_vars.extend(self.collect_temp_vars_from_instructions(condition_instrs));
                
                // Pre-declare loop temps before the loop
                let loop_ind = format!("{}    ", ind);
                for temp_var in &all_temp_vars {
                    if temp_var != &resolved_var && temp_var != &resolved_iterable && temp_var != &resolved_result {
                        output.push_str(&format!("{}TauValue {};\n", loop_ind, temp_var));
                    }
                }
                
                // Iterate over source
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL) {{\n", ind, resolved_iterable, resolved_iterable));
                output.push_str(&format!("{}    for (size_t _i = 0; _i < {}.value.list->size; _i++) {{\n", ind, resolved_iterable));
                output.push_str(&format!("{}        TauValue {} = {}.value.list->items[_i];\n", ind, resolved_var, resolved_iterable));
                
                // Apply condition if present
                if !condition_instrs.is_empty() {
                    if let Some(cond_result) = condition_result {
                        let cond_res = self.resolve_var_name(cond_result);
                        // Compile condition instructions
                        for instr in condition_instrs {
                            output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                        }
                        output.push_str(&format!("{}        if (!({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))) continue;\n", 
                            ind, cond_res, cond_res, cond_res, cond_res));
                    }
                }
                
                // Compute key - execute stored instructions
                for instr in key_instrs {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                }
                
                // Compute value - execute stored instructions
                for instr in value_instrs {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                }
                
                // Add to dict
                output.push_str(&format!("{}        if ({}.type == 2) {{\n", ind, resolved_key));
                output.push_str(&format!("{}            tauraro_dict_set({}, {}.value.s, {});\n", ind, dc_dict, resolved_key, resolved_val));
                output.push_str(&format!("{}        }}\n", ind));
                
                output.push_str(&format!("{}    }}\n", ind));
                output.push_str(&format!("{}}}\n", ind));
                
                // Store result
                output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = {}}};\n", ind, resolved_result, dc_dict));
                self.var_types.insert(resolved_result, NativeType::Dict);
            }
            
            // Slicing - arr[start:stop:step]
            IRInstruction::Slice { object, start, stop, step, result } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_object = self.resolve_var_name(object);
                
                // Extract integer values from TauValue for slice parameters
                let start_val = start.as_ref()
                    .map(|s| format!("tauraro_raw_int({})", self.resolve_var_name(s)))
                    .unwrap_or("0".to_string());
                let stop_val = stop.as_ref()
                    .map(|s| format!("tauraro_raw_int({})", self.resolve_var_name(s)));
                let step_val = step.as_ref()
                    .map(|s| format!("tauraro_raw_int({})", self.resolve_var_name(s)))
                    .unwrap_or("1".to_string());
                
                // For stop, if not provided, use the object's length
                let stop_expr = stop_val.unwrap_or(format!(
                    "({}.type == 4 && {}.value.list ? (long long){}.value.list->size : ({}.type == 2 && {}.value.s ? (long long)strlen({}.value.s) : 0))",
                    resolved_object, resolved_object, resolved_object, resolved_object, resolved_object, resolved_object));
                
                output.push_str(&format!("{}// Slice operation\n", ind));
                output.push_str(&format!("{}{} = tauraro_slice({}, {}, {}, {});\n", 
                    ind, resolved_result, resolved_object, 
                    start_val, stop_expr, step_val));
                self.var_types.insert(resolved_result, NativeType::List);
            }
            
            // Tuple creation
            IRInstruction::TupleCreate { elements, result } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_elements: Vec<String> = elements.iter().map(|e| self.resolve_var_name(e)).collect();
                
                output.push_str(&format!("{}// Tuple creation\n", ind));
                output.push_str(&format!("{}TauList* _tuple_{} = tauraro_create_list({});\n", ind, self.temp_var_counter, elements.len()));
                self.temp_var_counter += 1;
                let tuple_var = format!("_tuple_{}", self.temp_var_counter - 1);
                
                for elem in &resolved_elements {
                    output.push_str(&format!("{}tauraro_list_append({}, {});\n", ind, tuple_var, elem));
                }
                
                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}}};\n", ind, resolved_result, tuple_var));
                self.var_types.insert(resolved_result, NativeType::List);
            }
            
            // Tuple unpacking
            IRInstruction::TupleUnpack { tuple, targets } => {
                let resolved_tuple = self.resolve_var_name(tuple);
                
                output.push_str(&format!("{}// Tuple unpacking\n", ind));
                
                // First declare all target variables as TauValue
                for target in targets {
                    let resolved_target = self.resolve_var_name(target);
                    output.push_str(&format!("{}TauValue {};\n", ind, resolved_target));
                    self.var_types.insert(resolved_target.clone(), NativeType::Generic);
                }
                
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL) {{\n", ind, resolved_tuple, resolved_tuple));
                
                for (i, target) in targets.iter().enumerate() {
                    let resolved_target = self.resolve_var_name(target);
                    output.push_str(&format!("{}    {} = ({}.value.list->size > {}) ? {}.value.list->items[{}] : (TauValue){{.type = 0, .value.i = 0}};\n", 
                        ind, resolved_target, resolved_tuple, i, resolved_tuple, i));
                }
                
                output.push_str(&format!("{}}}\n", ind));
            }
            
            // Tuple get item
            IRInstruction::TupleGetItem { tuple, index, result } => {
                let resolved_tuple = self.resolve_var_name(tuple);
                let resolved_result = self.resolve_var_name(result);
                
                output.push_str(&format!("{}// Tuple get item [{}]\n", ind, index));
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL && {}.value.list->size > {}) {{\n", 
                    ind, resolved_tuple, resolved_tuple, resolved_tuple, index));
                output.push_str(&format!("{}    {} = {}.value.list->items[{}];\n", 
                    ind, resolved_result, resolved_tuple, index));
                output.push_str(&format!("{}}} else {{\n", ind));
                output.push_str(&format!("{}    {} = (TauValue){{.type = 0, .value.i = 0}};\n", ind, resolved_result));
                output.push_str(&format!("{}}}\n", ind));
                self.var_types.insert(resolved_result, NativeType::Generic);
            }
            
            // F-string / Format string
            IRInstruction::FormatString { parts, result } => {
                let resolved_result = self.resolve_var_name(result);
                
                output.push_str(&format!("{}// F-string formatting\n", ind));
                output.push_str(&format!("{}char _fstr_buf_{}[4096] = {{0}};\n", ind, self.temp_var_counter));
                self.temp_var_counter += 1;
                let buf_name = format!("_fstr_buf_{}", self.temp_var_counter - 1);
                
                for part in parts {
                    match part {
                        crate::ir::IRFormatPart::Literal(s) => {
                            let escaped = s.replace("\\", "\\\\").replace("\"", "\\\"").replace("\n", "\\n");
                            output.push_str(&format!("{}strcat({}, \"{}\");\n", ind, buf_name, escaped));
                        }
                        crate::ir::IRFormatPart::Expression { var, format_spec } => {
                            let resolved_var = self.resolve_var_name(var);
                            output.push_str(&format!("{}{{ char _fmt_tmp[256];\n", ind));
                            
                            // Format based on type
                            output.push_str(&format!("{}  if ({}.type == 0) snprintf(_fmt_tmp, 256, \"%lld\", {}.value.i);\n", ind, resolved_var, resolved_var));
                            output.push_str(&format!("{}  else if ({}.type == 1) snprintf(_fmt_tmp, 256, \"{}\", {}.value.f);\n", 
                                ind, resolved_var, format_spec.as_ref().map(|s| s.as_str()).unwrap_or("%g"), resolved_var));
                            output.push_str(&format!("{}  else if ({}.type == 2) snprintf(_fmt_tmp, 256, \"%s\", {}.value.s);\n", ind, resolved_var, resolved_var));
                            output.push_str(&format!("{}  else if ({}.type == 3) snprintf(_fmt_tmp, 256, \"%s\", {}.value.i ? \"True\" : \"False\");\n", ind, resolved_var, resolved_var));
                            output.push_str(&format!("{}  else snprintf(_fmt_tmp, 256, \"<object>\");\n", ind));
                            output.push_str(&format!("{}  strcat({}, _fmt_tmp); }}\n", ind, buf_name));
                        }
                    }
                }
                
                output.push_str(&format!("{}{} = (TauValue){{.type = 2, .value.s = strdup({}), .refcount = 1}};\n", ind, resolved_result, buf_name));
                self.var_types.insert(resolved_result, NativeType::CStr);
            }
            
            // Context manager (with statement)
            IRInstruction::With { context_expr, alias, body } => {
                let resolved_context = self.resolve_var_name(context_expr);
                
                output.push_str(&format!("{}// With statement (context manager)\n", ind));
                output.push_str(&format!("{}{{ // Context manager scope\n", ind));
                output.push_str(&format!("{}    TauValue _ctx_{} = {};\n", ind, self.temp_var_counter, resolved_context));
                self.temp_var_counter += 1;
                let ctx_var = format!("_ctx_{}", self.temp_var_counter - 1);
                
                // Call __enter__
                output.push_str(&format!("{}    TauValue _ctx_val = tauraro_context_enter({});\n", ind, ctx_var));
                
                // Assign to alias if present
                if let Some(alias_name) = alias {
                    let resolved_alias = self.resolve_var_name(alias_name);
                    output.push_str(&format!("{}    {} = _ctx_val;\n", ind, resolved_alias));
                }
                
                // Execute body
                for body_instr in body {
                    output.push_str(&self.transpile_instruction(body_instr, indent_level + 1)?);
                }
                
                // Call __exit__
                output.push_str(&format!("{}    tauraro_context_exit({});\n", ind, ctx_var));
                output.push_str(&format!("{}}} // End context manager\n", ind));
            }
            
            // Yield (for generators)
            IRInstruction::Yield { value } => {
                output.push_str(&format!("{}// Yield (generator)\n", ind));
                if let Some(val) = value {
                    let resolved_val = self.resolve_var_name(val);
                    output.push_str(&format!("{}_gen_yield_value = {};\n", ind, resolved_val));
                }
                output.push_str(&format!("{}_gen_state++; return _gen_yield_value;\n", ind));
            }
            
            IRInstruction::YieldFrom { iterable } => {
                let resolved_iter = self.resolve_var_name(iterable);
                output.push_str(&format!("{}// Yield from\n", ind));
                output.push_str(&format!("{}for (size_t _yf_i = 0; _yf_i < {}.value.list->size; _yf_i++) {{\n", ind, resolved_iter));
                output.push_str(&format!("{}    _gen_yield_value = {}.value.list->items[_yf_i];\n", ind, resolved_iter));
                output.push_str(&format!("{}    _gen_state++; return _gen_yield_value;\n", ind));
                output.push_str(&format!("{}}}\n", ind));
            }
            
            // Match statement (pattern matching)
            IRInstruction::Match { value, cases, result } => {
                let resolved_value = self.resolve_var_name(value);
                let result_var = result.as_ref().map(|r| self.resolve_var_name(r));
                
                output.push_str(&format!("{}// Match statement\n", ind));
                output.push_str(&format!("{}do {{ // Match block\n", ind));
                
                for (i, case) in cases.iter().enumerate() {
                    let case_label = format!("_match_case_{}", i);
                    
                    // Generate pattern matching condition
                    let cond = self.generate_pattern_condition(&resolved_value, &case.pattern);
                    
                    if i == 0 {
                        output.push_str(&format!("{}    if ({}) {{\n", ind, cond));
                    } else {
                        output.push_str(&format!("{}    }} else if ({}) {{\n", ind, cond));
                    }
                    
                    // Optional guard
                    if let Some(guard) = &case.guard {
                        let resolved_guard = self.resolve_var_name(guard);
                        output.push_str(&format!("{}        if (!({}.value.i)) goto {};\n", ind, resolved_guard, case_label));
                    }
                    
                    // Execute case body
                    for body_instr in &case.body {
                        output.push_str(&self.transpile_instruction(body_instr, indent_level + 2)?);
                    }
                    
                    output.push_str(&format!("{}        break; // Exit match\n", ind));
                    output.push_str(&format!("{}        {}: ;\n", ind, case_label));
                }
                
                output.push_str(&format!("{}    }}\n", ind));
                output.push_str(&format!("{}}} while(0);\n", ind));
                
                if let Some(res) = result_var {
                    self.var_types.insert(res, NativeType::Generic);
                }
            }
            
            // Pack arguments into tuple (*args)
            IRInstruction::PackArgs { args, result } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_args: Vec<String> = args.iter().map(|a| self.resolve_var_name(a)).collect();
                
                output.push_str(&format!("{}// Pack *args\n", ind));
                output.push_str(&format!("{}TauList* _args_{} = tauraro_create_list({});\n", ind, self.temp_var_counter, args.len()));
                self.temp_var_counter += 1;
                let args_list = format!("_args_{}", self.temp_var_counter - 1);
                
                for arg in &resolved_args {
                    output.push_str(&format!("{}tauraro_list_append({}, {});\n", ind, args_list, arg));
                }
                
                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = {}}};\n", ind, resolved_result, args_list));
                self.var_types.insert(resolved_result, NativeType::List);
            }
            
            // Unpack arguments from tuple
            IRInstruction::UnpackArgs { args, targets } => {
                let resolved_args = self.resolve_var_name(args);
                
                output.push_str(&format!("{}// Unpack *args\n", ind));
                for (i, target) in targets.iter().enumerate() {
                    let resolved_target = self.resolve_var_name(target);
                    output.push_str(&format!("{}{} = tauraro_list_get({}.value.list, {});\n", ind, resolved_target, resolved_args, i));
                    self.var_types.insert(resolved_target, NativeType::Generic);
                }
            }
            
            // Pack keyword arguments into dict (**kwargs)
            IRInstruction::PackKwargs { pairs, result } => {
                let resolved_result = self.resolve_var_name(result);
                
                output.push_str(&format!("{}// Pack **kwargs\n", ind));
                output.push_str(&format!("{}TauDict* _kwargs_{} = tauraro_create_dict();\n", ind, self.temp_var_counter));
                self.temp_var_counter += 1;
                let kwargs_dict = format!("_kwargs_{}", self.temp_var_counter - 1);
                
                for (key, value) in pairs {
                    let resolved_value = self.resolve_var_name(value);
                    output.push_str(&format!("{}tauraro_dict_set({}, \"{}\", {});\n", ind, kwargs_dict, key, resolved_value));
                }
                
                output.push_str(&format!("{}{} = (TauValue){{.type = 5, .value.dict = {}}};\n", ind, resolved_result, kwargs_dict));
                self.var_types.insert(resolved_result, NativeType::Dict);
            }
            
            // Unpack keyword arguments from dict
            IRInstruction::UnpackKwargs { kwargs, targets } => {
                let resolved_kwargs = self.resolve_var_name(kwargs);
                
                output.push_str(&format!("{}// Unpack **kwargs\n", ind));
                for target in targets {
                    let resolved_target = self.resolve_var_name(target);
                    output.push_str(&format!("{}{{ TauValue* _kw_ptr = tauraro_dict_get({}.value.dict, \"{}\");\n", ind, resolved_kwargs, target));
                    output.push_str(&format!("{}  {} = _kw_ptr ? *_kw_ptr : (TauValue){{.type = 0, .value.i = 0}}; }}\n", ind, resolved_target));
                    self.var_types.insert(resolved_target, NativeType::Generic);
                }
            }

            _ => {
                output.push_str(&format!("{}// Unhandled instruction: {:?}\n", ind, instr));
            }
        }

        Ok(output)
    }
    
    /// Generate pattern matching condition for match statement
    fn generate_pattern_condition(&self, value: &str, pattern: &crate::ir::MatchPattern) -> String {
        match pattern {
            crate::ir::MatchPattern::Literal(lit) => {
                match lit {
                    Value::Int(i) => format!("({}.type == 0 && {}.value.i == {})", value, value, i),
                    Value::Float(f) => format!("({}.type == 1 && {}.value.f == {})", value, value, f),
                    Value::Str(s) => format!("({}.type == 2 && strcmp({}.value.s, \"{}\") == 0)", value, value, s),
                    Value::Bool(b) => format!("({}.type == 3 && {}.value.i == {})", value, value, if *b { 1 } else { 0 }),
                    Value::None => format!("({}.type == 0 && {}.value.i == 0)", value, value),
                    _ => "1".to_string(),
                }
            }
            crate::ir::MatchPattern::Variable(_) => "1".to_string(), // Always matches, binds value
            crate::ir::MatchPattern::Wildcard => "1".to_string(), // Always matches
            crate::ir::MatchPattern::Tuple(patterns) => {
                let conditions: Vec<String> = patterns.iter().enumerate()
                    .map(|(i, p)| {
                        let elem = format!("{}.value.list->items[{}]", value, i);
                        self.generate_pattern_condition(&elem, p)
                    })
                    .collect();
                format!("({}.type == 4 && {}.value.list->size >= {} && {})", 
                    value, value, patterns.len(), conditions.join(" && "))
            }
            crate::ir::MatchPattern::List(patterns) => {
                let conditions: Vec<String> = patterns.iter().enumerate()
                    .map(|(i, p)| {
                        let elem = format!("{}.value.list->items[{}]", value, i);
                        self.generate_pattern_condition(&elem, p)
                    })
                    .collect();
                format!("({}.type == 4 && {}.value.list->size == {} && {})", 
                    value, value, patterns.len(), conditions.join(" && "))
            }
            crate::ir::MatchPattern::Dict(pairs) => {
                let conditions: Vec<String> = pairs.iter()
                    .map(|(key, p)| {
                        let val = format!("(*tauraro_dict_get({}.value.dict, \"{}\"))", value, key);
                        format!("(tauraro_dict_get({}.value.dict, \"{}\") != NULL && {})", 
                            value, key, self.generate_pattern_condition(&val, p))
                    })
                    .collect();
                format!("({}.type == 5 && {})", value, conditions.join(" && "))
            }
            crate::ir::MatchPattern::Or(patterns) => {
                let conditions: Vec<String> = patterns.iter()
                    .map(|p| self.generate_pattern_condition(value, p))
                    .collect();
                format!("({})", conditions.join(" || "))
            }
            crate::ir::MatchPattern::Class { name, patterns: _ } => {
                format!("({}.type == 6 && {}.value.obj && strcmp({}.value.obj->class_name, \"{}\") == 0)", 
                    value, value, value, name)
            }
        }
    }

    fn format_value(&self, value: &Value) -> (NativeType, String) {
        match value {
            Value::Int(i) => (NativeType::Int64, i.to_string()),
            Value::Float(f) => (NativeType::Double, f.to_string()),
            Value::Str(s) => (NativeType::CStr, format!("\"{}\"", s.replace("\"", "\\\""))),
            Value::Bool(b) => (NativeType::Bool, if *b { "1" } else { "0" }.to_string()),
            Value::None => (NativeType::Generic, "NULL".to_string()),
            _ => (NativeType::Generic, "NULL".to_string()),
        }
    }

    fn wrap_value_in_tauvalue(&self, value: &Value) -> String {
        match value {
            Value::Int(i) => format!("(TauValue){{.type = 0, .value.i = {}}}", i),
            Value::Float(f) => format!("(TauValue){{.type = 1, .value.f = {}}}", f),
            Value::Str(s) => {
                let escaped = s
                    .replace("\\", "\\\\")  // Escape backslashes first
                    .replace("\"", "\\\"")  // Escape quotes
                    .replace("\n", "\\n")   // Escape newlines
                    .replace("\r", "\\r")   // Escape carriage returns
                    .replace("\t", "\\t");  // Escape tabs
                format!("(TauValue){{.type = 2, .value.s = strdup(\"{}\"), .refcount = 1}}", escaped)
            },
            Value::Bool(b) => format!("(TauValue){{.type = 3, .value.i = {}}}", if *b { "1" } else { "0" }),
            Value::None => "(TauValue){.type = 0, .value.i = 0}".to_string(),
            _ => "(TauValue){.type = 0, .value.i = 0}".to_string(),
        }
    }

    fn format_type(&self, native_type: NativeType) -> &'static str {
        match native_type {
            NativeType::Int64 => "long long",
            NativeType::Double => "double", 
            NativeType::CStr => "const char*",
            NativeType::Bool => "int",
            NativeType::Object => "TauObject*",
            NativeType::Dict => "TauDict*",
            NativeType::List => "TauList*",
            NativeType::Function => "TauFunction*",
            NativeType::Iterator => "TauIterator*",
            NativeType::Exception => "TauException*",
            NativeType::Module => "TauModule*",
            NativeType::Generic => "TauValue",
        }
    }

    fn format_binary_op(&self, op: BinaryOp) -> &'static str {
        match op {
            BinaryOp::Add => "+",
            BinaryOp::Sub => "-",
            BinaryOp::Mul => "*",
            BinaryOp::Div => "/",
            BinaryOp::FloorDiv => "/", // Integer division in C (for int types)
            BinaryOp::Mod => "%",
            BinaryOp::Pow => "**POWER**", // Special marker for power operations
            BinaryOp::LShift => "<<",
            BinaryOp::RShift => ">>",
            BinaryOp::BitOr => "|",
            BinaryOp::BitXor => "^",
            BinaryOp::BitAnd => "&",
            BinaryOp::Eq => "==",
            BinaryOp::Ne | BinaryOp::Neq => "!=",
            BinaryOp::Lt => "<",
            BinaryOp::Le | BinaryOp::Lte => "<=",
            BinaryOp::Gt => ">",
            BinaryOp::Ge | BinaryOp::Gte => ">=",
            BinaryOp::And => "&&",
            BinaryOp::Or => "||",
            _ => "?",
        }
    }

    fn infer_binary_op_type(&self, left: NativeType, _right: NativeType, op: BinaryOp) -> NativeType {
        match op {
            BinaryOp::Eq | BinaryOp::Ne | BinaryOp::Neq |
            BinaryOp::Lt | BinaryOp::Le | BinaryOp::Lte |
            BinaryOp::Gt | BinaryOp::Ge | BinaryOp::Gte |
            BinaryOp::And | BinaryOp::Or => NativeType::Bool,
            _ => left,
        }
    }

    fn extract_value(&self, var_name: &str, var_type: NativeType) -> String {
        // Check if this variable is declared as a native type (not TauValue)
        // If it's a native type variable, we don't need to extract from .value
        let is_native_variable = self.scope_variables.get(var_name)
            .map(|t| !matches!(t, NativeType::Generic))
            .unwrap_or(false);
        
        // Also check function parameters which are native typed
        let is_native_param = self.function_parameters.contains(var_name) && 
            self.scope_variables.get(var_name).map(|t| !matches!(t, NativeType::Generic)).unwrap_or(false);
        
        if is_native_variable || is_native_param {
            // Variable is already a native type, return it directly
            return var_name.to_string();
        }
        
        // Otherwise, extract from TauValue
        match var_type {
            NativeType::Int64 | NativeType::Bool | NativeType::Generic => format!("{}.value.i", var_name),
            NativeType::Double => format!("{}.value.f", var_name),
            NativeType::CStr => format!("{}.value.s", var_name),
            // For complex types, return the variable name (TauValue)
            _ => var_name.to_string(),
        }
    }

    fn get_c_type(&self, native_type: NativeType) -> &'static str {
        match native_type {
            NativeType::Int64 => "long long",
            NativeType::Double => "double",
            NativeType::CStr => "const char*",
            NativeType::Bool => "int",
            NativeType::Object => "TauObject*",
            NativeType::Dict => "TauDict*",
            NativeType::List => "TauList*",
            NativeType::Function => "TauFunction*",
            NativeType::Iterator => "TauIterator*",
            NativeType::Exception => "TauException*",
            NativeType::Module => "TauModule*",
            NativeType::Generic => "TauValue",
        }
    }

    /// Convert AST Type to NativeType for static typing optimization
    /// This enables direct C types instead of TauValue wrapper when types are known
    fn ast_type_to_native(&self, ast_type: &Type) -> NativeType {
        match ast_type {
            Type::Simple(name) => {
                match name.as_str() {
                    "int" | "Int" | "int64" | "i64" | "i32" | "i16" | "i8" => NativeType::Int64,
                    "float" | "Float" | "double" | "f64" | "f32" => NativeType::Double,
                    "str" | "Str" | "string" | "String" => NativeType::CStr,
                    "bool" | "Bool" | "boolean" | "Boolean" => NativeType::Bool,
                    "list" | "List" | "array" | "Array" => NativeType::List,
                    "dict" | "Dict" | "map" | "Map" | "HashMap" => NativeType::Dict,
                    "None" | "NoneType" | "void" => NativeType::Generic, // Use Generic for None/void
                    _ => NativeType::Object, // User-defined types become objects
                }
            }
            Type::Generic { name, args: _ } => {
                // Handle generic types like list[int], dict[str, int]
                // Note: For function parameters, these should be TauList*/TauDict* for native operations
                // For local variables in main(), they are stored as TauValue
                match name.as_str() {
                    "list" | "List" | "array" | "Array" => NativeType::List,
                    "dict" | "Dict" | "map" | "Map" => NativeType::Dict,
                    "tuple" | "Tuple" => NativeType::Generic, // Tuples use generic for now
                    "set" | "Set" => NativeType::Generic,
                    "Optional" | "optional" => NativeType::Generic, // Optional uses generic
                    _ => NativeType::Object,
                }
            }
            Type::Tuple(_) => NativeType::Generic, // Tuples use generic
            Type::Union(_) => NativeType::Generic, // Union types use generic
            Type::Optional(_) => NativeType::Generic, // Optional types use generic
            Type::Function { .. } => NativeType::Function,
            Type::Any => NativeType::Generic,
            _ => NativeType::Generic,
        }
    }

    /// Get C type string for AST Type (for typed variable declarations)
    fn get_typed_c_type(&self, ast_type: &Type) -> String {
        let native_type = self.ast_type_to_native(ast_type);
        self.format_type(native_type).to_string()
    }

    /// Check if a type allows direct native C operations (vs TauValue operations)
    fn is_native_operable(&self, native_type: NativeType) -> bool {
        matches!(native_type, 
            NativeType::Int64 | 
            NativeType::Double | 
            NativeType::Bool |
            NativeType::CStr
        )
    }

    fn generate_lambda_function(&self, lambda_name: &str, params: &[String], body_instrs: &[IRInstruction], body_result_var: &str) -> Result<String> {
        let mut output = String::new();
        
        // Generate function signature
        output.push_str(&format!("TauValue {}(int argc, TauValue* argv) {{\n", lambda_name));
        
        // Generate parameter assignments from argv
        for (i, param) in params.iter().enumerate() {
            output.push_str(&format!("    TauValue {} = (argc > {}) ? argv[{}] : (TauValue){{.type = 0}};\n", 
                param, i, i));
        }
        
        // Generate lambda body
        let mut lambda_transpiler = self.clone();
        lambda_transpiler.indent_level = 1;
        lambda_transpiler.declared_vars.clear();
        lambda_transpiler.var_types.clear();
        lambda_transpiler.temp_var_counter = 0;
        lambda_transpiler.scope_variables.clear();
        
        // Pre-add parameter variables to scope
        for param in params {
            lambda_transpiler.scope_variables.insert(param.clone(), NativeType::Generic);
        }
        
        // Collect ALL variables from body (including temporary ones)
        lambda_transpiler.collect_variables(body_instrs);
        lambda_transpiler.collect_store_local_targets(body_instrs);
        
        // Declare all necessary variables (except parameters which are already declared)
        for (var_name, _var_type) in &lambda_transpiler.scope_variables {
            if !params.contains(var_name) {
                output.push_str(&format!("    TauValue {};\n", var_name));
            }
        }
        
        // Execute body instructions
        for instr in body_instrs {
            output.push_str(&lambda_transpiler.transpile_instruction(instr, 1)?);
        }
        
        // Return the result (using the body_result_var that was computed)
        output.push_str(&format!("    return {};\n", body_result_var));
        output.push_str("}\n\n");
        
        Ok(output)
    }

    fn generate_utilities(&self) -> String {
        let mut output = String::new();

        output.push_str("// ===== COMPREHENSIVE TAURARO RUNTIME UTILITIES =====\n\n");

        // ===== CORE VALUE UTILITIES =====
        output.push_str("// Core value creation utilities\n");
        output.push_str("TauValue tauraro_int(long long i) {\n");
        output.push_str("    return (TauValue){.type = 0, .value.i = i, .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_float(double f) {\n");
        output.push_str("    return (TauValue){.type = 1, .value.f = f, .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str(const char* s) {\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = strdup(s), .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_bool(int b) {\n");
        output.push_str("    return (TauValue){.type = 3, .value.i = b ? 1 : 0, .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_none() {\n");
        output.push_str("    return (TauValue){.type = -1, .value.ptr = NULL, .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        // ===== STRING CONVERSION =====
        output.push_str("// Forward declaration for recursive formatting\n");
        output.push_str("char* tauraro_format_value(TauValue val);\n\n");

        output.push_str("// Format list to string recursively\n");
        output.push_str("char* tauraro_format_list(TauList* lst) {\n");
        output.push_str("    if (!lst) return strdup(\"[]\");\n");
        output.push_str("    char* result = malloc(16384);\n");
        output.push_str("    result[0] = '[';\n");
        output.push_str("    result[1] = '\\0';\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (i > 0) strcat(result, \", \");\n");
        output.push_str("        char* item = tauraro_format_value(lst->items[i]);\n");
        output.push_str("        strcat(result, item);\n");
        output.push_str("        free(item);\n");
        output.push_str("    }\n");
        output.push_str("    strcat(result, \"]\");\n");
        output.push_str("    return result;\n");
        output.push_str("}\n\n");

        output.push_str("// Format any value to string\n");
        output.push_str("char* tauraro_format_value(TauValue val) {\n");
        output.push_str("    char buffer[512];\n");
        output.push_str("    switch(val.type) {\n");
        output.push_str("        case 0: snprintf(buffer, sizeof(buffer), \"%lld\", val.value.i); return strdup(buffer);\n");
        output.push_str("        case 1: snprintf(buffer, sizeof(buffer), \"%g\", val.value.f); return strdup(buffer);\n");
        output.push_str("        case 2: {\n");
        output.push_str("            if (!val.value.s) return strdup(\"''\");\n");
        output.push_str("            char* r = malloc(strlen(val.value.s) + 3);\n");
        output.push_str("            sprintf(r, \"'%s'\", val.value.s);\n");
        output.push_str("            return r;\n");
        output.push_str("        }\n");
        output.push_str("        case 3: return strdup(val.value.i ? \"True\" : \"False\");\n");
        output.push_str("        case 4: return tauraro_format_list(val.value.list);\n");
        output.push_str("        case 5: return strdup(\"<dict>\");\n");
        output.push_str("        case 6: return strdup(\"<object>\");\n");
        output.push_str("        case 7: return strdup(\"<function>\");\n");
        output.push_str("        case -1: return strdup(\"None\");\n");
        output.push_str("        default: return strdup(\"<unknown>\");\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("// String conversion utilities\n");
        output.push_str("TauValue tauraro_str_from_value(TauValue* val) {\n");
        output.push_str("    TauValue result = {.type = 2, .value.s = NULL, .refcount = 1};\n");
        output.push_str("    if (!val) {\n");
        output.push_str("        result.value.s = strdup(\"None\");\n");
        output.push_str("        return result;\n");
        output.push_str("    }\n");
        output.push_str("    result.value.s = tauraro_format_value(*val);\n");
        output.push_str("    return result;\n");
        output.push_str("}\n\n");

        // ===== ATTRIBUTE SYSTEM =====
        output.push_str("TauValue tauraro_get_attribute(TauObject* obj, const char* name) {\n");
        output.push_str("    if (!obj || !obj->attributes) {\n");
        output.push_str("        return (TauValue){.type = 0, .value.i = 0, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    \n");
        output.push_str("    unsigned int index = tauraro_hash(name) % obj->attributes->capacity;\n");
        output.push_str("    TauDictEntry* entry = obj->attributes->buckets[index];\n");
        output.push_str("    \n");
        output.push_str("    while (entry) {\n");
        output.push_str("        if (strcmp(entry->key, name) == 0) {\n");
        output.push_str("            return entry->value;\n");
        output.push_str("        }\n");
        output.push_str("        entry = entry->next;\n");
        output.push_str("    }\n");
        output.push_str("    \n");
        output.push_str("    return (TauValue){.type = 0, .value.i = 0, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_set_attribute(TauObject* obj, const char* name, TauValue value) {\n");
        output.push_str("    if (!obj) return;\n");
        output.push_str("    if (!obj->attributes) obj->attributes = tauraro_create_dict();\n");
        output.push_str("    tauraro_dict_set(obj->attributes, name, value);\n");
        output.push_str("}\n\n");

        // Add polymorphic method dispatch helper
        output.push_str("// Polymorphic method dispatcher - handles method dispatch by checking actual class\n");
        output.push_str("// This is used for polymorphic calls where the object type is known at runtime\n");
        output.push_str("typedef TauValue (*MethodDispatcher)(TauValue obj);\n");
        output.push_str("TauValue tauraro_dispatch_method(TauValue obj, const char* method_name) {\n");
        output.push_str("    if (obj.type != 6 || !obj.value.obj || !obj.value.obj->class_name) {\n");
        output.push_str("        return (TauValue){.type = 0, .value.i = 0, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    const char* class_name = obj.value.obj->class_name;\n");
        output.push_str("    // Dispatch to appropriate method based on class name\n");
        output.push_str("    // Format: ClassName__method_name\n");
        output.push_str("    char full_method[256];\n");
        output.push_str("    snprintf(full_method, sizeof(full_method), \"%s__%s\", class_name, method_name);\n");
        output.push_str("    // This will be filled in by the caller with appropriate function pointers\n");
        output.push_str("    return (TauValue){.type = 0, .value.i = 0, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== CLASS AND OBJECT SYSTEM =====
        output.push_str("// Advanced OOP support with inheritance\n");
        output.push_str("TauClass* tauraro_create_class(const char* name, TauClass* parent) {\n");
        output.push_str("    TauClass* cls = malloc(sizeof(TauClass));\n");
        output.push_str("    cls->name = strdup(name);\n");
        output.push_str("    cls->parent = parent;\n");
        output.push_str("    cls->methods = tauraro_create_dict();\n");
        output.push_str("    cls->static_methods = tauraro_create_dict();\n");
        output.push_str("    cls->properties = tauraro_create_dict();\n");
        output.push_str("    cls->instance_size = sizeof(TauObject);\n");
        output.push_str("    cls->refcount = 1;\n");
        output.push_str("    return cls;\n");
        output.push_str("}\n\n");

        output.push_str("TauObject* tauraro_create_object(const char* class_name) {\n");
        output.push_str("    TauObject* obj = malloc(sizeof(TauObject));\n");
        output.push_str("    obj->class_name = strdup(class_name);\n");
        output.push_str("    obj->class_ref = NULL; // Set by class system\n");
        output.push_str("    obj->attributes = tauraro_create_dict();\n");
        output.push_str("    obj->native_data = NULL;\n");
        output.push_str("    obj->refcount = 1;\n");
        output.push_str("    return obj;\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_class_add_method(TauClass* cls, const char* name, TauFunction* method) {\n");
        output.push_str("    if (!cls || !name || !method) return;\n");
        output.push_str("    TauValue method_val = {.type = 7, .value.func = method, .refcount = 1, .next = NULL};\n");
        output.push_str("    tauraro_dict_set(cls->methods, name, method_val);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_call_method(TauObject* obj, const char* method_name, int argc, TauValue* argv) {\n");
        output.push_str("    if (!obj || !method_name) return tauraro_none();\n");
        output.push_str("    TauClass* cls = obj->class_ref;\n");
        output.push_str("    while (cls) {\n");
        output.push_str("        TauValue* method_val = tauraro_dict_get(cls->methods, method_name);\n");
        output.push_str("        if (method_val && method_val->type == 7) {\n");
        output.push_str("            TauFunction* method = method_val->value.func;\n");
        output.push_str("            if (method->native_func) {\n");
        output.push_str("                return method->native_func(argc, argv);\n");
        output.push_str("            }\n");
        output.push_str("        }\n");
        output.push_str("        cls = cls->parent;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        // ===== FUNCTION AND CLOSURE SYSTEM =====
        output.push_str("// Function and closure support\n");
        output.push_str("TauFunction* tauraro_create_function(const char* name, TauNativeFunc func, int param_count) {\n");
        output.push_str("    TauFunction* f = malloc(sizeof(TauFunction));\n");
        output.push_str("    f->name = strdup(name);\n");
        output.push_str("    f->native_func = func;\n");
        output.push_str("    f->closure = NULL;\n");
        output.push_str("    f->param_count = param_count;\n");
        output.push_str("    f->param_names = NULL;\n");
        output.push_str("    f->is_native = 1;\n");
        output.push_str("    f->refcount = 1;\n");
        output.push_str("    return f;\n");
        output.push_str("}\n\n");

        output.push_str("TauClosure* tauraro_create_closure(TauFunction* func, int captured_count) {\n");
        output.push_str("    TauClosure* closure = malloc(sizeof(TauClosure));\n");
        output.push_str("    closure->captured_vars = tauraro_create_dict();\n");
        output.push_str("    closure->function = func;\n");
        output.push_str("    closure->refcount = 1;\n");
        output.push_str("    return closure;\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_closure_capture(TauClosure* closure, const char* var_name, TauValue* value) {\n");
        output.push_str("    if (!closure || !var_name || !value) return;\n");
        output.push_str("    tauraro_dict_set(closure->captured_vars, var_name, *value);\n");
        output.push_str("}\n\n");

        // ===== EXCEPTION HANDLING =====
        output.push_str("// Exception handling system\n");
        output.push_str("#include <setjmp.h>\n");
        output.push_str("jmp_buf tauraro_exception_buf;\n");
        output.push_str("TauException* tauraro_current_exception = NULL;\n\n");

        output.push_str("TauException* tauraro_create_exception(const char* type, const char* message) {\n");
        output.push_str("    TauException* exc = malloc(sizeof(TauException));\n");
        output.push_str("    exc->type = strdup(type);\n");
        output.push_str("    exc->message = strdup(message);\n");
        output.push_str("    exc->traceback = NULL;\n");
        output.push_str("    exc->value = tauraro_none();\n");
        output.push_str("    exc->refcount = 1;\n");
        output.push_str("    return exc;\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_throw_exception(TauException* exc) {\n");
        output.push_str("    tauraro_current_exception = exc;\n");
        output.push_str("    longjmp(tauraro_exception_buf, 1);\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_exception_matches(const char* type) {\n");
        output.push_str("    return tauraro_current_exception && \n");
        output.push_str("           strcmp(tauraro_current_exception->type, type) == 0;\n");
        output.push_str("}\n\n");

        // ===== SUPER CALL SUPPORT =====
        output.push_str("// Super call for inheritance\n");
        output.push_str("TauValue tauraro_super_call(TauObject* self, TauValue* args, int argc) {\n");
        output.push_str("    if (!self || !self->class_ref || !self->class_ref->parent) {\n");
        output.push_str("        return tauraro_none();\n");
        output.push_str("    }\n");
        output.push_str("    TauClass* parent = self->class_ref->parent;\n");
        output.push_str("    // Look up __init__ in parent class\n");
        output.push_str("    TauValue* init_method = tauraro_dict_get(parent->methods, \"__init__\");\n");
        output.push_str("    if (init_method && init_method->type == 7 && init_method->value.func) {\n");
        output.push_str("        return init_method->value.func->native_func(argc, args);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        // ===== TUPLE SUPPORT =====
        output.push_str("// Tuple support (immutable fixed-size array)\n");
        output.push_str("typedef struct TauTuple {\n");
        output.push_str("    TauValue* items;\n");
        output.push_str("    size_t size;\n");
        output.push_str("    int refcount;\n");
        output.push_str("} TauTuple;\n\n");

        output.push_str("TauTuple* tauraro_create_tuple(size_t size) {\n");
        output.push_str("    TauTuple* tuple = malloc(sizeof(TauTuple));\n");
        output.push_str("    tuple->items = calloc(size, sizeof(TauValue));\n");
        output.push_str("    tuple->size = size;\n");
        output.push_str("    tuple->refcount = 1;\n");
        output.push_str("    return tuple;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_tuple_get(TauTuple* tuple, int index) {\n");
        output.push_str("    if (!tuple || index < 0 || index >= (int)tuple->size) return tauraro_none();\n");
        output.push_str("    return tuple->items[index];\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_tuple_to_value(TauTuple* tuple) {\n");
        output.push_str("    return (TauValue){.type = 10, .value.ptr = tuple, .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        // ===== SET SUPPORT =====
        output.push_str("// Set support (unordered unique values)\n");
        output.push_str("typedef struct TauSet {\n");
        output.push_str("    TauDict* data; // Use dict internally with null values\n");
        output.push_str("    int refcount;\n");
        output.push_str("} TauSet;\n\n");

        output.push_str("TauSet* tauraro_create_set() {\n");
        output.push_str("    TauSet* set = malloc(sizeof(TauSet));\n");
        output.push_str("    set->data = tauraro_create_dict();\n");
        output.push_str("    set->refcount = 1;\n");
        output.push_str("    return set;\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_set_add(TauSet* set, const char* value) {\n");
        output.push_str("    if (!set || !value) return;\n");
        output.push_str("    tauraro_dict_set(set->data, value, tauraro_bool(1));\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_set_contains(TauSet* set, const char* value) {\n");
        output.push_str("    if (!set || !value) return 0;\n");
        output.push_str("    return tauraro_dict_get(set->data, value) != NULL;\n");
        output.push_str("}\n\n");

        // ===== RANGE/SLICE SUPPORT =====
        output.push_str("// Range iterator for for loops\n");
        output.push_str("typedef struct TauRange {\n");
        output.push_str("    long long start;\n");
        output.push_str("    long long stop;\n");
        output.push_str("    long long step;\n");
        output.push_str("    long long current;\n");
        output.push_str("} TauRange;\n\n");

        output.push_str("TauRange* tauraro_range(long long start, long long stop, long long step) {\n");
        output.push_str("    TauRange* r = malloc(sizeof(TauRange));\n");
        output.push_str("    r->start = start;\n");
        output.push_str("    r->stop = stop;\n");
        output.push_str("    r->step = step != 0 ? step : 1;\n");
        output.push_str("    r->current = start;\n");
        output.push_str("    return r;\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_range_has_next(TauRange* r) {\n");
        output.push_str("    if (!r) return 0;\n");
        output.push_str("    if (r->step > 0) return r->current < r->stop;\n");
        output.push_str("    return r->current > r->stop;\n");
        output.push_str("}\n\n");

        output.push_str("long long tauraro_range_next(TauRange* r) {\n");
        output.push_str("    if (!r) return 0;\n");
        output.push_str("    long long val = r->current;\n");
        output.push_str("    r->current += r->step;\n");
        output.push_str("    return val;\n");
        output.push_str("}\n\n");

        // ===== CONTEXT MANAGER SUPPORT (for with statement) =====
        output.push_str("// Context manager support (for 'with' statement)\n");
        output.push_str("typedef struct TauContextManager {\n");
        output.push_str("    TauValue value;\n");
        output.push_str("    TauNativeFunc enter_func;\n");
        output.push_str("    TauNativeFunc exit_func;\n");
        output.push_str("} TauContextManager;\n\n");

        output.push_str("TauValue tauraro_context_enter(TauContextManager* ctx) {\n");
        output.push_str("    if (ctx && ctx->enter_func) {\n");
        output.push_str("        return ctx->enter_func(1, &ctx->value);\n");
        output.push_str("    }\n");
        output.push_str("    return ctx ? ctx->value : tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_context_exit(TauContextManager* ctx, TauException* exc) {\n");
        output.push_str("    if (ctx && ctx->exit_func) {\n");
        output.push_str("        TauValue args[2] = { ctx->value, tauraro_none() };\n");
        output.push_str("        if (exc) args[1] = (TauValue){.type = 8, .value.exc = exc};\n");
        output.push_str("        ctx->exit_func(2, args);\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        // ===== ITERATOR SYSTEM =====
        output.push_str("// Iterator support\n");
        output.push_str("TauIterator* tauraro_create_iterator(TauValue* iterable) {\n");
        output.push_str("    TauIterator* iter = malloc(sizeof(TauIterator));\n");
        output.push_str("    iter->data = iterable;\n");
        output.push_str("    iter->next = NULL;\n");
        output.push_str("    iter->has_next = NULL;\n");
        output.push_str("    iter->cleanup = NULL;\n");
        output.push_str("    iter->refcount = 1;\n");
        output.push_str("    return iter;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_iterator_next(TauIterator* iter) {\n");
        output.push_str("    if (!iter || !iter->next) return tauraro_none();\n");
        output.push_str("    return iter->next(iter);\n");
        output.push_str("}\n\n");

        // ===== MODULE SYSTEM =====
        output.push_str("// Module system support\n");
        output.push_str("TauModule* tauraro_create_module(const char* name, const char* path) {\n");
        output.push_str("    TauModule* mod = malloc(sizeof(TauModule));\n");
        output.push_str("    mod->name = strdup(name);\n");
        output.push_str("    mod->path = path ? strdup(path) : NULL;\n");
        output.push_str("    mod->globals = tauraro_create_dict();\n");
        output.push_str("    mod->exports = tauraro_create_dict();\n");
        output.push_str("    mod->is_loaded = 0;\n");
        output.push_str("    mod->refcount = 1;\n");
        output.push_str("    return mod;\n");
        output.push_str("}\n\n");

        output.push_str("TauModule* tauraro_import_module(const char* name) {\n");
        output.push_str("    // Simplified import - in real implementation would load from file\n");
        output.push_str("    return tauraro_create_module(name, NULL);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_module_get(TauModule* mod, const char* name) {\n");
        output.push_str("    if (!mod || !name) return tauraro_none();\n");
        output.push_str("    TauValue* val = tauraro_dict_get(mod->exports, name);\n");
        output.push_str("    return val ? *val : tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_module_to_value(TauModule* mod) {\n");
        output.push_str("    return (TauValue){.type = 9, .value.ptr = mod, .refcount = 1, .next = NULL};\n");
        output.push_str("}\n\n");

        // Dictionary implementation
        if !self.generated_utilities.contains("dict_utils") {
            output.push_str("// Dictionary implementation\n");
            output.push_str("#define DICT_INITIAL_CAPACITY 16\n");
            output.push_str("#define DICT_LOAD_FACTOR 0.75\n\n");
            
            output.push_str("unsigned int tauraro_hash(const char* key) {\n");
            output.push_str("    unsigned int hash = 5381;\n");
            output.push_str("    int c;\n");
            output.push_str("    while ((c = *key++)) {\n");
            output.push_str("        hash = ((hash << 5) + hash) + c;\n");
            output.push_str("    }\n");
            output.push_str("    return hash;\n");
            output.push_str("}\n\n");

            output.push_str("TauDict* tauraro_create_dict() {\n");
            output.push_str("    TauDict* dict = malloc(sizeof(TauDict));\n");
            output.push_str("    if (dict) {\n");
            output.push_str("        dict->capacity = DICT_INITIAL_CAPACITY;\n");
            output.push_str("        dict->size = 0;\n");
            output.push_str("        dict->buckets = calloc(dict->capacity, sizeof(TauDictEntry*));\n");
            output.push_str("    }\n");
            output.push_str("    return dict;\n");
            output.push_str("}\n\n");
        }

        output.push_str("void tauraro_dict_set(TauDict* dict, const char* key, TauValue value) {\n");
        output.push_str("    if (!dict || !key) return;\n");
        output.push_str("    \n");
        output.push_str("    unsigned int index = tauraro_hash(key) % dict->capacity;\n");
        output.push_str("    TauDictEntry* entry = dict->buckets[index];\n");
        output.push_str("    \n");
        output.push_str("    // Search for existing key\n");
        output.push_str("    while (entry) {\n");
        output.push_str("        if (strcmp(entry->key, key) == 0) {\n");
        output.push_str("            entry->value = value;\n");
        output.push_str("            return;\n");
        output.push_str("        }\n");
        output.push_str("        entry = entry->next;\n");
        output.push_str("    }\n");
        output.push_str("    \n");
        output.push_str("    // Create new entry\n");
        output.push_str("    TauDictEntry* new_entry = malloc(sizeof(TauDictEntry));\n");
        output.push_str("    if (new_entry) {\n");
        output.push_str("        new_entry->key = strdup(key);\n");
        output.push_str("        new_entry->value = value;\n");
        output.push_str("        new_entry->next = dict->buckets[index];\n");
        output.push_str("        dict->buckets[index] = new_entry;\n");
        output.push_str("        dict->size++;\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue* tauraro_dict_get(TauDict* dict, const char* key) {\n");
        output.push_str("    if (!dict || !key) return NULL;\n");
        output.push_str("    \n");
        output.push_str("    unsigned int index = tauraro_hash(key) % dict->capacity;\n");
        output.push_str("    TauDictEntry* entry = dict->buckets[index];\n");
        output.push_str("    \n");
        output.push_str("    while (entry) {\n");
        output.push_str("        if (strcmp(entry->key, key) == 0) {\n");
        output.push_str("            return &entry->value;\n");
        output.push_str("        }\n");
        output.push_str("        entry = entry->next;\n");
        output.push_str("    }\n");
        output.push_str("    return NULL;\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_dict_len(TauDict* dict) {\n");
        output.push_str("    return dict ? dict->size : 0;\n");
        output.push_str("}\n\n");

        output.push_str("char* tauraro_dict_to_string(TauDict* dict) {\n");
        output.push_str("    if (!dict) return strdup(\"{}\");\n");
        output.push_str("    \n");
        output.push_str("    static char buffer[2048];\n");
        output.push_str("    strcpy(buffer, \"{\");\n");
        output.push_str("    \n");
        output.push_str("    int first = 1;\n");
        output.push_str("    for (int i = 0; i < dict->capacity; i++) {\n");
        output.push_str("        TauDictEntry* entry = dict->buckets[i];\n");
        output.push_str("        while (entry) {\n");
        output.push_str("            if (!first) strcat(buffer, \", \");\n");
        output.push_str("            strcat(buffer, \"'\");\n");
        output.push_str("            strcat(buffer, entry->key);\n");
        output.push_str("            strcat(buffer, \"': \");\n");
        output.push_str("            // Simple value representation\n");
        output.push_str("            char temp[64];\n");
        output.push_str("            snprintf(temp, sizeof(temp), \"%p\", entry->value);\n");
        output.push_str("            strcat(buffer, temp);\n");
        output.push_str("            first = 0;\n");
        output.push_str("            entry = entry->next;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    strcat(buffer, \"}\");\n");
        output.push_str("    return buffer;\n");
        output.push_str("}\n\n");

        // Built-in functions for enhanced functionality
        output.push_str("// Enhanced built-in functions\n");
        output.push_str("TauValue tauraro_str_int(long long val) {\n");
        output.push_str("    static char buffer[32];\n");
        output.push_str("    sprintf(buffer, \"%lld\", val);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_double(double val) {\n");
        output.push_str("    static char buffer[32];\n");
        output.push_str("    sprintf(buffer, \"%f\", val);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_int_string(const char* str) {\n");
        output.push_str("    long long val = strtoll(str, NULL, 10);\n");
        output.push_str("    return (TauValue){.type = 0, .value.i = val, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_float_string(const char* str) {\n");
        output.push_str("    double val = strtod(str, NULL);\n");
        output.push_str("    return (TauValue){.type = 1, .value.f = val, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_len(TauList* list) {\n");
        output.push_str("    if (!list) return (TauValue){.type = 0, .value.i = 0, .refcount = 1};\n");
        output.push_str("    return (TauValue){.type = 0, .value.i = (long long)list->size, .refcount = 1};\n");
        output.push_str("}\n\n");

        // List append method
        output.push_str("TauValue lst__append(TauValue lst, TauValue item) {\n");
        output.push_str("    if (lst.type == 4 && lst.value.list) {\n");
        output.push_str("        TauList* list = lst.value.list;\n");
        output.push_str("        if (list->size >= list->capacity) {\n");
        output.push_str("            list->capacity = (list->capacity + 1) * 2;\n");
        output.push_str("            list->items = realloc(list->items, sizeof(TauValue) * list->capacity);\n");
        output.push_str("        }\n");
        output.push_str("        list->items[list->size++] = item;\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 0, .value.i = 0, .refcount = 1}; // None\n");
        output.push_str("}\n\n");

        // String upper method
        output.push_str("TauValue text__upper(TauValue str) {\n");
        output.push_str("    if (str.type == 2 && str.value.s) {\n");
        output.push_str("        char* result = strdup(str.value.s);\n");
        output.push_str("        for (int i = 0; result[i]; i++) {\n");
        output.push_str("            if (result[i] >= 'a' && result[i] <= 'z') {\n");
        output.push_str("                result[i] = result[i] - 32;\n");
        output.push_str("            }\n");
        output.push_str("        }\n");
        output.push_str("        return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    return str;\n");
        output.push_str("}\n\n");

        // String lower method
        output.push_str("TauValue text__lower(TauValue str) {\n");
        output.push_str("    if (str.type == 2 && str.value.s) {\n");
        output.push_str("        char* result = strdup(str.value.s);\n");
        output.push_str("        for (int i = 0; result[i]; i++) {\n");
        output.push_str("            if (result[i] >= 'A' && result[i] <= 'Z') {\n");
        output.push_str("                result[i] = result[i] + 32;\n");
        output.push_str("            }\n");
        output.push_str("        }\n");
        output.push_str("        return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    return str;\n");
        output.push_str("}\n\n");

        // Range function
        output.push_str("TauValue range(TauValue end) {\n");
        output.push_str("    long long n = 0;\n");
        output.push_str("    if (end.type == 0) n = end.value.i;\n");
        output.push_str("    TauList* list = malloc(sizeof(TauList));\n");
        output.push_str("    list->size = n;\n");
        output.push_str("    list->capacity = n;\n");
        output.push_str("    list->items = malloc(sizeof(TauValue) * n);\n");
        output.push_str("    for (long long i = 0; i < n; i++) {\n");
        output.push_str("        list->items[i] = (TauValue){.type = 0, .value.i = i, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = list, .refcount = 1};\n");
        output.push_str("}\n\n");

        // Range with start and end
        output.push_str("TauValue range2(TauValue start, TauValue end) {\n");
        output.push_str("    long long s = 0, e = 0;\n");
        output.push_str("    if (start.type == 0) s = start.value.i;\n");
        output.push_str("    if (end.type == 0) e = end.value.i;\n");
        output.push_str("    long long n = e > s ? e - s : 0;\n");
        output.push_str("    TauList* list = malloc(sizeof(TauList));\n");
        output.push_str("    list->size = n;\n");
        output.push_str("    list->capacity = n;\n");
        output.push_str("    list->items = malloc(sizeof(TauValue) * n);\n");
        output.push_str("    for (long long i = 0; i < n; i++) {\n");
        output.push_str("        list->items[i] = (TauValue){.type = 0, .value.i = s + i, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = list, .refcount = 1};\n");
        output.push_str("}\n\n");

        // Range with start, end, and step
        output.push_str("TauValue range3(TauValue start, TauValue end, TauValue step) {\n");
        output.push_str("    long long s = 0, e = 0, st = 1;\n");
        output.push_str("    if (start.type == 0) s = start.value.i;\n");
        output.push_str("    if (end.type == 0) e = end.value.i;\n");
        output.push_str("    if (step.type == 0 && step.value.i != 0) st = step.value.i;\n");
        output.push_str("    long long n = 0;\n");
        output.push_str("    if (st > 0 && e > s) n = (e - s + st - 1) / st;\n");
        output.push_str("    else if (st < 0 && s > e) n = (s - e - st - 1) / (-st);\n");
        output.push_str("    if (n < 0) n = 0;\n");
        output.push_str("    TauList* list = malloc(sizeof(TauList));\n");
        output.push_str("    list->size = n;\n");
        output.push_str("    list->capacity = n;\n");
        output.push_str("    list->items = malloc(sizeof(TauValue) * n);\n");
        output.push_str("    for (long long i = 0; i < n; i++) {\n");
        output.push_str("        list->items[i] = (TauValue){.type = 0, .value.i = s + i * st, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = list, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== LIST UTILITIES =====
        output.push_str("// List utility functions\n");
        output.push_str("TauList* tauraro_create_list(size_t initial_capacity) {\n");
        output.push_str("    TauList* list = malloc(sizeof(TauList));\n");
        output.push_str("    list->size = 0;\n");
        output.push_str("    list->capacity = initial_capacity > 0 ? initial_capacity : 8;\n");
        output.push_str("    list->items = malloc(sizeof(TauValue) * list->capacity);\n");
        output.push_str("    list->refcount = 1;\n");
        output.push_str("    return list;\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_list_append(TauList* list, TauValue item) {\n");
        output.push_str("    if (!list) return;\n");
        output.push_str("    if (list->size >= list->capacity) {\n");
        output.push_str("        list->capacity = (list->capacity + 1) * 2;\n");
        output.push_str("        list->items = realloc(list->items, sizeof(TauValue) * list->capacity);\n");
        output.push_str("    }\n");
        output.push_str("    list->items[list->size++] = item;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_get(TauList* list, long long index) {\n");
        output.push_str("    if (!list) return tauraro_none();\n");
        output.push_str("    if (index < 0) index = list->size + index; // Negative indexing\n");
        output.push_str("    if (index < 0 || index >= (long long)list->size) return tauraro_none();\n");
        output.push_str("    return list->items[index];\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_list_set(TauList* list, long long index, TauValue item) {\n");
        output.push_str("    if (!list) return;\n");
        output.push_str("    if (index < 0) index = list->size + index;\n");
        output.push_str("    if (index < 0 || index >= (long long)list->size) return;\n");
        output.push_str("    list->items[index] = item;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_pop(TauList* list) {\n");
        output.push_str("    if (!list || list->size == 0) return tauraro_none();\n");
        output.push_str("    return list->items[--list->size];\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_extend(TauList* list, TauList* other) {\n");
        output.push_str("    if (!list || !other) return tauraro_none();\n");
        output.push_str("    for (size_t i = 0; i < other->size; i++) {\n");
        output.push_str("        tauraro_list_append(list, other->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_list_contains(TauList* list, TauValue item) {\n");
        output.push_str("    if (!list) return 0;\n");
        output.push_str("    for (size_t i = 0; i < list->size; i++) {\n");
        output.push_str("        if (list->items[i].type == item.type) {\n");
        output.push_str("            if (item.type == 0 && list->items[i].value.i == item.value.i) return 1;\n");
        output.push_str("            if (item.type == 2 && strcmp(list->items[i].value.s, item.value.s) == 0) return 1;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return 0;\n");
        output.push_str("}\n\n");

        // ===== STRING UTILITIES =====
        output.push_str("// String utility functions\n");
        output.push_str("TauValue text__strip(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return str;\n");
        output.push_str("    char* s = str.value.s;\n");
        output.push_str("    while (*s == ' ' || *s == '\\t' || *s == '\\n') s++;\n");
        output.push_str("    char* result = strdup(s);\n");
        output.push_str("    size_t len = strlen(result);\n");
        output.push_str("    while (len > 0 && (result[len-1] == ' ' || result[len-1] == '\\t' || result[len-1] == '\\n')) {\n");
        output.push_str("        result[--len] = '\\0';\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue text__split(TauValue str, TauValue delim) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_none();\n");
        output.push_str("    char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : \" \";\n");
        output.push_str("    TauList* result = tauraro_create_list(8);\n");
        output.push_str("    char* s = strdup(str.value.s);\n");
        output.push_str("    char* token = strtok(s, d);\n");
        output.push_str("    while (token) {\n");
        output.push_str("        tauraro_list_append(result, tauraro_str(token));\n");
        output.push_str("        token = strtok(NULL, d);\n");
        output.push_str("    }\n");
        output.push_str("    free(s);\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue text__join(TauValue delim, TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_str(\"\");\n");
        output.push_str("    char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : \"\";\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    if (lst->size == 0) return tauraro_str(\"\");\n");
        output.push_str("    size_t total_len = 0;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (lst->items[i].type == 2 && lst->items[i].value.s) {\n");
        output.push_str("            total_len += strlen(lst->items[i].value.s);\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    total_len += strlen(d) * (lst->size - 1) + 1;\n");
        output.push_str("    char* result = malloc(total_len);\n");
        output.push_str("    result[0] = '\\0';\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (i > 0) strcat(result, d);\n");
        output.push_str("        if (lst->items[i].type == 2 && lst->items[i].value.s) {\n");
        output.push_str("            strcat(result, lst->items[i].value.s);\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue text__replace(TauValue str, TauValue old_s, TauValue new_s) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return str;\n");
        output.push_str("    if (old_s.type != 2 || !old_s.value.s) return str;\n");
        output.push_str("    char* olds = old_s.value.s;\n");
        output.push_str("    char* news = (new_s.type == 2 && new_s.value.s) ? new_s.value.s : \"\";\n");
        output.push_str("    char* src = str.value.s;\n");
        output.push_str("    size_t old_len = strlen(olds);\n");
        output.push_str("    if (old_len == 0) return str;\n");
        output.push_str("    size_t count = 0;\n");
        output.push_str("    char* p = src;\n");
        output.push_str("    while ((p = strstr(p, olds))) { count++; p += old_len; }\n");
        output.push_str("    size_t new_len = strlen(news);\n");
        output.push_str("    size_t result_len = strlen(src) + count * (new_len - old_len) + 1;\n");
        output.push_str("    char* result = malloc(result_len);\n");
        output.push_str("    result[0] = '\\0';\n");
        output.push_str("    p = src;\n");
        output.push_str("    char* r = result;\n");
        output.push_str("    while (*p) {\n");
        output.push_str("        if (strncmp(p, olds, old_len) == 0) {\n");
        output.push_str("            strcpy(r, news);\n");
        output.push_str("            r += new_len;\n");
        output.push_str("            p += old_len;\n");
        output.push_str("        } else {\n");
        output.push_str("            *r++ = *p++;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    *r = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue text__startswith(TauValue str, TauValue prefix) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || prefix.type != 2 || !prefix.value.s) {\n");
        output.push_str("        return tauraro_bool(0);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_bool(strncmp(str.value.s, prefix.value.s, strlen(prefix.value.s)) == 0);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue text__endswith(TauValue str, TauValue suffix) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || suffix.type != 2 || !suffix.value.s) {\n");
        output.push_str("        return tauraro_bool(0);\n");
        output.push_str("    }\n");
        output.push_str("    size_t str_len = strlen(str.value.s);\n");
        output.push_str("    size_t suf_len = strlen(suffix.value.s);\n");
        output.push_str("    if (suf_len > str_len) return tauraro_bool(0);\n");
        output.push_str("    return tauraro_bool(strcmp(str.value.s + str_len - suf_len, suffix.value.s) == 0);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue text__find(TauValue str, TauValue substr) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || substr.type != 2 || !substr.value.s) {\n");
        output.push_str("        return tauraro_int(-1);\n");
        output.push_str("    }\n");
        output.push_str("    char* p = strstr(str.value.s, substr.value.s);\n");
        output.push_str("    if (!p) return tauraro_int(-1);\n");
        output.push_str("    return tauraro_int((long long)(p - str.value.s));\n");
        output.push_str("}\n\n");

        // Additional text__ wrapper methods
        output.push_str("TauValue text__title(TauValue str) { return tauraro_str_title(str); }\n");
        output.push_str("TauValue text__capitalize(TauValue str) { return tauraro_str_capitalize(str); }\n");
        output.push_str("TauValue text__swapcase(TauValue str) { return tauraro_str_swapcase(str); }\n");
        output.push_str("TauValue text__lstrip(TauValue str) { return tauraro_str_lstrip(str); }\n");
        output.push_str("TauValue text__rstrip(TauValue str) { return tauraro_str_rstrip(str); }\n");
        output.push_str("TauValue text__isdigit(TauValue str) { return tauraro_str_isdigit(str); }\n");
        output.push_str("TauValue text__isalpha(TauValue str) { return tauraro_str_isalpha(str); }\n");
        output.push_str("TauValue text__isalnum(TauValue str) { return tauraro_str_isalnum(str); }\n");
        output.push_str("TauValue text__isspace(TauValue str) { return tauraro_str_isspace(str); }\n");
        output.push_str("TauValue text__isupper(TauValue str) { return tauraro_str_isupper(str); }\n");
        output.push_str("TauValue text__islower(TauValue str) { return tauraro_str_islower(str); }\n");
        output.push_str("TauValue text__count(TauValue val, TauValue sub) {\n");
        output.push_str("    if (val.type == 4 && val.value.list) { // list.count()\n");
        output.push_str("        TauList* list = val.value.list;\n");
        output.push_str("        long long cnt = 0;\n");
        output.push_str("        for (size_t i = 0; i < list->size; i++) {\n");
        output.push_str("            if (tauraro_equals(list->items[i], sub)) cnt++;\n");
        output.push_str("        }\n");
        output.push_str("        return tauraro_int(cnt);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_str_count(val, sub); // str.count()\n");
        output.push_str("}\n");
        output.push_str("TauValue text__center(TauValue str, TauValue width) { return tauraro_str_center(str, width); }\n");
        output.push_str("TauValue text__ljust(TauValue str, TauValue width) { return tauraro_str_ljust(str, width); }\n");
        output.push_str("TauValue text__rjust(TauValue str, TauValue width) { return tauraro_str_rjust(str, width); }\n");
        output.push_str("TauValue text__zfill(TauValue str, TauValue width) { return tauraro_str_zfill(str, width); }\n\n");

        // ===== LIST WRAPPER METHODS (lst__*) =====
        output.push_str("TauValue lst__pop(TauValue lst) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list || lst.value.list->size == 0) return tauraro_none();\n");
        output.push_str("    return tauraro_list_pop_v(lst);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__insert(TauValue lst, TauValue index, TauValue item) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_none();\n");
        output.push_str("    return tauraro_list_insert(lst, index, item);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__remove(TauValue lst, TauValue item) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_none();\n");
        output.push_str("    return tauraro_list_remove(lst, item);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__extend(TauValue lst, TauValue other) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_none();\n");
        output.push_str("    return tauraro_list_extend_v(lst, other);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__index(TauValue lst, TauValue item) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_int(-1);\n");
        output.push_str("    TauList* list = lst.value.list;\n");
        output.push_str("    for (size_t i = 0; i < list->size; i++) {\n");
        output.push_str("        if (tauraro_equals(list->items[i], item)) return tauraro_int((long long)i);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_int(-1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__count(TauValue lst, TauValue item) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_int(0);\n");
        output.push_str("    TauList* list = lst.value.list;\n");
        output.push_str("    long long count = 0;\n");
        output.push_str("    for (size_t i = 0; i < list->size; i++) {\n");
        output.push_str("        if (tauraro_equals(list->items[i], item)) count++;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_int(count);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__reverse(TauValue lst) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_none();\n");
        output.push_str("    TauList* list = lst.value.list;\n");
        output.push_str("    for (size_t i = 0; i < list->size / 2; i++) {\n");
        output.push_str("        TauValue tmp = list->items[i];\n");
        output.push_str("        list->items[i] = list->items[list->size - 1 - i];\n");
        output.push_str("        list->items[list->size - 1 - i] = tmp;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__sort(TauValue lst) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list || lst.value.list->size < 2) return tauraro_none();\n");
        output.push_str("    TauList* list = lst.value.list;\n");
        output.push_str("    // Simple bubble sort for now\n");
        output.push_str("    for (size_t i = 0; i < list->size - 1; i++) {\n");
        output.push_str("        for (size_t j = 0; j < list->size - 1 - i; j++) {\n");
        output.push_str("            TauValue a = list->items[j], b = list->items[j+1];\n");
        output.push_str("            int swap = 0;\n");
        output.push_str("            if (a.type == 0 && b.type == 0) swap = a.value.i > b.value.i;\n");
        output.push_str("            else if (a.type == 1 && b.type == 1) swap = a.value.f > b.value.f;\n");
        output.push_str("            else if (a.type == 0 && b.type == 1) swap = (double)a.value.i > b.value.f;\n");
        output.push_str("            else if (a.type == 1 && b.type == 0) swap = a.value.f > (double)b.value.i;\n");
        output.push_str("            else if (a.type == 2 && b.type == 2 && a.value.s && b.value.s) swap = strcmp(a.value.s, b.value.s) > 0;\n");
        output.push_str("            if (swap) { list->items[j] = b; list->items[j+1] = a; }\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__copy(TauValue lst) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = lst.value.list;\n");
        output.push_str("    TauList* copy = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        tauraro_list_append(copy, src->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = copy, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue lst__clear(TauValue lst) {\n");
        output.push_str("    if (lst.type != 4 || !lst.value.list) return tauraro_none();\n");
        output.push_str("    lst.value.list->size = 0;\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        // ===== TYPE CONVERSION UTILITIES =====
        output.push_str("// Type conversion utilities\n");
        output.push_str("TauValue tauraro_abs(TauValue val) {\n");
        output.push_str("    if (val.type == 0) return tauraro_int(val.value.i < 0 ? -val.value.i : val.value.i);\n");
        output.push_str("    if (val.type == 1) return tauraro_float(val.value.f < 0 ? -val.value.f : val.value.f);\n");
        output.push_str("    return val;\n");
        output.push_str("}\n\n");

        // Helper for value equality comparison
        output.push_str("int tauraro_equals(TauValue a, TauValue b) {\n");
        output.push_str("    if (a.type != b.type) {\n");
        output.push_str("        // Allow int/float comparison\n");
        output.push_str("        if ((a.type == 0 && b.type == 1) || (a.type == 1 && b.type == 0)) {\n");
        output.push_str("            double av = a.type == 0 ? (double)a.value.i : a.value.f;\n");
        output.push_str("            double bv = b.type == 0 ? (double)b.value.i : b.value.f;\n");
        output.push_str("            return av == bv;\n");
        output.push_str("        }\n");
        output.push_str("        return 0;\n");
        output.push_str("    }\n");
        output.push_str("    switch (a.type) {\n");
        output.push_str("        case 0: return a.value.i == b.value.i;\n");
        output.push_str("        case 1: return a.value.f == b.value.f;\n");
        output.push_str("        case 2: return (a.value.s && b.value.s) ? strcmp(a.value.s, b.value.s) == 0 : (a.value.s == b.value.s);\n");
        output.push_str("        case 3: return a.value.i == b.value.i; // bool stored as int\n");
        output.push_str("        default: return 0;\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_min(TauValue a, TauValue b) {\n");
        output.push_str("    if (a.type == 0 && b.type == 0) return tauraro_int(a.value.i < b.value.i ? a.value.i : b.value.i);\n");
        output.push_str("    if (a.type == 1 || b.type == 1) {\n");
        output.push_str("        double av = a.type == 0 ? (double)a.value.i : a.value.f;\n");
        output.push_str("        double bv = b.type == 0 ? (double)b.value.i : b.value.f;\n");
        output.push_str("        return tauraro_float(av < bv ? av : bv);\n");
        output.push_str("    }\n");
        output.push_str("    return a;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_max(TauValue a, TauValue b) {\n");
        output.push_str("    if (a.type == 0 && b.type == 0) return tauraro_int(a.value.i > b.value.i ? a.value.i : b.value.i);\n");
        output.push_str("    if (a.type == 1 || b.type == 1) {\n");
        output.push_str("        double av = a.type == 0 ? (double)a.value.i : a.value.f;\n");
        output.push_str("        double bv = b.type == 0 ? (double)b.value.i : b.value.f;\n");
        output.push_str("        return tauraro_float(av > bv ? av : bv);\n");
        output.push_str("    }\n");
        output.push_str("    return a;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_sum(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_int(0);\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    long long isum = 0;\n");
        output.push_str("    double fsum = 0.0;\n");
        output.push_str("    int is_float = 0;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (lst->items[i].type == 0) isum += lst->items[i].value.i;\n");
        output.push_str("        else if (lst->items[i].type == 1) { fsum += lst->items[i].value.f; is_float = 1; }\n");
        output.push_str("    }\n");
        output.push_str("    if (is_float) return tauraro_float(fsum + (double)isum);\n");
        output.push_str("    return tauraro_int(isum);\n");
        output.push_str("}\n\n");

        // ===== LIST SLICING UTILITIES (OPTIMIZED) =====
        output.push_str("// Optimized list slicing\n");
        output.push_str("TauValue tauraro_list_slice(TauValue list, long long start, long long stop, long long step) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    long long len = (long long)src->size;\n");
        output.push_str("    // Normalize negative indices\n");
        output.push_str("    if (start < 0) start = start + len;\n");
        output.push_str("    if (stop < 0) stop = stop + len;\n");
        output.push_str("    // Clamp to bounds\n");
        output.push_str("    if (start < 0) start = 0;\n");
        output.push_str("    if (start > len) start = len;\n");
        output.push_str("    if (stop < 0) stop = 0;\n");
        output.push_str("    if (stop > len) stop = len;\n");
        output.push_str("    if (step == 0) step = 1; // Prevent infinite loop\n");
        output.push_str("    // Calculate result size\n");
        output.push_str("    size_t result_size = 0;\n");
        output.push_str("    if (step > 0 && start < stop) {\n");
        output.push_str("        result_size = (size_t)((stop - start + step - 1) / step);\n");
        output.push_str("    } else if (step < 0 && start > stop) {\n");
        output.push_str("        result_size = (size_t)((start - stop - step - 1) / (-step));\n");
        output.push_str("    }\n");
        output.push_str("    TauList* result = tauraro_create_list(result_size > 0 ? result_size : 1);\n");
        output.push_str("    if (step > 0) {\n");
        output.push_str("        for (long long i = start; i < stop; i += step) {\n");
        output.push_str("            tauraro_list_append(result, src->items[i]);\n");
        output.push_str("        }\n");
        output.push_str("    } else {\n");
        output.push_str("        for (long long i = start; i > stop; i += step) {\n");
        output.push_str("            tauraro_list_append(result, src->items[i]);\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== STRING SLICING (OPTIMIZED) =====
        output.push_str("TauValue tauraro_string_slice(TauValue str, long long start, long long stop, long long step) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    long long len = (long long)strlen(str.value.s);\n");
        output.push_str("    if (start < 0) start = start + len;\n");
        output.push_str("    if (stop < 0) stop = stop + len;\n");
        output.push_str("    if (start < 0) start = 0;\n");
        output.push_str("    if (start > len) start = len;\n");
        output.push_str("    if (stop < 0) stop = 0;\n");
        output.push_str("    if (stop > len) stop = len;\n");
        output.push_str("    if (step == 0) step = 1;\n");
        output.push_str("    // Calculate result size\n");
        output.push_str("    size_t result_size = 0;\n");
        output.push_str("    if (step > 0 && start < stop) {\n");
        output.push_str("        result_size = (size_t)((stop - start + step - 1) / step);\n");
        output.push_str("    } else if (step < 0 && start > stop) {\n");
        output.push_str("        result_size = (size_t)((start - stop - step - 1) / (-step));\n");
        output.push_str("    }\n");
        output.push_str("    char* result = malloc(result_size + 1);\n");
        output.push_str("    size_t j = 0;\n");
        output.push_str("    if (step > 0) {\n");
        output.push_str("        for (long long i = start; i < stop && j < result_size; i += step) {\n");
        output.push_str("            result[j++] = str.value.s[i];\n");
        output.push_str("        }\n");
        output.push_str("    } else {\n");
        output.push_str("        for (long long i = start; i > stop && j < result_size; i += step) {\n");
        output.push_str("            result[j++] = str.value.s[i];\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    result[j] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== GENERIC SLICING DISPATCHER =====
        output.push_str("TauValue tauraro_slice(TauValue obj, long long start, long long stop, long long step) {\n");
        output.push_str("    if (obj.type == 4) return tauraro_list_slice(obj, start, stop, step);\n");
        output.push_str("    if (obj.type == 2) return tauraro_string_slice(obj, start, stop, step);\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        // ===== FORMAT STRING UTILITIES (OPTIMIZED) =====
        output.push_str("// Note: tauraro_format_value is defined earlier with full list/dict support\n");

        output.push_str("TauValue tauraro_fstring_concat(int count, ...) {\n");
        output.push_str("    va_list args;\n");
        output.push_str("    va_start(args, count);\n");
        output.push_str("    // First pass: calculate total length\n");
        output.push_str("    size_t total_len = 1; // For null terminator\n");
        output.push_str("    char** parts = malloc(count * sizeof(char*));\n");
        output.push_str("    for (int i = 0; i < count; i++) {\n");
        output.push_str("        TauValue v = va_arg(args, TauValue);\n");
        output.push_str("        parts[i] = tauraro_format_value(v);\n");
        output.push_str("        total_len += strlen(parts[i]);\n");
        output.push_str("    }\n");
        output.push_str("    va_end(args);\n");
        output.push_str("    // Second pass: build result\n");
        output.push_str("    char* result = malloc(total_len);\n");
        output.push_str("    result[0] = '\\0';\n");
        output.push_str("    for (int i = 0; i < count; i++) {\n");
        output.push_str("        strcat(result, parts[i]);\n");
        output.push_str("        free(parts[i]);\n");
        output.push_str("    }\n");
        output.push_str("    free(parts);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== LIST COMPREHENSION HELPERS =====
        output.push_str("// List comprehension optimization macros\n");
        output.push_str("#define TAURARO_LISTCOMP_BEGIN(result_var, capacity) \\\n");
        output.push_str("    TauList* result_var = tauraro_create_list(capacity)\n\n");

        output.push_str("#define TAURARO_LISTCOMP_ADD(result_var, value) \\\n");
        output.push_str("    tauraro_list_append(result_var, value)\n\n");

        output.push_str("#define TAURARO_LISTCOMP_END(result_var) \\\n");
        output.push_str("    (TauValue){.type = 4, .value.list = result_var, .refcount = 1}\n\n");

        // ===== VALUE EQUALITY HELPER =====
        output.push_str("// Value equality check for sets/dicts\n");
        output.push_str("int tauraro_value_equals(TauValue a, TauValue b) {\n");
        output.push_str("    if (a.type != b.type) return 0;\n");
        output.push_str("    switch (a.type) {\n");
        output.push_str("        case 0: return a.value.i == b.value.i;\n");
        output.push_str("        case 1: return a.value.f == b.value.f;\n");
        output.push_str("        case 2: return strcmp(a.value.s ? a.value.s : \"\", b.value.s ? b.value.s : \"\") == 0;\n");
        output.push_str("        default: return 0;\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        // ===== GENERATOR STATE =====
        output.push_str("// Generator state structure\n");
        output.push_str("typedef struct TauGeneratorState {\n");
        output.push_str("    int state;\n");
        output.push_str("    TauValue last_value;\n");
        output.push_str("    void* context;\n");
        output.push_str("} TauGeneratorState;\n\n");

        output.push_str("TauGeneratorState* tauraro_create_generator_state() {\n");
        output.push_str("    TauGeneratorState* g = malloc(sizeof(TauGeneratorState));\n");
        output.push_str("    g->state = 0;\n");
        output.push_str("    g->last_value = tauraro_none();\n");
        output.push_str("    g->context = NULL;\n");
        output.push_str("    return g;\n");
        output.push_str("}\n\n");

        // ===== ENUMERATE UTILITY =====
        output.push_str("typedef struct TauEnumerate {\n");
        output.push_str("    TauList* list;\n");
        output.push_str("    size_t index;\n");
        output.push_str("    long long start;\n");
        output.push_str("} TauEnumerate;\n\n");

        output.push_str("TauEnumerate* tauraro_enumerate(TauValue list, long long start) {\n");
        output.push_str("    TauEnumerate* e = malloc(sizeof(TauEnumerate));\n");
        output.push_str("    e->list = list.type == 4 ? list.value.list : NULL;\n");
        output.push_str("    e->index = 0;\n");
        output.push_str("    e->start = start;\n");
        output.push_str("    return e;\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_enumerate_next(TauEnumerate* e, long long* idx, TauValue* val) {\n");
        output.push_str("    if (!e || !e->list || e->index >= e->list->size) return 0;\n");
        output.push_str("    *idx = e->start + (long long)e->index;\n");
        output.push_str("    *val = e->list->items[e->index++];\n");
        output.push_str("    return 1;\n");
        output.push_str("}\n\n");

        // ===== ZIP UTILITY =====
        output.push_str("typedef struct TauZip {\n");
        output.push_str("    TauList** lists;\n");
        output.push_str("    size_t list_count;\n");
        output.push_str("    size_t index;\n");
        output.push_str("    size_t min_len;\n");
        output.push_str("} TauZip;\n\n");

        output.push_str("TauZip* tauraro_zip(int count, ...) {\n");
        output.push_str("    va_list args;\n");
        output.push_str("    va_start(args, count);\n");
        output.push_str("    TauZip* z = malloc(sizeof(TauZip));\n");
        output.push_str("    z->lists = malloc(count * sizeof(TauList*));\n");
        output.push_str("    z->list_count = count;\n");
        output.push_str("    z->index = 0;\n");
        output.push_str("    z->min_len = SIZE_MAX;\n");
        output.push_str("    for (int i = 0; i < count; i++) {\n");
        output.push_str("        TauValue v = va_arg(args, TauValue);\n");
        output.push_str("        z->lists[i] = v.type == 4 ? v.value.list : NULL;\n");
        output.push_str("        if (z->lists[i] && z->lists[i]->size < z->min_len) {\n");
        output.push_str("            z->min_len = z->lists[i]->size;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    va_end(args);\n");
        output.push_str("    return z;\n");
        output.push_str("}\n\n");

        output.push_str("int tauraro_zip_next(TauZip* z, TauValue* results) {\n");
        output.push_str("    if (!z || z->index >= z->min_len) return 0;\n");
        output.push_str("    for (size_t i = 0; i < z->list_count; i++) {\n");
        output.push_str("        results[i] = z->lists[i] ? z->lists[i]->items[z->index] : tauraro_none();\n");
        output.push_str("    }\n");
        output.push_str("    z->index++;\n");
        output.push_str("    return 1;\n");
        output.push_str("}\n\n");

        // ===== ALL/ANY UTILITIES =====
        output.push_str("TauValue tauraro_all(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_bool(1);\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        TauValue v = lst->items[i];\n");
        output.push_str("        if (v.type == 0 && v.value.i == 0) return tauraro_bool(0);\n");
        output.push_str("        if (v.type == 1 && v.value.f == 0.0) return tauraro_bool(0);\n");
        output.push_str("        if (v.type == 3 && v.value.i == 0) return tauraro_bool(0);\n");
        output.push_str("        if (v.type == 2 && (!v.value.s || v.value.s[0] == '\\0')) return tauraro_bool(0);\n");
        output.push_str("        if (v.type == 4 && (!v.value.list || v.value.list->size == 0)) return tauraro_bool(0);\n");
        output.push_str("        if (v.type == 5 && (!v.value.dict || v.value.dict->size == 0)) return tauraro_bool(0);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_bool(1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_any(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_bool(0);\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        TauValue v = lst->items[i];\n");
        output.push_str("        if (v.type == 0 && v.value.i != 0) return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 1 && v.value.f != 0.0) return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 3 && v.value.i != 0) return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 2 && v.value.s && v.value.s[0] != '\\0') return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 4 && v.value.list && v.value.list->size > 0) return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 5 && v.value.dict && v.value.dict->size > 0) return tauraro_bool(1);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_bool(0);\n");
        output.push_str("}\n\n");

        // ===== SORTED/REVERSED =====
        output.push_str("int tauraro_compare_values(const void* a, const void* b) {\n");
        output.push_str("    TauValue* va = (TauValue*)a;\n");
        output.push_str("    TauValue* vb = (TauValue*)b;\n");
        output.push_str("    if (va->type == 0 && vb->type == 0) {\n");
        output.push_str("        return va->value.i < vb->value.i ? -1 : (va->value.i > vb->value.i ? 1 : 0);\n");
        output.push_str("    }\n");
        output.push_str("    if (va->type == 2 && vb->type == 2) {\n");
        output.push_str("        return strcmp(va->value.s ? va->value.s : \"\", vb->value.s ? vb->value.s : \"\");\n");
        output.push_str("    }\n");
        output.push_str("    return 0;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_sorted(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    TauList* result = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        tauraro_list_append(result, src->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    qsort(result->items, result->size, sizeof(TauValue), tauraro_compare_values);\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_reversed(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    TauList* result = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = src->size; i > 0; i--) {\n");
        output.push_str("        tauraro_list_append(result, src->items[i - 1]);\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== LIST INDEXOF/COUNT =====
        output.push_str("TauValue tauraro_list_index(TauValue list, TauValue item) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_int(-1);\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (tauraro_value_equals(lst->items[i], item)) return tauraro_int((long long)i);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_int(-1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_count(TauValue list, TauValue item) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_int(0);\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    long long count = 0;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (tauraro_value_equals(lst->items[i], item)) count++;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_int(count);\n");
        output.push_str("}\n\n");

        // ===== DICT UTILITIES (using bucket-based hash table) =====
        output.push_str("TauValue tauraro_dict_keys_v(TauValue dict) {\n");
        output.push_str("    if (dict.type != 5 || !dict.value.dict) return tauraro_none();\n");
        output.push_str("    TauDict* d = dict.value.dict;\n");
        output.push_str("    TauList* keys = tauraro_create_list(d->size);\n");
        output.push_str("    for (size_t i = 0; i < d->capacity; i++) {\n");
        output.push_str("        TauDictEntry* entry = d->buckets[i];\n");
        output.push_str("        while (entry) {\n");
        output.push_str("            tauraro_list_append(keys, tauraro_str(entry->key));\n");
        output.push_str("            entry = entry->next;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = keys, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_dict_values_v(TauValue dict) {\n");
        output.push_str("    if (dict.type != 5 || !dict.value.dict) return tauraro_none();\n");
        output.push_str("    TauDict* d = dict.value.dict;\n");
        output.push_str("    TauList* values = tauraro_create_list(d->size);\n");
        output.push_str("    for (size_t i = 0; i < d->capacity; i++) {\n");
        output.push_str("        TauDictEntry* entry = d->buckets[i];\n");
        output.push_str("        while (entry) {\n");
        output.push_str("            tauraro_list_append(values, entry->value);\n");
        output.push_str("            entry = entry->next;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = values, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_dict_items_v(TauValue dict) {\n");
        output.push_str("    if (dict.type != 5 || !dict.value.dict) return tauraro_none();\n");
        output.push_str("    TauDict* d = dict.value.dict;\n");
        output.push_str("    TauList* items = tauraro_create_list(d->size);\n");
        output.push_str("    for (size_t i = 0; i < d->capacity; i++) {\n");
        output.push_str("        TauDictEntry* entry = d->buckets[i];\n");
        output.push_str("        while (entry) {\n");
        output.push_str("            TauList* pair = tauraro_create_list(2);\n");
        output.push_str("            tauraro_list_append(pair, tauraro_str(entry->key));\n");
        output.push_str("            tauraro_list_append(pair, entry->value);\n");
        output.push_str("            TauValue pair_val = {.type = 4, .value.list = pair, .refcount = 1};\n");
        output.push_str("            tauraro_list_append(items, pair_val);\n");
        output.push_str("            entry = entry->next;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = items, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_dict_get_v(TauValue dict, TauValue key, TauValue default_val) {\n");
        output.push_str("    if (dict.type != 5 || !dict.value.dict) return default_val;\n");
        output.push_str("    if (key.type != 2 || !key.value.s) return default_val;\n");
        output.push_str("    TauValue* result = tauraro_dict_get(dict.value.dict, key.value.s);\n");
        output.push_str("    return result ? *result : default_val;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_dict_pop_v(TauValue dict, TauValue key, TauValue default_val) {\n");
        output.push_str("    if (dict.type != 5 || !dict.value.dict) return default_val;\n");
        output.push_str("    if (key.type != 2 || !key.value.s) return default_val;\n");
        output.push_str("    TauValue* result = tauraro_dict_get(dict.value.dict, key.value.s);\n");
        output.push_str("    if (!result) return default_val;\n");
        output.push_str("    TauValue val = *result;\n");
        output.push_str("    // Note: actual removal would require more complex logic\n");
        output.push_str("    return val;\n");
        output.push_str("}\n\n");

        // ===== SIMPLIFIED CONTEXT MANAGER (TAUVALUE-BASED) =====
        output.push_str("// Simplified context manager for TauValue-based contexts\n");
        output.push_str("TauValue tauraro_ctx_enter(TauValue ctx) {\n");
        output.push_str("    // For objects with __enter__ method, call it\n");
        output.push_str("    // For now, just return the context itself\n");
        output.push_str("    return ctx;\n");
        output.push_str("}\n\n");

        output.push_str("void tauraro_ctx_exit(TauValue ctx) {\n");
        output.push_str("    // For objects with __exit__ method, call it\n");
        output.push_str("    // For now, do nothing\n");
        output.push_str("    (void)ctx;\n");
        output.push_str("}\n\n");

        // ===== LAMBDA/CLOSURE UTILITIES =====
        output.push_str("// Lambda/Closure support with variable capture\n");
        output.push_str("typedef struct TauLambda {\n");
        output.push_str("    TauValue (*func)(struct TauLambda*, int, TauValue*);\n");
        output.push_str("    TauValue* captures;\n");
        output.push_str("    int capture_count;\n");
        output.push_str("    int param_count;\n");
        output.push_str("} TauLambda;\n\n");

        output.push_str("TauLambda* tauraro_create_lambda(TauValue (*func)(TauLambda*, int, TauValue*), int param_count, int capture_count) {\n");
        output.push_str("    TauLambda* l = malloc(sizeof(TauLambda));\n");
        output.push_str("    l->func = func;\n");
        output.push_str("    l->param_count = param_count;\n");
        output.push_str("    l->capture_count = capture_count;\n");
        output.push_str("    l->captures = capture_count > 0 ? malloc(capture_count * sizeof(TauValue)) : NULL;\n");
        output.push_str("    return l;\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_call_lambda(TauLambda* l, int argc, TauValue* argv) {\n");
        output.push_str("    if (!l || !l->func) return tauraro_none();\n");
        output.push_str("    return l->func(l, argc, argv);\n");
        output.push_str("}\n\n");

        // ===== MAP/FILTER/REDUCE WITH FUNCTION POINTERS =====
        output.push_str("// Functional programming utilities\n");
        output.push_str("typedef TauValue (*TauMapFunc)(TauValue);\n");
        output.push_str("typedef int (*TauFilterFunc)(TauValue);\n");
        output.push_str("typedef TauValue (*TauReduceFunc)(TauValue, TauValue);\n\n");

        output.push_str("TauValue tauraro_map_fn(TauMapFunc fn, TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    TauList* result = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        tauraro_list_append(result, fn(src->items[i]));\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_filter_fn(TauFilterFunc fn, TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    TauList* result = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        if (fn(src->items[i])) tauraro_list_append(result, src->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_reduce_fn(TauReduceFunc fn, TauValue list, TauValue initial) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return initial;\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    TauValue acc = initial;\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        acc = fn(acc, src->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    return acc;\n");
        output.push_str("}\n\n");

        // ===== RANGE WITH STEP =====
        output.push_str("// Range with step support\n");
        output.push_str("TauValue tauraro_range_list(long long start, long long stop, long long step) {\n");
        output.push_str("    if (step == 0) step = 1;\n");
        output.push_str("    size_t count = 0;\n");
        output.push_str("    if (step > 0 && start < stop) count = (size_t)((stop - start + step - 1) / step);\n");
        output.push_str("    else if (step < 0 && start > stop) count = (size_t)((start - stop - step - 1) / (-step));\n");
        output.push_str("    TauList* result = tauraro_create_list(count > 0 ? count : 1);\n");
        output.push_str("    if (step > 0) {\n");
        output.push_str("        for (long long i = start; i < stop; i += step) {\n");
        output.push_str("            tauraro_list_append(result, tauraro_int(i));\n");
        output.push_str("        }\n");
        output.push_str("    } else {\n");
        output.push_str("        for (long long i = start; i > stop; i += step) {\n");
        output.push_str("            tauraro_list_append(result, tauraro_int(i));\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== STRING METHODS =====
        output.push_str("// String manipulation methods\n");
        output.push_str("TauValue tauraro_str_split(TauValue str, TauValue delim) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_none();\n");
        output.push_str("    const char* s = str.value.s;\n");
        output.push_str("    const char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : \" \";\n");
        output.push_str("    TauList* result = tauraro_create_list(16);\n");
        output.push_str("    char* copy = strdup(s);\n");
        output.push_str("    char* token = strtok(copy, d);\n");
        output.push_str("    while (token) {\n");
        output.push_str("        tauraro_list_append(result, tauraro_str(token));\n");
        output.push_str("        token = strtok(NULL, d);\n");
        output.push_str("    }\n");
        output.push_str("    free(copy);\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_join(TauValue delim, TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_str(\"\");\n");
        output.push_str("    const char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : \"\";\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    if (lst->size == 0) return tauraro_str(\"\");\n");
        output.push_str("    // Calculate total length\n");
        output.push_str("    size_t total = 0, dlen = strlen(d);\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (lst->items[i].type == 2 && lst->items[i].value.s)\n");
        output.push_str("            total += strlen(lst->items[i].value.s);\n");
        output.push_str("        if (i < lst->size - 1) total += dlen;\n");
        output.push_str("    }\n");
        output.push_str("    char* result = malloc(total + 1);\n");
        output.push_str("    result[0] = '\\0';\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (lst->items[i].type == 2 && lst->items[i].value.s)\n");
        output.push_str("            strcat(result, lst->items[i].value.s);\n");
        output.push_str("        if (i < lst->size - 1) strcat(result, d);\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_replace(TauValue str, TauValue old, TauValue new_str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return str;\n");
        output.push_str("    if (old.type != 2 || !old.value.s || old.value.s[0] == '\\0') return str;\n");
        output.push_str("    const char* s = str.value.s;\n");
        output.push_str("    const char* o = old.value.s;\n");
        output.push_str("    const char* n = (new_str.type == 2 && new_str.value.s) ? new_str.value.s : \"\";\n");
        output.push_str("    size_t olen = strlen(o), nlen = strlen(n), slen = strlen(s);\n");
        output.push_str("    // Count occurrences\n");
        output.push_str("    size_t count = 0;\n");
        output.push_str("    const char* p = s;\n");
        output.push_str("    while ((p = strstr(p, o)) != NULL) { count++; p += olen; }\n");
        output.push_str("    // Allocate result\n");
        output.push_str("    size_t rlen = slen + count * (nlen - olen);\n");
        output.push_str("    char* result = malloc(rlen + 1);\n");
        output.push_str("    char* r = result;\n");
        output.push_str("    p = s;\n");
        output.push_str("    const char* q;\n");
        output.push_str("    while ((q = strstr(p, o)) != NULL) {\n");
        output.push_str("        size_t len = q - p;\n");
        output.push_str("        memcpy(r, p, len); r += len;\n");
        output.push_str("        memcpy(r, n, nlen); r += nlen;\n");
        output.push_str("        p = q + olen;\n");
        output.push_str("    }\n");
        output.push_str("    strcpy(r, p);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_find(TauValue str, TauValue sub) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_int(-1);\n");
        output.push_str("    if (sub.type != 2 || !sub.value.s) return tauraro_int(-1);\n");
        output.push_str("    const char* p = strstr(str.value.s, sub.value.s);\n");
        output.push_str("    if (!p) return tauraro_int(-1);\n");
        output.push_str("    return tauraro_int((long long)(p - str.value.s));\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_startswith(TauValue str, TauValue prefix) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_bool(0);\n");
        output.push_str("    if (prefix.type != 2 || !prefix.value.s) return tauraro_bool(0);\n");
        output.push_str("    size_t plen = strlen(prefix.value.s);\n");
        output.push_str("    return tauraro_bool(strncmp(str.value.s, prefix.value.s, plen) == 0);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_endswith(TauValue str, TauValue suffix) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_bool(0);\n");
        output.push_str("    if (suffix.type != 2 || !suffix.value.s) return tauraro_bool(0);\n");
        output.push_str("    size_t slen = strlen(str.value.s), xlen = strlen(suffix.value.s);\n");
        output.push_str("    if (xlen > slen) return tauraro_bool(0);\n");
        output.push_str("    return tauraro_bool(strcmp(str.value.s + slen - xlen, suffix.value.s) == 0);\n");
        output.push_str("}\n\n");

        // upper() - convert to uppercase
        output.push_str("TauValue tauraro_str_upper(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    char* result = strdup(str.value.s);\n");
        output.push_str("    for (char* p = result; *p; p++) *p = toupper((unsigned char)*p);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // lower() - convert to lowercase
        output.push_str("TauValue tauraro_str_lower(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    char* result = strdup(str.value.s);\n");
        output.push_str("    for (char* p = result; *p; p++) *p = tolower((unsigned char)*p);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // strip() - remove leading/trailing whitespace
        output.push_str("TauValue tauraro_str_strip(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    const char* s = str.value.s;\n");
        output.push_str("    while (*s && isspace((unsigned char)*s)) s++;\n");
        output.push_str("    if (!*s) return tauraro_str(\"\");\n");
        output.push_str("    const char* e = s + strlen(s) - 1;\n");
        output.push_str("    while (e > s && isspace((unsigned char)*e)) e--;\n");
        output.push_str("    size_t len = e - s + 1;\n");
        output.push_str("    char* result = malloc(len + 1);\n");
        output.push_str("    memcpy(result, s, len);\n");
        output.push_str("    result[len] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // lstrip() - remove leading whitespace
        output.push_str("TauValue tauraro_str_lstrip(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    const char* s = str.value.s;\n");
        output.push_str("    while (*s && isspace((unsigned char)*s)) s++;\n");
        output.push_str("    return tauraro_str(s);\n");
        output.push_str("}\n\n");

        // rstrip() - remove trailing whitespace
        output.push_str("TauValue tauraro_str_rstrip(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    size_t len = strlen(str.value.s);\n");
        output.push_str("    char* result = strdup(str.value.s);\n");
        output.push_str("    while (len > 0 && isspace((unsigned char)result[len-1])) len--;\n");
        output.push_str("    result[len] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // title() - title case
        output.push_str("TauValue tauraro_str_title(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    char* result = strdup(str.value.s);\n");
        output.push_str("    int cap = 1;\n");
        output.push_str("    for (char* p = result; *p; p++) {\n");
        output.push_str("        if (isspace((unsigned char)*p)) { cap = 1; }\n");
        output.push_str("        else if (cap) { *p = toupper((unsigned char)*p); cap = 0; }\n");
        output.push_str("        else { *p = tolower((unsigned char)*p); }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // capitalize() - capitalize first char
        output.push_str("TauValue tauraro_str_capitalize(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    char* result = strdup(str.value.s);\n");
        output.push_str("    if (result[0]) result[0] = toupper((unsigned char)result[0]);\n");
        output.push_str("    for (char* p = result + 1; *p; p++) *p = tolower((unsigned char)*p);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // swapcase() - swap case
        output.push_str("TauValue tauraro_str_swapcase(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    char* result = strdup(str.value.s);\n");
        output.push_str("    for (char* p = result; *p; p++) {\n");
        output.push_str("        if (isupper((unsigned char)*p)) *p = tolower((unsigned char)*p);\n");
        output.push_str("        else if (islower((unsigned char)*p)) *p = toupper((unsigned char)*p);\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // isdigit(), isalpha(), isalnum(), isspace(), isupper(), islower()
        output.push_str("TauValue tauraro_str_isdigit(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);\n");
        output.push_str("    for (const char* p = str.value.s; *p; p++) if (!isdigit((unsigned char)*p)) return tauraro_bool(0);\n");
        output.push_str("    return tauraro_bool(1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_isalpha(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);\n");
        output.push_str("    for (const char* p = str.value.s; *p; p++) if (!isalpha((unsigned char)*p)) return tauraro_bool(0);\n");
        output.push_str("    return tauraro_bool(1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_isalnum(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);\n");
        output.push_str("    for (const char* p = str.value.s; *p; p++) if (!isalnum((unsigned char)*p)) return tauraro_bool(0);\n");
        output.push_str("    return tauraro_bool(1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_isspace(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);\n");
        output.push_str("    for (const char* p = str.value.s; *p; p++) if (!isspace((unsigned char)*p)) return tauraro_bool(0);\n");
        output.push_str("    return tauraro_bool(1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_isupper(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);\n");
        output.push_str("    int has_cased = 0;\n");
        output.push_str("    for (const char* p = str.value.s; *p; p++) {\n");
        output.push_str("        if (islower((unsigned char)*p)) return tauraro_bool(0);\n");
        output.push_str("        if (isupper((unsigned char)*p)) has_cased = 1;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_bool(has_cased);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_islower(TauValue str) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);\n");
        output.push_str("    int has_cased = 0;\n");
        output.push_str("    for (const char* p = str.value.s; *p; p++) {\n");
        output.push_str("        if (isupper((unsigned char)*p)) return tauraro_bool(0);\n");
        output.push_str("        if (islower((unsigned char)*p)) has_cased = 1;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_bool(has_cased);\n");
        output.push_str("}\n\n");

        // count() - count occurrences
        output.push_str("TauValue tauraro_str_count(TauValue str, TauValue sub) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_int(0);\n");
        output.push_str("    if (sub.type != 2 || !sub.value.s || !sub.value.s[0]) return tauraro_int(0);\n");
        output.push_str("    long long count = 0;\n");
        output.push_str("    size_t sublen = strlen(sub.value.s);\n");
        output.push_str("    const char* p = str.value.s;\n");
        output.push_str("    while ((p = strstr(p, sub.value.s)) != NULL) { count++; p += sublen; }\n");
        output.push_str("    return tauraro_int(count);\n");
        output.push_str("}\n\n");

        // center(), ljust(), rjust() - padding
        output.push_str("TauValue tauraro_str_center(TauValue str, TauValue width) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    long long w = width.type == 0 ? width.value.i : 0;\n");
        output.push_str("    size_t slen = strlen(str.value.s);\n");
        output.push_str("    if (w <= (long long)slen) return str;\n");
        output.push_str("    size_t pad = (size_t)w - slen;\n");
        output.push_str("    size_t left = pad / 2, right = pad - left;\n");
        output.push_str("    char* result = malloc((size_t)w + 1);\n");
        output.push_str("    memset(result, ' ', left);\n");
        output.push_str("    memcpy(result + left, str.value.s, slen);\n");
        output.push_str("    memset(result + left + slen, ' ', right);\n");
        output.push_str("    result[w] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_ljust(TauValue str, TauValue width) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    long long w = width.type == 0 ? width.value.i : 0;\n");
        output.push_str("    size_t slen = strlen(str.value.s);\n");
        output.push_str("    if (w <= (long long)slen) return str;\n");
        output.push_str("    char* result = malloc((size_t)w + 1);\n");
        output.push_str("    memcpy(result, str.value.s, slen);\n");
        output.push_str("    memset(result + slen, ' ', (size_t)w - slen);\n");
        output.push_str("    result[w] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_str_rjust(TauValue str, TauValue width) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    long long w = width.type == 0 ? width.value.i : 0;\n");
        output.push_str("    size_t slen = strlen(str.value.s);\n");
        output.push_str("    if (w <= (long long)slen) return str;\n");
        output.push_str("    char* result = malloc((size_t)w + 1);\n");
        output.push_str("    size_t pad = (size_t)w - slen;\n");
        output.push_str("    memset(result, ' ', pad);\n");
        output.push_str("    memcpy(result + pad, str.value.s, slen);\n");
        output.push_str("    result[w] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // zfill() - zero padding
        output.push_str("TauValue tauraro_str_zfill(TauValue str, TauValue width) {\n");
        output.push_str("    if (str.type != 2 || !str.value.s) return tauraro_str(\"\");\n");
        output.push_str("    long long w = width.type == 0 ? width.value.i : 0;\n");
        output.push_str("    size_t slen = strlen(str.value.s);\n");
        output.push_str("    if (w <= (long long)slen) return str;\n");
        output.push_str("    char* result = malloc((size_t)w + 1);\n");
        output.push_str("    size_t pad = (size_t)w - slen;\n");
        output.push_str("    int sign_offset = 0;\n");
        output.push_str("    if (str.value.s[0] == '+' || str.value.s[0] == '-') {\n");
        output.push_str("        result[0] = str.value.s[0];\n");
        output.push_str("        sign_offset = 1;\n");
        output.push_str("    }\n");
        output.push_str("    memset(result + sign_offset, '0', pad);\n");
        output.push_str("    memcpy(result + sign_offset + pad, str.value.s + sign_offset, slen - sign_offset);\n");
        output.push_str("    result[w] = '\\0';\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== LIST METHODS (TAUVALUE-BASED) =====
        output.push_str("// List manipulation methods (TauValue wrappers)\n");
        output.push_str("TauValue tauraro_list_pop_v(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list || list.value.list->size == 0)\n");
        output.push_str("        return tauraro_none();\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    TauValue val = lst->items[lst->size - 1];\n");
        output.push_str("    lst->size--;\n");
        output.push_str("    return val;\n");
        output.push_str("}\n\n");;

        output.push_str("TauValue tauraro_list_insert(TauValue list, TauValue index, TauValue value) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    long long idx = index.type == 0 ? index.value.i : 0;\n");
        output.push_str("    if (idx < 0) idx = (long long)lst->size + idx;\n");
        output.push_str("    if (idx < 0) idx = 0;\n");
        output.push_str("    if ((size_t)idx > lst->size) idx = (long long)lst->size;\n");
        output.push_str("    // Ensure capacity\n");
        output.push_str("    if (lst->size >= lst->capacity) {\n");
        output.push_str("        lst->capacity = lst->capacity * 2 + 1;\n");
        output.push_str("        lst->items = realloc(lst->items, lst->capacity * sizeof(TauValue));\n");
        output.push_str("    }\n");
        output.push_str("    // Shift elements\n");
        output.push_str("    for (size_t i = lst->size; i > (size_t)idx; i--) {\n");
        output.push_str("        lst->items[i] = lst->items[i - 1];\n");
        output.push_str("    }\n");
        output.push_str("    lst->items[idx] = value;\n");
        output.push_str("    lst->size++;\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_remove(TauValue list, TauValue value) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        if (tauraro_value_equals(lst->items[i], value)) {\n");
        output.push_str("            for (size_t j = i; j < lst->size - 1; j++) {\n");
        output.push_str("                lst->items[j] = lst->items[j + 1];\n");
        output.push_str("            }\n");
        output.push_str("            lst->size--;\n");
        output.push_str("            return tauraro_none();\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_extend_v(TauValue list, TauValue other) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    if (other.type != 4 || !other.value.list) return tauraro_none();\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    TauList* ext = other.value.list;\n");
        output.push_str("    for (size_t i = 0; i < ext->size; i++) {\n");
        output.push_str("        tauraro_list_append(lst, ext->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_clear(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    list.value.list->size = 0;\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_list_copy(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_none();\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    TauList* dst = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        tauraro_list_append(dst, src->items[i]);\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = dst, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== RAW VALUE EXTRACTORS =====
        output.push_str("// Extract raw values from TauValue (for internal use)\n");
        output.push_str("static inline long long tauraro_raw_int(TauValue val) {\n");
        output.push_str("    switch (val.type) {\n");
        output.push_str("        case 0: return val.value.i;\n");
        output.push_str("        case 1: return (long long)val.value.f;\n");
        output.push_str("        case 3: return val.value.i;\n");
        output.push_str("        default: return 0;\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        // ===== TYPE CONVERSION =====
        output.push_str("// Type conversion functions\n");
        output.push_str("TauValue tauraro_to_int(TauValue val) {\n");
        output.push_str("    switch (val.type) {\n");
        output.push_str("        case 0: return val;\n");
        output.push_str("        case 1: return tauraro_int((long long)val.value.f);\n");
        output.push_str("        case 2: return tauraro_int(val.value.s ? atoll(val.value.s) : 0);\n");
        output.push_str("        case 3: return tauraro_int(val.value.i);\n");
        output.push_str("        default: return tauraro_int(0);\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_to_float(TauValue val) {\n");
        output.push_str("    switch (val.type) {\n");
        output.push_str("        case 0: return tauraro_float((double)val.value.i);\n");
        output.push_str("        case 1: return val;\n");
        output.push_str("        case 2: return tauraro_float(val.value.s ? atof(val.value.s) : 0.0);\n");
        output.push_str("        case 3: return tauraro_float((double)val.value.i);\n");
        output.push_str("        default: return tauraro_float(0.0);\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_to_str(TauValue val) {\n");
        output.push_str("    char* buf = tauraro_format_value(val);\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = buf, .refcount = 1};\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_to_bool(TauValue val) {\n");
        output.push_str("    switch (val.type) {\n");
        output.push_str("        case 0: return tauraro_bool(val.value.i != 0);\n");
        output.push_str("        case 1: return tauraro_bool(val.value.f != 0.0);\n");
        output.push_str("        case 2: return tauraro_bool(val.value.s && val.value.s[0] != '\\0');\n");
        output.push_str("        case 3: return val;\n");
        output.push_str("        case 4: return tauraro_bool(val.value.list && val.value.list->size > 0);\n");
        output.push_str("        default: return tauraro_bool(0);\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        // ===== INPUT FUNCTION =====
        output.push_str("// Input function\n");
        output.push_str("TauValue tauraro_input(TauValue prompt) {\n");
        output.push_str("    if (prompt.type == 2 && prompt.value.s) {\n");
        output.push_str("        printf(\"%s\", prompt.value.s);\n");
        output.push_str("        fflush(stdout);\n");
        output.push_str("    }\n");
        output.push_str("    char buf[4096];\n");
        output.push_str("    if (fgets(buf, sizeof(buf), stdin)) {\n");
        output.push_str("        size_t len = strlen(buf);\n");
        output.push_str("        if (len > 0 && buf[len-1] == '\\n') buf[len-1] = '\\0';\n");
        output.push_str("        return tauraro_str(buf);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_str(\"\");\n");
        output.push_str("}\n\n");

        // ===== ASSERTION =====
        output.push_str("// Assertion\n");
        output.push_str("void tauraro_assert(TauValue condition, TauValue message) {\n");
        output.push_str("    int cond = 0;\n");
        output.push_str("    if (condition.type == 0) cond = condition.value.i != 0;\n");
        output.push_str("    else if (condition.type == 3) cond = condition.value.i != 0;\n");
        output.push_str("    else if (condition.type == 2) cond = condition.value.s && condition.value.s[0];\n");
        output.push_str("    if (!cond) {\n");
        output.push_str("        if (message.type == 2 && message.value.s)\n");
        output.push_str("            fprintf(stderr, \"AssertionError: %s\\n\", message.value.s);\n");
        output.push_str("        else\n");
        output.push_str("            fprintf(stderr, \"AssertionError\\n\");\n");
        output.push_str("        exit(1);\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        // ===== REPR FUNCTION (proper list display) =====
        output.push_str("// Repr function for proper display\n");
        output.push_str("TauValue tauraro_repr(TauValue val) {\n");
        output.push_str("    char* buf = malloc(4096);\n");
        output.push_str("    buf[0] = '\\0';\n");
        output.push_str("    switch (val.type) {\n");
        output.push_str("        case 0: snprintf(buf, 4096, \"%lld\", val.value.i); break;\n");
        output.push_str("        case 1: snprintf(buf, 4096, \"%g\", val.value.f); break;\n");
        output.push_str("        case 2: snprintf(buf, 4096, \"'%s'\", val.value.s ? val.value.s : \"\"); break;\n");
        output.push_str("        case 3: snprintf(buf, 4096, \"%s\", val.value.i ? \"True\" : \"False\"); break;\n");
        output.push_str("        case 4: {\n");
        output.push_str("            strcat(buf, \"[\");\n");
        output.push_str("            TauList* lst = val.value.list;\n");
        output.push_str("            if (lst) {\n");
        output.push_str("                for (size_t i = 0; i < lst->size && strlen(buf) < 3900; i++) {\n");
        output.push_str("                    if (i > 0) strcat(buf, \", \");\n");
        output.push_str("                    char* elem = tauraro_format_value(lst->items[i]);\n");
        output.push_str("                    strcat(buf, elem);\n");
        output.push_str("                    free(elem);\n");
        output.push_str("                }\n");
        output.push_str("            }\n");
        output.push_str("            strcat(buf, \"]\");\n");
        output.push_str("            break;\n");
        output.push_str("        }\n");
        output.push_str("        case 5: snprintf(buf, 4096, \"<dict>\"); break;\n");
        output.push_str("        case 6: snprintf(buf, 4096, \"<object>\"); break;\n");
        output.push_str("        default: snprintf(buf, 4096, \"<unknown>\"); break;\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 2, .value.s = buf, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== ADDITIONAL BUILTIN FUNCTIONS =====

        // enumerate_list() - return list of (index, value) tuples
        output.push_str("TauValue tauraro_enumerate_list(TauValue list, TauValue start_val) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return (TauValue){.type = 4, .value.list = tauraro_create_list(0)};\n");
        output.push_str("    TauList* src = list.value.list;\n");
        output.push_str("    long long start = start_val.type == 0 ? start_val.value.i : 0;\n");
        output.push_str("    TauList* dst = tauraro_create_list(src->size);\n");
        output.push_str("    for (size_t i = 0; i < src->size; i++) {\n");
        output.push_str("        TauList* tuple = tauraro_create_list(2);\n");
        output.push_str("        tauraro_list_append(tuple, tauraro_int(start + (long long)i));\n");
        output.push_str("        tauraro_list_append(tuple, src->items[i]);\n");
        output.push_str("        tauraro_list_append(dst, (TauValue){.type = 4, .value.list = tuple});\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = dst, .refcount = 1};\n");
        output.push_str("}\n\n");

        // zip_lists() - return list of tuples
        output.push_str("TauValue tauraro_zip_lists(TauValue list1, TauValue list2) {\n");
        output.push_str("    if (list1.type != 4 || !list1.value.list || list2.type != 4 || !list2.value.list)\n");
        output.push_str("        return (TauValue){.type = 4, .value.list = tauraro_create_list(0)};\n");
        output.push_str("    TauList* src1 = list1.value.list;\n");
        output.push_str("    TauList* src2 = list2.value.list;\n");
        output.push_str("    size_t min_size = src1->size < src2->size ? src1->size : src2->size;\n");
        output.push_str("    TauList* dst = tauraro_create_list(min_size);\n");
        output.push_str("    for (size_t i = 0; i < min_size; i++) {\n");
        output.push_str("        TauList* tuple = tauraro_create_list(2);\n");
        output.push_str("        tauraro_list_append(tuple, src1->items[i]);\n");
        output.push_str("        tauraro_list_append(tuple, src2->items[i]);\n");
        output.push_str("        tauraro_list_append(dst, (TauValue){.type = 4, .value.list = tuple});\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = dst, .refcount = 1};\n");
        output.push_str("}\n\n");

        // type_name() - return type as string
        output.push_str("TauValue tauraro_type_name(TauValue val) {\n");
        output.push_str("    const char* names[] = {\"int\", \"float\", \"str\", \"bool\", \"list\", \"dict\", \"object\", \"function\", \"exception\", \"module\", \"none\"};\n");
        output.push_str("    int idx = val.type < 11 ? val.type : 10;\n");
        output.push_str("    return tauraro_str(names[idx]);\n");
        output.push_str("}\n\n");

        // isinstance() - type check
        output.push_str("TauValue tauraro_isinstance(TauValue obj, TauValue type_str) {\n");
        output.push_str("    if (type_str.type != 2 || !type_str.value.s) return tauraro_bool(0);\n");
        output.push_str("    const char* t = type_str.value.s;\n");
        output.push_str("    if (strcmp(t, \"int\") == 0) return tauraro_bool(obj.type == 0);\n");
        output.push_str("    if (strcmp(t, \"float\") == 0) return tauraro_bool(obj.type == 1);\n");
        output.push_str("    if (strcmp(t, \"str\") == 0) return tauraro_bool(obj.type == 2);\n");
        output.push_str("    if (strcmp(t, \"bool\") == 0) return tauraro_bool(obj.type == 3);\n");
        output.push_str("    if (strcmp(t, \"list\") == 0) return tauraro_bool(obj.type == 4);\n");
        output.push_str("    if (strcmp(t, \"dict\") == 0) return tauraro_bool(obj.type == 5);\n");
        output.push_str("    return tauraro_bool(0);\n");
        output.push_str("}\n\n");

        // ord() - char to int
        output.push_str("TauValue tauraro_ord(TauValue ch) {\n");
        output.push_str("    if (ch.type == 2 && ch.value.s && ch.value.s[0]) {\n");
        output.push_str("        return tauraro_int((unsigned char)ch.value.s[0]);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_int(0);\n");
        output.push_str("}\n\n");

        // chr() - int to char
        output.push_str("TauValue tauraro_chr(TauValue num) {\n");
        output.push_str("    char buf[2] = {0};\n");
        output.push_str("    if (num.type == 0 && num.value.i >= 0 && num.value.i <= 127) {\n");
        output.push_str("        buf[0] = (char)num.value.i;\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_str(buf);\n");
        output.push_str("}\n\n");

        // round() - round to decimal places
        output.push_str("TauValue tauraro_round(TauValue num, TauValue places) {\n");
        output.push_str("    double val = num.type == 0 ? (double)num.value.i : (num.type == 1 ? num.value.f : 0.0);\n");
        output.push_str("    long long p = places.type == 0 ? places.value.i : 0;\n");
        output.push_str("    if (p == 0) return tauraro_int((long long)round(val));\n");
        output.push_str("    double mult = pow(10.0, (double)p);\n");
        output.push_str("    return tauraro_float(round(val * mult) / mult);\n");
        output.push_str("}\n\n");

        // pow() - power function
        output.push_str("TauValue tauraro_pow(TauValue base, TauValue exp) {\n");
        output.push_str("    double b = base.type == 0 ? (double)base.value.i : (base.type == 1 ? base.value.f : 0.0);\n");
        output.push_str("    double e = exp.type == 0 ? (double)exp.value.i : (exp.type == 1 ? exp.value.f : 0.0);\n");
        output.push_str("    double result = pow(b, e);\n");
        output.push_str("    if (base.type == 0 && exp.type == 0 && exp.value.i >= 0) return tauraro_int((long long)result);\n");
        output.push_str("    return tauraro_float(result);\n");
        output.push_str("}\n\n");

        // sqrt() - square root
        output.push_str("TauValue tauraro_sqrt(TauValue num) {\n");
        output.push_str("    double val = num.type == 0 ? (double)num.value.i : (num.type == 1 ? num.value.f : 0.0);\n");
        output.push_str("    return tauraro_float(sqrt(val));\n");
        output.push_str("}\n\n");

        // hex() - int to hex string
        output.push_str("TauValue tauraro_hex(TauValue num) {\n");
        output.push_str("    char buf[32];\n");
        output.push_str("    long long n = num.type == 0 ? num.value.i : 0;\n");
        output.push_str("    if (n >= 0) snprintf(buf, 32, \"0x%llx\", n);\n");
        output.push_str("    else snprintf(buf, 32, \"-0x%llx\", -n);\n");
        output.push_str("    return tauraro_str(buf);\n");
        output.push_str("}\n\n");

        // bin() - int to binary string
        output.push_str("TauValue tauraro_bin(TauValue num) {\n");
        output.push_str("    char buf[72];\n");
        output.push_str("    long long n = num.type == 0 ? num.value.i : 0;\n");
        output.push_str("    int neg = n < 0;\n");
        output.push_str("    if (neg) n = -n;\n");
        output.push_str("    char* p = buf + 70;\n");
        output.push_str("    *p = '\\0';\n");
        output.push_str("    if (n == 0) { *--p = '0'; }\n");
        output.push_str("    while (n > 0) { *--p = '0' + (n & 1); n >>= 1; }\n");
        output.push_str("    *--p = 'b'; *--p = '0';\n");
        output.push_str("    if (neg) *--p = '-';\n");
        output.push_str("    return tauraro_str(p);\n");
        output.push_str("}\n\n");

        // oct() - int to octal string
        output.push_str("TauValue tauraro_oct(TauValue num) {\n");
        output.push_str("    char buf[32];\n");
        output.push_str("    long long n = num.type == 0 ? num.value.i : 0;\n");
        output.push_str("    if (n >= 0) snprintf(buf, 32, \"0o%llo\", n);\n");
        output.push_str("    else snprintf(buf, 32, \"-0o%llo\", -n);\n");
        output.push_str("    return tauraro_str(buf);\n");
        output.push_str("}\n\n");

        // divmod() - returns (quotient, remainder)
        output.push_str("TauValue tauraro_divmod(TauValue a, TauValue b) {\n");
        output.push_str("    long long av = a.type == 0 ? a.value.i : (long long)a.value.f;\n");
        output.push_str("    long long bv = b.type == 0 ? b.value.i : (long long)b.value.f;\n");
        output.push_str("    if (bv == 0) bv = 1;\n");
        output.push_str("    TauList* result = tauraro_create_list(2);\n");
        output.push_str("    tauraro_list_append(result, tauraro_int(av / bv));\n");
        output.push_str("    tauraro_list_append(result, tauraro_int(av % bv));\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = result, .refcount = 1};\n");
        output.push_str("}\n\n");

        // to_list() - convert iterable to list
        output.push_str("TauValue tauraro_to_list(TauValue val) {\n");
        output.push_str("    if (val.type == 4) return val;\n");
        output.push_str("    if (val.type == 2 && val.value.s) {\n");
        output.push_str("        size_t len = strlen(val.value.s);\n");
        output.push_str("        TauList* lst = tauraro_create_list(len);\n");
        output.push_str("        for (size_t i = 0; i < len; i++) {\n");
        output.push_str("            char c[2] = {val.value.s[i], '\\0'};\n");
        output.push_str("            tauraro_list_append(lst, tauraro_str(c));\n");
        output.push_str("        }\n");
        output.push_str("        return (TauValue){.type = 4, .value.list = lst, .refcount = 1};\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 4, .value.list = tauraro_create_list(0)};\n");
        output.push_str("}\n\n");

        // to_set() - convert iterable to set (dict with None values)
        output.push_str("TauValue tauraro_to_set(TauValue val) {\n");
        output.push_str("    TauDict* dict = tauraro_create_dict();\n");
        output.push_str("    if (val.type == 4 && val.value.list) {\n");
        output.push_str("        TauList* lst = val.value.list;\n");
        output.push_str("        for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("            if (lst->items[i].type == 2 && lst->items[i].value.s) {\n");
        output.push_str("                tauraro_dict_set(dict, lst->items[i].value.s, tauraro_none());\n");
        output.push_str("            }\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return (TauValue){.type = 5, .value.dict = dict, .refcount = 1};\n");
        output.push_str("}\n\n");

        // ===== MEMORY MANAGEMENT FUNCTIONS =====
        output.push_str("// Memory management structures\n");
        output.push_str("typedef struct tauraro_arena {\n");
        output.push_str("    void** buffers;\n");
        output.push_str("    size_t* sizes;\n");
        output.push_str("    size_t count;\n");
        output.push_str("    size_t capacity;\n");
        output.push_str("    char* name;\n");
        output.push_str("} tauraro_arena_t;\n\n");

        output.push_str("static struct {\n");
        output.push_str("    void** manual_buffers;\n");
        output.push_str("    size_t* buffer_sizes;\n");
        output.push_str("    size_t buffer_count;\n");
        output.push_str("    size_t buffer_capacity;\n");
        output.push_str("    tauraro_arena_t** arenas;\n");
        output.push_str("    size_t arena_count;\n");
        output.push_str("    size_t arena_capacity;\n");
        output.push_str("    char* current_arena;\n");
        output.push_str("} tauraro_memory_state = {NULL, NULL, 0, 0, NULL, 0, 0, NULL};\n\n");

        output.push_str("static void tauraro_memory_init(void) {\n");
        output.push_str("    if (tauraro_memory_state.manual_buffers == NULL) {\n");
        output.push_str("        tauraro_memory_state.buffer_capacity = 64;\n");
        output.push_str("        tauraro_memory_state.manual_buffers = (void**)malloc(sizeof(void*) * 64);\n");
        output.push_str("        tauraro_memory_state.buffer_sizes = (size_t*)malloc(sizeof(size_t) * 64);\n");
        output.push_str("        tauraro_memory_state.buffer_count = 0;\n");
        output.push_str("        tauraro_memory_state.arena_capacity = 16;\n");
        output.push_str("        tauraro_memory_state.arenas = (tauraro_arena_t**)malloc(sizeof(tauraro_arena_t*) * 16);\n");
        output.push_str("        tauraro_memory_state.arena_count = 0;\n");
        output.push_str("        tauraro_memory_state.current_arena = NULL;\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_allocate(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1 || (*args)->type != 0) return tauraro_none();\n");
        output.push_str("    tauraro_memory_init();\n");
        output.push_str("    size_t size = (size_t)(*args)->value.i;\n");
        output.push_str("    void* buffer = malloc(size);\n");
        output.push_str("    if (!buffer) return tauraro_none();\n");
        output.push_str("    if (tauraro_memory_state.buffer_count >= tauraro_memory_state.buffer_capacity) {\n");
        output.push_str("        tauraro_memory_state.buffer_capacity *= 2;\n");
        output.push_str("        tauraro_memory_state.manual_buffers = (void**)realloc(tauraro_memory_state.manual_buffers, sizeof(void*) * tauraro_memory_state.buffer_capacity);\n");
        output.push_str("        tauraro_memory_state.buffer_sizes = (size_t*)realloc(tauraro_memory_state.buffer_sizes, sizeof(size_t) * tauraro_memory_state.buffer_capacity);\n");
        output.push_str("    }\n");
        output.push_str("    size_t idx = tauraro_memory_state.buffer_count++;\n");
        output.push_str("    tauraro_memory_state.manual_buffers[idx] = buffer;\n");
        output.push_str("    tauraro_memory_state.buffer_sizes[idx] = size;\n");
        output.push_str("    return tauraro_int((long long)(uintptr_t)buffer);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_free(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1) return tauraro_none();\n");
        output.push_str("    void* ptr = (void*)(uintptr_t)(*args)->value.i;\n");
        output.push_str("    if (ptr) {\n");
        output.push_str("        for (size_t i = 0; i < tauraro_memory_state.buffer_count; i++) {\n");
        output.push_str("            if (tauraro_memory_state.manual_buffers[i] == ptr) {\n");
        output.push_str("                free(ptr);\n");
        output.push_str("                for (size_t j = i; j < tauraro_memory_state.buffer_count - 1; j++) {\n");
        output.push_str("                    tauraro_memory_state.manual_buffers[j] = tauraro_memory_state.manual_buffers[j + 1];\n");
        output.push_str("                    tauraro_memory_state.buffer_sizes[j] = tauraro_memory_state.buffer_sizes[j + 1];\n");
        output.push_str("                }\n");
        output.push_str("                tauraro_memory_state.buffer_count--;\n");
        output.push_str("                break;\n");
        output.push_str("            }\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_create_arena(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1 || (*args)->type != 2) return tauraro_none();\n");
        output.push_str("    tauraro_memory_init();\n");
        output.push_str("    char* name = (*args)->value.s;\n");
        output.push_str("    tauraro_arena_t* arena = (tauraro_arena_t*)malloc(sizeof(tauraro_arena_t));\n");
        output.push_str("    arena->name = strdup(name);\n");
        output.push_str("    arena->capacity = 64;\n");
        output.push_str("    arena->count = 0;\n");
        output.push_str("    arena->buffers = (void**)malloc(sizeof(void*) * arena->capacity);\n");
        output.push_str("    arena->sizes = (size_t*)malloc(sizeof(size_t) * arena->capacity);\n");
        output.push_str("    if (tauraro_memory_state.arena_count >= tauraro_memory_state.arena_capacity) {\n");
        output.push_str("        tauraro_memory_state.arena_capacity *= 2;\n");
        output.push_str("        tauraro_memory_state.arenas = (tauraro_arena_t**)realloc(tauraro_memory_state.arenas, sizeof(tauraro_arena_t*) * tauraro_memory_state.arena_capacity);\n");
        output.push_str("    }\n");
        output.push_str("    tauraro_memory_state.arenas[tauraro_memory_state.arena_count++] = arena;\n");
        output.push_str("    if (tauraro_memory_state.current_arena) free(tauraro_memory_state.current_arena);\n");
        output.push_str("    tauraro_memory_state.current_arena = strdup(name);\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_destroy_arena(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1 || (*args)->type != 2) return tauraro_none();\n");
        output.push_str("    char* name = (*args)->value.s;\n");
        output.push_str("    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {\n");
        output.push_str("        if (strcmp(tauraro_memory_state.arenas[i]->name, name) == 0) {\n");
        output.push_str("            tauraro_arena_t* arena = tauraro_memory_state.arenas[i];\n");
        output.push_str("            for (size_t j = 0; j < arena->count; j++) free(arena->buffers[j]);\n");
        output.push_str("            free(arena->buffers);\n");
        output.push_str("            free(arena->sizes);\n");
        output.push_str("            free(arena->name);\n");
        output.push_str("            free(arena);\n");
        output.push_str("            for (size_t j = i; j < tauraro_memory_state.arena_count - 1; j++)\n");
        output.push_str("                tauraro_memory_state.arenas[j] = tauraro_memory_state.arenas[j + 1];\n");
        output.push_str("            tauraro_memory_state.arena_count--;\n");
        output.push_str("            break;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_reset_arena(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1 || (*args)->type != 2) return tauraro_none();\n");
        output.push_str("    char* name = (*args)->value.s;\n");
        output.push_str("    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {\n");
        output.push_str("        if (strcmp(tauraro_memory_state.arenas[i]->name, name) == 0) {\n");
        output.push_str("            tauraro_arena_t* arena = tauraro_memory_state.arenas[i];\n");
        output.push_str("            for (size_t j = 0; j < arena->count; j++) free(arena->buffers[j]);\n");
        output.push_str("            arena->count = 0;\n");
        output.push_str("            break;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_memory_stats(int argc, TauValue** args) {\n");
        output.push_str("    (void)argc; (void)args;\n");
        output.push_str("    tauraro_memory_init();\n");
        output.push_str("    size_t total_manual = 0;\n");
        output.push_str("    for (size_t i = 0; i < tauraro_memory_state.buffer_count; i++)\n");
        output.push_str("        total_manual += tauraro_memory_state.buffer_sizes[i];\n");
        output.push_str("    size_t total_arena = 0;\n");
        output.push_str("    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++)\n");
        output.push_str("        for (size_t j = 0; j < tauraro_memory_state.arenas[i]->count; j++)\n");
        output.push_str("            total_arena += tauraro_memory_state.arenas[i]->sizes[j];\n");
        output.push_str("    char buffer[512];\n");
        output.push_str("    snprintf(buffer, sizeof(buffer), \"Memory Strategy: Manual\\nManual Buffers: %zu (%zu bytes)\\nArenas: %zu (%zu bytes)\",\n");
        output.push_str("        tauraro_memory_state.buffer_count, total_manual, tauraro_memory_state.arena_count, total_arena);\n");
        output.push_str("    return tauraro_str(buffer);\n");
        output.push_str("}\n\n");

        // ===== SYSTEM PROGRAMMING FUNCTIONS =====
        output.push_str("TauValue tauraro_sizeof(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1) return tauraro_int(0);\n");
        output.push_str("    switch ((*args)->type) {\n");
        output.push_str("        case 0: return tauraro_int(sizeof(long long));\n");
        output.push_str("        case 1: return tauraro_int(sizeof(double));\n");
        output.push_str("        case 3: return tauraro_int(sizeof(int));\n");
        output.push_str("        case 2: return tauraro_int((*args)->value.s ? strlen((*args)->value.s) + 1 : 0);\n");
        output.push_str("        default: return tauraro_int(sizeof(TauValue));\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_alignof(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1) return tauraro_int(0);\n");
        output.push_str("    switch ((*args)->type) {\n");
        output.push_str("        case 0: return tauraro_int(_Alignof(long long));\n");
        output.push_str("        case 1: return tauraro_int(_Alignof(double));\n");
        output.push_str("        default: return tauraro_int(_Alignof(void*));\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_memcpy(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 3) return tauraro_none();\n");
        output.push_str("    void* dest = (void*)(uintptr_t)args[0]->value.i;\n");
        output.push_str("    void* src = (void*)(uintptr_t)args[1]->value.i;\n");
        output.push_str("    size_t n = (size_t)args[2]->value.i;\n");
        output.push_str("    if (dest && src && n > 0) memcpy(dest, src, n);\n");
        output.push_str("    return tauraro_int((long long)(uintptr_t)dest);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_memset(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 3) return tauraro_none();\n");
        output.push_str("    void* dest = (void*)(uintptr_t)args[0]->value.i;\n");
        output.push_str("    int value = (int)args[1]->value.i;\n");
        output.push_str("    size_t n = (size_t)args[2]->value.i;\n");
        output.push_str("    if (dest && n > 0) memset(dest, value, n);\n");
        output.push_str("    return tauraro_int((long long)(uintptr_t)dest);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_memmove(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 3) return tauraro_none();\n");
        output.push_str("    void* dest = (void*)(uintptr_t)args[0]->value.i;\n");
        output.push_str("    void* src = (void*)(uintptr_t)args[1]->value.i;\n");
        output.push_str("    size_t n = (size_t)args[2]->value.i;\n");
        output.push_str("    if (dest && src && n > 0) memmove(dest, src, n);\n");
        output.push_str("    return tauraro_int((long long)(uintptr_t)dest);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_memcmp(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 3) return tauraro_int(-1);\n");
        output.push_str("    void* s1 = (void*)(uintptr_t)args[0]->value.i;\n");
        output.push_str("    void* s2 = (void*)(uintptr_t)args[1]->value.i;\n");
        output.push_str("    size_t n = (size_t)args[2]->value.i;\n");
        output.push_str("    if (s1 && s2 && n > 0) return tauraro_int(memcmp(s1, s2, n));\n");
        output.push_str("    return tauraro_int(-1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_ptr_read(int argc, TauValue** args) {\n");
        output.push_str("    if (argc < 1) return tauraro_none();\n");
        output.push_str("    void* ptr = (void*)(uintptr_t)args[0]->value.i;\n");
        output.push_str("    int byte_size = (argc > 1) ? (int)args[1]->value.i : 8;\n");
        output.push_str("    if (!ptr) return tauraro_int(0);\n");
        output.push_str("    switch (byte_size) {\n");
        output.push_str("        case 1: return tauraro_int(*(int8_t*)ptr);\n");
        output.push_str("        case 2: return tauraro_int(*(int16_t*)ptr);\n");
        output.push_str("        case 4: return tauraro_int(*(int32_t*)ptr);\n");
        output.push_str("        default: return tauraro_int(*(int64_t*)ptr);\n");
        output.push_str("    }\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_ptr_write(int argc, TauValue** args) {\n");
        output.push_str("    if (argc < 2) return tauraro_none();\n");
        output.push_str("    void* ptr = (void*)(uintptr_t)args[0]->value.i;\n");
        output.push_str("    long long value = args[1]->value.i;\n");
        output.push_str("    int byte_size = (argc > 2) ? (int)args[2]->value.i : 8;\n");
        output.push_str("    if (ptr) {\n");
        output.push_str("        switch (byte_size) {\n");
        output.push_str("            case 1: *(int8_t*)ptr = (int8_t)value; break;\n");
        output.push_str("            case 2: *(int16_t*)ptr = (int16_t)value; break;\n");
        output.push_str("            case 4: *(int32_t*)ptr = (int32_t)value; break;\n");
        output.push_str("            default: *(int64_t*)ptr = (int64_t)value; break;\n");
        output.push_str("        }\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_none();\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_ptr_offset(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 2) return tauraro_int(0);\n");
        output.push_str("    uintptr_t ptr = (uintptr_t)args[0]->value.i;\n");
        output.push_str("    long long offset = args[1]->value.i;\n");
        output.push_str("    return tauraro_int((long long)(ptr + offset));\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_null_ptr(int argc, TauValue** args) {\n");
        output.push_str("    (void)argc; (void)args;\n");
        output.push_str("    return tauraro_int(0);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_is_null(int argc, TauValue** args) {\n");
        output.push_str("    if (argc != 1) return tauraro_bool(1);\n");
        output.push_str("    return tauraro_bool(args[0]->value.i == 0);\n");
        output.push_str("}\n\n");

        output
    }

    fn indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }

    fn gen_temp_var(&mut self) -> String {
        let name = format!("_tmp{}", self.temp_var_counter);
        self.temp_var_counter += 1;
        name
    }

    /// First pass: collect all variables and their types from instructions
    fn collect_variables(&mut self, instructions: &[IRInstruction]) {
        for instr in instructions {
            match instr {
                IRInstruction::LoadConst { result, value: _ } => {
                    // Don't mark as native type - temp/arg variables are usually reused
                    // Mark as Generic unless it's explicitly typed
                    if !self.scope_variables.contains_key(result) {
                        self.scope_variables.insert(result.clone(), NativeType::Generic);
                    }
                }
                IRInstruction::LoadLocal { name, result } => {
                    // Only set result type if it hasn't been set to a native type already
                    let existing_type = self.scope_variables.get(result).copied();
                    let should_set = existing_type.is_none() || matches!(existing_type, Some(NativeType::Generic));
                    
                    // Get source variable type
                    let source_type = self.scope_variables.get(name).copied();
                    
                    if should_set {
                        if let Some(var_type) = source_type {
                            self.scope_variables.insert(result.clone(), var_type);
                            // Also update var_types to track native types
                            if !matches!(var_type, NativeType::Generic) {
                                self.var_types.insert(result.clone(), var_type);
                            }
                        } else {
                            self.scope_variables.insert(result.clone(), NativeType::Generic);
                        }
                    }
                    // Also ensure the source variable is declared
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::LoadTypedLocal { name, result, type_info } => {
                    let var_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(result.clone(), var_type);
                    self.var_types.insert(result.clone(), var_type);
                    self.static_typed_vars.insert(result.clone(), type_info.clone());
                    // Source variable should also be present with same type
                    self.scope_variables.entry(name.clone()).or_insert(var_type);
                    self.var_types.entry(name.clone()).or_insert(var_type);
                }
                IRInstruction::LoadGlobal { result, .. } => {
                    // Only set result type if it hasn't been set to a native type already
                    // This prevents overwriting typed variables when they're reused
                    // Check BOTH scope_variables and var_types before overwriting
                    let existing_type = self.scope_variables.get(result).copied();
                    let var_type_existing = self.var_types.get(result).copied();
                    let should_overwrite = existing_type.is_none() || matches!(existing_type, Some(NativeType::Generic));
                    let has_native_in_var_types = var_type_existing.is_some() && !matches!(var_type_existing, Some(NativeType::Generic));
                    if should_overwrite && !has_native_in_var_types {
                        self.scope_variables.insert(result.clone(), NativeType::Generic);
                    }
                }
                IRInstruction::LoadTypedGlobal { result, type_info, .. } => {
                    let var_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(result.clone(), var_type);
                    self.var_types.insert(result.clone(), var_type);
                    self.static_typed_vars.insert(result.clone(), type_info.clone());
                }
                IRInstruction::StoreLocal { name, value } => {
                    // Keep destination's type if it has one, otherwise infer from source
                    // IMPORTANT: Don't override source's type just because target has a type
                    // The transpiler will handle type conversions
                    let target_type = self.scope_variables.get(name).copied()
                        .or_else(|| self.var_types.get(name).copied());
                    
                    let source_type = self.var_types.get(value).copied()
                        .or_else(|| self.scope_variables.get(value).copied());
                    
                    if target_type.is_none() && source_type.is_some() {
                        // Source has a type and target doesn't, propagate source type to target
                        self.scope_variables.insert(name.clone(), source_type.unwrap());
                        if !matches!(source_type.unwrap(), NativeType::Generic) {
                            self.var_types.insert(name.clone(), source_type.unwrap());
                        }
                    }
                    
                    // Ensure both are in scope_variables
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                    self.scope_variables.entry(value.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::StoreTypedLocal { name, value, type_info } => {
                    // Get the type from type_info for this variable
                    let var_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(name.clone(), var_type);
                    self.var_types.insert(name.clone(), var_type);
                    self.static_typed_vars.insert(name.clone(), type_info.clone());
                    // DON'T mark source with the destination's type - source might be a temp variable
                    // The transpiler will handle type conversion during assignment
                    self.scope_variables.entry(value.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::TypedBinaryOp { result, type_info, .. } => {
                    // Use the type information from the instruction
                    let result_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(result.clone(), result_type);
                    self.var_types.insert(result.clone(), result_type);
                }
                IRInstruction::BinaryOp { op, left, right, result } => {
                    // Infer result type from operand types
                    let left_type = self.scope_variables.get(left).copied()
                        .or_else(|| self.var_types.get(left).copied())
                        .unwrap_or(NativeType::Generic);
                    let right_type = self.scope_variables.get(right).copied()
                        .or_else(|| self.var_types.get(right).copied())
                        .unwrap_or(NativeType::Generic);
                    
                    // Infer result type based on operand types and operation
                    let result_type = self.infer_binary_op_type(left_type, right_type, op.clone());
                    
                    // Only assign native type to result if both operands are native types
                    if matches!(left_type, NativeType::Int64 | NativeType::Double) && 
                       matches!(right_type, NativeType::Int64 | NativeType::Double) {
                        self.scope_variables.insert(result.clone(), result_type);
                        self.var_types.insert(result.clone(), result_type);
                    } else {
                        self.scope_variables.insert(result.clone(), NativeType::Generic);
                    }
                }
                IRInstruction::Call { func, result, .. } => {
                    if let Some(res) = result {
                        // Always keep temporary variables as Generic since they're declared as TauValue
                        // and may be used to store wrapped values
                        if res == "temp" || res == "temp_result" || res.starts_with("temp_") {
                            self.scope_variables.insert(res.clone(), NativeType::Generic);
                        } else if let Some(func_info) = self.function_definitions.get(func) {
                            // For named result variables, use function return type
                            let return_type = func_info.return_type;
                            if !matches!(return_type, NativeType::Generic) {
                                self.scope_variables.insert(res.clone(), return_type);
                                self.var_types.insert(res.clone(), return_type);
                            } else {
                                self.scope_variables.insert(res.clone(), NativeType::Generic);
                            }
                        } else {
                            self.scope_variables.insert(res.clone(), NativeType::Generic);
                        }
                    }
                }
                IRInstruction::ObjectCreate { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::ObjectGetAttr { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::ListCreate { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::DictCreate { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::DictGetItem { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::If { then_body, elif_branches, else_body, .. } => {
                    self.collect_variables(then_body);
                    for (_, elif_instrs) in elif_branches {
                        self.collect_variables(elif_instrs);
                    }
                    if let Some(else_instrs) = else_body {
                        self.collect_variables(else_instrs);
                    }
                }
                IRInstruction::While { condition_instructions, body, .. } => {
                    // Collect variables from condition evaluation instructions
                    self.collect_variables(condition_instructions);
                    self.collect_variables(body);
                }
                IRInstruction::For { body, .. } => {
                    self.collect_variables(body);
                }
                IRInstruction::StoreGlobal { name, value } => {
                    // Collect both the target and source variables
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                    self.scope_variables.entry(value.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::StoreTypedGlobal { name, value, type_info } => {
                    // Collect both the target and source variables
                    let var_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(name.clone(), var_type);
                    self.var_types.insert(name.clone(), var_type);
                    self.static_typed_vars.insert(name.clone(), type_info.clone());
                    self.scope_variables.entry(value.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::Lambda { result, .. } => {
                    // Collect the result variable where the lambda function pointer is stored
                    self.scope_variables.insert(result.clone(), NativeType::Function);
                }
                _ => {}
            }
        }
    }

    /// Explicitly collect all StoreLocal/StoreGlobal target names to ensure user variables are declared
    fn collect_store_local_targets(&mut self, instructions: &[IRInstruction]) {
        for instr in instructions {
            match instr {
                IRInstruction::StoreLocal { name, .. } => {
                    // Ensure this variable is in scope_variables
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::StoreTypedLocal { name, type_info, .. } => {
                    // Ensure this typed variable is in scope_variables with its type
                    let var_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(name.clone(), var_type);
                    self.var_types.insert(name.clone(), var_type);
                    self.static_typed_vars.insert(name.clone(), type_info.clone());
                }
                IRInstruction::StoreGlobal { name, .. } => {
                    // For globals in main function, also collect them
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::StoreTypedGlobal { name, type_info, .. } => {
                    // For typed globals in main function, also collect them
                    let var_type = self.ast_type_to_native(type_info);
                    self.scope_variables.insert(name.clone(), var_type);
                    self.var_types.insert(name.clone(), var_type);
                    self.static_typed_vars.insert(name.clone(), type_info.clone());
                }
                IRInstruction::If { then_body, elif_branches, else_body, .. } => {
                    self.collect_store_local_targets(then_body);
                    for (_, elif_instrs) in elif_branches {
                        self.collect_store_local_targets(elif_instrs);
                    }
                    if let Some(else_instrs) = else_body {
                        self.collect_store_local_targets(else_instrs);
                    }
                }
                IRInstruction::While { body, .. } => {
                    self.collect_store_local_targets(body);
                }
                IRInstruction::For { body, .. } => {
                    self.collect_store_local_targets(body);
                }
                _ => {}
            }
        }
    }

    /// Generate variable declarations for the current scope with unique names
    fn generate_variable_declarations(&mut self) -> String {
        let mut output = String::new();
        let ind = self.indent();

        // Group variables by their native type
        let mut tauvalue_vars: Vec<String> = Vec::new();
        let mut int64_vars: Vec<String> = Vec::new();
        let mut double_vars: Vec<String> = Vec::new();
        let mut cstr_vars: Vec<String> = Vec::new();
        let mut unique_vars: HashMap<String, NativeType> = HashMap::new();
        
        for (var_name, var_type) in &self.scope_variables {
            // Skip function parameters - they're already declared in the signature
            if self.function_parameters.contains(var_name) {
                continue;
            }

            // For temp variables: only force to Generic if they were already Generic
            // If they have a native type from TypedBinaryOp, keep that type
            let final_type = if var_name.starts_with("var_") && var_name.ends_with("_temp") 
                              && matches!(var_type, NativeType::Generic) {
                NativeType::Generic
            } else {
                *var_type
            };

            let unique_name = if unique_vars.contains_key(var_name) {
                // Generate unique name if there's already a variable with this name
                let mut counter = 1;
                let mut candidate = format!("{}_v{}", var_name, counter);
                while unique_vars.contains_key(&candidate) {
                    counter += 1;
                    candidate = format!("{}_v{}", var_name, counter);
                }
                self.var_name_mapping.insert(var_name.clone(), candidate.clone());
                candidate
            } else {
                self.var_name_mapping.insert(var_name.clone(), var_name.clone());
                var_name.clone()
            };
            
            unique_vars.insert(unique_name.clone(), final_type);
            // Mark this variable as declared so ensure_var_declared won't redeclare it
            self.declared_vars.insert(unique_name.clone());
            
            // Group by type
            match final_type {
                NativeType::Int64 => int64_vars.push(unique_name),
                NativeType::Double => double_vars.push(unique_name),
                NativeType::CStr => cstr_vars.push(unique_name),
                _ => tauvalue_vars.push(unique_name),
            }
        }

        // Output declarations grouped by type
        if !int64_vars.is_empty() {
            let var_list = int64_vars.join(", ");
            output.push_str(&format!("{}long long {};\n", ind, var_list));
        }
        if !double_vars.is_empty() {
            let var_list = double_vars.join(", ");
            output.push_str(&format!("{}double {};\n", ind, var_list));
        }
        if !cstr_vars.is_empty() {
            let var_list = cstr_vars.join(", ");
            output.push_str(&format!("{}const char* {};\n", ind, var_list));
        }
        if !tauvalue_vars.is_empty() {
            let var_list = tauvalue_vars.join(", ");
            output.push_str(&format!("{}TauValue {};\n", ind, var_list));
        }

        output
    }
}

impl Clone for CTranspiler {
    fn clone(&self) -> Self {
        CTranspiler {
            var_types: self.var_types.clone(),
            declared_vars: self.declared_vars.clone(),
            var_name_mapping: self.var_name_mapping.clone(),
            indent_level: self.indent_level,
            temp_var_counter: self.temp_var_counter,
            scope_variables: self.scope_variables.clone(),
            function_parameters: self.function_parameters.clone(),
            class_definitions: self.class_definitions.clone(),
            function_definitions: self.function_definitions.clone(),
            current_function: self.current_function.clone(),
            loop_depth: self.loop_depth,
            exception_handlers: self.exception_handlers.clone(),
            imports: self.imports.clone(),
            generated_utilities: self.generated_utilities.clone(),
            function_params: self.function_params.clone(),
            lambdas_to_generate: self.lambdas_to_generate.clone(),
            static_typed_vars: self.static_typed_vars.clone(),
            enable_native_types: self.enable_native_types,
        }
    }
}

impl Default for CTranspiler {
    fn default() -> Self {
        Self::new()
    }
}
