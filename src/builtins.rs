//! Built-in functions and values for Tauraro

use crate::value::Value;
use std::collections::HashMap;
use crate::modules::hplist::HPList;
use crate::base_object::{BaseObject, MRO};

pub fn init_builtins() -> HashMap<String, Value> {
    let mut builtins = HashMap::new();
    
    // None value
    builtins.insert("None".to_string(), Value::None);
    
    // Boolean values
    builtins.insert("True".to_string(), Value::Bool(true));
    builtins.insert("False".to_string(), Value::Bool(false));
    
    // Built-in functions
    builtins.insert("print".to_string(), Value::BuiltinFunction("print".to_string(), print_builtin));
    builtins.insert("len".to_string(), Value::BuiltinFunction("len".to_string(), len_builtin));
    builtins.insert("str".to_string(), Value::BuiltinFunction("str".to_string(), str_builtin));
    builtins.insert("int".to_string(), Value::BuiltinFunction("int".to_string(), int_builtin));
    builtins.insert("float".to_string(), Value::BuiltinFunction("float".to_string(), float_builtin));
    builtins.insert("bool".to_string(), Value::BuiltinFunction("bool".to_string(), bool_builtin));
    builtins.insert("list".to_string(), Value::BuiltinFunction("list".to_string(), list_builtin));
    builtins.insert("dict".to_string(), Value::BuiltinFunction("dict".to_string(), dict_builtin));
    builtins.insert("range".to_string(), Value::BuiltinFunction("range".to_string(), range_builtin));
    builtins.insert("input".to_string(), Value::BuiltinFunction("input".to_string(), input_builtin));
    builtins.insert("abs".to_string(), Value::BuiltinFunction("abs".to_string(), abs_builtin));
    builtins.insert("min".to_string(), Value::BuiltinFunction("min".to_string(), min_builtin));
    builtins.insert("max".to_string(), Value::BuiltinFunction("max".to_string(), max_builtin));
    builtins.insert("sum".to_string(), Value::BuiltinFunction("sum".to_string(), sum_builtin));
    builtins.insert("round".to_string(), Value::BuiltinFunction("round".to_string(), round_builtin));
    builtins.insert("pow".to_string(), Value::BuiltinFunction("pow".to_string(), pow_builtin));
    builtins.insert("divmod".to_string(), Value::BuiltinFunction("divmod".to_string(), divmod_builtin));
    builtins.insert("enumerate".to_string(), Value::BuiltinFunction("enumerate".to_string(), enumerate_builtin));
    builtins.insert("zip".to_string(), Value::BuiltinFunction("zip".to_string(), zip_builtin));
    builtins.insert("map".to_string(), Value::BuiltinFunction("map".to_string(), map_builtin));
    builtins.insert("filter".to_string(), Value::BuiltinFunction("filter".to_string(), filter_builtin));
    builtins.insert("sorted".to_string(), Value::BuiltinFunction("sorted".to_string(), sorted_builtin));
    builtins.insert("reversed".to_string(), Value::BuiltinFunction("reversed".to_string(), reversed_builtin));
    builtins.insert("any".to_string(), Value::BuiltinFunction("any".to_string(), any_builtin));
    builtins.insert("all".to_string(), Value::BuiltinFunction("all".to_string(), all_builtin));
    builtins.insert("chr".to_string(), Value::BuiltinFunction("chr".to_string(), chr_builtin));
    builtins.insert("ord".to_string(), Value::BuiltinFunction("ord".to_string(), ord_builtin));
    builtins.insert("hex".to_string(), Value::BuiltinFunction("hex".to_string(), hex_builtin));
    builtins.insert("bin".to_string(), Value::BuiltinFunction("bin".to_string(), bin_builtin));
    builtins.insert("oct".to_string(), Value::BuiltinFunction("oct".to_string(), oct_builtin));
    builtins.insert("isinstance".to_string(), Value::BuiltinFunction("isinstance".to_string(), isinstance_builtin));
    builtins.insert("type".to_string(), Value::BuiltinFunction("type".to_string(), type_builtin));
    builtins.insert("callable".to_string(), Value::BuiltinFunction("callable".to_string(), callable_builtin));
    builtins.insert("hasattr".to_string(), Value::BuiltinFunction("hasattr".to_string(), hasattr_builtin));
    builtins.insert("getattr".to_string(), Value::BuiltinFunction("getattr".to_string(), getattr_builtin));
    builtins.insert("setattr".to_string(), Value::BuiltinFunction("setattr".to_string(), setattr_builtin));
    builtins.insert("delattr".to_string(), Value::BuiltinFunction("delattr".to_string(), delattr_builtin));
    builtins.insert("issubclass".to_string(), Value::BuiltinFunction("issubclass".to_string(), issubclass_builtin));
    builtins.insert("super".to_string(), Value::BuiltinFunction("super".to_string(), super_builtin));
    builtins.insert("staticmethod".to_string(), Value::BuiltinFunction("staticmethod".to_string(), staticmethod_builtin));
    builtins.insert("classmethod".to_string(), Value::BuiltinFunction("classmethod".to_string(), classmethod_builtin));
    builtins.insert("property".to_string(), Value::BuiltinFunction("property".to_string(), property_builtin));
    builtins.insert("dataclass".to_string(), Value::BuiltinFunction("dataclass".to_string(), dataclass_builtin));
    builtins.insert("Enum".to_string(), Value::BuiltinFunction("Enum".to_string(), enum_builtin));
    builtins.insert("open".to_string(), Value::BuiltinFunction("open".to_string(), open_builtin));
    builtins.insert("iter".to_string(), Value::BuiltinFunction("iter".to_string(), iter_builtin));
    builtins.insert("next".to_string(), Value::BuiltinFunction("next".to_string(), next_builtin));
    builtins.insert("id".to_string(), Value::BuiltinFunction("id".to_string(), id_builtin));
    builtins.insert("hash".to_string(), Value::BuiltinFunction("hash".to_string(), hash_builtin));
    builtins.insert("help".to_string(), Value::BuiltinFunction("help".to_string(), help_builtin));
    builtins.insert("dir".to_string(), Value::BuiltinFunction("dir".to_string(), dir_builtin));
    builtins.insert("vars".to_string(), Value::BuiltinFunction("vars".to_string(), vars_builtin));
    builtins.insert("locals".to_string(), Value::BuiltinFunction("locals".to_string(), locals_builtin));
    builtins.insert("globals".to_string(), Value::BuiltinFunction("globals".to_string(), globals_builtin));
    builtins.insert("eval".to_string(), Value::BuiltinFunction("eval".to_string(), eval_builtin));
    builtins.insert("exec".to_string(), Value::BuiltinFunction("exec".to_string(), exec_builtin));
    builtins.insert("compile".to_string(), Value::BuiltinFunction("compile".to_string(), compile_builtin));
    builtins.insert("repr".to_string(), Value::BuiltinFunction("repr".to_string(), repr_builtin));
    builtins.insert("ascii".to_string(), Value::BuiltinFunction("ascii".to_string(), ascii_builtin));
    builtins.insert("format".to_string(), Value::BuiltinFunction("format".to_string(), format_builtin));
    
    // Exception classes
    builtins.insert("Exception".to_string(), Value::BuiltinFunction("Exception".to_string(), exception_builtin));
    builtins.insert("ValueError".to_string(), Value::BuiltinFunction("ValueError".to_string(), value_error_builtin));
    builtins.insert("TypeError".to_string(), Value::BuiltinFunction("TypeError".to_string(), type_error_builtin));
    builtins.insert("RuntimeError".to_string(), Value::BuiltinFunction("RuntimeError".to_string(), runtime_error_builtin));
    
    builtins
}

