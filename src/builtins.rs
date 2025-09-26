use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::io::{self, Write};

/// I/O Functions

pub fn builtin_print(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        println!();
    } else {
        let output = args.iter()
            .map(|v| match v {
                Value::String(s) => s.clone(),
                _ => v.to_string(),
            })
            .collect::<Vec<_>>()
            .join(" ");
        println!("{}", output);
    }
    Ok(Value::None)
}

pub fn builtin_input(args: Vec<Value>) -> Result<Value> {
    // Print prompt if provided
    if !args.is_empty() {
        match &args[0] {
            Value::String(prompt) => {
                print!("{}", prompt);
                io::stdout().flush().unwrap_or(());
            }
            _ => {
                print!("{}", args[0].to_string());
                io::stdout().flush().unwrap_or(());
            }
        }
    }
    
    // Read input from stdin
    let mut input = String::new();
    match io::stdin().read_line(&mut input) {
        Ok(_) => {
            // Remove trailing newline
            if input.ends_with('\n') {
                input.pop();
                if input.ends_with('\r') {
                    input.pop();
                }
            }
            Ok(Value::String(input))
        }
        Err(e) => Err(anyhow::anyhow!("Failed to read input: {}", e)),
    }
}

/// Type Functions

pub fn builtin_len(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("len() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::String(s) => Ok(Value::Int(s.len() as i64)),
        Value::List(items) => Ok(Value::Int(items.len() as i64)),
        Value::Tuple(items) => Ok(Value::Int(items.len() as i64)),
        Value::Set(items) => Ok(Value::Int(items.len() as i64)),
        Value::Bytes(bytes) => Ok(Value::Int(bytes.len() as i64)),
        Value::ByteArray(bytes) => Ok(Value::Int(bytes.len() as i64)),
        Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
        _ => Err(anyhow::anyhow!("object of type '{}' has no len()", type_name(&args[0]))),
    }
}

pub fn builtin_type(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("type() takes exactly one argument"));
    }
    Ok(Value::String(type_name(&args[0])))
}

pub fn builtin_str(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("str() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::String(s) => Ok(Value::String(s.clone())),
        _ => Ok(Value::String(args[0].to_string())),
    }
}

pub fn builtin_int(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("int() takes 1 or 2 arguments"));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Int(*n)),
        Value::Float(f) => Ok(Value::Int(*f as i64)),
        Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
        Value::String(s) => {
            let base = if args.len() == 2 {
                match &args[1] {
                    Value::Int(b) => *b as u32,
                    _ => return Err(anyhow::anyhow!("int() base must be an integer")),
                }
            } else {
                10
            };
            
            match i64::from_str_radix(s.trim(), base) {
                Ok(n) => Ok(Value::Int(n)),
                Err(_) => Err(anyhow::anyhow!("invalid literal for int() with base {}: '{}'", base, s)),
            }
        }
        Value::Bytes(bytes) => {
            let s = String::from_utf8_lossy(bytes);
            match i64::from_str_radix(s.trim(), 10) {
                Ok(n) => Ok(Value::Int(n)),
                Err(_) => Err(anyhow::anyhow!("invalid literal for int(): '{}'", s)),
            }
        }
        Value::ByteArray(bytes) => {
            let s = String::from_utf8_lossy(bytes);
            match i64::from_str_radix(s.trim(), 10) {
                Ok(n) => Ok(Value::Int(n)),
                Err(_) => Err(anyhow::anyhow!("invalid literal for int(): '{}'", s)),
            }
        }
        _ => Err(anyhow::anyhow!("int() argument must be a string, a bytes-like object or a number")),
    }
}

pub fn builtin_float(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("float() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Float(f) => Ok(Value::Float(*f)),
        Value::Int(i) => Ok(Value::Float(*i as f64)),
        Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
        Value::String(s) => {
            match s.trim().parse::<f64>() {
                Ok(f) => Ok(Value::Float(f)),
                Err(_) => Err(anyhow::anyhow!("could not convert string to float: '{}'", s)),
            }
        }
        #[cfg(feature = "ffi")]
        Value::ExternFunction { .. } => Err(anyhow::anyhow!("float() argument must be a string or a number")),
        _ => Err(anyhow::anyhow!("float() argument must be a string or a number")),
    }
}

pub fn builtin_bool(args: Vec<Value>) -> Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("bool() takes at most 1 argument"));
    }
    
    if args.is_empty() {
        return Ok(Value::Bool(false));
    }
    
    Ok(Value::Bool(is_truthy(&args[0])))
}

pub fn builtin_range(args: Vec<Value>) -> Result<Value> {
    let (start, stop, step) = match args.len() {
        1 => {
            match &args[0] {
                Value::Int(stop) => (0, *stop, 1),
                _ => return Err(anyhow::anyhow!("range() argument must be an integer")),
            }
        }
        2 => {
            match (&args[0], &args[1]) {
                (Value::Int(start), Value::Int(stop)) => (*start, *stop, 1),
                _ => return Err(anyhow::anyhow!("range() arguments must be integers")),
            }
        }
        3 => {
            match (&args[0], &args[1], &args[2]) {
                (Value::Int(start), Value::Int(stop), Value::Int(step)) => (*start, *stop, *step),
                _ => return Err(anyhow::anyhow!("range() arguments must be integers")),
            }
        }
        _ => return Err(anyhow::anyhow!("range() takes 1 to 3 arguments")),
    };
    
    if step == 0 {
        return Err(anyhow::anyhow!("range() step argument must not be zero"));
    }
    
    let mut result = Vec::new();
    let mut current = start;
    
    if step > 0 {
        while current < stop {
            result.push(Value::Int(current));
            current += step;
        }
    } else {
        while current > stop {
            result.push(Value::Int(current));
            current += step;
        }
    }
    
    Ok(Value::List(result))
}

