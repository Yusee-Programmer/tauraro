/// Copy module - provides shallow and deep copy functionality
/// Similar to Python's copy module

use crate::value::Value;
use std::collections::HashMap;
use crate::modules::hplist::HPList;

type Result<T> = anyhow::Result<T>;

/// Create the copy module
pub fn create_copy_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Add copy functions
    namespace.insert("copy".to_string(), Value::BuiltinFunction("copy".to_string(), copy_copy));
    namespace.insert("deepcopy".to_string(), Value::BuiltinFunction("deepcopy".to_string(), copy_deepcopy));
    namespace.insert("Error".to_string(), Value::BuiltinFunction("Error".to_string(), copy_error));
    
    Value::Module("copy".to_string(), namespace)
}

/// copy.Error.__str__ - String representation of copy error
fn copy_error_str(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("__str__() missing self argument"));
    }
    
    if let Value::Object { fields, .. } = &args[0] {
        if let Some(Value::Str(message)) = fields.get("message") {
            return Ok(Value::Str(message.clone()));
        }
    }
    
    Ok(Value::Str("Copy error".to_string()))
}

/// Get a copy module function by name
pub fn get_copy_function(name: &str) -> Option<fn(Vec<Value>) -> Result<Value>> {
    match name {
        "copy" => Some(copy_copy),
        "deepcopy" => Some(copy_deepcopy),
        "Error" => Some(copy_error),
        _ => None,
    }
}

/// copy.copy(x) - Return a shallow copy of x
fn copy_copy(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("copy() missing required argument: 'x'"));
    }
    
    let obj = &args[0];
    
    // Perform shallow copy based on type
    match obj {
        Value::Int(i) => Ok(Value::Int(*i)),
        Value::Float(f) => Ok(Value::Float(*f)),
        Value::Str(s) => Ok(Value::Str(s.clone())),
        Value::Bool(b) => Ok(Value::Bool(*b)),
        Value::None => Ok(Value::None),
        Value::List(items) => {
            // Shallow copy - copy the list but not the items
            Ok(Value::List(items.clone()))
        },
        Value::Dict(map) => {
            // Shallow copy - copy the dict but not the values
            Ok(Value::Dict(map.clone()))
        },
        Value::Tuple(items) => {
            // Tuples are immutable, so return the same tuple
            Ok(Value::Tuple(items.clone()))
        },
        Value::Set(items) => {
            // Shallow copy of set
            Ok(Value::Set(items.clone()))
        },
        Value::Object { class_name, fields, base_object, mro, .. } => {
            // Shallow copy of object
            Ok(Value::Object {
                class_name: class_name.clone(),
                fields: fields.clone(),
                class_methods: HashMap::new(),
                base_object: base_object.clone(),
                mro: mro.clone(),
            })
        },
        _ => {
            // For other types, return a reference copy
            Ok(obj.clone())
        }
    }
}

/// copy.deepcopy(x[, memo]) - Return a deep copy of x
fn copy_deepcopy(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("deepcopy() missing required argument: 'x'"));
    }
    
    let obj = &args[0];
    let mut memo = HashMap::new();
    
    // If memo dict is provided, use it
    if args.len() > 1 {
        if let Value::Dict(memo_dict) = &args[1] {
            memo = memo_dict.clone();
        }
    }
    
    deepcopy_recursive(obj, &mut memo)
}

/// Recursive helper for deep copy
fn deepcopy_recursive(obj: &Value, memo: &mut HashMap<String, Value>) -> Result<Value> {
    match obj {
        Value::Int(i) => Ok(Value::Int(*i)),
        Value::Float(f) => Ok(Value::Float(*f)),
        Value::Str(s) => Ok(Value::Str(s.clone())),
        Value::Bool(b) => Ok(Value::Bool(*b)),
        Value::None => Ok(Value::None),
        Value::List(items) => {
            let mut new_items = Vec::new();
            for item in items {
                new_items.push(deepcopy_recursive(item, memo)?);
            }
            Ok(Value::List(HPList::from_values(new_items)))
        },
        Value::Dict(map) => {
            let mut new_map = HashMap::new();
            for (key, value) in map {
                new_map.insert(key.clone(), deepcopy_recursive(value, memo)?);
            }
            Ok(Value::Dict(new_map))
        },
        Value::Tuple(items) => {
            let mut new_items = Vec::new();
            for item in items {
                new_items.push(deepcopy_recursive(item, memo)?);
            }
            Ok(Value::Tuple(new_items))
        },
        Value::Set(items) => {
            let mut new_items = Vec::new();
            for item in items {
                new_items.push(deepcopy_recursive(item, memo)?);
            }
            Ok(Value::Set(new_items))
        },
        Value::Object { class_name, fields, base_object, mro, .. } => {
            let mut new_fields = HashMap::new();
            for (key, value) in fields {
                new_fields.insert(key.clone(), deepcopy_recursive(value, memo)?);
            }
            Ok(Value::Object {
                class_name: class_name.clone(),
                fields: new_fields,
                class_methods: HashMap::new(),
                base_object: base_object.clone(),
                mro: mro.clone(),
            })
        },
        _ => {
            // For other types, return a reference copy
            Ok(obj.clone())
        }
    }
}

/// copy.Error - Exception raised for copy-related errors
fn copy_error(args: Vec<Value>) -> Result<Value> {
    let message = if args.is_empty() {
        "Copy error".to_string()
    } else {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => "Copy error".to_string(),
        }
    };
    
    let mut error_obj = HashMap::new();
    error_obj.insert("message".to_string(), Value::Str(message));
    error_obj.insert("__str__".to_string(), Value::NativeFunction(copy_error_str));
    
    Ok(Value::Object {
        class_name: "Error".to_string(),
        fields: error_obj,
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("Error".to_string(), vec!["Exception".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Error".to_string(), "Exception".to_string(), "object".to_string()]),
    })
}

/// Helper function to check if an object is copyable
pub fn is_copyable(obj: &Value) -> bool {
    match obj {
        Value::Closure { .. } => false,
        Value::BuiltinFunction(_, _) => false,
        Value::NativeFunction(_) => false,
        Value::Module(..) => false,
        _ => true,
    }
}

/// Helper function to get object id for memo tracking
pub fn get_object_id(obj: &Value) -> String {
    // Simple implementation - in a real system this would use memory addresses
    match obj {
        Value::Str(s) => format!("str_{}", s),
        Value::List(_) => format!("list_{:p}", obj as *const Value),
        Value::Dict(_) => format!("dict_{:p}", obj as *const Value),
        Value::Object { class_name, .. } => format!("obj_{}_{:p}", class_name, obj as *const Value),
        _ => format!("val_{:p}", obj as *const Value),
    }
}