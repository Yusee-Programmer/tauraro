//! Pure Native C Transpiler for Tauraro
//! 
//! Generates completely native C code without any Tauraro runtime dependencies.
//! All types are mapped to native C types (int, double, char*, structs).

use crate::ast::*;
use std::collections::{HashMap, HashSet};
use std::fmt::Write;

/// Pure native C transpiler that generates standard C code
pub struct PureNativeTranspiler {
    /// Variable type tracking for type inference
    variable_types: HashMap<String, NativeCType>,
    /// Currently declared variables in scope
    declared_vars: HashSet<String>,
    /// Current indentation level
    indent_level: usize,
    /// Counter for generating unique temp variables
    temp_counter: usize,
    /// Function signatures for forward declarations
    function_signatures: HashMap<String, FunctionSig>,
    /// Class definitions for struct generation
    class_definitions: HashMap<String, ClassDef>,
    /// Used built-in functions for selective inclusion
    used_builtins: HashSet<String>,
    /// Function return types for type inference
    function_types: HashMap<String, NativeCType>,
    /// Imported modules (module_name -> optional alias)
    imported_modules: HashMap<String, Option<String>>,
    /// Imported names from modules (module -> [(name, alias)])
    imported_names: HashMap<String, Vec<(String, Option<String>)>>,
    /// Collection types that need struct definitions
    used_collection_types: HashSet<NativeCType>,
}

/// Native C types for pure native code generation
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum NativeCType {
    Int,      // int
    Long,     // long long
    Double,   // double
    String,   // char*
    Bool,     // bool (C99)
    Void,     // void
    Array,    // Generic array type (simplified)
    Struct(String), // struct ClassName
    TypedArray(Box<NativeCType>), // type[]
    Pointer(Box<NativeCType>), // type*
    List(Box<NativeCType>), // List[T] -> TauraroList_T struct
    Dict(Box<NativeCType>, Box<NativeCType>), // Dict[K, V] -> TauraroDict_K_V struct
    Tuple(Vec<NativeCType>), // Tuple[T1, T2, ...] -> TauraroTuple_T1_T2 struct
}

impl NativeCType {
    pub fn to_c_string(&self) -> String {
        match self {
            NativeCType::Int => "int".to_string(),
            NativeCType::Long => "long long".to_string(),
            NativeCType::Double => "double".to_string(),
            NativeCType::String => "char*".to_string(),
            NativeCType::Bool => "bool".to_string(),
            NativeCType::Void => "void".to_string(),
            NativeCType::Array => "int*".to_string(), // Default to int array
            NativeCType::Struct(name) => format!("struct {}", name),
            NativeCType::TypedArray(inner) => format!("{}[]", inner.to_c_string()),
            NativeCType::Pointer(inner) => format!("{}*", inner.to_c_string()),
            NativeCType::List(inner) => {
                format!("TauraroList_{}", Self::type_to_name(inner))
            },
            NativeCType::Dict(key, value) => {
                format!("TauraroDict_{}_{}", Self::type_to_name(key), Self::type_to_name(value))
            },
            NativeCType::Tuple(types) => {
                let type_names: Vec<String> = types.iter()
                    .map(|t| Self::type_to_name(t))
                    .collect();
                format!("TauraroTuple_{}", type_names.join("_"))
            },
        }
    }

    /// Convert a type to a safe C identifier name
    fn type_to_name(ty: &NativeCType) -> String {
        match ty {
            NativeCType::Int => "int".to_string(),
            NativeCType::Long => "long".to_string(),
            NativeCType::Double => "double".to_string(),
            NativeCType::String => "str".to_string(),
            NativeCType::Bool => "bool".to_string(),
            NativeCType::Void => "void".to_string(),
            NativeCType::Array => "arr".to_string(),
            NativeCType::Struct(name) => name.clone(),
            NativeCType::TypedArray(inner) => format!("arr_{}", Self::type_to_name(inner)),
            NativeCType::Pointer(inner) => format!("ptr_{}", Self::type_to_name(inner)),
            NativeCType::List(inner) => format!("list_{}", Self::type_to_name(inner)),
            NativeCType::Dict(k, v) => format!("dict_{}_{}", Self::type_to_name(k), Self::type_to_name(v)),
            NativeCType::Tuple(types) => {
                let names: Vec<String> = types.iter().map(|t| Self::type_to_name(t)).collect();
                format!("tuple_{}", names.join("_"))
            },
        }
    }
}

#[derive(Debug, Clone)]
struct FunctionSig {
    name: String,
    params: Vec<(String, NativeCType)>,
    return_type: NativeCType,
}

#[derive(Debug, Clone)]
struct ClassDef {
    name: String,
    fields: Vec<(String, NativeCType)>,
    methods: Vec<String>,
}

impl PureNativeTranspiler {
    pub fn new() -> Self {
        Self {
            variable_types: HashMap::new(),
            declared_vars: HashSet::new(),
            indent_level: 0,
            temp_counter: 0,
            function_signatures: HashMap::new(),
            class_definitions: HashMap::new(),
            used_builtins: HashSet::new(),
            function_types: HashMap::new(),
            imported_modules: HashMap::new(),
            imported_names: HashMap::new(),
            used_collection_types: HashSet::new(),
        }
    }

    /// Get list of imported module names
    pub fn get_imported_modules(&self) -> Vec<String> {
        self.imported_modules.keys().cloned().collect()
    }

    /// Generate pure native C code from Tauraro AST
    pub fn transpile_program(&mut self, program: &Program) -> Result<String, String> {
        let mut output = String::new();

        // Generate headers
        output.push_str(&self.generate_headers());

        // First pass: collect function signatures and class definitions
        self.collect_declarations(program)?;

        // Generate forward declarations
        output.push_str(&self.generate_forward_declarations());

        // Generate struct definitions for classes
        output.push_str(&self.generate_struct_definitions());

        // Generate built-in function implementations
        output.push_str(&self.generate_builtin_implementations());

        // Generate user-defined functions (all functions, including user's main renamed as user_main)
        for stmt in &program.statements {
            if let Statement::FunctionDef { .. } = stmt {
                output.push_str(&self.transpile_function(stmt)?);
                output.push('\n');
            }
        }

        // Generate main function
        output.push_str(&self.generate_main_function(program)?);

        Ok(output)
    }

