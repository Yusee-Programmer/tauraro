//! Tauraro Runtime Library for Rust
//! Provides type-safe operations and builtin functions for generated Rust code

use std::collections::{HashMap, HashSet};
use std::sync::{Arc, Mutex};
use std::fmt;
use std::cmp::Ordering;

/// Unified type for all Tauraro values
#[derive(Clone, Debug)]
pub enum TauObject {
    None,
    Bool(bool),
    Int(i64),
    Float(f64),
    String(String),
    List(Vec<TauObject>),
    Dict(HashMap<String, TauObject>),
    Set(HashSet<String>), // Sets store string representations for hashing
    Tuple(Vec<TauObject>),
    Callable(String), // Function name reference
    Custom(String, Arc<Mutex<HashMap<String, TauObject>>>),
}

impl TauObject {
    /// Convert to string representation (Python-like)
    pub fn to_string(&self) -> String {
        match self {
            TauObject::None => "None".to_string(),
            TauObject::Bool(b) => if *b { "True" } else { "False" }.to_string(),
            TauObject::Int(i) => i.to_string(),
            TauObject::Float(f) => {
                let s = f.to_string();
                if s.contains('.') { s } else { format!("{}.0", s) }
            }
            TauObject::String(s) => s.clone(),
            TauObject::List(items) => {
                let strs: Vec<_> = items.iter().map(|i| i.to_string()).collect();
                format!("[{}]", strs.join(", "))
            }
            TauObject::Tuple(items) => {
                let strs: Vec<_> = items.iter().map(|i| i.to_string()).collect();
                if strs.len() == 1 {
                    format!("({},)", strs[0])
                } else {
                    format!("({})", strs.join(", "))
                }
            }
            TauObject::Dict(map) => {
                let mut items: Vec<_> = map.iter()
                    .map(|(k, v)| format!("'{}': {}", k, v.to_string()))
                    .collect();
                items.sort();
                format!("{{{}}}", items.join(", "))
            }
            TauObject::Set(items) => {
                let mut items: Vec<_> = items.iter().cloned().collect();
                items.sort();
                format!("{{{}}}", items.join(", "))
            }
            TauObject::Callable(name) => format!("<function {}>", name),
            TauObject::Custom(name, _) => format!("<{} object>", name),
        }
    }

    /// Check if value is truthy (Python semantics)
    pub fn is_truthy(&self) -> bool {
        match self {
            TauObject::None => false,
            TauObject::Bool(b) => *b,
            TauObject::Int(i) => *i != 0,
            TauObject::Float(f) => *f != 0.0 && !f.is_nan(),
            TauObject::String(s) => !s.is_empty(),
            TauObject::List(items) => !items.is_empty(),
            TauObject::Tuple(items) => !items.is_empty(),
            TauObject::Dict(map) => !map.is_empty(),
            TauObject::Set(items) => !items.is_empty(),
            TauObject::Callable(_) => true,
            TauObject::Custom(_, _) => true,
        }
    }

    /// Convert to integer (Python-like)
    pub fn to_int(&self) -> Result<i64, String> {
        match self {
            TauObject::Int(i) => Ok(*i),
            TauObject::Float(f) => Ok(*f as i64),
            TauObject::Bool(b) => Ok(if *b { 1 } else { 0 }),
            TauObject::String(s) => s.parse::<i64>()
                .map_err(|_| format!("invalid literal for int() with base 10: '{}'", s)),
            _ => Err(format!("cannot convert {} to int", self.type_name())),
        }
    }

    /// Convert to float
    pub fn to_float(&self) -> Result<f64, String> {
        match self {
            TauObject::Float(f) => Ok(*f),
            TauObject::Int(i) => Ok(*i as f64),
            TauObject::Bool(b) => Ok(if *b { 1.0 } else { 0.0 }),
            TauObject::String(s) => s.parse::<f64>()
                .map_err(|_| format!("could not convert string to float: '{}'", s)),
            _ => Err(format!("cannot convert {} to float", self.type_name())),
        }
    }

    /// Convert to bool
    pub fn to_bool(&self) -> bool {
        self.is_truthy()
    }

    /// Convert to Vec<TauObject> for iteration
    pub fn to_vec(&self) -> Result<Vec<TauObject>, String> {
        match self {
            TauObject::List(items) => Ok(items.clone()),
            TauObject::Tuple(items) => Ok(items.clone()),
            TauObject::String(s) => Ok(s.chars().map(|c| TauObject::String(c.to_string())).collect()),
            _ => Err(format!("'{}' object is not iterable", self.type_name())),
        }
    }

    /// Get the length of a sequence
    pub fn len(&self) -> Result<usize, String> {
        match self {
            TauObject::String(s) => Ok(s.len()),
            TauObject::List(items) => Ok(items.len()),
            TauObject::Tuple(items) => Ok(items.len()),
            TauObject::Dict(map) => Ok(map.len()),
            TauObject::Set(items) => Ok(items.len()),
            _ => Err(format!("object of type '{}' has no len()", self.type_name())),
        }
    }

