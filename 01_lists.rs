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

// ===== Helper Functions for Methods and Builtins =====

// Wrapper for Vec to implement Display
#[derive(Clone, Debug)]
struct VecDisplay<T: std::fmt::Display>(Vec<T>);

impl<T: std::fmt::Display> std::fmt::Display for VecDisplay<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let items: Vec<String> = self.0.iter().map(|x| format!("{}", x)).collect();
        write!(f, "[{}]", items.join(", "))
    }
}

// Simple Display impl for i64 Vec
fn format_list(list: &[i64]) -> String {
    let items: Vec<String> = list.iter().map(|x| format!("{}", x)).collect();
    format!("[{}]", items.join(", "))
}

// List method implementations
fn lst__append(mut list: Vec<i64>, item: i64) -> Vec<i64> {
    list.push(item);
    list
}

fn lst__pop(mut list: Vec<i64>) -> i64 {
    list.pop().unwrap_or(0)
}

fn lst__reverse(mut list: Vec<i64>) -> Vec<i64> {
    list.reverse();
    list
}

fn lst__index(list: &[i64], item: i64) -> i64 {
    for (i, elem) in list.iter().enumerate() {
        if *elem == item {
            return i as i64;
        }
    }
    -1
}

fn lst__count(list: &[i64], item: i64) -> i64 {
    list.iter().filter(|x| **x == item).count() as i64
}

fn lst__extend(mut list: Vec<i64>, other: Vec<i64>) -> Vec<i64> {
    list.extend(other);
    list
}

fn lst__insert(mut list: Vec<i64>, index: usize, item: i64) -> Vec<i64> {
    if index <= list.len() {
        list.insert(index, item);
    }
    list
}

fn lst__remove(mut list: Vec<i64>, item: i64) -> Vec<i64> {
    list.retain(|x| *x != item);
    list
}

fn lst__clear(mut list: Vec<i64>) -> Vec<i64> {
    list.clear();
    list
}

// String method implementations
fn text__upper(s: &str) -> String {
    s.to_uppercase()
}

fn text__lower(s: &str) -> String {
    s.to_lowercase()
}

fn text__strip(s: &str) -> String {
    s.trim().to_string()
}

fn text__replace(s: &str, old: &str, new: &str) -> String {
    s.replace(old, new)
}

fn text__split(s: &str, sep: &str) -> Vec<String> {
    s.split(sep).map(|x| x.to_string()).collect()
}

fn text__join(sep: &str, items: &[String]) -> String {
    items.join(sep)
}

fn text__startswith(s: &str, prefix: &str) -> bool {
    s.starts_with(prefix)
}

fn text__endswith(s: &str, suffix: &str) -> bool {
    s.ends_with(suffix)
}

fn text__find(s: &str, sub: &str) -> i64 {
    s.find(sub).map(|i| i as i64).unwrap_or(-1)
}

fn text__index(s: &str, sub: &str) -> i64 {
    s.find(sub).map(|i| i as i64).unwrap_or(-1)
}

fn text__count(s: &str, sub: &str) -> i64 {
    s.matches(sub).count() as i64
}

fn text__capitalize(s: &str) -> String {
    let mut chars = s.chars();
    match chars.next() {
        None => String::new(),
        Some(first) => first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase(),
    }
}

fn text__title(s: &str) -> String {
    s.split_whitespace()
        .map(|word| text__capitalize(word))
        .collect::<Vec<_>>()
        .join(" ")
}

fn text__isdigit(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_ascii_digit())
}

fn text__isalpha(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_alphabetic())
}

// Dict methods - generic implementations
fn dict__get(map: &std::collections::HashMap<String, String>, key: &str, default: &str) -> String {
    map.get(key).cloned().unwrap_or_else(|| default.to_string())
}

fn dict__keys(map: &std::collections::HashMap<String, String>) -> Vec<String> {
    map.keys().cloned().collect()
}

fn dict__values(map: &std::collections::HashMap<String, String>) -> Vec<String> {
    map.values().cloned().collect()
}

fn dict__update(mut map: std::collections::HashMap<String, String>, other: std::collections::HashMap<String, String>) -> std::collections::HashMap<String, String> {
    for (k, v) in other {
        map.insert(k, v);
    }
    map
}

fn dict__clear(mut map: std::collections::HashMap<String, String>) -> std::collections::HashMap<String, String> {
    map.clear();
    map
}

// Builtin functions
fn tau_abs(n: i64) -> i64 {
    n.abs()
}

fn tau_min(numbers: &[i64]) -> i64 {
    *numbers.iter().min().unwrap_or(&0)
}