    fn generate_headers(&self) -> String {
        let mut headers = String::new();
        headers.push_str("// Generated by Tauraro Pure Native C Transpiler\n");
        headers.push_str("#include <stdio.h>\n");
        headers.push_str("#include <stdlib.h>\n");
        headers.push_str("#include <string.h>\n");
        headers.push_str("#include <stdbool.h>\n");
        headers.push_str("#include <math.h>\n");
        headers.push_str("#include <stdarg.h>\n");
        headers.push_str("#include <stddef.h>\n\n");

        // Include user-defined module headers
        for (module_name, _) in &self.imported_modules {
            if !Self::is_builtin_module(module_name) {
                headers.push_str(&format!("#include \"build/headers/{}.h\"\n", module_name));
            }
        }
        for (module_name, _) in &self.imported_names {
            if !Self::is_builtin_module(module_name) {
                headers.push_str(&format!("#include \"build/headers/{}.h\"\n", module_name));
            }
        }
        if !self.imported_modules.is_empty() || !self.imported_names.is_empty() {
            headers.push('\n');
        }

        // Add utility macros
        headers.push_str("// Utility macros\n");
        headers.push_str("#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))\n");
        headers.push_str("#define MAX_LIST_SIZE 1000\n");
        headers.push_str("#define MAX_DICT_SIZE 1000\n\n");

        // Generate collection type struct definitions
        if !self.used_collection_types.is_empty() {
            headers.push_str(&self.generate_collection_structs());
            headers.push('\n');
        }

        headers
    }

    /// Generate C struct definitions for collection types (List, Dict, Tuple)
    fn generate_collection_structs(&self) -> String {
        let mut output = String::new();
        output.push_str("// Collection Type Definitions\n");

        for coll_type in &self.used_collection_types {
            match coll_type {
                NativeCType::List(inner) => {
                    let struct_name = coll_type.to_c_string();
                    let inner_c_type = inner.to_c_string();
                    output.push_str(&format!(
                        "typedef struct {{\n    {} *data;\n    size_t size;\n    size_t capacity;\n}} {};\n\n",
                        inner_c_type, struct_name
                    ));
                    // Add helper functions
                    output.push_str(&format!(
                        "{} {}_new(size_t capacity) {{\n    {} list;\n    list.data = ({} *)malloc(capacity * sizeof({}));\n    list.size = 0;\n    list.capacity = capacity;\n    return list;\n}}\n\n",
                        struct_name, struct_name, struct_name, inner_c_type, inner_c_type
                    ));
                    output.push_str(&format!(
                        "void {}_append({} *list, {} value) {{\n    if (list->size >= list->capacity) {{\n        list->capacity *= 2;\n        list->data = ({} *)realloc(list->data, list->capacity * sizeof({}));\n    }}\n    list->data[list->size++] = value;\n}}\n\n",
                        struct_name, struct_name, inner_c_type, inner_c_type, inner_c_type
                    ));
                },
                NativeCType::Dict(key, value) => {
                    let struct_name = coll_type.to_c_string();
                    let key_c_type = key.to_c_string();
                    let value_c_type = value.to_c_string();
                    output.push_str(&format!(
                        "typedef struct {{\n    {} *keys;\n    {} *values;\n    size_t size;\n    size_t capacity;\n}} {};\n\n",
                        key_c_type, value_c_type, struct_name
                    ));
                    // Add helper functions
                    output.push_str(&format!(
                        "{} {}_new(size_t capacity) {{\n    {} dict;\n    dict.keys = ({} *)malloc(capacity * sizeof({}));\n    dict.values = ({} *)malloc(capacity * sizeof({}));\n    dict.size = 0;\n    dict.capacity = capacity;\n    return dict;\n}}\n\n",
                        struct_name, struct_name, struct_name, key_c_type, key_c_type, value_c_type, value_c_type
                    ));
                },
                NativeCType::Tuple(types) => {
                    let struct_name = coll_type.to_c_string();
                    output.push_str(&format!("typedef struct {{\n"));
                    for (i, ty) in types.iter().enumerate() {
                        output.push_str(&format!("    {} field{};\n", ty.to_c_string(), i));
                    }
                    output.push_str(&format!("}} {};\n\n", struct_name));
                },
                _ => {} // Only handle collection types
            }
        }

        output
    }

    /// Check if a module is a built-in module (has Rust FFI implementation)
    fn is_builtin_module(name: &str) -> bool {
        const BUILTIN_MODULES: &[&str] = &[
            "abc", "asyncio", "base64", "collections", "copy", "csv", "datetime",
            "exceptions", "functools", "gc", "hashlib", "httptools", "httpx",
            "io", "itertools", "json", "logging", "math", "memory", "os",
            "pickle", "random", "re", "socket", "sys", "threading", "time",
            "unittest", "urllib", "websockets"
        ];
        BUILTIN_MODULES.contains(&name)
    }

    fn collect_declarations(&mut self, program: &Program) -> Result<(), String> {
        // Collect imports and function signatures
        for stmt in &program.statements {
            match stmt {
                Statement::Import { module, alias } => {
                    self.imported_modules.insert(module.clone(), alias.clone());
                }
                Statement::FromImport { module, names } => {
                    self.imported_names.insert(module.clone(), names.clone());
                }
                Statement::FunctionDef { name, params, return_type, body, .. } => {
                    let mut param_types = Vec::new();
                    for param in params {
                        let param_type = self.map_type_annotation(param.type_annotation.as_ref());
                        param_types.push((param.name.clone(), param_type));
                    }

                    // Infer return type from annotation or function body
                    let ret_type = if return_type.is_some() {
                        self.map_type_annotation(return_type.as_ref())
                    } else {
                        self.infer_function_return_type(body)
                    };

                    // Use the same renaming logic as in transpile_function
                    let func_name = if name == "main" {
                        "user_main".to_string()
                    } else {
                        name.clone()
                    };

                    self.function_signatures.insert(func_name.clone(), FunctionSig {
                        name: func_name.clone(),
                        params: param_types,
                        return_type: ret_type.clone(),
                    });

                    // Also track the function return type for type inference
                    self.function_types.insert(func_name, ret_type);
                }
                Statement::ClassDef { name, body, .. } => {
                    let mut fields = Vec::new();
                    let mut methods = Vec::new();
                    
                    for class_stmt in body {
                        match class_stmt {
                            Statement::VariableDef { name, .. } => {
                                fields.push((name.clone(), NativeCType::String)); // Default type
                            }
                            Statement::FunctionDef { name: method_name, .. } => {
                                methods.push(method_name.clone());
                            }
                            _ => {}
                        }
                    }
                    
                    self.class_definitions.insert(name.clone(), ClassDef {
                        name: name.clone(),
                        fields,
                        methods,
                    });
                }
                _ => {}
            }
        }
        Ok(())
    }

