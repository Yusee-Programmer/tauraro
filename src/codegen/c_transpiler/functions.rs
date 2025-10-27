//! Function Compilation Module
//!
//! This module handles compiling Tauraro functions to C code,
//! including parameter handling, local variables, and return statements.

use crate::ir::{IRFunction, IRBlock, IRInstruction};
use crate::value::Value;
use crate::ast::Type;
use anyhow::Result;
use std::collections::HashMap;

/// Generate C code for a function
pub fn generate_function(function: &IRFunction) -> Result<String> {
    let mut func_code = String::new();

    // Check if function should return a value
    let returns_value = function_returns_value(function);

    // Function signature
    if returns_value {
        func_code.push_str(&format!("tauraro_value_t* {}(", function.name));
    } else {
        func_code.push_str(&format!("void {}(", function.name));
    }

    // Parameters
    for (i, param) in function.params.iter().enumerate() {
        if i > 0 {
            func_code.push_str(", ");
        }
        // Check if parameter has a specific type
        if let Some(param_type) = function.param_types.get(param) {
            match param_type {
                Type::Simple(type_name) if type_name == "int" => {
                    func_code.push_str(&format!("int64_t {}", param));
                }
                Type::Simple(type_name) if type_name == "float" => {
                    func_code.push_str(&format!("double {}", param));
                }
                Type::Simple(type_name) if type_name == "bool" => {
                    func_code.push_str(&format!("bool {}", param));
                }
                _ => {
                    func_code.push_str(&format!("tauraro_value_t* {}", param));
                }
            }
        } else {
            func_code.push_str(&format!("tauraro_value_t* {}", param));
        }
    }
    func_code.push_str(") {\n");

    // Local variables
    let mut local_vars = HashMap::new();
    func_code.push_str("    // Local variables\n");

    // Process instructions
    for block in &function.blocks {
        for instruction in &block.instructions {
            func_code.push_str(&format!("    {}\n", generate_instruction(instruction, &mut local_vars, &function.param_types)?));
        }
    }

    // Add default return for functions that should return values
    if returns_value {
        let has_return = has_return_statement(function);
        if !has_return {
            func_code.push_str("    return NULL;\n");
        }
    }

    func_code.push_str("}\n");
    Ok(func_code)
}

/// Check if a function returns a value
fn function_returns_value(function: &IRFunction) -> bool {
    function.blocks.iter().any(|block| {
        block.instructions.iter().any(|instr| {
            matches!(instr, IRInstruction::Return { value: Some(_) })
        })
    })
}

/// Check if a function has a return statement
fn has_return_statement(function: &IRFunction) -> bool {
    function.blocks.iter().any(|block| {
        block.instructions.iter().any(|instr| {
            matches!(instr, IRInstruction::Return { .. })
        })
    })
}

