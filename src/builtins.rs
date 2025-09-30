//! Built-in Functions and Module Registry for TauraroLang
//! 
//! This module serves as the central hub for all built-in functions and modules in TauraroLang.

use crate::value::Value;
use crate::modules;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::io::Write;

/// Type alias for built-in function signatures
pub type BuiltinFunction = fn(Vec<Value>) -> Result<Value>;

/// Registry for all built-in functions and modules
pub struct BuiltinRegistry {
    functions: HashMap<String, BuiltinFunction>,
    modules: HashMap<String, fn() -> Value>,
}

impl BuiltinRegistry {
    pub fn new() -> Self {
        let mut registry = Self {
            functions: HashMap::new(),
            modules: HashMap::new(),
        };
        
        registry.register_core_functions();
        registry.register_type_functions();
        registry.register_builtin_modules();
        
        registry
    }
    
    pub fn get_functions(&self) -> HashMap<String, Value> {
        self.functions.iter()
            .map(|(name, func)| {
                (name.clone(), Value::BuiltinFunction(name.clone(), *func))
            })
            .collect()
    }
    
    pub fn get_module(&self, name: &str) -> Option<Value> {
        self.modules.get(name).map(|create_fn| create_fn())
    }
    
    pub fn is_builtin_module(&self, name: &str) -> bool {
        self.modules.contains_key(name)
    }
    
    pub fn get_builtin_module_names(&self) -> Vec<String> {
        self.modules.keys().cloned().collect()
    }
    
    fn register_core_functions(&mut self) {
        // Core I/O functions
        self.functions.insert("print".to_string(), builtin_print);
        self.functions.insert("input".to_string(), builtin_input);
        
        // Core introspection functions
        self.functions.insert("len".to_string(), builtin_len);
        self.functions.insert("type".to_string(), builtin_type);
        self.functions.insert("id".to_string(), builtin_id);
        self.functions.insert("hash".to_string(), builtin_hash);
        self.functions.insert("repr".to_string(), builtin_repr);
        self.functions.insert("dir".to_string(), builtin_dir);
        self.functions.insert("help".to_string(), builtin_help);
        
        // Core utility functions
        self.functions.insert("callable".to_string(), builtin_callable);
        self.functions.insert("iter".to_string(), builtin_iter);
        self.functions.insert("next".to_string(), builtin_next);
        self.functions.insert("range".to_string(), builtin_range);
        
        // Advanced iteration functions (temporary placeholders)
        self.functions.insert("aiter".to_string(), |_args| Ok(Value::None));
        self.functions.insert("anext".to_string(), |_args| Ok(Value::None));
        
        // Special object creation (temporary placeholders)
        self.functions.insert("object".to_string(), |_args| Ok(Value::Object {
            class_name: "object".to_string(),
            fields: HashMap::new(),
            base_object: crate::base_object::BaseObject::new("object".to_string(), vec![]),
            mro: crate::base_object::MRO::from_linearization(vec!["object".to_string()]),
        }));
        self.functions.insert("super".to_string(), |_args| Ok(Value::Super("object".to_string(), "object".to_string(), None)));
        self.functions.insert("memoryview".to_string(), |_args| Ok(Value::None));
        self.functions.insert("slice".to_string(), builtin_slice);
        
        // Class and method decorators (temporary placeholders)
        self.functions.insert("classmethod".to_string(), |args| Ok(if args.is_empty() { Value::None } else { args[0].clone() }));
        self.functions.insert("staticmethod".to_string(), |args| Ok(if args.is_empty() { Value::None } else { args[0].clone() }));
        self.functions.insert("property".to_string(), |args| Ok(if args.is_empty() { Value::None } else { args[0].clone() }));
        
        // Import system (temporary placeholder)
        self.functions.insert("__import__".to_string(), |args| {
            if args.is_empty() {
                return Err(anyhow::anyhow!("__import__() missing required argument 'name'"));
            }
            match &args[0] {
                Value::Str(module_name) => {
                    if let Some(module) = get_builtin_module(module_name) {
                        Ok(module)
                    } else {
                        Err(anyhow::anyhow!("No module named '{}'", module_name))
                    }
                }
                _ => Err(anyhow::anyhow!("__import__() argument 1 must be str")),
            }
        });
    }
    
