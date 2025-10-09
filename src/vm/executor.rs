//! Complete VM statement executor with JIT-style optimizations
//! This module provides full Python-compatible statement execution

use crate::ast::*;
use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::vm::memory::Scope;
use anyhow::{Result, anyhow};
use std::collections::HashMap;

/// JIT hot path tracker for optimization
pub struct HotPathTracker {
    execution_counts: HashMap<usize, u64>,
    hot_threshold: u64,
}

impl HotPathTracker {
    pub fn new() -> Self {
        Self {
            execution_counts: HashMap::new(),
            hot_threshold: 1000, // Mark as hot after 1000 executions
        }
    }

    #[inline(always)]
    pub fn record_execution(&mut self, stmt_id: usize) -> bool {
        let count = self.execution_counts.entry(stmt_id).or_insert(0);
        *count += 1;
        *count >= self.hot_threshold
    }
}

/// Complete statement executor implementation
pub trait CompleteExecutor {
    fn execute_statement_complete(&mut self, statement: &Statement) -> Result<()>;
    fn evaluate_expression_complete(&mut self, expr: &Expr) -> Result<Value>;
    fn get_scope_mut(&mut self) -> &mut Scope;
    fn get_scope(&self) -> &Scope;
    fn push_scope_internal(&mut self, scope: Scope);
    fn pop_scope_internal(&mut self) -> Scope;
    fn set_return(&mut self, value: Option<Value>);
    fn check_return(&self) -> bool;
    fn check_break(&self) -> bool;
    fn check_continue(&self) -> bool;
    fn set_break(&mut self, val: bool);
    fn set_continue(&mut self, val: bool);
}

/// Execute For loop with JIT-style optimizations
#[inline(always)]
pub fn execute_for_loop<E: CompleteExecutor>(
    executor: &mut E,
    variable: &str,
    iterable: &Expr,
    body: &[Statement],
) -> Result<()> {
    let iter_val = executor.evaluate_expression_complete(iterable)?;

    match iter_val {
        Value::List(ref list) => {
            // Fast path for lists - direct iteration
            for i in 0..list.len() {
                if let Some(val) = list.get(i as isize) {
                    executor.get_scope_mut().variables.insert(variable.to_string(), val.clone());

                    for stmt in body {
                        executor.execute_statement_complete(stmt)?;

                        if executor.check_break() {
                            executor.set_break(false);
                            return Ok(());
                        }
                        if executor.check_continue() {
                            executor.set_continue(false);
                            break;
                        }
                        if executor.check_return() {
                            return Ok(());
                        }
                    }
                }
            }
        }

        Value::Range { start, stop, step } => {
            // Ultra-fast path for ranges - inline arithmetic
            let mut current = start;
            if step > 0 {
                while current < stop {
                    executor.get_scope_mut().variables.insert(
                        variable.to_string(),
                        Value::Int(current),
                    );

                    for stmt in body {
                        executor.execute_statement_complete(stmt)?;

                        if executor.check_break() {
                            executor.set_break(false);
                            return Ok(());
                        }
                        if executor.check_continue() {
                            executor.set_continue(false);
                            break;
                        }
                        if executor.check_return() {
                            return Ok(());
                        }
                    }

                    current += step;
                }
            } else if step < 0 {
                while current > stop {
                    executor.get_scope_mut().variables.insert(
                        variable.to_string(),
                        Value::Int(current),
                    );

                    for stmt in body {
                        executor.execute_statement_complete(stmt)?;

                        if executor.check_break() {
                            executor.set_break(false);
                            return Ok(());
                        }
                        if executor.check_continue() {
                            executor.set_continue(false);
                            break;
                        }
                        if executor.check_return() {
                            return Ok(());
                        }
                    }

                    current += step;
                }
            }
        }

        Value::Tuple(ref items) => {
            for item in items {
                executor.get_scope_mut().variables.insert(variable.to_string(), item.clone());

                for stmt in body {
                    executor.execute_statement_complete(stmt)?;

                    if executor.check_break() {
                        executor.set_break(false);
                        return Ok(());
                    }
                    if executor.check_continue() {
                        executor.set_continue(false);
                        break;
                    }
                    if executor.check_return() {
                        return Ok(());
                    }
                }
            }
        }

        _ => return Err(anyhow!("Cannot iterate over {}", iter_val.type_name())),
    }

    Ok(())
}

/// Execute While loop
#[inline(always)]
pub fn execute_while_loop<E: CompleteExecutor>(
    executor: &mut E,
    condition: &Expr,
    body: &[Statement],
) -> Result<()> {
    loop {
        let cond_val = executor.evaluate_expression_complete(condition)?;

        if !cond_val.is_truthy() {
            break;
        }

        for stmt in body {
            executor.execute_statement_complete(stmt)?;

            if executor.check_break() {
                executor.set_break(false);
                return Ok(());
            }
            if executor.check_continue() {
                executor.set_continue(false);
                break;
            }
            if executor.check_return() {
                return Ok(());
            }
        }
    }

    Ok(())
}

