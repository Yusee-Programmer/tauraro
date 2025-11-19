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
            // Set difference: s1 - s2
            (Value::Set(a), Value::Set(b)) => {
                let result: Vec<Value> = a.iter()
                    .filter(|item| !b.contains(item))
                    .cloned()
                    .collect();
                Ok(Value::Set(result))
            },
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
            // String formatting: "format %s %d" % (value1, value2)
            (Value::Str(format_str), Value::Tuple(values)) => {
                self.format_string_old_style(&format_str, values.clone())
            },
            // String formatting with single value: "format %s" % value
            (Value::Str(format_str), right_val) => {
                self.format_string_old_style(&format_str, vec![right_val])
            },
            _ => Err(anyhow!("Unsupported types for modulo")),
        }
    }

    /// Old-style string formatting with % operator
    /// Supports: %s (string), %d (integer), %f (float), %r (repr)
    fn format_string_old_style(&self, format_str: &str, values: Vec<Value>) -> Result<Value> {
        let mut result = format_str.to_string();
        let mut value_idx = 0;
        let mut chars = format_str.chars().peekable();
        let mut pos = 0;
        let mut replacements = Vec::new();

        while let Some(ch) = chars.next() {
            if ch == '%' {
                if let Some(&next_ch) = chars.peek() {
                    match next_ch {
                        '%' => {
                            // %% -> %
                            chars.next();
                            pos += 2;
                        },
                        's' | 'd' | 'f' | 'r' | 'x' | 'o' => {
                            // Found a format specifier
                            if value_idx < values.len() {
                                let formatted = match next_ch {
                                    's' => values[value_idx].to_string(),
                                    'd' => {
                                        match &values[value_idx] {
                                            Value::Int(i) => i.to_string(),
                                            Value::Float(f) => (*f as i64).to_string(),
                                            _ => return Err(anyhow!("%d format: an integer is required")),
                                        }
                                    },
                                    'f' => {
                                        match &values[value_idx] {
                                            Value::Float(f) => f.to_string(),
                                            Value::Int(i) => (*i as f64).to_string(),
                                            _ => return Err(anyhow!("%f format: a number is required")),
                                        }
                                    },
                                    'r' => format!("{:?}", values[value_idx]),
                                    'x' => {
                                        match &values[value_idx] {
                                            Value::Int(i) => format!("{:x}", i),
                                            _ => return Err(anyhow!("%x format: an integer is required")),
                                        }
                                    },
                                    'o' => {
                                        match &values[value_idx] {
                                            Value::Int(i) => format!("{:o}", i),
                                            _ => return Err(anyhow!("%o format: an integer is required")),
                                        }
                                    },
                                    _ => unreachable!(),
                                };
                                replacements.push((format!("%{}", next_ch), formatted));
                                value_idx += 1;
                            } else {
                                return Err(anyhow!("not enough arguments for format string"));
                            }
                            chars.next();
                            pos += 2;
                        },
                        _ => {
                            pos += 1;
                        }
                    }
                } else {
                    pos += 1;
                }
            } else {
                pos += 1;
            }
        }

        // Apply replacements in order
        for (pattern, replacement) in replacements {
            if let Some(idx) = result.find(&pattern) {
                result = result[..idx].to_string() + &replacement + &result[idx + pattern.len()..];
            }
        }

        Ok(Value::Str(result))
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
            // Set intersection: s1 & s2
            (Value::Set(a), Value::Set(b)) => {
                let result: Vec<Value> = a.iter()
                    .filter(|item| b.contains(item))
                    .cloned()
                    .collect();
                Ok(Value::Set(result))
            },
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
            // Set union: s1 | s2
            (Value::Set(a), Value::Set(b)) => {
                let mut result = a.clone();
                for item in b.iter() {
                    if !result.contains(item) {
                        result.push(item.clone());
                    }
                }
                Ok(Value::Set(result))
            },
            _ => Err(anyhow!("Unsupported types for bitwise OR operation")),
        }
    }

    /// Helper to call dunder method on object
    pub fn call_dunder_method(&self, obj: &Value, dunder_name: &str, args: Vec<Value>) -> Option<Result<Value>> {
        // Check if this is a custom object with a dunder method
        if let Value::Object { class_methods, .. } = obj {
            if let Some(method) = class_methods.get(dunder_name) {
                // We found a dunder method, but we can't call it from here since we're not in a mutable context
                // Return the method for the caller to invoke
                return Some(Ok(method.clone()));
            }
        }
        None
    }


}