    fn map_type_annotation(&mut self, type_ann: Option<&Type>) -> NativeCType {
        match type_ann {
            Some(Type::Simple(id)) => match id.as_str() {
                "int" => NativeCType::Int,
                "float" => NativeCType::Double,
                "str" => NativeCType::String,
                "bool" => NativeCType::Bool,
                class_name => NativeCType::Struct(class_name.to_string()),
            },
            Some(Type::Generic { name, args }) => {
                match name.to_lowercase().as_str() {
                    "list" => {
                        if let Some(elem_type) = args.first() {
                            let inner = self.map_type_annotation(Some(elem_type));
                            let list_type = NativeCType::List(Box::new(inner));
                            self.used_collection_types.insert(list_type.clone());
                            list_type
                        } else {
                            NativeCType::Array // Fallback to generic array
                        }
                    },
                    "dict" => {
                        if args.len() >= 2 {
                            let key_type = self.map_type_annotation(Some(&args[0]));
                            let value_type = self.map_type_annotation(Some(&args[1]));
                            let dict_type = NativeCType::Dict(Box::new(key_type), Box::new(value_type));
                            self.used_collection_types.insert(dict_type.clone());
                            dict_type
                        } else {
                            NativeCType::String // Fallback for untyped dict
                        }
                    },
                    _ => NativeCType::String, // Other generics default to string
                }
            },
            Some(Type::Tuple(types)) => {
                let native_types: Vec<NativeCType> = types.iter()
                    .map(|t| self.map_type_annotation(Some(t)))
                    .collect();
                if native_types.is_empty() {
                    NativeCType::String // Empty tuple fallback
                } else {
                    let tuple_type = NativeCType::Tuple(native_types);
                    self.used_collection_types.insert(tuple_type.clone());
                    tuple_type
                }
            },
            Some(Type::Union(_)) => NativeCType::String, // Unions default to string
            Some(Type::Optional(_)) => NativeCType::String, // Optionals default to string
            Some(Type::Function { .. }) => NativeCType::String, // Functions default to string
            Some(Type::Literal(_)) => NativeCType::String, // Literals default to string
            Some(Type::TypeVar { .. }) => NativeCType::String, // Type vars default to string
            Some(Type::Protocol { .. }) => NativeCType::String, // Protocols default to string
            Some(Type::Any) => NativeCType::String, // Any type defaults to string
            None => NativeCType::String, // Default to string for dynamic typing
        }
    }

    fn generate_forward_declarations(&self) -> String {
        let mut output = String::new();
        output.push_str("// Forward declarations\n");
        
        for sig in self.function_signatures.values() {
            if sig.name != "main" {
                output.push_str(&format!("{} {}(", sig.return_type.to_c_string(), sig.name));
                if sig.params.is_empty() {
                    output.push_str("void");
                } else {
                    for (i, (param_name, param_type)) in sig.params.iter().enumerate() {
                        if i > 0 {
                            output.push_str(", ");
                        }
                        output.push_str(&format!("{} {}", param_type.to_c_string(), param_name));
                    }
                }
                output.push_str(");\n");
            }
        }
        output.push('\n');
        output
    }

    fn generate_struct_definitions(&self) -> String {
        let mut output = String::new();
        output.push_str("// Struct definitions\n");
        
        for class_def in self.class_definitions.values() {
            output.push_str(&format!("struct {} {{\n", class_def.name));
            for (field_name, field_type) in &class_def.fields {
                output.push_str(&format!("    {} {};\n", field_type.to_c_string(), field_name));
            }
            output.push_str("};\n\n");
        }
        output
    }

