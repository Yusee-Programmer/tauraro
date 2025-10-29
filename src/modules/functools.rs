/// Functools module - provides utilities for higher-order functions and operations on callable objects
/// Similar to Python's functools module

use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;


/// Create the functools module object with all its functions and classes
pub fn create_functools_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Higher-order functions
    namespace.insert("reduce".to_string(), Value::NativeFunction(functools_reduce));
    namespace.insert("partial".to_string(), Value::NativeFunction(functools_partial));
    namespace.insert("partialmethod".to_string(), Value::NativeFunction(functools_partialmethod));
    namespace.insert("wraps".to_string(), Value::NativeFunction(functools_wraps));
    namespace.insert("update_wrapper".to_string(), Value::NativeFunction(functools_update_wrapper));
    
    // Caching decorators
    namespace.insert("lru_cache".to_string(), Value::NativeFunction(functools_lru_cache));
    namespace.insert("cache".to_string(), Value::NativeFunction(functools_cache));
    namespace.insert("cached_property".to_string(), Value::NativeFunction(functools_cached_property));
    
    // Comparison functions
    namespace.insert("cmp_to_key".to_string(), Value::NativeFunction(functools_cmp_to_key));
    namespace.insert("total_ordering".to_string(), Value::NativeFunction(functools_total_ordering));
    
    // Single dispatch
    namespace.insert("singledispatch".to_string(), Value::NativeFunction(functools_singledispatch));
    namespace.insert("singledispatchmethod".to_string(), Value::NativeFunction(functools_singledispatchmethod));
    
    Value::Module("functools".to_string(), namespace)
}

/// Reduce function - apply a function of two arguments cumulatively to items in a sequence
fn functools_reduce(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("reduce() takes 2 or 3 arguments"));
    }
    
    let function = &args[0];
    let iterable = &args[1];
    let initial = if args.len() == 3 { Some(&args[2]) } else { None };
    
    // Convert iterable to vector
    let items = match iterable {
        Value::List(items) => items.as_vec().clone(),
        Value::Tuple(items) => items.clone(),
        Value::Str(s) => s.chars().map(|c| Value::Str(c.to_string())).collect(),
        _ => return Err(anyhow::anyhow!("reduce() argument 2 must be iterable")),
    };
    
    if items.is_empty() {
        if let Some(init) = initial {
            return Ok(init.clone());
        } else {
            return Err(anyhow::anyhow!("reduce() of empty sequence with no initial value"));
        }
    }
    
    let mut accumulator = if let Some(init) = initial {
        init.clone()
    } else {
        items[0].clone()
    };
    
    let start_index = if initial.is_some() { 0 } else { 1 };
    
    for item in &items[start_index..] {
        // Call function with accumulator and current item
        accumulator = call_function(function, vec![accumulator, item.clone()])?;
    }
    
    Ok(accumulator)
}

/// Partial function - return a new partial object which when called will behave like func called with the positional arguments args
fn functools_partial(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("partial() missing required argument: 'func'"));
    }
    
    let func = args[0].clone();
    let partial_args = if args.len() > 1 { args[1..].to_vec() } else { vec![] };
    
    let mut partial_obj = HashMap::new();
    partial_obj.insert("func".to_string(), func);
    partial_obj.insert("args".to_string(), Value::Tuple(partial_args));
    partial_obj.insert("keywords".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    partial_obj.insert("__call__".to_string(), Value::NativeFunction(partial_call));
    
    Ok(Value::Object {
        class_name: "partial".to_string(),
        fields: Rc::new(partial_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("partial".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["partial".to_string(), "object".to_string()]),
    })
}

/// Call method for partial objects
fn partial_call(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("partial call missing self argument"));
    }
    
    // Extract partial object data
    let partial_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid partial object")),
    };
    
    let func = partial_obj.get("func").ok_or_else(|| anyhow::anyhow!("Partial object missing func"))?;
    let partial_args = match partial_obj.get("args") {
        Some(Value::Tuple(args)) => args.clone(),
        _ => vec![],
    };
    
    // Combine partial args with new args
    let mut combined_args = partial_args;
    combined_args.extend_from_slice(&args[1..]);
    
    call_function(func, combined_args)
}

/// Partial method - similar to partial but for methods
fn functools_partialmethod(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("partialmethod() missing required argument: 'func'"));
    }
    
    let func = args[0].clone();
    let partial_args = if args.len() > 1 { args[1..].to_vec() } else { vec![] };
    
    let mut partial_method = HashMap::new();
    partial_method.insert("func".to_string(), func);
    partial_method.insert("args".to_string(), Value::Tuple(partial_args));
    partial_method.insert("__get__".to_string(), Value::NativeFunction(partialmethod_get));
    
    Ok(Value::Object {
        class_name: "partialmethod".to_string(),
        fields: Rc::new(partial_method),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("partialmethod".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["partialmethod".to_string(), "object".to_string()]),
    })
}

