use crate::ast::*;
#[cfg(feature = "ffi")]
use crate::ffi::FFIType;
use crate::object_system::{resolve_dunder_method, call_dunder_method};
use crate::base_object::{BaseObject, MRO, DunderMethod};
use std::collections::HashMap;
use std::fmt;
use std::hash::{Hash, Hasher};

/// Dynamic value supporting optional types with inheritance
#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Int(i64),
    Float(f64),
    Bool(bool),
    Str(String),
    List(Vec<Value>),
    Dict(HashMap<String, Value>),
    Tuple(Vec<Value>),
    Set(Vec<Value>), // Using Vec for simplicity, should be HashSet in production
    Bytes(Vec<u8>),
    ByteArray(Vec<u8>),
    Object {
        class_name: String,
        fields: HashMap<String, Value>,
        base_object: BaseObject,
        mro: MRO,
    },
    Super(String, String), // current class name, parent class name
    Function(String, Vec<String>, Vec<Statement>, Option<String>), // name, parameters, body, docstring
    TypedFunction {
        name: String,
        params: Vec<String>,
        param_types: Vec<Option<Type>>,
        return_type: Option<Type>,
        body: Vec<Statement>,
        docstring: Option<String>,
    }, // Enhanced function with type information
    NativeFunction(fn(Vec<Value>) -> anyhow::Result<Value>),
    BuiltinFunction(String, fn(Vec<Value>) -> anyhow::Result<Value>),
    Module(String, HashMap<String, Value>), // module name, namespace
    #[cfg(feature = "ffi")]
    ExternFunction {
        name: String,
        signature: String,
        return_type: FFIType,
        param_types: Vec<FFIType>,
    },
    None,
    // For optional static typing
    TypedValue { value: Box<Value>, type_info: Type },
}

impl Value {
    /// Create a new object with inheritance support
    pub fn new_object(class_name: String, fields: HashMap<String, Value>, parents: Vec<String>) -> Self {
        let class_mros = HashMap::new(); // This would be populated from a class registry
        let linearization = MRO::compute_c3_linearization(&class_name, &parents, &class_mros)
            .unwrap_or_else(|_| vec![class_name.clone(), "object".to_string()]);
        let mro = MRO::from_linearization(linearization);
        
        Value::Object {
            class_name: class_name.clone(),
            fields,
            base_object: BaseObject::new(class_name, parents),
            mro,
        }
    }

    /// Create built-in type instances that inherit from object
    pub fn new_int(value: i64) -> Self {
        // For now, we'll use the simple Value::Int, but in a full implementation
        // we might want to wrap it in an Object with int-specific methods
        Value::Int(value)
    }

    pub fn new_float(value: f64) -> Self {
        Value::Float(value)
    }

    pub fn new_string(value: String) -> Self {
        Value::Str(value)
    }

    pub fn new_list(items: Vec<Value>) -> Self {
        Value::List(items)
    }

    pub fn new_bool(value: bool) -> Self {
        Value::Bool(value)
    }

    pub fn new_none() -> Self {
        Value::None
    }

    pub fn new_dict(items: HashMap<String, Value>) -> Self {
        Value::Dict(items)
    }

    /// Get dunder method for this value type with inheritance support
    pub fn get_dunder_method(&self, method_name: &str) -> Option<DunderMethod> {
        match self {
            Value::Object { mro, .. } => {
                // For now, we'll use an empty class_methods HashMap since we need to refactor this
                let class_methods = HashMap::new();
                mro.find_method(method_name, &class_methods)
            }
            _ => {
                // For built-in types, get methods from BaseObject
                let base_methods = BaseObject::get_base_methods();
                base_methods.get(method_name).copied()
            }
        }
    }
    
    /// Call a dunder method on this value with inheritance support
    pub fn call_dunder_method(&self, method_name: &str, args: Vec<Value>) -> Option<Value> {
        if let Some(method) = self.get_dunder_method(method_name) {
            method(self, &args).ok()
        } else {
            None
        }
    }