fn print_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    let output = args.iter().map(|arg| format!("{}", arg)).collect::<Vec<_>>().join(" ");
    println!("{}", output);
    Ok(Value::None)
}

fn len_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("len() takes exactly one argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Str(s) => Ok(Value::Int(s.len() as i64)),
        Value::List(items) => Ok(Value::Int(items.len() as i64)),
        Value::Tuple(items) => Ok(Value::Int(items.len() as i64)),
        Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
        Value::Set(items) => Ok(Value::Int(items.len() as i64)),
        Value::FrozenSet(items) => Ok(Value::Int(items.len() as i64)),
        Value::Bytes(bytes) => Ok(Value::Int(bytes.len() as i64)),
        Value::ByteArray(bytes) => Ok(Value::Int(bytes.len() as i64)),
        _ => Err(anyhow::anyhow!("object of type '{}' has no len()", args[0].type_name())),
    }
}

fn str_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Str(String::new()))
    } else if args.len() == 1 {
        Ok(Value::Str(format!("{}", args[0])))
    } else {
        Err(anyhow::anyhow!("str() takes at most 1 argument ({} given)", args.len()))
    }
}

fn int_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Int(0))
    } else if args.len() == 1 {
        args[0].to_int()
    } else {
        Err(anyhow::anyhow!("int() takes at most 1 argument ({} given)", args.len()))
    }
}

fn float_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Float(0.0))
    } else if args.len() == 1 {
        args[0].to_float()
    } else {
        Err(anyhow::anyhow!("float() takes at most 1 argument ({} given)", args.len()))
    }
}

fn bool_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Bool(false))
    } else if args.len() == 1 {
        Ok(Value::Bool(args[0].is_truthy()))
    } else {
        Err(anyhow::anyhow!("bool() takes at most 1 argument ({} given)", args.len()))
    }
}

