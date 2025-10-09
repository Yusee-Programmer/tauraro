use crate::ast::*;
#[cfg(feature = "ffi")]
use crate::ffi::FFIType;
use crate::base_object::{BaseObject, MRO, DunderMethod};
use crate::ast::{Param, Statement, Type, Expr, Literal};
use std::collections::HashMap;
use std::fmt;
use std::hash::{Hash, Hasher};

// Import HPList
use crate::modules::hplist::HPList;

/// Dynamic value supporting optional types with inheritance
#[derive(Clone)]
pub enum Value {
    Int(i64),
    Float(f64),
    Complex { real: f64, imag: f64 },
    Bool(bool),
    Str(String),
    List(HPList),  // Changed from Vec<Value> to HPList
    Dict(HashMap<String, Value>),
    Tuple(Vec<Value>),
    Set(Vec<Value>), // Using Vec for simplicity, should be HashSet in production
    FrozenSet(Vec<Value>), // Immutable set
    Range { start: i64, stop: i64, step: i64 }, // Arithmetic sequence
    RangeIterator { start: i64, stop: i64, step: i64, current: i64 }, // Range iterator
    Bytes(Vec<u8>),
    ByteArray(Vec<u8>),
    MemoryView { data: Vec<u8>, format: String, shape: Vec<usize> }, // Memory buffer view
    Ellipsis,
    NotImplemented,
    Object {
        class_name: String,
        fields: HashMap<String, Value>,
        class_methods: HashMap<String, Value>,
        base_object: BaseObject,
        mro: MRO,
    },
    Class {
        name: String,
        bases: Vec<String>, // Base class names
        methods: HashMap<String, Value>, // Class methods, attributes, and __init__, __new__, etc.
        metaclass: Option<String>, // Metaclass name (usually 'type')
        mro: MRO,
        base_object: BaseObject,
    },
    Super(String, String, Option<Box<Value>>), // current class name, parent class name, object instance
    Closure {
        name: String,
        params: Vec<Param>,
        body: Vec<Statement>,
        captured_scope: HashMap<String, Value>,
        docstring: Option<String>,
        // Add a field to store the compiled code directly in the Closure
        compiled_code: Option<Box<crate::bytecode::arithmetic::CodeObject>>,
    },
    Code(Box<crate::bytecode::arithmetic::CodeObject>), // Compiled function code
    NativeFunction(fn(Vec<Value>) -> anyhow::Result<Value>),
    BuiltinFunction(String, fn(Vec<Value>) -> anyhow::Result<Value>),
    Module(String, HashMap<String, Value>), // module name, namespace
    BoundMethod {
        object: Box<Value>,
        method_name: String,
    },
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

// Manual implementation of PartialEq trait for Value enum
impl PartialEq for Value {
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Complex { real: real_a, imag: imag_a }, Value::Complex { real: real_b, imag: imag_b }) => real_a == real_b && imag_a == imag_b,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::Str(a), Value::Str(b)) => a == b,
            (Value::List(a), Value::List(b)) => a == b,
            (Value::Dict(a), Value::Dict(b)) => a == b,
            (Value::Tuple(a), Value::Tuple(b)) => a == b,
            (Value::Set(a), Value::Set(b)) => a == b,
            (Value::FrozenSet(a), Value::FrozenSet(b)) => a == b,
            (Value::Range { start: start_a, stop: stop_a, step: step_a }, Value::Range { start: start_b, stop: stop_b, step: step_b }) => start_a == start_b && stop_a == stop_b && step_a == step_b,
            (Value::Bytes(a), Value::Bytes(b)) => a == b,
            (Value::ByteArray(a), Value::ByteArray(b)) => a == b,
            (Value::MemoryView { data: data_a, format: format_a, shape: shape_a }, Value::MemoryView { data: data_b, format: format_b, shape: shape_b }) => data_a == data_b && format_a == format_b && shape_a == shape_b,
            (Value::Ellipsis, Value::Ellipsis) => true,
            (Value::NotImplemented, Value::NotImplemented) => true,
            (Value::Object { class_name: class_name_a, fields: fields_a, class_methods: class_methods_a, base_object: base_object_a, mro: mro_a }, Value::Object { class_name: class_name_b, fields: fields_b, class_methods: class_methods_b, base_object: base_object_b, mro: mro_b }) => class_name_a == class_name_b && fields_a == fields_b && class_methods_a == class_methods_b && base_object_a == base_object_b && mro_a == mro_b,
            (Value::Class { name: name_a, bases: bases_a, methods: methods_a, metaclass: metaclass_a, mro: mro_a, base_object: base_object_a }, Value::Class { name: name_b, bases: bases_b, methods: methods_b, metaclass: metaclass_b, mro: mro_b, base_object: base_object_b }) => name_a == name_b && bases_a == bases_b && methods_a == methods_b && metaclass_a == metaclass_b && mro_a == mro_b && base_object_a == base_object_b,
            (Value::Super(current_class_a, parent_class_a, obj_a), Value::Super(current_class_b, parent_class_b, obj_b)) => current_class_a == current_class_b && parent_class_a == parent_class_b && obj_a == obj_b,
            // For Closure, we compare name, params, and compiled_code
            (Value::Closure { name: name_a, params: params_a, compiled_code: code_a, .. }, Value::Closure { name: name_b, params: params_b, compiled_code: code_b, .. }) => name_a == name_b && params_a == params_b && code_a == code_b,
            (Value::Code(a), Value::Code(b)) => a == b,
            (Value::NativeFunction(_), Value::NativeFunction(_)) => false, // Function pointers can't be compared
            (Value::BuiltinFunction(name_a, _), Value::BuiltinFunction(name_b, _)) => name_a == name_b,
            (Value::Module(name_a, namespace_a), Value::Module(name_b, namespace_b)) => name_a == name_b && namespace_a == namespace_b,
            (Value::BoundMethod { object: object_a, method_name: method_name_a }, Value::BoundMethod { object: object_b, method_name: method_name_b }) => object_a == object_b && method_name_a == method_name_b,
            #[cfg(feature = "ffi")]
            (Value::ExternFunction { name: name_a, signature: signature_a, return_type: return_type_a, param_types: param_types_a }, Value::ExternFunction { name: name_b, signature: signature_b, return_type: return_type_b, param_types: param_types_b }) => name_a == name_b && signature_a == signature_b && return_type_a == return_type_b && param_types_a == param_types_b,
            (Value::None, Value::None) => true,
            (Value::TypedValue { value: value_a, type_info: type_info_a }, Value::TypedValue { value: value_b, type_info: type_info_b }) => value_a == value_b && type_info_a == type_info_b,
            _ => false, // Different variants are not equal
        }
    }
}