    /// Get the MRO for this value (for objects with inheritance)
    pub fn get_mro(&self) -> Option<&MRO> {
        match self {
            Value::Object { mro, .. } => Some(mro),
            _ => None,
        }
    }

    /// Enhanced isinstance that uses MRO for inheritance checking
    pub fn isinstance(&self, expected_type: &str) -> bool {
        crate::type_hierarchy::TypeHierarchy::isinstance(self, expected_type)
    }

    /// Get a string representation for debugging
    pub fn debug_string(&self) -> String {
        match self {
            Value::Int(n) => n.to_string(),
            Value::Float(n) => format!("{:.6}", n),
            Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
            Value::Str(s) => format!("\"{}\"", s),
            Value::List(items) => {
                let items_str: Vec<String> = items.iter()
                    .take(5) // Limit to prevent deep recursion
                    .map(|v| match v {
                        Value::Str(s) => format!("\"{}\"", s),
                        Value::Int(n) => n.to_string(),
                        Value::Float(n) => format!("{:.6}", n),
                        Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
                        Value::None => "None".to_string(),
                        _ => "<complex value>".to_string(), // Avoid deep recursion
                    })
                    .collect();
                let suffix = if items.len() > 5 { ", ..." } else { "" };
                format!("[{}{}]", items_str.join(", "), suffix)
            }
            Value::Tuple(items) => {
                let items_str: Vec<String> = items.iter()
                    .take(5) // Limit to prevent deep recursion
                    .map(|v| match v {
                        Value::Str(s) => format!("\"{}\"", s),
                        Value::Int(n) => n.to_string(),
                        Value::Float(n) => format!("{:.6}", n),
                        Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
                        Value::None => "None".to_string(),
                        _ => "<complex value>".to_string(), // Avoid deep recursion
                    })
                    .collect();
                let suffix = if items.len() > 5 { ", ..." } else { "" };
                format!("({}{})", items_str.join(", "), suffix)
            }
            Value::Set(items) => {
                let items_str: Vec<String> = items.iter()
                    .take(5) // Limit to prevent deep recursion
                    .map(|v| match v {
                        Value::Str(s) => format!("\"{}\"", s),
                        Value::Int(n) => n.to_string(),
                        Value::Float(n) => format!("{:.6}", n),
                        Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
                        Value::None => "None".to_string(),
                        _ => "<complex value>".to_string(), // Avoid deep recursion
                    })
                    .collect();
                let suffix = if items.len() > 5 { ", ..." } else { "" };
                format!("{{{}{}}}", items_str.join(", "), suffix)
            }
            Value::Bytes(bytes) => {
                format!("b\"{}\"", String::from_utf8_lossy(bytes))
            }
            Value::ByteArray(bytes) => {
                format!("bytearray(b\"{}\")", String::from_utf8_lossy(bytes))
            }
            Value::Dict(dict) => {
                let pairs: Vec<String> = dict.iter()
                    .take(3) // Limit to prevent deep recursion
                    .map(|(k, v)| {
                        let v_str = match v {
                            Value::Str(s) => format!("\"{}\"", s),
                            Value::Int(n) => n.to_string(),
                            Value::Float(n) => format!("{:.6}", n),
                            Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
                            Value::None => "None".to_string(),
                            _ => "<complex value>".to_string(), // Avoid deep recursion
                        };
                        format!("{}: {}", k, v_str)
                    })
                    .collect();
                format!("{{{}}}", pairs.join(", "))
            }
            Value::None => "None".to_string(),
            Value::Function(name, params, _, _) => {
                format!("<function {}({})>", name, params.join(", "))
            }
            Value::TypedFunction { name, params, .. } => {
                format!("<typed function {}({})>", name, params.join(", "))
            }
            Value::BuiltinFunction(name, _) => {
                format!("<built-in function {}>", name)
            }
            Value::NativeFunction(_) => "<native function>".to_string(),
            Value::Object { class_name, fields, .. } => {
                 format!("<{} object with {} fields>", class_name, fields.len())
             }
             Value::Super(current_class, parent_class) => {
                 format!("<super: {} -> {}>", current_class, parent_class)
             }
             Value::Module(name, namespace) => {
                 format!("<module '{}' with {} items>", name, namespace.len())
             }
             Value::TypedValue { value, type_info } => {
                 // Prevent recursion by handling the inner value safely
                 let value_str = match value.as_ref() {
                     Value::Str(s) => format!("\"{}\"", s),
                     Value::Int(n) => n.to_string(),
                     Value::Float(n) => format!("{:.6}", n),
                     Value::Bool(b) => b.to_string(),
                     Value::None => "None".to_string(),
                     _ => "<complex value>".to_string(),
                 };
                 format!("{}: {}", value_str, type_info)
             }
             #[cfg(feature = "ffi")]
             Value::ExternFunction { name, .. } => {
                 format!("<extern function {}>", name)
             }
        }
    }
    