fn list_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::List(HPList::new()))
    } else if args.len() == 1 {
        args[0].to_list()
    } else {
        Err(anyhow::anyhow!("list() takes at most 1 argument ({} given)", args.len()))
    }
}

fn dict_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Dict(HashMap::new()))
    } else if args.len() == 1 {
        match &args[0] {
            Value::Dict(dict) => Ok(Value::Dict(dict.clone())),
            Value::List(items) => {
                let mut dict = HashMap::new();
                for item in items.iter() {
                    match item {
                        Value::Tuple(pair) if pair.len() == 2 => {
                            if let (Value::Str(key), value) = (&pair[0], &pair[1]) {
                                dict.insert(key.clone(), value.clone());
                            } else {
                                return Err(anyhow::anyhow!("dictionary update sequence element #0 has wrong type"));
                            }
                        }
                        _ => return Err(anyhow::anyhow!("dictionary update sequence element #0 has wrong type")),
                    }
                }
                Ok(Value::Dict(dict))
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", args[0].type_name())),
        }
    } else {
        Err(anyhow::anyhow!("dict() takes at most 1 argument ({} given)", args.len()))
    }
}

fn range_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    match args.len() {
        1 => {
            if let Value::Int(stop) = args[0] {
                Ok(Value::Range { start: 0, stop, step: 1 })
            } else {
                Err(anyhow::anyhow!("range() argument must be an integer"))
            }
        }
        2 => {
            if let (Value::Int(start), Value::Int(stop)) = (&args[0], &args[1]) {
                Ok(Value::Range { start: *start, stop: *stop, step: 1 })
            } else {
                Err(anyhow::anyhow!("range() arguments must be integers"))
            }
        }
        3 => {
            if let (Value::Int(start), Value::Int(stop), Value::Int(step)) = (&args[0], &args[1], &args[2]) {
                if *step == 0 {
                    Err(anyhow::anyhow!("range() step argument must not be zero"))
                } else {
                    Ok(Value::Range { start: *start, stop: *stop, step: *step })
                }
            } else {
                Err(anyhow::anyhow!("range() arguments must be integers"))
            }
        }
        _ => Err(anyhow::anyhow!("range() takes 1 to 3 arguments ({} given)", args.len())),
    }
}

fn input_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("input() takes at most 1 argument ({} given)", args.len()));
    }
    
    if !args.is_empty() {
        print!("{}", args[0]);
    }
    
    let mut input = String::new();
    std::io::stdin().read_line(&mut input)?;
    // Remove trailing newline
    if input.ends_with('\n') {
        input.pop();
        if input.ends_with('\r') {
            input.pop();
        }
    }
    
    Ok(Value::Str(input))
}

fn abs_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("abs() takes exactly one argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Int(n.abs())),
        Value::Float(f) => Ok(Value::Float(f.abs())),
        Value::Complex { real, imag } => {
            Ok(Value::Float((real * real + imag * imag).sqrt()))
        }
        _ => Err(anyhow::anyhow!("bad operand type for abs(): '{}'", args[0].type_name())),
    }
}

fn min_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("min() expected at least 1 argument, got 0"));
    }
    
    if args.len() == 1 {
        // Single iterable argument
        match &args[0] {
            Value::List(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("min() arg is an empty sequence"));
                }
                let mut min_val = items.get(0).unwrap().clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&min_val) == Some(std::cmp::Ordering::Less) {
                        min_val = item.clone();
                    }
                }
                Ok(min_val)
            }
            Value::Tuple(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("min() arg is an empty sequence"));
                }
                let mut min_val = items[0].clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&min_val) == Some(std::cmp::Ordering::Less) {
                        min_val = item.clone();
                    }
                }
                Ok(min_val)
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", args[0].type_name())),
        }
    } else {
        // Multiple arguments
        let mut min_val = args[0].clone();
        for arg in args.iter().skip(1) {
            if arg.partial_cmp(&min_val) == Some(std::cmp::Ordering::Less) {
                min_val = arg.clone();
            }
        }
        Ok(min_val)
    }
}

fn max_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("max() expected at least 1 argument, got 0"));
    }
    
    if args.len() == 1 {
        // Single iterable argument
        match &args[0] {
            Value::List(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("max() arg is an empty sequence"));
                }
                let mut max_val = items.get(0).unwrap().clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&max_val) == Some(std::cmp::Ordering::Greater) {
                        max_val = item.clone();
                    }
                }
                Ok(max_val)
            }
            Value::Tuple(items) => {
                if items.is_empty() {
                    return Err(anyhow::anyhow!("max() arg is an empty sequence"));
                }
                let mut max_val = items[0].clone();
                for item in items.iter().skip(1) {
                    if item.partial_cmp(&max_val) == Some(std::cmp::Ordering::Greater) {
                        max_val = item.clone();
                    }
                }
                Ok(max_val)
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", args[0].type_name())),
        }
    } else {
        // Multiple arguments
        let mut max_val = args[0].clone();
        for arg in args.iter().skip(1) {
            if arg.partial_cmp(&max_val) == Some(std::cmp::Ordering::Greater) {
                max_val = arg.clone();
            }
        }
        Ok(max_val)
    }
}