    /// Check if value is in container
    pub fn contains(&self, item: &TauObject) -> Result<bool, String> {
        match self {
            TauObject::List(items) => {
                Ok(items.iter().any(|x| x == item))
            }
            TauObject::Tuple(items) => {
                Ok(items.iter().any(|x| x == item))
            }
            TauObject::Set(items) => {
                Ok(items.contains(&item.to_string()))
            }
            TauObject::Dict(map) => {
                if let TauObject::String(key) = item {
                    Ok(map.contains_key(key))
                } else {
                    Ok(false)
                }
            }
            TauObject::String(s) => {
                if let TauObject::String(needle) = item {
                    Ok(s.contains(needle))
                } else {
                    Err("'in' requires string as both operands for string search".to_string())
                }
            }
            _ => Err(format!("argument of type '{}' is not iterable", self.type_name())),
        }
    }

    /// Get element at index
    pub fn get_index(&self, index: &TauObject) -> Result<TauObject, String> {
        let idx = index.to_int()? as usize;

        match self {
            TauObject::List(items) => {
                items.get(idx)
                    .cloned()
                    .ok_or_else(|| "list index out of range".to_string())
            }
            TauObject::Tuple(items) => {
                items.get(idx)
                    .cloned()
                    .ok_or_else(|| "tuple index out of range".to_string())
            }
            TauObject::String(s) => {
                s.chars()
                    .nth(idx)
                    .map(|c| TauObject::String(c.to_string()))
                    .ok_or_else(|| "string index out of range".to_string())
            }
            TauObject::Dict(map) => {
                if let TauObject::String(key) = index {
                    map.get(key)
                        .cloned()
                        .ok_or_else(|| format!("KeyError: '{}'", key))
                } else {
                    Err("dict keys must be strings".to_string())
                }
            }
            _ => Err(format!("'{}' object is not subscriptable", self.type_name())),
        }
    }

    /// Set element at index
    pub fn set_index(&mut self, index: &TauObject, value: TauObject) -> Result<(), String> {
        let idx = index.to_int()? as usize;

        match self {
            TauObject::List(items) => {
                if idx >= items.len() {
                    return Err("list assignment index out of range".to_string());
                }
                items[idx] = value;
                Ok(())
            }
            TauObject::Dict(map) => {
                if let TauObject::String(key) = index {
                    map.insert(key.clone(), value);
                    Ok(())
                } else {
                    Err("dict keys must be strings".to_string())
                }
            }
            _ => Err(format!("'{}' object does not support item assignment", self.type_name())),
        }
    }

    /// Get type name as string
    pub fn type_name(&self) -> String {
        match self {
            TauObject::None => "NoneType".to_string(),
            TauObject::Bool(_) => "bool".to_string(),
            TauObject::Int(_) => "int".to_string(),
            TauObject::Float(_) => "float".to_string(),
            TauObject::String(_) => "str".to_string(),
            TauObject::List(_) => "list".to_string(),
            TauObject::Tuple(_) => "tuple".to_string(),
            TauObject::Dict(_) => "dict".to_string(),
            TauObject::Set(_) => "set".to_string(),
            TauObject::Callable(_) => "function".to_string(),
            TauObject::Custom(name, _) => name.clone(),
        }
    }

    /// Compare two values
    pub fn compare(&self, other: &TauObject) -> Result<Ordering, String> {
        match (self, other) {
            (TauObject::Int(a), TauObject::Int(b)) => Ok(a.cmp(b)),
            (TauObject::Float(a), TauObject::Float(b)) => {
                Ok(if a < b { Ordering::Less } else if a > b { Ordering::Greater } else { Ordering::Equal })
            }
            (TauObject::Int(a), TauObject::Float(b)) => {
                let a_f = *a as f64;
                Ok(if a_f < *b { Ordering::Less } else if a_f > *b { Ordering::Greater } else { Ordering::Equal })
            }
            (TauObject::Float(a), TauObject::Int(b)) => {
                let b_f = *b as f64;
                Ok(if a < &b_f { Ordering::Less } else if a > &b_f { Ordering::Greater } else { Ordering::Equal })
            }
            (TauObject::String(a), TauObject::String(b)) => Ok(a.cmp(b)),
            (TauObject::Bool(a), TauObject::Bool(b)) => Ok(a.cmp(b)),
            _ => Err(format!("'<' not supported between instances of '{}' and '{}'", 
                self.type_name(), other.type_name())),
        }
    }
}