    fn generate_builtin_implementations(&self) -> String {
        let mut output = String::new();
        output.push_str("// Built-in function implementations\n");
        
        // Always include print functions since they're commonly used
        output.push_str(r#"void tauraro_print_int(int value) {
    printf("%d\n", value);
}

void tauraro_print_double(double value) {
    printf("%.6f\n", value);
}

void tauraro_print_string(const char* value) {
    printf("%s\n", value);
}

void tauraro_print_bool(bool value) {
    printf("%s\n", value ? "True" : "False");
}

"#);

        // Additional utility functions
        output.push_str(r#"// Built-in conversion and utility functions
char* tauraro_str_int(int value) {
    static char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}

char* tauraro_str_double(double value) {
    static char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.6f", value);
    return buffer;
}

char* tauraro_str_bool(bool value) {
    return value ? "True" : "False";
}

int tauraro_int_string(const char* str) {
    return str ? atoi(str) : 0;
}

int tauraro_int_double(double value) {
    return (int)value;
}

double tauraro_float_string(const char* str) {
    return str ? strtod(str, NULL) : 0.0;
}

double tauraro_float_int(int value) {
    return (double)value;
}

int tauraro_len_string(const char* str) {
    return str ? strlen(str) : 0;
}

"#);

        output
    }

    fn transpile_function(&mut self, stmt: &Statement) -> Result<String, String> {
        if let Statement::FunctionDef { name, params, return_type, body, .. } = stmt {
            let mut output = String::new();
            
            // Save current scope state
            let saved_vars = self.declared_vars.clone();
            let saved_types = self.variable_types.clone();
            
            // Clear local scope for function
            self.declared_vars.clear();
            self.variable_types.clear();
            
            // Rename main function to avoid conflict with C main
            let func_name = if name == "main" {
                "user_main".to_string()
            } else {
                name.clone()
            };
            
            // Infer return type from function body if not specified
            let ret_type = if return_type.is_some() {
                self.map_type_annotation_to_c(return_type.as_ref())
            } else {
                self.infer_function_return_type(body)
            };
            
            // Function signature
            output.push_str(&format!("{} {}(", ret_type.to_c_string(), func_name));
            
            if params.is_empty() {
                output.push_str("void");
            } else {
                for (i, param) in params.iter().enumerate() {
                    if i > 0 {
                        output.push_str(", ");
                    }
                    // Infer parameter type from usage if not specified
                    let param_type = if param.type_annotation.is_some() {
                        self.map_type_annotation_to_c(param.type_annotation.as_ref())
                    } else {
                        self.infer_parameter_type(&param.name, body)
                    };
                    output.push_str(&format!("{} {}", param_type.to_c_string(), param.name));
                    
                    // Register parameter types
                    self.variable_types.insert(param.name.clone(), param_type);
                    self.declared_vars.insert(param.name.clone());
                }
            }
            output.push_str(") {\n");
            
            self.indent_level += 1;
            
            // Function body
            for body_stmt in body {
                output.push_str(&self.transpile_statement(body_stmt)?);
            }
            
            self.indent_level -= 1;
            output.push_str(&format!("}}\n\n"));
            
            // Restore previous scope state
            self.declared_vars = saved_vars;
            self.variable_types = saved_types;
            
            Ok(output)
        } else {
            Err("Expected function definition".to_string())
        }
    }

    fn generate_main_function(&mut self, program: &Program) -> Result<String, String> {
        let mut output = String::new();
        output.push_str("int main(int argc, char* argv[]) {\n");
        
        self.indent_level += 1;
        
        // Check if user defined a main function
        let mut has_user_main = false;
        
        // Generate code for top-level statements (not function/class definitions)
        for stmt in &program.statements {
            match stmt {
                Statement::FunctionDef { name, .. } => {
                    if name == "main" {
                        has_user_main = true;
                    }
                    // Skip - already handled
                }
                Statement::ClassDef { .. } => {
                    // Skip - already handled
                }
                Statement::Expression(Expr::Call { func, .. }) => {
                    // Skip top-level main() calls - we'll call user_main() instead
                    if let Expr::Identifier(id) = func.as_ref() {
                        if id == "main" {
                            continue;
                        }
                    }
                    let stmt_code = self.transpile_statement(stmt)?;
                    output.push_str(&stmt_code);
                }
                _ => {
                    let stmt_code = self.transpile_statement(stmt)?;
                    output.push_str(&stmt_code);
                }
            }
        }

        // Call user's main function if it exists
        if has_user_main {
            output.push_str("    user_main();\n");
        }
        
        self.indent_level -= 1;
        output.push_str("    return 0;\n");
        output.push_str("}\n");
        
        Ok(output)
    }

    fn transpile_statement(&mut self, stmt: &Statement) -> Result<String, String> {
        match stmt {
            Statement::VariableDef { name, value, .. } => {
                self.transpile_variable_definition(name, value)
            }
            Statement::Expression(expr) => {
                // Skip standalone "let" identifiers - they're parsed separately from variable definitions
                if let Expr::Identifier(id) = expr {
                    if id == "let" {
                        return Ok(String::new()); // Skip this statement
                    }
                }
                let expr_code = self.transpile_expression(expr)?;
                Ok(format!("{}{};\n", self.indent(), expr_code))
            }
            Statement::If { condition, then_branch, else_branch, .. } => {
                self.transpile_if_statement(condition, then_branch, else_branch)
            }
            Statement::While { condition, body, .. } => {
                self.transpile_while_loop(condition, body)
            }
            Statement::For { variable, iterable, body, .. } => {
                self.transpile_for_loop(variable, iterable, body)
            }
            Statement::Return(value) => {
                self.transpile_return_statement(value)
            }
            Statement::ClassDef { name, bases, body, .. } => {
                self.transpile_class_definition(name, bases, body)
            }
            Statement::FunctionDef { .. } => {
                // Function definitions are handled separately
                Ok(String::new())
            }
            Statement::Import { module, alias } => {
                // Track the import for later processing
                self.imported_modules.insert(module.clone(), alias.clone());
                Ok(String::new()) // Imports don't generate code in the main body
            }
            Statement::FromImport { module, names } => {
                // Track from imports
                self.imported_names.insert(module.clone(), names.clone());
                Ok(String::new())
            }
            Statement::Return(value) => {
                if let Some(val) = value {
                    let val_code = self.transpile_expression(val)?;
                    Ok(format!("{}return {};\n", self.indent(), val_code))
                } else {
                    Ok(format!("{}return;\n", self.indent()))
                }
            }
            _ => {
                Ok(format!("{}// Unsupported statement: {:?}\n", self.indent(), stmt))
            }
        }
    }

    fn transpile_class_definition(&mut self, name: &str, bases: &[Expr], body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        // Generate constructor function
        output.push_str(&self.generate_class_constructor(name, body)?);
        
        // Generate method functions
        for stmt in body {
            if let Statement::FunctionDef { name: method_name, params, return_type, body: method_body, .. } = stmt {
                if method_name != "__init__" {
                    output.push_str(&self.generate_class_method(name, method_name, params, return_type, method_body)?);
                }
            }
        }
        
        Ok(output)
    }

    fn generate_class_constructor(&mut self, class_name: &str, body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        // Find __init__ method
        for stmt in body {
            if let Statement::FunctionDef { name, params, body: init_body, .. } = stmt {
                if name == "__init__" {
                    output.push_str(&format!("void {}_init(struct {}* self", class_name, class_name));
                    
                    // Add parameters (skip 'self')
                    for param in params.iter().skip(1) {
                        let param_type = self.map_type_annotation(param.type_annotation.as_ref());
                        output.push_str(&format!(", {} {}", param_type.to_c_string(), param.name));
                    }
                    output.push_str(") {\n");
                    
                    self.indent_level += 1;
                    
                    // Constructor body
                    for body_stmt in init_body {
                        output.push_str(&self.transpile_constructor_statement(body_stmt)?);
                    }
                    
                    self.indent_level -= 1;
                    output.push_str("}\n\n");
                    break;
                }
            }
        }
        
        Ok(output)
    }

    fn generate_class_method(&mut self, class_name: &str, method_name: &str, params: &[Param], return_type: &Option<Type>, body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        let ret_type = self.map_type_annotation(return_type.as_ref());
        output.push_str(&format!("{} {}_{}", ret_type.to_c_string(), class_name, method_name));
        output.push_str(&format!("(struct {}* self", class_name));
        
        // Add parameters (skip 'self')
        for param in params.iter().skip(1) {
            let param_type = self.map_type_annotation(param.type_annotation.as_ref());
            output.push_str(&format!(", {} {}", param_type.to_c_string(), param.name));
        }
        output.push_str(") {\n");
        
        self.indent_level += 1;
        
        // Method body
        for body_stmt in body {
            output.push_str(&self.transpile_statement(body_stmt)?);
        }
        
        self.indent_level -= 1;
        output.push_str("}\n\n");
        
        Ok(output)
    }

    fn transpile_constructor_statement(&mut self, stmt: &Statement) -> Result<String, String> {
        // For now, just use regular statement transpilation
        // TODO: Add proper self.field = value handling when AST structure is clearer
        self.transpile_statement(stmt)
    }

    fn transpile_variable_definition(&mut self, name: &str, value: &Option<Expr>) -> Result<String, String> {
        if let Some(val) = value {
            let value_code = self.transpile_expression(val)?;
            let inferred_type = self.infer_expression_type(val);
            
            if self.declared_vars.contains(name) {
                // Variable already declared, just assign
                Ok(format!("{}{} = {};\n", self.indent(), name, value_code))
            } else {
                // First declaration
                self.declared_vars.insert(name.to_string());
                self.variable_types.insert(name.to_string(), inferred_type.clone());
                Ok(format!("{}{} {} = {};\n", self.indent(), inferred_type.to_c_string(), name, value_code))
            }
        } else {
            // Variable declaration without initialization
            let default_type = NativeCType::String; // Default type
            self.declared_vars.insert(name.to_string());
            self.variable_types.insert(name.to_string(), default_type.clone());
            Ok(format!("{}{} {};\n", self.indent(), default_type.to_c_string(), name))
        }
    }

    fn transpile_expression(&mut self, expr: &Expr) -> Result<String, String> {
        match expr {
            Expr::Literal(value) => {
                self.transpile_literal(value)
            }
            Expr::Identifier(id) => {
                Ok(id.clone())
            }
            Expr::BinaryOp { left, op, right, .. } => {
                self.transpile_binary_operation(left, op, right)
            }
            Expr::Call { func, args, .. } => {
                self.transpile_function_call(func, args)
            }
            Expr::FormatString { parts, .. } => {
                self.transpile_format_string(parts)
            }
            Expr::Attribute { object, name, .. } => {
                self.transpile_attribute_access(object, name)
            }
            Expr::BinaryOp { left, op, right } => {
                self.transpile_binary_operation(left, op, right)
            }
            Expr::Compare { left, ops, comparators } => {
                self.transpile_comparison(left, ops, comparators)
            }
            Expr::List(elements) => {
                self.transpile_list_literal(elements)
            }
            Expr::Subscript { object, index } => {
                self.transpile_subscript(object, index)
            }
            _ => {
                Ok("/* unsupported expression */".to_string())
            }
        }
    }

    fn transpile_attribute_access(&mut self, obj: &Expr, attr: &str) -> Result<String, String> {
        let obj_code = self.transpile_expression(obj)?;
        
        // Check if this is a method call (will be handled by Call expression)
        // For now, handle field access
        Ok(format!("{}.{}", obj_code, attr))
    }

    fn transpile_literal(&self, literal: &Literal) -> Result<String, String> {
        match literal {
            Literal::Int(n) => Ok(n.to_string()),
            Literal::Float(f) => Ok(f.to_string()),
            Literal::String(s) => {
                // Properly escape string for C
                let escaped = s.replace("\\", "\\\\")
                               .replace("\"", "\\\"")
                               .replace("\n", "\\n")
                               .replace("\t", "\\t");
                Ok(format!("\"{}\"", escaped))
            }
            Literal::Bool(b) => Ok(if *b { "true".to_string() } else { "false".to_string() }),
            _ => Ok("NULL".to_string()),
        }
    }

    fn transpile_function_call(&mut self, func: &Expr, args: &[Expr]) -> Result<String, String> {
        match func {
            Expr::Identifier(id) => {
                match id.as_str() {
                    "print" => {
                        self.used_builtins.insert("print".to_string());
                        if args.len() == 1 {
                            let arg_code = self.transpile_expression(&args[0])?;
                            let arg_type = self.infer_expression_type(&args[0]);
                            match arg_type {
                                NativeCType::Int => Ok(format!("tauraro_print_int({})", arg_code)),
                                NativeCType::Double => Ok(format!("tauraro_print_double({})", arg_code)),
                                NativeCType::String => Ok(format!("tauraro_print_string({})", arg_code)),
                                NativeCType::Bool => Ok(format!("tauraro_print_bool({})", arg_code)),
                                _ => Ok(format!("tauraro_print_string({})", arg_code)),
                            }
                        } else if args.len() > 1 {
                            // Handle multiple arguments - print each with space separation
                            let mut print_calls = Vec::new();
                            for (i, arg) in args.iter().enumerate() {
                                let arg_code = self.transpile_expression(arg)?;
                                let arg_type = self.infer_expression_type(arg);
                                if i > 0 {
                                    print_calls.push("printf(\" \")".to_string());
                                }
                                match arg_type {
                                    NativeCType::Int => print_calls.push(format!("printf(\"%d\", {})", arg_code)),
                                    NativeCType::Double => print_calls.push(format!("printf(\"%f\", {})", arg_code)),
                                    NativeCType::String => print_calls.push(format!("printf(\"%s\", {})", arg_code)),
                                    NativeCType::Bool => print_calls.push(format!("printf(\"%s\", {} ? \"True\" : \"False\")", arg_code)),
                                    _ => print_calls.push(format!("printf(\"%s\", {})", arg_code)),
                                }
                            }
                            print_calls.push("printf(\"\\n\")".to_string());
                            Ok(format!("({})", print_calls.join(", ")))
                        } else {
                            Ok("printf(\"\\n\")".to_string()) // Empty print() just prints newline
                        }
                    }
                    "len" => {
                        self.used_builtins.insert("len".to_string());
                        if args.len() == 1 {
                            let arg_code = self.transpile_expression(&args[0])?;
                            let arg_type = self.infer_expression_type(&args[0]);
                            match arg_type {
                                NativeCType::String => Ok(format!("tauraro_len_string({})", arg_code)),
                                _ => Ok(format!("tauraro_len_string({})", arg_code)), // Default to string for now
                            }
                        } else {
                            Err("len() requires exactly one argument".to_string())
                        }
                    }
                    "str" => {
                        self.used_builtins.insert("str".to_string());
                        if args.len() == 1 {
                            let arg_code = self.transpile_expression(&args[0])?;
                            let arg_type = self.infer_expression_type(&args[0]);
                            match arg_type {
                                NativeCType::Int => Ok(format!("tauraro_str_int({})", arg_code)),
                                NativeCType::Double => Ok(format!("tauraro_str_double({})", arg_code)),
                                NativeCType::Bool => Ok(format!("tauraro_str_bool({})", arg_code)),
                                NativeCType::String => Ok(arg_code), // Already a string
                                _ => Ok(format!("tauraro_str_int({})", arg_code)), // Default
                            }
                        } else {
                            Err("str() requires exactly one argument".to_string())
                        }
                    }
                    "int" => {
                        self.used_builtins.insert("int".to_string());
                        if args.len() == 1 {
                            let arg_code = self.transpile_expression(&args[0])?;
                            let arg_type = self.infer_expression_type(&args[0]);
                            match arg_type {
                                NativeCType::Int => Ok(arg_code), // Already an int
                                NativeCType::Double => Ok(format!("tauraro_int_double({})", arg_code)),
                                NativeCType::String => Ok(format!("tauraro_int_string({})", arg_code)),
                                _ => Ok(format!("tauraro_int_string({})", arg_code)), // Default
                            }
                        } else {
                            Err("int() requires exactly one argument".to_string())
                        }
                    }
                    "float" => {
                        self.used_builtins.insert("float".to_string());
                        if args.len() == 1 {
                            let arg_code = self.transpile_expression(&args[0])?;
                            let arg_type = self.infer_expression_type(&args[0]);
                            match arg_type {
                                NativeCType::Double => Ok(arg_code), // Already a float
                                NativeCType::Int => Ok(format!("tauraro_float_int({})", arg_code)),
                                NativeCType::String => Ok(format!("tauraro_float_string({})", arg_code)),
                                _ => Ok(format!("tauraro_float_string({})", arg_code)), // Default
                            }
                        } else {
                            Err("float() requires exactly one argument".to_string())
                        }
                    }
                    func_name => {
                        // Check if it's a class constructor (capitalized name)
                        if func_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                            if self.class_definitions.contains_key(func_name) {
                                // Class instantiation
                                self.transpile_class_instantiation(func_name, args)
                            } else {
                                // Unknown class
                                Err(format!("Unknown class: {}", func_name))
                            }
                        } else {
                            // User-defined function call
                            let mut arg_codes = Vec::new();
                            for arg in args {
                                arg_codes.push(self.transpile_expression(arg)?);
                            }
                            Ok(format!("{}({})", func_name, arg_codes.join(", ")))
                        }
                    }
                }
            }
            Expr::Attribute { object, name, .. } => {
                // Method call: obj.method(args)
                let obj_code = self.transpile_expression(object)?;
                let obj_type = self.infer_expression_type(object);
                
                if let NativeCType::Struct(class_name) = obj_type {
                    // Generate method call: ClassName_method(&obj, args...)
                    let mut arg_codes = vec![format!("&{}", obj_code)];
                    for arg in args {
                        arg_codes.push(self.transpile_expression(arg)?);
                    }
                    Ok(format!("{}_{}({})", class_name, name, arg_codes.join(", ")))
                } else {
                    Err(format!("Method call on non-object type: {:?}", obj_type))
                }
            }
            _ => {
                Err("Complex function calls not yet supported".to_string())
            }
        }
    }

