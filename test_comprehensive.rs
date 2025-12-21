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

// Also support list.count() via text__count (convenience overload)
fn text__count_list(list: &[i64], item: i64) -> i64 {
    list.iter().filter(|x| **x == item).count() as i64
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

fn test_basic_math() -> () {
    let arg_0 = "=== Math Test ===";
    println!("{}", arg_0);
    let temp_result = 5;
    let a = temp_result;
    let temp_result = 3;
    let b = temp_result;
    let binop_left = a;
    let binop_right = b;
    let temp_result = binop_left + binop_right;
    let c = temp_result;
    let arg_0 = "Addition result:";
    println!("{}", arg_0);
    let arg_0 = c;
    println!("{}", arg_0);
    let binop_left = c;
    let binop_right = 2;
    let temp_result = binop_left - binop_right;
    let d = temp_result;
    let arg_0 = "Multiplication result:";
    println!("{}", arg_0);
    let arg_0 = d;
    println!("{}", arg_0);
    let arg_0 = "Math test done";
    println!("{}", arg_0);
}

fn test_lists() -> () {
    let arg_0 = "=== Lists Test ===";
    println!("{}", arg_0);
    let temp_elem_0 = 10;
    let temp_elem_1 = 20;
    let temp_elem_2 = 30;
    let temp_result = vec![temp_elem_0, temp_elem_1, temp_elem_2];
    let nums = temp_result;
    let arg_0 = "List items:";
    println!("{}", arg_0);
    let temp_result = nums;
    for num in &temp_result {
        let arg_0 = num;
        println!("{}", arg_0);
    }
    let arg_0 = "Lists test done";
    println!("{}", arg_0);
}

fn test_loops() -> () {
    let arg_0 = "=== Loops Test ===";
    println!("{}", arg_0);
    let arg_0 = 3;
    let temp_result = (0..arg_0).collect::<Vec<_>>();
    for i in &temp_result {
        let arg_0 = "Loop iteration:";
        println!("{}", arg_0);
        let arg_0 = i;
        println!("{}", arg_0);
    }
    let arg_0 = "Loops test done";
    println!("{}", arg_0);
}

fn test_strings() -> () {
    let arg_0 = "=== Strings Test ===";
    println!("{}", arg_0);
    let temp_result = "hello";
    let msg = temp_result;
    let temp_result = msg;
    let temp_object = temp_result;
    let temp_result = text__upper(&temp_object);
    let upper_msg = temp_result;
    let arg_0 = "Uppercase:";
    println!("{}", arg_0);
    let arg_0 = upper_msg;
    println!("{}", arg_0);
    let arg_0 = msg;
    let temp_result = arg_0.len() as i64;
    let len_msg = temp_result;
    let arg_0 = "Length:";
    println!("{}", arg_0);
    let arg_0 = len_msg;
    println!("{}", arg_0);
    let arg_0 = "Strings test done";
    println!("{}", arg_0);
}


fn main() {
    // Run all tests
    let temp = test_basic_math();
    let temp = test_loops();
    let temp = test_strings();
    let temp = test_lists();
    let arg_0 = "=== ALL TESTS COMPLETE ===";
    println!("{}", arg_0);
}
