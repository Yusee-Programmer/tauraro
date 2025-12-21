// Rust Transpiler Helper Functions for Methods and Builtins
// This is injected into generated Rust code

// List methods
fn lst__append<T: Clone>(mut list: Vec<T>, item: T) -> Vec<T> {
    list.push(item);
    list
}

fn lst__pop<T: Clone + Default>(mut list: Vec<T>) -> (Vec<T>, T) {
    let item = list.pop().unwrap_or_default();
    (list, item)
}

fn lst__extend<T: Clone>(mut list: Vec<T>, other: Vec<T>) -> Vec<T> {
    list.extend(other);
    list
}

fn lst__index<T: Clone + PartialEq>(list: &[T], item: T) -> i64 {
    for (i, elem) in list.iter().enumerate() {
        if elem == &item {
            return i as i64;
        }
    }
    -1 // Not found
}

fn lst__reverse<T: Clone>(mut list: Vec<T>) -> Vec<T> {
    list.reverse();
    list
}

fn lst__count<T: Clone + PartialEq>(list: &[T], item: T) -> i64 {
    list.iter().filter(|x| x == &&item).count() as i64
}

fn lst__insert<T: Clone>(mut list: Vec<T>, index: usize, item: T) -> Vec<T> {
    if index <= list.len() {
        list.insert(index, item);
    }
    list
}

fn lst__remove<T: Clone + PartialEq>(mut list: Vec<T>, item: T) -> Vec<T> {
    list.retain(|x| x != &item);
    list
}

fn lst__clear<T: Clone>(mut list: Vec<T>) -> Vec<T> {
    list.clear();
    list
}

// String methods
fn str__upper(s: &str) -> String {
    s.to_uppercase()
}

fn str__lower(s: &str) -> String {
    s.to_lowercase()
}

fn str__strip(s: &str) -> String {
    s.trim().to_string()
}

fn str__replace(s: &str, old: &str, new: &str) -> String {
    s.replace(old, new)
}

fn str__split(s: &str, sep: &str) -> Vec<String> {
    s.split(sep).map(|x| x.to_string()).collect()
}

fn str__join(sep: &str, items: &[String]) -> String {
    items.join(sep)
}

fn str__startswith(s: &str, prefix: &str) -> bool {
    s.starts_with(prefix)
}

fn str__endswith(s: &str, suffix: &str) -> bool {
    s.ends_with(suffix)
}

fn str__find(s: &str, sub: &str) -> i64 {
    s.find(sub).map(|i| i as i64).unwrap_or(-1)
}

fn str__index(s: &str, sub: &str) -> i64 {
    s.find(sub).map(|i| i as i64).unwrap_or(-1)
}

fn str__count(s: &str, sub: &str) -> i64 {
    s.matches(sub).count() as i64
}

fn str__capitalize(s: &str) -> String {
    let mut chars = s.chars();
    match chars.next() {
        None => String::new(),
        Some(first) => first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase(),
    }
}

fn str__title(s: &str) -> String {
    s.split_whitespace()
        .map(|word| str__capitalize(word))
        .collect::<Vec<_>>()
        .join(" ")
}

fn str__isdigit(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_ascii_digit())
}

fn str__isalpha(s: &str) -> bool {
    !s.is_empty() && s.chars().all(|c| c.is_alphabetic())
}

// Dict methods
fn dict__get(map: &std::collections::HashMap<String, String>, key: &str, default: &str) -> String {
    map.get(key).cloned().unwrap_or_else(|| default.to_string())
}

fn dict__pop(mut map: std::collections::HashMap<String, String>, key: &str) -> (std::collections::HashMap<String, String>, Option<String>) {
    let value = map.remove(key);
    (map, value)
}

fn dict__keys(map: &std::collections::HashMap<String, String>) -> Vec<String> {
    map.keys().cloned().collect()
}

fn dict__values(map: &std::collections::HashMap<String, String>) -> Vec<String> {
    map.values().cloned().collect()
}

fn dict__items(map: &std::collections::HashMap<String, String>) -> Vec<(String, String)> {
    map.iter().map(|(k, v)| (k.clone(), v.clone())).collect()
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

fn tau_min(a: i64, b: i64) -> i64 {
    a.min(b)
}

fn tau_max(a: i64, b: i64) -> i64 {
    a.max(b)
}

fn tau_pow(base: i64, exp: i64) -> i64 {
    base.pow(exp as u32)
}

fn tau_round(f: f64) -> i64 {
    f.round() as i64
}