fn tau_max(numbers: &[i64]) -> i64 {
    *numbers.iter().max().unwrap_or(&0)
}

fn tau_sum(numbers: &[i64]) -> i64 {
    numbers.iter().sum()
}

fn tau_pow(base: i64, exp: i64) -> i64 {
    base.pow(exp as u32)
}

fn tau_round(f: f64) -> i64 {
    f.round() as i64
}


// Function implementations

fn test_lists() -> () {
    let arg_0 = "=== List Operations ===";
    println!("{}", arg_0);
    let temp_elem_0 = 1;
    let temp_elem_1 = 2;
    let temp_elem_2 = 3;
    let temp_elem_3 = 4;
    let temp_elem_4 = 5;
    let temp_result = vec![temp_elem_0, temp_elem_1, temp_elem_2, temp_elem_3, temp_elem_4];
    let lst = temp_result;
    let temp_result = "Original list: ";
    let fstring_left_1 = temp_result;
    let temp_result = lst;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_result = "First element: ";
    let fstring_left_1 = temp_result;
    let temp_result = 0;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_result = "Last element: ";
    let fstring_left_1 = temp_result;
    let temp_result = 0;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let arg_0 = lst;
    let temp_result = arg_0.len() as i64;
    let length = temp_result;
    let temp_result = "Length: ";
    let fstring_left_1 = temp_result;
    let temp_result = length;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let arg_0 = "Iterating:";
    println!("{}", arg_0);
    let temp_result = lst;
    for item in &temp_result {
        let temp_result = "  Item: ";
        let fstring_left_1 = temp_result;
        let temp_result = item;
        let fstring_right_1 = temp_result;
        let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
        let temp_result = fstring_result_1;
        let arg_0 = temp_result;
        println!("{}", arg_0);
    }
    // Slicing
    let temp_result = 0;
    let slice_result = temp_result;
    let temp_result = "Slice [1:4]: ";
    let fstring_left_1 = temp_result;
    let temp_result = slice_result;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_elem_0 = 10;
    let temp_elem_1 = 20;
    let temp_result = vec![temp_elem_0, temp_elem_1];
    let lst2 = temp_result;
    let temp_result = lst2;
    let temp_object = temp_result;
    let temp_result = 30;
    let method_arg_0 = temp_result;
    let temp_result = lst__append(temp_object, method_arg_0);
    let temp_result = "After append: ";
    let fstring_left_1 = temp_result;
    let temp_result = lst2;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_result = lst2;
    let temp_object = temp_result;
    let temp_result = lst__pop(temp_object);
    let popped = temp_result;
    let temp_result = "Popped value: ";
    let fstring_left_1 = temp_result;
    let temp_result = popped;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_result = "After pop: ";
    let fstring_left_1 = temp_result;
    let temp_result = lst2;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_result = lst;
    let temp_object = temp_result;
    let temp_result = 3;
    let method_arg_0 = temp_result;
    let temp_result = lst__index(&temp_object, method_arg_0);
    let idx = temp_result;
    let temp_result = "Index of 3: ";
    let fstring_left_1 = temp_result;
    let temp_result = idx;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_elem_0 = 1;
    let temp_elem_1 = 2;
    let temp_elem_2 = 3;
    let temp_result = vec![temp_elem_0, temp_elem_1, temp_elem_2];
    let lst3 = temp_result;
    let temp_result = lst3;
    let temp_object = temp_result;
    let temp_result = lst__reverse(temp_object);
    let temp_result = "Reversed: ";
    let fstring_left_1 = temp_result;
    let temp_result = lst3;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
    let temp_elem_0 = 1;
    let temp_elem_1 = 2;
    let temp_elem_2 = 1;
    let temp_elem_3 = 3;
    let temp_elem_4 = 1;
    let temp_result = vec![temp_elem_0, temp_elem_1, temp_elem_2, temp_elem_3, temp_elem_4];
    let lst4 = temp_result;
    let temp_result = lst4;
    let temp_object = temp_result;
    let temp_result = 1;
    let method_arg_0 = temp_result;
    let temp_result = text__count(&temp_object, method_arg_0);
    let count = temp_result;
    let temp_result = "Count of 1: ";
    let fstring_left_1 = temp_result;
    let temp_result = count;
    let fstring_right_1 = temp_result;
    let fstring_result_1 = { let l = format!("{:?}", &fstring_left_1); format!("{}{}", l, fstring_right_1) };
    let temp_result = fstring_result_1;
    let arg_0 = temp_result;
    println!("{}", arg_0);
}


fn main() {
    println!("Program completed successfully");
}
