//! Expression Compilation Module
//!
//! This module handles compiling Tauraro expressions to C code.
//! NOTE: This is currently a placeholder for future direct AST->C compilation.
//! The main transpiler uses IR instructions instead (see functions.rs).

use crate::ast::*;

/// Generate C code for an expression
pub fn generate_expression(expr: &Expr) -> String {
    match expr {
        Expr::Literal(lit) => generate_literal(lit),
        Expr::Identifier(name) => name.clone(),
        Expr::BinaryOp { op, left, right } => {
            generate_binary_op_expr(op, left, right)
        }
        Expr::UnaryOp { op, operand } => {
            generate_unary_op_expr(op, operand)
        }
        Expr::Call { func, args, kwargs: _ } => {
            generate_call_expr(func, args)
        }
        Expr::Attribute { object, name } => {
            generate_attribute_expr(object, name)
        }
        Expr::Subscript { object, index } => {
            generate_index_expr(object, index)
        }
        Expr::List(elements) => {
            generate_list_expr(elements)
        }
        Expr::Dict(pairs) => {
            generate_dict_expr(pairs)
        }
        Expr::Tuple(elements) => {
            generate_tuple_expr(elements)
        }
        Expr::Set(elements) => {
            generate_set_expr(elements)
        }
        _ => "/* unsupported expression */".to_string()
    }
}

fn generate_literal(lit: &Literal) -> String {
    match lit {
        Literal::Int(i) => i.to_string(),
        Literal::Float(f) => f.to_string(),
        Literal::String(s) => format!("\"{}\"", s.replace("\"", "\\\"")),
        Literal::Bool(b) => if *b { "true" } else { "false" }.to_string(),
        Literal::None => "NULL".to_string(),
        _ => "/* unsupported literal */".to_string()
    }
}

fn generate_binary_op_expr(op: &BinaryOp, left: &Box<Expr>, right: &Box<Expr>) -> String {
    let left_code = generate_expression(left);
    let right_code = generate_expression(right);

    let op_func = match op {
        BinaryOp::Add => "tauraro_add",
        BinaryOp::Sub => "tauraro_sub",
        BinaryOp::Mul => "tauraro_mul",
        BinaryOp::Div => "tauraro_div",
        BinaryOp::Mod => "tauraro_mod",
        BinaryOp::Eq => "tauraro_eq",
        BinaryOp::Ne => "tauraro_ne",
        BinaryOp::Lt => "tauraro_lt",
        BinaryOp::Le => "tauraro_le",
        BinaryOp::Gt => "tauraro_gt",
        BinaryOp::Ge => "tauraro_ge",
        _ => "tauraro_add"
    };

    format!("{}({}, {})", op_func, left_code, right_code)
}

fn generate_unary_op_expr(op: &UnaryOp, operand: &Box<Expr>) -> String {
    let operand_code = generate_expression(operand);

    match op {
        UnaryOp::Not => format!("(!{})", operand_code),
        UnaryOp::USub | UnaryOp::Minus => format!("(-{})", operand_code),
        UnaryOp::UAdd => operand_code,
        _ => operand_code,
    }
}

fn generate_call_expr(func: &Box<Expr>, args: &[Expr]) -> String {
    let func_name = match &**func {
        Expr::Identifier(name) => name.clone(),
        Expr::Attribute { object: _, name } => name.clone(),
        _ => "unknown_func".to_string()
    };

    let args_str = if args.is_empty() {
        "0, NULL".to_string()
    } else {
        let arg_exprs: Vec<String> = args.iter()
            .map(|arg| generate_expression(arg))
            .collect();
        format!("{}, (tauraro_value_t*[]){{{}}}", args.len(), arg_exprs.join(", "))
    };

    format!("tauraro_{}({})", func_name, args_str)
}

fn generate_attribute_expr(object: &Box<Expr>, name: &str) -> String {
    let object_code = generate_expression(object);
    format!("tauraro_object_get_attr({}, \"{}\")", object_code, name)
}

fn generate_index_expr(object: &Box<Expr>, index: &Box<Expr>) -> String {
    let object_code = generate_expression(object);
    let index_code = generate_expression(index);
    format!("tauraro_list_get({}, {}->data.int_val)", object_code, index_code)
}

fn generate_list_expr(elements: &[Expr]) -> String {
    let mut code = "tauraro_list(0, NULL)".to_string();

    if !elements.is_empty() {
        code = format!("/* list with {} elements */", elements.len());
    }

    code
}

fn generate_dict_expr(items: &[crate::ast::DictItem]) -> String {
    let mut code = "tauraro_dict(0, NULL)".to_string();

    if !items.is_empty() {
        code = format!("/* dict with {} items */", items.len());
    }

    code
}

fn generate_tuple_expr(elements: &[Expr]) -> String {
    let args_str = if elements.is_empty() {
        "0, NULL".to_string()
    } else {
        let element_exprs: Vec<String> = elements.iter()
            .map(|elem| generate_expression(elem))
            .collect();
        format!("{}, (tauraro_value_t*[]){{{}}}", elements.len(), element_exprs.join(", "))
    };

    format!("tauraro_tuple({})", args_str)
}

fn generate_set_expr(elements: &[Expr]) -> String {
    let mut code = "tauraro_set(0, NULL)".to_string();

    if !elements.is_empty() {
        code = format!("/* set with {} elements */", elements.len());
    }

    code
}