fn sum_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("sum() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    let start = if args.len() == 2 { &args[1] } else { &Value::Int(0) };
    
    let mut sum = start.clone();
    
    match iterable {
        Value::List(items) => {
            for item in items.iter() {
                sum = match (&sum, item) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Int(a), Value::Float(b)) => Value::Float(*a as f64 + b),
                    (Value::Float(a), Value::Int(b)) => Value::Float(a + *b as f64),
                    _ => return Err(anyhow::anyhow!("unsupported operand type(s) for +: '{}' and '{}'", sum.type_name(), item.type_name())),
                };
            }
            Ok(sum)
        }
        Value::Tuple(items) => {
            for item in items {
                sum = match (&sum, item) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Int(a), Value::Float(b)) => Value::Float(*a as f64 + b),
                    (Value::Float(a), Value::Int(b)) => Value::Float(a + *b as f64),
                    _ => return Err(anyhow::anyhow!("unsupported operand type(s) for +: '{}' and '{}'", sum.type_name(), item.type_name())),
                };
            }
            Ok(sum)
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn round_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("round() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let number = &args[0];
    let ndigits = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => *n,
            _ => return Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[1].type_name())),
        }
    } else {
        0
    };
    
    match number {
        Value::Int(n) => {
            if ndigits >= 0 {
                Ok(Value::Int(*n))
            } else {
                let factor = 10_i64.pow((-ndigits) as u32);
                let rounded = (*n + factor / 2) / factor * factor;
                Ok(Value::Int(rounded))
            }
        }
        Value::Float(f) => {
            if ndigits >= 0 {
                let factor = 10_f64.powi(ndigits as i32);
                Ok(Value::Float((f * factor).round() / factor))
            } else {
                let factor = 10_f64.powi((-ndigits) as i32);
                Ok(Value::Float((f / factor).round() * factor))
            }
        }
        _ => Err(anyhow::anyhow!("type {} doesn't define __round__ method", number.type_name())),
    }
}

fn pow_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("pow() takes 2 or 3 arguments ({} given)", args.len()));
    }
    
    let base = &args[0];
    let exp = &args[1];
    
    if args.len() == 3 {
        // Modular exponentiation
        let modulus = &args[2];
        match (base, exp, modulus) {
            (Value::Int(b), Value::Int(e), Value::Int(m)) => {
                if *e < 0 {
                    return Err(anyhow::anyhow!("pow() 2nd argument cannot be negative when 3rd argument specified"));
                }
                if *m == 0 {
                    return Err(anyhow::anyhow!("pow() 3rd argument cannot be 0"));
                }
                // Simple implementation for now
                Ok(Value::Int(b.pow(*e as u32) % m))
            }
            _ => Err(anyhow::anyhow!("pow() with 3 arguments requires integers")),
        }
    } else {
        // Regular exponentiation
        match (base, exp) {
            (Value::Int(b), Value::Int(e)) => {
                if *e >= 0 {
                    Ok(Value::Int(b.pow(*e as u32)))
                } else {
                    Ok(Value::Float((*b as f64).powf(*e as f64)))
                }
            }
            (Value::Float(b), Value::Float(e)) => Ok(Value::Float(b.powf(*e))),
            (Value::Int(b), Value::Float(e)) => Ok(Value::Float((*b as f64).powf(*e))),
            (Value::Float(b), Value::Int(e)) => Ok(Value::Float(b.powf(*e as f64))),
            _ => Err(anyhow::anyhow!("unsupported operand type(s) for pow()")),
        }
    }
}

fn divmod_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("divmod() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let a = &args[0];
    let b = &args[1];
    
    match (a, b) {
        (Value::Int(x), Value::Int(y)) => {
            if *y == 0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let div = x / y;
            let rem = x % y;
            Ok(Value::Tuple(vec![Value::Int(div), Value::Int(rem)]))
        }
        (Value::Float(x), Value::Float(y)) => {
            if *y == 0.0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let div = (x / y).floor();
            let rem = x - div * y;
            Ok(Value::Tuple(vec![Value::Float(div), Value::Float(rem)]))
        }
        (Value::Int(x), Value::Float(y)) => {
            if *y == 0.0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let xf = *x as f64;
            let div = (xf / y).floor();
            let rem = xf - div * y;
            Ok(Value::Tuple(vec![Value::Float(div), Value::Float(rem)]))
        }
        (Value::Float(x), Value::Int(y)) => {
            if *y == 0 {
                return Err(anyhow::anyhow!("division by zero"));
            }
            let yf = *y as f64;
            let div = (x / yf).floor();
            let rem = x - div * yf;
            Ok(Value::Tuple(vec![Value::Float(div), Value::Float(rem)]))
        }
        _ => Err(anyhow::anyhow!("unsupported operand type(s) for divmod()")),
    }
}

