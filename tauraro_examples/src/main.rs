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

// Type-safe operation helpers
impl TauObject {
    fn type_name(&self) -> String {
        match self {
            TauObject::None => "NoneType".to_string(),
            TauObject::Bool(_) => "bool".to_string(),
            TauObject::Int(_) => "int".to_string(),
            TauObject::Float(_) => "float".to_string(),
            TauObject::String(_) => "str".to_string(),
            TauObject::List(_) => "list".to_string(),
            TauObject::Dict(_) => "dict".to_string(),
            TauObject::Custom(name, _) => format!("{}", name),
        }
    }

    fn compare(&self, other: &TauObject) -> Result<std::cmp::Ordering, String> {
        use std::cmp::Ordering;
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
            _ => Err(format!("'<' not supported between instances of '{}' and '{}'", self.type_name(), other.type_name())),
        }
    }

    fn contains(&self, item: &TauObject) -> Result<bool, String> {
        match self {
            TauObject::List(items) => {
                Ok(items.iter().any(|x| x == item))
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

    fn get_item(&self, key: &TauObject) -> Result<TauObject, String> {
        match self {
            TauObject::Dict(map) => {
                if let TauObject::String(key_str) = key {
                    Ok(map.get(key_str).cloned().unwrap_or(TauObject::None))
                } else {
                    Err("dictionary keys must be strings".to_string())
                }
            }
            TauObject::List(items) => {
                let idx = key.to_int()? as usize;
                Ok(items.get(idx).cloned().unwrap_or(TauObject::None))
            }
            _ => Err(format!("'{}' object is not subscriptable", self.type_name())),
        }
    }

    fn to_bool(&self) -> bool {
        match self {
            TauObject::None => false,
            TauObject::Bool(b) => *b,
            TauObject::Int(i) => *i != 0,
            TauObject::Float(f) => *f != 0.0,
            TauObject::String(s) => !s.is_empty(),
            TauObject::List(l) => !l.is_empty(),
            TauObject::Dict(d) => !d.is_empty(),
            TauObject::Custom(_, _) => true,
        }
    }

    fn to_int(&self) -> Result<i64, String> {
        match self {
            TauObject::Int(i) => Ok(*i),
            TauObject::Float(f) => Ok(*f as i64),
            TauObject::Bool(b) => Ok(if *b { 1 } else { 0 }),
            TauObject::String(s) => s.parse::<i64>().map_err(|_| format!("invalid literal for int() with base 10: '{}'", s)),
            _ => Err(format!("int() argument must be a string or a number, not '{}'", self.type_name())),
        }
    }

    fn to_float(&self) -> Result<f64, String> {
        match self {
            TauObject::Float(f) => Ok(*f),
            TauObject::Int(i) => Ok(*i as f64),
            TauObject::Bool(b) => Ok(if *b { 1.0 } else { 0.0 }),
            TauObject::String(s) => s.parse::<f64>().map_err(|_| format!("could not convert string to float: '{}'", s)),
            _ => Err(format!("float() argument must be a string or a number, not '{}'", self.type_name())),
        }
    }

    fn iter(&self) -> std::vec::IntoIter<&TauObject> {
        match self {
            TauObject::List(items) => items.iter().collect::<Vec<_>>().into_iter(),
            _ => Vec::new().into_iter(),
        }
    }

    fn len(&self) -> usize {
        match self {
            TauObject::List(items) => items.len(),
            TauObject::String(s) => s.len(),
            TauObject::Dict(map) => map.len(),
            _ => 0,
        }
    }

    fn to_vec(&self) -> std::result::Result<Vec<TauObject>, String> {
        match self {
            TauObject::List(items) => Ok(items.clone()),
            TauObject::String(s) => Ok(s.chars().map(|c| TauObject::String(c.to_string())).collect()),
            _ => Err(format!("'{}' object is not iterable", self.type_name())),
        }
    }
}

// Implement Neg trait for TauObject
impl std::ops::Neg for TauObject {
    type Output = TauObject;
    fn neg(self) -> TauObject {
        match self {
            TauObject::Int(i) => TauObject::Int(-i),
            TauObject::Float(f) => TauObject::Float(-f),
            _ => TauObject::None,
        }
    }
}

// Implement Neg trait for &TauObject
impl std::ops::Neg for &TauObject {
    type Output = TauObject;
    fn neg(self) -> TauObject {
        match self {
            TauObject::Int(i) => TauObject::Int(-i),
            TauObject::Float(f) => TauObject::Float(-f),
            _ => TauObject::None,
        }
    }
}

// Display implementations for collections using wrapper types
pub struct TauList(pub Vec<TauObject>);

impl fmt::Display for TauList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let items: Vec<String> = self.0.iter().map(|item| item.to_string()).collect();
        write!(f, "[{}]", items.join(", "))
    }
}

