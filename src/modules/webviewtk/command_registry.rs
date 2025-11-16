// Command Registry for exposing Tauraro functions to frontend

use crate::value::Value;
use anyhow::Result;
use serde_json;
use std::collections::HashMap;
use std::sync::{Arc, Mutex};

/// A command that can be invoked from the frontend
pub type CommandHandler = Arc<dyn Fn(serde_json::Value) -> Result<serde_json::Value> + Send + Sync>;

/// Registry of commands that can be called from frontend
pub struct CommandRegistry {
    commands: Arc<Mutex<HashMap<String, CommandHandler>>>,
}

impl CommandRegistry {
    pub fn new() -> Self {
        Self {
            commands: Arc::new(Mutex::new(HashMap::new())),
        }
    }

    /// Register a command handler
    pub fn register<F>(&self, name: &str, handler: F)
    where
        F: Fn(serde_json::Value) -> Result<serde_json::Value> + Send + Sync + 'static,
    {
        let mut commands = self.commands.lock().unwrap();
        commands.insert(name.to_string(), Arc::new(handler));
    }

    /// Register a Tauraro function as a command
    pub fn register_tauraro_function<F>(&self, name: &str, func: F)
    where
        F: Fn(Vec<Value>) -> Result<Value> + Send + Sync + 'static,
    {
        let handler = move |args: serde_json::Value| -> Result<serde_json::Value> {
            // Convert JSON args to Tauraro Values
            let tauraro_args = json_to_tauraro_values(&args)?;
            
            // Call the Tauraro function
            let result = func(tauraro_args)?;
            
            // Convert result back to JSON
            tauraro_value_to_json(&result)
        };
        
        self.register(name, handler);
    }

    /// Invoke a command by name
    pub fn invoke(&self, name: &str, args: serde_json::Value) -> Result<serde_json::Value> {
        let commands = self.commands.lock().unwrap();
        
        match commands.get(name) {
            Some(handler) => handler(args),
            None => Err(anyhow::anyhow!("Command '{}' not found", name)),
        }
    }

    /// Get list of registered command names
    pub fn list_commands(&self) -> Vec<String> {
        let commands = self.commands.lock().unwrap();
        commands.keys().cloned().collect()
    }
}

impl Clone for CommandRegistry {
    fn clone(&self) -> Self {
        Self {
            commands: Arc::clone(&self.commands),
        }
    }
}

/// Convert JSON value to Tauraro Value
pub fn json_to_tauraro_values(json: &serde_json::Value) -> Result<Vec<Value>> {
    match json {
        serde_json::Value::Array(arr) => {
            arr.iter().map(json_to_tauraro_value).collect()
        }
        other => Ok(vec![json_to_tauraro_value(other)?]),
    }
}

pub fn json_to_tauraro_value(json: &serde_json::Value) -> Result<Value> {
    match json {
        serde_json::Value::Null => Ok(Value::None),
        serde_json::Value::Bool(b) => Ok(Value::Bool(*b)),
        serde_json::Value::Number(n) => {
            if let Some(i) = n.as_i64() {
                Ok(Value::Int(i))
            } else if let Some(f) = n.as_f64() {
                Ok(Value::Float(f))
            } else {
                Ok(Value::Float(n.as_f64().unwrap_or(0.0)))
            }
        }
        serde_json::Value::String(s) => Ok(Value::Str(s.clone())),
        serde_json::Value::Array(arr) => {
            use crate::modules::hplist::HPList;
            let mut list = HPList::new();
            for item in arr {
                list.data.borrow_mut().push(json_to_tauraro_value(item)?);
            }
            Ok(Value::List(list))
        }
        serde_json::Value::Object(obj) => {
            use std::rc::Rc;
            use std::cell::RefCell;
            let mut dict = HashMap::new();
            for (key, value) in obj {
                dict.insert(key.clone(), json_to_tauraro_value(value)?);
            }
            Ok(Value::Dict(Rc::new(RefCell::new(dict))))
        }
    }
}

/// Convert Tauraro Value to JSON
pub fn tauraro_value_to_json(value: &Value) -> Result<serde_json::Value> {
    match value {
        Value::None => Ok(serde_json::Value::Null),
        Value::Bool(b) => Ok(serde_json::Value::Bool(*b)),
        Value::Int(i) => Ok(serde_json::json!(*i)),
        Value::Float(f) => Ok(serde_json::json!(*f)),
        Value::Str(s) => Ok(serde_json::Value::String(s.clone())),
        Value::List(list) => {
            let items: Result<Vec<_>> = list.data
                .borrow()
                .iter()
                .map(tauraro_value_to_json)
                .collect();
            Ok(serde_json::Value::Array(items?))
        }
        Value::Dict(dict) => {
            let mut map = serde_json::Map::new();
            for (key, val) in dict.borrow().iter() {
                map.insert(key.clone(), tauraro_value_to_json(val)?);
            }
            Ok(serde_json::Value::Object(map))
        }
        Value::NativeFunction(_) => {
            Ok(serde_json::json!({ "type": "function" }))
        }
        _ => Ok(serde_json::json!({ "type": "unsupported", "value": "complex_value" })),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_command_registration() {
        let registry = CommandRegistry::new();
        
        registry.register("test_command", |args| {
            Ok(serde_json::json!({ "received": args }))
        });

        let result = registry.invoke("test_command", serde_json::json!({"test": 123}));
        assert!(result.is_ok());
    }

    #[test]
    fn test_tauraro_function() {
        let registry = CommandRegistry::new();
        
        registry.register_tauraro_function("add", |args| {
            if args.len() != 2 {
                return Err(anyhow::anyhow!("Expected 2 arguments"));
            }
            
            let a = match &args[0] {
                Value::Int(i) => *i as f64,
                Value::Float(f) => *f,
                _ => return Err(anyhow::anyhow!("Expected number")),
            };
            
            let b = match &args[1] {
                Value::Int(i) => *i as f64,
                Value::Float(f) => *f,
                _ => return Err(anyhow::anyhow!("Expected number")),
            };
            
            Ok(Value::Float(a + b))
        });

        let result = registry.invoke("add", serde_json::json!([10, 20]));
        assert!(result.is_ok());
        assert_eq!(result.unwrap(), serde_json::json!(30.0));
    }
}