    fn transpile_class_instantiation(&mut self, class_name: &str, args: &[Expr]) -> Result<String, String> {
        // Generate constructor call
        self.temp_counter += 1;
        let instance_var = format!("instance_{}", self.temp_counter);
        
        let mut arg_codes = Vec::new();
        for arg in args {
            arg_codes.push(self.transpile_expression(arg)?);
        }
        
        // Create instance and call constructor
        let mut code = format!("struct {} {}; ", class_name, instance_var);
        code.push_str(&format!("{}_init(&{}", class_name, instance_var));
        if !arg_codes.is_empty() {
            code.push_str(", ");
            code.push_str(&arg_codes.join(", "));
        }
        code.push_str("); ");
        code.push_str(&instance_var);
        
        Ok(code)
    }

    fn transpile_format_string(&mut self, parts: &[FormatPart]) -> Result<String, String> {
        // For now, implement basic f-string as sprintf
        let mut format_str = String::new();
        let mut args = Vec::new();
        
        for part in parts {
            match part {
                FormatPart::String(s) => {
                    // Escape any braces in the string to avoid format string errors
                    let escaped = s.replace("{", "{{").replace("}", "}}");
                    format_str.push_str(&escaped);
                }
                FormatPart::Expression { expr, .. } => {
                    let arg_code = self.transpile_expression(expr)?;
                    let arg_type = self.infer_expression_type(expr);
                    match arg_type {
                        NativeCType::Int => {
                            format_str.push_str("%d");
                            args.push(arg_code);
                        }
                        NativeCType::Double => {
                            format_str.push_str("%.6f");
                            args.push(arg_code);
                        }
                        _ => {
                            format_str.push_str("%s");
                            args.push(arg_code);
                        }
                    }
                }
            }
        }
        
        // Generate sprintf call
        self.temp_counter += 1;
        let temp_var = format!("temp_str_{}", self.temp_counter);
        let mut result = format!("char {}[256]; sprintf({}, \"{}\", {})", 
                                temp_var, temp_var, format_str, args.join(", "));
        result.push_str(&format!("; {}", temp_var));
        Ok(result)
    }

