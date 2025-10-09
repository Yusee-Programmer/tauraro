//! Built-in Functions and Module Registry for TauraroLang
//! 
//! This module serves as the central hub for all built-in functions and modules in TauraroLang.

use crate::value::Value;
use crate::modules;
use crate::ast::*;
use crate::base_object::BaseObject;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::io::Write;

// Import HPList
use crate::modules::hplist::HPList;

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
        self.functions.insert("globals".to_string(), builtin_globals);
        self.functions.insert("locals".to_string(), builtin_locals);
        self.functions.insert("vars".to_string(), builtin_vars);
        self.functions.insert("help".to_string(), builtin_help);
        
        // Core utility functions
        self.functions.insert("callable".to_string(), builtin_callable);
        self.functions.insert("iter".to_string(), builtin_iter);
        self.functions.insert("next".to_string(), builtin_next);
        self.functions.insert("range".to_string(), builtin_range);

        // String and numeric functions
        self.functions.insert("ord".to_string(), builtin_ord);
        self.functions.insert("chr".to_string(), builtin_chr);
        self.functions.insert("abs".to_string(), builtin_abs);
        self.functions.insert("sum".to_string(), builtin_sum);
        self.functions.insert("min".to_string(), builtin_min);
        self.functions.insert("max".to_string(), builtin_max);
        self.functions.insert("round".to_string(), builtin_round);
        self.functions.insert("pow".to_string(), builtin_pow);
        self.functions.insert("divmod".to_string(), builtin_divmod);

        // Sequence and iteration functions
        self.functions.insert("all".to_string(), builtin_all);
        self.functions.insert("any".to_string(), builtin_any);
        self.functions.insert("enumerate".to_string(), builtin_enumerate);
        self.functions.insert("filter".to_string(), builtin_filter);
        self.functions.insert("map".to_string(), builtin_map);
        self.functions.insert("zip".to_string(), builtin_zip);
        self.functions.insert("sorted".to_string(), builtin_sorted);
        self.functions.insert("reversed".to_string(), builtin_reversed);

        // Other utility functions
        self.functions.insert("isinstance".to_string(), builtin_isinstance);
        self.functions.insert("issubclass".to_string(), builtin_issubclass);
        self.functions.insert("hasattr".to_string(), builtin_hasattr);
        self.functions.insert("getattr".to_string(), builtin_getattr);
        self.functions.insert("setattr".to_string(), builtin_setattr);
        self.functions.insert("delattr".to_string(), builtin_delattr);
        self.functions.insert("ascii".to_string(), builtin_ascii);
        self.functions.insert("bin".to_string(), builtin_bin);
        self.functions.insert("hex".to_string(), builtin_hex);
        self.functions.insert("oct".to_string(), builtin_oct);
        self.functions.insert("format".to_string(), builtin_format);
        self.functions.insert("open".to_string(), builtin_open);
        
        // Advanced iteration functions (temporary placeholders)
        self.functions.insert("aiter".to_string(), |_args| Ok(Value::None));
        self.functions.insert("anext".to_string(), |_args| Ok(Value::None));
        
        // Special object creation (temporary placeholders)
        self.functions.insert("object".to_string(), |_args| Ok(Value::Object {
            class_name: "object".to_string(),
            fields: HashMap::new(),
            class_methods: HashMap::new(),
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
        // Return high-performance list
        return Ok(Value::List(HPList::new()));
    }
    
    match &args[0] {
        Value::List(items) => Ok(Value::List(items.clone())),
        Value::Tuple(items) => {
            let mut hplist = HPList::new();
            for item in items {
                hplist.append(item.clone());
            }
            Ok(Value::List(hplist))
        },
        Value::Set(items) => {
            let mut hplist = HPList::new();
            for item in items {
                hplist.append(item.clone());
            }
            Ok(Value::List(hplist))
        },
        Value::FrozenSet(items) => {
            let mut hplist = HPList::new();
            for item in items {
                hplist.append(item.clone());
            }
            Ok(Value::List(hplist))
        },
        Value::Range { start, stop, step } => {
            let mut result = HPList::new();
            let mut current = *start;
            
            if *step > 0 {
                while current < *stop {
                    result.append(Value::Int(current));
                    current += step;
                }
            } else if *step < 0 {
                while current > *stop {
                    result.append(Value::Int(current));
                    current += step;
                }
            }
            
            Ok(Value::List(result))
        }
        Value::Str(s) => {
            let mut hplist = HPList::new();
            for c in s.chars() {
                hplist.append(Value::Str(c.to_string()));
            }
            Ok(Value::List(hplist))
        }
        _ => Err(anyhow::anyhow!("'{}' object is not iterable", args[0].type_name()))
    }
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

pub fn builtin_frozenset(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::FrozenSet(vec![]))
}

pub fn builtin_iter(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

pub fn builtin_next(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

pub fn builtin_slice(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

pub fn builtin_range(args: Vec<Value>) -> Result<Value> {
    let mut start: i64 = 0;
    let stop: i64;
    let mut step: i64 = 1;
    
    match args.len() {
        1 => {
            stop = match &args[0] {
                Value::Int(n) => *n,
                _ => return Err(anyhow::anyhow!("range() integer argument expected, got {}", type_name(&args[0]))),
            };
        }
        2 => {
            start = match &args[0] {
                Value::Int(n) => *n,
                _ => return Err(anyhow::anyhow!("range() integer argument expected, got {}", type_name(&args[0]))),
            };
            stop = match &args[1] {
                Value::Int(n) => *n,
                _ => return Err(anyhow::anyhow!("range() integer argument expected, got {}", type_name(&args[0]))),
            };
        }
        3 => {
            start = match &args[0] {
                Value::Int(n) => *n,
                _ => return Err(anyhow::anyhow!("range() integer argument expected, got {}", type_name(&args[0]))),
            };
            stop = match &args[1] {
                Value::Int(n) => *n,
                _ => return Err(anyhow::anyhow!("range() integer argument expected, got {}", type_name(&args[0]))),
            };
            step = match &args[2] {
                Value::Int(n) => *n,
                _ => return Err(anyhow::anyhow!("range() integer argument expected, got {}", type_name(&args[0]))),
            };
            if step == 0 {
                return Err(anyhow::anyhow!("range() step argument must not be zero"));
            }
        }
        _ => return Err(anyhow::anyhow!("range() expected 1-3 arguments, got {}", args.len())),
    }
    
    Ok(Value::Range { start, stop, step })
}

pub fn builtin_memoryview(_args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

// Placeholder implementations for other functions
pub fn builtin_id(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_hash(_args: Vec<Value>) -> Result<Value> { Ok(Value::Int(0)) }
pub fn builtin_repr(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_dir(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        // Return empty list as placeholder - proper implementation needs VM context
        Ok(Value::List(HPList::new()))
    } else {
        let mut names = Vec::new();
        match &args[0] {
            Value::Str(_) => {
                // For strings, show only string methods (no inherited dunder methods)
                names.extend([
                    "capitalize", "casefold", "center", "count", "encode", "endswith",
                    "expandtabs", "find", "format", "format_map", "index", "isalnum",
                    "isalpha", "isascii", "isdecimal", "isdigit", "isidentifier",
                    "islower", "isnumeric", "isprintable", "isspace", "istitle",
                    "isupper", "join", "ljust", "lower", "lstrip", "maketrans",
                    "partition", "removeprefix", "removesuffix", "replace", "rfind",
                    "rindex", "rjust", "rpartition", "rsplit", "rstrip", "split",
                    "splitlines", "startswith", "strip", "swapcase", "title",
                    "translate", "upper", "zfill"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::List(_) => {
                // For lists, show only list methods (no inherited dunder methods)
                names.extend([
                    "append", "clear", "copy", "count", "extend", "index", "insert",
                    "pop", "remove", "reverse", "sort"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::Dict(_) => {
                // For dicts, show only dict methods (no inherited dunder methods)
                names.extend([
                    "clear", "copy", "fromkeys", "get", "items", "keys", "pop",
                    "popitem", "setdefault", "update", "values"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::Set(_) => {
                // For sets, show only set methods (no inherited dunder methods)
                names.extend([
                    "add", "clear", "copy", "difference", "difference_update",
                    "discard", "intersection", "intersection_update", "isdisjoint",
                    "issubset", "issuperset", "pop", "remove", "symmetric_difference",
                    "symmetric_difference_update", "union", "update"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::Tuple(_) => {
                // For tuples, show only tuple methods (no inherited dunder methods)
                names.extend([
                    "count", "index"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::Int(_) => {
                // For ints, show only int methods (no inherited dunder methods)
                names.extend([
                    "bit_length", "conjugate", "denominator", "from_bytes",
                    "numerator", "to_bytes"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::Float(_) => {
                // For floats, show only float methods (no inherited dunder methods)
                names.extend([
                    "as_integer_ratio", "conjugate", "fromhex", "hex", "is_integer"
                ].iter().map(|s| Value::Str(s.to_string())));
            },
            Value::Object { fields, class_methods, base_object, .. } => {
                // For custom objects, show user-defined attributes (fields), class methods, and only overridden dunder methods

                // Add instance attributes (fields)
                for (field_name, _) in fields {
                    names.push(Value::Str(field_name.clone()));
                }

                // Add class methods (non-dunder methods defined in the class)
                for (method_name, _) in class_methods {
                    if !method_name.starts_with("__") || !method_name.ends_with("__") {
                        names.push(Value::Str(method_name.clone()));
                    }
                }

                // Add only overridden dunder methods from class_methods
                for (method_name, _) in class_methods {
                    if method_name.starts_with("__") && method_name.ends_with("__") {
                        names.push(Value::Str(method_name.clone()));
                    }
                }

                // Note: We don't add dunder methods from base_object because in Python,
                // dir() only shows dunders that are explicitly defined/overridden in the class
            },
            Value::Module(_, namespace) => {
                // For modules, show all attributes
                for (name, _) in namespace {
                    names.push(Value::Str(name.clone()));
                }
            },
            _ => {
                // For other types, return empty list (no inherited dunder methods)
            }
        }

        // Sort the names alphabetically like Python does
        names.sort_by(|a, b| {
            match (a, b) {
                (Value::Str(s1), Value::Str(s2)) => s1.cmp(s2),
                _ => std::cmp::Ordering::Equal,
            }
        });

        let mut hplist = HPList::new();
        for name in names {
            hplist.append(name);
        }
        Ok(Value::List(hplist))
    }
}

// Introspection functions - placeholders that will be overridden by VM's special handling
pub fn builtin_globals(_args: Vec<Value>) -> Result<Value> {
    // Placeholder - actual implementation requires VM context
    // The VM intercepts this in call_function_fast and provides the real globals dict
    Ok(Value::Dict(HashMap::new()))
}

pub fn builtin_locals(_args: Vec<Value>) -> Result<Value> {
    // Placeholder - actual implementation requires VM context
    // The VM intercepts this in call_function_fast and provides the real locals dict
    Ok(Value::Dict(HashMap::new()))
}

pub fn builtin_vars(args: Vec<Value>) -> Result<Value> {
    if args.is_empty() {
        // Placeholder - actual implementation requires VM context
        // The VM intercepts this in call_function_fast and provides the real locals dict
        Ok(Value::Dict(HashMap::new()))
    } else {
        // vars(object) - return object's __dict__
        // For now, just return empty dict as placeholder
        Ok(Value::Dict(HashMap::new()))
    }
}

pub fn builtin_help(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_open(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_format(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_callable(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

// Math functions
pub fn builtin_abs(args: Vec<Value>) -> Result<Value> { 
    if args.len() != 1 {
        return Err(anyhow::anyhow!("abs() takes exactly one argument"));
    }
    match &args[0] {
        Value::Int(n) => Ok(Value::Int(n.abs())),
        Value::Float(f) => Ok(Value::Float(f.abs())),
        _ => Err(anyhow::anyhow!("abs() unsupported for type"))
    }
}

pub fn builtin_min(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_max(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_round(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_pow(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }
pub fn builtin_divmod(_args: Vec<Value>) -> Result<Value> { Ok(Value::None) }

// String functions
pub fn builtin_hex(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("0x0".to_string())) }
pub fn builtin_oct(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("0o0".to_string())) }
pub fn builtin_bin(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("0b0".to_string())) }
pub fn builtin_ascii(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }

// Object functions
pub fn builtin_hasattr(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("hasattr expected 2 arguments, got {}", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("hasattr(): attribute name must be string")),
    };
    
    // Check if object has the attribute
    let has_attr = match obj {
        Value::Object { fields, base_object, .. } => {
            // For custom objects, check both user-defined fields and dunder methods
            fields.contains_key(attr_name) || base_object.dunder_methods.contains_key(attr_name)
        },
        Value::Module(_, namespace) => {
            namespace.contains_key(attr_name)
        },
        Value::Dict(dict) => {
            dict.contains_key(attr_name)
        },
        // For other types, check if they have methods with this name or inherit dunder methods
        _ => {
            // Check if it's a built-in method or a dunder method from base object
            obj.get_method(attr_name).is_some() || 
            // Check if it's a dunder method that all objects inherit
            BaseObject::get_base_methods().contains_key(attr_name)
        }
    };
    
    Ok(Value::Bool(has_attr))
}
pub fn builtin_getattr(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 3 {
        return Err(anyhow::anyhow!("getattr expected 2 or 3 arguments, got {}", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("getattr(): attribute name must be string")),
    };
    
    // Try to get the attribute
    match obj {
        Value::Object { fields, base_object, .. } => {
            if let Some(value) = fields.get(attr_name) {
                Ok(value.clone())
            } else if base_object.dunder_methods.contains_key(attr_name) {
                // Create a bound method object for dunder methods
                Ok(Value::BoundMethod {
                    object: Box::new(obj.clone()),
                    method_name: attr_name.clone(),
                })
            } else if args.len() == 3 {
                // Return default value
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Module(_, namespace) => {
            if let Some(value) = namespace.get(attr_name) {
                Ok(value.clone())
            } else if args.len() == 3 {
                // Return default value
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("module '{}' has no attribute '{}'", obj.type_name(), attr_name))
            }
        },
        Value::Dict(dict) => {
            if let Some(value) = dict.get(attr_name) {
                Ok(value.clone())
            } else if args.len() == 3 {
                // Return default value
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'dict' object has no attribute '{}'", attr_name))
            }
        },
        // For other types, try to get method
        _ => {
            // First check if there's a method with this name
            if let Some(_) = obj.get_method(attr_name) {
                // Create a bound method object
                Ok(Value::BoundMethod {
                    object: Box::new(obj.clone()),
                    method_name: attr_name.clone(),
                })
            } else if BaseObject::get_base_methods().contains_key(attr_name) {
                // Create a bound method object for inherited dunder methods
                Ok(Value::BoundMethod {
                    object: Box::new(obj.clone()),
                    method_name: attr_name.clone(),
                })
            } else if args.len() == 3 {
                // Return default value
                Ok(args[2].clone())
            } else {
                Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
            }
        }
    }
}
pub fn builtin_setattr(args: Vec<Value>) -> Result<Value> {
    if args.len() != 3 {
        return Err(anyhow::anyhow!("setattr expected 3 arguments, got {}", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("setattr(): attribute name must be string")),
    };
    let value = &args[2];
    
    // Set the attribute based on object type
    match obj {
        Value::Object { fields, .. } => {
            // This is a bit tricky since we can't mutate the object directly
            // In a real implementation, we'd need to handle this differently
            Err(anyhow::anyhow!("setattr not fully implemented for objects"))
        },
        Value::Module(_, namespace) => {
            // This is also tricky for the same reason
            Err(anyhow::anyhow!("setattr not fully implemented for modules"))
        },
        Value::Dict(dict) => {
            // For dicts, we can insert the key-value pair
            // But again, we can't mutate directly
            Err(anyhow::anyhow!("setattr not fully implemented for dicts"))
        },
        _ => {
            Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
        }
    }
}
pub fn builtin_delattr(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("delattr expected 2 arguments, got {}", args.len()));
    }
    
    let obj = &args[0];
    let attr_name = match &args[1] {
        Value::Str(s) => s,
        _ => return Err(anyhow::anyhow!("delattr(): attribute name must be string")),
    };
    
    // Delete the attribute based on object type
    match obj {
        Value::Object { .. } => {
            // Can't mutate directly
            Err(anyhow::anyhow!("delattr not fully implemented for objects"))
        },
        Value::Module(_, _) => {
            // Can't mutate directly
            Err(anyhow::anyhow!("delattr not fully implemented for modules"))
        },
        Value::Dict(dict) => {
            // Can't mutate directly
            Err(anyhow::anyhow!("delattr not fully implemented for dicts"))
        },
        _ => {
            Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj.type_name(), attr_name))
        }
    }
}
pub fn builtin_isinstance(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }
pub fn builtin_issubclass(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

// Collection functions
pub fn builtin_enumerate(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(HPList::new())) }
pub fn builtin_zip(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(HPList::new())) }
pub fn builtin_sorted(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(HPList::new())) }
pub fn builtin_reversed(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(HPList::new())) }
pub fn builtin_filter(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(HPList::new())) }
pub fn builtin_map(_args: Vec<Value>) -> Result<Value> { Ok(Value::List(HPList::new())) }

// Advanced collection functions
pub fn builtin_all(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(true)) }
pub fn builtin_any(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bool(false)) }

// Built-in type constructors that were missing
pub fn builtin_bytearray(_args: Vec<Value>) -> Result<Value> { Ok(Value::ByteArray(vec![])) }
pub fn builtin_bytes(_args: Vec<Value>) -> Result<Value> { Ok(Value::Bytes(vec![])) }
pub fn builtin_complex(_args: Vec<Value>) -> Result<Value> { Ok(Value::Complex { real: 0.0, imag: 0.0 }) }

// Method implementations for built-in types
pub fn builtin_str_join(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_str_split(_args: Vec<Value>) -> Result<Value> { 
    let mut hplist = HPList::new();
    hplist.append(Value::Str("".to_string()));
    Ok(Value::List(hplist))
}
pub fn builtin_str_strip(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_str_upper(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }
pub fn builtin_str_lower(_args: Vec<Value>) -> Result<Value> { Ok(Value::Str("".to_string())) }

// List methods
pub fn builtin_list_append(mut args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("append() takes exactly one argument"));
    }
    
    let item = args.pop().unwrap();
    let list = args.pop().unwrap();
    
    match list {
        Value::List(mut items) => {
            items.append(item);
            Ok(Value::None)
        },
        _ => Err(anyhow::anyhow!("append() method only available for lists"))
    }
}

pub fn builtin_list_extend(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("extend() takes exactly one argument"));
    }
    
    let iterable = args[1].clone();
    let list = args[0].clone();
    
    match (list, iterable) {
        (Value::List(mut items), Value::List(other_items)) => {
            items.extend(other_items);
            Ok(Value::None)
        },
        (Value::List(mut items), Value::Tuple(other_items)) => {
            items.extend(other_items.iter().cloned());
            Ok(Value::None)
        },
        _ => Err(anyhow::anyhow!("extend() argument must be iterable"))
    }
}

pub fn builtin_list_count(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("count() takes exactly one argument"));
    }
    
    let item = &args[1];
    let list = &args[0];
    
    match list {
        Value::List(items) => {
            let count = items.count(item);
            Ok(Value::Int(count as i64))
        },
        _ => Err(anyhow::anyhow!("count() method only available for lists"))
    }
}

pub fn builtin_list_index(args: Vec<Value>) -> Result<Value> {
    if args.len() < 2 || args.len() > 4 {
        return Err(anyhow::anyhow!("index() takes at least 1 argument and at most 3 arguments"));
    }
    
    let item = &args[1];
    let list = &args[0];
    
    match list {
        Value::List(items) => {
            let pos = items.index(item, None, None)
                .map_err(|_| anyhow::anyhow!("list.index(x): x not in list"))?;
            Ok(Value::Int(pos as i64))
        },
        _ => Err(anyhow::anyhow!("index() method only available for lists"))
    }
}

// Dict methods
pub fn builtin_dict_keys(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::List(HPList::new())) 
}

pub fn builtin_dict_values(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::List(HPList::new())) 
}

pub fn builtin_dict_items(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::List(HPList::new())) 
}

pub fn builtin_dict_get(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::None) 
}

// Int methods
pub fn builtin_int_bit_length(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::Int(0)) 
}

// Float methods
pub fn builtin_float_is_integer(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::Bool(false)) 
}

// Tuple methods
pub fn builtin_tuple_count(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::Int(0)) 
}

pub fn builtin_tuple_index(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::Int(0)) 
}

// Set methods
pub fn builtin_set_add(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::None) 
}

pub fn builtin_set_remove(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::None) 
}

// Bytes methods
pub fn builtin_bytes_decode(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::Str("".to_string())) 
}

// Bytearray methods
pub fn builtin_bytearray_append(_args: Vec<Value>) -> Result<Value> { 
    Ok(Value::None) 
}

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
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("object".to_string(), vec![]),
        mro: crate::base_object::MRO::from_linearization(vec!["object".to_string()]),
    })
}

// VM-specific functions with VM context
pub fn builtin_eval_with_vm(_vm: &mut crate::vm::VM, _args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}

pub fn builtin_exec_with_vm(_vm: &mut crate::vm::VM, _args: Vec<Value>) -> Result<Value> {
    Ok(Value::None)
}
