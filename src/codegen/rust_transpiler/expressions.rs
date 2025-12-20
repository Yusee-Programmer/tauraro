//! Rust code generator for expressions

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
            Expr::Call { func, args } => {
                let func_code = self.gen_expr(func)?;
                let args_code = args.iter()
                    .map(|arg| self.gen_expr(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("{}({})", func_code, args_code))
            }
            Expr::Index { object, index } => {
                let obj_code = self.gen_expr(object)?;
                let idx_code = self.gen_expr(index)?;
                Ok(format!("{}[{}]", obj_code, idx_code))
            }
            Expr::Attribute { object, attr } => {
                let obj_code = self.gen_expr(object)?;
                Ok(format!("{}.{}", obj_code, attr))
            }
            Expr::List(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("vec![{}]", items_code))
            }
            Expr::Dict(pairs) => {
                let mut dict_code = String::from("{\n");
                for (key, value) in pairs {
                    let key_code = self.gen_expr(key)?;
                    let val_code = self.gen_expr(value)?;
                    dict_code.push_str(&format!("    {} => {},\n", key_code, val_code));
                }
                dict_code.push_str("}");
                Ok(format!("HashMap::from([{}])", dict_code))
            }
            Expr::String(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
            _ => Err(anyhow::anyhow!("Unsupported expression type")),
        }
    }

    /// Generate code for a literal value
    pub fn gen_literal(&self, lit: &Literal) -> Result<String> {
        match lit {
            Literal::None => Ok("TauObject::None".to_string()),
            Literal::Bool(b) => Ok(format!("TauObject::Bool({})", b)),
            Literal::Integer(i) => Ok(format!("TauObject::Int({})", i)),
            Literal::Float(f) => Ok(format!("TauObject::Float({})", f)),
            Literal::String(s) => Ok(format!("TauObject::String(\"{}\".to_string())", 
                s.replace("\"", "\\\""))),
        }
    }

    /// Generate code for a binary operator
    pub fn gen_binary_op(&self, op: &BinaryOp) -> Result<String> {
        let op_str = match op {
            BinaryOp::Add => "+",
            BinaryOp::Sub => "-",
            BinaryOp::Mul => "*",
            BinaryOp::Div => "/",
            BinaryOp::Mod => "%",
            BinaryOp::Pow => "// Use .pow()",  // Special handling needed
            BinaryOp::Lt => "<",
            BinaryOp::Gt => ">",
            BinaryOp::LtEq => "<=",
            BinaryOp::GtEq => ">=",
            BinaryOp::Eq => "==",
            BinaryOp::NotEq => "!=",
            BinaryOp::And => "&&",
            BinaryOp::Or => "||",
            BinaryOp::BitAnd => "&",
            BinaryOp::BitOr => "|",
            BinaryOp::BitXor => "^",
            BinaryOp::LeftShift => "<<",
            BinaryOp::RightShift => ">>",
            BinaryOp::In => "// Special handling for 'in'",
            BinaryOp::NotIn => "// Special handling for 'not in'",
            _ => return Err(anyhow::anyhow!("Unknown binary operator")),
        };
        Ok(op_str.to_string())
    }

    /// Generate code for a unary operator
    pub fn gen_unary_op(&self, op: &UnaryOp) -> Result<String> {
        let op_str = match op {
            UnaryOp::Not => "!",
            UnaryOp::Neg => "-",
            UnaryOp::Pos => "+",
            UnaryOp::BitNot => "~",
            _ => return Err(anyhow::anyhow!("Unknown unary operator")),
        };
        Ok(op_str.to_string())
    }
}
