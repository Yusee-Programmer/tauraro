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
            Expr::Call { func, args, kwargs: _ } => {
                let func_code = self.gen_expr(func)?;
                let args_code = args.iter()
                    .map(|arg| self.gen_expr(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("{}({})", func_code, args_code))
            }
            Expr::MethodCall { object, method, args, kwargs: _ } => {
                let obj_code = self.gen_expr(object)?;
                let args_code = args.iter()
                    .map(|arg| self.gen_expr(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("{}.{}({})", obj_code, method, args_code))
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
            Expr::Slice { object, start, stop, step: _ } => {
                let obj_code = self.gen_expr(object)?;
                let start_str = if let Some(s) = start {
                    self.gen_expr(s)?
                } else {
                    "0".to_string()
                };
                let stop_str = if let Some(s) = stop {
                    self.gen_expr(s)?
                } else {
                    "len".to_string()
                };
                Ok(format!("{}[{}..{}]", obj_code, start_str, stop_str))
            }
            Expr::List(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("vec![{}]", items_code))
            }
            Expr::Tuple(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("({})", items_code))
            }
            Expr::Dict(pairs) => {
                let items: Result<Vec<_>> = pairs.iter()
                    .map(|item| {
                        match item {
                            crate::ast::DictItem::KeyValue(key, val) => {
                                let key_code = self.gen_expr(key)?;
                                let val_code = self.gen_expr(val)?;
                                Ok(format!("({}, {})", key_code, val_code))
                            }
                            crate::ast::DictItem::Unpacking(_) => {
                                Ok("// dict unpacking not supported".to_string())
                            }
                        }
                    })
                    .collect();
                let items_str = items?.join(", ");
                Ok(format!("HashMap::from([{}])", items_str))
            }
            Expr::Set(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("std::collections::HashSet::from([{}])", items_code))
            }
            Expr::ListComp { .. } | Expr::DictComp { .. } | Expr::SetComp { .. } | Expr::GeneratorExp { .. } => {
                Ok("// Comprehension not yet supported".to_string())
            }
            Expr::Lambda { .. } => {
                Ok("// Lambda not yet supported".to_string())
            }
            Expr::IfExp { .. } => {
                Ok("// Conditional expression not yet supported".to_string())
            }
            Expr::Await(_) => {
                Ok("// Await not yet supported".to_string())
            }
            Expr::Yield(_) => {
                Ok("// Yield not yet supported".to_string())
            }
            Expr::YieldFrom(_) => {
                Ok("// YieldFrom not yet supported".to_string())
            }
            Expr::FormatString { .. } => {
                Ok("// Format string not yet supported".to_string())
            }
            Expr::Starred(_) => {
                Ok("// Starred expression not yet supported".to_string())
            }
            Expr::StarredKwargs(_) => {
                Ok("// StarredKwargs not yet supported".to_string())
            }
            Expr::NamedExpr { .. } => {
                Ok("// Named expression not yet supported".to_string())
            }
            Expr::Compare { .. } => {
                Ok("// Compare expression not yet supported".to_string())
            }
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
            Literal::Complex { .. } => Ok("// Complex numbers not supported".to_string()),
            Literal::Ellipsis => Ok("// Ellipsis not supported".to_string()),
        }
    }

    /// Generate code for a binary operator
    pub fn gen_binary_op(&self, op: &BinaryOp) -> Result<String> {
        let op_str = match op {
            BinaryOp::Add => "+",
            BinaryOp::Sub => "-",
            BinaryOp::Mul => "*",
            BinaryOp::Div => "/",
            BinaryOp::FloorDiv => "/",
            BinaryOp::Mod => "%",
            BinaryOp::Pow => "// pow",
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
            BinaryOp::MatMul => "// @",
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
