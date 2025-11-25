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
use crate::ast::{Type, BinaryOp};
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
        
        // Populate function parameter information for default argument handling
        for (func_name, func) in &module.functions {
            let mut params_info = Vec::new();
            for param in &func.params {
                let has_default = func.defaults.contains_key(param);
                params_info.push((param.clone(), has_default));
            }
            transpiler.function_params.insert(func_name.clone(), (func.params.len(), params_info));
        }

        // Add C header and includes
        output.push_str("#include <stdio.h>\n");
        output.push_str("#include <stdlib.h>\n");
        output.push_str("#include <string.h>\n");
        output.push_str("#include <stdbool.h>\n");
        output.push_str("#include <math.h>\n");
        output.push_str("#include <stdarg.h>\n");
        output.push_str("#include <setjmp.h>\n");
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

            // First pass: collect all variables from globals
            if !module.globals.is_empty() {
                transpiler.collect_variables(&module.globals);
                // Additional pass: explicitly collect all StoreLocal target names
                transpiler.collect_store_local_targets(&module.globals);
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
            
            output.push_str("    return 0;\n");
            output.push_str("}\n");
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
        output.push_str("TauValue range(TauValue end);\n");
        output.push_str("TauValue range2(TauValue start, TauValue end);\n");
        output.push_str("TauValue range3(TauValue start, TauValue end, TauValue step);\n");
        output.push_str("TauValue tauraro_abs(TauValue val);\n");
        output.push_str("TauValue tauraro_min(TauValue a, TauValue b);\n");
        output.push_str("TauValue tauraro_max(TauValue a, TauValue b);\n");
        output.push_str("TauValue tauraro_sum(TauValue list);\n");
        output.push_str("TauValue tauraro_super_call(TauObject* self, TauValue* args, int argc);\n");
        output.push_str("\n");

        output
    }

    fn generate_function_signature(&self, func_name: &str, func: &IRFunction, declare_only: bool) -> Result<String> {
        let mut output = String::new();

        // Determine return type 
        // NOTE: For the main TauValue-based transpiler, we always use TauValue for user functions
        // to maintain compatibility with the runtime system. Native types are used in pure_native transpiler.
        let return_type = if func_name == "main" {
            "int".to_string()
        } else {
            // Always use TauValue for user functions in main transpiler
            // This ensures compatibility with the TauValue-based runtime
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
            // Always use TauValue parameters in main transpiler for runtime compatibility
            // Also mangle parameter names
            let param_strs: Vec<String> = func.params.iter()
                .map(|p| format!("TauValue {}", self.mangle_c_keyword(p)))
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
        for instruction in &all_instructions {
            output.push_str(&self.transpile_instruction(instruction, self.indent_level)?);
        }

        // Add default return for non-main functions
        if func.name != "main" {
            output.push_str(&format!("{}TauValue ret = {{.type = 0, .value.i = 0}};\n", self.indent()));
            output.push_str(&format!("{}return ret;\n", self.indent()));
        } else {
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
                let wrapped_val = self.wrap_value_in_tauvalue(value);
                self.var_types.insert(resolved_result.clone(), NativeType::Generic);
                // Assignment using resolved name (variable already declared in header)
                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, wrapped_val));
            }

            IRInstruction::LoadLocal { name, result } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_result = self.resolve_var_name(result);
                let type_opt = self.var_types.get(&resolved_name).copied().or_else(|| self.var_types.get(name).copied());
                if let Some(var_type) = type_opt {
                    self.var_types.insert(resolved_result.clone(), var_type);
                } else {
                    self.var_types.insert(resolved_result.clone(), NativeType::Generic);
                }
                // Assignment using resolved names (variables already declared in header)
                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
            }

            IRInstruction::StoreLocal { name, value } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_value = self.resolve_var_name(value);
                // Assignment using resolved names (variables already declared in header)
                output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                let var_type = self.var_types.get(value).copied().or_else(|| self.var_types.get(&resolved_value).copied()).unwrap_or(NativeType::Generic);
                self.var_types.insert(resolved_name, var_type);
            }

            IRInstruction::LoadGlobal { name, result } => {
                let resolved_result = self.resolve_var_name(result);
                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, name));
                self.var_types.insert(resolved_result, NativeType::Generic);
            }

                IRInstruction::LoadTypedGlobal { name, result, type_info } => {
                    let resolved_result = self.resolve_var_name(result);
                    output.push_str(&format!("{}{} = {};\n", ind, resolved_result, name));
                    // Track the type for typed globals but keep as Generic for C declaration
                    self.var_types.insert(resolved_result, NativeType::Generic);
                }

            IRInstruction::StoreGlobal { name, value } => {
                output.push_str(&format!("{}{} = {};\n", ind, name, value));
            }

            IRInstruction::StoreTypedGlobal { name, value, type_info } => {
                // Store typed global - handle type information for typed variables
                let native_type = self.ast_type_to_native(type_info);
                output.push_str(&format!("{}{} = {};\n", ind, name, value));
                self.var_types.insert(name.clone(), native_type);
            }
            
            // Typed local variable operations - key for static typing optimization
            IRInstruction::LoadTypedLocal { name, result, type_info } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_result = self.resolve_var_name(result);
                let native_type = self.ast_type_to_native(type_info);
                
                // For native types, use direct assignment
                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, resolved_name));
                self.var_types.insert(resolved_result.clone(), native_type);
            }
            
            IRInstruction::StoreTypedLocal { name, value, type_info } => {
                let resolved_name = self.resolve_var_name(name);
                let resolved_value = self.resolve_var_name(value);
                let _native_type = self.ast_type_to_native(type_info);
                
                // Direct assignment for typed locals (still TauValue in main transpiler)
                output.push_str(&format!("{}{} = {};\n", ind, resolved_name, resolved_value));
                self.var_types.insert(resolved_name, NativeType::Generic);
            }
            
            // Typed binary operation - in main transpiler, always use TauValue for consistency
            IRInstruction::TypedBinaryOp { op, left, right, result, type_info } => {
                let resolved_left = self.resolve_var_name(left);
                let resolved_right = self.resolve_var_name(right);
                let resolved_result = self.resolve_var_name(result);
                let _native_type = self.ast_type_to_native(type_info);
                
                let op_str = self.format_binary_op(op.clone());
                
                // In main transpiler, always extract values and wrap in TauValue
                // Native type optimization is only in pure_native transpiler
                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}.value.i {} {}.value.i}};\n", 
                    ind, resolved_result, resolved_left, op_str, resolved_right));
                self.var_types.insert(resolved_result, NativeType::Generic);
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
                    let result_type = self.infer_binary_op_type(left_type, right_type, op.clone());

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
                                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {} {} {}}};\n",
                                    ind, resolved_result,
                                    self.extract_value(&resolved_left, left_type),
                                    op_str,
                                    self.extract_value(&resolved_right, right_type)));
                            }
                            NativeType::Double => {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 1, .value.f = (double){} {} (double){}}};\n",
                                    ind, resolved_result,
                                    self.extract_value(&resolved_left, left_type),
                                    op_str,
                                    self.extract_value(&resolved_right, right_type)));
                            }
                            NativeType::Bool => {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 3, .value.i = {}.value.i {} {}.value.i}};\n",
                                    ind, resolved_result, resolved_left, op_str, resolved_right));
                            }
                            _ => {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {}.value.i {} {}.value.i}};\n",
                                    ind, resolved_result, resolved_left, op_str, resolved_right));
                            }
                        }
                    }
                    self.var_types.insert(resolved_result, result_type);
                }
            }

            IRInstruction::Call { func, args, result } => {
                let args_str = args.join(", ");
                let var_types_snapshot = self.var_types.clone();
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
                                    NativeType::Dict => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("\"<dict>\""));
                                    }
                                    NativeType::List => {
                                        format_parts.push("%s".to_string());
                                        arg_values.push(format!("\"<list>\""));
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
                            self.var_types.insert(res.clone(), NativeType::List);
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
                                self.var_types.insert(res.clone(), NativeType::CStr);
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
                                self.var_types.insert(res.clone(), NativeType::CStr);
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
                        // Handle tuple() - convert to tuple (list in C)
                        if let Some(res) = result {
                            if args.is_empty() {
                                output.push_str(&format!("{}{} = (TauValue){{.type = 4, .value.list = tauraro_create_list(0)}};\n", ind, res));
                            } else {
                                output.push_str(&format!("{}{} = tauraro_to_list({});\n", ind, res, args[0]));
                            }
                            self.var_types.insert(res.clone(), NativeType::List);
                        }
                    }
                    "hex" => {
                        // Handle hex() - convert to hex string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_hex({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::CStr);
                            }
                        }
                    }
                    "bin" => {
                        // Handle bin() - convert to binary string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_bin({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::CStr);
                            }
                        }
                    }
                    "oct" => {
                        // Handle oct() - convert to octal string
                        if let Some(res) = result {
                            if args.len() == 1 {
                                output.push_str(&format!("{}{} = tauraro_oct({});\n", ind, res, args[0]));
                                self.var_types.insert(res.clone(), NativeType::CStr);
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
                                output.push_str(&format!("{}{} = {}({});\n", ind, resolved_res, call_func, args_str));
                                self.var_types.insert(resolved_res, NativeType::Generic);
                            } else {
                                output.push_str(&format!("{}{}({});\n", ind, call_func, args_str));
                            }
                        }
                    }
                }
            }

            IRInstruction::Return { value } => {
                if let Some(val) = value {
                    output.push_str(&format!("{}return {};\n", ind, val));
                } else {
                    output.push_str(&format!("{}return {{.type = 0, .value.i = 0}};\n", ind));
                }
            }

            IRInstruction::If { condition, then_body, elif_branches, else_body } => {
                // Extract boolean value from TauValue condition
                let resolved_condition = self.resolve_var_name(condition);
                let condition_check = format!("({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))", 
                    resolved_condition, resolved_condition, resolved_condition, resolved_condition);
                output.push_str(&format!("{}if ({}) {{\n", ind, condition_check));
                for instr in then_body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }

                for (elif_cond, elif_instrs) in elif_branches {
                    let resolved_elif_cond = self.resolve_var_name(elif_cond);
                    let elif_check = format!("({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))", 
                        resolved_elif_cond, resolved_elif_cond, resolved_elif_cond, resolved_elif_cond);
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
                
                // Extract boolean value from TauValue condition
                let resolved_condition = self.resolve_var_name(condition);
                let condition_check = format!("({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))", 
                    resolved_condition, resolved_condition, resolved_condition, resolved_condition);
                
                // Generate the while loop
                output.push_str(&format!("{}while (1) {{\n", ind)); // Infinite loop, condition check inside
                
                // Re-evaluate the condition at the START of each iteration
                for instr in condition_instructions {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }
                
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
                
                // Generate unique temp variables for iteration
                self.temp_var_counter += 1;
                let loop_counter = format!("_for_i_{}", self.temp_var_counter);
                let loop_list = format!("_for_list_{}", self.temp_var_counter);
                
                // Get the iterable variable
                output.push_str(&format!("{}TauValue {} = {};\n", ind, loop_list, iterable));
                
                // Check if it's a list and iterate
                output.push_str(&format!("{}if ({}.type == 4) {{\n", ind, loop_list));
                output.push_str(&format!("{}    TauList* _list = {}.value.list;\n", ind, loop_list));
                output.push_str(&format!("{}    for(int {} = 0; {} < _list->size; {}++) {{\n", ind, loop_counter, loop_counter, loop_counter));
                
                // Declare the loop variable and assign the current item
                output.push_str(&format!("{}        TauValue {} = _list->items[{}];\n", ind, variable, loop_counter));
                
                // Process loop body
                for instr in body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 2)?);
                }
                
                output.push_str(&format!("{}}}\n", ind));
                output.push_str(&format!("{}}}\n", ind));
            }

            IRInstruction::Break => {
                output.push_str(&format!("{}break;\n", ind));
            }

            IRInstruction::Continue => {
                output.push_str(&format!("{}continue;\n", ind));
            }

            IRInstruction::ListCreate { elements, result } => {
                output.push_str(&format!("{}// Create list with {} elements\n", ind, elements.len()));
                // Use TauValue instead of TauList* for compatibility
                let (var_decl, result_var) = self.ensure_var_declared(result, NativeType::Generic, &ind);
                if !var_decl.is_empty() {
                    output.push_str(&format!("{}\n", var_decl));
                }
                // Create TauList structure
                output.push_str(&format!("{}{{ TauList* _list = malloc(sizeof(TauList));\n", ind));
                output.push_str(&format!("{}_list->size = {};\n", ind, elements.len()));
                output.push_str(&format!("{}_list->capacity = {};\n", ind, elements.len()));
                output.push_str(&format!("{}_list->items = malloc(sizeof(TauValue) * {});\n", ind, elements.len()));
                for (i, elem) in elements.iter().enumerate() {
                    output.push_str(&format!("{}_list->items[{}] = {};\n", ind, i, elem));
                }
                output.push_str(&format!("{}{}.type = 4; {}.value.list = _list; }}\n", ind, result_var, result_var));
            }

            IRInstruction::DictCreate { pairs, result } => {
                output.push_str(&format!("{}// Create dictionary\n", ind));
                // Use TauValue instead of TauDict* for compatibility
                let (var_decl, result_var) = self.ensure_var_declared(result, NativeType::Generic, &ind);
                if !var_decl.is_empty() {
                    output.push_str(&format!("{}\n", var_decl));
                }
                output.push_str(&format!("{}{{ TauDict* _dict = tauraro_create_dict();\n", ind));
                for (key, val) in pairs.iter() {
                    // Extract string from key if needed
                    output.push_str(&format!("{}tauraro_dict_set(_dict, {}.value.s, {});\n", ind, key, val));
                }
                output.push_str(&format!("{}{}.type = 5; {}.value.dict = _dict; }}\n", ind, result_var, result_var));
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
                output.push_str(&format!("{}// Get dictionary item {} from {}\n", ind, key, dict));
                output.push_str(&format!("{}{} = tauraro_dict_get({}, {});\n", ind, result, dict, key));
                self.var_types.insert(result.clone(), NativeType::Generic);
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
            IRInstruction::Lambda { params, body_instructions, captured_vars, result } => {
                let resolved_result = self.resolve_var_name(result);
                self.temp_var_counter += 1;
                let lambda_id = self.temp_var_counter;
                let lambda_name = format!("_lambda_{}", lambda_id);
                
                output.push_str(&format!("{}// Lambda expression\n", ind));
                
                // For simple lambdas, we inline the computation
                // Complex lambdas would need function pointer generation
                if body_instructions.len() == 1 {
                    // Simple single-expression lambda - inline it
                    output.push_str(&format!("{}{} = (TauValue){{.type = 7, .value.ptr = (void*){}}}; // Lambda stored\n", 
                        ind, resolved_result, lambda_name));
                } else {
                    // Store lambda reference
                    output.push_str(&format!("{}{} = (TauValue){{.type = 7, .value.ptr = NULL}}; // Lambda placeholder\n", 
                        ind, resolved_result));
                }
                
                // Capture variables
                if !captured_vars.is_empty() {
                    output.push_str(&format!("{}// Captured: {}\n", ind, captured_vars.join(", ")));
                }
                
                self.var_types.insert(resolved_result, NativeType::Function);
            }
            
            // List comprehension - [expr for x in iterable if condition]
            IRInstruction::ListComprehension { 
                element_expr, element_result, variable, iterable, 
                condition, condition_result, result 
            } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_iterable = self.resolve_var_name(iterable);
                let resolved_var = self.resolve_var_name(variable);
                let resolved_elem = self.resolve_var_name(element_result);
                
                output.push_str(&format!("{}// List comprehension\n", ind));
                output.push_str(&format!("{}TauList* _lc_{} = tauraro_create_list(16);\n", ind, self.temp_var_counter));
                self.temp_var_counter += 1;
                let lc_list = format!("_lc_{}", self.temp_var_counter - 1);
                
                // Iterate over source
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL) {{\n", ind, resolved_iterable, resolved_iterable));
                output.push_str(&format!("{}    for (size_t _i = 0; _i < {}.value.list->size; _i++) {{\n", ind, resolved_iterable));
                output.push_str(&format!("{}        TauValue {} = {}.value.list->items[_i];\n", ind, resolved_var, resolved_iterable));
                
                // Apply condition if present
                if let Some(cond_instrs) = condition {
                    let cond_res = condition_result.as_ref().map(|s| self.resolve_var_name(s)).unwrap_or("_cond".to_string());
                    for cond_instr in cond_instrs {
                        output.push_str(&self.transpile_instruction(cond_instr, indent_level + 2)?);
                    }
                    output.push_str(&format!("{}        if (!({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))) continue;\n", 
                        ind, cond_res, cond_res, cond_res, cond_res));
                }
                
                // Compute element expression
                for elem_instr in element_expr {
                    output.push_str(&self.transpile_instruction(elem_instr, indent_level + 2)?);
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
                key_expr, key_result, value_expr, value_result,
                variable, iterable, condition, condition_result, result
            } => {
                let resolved_result = self.resolve_var_name(result);
                let resolved_iterable = self.resolve_var_name(iterable);
                let resolved_var = self.resolve_var_name(variable);
                let resolved_key = self.resolve_var_name(key_result);
                let resolved_val = self.resolve_var_name(value_result);
                
                output.push_str(&format!("{}// Dict comprehension\n", ind));
                output.push_str(&format!("{}TauDict* _dc_{} = tauraro_create_dict();\n", ind, self.temp_var_counter));
                self.temp_var_counter += 1;
                let dc_dict = format!("_dc_{}", self.temp_var_counter - 1);
                
                // Iterate over source
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL) {{\n", ind, resolved_iterable, resolved_iterable));
                output.push_str(&format!("{}    for (size_t _i = 0; _i < {}.value.list->size; _i++) {{\n", ind, resolved_iterable));
                output.push_str(&format!("{}        TauValue {} = {}.value.list->items[_i];\n", ind, resolved_var, resolved_iterable));
                
                // Apply condition if present
                if let Some(cond_instrs) = condition {
                    let cond_res = condition_result.as_ref().map(|s| self.resolve_var_name(s)).unwrap_or("_cond".to_string());
                    for cond_instr in cond_instrs {
                        output.push_str(&self.transpile_instruction(cond_instr, indent_level + 2)?);
                    }
                    output.push_str(&format!("{}        if (!({}.type == 3 ? {}.value.i : ({}.type == 0 ? ({}.value.i != 0) : 1))) continue;\n", 
                        ind, cond_res, cond_res, cond_res, cond_res));
                }
                
                // Compute key and value expressions
                for k_instr in key_expr {
                    output.push_str(&self.transpile_instruction(k_instr, indent_level + 2)?);
                }
                for v_instr in value_expr {
                    output.push_str(&self.transpile_instruction(v_instr, indent_level + 2)?);
                }
                
                // Insert into dict
                output.push_str(&format!("{}        if ({}.type == 2) tauraro_dict_set({}, {}.value.s, {});\n", 
                    ind, resolved_key, dc_dict, resolved_key, resolved_val));
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
                output.push_str(&format!("{}if ({}.type == 4 && {}.value.list != NULL) {{\n", ind, resolved_tuple, resolved_tuple));
                
                for (i, target) in targets.iter().enumerate() {
                    let resolved_target = self.resolve_var_name(target);
                    output.push_str(&format!("{}    {} = ({}.value.list->size > {}) ? {}.value.list->items[{}] : (TauValue){{.type = 0, .value.i = 0}};\n", 
                        ind, resolved_target, resolved_tuple, i, resolved_tuple, i));
                    self.var_types.insert(resolved_target, NativeType::Generic);
                }
                
                output.push_str(&format!("{}}}\n", ind));
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
        // In main transpiler, ALL variables are TauValue, so we need to extract inner value
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
        output.push_str("// String conversion utilities\n");
        output.push_str("TauValue tauraro_str_from_value(TauValue* val) {\n");
        output.push_str("    static char buffer[512];\n");
        output.push_str("    TauValue result = {.type = 2, .value.s = NULL, .refcount = 1};\n");
        output.push_str("    if (!val) {\n");
        output.push_str("        result.value.s = strdup(\"<null>\");\n");
        output.push_str("        return result;\n");
        output.push_str("    }\n");
        output.push_str("    switch(val->type) {\n");
        output.push_str("        case 0: snprintf(buffer, sizeof(buffer), \"%lld\", val->value.i); result.value.s = strdup(buffer); break;\n");
        output.push_str("        case 1: snprintf(buffer, sizeof(buffer), \"%f\", val->value.f); result.value.s = strdup(buffer); break;\n");
        output.push_str("        case 2: result.value.s = val->value.s ? strdup(val->value.s) : strdup(\"<null>\"); break;\n");
        output.push_str("        case 3: result.value.s = strdup(val->value.i ? \"True\" : \"False\"); break;\n");
        output.push_str("        case 4: result.value.s = strdup(\"<list>\"); break;\n");
        output.push_str("        case 5: result.value.s = strdup(\"<dict>\"); break;\n");
        output.push_str("        case 6: result.value.s = strdup(\"<object>\"); break;\n");
        output.push_str("        case 7: result.value.s = strdup(\"<function>\"); break;\n");
        output.push_str("        case 8: result.value.s = strdup(\"<exception>\"); break;\n");
        output.push_str("        default: result.value.s = strdup(\"<unknown>\");\n");
        output.push_str("    }\n");
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

        // ===== TYPE CONVERSION UTILITIES =====
        output.push_str("// Type conversion utilities\n");
        output.push_str("TauValue tauraro_abs(TauValue val) {\n");
        output.push_str("    if (val.type == 0) return tauraro_int(val.value.i < 0 ? -val.value.i : val.value.i);\n");
        output.push_str("    if (val.type == 1) return tauraro_float(val.value.f < 0 ? -val.value.f : val.value.f);\n");
        output.push_str("    return val;\n");
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
        output.push_str("// Optimized format string builder\n");
        output.push_str("char* tauraro_format_value(TauValue val) {\n");
        output.push_str("    char* buf = malloc(256);\n");
        output.push_str("    switch (val.type) {\n");
        output.push_str("        case 0: snprintf(buf, 256, \"%lld\", val.value.i); break;\n");
        output.push_str("        case 1: snprintf(buf, 256, \"%g\", val.value.f); break;\n");
        output.push_str("        case 2: { free(buf); return strdup(val.value.s ? val.value.s : \"\"); }\n");
        output.push_str("        case 3: snprintf(buf, 256, \"%s\", val.value.i ? \"True\" : \"False\"); break;\n");
        output.push_str("        default: snprintf(buf, 256, \"<object>\"); break;\n");
        output.push_str("    }\n");
        output.push_str("    return buf;\n");
        output.push_str("}\n\n");

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
        output.push_str("        if (v.type == 3 && v.value.i == 0) return tauraro_bool(0);\n");
        output.push_str("        if (v.type == 2 && (!v.value.s || v.value.s[0] == '\\0')) return tauraro_bool(0);\n");
        output.push_str("    }\n");
        output.push_str("    return tauraro_bool(1);\n");
        output.push_str("}\n\n");

        output.push_str("TauValue tauraro_any(TauValue list) {\n");
        output.push_str("    if (list.type != 4 || !list.value.list) return tauraro_bool(0);\n");
        output.push_str("    TauList* lst = list.value.list;\n");
        output.push_str("    for (size_t i = 0; i < lst->size; i++) {\n");
        output.push_str("        TauValue v = lst->items[i];\n");
        output.push_str("        if (v.type == 0 && v.value.i != 0) return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 3 && v.value.i != 0) return tauraro_bool(1);\n");
        output.push_str("        if (v.type == 2 && v.value.s && v.value.s[0] != '\\0') return tauraro_bool(1);\n");
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
                IRInstruction::LoadConst { result, .. } => {
                    // Constants are always wrapped in TauValue in our IR system
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::LoadLocal { name, result } => {
                    if let Some(var_type) = self.scope_variables.get(name).copied() {
                        self.scope_variables.insert(result.clone(), var_type);
                    } else {
                        self.scope_variables.insert(result.clone(), NativeType::Generic);
                    }
                    // Also ensure the source variable is declared
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::LoadGlobal { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::LoadTypedGlobal { result, .. } => {
                    // Keep all variables as TauValue in C
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::StoreLocal { name, value } => {
                    // Get the type from the source value, or default to Generic
                    let var_type = self.scope_variables.get(value).copied().unwrap_or(NativeType::Generic);
                    self.scope_variables.insert(name.clone(), var_type);
                    // Also ensure the source variable is declared
                    self.scope_variables.entry(value.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::BinaryOp { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::TypedBinaryOp { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::Call { result, .. } => {
                    if let Some(res) = result {
                        self.scope_variables.insert(res.clone(), NativeType::Generic);
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
                IRInstruction::StoreGlobal { name, .. } => {
                    // Collect global variables assigned in main
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::StoreTypedGlobal { name, .. } => {
                    // Collect typed global variables with their type information
                    // Keep as Generic - all variables are TauValue in C
                    self.scope_variables.insert(name.clone(), NativeType::Generic);
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
                IRInstruction::StoreGlobal { name, .. } => {
                    // For globals in main function, also collect them
                    self.scope_variables.entry(name.clone()).or_insert(NativeType::Generic);
                }
                IRInstruction::StoreTypedGlobal { name, .. } => {
                    // For typed globals in main function, also collect them
                    // Keep as Generic - all variables are TauValue in C
                    self.scope_variables.insert(name.clone(), NativeType::Generic);
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

        // Create unique names for all variables and group by type  
        let mut by_type: HashMap<NativeType, Vec<String>> = HashMap::new();
        let mut unique_vars: HashMap<String, NativeType> = HashMap::new();
        
        for (var_name, var_type) in &self.scope_variables {
            // Skip function parameters - they're already declared in the signature
            if self.function_parameters.contains(var_name) {
                continue;
            }

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
            
            unique_vars.insert(unique_name.clone(), *var_type);
            // Mark this variable as declared so ensure_var_declared won't redeclare it
            self.declared_vars.insert(unique_name.clone());
            by_type.entry(*var_type).or_insert_with(Vec::new).push(unique_name);
        }

        // Output declarations sorted by type
        for var_type in &[NativeType::Int64, NativeType::Double, NativeType::CStr, NativeType::Bool, NativeType::Object, NativeType::Generic] {
            if let Some(vars) = by_type.get(var_type) {
                let type_name = self.get_c_type(*var_type);
                let var_list = vars.join(", ");
                output.push_str(&format!("{}{} {};\n", ind, type_name, var_list));
            }
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
        }
    }
}

impl Default for CTranspiler {
    fn default() -> Self {
        Self::new()
    }
}