// Manual implementation of Debug trait for Value enum
impl fmt::Debug for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Value::Int(n) => write!(f, "Int({})", n),
            Value::Float(n) => write!(f, "Float({})", n),
            Value::Complex { real, imag } => write!(f, "Complex {{ real: {}, imag: {} }}", real, imag),
            Value::Bool(b) => write!(f, "Bool({})", b),
            Value::Str(s) => write!(f, "Str(\"{}\")", s),
            Value::List(items) => write!(f, "List({:?})", items),
            Value::Dict(dict) => write!(f, "Dict({:?})", dict),
            Value::Tuple(items) => write!(f, "Tuple({:?})", items),
            Value::Set(items) => write!(f, "Set({:?})", items),
            Value::FrozenSet(items) => write!(f, "FrozenSet({:?})", items),
            Value::Range { start, stop, step } => write!(f, "Range {{ start: {}, stop: {}, step: {} }}", start, stop, step),
            Value::RangeIterator { start, stop, step, current } => write!(f, "RangeIterator {{ start: {}, stop: {}, step: {}, current: {} }}", start, stop, step, current),
            Value::Bytes(data) => write!(f, "Bytes({:?})", data),
            Value::ByteArray(data) => write!(f, "ByteArray({:?})", data),
            Value::MemoryView { data, format, shape } => write!(f, "MemoryView {{ data: {:?}, format: {}, shape: {:?} }}", data, format, shape),
            Value::Ellipsis => write!(f, "Ellipsis"),
            Value::NotImplemented => write!(f, "NotImplemented"),
            Value::Object { class_name, fields, class_methods, base_object, mro } => {
                f.debug_struct("Object")
                    .field("class_name", class_name)
                    .field("fields", fields)
                    .field("class_methods", class_methods)
                    .field("base_object", base_object)
                    .field("mro", mro)
                    .finish()
            },
            Value::Class { name, bases, methods, metaclass, mro, base_object } => {
                f.debug_struct("Class")
                    .field("name", name)
                    .field("bases", bases)
                    .field("methods", methods)
                    .field("metaclass", metaclass)
                    .field("mro", mro)
                    .field("base_object", base_object)
                    .finish()
            },
            Value::Super(current_class, parent_class, obj) => {
                f.debug_tuple("Super")
                    .field(current_class)
                    .field(parent_class)
                    .field(obj)
                    .finish()
            },
            Value::Closure { name, params, body, captured_scope, docstring, compiled_code } => {
                f.debug_struct("Closure")
                    .field("name", name)
                    .field("params", params)
                    .field("body", body)
                    .field("captured_scope", captured_scope)
                    .field("docstring", docstring)
                    .field("compiled_code", &compiled_code.as_ref().map(|code| format!("Some({} instructions)", code.instructions.len())))
                    .finish()
            },
            Value::Code(code_obj) => write!(f, "Code({})", code_obj.name),
            Value::NativeFunction(_) => write!(f, "NativeFunction"),
            Value::BuiltinFunction(name, _) => write!(f, "BuiltinFunction({})", name),
            Value::Module(name, namespace) => write!(f, "Module({}, {:?})", name, namespace),
            Value::BoundMethod { object, method_name } => {
                f.debug_struct("BoundMethod")
                    .field("object", object)
                    .field("method_name", method_name)
                    .finish()
            },
            #[cfg(feature = "ffi")]
            Value::ExternFunction { name, signature, return_type, param_types } => {
                f.debug_struct("ExternFunction")
                    .field("name", name)
                    .field("signature", signature)
                    .field("return_type", return_type)
                    .field("param_types", param_types)
                    .finish()
            },
            Value::None => write!(f, "None"),
            Value::TypedValue { value, type_info } => {
                f.debug_struct("TypedValue")
                    .field("value", value)
                    .field("type_info", type_info)
                    .finish()
            },
        }
    }
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
            class_methods: HashMap::new(),
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
        Value::List(HPList::from_values(items))
    }

    pub fn new_bool(value: bool) -> Self {
        Value::Bool(value)
    }

    pub fn new_none() -> Self {
        Value::None
    }

    pub fn new_set(items: Vec<Value>) -> Self {
        Value::Set(items)
    }

    pub fn new_frozenset(items: Vec<Value>) -> Self {
        Value::FrozenSet(items)
    }

    pub fn new_range(start: i64, stop: i64, step: i64) -> Self {
        Value::Range { start, stop, step }
    }

    pub fn new_bytes(data: Vec<u8>) -> Self {
        Value::Bytes(data)
    }

    pub fn new_bytearray(data: Vec<u8>) -> Self {
        Value::ByteArray(data)
    }

    pub fn new_memoryview(data: Vec<u8>, format: String, shape: Vec<usize>) -> Self {
        Value::MemoryView { data, format, shape }
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
        // Simple isinstance implementation - for full implementation we'd need TypeHierarchy
        match self {
            Value::Object { class_name, mro, .. } => {
                // Check direct class name match
                if class_name == expected_type {
                    return true;
                }
                // Check MRO for inheritance
                mro.get_linearization().iter().any(|class| class == expected_type)
            }
            _ => {
                // For built-in types, check direct type name
                self.type_name() == expected_type
            }
        }
    }

    /// Get a string representation for debugging
    pub fn debug_string(&self) -> String {
        match self {
            Value::Int(n) => n.to_string(),
            Value::Float(n) => format!("{:.6}", n),
            Value::Complex { real, imag } => format!("({:.6}{:+.6}i)", real, imag),
            Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
            Value::Str(s) => format!("\"{}\"", s),
            Value::Ellipsis => "...".to_string(),
            Value::NotImplemented => "NotImplemented".to_string(),
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
            Value::FrozenSet(items) => {
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
                format!("frozenset({{{}{}}})", items_str.join(", "), suffix)
            }
            Value::Range { start, stop, step } => {
                if *step == 1 {
                    format!("range({}, {})", start, stop)
                } else {
                    format!("range({}, {}, {})", start, stop, step)
                }
            }
            Value::RangeIterator { start, stop, step, current } => {
                format!("range_iterator({}, {}, {}, {})", start, stop, step, current)
            }
            Value::MemoryView { data, format, shape } => {
                format!("<memory at 0x{:x}, format: {}, shape: {:?}>", data.as_ptr() as usize, format, shape)
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
            Value::Closure { name, params, .. } => {
                format!("<function {}({})>", name, params.iter().map(|p| p.name.clone()).collect::<Vec<_>>().join(", "))
            }
            Value::Code(code_obj) => {
                format!("<code object {}>", code_obj.name)
            }
            Value::BuiltinFunction(name, _) => {
                format!("<built-in function {}>", name)
            }

            Value::NativeFunction(_) => "<native function>".to_string(),
            Value::Object { class_name, .. } => {
                 format!("<{} object>", class_name)
             }
             Value::Class { name, bases, methods, .. } => {
                 if bases.is_empty() {
                     format!("<class '{}' with {} methods>", name, methods.len())
                 } else {
                     format!("<class '{}' bases: ({}) with {} methods>", name, bases.join(", "), methods.len())
                 }
             }
             Value::Super(current_class, parent_class, _) => {
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
             Value::BoundMethod { object, method_name } => {
                 format!("<bound method '{}' of {}>", method_name, object.debug_string())
             }
        }
    }
    
    /// Get type name for error messages
    pub fn type_name(&self) -> &str {
        match self {
            Value::Int(_) => "int",
            Value::Float(_) => "float",
            Value::Complex { .. } => "complex",
            Value::Bool(_) => "bool",
            Value::Str(_) => "str",
            Value::Ellipsis => "ellipsis",
            Value::NotImplemented => "NotImplementedType",
            Value::List(_) => "list",
            Value::Dict(_) => "dict",
            Value::Tuple(_) => "tuple",
            Value::Set(_) => "set",
            Value::FrozenSet(_) => "frozenset",
            Value::Range { .. } => "range",
            Value::RangeIterator { .. } => "range_iterator",
            Value::Bytes(_) => "bytes",
            Value::ByteArray(_) => "bytearray",
            Value::MemoryView { .. } => "memoryview",
            Value::Object { class_name, .. } => class_name,
            Value::Class { .. } => "type",
            Value::Super(_, _, _) => "super",
            Value::Closure { .. } => "function",
            Value::BuiltinFunction(_, _) => "builtin function",
            Value::NativeFunction(_) => "native function",
            Value::Module(_, _) => "module",
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => "extern function",
            Value::None => "None",
            Value::TypedValue { value, .. } => value.type_name(),
            Value::BoundMethod { .. } => "bound method",
            Value::Code(_) => "code",
        }
    }
    
    /// Dynamic type checking for optional static typing
    pub fn check_type(&self, expected: &Type) -> bool {
        match (self, expected) {
            (Value::Int(_), Type::Simple(name)) if name == "int" => true,
            (Value::Float(_), Type::Simple(name)) if name == "float" => true,
            (Value::Complex { .. }, Type::Simple(name)) if name == "complex" => true,
            (Value::Bool(_), Type::Simple(name)) if name == "bool" => true,
            (Value::Str(_), Type::Simple(name)) if name == "str" => true,
            (Value::Ellipsis, Type::Simple(name)) if name == "ellipsis" => true,
            (Value::NotImplemented, Type::Simple(name)) if name == "NotImplementedType" => true,
            (Value::List(_), Type::Simple(name)) if name == "list" => true,
            (Value::Dict(_), Type::Simple(name)) if name == "dict" => true,
            (Value::Tuple(_), Type::Simple(name)) if name == "tuple" => true,
            (Value::Set(_), Type::Simple(name)) if name == "set" => true,
            (Value::FrozenSet(_), Type::Simple(name)) if name == "frozenset" => true,
            (Value::Range { .. }, Type::Simple(name)) if name == "range" => true,
            (Value::Bytes(_), Type::Simple(name)) if name == "bytes" => true,
            (Value::ByteArray(_), Type::Simple(name)) if name == "bytearray" => true,
            (Value::MemoryView { .. }, Type::Simple(name)) if name == "memoryview" => true,
            (Value::Module(_,_), Type::Simple(name)) if name == "module" => true,
            (Value::Closure { .. }, Type::Simple(name)) if name == "function" => true,
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
            Value::Complex { .. } => Type::Simple("complex".to_string()),
            Value::Bool(_) => Type::Simple("bool".to_string()),
            Value::Str(_) => Type::Simple("str".to_string()),
            Value::Ellipsis => Type::Simple("ellipsis".to_string()),
            Value::NotImplemented => Type::Simple("NotImplementedType".to_string()),
            Value::List(_) => Type::Simple("list".to_string()),
            Value::Dict(_) => Type::Simple("dict".to_string()),
            Value::Tuple(_) => Type::Simple("tuple".to_string()),
            Value::Set(_) => Type::Simple("set".to_string()),
            Value::FrozenSet(_) => Type::Simple("frozenset".to_string()),
            Value::Range { .. } => Type::Simple("range".to_string()),
            Value::RangeIterator { .. } => Type::Simple("range_iterator".to_string()),
            Value::Bytes(_) => Type::Simple("bytes".to_string()),
            Value::ByteArray(_) => Type::Simple("bytearray".to_string()),
            Value::MemoryView { .. } => Type::Simple("memoryview".to_string()),
            Value::Module(_, _) => Type::Simple("module".to_string()),
            Value::Closure { .. } => Type::Simple("function".to_string()),
            Value::BuiltinFunction(_, _) => Type::Simple("function".to_string()),
            Value::NativeFunction(_) => Type::Simple("function".to_string()),
            Value::Object { class_name, .. } => Type::Simple(class_name.clone()),
            Value::Class { .. } => Type::Simple("type".to_string()),
            Value::None => Type::Simple("None".to_string()),
            Value::TypedValue { type_info, .. } => type_info.clone(),
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => Type::Simple("function".to_string()),
            Value::Super(_, _, _) => Type::Simple("super".to_string()),
            Value::BoundMethod { .. } => Type::Simple("bound method".to_string()),
            Value::Code(_) => Type::Simple("code".to_string()),
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
            Value::Str(s) => {
                let mut hplist = HPList::new();
                for c in s.chars() {
                    hplist.append(Value::Str(c.to_string()));
                }
                Ok(Value::List(hplist))
            },
            Value::Bytes(bytes) => {
                let mut hplist = HPList::new();
                for &b in bytes {
                    hplist.append(Value::Int(b as i64));
                }
                Ok(Value::List(hplist))
            },
            Value::ByteArray(bytes) => {
                let mut hplist = HPList::new();
                for &b in bytes {
                    hplist.append(Value::Int(b as i64));
                }
                Ok(Value::List(hplist))
            },
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", self.type_name())),
        }
    }

    /// Convert to tuple (similar to Python's tuple() function)
    pub fn to_tuple(&self) -> anyhow::Result<Value> {
        match self {
            Value::Tuple(items) => Ok(Value::Tuple(items.clone())),
            Value::List(items) => {
                let vec_items = items.as_vec().clone();
                Ok(Value::Tuple(vec_items))
            },
            Value::Set(items) => Ok(Value::Tuple(items.clone())),
            Value::Str(s) => {
                let chars: Vec<Value> = s.chars().map(|c| Value::Str(c.to_string())).collect();
                Ok(Value::Tuple(chars))
            },
            Value::Bytes(bytes) => {
                let items: Vec<Value> = bytes.iter().map(|&b| Value::Int(b as i64)).collect();
                Ok(Value::Tuple(items))
            },
            Value::ByteArray(bytes) => {
                let items: Vec<Value> = bytes.iter().map(|&b| Value::Int(b as i64)).collect();
                Ok(Value::Tuple(items))
            },
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
                for item in items.as_vec() {
                    if !unique_items.contains(item) {
                        unique_items.push(item.clone());
                    }
                }
                Ok(Value::Set(unique_items))
            },
            Value::Tuple(items) => {
                // Remove duplicates (simple implementation)
                let mut unique_items = Vec::new();
                for item in items {
                    if !unique_items.contains(item) {
                        unique_items.push(item.clone());
                    }
                }
                Ok(Value::Set(unique_items))
            },
            Value::Str(s) => {
                let mut unique_chars = Vec::new();
                for c in s.chars() {
                    let char_val = Value::Str(c.to_string());
                    if !unique_chars.contains(&char_val) {
                        unique_chars.push(char_val);
                    }
                }
                Ok(Value::Set(unique_chars))
            },
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", self.type_name())),
        }
    }
    
    /// Convert to boolean for truthiness testing
    pub fn is_truthy(&self) -> bool {
        match self {
            Value::Bool(b) => *b,
            Value::Int(n) => *n != 0,
            Value::Float(n) => *n != 0.0,
            Value::Complex { real, imag } => *real != 0.0 || *imag != 0.0,
            Value::Str(s) => !s.is_empty(),
            Value::Ellipsis => true,
            Value::NotImplemented => true,
            Value::List(items) => !items.is_empty(),
            Value::Dict(dict) => !dict.is_empty(),
            Value::Tuple(items) => !items.is_empty(),
            Value::Set(items) => !items.is_empty(),
            Value::FrozenSet(items) => !items.is_empty(),
            Value::Range { start, stop, step } => {
                if *step > 0 {
                    start < stop
                } else if *step < 0 {
                    start > stop
                } else {
                    false // step == 0 should not happen, but considered empty
                }
            },
            Value::RangeIterator { start, stop, step, current } => {
                if *step > 0 {
                    current < stop
                } else if *step < 0 {
                    current > stop
                } else {
                    false // step == 0 should not happen, but considered empty
                }
            },
            Value::Bytes(bytes) => !bytes.is_empty(),
            Value::ByteArray(bytes) => !bytes.is_empty(),
            Value::MemoryView { data, .. } => !data.is_empty(),
            Value::None => false,
            Value::Object { .. } => true,
            Value::Class { .. } => true,
            Value::Super(_, _, _) => true,
            Value::Closure { .. } => true,
            Value::BuiltinFunction(_, _) => true,
            Value::NativeFunction(_) => true,
            Value::Module(_, _) => true,
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => true,
            Value::TypedValue { value, .. } => value.is_truthy(),
            Value::BoundMethod { .. } => true,
            Value::Code(_) => true,
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
            (Value::Complex { .. }, _) | (_, Value::Complex { .. }) => None, // Complex numbers can't be ordered
            (Value::Ellipsis, Value::Ellipsis) => Some(std::cmp::Ordering::Equal),
            (Value::NotImplemented, Value::NotImplemented) => Some(std::cmp::Ordering::Equal),
            (Value::RangeIterator { .. }, _) | (_, Value::RangeIterator { .. }) => None, // RangeIterator can't be ordered
            _ => None,
        }
    }
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Value::Int(n) => write!(f, "{}", n),
            Value::Float(n) => write!(f, "{:.6}", n),
            Value::Complex { real, imag } => {
                if *imag >= 0.0 {
                    write!(f, "({:.6}+{:.6}j)", real, imag)
                } else {
                    write!(f, "({:.6}{:.6}j)", real, imag)
                }
            },
            Value::Bool(b) => write!(f, "{}", if *b { "True" } else { "False" }),
            Value::Str(s) => write!(f, "{}", s), // No quotes for display
            Value::Ellipsis => write!(f, "..."),
            Value::NotImplemented => write!(f, "NotImplemented"),
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
            Value::FrozenSet(items) => {
                let items_str: Vec<String> = items.iter().map(|v| format!("{}", v)).collect();
                if items.is_empty() {
                    write!(f, "frozenset()")
                } else {
                    write!(f, "frozenset({{{}}})", items_str.join(", "))
                }
            }
            Value::Range { start, stop, step } => {
                if *step == 1 {
                    write!(f, "range({}, {})", start, stop)
                } else {
                    write!(f, "range({}, {}, {})", start, stop, step)
                }
            }
            Value::RangeIterator { start, stop, step, current } => {
                write!(f, "range_iterator({}, {}, {}, {})", start, stop, step, current)
            }
            Value::Bytes(bytes) => {
                write!(f, "b'{}'", String::from_utf8_lossy(bytes))
            }
            Value::ByteArray(bytes) => {
                write!(f, "bytearray(b'{}')", String::from_utf8_lossy(bytes))
            }
            Value::MemoryView { data, format, shape } => {
                write!(f, "<memory at 0x{:p} format='{}' shape={:?}>", data.as_ptr(), format, shape)
            }
            Value::Dict(dict) => {
                let pairs: Vec<String> = dict.iter()
                    .map(|(k, v)| format!("{}: {}", k, v))
                    .collect();
                write!(f, "{{{}}}", pairs.join(", "))
            }
            Value::None => write!(f, "None"),
            Value::Closure { name, params, .. } => {
                write!(f, "<function {}({})>", name, params.iter().map(|p| p.name.clone()).collect::<Vec<_>>().join(", "))
            }
            Value::BuiltinFunction(name, _) => {
                write!(f, "<built-in function {}>", name)
            }
            Value::NativeFunction(_) => write!(f, "<native function>"),
            Value::Object { class_name, .. } => {
                write!(f, "<{} object>", class_name)
            }
            Value::Class { name, bases, methods, .. } => {
                if bases.is_empty() {
                    write!(f, "<class '{}'>", name)
                } else {
                    write!(f, "<class '{}' bases: ({})>", name, bases.join(", "))
                }
            }
            Value::Super(current_class, parent_class, _) => {
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
            Value::BoundMethod { object, method_name } => {
                write!(f, "<bound method '{}' of {}>", method_name, object)
            }
            Value::Code(code_obj) => {
                write!(f, "<code object {}>", code_obj.name)
            }
        }
    }
}