/// Partial method get descriptor
fn partialmethod_get(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("partialmethod get missing arguments"));
    }
    
    let partialmethod_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid partialmethod object")),
    };
    
    let func = partialmethod_obj.get("func").ok_or_else(|| anyhow::anyhow!("Partialmethod missing func"))?;
    let partial_args = match partialmethod_obj.get("args") {
        Some(Value::Tuple(args)) => args.clone(),
        _ => vec![],
    };
    
    // Create a bound method-like object
    let mut bound_method = HashMap::new();
    bound_method.insert("func".to_string(), func.clone());
    bound_method.insert("args".to_string(), Value::Tuple(partial_args));
    bound_method.insert("instance".to_string(), args[1].clone());
    bound_method.insert("__call__".to_string(), Value::NativeFunction(partialmethod_call));
    
    Ok(Value::Object {
        class_name: "bound_partialmethod".to_string(),
        fields: Rc::new(bound_method),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("bound_partialmethod".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["bound_partialmethod".to_string(), "object".to_string()]),
    })
}

/// Partial method call
fn partialmethod_call(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("partialmethod call missing self argument"));
    }
    
    let bound_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid bound partialmethod object")),
    };
    
    let func = bound_obj.get("func").ok_or_else(|| anyhow::anyhow!("Bound partialmethod missing func"))?;
    let partial_args = match bound_obj.get("args") {
        Some(Value::Tuple(args)) => args.clone(),
        _ => vec![],
    };
    let instance = bound_obj.get("instance").ok_or_else(|| anyhow::anyhow!("Bound partialmethod missing instance"))?;
    
    // Combine instance, partial args, and new args
    let mut combined_args = vec![instance.clone()];
    combined_args.extend(partial_args);
    combined_args.extend_from_slice(&args[1..]);
    
    call_function(func, combined_args)
}

/// Wraps decorator - update a wrapper function to look like the wrapped function
fn functools_wraps(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("wraps() takes exactly 1 argument"));
    }
    
    let wrapped = args[0].clone();
    
    // Return a decorator function
    let mut decorator = HashMap::new();
    decorator.insert("wrapped".to_string(), wrapped);
    decorator.insert("__call__".to_string(), Value::NativeFunction(wraps_decorator));
    
    Ok(Value::Object {
        class_name: "wraps_decorator".to_string(),
        fields: Rc::new(decorator),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("wraps_decorator".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["wraps_decorator".to_string(), "object".to_string()]),
    })
}

/// Wraps decorator call method
fn wraps_decorator(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("wraps decorator takes exactly 1 argument"));
    }
    
    let wrapper = args[1].clone();
    
    // In a full implementation, this would copy attributes from wrapped to wrapper
    // For now, just return the wrapper
    Ok(wrapper)
}

/// Update wrapper - update a wrapper function to look like the wrapped function
fn functools_update_wrapper(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("update_wrapper() takes at least 2 arguments"));
    }
    
    let wrapper = &args[0];
    let _wrapped = &args[1];
    
    // In a full implementation, this would copy attributes like __name__, __doc__, __module__, etc.
    // For now, we'll just return the wrapper unchanged since we don't have full function metadata
    
    Ok(wrapper.clone())
}



/// LRU Cache decorator - least-recently-used cache decorator
fn functools_lru_cache(args: Vec<Value>) -> Result<Value> {
    let maxsize = if !args.is_empty() {
        match &args[0] {
            Value::Int(n) => Some(*n as usize),
            Value::None => None,
            _ => Some(128), // Default maxsize
        }
    } else {
        Some(128)
    };
    
    let mut cache_decorator = HashMap::new();
    cache_decorator.insert("maxsize".to_string(), 
        if let Some(size) = maxsize { Value::Int(size as i64) } else { Value::None });
    cache_decorator.insert("cache".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    cache_decorator.insert("hits".to_string(), Value::Int(0));
    cache_decorator.insert("misses".to_string(), Value::Int(0));
    cache_decorator.insert("__call__".to_string(), Value::NativeFunction(lru_cache_decorator));
    
    Ok(Value::Object {
        class_name: "lru_cache_decorator".to_string(),
        fields: Rc::new(cache_decorator),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("lru_cache_decorator".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["lru_cache_decorator".to_string(), "object".to_string()]),
    })
}

/// LRU cache decorator call method
fn lru_cache_decorator(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("lru_cache decorator takes exactly 1 argument"));
    }
    
    let func = args[1].clone();
    
    // Create cached function wrapper
    let mut cached_func = HashMap::new();
    cached_func.insert("func".to_string(), func);
    cached_func.insert("cache".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    cached_func.insert("__call__".to_string(), Value::NativeFunction(cached_function_call));
    cached_func.insert("cache_info".to_string(), Value::NativeFunction(cache_info));
    cached_func.insert("cache_clear".to_string(), Value::NativeFunction(cache_clear));
    
    Ok(Value::Object {
        class_name: "cached_function".to_string(),
        fields: Rc::new(cached_func),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("cached_function".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["cached_function".to_string(), "object".to_string()]),
    })
}