    /// Get type name for error messages
    pub fn type_name(&self) -> &str {
        match self {
            Value::Int(_) => "int",
            Value::Float(_) => "float",
            Value::Bool(_) => "bool",
            Value::Str(_) => "str",
            Value::List(_) => "list",
            Value::Dict(_) => "dict",
            Value::Tuple(_) => "tuple",
            Value::Set(_) => "set",
            Value::Bytes(_) => "bytes",
            Value::ByteArray(_) => "bytearray",
            Value::Object { class_name, .. } => class_name,
            Value::Super(_, _) => "super",
            Value::Function(_, _, _, _) => "function",
            Value::TypedFunction { .. } => "typed function",
            Value::BuiltinFunction(_, _) => "builtin function",
            Value::NativeFunction(_) => "native function",
            Value::Module(_, _) => "module",
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => "extern function",
            Value::None => "None",
            Value::TypedValue { value, .. } => value.type_name(),
        }
    }
    
    /// Dynamic type checking for optional static typing
    pub fn check_type(&self, expected: &Type) -> bool {
        match (self, expected) {
            (Value::Int(_), Type::Simple(name)) if name == "int" => true,
            (Value::Float(_), Type::Simple(name)) if name == "float" => true,
            (Value::Bool(_), Type::Simple(name)) if name == "bool" => true,
            (Value::Str(_), Type::Simple(name)) if name == "str" => true,
            (Value::List(_), Type::Simple(name)) if name == "list" => true,
            (Value::Dict(_), Type::Simple(name)) if name == "dict" => true,
            (Value::Tuple(_), Type::Simple(name)) if name == "tuple" => true,
            (Value::Set(_), Type::Simple(name)) if name == "set" => true,
            (Value::Bytes(_), Type::Simple(name)) if name == "bytes" => true,
            (Value::ByteArray(_), Type::Simple(name)) if name == "bytearray" => true,
            (Value::Module(_,_), Type::Simple(name)) if name == "module" => true,
            (Value::Function(_, _, _, _), Type::Simple(name)) if name == "function" => true,
            (Value::TypedFunction { .. }, Type::Simple(name)) if name == "function" => true,
            (Value::BuiltinFunction(_, _), Type::Simple(name)) if name == "function" => true,
            (Value::NativeFunction(_), Type::Simple(name)) if name == "function" => true,
            (Value::Object { class_name, .. }, Type::Simple(name)) if class_name == name => true,
            (Value::None, Type::Simple(name)) if name == "None" => true,
            (Value::None, Type::Any) => true,
            (Value::TypedValue { type_info, .. }, expected_type) => type_info == expected_type,
            (_, Type::Any) => true, // Any accepts all types
            // Handle Optional types
            (Value::None, Type::Optional(_)) => true,
            (value, Type::Optional(inner_type)) => value.check_type(inner_type),
            // Handle Union types
            (value, Type::Union(types)) => types.iter().any(|t| value.check_type(t)),
            _ => false, // Type mismatch
        }
    }

