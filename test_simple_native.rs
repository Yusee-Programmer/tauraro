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

fn main() {
    let arg_0 = "Hello from native Rust!";
    println!("{}", arg_0);
    let temp_result = 42;
    let x = temp_result;
    let binop_left = x;
    let binop_right = 8;
    let temp_result = format!("{}{}", binop_left, binop_right);
    let y = temp_result;
    let temp_result = "Result: ";
    let fstring_left_1 = temp_result;
    let temp_result = y;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = format!("{}{}", fstring_left_1, fstring_right_1);
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let arg_0 = 5;
    let temp_result = (0..arg_0).collect::<Vec<_>>();
    for i in &temp_result {
        let temp_result = "Count: ";
        let fstring_left_1 = temp_result;
        let temp_result = i;
        let fstring_right_1 = temp_result;
        let fstring_result_1 = format!("{}{}", fstring_left_1, fstring_right_1);
        let temp_result = fstring_result_1;
        let arg_0 = temp_result;
        println!("{}", arg_0);
    }
    let temp_result = 0;
    // return temp_result;
}

