use crate::ast::*;
#[cfg(feature = "ffi")]
use crate::ffi::FFIType;
use crate::object_system::{resolve_dunder_method, call_dunder_method};
use std::collections::HashMap;
use std::fmt;
use std::hash::{Hash, Hasher};

/// Dynamic value supporting optional types
#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Int(i64),
    Float(f64),
    Bool(bool),
    String(String),
    List(Vec<Value>),
    Dict(HashMap<String, Value>),
    Tuple(Vec<Value>),
    Set(Vec<Value>), // Using Vec for simplicity, should be HashSet in production
    Bytes(Vec<u8>),
    ByteArray(Vec<u8>),
    Object(String, HashMap<String, Value>), // class name, fields
    Function(String, Vec<String>, Vec<Statement>), // name, parameters, body
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
    /// Get dunder method for this value type
    pub fn get_dunder_method(&self, method_name: &str) -> Option<fn(&Value, Vec<Value>) -> Option<Value>> {
        resolve_dunder_method(self, method_name)
    }
    
    /// Call a dunder method on this value
    pub fn call_dunder_method(&self, method_name: &str, args: &[Value]) -> Option<Value> {
        call_dunder_method(self, method_name, args.to_vec())
    }

    /// Get a string representation for debugging
    pub fn debug_string(&self) -> String {
        match self {
            Value::Int(n) => n.to_string(),
            Value::Float(n) => format!("{:.6}", n),
            Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
            Value::String(s) => format!("\"{}\"", s),
            Value::List(items) => {
                let items_str: Vec<String> = items.iter()
                    .take(5) // Limit to prevent deep recursion
                    .map(|v| match v {
                        Value::String(s) => format!("\"{}\"", s),
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
                        Value::String(s) => format!("\"{}\"", s),
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
                        Value::String(s) => format!("\"{}\"", s),
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
                            Value::String(s) => format!("\"{}\"", s),
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
            Value::Function(name, params, _) => {
                format!("<function {}({})>", name, params.join(", "))
            }
            Value::BuiltinFunction(name, _) => {
                format!("<built-in function {}>", name)
            }
            Value::NativeFunction(_) => "<native function>".to_string(),
            Value::Object(name, fields) => {
                 format!("<{} object with {} fields>", name, fields.len())
             }
             Value::Module(name, namespace) => {
                 format!("<module '{}' with {} items>", name, namespace.len())
             }
             Value::TypedValue { value, type_info } => {
                 // Prevent recursion by handling the inner value safely
                 let value_str = match value.as_ref() {
                     Value::String(s) => format!("\"{}\"", s),
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
    pub fn type_name(&self) -> &'static str {
        match self {
            Value::Int(_) => "int",
            Value::Float(_) => "float",
            Value::Bool(_) => "bool",
            Value::String(_) => "str",
            Value::List(_) => "list",
            Value::Dict(_) => "dict",
            Value::Tuple(_) => "tuple",
            Value::Set(_) => "set",
            Value::Bytes(_) => "bytes",
            Value::ByteArray(_) => "bytearray",
            Value::Object(_, _) => "object",
            Value::Function(_, _, _) => "function",
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
            (Value::String(_), Type::Simple(name)) if name == "str" => true,
            (Value::List(_), Type::Simple(name)) if name == "list" => true,
            (Value::Dict(_), Type::Simple(name)) if name == "dict" => true,
            (Value::Tuple(_), Type::Simple(name)) if name == "tuple" => true,
            (Value::Set(_), Type::Simple(name)) if name == "set" => true,
            (Value::Bytes(_), Type::Simple(name)) if name == "bytes" => true,
            (Value::ByteArray(_), Type::Simple(name)) if name == "bytearray" => true,
            (Value::Module(_,_), Type::Simple(name)) if name == "module" => true,
            (Value::None, Type::Any) => true,
            (Value::TypedValue { type_info, .. }, expected_type) => type_info == expected_type,
            (_, Type::Any) => true, // Any accepts all types
            _ => false, // Type mismatch
        }
    }
    
    /// Convert to boolean for truthiness testing
    pub fn is_truthy(&self) -> bool {
        match self {
            Value::Bool(b) => *b,
            Value::Int(n) => *n != 0,
            Value::Float(n) => *n != 0.0,
            Value::String(s) => !s.is_empty(),
            Value::List(items) => !items.is_empty(),
            Value::Dict(dict) => !dict.is_empty(),
            Value::Tuple(items) => !items.is_empty(),
            Value::Set(items) => !items.is_empty(),
            Value::Bytes(bytes) => !bytes.is_empty(),
            Value::ByteArray(bytes) => !bytes.is_empty(),
            Value::None => false,
            Value::Object(_, _) => true,
            Value::Function(_, _, _) => true,
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
            (Value::String(a), Value::String(b)) => a.partial_cmp(b),
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
            Value::String(s) => write!(f, "{}", s), // No quotes for display
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
            Value::Function(name, params, _) => {
                write!(f, "<function {}({})>", name, params.join(", "))
            }
            Value::BuiltinFunction(name, _) => {
                write!(f, "<built-in function {}>", name)
            }
            Value::NativeFunction(_) => write!(f, "<native function>"),
            Value::Object(name, fields) => {
                write!(f, "<{} object with {} fields>", name, fields.len())
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
        }
    }
}

impl Value {
    /// Get method for a value type
    pub fn get_method(&self, method_name: &str) -> Option<fn(Vec<Value>) -> anyhow::Result<Value>> {
        match self {
            Value::String(_) => match method_name {
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
            Value::String(s) => Expr::Literal(Literal::String(s.clone())),
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
            Value::Function(name, _, _) => Expr::Identifier(name.clone()),
            Value::BuiltinFunction(name, _) => Expr::Identifier(name.clone()),
            Value::NativeFunction(_) => Expr::Identifier("native_function".to_string()),
            Value::Object(class_name, _) => Expr::Identifier(class_name.clone()),
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
            Value::String(s) => {
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
            Value::Object(class_name, fields) => {
                10u8.hash(state);
                class_name.hash(state);
                let mut pairs: Vec<_> = fields.iter().collect();
                pairs.sort_by_key(|(k, _)| *k);
                pairs.hash(state);
            }
            Value::Function(name, params, _) => {
                11u8.hash(state);
                name.hash(state);
                params.hash(state);
                // We can't hash the body (statements) easily, so we just use name and params
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