impl PartialEq for TauObject {
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (TauObject::None, TauObject::None) => true,
            (TauObject::Bool(a), TauObject::Bool(b)) => a == b,
            (TauObject::Int(a), TauObject::Int(b)) => a == b,
            (TauObject::Float(a), TauObject::Float(b)) => a == b,
            (TauObject::Int(a), TauObject::Float(b)) => (*a as f64) == *b,
            (TauObject::Float(a), TauObject::Int(b)) => *a == (*b as f64),
            (TauObject::String(a), TauObject::String(b)) => a == b,
            (TauObject::List(a), TauObject::List(b)) => a == b,
            (TauObject::Tuple(a), TauObject::Tuple(b)) => a == b,
            (TauObject::Dict(a), TauObject::Dict(b)) => a == b,
            (TauObject::Set(a), TauObject::Set(b)) => a == b,
            _ => false,
        }
    }
}

impl Eq for TauObject {}

impl fmt::Display for TauObject {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.to_string())
    }
}

// Helper functions for common operations

/// Add two values (Python semantics)
pub fn tau_add(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x + y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x + y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 + y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x + *y as f64)),
        (TauObject::String(x), TauObject::String(y)) => Ok(TauObject::String(format!("{}{}", x, y))),
        (TauObject::List(x), TauObject::List(y)) => {
            let mut result = x.clone();
            result.extend(y.clone());
            Ok(TauObject::List(result))
        }
        _ => Err(format!("unsupported operand type(s) for +: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

/// Subtract two values
pub fn tau_sub(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x - y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x - y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 - y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x - *y as f64)),
        _ => Err(format!("unsupported operand type(s) for -: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

/// Multiply two values
pub fn tau_mul(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x * y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x * y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 * y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x * *y as f64)),
        (TauObject::String(x), TauObject::Int(y)) => {
            if *y < 0 { return Err("can't multiply sequence by non-int of type 'NoneType'".to_string()); }
            Ok(TauObject::String(x.repeat(*y as usize)))
        }
        (TauObject::Int(x), TauObject::String(y)) => {
            if *x < 0 { return Err("can't multiply sequence by non-int of type 'NoneType'".to_string()); }
            Ok(TauObject::String(y.repeat(*x as usize)))
        }
        (TauObject::List(x), TauObject::Int(y)) => {
            if *y < 0 { return Err("can't multiply sequence by non-int of type 'NoneType'".to_string()); }
            let mut result = Vec::new();
            for _ in 0..*y {
                result.extend(x.clone());
            }
            Ok(TauObject::List(result))
        }
        (TauObject::Int(x), TauObject::List(y)) => {
            if *x < 0 { return Err("can't multiply sequence by non-int of type 'NoneType'".to_string()); }
            let mut result = Vec::new();
            for _ in 0..*x {
                result.extend(y.clone());
            }
            Ok(TauObject::List(result))
        }
        _ => Err(format!("unsupported operand type(s) for *: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

/// Divide two values
pub fn tau_div(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("division by zero".to_string()); }
            Ok(TauObject::Float(*x as f64 / *y as f64))
        }
        (TauObject::Float(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float division by zero".to_string()); }
            Ok(TauObject::Float(x / y))
        }
        (TauObject::Int(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float division by zero".to_string()); }
            Ok(TauObject::Float(*x as f64 / y))
        }
        (TauObject::Float(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("float division by zero".to_string()); }
            Ok(TauObject::Float(x / *y as f64))
        }
        _ => Err(format!("unsupported operand type(s) for /: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

/// Floor divide
pub fn tau_floordiv(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("integer division or modulo by zero".to_string()); }
            Ok(TauObject::Int(x / y))
        }
        (TauObject::Float(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float floor division by zero".to_string()); }
            Ok(TauObject::Float((x / y).floor()))
        }
        (TauObject::Int(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float floor division by zero".to_string()); }
            Ok(TauObject::Float((*x as f64 / y).floor()))
        }
        (TauObject::Float(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("float floor division by zero".to_string()); }
            Ok(TauObject::Float((x / *y as f64).floor()))
        }
        _ => Err(format!("unsupported operand type(s) for //: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

/// Modulo
pub fn tau_mod(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("integer division or modulo by zero".to_string()); }
            Ok(TauObject::Int(x % y))
        }
        (TauObject::Float(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float modulo".to_string()); }
            Ok(TauObject::Float(x % y))
        }
        (TauObject::Int(x), TauObject::Float(y)) => {
            if *y == 0.0 { return Err("float modulo".to_string()); }
            Ok(TauObject::Float(*x as f64 % y))
        }
        (TauObject::Float(x), TauObject::Int(y)) => {
            if *y == 0 { return Err("float modulo".to_string()); }
            Ok(TauObject::Float(x % *y as f64))
        }
        _ => Err(format!("unsupported operand type(s) for %: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

/// Power
pub fn tau_pow(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => {
            if *y < 0 {
                Ok(TauObject::Float((*x as f64).powf(*y as f64)))
            } else {
                Ok(TauObject::Int(x.pow(*y as u32)))
            }
        }
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x.powf(*y))),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float((*x as f64).powf(*y))),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x.powf(*y as f64))),
        _ => Err(format!("unsupported operand type(s) for **: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}