pub fn builtin_isinstance(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("isinstance() takes exactly 2 arguments"));
    }
    
    let obj_type = type_name(&args[0]);
    let expected_type = match &args[1] {
        Value::String(type_str) => type_str.clone(),
        _ => return Err(anyhow::anyhow!("isinstance() second argument must be a type name")),
    };
    
    Ok(Value::Bool(obj_type == expected_type))
}

/// Collection Constructors

pub fn builtin_list(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        0 => Ok(Value::List(Vec::new())),
        1 => {
            match &args[0] {
                Value::List(items) => Ok(Value::List(items.clone())),
                Value::String(s) => {
                    let chars: Vec<Value> = s.chars()
                        .map(|c| Value::String(c.to_string()))
                        .collect();
                    Ok(Value::List(chars))
                }
                _ => Ok(Value::List(vec![args[0].clone()])),
            }
        }
        _ => Ok(Value::List(args)),
    }
}

pub fn builtin_dict(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        0 => Ok(Value::Dict(HashMap::new())),
        1 => {
            match &args[0] {
                Value::Dict(dict) => Ok(Value::Dict(dict.clone())),
                Value::List(items) => {
                    let mut dict = HashMap::new();
                    for (i, item) in items.iter().enumerate() {
                        dict.insert(i.to_string(), item.clone());
                    }
                    Ok(Value::Dict(dict))
                }
                _ => Err(anyhow::anyhow!("dict() argument must be a dictionary or list")),
            }
        }
        _ => Err(anyhow::anyhow!("dict() takes at most 1 argument")),
    }
}

pub fn builtin_tuple(args: Vec<Value>) -> Result<Value> {
    // Since we don't have a separate Tuple type, return a list
    match args.len() {
        0 => Ok(Value::List(Vec::new())),
        1 => {
            match &args[0] {
                Value::List(items) => Ok(Value::List(items.clone())),
                Value::String(s) => {
                    let chars: Vec<Value> = s.chars()
                        .map(|c| Value::String(c.to_string()))
                        .collect();
                    Ok(Value::List(chars))
                }
                _ => Ok(Value::List(vec![args[0].clone()])),
            }
        }
        _ => Ok(Value::List(args)),
    }
}

pub fn builtin_set(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        0 => Ok(Value::Set(Vec::new())), // Empty set
        1 => {
            let items = match &args[0] {
                Value::List(items) => items.clone(),
                Value::String(s) => {
                    s.chars().map(|c| Value::String(c.to_string())).collect()
                }
                _ => vec![args[0].clone()],
            };
            
            // Remove duplicates (simple implementation)
            let mut unique_items = Vec::new();
            for item in items {
                if !unique_items.iter().any(|x| values_equal(x, &item)) {
                    unique_items.push(item);
                }
            }
            Ok(Value::Set(unique_items))
        }
        _ => Err(anyhow::anyhow!("set() takes at most 1 argument")),
    }
}

/// Object Introspection Functions

pub fn builtin_dir(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        0 => {
            // Return builtin names - this will be enhanced when VM context is available
            let builtins = vec![
                "print", "input", "type", "isinstance", "len", "str", "int", "float", "bool",
                "list", "dict", "tuple", "set", "range", "dir", "hasattr", "getattr",
                "abs", "min", "max", "sum", "round", "pow", "divmod",
                "enumerate", "zip", "map", "filter", "sorted", "reversed",
                "id", "hash", "repr", "format", "ord", "chr", "hex", "oct", "bin",
                "globals", "locals"
            ];
            let builtin_values: Vec<Value> = builtins.into_iter()
                .map(|name| Value::String(name.to_string()))
                .collect();
            Ok(Value::List(builtin_values))
        }
        1 => {
            // Return attributes of the object
            match &args[0] {
                Value::Dict(dict) => {
                    let mut keys: Vec<String> = dict.keys().cloned().collect();
                    keys.sort();
                    let key_values: Vec<Value> = keys.into_iter()
                        .map(|k| Value::String(k))
                        .collect();
                    Ok(Value::List(key_values))
                }
                Value::Object(_, fields) => {
                    let mut keys: Vec<String> = fields.keys().cloned().collect();
                    keys.sort();
                    let key_values: Vec<Value> = keys.into_iter()
                        .map(|k| Value::String(k))
                        .collect();
                    Ok(Value::List(key_values))
                }
                Value::Module(_, namespace) => {
                    let mut keys: Vec<String> = namespace.keys().cloned().collect();
                    keys.sort();
                    let key_values: Vec<Value> = keys.into_iter()
                        .map(|k| Value::String(k))
                        .collect();
                    Ok(Value::List(key_values))
                }
                _ => {
                    // Return basic type methods/attributes
                    let type_name = type_name(&args[0]);
                    let attrs = match type_name.as_str() {
                        "str" => vec![
                            "count", "endswith", "find", "index", "isalnum", "isalpha", 
                            "isdigit", "isspace", "join", "lower", "replace", "split", 
                            "startswith", "strip", "upper"
                        ],
                        "list" => vec![
                            "append", "clear", "copy", "count", "extend", "index", 
                            "insert", "pop", "remove", "reverse", "sort"
                        ],
                        "dict" => vec![
                            "clear", "copy", "get", "items", "keys", "pop", "popitem", 
                            "setdefault", "update", "values"
                        ],
                        "tuple" => vec!["count", "index"],
                        "set" => vec![
                            "add", "clear", "copy", "difference", "discard", "intersection", 
                            "pop", "remove", "union"
                        ],
                        "int" => vec!["bit_length"],
                        "float" => vec!["is_integer"],
                        _ => vec![],
                    };
                    let mut attr_values: Vec<Value> = attrs.into_iter()
                        .map(|attr| Value::String(attr.to_string()))
                        .collect();
                    attr_values.sort_by(|a, b| {
                        if let (Value::String(s1), Value::String(s2)) = (a, b) {
                            s1.cmp(s2)
                        } else {
                            std::cmp::Ordering::Equal
                        }
                    });
                    Ok(Value::List(attr_values))
                }
            }
        }
        _ => Err(anyhow::anyhow!("dir() takes at most 1 argument")),
    }
}

