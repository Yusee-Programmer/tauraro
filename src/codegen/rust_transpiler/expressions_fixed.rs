//! Rust code generator for expressions - FIXED VERSION

use crate::ast::{Expr, Literal, BinaryOp, UnaryOp};
use anyhow::Result;
use super::RustCodegenContext;

impl RustCodegenContext {
    /// Generate Rust code for an expression
    pub fn gen_expr(&self, expr: &Expr) -> Result<String> {
        match expr {
            Expr::Literal(lit) => self.gen_literal(lit),
            Expr::Identifier(name) => Ok(name.clone()),
            Expr::BinaryOp { left, op, right } => {
                let left_code = self.gen_expr(left)?;
                let right_code = self.gen_expr(right)?;
                let op_code = self.gen_binary_op(op)?;
                Ok(format!("({} {} {})", left_code, op_code, right_code))
            }
            Expr::UnaryOp { op, operand } => {
                let operand_code = self.gen_expr(operand)?;
                let op_code = self.gen_unary_op(op)?;
                Ok(format!("({}{})", op_code, operand_code))
            }
            Expr::Call { func, args, kwargs: _ } => {
                // Note: kwargs would need special handling for named parameters
                let func_code = self.gen_expr(func)?;
                let args_code = args.iter()
                    .map(|arg| self.gen_expr(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("{}({})", func_code, args_code))
            }
            Expr::Subscript { object, index } => {
                let obj_code = self.gen_expr(object)?;
                let idx_code = self.gen_expr(index)?;
                Ok(format!("{}[{}]", obj_code, idx_code))
            }
            Expr::Attribute { object, name } => {
                let obj_code = self.gen_expr(object)?;
                Ok(format!("{}.{}", obj_code, name))
            }
            Expr::List(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("vec![{}]", items_code))
            }
            Expr::Dict(pairs) => {
                let items: Result<Vec<_>> = pairs.iter()
                    .map(|item| {
                        let key = self.gen_expr(&item.key)?;
                        let val = self.gen_expr(&item.value)?;
                        Ok(format!("({}, {})", key, val))
                    })
                    .collect();
                let items_str = items?.join(", ");
                Ok(format!("HashMap::from([{}])", items_str))
            }
            Expr::Tuple(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("({})", items_code))
            }
            Expr::Set(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("std::collections::HashSet::from([{}])", items_code))
            }
            _ => Ok("// Unsupported expression".to_string()),
        }
    }

    /// Generate code for a literal value
    pub fn gen_literal(&self, lit: &Literal) -> Result<String> {
        match lit {
            Literal::None => Ok("None".to_string()),
            Literal::Bool(b) => Ok(format!("{}", b)),
            Literal::Int(i) => Ok(format!("{}", i)),
            Literal::Float(f) => Ok(format!("{}", f)),
            Literal::String(s) => Ok(format!("\"{}\"", 
                s.replace("\"", "\\\"").replace("\n", "\\n").replace("\r", "\\r"))),
            Literal::Bytes(_b) => Ok("b\"bytes\".to_vec()".to_string()),
        }
    }

    /// Generate code for a binary operator
    pub fn gen_binary_op(&self, op: &BinaryOp) -> Result<String> {
        let op_str = match op {
            BinaryOp::Add => "+",
            BinaryOp::Sub => "-",
            BinaryOp::Mul => "*",
            BinaryOp::Div => "/",
            BinaryOp::FloorDiv => "/",  // Integer division
            BinaryOp::Mod => "%",
            BinaryOp::Pow => "// pow",  // Needs special handling
            BinaryOp::Lt => "<",
            BinaryOp::Gt => ">",
            BinaryOp::Le => "<=",
            BinaryOp::Ge => ">=",
            BinaryOp::Lte => "<=",
            BinaryOp::Gte => ">=",
            BinaryOp::Eq => "==",
            BinaryOp::Ne | BinaryOp::Neq => "!=",
            BinaryOp::And => "&&",
            BinaryOp::Or => "||",
            BinaryOp::BitAnd => "&",
            BinaryOp::BitOr => "|",
            BinaryOp::BitXor => "^",
            BinaryOp::LShift => "<<",
            BinaryOp::RShift => ">>",
            BinaryOp::In => "// in",
            BinaryOp::NotIn => "// not_in",
            BinaryOp::Is => "==",
            BinaryOp::IsNot => "!=",
            BinaryOp::MatMul => "// @",  // Not standard
        };
        Ok(op_str.to_string())
    }

    /// Generate code for a unary operator
    pub fn gen_unary_op(&self, op: &UnaryOp) -> Result<String> {
        let op_str = match op {
            UnaryOp::Not => "!",
            UnaryOp::USub | UnaryOp::Minus => "-",
            UnaryOp::UAdd => "+",
            UnaryOp::Invert | UnaryOp::BitNot => "!",
        };
        Ok(op_str.to_string())
    }
}