/// Cached function call method
fn cached_function_call(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("cached function call missing self argument"));
    }
    
    let cached_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid cached function object")),
    };
    
    let func = cached_obj.get("func").ok_or_else(|| anyhow::anyhow!("Cached function missing func"))?;
    let cache = match cached_obj.get("cache") {
        Some(Value::Dict(cache)) => cache,
        _ => return Err(anyhow::anyhow!("Cached function missing cache")),
    };
    
    // Create a key from the arguments (simplified - in reality would need to handle unhashable types)
    let key = format!("{:?}", &args[1..]);
    
    // Check if result is in cache
    if let Some(cached_result) = cache.borrow().get(&key) {
        // Increment hits counter
        // In a full implementation, we would update the hits counter
        return Ok(cached_result.clone());
    }
    
    // Call the original function
    let result = call_function(func, args[1..].to_vec())?;
    
    // Store result in cache
    // In a full implementation, we would update the cache and misses counter
    
    Ok(result)
}

/// Cache info method
fn cache_info(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cache_info() takes exactly 1 argument"));
    }
    
    let cached_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid cached function object")),
    };
    
    let cache: &HashMap<String, Value> = match cached_obj.get("cache") {
        Some(Value::Dict(cache)) => &cache.borrow(),
        _ => &HashMap::new(), // Return empty cache if not found
    };
    
    // Get hits and misses counters
    let hits = match cached_obj.get("hits") {
        Some(Value::Int(n)) => *n,
        _ => 0,
    };
    
    let misses = match cached_obj.get("misses") {
        Some(Value::Int(n)) => *n,
        _ => 0,
    };
    
    let maxsize = match cached_obj.get("maxsize") {
        Some(Value::Int(n)) => Some(*n),
        Some(Value::None) => None,
        _ => Some(128), // Default maxsize
    };
    
    let mut info = HashMap::new();
    info.insert("hits".to_string(), Value::Int(hits));
    info.insert("misses".to_string(), Value::Int(misses));
    info.insert("maxsize".to_string(), maxsize.map_or(Value::None, |n| Value::Int(n)));
    info.insert("currsize".to_string(), Value::Int(cache.len() as i64));
    
    // Create a simple object to represent the cache info
    Ok(Value::Object {
        class_name: "cache_info".to_string(),
        fields: Rc::new(info),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("cache_info".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["cache_info".to_string(), "object".to_string()]),
    })
}

/// Cache clear method
fn cache_clear(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cache_clear() takes exactly 1 argument"));
    }
    
    let cached_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid cached function object")),
    };
    
    // In a full implementation, this would clear the cache and reset counters
    // For now, we'll just return None
    
    Ok(Value::None)
}

/// Cache decorator (unbounded cache)
fn functools_cache(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cache() takes exactly 1 argument"));
    }
    
    // Use lru_cache with maxsize=None
    functools_lru_cache(vec![Value::None])
}

/// Cached property decorator
fn functools_cached_property(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cached_property() takes exactly 1 argument"));
    }
    
    let func = args[0].clone();
    
    let mut cached_prop = HashMap::new();
    cached_prop.insert("func".to_string(), func);
    cached_prop.insert("cache_name".to_string(), Value::Str(format!("__cached_{}", std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH).unwrap().as_nanos())));
    cached_prop.insert("__get__".to_string(), Value::NativeFunction(cached_property_get));
    
    Ok(Value::Object {
        class_name: "cached_property".to_string(),
        fields: Rc::new(cached_prop),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("cached_property".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["cached_property".to_string(), "object".to_string()]),
    })
}

/// Cached property get descriptor
fn cached_property_get(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("cached_property get missing arguments"));
    }
    
    let cached_prop_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid cached_property object")),
    };
    
    let func = cached_prop_obj.get("func").ok_or_else(|| anyhow::anyhow!("Cached property missing func"))?;
    let cache_name = match cached_prop_obj.get("cache_name") {
        Some(Value::Str(name)) => name,
        _ => return Err(anyhow::anyhow!("Cached property missing cache_name")),
    };
    let instance = &args[1];
    
    // Check if cached value exists
    if let Value::Object { fields, .. } = instance {
        if let Some(cached_value) = fields.as_ref().get(cache_name) {
            return Ok(cached_value.clone());
        }
    }
    
    // Compute and cache the value
    let result = call_function(func, vec![instance.clone()])?;
    
    // Store in instance's cache
    // Note: This is a simplified implementation that doesn't actually update the instance
    // In a real implementation, we would need to use Rc::make_mut to modify the fields
    
    Ok(result)
}