pub fn builtin_hasattr(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("hasattr() takes exactly 2 arguments"));
    }
    
    let attr_name = match &args[1] {
        Value::String(name) => name,
        _ => return Err(anyhow::anyhow!("hasattr() attribute name must be a string")),
    };
    
    let has_attr = match &args[0] {
        Value::Dict(dict) => dict.contains_key(attr_name),
        Value::Object(_, fields) => fields.contains_key(attr_name),
        _ => false, // Basic types don't have dynamic attributes in this implementation
    };
    
    Ok(Value::Bool(has_attr))
}

pub fn builtin_getattr(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("getattr() takes 2 or 3 arguments"));
    }
    
    let attr_name = match &args[1] {
        Value::String(name) => name,
        _ => return Err(anyhow::anyhow!("getattr() attribute name must be a string")),
    };
    
    let result = match &args[0] {
        Value::Dict(dict) => dict.get(attr_name).cloned(),
        Value::Object(_, fields) => fields.get(attr_name).cloned(),
        _ => None,
    };
    
    match result {
        Some(value) => Ok(value),
        None => {
            if args.len() == 3 {
                Ok(args[2].clone()) // Return default value
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", type_name(&args[0]), attr_name))
            }
        }
    }
}

/// Math Functions

pub fn builtin_abs(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("abs() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Int(n.abs())),
        Value::Float(f) => Ok(Value::Float(f.abs())),
        _ => Err(anyhow::anyhow!("bad operand type for abs(): '{}'", type_name(&args[0]))),
    }
}

pub fn builtin_min(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("min expected at least 1 argument, got 0"));
    }
    
    let mut min_val = &args[0];
    
    for arg in &args[1..] {
        match (min_val, arg) {
            (Value::Int(a), Value::Int(b)) => if b < a { min_val = arg; },
            (Value::Float(a), Value::Float(b)) => if b < a { min_val = arg; },
            (Value::Int(a), Value::Float(b)) => if b < &(*a as f64) { min_val = arg; },
            (Value::Float(a), Value::Int(b)) => if (*b as f64) < *a { min_val = arg; },
            (Value::String(a), Value::String(b)) => if b < a { min_val = arg; },
            _ => return Err(anyhow::anyhow!("'<' not supported between instances of '{}' and '{}'", type_name(arg), type_name(min_val))),
        }
    }
    
    Ok(min_val.clone())
}

pub fn builtin_max(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("max expected at least 1 argument, got 0"));
    }
    
    let mut max_val = &args[0];
    
    for arg in &args[1..] {
        match (max_val, arg) {
            (Value::Int(a), Value::Int(b)) => if b > a { max_val = arg; },
            (Value::Float(a), Value::Float(b)) => if b > a { max_val = arg; },
            (Value::Int(a), Value::Float(b)) => if b > &(*a as f64) { max_val = arg; },
            (Value::Float(a), Value::Int(b)) => if (*b as f64) > *a { max_val = arg; },
            (Value::String(a), Value::String(b)) => if b > a { max_val = arg; },
            _ => return Err(anyhow::anyhow!("'>' not supported between instances of '{}' and '{}'", type_name(arg), type_name(max_val))),
        }
    }
    
    Ok(max_val.clone())
}

pub fn builtin_sum(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("sum expected at least 1 argument, got 0"));
    }
    
    let start = if args.len() > 1 { &args[1] } else { &Value::Int(0) };
    let mut result = start.clone();
    
    let iterable = &args[0];
    let items = match iterable {
        Value::List(items) => items,
        _ => return Err(anyhow::anyhow!("sum() can't sum {}", type_name(iterable))),
    };
    
    for item in items {
        match (&result, item) {
            (Value::Int(a), Value::Int(b)) => result = Value::Int(a + b),
            (Value::Float(a), Value::Float(b)) => result = Value::Float(a + b),
            (Value::Int(a), Value::Float(b)) => result = Value::Float(*a as f64 + b),
            (Value::Float(a), Value::Int(b)) => result = Value::Float(a + *b as f64),
            _ => return Err(anyhow::anyhow!("unsupported operand type(s) for +: '{}' and '{}'", type_name(&result), type_name(item))),
        }
    }
    
    Ok(result)
}

pub fn builtin_round(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("round() takes 1 or 2 arguments"));
    }
    
    let number = &args[0];
    let ndigits = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => Some(*n),
            Value::None => None,
            _ => return Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", type_name(&args[1]))),
        }
    } else {
        None
    };
    
    match number {
        Value::Float(f) => {
            if let Some(digits) = ndigits {
                let multiplier = 10_f64.powi(digits as i32);
                Ok(Value::Float((f * multiplier).round() / multiplier))
            } else {
                Ok(Value::Int(f.round() as i64))
            }
        }
        Value::Int(i) => {
            if ndigits.is_some() {
                Ok(Value::Float(*i as f64))
            } else {
                Ok(Value::Int(*i))
            }
        }
        _ => Err(anyhow::anyhow!("must be real number, not {}", type_name(number))),
    }
}

pub fn builtin_pow(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("pow() takes 2 or 3 arguments"));
    }
    
    let base = &args[0];
    let exp = &args[1];
    
    let result = match (base, exp) {
        (Value::Int(a), Value::Int(b)) => {
            if *b >= 0 {
                Value::Int(a.pow(*b as u32))
            } else {
                Value::Float((*a as f64).powf(*b as f64))
            }
        }
        (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
        (Value::Int(a), Value::Float(b)) => Value::Float((*a as f64).powf(*b)),
        (Value::Float(a), Value::Int(b)) => Value::Float(a.powf(*b as f64)),
        _ => return Err(anyhow::anyhow!("unsupported operand type(s) for ** or pow(): '{}' and '{}'", type_name(base), type_name(exp))),
    };
    
    if args.len() == 3 {
        // Modular exponentiation - simplified implementation
        match (&result, &args[2]) {
            (Value::Int(r), Value::Int(m)) => Ok(Value::Int(r % m)),
            _ => Err(anyhow::anyhow!("pow() 3rd argument not allowed unless all arguments are integers")),
        }
    } else {
        Ok(result)
    }
}