impl Value {
    /// Call a method on this value with the given arguments
    /// This is the main entry point for method calls on builtin types
    pub fn call_method(&mut self, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match self {
            Value::Str(s) => Self::call_str_method_static(s.clone(), method_name, args),
            Value::List(ref mut list) => Self::call_list_method_static(list, method_name, args),
            Value::Dict(ref mut dict) => Self::call_dict_method_static(dict, method_name, args),
            Value::Set(ref mut set) => Self::call_set_method_static(set, method_name, args),
            Value::Tuple(tuple) => Self::call_tuple_method_static(tuple.clone(), method_name, args),
            Value::Int(n) => Self::call_int_method_static(*n, method_name, args),
            Value::Float(f) => Self::call_float_method_static(*f, method_name, args),
            Value::Bytes(bytes) => Self::call_bytes_method_static(bytes.clone(), method_name, args),
            Value::ByteArray(ref mut ba) => Self::call_bytearray_method_static(ba, method_name, args),
            Value::Object { class_methods, .. } => {
                // For custom objects, check if the method exists in class_methods
                if let Some(method) = class_methods.get(method_name) {
                    // Method exists, but we can't call it directly from here
                    // This error indicates the VM should handle it
                    Err(anyhow::anyhow!("Method '{}' exists but needs to be called through VM", method_name))
                } else {
                    Err(anyhow::anyhow!("'{}' object has no attribute '{}'", self.type_name(), method_name))
                }
            },
            _ => Err(anyhow::anyhow!("'{}' object has no attribute '{}'", self.type_name(), method_name)),
        }
    }

