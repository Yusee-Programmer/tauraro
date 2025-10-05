//! Built-in functions & constants
use crate::value::Value;
use crate::modules::hplist::HPList;
use anyhow::Result;
use std::collections::HashMap;

/// Built-in functions and constants
pub struct Builtins;

impl Builtins {
    /// Initialize built-in functions and constants
    pub fn init_builtins() -> HashMap<String, Value> {
        let mut builtins = HashMap::new();
        
        // Basic built-in functions
        builtins.insert("print".to_string(), Value::BuiltinFunction("print".to_string(), Self::builtin_print));
        builtins.insert("len".to_string(), Value::BuiltinFunction("len".to_string(), Self::builtin_len));
        builtins.insert("range".to_string(), Value::BuiltinFunction("range".to_string(), Self::builtin_range));
        builtins.insert("str".to_string(), Value::BuiltinFunction("str".to_string(), Self::builtin_str));
        builtins.insert("int".to_string(), Value::BuiltinFunction("int".to_string(), Self::builtin_int));
        builtins.insert("float".to_string(), Value::BuiltinFunction("float".to_string(), Self::builtin_float));
        builtins.insert("bool".to_string(), Value::BuiltinFunction("bool".to_string(), Self::builtin_bool));
        builtins.insert("list".to_string(), Value::BuiltinFunction("list".to_string(), Self::builtin_list));
        builtins.insert("sum".to_string(), Value::BuiltinFunction("sum".to_string(), Self::builtin_sum));
        builtins.insert("abs".to_string(), Value::BuiltinFunction("abs".to_string(), Self::builtin_abs));
        builtins.insert("chr".to_string(), Value::BuiltinFunction("chr".to_string(), Self::builtin_chr));
        builtins.insert("isinstance".to_string(), Value::BuiltinFunction("isinstance".to_string(), Self::builtin_isinstance));
        builtins.insert("type".to_string(), Value::BuiltinFunction("type".to_string(), Self::builtin_type));
        
        // Constants
        builtins.insert("None".to_string(), Value::None);
        builtins.insert("True".to_string(), Value::Bool(true));
        builtins.insert("False".to_string(), Value::Bool(false));
        
        builtins
    }
    
    /// Built-in print function
    pub fn builtin_print(args: Vec<Value>) -> Result<Value> {
        let output = args.iter()
            .map(|arg| format!("{}", arg))
            .collect::<Vec<_>>()
            .join(" ");
        println!("{}", output);
        Ok(Value::None)
    }
    
    /// Built-in len function
    pub fn builtin_len(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("len() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Str(s) => Ok(Value::Int(s.len() as i64)),
            Value::List(l) => Ok(Value::Int(l.len() as i64)),
            Value::Dict(d) => Ok(Value::Int(d.len() as i64)),
            _ => Err(anyhow::anyhow!("len() argument must be a string, list, or dict")),
        }
    }
    
    /// Built-in range function
    pub fn builtin_range(args: Vec<Value>) -> Result<Value> {
        match args.len() {
            1 => {
                if let Value::Int(end) = args[0] {
                    let values: Vec<Value> = (0..end).map(|i| Value::Int(i)).collect();
                    Ok(Value::List(HPList::from_values(values)))
                } else {
                    Err(anyhow::anyhow!("range() argument must be an integer"))
                }
            }
            2 => {
                if let (Value::Int(start), Value::Int(end)) = (&args[0], &args[1]) {
                    let values: Vec<Value> = (*start..*end).map(|i| Value::Int(i)).collect();
                    Ok(Value::List(HPList::from_values(values)))
                } else {
                    Err(anyhow::anyhow!("range() arguments must be integers"))
                }
            }
            _ => Err(anyhow::anyhow!("range() takes 1 or 2 arguments")),
        }
    }
    
    /// Built-in str function
    pub fn builtin_str(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("str() takes exactly one argument"));
        }
        
        Ok(Value::Str(format!("{}", args[0])))
    }
    
    /// Built-in int function
    pub fn builtin_int(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("int() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(i) => Ok(Value::Int(*i)),
            Value::Float(f) => Ok(Value::Int(*f as i64)),
            Value::Str(s) => {
                match s.parse::<i64>() {
                    Ok(i) => Ok(Value::Int(i)),
                    Err(_) => Err(anyhow::anyhow!("invalid literal for int()")),
                }
            }
            _ => Err(anyhow::anyhow!("int() argument must be a string or number")),
        }
    }
    
    /// Built-in float function
    pub fn builtin_float(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("float() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(i) => Ok(Value::Float(*i as f64)),
            Value::Float(f) => Ok(Value::Float(*f)),
            Value::Str(s) => {
                match s.parse::<f64>() {
                    Ok(f) => Ok(Value::Float(f)),
                    Err(_) => Err(anyhow::anyhow!("invalid literal for float()")),
                }
            }
            _ => Err(anyhow::anyhow!("float() argument must be a string or number")),
        }
    }
    
    /// Built-in bool function
    pub fn builtin_bool(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("bool() takes exactly one argument"));
        }
        
        Ok(Value::Bool(args[0].is_truthy()))
    }
    
    /// Built-in list function
    pub fn builtin_list(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("list() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::List(l) => Ok(Value::List(l.clone())),
            _ => Err(anyhow::anyhow!("list() argument must be iterable")),
        }
    }
    
    /// Built-in sum function
    pub fn builtin_sum(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("sum() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::List(l) => {
                let mut sum = 0i64;
                for value in l.iter() {
                    if let Value::Int(i) = value {
                        sum += *i;
                    } else {
                        return Err(anyhow::anyhow!("sum() argument must be a list of integers"));
                    }
                }
                Ok(Value::Int(sum))
            }
            _ => Err(anyhow::anyhow!("sum() argument must be a list")),
        }
    }
    
    /// Built-in abs function
    pub fn builtin_abs(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("abs() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(i) => Ok(Value::Int(i.abs())),
            Value::Float(f) => Ok(Value::Float(f.abs())),
            _ => Err(anyhow::anyhow!("abs() argument must be a number")),
        }
    }
    
    /// Built-in chr function
    pub fn builtin_chr(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("chr() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(i) => {
                if *i >= 0 && *i <= 0x10FFFF {
                    match char::from_u32(*i as u32) {
                        Some(c) => Ok(Value::Str(c.to_string())),
                        None => Ok(Value::Str('\u{FFFD}'.to_string())),
                    }
                } else {
                    Err(anyhow::anyhow!("chr() argument out of range"))
                }
            }
            _ => Err(anyhow::anyhow!("chr() argument must be an integer")),
        }
    }
    
    /// Built-in isinstance function
    pub fn builtin_isinstance(args: Vec<Value>) -> Result<Value> {
        if args.len() != 2 {
            return Err(anyhow::anyhow!("isinstance() takes exactly two arguments"));
        }
        
        let obj_type = args[0].type_name();
        let result = match &args[1] {
            Value::Str(s) => obj_type == s,
            _ => false,
        };
        
        Ok(Value::Bool(result))
    }
    
    /// Built-in type function
    pub fn builtin_type(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("type() takes exactly one argument"));
        }
        
        Ok(Value::Str(args[0].type_name().to_string()))
    }
}