    fn register_type_functions(&mut self) {
        self.functions.insert("str".to_string(), builtin_str);
        self.functions.insert("int".to_string(), builtin_int);
        self.functions.insert("float".to_string(), builtin_float);
        self.functions.insert("bool".to_string(), builtin_bool);
        self.functions.insert("complex".to_string(), crate::builtins::builtin_complex);
        self.functions.insert("bytes".to_string(), crate::builtins::builtin_bytes);
        self.functions.insert("bytearray".to_string(), crate::builtins::builtin_bytearray);
        self.functions.insert("list".to_string(), builtin_list);
        self.functions.insert("tuple".to_string(), builtin_tuple);
        self.functions.insert("dict".to_string(), builtin_dict);
        self.functions.insert("set".to_string(), builtin_set);
        self.functions.insert("frozenset".to_string(), builtin_frozenset);
        self.functions.insert("range".to_string(), builtin_range);
        self.functions.insert("memoryview".to_string(), builtin_memoryview);
    }
    
    fn register_builtin_modules(&mut self) {
        // Core system modules
        self.modules.insert("sys".to_string(), modules::sys::create_sys_module);
        self.modules.insert("os".to_string(), modules::os::create_os_module);
        self.modules.insert("io".to_string(), modules::io::create_io_module);
        
        // Math and computation modules
        self.modules.insert("math".to_string(), modules::math::create_math_module);
        self.modules.insert("random".to_string(), modules::random::create_random_module);
        
        // Date and time modules
        self.modules.insert("time".to_string(), modules::time::create_time_module);
        self.modules.insert("datetime".to_string(), modules::datetime::create_datetime_module);
        
        // Text processing and data formats
        self.modules.insert("json".to_string(), modules::json::create_json_module);
        self.modules.insert("re".to_string(), modules::re::create_re_module);
        self.modules.insert("csv".to_string(), modules::csv::create_csv_module);
        self.modules.insert("base64".to_string(), modules::base64::create_base64_module);
        
        // Collections and functional programming
        self.modules.insert("collections".to_string(), modules::collections::create_collections_module);
        self.modules.insert("itertools".to_string(), modules::itertools::create_itertools_module);
        self.modules.insert("functools".to_string(), modules::functools::create_functools_module);
        self.modules.insert("copy".to_string(), modules::copy::create_copy_module);
        
        // Serialization and encoding
        self.modules.insert("pickle".to_string(), modules::pickle::create_pickle_module);
        self.modules.insert("hashlib".to_string(), modules::hashlib::create_hashlib_module);
        
        // Network and web modules
        self.modules.insert("socket".to_string(), modules::socket::create_socket_module);
        self.modules.insert("urllib".to_string(), modules::urllib::create_urllib_module);
        self.modules.insert("httpx".to_string(), modules::httpx::create_httpx_module);
        self.modules.insert("httptools".to_string(), modules::httptools::create_httptools_module);
        self.modules.insert("websockets".to_string(), modules::websockets::create_websockets_module);
        
        // Concurrency modules
        self.modules.insert("threading".to_string(), modules::threading::create_threading_module);
        self.modules.insert("asyncio".to_string(), modules::asyncio::create_asyncio_module);
        
        // Memory management modules
        self.modules.insert("memory".to_string(), modules::memory::create_memory_module);
        self.modules.insert("gc".to_string(), modules::gc::create_gc_module);
        
        // Development and testing modules
        self.modules.insert("logging".to_string(), modules::logging::create_logging_module);
        self.modules.insert("unittest".to_string(), modules::unittest::create_unittest_module);
    }
}

/// Global builtin registry instance
static mut BUILTIN_REGISTRY: Option<BuiltinRegistry> = None;
static REGISTRY_INIT: std::sync::Once = std::sync::Once::new();

pub fn get_builtin_registry() -> &'static BuiltinRegistry {
    unsafe {
        REGISTRY_INIT.call_once(|| {
            BUILTIN_REGISTRY = Some(BuiltinRegistry::new());
        });
        BUILTIN_REGISTRY.as_ref().unwrap()
    }
}

pub fn init_builtins() -> HashMap<String, Value> {
    get_builtin_registry().get_functions()
}

pub fn get_builtin_module(name: &str) -> Option<Value> {
    get_builtin_registry().get_module(name)
}

pub fn is_builtin_module(name: &str) -> bool {
    get_builtin_registry().is_builtin_module(name)
}