/// Execute If statement
#[inline(always)]
pub fn execute_if_statement<E: CompleteExecutor>(
    executor: &mut E,
    condition: &Expr,
    then_branch: &[Statement],
    elif_branches: &[(Expr, Vec<Statement>)],
    else_branch: &Option<Vec<Statement>>,
) -> Result<()> {
    let cond_val = executor.evaluate_expression_complete(condition)?;

    if cond_val.is_truthy() {
        for stmt in then_branch {
            executor.execute_statement_complete(stmt)?;
            if executor.check_return() || executor.check_break() || executor.check_continue() {
                return Ok(());
            }
        }
    } else {
        // Check elif branches
        let mut executed = false;
        for (elif_cond, elif_body) in elif_branches {
            let elif_val = executor.evaluate_expression_complete(elif_cond)?;
            if elif_val.is_truthy() {
                for stmt in elif_body {
                    executor.execute_statement_complete(stmt)?;
                    if executor.check_return() || executor.check_break() || executor.check_continue() {
                        return Ok(());
                    }
                }
                executed = true;
                break;
            }
        }

        // Execute else branch if no elif executed
        if !executed {
            if let Some(else_body) = else_branch {
                for stmt in else_body {
                    executor.execute_statement_complete(stmt)?;
                    if executor.check_return() || executor.check_break() || executor.check_continue() {
                        return Ok(());
                    }
                }
            }
        }
    }

    Ok(())
}

/// Fast binary operation evaluation with inline arithmetic
#[inline(always)]
pub fn evaluate_binop_fast(left: Value, op: &BinaryOp, right: Value) -> Result<Value> {
    // Ultra-fast path for integer arithmetic (most common case)
    if let (Value::Int(a), Value::Int(b)) = (&left, &right) {
        return Ok(match op {
            BinaryOp::Add => Value::Int(a + b),
            BinaryOp::Sub => Value::Int(a - b),
            BinaryOp::Mul => Value::Int(a * b),
            BinaryOp::Div => {
                if *b == 0 {
                    return Err(anyhow!("Division by zero"));
                }
                Value::Float(*a as f64 / *b as f64)
            }
            BinaryOp::FloorDiv => {
                if *b == 0 {
                    return Err(anyhow!("Division by zero"));
                }
                Value::Int(a / b)
            }
            BinaryOp::Mod => {
                if *b == 0 {
                    return Err(anyhow!("Modulo by zero"));
                }
                Value::Int(a % b)
            }
            BinaryOp::Pow => {
                if *b >= 0 {
                    Value::Int(a.pow(*b as u32))
                } else {
                    Value::Float((*a as f64).powf(*b as f64))
                }
            }
            _ => {
                // Fall through to generic handling
                return evaluate_binop_generic(left, op, right);
            }
        });
    }

    // Generic fallback
    evaluate_binop_generic(left, op, right)
}

fn evaluate_binop_generic(left: Value, op: &BinaryOp, right: Value) -> Result<Value> {
    match op {
        BinaryOp::Add => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(*a as f64 + b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + *b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Str(format!("{}{}", a, b))),
            _ => Err(anyhow!("Unsupported types for +")),
        },
        BinaryOp::Sub => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(*a as f64 - b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - *b as f64)),
            _ => Err(anyhow!("Unsupported types for -")),
        },
        BinaryOp::Mul => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(*a as f64 * b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a * *b as f64)),
            _ => Err(anyhow!("Unsupported types for *")),
        },
        BinaryOp::Div => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => {
                if *b == 0.0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if *b == 0.0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(*a as f64 / b))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if *b == 0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / *b as f64))
                }
            }
            _ => Err(anyhow!("Unsupported types for /")),
        },
        BinaryOp::FloorDiv => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => {
                if *b == 0.0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float((a / b).floor()))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if *b == 0.0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float((*a as f64 / b).floor()))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if *b == 0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float((a / *b as f64).floor()))
                }
            }
            _ => Err(anyhow!("Unsupported types for //")),
        },
        BinaryOp::Mod => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => {
                if *b == 0.0 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a % b))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if *b == 0.0 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(*a as f64 % b))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if *b == 0 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a % *b as f64))
                }
            }
            _ => Err(anyhow!("Unsupported types for %")),
        },
        BinaryOp::Pow => match (&left, &right) {
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a.powf(*b))),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float((*a as f64).powf(*b))),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a.powf(*b as f64))),
            _ => Err(anyhow!("Unsupported types for **")),
        },
        // Comparison operators
        BinaryOp::Eq => Ok(Value::Bool(left == right)),
        BinaryOp::Ne => Ok(Value::Bool(left != right)),
        BinaryOp::Lt => match (&left, &right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((*a as f64) < *b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(*a < (*b as f64))),
            _ => Err(anyhow!("Unsupported types for <")),
        },
        BinaryOp::Le => match (&left, &right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((*a as f64) <= *b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(*a <= (*b as f64))),
            _ => Err(anyhow!("Unsupported types for <=")),
        },
        BinaryOp::Gt => match (&left, &right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((*a as f64) > *b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(*a > (*b as f64))),
            _ => Err(anyhow!("Unsupported types for >")),
        },
        BinaryOp::Ge => match (&left, &right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((*a as f64) >= *b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(*a >= (*b as f64))),
            _ => Err(anyhow!("Unsupported types for >=")),
        },
        _ => Err(anyhow!("Unsupported binary operation: {:?}", op)),
    }
}