    /// Get the Type representation of this value
    pub fn get_type(&self) -> Type {
        match self {
            Value::Int(_) => Type::Simple("int".to_string()),
            Value::Float(_) => Type::Simple("float".to_string()),
            Value::Bool(_) => Type::Simple("bool".to_string()),
            Value::Str(_) => Type::Simple("str".to_string()),
            Value::List(_) => Type::Simple("list".to_string()),
            Value::Dict(_) => Type::Simple("dict".to_string()),
            Value::Tuple(_) => Type::Simple("tuple".to_string()),
            Value::Set(_) => Type::Simple("set".to_string()),
            Value::Bytes(_) => Type::Simple("bytes".to_string()),
            Value::ByteArray(_) => Type::Simple("bytearray".to_string()),
            Value::Module(_, _) => Type::Simple("module".to_string()),
            Value::Function(_, _, _, _) => Type::Simple("function".to_string()),
            Value::TypedFunction { .. } => Type::Simple("function".to_string()),
            Value::BuiltinFunction(_, _) => Type::Simple("function".to_string()),
            Value::NativeFunction(_) => Type::Simple("function".to_string()),
            Value::Object { class_name, .. } => Type::Simple(class_name.clone()),
            Value::None => Type::Simple("None".to_string()),
            Value::TypedValue { type_info, .. } => type_info.clone(),
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => Type::Simple("function".to_string()),
            Value::Super(_, _) => Type::Simple("super".to_string()),
        }
    }

    /// Attempt to convert this value to the specified type
    pub fn convert_to_type(&self, target_type: &Type) -> anyhow::Result<Value> {
        // If already the correct type, return as-is
        if self.check_type(target_type) {
            return Ok(self.clone());
        }

        match target_type {
            Type::Simple(type_name) => {
                match type_name.as_str() {
                    "int" => self.to_int(),
                    "float" => self.to_float(),
                    "bool" => self.to_bool(),
                    "str" => self.to_str(),
                    "list" => self.to_list(),
                    "tuple" => self.to_tuple(),
                    "set" => self.to_set(),
                    _ => Err(anyhow::anyhow!("Cannot convert {} to {}", self.type_name(), type_name)),
                }
            }
            Type::Optional(inner_type) => {
                if matches!(self, Value::None) {
                    Ok(Value::None)
                } else {
                    self.convert_to_type(inner_type)
                }
            }
            Type::Union(types) => {
                // Try to convert to the first compatible type
                for ty in types {
                    if let Ok(converted) = self.convert_to_type(ty) {
                        return Ok(converted);
                    }
                }
                Err(anyhow::anyhow!("Cannot convert {} to any type in union", self.type_name()))
            }
            _ => Err(anyhow::anyhow!("Complex type conversion not supported: {}", target_type)),
        }
    }

    /// Convert to int (similar to Python's int() function)
    pub fn to_int(&self) -> anyhow::Result<Value> {
        match self {
            Value::Int(n) => Ok(Value::Int(*n)),
            Value::Float(f) => Ok(Value::Int(*f as i64)),
            Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
            Value::Str(s) => {
                s.trim().parse::<i64>()
                    .map(Value::Int)
                    .map_err(|_| anyhow::anyhow!("invalid literal for int() with base 10: '{}'", s))
            }
            _ => Err(anyhow::anyhow!("int() argument must be a string, a bytes-like object or a number, not '{}'", self.type_name())),
        }
    }

    /// Convert to float (similar to Python's float() function)
    pub fn to_float(&self) -> anyhow::Result<Value> {
        match self {
            Value::Float(f) => Ok(Value::Float(*f)),
            Value::Int(n) => Ok(Value::Float(*n as f64)),
            Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
            Value::Str(s) => {
                s.trim().parse::<f64>()
                    .map(Value::Float)
                    .map_err(|_| anyhow::anyhow!("could not convert string to float: '{}'", s))
            }
            _ => Err(anyhow::anyhow!("float() argument must be a string or a number, not '{}'", self.type_name())),
        }
    }