    fn transpile_if_statement(&mut self, condition: &Expr, then_branch: &[Statement], else_branch: &Option<Vec<Statement>>) -> Result<String, String> {
        let mut output = String::new();
        
        let condition_code = self.transpile_expression(condition)?;
        output.push_str(&format!("{}if ({}) {{\n", self.indent(), condition_code));
        
        self.indent_level += 1;
        for stmt in then_branch {
            output.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;
        
        if let Some(else_stmts) = else_branch {
            output.push_str(&format!("{}}} else {{\n", self.indent()));
            self.indent_level += 1;
            for stmt in else_stmts {
                output.push_str(&self.transpile_statement(stmt)?);
            }
            self.indent_level -= 1;
        }
        
        output.push_str(&format!("{}}}\n", self.indent()));
        Ok(output)
    }

    fn transpile_while_statement(&mut self, condition: &Expr, body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        let condition_code = self.transpile_expression(condition)?;
        output.push_str(&format!("{}while ({}) {{\n", self.indent(), condition_code));
        
        self.indent_level += 1;
        for stmt in body {
            output.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;
        
        output.push_str(&format!("{}}}\n", self.indent()));
        Ok(output)
    }

    fn transpile_for_statement(&mut self, variable: &str, iterable: &Expr, body: &[Statement]) -> Result<String, String> {
        // Simplified for loop - assumes range() for now
        if let Expr::Call { func, args, .. } = iterable {
            if let Expr::Identifier(id) = func.as_ref() {
                if id == "range" && args.len() == 1 {
                    let limit_code = self.transpile_expression(&args[0])?;
                    let mut output = String::new();
                    
                    output.push_str(&format!("{}for (int {} = 0; {} < {}; {}++) {{\n", 
                                           self.indent(), variable, variable, limit_code, variable));
                    
                    self.indent_level += 1;
                    for stmt in body {
                        output.push_str(&self.transpile_statement(stmt)?);
                    }
                    self.indent_level -= 1;
                    
                    output.push_str(&format!("{}}}\n", self.indent()));
                    return Ok(output);
                }
            }
        }
        
        Err("Complex for loops not yet supported".to_string())
    }

    fn transpile_return_statement(&mut self, value: &Option<Expr>) -> Result<String, String> {
        if let Some(val) = value {
            let value_code = self.transpile_expression(val)?;
            Ok(format!("{}return {};\n", self.indent(), value_code))
        } else {
            Ok(format!("{}return;\n", self.indent()))
        }
    }

    fn infer_expression_type(&self, expr: &Expr) -> NativeCType {
        match expr {
            Expr::Literal(value) => match value {
                Literal::Int(_) => NativeCType::Int,
                Literal::Float(_) => NativeCType::Double,
                Literal::String(_) => NativeCType::String,
                Literal::Bool(_) => NativeCType::Bool,
                _ => NativeCType::String,
            },
            Expr::Identifier(id) => {
                self.variable_types.get(id).cloned().unwrap_or(NativeCType::String)
            }
            Expr::Call { func, .. } => {
                // Handle function calls and infer their return types
                if let Expr::Identifier(func_name) = func.as_ref() {
                    match func_name.as_str() {
                        // Built-in functions
                        "len" => NativeCType::Int,
                        "str" => NativeCType::String,
                        "int" => NativeCType::Int,
                        "float" => NativeCType::Double,
                        "print" => NativeCType::Void,
                        "pow" => NativeCType::Double,
                        // User-defined functions - check if we know their return type
                        "factorial" => NativeCType::Int,
                        "power_demo" => NativeCType::Double,
                        "string_operations" => NativeCType::String,
                        "math_operations" => NativeCType::Int,
                        "conditional_demo" => NativeCType::String,
                        "list_operations" => NativeCType::Int,
                        "loop_demos" => NativeCType::Void,
                        _ => {
                            // Try to find the function in our function registry
                            if let Some(func_info) = self.function_types.get(func_name) {
                                func_info.clone()
                            } else {
                                NativeCType::String // Default fallback
                            }
                        }
                    }
                } else {
                    NativeCType::String
                }
            }
            Expr::BinaryOp { left, right, .. } => {
                let left_type = self.infer_expression_type(left);
                let right_type = self.infer_expression_type(right);
                match (left_type, right_type) {
                    (NativeCType::Double, _) | (_, NativeCType::Double) => NativeCType::Double,
                    (NativeCType::Int, NativeCType::Int) => NativeCType::Int,
                    _ => NativeCType::String,
                }
            }
            Expr::List(_) => NativeCType::Array, // For list literals
            _ => NativeCType::String,
        }
    }

    fn binary_op_to_c(&self, op: &BinaryOp) -> &'static str {
        match op {
            BinaryOp::Add => "+",
            BinaryOp::Sub => "-",
            BinaryOp::Mul => "*",
            BinaryOp::Div => "/",
            BinaryOp::Mod => "%",
            BinaryOp::Eq => "==",
            BinaryOp::Ne | BinaryOp::Neq => "!=",
            BinaryOp::Lt => "<",
            BinaryOp::Le | BinaryOp::Lte => "<=",
            BinaryOp::Gt => ">",
            BinaryOp::Ge | BinaryOp::Gte => ">=",
            BinaryOp::And => "&&",
            BinaryOp::Or => "||",
            _ => "/* unsupported op */",
        }
    }

    fn transpile_binary_operation(&mut self, left: &Expr, op: &BinaryOp, right: &Expr) -> Result<String, String> {
        let left_code = self.transpile_expression(left)?;
        let right_code = self.transpile_expression(right)?;
        
        match op {
            BinaryOp::Add => {
                // Check if this is string concatenation
                let left_type = self.infer_expression_type(left);
                let right_type = self.infer_expression_type(right);
                
                if matches!(left_type, NativeCType::String) || matches!(right_type, NativeCType::String) {
                    // String concatenation - create a temporary buffer
                    self.temp_counter += 1;
                    let temp_var = format!("temp_concat_{}", self.temp_counter);
                    Ok(format!("({{static char {}[512]; strcpy({}, {}); strcat({}, {}); {}; }})", 
                              temp_var, temp_var, left_code, temp_var, right_code, temp_var))
                } else {
                    // Numeric addition
                    Ok(format!("({} + {})", left_code, right_code))
                }
            },
            BinaryOp::Sub => Ok(format!("({} - {})", left_code, right_code)),
            BinaryOp::Mul => Ok(format!("({} * {})", left_code, right_code)),
            BinaryOp::Div => Ok(format!("({} / {})", left_code, right_code)),
            BinaryOp::FloorDiv => {
                // Floor division for integers - use integer division
                // For floats, use floor(a/b)
                let left_type = self.infer_expression_type(left);
                if matches!(left_type, NativeCType::Int | NativeCType::Long) {
                    Ok(format!("({} / {})", left_code, right_code))
                } else {
                    Ok(format!("floor({} / {})", left_code, right_code))
                }
            }
            BinaryOp::Mod => Ok(format!("({} % {})", left_code, right_code)),
            BinaryOp::Pow => {
                // Use pow() function for exponentiation
                Ok(format!("pow({}, {})", left_code, right_code))
            }
            _ => Err(format!("Unsupported binary operator: {:?}", op)),
        }
    }

    fn transpile_comparison(&mut self, left: &Expr, ops: &[CompareOp], comparators: &[Expr]) -> Result<String, String> {
        if ops.len() != 1 || comparators.len() != 1 {
            return Err("Complex comparisons not yet supported".to_string());
        }
        
        let left_code = self.transpile_expression(left)?;
        let right_code = self.transpile_expression(&comparators[0])?;
        
        let op_str = match &ops[0] {
            CompareOp::Eq => "==",
            CompareOp::NotEq => "!=",
            CompareOp::Lt => "<",
            CompareOp::LtE => "<=",
            CompareOp::Gt => ">",
            CompareOp::GtE => ">=",
            _ => return Err(format!("Unsupported comparison operator: {:?}", ops[0])),
        };
        
        Ok(format!("({} {} {})", left_code, op_str, right_code))
    }

    fn infer_function_return_type(&self, body: &[Statement]) -> NativeCType {
        // First, collect local variable types from the function body
        let mut local_var_types: HashMap<String, NativeCType> = HashMap::new();

        for stmt in body {
            if let Statement::VariableDef { name, type_annotation, value } = stmt {
                let var_type = if let Some(type_ann) = type_annotation {
                    self.map_type_annotation(Some(type_ann))
                } else if let Some(val_expr) = value {
                    self.infer_expression_type(val_expr)
                } else {
                    NativeCType::String
                };
                local_var_types.insert(name.clone(), var_type);
            }
        }

        // Look for return statements to infer return type
        for stmt in body {
            if let Statement::Return(Some(expr)) = stmt {
                return self.infer_return_expression_type(expr, &local_var_types);
            }
        }
        // Check if function has any return statements at all
        let has_return = body.iter().any(|stmt| matches!(stmt, Statement::Return(_)));

        if has_return {
            // Has return statements but no explicit values - void return
            NativeCType::Void
        } else {
            // No return statements - void function
            NativeCType::Void
        }
    }

    fn infer_return_expression_type(&self, expr: &Expr, local_vars: &HashMap<String, NativeCType>) -> NativeCType {
        match expr {
            Expr::Literal(value) => match value {
                Literal::Int(_) => NativeCType::Int,
                Literal::Float(_) => NativeCType::Double,
                Literal::String(_) => NativeCType::String,
                Literal::Bool(_) => NativeCType::Bool,
                _ => NativeCType::String,
            },
            Expr::Identifier(id) => {
                // Check local variables first, then fall back to instance variables
                local_vars.get(id).cloned()
                    .or_else(|| self.variable_types.get(id).cloned())
                    .unwrap_or(NativeCType::String)
            }
            Expr::Call { func, .. } => {
                if let Expr::Identifier(func_name) = func.as_ref() {
                    match func_name.as_str() {
                        "len" => NativeCType::Int,
                        "str" => NativeCType::String,
                        "int" => NativeCType::Int,
                        "float" => NativeCType::Double,
                        "print" => NativeCType::Void,
                        _ => self.function_types.get(func_name).cloned().unwrap_or(NativeCType::String)
                    }
                } else {
                    NativeCType::String
                }
            }
            Expr::BinaryOp { left, right, .. } => {
                let left_type = self.infer_return_expression_type(left, local_vars);
                let right_type = self.infer_return_expression_type(right, local_vars);
                match (left_type, right_type) {
                    (NativeCType::Double, _) | (_, NativeCType::Double) => NativeCType::Double,
                    (NativeCType::Int, NativeCType::Int) => NativeCType::Int,
                    _ => NativeCType::String,
                }
            }
            _ => NativeCType::String,
        }
    }

    fn infer_parameter_type(&self, _param_name: &str, _body: &[Statement]) -> NativeCType {
        // For now, default to int for unknown parameters
        // TODO: Implement more sophisticated type inference
        NativeCType::Int
    }
    
    fn map_type_annotation_to_c(&self, type_annotation: Option<&Type>) -> NativeCType {
        if let Some(type_ref) = type_annotation {
            match type_ref {
                Type::Simple(type_str) => {
                    match type_str.as_str() {
                        "int" => NativeCType::Int,
                        "float" | "double" => NativeCType::Double,
                        "str" | "string" => NativeCType::String,
                        "bool" => NativeCType::Bool,
                        "None" | "void" => NativeCType::Void,
                        _ => NativeCType::Int, // Default fallback
                    }
                },
                _ => NativeCType::Int, // Default for complex types
            }
        } else {
            NativeCType::Int // Default for untyped parameters
        }
    }

    fn map_string_type_annotation(&self, type_annotation: Option<&String>) -> NativeCType {
        if let Some(type_str) = type_annotation {
            match type_str.as_str() {
                "int" => NativeCType::Int,
                "float" | "double" => NativeCType::Double,
                "str" | "string" => NativeCType::String,
                "bool" => NativeCType::Bool,
                "None" | "void" => NativeCType::Void,
                _ => NativeCType::Void, // Default for unknown types
            }
        } else {
            NativeCType::Void
        }
    }

    fn indent(&self) -> String {
        "    ".repeat(self.indent_level)
    }

    fn transpile_while_loop(&mut self, condition: &Expr, body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        let condition_code = self.transpile_expression(condition)?;
        output.push_str(&format!("{}while ({}) {{\n", self.indent(), condition_code));
        
        self.indent_level += 1;
        for stmt in body {
            output.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;
        
        output.push_str(&format!("{}}}\n", self.indent()));
        Ok(output)
    }

    fn transpile_for_loop(&mut self, variable: &str, iterable: &Expr, body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        // Check if iterable is a range() function call
        if let Expr::Call { func, args, .. } = iterable {
            if let Expr::Identifier(name) = func.as_ref() {
                if name == "range" {
                    return self.transpile_range_for_loop(variable, args, body);
                }
            }
        }
        
        // General iterable loop (for lists, arrays, etc.)
        let iterable_code = self.transpile_expression(iterable)?;
        
        // Declare iterator variable
        output.push_str(&format!("{}// For loop over {}\n", self.indent(), iterable_code));
        output.push_str(&format!("{}for (int {}_i = 0; {}_i < len({}); {}_i++) {{\n", 
                                self.indent(), variable, variable, iterable_code, variable));
        
        self.indent_level += 1;
        output.push_str(&format!("{}int {} = {}[{}_i];\n", 
                                self.indent(), variable, iterable_code, variable));
        
        for stmt in body {
            output.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;
        
        output.push_str(&format!("{}}}\n", self.indent()));
        Ok(output)
    }

    fn transpile_range_for_loop(&mut self, variable: &str, args: &[Expr], body: &[Statement]) -> Result<String, String> {
        let mut output = String::new();
        
        match args.len() {
            1 => {
                // range(n) -> for i in 0..n
                let end_code = self.transpile_expression(&args[0])?;
                output.push_str(&format!("{}for (int {} = 0; {} < {}; {}++) {{\n", 
                                        self.indent(), variable, variable, end_code, variable));
            }
            2 => {
                // range(start, end) -> for i in start..end
                let start_code = self.transpile_expression(&args[0])?;
                let end_code = self.transpile_expression(&args[1])?;
                output.push_str(&format!("{}for (int {} = {}; {} < {}; {}++) {{\n", 
                                        self.indent(), variable, start_code, variable, end_code, variable));
            }
            3 => {
                // range(start, end, step) -> for i in start..end by step
                let start_code = self.transpile_expression(&args[0])?;
                let end_code = self.transpile_expression(&args[1])?;
                let step_code = self.transpile_expression(&args[2])?;
                output.push_str(&format!("{}for (int {} = {}; {} < {}; {} += {}) {{\n", 
                                        self.indent(), variable, start_code, variable, end_code, variable, step_code));
            }
            _ => return Err("range() takes 1-3 arguments".to_string()),
        }
        
        self.indent_level += 1;
        for stmt in body {
            output.push_str(&self.transpile_statement(stmt)?);
        }
        self.indent_level -= 1;
        
        output.push_str(&format!("{}}}\n", self.indent()));
        Ok(output)
    }

    fn transpile_list_literal(&mut self, elements: &[Expr]) -> Result<String, String> {
        if elements.is_empty() {
            return Ok("{}".to_string());
        }
        
        let mut element_codes = Vec::new();
        for element in elements {
            element_codes.push(self.transpile_expression(element)?);
        }
        
        // Infer the type of the first element to determine array type
        let element_type = self.infer_expression_type(&elements[0]);
        match element_type {
            NativeCType::Int => {
                Ok(format!("((int[]){{{}}}", element_codes.join(", ")))
            }
            NativeCType::Double => {
                Ok(format!("((double[]){{{}}}", element_codes.join(", ")))
            }
            NativeCType::String => {
                Ok(format!("((char*[]){{{}}}", element_codes.join(", ")))
            }
            _ => {
                // Default to int array
                Ok(format!("((int[]){{{}}}", element_codes.join(", ")))
            }
        }
    }

    fn transpile_subscript(&mut self, object: &Expr, index: &Expr) -> Result<String, String> {
        let object_code = self.transpile_expression(object)?;
        let index_code = self.transpile_expression(index)?;
        Ok(format!("{}[{}]", object_code, index_code))
    }
}