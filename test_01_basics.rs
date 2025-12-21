use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use std::fmt;

// Type definitions

type TauInteger = i64;
type TauFloat = f64;
type TauBool = bool;
type TauString = String;

// Object type for dynamic values
#[derive(Clone, Debug)]
pub enum TauObject {
    None,
    Bool(bool),
    Int(i64),
    Float(f64),
    String(String),
    List(Vec<TauObject>),
    Dict(HashMap<String, TauObject>),
    Custom(String, Arc<Mutex<HashMap<String, TauObject>>>),
}

impl TauObject {
    pub fn to_string(&self) -> String {
        match self {
            TauObject::None => "None".to_string(),
            TauObject::Bool(b) => b.to_string(),
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
            TauObject::Dict(map) => {
                let mut items = Vec::new();
                for (k, v) in map.iter() {
                    items.push(format!("'{}': {}", k, v.to_string()));
                }
                format!("{{{}}}", items.join(", "))
            }
            TauObject::Custom(name, _) => format!("<{} object>", name),
        }
    }

    pub fn is_truthy(&self) -> bool {
        match self {
            TauObject::None => false,
            TauObject::Bool(b) => *b,
            TauObject::Int(i) => *i != 0,
            TauObject::Float(f) => *f != 0.0,
            TauObject::String(s) => !s.is_empty(),
            TauObject::List(items) => !items.is_empty(),
            TauObject::Dict(map) => !map.is_empty(),
            TauObject::Custom(_, _) => true,
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
            (TauObject::String(a), TauObject::String(b)) => a == b,
            (TauObject::List(a), TauObject::List(b)) => a == b,
            (TauObject::Dict(a), TauObject::Dict(b)) => a == b,
            _ => false,
        }
    }
}

impl fmt::Display for TauObject {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.to_string())
    }
}

// Function implementations

fn test_strings() -> i64 {
    let temp_result = "Hello";
    let s = temp_result;
    let temp_result = "World";
    let s2 = temp_result;
    let binop_left = s;
    let binop_right = " ";
    let temp_result = format!("{}{}", binop_left, binop_right);
    let arg_0_left = temp_result;
    let temp_result = s2;
    let arg_0_right = temp_result;
    let arg_0 = format!("{}{}", arg_0_left, arg_0_right);
    println!("{}", arg_0);
    let arg_0 = S__upper(s);
    println!("{}", arg_0);
    let arg_0 = S__lower(s);
    println!("{}", arg_0);
    let arg_0 = s;
    let temp_result = arg_0.len() as i64;
    let arg_0 = temp_result;
    println!("{}", arg_0);
}

fn test_lists() -> i64 {
    let temp_elem_0 = 1;
    let temp_elem_1 = 2;
    let temp_elem_2 = 3;
    let temp_elem_3 = 4;
    let temp_elem_4 = 5;
    let temp_result = vec![temp_elem_0, temp_elem_1, temp_elem_2, temp_elem_3, temp_elem_4];
    let lst = temp_result;
    let arg_0 = lst;
    println!("{}", arg_0);
    let arg_0 = lst;
    let temp_result = arg_0.len() as i64;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_result = lst;
    let temp_object = temp_result;
    let temp_result = 6;
    let method_arg_0 = temp_result;
    let temp_result = lst__append(temp_object, method_arg_0);
    let arg_0 = lst;
    println!("{}", arg_0);
}

fn test_dict() -> i64 {
    let temp_key_0 = "name";
    let temp_value_0 = "Tauraro";
    let temp_key_1 = "version";
    let temp_value_1 = "1.0";
    let mut temp_result = HashMap::new();
    temp_result.insert(temp_key_0, temp_value_0);
    temp_result.insert(temp_key_1, temp_value_1);
    let d = temp_result;
    let temp_result = 0;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let _ = 0;
    let arg_0 = d;
    println!("{}", arg_0);
}

fn test_arithmetic() -> i64 {
    let temp_result = 10;
    let a = temp_result;
    let temp_result = 20;
    let b = temp_result;
    let temp_result = a;
    let arg_0_left = temp_result;
    let temp_result = b;
    let arg_0_right = temp_result;
    let arg_0 = format!("{}{}", arg_0_left, arg_0_right);
    println!("{}", arg_0);
    let temp_result = a;
    let arg_0_left = temp_result;
    let temp_result = b;
    let arg_0_right = temp_result;
    let arg_0 = arg_0_left - arg_0_right;
    println!("{}", arg_0);
    let temp_result = a;
    let arg_0_left = temp_result;
    let temp_result = b;
    let arg_0_right = temp_result;
    let arg_0 = arg_0_left - arg_0_right;
    println!("{}", arg_0);
    let temp_result = a;
    let arg_0_left = temp_result;
    let temp_result = b;
    let arg_0_right = temp_result;
    let arg_0 = arg_0_left - arg_0_right;
    println!("{}", arg_0);
    let temp_result = a;
    let arg_0_left = temp_result;
    let temp_result = b;
    let arg_0_right = temp_result;
    let arg_0 = arg_0_left - arg_0_right;
    println!("{}", arg_0);
    let temp_result = a;
    let arg_0_left = temp_result;
    let temp_result = b;
    let arg_0_right = temp_result;
    let arg_0 = arg_0_left - arg_0_right;
    println!("{}", arg_0);
}

fn main() {
    let temp_result = test_arithmetic();
    let temp_result = test_strings();
    let temp_result = test_lists();
    let temp_result = test_dict();
}

