//! Arithmetic + logical ops (ADD, SUB, MUL, DIV, AND, OR, etc.)

use crate::value::Value;
use crate::modules::hplist::HPList;
use std::rc::Rc;
use std::cell::RefCell;
use super::vm::SuperBytecodeVM;
use anyhow::{Result, anyhow};

// Arithmetic operations implementation for SuperBytecodeVM
impl SuperBytecodeVM {
    #[inline]
    pub fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 + b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + b as f64)),
            (Value::Str(a), Value::Str(b)) => {
                // Preallocate capacity for string concatenation to avoid intermediate formatting allocations
                let mut s = String::with_capacity(a.len() + b.len());
                s.push_str(&a);
                s.push_str(&b);
                Ok(Value::Str(s))
            },
            (Value::List(a), Value::List(b)) => {
                // Avoid intermediate clones; allocate exact capacity and clone elements once
                let mut c = HPList::with_capacity(a.len() + b.len());
                for item in a {
                    c.append(item.clone());
                }
                for item in b {
                    c.append(item.clone());
                }
                Ok(Value::List(c))
            },
            // Handle mixed types by converting to strings (for print statements)
            (Value::Str(s), Value::Int(n)) => {
                let mut result = String::with_capacity(s.len() + 16); // Rough estimate for int string representation
                result.push_str(&s);
                result.push_str(&n.to_string());
                Ok(Value::Str(result))
            },
            (Value::Int(n), Value::Str(s)) => {
                let mut result = String::with_capacity(s.len() + 16); // Rough estimate for int string representation
                result.push_str(&n.to_string());
                result.push_str(&s);
                Ok(Value::Str(result))
            },
            (Value::Str(s), Value::Float(f)) => {
                let mut result = String::with_capacity(s.len() + 16); // Rough estimate for float string representation
                result.push_str(&s);
                result.push_str(&format!("{:.6}", f)); // Format float similar to how it's done in value.rs
                Ok(Value::Str(result))
            },
            (Value::Float(f), Value::Str(s)) => {
                let mut result = String::with_capacity(s.len() + 16); // Rough estimate for float string representation
                result.push_str(&format!("{:.6}", f)); // Format float similar to how it's done in value.rs
                result.push_str(&s);
                Ok(Value::Str(result))
            },
            (Value::Str(s), Value::Bool(b)) => {
                let mut result = String::with_capacity(s.len() + 8); // Rough estimate for bool string representation
                result.push_str(&s);
                result.push_str(if b { "True" } else { "False" });
                Ok(Value::Str(result))
            },
            (Value::Bool(b), Value::Str(s)) => {
                let mut result = String::with_capacity(s.len() + 8); // Rough estimate for bool string representation
                result.push_str(if b { "True" } else { "False" });
                result.push_str(&s);
                Ok(Value::Str(result))
            },
            _ => Err(anyhow!("Unsupported types for addition")),
        }
    }

    #[inline]
    pub fn sub_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 - b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - b as f64)),
            _ => Err(anyhow!("Unsupported types for subtraction")),
        }
    }

    #[inline]
    pub fn mul_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 * b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a * b as f64)),
            // Boolean multiplication: treat as integers
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Int((a as i64) * (b as i64))),
            (Value::Bool(a), Value::Int(b)) => Ok(Value::Int((a as i64) * b)),
            (Value::Int(a), Value::Bool(b)) => Ok(Value::Int(a * (b as i64))),
            (Value::Bool(a), Value::Float(b)) => Ok(Value::Float((a as i64) as f64 * b)),
            (Value::Float(a), Value::Bool(b)) => Ok(Value::Float(a * (b as i64) as f64)),
            // String repetition: "abc" * 3 or 3 * "abc"
            (Value::Str(s), Value::Int(n)) => {
                if n < 0 {
                    Ok(Value::Str(String::new()))
                } else {
                    Ok(Value::Str(s.repeat(n as usize)))
                }
            },
            (Value::Int(n), Value::Str(s)) => {
                if n < 0 {
                    Ok(Value::Str(String::new()))
                } else {
                    Ok(Value::Str(s.repeat(n as usize)))
                }
            },
            // List repetition: [1, 2] * 3 or 3 * [1, 2]
            (Value::List(list), Value::Int(n)) => {
                if n < 0 {
                    Ok(Value::List(HPList::new()))
                } else {
                    let mut result = HPList::new();
                    for _ in 0..n {
                        for item in list.iter() {
                            result.append(item.clone());
                        }
                    }
                    Ok(Value::List(result))
                }
            },
            (Value::Int(n), Value::List(list)) => {
                if n < 0 {
                    Ok(Value::List(HPList::new()))
                } else {
                    let mut result = HPList::new();
                    for _ in 0..n {
                        for item in list.iter() {
                            result.append(item.clone());
                        }
                    }
                    Ok(Value::List(result))
                }
            },
            (Value::Dict(a), Value::Dict(b)) => {
                let mut result = a.borrow().clone();
                for (k, v) in b.borrow().iter() {
                    result.insert(k.clone(), v.clone());
                }
                Ok(Value::Dict(Rc::new(RefCell::new(result))))
            },
            // Handle None values - multiplying None should raise a clear error
            (ref l, ref r) => {
                // Provide more detailed error message for debugging - only compute type names in error path
                Err(anyhow!("unsupported operand type(s) for *: '{}' and '{}'", l.type_name(), r.type_name()))
            }
        }
    }

    #[inline]
    pub fn div_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Int(a / b))
                }
            },
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            },
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b))
                }
            },
            (Value::Float(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b as f64))
                }
            },
            _ => Err(anyhow!("Unsupported types for division")),
        }
    }

    #[inline]
    pub fn mod_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Int(a % b))
                }
            },
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a % b))
                }
            },
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a as f64 % b))
                }
            },
            (Value::Float(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a % b as f64))
                }
            },
            _ => Err(anyhow!("Unsupported types for modulo")),
        }
    }

    #[inline]
    pub fn pow_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b >= 0 {
                    Ok(Value::Int(a.pow(b as u32)))
                } else {
                    Ok(Value::Float((a as f64).powf(b as f64)))
                }
            },
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a.powf(b))),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float((a as f64).powf(b))),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a.powf(b as f64))),
            _ => Err(anyhow!("Unsupported types for power")),
        }
    }

    #[inline]
    pub fn lt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) < b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a < (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a < b)),
            _ => Err(anyhow!("Unsupported types for less than comparison")),
        }
    }

    #[inline]
    pub fn gt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) > b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a > (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a > b)),
            _ => Err(anyhow!("Unsupported types for greater than comparison")),
        }
    }

    #[inline]
    pub fn le_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) <= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a <= (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a <= b)),
            _ => Err(anyhow!("Unsupported types for less than or equal comparison")),
        }
    }

    #[inline]
    pub fn ge_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) >= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a >= (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a >= b)),
            _ => Err(anyhow!("Unsupported types for greater than or equal comparison")),
        }
    }

    #[inline]
    pub fn eq_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a == b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a == b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) == b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a == (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a == b)),
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Bool(a == b)),
            _ => Err(anyhow!("Unsupported types for equality comparison")),
        }
    }

    #[inline]
    pub fn ne_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a != b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a != b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) != b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a != (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a != b)),
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Bool(a != b)),
            _ => Err(anyhow!("Unsupported types for not equal comparison")),
        }
    }

    /// Bitwise AND operation
    #[inline]
    pub fn bitand_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a & b)),
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Bool(a & b)),
            (Value::Int(a), Value::Bool(b)) => Ok(Value::Int(a & (b as i64))),
            (Value::Bool(a), Value::Int(b)) => Ok(Value::Int((a as i64) & b)),
            _ => Err(anyhow!("Unsupported types for bitwise AND operation")),
        }
    }

    /// Bitwise OR operation
    #[inline]
    pub fn bitor_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a | b)),
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Bool(a | b)),
            (Value::Int(a), Value::Bool(b)) => Ok(Value::Int(a | (b as i64))),
            (Value::Bool(a), Value::Int(b)) => Ok(Value::Int((a as i64) | b)),
            _ => Err(anyhow!("Unsupported types for bitwise OR operation")),
        }
    }
}