/// Generate C code for an IR instruction
pub fn generate_instruction(
    instruction: &IRInstruction,
    local_vars: &mut HashMap<String, String>,
    param_types: &HashMap<String, Type>,
) -> Result<String> {
    match instruction {
        IRInstruction::Comment(text) => {
            // Generate C comment
            Ok(format!("// {}", text))
        },
        IRInstruction::LoadConst { value, result } => {
            generate_load_const(value, result, local_vars)
        }
        IRInstruction::LoadLocal { name, result } => {
            generate_load_local(name, result, local_vars)
        }
        IRInstruction::StoreLocal { name, value } => {
            // Sanitize variable name to avoid C keywords
            let sanitized_name = sanitize_c_identifier(name);

            if !local_vars.contains_key(&sanitized_name) {
                local_vars.insert(sanitized_name.clone(), "tauraro_value_t*".to_string());
                Ok(format!("tauraro_value_t* {} = {};", sanitized_name, value))
            } else {
                Ok(format!("{} = {};", sanitized_name, value))
            }
        }
        IRInstruction::LoadTypedLocal { name, result, type_info: _ } => {
            generate_load_local(name, result, local_vars)
        }
        IRInstruction::StoreTypedLocal { name, value, type_info: _ } => {
            Ok(format!("{} = {};", name, value))
        }
        IRInstruction::LoadGlobal { name, result } => {
            generate_load_global(name, result, local_vars)
        }
        IRInstruction::StoreGlobal { name, value } => {
            Ok(format!("{} = {};", name, value))
        }
        IRInstruction::LoadTypedGlobal { name, result, type_info: _ } => {
            generate_load_global(name, result, local_vars)
        }
        IRInstruction::StoreTypedGlobal { name, value, type_info: _ } => {
            Ok(format!("{} = {};", name, value))
        }
        IRInstruction::BinaryOp { op, left, right, result } => {
            generate_binary_op(op, left, right, result, local_vars)
        }
        IRInstruction::TypedBinaryOp { op, left, right, result, type_info } => {
            generate_typed_binary_op(op, left, right, result, type_info, local_vars)
        }
        IRInstruction::Call { func, args, result } => {
            generate_call(func, args, result, local_vars, param_types)
        }
        IRInstruction::Return { value } => {
            generate_return(value)
        }
        IRInstruction::Jump { target: _ } => {
            Ok("// Jump instruction".to_string())
        }
        IRInstruction::JumpIf { condition, target: _ } => {
            Ok(format!("if ({}->data.bool_val) {{ /* jump */ }}", condition))
        }
        IRInstruction::JumpIfNot { condition, target: _ } => {
            Ok(format!("if (!{}->data.bool_val) {{ /* jump */ }}", condition))
        }
        IRInstruction::ListCreate { elements: _, result } => {
            generate_list_create(result, local_vars)
        }
        IRInstruction::DictCreate { pairs: _, result } => {
            generate_dict_create(result, local_vars)
        }
        IRInstruction::Import { module } => {
            Ok(format!("// Import module: {}", module))
        }
        IRInstruction::ImportFrom { module, names: _ } => {
            Ok(format!("// Import from module: {}", module))
        }
        IRInstruction::ObjectCreate { class_name, result } => {
            generate_object_create(class_name, result, local_vars)
        }
        IRInstruction::ObjectSetAttr { object, attr, value } => {
            Ok(format!("tauraro_object_set_attr({}, \"{}\", {});", object, attr, value))
        }
        IRInstruction::ObjectGetAttr { object, attr, result } => {
            generate_object_get_attr(object, attr, result, local_vars)
        }
        IRInstruction::SuperCall { args, result } => {
            let args_str = if args.is_empty() {
                "0, NULL".to_string()
            } else {
                let arg_list = args.join(", ");
                format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
            };
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = tauraro_super_call({});", result, args_str))
        }
    }
}

fn generate_load_const(value: &Value, result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());

    match value {
        Value::Int(i) => {
            Ok(format!(
                "tauraro_value_t* {} = tauraro_value_new(); {}->type = TAURARO_INT; {}->data.int_val = {};",
                unique_result, unique_result, unique_result, i
            ))
        }
        Value::Float(f) => {
            Ok(format!(
                "tauraro_value_t* {} = tauraro_value_new(); {}->type = TAURARO_FLOAT; {}->data.float_val = {};",
                unique_result, unique_result, unique_result, f
            ))
        }
        Value::Str(s) => {
            // Escape special characters in strings
            let escaped = s
                .replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t");
            Ok(format!(
                "tauraro_value_t* {} = tauraro_value_new(); {}->type = TAURARO_STRING; {}->data.str_val = strdup(\"{}\");",
                unique_result, unique_result, unique_result, escaped
            ))
        }
        Value::Bool(b) => {
            Ok(format!(
                "tauraro_value_t* {} = tauraro_value_new(); {}->type = TAURARO_BOOL; {}->data.bool_val = {};",
                unique_result, unique_result, unique_result, if *b { "true" } else { "false" }
            ))
        }
        Value::None => {
            Ok(format!(
                "tauraro_value_t* {} = tauraro_value_new(); {}->type = TAURARO_NONE;",
                unique_result, unique_result
            ))
        }
        _ => {
            Ok(format!(
                "tauraro_value_t* {} = tauraro_value_new(); {}->type = TAURARO_NONE; // Unsupported constant type",
                unique_result, unique_result
            ))
        }
    }
}

fn generate_load_local(name: &str, result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let sanitized_name = sanitize_c_identifier(name);
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());
    Ok(format!("tauraro_value_t* {} = {};", unique_result, sanitized_name))
}

fn generate_load_global(name: &str, result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());
    Ok(format!("tauraro_value_t* {} = {};", unique_result, name))
}