fn enumerate_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("enumerate() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    let start = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => *n,
            _ => return Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[1].type_name())),
        }
    } else {
        0
    };
    
    match iterable {
        Value::List(items) => {
            let mut result = Vec::new();
            for (i, item) in items.iter().enumerate() {
                result.push(Value::Tuple(vec![Value::Int(start + i as i64), item.clone()]));
            }
            Ok(Value::List(HPList::from_values(result)))
        }
        Value::Tuple(items) => {
            let mut result = Vec::new();
            for (i, item) in items.iter().enumerate() {
                result.push(Value::Tuple(vec![Value::Int(start + i as i64), item.clone()]));
            }
            Ok(Value::List(HPList::from_values(result)))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn zip_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        return Ok(Value::List(HPList::new()));
    }
    
    // Convert all arguments to vectors
    let mut iterables = Vec::new();
    for arg in &args {
        match arg {
            Value::List(items) => iterables.push(items.as_vec().clone()),
            Value::Tuple(items) => iterables.push(items.clone()),
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", arg.type_name())),
        }
    }
    
    // Zip the iterables
    let min_len = iterables.iter().map(|v| v.len()).min().unwrap_or(0);
    let mut result = Vec::new();
    
    for i in 0..min_len {
        let mut tuple = Vec::new();
        for iterable in &iterables {
            tuple.push(iterable[i].clone());
        }
        result.push(Value::Tuple(tuple));
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

fn map_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("map() must have at least two arguments"));
    }
    
    let func = &args[0];
    let iterables: Vec<&Value> = args.iter().skip(1).collect();
    
    // Convert iterables to vectors
    let mut iterable_vecs = Vec::new();
    for iterable in iterables {
        match iterable {
            Value::List(items) => iterable_vecs.push(items.as_vec().clone()),
            Value::Tuple(items) => iterable_vecs.push(items.clone()),
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
        }
    }
    
    // Map the function over the iterables
    let min_len = iterable_vecs.iter().map(|v| v.len()).min().unwrap_or(0);
    let mut result = Vec::new();
    
    for i in 0..min_len {
        let func_args: Vec<Value> = iterable_vecs.iter().map(|v| v[i].clone()).collect();
        
        // Call the function with the arguments
        let func_result = match func {
            Value::BuiltinFunction(_, f) => f(func_args)?,
            Value::NativeFunction(f) => f(func_args)?,
            _ => return Err(anyhow::anyhow!("'{}' object is not callable", func.type_name())),
        };
        
        result.push(func_result);
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

fn filter_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("filter() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let func = &args[0];
    let iterable = &args[1];
    
    // Convert iterable to vector
    let items = match iterable {
        Value::List(items) => items.as_vec().clone(),
        Value::Tuple(items) => items.clone(),
        _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    };
    
    // Filter the items
    let mut result = Vec::new();
    
    for item in items {
        // Call the function with the item
        let func_result = match func {
            Value::BuiltinFunction(_, f) => f(vec![item.clone()])?,
            Value::NativeFunction(f) => f(vec![item.clone()])?,
            Value::None => item.clone(), // None means use truthiness
            _ => return Err(anyhow::anyhow!("'{}' object is not callable", func.type_name())),
        };
        
        // If the result is truthy, include the item
        if func_result.is_truthy() {
            result.push(item);
        }
    }
    
    Ok(Value::List(HPList::from_values(result)))
}

fn sorted_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 1 {
        return Err(anyhow::anyhow!("sorted() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            let mut sorted_items = items.as_vec().clone();
            sorted_items.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
            Ok(Value::List(HPList::from_values(sorted_items)))
        }
        Value::Tuple(items) => {
            let mut sorted_items = items.clone();
            sorted_items.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
            Ok(Value::List(HPList::from_values(sorted_items)))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn reversed_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("reversed() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            let mut reversed_items = items.as_vec().clone();
            reversed_items.reverse();
            Ok(Value::List(HPList::from_values(reversed_items)))
        }
        Value::Tuple(items) => {
            let mut reversed_items = items.clone();
            reversed_items.reverse();
            Ok(Value::List(HPList::from_values(reversed_items)))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not reversible", iterable.type_name())),
    }
}

fn any_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("any() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            for item in items.iter() {
                if item.is_truthy() {
                    return Ok(Value::Bool(true));
                }
            }
            Ok(Value::Bool(false))
        }
        Value::Tuple(items) => {
            for item in items {
                if item.is_truthy() {
                    return Ok(Value::Bool(true));
                }
            }
            Ok(Value::Bool(false))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn all_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("all() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let iterable = &args[0];
    
    match iterable {
        Value::List(items) => {
            for item in items.iter() {
                if !item.is_truthy() {
                    return Ok(Value::Bool(false));
                }
            }
            Ok(Value::Bool(true))
        }
        Value::Tuple(items) => {
            for item in items {
                if !item.is_truthy() {
                    return Ok(Value::Bool(false));
                }
            }
            Ok(Value::Bool(true))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", iterable.type_name())),
    }
}

fn chr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("chr() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => {
            if *n < 0 || *n > 0x10FFFF {
                return Err(anyhow::anyhow!("chr() arg not in range(0x110000)"));
            }
            Ok(Value::Str(String::from_utf8(vec![*n as u8]).unwrap_or_else(|_| String::from("\u{FFFD}"))))
        }
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn ord_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ord() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Str(s) => {
            if s.chars().count() != 1 {
                return Err(anyhow::anyhow!("ord() expected a character, but string of length {} found", s.chars().count()));
            }
            Ok(Value::Int(s.chars().next().unwrap() as i64))
        }
        _ => Err(anyhow::anyhow!("ord() expected string of length 1, but {} found", args[0].type_name())),
    }
}

fn hex_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("hex() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Str(format!("0x{:x}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn bin_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("bin() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Str(format!("0b{:b}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn oct_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("oct() takes exactly 1 argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Int(n) => Ok(Value::Str(format!("0o{:o}", n))),
        _ => Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name())),
    }
}

fn isinstance_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("isinstance() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let type_info = &args[1];
    
    match type_info {
        Value::Str(type_name) => Ok(Value::Bool(obj.type_name() == type_name)),
        _ => Err(anyhow::anyhow!("isinstance() arg 2 must be a type or tuple of types")),
    }
}

fn type_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("type() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let obj = &args[0];
    
    // Return the type name as a string
    match obj {
        Value::Object { class_name, .. } => {
            Ok(Value::Str(class_name.clone()))
        },
        Value::Class { name, .. } => {
            Ok(Value::Str(name.clone()))
        },
        _ => {
            Ok(Value::Str(obj.type_name().to_string()))
        }
    }
}

fn callable_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("callable() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let is_callable = match obj {
        Value::BuiltinFunction(_, _) | Value::NativeFunction(_) | Value::Closure { .. } => true,
        _ => false,
    };
    
    Ok(Value::Bool(is_callable))
}