    /// Convert to bool (similar to Python's bool() function)
    pub fn to_bool(&self) -> anyhow::Result<Value> {
        Ok(Value::Bool(self.is_truthy()))
    }

    /// Convert to str (similar to Python's str() function)
    pub fn to_str(&self) -> anyhow::Result<Value> {
        Ok(Value::Str(self.to_string()))
    }

    /// Convert to list (similar to Python's list() function)
    pub fn to_list(&self) -> anyhow::Result<Value> {
        match self {
            Value::List(items) => Ok(Value::List(items.clone())),
            Value::Tuple(items) => Ok(Value::List(items.clone())),
            Value::Set(items) => Ok(Value::List(items.clone())),
            Value::Str(s) => {
                let chars: Vec<Value> = s.chars().map(|c| Value::Str(c.to_string())).collect();
                Ok(Value::List(chars))
            }
            Value::Bytes(bytes) => {
                let items: Vec<Value> = bytes.iter().map(|&b| Value::Int(b as i64)).collect();
                Ok(Value::List(items))
            }
            Value::ByteArray(bytes) => {
                let items: Vec<Value> = bytes.iter().map(|&b| Value::Int(b as i64)).collect();
                Ok(Value::List(items))
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", self.type_name())),
        }
    }

    /// Convert to tuple (similar to Python's tuple() function)
    pub fn to_tuple(&self) -> anyhow::Result<Value> {
        match self {
            Value::Tuple(items) => Ok(Value::Tuple(items.clone())),
            Value::List(items) => Ok(Value::Tuple(items.clone())),
            Value::Set(items) => Ok(Value::Tuple(items.clone())),
            Value::Str(s) => {
                let chars: Vec<Value> = s.chars().map(|c| Value::Str(c.to_string())).collect();
                Ok(Value::Tuple(chars))
            }
            Value::Bytes(bytes) => {
                let items: Vec<Value> = bytes.iter().map(|&b| Value::Int(b as i64)).collect();
                Ok(Value::Tuple(items))
            }
            Value::ByteArray(bytes) => {
                let items: Vec<Value> = bytes.iter().map(|&b| Value::Int(b as i64)).collect();
                Ok(Value::Tuple(items))
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", self.type_name())),
        }
    }

    /// Convert to set (similar to Python's set() function)
    pub fn to_set(&self) -> anyhow::Result<Value> {
        match self {
            Value::Set(items) => Ok(Value::Set(items.clone())),
            Value::List(items) => {
                // Remove duplicates (simple implementation)
                let mut unique_items = Vec::new();
                for item in items {
                    if !unique_items.contains(item) {
                        unique_items.push(item.clone());
                    }
                }
                Ok(Value::Set(unique_items))
            }
            Value::Tuple(items) => {
                // Remove duplicates (simple implementation)
                let mut unique_items = Vec::new();
                for item in items {
                    if !unique_items.contains(item) {
                        unique_items.push(item.clone());
                    }
                }
                Ok(Value::Set(unique_items))
            }
            Value::Str(s) => {
                let mut unique_chars = Vec::new();
                for c in s.chars() {
                    let char_val = Value::Str(c.to_string());
                    if !unique_chars.contains(&char_val) {
                        unique_chars.push(char_val);
                    }
                }
                Ok(Value::Set(unique_chars))
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", self.type_name())),
        }
    }
    
    /// Convert to boolean for truthiness testing
    pub fn is_truthy(&self) -> bool {
        match self {
            Value::Bool(b) => *b,
            Value::Int(n) => *n != 0,
            Value::Float(n) => *n != 0.0,
            Value::Str(s) => !s.is_empty(),
            Value::List(items) => !items.is_empty(),
            Value::Dict(dict) => !dict.is_empty(),
            Value::Tuple(items) => !items.is_empty(),
            Value::Set(items) => !items.is_empty(),
            Value::Bytes(bytes) => !bytes.is_empty(),
            Value::ByteArray(bytes) => !bytes.is_empty(),
            Value::None => false,
            Value::Object { .. } => true,
            Value::Super(_, _) => true,
            Value::Function(_, _, _, _) => true,
            Value::TypedFunction { .. } => true,
            Value::BuiltinFunction(_, _) => true,
            Value::NativeFunction(_) => true,
            Value::Module(_, _) => true,
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => true,
            Value::TypedValue { value, .. } => value.is_truthy(),
        }
    }
}

impl PartialOrd for Value {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        match (self, other) {
            (Value::Int(a), Value::Int(b)) => a.partial_cmp(b),
            (Value::Float(a), Value::Float(b)) => a.partial_cmp(b),
            (Value::Int(a), Value::Float(b)) => (*a as f64).partial_cmp(b),
            (Value::Float(a), Value::Int(b)) => a.partial_cmp(&(*b as f64)),
            (Value::Str(a), Value::Str(b)) => a.partial_cmp(b),
            (Value::Bool(a), Value::Bool(b)) => a.partial_cmp(b),
            _ => None,
        }
    }
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Value::Int(n) => write!(f, "{}", n),
            Value::Float(n) => write!(f, "{:.6}", n),
            Value::Bool(b) => write!(f, "{}", if *b { "True" } else { "False" }),
            Value::Str(s) => write!(f, "{}", s), // No quotes for display
            Value::List(items) => {
                let items_str: Vec<String> = items.iter().map(|v| format!("{}", v)).collect();
                write!(f, "[{}]", items_str.join(", "))
            }
            Value::Tuple(items) => {
                let items_str: Vec<String> = items.iter().map(|v| format!("{}", v)).collect();
                if items.len() == 1 {
                    write!(f, "({},)", items_str[0])
                } else {
                    write!(f, "({})", items_str.join(", "))
                }
            }
            Value::Set(items) => {
                let items_str: Vec<String> = items.iter().map(|v| format!("{}", v)).collect();
                write!(f, "{{{}}}", items_str.join(", "))
            }
            Value::Bytes(bytes) => {
                write!(f, "b'{}'", String::from_utf8_lossy(bytes))
            }
            Value::ByteArray(bytes) => {
                write!(f, "bytearray(b'{}')", String::from_utf8_lossy(bytes))
            }
            Value::Dict(dict) => {
                let pairs: Vec<String> = dict.iter()
                    .map(|(k, v)| format!("{}: {}", k, v))
                    .collect();
                write!(f, "{{{}}}", pairs.join(", "))
            }
            Value::None => write!(f, "None"),
            Value::Function(name, params, _, _) => {
                write!(f, "<function {}({})>", name, params.join(", "))
            }
            Value::TypedFunction { name, params, .. } => {
                write!(f, "<typed function {}({})>", name, params.join(", "))
            }
            Value::BuiltinFunction(name, _) => {
                write!(f, "<built-in function {}>", name)
            }
            Value::NativeFunction(_) => write!(f, "<native function>"),
            Value::Object { class_name, fields, .. } => {
                 write!(f, "<{} object with {} fields>", class_name, fields.len())
             }
            Value::Super(current_class, parent_class) => {
                write!(f, "<super: {} -> {}>", current_class, parent_class)
            }
            Value::Module(name, namespace) => {
                write!(f, "<module '{}' with {} items>", name, namespace.len())
            }
            #[cfg(feature = "ffi")]
            Value::ExternFunction { name, signature, .. } => {
                write!(f, "<extern function {} with signature {}>", name, signature)
            }
            Value::TypedValue { value, type_info } => {
                write!(f, "{}: {}", value, type_info)
            }
            Value::Super(current_class, parent_class) => {
                write!(f, "<super: {} -> {}>", current_class, parent_class)
            }
        }
    }
}

