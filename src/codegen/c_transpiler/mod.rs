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
    /// Track which utility functions have been generated
    generated_utilities: HashSet<String>,
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
            class_definitions: HashMap::new(),
            function_definitions: HashMap::new(),
            current_function: None,
            loop_depth: 0,
            exception_handlers: Vec::new(),
            imports: HashMap::new(),
            generated_utilities: HashSet::new(),
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

    /// Resolve variable name to unique name (read-only lookup)
    fn resolve_var_name(&self, original_name: &str) -> String {
        self.var_name_mapping.get(original_name).cloned().unwrap_or_else(|| original_name.to_string())
    }

    /// Declare a variable if not already declared
    fn ensure_var_declared(&mut self, var_name: &str, var_type: NativeType, indent: &str) -> String {
        if !self.declared_vars.contains(var_name) {
            self.declared_vars.insert(var_name.to_string());
            self.var_types.insert(var_name.to_string(), var_type);
            format!("{}{} {};", indent, self.format_type(var_type), var_name)
        } else {
            String::new()
        }
    }

    /// Transpile an IR module to optimized C code
    pub fn transpile(&self, module: &IRModule) -> Result<String> {
        let mut transpiler = self.clone();
        let mut output = String::new();

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
            }

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
        output.push_str("\n");

        output
    }

    fn generate_function_signature(&self, func_name: &str, func: &IRFunction, declare_only: bool) -> Result<String> {
        let mut output = String::new();

        // Determine return type
        let return_type = if func_name == "main" {
            "int"
        } else {
            "TauValue"
        };

        output.push_str(return_type);
        output.push_str(" ");
        output.push_str(func_name);
        output.push_str("(");

        if func_name == "main" {
            output.push_str("int argc, char* argv[]");
        } else {
            let param_strs: Vec<String> = func.params.iter()
                .map(|p| format!("TauValue {}", p))
                .collect();
            output.push_str(&param_strs.join(", "));
        }

        output.push_str(")");

        if !declare_only {
            output.push_str(" {\n");
        }

        Ok(output)
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

        // Collect all variables needed in this function (first pass)
        let mut all_instructions = Vec::new();
        for block in &func.blocks {
            all_instructions.extend(block.instructions.clone());
        }
        self.collect_variables(&all_instructions);

        // Function signature
        output.push_str(&self.generate_function_signature(&func.name, func, false)?);

        // Generate variable declarations
        output.push_str(&self.generate_variable_declarations());

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
                let (type_name, val_str) = self.format_value(value);
                let resolved_result = self.resolve_var_name(result);
                self.var_types.insert(resolved_result.clone(), type_name);
                // Assignment using resolved name (variable already declared in header)
                output.push_str(&format!("{}{} = {};\n", ind, resolved_result, val_str));
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
                output.push_str(&format!("{}TauValue {} = {};\n", ind, result, name));
                self.declared_vars.insert(result.clone());
            }

            IRInstruction::StoreGlobal { name, value } => {
                output.push_str(&format!("{}{} = {};\n", ind, name, value));
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
                    // Power operations return double by default
                    output.push_str(&format!("{}{} = pow((double){}, (double){});\n",
                        ind, resolved_result,
                        self.extract_value(&resolved_left, left_type),
                        self.extract_value(&resolved_right, right_type)));
                    self.var_types.insert(resolved_result, NativeType::Double);
                } else if matches!(op, BinaryOp::Add) && 
                         (matches!(left_type, NativeType::CStr) || matches!(right_type, NativeType::CStr)) {
                    // String concatenation
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
                    output.push_str(&format!("{}{} = strdup({});\n", ind, resolved_result, temp_var));
                    self.var_types.insert(resolved_result, NativeType::CStr);
                } else {
                    // Regular binary operations
                    let op_str = self.format_binary_op(op.clone());
                    let result_type = self.infer_binary_op_type(left_type, right_type, op.clone());

                    match result_type {
                        NativeType::Int64 => {
                            output.push_str(&format!("{}{} = {} {} {};\n",
                                ind, resolved_result,
                                self.extract_value(&resolved_left, left_type),
                                op_str,
                                self.extract_value(&resolved_right, right_type)));
                        }
                        NativeType::Double => {
                            output.push_str(&format!("{}{} = (double){} {} (double){};\n",
                                ind, resolved_result,
                                self.extract_value(&resolved_left, left_type),
                                op_str,
                                self.extract_value(&resolved_right, right_type)));
                        }
                        _ => {
                            output.push_str(&format!("{}{} = (TauValue){{.type = 0, .value.i = {} {} {}}};\n",
                                ind, resolved_result, resolved_left, op_str, resolved_right));
                        }
                    }
                    self.var_types.insert(resolved_result, result_type);
                }
            }

            IRInstruction::TypedBinaryOp { op, left, right, result, type_info: _ } => {
                let op_str = self.format_binary_op(op.clone());
                let left_type = self.var_types.get(left).copied().unwrap_or(NativeType::Generic);
                let right_type = self.var_types.get(right).copied().unwrap_or(NativeType::Generic);

                // For typed operations, prefer Int64 for integer operations
                let result_type = NativeType::Int64;

                // Extract values properly based on their types
                let left_val = match left_type {
                    NativeType::Int64 | NativeType::Double | NativeType::Bool => left.clone(),
                    NativeType::CStr => left.clone(),
                    NativeType::Object => left.clone(),
                    NativeType::Dict => left.clone(),
                    NativeType::List => left.clone(),
                    NativeType::Function => left.clone(),
                    NativeType::Iterator => left.clone(),
                    NativeType::Exception => left.clone(),
                    NativeType::Module => left.clone(),
                    NativeType::Generic => format!("{}.value.i", left),
                };
                
                let right_val = match right_type {
                    NativeType::Int64 | NativeType::Double | NativeType::Bool => right.clone(),
                    NativeType::CStr => right.clone(),
                    NativeType::Object => right.clone(),
                    NativeType::Dict => right.clone(),
                    NativeType::List => right.clone(),
                    NativeType::Function => right.clone(),
                    NativeType::Iterator => right.clone(),
                    NativeType::Exception => right.clone(),
                    NativeType::Module => right.clone(),
                    NativeType::Generic => format!("{}.value.i", right),
                };

                let var_decl = self.ensure_var_declared(result, NativeType::Int64, &ind);
                if !var_decl.is_empty() {
                    output.push_str(&format!("{}\n", var_decl));
                    output.push_str(&format!("{}{} = {} {} {};\n", ind, result, left_val, op_str, right_val));
                } else {
                    output.push_str(&format!("{}{} = {} {} {};\n", ind, result, left_val, op_str, right_val));
                }
                
                self.var_types.insert(result.clone(), result_type);
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
                                        arg_values.push(format!("({} ? \"true\" : \"false\")", arg));
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
                                        arg_values.push(format!("tauraro_str_from_value(&{})", arg));
                                    }
                                }
                            } else {
                                // Unknown type, treat as string
                                format_parts.push("%s".to_string());
                                arg_values.push(format!("tauraro_str_from_value(&{})", arg));
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
                                            output.push_str(&format!("{}TauValue {} = tauraro_str_int({});\n", ind, res, arg));
                                        }
                                        NativeType::Double => {
                                            output.push_str(&format!("{}TauValue {} = tauraro_str_double({});\n", ind, res, arg));
                                        }
                                        _ => {
                                            output.push_str(&format!("{}TauValue {} = tauraro_str_from_value(&{});\n", ind, res, arg));
                                        }
                                    }
                                } else {
                                    output.push_str(&format!("{}TauValue {} = tauraro_str_from_value(&{});\n", ind, res, arg));
                                }
                                self.declared_vars.insert(res.clone());
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
                                            output.push_str(&format!("{}TauValue {} = tauraro_int_string({});\n", ind, res, arg));
                                        }
                                        _ => {
                                            output.push_str(&format!("{}TauValue {} = tauraro_int({});\n", ind, res, arg));
                                        }
                                    }
                                } else {
                                    output.push_str(&format!("{}TauValue {} = tauraro_int({});\n", ind, res, arg));
                                }
                                self.declared_vars.insert(res.clone());
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
                                            output.push_str(&format!("{}TauValue {} = tauraro_float_string({});\n", ind, res, arg));
                                        }
                                        _ => {
                                            output.push_str(&format!("{}TauValue {} = tauraro_float({});\n", ind, res, arg));
                                        }
                                    }
                                } else {
                                    output.push_str(&format!("{}TauValue {} = tauraro_float({});\n", ind, res, arg));
                                }
                                self.declared_vars.insert(res.clone());
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    "len" => {
                        // Handle len() for different types
                        if let Some(res) = result {
                            if args.len() == 1 {
                                let arg = &args[0];
                                output.push_str(&format!("{}TauValue {} = tauraro_list_len(&{});\n", ind, res, arg));
                                self.declared_vars.insert(res.clone());
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            }
                        }
                    }
                    _ => {
                        // Check if this is a method call (contains dot notation)
                        if func.contains('.') {
                            let parts: Vec<&str> = func.split('.').collect();
                            if parts.len() == 2 {
                                let object = parts[0];
                                let method = parts[1];
                                
                                if let Some(res) = result {
                                    output.push_str(&format!("{}TauValue {} = tauraro_call_method({}, \"{}\", {}, {});\n", 
                                        ind, res, object, method, args.len(), args_str));
                                    self.declared_vars.insert(res.clone());
                                    self.var_types.insert(res.clone(), NativeType::Generic);
                                } else {
                                    output.push_str(&format!("{}tauraro_call_method({}, \"{}\", {}, {});\n", 
                                        ind, object, method, args.len(), args_str));
                                }
                            }
                        } else {
                            // Regular function call
                            if let Some(res) = result {
                                output.push_str(&format!("{}TauValue {} = {}({});\n", ind, res, func, args_str));
                                self.declared_vars.insert(res.clone());
                                self.var_types.insert(res.clone(), NativeType::Generic);
                            } else {
                                output.push_str(&format!("{}{}({});\n", ind, func, args_str));
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
                output.push_str(&format!("{}if ({}) {{\n", ind, condition));
                for instr in then_body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }

                for (elif_cond, elif_instrs) in elif_branches {
                    output.push_str(&format!("{}}} else if ({}) {{\n", ind, elif_cond));
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
                // Emit condition instructions if any
                for instr in condition_instructions {
                    output.push_str(&self.transpile_instruction(instr, indent_level)?);
                }
                output.push_str(&format!("{}while ({}) {{\n", ind, condition));
                for instr in body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }
                output.push_str(&format!("{}}}\n", ind));
            }

            IRInstruction::For { variable, iterable, body, .. } => {
                // Simple for loop - iterate with counter
                output.push_str(&format!("{}// for loop over {}\n", ind, iterable));
                output.push_str(&format!("{}for(int {} = 0; {} < 100; {}++) {{\n", ind, variable, variable, variable));
                for instr in body {
                    output.push_str(&self.transpile_instruction(instr, indent_level + 1)?);
                }
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
                let var_decl = self.ensure_var_declared(result, NativeType::List, &ind);
                if !var_decl.is_empty() {
                    output.push_str(&format!("{};\n", var_decl.trim_end()));
                    output.push_str(&format!("{}{} = malloc(sizeof(TauList));\n", ind, result));
                } else {
                    output.push_str(&format!("{}{} = malloc(sizeof(TauList));\n", ind, result));
                }
                output.push_str(&format!("{}{}->size = {};\n", ind, result, elements.len()));
                output.push_str(&format!("{}{}->capacity = {};\n", ind, result, elements.len()));
                output.push_str(&format!("{}{}->items = malloc(sizeof(TauValue) * {});\n", ind, result, elements.len()));
                for (i, elem) in elements.iter().enumerate() {
                    output.push_str(&format!("{}{}->items[{}] = {};\n", ind, result, i, elem));
                }
            }

            IRInstruction::DictCreate { pairs, result } => {
                output.push_str(&format!("{}// Create dictionary\n", ind));
                let var_decl = self.ensure_var_declared(result, NativeType::Dict, &ind);
                if !var_decl.is_empty() {
                    output.push_str(&format!("{};\n", var_decl.trim_end()));
                    output.push_str(&format!("{}{} = malloc(sizeof(TauDict));\n", ind, result));
                } else {
                    output.push_str(&format!("{}{} = malloc(sizeof(TauDict));\n", ind, result));
                }
                output.push_str(&format!("{}{}->size = {};\n", ind, result, pairs.len()));
                output.push_str(&format!("{}{}->capacity = {};\n", ind, result, pairs.len()));
                output.push_str(&format!("{}{}->keys = malloc(sizeof(char*) * {});\n", ind, result, pairs.len()));
                output.push_str(&format!("{}{}->values = malloc(sizeof(TauValue) * {});\n", ind, result, pairs.len()));
                for (i, (key, val)) in pairs.iter().enumerate() {
                    output.push_str(&format!("{}{}->keys[{}] = {};\n", ind, result, i, key));
                    output.push_str(&format!("{}{}->values[{}] = {};\n", ind, result, i, val));
                }
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
                
                // First create the object
                output.push_str(&format!("{}TauObject* temp_obj = tauraro_create_object(\"{}\");\n", ind, class_name));
                
                // Wrap it in a TauValue
                output.push_str(&format!("{}{} = (TauValue){{.type = 6, .value.obj = temp_obj, .refcount = 1}};\n", ind, resolved_result));
                
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
                output.push_str(&format!("{}void* {} = tauraro_dict_get({}, {});\n", ind, result, dict, key));
                self.declared_vars.insert(result.clone());
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
                    output.push_str(&format!("{}TauValue {} = tauraro_module_get(temp_module, \"{}\");\n", 
                        ind, name, name));
                    self.declared_vars.insert(name.clone());
                    self.var_types.insert(name.clone(), NativeType::Generic);
                }
            }

            _ => {
                output.push_str(&format!("{}// Unhandled instruction: {:?}\n", ind, instr));
            }
        }

        Ok(output)
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
        match var_type {
            NativeType::Generic => format!("{}.value.i", var_name),
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
        output.push_str("char* tauraro_str_from_value(TauValue* val) {\n");
        output.push_str("    static char buffer[512];\n");
        output.push_str("    if (!val) return \"<null>\";\n");
        output.push_str("    switch(val->type) {\n");
        output.push_str("        case 0: snprintf(buffer, sizeof(buffer), \"%lld\", val->value.i); break;\n");
        output.push_str("        case 1: snprintf(buffer, sizeof(buffer), \"%f\", val->value.f); break;\n");
        output.push_str("        case 2: return val->value.s ? val->value.s : \"<null>\";\n");
        output.push_str("        case 3: return val->value.i ? \"True\" : \"False\";\n");
        output.push_str("        case 4: return \"<list>\";\n");
        output.push_str("        case 5: return \"<dict>\";\n");
        output.push_str("        case 6: return \"<object>\";\n");
        output.push_str("        case 7: return \"<function>\";\n");
        output.push_str("        case 8: return \"<exception>\";\n");
        output.push_str("        default: return \"<unknown>\";\n");
        output.push_str("    }\n");
        output.push_str("    return buffer;\n");
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
                IRInstruction::LoadConst { value, result } => {
                    let (type_name, _) = self.format_value(value);
                    self.scope_variables.insert(result.clone(), type_name);
                }
                IRInstruction::LoadLocal { name, result } => {
                    if let Some(var_type) = self.scope_variables.get(name).copied() {
                        self.scope_variables.insert(result.clone(), var_type);
                    } else {
                        self.scope_variables.insert(result.clone(), NativeType::Generic);
                    }
                }
                IRInstruction::StoreLocal { name, value } => {
                    if let Some(var_type) = self.scope_variables.get(value).copied() {
                        self.scope_variables.insert(name.clone(), var_type);
                    } else {
                        self.scope_variables.insert(name.clone(), NativeType::Generic);
                    }
                }
                IRInstruction::BinaryOp { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Int64);
                }
                IRInstruction::TypedBinaryOp { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Int64);
                }
                IRInstruction::Call { result, .. } => {
                    if let Some(res) = result {
                        self.scope_variables.insert(res.clone(), NativeType::Generic);
                    }
                }
                IRInstruction::ObjectCreate { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::ListCreate { result, .. } => {
                    self.scope_variables.insert(result.clone(), NativeType::Generic);
                }
                IRInstruction::DictCreate { result, .. } => {
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
                IRInstruction::While { body, .. } => {
                    self.collect_variables(body);
                }
                IRInstruction::For { body, .. } => {
                    self.collect_variables(body);
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
            class_definitions: self.class_definitions.clone(),
            function_definitions: self.function_definitions.clone(),
            current_function: self.current_function.clone(),
            loop_depth: self.loop_depth,
            exception_handlers: self.exception_handlers.clone(),
            imports: self.imports.clone(),
            generated_utilities: self.generated_utilities.clone(),
        }
    }
}

impl Default for CTranspiler {
    fn default() -> Self {
        Self::new()
    }
}