    /// String method implementations
    fn call_str_method_static(s: String, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "upper" => Ok(Value::Str(s.to_uppercase())),
            "lower" => Ok(Value::Str(s.to_lowercase())),
            "capitalize" => {
                let mut chars = s.chars();
                match chars.next() {
                    None => Ok(Value::Str(String::new())),
                    Some(first) => Ok(Value::Str(first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase())),
                }
            }
            "title" => {
                let mut result = String::new();
                let mut capitalize_next = true;
                for ch in s.chars() {
                    if ch.is_whitespace() {
                        capitalize_next = true;
                        result.push(ch);
                    } else if capitalize_next {
                        result.push_str(&ch.to_uppercase().to_string());
                        capitalize_next = false;
                    } else {
                        result.push_str(&ch.to_lowercase().to_string());
                    }
                }
                Ok(Value::Str(result))
            }
            "swapcase" => {
                let result: String = s.chars().map(|c| {
                    if c.is_uppercase() {
                        c.to_lowercase().to_string()
                    } else {
                        c.to_uppercase().to_string()
                    }
                }).collect();
                Ok(Value::Str(result))
            }
            "strip" => Ok(Value::Str(s.trim().to_string())),
            "lstrip" => Ok(Value::Str(s.trim_start().to_string())),
            "rstrip" => Ok(Value::Str(s.trim_end().to_string())),
            "split" => {
                let sep = if args.is_empty() {
                    None
                } else {
                    match &args[0] {
                        Value::Str(sep) => Some(sep.as_str()),
                        _ => return Err(anyhow::anyhow!("split() separator must be str")),
                    }
                };

                let parts: Vec<Value> = if let Some(sep) = sep {
                    s.split(sep).map(|p| Value::Str(p.to_string())).collect()
                } else {
                    s.split_whitespace().map(|p| Value::Str(p.to_string())).collect()
                };

                Ok(Value::List(HPList::from_values(parts)))
            }
            "join" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("join() takes exactly one argument"));
                }
                match &args[0] {
                    Value::List(items) => {
                        let strings: Result<Vec<String>, _> = items.iter().map(|v| {
                            match v {
                                Value::Str(s) => Ok(s.clone()),
                                _ => Err(anyhow::anyhow!("join() requires all items to be strings")),
                            }
                        }).collect();
                        Ok(Value::Str(strings?.join(&s)))
                    }
                    Value::Tuple(items) => {
                        let strings: Result<Vec<String>, _> = items.iter().map(|v| {
                            match v {
                                Value::Str(s) => Ok(s.clone()),
                                _ => Err(anyhow::anyhow!("join() requires all items to be strings")),
                            }
                        }).collect();
                        Ok(Value::Str(strings?.join(&s)))
                    }
                    _ => Err(anyhow::anyhow!("join() argument must be an iterable")),
                }
            }
            "replace" => {
                if args.len() < 2 {
                    return Err(anyhow::anyhow!("replace() takes at least 2 arguments"));
                }
                let old = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("replace() argument 1 must be str")),
                };
                let new = match &args[1] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("replace() argument 2 must be str")),
                };
                Ok(Value::Str(s.replace(&old, &new)))
            }
            "find" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("find() takes at least 1 argument"));
                }
                let sub = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("find() argument must be str")),
                };
                match s.find(&sub) {
                    Some(pos) => Ok(Value::Int(pos as i64)),
                    None => Ok(Value::Int(-1)),
                }
            }
            "startswith" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("startswith() takes at least 1 argument"));
                }
                let prefix = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("startswith() argument must be str")),
                };
                Ok(Value::Bool(s.starts_with(&prefix)))
            }
            "endswith" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("endswith() takes at least 1 argument"));
                }
                let suffix = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("endswith() argument must be str")),
                };
                Ok(Value::Bool(s.ends_with(&suffix)))
            }
            "isdigit" => Ok(Value::Bool(s.chars().all(|c| c.is_numeric()))),
            "isalpha" => Ok(Value::Bool(s.chars().all(|c| c.is_alphabetic()))),
            "isalnum" => Ok(Value::Bool(s.chars().all(|c| c.is_alphanumeric()))),
            "isspace" => Ok(Value::Bool(s.chars().all(|c| c.is_whitespace()))),
            "islower" => Ok(Value::Bool(s.chars().filter(|c| c.is_alphabetic()).all(|c| c.is_lowercase()))),
            "isupper" => Ok(Value::Bool(s.chars().filter(|c| c.is_alphabetic()).all(|c| c.is_uppercase()))),
            _ => Err(anyhow::anyhow!("'str' object has no attribute '{}'", method_name)),
        }
    }

    /// List method implementations
    fn call_list_method_static(list: &mut HPList, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "append" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("append() takes exactly one argument"));
                }
                list.append(args[0].clone());
                Ok(Value::None)
            }
            "extend" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("extend() takes exactly one argument"));
                }
                match &args[0] {
                    Value::List(other) => {
                        list.extend(other.clone());
                        Ok(Value::None)
                    }
                    Value::Tuple(items) => {
                        list.extend(items.iter().cloned());
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("extend() argument must be iterable")),
                }
            }
            "insert" => {
                if args.len() != 2 {
                    return Err(anyhow::anyhow!("insert() takes exactly 2 arguments"));
                }
                let index = match &args[0] {
                    Value::Int(i) => *i as isize,
                    _ => return Err(anyhow::anyhow!("insert() index must be int")),
                };
                list.insert(index, args[1].clone())?;
                Ok(Value::None)
            }
            "remove" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("remove() takes exactly one argument"));
                }
                list.remove(&args[0])?;
                Ok(Value::None)
            }
            "pop" => {
                if args.is_empty() {
                    // Pop from end
                    match list.pop() {
                        Some(v) => Ok(v),
                        None => Err(anyhow::anyhow!("pop from empty list")),
                    }
                } else {
                    // Pop from specific index
                    let index = match &args[0] {
                        Value::Int(i) => *i as isize,
                        _ => return Err(anyhow::anyhow!("pop() index must be int")),
                    };
                    list.pop_at(index)
                }
            }
            "clear" => {
                list.clear();
                Ok(Value::None)
            }
            "sort" => {
                list.sort();
                Ok(Value::None)
            }
            "reverse" => {
                list.reverse();
                Ok(Value::None)
            }
            "index" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("index() takes at least 1 argument"));
                }
                let pos = list.index(&args[0], None, None)?;
                Ok(Value::Int(pos as i64))
            }
            "count" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("count() takes exactly one argument"));
                }
                let count = list.count(&args[0]);
                Ok(Value::Int(count as i64))
            }
            "copy" => {
                Ok(Value::List(list.clone()))
            }
            _ => Err(anyhow::anyhow!("'list' object has no attribute '{}'", method_name)),
        }
    }

    /// Dict method implementations
    fn call_dict_method_static(dict: &mut HashMap<String, Value>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "keys" => {
                let keys: Vec<Value> = dict.keys().map(|k| Value::Str(k.clone())).collect();
                Ok(Value::List(HPList::from_values(keys)))
            }
            "values" => {
                let values: Vec<Value> = dict.values().cloned().collect();
                Ok(Value::List(HPList::from_values(values)))
            }
            "items" => {
                let items: Vec<Value> = dict.iter()
                    .map(|(k, v)| Value::Tuple(vec![Value::Str(k.clone()), v.clone()]))
                    .collect();
                Ok(Value::List(HPList::from_values(items)))
            }
            "get" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("get() takes at least 1 argument"));
                }
                let key = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("dict key must be str")),
                };
                let default = if args.len() > 1 {
                    args[1].clone()
                } else {
                    Value::None
                };
                Ok(dict.get(&key).cloned().unwrap_or(default))
            }
            "pop" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("pop() takes at least 1 argument"));
                }
                let key = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("dict key must be str")),
                };
                let default = if args.len() > 1 {
                    Some(args[1].clone())
                } else {
                    None
                };
                match dict.remove(&key) {
                    Some(value) => Ok(value),
                    None => default.ok_or_else(|| anyhow::anyhow!("KeyError: '{}'", key)),
                }
            }
            "clear" => {
                dict.clear();
                Ok(Value::None)
            }
            "update" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("update() takes exactly one argument"));
                }
                match &args[0] {
                    Value::Dict(other) => {
                        for (k, v) in other {
                            dict.insert(k.clone(), v.clone());
                        }
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("update() argument must be dict")),
                }
            }
            "copy" => {
                Ok(Value::Dict(dict.clone()))
            }
            "setdefault" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("setdefault() takes at least 1 argument"));
                }
                let key = match &args[0] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow::anyhow!("dict key must be str")),
                };
                let default = if args.len() > 1 {
                    args[1].clone()
                } else {
                    Value::None
                };
                Ok(dict.entry(key).or_insert(default.clone()).clone())
            }
            "popitem" => {
                match dict.iter().next() {
                    Some((k, v)) => {
                        let key = k.clone();
                        let value = v.clone();
                        dict.remove(&key);
                        Ok(Value::Tuple(vec![Value::Str(key), value]))
                    }
                    None => Err(anyhow::anyhow!("popitem(): dictionary is empty")),
                }
            }
            _ => Err(anyhow::anyhow!("'dict' object has no attribute '{}'", method_name)),
        }
    }

    /// Set method implementations
    fn call_set_method_static(set: &mut Vec<Value>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "add" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("add() takes exactly one argument"));
                }
                if !set.contains(&args[0]) {
                    set.push(args[0].clone());
                }
                Ok(Value::None)
            }
            "remove" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("remove() takes exactly one argument"));
                }
                if let Some(pos) = set.iter().position(|x| x == &args[0]) {
                    set.remove(pos);
                    Ok(Value::None)
                } else {
                    Err(anyhow::anyhow!("KeyError"))
                }
            }
            "discard" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("discard() takes exactly one argument"));
                }
                if let Some(pos) = set.iter().position(|x| x == &args[0]) {
                    set.remove(pos);
                }
                Ok(Value::None)
            }
            "pop" => {
                if set.is_empty() {
                    Err(anyhow::anyhow!("pop from an empty set"))
                } else {
                    Ok(set.remove(0))
                }
            }
            "clear" => {
                set.clear();
                Ok(Value::None)
            }
            "union" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("union() takes exactly one argument"));
                }
                match &args[0] {
                    Value::Set(other) => {
                        let mut result = set.clone();
                        for item in other {
                            if !result.contains(item) {
                                result.push(item.clone());
                            }
                        }
                        Ok(Value::Set(result))
                    }
                    _ => Err(anyhow::anyhow!("union() argument must be set")),
                }
            }
            "intersection" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("intersection() takes exactly one argument"));
                }
                match &args[0] {
                    Value::Set(other) => {
                        let result: Vec<Value> = set.iter()
                            .filter(|item| other.contains(item))
                            .cloned()
                            .collect();
                        Ok(Value::Set(result))
                    }
                    _ => Err(anyhow::anyhow!("intersection() argument must be set")),
                }
            }
            "difference" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("difference() takes exactly one argument"));
                }
                match &args[0] {
                    Value::Set(other) => {
                        let result: Vec<Value> = set.iter()
                            .filter(|item| !other.contains(item))
                            .cloned()
                            .collect();
                        Ok(Value::Set(result))
                    }
                    _ => Err(anyhow::anyhow!("difference() argument must be set")),
                }
            }
            "copy" => {
                Ok(Value::Set(set.clone()))
            }
            _ => Err(anyhow::anyhow!("'set' object has no attribute '{}'", method_name)),
        }
    }

    /// Tuple method implementations
    fn call_tuple_method_static(tuple: Vec<Value>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "count" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("count() takes exactly one argument"));
                }
                let count = tuple.iter().filter(|x| *x == &args[0]).count();
                Ok(Value::Int(count as i64))
            }
            "index" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("index() takes at least 1 argument"));
                }
                match tuple.iter().position(|x| x == &args[0]) {
                    Some(pos) => Ok(Value::Int(pos as i64)),
                    None => Err(anyhow::anyhow!("tuple.index(x): x not in tuple")),
                }
            }
            _ => Err(anyhow::anyhow!("'tuple' object has no attribute '{}'", method_name)),
        }
    }

    /// Int method implementations
    fn call_int_method_static(n: i64, method_name: &str, _args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "bit_length" => {
                let bits = if n == 0 { 0 } else { (n.abs() as f64).log2().floor() as i64 + 1 };
                Ok(Value::Int(bits))
            }
            _ => Err(anyhow::anyhow!("'int' object has no attribute '{}'", method_name)),
        }
    }

    /// Float method implementations
    fn call_float_method_static(f: f64, method_name: &str, _args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "is_integer" => Ok(Value::Bool(f.fract() == 0.0)),
            _ => Err(anyhow::anyhow!("'float' object has no attribute '{}'", method_name)),
        }
    }

    /// Bytes method implementations
    fn call_bytes_method_static(bytes: Vec<u8>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "decode" => {
                let encoding = if args.is_empty() {
                    "utf-8"
                } else {
                    match &args[0] {
                        Value::Str(s) => s.as_str(),
                        _ => return Err(anyhow::anyhow!("decode() encoding must be str")),
                    }
                };
                match encoding {
                    "utf-8" | "utf8" => {
                        match String::from_utf8(bytes) {
                            Ok(s) => Ok(Value::Str(s)),
                            Err(_) => Err(anyhow::anyhow!("'utf-8' codec can't decode bytes")),
                        }
                    }
                    _ => Err(anyhow::anyhow!("unknown encoding: {}", encoding)),
                }
            }
            _ => Err(anyhow::anyhow!("'bytes' object has no attribute '{}'", method_name)),
        }
    }

    /// ByteArray method implementations
    fn call_bytearray_method_static(ba: &mut Vec<u8>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "append" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("append() takes exactly one argument"));
                }
                let byte = match &args[0] {
                    Value::Int(i) => {
                        if *i < 0 || *i > 255 {
                            return Err(anyhow::anyhow!("byte must be in range(0, 256)"));
                        }
                        *i as u8
                    }
                    _ => return Err(anyhow::anyhow!("an integer is required")),
                };
                ba.push(byte);
                Ok(Value::None)
            }
            "extend" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("extend() takes exactly one argument"));
                }
                match &args[0] {
                    Value::ByteArray(other) | Value::Bytes(other) => {
                        ba.extend(other);
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("extend() argument must be bytes-like")),
                }
            }
            _ => Err(anyhow::anyhow!("'bytearray' object has no attribute '{}'", method_name)),
        }
    }

    /// Get method for a value type (kept for backwards compatibility)
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
            Value::FrozenSet(_) => None, // FrozenSet methods would go here
            Value::Range { .. } => None, // Range methods would go here
            Value::MemoryView { .. } => None, // MemoryView methods would go here
            Value::Complex { .. } | Value::Ellipsis | Value::NotImplemented => None,
            _ => None,
        }
    }

    /// Convert a Value back to an Expr for decorator application
    pub fn to_expr(&self) -> Expr {
        match self {
            Value::Int(n) => Expr::Literal(Literal::Int(*n)),
            Value::Float(n) => Expr::Literal(Literal::Float(*n)),
            Value::Complex { real, imag } => Expr::Literal(Literal::Complex { real: *real, imag: *imag }),
            Value::Bool(b) => Expr::Literal(Literal::Bool(*b)),
            Value::Str(s) => Expr::Literal(Literal::String(s.clone())),
            Value::Ellipsis => Expr::Literal(Literal::Ellipsis),
            Value::NotImplemented => Expr::Identifier("NotImplemented".to_string()),
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
            Value::Closure { name, .. } => Expr::Identifier(name.clone()),
            Value::BuiltinFunction(name, _) => Expr::Identifier(name.clone()),
            Value::NativeFunction(_) => Expr::Identifier("native_function".to_string()),
            Value::Object { class_name, .. } => Expr::Identifier(class_name.clone()),
            Value::Class { name, .. } => Expr::Identifier(name.clone()),
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
            Value::FrozenSet(items) => {
                let exprs: Vec<Expr> = items.iter().map(|v| v.to_expr()).collect();
                Expr::Call {
                    func: Box::new(Expr::Identifier("frozenset".to_string())),
                    args: vec![Expr::Set(exprs)],
                    kwargs: vec![],
                }
            }
            Value::Range { start, stop, step } => {
                Expr::Call {
                    func: Box::new(Expr::Identifier("range".to_string())),
                    args: vec![
                        Expr::Literal(Literal::Int(*start)),
                        Expr::Literal(Literal::Int(*stop)),
                        Expr::Literal(Literal::Int(*step)),
                    ],
                    kwargs: vec![],
                }
            }
            Value::RangeIterator { start, stop, step, current } => {
                Expr::Call {
                    func: Box::new(Expr::Identifier("range_iterator".to_string())),
                    args: vec![
                        Expr::Literal(Literal::Int(*start)),
                        Expr::Literal(Literal::Int(*stop)),
                        Expr::Literal(Literal::Int(*step)),
                        Expr::Literal(Literal::Int(*current)),
                    ],
                    kwargs: vec![],
                }
            }
            Value::MemoryView { data, format, shape, .. } => {
                Expr::Call {
                    func: Box::new(Expr::Identifier("memoryview".to_string())),
                    args: vec![Expr::Literal(Literal::String(format!("b'{}'", String::from_utf8_lossy(data))))],
                    kwargs: vec![],
                }
            }
            Value::TypedValue { value, .. } => value.to_expr(),
            #[cfg(feature = "ffi")]
            Value::ExternFunction { name, .. } => Expr::Identifier(name.clone()),
            Value::Super(current_class, _, _) => Expr::Identifier(format!("super_{}", current_class)),
            Value::BoundMethod { object, method_name } => {
                Expr::Call {
                    func: Box::new(Expr::Attribute {
                        object: Box::new(object.to_expr()),
                        name: method_name.clone(),
                    }),
                    args: vec![],
                    kwargs: vec![],
                }
            }
            Value::Code(_) => Expr::Identifier("code".to_string()),
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
            Value::Complex { real, imag } => {
                18u8.hash(state);
                real.to_bits().hash(state);
                imag.to_bits().hash(state);
            }
            Value::Bool(b) => {
                2u8.hash(state);
                b.hash(state);
            }
            Value::Str(s) => {
                3u8.hash(state);
                s.hash(state);
            }
            Value::Ellipsis => {
                20u8.hash(state);
                "ellipsis".hash(state);
            }
            Value::NotImplemented => {
                21u8.hash(state);
                "NotImplemented".hash(state);
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
            Value::Object { class_name, fields, class_methods, .. } => {
                 10u8.hash(state);
                 class_name.hash(state);
                 let mut pairs: Vec<_> = fields.iter().collect();
                 pairs.sort_by_key(|(k, _)| *k);
                 pairs.hash(state);
                 let mut method_pairs: Vec<_> = class_methods.iter().collect();
                 method_pairs.sort_by_key(|(k, _)| *k);
                 method_pairs.hash(state);
             }
            Value::Class { name, bases, methods, metaclass, .. } => {
                 27u8.hash(state);
                 name.hash(state);
                 bases.hash(state);
                 let mut pairs: Vec<_> = methods.iter().collect();
                 pairs.sort_by_key(|(k, _)| *k);
                 pairs.hash(state);
                 metaclass.hash(state);
             }
            Value::Super(current_class, parent_class, _) => {
                19u8.hash(state);
                current_class.hash(state);
                parent_class.hash(state);
            }
            Value::Closure { name, params, compiled_code, .. } => {
                11u8.hash(state);
                name.hash(state);
                // We can't easily hash params, so we use their count
                params.len().hash(state);
                // Also hash whether we have compiled_code and its name if it exists
                match compiled_code {
                    Some(code) => {
                        true.hash(state);
                        code.name.hash(state);
                        code.instructions.len().hash(state);
                    }
                    None => false.hash(state),
                }
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
            Value::Code(code_obj) => {
                26u8.hash(state);
                code_obj.name.hash(state);
            }
            Value::None => {
                16u8.hash(state);
            }
            Value::FrozenSet(items) => {
                22u8.hash(state);
                // For frozensets, we need to hash in a deterministic order
                let mut sorted_items = items.clone();
                sorted_items.sort_by(|a, b| {
                    format!("{:?}", a).cmp(&format!("{:?}", b))
                });
                sorted_items.hash(state);
            }
            Value::Range { start, stop, step } => {
                23u8.hash(state);
                start.hash(state);
                stop.hash(state);
                step.hash(state);
            }
            Value::RangeIterator { start, stop, step, current } => {
                27u8.hash(state);
                start.hash(state);
                stop.hash(state);
                step.hash(state);
                current.hash(state);
            }
            Value::MemoryView { data, format, shape } => {
                24u8.hash(state);
                data.hash(state);
                format.hash(state);
                shape.hash(state);
            }
            Value::TypedValue { value, type_info } => {
                17u8.hash(state);
                value.hash(state);
                // We can't easily hash type_info, so we use its debug representation
                format!("{:?}", type_info).hash(state);
            }
            Value::BoundMethod { object, method_name } => {
                25u8.hash(state);
                object.hash(state);
                method_name.hash(state);
            }
        }
    }
}
