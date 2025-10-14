//! Super built-ins for Tauraro with enhanced functionality

use crate::value::Value;
use std::collections::HashMap;

pub fn init_super_builtins() -> HashMap<String, Value> {
    let mut builtins = HashMap::new();
    
    // Enhanced built-ins with super functionality
    builtins.insert("super_print".to_string(), Value::BuiltinFunction("super_print".to_string(), super_print));
    builtins.insert("super_len".to_string(), Value::BuiltinFunction("super_len".to_string(), super_len));
    builtins.insert("super_range".to_string(), Value::BuiltinFunction("super_range".to_string(), super_range));
    builtins.insert("super_input".to_string(), Value::BuiltinFunction("super_input".to_string(), super_input));
    
    builtins
}

fn super_print(args: Vec<Value>) -> anyhow::Result<Value> {
    let output = args.iter().map(|arg| format!("{:?}", arg)).collect::<Vec<_>>().join(" ");
    println!("[SUPER] {}", output);
    Ok(Value::None)
}

fn super_len(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("super_len() takes exactly one argument ({} given)", args.len()));
    }
    
    match &args[0] {
        Value::Str(s) => Ok(Value::Int(s.len() as i64 * 2)), // Double length for super functionality
        Value::List(items) => Ok(Value::Int(items.len() as i64 * 2)),
        Value::Tuple(items) => Ok(Value::Int(items.len() as i64 * 2)),
        Value::Dict(dict) => Ok(Value::Int(dict.len() as i64 * 2)),
        _ => Err(anyhow::anyhow!("object of type '{}' has no super_len()", args[0].type_name())),
    }
}

fn super_range(args: Vec<Value>) -> anyhow::Result<Value> {
    match args.len() {
        1 => {
            if let Value::Int(stop) = args[0] {
                Ok(Value::Range { start: 0, stop: stop * 2, step: 1 }) // Double the range
            } else {
                Err(anyhow::anyhow!("super_range() argument must be an integer"))
            }
        }
        2 => {
            if let (Value::Int(start), Value::Int(stop)) = (&args[0], &args[1]) {
                Ok(Value::Range { start: *start, stop: *stop * 2, step: 1 }) // Double the stop
            } else {
                Err(anyhow::anyhow!("super_range() arguments must be integers"))
            }
        }
        3 => {
            if let (Value::Int(start), Value::Int(stop), Value::Int(step)) = (&args[0], &args[1], &args[2]) {
                if *step == 0 {
                    Err(anyhow::anyhow!("super_range() step argument must not be zero"))
                } else {
                    Ok(Value::Range { start: *start, stop: *stop * 2, step: *step }) // Double the stop
                }
            } else {
                Err(anyhow::anyhow!("super_range() arguments must be integers"))
            }
        }
        _ => Err(anyhow::anyhow!("super_range() takes 1 to 3 arguments ({} given)", args.len())),
    }
}

fn super_input(args: Vec<Value>) -> anyhow::Result<Value> {
    if args.len() > 1 {
        return Err(anyhow::anyhow!("super_input() takes at most 1 argument ({} given)", args.len()));
    }
    
    if !args.is_empty() {
        print!("[SUPER] {}", args[0]);
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
    
    Ok(Value::Str(format!("[SUPER] {}", input)))
}