pub fn builtin_divmod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("divmod() takes exactly 2 arguments"));
    }
    
    match (&args[0], &args[1]) {
        (Value::Int(a), Value::Int(b)) => {
            if *b == 0 {
                return Err(anyhow::anyhow!("integer division or modulo by zero"));
            }
            let div = a / b;
            let rem = a % b;
            Ok(Value::List(vec![Value::Int(div), Value::Int(rem)]))
        }
        (Value::Float(a), Value::Float(b)) => {
            if *b == 0.0 {
                return Err(anyhow::anyhow!("float divmod()"));
            }
            let div = (a / b).floor();
            let rem = a % b;
            Ok(Value::List(vec![Value::Float(div), Value::Float(rem)]))
        }
        (Value::Int(a), Value::Float(b)) => {
            if *b == 0.0 {
                return Err(anyhow::anyhow!("float divmod()"));
            }
            let a_f = *a as f64;
            let div = (a_f / b).floor();
            let rem = a_f % b;
            Ok(Value::List(vec![Value::Float(div), Value::Float(rem)]))
        }
        (Value::Float(a), Value::Int(b)) => {
            if *b == 0 {
                return Err(anyhow::anyhow!("float divmod()"));
            }
            let b_f = *b as f64;
            let div = (a / b_f).floor();
            let rem = a % b_f;
            Ok(Value::List(vec![Value::Float(div), Value::Float(rem)]))
        }
        _ => Err(anyhow::anyhow!("unsupported operand type(s) for divmod(): '{}' and '{}'", type_name(&args[0]), type_name(&args[1]))),
    }
}

/// Iterator Functions

pub fn builtin_enumerate(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("enumerate() takes 1 or 2 arguments"));
    }
    
    let start = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => *n,
            _ => return Err(anyhow::anyhow!("enumerate() start must be an integer")),
        }
    } else {
        0
    };
    
    let items = match &args[0] {
        Value::List(items) => items,
        Value::String(s) => {
            let chars: Vec<Value> = s.chars()
                .map(|c| Value::String(c.to_string()))
                .collect();
            return Ok(Value::List(
                chars.into_iter()
                    .enumerate()
                    .map(|(i, c)| Value::List(vec![Value::Int(start + i as i64), c]))
                    .collect()
            ));
        }
        _ => return Err(anyhow::anyhow!("enumerate() argument must be a sequence")),
    };
    
    let result: Vec<Value> = items.iter()
        .enumerate()
        .map(|(i, item)| Value::List(vec![Value::Int(start + i as i64), item.clone()]))
        .collect();
    
    Ok(Value::List(result))
}

pub fn builtin_zip(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::List(Vec::new()));
    }
    
    // Convert all arguments to iterables
    let mut iterables = Vec::new();
    for arg in &args {
        match arg {
            Value::List(items) => iterables.push(items.clone()),
            Value::String(s) => {
                let chars: Vec<Value> = s.chars()
                    .map(|c| Value::String(c.to_string()))
                    .collect();
                iterables.push(chars);
            }
            _ => return Err(anyhow::anyhow!("zip argument must be iterable")),
        }
    }
    
    // Find the minimum length
    let min_len = iterables.iter().map(|it| it.len()).min().unwrap_or(0);
    
    // Create tuples
    let mut result = Vec::new();
    for i in 0..min_len {
        let tuple: Vec<Value> = iterables.iter()
            .map(|it| it[i].clone())
            .collect();
        result.push(Value::List(tuple));
    }
    
    Ok(Value::List(result))
}

pub fn builtin_map(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("map() must have at least two arguments"));
    }
    
    let function = &args[0];
    let iterable = &args[1];
    
    let items = match iterable {
        Value::List(items) => items,
        Value::String(s) => {
            let chars: Vec<Value> = s.chars()
                .map(|c| Value::String(c.to_string()))
                .collect();
            return apply_function_to_items(function, &chars);
        }
        _ => return Err(anyhow::anyhow!("map() argument 2 must be iterable")),
    };
    
    apply_function_to_items(function, items)
}

pub fn builtin_filter(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("filter() takes exactly 2 arguments"));
    }
    
    let predicate = &args[0];
    let iterable = &args[1];
    
    let items = match iterable {
        Value::List(items) => items,
        Value::String(s) => {
            let chars: Vec<Value> = s.chars()
                .map(|c| Value::String(c.to_string()))
                .collect();
            return filter_items(predicate, &chars);
        }
        _ => return Err(anyhow::anyhow!("filter() argument 2 must be iterable")),
    };
    
    filter_items(predicate, items)
}

pub fn builtin_sorted(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("sorted() takes 1 to 3 arguments"));
    }
    
    let mut items = match &args[0] {
        Value::List(items) => items.clone(),
        Value::String(s) => {
            s.chars().map(|c| Value::String(c.to_string())).collect()
        }
        _ => return Err(anyhow::anyhow!("sorted() argument must be iterable")),
    };
    
    // Simple sorting - only handles basic types
    items.sort_by(|a, b| {
        match (a, b) {
            (Value::Int(x), Value::Int(y)) => x.cmp(y),
            (Value::Float(x), Value::Float(y)) => x.partial_cmp(y).unwrap_or(std::cmp::Ordering::Equal),
            (Value::String(x), Value::String(y)) => x.cmp(y),
            (Value::Bool(x), Value::Bool(y)) => x.cmp(y),
            _ => std::cmp::Ordering::Equal,
        }
    });
    
    Ok(Value::List(items))
}

pub fn builtin_reversed(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("reversed() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::List(items) => {
            let mut reversed_items = items.clone();
            reversed_items.reverse();
            Ok(Value::List(reversed_items))
        }
        Value::String(s) => {
            let reversed: String = s.chars().rev().collect();
            Ok(Value::String(reversed))
        }
        _ => Err(anyhow::anyhow!("reversed() argument must be a sequence")),
    }
}