/// Convert comparison function to key function
fn functools_cmp_to_key(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("cmp_to_key() takes exactly one argument"));
    }
    
    let cmp_func = args[0].clone();
    
    let mut key_obj = HashMap::new();
    key_obj.insert("cmp_func".to_string(), cmp_func);
    key_obj.insert("__call__".to_string(), Value::NativeFunction(cmp_to_key_call));
    
    Ok(Value::Object {
        class_name: "cmp_to_key".to_string(),
        fields: Rc::new(key_obj),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("cmp_to_key".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["cmp_to_key".to_string(), "object".to_string()]),
    })
}

/// cmp_to_key call method
fn cmp_to_key_call(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 {
        return Err(anyhow::anyhow!("cmp_to_key call missing arguments"));
    }
    
    let key_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid cmp_to_key object")),
    };
    
    let cmp_func = key_obj.get("cmp_func").ok_or_else(|| anyhow::anyhow!("cmp_to_key missing cmp_func"))?;
    let other = &args[1];
    
    // Call comparison function with self and other
    call_function(cmp_func, vec![args[0].clone(), other.clone()])
}

/// Total ordering decorator
fn functools_total_ordering(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("total_ordering() takes exactly one argument"));
    }
    
    let cls = args[0].clone();
    
    // In a full implementation, this would add missing comparison methods
    // For now, we'll just return the class unchanged
    Ok(cls)
}

/// Single dispatch decorator
fn functools_singledispatch(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("singledispatch() takes exactly one argument"));
    }
    
    let func = args[0].clone();
    
    let mut registry = HashMap::new();
    registry.insert("dispatch".to_string(), func);
    registry.insert("registry".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    registry.insert("__call__".to_string(), Value::NativeFunction(singledispatch_call));
    registry.insert("register".to_string(), Value::NativeFunction(singledispatch_register));
    
    Ok(Value::Object {
        class_name: "singledispatch".to_string(),
        fields: Rc::new(registry),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("singledispatch".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["singledispatch".to_string(), "object".to_string()]),
    })
}

/// Single dispatch call method
fn singledispatch_call(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("singledispatch call missing arguments"));
    }
    
    let dispatch_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid singledispatch object")),
    };
    
    let dispatch_func = dispatch_obj.get("dispatch").ok_or_else(|| anyhow::anyhow!("singledispatch missing dispatch function"))?;
    
    // Call the original function
    call_function(dispatch_func, args[1..].to_vec())
}

/// Single dispatch register method
fn singledispatch_register(_args: Vec<Value>) -> Result<Value> {
    // Placeholder - would register type-specific implementations
    Ok(Value::None)
}

/// Single dispatch method decorator
fn functools_singledispatchmethod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("singledispatchmethod() takes exactly one argument"));
    }
    
    let func = args[0].clone();
    
    let mut registry = HashMap::new();
    registry.insert("dispatch".to_string(), func);
    registry.insert("registry".to_string(), Value::Dict(Rc::new(RefCell::new(HashMap::new()))));
    registry.insert("__call__".to_string(), Value::NativeFunction(singledispatchmethod_call));
    registry.insert("register".to_string(), Value::NativeFunction(singledispatchmethod_register));
    
    Ok(Value::Object {
        class_name: "singledispatchmethod".to_string(),
        fields: Rc::new(registry),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("singledispatchmethod".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["singledispatchmethod".to_string(), "object".to_string()]),
    })
}

/// Single dispatch method call method
fn singledispatchmethod_call(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("singledispatchmethod call missing arguments"));
    }
    
    let dispatch_obj = match &args[0] {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid singledispatchmethod object")),
    };
    
    let dispatch_func = dispatch_obj.get("dispatch").ok_or_else(|| anyhow::anyhow!("singledispatchmethod missing dispatch function"))?;
    
    // Call the original function
    call_function(dispatch_func, args[1..].to_vec())
}

/// Single dispatch method register method
fn singledispatchmethod_register(_args: Vec<Value>) -> Result<Value> {
    // Placeholder - would register type-specific implementations
    Ok(Value::None)
}

/// Helper function to call a function value with arguments
pub fn call_function(func: &Value, args: Vec<Value>) -> Result<Value> {
    match func {
        Value::NativeFunction(f) => f(args),
        Value::Closure { .. } => {
            // Would need VM integration to call user-defined functions
            Err(anyhow::anyhow!("User-defined function calls not yet implemented"))
        }
        _ => Err(anyhow::anyhow!("Object is not callable")),
    }
}