fn hasattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("hasattr() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("hasattr() attribute name must be string")),
    };
    
    // For now, we'll just check if the object has a method with this name
    let has_attr = obj.get_method(attr_name).is_some();
    Ok(Value::Bool(has_attr))
}

fn getattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("getattr() takes 2 or 3 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("getattr() attribute name must be string")),
    };
    
    // Try to get the attribute from the object
    match obj {
        Value::Object { fields, class_methods, .. } => {
            // First check fields
            if let Some(value) = fields.get(attr_name) {
                Ok(value.clone())
            }
            // Then check methods
            else if let Some(method) = class_methods.get(attr_name) {
                Ok(method.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Class { methods, .. } => {
            // Check class methods
            if let Some(method) = methods.get(attr_name) {
                Ok(method.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Module(_, namespace) => {
            // Check module attributes
            if let Some(value) = namespace.get(attr_name) {
                Ok(value.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Dict(dict) => {
            // For dictionaries, treat keys as attributes
            if let Some(value) = dict.get(attr_name) {
                Ok(value.clone())
            }
            // If not found and default provided, return default
            else if args.len() == 3 {
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        _ => {
            // For other objects, try to get method
            if let Some(method) = obj.get_method(attr_name) {
                Ok(method)
            } else if args.len() == 3 {
                // Return default value
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        }
    }
}

fn setattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 3 {
        return Err(anyhow::anyhow!("setattr() takes exactly 3 arguments ({} given)", args.len()));
    }
    
    let obj = &mut args[0].clone(); // We'll need to make this mutable in a full implementation
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("setattr() attribute name must be string")),
    };
    let value = &args[2];
    
    // For now, we'll just return None as setting attributes requires mutable references
    // In a full implementation, we would modify the object's fields
    Ok(Value::None)
}

fn delattr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("delattr() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as deleting attributes is complex
    Ok(Value::None)
}

fn issubclass_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("issubclass() takes exactly 2 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return False as subclass checking is complex
    Ok(Value::Bool(false))
}

fn super_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 2 {
        return Err(anyhow::anyhow!("super() takes at most 2 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as super() is complex
    Ok(Value::None)
}

fn staticmethod_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("staticmethod() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return the function as-is
    Ok(args[0].clone())
}

fn classmethod_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("classmethod() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return the function as-is
    Ok(args[0].clone())
}

fn dataclass_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("dataclass() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return the class as-is
    // In a full implementation, we would add __init__, __repr__, __eq__, etc. methods
    Ok(args[0].clone())
}

fn enum_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("Enum() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return the class as-is
    // In a full implementation, we would process the class to create enum members
    Ok(args[0].clone())
}

fn property_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 4 {
        return Err(anyhow::anyhow!("property() takes at most 4 arguments ({} given)", args.len()));
    }
    
    // Create a property object that stores the getter, setter, and deleter functions
    let getter = if !args.is_empty() { Some(Box::new(args[0].clone())) } else { None };
    let setter = if args.len() > 1 { Some(Box::new(args[1].clone())) } else { None };
    let deleter = if args.len() > 2 { Some(Box::new(args[2].clone())) } else { None };
    let doc = if args.len() > 3 { 
        match &args[3] {
            Value::Str(s) => Some(s.clone()),
            _ => None
        }
    } else { 
        None 
    };
    
    // For now, we'll just return a special Property value that contains the functions
    // In a full implementation, this would be a proper property object
    Ok(Value::Object {
        class_name: "property".to_string(),
        fields: {
            let mut fields = HashMap::new();
            if let Some(getter) = getter {
                fields.insert("fget".to_string(), *getter);
            }
            if let Some(setter) = setter {
                fields.insert("fset".to_string(), *setter);
            }
            if let Some(deleter) = deleter {
                fields.insert("fdel".to_string(), *deleter);
            }
            if let Some(doc) = doc {
                fields.insert("__doc__".to_string(), Value::Str(doc));
            }
            fields
        },
        class_methods: HashMap::new(),
        base_object: BaseObject::new("property".to_string(), vec!["object".to_string()]),
        mro: MRO::from_linearization(vec!["property".to_string(), "object".to_string()]),
    })
}

fn open_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("open() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as file operations are complex
    Ok(Value::None)
}

fn iter_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("iter() takes exactly 1 argument ({} given)", args.len()));
    }
    
    let obj = &args[0];
    
    match obj {
        Value::List(items) => {
            // Create an Iterator object for the list
            Ok(Value::Iterator {
                items: items.as_vec().clone(),
                current_index: 0,
            })
        },
        Value::Tuple(items) => {
            // Create an Iterator object for the tuple
            Ok(Value::Iterator {
                items: items.clone(),
                current_index: 0,
            })
        },
        Value::Str(s) => {
            // Create an Iterator object for the string (character by character)
            let items: Vec<Value> = s.chars().map(|c| Value::Str(c.to_string())).collect();
            Ok(Value::Iterator {
                items,
                current_index: 0,
            })
        },
        Value::Dict(dict) => {
            // Create an Iterator object for the dict (keys)
            let items: Vec<Value> = dict.keys().map(|k| Value::Str(k.clone())).collect();
            Ok(Value::Iterator {
                items,
                current_index: 0,
            })
        },
        Value::Set(items) => {
            // Create an Iterator object for the set
            Ok(Value::Iterator {
                items: items.clone(),
                current_index: 0,
            })
        },
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", obj.type_name())),
    }
}

fn next_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() < 1 || args.len() > 2 {
        return Err(anyhow::anyhow!("next() takes at least 1 and at most 2 arguments ({} given)", args.len()));
    }
    
    // Get the iterator
    let iterator = &args[0];
    
    // For generators, we would resume execution and get the next value
    match iterator {
        Value::Generator { code, frame, finished } => {
            // For generators, we need to resume execution
            if *finished {
                // Generator is finished, raise StopIteration
                return Err(anyhow::anyhow!("StopIteration"));
            }
            
            // Create a VM instance to execute the generator
            // For now, we'll just return None as proper generator implementation is complex
            // In a full implementation, we would resume the generator execution
            Ok(Value::None)
        },
        Value::Iterator { ref items, ref current_index } => {
            // For Iterator objects, check if we've reached the end
            if *current_index < items.len() {
                // Return the current item
                Ok(items[*current_index].clone())
            } else {
                // Iterator exhausted, raise StopIteration
                Err(anyhow::anyhow!("StopIteration"))
            }
        },
        _ => {
            // For other iterators, return None for now
            Ok(Value::None)
        }
    }
}

fn id_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("id() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // Return a unique ID based on the object's memory address or hash
    // For now, we'll use a simple hash-based approach
    let obj = &args[0];
    use std::collections::hash_map::DefaultHasher;
    use std::hash::{Hash, Hasher};
    
    let mut hasher = DefaultHasher::new();
    obj.hash(&mut hasher);
    let id = hasher.finish() as i64;
    
    Ok(Value::Int(id))
}

fn hash_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("hash() takes exactly 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return a dummy hash
    Ok(Value::Int(0))
}

fn help_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("help() takes at most 1 argument ({} given)", args.len()));
    }
    
    println!("Tauraro help system - for more information, visit the documentation");
    Ok(Value::None)
}

fn dir_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("dir() takes at most 1 argument ({} given)", args.len()));
    }
    
    if args.is_empty() {
        // dir() without arguments - return names in current local scope
        // For now, we'll just return an empty list
        Ok(Value::List(HPList::new()))
    } else {
        // dir(obj) - return attributes of the object
        let obj = &args[0];
        let mut attrs = Vec::new();
        
        // Add built-in attributes based on object type
        match obj {
            Value::Object { fields, class_methods, .. } => {
                // Add field names
                for field_name in fields.keys() {
                    attrs.push(Value::Str(field_name.clone()));
                }
                // Add method names
                for method_name in class_methods.keys() {
                    attrs.push(Value::Str(method_name.clone()));
                }
            },
            Value::Class { methods, .. } => {
                // Add method names
                for method_name in methods.keys() {
                    attrs.push(Value::Str(method_name.clone()));
                }
            },
            Value::Module(_, namespace) => {
                // Add module attributes
                for attr_name in namespace.keys() {
                    attrs.push(Value::Str(attr_name.clone()));
                }
            },
            Value::Dict(dict) => {
                // Add dictionary keys
                for key in dict.keys() {
                    attrs.push(Value::Str(key.clone()));
                }
            },
            _ => {
                // For other types, we could add dunder methods
                // For now, we'll just return an empty list
            }
        }
        
        // Sort the attributes
        attrs.sort_by(|a, b| {
            if let (Value::Str(a_str), Value::Str(b_str)) = (a, b) {
                a_str.cmp(b_str)
            } else {
                std::cmp::Ordering::Equal
            }
        });
        
        Ok(Value::List(HPList::from_values(attrs)))
    }
}