/// Utility Functions

pub fn builtin_id(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("id() takes exactly one argument"));
    }
    
    // Simple implementation - use memory address as pointer
    let ptr = &args[0] as *const Value as usize;
    Ok(Value::Int(ptr as i64))
}

pub fn builtin_hash(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("hash() takes exactly one argument"));
    }
    
    use std::collections::hash_map::DefaultHasher;
    use std::hash::{Hash, Hasher};
    
    let hash_value = match &args[0] {
        Value::Int(n) => {
            let mut hasher = DefaultHasher::new();
            n.hash(&mut hasher);
            hasher.finish() as i64
        }
        Value::Float(f) => {
            let mut hasher = DefaultHasher::new();
            f.to_bits().hash(&mut hasher);
            hasher.finish() as i64
        }
        Value::Bool(b) => {
            let mut hasher = DefaultHasher::new();
            b.hash(&mut hasher);
            hasher.finish() as i64
        }
        Value::String(s) => {
            let mut hasher = DefaultHasher::new();
            s.hash(&mut hasher);
            hasher.finish() as i64
        }
        _ => return Err(anyhow::anyhow!("unhashable type: '{}'", type_name(&args[0]))),
    };
    
    Ok(Value::Int(hash_value))
}

pub fn builtin_repr(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("repr() takes exactly one argument"));
    }
    
    let repr_str = match &args[0] {
        Value::String(s) => format!("'{}'", s),
        Value::Int(n) => n.to_string(),
        Value::Float(f) => f.to_string(),
        Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
        Value::None => "None".to_string(),
        Value::List(items) => {
            let item_reprs: Vec<String> = items.iter()
                .map(|item| match builtin_repr(vec![item.clone()]) {
                    Ok(Value::String(s)) => s,
                    _ => "...".to_string(),
                })
                .collect();
            format!("[{}]", item_reprs.join(", "))
        }
        Value::Tuple(items) => {
            let item_reprs: Vec<String> = items.iter()
                .map(|item| match builtin_repr(vec![item.clone()]) {
                    Ok(Value::String(s)) => s,
                    _ => "...".to_string(),
                })
                .collect();
            format!("({})", item_reprs.join(", "))
        }
        Value::Set(items) => {
            let item_reprs: Vec<String> = items.iter()
                .map(|item| match builtin_repr(vec![item.clone()]) {
                    Ok(Value::String(s)) => s,
                    _ => "...".to_string(),
                })
                .collect();
            format!("{{{}}}", item_reprs.join(", "))
        }
        Value::Bytes(bytes) => {
            format!("b'{}'", String::from_utf8_lossy(bytes))
        }
        Value::ByteArray(bytes) => {
            format!("bytearray(b'{}')", String::from_utf8_lossy(bytes))
        }
        Value::Dict(dict) => {
            let mut pairs = Vec::new();
            for (k, v) in dict.iter() {
                let key_repr = builtin_repr(vec![Value::String(k.clone())]).unwrap_or(Value::String("...".to_string()));
                let val_repr = builtin_repr(vec![v.clone()]).unwrap_or(Value::String("...".to_string()));
                if let (Value::String(kr), Value::String(vr)) = (key_repr, val_repr) {
                    pairs.push(format!("{}: {}", kr, vr));
                }
            }
            format!("{{{}}}", pairs.join(", "))
        }
        Value::Function(name, params, _) => {
            format!("<function {}({})>", name, params.join(", "))
        }
        Value::BuiltinFunction(name, _) => {
            format!("<built-in function {}>", name)
        }
        Value::NativeFunction(_) => "<native function>".to_string(),
        Value::Object(name, fields) => {
            format!("<{} object with {} fields>", name, fields.len())
        }
        Value::TypedValue { value, type_info } => {
            let inner_repr = builtin_repr(vec![value.as_ref().clone()]).unwrap_or(Value::String("...".to_string()));
            if let Value::String(ir) = inner_repr {
                format!("{}: {}", ir, type_info)
            } else {
                format!("<complex value>: {}", type_info)
            }
        }
        Value::Module(name, namespace) => {
            format!("<module '{}' with {} items>", name, namespace.len())
        }
        #[cfg(feature = "ffi")]
        Value::ExternFunction { name, signature, .. } => {
            format!("<extern function '{}' with signature '{}'>", name, signature)
        }
    };
    
    Ok(Value::String(repr_str))
}

pub fn builtin_ord(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ord() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::String(s) => {
            if s.len() != 1 {
                return Err(anyhow::anyhow!("ord() expected a character, but string of length {} found", s.len()));
            }
            let ch = s.chars().next().unwrap();
            Ok(Value::Int(ch as u32 as i64))
        }
        _ => Err(anyhow::anyhow!("ord() expected str object, not '{}'", type_name(&args[0]))),
    }
}

pub fn builtin_chr(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("chr() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Int(n) => {
            if *n < 0 || *n > 1114111 {
                return Err(anyhow::anyhow!("chr() arg not in range(0x110000)"));
            }
            match char::from_u32(*n as u32) {
                Some(ch) => Ok(Value::String(ch.to_string())),
                None => Err(anyhow::anyhow!("chr() arg not in range(0x110000)")),
            }
        }
        _ => Err(anyhow::anyhow!("chr() expected int object, not '{}'", type_name(&args[0]))),
    }
}

pub fn builtin_bin(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("bin() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::String(format!("0b{:b}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", type_name(&args[0]))),
    }
}

pub fn builtin_oct(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("oct() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::String(format!("0o{:o}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", type_name(&args[0]))),
    }
}

pub fn builtin_hex(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("hex() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::String(format!("0x{:x}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", type_name(&args[0]))),
    }
}

pub fn builtin_format(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("format() takes 1 or 2 arguments"));
    }
    
    // Simple format implementation - just converts to string
    // Real Python format() is much more complex with format specs
    Ok(Value::String(args[0].to_string()))
}