impl Value {
    /// Get method for a value type
    pub fn get_method(&self, method_name: &str) -> Option<fn(Vec<Value>) -> anyhow::Result<Value>> {
        match self {
            Value::Str(_) => match method_name {
                "join" => Some(crate::builtins::builtin_str_join),
                "split" => Some(crate::builtins::builtin_str_split),
                "strip" => Some(crate::builtins::builtin_str_strip),
                "upper" => Some(crate::builtins::builtin_str_upper),
                "lower" => Some(crate::builtins::builtin_str_lower),
                _ => None,
            },
            Value::List(_) => match method_name {
                "append" => Some(crate::builtins::builtin_list_append),
                "extend" => Some(crate::builtins::builtin_list_extend),
                "count" => Some(crate::builtins::builtin_list_count),
                "index" => Some(crate::builtins::builtin_list_index),
                _ => None,
            },
            Value::Dict(_) => match method_name {
                "keys" => Some(crate::builtins::builtin_dict_keys),
                "values" => Some(crate::builtins::builtin_dict_values),
                "items" => Some(crate::builtins::builtin_dict_items),
                "get" => Some(crate::builtins::builtin_dict_get),
                _ => None,
            },
            Value::Int(_) => match method_name {
                "bit_length" => Some(crate::builtins::builtin_int_bit_length),
                _ => None,
            },
            Value::Float(_) => match method_name {
                "is_integer" => Some(crate::builtins::builtin_float_is_integer),
                _ => None,
            },
            Value::Tuple(_) => match method_name {
                "count" => Some(crate::builtins::builtin_tuple_count),
                "index" => Some(crate::builtins::builtin_tuple_index),
                _ => None,
            },
            Value::Set(_) => match method_name {
                "add" => Some(crate::builtins::builtin_set_add),
                "remove" => Some(crate::builtins::builtin_set_remove),
                _ => None,
            },
            Value::Bytes(_) => match method_name {
                "decode" => Some(crate::builtins::builtin_bytes_decode),
                _ => None,
            },
            Value::ByteArray(_) => match method_name {
                "append" => Some(crate::builtins::builtin_bytearray_append),
                _ => None,
            },
            _ => None,
        }
    }

