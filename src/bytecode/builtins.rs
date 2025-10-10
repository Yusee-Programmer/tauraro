//! Builtin functions implementation


use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::vm::SuperBytecodeVM;
use crate::bytecode::objects::RcValue;
use anyhow::{Result, anyhow};
use std::collections::HashMap;

impl SuperBytecodeVM {
    // Builtin functions (associated functions, not methods)
    pub fn builtin_print(args: Vec<Value>) -> Result<Value> {
        let output: Vec<String> = args.iter().map(|v| v.to_string()).collect();
        println!("{}", output.join(" "));
        Ok(Value::None)
    }
    
    pub fn builtin_len(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("len() takes exactly one argument"));
        }
        
        let length = match &args[0] {
            Value::List(list) => list.len(),
            Value::Tuple(tuple) => tuple.len(),
            Value::Dict(dict) => dict.len(),
            Value::Str(s) => s.len(),
            _ => return Err(anyhow!("len() unsupported for type")),
        };
        
        Ok(Value::Int(length as i64))
    }
    
    pub fn builtin_range(args: Vec<Value>) -> Result<Value> {
        let mut start: i64 = 0;
        let stop: i64;
        let mut step: i64 = 1;
        
        match args.len() {
            1 => {
                stop = match &args[0] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
            }
            2 => {
                start = match &args[0] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                stop = match &args[1] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
            }
            3 => {
                start = match &args[0] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                stop = match &args[1] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                let step_val = match &args[2] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                if step_val == 0 {
                    return Err(anyhow!("range() step cannot be zero"));
                }
                step = step_val;
            }
            _ => return Err(anyhow!("range() takes 1-3 arguments")),
        }
        
        // Return an unboxed Range value to enable fast iteration in ForIter
        Ok(Value::Range { start, stop, step })
    }
    
    pub fn builtin_str(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("str() takes exactly one argument"));
        }
        
        let string_repr = match &args[0] {
            Value::Str(s) => s.clone(), // Don't add quotes for str() conversion
            _ => format!("{}", args[0]), // Use Display trait
        };
        Ok(Value::Str(string_repr))
    }
    
    pub fn builtin_int(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("int() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(*n)),
            Value::Float(f) => Ok(Value::Int(*f as i64)),
            Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
            Value::Str(s) => {
                s.trim().parse::<i64>()
                    .map(Value::Int)
                    .map_err(|_| anyhow!("invalid literal for int() with base 10: '{}'", s))
            }
            _ => Err(anyhow!("int() argument must be a string, a bytes-like object or a number, not '{}'", args[0].type_name())),
        }
    }
    
    pub fn builtin_float(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("float() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Float(f) => Ok(Value::Float(*f)),
            Value::Int(n) => Ok(Value::Float(*n as f64)),
            Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
            Value::Str(s) => {
                s.trim().parse::<f64>()
                    .map(Value::Float)
                    .map_err(|_| anyhow!("could not convert string to float: '{}'", s))
            }
            _ => Err(anyhow!("float() argument must be a string or a number, not '{}'", args[0].type_name())),
        }
    }
    
    pub fn builtin_bool(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("bool() takes exactly one argument"));
        }
        
        Ok(Value::Bool(args[0].is_truthy()))
    }
    
    pub fn builtin_list(args: Vec<Value>) -> Result<Value> {
        if args.len() > 1 {
            return Err(anyhow!("list() takes at most 1 argument"));
        }
        
        if args.is_empty() {
            Ok(Value::List(HPList::new()))
        } else {
            args[0].to_list()
        }
    }
    
    pub fn builtin_sum(args: Vec<Value>) -> Result<Value> {
        if args.len() < 1 || args.len() > 2 {
            return Err(anyhow!("sum() takes at least 1 argument and at most 2 arguments"));
        }
        
        let iterable = &args[0];
        let start = if args.len() == 2 { &args[1] } else { &Value::Int(0) };
        
        match iterable {
            Value::List(items) => {
                let mut sum = start.clone();
                for item in items.as_vec() {
                    sum = match (&sum, item) {
                        (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                        (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                        (Value::Int(a), Value::Float(b)) => Value::Float(*a as f64 + b),
                        (Value::Float(a), Value::Int(b)) => Value::Float(a + *b as f64),
                        _ => return Err(anyhow!("unsupported operand type(s) for +")),
                    };
                }
                Ok(sum)
            }
            _ => Err(anyhow!("'{}' object is not iterable", iterable.type_name())),
        }
    }
    
    pub fn builtin_abs(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("abs() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(n.abs())),
            Value::Float(f) => Ok(Value::Float(f.abs())),
            _ => Err(anyhow!("bad operand type for abs(): '{}'", args[0].type_name())),
        }
    }
    
    pub fn builtin_chr(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("chr() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => {
                if *n < 0 || *n > 0x10FFFF {
                    return Err(anyhow!("chr() arg not in range(0x110000)"));
                }
                Ok(Value::Str(String::from_utf8(vec![*n as u8]).unwrap_or_else(|_| String::from("\u{FFFD}"))))
            }
            _ => Err(anyhow!("chr() requires an integer argument")),
        }
    }
    
    pub fn builtin_isinstance(args: Vec<Value>) -> Result<Value> {
        if args.len() != 2 {
            return Err(anyhow!("isinstance() takes exactly 2 arguments"));
        }
        
        let obj = &args[0];
        let class_info = &args[1];
        
        // Simple isinstance implementation - for full implementation we'd need TypeHierarchy
        let result = match class_info {
            Value::Str(type_name) => obj.type_name() == type_name,
            _ => false, // For now, we only support string type names
        };
        
        Ok(Value::Bool(result))
    }
    
    pub fn builtin_type(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("type() takes exactly one argument"));
        }

        Ok(Value::Str(args[0].type_name().to_string()))
    }

    pub fn builtin_dir(_args: Vec<Value>) -> Result<Value> {
        // Return common builtin names
        // In a full implementation, this would inspect the current namespace
        let mut items = vec![
            "__name__", "__builtins__", "builtins",
            "print", "len", "range", "str", "int", "float", "bool",
            "list", "dict", "tuple", "set",
            "sum", "abs", "chr", "type", "isinstance",
            "dir", "globals", "locals", "vars", "help",
        ];
        items.sort();

        let values: Vec<Value> = items.iter().map(|s| Value::Str(s.to_string())).collect();
        Ok(Value::List(crate::modules::hplist::HPList::from_values(values)))
    }

    pub fn builtin_globals(_args: Vec<Value>) -> Result<Value> {
        // Return a simple dict representation
        // In a full implementation, this would return actual globals from the current frame
        let mut dict = HashMap::new();
        dict.insert("__name__".to_string(), Value::Str("__main__".to_string()));
        dict.insert("args".to_string(), Value::List(crate::modules::hplist::HPList::new()));
        Ok(Value::Dict(dict))
    }

    pub fn builtin_locals(_args: Vec<Value>) -> Result<Value> {
        // Return a simple dict representation
        // In a full implementation, this would return actual locals from the current frame
        let dict = HashMap::new();
        Ok(Value::Dict(dict))
    }

    pub fn builtin_vars(_args: Vec<Value>) -> Result<Value> {
        // vars() without arguments is equivalent to locals()
        if _args.is_empty() {
            return Err(anyhow!("vars() requires an argument"));
        }
        // With an argument, return the object's __dict__
        Ok(Value::Dict(HashMap::new()))
    }

    pub fn builtin_help(_args: Vec<Value>) -> Result<Value> {
        // Simple help message
        println!("\nWelcome to Tauraro!\n");
        println!("Tauraro is a Python-compatible programming language with Rust-like performance.\n");
        println!("Type help() for interactive help, or help(object) for help about object.\n");
        println!("Quick Reference:");
        println!("  Variables:    x = 10");
        println!("  Functions:    def greet(name): return f'Hello, {{name}}'");
        println!("  Classes:      class MyClass: pass");
        println!("  Loops:        for i in range(10): print(i)");
        println!("  Conditions:   if x > 5: print('big')\n");
        println!("  Import:       import math\n");
        println!("Built-in Functions:");
        println!("  print()       Print values to stdout");
        println!("  input()       Read input from stdin");
        println!("  len()         Get length of sequence");
        println!("  range()       Generate range of numbers");
        println!("  type()        Get type of object");
        println!("  dir()         List attributes");
        println!("  help()        Show this help");
        println!("  exit()        Exit the REPL\n");
        Ok(Value::None)
    }
}