//! Statement Compilation Module
//!
//! This module handles compiling Tauraro statements to C code.
//! NOTE: This is currently a placeholder for future direct AST->C compilation.
//! The main transpiler uses IR instructions instead (see functions.rs).

use crate::ast::*;
use super::expressions;

/// Generate C code for a statement
/// This is a placeholder implementation for future AST->C compilation
pub fn generate_statement(stmt: &Statement) -> String {
    match stmt {
        Statement::Expression(expr) => {
            format!("{};", expressions::generate_expression(expr))
        }
        Statement::VariableDef { name, value, .. } => {
            if let Some(val) = value {
                let value_code = expressions::generate_expression(val);
                format!("tauraro_value_t* {} = {};", name, value_code)
            } else {
                format!("tauraro_value_t* {} = tauraro_value_new();", name)
            }
        }
        Statement::Return(expr) => {
            if let Some(e) = expr {
                format!("return {};", expressions::generate_expression(e))
            } else {
                "return;".to_string()
            }
        }
        Statement::Break => "break;".to_string(),
        Statement::Continue => "continue;".to_string(),
        Statement::Pass => "/* pass */".to_string(),
        _ => "/* statement not yet implemented */".to_string()
    }
}
