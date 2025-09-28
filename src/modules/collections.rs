/// Collections module - provides specialized container datatypes
/// Similar to Python's collections module

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;

/// Create the collections module object with all its classes and functions
pub fn create_collections_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Container datatypes
    namespace.insert("deque".to_string(), Value::NativeFunction(create_deque));
    namespace.insert("Counter".to_string(), Value::NativeFunction(create_counter));
    namespace.insert("defaultdict".to_string(), Value::NativeFunction(create_defaultdict));
    namespace.insert("OrderedDict".to_string(), Value::NativeFunction(create_ordereddict));
    namespace.insert("ChainMap".to_string(), Value::NativeFunction(create_chainmap));
    namespace.insert("UserDict".to_string(), Value::NativeFunction(create_userdict));
    namespace.insert("UserList".to_string(), Value::NativeFunction(create_userlist));
    namespace.insert("UserString".to_string(), Value::NativeFunction(create_userstring));
    
    // Named tuple factory
    namespace.insert("namedtuple".to_string(), Value::NativeFunction(create_namedtuple));
    
    Value::Module("collections".to_string(), namespace)
}

/// Create a deque (double-ended queue) object
fn create_deque(args: Vec<Value>) -> Result<Value> {
    let iterable = if !args.is_empty() {
        to_list(&args[0])?
    } else {
        vec![]
    };
    
    let maxlen = if args.len() > 1 {
        match &args[1] {
            Value::Int(n) => Some(*n as usize),
            Value::None => None,
            _ => return Err(anyhow::anyhow!("deque maxlen must be an integer or None")),
        }
    } else {
        None
    };
    
    let mut deque_obj = HashMap::new();
    deque_obj.insert("data".to_string(), Value::List(iterable));
    deque_obj.insert("maxlen".to_string(), 
        if let Some(max) = maxlen { Value::Int(max as i64) } else { Value::None });
    
    // Deque methods
    deque_obj.insert("append".to_string(), Value::NativeFunction(deque_append));
    deque_obj.insert("appendleft".to_string(), Value::NativeFunction(deque_appendleft));
    deque_obj.insert("clear".to_string(), Value::NativeFunction(deque_clear));
    deque_obj.insert("copy".to_string(), Value::NativeFunction(deque_copy));
    deque_obj.insert("count".to_string(), Value::NativeFunction(deque_count));
    deque_obj.insert("extend".to_string(), Value::NativeFunction(deque_extend));
    deque_obj.insert("extendleft".to_string(), Value::NativeFunction(deque_extendleft));
    deque_obj.insert("index".to_string(), Value::NativeFunction(deque_index));
    deque_obj.insert("insert".to_string(), Value::NativeFunction(deque_insert));
    deque_obj.insert("pop".to_string(), Value::NativeFunction(deque_pop));
    deque_obj.insert("popleft".to_string(), Value::NativeFunction(deque_popleft));
    deque_obj.insert("remove".to_string(), Value::NativeFunction(deque_remove));
    deque_obj.insert("reverse".to_string(), Value::NativeFunction(deque_reverse));
    deque_obj.insert("rotate".to_string(), Value::NativeFunction(deque_rotate));
    
    Ok(Value::Object {
        class_name: "deque".to_string(),
        fields: deque_obj,
        base_object: crate::base_object::BaseObject::new("deque".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["deque".to_string(), "object".to_string()]),
    })
}

// Deque method implementations (simplified)
fn deque_append(_args: Vec<Value>) -> Result<Value> {
    // Placeholder - would need mutable access to deque data
    Ok(Value::None)
}

