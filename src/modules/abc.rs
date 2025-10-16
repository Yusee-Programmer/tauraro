//! Abstract Base Classes (ABC) module implementation

use crate::value::Value;
use std::collections::HashMap;
use anyhow::Result;

/// ABC module implementation
pub fn abc_module() -> HashMap<String, Value> {
    let mut module = HashMap::new();
    
    // Add ABC-related classes and functions
    module.insert("ABCMeta".to_string(), Value::BuiltinFunction("ABCMeta".to_string(), abcmeta_builtin));
    module.insert("ABC".to_string(), create_abc_class());
    module.insert("abstractmethod".to_string(), Value::BuiltinFunction("abstractmethod".to_string(), abstractmethod_builtin));
    
    module
}

/// ABCMeta metaclass implementation
fn abcmeta_builtin(args: Vec<Value>) -> Result<Value> {
    // For now, just return the class as-is
    // In a full implementation, this would be a proper metaclass
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ABCMeta() takes exactly 1 argument"));
    }
    Ok(args[0].clone())
}

/// ABC base class
fn create_abc_class() -> Value {
    let mut methods = HashMap::new();
    
    // ABC class with ABCMeta as metaclass
    Value::Class {
        name: "ABC".to_string(),
        bases: vec!["object".to_string()],
        methods,
        metaclass: Some("ABCMeta".to_string()),
        mro: crate::base_object::MRO::from_linearization(vec!["ABC".to_string(), "object".to_string()]),
        base_object: crate::base_object::BaseObject::new("ABC".to_string(), vec!["object".to_string()]),
        slots: None, // Default to no slots
    }
}

/// abstractmethod decorator
fn abstractmethod_builtin(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("abstractmethod() takes exactly 1 argument"));
    }
    
    let func = &args[0];
    
    // Mark the function as abstract by adding a special attribute
    // In a full implementation, this would prevent instantiation of classes that don't override this method
    match func {
        Value::Closure { name, params, body, captured_scope, docstring, compiled_code } => {
            // Create a new closure with an abstract marker
            Ok(Value::Object {
                class_name: "abstractmethod".to_string(),
                fields: {
                    let mut fields = HashMap::new();
                    fields.insert("func".to_string(), func.clone());
                    fields.insert("is_abstract".to_string(), Value::Bool(true));
                    fields
                },
                class_methods: HashMap::new(),
                base_object: crate::base_object::BaseObject::new("abstractmethod".to_string(), vec!["object".to_string()]),
                mro: crate::base_object::MRO::from_linearization(vec!["abstractmethod".to_string(), "object".to_string()]),
                slots: None, // Default to no slots
            })
        },
        _ => Ok(func.clone())
    }
}