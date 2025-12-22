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
                
                // Use type-safe helper functions for binary operations
                let op_call = match op {
                    BinaryOp::Add => format!("tau_add(&{}, &{})", left_code, right_code),
                    BinaryOp::Sub => format!("tau_sub(&{}, &{})", left_code, right_code),
                    BinaryOp::Mul => format!("tau_mul(&{}, &{})", left_code, right_code),
                    BinaryOp::Div => format!("tau_div(&{}, &{})", left_code, right_code),
                    BinaryOp::FloorDiv => format!("tau_floordiv(&{}, &{})", left_code, right_code),
                    BinaryOp::Mod => format!("tau_mod(&{}, &{})", left_code, right_code),
                    BinaryOp::Pow => format!("tau_pow(&{}, &{})", left_code, right_code),
                    BinaryOp::Eq => format!("TauObject::Bool({} == {})", left_code, right_code),
                    BinaryOp::Ne | BinaryOp::Neq => format!("TauObject::Bool({} != {})", left_code, right_code),
                    BinaryOp::Lt => format!("TauObject::Bool({}.compare(&{}).map(|o| o == std::cmp::Ordering::Less).unwrap_or(false))", left_code, right_code),
                    BinaryOp::Gt => format!("TauObject::Bool({}.compare(&{}).map(|o| o == std::cmp::Ordering::Greater).unwrap_or(false))", left_code, right_code),
                    BinaryOp::Le | BinaryOp::Lte => format!("TauObject::Bool({}.compare(&{}).map(|o| o != std::cmp::Ordering::Greater).unwrap_or(false))", left_code, right_code),
                    BinaryOp::Ge | BinaryOp::Gte => format!("TauObject::Bool({}.compare(&{}).map(|o| o != std::cmp::Ordering::Less).unwrap_or(false))", left_code, right_code),
                    BinaryOp::And => format!("TauObject::Bool({}.is_truthy() && {}.is_truthy())", left_code, right_code),
                    BinaryOp::Or => format!("if {}.is_truthy() {{ {} }} else {{ {} }}", left_code, left_code, right_code),
                    BinaryOp::In => format!("{}.contains(&{}).map(TauObject::Bool).unwrap_or(TauObject::Bool(false))", right_code, left_code),
                    BinaryOp::NotIn => format!("{{ let result = {}.contains(&{}).unwrap_or(false); TauObject::Bool(!result) }}", right_code, left_code),
                    BinaryOp::Is => format!("TauObject::Bool({} == {})", left_code, right_code),
                    BinaryOp::IsNot => format!("TauObject::Bool({} != {})", left_code, right_code),
                    _ => format!("// unsupported operator"),
                };
                
                // Wrap result for error handling
                if matches!(op, BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::FloorDiv | BinaryOp::Mod | BinaryOp::Pow) {
                    Ok(format!("({}).unwrap_or(TauObject::None)", op_call))
                } else {
                    Ok(op_call)
                }
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
                Ok(format!("TauObject::List(vec![{}])", items_code))
            }
            Expr::Tuple(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("TauObject::List(vec![{}])", items_code))
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
                Ok(format!("TauObject::Dict(HashMap::from([{}]))", items_str))
            }
            Expr::Set(items) => {
                let items_code = items.iter()
                    .map(|item| self.gen_expr(item))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("TauObject::List(vec![{}])", items_code))
            }
            Expr::ListComp { element, generators } => {
                self.gen_list_comp(element, generators)
            }
            Expr::DictComp { key, value, generators } => {
                self.gen_dict_comp(key, value, generators)
            }
            Expr::SetComp { element, generators } => {
                self.gen_set_comp(element, generators)
            }
            Expr::GeneratorExp { element, generators } => {
                // Generator expressions can be represented as iterators
                self.gen_list_comp(element, generators)
            }
            Expr::Lambda { params, body } => {
                self.gen_lambda(params, body)
            }
            Expr::IfExp { condition, then_expr, else_expr } => {
                self.gen_if_expr(condition, then_expr, else_expr)
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
            Expr::FormatString { parts } => {
                self.gen_format_string(parts)
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
            Expr::Compare { left, ops, comparators } => {
                self.gen_compare(left, ops, comparators)
            }
        }
    }

    /// Generate code for a literal value
    pub fn gen_literal(&self, lit: &Literal) -> Result<String> {
        match lit {
            Literal::None => Ok("TauObject::None".to_string()),
            Literal::Bool(b) => Ok(format!("TauObject::Bool({})", b)),
            Literal::Int(i) => Ok(format!("TauObject::Int({})", i)),
            Literal::Float(f) => Ok(format!("TauObject::Float({})", f)),
            Literal::String(s) => Ok(format!("TauObject::String(\"{}\".to_string())", 
                s.replace("\"", "\\\"").replace("\n", "\\n").replace("\r", "\\r"))),
            Literal::Bytes(_b) => Ok("TauObject::String(\"bytes\".to_string())".to_string()),
            Literal::Complex { .. } => Ok("TauObject::None // Complex numbers not supported".to_string()),
            Literal::Ellipsis => Ok("TauObject::None // Ellipsis not supported".to_string()),
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

    /// Generate code for a lambda function
    pub fn gen_lambda(&self, params: &[crate::ast::Param], body: &Expr) -> Result<String> {
        // Generate parameter list
        let params_code = if params.is_empty() {
            String::from("|_|")
        } else {
            let param_names: Vec<String> = params.iter()
                .map(|p| p.name.clone())
                .collect();
            format!("|{}|", param_names.join(", "))
        };
        
        let body_code = self.gen_expr(body)?;
        Ok(format!("{} {}", params_code, body_code))
    }

    /// Generate code for list comprehension
    pub fn gen_list_comp(&self, element: &Expr, generators: &[crate::ast::Comprehension]) -> Result<String> {
        if generators.is_empty() {
            return Ok("vec![]".to_string());
        }

        // For now, support single generator with optional filter
        let gen = &generators[0];
        let iter_code = self.gen_expr(&gen.iter)?;
        let element_code = self.gen_expr(element)?;
        
        let mut code = format!("{}.iter()", iter_code);
        
        // Add filters if any
        for filter_expr in &gen.ifs {
            let filter_code = self.gen_expr(filter_expr)?;
            // Replace the target variable in the filter with the iterator variable
            let filter_code = filter_code.replace(&gen.target, &gen.target);
            code = format!("{}.filter(|&{}| {})", code, gen.target, filter_code);
        }
        
        // Map to the element expression
        let element_code = element_code.replace(&gen.target, &gen.target);
        code = format!("{}.map(|&{}| {}).collect::<Vec<_>>()", code, gen.target, element_code);
        
        Ok(code)
    }

    /// Generate code for dict comprehension
    pub fn gen_dict_comp(&self, key: &Expr, value: &Expr, generators: &[crate::ast::Comprehension]) -> Result<String> {
        if generators.is_empty() {
            return Ok("HashMap::new()".to_string());
        }

        let gen = &generators[0];
        let iter_code = self.gen_expr(&gen.iter)?;
        let key_code = self.gen_expr(key)?;
        let value_code = self.gen_expr(value)?;
        
        let mut code = format!("{}.iter()", iter_code);
        
        // Add filters if any
        for filter_expr in &gen.ifs {
            let filter_code = self.gen_expr(filter_expr)?;
            code = format!("{}.filter(|&{}| {})", code, gen.target, filter_code);
        }
        
        // Map to (key, value) pairs
        code = format!("{}.map(|&{}| ({}, {})).collect::<HashMap<_, _>>()", 
            code, gen.target, key_code, value_code);
        
        Ok(code)
    }

    /// Generate code for set comprehension
    pub fn gen_set_comp(&self, element: &Expr, generators: &[crate::ast::Comprehension]) -> Result<String> {
        if generators.is_empty() {
            return Ok("std::collections::HashSet::new()".to_string());
        }

        let gen = &generators[0];
        let iter_code = self.gen_expr(&gen.iter)?;
        let element_code = self.gen_expr(element)?;
        
        let mut code = format!("{}.iter()", iter_code);
        
        // Add filters if any
        for filter_expr in &gen.ifs {
            let filter_code = self.gen_expr(filter_expr)?;
            code = format!("{}.filter(|&{}| {})", code, gen.target, filter_code);
        }
        
        // Map to the element expression
        code = format!("{}.map(|&{}| {}).collect::<std::collections::HashSet<_>>()", 
            code, gen.target, element_code);
        
        Ok(code)
    }

    /// Generate code for conditional expression (ternary)
    pub fn gen_if_expr(&self, condition: &Expr, then_expr: &Expr, else_expr: &Expr) -> Result<String> {
        let cond_code = self.gen_expr(condition)?;
        let then_code = self.gen_expr(then_expr)?;
        let else_code = self.gen_expr(else_expr)?;
        
        Ok(format!("if {} {{ {} }} else {{ {} }}", cond_code, then_code, else_code))
    }

    /// Generate code for format string (f-string)
    pub fn gen_format_string(&self, parts: &[crate::ast::FormatPart]) -> Result<String> {
        if parts.is_empty() {
            return Ok("String::new()".to_string());
        }

        let mut format_parts = Vec::new();
        
        for part in parts {
            match part {
                crate::ast::FormatPart::String(s) => {
                    format_parts.push(format!("\"{}\"", s.replace("\"", "\\\"")));
                }
                crate::ast::FormatPart::Expression { expr, format_spec, conversion: _ } => {
                    let expr_code = self.gen_expr(expr)?;
                    
                    if let Some(spec) = format_spec {
                        // Handle format specifiers like :.2f
                        if spec.starts_with(".") && spec.ends_with("f") {
                            // Float formatting
                            format_parts.push(format!("format!(\"{{:{}}}\", {})", spec, expr_code));
                        } else {
                            format_parts.push(format!("format!(\"{{}}\", {})", expr_code));
                        }
                    } else {
                        format_parts.push(format!("format!(\"{{}}\", {})", expr_code));
                    }
                }
            }
        }
        
        if format_parts.len() == 1 {
            Ok(format_parts[0].clone())
        } else {
            Ok(format!("format!(\"{{}}\", [{}].join(\"\"))", format_parts.join(", ")))
        }
    }

    /// Generate code for comparison expression
    pub fn gen_compare(&self, left: &Expr, ops: &[crate::ast::CompareOp], comparators: &[Expr]) -> Result<String> {
        if ops.is_empty() || comparators.is_empty() {
            return self.gen_expr(left);
        }

        let mut code = String::new();
        let mut current_left = self.gen_expr(left)?;
        
        for (i, (op, right)) in ops.iter().zip(comparators.iter()).enumerate() {
            let right_code = self.gen_expr(right)?;
            
            let comparison = match op {
                crate::ast::CompareOp::In => {
                    // Use contains() method on right operand
                    format!("{}.contains(&{}).map_err(|e| panic!(\"{{}}\" , e)).unwrap_or(false)", right_code, current_left)
                }
                crate::ast::CompareOp::NotIn => {
                    // Negate contains() result
                    format!("!{}.contains(&{}).map_err(|e| panic!(\"{{}}\" , e)).unwrap_or(false)", right_code, current_left)
                }
                crate::ast::CompareOp::Eq => {
                    format!("{} == {}", current_left, right_code)
                }
                crate::ast::CompareOp::NotEq => {
                    format!("{} != {}", current_left, right_code)
                }
                crate::ast::CompareOp::Lt => {
                    format!("({}.compare(&{}).map(|ord| ord == std::cmp::Ordering::Less).unwrap_or(false))", current_left, right_code)
                }
                crate::ast::CompareOp::LtE => {
                    format!("({}.compare(&{}).map(|ord| ord != std::cmp::Ordering::Greater).unwrap_or(false))", current_left, right_code)
                }
                crate::ast::CompareOp::Gt => {
                    format!("({}.compare(&{}).map(|ord| ord == std::cmp::Ordering::Greater).unwrap_or(false))", current_left, right_code)
                }
                crate::ast::CompareOp::GtE => {
                    format!("({}.compare(&{}).map(|ord| ord != std::cmp::Ordering::Less).unwrap_or(false))", current_left, right_code)
                }
                crate::ast::CompareOp::Is => {
                    format!("{} == {}", current_left, right_code)
                }
                crate::ast::CompareOp::IsNot => {
                    format!("{} != {}", current_left, right_code)
                }
            };
            
            if i == 0 {
                code = comparison;
            } else {
                code = format!("{} && {}", code, comparison);
            }
            
            current_left = right_code;
        }
        
        Ok(format!("({})", code))
    }

    /// Generate code for comparison operator
    pub fn gen_compare_op(&self, op: &crate::ast::CompareOp) -> Result<String> {
        use crate::ast::CompareOp;
        let op_str = match op {
            CompareOp::Eq => "==",
            CompareOp::NotEq => "!=",
            CompareOp::Lt => "<",
            CompareOp::LtE => "<=",
            CompareOp::Gt => ">",
            CompareOp::GtE => ">=",
            CompareOp::In | CompareOp::NotIn => return Ok("".to_string()), // Handled in gen_compare
            CompareOp::Is => "==",
            CompareOp::IsNot => "!=",
        };
        Ok(op_str.to_string())
    }
}