    /// Convert a Value back to an Expr for decorator application
    pub fn to_expr(&self) -> Expr {
        match self {
            Value::Int(n) => Expr::Literal(Literal::Int(*n)),
            Value::Float(n) => Expr::Literal(Literal::Float(*n)),
            Value::Bool(b) => Expr::Literal(Literal::Bool(*b)),
            Value::Str(s) => Expr::Literal(Literal::String(s.clone())),
            Value::None => Expr::Literal(Literal::None),
            Value::List(items) => {
                let exprs: Vec<Expr> = items.iter().map(|v| v.to_expr()).collect();
                Expr::List(exprs)
            }
            Value::Tuple(items) => {
                let exprs: Vec<Expr> = items.iter().map(|v| v.to_expr()).collect();
                Expr::Tuple(exprs)
            }
            Value::Dict(dict) => {
                let pairs: Vec<(Expr, Expr)> = dict.iter()
                    .map(|(k, v)| (Expr::Literal(Literal::String(k.clone())), v.to_expr()))
                    .collect();
                Expr::Dict(pairs)
            }
            Value::Set(items) => {
                let exprs: Vec<Expr> = items.iter().map(|v| v.to_expr()).collect();
                Expr::Set(exprs)
            }
            Value::Function(name, _, _, _) => Expr::Identifier(name.clone()),
            Value::TypedFunction { name, .. } => Expr::Identifier(name.clone()),
            Value::BuiltinFunction(name, _) => Expr::Identifier(name.clone()),
            Value::NativeFunction(_) => Expr::Identifier("native_function".to_string()),
            Value::Object { class_name, .. } => Expr::Identifier(class_name.clone()),
            Value::Module(name, _) => Expr::Identifier(name.clone()),
            Value::Bytes(bytes) => {
                // Convert bytes to a bytes literal expression
                Expr::Literal(Literal::String(format!("b'{}'", String::from_utf8_lossy(bytes))))
            }
            Value::ByteArray(bytes) => {
                // Convert bytearray to a bytearray call expression
                Expr::Call {
                    func: Box::new(Expr::Identifier("bytearray".to_string())),
                    args: vec![Expr::Literal(Literal::String(format!("b'{}'", String::from_utf8_lossy(bytes))))],
                    kwargs: vec![],
                }
            }
            Value::TypedValue { value, .. } => value.to_expr(),
            #[cfg(feature = "ffi")]
            Value::ExternFunction { name, .. } => Expr::Identifier(name.clone()),
            Value::Super(current_class, _) => Expr::Identifier(format!("super_{}", current_class)),
        }
    }
}