pub fn get_builtin_module_names() -> Vec<String> {
    get_builtin_registry().get_builtin_module_names()
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

pub fn is_truthy(value: &Value) -> bool {
    match value {
        Value::None => false,
        Value::Bool(b) => *b,
        Value::Int(n) => *n != 0,
        Value::Float(f) => *f != 0.0,
        Value::Str(s) => !s.is_empty(),
        Value::List(items) => !items.is_empty(),
        Value::Tuple(items) => !items.is_empty(),
        Value::Dict(dict) => !dict.is_empty(),
        _ => true,
    }
}

pub fn type_name(value: &Value) -> String {
    match value {
        Value::None => "NoneType".to_string(),
        Value::Bool(_) => "bool".to_string(),
        Value::Int(_) => "int".to_string(),
        Value::Float(_) => "float".to_string(),
        Value::Str(_) => "str".to_string(),
        Value::List(_) => "list".to_string(),
        Value::Tuple(_) => "tuple".to_string(),
        Value::Dict(_) => "dict".to_string(),
        Value::Set(_) => "set".to_string(),
        Value::Closure { .. } => "function".to_string(),
        Value::BuiltinFunction(_, _) => "builtin_function_or_method".to_string(),
        Value::Object { class_name, .. } => class_name.clone(),
        Value::Module(name, _) => format!("module '{}'", name),
        _ => "object".to_string(),
    }
}

// ============================================================================
// CORE BUILT-IN FUNCTION IMPLEMENTATIONS
// ============================================================================

pub fn builtin_print(args: Vec<Value>) -> Result<Value> {
    for (i, arg) in args.iter().enumerate() {
        if i > 0 { print!(" "); }
        print!("{}", arg);
    }
    println!();
    Ok(Value::None)
}

pub fn builtin_input(args: Vec<Value>) -> Result<Value> {
    if !args.is_empty() {
        print!("{}", args[0]);
        std::io::stdout().flush().map_err(|e| anyhow::anyhow!("Failed to flush stdout: {}", e))?;
    }
    
    let mut line = String::new();
    std::io::stdin().read_line(&mut line)
        .map_err(|e| anyhow::anyhow!("Failed to read input: {}", e))?;
    
    if line.ends_with('\n') {
        line.pop();
        if line.ends_with('\r') {
            line.pop();
        }
    }
    
    Ok(Value::Str(line))
}

pub fn builtin_len(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("len() takes exactly one argument"));
    }
    match &args[0] {
        Value::Str(s) => Ok(Value::Int(s.len() as i64)),
        Value::List(items) => Ok(Value::Int(items.len() as i64)),
        Value::Tuple(items) => Ok(Value::Int(items.len() as i64)),
        Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
        _ => Err(anyhow::anyhow!("object of type '{}' has no len()", type_name(&args[0]))),
    }
}

pub fn builtin_type(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("type() takes exactly one argument"));
    }
    Ok(Value::Str(format!("<class '{}'>", type_name(&args[0]))))
}



pub fn builtin_str(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Str(String::new()));
    }
    Ok(Value::Str(format!("{}", args[0])))
}

pub fn builtin_int(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Int(0));
    }
    match &args[0] {
        Value::Int(n) => Ok(Value::Int(*n)),
        Value::Float(f) => Ok(Value::Int(*f as i64)),
        Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
        Value::Str(s) => {
            s.trim().parse::<i64>()
                .map(Value::Int)
                .map_err(|_| anyhow::anyhow!("invalid literal for int(): '{}'", s))
        }
        _ => Err(anyhow::anyhow!("int() argument must be a string or a number")),
    }
}

pub fn builtin_float(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Float(0.0));
    }
    match &args[0] {
        Value::Int(n) => Ok(Value::Float(*n as f64)),
        Value::Float(n) => Ok(Value::Float(*n)),
        Value::Str(s) => {
            s.parse::<f64>()
                .map(Value::Float)
                .map_err(|_| anyhow::anyhow!("could not convert string to float: '{}'", s))
        }
        Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
        _ => Err(anyhow::anyhow!("float() argument must be a string or a number")),
    }
}

pub fn builtin_bool(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Bool(false));
    }
    Ok(Value::Bool(is_truthy(&args[0])))
}

pub fn builtin_list(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::List(vec![]));
    }
    Ok(Value::List(vec![args[0].clone()]))
}

pub fn builtin_tuple(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Ok(Value::Tuple(vec![]));
    }
    Ok(Value::Tuple(vec![args[0].clone()]))
}