fn generate_binary_op(
    op: &crate::ast::BinaryOp,
    left: &str,
    right: &str,
    result: &str,
    local_vars: &mut HashMap<String, String>
) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());

    let op_func = match op {
        crate::ast::BinaryOp::Add => "tauraro_add",
        crate::ast::BinaryOp::Sub => "tauraro_sub",
        crate::ast::BinaryOp::Mul => "tauraro_mul",
        crate::ast::BinaryOp::Div => "tauraro_div",
        crate::ast::BinaryOp::Mod => "tauraro_mod",
        crate::ast::BinaryOp::Eq => "tauraro_eq",
        crate::ast::BinaryOp::Ne => "tauraro_ne",
        crate::ast::BinaryOp::Lt => "tauraro_lt",
        crate::ast::BinaryOp::Le => "tauraro_le",
        crate::ast::BinaryOp::Gt => "tauraro_gt",
        crate::ast::BinaryOp::Ge => "tauraro_ge",
        _ => "tauraro_add"  // Fallback
    };

    Ok(format!("tauraro_value_t* {} = {}({}, {});", unique_result, op_func, left, right))
}

fn generate_typed_binary_op(
    op: &crate::ast::BinaryOp,
    left: &str,
    right: &str,
    result: &str,
    type_info: &Type,
    local_vars: &mut HashMap<String, String>
) -> Result<String> {
    match type_info {
        Type::Simple(type_name) if type_name == "int" => {
            local_vars.insert(result.to_string(), "int64_t".to_string());
            match op {
                crate::ast::BinaryOp::Add => {
                    Ok(format!("int64_t {} = tauraro_add_int({}, {});", result, left, right))
                }
                crate::ast::BinaryOp::Sub => {
                    Ok(format!("int64_t {} = {} - {};", result, left, right))
                }
                crate::ast::BinaryOp::Mul => {
                    Ok(format!("int64_t {} = {} * {};", result, left, right))
                }
                crate::ast::BinaryOp::Div => {
                    Ok(format!("int64_t {} = {} / {};", result, left, right))
                }
                _ => {
                    // Fall back to generic operation for other operators
                    Ok(format!("int64_t {} = tauraro_add_int({}, {}); // Typed operation", result, left, right))
                }
            }
        }
        Type::Simple(type_name) if type_name == "float" => {
            local_vars.insert(result.to_string(), "double".to_string());
            match op {
                crate::ast::BinaryOp::Add => {
                    Ok(format!("double {} = tauraro_add_float({}, {});", result, left, right))
                }
                crate::ast::BinaryOp::Sub => {
                    Ok(format!("double {} = {} - {};", result, left, right))
                }
                crate::ast::BinaryOp::Mul => {
                    Ok(format!("double {} = {} * {};", result, left, right))
                }
                crate::ast::BinaryOp::Div => {
                    Ok(format!("double {} = {} / {};", result, left, right))
                }
                _ => {
                    // Fall back to generic operation for other operators
                    Ok(format!("double {} = tauraro_add_float({}, {}); // Typed operation", result, left, right))
                }
            }
        }
        Type::Simple(type_name) if type_name == "str" => {
            local_vars.insert(result.to_string(), "char*".to_string());
            match op {
                crate::ast::BinaryOp::Add => {
                    Ok(format!("char* {} = tauraro_add_string({}, {});", result, left, right))
                }
                _ => {
                    // Fall back to generic operation for other operators
                    Ok(format!("char* {} = tauraro_add_string({}, {}); // Typed operation", result, left, right))
                }
            }
        }
        _ => {
            // Fall back to generic operation for other types
            local_vars.insert(result.to_string(), "tauraro_value_t*".to_string());
            let op_func = match op {
                crate::ast::BinaryOp::Add => "tauraro_add",
                crate::ast::BinaryOp::Sub => "tauraro_sub",
                crate::ast::BinaryOp::Mul => "tauraro_mul",
                crate::ast::BinaryOp::Div => "tauraro_div",
                crate::ast::BinaryOp::Mod => "tauraro_mod",
                crate::ast::BinaryOp::Eq => "tauraro_eq",
                crate::ast::BinaryOp::Ne => "tauraro_ne",
                crate::ast::BinaryOp::Lt => "tauraro_lt",
                crate::ast::BinaryOp::Le => "tauraro_le",
                crate::ast::BinaryOp::Gt => "tauraro_gt",
                crate::ast::BinaryOp::Ge => "tauraro_ge",
                _ => "tauraro_add"  // Fallback
            };
            Ok(format!("tauraro_value_t* {} = {}({}, {}); // Typed operation", result, op_func, left, right))
        }
    }
}