impl Eq for Value {}

impl Hash for Value {
    fn hash<H: Hasher>(&self, state: &mut H) {
        match self {
            Value::Int(i) => {
                0u8.hash(state);
                i.hash(state);
            }
            Value::Float(f) => {
                1u8.hash(state);
                // For floats, we need to handle NaN and special cases
                if f.is_nan() {
                    "NaN".hash(state);
                } else if f.is_infinite() {
                    if f.is_sign_positive() {
                        "inf".hash(state);
                    } else {
                        "-inf".hash(state);
                    }
                } else {
                    f.to_bits().hash(state);
                }
            }
            Value::Bool(b) => {
                2u8.hash(state);
                b.hash(state);
            }
            Value::Str(s) => {
                3u8.hash(state);
                s.hash(state);
            }
            Value::List(items) => {
                4u8.hash(state);
                items.hash(state);
            }
            Value::Dict(map) => {
                5u8.hash(state);
                // For HashMap, we need to hash in a deterministic order
                let mut pairs: Vec<_> = map.iter().collect();
                pairs.sort_by_key(|(k, _)| *k);
                pairs.hash(state);
            }
            Value::Tuple(items) => {
                6u8.hash(state);
                items.hash(state);
            }
            Value::Set(items) => {
                7u8.hash(state);
                // For sets, we need to hash in a deterministic order
                let mut sorted_items = items.clone();
                sorted_items.sort_by(|a, b| {
                    // Simple comparison for sorting - this is a basic implementation
                    format!("{:?}", a).cmp(&format!("{:?}", b))
                });
                sorted_items.hash(state);
            }
            Value::Bytes(bytes) => {
                8u8.hash(state);
                bytes.hash(state);
            }
            Value::ByteArray(bytes) => {
                9u8.hash(state);
                bytes.hash(state);
            }
            Value::Object { class_name, fields, .. } => {
                 10u8.hash(state);
                 class_name.hash(state);
                 let mut pairs: Vec<_> = fields.iter().collect();
                 pairs.sort_by_key(|(k, _)| *k);
                 pairs.hash(state);
             }
            Value::Super(current_class, parent_class) => {
                19u8.hash(state);
                current_class.hash(state);
                parent_class.hash(state);
            }
            Value::Function(name, params, _, _) => {
                11u8.hash(state);
                name.hash(state);
                params.hash(state);
                // We can't hash the body (statements) easily, so we just use name and params
            }
            Value::TypedFunction { name, params, .. } => {
                18u8.hash(state);
                name.hash(state);
                params.hash(state);
                // We can't hash the body or types easily, so we just use name and params
            }
            Value::NativeFunction(_) => {
                12u8.hash(state);
                // Function pointers can't be hashed directly, so we use a constant
                "native_function".hash(state);
            }
            Value::BuiltinFunction(name, _) => {
                13u8.hash(state);
                name.hash(state);
            }
            Value::Module(name, namespace) => {
                14u8.hash(state);
                name.hash(state);
                let mut pairs: Vec<_> = namespace.iter().collect();
                pairs.sort_by_key(|(k, _)| *k);
                pairs.hash(state);
            }
            #[cfg(feature = "ffi")]
            Value::ExternFunction { name, signature, .. } => {
                15u8.hash(state);
                name.hash(state);
                signature.hash(state);
            }
            Value::None => {
                16u8.hash(state);
            }
            Value::TypedValue { value, type_info } => {
                17u8.hash(state);
                value.hash(state);
                // We can't easily hash type_info, so we use its debug representation
                format!("{:?}", type_info).hash(state);
            }
        }
    }
}