pub fn builtin_globals(args: Vec<Value>) -> Result<Value> {
    if !args.is_empty() {
        return Err(anyhow::anyhow!("globals() takes no arguments"));
    }
    
    // This is a placeholder - the actual implementation needs VM context
    // The VM will need to provide the global scope variables
    Ok(Value::Dict(HashMap::new()))
}

pub fn builtin_locals(args: Vec<Value>) -> Result<Value> {
    if !args.is_empty() {
        return Err(anyhow::anyhow!("locals() takes no arguments"));
    }
    
    // This is a placeholder - the actual implementation needs VM context
    // The VM will need to provide the current local scope variables
    Ok(Value::Dict(HashMap::new()))
}

// String methods
pub fn builtin_str_join(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("str.join() takes exactly 2 arguments"));
    }
    
    match (&args[0], &args[1]) {
        (Value::String(separator), Value::List(items)) => {
            let string_items: Result<Vec<String>, _> = items.iter()
                .map(|item| match item {
                    Value::String(s) => Ok(s.clone()),
                    _ => Ok(item.to_string()),
                })
                .collect();
            
            match string_items {
                Ok(strings) => Ok(Value::String(strings.join(separator))),
                Err(e) => Err(e),
            }
        }
        _ => Err(anyhow::anyhow!("join() requires a string and a list")),
    }
}

pub fn builtin_str_split(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("str.split() takes 1 or 2 arguments"));
    }
    
    match &args[0] {
        Value::String(s) => {
            let separator = if args.len() == 2 {
                match &args[1] {
                    Value::String(sep) => sep.as_str(),
                    _ => return Err(anyhow::anyhow!("separator must be a string")),
                }
            } else {
                " " // Default separator
            };
            
            let parts: Vec<Value> = s.split(separator)
                .map(|part| Value::String(part.to_string()))
                .collect();
            Ok(Value::List(parts))
        }
        _ => Err(anyhow::anyhow!("split() can only be called on strings")),
    }
}

pub fn builtin_str_strip(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("str.strip() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::String(s) => Ok(Value::String(s.trim().to_string())),
        _ => Err(anyhow::anyhow!("strip() can only be called on strings")),
    }
}

pub fn builtin_str_upper(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("str.upper() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::String(s) => Ok(Value::String(s.to_uppercase())),
        _ => Err(anyhow::anyhow!("upper() can only be called on strings")),
    }
}

pub fn builtin_str_lower(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("str.lower() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::String(s) => Ok(Value::String(s.to_lowercase())),
        _ => Err(anyhow::anyhow!("lower() can only be called on strings")),
    }
}

// List methods
pub fn builtin_list_append(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("list.append() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::List(ref items) => {
            let mut new_items = items.clone();
            new_items.push(args[1].clone());
            Ok(Value::List(new_items))
        }
        _ => Err(anyhow::anyhow!("append() can only be called on lists")),
    }
}

pub fn builtin_list_extend(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("list.extend() takes exactly 2 arguments"));
    }
    
    match (&args[0], &args[1]) {
        (Value::List(ref items), Value::List(other_items)) => {
            let mut new_items = items.clone();
            new_items.extend(other_items.clone());
            Ok(Value::List(new_items))
        }
        _ => Err(anyhow::anyhow!("extend() requires two lists")),
    }
}

pub fn builtin_list_count(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("list.count() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::List(items) => {
            let count = items.iter()
                .filter(|item| values_equal(item, &args[1]))
                .count();
            Ok(Value::Int(count as i64))
        }
        _ => Err(anyhow::anyhow!("count() can only be called on lists")),
    }
}

pub fn builtin_list_index(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("list.index() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::List(items) => {
            for (i, item) in items.iter().enumerate() {
                if values_equal(item, &args[1]) {
                    return Ok(Value::Int(i as i64));
                }
            }
            Err(anyhow::anyhow!("value not found in list"))
        }
        _ => Err(anyhow::anyhow!("index() can only be called on lists")),
    }
}

// Dict methods
pub fn builtin_dict_keys(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("dict.keys() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::Dict(dict) => {
            let keys: Vec<Value> = dict.keys()
                .map(|k| Value::String(k.clone()))
                .collect();
            Ok(Value::List(keys))
        }
        _ => Err(anyhow::anyhow!("keys() can only be called on dictionaries")),
    }
}

pub fn builtin_dict_values(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("dict.values() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::Dict(dict) => {
            let values: Vec<Value> = dict.values().cloned().collect();
            Ok(Value::List(values))
        }
        _ => Err(anyhow::anyhow!("values() can only be called on dictionaries")),
    }
}

pub fn builtin_dict_items(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("dict.items() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::Dict(dict) => {
            let items: Vec<Value> = dict.iter()
                .map(|(k, v)| Value::Tuple(vec![Value::String(k.clone()), v.clone()]))
                .collect();
            Ok(Value::List(items))
        }
        _ => Err(anyhow::anyhow!("items() can only be called on dictionaries")),
    }
}

pub fn builtin_dict_get(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("dict.get() takes 2 or 3 arguments"));
    }
    
    match &args[0] {
        Value::Dict(dict) => {
            let key = match &args[1] {
                Value::String(s) => s,
                _ => return Err(anyhow::anyhow!("dictionary key must be a string")),
            };
            
            match dict.get(key) {
                Some(value) => Ok(value.clone()),
                None => {
                    if args.len() == 3 {
                        Ok(args[2].clone()) // Return default value
                    } else {
                        Ok(Value::None)
                    }
                }
            }
        }
        _ => Err(anyhow::anyhow!("get() can only be called on dictionaries")),
    }
}

// Numeric methods
pub fn builtin_int_bit_length(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("int.bit_length() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::Int(n) => {
            let bit_length = if *n == 0 { 0 } else { (n.abs() as u64).ilog2() + 1 };
            Ok(Value::Int(bit_length as i64))
        }
        _ => Err(anyhow::anyhow!("bit_length() can only be called on integers")),
    }
}

