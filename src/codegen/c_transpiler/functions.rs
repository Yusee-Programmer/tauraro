//! Function Compilation Module
//!
//! This module handles compiling Tauraro functions to C code,
//! including parameter handling, local variables, and return statements.

use crate::ir::{IRFunction, IRInstruction};
use crate::value::Value;
use crate::ast::Type;
use anyhow::Result;
use std::collections::{HashMap, HashSet};

/// Generate C code for a function
pub fn generate_function(function: &IRFunction, class_names: &HashSet<String>) -> Result<String> {
    let mut func_code = String::new();

    // All Python functions return a value (None if no explicit return)
    // So all C functions should return tauraro_value_t*
    func_code.push_str(&format!("tauraro_value_t* {}(int argc, tauraro_value_t** argv) {{\n", function.name));

    // Extract parameters from argv array
    if !function.params.is_empty() {
        func_code.push_str("    // Extract parameters\n");
        for (i, param) in function.params.iter().enumerate() {
            // Check if parameter has a specific type
            if let Some(param_type) = function.param_types.get(param) {
                match param_type {
                    Type::Simple(type_name) if type_name == "int" => {
                        func_code.push_str(&format!("    int64_t {} = (argc > {}) ? argv[{}]->data.int_val : 0;\n", param, i, i));
                    }
                    Type::Simple(type_name) if type_name == "float" => {
                        func_code.push_str(&format!("    double {} = (argc > {}) ? argv[{}]->data.float_val : 0.0;\n", param, i, i));
                    }
                    Type::Simple(type_name) if type_name == "bool" => {
                        func_code.push_str(&format!("    bool {} = (argc > {}) ? argv[{}]->data.bool_val : false;\n", param, i, i));
                    }
                    _ => {
                        func_code.push_str(&format!("    tauraro_value_t* {} = (argc > {}) ? argv[{}] : NULL;\n", param, i, i));
                    }
                }
            } else {
                func_code.push_str(&format!("    tauraro_value_t* {} = (argc > {}) ? argv[{}] : NULL;\n", param, i, i));
            }
        }
        func_code.push_str("\n");
    }

    // Local variables
    let mut local_vars = HashMap::new();
    func_code.push_str("    // Local variables\n");

    // Process instructions
    for block in &function.blocks {
        for instruction in &block.instructions {
            func_code.push_str(&format!("    {}\n", generate_instruction(instruction, &mut local_vars, &function.param_types, class_names)?));
        }
    }

    // Add default return for functions without explicit return
    // Python functions always return None if no explicit return
    let has_return = has_return_statement(function);
    if !has_return {
        func_code.push_str("    // Implicit return None\n");
        func_code.push_str("    tauraro_value_t* none_val = tauraro_value_new();\n");
        func_code.push_str("    none_val->type = TAURARO_NONE;\n");
        func_code.push_str("    return none_val;\n");
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
    class_names: &HashSet<String>,
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
        IRInstruction::LoadTypedLocal { name, result, type_info } => {
            generate_load_typed_local(name, result, type_info, local_vars)
        }
        IRInstruction::StoreTypedLocal { name, value, type_info } => {
            generate_store_typed_local(name, value, type_info, local_vars)
        }
        IRInstruction::LoadGlobal { name, result } => {
            generate_load_global(name, result, local_vars, class_names)
        }
        IRInstruction::StoreGlobal { name, value } => {
            Ok(format!("{} = {};", name, value))
        }
        IRInstruction::LoadTypedGlobal { name, result, type_info } => {
            generate_load_typed_global(name, result, type_info, local_vars, class_names)
        }
        IRInstruction::StoreTypedGlobal { name, value, type_info } => {
            generate_store_typed_global(name, value, type_info, local_vars)
        }
        IRInstruction::BinaryOp { op, left, right, result } => {
            generate_binary_op(op, left, right, result, local_vars)
        }
        IRInstruction::UnaryOp { op, operand, result } => {
            generate_unary_op(op, operand, result, local_vars)
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
        IRInstruction::If { condition, then_body, elif_branches, else_body } => {
            generate_if(condition, then_body, elif_branches, else_body, local_vars, param_types, class_names)
        }
        IRInstruction::While { condition, condition_instructions, body } => {
            generate_while(condition, condition_instructions, body, local_vars, param_types, class_names)
        }
        IRInstruction::For { variable, variables, iterable, body } => {
            // The C generator currently supports simple loop variable cases.
            // For complex nested targets, fall back to using the primary variable name
            // (the first identifier) when generating C code. Proper handling of
            // nested assigns requires more advanced codegen which is out of scope
            // for the quick patch.
            generate_for(variable, &variables, iterable, body, local_vars, param_types, class_names)
        }
        IRInstruction::Break => {
            Ok("break;".to_string())
        }
        IRInstruction::Continue => {
            Ok("continue;".to_string())
        }
        IRInstruction::Try { body, handlers: _, else_body: _, finally_body: _ } => {
            // Simple try block - just execute the body for now
            let mut code = String::new();
            code.push_str("// Try block (exception handling not fully implemented)\n");
            for instruction in body {
                let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
                code.push_str(&format!("    {}\n", instr_code));
            }
            Ok(code)
        }
        IRInstruction::Raise { exception: _ } => {
            Ok("// Raise exception (not fully implemented)".to_string())
        }
        IRInstruction::ListCreate { elements: _, result } => {
            generate_list_create(result, local_vars)
        }
        IRInstruction::DictCreate { pairs, result } => {
            generate_dict_create(pairs, result, local_vars)
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
        IRInstruction::DictSetItem { dict, key, value } => {
            Ok(format!("tauraro_dict_set({}, {}, {});", dict, key, value))
        }
        IRInstruction::DictGetItem { dict, key, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = tauraro_dict_get({}, {});", result, dict, key))
        }
        
        // ===== NEW ADVANCED INSTRUCTION HANDLERS =====
        
        // Lambda expression
        IRInstruction::Lambda { params, body_instructions, captured_vars, result, body_result_var } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = NULL; // Lambda (handled by main transpiler)", result))
        }
        
        // List comprehension
        IRInstruction::ListComprehension { element_instrs, element_result, variable, iterable, condition_instrs, condition_result, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = tauraro_create_list(); // List comprehension", result))
        }
        
        // Dict comprehension
        IRInstruction::DictComprehension { key_instrs, key_result, value_instrs, value_result, variable, iterable, condition_instrs, condition_result, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = tauraro_create_dict(); // Dict comprehension", result))
        }
        
        // Slice operation
        IRInstruction::Slice { object, start, stop, step, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            let start_str = start.as_ref().map(|s| s.as_str()).unwrap_or("0");
            let stop_str = stop.as_ref().map(|s| s.as_str()).unwrap_or("-1");
            let step_str = step.as_ref().map(|s| s.as_str()).unwrap_or("1");
            Ok(format!("tauraro_value_t* {} = tauraro_slice({}, {}, {}, {});", result, object, start_str, stop_str, step_str))
        }
        
        // Tuple create
        IRInstruction::TupleCreate { elements, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            let elem_list = elements.join(", ");
            Ok(format!("tauraro_value_t* {} = tauraro_create_tuple({}, (tauraro_value_t*[]){{{}}});", result, elements.len(), elem_list))
        }
        
        // Tuple unpack
        IRInstruction::TupleUnpack { tuple, targets } => {
            let mut code = String::new();
            for (i, target) in targets.iter().enumerate() {
                local_vars.insert(target.clone(), "tauraro_value_t*".to_string());
                code.push_str(&format!("tauraro_value_t* {} = tauraro_tuple_get({}, {});\n", target, tuple, i));
            }
            Ok(code)
        }
        
        // Tuple get item
        IRInstruction::TupleGetItem { tuple, index, result } => {
            local_vars.insert(result.clone(), "TauValue".to_string());
            Ok(format!("TauValue {} = tauraro_tuple_get({}, {});", result, tuple, index))
        }
        
        // F-string / Format string
        IRInstruction::FormatString { parts, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = tauraro_str(\"\"); // Format string", result))
        }
        
        // Context manager (with statement)
        IRInstruction::With { context_expr, alias, body } => {
            let mut code = String::new();
            code.push_str("// With statement (context manager)\n");
            code.push_str("{ // Context manager scope\n");
            for instr in body {
                let instr_code = generate_instruction(instr, local_vars, param_types, class_names)?;
                code.push_str(&format!("    {}\n", instr_code));
            }
            code.push_str("}\n");
            Ok(code)
        }
        
        // Yield (generators)
        IRInstruction::Yield { value } => {
            if let Some(val) = value {
                Ok(format!("_gen_yield = {}; return _gen_yield;", val))
            } else {
                Ok("return tauraro_none();".to_string())
            }
        }
        
        // Yield from
        IRInstruction::YieldFrom { iterable } => {
            Ok(format!("// Yield from {} (handled at generator level)", iterable))
        }
        
        // Match statement
        IRInstruction::Match { value, cases, result } => {
            let mut code = String::new();
            code.push_str(&format!("// Match statement on {}\n", value));
            code.push_str("do {\n");
            for (i, case) in cases.iter().enumerate() {
                code.push_str(&format!("    // Case {}\n", i));
                for instr in &case.body {
                    let instr_code = generate_instruction(instr, local_vars, param_types, class_names)?;
                    code.push_str(&format!("    {}\n", instr_code));
                }
            }
            code.push_str("} while(0);\n");
            Ok(code)
        }
        
        // Pack *args
        IRInstruction::PackArgs { args, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            let args_list = args.join(", ");
            Ok(format!("tauraro_value_t* {} = tauraro_pack_args({}, (tauraro_value_t*[]){{{}}});", result, args.len(), args_list))
        }
        
        // Unpack *args
        IRInstruction::UnpackArgs { args, targets } => {
            let mut code = String::new();
            for (i, target) in targets.iter().enumerate() {
                local_vars.insert(target.clone(), "tauraro_value_t*".to_string());
                code.push_str(&format!("tauraro_value_t* {} = tauraro_tuple_get({}, {});\n", target, args, i));
            }
            Ok(code)
        }
        
        // Pack **kwargs
        IRInstruction::PackKwargs { pairs, result } => {
            local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
            Ok(format!("tauraro_value_t* {} = tauraro_create_dict(); // Pack kwargs", result))
        }
        
        // Unpack **kwargs
        IRInstruction::UnpackKwargs { kwargs, targets } => {
            let mut code = String::new();
            for target in targets {
                local_vars.insert(target.clone(), "tauraro_value_t*".to_string());
                code.push_str(&format!("tauraro_value_t* {} = tauraro_dict_get({}, \"{}\");\n", target, kwargs, target));
            }
            Ok(code)
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

fn generate_load_global(name: &str, result: &str, local_vars: &mut HashMap<String, String>, class_names: &HashSet<String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());

    // If this is a class name, use the class instance variable (class_ClassName)
    let actual_name = if class_names.contains(name) {
        format!("class_{}", name)
    } else {
        name.to_string()
    };

    Ok(format!("tauraro_value_t* {} = {};", unique_result, actual_name))
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

fn generate_unary_op(
    op: &crate::ast::UnaryOp,
    operand: &str,
    result: &str,
    local_vars: &mut HashMap<String, String>
) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());

    let op_func = match op {
        crate::ast::UnaryOp::Not => "tauraro_not",
        crate::ast::UnaryOp::USub | crate::ast::UnaryOp::Minus => "tauraro_negate",
        crate::ast::UnaryOp::UAdd => "tauraro_positive",
        crate::ast::UnaryOp::Invert | crate::ast::UnaryOp::BitNot => "tauraro_invert",
    };

    Ok(format!("tauraro_value_t* {} = {}({});", unique_result, op_func, operand))
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
            let unique_res = get_unique_var_name(res, local_vars);
            local_vars.insert(unique_res.clone(), "tauraro_value_t*".to_string());
            // Check what kind of function this is
            if func.contains("__") {
                // Method call (class__method) - first argument is self
                if !args.is_empty() {
                    Ok(format!("tauraro_value_t* {} = {}({});", unique_res, func, args_str))
                } else {
                    Ok(format!("tauraro_value_t* {} = {}(0, NULL);", unique_res, func))
                }
            } else if func.contains("_") {
                // Module function (module_function) - call directly
                Ok(format!("tauraro_value_t* {} = {}({});", unique_res, func, args_str))
            } else {
                // Regular builtin function - add tauraro_ prefix
                Ok(format!("tauraro_value_t* {} = tauraro_{}({});", unique_res, func, args_str))
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

fn generate_dict_create(pairs: &[(String, String)], result: &str, local_vars: &mut HashMap<String, String>) -> Result<String> {
    let unique_result = get_unique_var_name(result, local_vars);
    local_vars.insert(unique_result.clone(), "tauraro_value_t*".to_string());

    if pairs.is_empty() {
        return Ok(format!("tauraro_value_t* {} = tauraro_dict(0, NULL);", unique_result));
    }

    // Create a dict with the key-value pairs
    let mut code = String::new();
    code.push_str(&format!("tauraro_value_t* {} = tauraro_dict(0, NULL);\n", unique_result));

    // Add each key-value pair to the dict
    for (key, value) in pairs {
        code.push_str(&format!("    tauraro_dict_set({}, {}, {});\n", unique_result, key, value));
    }

    Ok(code.trim_end().to_string())
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

/// Generate C code for an if statement
fn generate_if(
    condition: &str,
    then_body: &[IRInstruction],
    elif_branches: &[(String, Vec<IRInstruction>)],
    else_body: &Option<Vec<IRInstruction>>,
    local_vars: &mut HashMap<String, String>,
    param_types: &HashMap<String, Type>,
    class_names: &HashSet<String>,
) -> Result<String> {
    let mut code = String::new();

    // Generate condition check - handle both tauraro_value_t* and direct bool values
    code.push_str(&format!("if (tauraro_is_truthy({})) {{\n", condition));

    // Generate then body
    for instruction in then_body {
        let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
        code.push_str(&format!("        {}\n", instr_code));
    }
    code.push_str("    }");

    // Generate elif branches
    for (elif_cond, elif_body) in elif_branches {
        code.push_str(&format!(" else if (tauraro_is_truthy({})) {{\n", elif_cond));
        for instruction in elif_body {
            let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
            code.push_str(&format!("        {}\n", instr_code));
        }
        code.push_str("    }");
    }

    // Generate else body
    if let Some(else_instructions) = else_body {
        code.push_str(" else {\n");
        for instruction in else_instructions {
            let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
            code.push_str(&format!("        {}\n", instr_code));
        }
        code.push_str("    }");
    }

    Ok(code)
}

/// Generate C code for a while loop
fn generate_while(
    condition: &str,
    condition_instructions: &[IRInstruction],
    body: &[IRInstruction],
    local_vars: &mut HashMap<String, String>,
    param_types: &HashMap<String, Type>,
    class_names: &HashSet<String>,
) -> Result<String> {
    let mut code = String::new();

    // Generate while header
    code.push_str(&format!("while (tauraro_is_truthy({})) {{\n", condition));

    // Generate body
    for instruction in body {
        let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
        code.push_str(&format!("        {}\n", instr_code));
    }

    // Re-evaluate condition at end of loop
    code.push_str("        // Re-evaluate condition\n");
    for instr in condition_instructions {
        let instr_code = generate_instruction(instr, local_vars, param_types, class_names)?;
        code.push_str(&format!("        {}\n", instr_code));
    }
    code.push_str("    }");

    Ok(code)
}

/// Generate C code for a for loop
fn generate_for(
    variable: &str,
    variables: &Vec<crate::ast::AssignTarget>,
    iterable: &str,
    body: &[IRInstruction],
    local_vars: &mut HashMap<String, String>,
    param_types: &HashMap<String, Type>,
    class_names: &HashSet<String>,
) -> Result<String> {
    let mut code = String::new();

    // Generate iterator setup
    let iterator_var = format!("{}_iter", variable);
    let index_var = format!("{}_index", variable);

    code.push_str(&format!("// For loop: {} in {}\n", variable, iterable));
    code.push_str(&format!("    tauraro_value_t* {} = {};\n", iterator_var, iterable));
    code.push_str(&format!("    int {} = 0;\n", index_var));

    // Handle different iterable types
    code.push_str(&format!("    if ({}->type == TAURARO_LIST) {{\n", iterator_var));
    code.push_str(&format!("        int iter_len = tauraro_len(1, (tauraro_value_t*[]){{{}}})-> data.int_val;\n", iterator_var));
    code.push_str(&format!("        for ({} = 0; {} < iter_len; {}++) {{\n", index_var, index_var, index_var));

    // Get current element
    // If the loop target is a simple identifier, use it directly. For complex
    // nested targets we still store the element into the primary variable name
    // (variable) and the bytecode compiler will be responsible for unpacking
    // into nested targets at runtime. Here we ensure the C code has a variable
    // to reference.
    local_vars.insert(variable.to_string(), "tauraro_value_t*".to_string());
    code.push_str(&format!("            tauraro_value_t* {} = {}->data.list_val[{}];\n", variable, iterator_var, index_var));

    // Generate body
    for instruction in body {
        let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
        code.push_str(&format!("            {}\n", instr_code));
    }

    code.push_str("        }\n");
    code.push_str("    } else if (");
    code.push_str(&format!("{}->type == TAURARO_RANGE) {{\n", iterator_var));
    code.push_str(&format!("        int start = {}->data.range_val.start;\n", iterator_var));
    code.push_str(&format!("        int stop = {}->data.range_val.stop;\n", iterator_var));
    code.push_str(&format!("        int step = {}->data.range_val.step;\n", iterator_var));
    code.push_str(&format!("        for (int i = start; (step > 0) ? (i < stop) : (i > stop); i += step) {{\n"));

    // Create tauraro_value_t for the loop variable
    local_vars.insert(variable.to_string(), "tauraro_value_t*".to_string());
    code.push_str(&format!("            tauraro_value_t* {} = tauraro_value_new();\n", variable));
    code.push_str(&format!("            {}->type = TAURARO_INT;\n", variable));
    code.push_str(&format!("            {}->data.int_val = i;\n", variable));

    // Generate body
    for instruction in body {
        let instr_code = generate_instruction(instruction, local_vars, param_types, class_names)?;
        code.push_str(&format!("            {}\n", instr_code));
    }

    code.push_str("        }\n");
    code.push_str("    } else {\n");
    code.push_str("        // TODO: Handle other iterable types (dict, tuple, set, string)\n");
    code.push_str("    }");

    Ok(code)
}

/// Generate optimized code for loading a typed local variable
fn generate_load_typed_local(
    name: &str,
    result: &str,
    type_info: &Type,
    local_vars: &mut HashMap<String, String>
) -> Result<String> {
    let sanitized_name = sanitize_c_identifier(name);

    match type_info {
        Type::Simple(type_name) if type_name == "int" => {
            // For typed int variables, we can use primitive int64_t directly
            local_vars.insert(result.to_string(), "int64_t".to_string());
            Ok(format!("int64_t {} = {}; // Optimized: typed int", result, sanitized_name))
        }
        Type::Simple(type_name) if type_name == "float" => {
            // For typed float variables, we can use primitive double directly
            local_vars.insert(result.to_string(), "double".to_string());
            Ok(format!("double {} = {}; // Optimized: typed float", result, sanitized_name))
        }
        Type::Simple(type_name) if type_name == "bool" => {
            // For typed bool variables, we can use primitive bool directly
            local_vars.insert(result.to_string(), "bool".to_string());
            Ok(format!("bool {} = {}; // Optimized: typed bool", result, sanitized_name))
        }
        Type::Simple(type_name) if type_name == "str" => {
            // For typed str variables, use char* for efficiency
            local_vars.insert(result.to_string(), "char*".to_string());
            Ok(format!("char* {} = {}; // Optimized: typed str", result, sanitized_name))
        }
        _ => {
            // For complex types, fall back to generic handling
            generate_load_local(name, result, local_vars)
        }
    }
}

/// Generate optimized code for storing a typed local variable
fn generate_store_typed_local(
    name: &str,
    value: &str,
    type_info: &Type,
    local_vars: &mut HashMap<String, String>
) -> Result<String> {
    let sanitized_name = sanitize_c_identifier(name);

    match type_info {
        Type::Simple(type_name) if type_name == "int" => {
            // For typed int variables, declare and use primitive int64_t
            if !local_vars.contains_key(&sanitized_name) {
                local_vars.insert(sanitized_name.clone(), "int64_t".to_string());
                Ok(format!("int64_t {} = {}; // Optimized: typed int", sanitized_name, value))
            } else {
                Ok(format!("{} = {}; // Optimized: typed int assignment", sanitized_name, value))
            }
        }
        Type::Simple(type_name) if type_name == "float" => {
            // For typed float variables, declare and use primitive double
            if !local_vars.contains_key(&sanitized_name) {
                local_vars.insert(sanitized_name.clone(), "double".to_string());
                Ok(format!("double {} = {}; // Optimized: typed float", sanitized_name, value))
            } else {
                Ok(format!("{} = {}; // Optimized: typed float assignment", sanitized_name, value))
            }
        }
        Type::Simple(type_name) if type_name == "bool" => {
            // For typed bool variables, declare and use primitive bool
            if !local_vars.contains_key(&sanitized_name) {
                local_vars.insert(sanitized_name.clone(), "bool".to_string());
                Ok(format!("bool {} = {}; // Optimized: typed bool", sanitized_name, value))
            } else {
                Ok(format!("{} = {}; // Optimized: typed bool assignment", sanitized_name, value))
            }
        }
        Type::Simple(type_name) if type_name == "str" => {
            // For typed str variables, use char* for efficiency
            if !local_vars.contains_key(&sanitized_name) {
                local_vars.insert(sanitized_name.clone(), "char*".to_string());
                Ok(format!("char* {} = {}; // Optimized: typed str", sanitized_name, value))
            } else {
                Ok(format!("{} = {}; // Optimized: typed str assignment", sanitized_name, value))
            }
        }
        _ => {
            // For complex types or unannotated, fall back to generic handling
            if !local_vars.contains_key(&sanitized_name) {
                local_vars.insert(sanitized_name.clone(), "tauraro_value_t*".to_string());
                Ok(format!("tauraro_value_t* {} = {};", sanitized_name, value))
            } else {
                Ok(format!("{} = {};", sanitized_name, value))
            }
        }
    }
}

/// Generate optimized code for loading a typed global variable
fn generate_load_typed_global(
    name: &str,
    result: &str,
    type_info: &Type,
    local_vars: &mut HashMap<String, String>,
    class_names: &HashSet<String>,
) -> Result<String> {
    match type_info {
        Type::Simple(type_name) if type_name == "int" => {
            local_vars.insert(result.to_string(), "int64_t".to_string());
            Ok(format!("int64_t {} = {}; // Optimized: typed int global", result, name))
        }
        Type::Simple(type_name) if type_name == "float" => {
            local_vars.insert(result.to_string(), "double".to_string());
            Ok(format!("double {} = {}; // Optimized: typed float global", result, name))
        }
        Type::Simple(type_name) if type_name == "bool" => {
            local_vars.insert(result.to_string(), "bool".to_string());
            Ok(format!("bool {} = {}; // Optimized: typed bool global", result, name))
        }
        _ => {
            generate_load_global(name, result, local_vars, class_names)
        }
    }
}

/// Generate optimized code for storing a typed global variable
fn generate_store_typed_global(
    name: &str,
    value: &str,
    type_info: &Type,
    _local_vars: &mut HashMap<String, String>
) -> Result<String> {
    match type_info {
        Type::Simple(type_name) if type_name == "int" => {
            Ok(format!("{} = {}; // Optimized: typed int global assignment", name, value))
        }
        Type::Simple(type_name) if type_name == "float" => {
            Ok(format!("{} = {}; // Optimized: typed float global assignment", name, value))
        }
        Type::Simple(type_name) if type_name == "bool" => {
            Ok(format!("{} = {}; // Optimized: typed bool global assignment", name, value))
        }
        _ => {
            Ok(format!("{} = {};", name, value))
        }
    }
}