pub struct TauDict(pub std::collections::HashMap<String, TauObject>);

impl fmt::Display for TauDict {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let items: Vec<String> = self.0.iter()
            .map(|(k, v)| format!("'{}': {}", k, v.to_string()))
            .collect();
        write!(f, "{{{}}}", items.join(", "))
    }
}

// Type-safe operation functions
fn tau_add(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
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

fn tau_sub(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x - y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x - y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 - y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x - *y as f64)),
        _ => Err(format!("unsupported operand type(s) for -: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_mul(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
    match (a, b) {
        (TauObject::Int(x), TauObject::Int(y)) => Ok(TauObject::Int(x * y)),
        (TauObject::Float(x), TauObject::Float(y)) => Ok(TauObject::Float(x * y)),
        (TauObject::Int(x), TauObject::Float(y)) => Ok(TauObject::Float(*x as f64 * y)),
        (TauObject::Float(x), TauObject::Int(y)) => Ok(TauObject::Float(x * *y as f64)),
        (TauObject::String(x), TauObject::Int(y)) => {
            if *y < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            Ok(TauObject::String(x.repeat(*y as usize)))
        }
        (TauObject::Int(x), TauObject::String(y)) => {
            if *x < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            Ok(TauObject::String(y.repeat(*x as usize)))
        }
        (TauObject::List(x), TauObject::Int(y)) => {
            if *y < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            let mut result = Vec::new();
            for _ in 0..*y {
                result.extend(x.clone());
            }
            Ok(TauObject::List(result))
        }
        (TauObject::Int(x), TauObject::List(y)) => {
            if *x < 0 { return Err("can't multiply sequence by non-int".to_string()); }
            let mut result = Vec::new();
            for _ in 0..*x {
                result.extend(y.clone());
            }
            Ok(TauObject::List(result))
        }
        _ => Err(format!("unsupported operand type(s) for *: '{}' and '{}'", a.type_name(), b.type_name())),
    }
}

fn tau_div(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
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

fn tau_floordiv(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
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

fn tau_mod(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
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

fn tau_pow(a: &TauObject, b: &TauObject) -> Result<TauObject, String> {
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

fn tau_range(start: &TauObject, end: &TauObject) -> Result<Vec<TauObject>, String> {
    match (start, end) {
        (TauObject::Int(s), TauObject::Int(e)) => {
            Ok((*s..*e).map(|i| TauObject::Int(i)).collect())
        }
        _ => Err(format!("range() requires integer arguments")),
    }
}

fn tau_range_step(start: &TauObject, end: &TauObject, step: &TauObject) -> Result<Vec<TauObject>, String> {
    match (start, end, step) {
        (TauObject::Int(s), TauObject::Int(e), TauObject::Int(st)) => {
            if *st == 0 {
                return Err("range() step argument must not be zero".to_string());
            }
            let mut result = Vec::new();
            if *st > 0 {
                let mut i = *s;
                while i < *e {
                    result.push(TauObject::Int(i));
                    i += st;
                }
            } else {
                let mut i = *s;
                while i > *e {
                    result.push(TauObject::Int(i));
                    i += st;
                }
            }
            Ok(result)
        }
        _ => Err(format!("range() requires integer arguments")),
    }
}

fn tau_sum(items: &TauObject) -> TauObject {
    match items {
        TauObject::List(list) => {
            let mut sum: i64 = 0;
            let mut is_float = false;
            let mut float_sum: f64 = 0.0;
            
            for item in list {
                match item {
                    TauObject::Int(i) => {
                        if is_float {
                            float_sum += *i as f64;
                        } else {
                            sum += i;
                        }
                    }
                    TauObject::Float(f) => {
                        if !is_float {
                            float_sum = sum as f64;
                            is_float = true;
                        }
                        float_sum += f;
                    }
                    _ => {}
                }
            }
            
            if is_float {
                TauObject::Float(float_sum)
            } else {
                TauObject::Int(sum)
            }
        }
        _ => TauObject::None,
    }
}

fn vec_to_display_string(vec: &Vec<TauObject>) -> String {
    let items: Vec<String> = vec.iter().map(|i| i.to_string()).collect();
    format!("[{}]", items.join(", "))
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

fn text__lstrip(s: &str) -> String {
    s.trim_start().to_string()
}

fn text__rstrip(s: &str) -> String {
    s.trim_end().to_string()
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

fn text__isalnum(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_alphanumeric())
}

fn text__isspace(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_whitespace())
}

fn text__isupper(s: &str) -> bool {
    let has_cased = s.chars().any(|c| c.is_alphabetic());
    let all_upper = s.chars().filter(|c| c.is_alphabetic()).all(|c| c.is_uppercase());
    has_cased && all_upper
}

fn text__islower(s: &str) -> bool {
    let has_cased = s.chars().any(|c| c.is_alphabetic());
    let all_lower = s.chars().filter(|c| c.is_alphabetic()).all(|c| c.is_lowercase());
    has_cased && all_lower
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

// Set methods
fn set__add(mut s: Vec<i64>, item: i64) -> Vec<i64> {
    if !s.contains(&item) {
        s.push(item);
    }
    s
}

fn set__discard(mut s: Vec<i64>, item: i64) -> Vec<i64> {
    s.retain(|x| x != &item);
    s
}

fn set__union(mut s: Vec<i64>, other: Vec<i64>) -> Vec<i64> {
    for item in other {
        if !s.contains(&item) {
            s.push(item);
        }
    }
    s
}

fn set__intersection(s: &[i64], other: &[i64]) -> Vec<i64> {
    let mut result = Vec::new();
    for item in s {
        if other.contains(item) && !result.contains(item) {
            result.push(*item);
        }
    }
    result
}

fn set__difference(s: &[i64], other: &[i64]) -> Vec<i64> {
    let mut result = Vec::new();
    for item in s {
        if !other.contains(item) && !result.contains(item) {
            result.push(*item);
        }
    }
    result
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

// Note: tau_sum is now defined in compile_builtins for TauObject types
// The old raw i64 version is no longer emitted to avoid conflicts

// Note: tau_pow is defined in TauObject impl for handling TauObject types
// The old raw i64 version is no longer emitted to avoid conflicts

fn tau_round(f: f64) -> i64 {
    f.round() as i64
}

fn tau_divmod(a: i64, b: i64) -> (i64, i64) {
    (a / b, a % b)
}

fn tau_hex(n: i64) -> String {
    format!("0x{:x}", n)
}

fn tau_oct(n: i64) -> String {
    format!("0o{:o}", n)
}

fn tau_bin(n: i64) -> String {
    format!("0b{:b}", n)
}

fn tau_ord(c: &str) -> i64 {
    c.chars().next().unwrap_or('\0') as i64
}

fn tau_chr(n: i64) -> String {
    ((n as u8) as char).to_string()
}

// Note: tau_type and tau_isinstance are defined in TauObject impl for handling TauObject types
// Old raw i64 versions removed to avoid conflicts


// Function implementations


fn main() {
    let arg_0 = TauObject::String("=== For Loop with Range ===".to_string());
    println!("{}", arg_0.to_string());
    let arg_0 = TauObject::Int(5);
    let temp = TauObject::List(tau_range(&TauObject::Int(0), &arg_0).unwrap_or_default());
    let _iter_items = if let TauObject::List(items) = &temp { items.clone() } else { vec![] };
    for i in &_iter_items {
        let i = if let TauObject::List(items) = &i { items.get(0).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let val = if let TauObject::List(items) = &i { items.get(1).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let arg_0 = i.clone();
        println!("{}", arg_0.to_string());
    }
    let arg_0 = TauObject::String("\n=== For Loop with Range Start/Stop ===".to_string());
    println!("{}", arg_0.to_string());
    let arg_0 = TauObject::Int(2);
    let arg_1 = TauObject::Int(6);
    let temp = TauObject::List(tau_range(&arg_0, &arg_1).unwrap_or_default());
    let _iter_items = if let TauObject::List(items) = &temp { items.clone() } else { vec![] };
    for i in &_iter_items {
        let i = if let TauObject::List(items) = &i { items.get(0).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let val = if let TauObject::List(items) = &i { items.get(1).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let arg_0 = i.clone();
        println!("{}", arg_0.to_string());
    }
    let arg_0 = TauObject::String("\n=== For Loop with Step ===".to_string());
    println!("{}", arg_0.to_string());
    let arg_0 = TauObject::Int(0);
    let arg_1 = TauObject::Int(10);
    let arg_2 = TauObject::Int(2);
    let temp = TauObject::List(tau_range_step(&arg_0, &arg_1, &arg_2).unwrap_or_default());
    let _iter_items = if let TauObject::List(items) = &temp { items.clone() } else { vec![] };
    for i in &_iter_items {
        let i = if let TauObject::List(items) = &i { items.get(0).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let val = if let TauObject::List(items) = &i { items.get(1).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let arg_0 = i.clone();
        println!("{}", arg_0.to_string());
    }
    let arg_0 = TauObject::String("\n=== Nested Loop ===".to_string());
    println!("{}", arg_0.to_string());
    let arg_0 = TauObject::Int(3);
    let temp = TauObject::List(tau_range(&TauObject::Int(0), &arg_0).unwrap_or_default());
    let _iter_items = if let TauObject::List(items) = &temp { items.clone() } else { vec![] };
    for i in &_iter_items {
        let i = if let TauObject::List(items) = &i { items.get(0).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let val = if let TauObject::List(items) = &i { items.get(1).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
        let arg_0 = TauObject::Int(2);
        let temp_result = TauObject::List(tau_range(&TauObject::Int(0), &arg_0).unwrap_or_default());
        let _iter_items = if let TauObject::List(items) = &temp_result { items.clone() } else { vec![] };
        for j in &_iter_items {
            let i = if let TauObject::List(items) = &j { items.get(0).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
            let val = if let TauObject::List(items) = &j { items.get(1).cloned().unwrap_or(TauObject::None) } else { TauObject::None };
            let binop_left = TauObject::String("i=".to_string());
            let arg_0 = i.clone();
            let temp_result = TauObject::String(format!("{}", arg_0));
            let binop_right = temp_result.clone();
            let temp_result = tau_add(&binop_left, &binop_right).unwrap_or(TauObject::None);
            let binop_left = temp_result.clone();
            let binop_right = TauObject::String(", j=".to_string());
            let temp_result = tau_add(&binop_left, &binop_right).unwrap_or(TauObject::None);
            let arg_0_left = temp_result.clone();
            let arg_0 = j.clone();
            let temp_result = TauObject::String(format!("{}", arg_0));
            let arg_0_right = temp_result.clone();
            let arg_0 = tau_add(&arg_0_left, &arg_0_right).unwrap_or(TauObject::None);
            println!("{}", arg_0.to_string());
        }
    }
}