pub fn builtin_float_is_integer(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("float.is_integer() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::Float(f) => Ok(Value::Bool(f.fract() == 0.0)),
        _ => Err(anyhow::anyhow!("is_integer() can only be called on floats")),
    }
}

// Tuple methods
pub fn builtin_tuple_count(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("tuple.count() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::Tuple(items) => {
            let count = items.iter()
                .filter(|item| values_equal(item, &args[1]))
                .count();
            Ok(Value::Int(count as i64))
        }
        _ => Err(anyhow::anyhow!("count() can only be called on tuples")),
    }
}

pub fn builtin_tuple_index(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("tuple.index() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::Tuple(items) => {
            for (i, item) in items.iter().enumerate() {
                if values_equal(item, &args[1]) {
                    return Ok(Value::Int(i as i64));
                }
            }
            Err(anyhow::anyhow!("value not found in tuple"))
        }
        _ => Err(anyhow::anyhow!("index() can only be called on tuples")),
    }
}

// Set methods
pub fn builtin_set_add(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("set.add() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::Set(ref items) => {
            // Convert to string for comparison (simplified approach)
            let item_str = args[1].to_string();
            let mut new_items = items.clone();
            if !new_items.iter().any(|existing| existing.to_string() == item_str) {
                new_items.push(args[1].clone());
            }
            Ok(Value::Set(new_items))
        }
        _ => Err(anyhow::anyhow!("add() can only be called on sets")),
    }
}

pub fn builtin_set_remove(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("set.remove() takes exactly 2 arguments"));
    }
    
    match &args[0] {
        Value::Set(ref items) => {
            let item_str = args[1].to_string();
            let original_len = items.len();
            let mut new_items = items.clone();
            new_items.retain(|existing| existing.to_string() != item_str);
            
            if new_items.len() == original_len {
                return Err(anyhow::anyhow!("KeyError: item not found in set"));
            }
            
            Ok(Value::Set(new_items))
        }
        _ => Err(anyhow::anyhow!("remove() can only be called on sets")),
    }
}

// Bytes and ByteArray methods (simplified implementations)
pub fn builtin_bytes_decode(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("bytes.decode() takes exactly 1 argument"));
    }
    
    match &args[0] {
        Value::Bytes(bytes) => {
            match String::from_utf8(bytes.clone()) {
                Ok(s) => Ok(Value::String(s)),
                Err(_) => Err(anyhow::anyhow!("invalid UTF-8 sequence")),
            }
        }
        _ => Err(anyhow::anyhow!("decode() can only be called on bytes")),
    }
}

pub fn builtin_bytearray_append(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("bytearray.append() takes exactly 2 arguments"));
    }
    
    match (&args[0], &args[1]) {
        (Value::ByteArray(ref bytes), Value::Int(byte_val)) => {
            if *byte_val < 0 || *byte_val > 255 {
                return Err(anyhow::anyhow!("byte value must be in range 0-255"));
            }
            let mut new_bytes = bytes.clone();
            new_bytes.push(*byte_val as u8);
            Ok(Value::ByteArray(new_bytes))
        }
        _ => Err(anyhow::anyhow!("append() requires a bytearray and an integer")),
    }
}

/// Helper Functions

fn type_name(value: &Value) -> String {
    match value {
        Value::Int(_) => "int".to_string(),
        Value::Float(_) => "float".to_string(),
        Value::Bool(_) => "bool".to_string(),
        Value::String(_) => "str".to_string(),
        Value::List(_) => "list".to_string(),
        Value::Dict(_) => "dict".to_string(),
        Value::Tuple(_) => "tuple".to_string(),
        Value::Set(_) => "set".to_string(),
        Value::Bytes(_) => "bytes".to_string(),
        Value::ByteArray(_) => "bytearray".to_string(),
        Value::None => "NoneType".to_string(),
        Value::Function(_, _, _) => "function".to_string(),
        Value::BuiltinFunction(_, _) => "builtin_function_or_method".to_string(),
        Value::NativeFunction(_) => "native_function".to_string(),
        Value::Object(name, _) => name.clone(),
        Value::TypedValue { type_info, .. } => format!("{}", type_info),
        Value::Module(_, _) => "module".to_string(),
        #[cfg(feature = "ffi")]
        Value::ExternFunction { .. } => "extern_function".to_string(),
    }
}

fn is_truthy(value: &Value) -> bool {
    match value {
        Value::Bool(b) => *b,
        Value::Int(n) => *n != 0,
        Value::Float(f) => *f != 0.0,
        Value::String(s) => !s.is_empty(),
        Value::List(items) => !items.is_empty(),
        Value::Dict(dict) => !dict.is_empty(),
        Value::Tuple(items) => !items.is_empty(),
        Value::Set(items) => !items.is_empty(),
        Value::Bytes(bytes) => !bytes.is_empty(),
        Value::ByteArray(bytes) => !bytes.is_empty(),
        Value::None => false,
        _ => true,
    }
}

pub fn values_equal(a: &Value, b: &Value) -> bool {
    match (a, b) {
        (Value::Int(a), Value::Int(b)) => a == b,
        (Value::Float(a), Value::Float(b)) => (a - b).abs() < f64::EPSILON,
        (Value::Bool(a), Value::Bool(b)) => a == b,
        (Value::String(a), Value::String(b)) => a == b,
        (Value::List(a), Value::List(b)) => a.len() == b.len() && a.iter().zip(b.iter()).all(|(x, y)| values_equal(x, y)),
        (Value::Tuple(a), Value::Tuple(b)) => a.len() == b.len() && a.iter().zip(b.iter()).all(|(x, y)| values_equal(x, y)),
        (Value::Set(a), Value::Set(b)) => a.len() == b.len() && a.iter().all(|x| b.iter().any(|y| values_equal(x, y))),
        (Value::Bytes(a), Value::Bytes(b)) => a == b,
        (Value::ByteArray(a), Value::ByteArray(b)) => a == b,
        (Value::None, Value::None) => true,
        _ => false,
    }
}