fn vars_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("vars() takes at most 1 argument ({} given)", args.len()));
    }
    
    // For now, we'll just return an empty dict
    Ok(Value::Dict(HashMap::new()))
}

fn locals_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if !args.is_empty() {
        return Err(anyhow::anyhow!("locals() takes no arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return an empty dict
    Ok(Value::Dict(HashMap::new()))
}

fn globals_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if !args.is_empty() {
        return Err(anyhow::anyhow!("globals() takes no arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return an empty dict
    Ok(Value::Dict(HashMap::new()))
}

fn eval_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("eval() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as eval is complex
    Ok(Value::None)
}

fn exec_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("exec() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as exec is complex
    Ok(Value::None)
}

fn compile_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 3 {
        return Err(anyhow::anyhow!("compile() takes at most 3 arguments ({} given)", args.len()));
    }
    
    // For now, we'll just return None as compile is complex
    Ok(Value::None)
}

fn repr_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("repr() takes exactly 1 argument ({} given)", args.len()));
    }
    
    Ok(Value::Str(format!("{:?}", args[0])))
}

fn ascii_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ascii() takes exactly 1 argument ({} given)", args.len()));
    }
    
    Ok(Value::Str(format!("{:?}", args[0])))
}

fn format_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("format() takes at most 2 arguments ({} given)", args.len()));
    }
    
    let obj = &args[0];
    Ok(Value::Str(format!("{}", obj)))
}

fn exception_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Exception {
            class_name: "Exception".to_string(),
            message: "".to_string(),
            traceback: None,
        })
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::Exception {
            class_name: "Exception".to_string(),
            message,
            traceback: None,
        })
    }
}

fn value_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Exception {
            class_name: "ValueError".to_string(),
            message: "".to_string(),
            traceback: None,
        })
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::Exception {
            class_name: "ValueError".to_string(),
            message,
            traceback: None,
        })
    }
}

fn type_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Exception {
            class_name: "TypeError".to_string(),
            message: "".to_string(),
            traceback: None,
        })
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::Exception {
            class_name: "TypeError".to_string(),
            message,
            traceback: None,
        })
    }
}

fn runtime_error_builtin(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.is_empty() {
        Ok(Value::Exception {
            class_name: "RuntimeError".to_string(),
            message: "".to_string(),
            traceback: None,
        })
    } else {
        let message = match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{}", args[0]),
        };
        Ok(Value::Exception {
            class_name: "RuntimeError".to_string(),
            message,
            traceback: None,
        })
    }
}