fn deque_appendleft(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_clear(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_copy(_args: Vec<Value>) -> Result<Value> {
    // Return a copy of the deque
    Ok(Value::None)
}

fn deque_count(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(0))
}

fn deque_extend(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_extendleft(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_index(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(0))
}

fn deque_insert(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_pop(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_popleft(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_remove(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_reverse(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn deque_rotate(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Create a Counter object (dict subclass for counting hashable objects)
fn create_counter(args: Vec<Value>) -> Result<Value> {
    let mut counter_data = HashMap::new();
    
    if !args.is_empty() {
        match &args[0] {
            Value::List(items) | Value::Tuple(items) => {
                for item in items {
                    let key = format!("{:?}", item); // Simplified key representation
                    let count = counter_data.get(&key).unwrap_or(&Value::Int(0));
                    if let Value::Int(n) = count {
                        counter_data.insert(key, Value::Int(n + 1));
                    }
                }
            }
            Value::Str(s) => {
                for ch in s.chars() {
                    let key = ch.to_string();
                    let count = counter_data.get(&key).unwrap_or(&Value::Int(0));
                    if let Value::Int(n) = count {
                        counter_data.insert(key, Value::Int(n + 1));
                    }
                }
            }
            Value::Dict(map) => {
                for (key, value) in map {
                    counter_data.insert(key.clone(), value.clone());
                }
            }
            _ => {}
        }
    }
    
    let mut counter_obj = HashMap::new();
    counter_obj.insert("data".to_string(), Value::Dict(counter_data));
    
    // Counter methods
    counter_obj.insert("most_common".to_string(), Value::NativeFunction(counter_most_common));
    counter_obj.insert("elements".to_string(), Value::NativeFunction(counter_elements));
    counter_obj.insert("subtract".to_string(), Value::NativeFunction(counter_subtract));
    counter_obj.insert("update".to_string(), Value::NativeFunction(counter_update));
    counter_obj.insert("total".to_string(), Value::NativeFunction(counter_total));
    
    Ok(Value::Object {
        class_name: "Counter".to_string(),
        fields: counter_obj,
        base_object: crate::base_object::BaseObject::new("Counter".to_string(), vec!["dict".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Counter".to_string(), "dict".to_string(), "object".to_string()]),
    })
}

// Counter method implementations (simplified)
fn counter_most_common(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("most_common() missing self argument"));
    }
    
    let _n = if args.len() > 1 {
        match &args[1] {
            Value::Int(num) => Some(*num as usize),
            Value::None => None,
            _ => return Err(anyhow::anyhow!("most_common() n must be an integer or None")),
        }
    } else {
        None
    };
    
    // Return empty list for now (simplified)
    Ok(Value::List(vec![]))
}

fn counter_elements(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::List(vec![]))
}

fn counter_subtract(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn counter_update(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn counter_total(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Int(0))
}

/// Create a defaultdict object (dict subclass that calls a factory function to supply missing values)
fn create_defaultdict(args: Vec<Value>) -> Result<Value> {
    let default_factory = if !args.is_empty() {
        args[0].clone()
    } else {
        Value::None
    };
    
    let mut defaultdict_obj = HashMap::new();
    defaultdict_obj.insert("default_factory".to_string(), default_factory);
    defaultdict_obj.insert("data".to_string(), Value::Dict(HashMap::new()));
    
    // defaultdict methods
    defaultdict_obj.insert("__missing__".to_string(), Value::NativeFunction(defaultdict_missing));
    defaultdict_obj.insert("__getitem__".to_string(), Value::NativeFunction(defaultdict_getitem));
    defaultdict_obj.insert("__setitem__".to_string(), Value::NativeFunction(defaultdict_setitem));
    
    Ok(Value::Object {
        class_name: "defaultdict".to_string(),
        fields: defaultdict_obj,
        base_object: crate::base_object::BaseObject::new("defaultdict".to_string(), vec!["dict".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["defaultdict".to_string(), "dict".to_string(), "object".to_string()]),
    })
}

fn defaultdict_missing(_args: Vec<Value>) -> Result<Value> {
    // Call default_factory if available
    Ok(Value::None)
}

fn defaultdict_getitem(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

fn defaultdict_setitem(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Create an OrderedDict object (dict subclass that remembers the order entries were added)
fn create_ordereddict(args: Vec<Value>) -> Result<Value> {
    let mut data = HashMap::new();
    let mut order = Vec::new();
    
    if !args.is_empty() {
        match &args[0] {
            Value::Dict(map) => {
                for (key, value) in map {
                    data.insert(key.clone(), value.clone());
                    order.push(Value::Str(key.clone()));
                }
            }
            Value::List(items) => {
                for item in items {
                    if let Value::Tuple(pair) = item {
                        if pair.len() == 2 {
                            if let (Value::Str(key), value) = (&pair[0], &pair[1]) {
                                data.insert(key.clone(), value.clone());
                                order.push(Value::Str(key.clone()));
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
    
    let mut ordereddict_obj = HashMap::new();
    ordereddict_obj.insert("data".to_string(), Value::Dict(data));
    ordereddict_obj.insert("order".to_string(), Value::List(order));
    
    // OrderedDict methods
    ordereddict_obj.insert("popitem".to_string(), Value::NativeFunction(ordereddict_popitem));
    ordereddict_obj.insert("move_to_end".to_string(), Value::NativeFunction(ordereddict_move_to_end));
    
    Ok(Value::Object {
        class_name: "OrderedDict".to_string(),
        fields: ordereddict_obj,
        base_object: crate::base_object::BaseObject::new("OrderedDict".to_string(), vec!["dict".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["OrderedDict".to_string(), "dict".to_string(), "object".to_string()]),
    })
}

fn ordereddict_popitem(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Tuple(vec![Value::None, Value::None]))
}

fn ordereddict_move_to_end(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

/// Create a ChainMap object (dict-like class for creating a single view of multiple mappings)
fn create_chainmap(args: Vec<Value>) -> Result<Value> {
    let mut maps = Vec::new();
    
    for arg in args {
        match arg {
            Value::Dict(_) => maps.push(arg),
            _ => return Err(anyhow::anyhow!("ChainMap arguments must be mappings")),
        }
    }
    
    if maps.is_empty() {
        maps.push(Value::Dict(HashMap::new()));
    }
    
    let mut chainmap_obj = HashMap::new();
    chainmap_obj.insert("maps".to_string(), Value::List(maps));
    
    // ChainMap methods
    chainmap_obj.insert("new_child".to_string(), Value::NativeFunction(chainmap_new_child));
    chainmap_obj.insert("parents".to_string(), Value::NativeFunction(chainmap_parents));
    
    Ok(Value::Object {
        class_name: "ChainMap".to_string(),
        fields: chainmap_obj,
        base_object: crate::base_object::BaseObject::new("ChainMap".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["ChainMap".to_string(), "object".to_string()]),
    })
}

fn chainmap_new_child(_args: Vec<Value>) -> Result<Value> {
    // Return a new ChainMap with an additional mapping
    Ok(Value::None)
}

fn chainmap_parents(_args: Vec<Value>) -> Result<Value> {
    // Return a ChainMap without the first mapping
    Ok(Value::None)
}

/// Create a UserDict object (wrapper around dictionary objects for easier dict subclassing)
fn create_userdict(args: Vec<Value>) -> Result<Value> {
    let data = if !args.is_empty() {
        match &args[0] {
            Value::Dict(map) => map.clone(),
            _ => HashMap::new(),
        }
    } else {
        HashMap::new()
    };
    
    let mut userdict_obj = HashMap::new();
    userdict_obj.insert("data".to_string(), Value::Dict(data));
    
    Ok(Value::Object {
        class_name: "UserDict".to_string(),
        fields: userdict_obj,
        base_object: crate::base_object::BaseObject::new("UserDict".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["UserDict".to_string(), "object".to_string()]),
    })
}

/// Create a UserList object (wrapper around list objects for easier list subclassing)
fn create_userlist(args: Vec<Value>) -> Result<Value> {
    let data = if !args.is_empty() {
        to_list(&args[0])?
    } else {
        vec![]
    };
    
    let mut userlist_obj = HashMap::new();
    userlist_obj.insert("data".to_string(), Value::List(data));
    
    Ok(Value::Object {
        class_name: "UserList".to_string(),
        fields: userlist_obj,
        base_object: crate::base_object::BaseObject::new("UserList".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["UserList".to_string(), "object".to_string()]),
    })
}

/// Create a UserString object (wrapper around string objects for easier string subclassing)
fn create_userstring(args: Vec<Value>) -> Result<Value> {
    let data = if !args.is_empty() {
        match &args[0] {
            Value::Str(s) => s.clone(),
            _ => format!("{:?}", args[0]),
        }
    } else {
        String::new()
    };
    
    let mut userstring_obj = HashMap::new();
    userstring_obj.insert("data".to_string(), Value::Str(data));
    
    Ok(Value::Object {
        class_name: "UserString".to_string(),
        fields: userstring_obj,
        base_object: crate::base_object::BaseObject::new("UserString".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["UserString".to_string(), "object".to_string()]),
    })
}

/// Create a namedtuple factory function
fn create_namedtuple(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("namedtuple() missing required arguments"));
    }
    
    let typename = match &args[0] {
        Value::Str(name) => name.clone(),
        _ => return Err(anyhow::anyhow!("namedtuple() typename must be a string")),
    };
    
    let field_names = match &args[1] {
        Value::Str(names) => {
            // Split by whitespace or comma
            names.split_whitespace()
                .flat_map(|s| s.split(','))
                .filter(|s| !s.is_empty())
                .map(|s| s.trim().to_string())
                .collect::<Vec<_>>()
        }
        Value::List(items) => {
            let mut names = Vec::new();
            for item in items {
                if let Value::Str(name) = item {
                    names.push(name.clone());
                } else {
                    return Err(anyhow::anyhow!("namedtuple() field names must be strings"));
                }
            }
            names
        }
        _ => return Err(anyhow::anyhow!("namedtuple() field_names must be a string or sequence")),
    };
    
    // Create namedtuple class factory
    let mut namedtuple_factory = HashMap::new();
    namedtuple_factory.insert("typename".to_string(), Value::Str(typename.clone()));
    namedtuple_factory.insert("field_names".to_string(), 
        Value::List(field_names.iter().map(|s| Value::Str(s.clone())).collect()));
    namedtuple_factory.insert("__call__".to_string(), Value::NativeFunction(namedtuple_constructor));
    
    Ok(Value::Object {
        class_name: format!("{}_factory", typename),
        fields: namedtuple_factory,
        base_object: crate::base_object::BaseObject::new(format!("{}_factory", typename), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec![format!("{}_factory", typename), "object".to_string()]),
    })
}

/// Namedtuple constructor
fn namedtuple_constructor(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("namedtuple constructor missing self argument"));
    }
    
    // Extract factory information
    let factory = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid namedtuple factory")),
    };
    
    let typename = match factory.get("typename") {
        Some(Value::Str(name)) => name.clone(),
        _ => return Err(anyhow::anyhow!("Invalid namedtuple typename")),
    };
    
    let field_names = match factory.get("field_names") {
        Some(Value::List(names)) => {
            let mut field_names = Vec::new();
            for name in names {
                if let Value::Str(s) = name {
                    field_names.push(s.clone());
                }
            }
            field_names
        }
        _ => return Err(anyhow::anyhow!("Invalid namedtuple field names")),
    };
    
    // Create namedtuple instance
    let values = &args[1..];
    if values.len() != field_names.len() {
        return Err(anyhow::anyhow!("namedtuple() takes exactly {} arguments", field_names.len()));
    }
    
    let mut namedtuple_obj = HashMap::new();
    for (i, field_name) in field_names.iter().enumerate() {
        namedtuple_obj.insert(field_name.clone(), values[i].clone());
    }
    
    // Add namedtuple methods
    namedtuple_obj.insert("_fields".to_string(), 
        Value::List(field_names.iter().map(|s| Value::Str(s.clone())).collect()));
    namedtuple_obj.insert("_asdict".to_string(), Value::NativeFunction(namedtuple_asdict));
    namedtuple_obj.insert("_replace".to_string(), Value::NativeFunction(namedtuple_replace));
    
    Ok(Value::Object {
        class_name: typename.clone(),
        fields: namedtuple_obj,
        base_object: crate::base_object::BaseObject::new(typename.clone(), vec!["tuple".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec![typename, "tuple".to_string(), "object".to_string()]),
    })
}

fn namedtuple_asdict(_args: Vec<Value>) -> Result<Value> {
    // Return a dict representation of the namedtuple
    Ok(Value::Dict(HashMap::new()))
}

fn namedtuple_replace(_args: Vec<Value>) -> Result<Value> {
    // Return a new namedtuple with specified fields replaced
    Ok(Value::None)
}

/// Helper function to convert a Value to a list
fn to_list(value: &Value) -> Result<Vec<Value>> {
    match value {
        Value::List(items) => Ok(items.clone()),
        Value::Tuple(items) => Ok(items.clone()),
        Value::Str(s) => Ok(s.chars().map(|c| Value::Str(c.to_string())).collect()),
        _ => Err(anyhow::anyhow!("Object is not iterable")),
    }
}