fn apply_function_to_items(function: &Value, items: &[Value]) -> Result<Value> {
    // Simplified implementation - would need VM context for actual function calls
    match function {
        Value::BuiltinFunction(name, _) => {
            match name.as_str() {
                "str" => {
                    let results: Vec<Value> = items.iter()
                        .map(|item| Value::String(item.to_string()))
                        .collect();
                    Ok(Value::List(results))
                }
                "int" => {
                    let mut results = Vec::new();
                    for item in items {
                        match builtin_int(vec![item.clone()]) {
                            Ok(val) => results.push(val),
                            Err(_) => results.push(Value::None),
                        }
                    }
                    Ok(Value::List(results))
                }
                _ => Err(anyhow::anyhow!("map() function not supported: {}", name)),
            }
        }
        _ => Err(anyhow::anyhow!("map() first argument must be a function")),
    }
}

fn filter_items(predicate: &Value, items: &[Value]) -> Result<Value> {
    let mut results = Vec::new();
    
    if matches!(predicate, Value::None) {
        // Filter by truthiness
        for item in items {
            if is_truthy(item) {
                results.push(item.clone());
            }
        }
    } else {
        // Simplified implementation - would need VM context for actual function calls
        return Err(anyhow::anyhow!("filter() with custom predicate not yet implemented"));
    }
    
    Ok(Value::List(results))
}

/// FFI Functions

pub fn builtin_load_library(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("load_library() takes exactly one argument"));
    }
    
    let library_name = match &args[0] {
        Value::String(name) => name,
        _ => return Err(anyhow::anyhow!("load_library() argument must be a string")),
    };
    
    // For now, just print that we're loading the library
    // The actual FFI manager integration would need VM context
    println!("Loading library: {}", library_name);
    
    // TODO: Integrate with VM's FFI manager to actually load the library
    // This would require access to the VM instance, which builtin functions don't have
    // For now, we'll assume the library is loaded successfully
    
    Ok(Value::None)
}

pub fn builtin_help(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        // General help message
        println!("Welcome to Tauraro Interactive Help!");
        println!();
        println!("This is the Tauraro programming language interpreter.");
        println!("Type help(object) for help about object.");
        println!();
        println!("Available built-in functions:");
        println!("  abs, all, any, bin, bool, bytearray, bytes, callable, chr, dict,");
        println!("  dir, divmod, enumerate, filter, float, format, frozenset, getattr,");
        println!("  globals, hasattr, hash, help, hex, id, input, int, isinstance,");
        println!("  len, list, locals, map, max, min, oct, ord, pow, print, range,");
        println!("  repr, reversed, round, set, sorted, str, sum, tuple, type, zip");
        println!();
        println!("For more information about a specific function, use help(function_name)");
        return Ok(Value::None);
    }
    
    if args.len() != 1 {
        return Err(anyhow::anyhow!("help() takes at most 1 argument"));
    }
    
    match &args[0] {
        Value::String(name) => {
            match name.as_str() {
                "print" => {
                    println!("Help on built-in function print:");
                    println!("print(value, ..., sep=' ', end='\\n')");
                    println!("    Prints the values to a text stream, separated by sep and followed by end.");
                }
                "len" => {
                    println!("Help on built-in function len:");
                    println!("len(obj)");
                    println!("    Return the number of items in a container.");
                }
                "type" => {
                    println!("Help on built-in function type:");
                    println!("type(object)");
                    println!("    Return the type of an object.");
                }
                "str" => {
                    println!("Help on built-in function str:");
                    println!("str(object='')");
                    println!("    Return a string version of object.");
                }
                "int" => {
                    println!("Help on built-in function int:");
                    println!("int(x=0)");
                    println!("    Convert a number or string to an integer.");
                }
                "float" => {
                    println!("Help on built-in function float:");
                    println!("float(x=0.0)");
                    println!("    Convert a string or number to a floating point number.");
                }
                "bool" => {
                    println!("Help on built-in function bool:");
                    println!("bool(x=False)");
                    println!("    Convert a value to a Boolean.");
                }
                "list" => {
                    println!("Help on built-in function list:");
                    println!("list(iterable=[])");
                    println!("    Create a list from an iterable.");
                }
                "dict" => {
                    println!("Help on built-in function dict:");
                    println!("dict(**kwarg)");
                    println!("    Create a new dictionary.");
                }
                "range" => {
                    println!("Help on built-in function range:");
                    println!("range(stop) or range(start, stop[, step])");
                    println!("    Create an object which is an iterable of integers.");
                }
                "abs" => {
                    println!("Help on built-in function abs:");
                    println!("abs(x)");
                    println!("    Return the absolute value of the argument.");
                }
                "min" => {
                    println!("Help on built-in function min:");
                    println!("min(iterable, *[, default, key])");
                    println!("    Return the smallest item in an iterable or the minimum of two or more arguments.");
                }
                "max" => {
                    println!("Help on built-in function max:");
                    println!("max(iterable, *[, default, key])");
                    println!("    Return the largest item in an iterable or the maximum of two or more arguments.");
                }
                "sum" => {
                    println!("Help on built-in function sum:");
                    println!("sum(iterable, start=0)");
                    println!("    Return the sum of a 'start' value plus an iterable of numbers.");
                }
                "input" => {
                    println!("Help on built-in function input:");
                    println!("input(prompt='')");
                    println!("    Read a string from standard input.");
                }
                _ => {
                    println!("No help available for '{}'", name);
                }
            }
        }
        Value::BuiltinFunction(name, _) => {
            println!("Help on built-in function {}:", name);
            println!("This is a built-in function. Use help('{}') for more details.", name);
        }
        Value::Function(name, params, _) => {
            println!("Help on user-defined function {}:", name);
            println!("{}({})", name, params.join(", "));
            println!("This is a user-defined function.");
        }
        _ => {
            println!("Help on {}:", args[0].type_name());
            println!("This is a {} object.", args[0].type_name());
        }
    }
    
    Ok(Value::None)
}