pub fn builtin_dict(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Dict(HashMap::new()))
}

pub fn builtin_set(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Set(vec![]))
}

// Placeholder implementations for other functions
pub fn builtin_id(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_hash(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_repr(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_dir(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_help(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }

// Math functions
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
    Ok(args[0].clone())
}

pub fn builtin_max(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("max expected at least 1 argument, got 0"));
    }
    Ok(args[0].clone())
}


pub fn builtin_hex(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_oct(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_bin(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_ascii(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }

// Object functions
pub fn builtin_hasattr(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }
pub fn builtin_getattr(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_setattr(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_delattr(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_isinstance(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }
pub fn builtin_issubclass(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

// Collection functions
pub fn builtin_enumerate(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_zip(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_sorted(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_reversed(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_filter(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_map(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_all(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(true)) }
pub fn builtin_any(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

// Advanced functions
pub fn builtin_iter(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_next(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_slice(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_vars(_args: Vec<Value>) -> Result<Value> { Ok(Value::Dict(HashMap::new())) }
pub fn builtin_globals(_args: Vec<Value>) -> Result<Value> { Ok(Value::Dict(HashMap::new())) }
pub fn builtin_locals(_args: Vec<Value>) -> Result<Value> { Ok(Value::Dict(HashMap::new())) }
pub fn builtin_eval(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_exec(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_compile(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_breakpoint(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_load_library(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }

// Special collection types
pub fn builtin_frozenset(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        0 => Ok(Value::FrozenSet(vec![])),
        1 => {
            match &args[0] {
                Value::List(items) => {
                    // Remove duplicates for set behavior
                    let mut unique_items = Vec::new();
                    for item in items {
                        if !unique_items.contains(item) {
                            unique_items.push(item.clone());
                        }
                    }
                    Ok(Value::FrozenSet(unique_items))
                }
                Value::Tuple(items) => {
                    let mut unique_items = Vec::new();
                    for item in items {
                        if !unique_items.contains(item) {
                            unique_items.push(item.clone());
                        }
                    }
                    Ok(Value::FrozenSet(unique_items))
                }
                Value::Set(items) => Ok(Value::FrozenSet(items.clone())),
                Value::FrozenSet(items) => Ok(Value::FrozenSet(items.clone())),
                Value::Str(s) => {
                    let mut unique_chars = Vec::new();
                    for c in s.chars() {
                        let char_val = Value::Str(c.to_string());
                        if !unique_chars.contains(&char_val) {
                            unique_chars.push(char_val);
                        }
                    }
                    Ok(Value::FrozenSet(unique_chars))
                }
                _ => Err(anyhow!("'{}' object is not iterable", args[0].type_name()))
            }
        }
        _ => Err(anyhow!("frozenset expected at most 1 argument, got {}", args.len()))
    }
}

pub fn builtin_range(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        1 => {
            if let Value::Int(stop) = &args[0] {
                Ok(Value::Range { start: 0, stop: *stop, step: 1 })
            } else {
                Err(anyhow!("'{}' object cannot be interpreted as an integer", args[0].type_name()))
            }
        }
        2 => {
            if let (Value::Int(start), Value::Int(stop)) = (&args[0], &args[1]) {
                Ok(Value::Range { start: *start, stop: *stop, step: 1 })
            } else {
                Err(anyhow!("range arguments must be integers"))
            }
        }
        3 => {
            if let (Value::Int(start), Value::Int(stop), Value::Int(step)) = (&args[0], &args[1], &args[2]) {
                if *step == 0 {
                    Err(anyhow!("range() arg 3 must not be zero"))
                } else {
                    Ok(Value::Range { start: *start, stop: *stop, step: *step })
                }
            } else {
                Err(anyhow!("range arguments must be integers"))
            }
        }
        _ => Err(anyhow!("range expected 1 to 3 arguments, got {}", args.len()))
    }
}



// I/O functions
pub fn builtin_open(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_format(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_callable(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

// Built-in type constructors that were missing
pub fn builtin_bytearray(_args: Vec<Value>) -> Result<Value> { Ok(Value::ByteArray(vec![])) }
pub fn builtin_bytes(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bytes(vec![])) }
pub fn builtin_complex(_args: Vec<Value>) -> Result<Value> { Ok(Value::Complex { real: 0.0, imag: 0.0 }) }

// Method implementations for built-in types
pub fn builtin_str_join(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_str_split(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_str_strip(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_str_upper(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_str_lower(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }

pub fn builtin_list_append(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_list_extend(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_list_count(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_list_index(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }

pub fn builtin_dict_keys(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_dict_values(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_dict_items(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(vec![])) }
pub fn builtin_dict_get(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }

pub fn builtin_int_bit_length(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_float_is_integer(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

pub fn builtin_tuple_count(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_tuple_index(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }

pub fn builtin_set_add(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_set_remove(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }

pub fn builtin_bytes_decode(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_bytearray_append(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }

// VM-specific functions with VM context
pub fn builtin_eval_with_vm(_vm: &mut crate::vm::VM, _args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

pub fn builtin_exec_with_vm(_vm: &mut crate::vm::VM, _args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

// ============================================================================
// COMPLETE PYTHON BUILT-IN FUNCTION IMPLEMENTATIONS
// ============================================================================

// Math and comparison functions
pub fn builtin_sum(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("sum expected at most 2 arguments, got {}", args.len()));
    }
    
    let start = if args.len() == 2 {
        args[1].clone()
    } else {
        Value::Int(0)
    };
    
    match &args[0] {
        Value::List(items) => {
            let mut result = start;
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
        },
        _ => Err(anyhow::anyhow!("sum() can't sum {}", type_name(&args[0]))),
    }
}

// Advanced iteration functions
pub fn builtin_aiter(_args: Vec<Value>) -> Result<Value> {
    // Async iterator - placeholder for future async support
    Ok(Value::None)
}

pub fn builtin_anext(_args: Vec<Value>) -> Result<Value> {
    // Async next - placeholder for future async support  
    Ok(Value::None)
}

// Special object creation
pub fn builtin_object(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::Object {
        class_name: "object".to_string(),
        fields: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("object".to_string(), vec![]),
        mro: crate::base_object::MRO::from_linearization(vec!["object".to_string()]),
    })
}

pub fn builtin_super(args: Vec<Value>) -> Result<Value> {
    // Simplified super() implementation
    Ok(Value::Super("object".to_string(), "object".to_string(), None))
}

pub fn builtin_memoryview(args: Vec<Value>) -> Result<Value> {
    match args.len() {
        1 => {
            match &args[0] {
                Value::Bytes(data) => {
                    Ok(Value::MemoryView {
                        data: data.clone(),
                        format: "B".to_string(), // unsigned byte
                        shape: vec![data.len()],
                    })
                }
                Value::ByteArray(data) => {
                    Ok(Value::MemoryView {
                        data: data.clone(),
                        format: "B".to_string(), // unsigned byte
                        shape: vec![data.len()],
                    })
                }
                _ => Err(anyhow!("memoryview: a bytes-like object is required, not '{}'", args[0].type_name()))
            }
        }
        _ => Err(anyhow!("memoryview() takes exactly one argument ({} given)", args.len()))
    }
}

// Class and method decorators
pub fn builtin_classmethod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("classmethod expected exactly 1 argument, got {}", args.len()));
    }
    // Return the function wrapped as a classmethod
    Ok(args[0].clone())
}

pub fn builtin_staticmethod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("staticmethod expected exactly 1 argument, got {}", args.len()));
    }
    // Return the function wrapped as a staticmethod
    Ok(args[0].clone())
}

pub fn builtin_property(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 4 {
        return Err(anyhow::anyhow!("property expected 1 to 4 arguments, got {}", args.len()));
    }
    // Return a property object
    Ok(args[0].clone())
}

// Import system
pub fn builtin_import(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        return Err(anyhow::anyhow!("__import__() missing required argument 'name'"));
    }
    
    match &args[0] {
        Value::Str(module_name) => {
            // Try to get built-in module first
            if let Some(module) = get_builtin_module(module_name) {
                Ok(module)
            } else {
                // For now, return None for non-builtin modules
                // TODO: Integrate with proper module loading system
                Err(anyhow::anyhow!("No module named '{}'", module_name))
            }
        }
        _ => Err(anyhow::anyhow!("__import__() argument 1 must be str, not {}", type_name(&args[0]))),
    }
}

// Enhanced mathematical functions with proper implementations
pub fn builtin_pow(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("pow expected 2 or 3 arguments, got {}", args.len()));
    }
    
    let base = &args[0];
    let exp = &args[1];
    
    let result = match (base, exp) {
        (Value::Int(a), Value::Int(b)) => {
            if *b < 0 {
                Value::Float((*a as f64).powf(*b as f64))
            } else {
                Value::Int(a.pow(*b as u32))
            }
        },
        (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
        (Value::Int(a), Value::Float(b)) => Value::Float((*a as f64).powf(*b)),
        (Value::Float(a), Value::Int(b)) => Value::Float(a.powf(*b as f64)),
        _ => return Err(anyhow::anyhow!("unsupported operand type(s) for ** or pow(): '{}' and '{}'", type_name(base), type_name(exp))),
    };
    
    if args.len() == 3 {
        // Modular exponentiation
        match (&result, &args[2]) {
            (Value::Int(r), Value::Int(m)) => Ok(Value::Int(r % m)),
            _ => Err(anyhow::anyhow!("pow() 3rd argument not allowed unless all arguments are integers")),
        }
    } else {
        Ok(result)
    }
}

pub fn builtin_round(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() || args.len() > 2 {
        return Err(anyhow::anyhow!("round expected 1 or 2 arguments, got {}", args.len()));
    }
    
    let ndigits = if args.len() == 2 {
        match &args[1] {
            Value::Int(n) => *n,
            _ => return Err(anyhow::anyhow!("'{}' object cannot be interpreted as an integer", type_name(&args[1]))),
        }
    } else {
        0
    };
    
    match &args[0] {
        Value::Float(f) => {
            if ndigits == 0 {
                Ok(Value::Int(f.round() as i64))
            } else {
                let multiplier = 10.0_f64.powi(ndigits as i32);
                Ok(Value::Float((f * multiplier).round() / multiplier))
            }
        },
        Value::Int(i) => {
            if ndigits >= 0 {
                Ok(Value::Int(*i))
            } else {
                let divisor = 10_i64.pow(-ndigits as u32);
                Ok(Value::Int((i / divisor) * divisor))
            }
        },
        _ => Err(anyhow::anyhow!("a float is required")),
    }
}

pub fn builtin_divmod(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("divmod expected 2 arguments, got {}", args.len()));
    }
    
    match (&args[0], &args[1]) {
        (Value::Int(a), Value::Int(b)) => {
            if *b == 0 {
                return Err(anyhow::anyhow!("integer division or modulo by zero"));
            }
            Ok(Value::Tuple(vec![Value::Int(a / b), Value::Int(a % b)]))
        },
        (Value::Float(a), Value::Float(b)) => {
            if *b == 0.0 {
                return Err(anyhow::anyhow!("float division or modulo by zero"));
            }
            Ok(Value::Tuple(vec![Value::Float((a / b).floor()), Value::Float(a % b)]))
        },
        (Value::Int(a), Value::Float(b)) => {
            if *b == 0.0 {
                return Err(anyhow::anyhow!("float division or modulo by zero"));
            }
            let a_f = *a as f64;
            Ok(Value::Tuple(vec![Value::Float((a_f / b).floor()), Value::Float(a_f % b)]))
        },
        (Value::Float(a), Value::Int(b)) => {
            if *b == 0 {
                return Err(anyhow::anyhow!("float division or modulo by zero"));
            }
            let b_f = *b as f64;
            Ok(Value::Tuple(vec![Value::Float((a / b_f).floor()), Value::Float(a % b_f)]))
        },
        _ => Err(anyhow::anyhow!("unsupported operand type(s) for divmod(): '{}' and '{}'", type_name(&args[0]), type_name(&args[1]))),
    }
}

// String manipulation functions with proper implementations
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
                Some(ch) => Ok(Value::Str(ch.to_string())),
                None => Err(anyhow::anyhow!("chr() arg not in range(0x110000)")),
            }
        }
        _ => Err(anyhow::anyhow!("chr() expected int object, not '{}'", type_name(&args[0]))),
    }
}

pub fn builtin_ord(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("ord() takes exactly one argument"));
    }
    
    match &args[0] {
        Value::Str(s) => {
            if s.len() != 1 {
                return Err(anyhow::anyhow!("ord() expected a character, but string of length {} found", s.len()));
            }
            let ch = s.chars().next().unwrap();
            Ok(Value::Int(ch as u32 as i64))
        }
        _ => Err(anyhow::anyhow!("ord() expected str object, not '{}'", type_name(&args[0]))),
    }
}