fn generate_call(
    func: &str,
    args: &[String],
    result: &Option<String>,
    local_vars: &mut HashMap<String, String>,
    param_types: &HashMap<String, Type>,
) -> Result<String> {
    let args_str = if args.is_empty() {
        "0, NULL".to_string()
    } else {
        let arg_list = args.join(", ");
        format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_list)
    };

    match result {
        Some(res) => {
            local_vars.insert(res.clone(), "tauraro_value_t*".to_string());
            // Check what kind of function this is
            if func.contains("__") {
                // Method call (class__method) - first argument is self
                if !args.is_empty() {
                    Ok(format!("tauraro_value_t* {} = {}({});", res, func, args_str))
                } else {
                    Ok(format!("tauraro_value_t* {} = {}(0, NULL);", res, func))
                }
            } else if func.contains("_") {
                // Module function (module_function) - call directly
                Ok(format!("tauraro_value_t* {} = {}({});", res, func, args_str))
            } else {
                // Regular builtin function - add tauraro_ prefix
                Ok(format!("tauraro_value_t* {} = tauraro_{}({});", res, func, args_str))
            }
        }
        None => {
            // Check what kind of function this is
            if func.contains("__") {
                // Method call (class__method)
                if !args.is_empty() {
                    Ok(format!("{}({});", func, args_str))
                } else {
                    Ok(format!("{}(0, NULL);", func))
                }
            } else if func.contains("_") {
                // Module function (module_function) - call directly
                if !args.is_empty() {
                    Ok(format!("{}({});", func, args_str))
                } else {
                    Ok(format!("{}(0, NULL);", func))
                }
            } else {
                // Regular builtin function - add tauraro_ prefix
                Ok(format!("tauraro_{}({});", func, args_str))
            }
        }
    }
}

fn generate_return(value: &Option<String>) -> Result<String> {
    match value {
        Some(val) => Ok(format!("return {};", val)),
        None => Ok("return;".to_string())
    }
}

fn generate_list_create(result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());
    Ok(format!("tauraro_value_t* {} = tauraro_list(0, NULL);", unique_result))
}

fn generate_dict_create(result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());
    Ok(format!("tauraro_value_t* {} = tauraro_dict(0, NULL);", unique_result))
}

fn generate_object_create(class_name: &str, result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());
    Ok(format!("tauraro_value_t* {} = tauraro_object_create(\"{}\");", unique_result, class_name))
}

fn generate_object_get_attr(object: &str, attr: &str, result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());
    Ok(format!("tauraro_value_t* {} = tauraro_object_get_attr({}, \"{}\");", unique_result, object, attr))
}

/// Get a unique variable name to avoid conflicts
fn get_unique_var_name(name: &str, local_vars: &HashMap<String, String>) -> String {
    // Sanitize the base name first
    let sanitized_name = sanitize_c_identifier(name);

    if local_vars.contains_key(&sanitized_name) {
        let mut counter = 1;
        let mut new_name = format!("{}_{}", sanitized_name, counter);
        while local_vars.contains_key(&new_name) {
            counter += 1;
            new_name = format!("{}_{}", sanitized_name, counter);
        }
        new_name
    } else {
        sanitized_name
    }
}

/// Sanitize identifiers to avoid C keywords and invalid characters
fn sanitize_c_identifier(name: &str) -> String {
    // List of C keywords that must be avoided
    const C_KEYWORDS: &[&str] = &[
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
        // C99 keywords
        "inline", "restrict", "_Bool", "_Complex", "_Imaginary",
        // C11 keywords
        "_Alignas", "_Alignof", "_Atomic", "_Generic", "_Noreturn",
        "_Static_assert", "_Thread_local"
    ];

    // Check if the name is a C keyword
    if C_KEYWORDS.contains(&name) {
        format!("var_{}", name)
    } else {
        name.to_string()
    }
}