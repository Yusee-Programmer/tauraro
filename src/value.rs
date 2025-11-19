#[cfg(feature = "ffi")]
use crate::ffi::FFIType;
use crate::base_object::{BaseObject, MRO, DunderMethod};
use crate::ast::{Param, Statement, Type};
use crate::bytecode::memory::CodeObject;
use crate::bytecode::objects::RcValue;
use std::collections::HashMap;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::rc::Rc;
use std::cell::RefCell;

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
    Dict(Rc<RefCell<HashMap<String, Value>>>),  // Rc<RefCell> for reference semantics like Python
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
    Starred(Box<Value>), // For *args and **kwargs in function calls
    KwargsMarker(HashMap<String, Value>), // Special marker for **kwargs in function calls
    Object {
        class_name: String,
        fields: Rc<RefCell<HashMap<String, Value>>>,
        class_methods: HashMap<String, Value>,
        base_object: BaseObject,
        mro: MRO,
    },
    Class {
        name: String,
        bases: Vec<String>,
        methods: HashMap<String, Value>,
        attributes: Rc<RefCell<HashMap<String, Value>>>,  // Class-level attributes like class variables
        metaclass: Option<Box<Value>>,
        mro: MRO,
        base_object: BaseObject,
    },
    Super(String, String, Option<Box<Value>>, Option<HashMap<String, Value>>), // current class name, parent class name, object instance, parent class methods
    Closure {
        name: String,
        params: Vec<Param>,
        body: Vec<Statement>,
        captured_scope: HashMap<String, Value>,
        docstring: Option<String>,
        // Add a field to store the compiled code directly in the Closure
        compiled_code: Option<Box<crate::bytecode::memory::CodeObject>>,
        // Store the module's globals for functions defined in modules
        module_globals: Option<std::rc::Rc<std::cell::RefCell<HashMap<String, RcValue>>>>,
    },
    Code(Box<crate::bytecode::memory::CodeObject>), // Compiled function code
    NativeFunction(fn(Vec<Value>) -> anyhow::Result<Value>),
    BuiltinFunction(String, fn(Vec<Value>) -> anyhow::Result<Value>),
    Module(String, HashMap<String, Value>), // module name, namespace
    BoundMethod {
        object: Box<Value>,
        method_name: String,
    },
    ClassMethod {
        method: Box<Value>,
        class: Box<Value>,
    },
    StaticMethod {
        method: Box<Value>,
    },
    #[cfg(feature = "ffi")]
    ExternFunction {
        library_name: String,
        name: String,
        signature: String,
        return_type: FFIType,
        param_types: Vec<FFIType>,
    },
    Generator {
        code: Box<CodeObject>,
        frame: Option<Box<crate::bytecode::memory::Frame>>,
        finished: bool,
    },
    Coroutine {
        name: String,
        code: Box<CodeObject>,
        frame: Option<Box<crate::bytecode::memory::Frame>>,
        finished: bool,
        awaiting: Option<Box<Value>>,
    },
    Iterator {
        // Simple iterator for lists, tuples, etc.
        items: Vec<Value>,
        current_index: usize,
    },
    Exception {
        class_name: String,
        message: String,
        traceback: Option<String>,
    },
    None,
    // For optional static typing
    TypedValue { value: Box<Value>, type_info: Type },
}

// Manual implementation of Hash trait for Value enum
impl Hash for Value {
    fn hash<H: Hasher>(&self, state: &mut H) {
        // We'll use a simple approach to hash the Value enum
        // In a production implementation, you'd want to hash each variant's data
        match self {
            Value::Int(n) => n.hash(state),
            Value::Float(f) => f.to_bits().hash(state), // Hash the bit representation
            Value::Complex { real, imag } => {
                real.to_bits().hash(state);
                imag.to_bits().hash(state);
            },
            Value::Bool(b) => b.hash(state),
            Value::Str(s) => s.hash(state),
            Value::Ellipsis => 0.hash(state),
            Value::NotImplemented => 1.hash(state),
            Value::KwargsMarker(dict) => {
                // Hash the dictionary contents
                for (key, value) in dict {
                    key.hash(state);
                    // For values, we'll use the debug representation for simplicity
                    format!("{:?}", value).hash(state);
                }
            },
            Value::Exception { class_name, message, .. } => {
                class_name.hash(state);
                message.hash(state);
            },
            Value::Iterator { items, current_index } => {
                // Hash the items and current index
                for item in items {
                    item.hash(state);
                }
                current_index.hash(state);
            },
            Value::Coroutine { name, finished, .. } => {
                name.hash(state);
                finished.hash(state);
            },
            // For complex types, we'll use a simple approach
            _ => {
                // For complex types that can't easily be hashed, we'll hash their debug representation
                // This is not ideal for performance but works for now
                format!("{:?}", self).hash(state);
            },
            // All other variants are handled by the _ pattern
        }
    }
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
            (Value::Class { name: name_a, bases: bases_a, methods: methods_a, attributes: attributes_a, metaclass: metaclass_a, mro: mro_a, base_object: base_object_a }, Value::Class { name: name_b, bases: bases_b, methods: methods_b, attributes: attributes_b, metaclass: metaclass_b, mro: mro_b, base_object: base_object_b }) => name_a == name_b && bases_a == bases_b && methods_a == methods_b && attributes_a == attributes_b && metaclass_a == metaclass_b && mro_a == mro_b && base_object_a == base_object_b,
            (Value::Super(current_class_a, parent_class_a, obj_a, _), Value::Super(current_class_b, parent_class_b, obj_b, _)) => current_class_a == current_class_b && parent_class_a == parent_class_b && obj_a == obj_b,
            // For Closure, we compare name, params, and compiled_code
            (Value::Closure { name: name_a, params: params_a, compiled_code: code_a, .. }, Value::Closure { name: name_b, params: params_b, compiled_code: code_b, .. }) => name_a == name_b && params_a == params_b && code_a == code_b,
            (Value::Code(a), Value::Code(b)) => a == b,
            (Value::NativeFunction(_), Value::NativeFunction(_)) => false, // Function pointers can't be compared
            (Value::BuiltinFunction(name_a, _), Value::BuiltinFunction(name_b, _)) => name_a == name_b,
            (Value::Module(name_a, namespace_a), Value::Module(name_b, namespace_b)) => name_a == name_b && namespace_a == namespace_b,
            (Value::BoundMethod { object: object_a, method_name: method_name_a }, Value::BoundMethod { object: object_b, method_name: method_name_b }) => object_a == object_b && method_name_a == method_name_b,
            (Value::ClassMethod { method: method_a, class: class_a }, Value::ClassMethod { method: method_b, class: class_b }) => method_a == method_b && class_a == class_b,
            (Value::StaticMethod { method: method_a }, Value::StaticMethod { method: method_b }) => method_a == method_b,
            #[cfg(feature = "ffi")]
            (Value::ExternFunction { library_name: library_name_a, name: name_a, signature: signature_a, return_type: return_type_a, param_types: param_types_a }, Value::ExternFunction { library_name: library_name_b, name: name_b, signature: signature_b, return_type: return_type_b, param_types: param_types_b }) => library_name_a == library_name_b && name_a == name_b && signature_a == signature_b && return_type_a == return_type_b && param_types_a == param_types_b,
            (Value::None, Value::None) => true,
            (Value::TypedValue { value: value_a, type_info: type_info_a }, Value::TypedValue { value: value_b, type_info: type_info_b }) => value_a == value_b && type_info_a == type_info_b,
            (Value::KwargsMarker(a), Value::KwargsMarker(b)) => a == b,
            (Value::Exception { class_name: class_name_a, message: message_a, .. }, Value::Exception { class_name: class_name_b, message: message_b, .. }) => class_name_a == class_name_b && message_a == message_b,
            (Value::Iterator { items: items_a, current_index: current_index_a }, Value::Iterator { items: items_b, current_index: current_index_b }) => items_a == items_b && current_index_a == current_index_b,
            (Value::Coroutine { name: name_a, finished: finished_a, .. }, Value::Coroutine { name: name_b, finished: finished_b, .. }) => name_a == name_b && finished_a == finished_b,
            _ => false, // Different variants are not equal
        }
    }
}

impl Eq for Value {}

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
            Value::Starred(value) => write!(f, "Starred({:?})", value),
            Value::KwargsMarker(dict) => write!(f, "KwargsMarker({:?})", dict),
            Value::Object { class_name, fields, class_methods, base_object, mro } => {
                f.debug_struct("Object")
                    .field("class_name", class_name)
                    .field("fields", fields)
                    .field("class_methods", class_methods)
                    .field("base_object", base_object)
                    .field("mro", mro)
                    .finish()
            },
            Value::Class { name, bases, methods, attributes, metaclass, mro, base_object } => {
                f.debug_struct("Class")
                    .field("name", name)
                    .field("bases", bases)
                    .field("methods", methods)
                    .field("attributes", attributes)
                    .field("metaclass", metaclass)
                    .field("mro", mro)
                    .field("base_object", base_object)
                    .finish()
            },
            Value::Super(current_class, parent_class, obj, _) => {
                f.debug_tuple("Super")
                    .field(current_class)
                    .field(parent_class)
                    .field(obj)
                    .finish()
            },
            Value::Closure { name, params, body, captured_scope, docstring, compiled_code, .. } => {
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
            Value::ClassMethod { method, class } => {
                f.debug_struct("ClassMethod")
                    .field("method", method)
                    .field("class", class)
                    .finish()
            },
            Value::StaticMethod { method } => {
                f.debug_struct("StaticMethod")
                    .field("method", method)
                    .finish()
            },
            #[cfg(feature = "ffi")]
            Value::ExternFunction { library_name, name, signature, return_type, param_types } => {
                f.debug_struct("ExternFunction")
                    .field("library_name", library_name)
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
            Value::Generator { .. } => write!(f, "Generator"),
            Value::Coroutine { name, finished, .. } => {
                write!(f, "Coroutine({}, finished={})", name, finished)
            },
            Value::Iterator { items, current_index } => {
                write!(f, "Iterator {{ items: {:?}, current_index: {} }}", items, current_index)
            },
            Value::Exception { class_name, message, .. } => {
                write!(f, "{}({})", class_name, message)
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
            fields: Rc::new(RefCell::new(fields)),
            class_methods: HashMap::new(),
            base_object: BaseObject::new(class_name, parents),
            mro,
        }
    }

    /// Check if a value matches a given type
    pub fn matches_type(&self, type_annotation: &crate::ast::Type) -> bool {
        match (self, type_annotation) {
            // Simple types
            (Value::Int(_), crate::ast::Type::Simple(name)) if name == "int" => true,
            (Value::Float(_), crate::ast::Type::Simple(name)) if name == "float" => true,
            (Value::Str(_), crate::ast::Type::Simple(name)) if name == "str" => true,
            (Value::Bool(_), crate::ast::Type::Simple(name)) if name == "bool" => true,
            (Value::List(_), crate::ast::Type::Simple(name)) if name == "list" => true,
            (Value::Dict(_), crate::ast::Type::Simple(name)) if name == "dict" => true,
            (Value::Tuple(_), crate::ast::Type::Simple(name)) if name == "tuple" => true,
            (Value::Set(_), crate::ast::Type::Simple(name)) if name == "set" => true,
            (Value::None, crate::ast::Type::Simple(name)) if name == "None" || name == "NoneType" => true,
            
            // Any type matches everything
            (_, crate::ast::Type::Any) => true,
            
            // Optional types
            (Value::None, crate::ast::Type::Optional(_)) => true,
            (value, crate::ast::Type::Optional(inner_type)) => value.matches_type(inner_type),
            
            // For other cases, fall back to basic type name checking
            _ => {
                let self_type_name = self.type_name();
                match type_annotation {
                    crate::ast::Type::Simple(name) => {
                        // Simple case-insensitive comparison
                        self_type_name.to_lowercase() == name.to_lowercase()
                    },
                    _ => false
                }
            }
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
    /// Call a dunder method on this value
    pub fn call_dunder_method(&self, method_name: &str, args: Vec<Value>) -> Option<Value> {
        // For now, we'll just return None since we can't actually call the method
        // In a full implementation, this would call the method
        None
    }

    /// Call a binary operator using dunder methods
    pub fn call_binary_op(&self, op: &str, other: &Value) -> Option<Value> {
        // Try to call the dunder method on the left operand
        match self {
            Value::Object { class_methods, .. } => {
                if let Some(method) = class_methods.get(op) {
                    // In a full implementation, we would call the method with 'other' as argument
                    // For now, we'll just return None
                    None
                } else {
                    None
                }
            },
            _ => None
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
    
    /// Create a new exception with traceback information
    pub fn new_exception(class_name: String, message: String, traceback: Option<String>) -> Self {
        Value::Exception {
            class_name,
            message,
            traceback,
        }
    }
    
    /// Get the traceback information from an exception
    pub fn get_traceback(&self) -> Option<&String> {
        match self {
            Value::Exception { traceback, .. } => traceback.as_ref(),
            _ => None,
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
            Value::Starred(value) => format!("*{}", value.debug_string()),
            Value::List(items) => {
                let items_str: Vec<String> = items.as_vec().iter()
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
                let pairs: Vec<String> = dict.borrow().iter()
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
            Value::KwargsMarker(dict) => {
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
             Value::Super(current_class, parent_class, _, _) => {
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
             },
             Value::ClassMethod { .. } => {
                 "<classmethod descriptor>".to_string()
             },
             Value::StaticMethod { .. } => {
                 "<staticmethod descriptor>".to_string()
             },
             Value::Generator { .. } => "<generator object>".to_string(),
             Value::Coroutine { name, finished, .. } => {
                 if *finished {
                     format!("<coroutine {} finished>", name)
                 } else {
                     format!("<coroutine {}>", name)
                 }
             },
             Value::Iterator { .. } => "<iterator object>".to_string(),
             Value::Exception { class_name, message, .. } => format!("{}: {}", class_name, message)
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
            Value::Starred(_) => "starred",
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
            Value::Super(_, _, _, _) => "super",
            Value::Closure { .. } => "function",
            Value::BuiltinFunction(_, _) => "builtin function",
            Value::NativeFunction(_) => "native function",
            Value::Module(_, _) => "module",
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => "extern function",
            Value::None => "None",
            Value::TypedValue { value, .. } => value.type_name(),
            Value::BoundMethod { .. } => "bound method",
            Value::ClassMethod { .. } => "classmethod",
            Value::StaticMethod { .. } => "staticmethod",
            Value::Code(_) => "code",
            Value::KwargsMarker(_) => "dict",
            Value::Generator { .. } => "generator",
            Value::Coroutine { .. } => "coroutine",
            Value::Iterator { .. } => "iterator",
            Value::Exception { class_name, .. } => class_name,
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
            Value::Starred(_) => Type::Simple("starred".to_string()),
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
            Value::Super(_, _, _, _) => Type::Simple("super".to_string()),
            Value::BoundMethod { .. } => Type::Simple("bound method".to_string()),
            Value::ClassMethod { .. } => Type::Simple("classmethod".to_string()),
            Value::StaticMethod { .. } => Type::Simple("staticmethod".to_string()),
            Value::Code(_) => Type::Simple("code".to_string()),
            Value::KwargsMarker(_) => Type::Simple("dict".to_string()),
            Value::Generator { .. } => Type::Simple("generator".to_string()),
            Value::Coroutine { .. } => Type::Simple("coroutine".to_string()),
            Value::Iterator { .. } => Type::Simple("iterator".to_string()),
            Value::Exception { class_name, .. } => Type::Simple(class_name.clone()),
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
            Value::Range { start, stop, step } => {
                let mut hplist = HPList::new();
                if *step > 0 {
                    let mut current = *start;
                    while current < *stop {
                        hplist.append(Value::Int(current));
                        current += step;
                    }
                } else if *step < 0 {
                    let mut current = *start;
                    while current > *stop {
                        hplist.append(Value::Int(current));
                        current += step;
                    }
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
            Value::Range { start, stop, step } => {
                let mut items = Vec::new();
                if *step > 0 {
                    let mut current = *start;
                    while current < *stop {
                        items.push(Value::Int(current));
                        current += step;
                    }
                } else if *step < 0 {
                    let mut current = *start;
                    while current > *stop {
                        items.push(Value::Int(current));
                        current += step;
                    }
                }
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
                    if !unique_items.contains(&item) {
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
            Value::Starred(_) => true,
            Value::List(items) => !items.is_empty(),
            Value::Dict(dict) => !dict.borrow().is_empty(),
            Value::KwargsMarker(dict) => !dict.is_empty(),
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
            Value::Super(_, _, _, _) => true,
            Value::Closure { .. } => true,
            Value::BuiltinFunction(_, _) => true,
            Value::NativeFunction(_) => true,
            Value::Module(_, _) => true,
            #[cfg(feature = "ffi")]
            Value::ExternFunction { .. } => true,
            Value::TypedValue { value, .. } => value.is_truthy(),
            Value::BoundMethod { .. } => true,
            Value::ClassMethod { .. } => true,
            Value::StaticMethod { .. } => true,
            Value::Code(_) => true,
            Value::Generator { .. } => true,
            Value::Coroutine { .. } => true,
            Value::Iterator { .. } => true,
            Value::Exception { .. } => true,
        }
    }
    
    /// Call a method on this value with the given arguments
    /// This is the main entry point for method calls on builtin types
    pub fn call_method(&mut self, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match self {
            Value::Str(s) => Self::call_str_method_static(s.clone(), method_name, args),
            Value::List(ref mut list) => Self::call_list_method_static(list, method_name, args),
            Value::Dict(dict) => Self::call_dict_method_static(dict.clone(), method_name, args),
            Value::KwargsMarker(ref mut dict) => Self::call_dict_method_static_old(dict, method_name, args),
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
            "__len__" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("__len__() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Int(s.len() as i64))
            }
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
                        result.push_str(&ch.to_uppercase().collect::<String>());
                        capitalize_next = false;
                    } else {
                        result.push_str(&ch.to_lowercase().collect::<String>());
                    }
                }
                Ok(Value::Str(result))
            }
            "swapcase" => {
                let mut result = String::new();
                for ch in s.chars() {
                    if ch.is_uppercase() {
                        result.push_str(&ch.to_lowercase().collect::<String>());
                    } else if ch.is_lowercase() {
                        result.push_str(&ch.to_uppercase().collect::<String>());
                    } else {
                        result.push(ch);
                    }
                }
                Ok(Value::Str(result))
            }
            "strip" => {
                if args.is_empty() {
                    Ok(Value::Str(s.trim().to_string()))
                } else if args.len() == 1 {
                    match &args[0] {
                        Value::Str(chars) => Ok(Value::Str(s.trim_matches(|c| chars.contains(c)).to_string())),
                        _ => Err(anyhow::anyhow!("strip() argument must be a string")),
                    }
                } else {
                    Err(anyhow::anyhow!("strip() takes at most 1 argument ({} given)", args.len()))
                }
            }
            "lstrip" => {
                if args.is_empty() {
                    Ok(Value::Str(s.trim_start().to_string()))
                } else if args.len() == 1 {
                    match &args[0] {
                        Value::Str(chars) => Ok(Value::Str(s.trim_start_matches(|c| chars.contains(c)).to_string())),
                        _ => Err(anyhow::anyhow!("lstrip() argument must be a string")),
                    }
                } else {
                    Err(anyhow::anyhow!("lstrip() takes at most 1 argument ({} given)", args.len()))
                }
            }
            "rstrip" => {
                if args.is_empty() {
                    Ok(Value::Str(s.trim_end().to_string()))
                } else if args.len() == 1 {
                    match &args[0] {
                        Value::Str(chars) => Ok(Value::Str(s.trim_end_matches(|c| chars.contains(c)).to_string())),
                        _ => Err(anyhow::anyhow!("rstrip() argument must be a string")),
                    }
                } else {
                    Err(anyhow::anyhow!("rstrip() takes at most 1 argument ({} given)", args.len()))
                }
            }
            "split" => {
                if args.is_empty() {
                    let parts: Vec<Value> = s.split_whitespace().map(|part| Value::Str(part.to_string())).collect();
                    Ok(Value::List(HPList::from_values(parts)))
                } else if args.len() == 1 {
                    match &args[0] {
                        Value::Str(sep) => {
                            let parts: Vec<Value> = s.split(sep).map(|part| Value::Str(part.to_string())).collect();
                            Ok(Value::List(HPList::from_values(parts)))
                        }
                        _ => Err(anyhow::anyhow!("split() argument must be a string")),
                    }
                } else if args.len() == 2 {
                    match (&args[0], &args[1]) {
                        (Value::Str(sep), Value::Int(maxsplit)) => {
                            let parts: Vec<Value> = s.splitn(*maxsplit as usize + 1, sep).map(|part| Value::Str(part.to_string())).collect();
                            Ok(Value::List(HPList::from_values(parts)))
                        }
                        _ => Err(anyhow::anyhow!("split() arguments must be string and int")),
                    }
                } else {
                    Err(anyhow::anyhow!("split() takes at most 2 arguments ({} given)", args.len()))
                }
            }
            "join" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("join() takes exactly 1 argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::List(items) => {
                        let strings: Result<Vec<String>, _> = items.as_vec().iter().map(|item| {
                            match item {
                                Value::Str(s) => Ok(s.clone()),
                                _ => Err(anyhow::anyhow!("sequence item {}: expected str instance, {} found", 0, item.type_name())),
                            }
                        }).collect();
                        match strings {
                            Ok(strs) => Ok(Value::Str(strs.join(&s))),
                            Err(e) => Err(e),
                        }
                    }
                    Value::Tuple(items) => {
                        let strings: Result<Vec<String>, _> = items.iter().map(|item| {
                            match item {
                                Value::Str(s) => Ok(s.clone()),
                                _ => Err(anyhow::anyhow!("sequence item {}: expected str instance, {} found", 0, item.type_name())),
                            }
                        }).collect();
                        match strings {
                            Ok(strs) => Ok(Value::Str(strs.join(&s))),
                            Err(e) => Err(e),
                        }
                    }
                    _ => Err(anyhow::anyhow!("join() argument must be iterable")),
                }
            }
            "replace" => {
                if args.len() < 2 || args.len() > 3 {
                    return Err(anyhow::anyhow!("replace() takes 2 or 3 arguments ({} given)", args.len()));
                }
                match (&args[0], &args[1]) {
                    (Value::Str(old), Value::Str(new)) => {
                        let count = if args.len() == 3 {
                            match &args[2] {
                                Value::Int(n) => *n as usize,
                                _ => return Err(anyhow::anyhow!("replace() count must be an integer")),
                            }
                        } else {
                            usize::MAX // Replace all occurrences
                        };
                        Ok(Value::Str(s.replacen(old, new, count)))
                    }
                    _ => Err(anyhow::anyhow!("replace() arguments must be strings")),
                }
            }
            "startswith" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("startswith() takes at least 1 argument (0 given)"));
                }
                match &args[0] {
                    Value::Str(prefix) => Ok(Value::Bool(s.starts_with(prefix))),
                    Value::Tuple(prefixes) => {
                        let mut result = false;
                        for prefix in prefixes {
                            if let Value::Str(prefix_str) = prefix {
                                if s.starts_with(prefix_str) {
                                    result = true;
                                    break;
                                }
                            }
                        }
                        Ok(Value::Bool(result))
                    }
                    _ => Err(anyhow::anyhow!("startswith() argument must be str or tuple of str")),
                }
            }
            "endswith" => {
                if args.is_empty() {
                    return Err(anyhow::anyhow!("endswith() takes at least 1 argument (0 given)"));
                }
                match &args[0] {
                    Value::Str(suffix) => Ok(Value::Bool(s.ends_with(suffix))),
                    Value::Tuple(suffixes) => {
                        let mut result = false;
                        for suffix in suffixes {
                            if let Value::Str(suffix_str) = suffix {
                                if s.ends_with(suffix_str) {
                                    result = true;
                                    break;
                                }
                            }
                        }
                        Ok(Value::Bool(result))
                    }
                    _ => Err(anyhow::anyhow!("endswith() argument must be str or tuple of str")),
                }
            }
            "find" => {
                if args.is_empty() || args.len() > 3 {
                    return Err(anyhow::anyhow!("find() takes 1 to 3 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(sub) => {
                        let start = if args.len() >= 2 {
                            match &args[1] {
                                Value::Int(n) => *n,
                                _ => return Err(anyhow::anyhow!("find() start must be an integer")),
                            }
                        } else {
                            0
                        };
                        let end = if args.len() >= 3 {
                            match &args[2] {
                                Value::Int(n) => *n,
                                _ => return Err(anyhow::anyhow!("find() end must be an integer")),
                            }
                        } else {
                            s.len() as i64
                        };
                        
                        // Normalize indices
                        let start_idx = if start < 0 {
                            (s.len() as i64 + start).max(0) as usize
                        } else {
                            start.min(s.len() as i64) as usize
                        };
                        
                        let end_idx = if end < 0 {
                            (s.len() as i64 + end).max(0) as usize
                        } else {
                            end.min(s.len() as i64) as usize
                        };
                        
                        if start_idx <= end_idx && start_idx <= s.len() {
                            let search_str = &s[start_idx..end_idx.min(s.len())];
                            if let Some(pos) = search_str.find(sub) {
                                Ok(Value::Int((start_idx + pos) as i64))
                            } else {
                                Ok(Value::Int(-1))
                            }
                        } else {
                            Ok(Value::Int(-1))
                        }
                    }
                    _ => Err(anyhow::anyhow!("find() argument must be a string")),
                }
            }
            "count" => {
                if args.is_empty() || args.len() > 3 {
                    return Err(anyhow::anyhow!("count() takes 1 to 3 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(sub) => {
                        let start = if args.len() >= 2 {
                            match &args[1] {
                                Value::Int(n) => *n,
                                _ => return Err(anyhow::anyhow!("count() start must be an integer")),
                            }
                        } else {
                            0
                        };
                        let end = if args.len() >= 3 {
                            match &args[2] {
                                Value::Int(n) => *n,
                                _ => return Err(anyhow::anyhow!("count() end must be an integer")),
                            }
                        } else {
                            s.len() as i64
                        };
                        
                        // Normalize indices
                        let start_idx = if start < 0 {
                            (s.len() as i64 + start).max(0) as usize
                        } else {
                            start.min(s.len() as i64) as usize
                        };
                        
                        let end_idx = if end < 0 {
                            (s.len() as i64 + end).max(0) as usize
                        } else {
                            end.min(s.len() as i64) as usize
                        };
                        
                        if start_idx <= end_idx && start_idx <= s.len() {
                            let search_str = &s[start_idx..end_idx.min(s.len())];
                            Ok(Value::Int(search_str.matches(sub).count() as i64))
                        } else {
                            Ok(Value::Int(0))
                        }
                    }
                    _ => Err(anyhow::anyhow!("count() argument must be a string")),
                }
            }
            _ => Err(anyhow::anyhow!("'str' object has no attribute '{}'", method_name)),
        }
    }

    /// List method implementations
    fn call_list_method_static(list: &mut HPList, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "__len__" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("__len__() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Int(list.len() as i64))
            }
            "append" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("append() takes exactly one argument ({} given)", args.len()));
                }
                list.append(args[0].clone());
                Ok(Value::None)
            }
            "extend" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("extend() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::List(other) => {
                        for item in other.iter() {
                            list.append(item.clone());
                        }
                        Ok(Value::None)
                    }
                    Value::Tuple(items) => {
                        for item in items {
                            list.append(item.clone());
                        }
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("extend() argument must be iterable")),
                }
            }
            "insert" => {
                if args.len() != 2 {
                    return Err(anyhow::anyhow!("insert() takes exactly two arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Int(index) => {
                        list.insert(*index as isize, args[1].clone())
                            .map_err(|e| anyhow::anyhow!("Error inserting into list: {}", e))?;
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("insert() index must be an integer")),
                }
            }
            "remove" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("remove() takes exactly one argument ({} given)", args.len()));
                }
                list.remove(&args[0])
                    .map_err(|_| anyhow::anyhow!("list.remove(x): x not in list"))?;
                Ok(Value::None)
            }
            "pop" => {
                let item = if args.is_empty() {
                    list.pop().ok_or_else(|| anyhow::anyhow!("pop from empty list"))?
                } else if args.len() == 1 {
                    match &args[0] {
                        Value::Int(index) => {
                            list.pop_at(*index as isize)
                                .map_err(|_| anyhow::anyhow!("pop index out of range"))?
                        }
                        _ => return Err(anyhow::anyhow!("pop() index must be an integer")),
                    }
                } else {
                    return Err(anyhow::anyhow!("pop() takes at most 1 argument ({} given)", args.len()));
                };
                Ok(item)
            }
            "clear" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("clear() takes no arguments ({} given)", args.len()));
                }
                list.clear();
                Ok(Value::None)
            }
            "index" => {
                if args.is_empty() || args.len() > 3 {
                    return Err(anyhow::anyhow!("index() takes 1 to 3 arguments ({} given)", args.len()));
                }
                let start = if args.len() >= 2 {
                    match &args[1] {
                        Value::Int(n) => Some(*n as usize),
                        _ => return Err(anyhow::anyhow!("index() start must be an integer")),
                    }
                } else {
                    None
                };
                let stop = if args.len() >= 3 {
                    match &args[2] {
                        Value::Int(n) => Some(*n as usize),
                        _ => return Err(anyhow::anyhow!("index() stop must be an integer")),
                    }
                } else {
                    None
                };
                let pos = list.index(&args[0], start, stop)
                    .map_err(|_| anyhow::anyhow!("list.index(x): x not in list"))?;
                Ok(Value::Int(pos as i64))
            }
            "count" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("count() takes exactly one argument ({} given)", args.len()));
                }
                let count = list.count(&args[0]);
                Ok(Value::Int(count as i64))
            }
            "sort" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("sort() takes no arguments ({} given)", args.len()));
                }
                list.sort();
                Ok(Value::None)
            }
            "reverse" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("reverse() takes no arguments ({} given)", args.len()));
                }
                list.reverse();
                Ok(Value::None)
            }
            "copy" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("copy() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::List(list.copy()))
            }
            _ => Err(anyhow::anyhow!("'list' object has no attribute '{}'", method_name)),
        }
    }

    /// Dict method implementations
    fn call_dict_method_static(dict: Rc<RefCell<HashMap<String, Value>>>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "__len__" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("__len__() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Int(dict.borrow().len() as i64))
            }
            "clear" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("clear() takes no arguments ({} given)", args.len()));
                }
                dict.borrow_mut().clear();
                Ok(Value::None)
            }
            "copy" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("copy() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Dict(Rc::new(RefCell::new(dict.borrow().clone()))))
            }
            "get" => {
                if args.is_empty() || args.len() > 2 {
                    return Err(anyhow::anyhow!("get() takes 1 to 2 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(key) => {
                        let default_value = if args.len() == 2 {
                            args[1].clone()
                        } else {
                            Value::None
                        };
                        Ok(dict.borrow().get(key).cloned().unwrap_or(default_value))
                    }
                    _ => Err(anyhow::anyhow!("get() key must be a string")),
                }
            }
            "items" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("items() takes no arguments ({} given)", args.len()));
                }
                let mut items = Vec::new();
                for (key, value) in dict.borrow().iter() {
                    items.push(Value::Tuple(vec![Value::Str(key.clone()), value.clone()]));
                }
                Ok(Value::List(HPList::from_values(items)))
            }
            "keys" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("keys() takes no arguments ({} given)", args.len()));
                }
                let keys: Vec<Value> = dict.borrow().keys().map(|k| Value::Str(k.clone())).collect();
                Ok(Value::List(HPList::from_values(keys)))
            }
            "values" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("values() takes no arguments ({} given)", args.len()));
                }
                let values: Vec<Value> = dict.borrow().values().cloned().collect();
                Ok(Value::List(HPList::from_values(values)))
            }
            "pop" => {
                if args.is_empty() || args.len() > 2 {
                    return Err(anyhow::anyhow!("pop() takes 1 to 2 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(key) => {
                        if args.len() == 1 {
                            dict.borrow_mut().remove(key).ok_or_else(|| anyhow::anyhow!("pop(): key '{}' not found", key))
                        } else {
                            Ok(dict.borrow_mut().remove(key).unwrap_or_else(|| args[1].clone()))
                        }
                    }
                    _ => Err(anyhow::anyhow!("pop() key must be a string")),
                }
            }
            "popitem" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("popitem() takes no arguments ({} given)", args.len()));
                }
                dict.borrow_mut().drain().next().map(|(k, v)| Value::Tuple(vec![Value::Str(k), v]))
                    .ok_or_else(|| anyhow::anyhow!("popitem(): dictionary is empty"))
            }
            "setdefault" => {
                if args.is_empty() || args.len() > 2 {
                    return Err(anyhow::anyhow!("setdefault() takes 1 to 2 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(key) => {
                        let default_value = if args.len() == 2 {
                            args[1].clone()
                        } else {
                            Value::None
                        };
                        Ok(dict.borrow_mut().entry(key.clone()).or_insert(default_value).clone())
                    }
                    _ => Err(anyhow::anyhow!("setdefault() key must be a string")),
                }
            }
            "update" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("update() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Dict(other) => {
                        dict.borrow_mut().extend(other.borrow().clone());
                        Ok(Value::None)
                    }
                    Value::List(items) => {
                        for item in items.as_vec().iter() {
                            match item {
                                Value::Tuple(pair) if pair.len() == 2 => {
                                    if let (Value::Str(key), value) = (&pair[0], &pair[1]) {
                                        dict.borrow_mut().insert(key.clone(), value.clone());
                                    } else {
                                        return Err(anyhow::anyhow!("update() argument must be a dictionary or iterable of key-value pairs"));
                                    }
                                }
                                _ => return Err(anyhow::anyhow!("update() argument must be a dictionary or iterable of key-value pairs")),
                            }
                        }
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("update() argument must be a dictionary or iterable of key-value pairs")),
                }
            }
            _ => Err(anyhow::anyhow!("'dict' object has no attribute '{}'", method_name)),
        }
    }

    /// Dict method implementations for KwargsMarker (old style HashMap)
    fn call_dict_method_static_old(dict: &mut HashMap<String, Value>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "__len__" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("__len__() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Int(dict.len() as i64))
            }
            "clear" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("clear() takes no arguments ({} given)", args.len()));
                }
                dict.clear();
                Ok(Value::None)
            }
            "copy" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("copy() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::KwargsMarker(dict.clone()))
            }
            "get" => {
                if args.is_empty() || args.len() > 2 {
                    return Err(anyhow::anyhow!("get() takes 1 to 2 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(key) => {
                        let default_value = if args.len() == 2 {
                            args[1].clone()
                        } else {
                            Value::None
                        };
                        Ok(dict.get(key).cloned().unwrap_or(default_value))
                    }
                    _ => Err(anyhow::anyhow!("get() key must be a string")),
                }
            }
            "items" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("items() takes no arguments ({} given)", args.len()));
                }
                let mut items = Vec::new();
                for (key, value) in dict.iter() {
                    items.push(Value::Tuple(vec![Value::Str(key.clone()), value.clone()]));
                }
                Ok(Value::List(HPList::from_values(items)))
            }
            "keys" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("keys() takes no arguments ({} given)", args.len()));
                }
                let keys: Vec<Value> = dict.keys().map(|k| Value::Str(k.clone())).collect();
                Ok(Value::List(HPList::from_values(keys)))
            }
            "values" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("values() takes no arguments ({} given)", args.len()));
                }
                let values: Vec<Value> = dict.values().cloned().collect();
                Ok(Value::List(HPList::from_values(values)))
            }
            "pop" => {
                if args.is_empty() || args.len() > 2 {
                    return Err(anyhow::anyhow!("pop() takes 1 to 2 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(key) => {
                        if args.len() == 1 {
                            dict.remove(key).ok_or_else(|| anyhow::anyhow!("pop(): key '{}' not found", key))
                        } else {
                            Ok(dict.remove(key).unwrap_or_else(|| args[1].clone()))
                        }
                    }
                    _ => Err(anyhow::anyhow!("pop() key must be a string")),
                }
            }
            "popitem" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("popitem() takes no arguments ({} given)", args.len()));
                }
                dict.drain().next().map(|(k, v)| Value::Tuple(vec![Value::Str(k), v]))
                    .ok_or_else(|| anyhow::anyhow!("popitem(): dictionary is empty"))
            }
            "setdefault" => {
                if args.is_empty() || args.len() > 2 {
                    return Err(anyhow::anyhow!("setdefault() takes 1 to 2 arguments ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Str(key) => {
                        let default_value = if args.len() == 2 {
                            args[1].clone()
                        } else {
                            Value::None
                        };
                        Ok(dict.entry(key.clone()).or_insert(default_value).clone())
                    }
                    _ => Err(anyhow::anyhow!("setdefault() key must be a string")),
                }
            }
            "update" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("update() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Dict(other) => {
                        dict.extend(other.borrow().clone());
                        Ok(Value::None)
                    }
                    Value::KwargsMarker(other) => {
                        dict.extend(other.clone());
                        Ok(Value::None)
                    }
                    Value::List(items) => {
                        for item in items.as_vec().iter() {
                            match item {
                                Value::Tuple(pair) if pair.len() == 2 => {
                                    if let (Value::Str(key), value) = (&pair[0], &pair[1]) {
                                        dict.insert(key.clone(), value.clone());
                                    } else {
                                        return Err(anyhow::anyhow!("update() argument must be a dictionary or iterable of key-value pairs"));
                                    }
                                }
                                _ => return Err(anyhow::anyhow!("update() argument must be a dictionary or iterable of key-value pairs")),
                            }
                        }
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("update() argument must be a dictionary or iterable of key-value pairs")),
                }
            }
            _ => Err(anyhow::anyhow!("'dict' object has no attribute '{}'", method_name)),
        }
    }

    /// Set method implementations
    fn call_set_method_static(set: &mut Vec<Value>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "__len__" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("__len__() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Int(set.len() as i64))
            }
            "add" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("add() takes exactly one argument ({} given)", args.len()));
                }
                if !set.contains(&args[0]) {
                    set.push(args[0].clone());
                }
                Ok(Value::None)
            }
            "clear" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("clear() takes no arguments ({} given)", args.len()));
                }
                set.clear();
                Ok(Value::None)
            }
            "copy" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("copy() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Set(set.clone()))
            }
            "discard" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("discard() takes exactly one argument ({} given)", args.len()));
                }
                set.retain(|item| item != &args[0]);
                Ok(Value::None)
            }
            "pop" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("pop() takes no arguments ({} given)", args.len()));
                }
                set.pop().ok_or_else(|| anyhow::anyhow!("pop(): set is empty"))
            }
            "remove" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("remove() takes exactly one argument ({} given)", args.len()));
                }
                let pos = set.iter().position(|item| item == &args[0])
                    .ok_or_else(|| anyhow::anyhow!("remove(): key not found"))?;
                set.remove(pos);
                Ok(Value::None)
            }
            "union" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("union() takes exactly one argument ({} given)", args.len()));
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
                    _ => Err(anyhow::anyhow!("union() argument must be a set")),
                }
            }
            "intersection" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("intersection() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Set(other) => {
                        let mut result = Vec::new();
                        for item in set {
                            if other.contains(item) && !result.contains(item) {
                                result.push(item.clone());
                            }
                        }
                        Ok(Value::Set(result))
                    }
                    _ => Err(anyhow::anyhow!("intersection() argument must be a set")),
                }
            }
            "difference" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("difference() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Set(other) => {
                        let mut result = Vec::new();
                        for item in set {
                            if !other.contains(item) && !result.contains(item) {
                                result.push(item.clone());
                            }
                        }
                        Ok(Value::Set(result))
                    }
                    _ => Err(anyhow::anyhow!("difference() argument must be a set")),
                }
            }
            _ => Err(anyhow::anyhow!("'set' object has no attribute '{}'", method_name)),
        }
    }

    /// Tuple method implementations
    fn call_tuple_method_static(tuple: Vec<Value>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "__len__" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("__len__() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Int(tuple.len() as i64))
            }
            "count" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("count() takes exactly one argument ({} given)", args.len()));
                }
                let count = tuple.iter().filter(|&item| item == &args[0]).count();
                Ok(Value::Int(count as i64))
            }
            "index" => {
                if args.is_empty() || args.len() > 3 {
                    return Err(anyhow::anyhow!("index() takes 1 to 3 arguments ({} given)", args.len()));
                }
                let start = if args.len() >= 2 {
                    match &args[1] {
                        Value::Int(n) => *n.max(&0) as usize,
                        _ => return Err(anyhow::anyhow!("index() start must be an integer")),
                    }
                } else {
                    0
                };
                let stop = if args.len() >= 3 {
                    match &args[2] {
                        Value::Int(n) => *n.max(&0) as usize,
                        _ => return Err(anyhow::anyhow!("index() stop must be an integer")),
                    }
                } else {
                    tuple.len()
                };
                
                for (i, item) in tuple.iter().enumerate().skip(start).take(stop.saturating_sub(start)) {
                    if item == &args[0] {
                        return Ok(Value::Int(i as i64));
                    }
                }
                Err(anyhow::anyhow!("tuple.index(x): x not in tuple"))
            }
            _ => Err(anyhow::anyhow!("'tuple' object has no attribute '{}'", method_name)),
        }
    }

    /// Int method implementations
    fn call_int_method_static(n: i64, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "bit_length" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("bit_length() takes no arguments ({} given)", args.len()));
                }
                // Calculate the number of bits needed to represent the absolute value of n
                Ok(Value::Int((64 - n.abs().leading_zeros()) as i64))
            }
            "to_bytes" => {
                if args.len() < 2 || args.len() > 4 {
                    return Err(anyhow::anyhow!("to_bytes() takes 2 to 4 arguments ({} given)", args.len()));
                }
                match (&args[0], &args[1]) {
                    (Value::Int(length), Value::Str(byteorder)) => {
                        let signed = if args.len() >= 3 {
                            match &args[2] {
                                Value::Bool(b) => *b,
                                _ => return Err(anyhow::anyhow!("to_bytes() signed must be a boolean")),
                            }
                        } else {
                            false
                        };
                        
                        if *length < 0 {
                            return Err(anyhow::anyhow!("length argument must be non-negative"));
                        }
                        
                        let bytes = match byteorder.as_str() {
                            "big" => {
                                if signed {
                                    n.to_be_bytes().to_vec()
                                } else {
                                    (n as u64).to_be_bytes().to_vec()
                                }
                            }
                            "little" => {
                                if signed {
                                    n.to_le_bytes().to_vec()
                                } else {
                                    (n as u64).to_le_bytes().to_vec()
                                }
                            }
                            _ => return Err(anyhow::anyhow!("to_bytes() byteorder must be 'big' or 'little'")),
                        };
                        
                        // Truncate or pad to the specified length
                        let mut result = if bytes.len() > *length as usize {
                            bytes[bytes.len() - *length as usize..].to_vec()
                        } else {
                            let mut padded = vec![0; *length as usize - bytes.len()];
                            padded.extend_from_slice(&bytes);
                            padded
                        };
                        
                        // Reverse for little endian
                        if byteorder == "little" {
                            result.reverse();
                        }
                        
                        Ok(Value::Bytes(result))
                    }
                    _ => Err(anyhow::anyhow!("to_bytes() arguments must be int and str")),
                }
            }
            _ => Err(anyhow::anyhow!("'int' object has no attribute '{}'", method_name)),
        }
    }

    /// Float method implementations
    fn call_float_method_static(f: f64, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "is_integer" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("is_integer() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Bool(f.fract() == 0.0))
            }
            "hex" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("hex() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::Str(format!("{:x}", f.to_bits())))
            }
            _ => Err(anyhow::anyhow!("'float' object has no attribute '{}'", method_name)),
        }
    }

    /// Bytes method implementations
    fn call_bytes_method_static(bytes: Vec<u8>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "decode" => {
                if args.len() > 1 {
                    return Err(anyhow::anyhow!("decode() takes at most 1 argument ({} given)", args.len()));
                }
                let encoding = if args.is_empty() {
                    "utf-8"
                } else {
                    match &args[0] {
                        Value::Str(s) => s.as_str(),
                        _ => return Err(anyhow::anyhow!("decode() argument must be a string")),
                    }
                };
                
                match encoding {
                    "utf-8" => {
                        match String::from_utf8(bytes) {
                            Ok(s) => Ok(Value::Str(s)),
                            Err(e) => Err(anyhow::anyhow!("decode() failed: {}", e)),
                        }
                    }
                    _ => Err(anyhow::anyhow!("decode() encoding '{}' not supported", encoding)),
                }
            }
            "hex" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("hex() takes no arguments ({} given)", args.len()));
                }
                let hex_string: String = bytes.iter().map(|b| format!("{:02x}", b)).collect();
                Ok(Value::Str(hex_string))
            }
            _ => Err(anyhow::anyhow!("'bytes' object has no attribute '{}'", method_name)),
        }
    }

    /// Bytearray method implementations
    fn call_bytearray_method_static(ba: &mut Vec<u8>, method_name: &str, args: Vec<Value>) -> anyhow::Result<Value> {
        match method_name {
            "append" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("append() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Int(n) if *n >= 0 && *n <= 255 => {
                        ba.push(*n as u8);
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("append() argument must be an integer between 0 and 255")),
                }
            }
            "clear" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("clear() takes no arguments ({} given)", args.len()));
                }
                ba.clear();
                Ok(Value::None)
            }
            "copy" => {
                if !args.is_empty() {
                    return Err(anyhow::anyhow!("copy() takes no arguments ({} given)", args.len()));
                }
                Ok(Value::ByteArray(ba.clone()))
            }
            "extend" => {
                if args.len() != 1 {
                    return Err(anyhow::anyhow!("extend() takes exactly one argument ({} given)", args.len()));
                }
                match &args[0] {
                    Value::Bytes(other) => {
                        ba.extend_from_slice(other);
                        Ok(Value::None)
                    }
                    Value::ByteArray(other) => {
                        ba.extend_from_slice(other);
                        Ok(Value::None)
                    }
                    Value::List(items) => {
                        for item in items.as_vec().iter() {
                            match item {
                                Value::Int(n) if *n >= 0 && *n <= 255 => ba.push(*n as u8),
                                _ => return Err(anyhow::anyhow!("extend() argument must be iterable of integers between 0 and 255")),
                            }
                        }
                        Ok(Value::None)
                    }
                    _ => Err(anyhow::anyhow!("extend() argument must be bytes, bytearray, or iterable of integers")),
                }
            }
            _ => Err(anyhow::anyhow!("'bytearray' object has no attribute '{}'", method_name)),
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
            Value::Starred(value) => write!(f, "*{}", value),
            Value::List(items) => {
                let items_str: Vec<String> = items.as_vec().iter().map(|v| format!("{}", v)).collect();
                write!(f, "[{}]", items_str.join(", "))
            }
            Value::Dict(dict) => {
                let pairs: Vec<String> = dict.borrow().iter()
                    .map(|(k, v)| format!("{}: {}", k, v))
                    .collect();
                write!(f, "{{{}}}", pairs.join(", "))
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
                write!(f, "frozenset({{{}}})", items_str.join(", "))
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
            Value::KwargsMarker(dict) => {
                let pairs: Vec<String> = dict.iter()
                    .map(|(k, v)| format!("{}: {}", k, v))
                    .collect();
                write!(f, "{{{}}}", pairs.join(", "))
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
            Value::Super(current_class, parent_class, _, _) => {
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
            Value::ClassMethod { .. } => {
                write!(f, "<classmethod descriptor>")
            }
            Value::StaticMethod { .. } => {
                write!(f, "<staticmethod descriptor>")
            }
            Value::Code(code_obj) => {
                write!(f, "<code object {}>", code_obj.name)
            },
            Value::Generator { .. } => write!(f, "<generator object>"),
            Value::Coroutine { name, finished, .. } => {
                if *finished {
                    write!(f, "<coroutine {} finished>", name)
                } else {
                    write!(f, "<coroutine {}>", name)
                }
            },
            Value::Iterator { .. } => write!(f, "<iterator object>"),
            Value::Exception { message, .. } => {
                // When displaying an exception object (like in print(e)), just show the message
                // The class name will be shown in the traceback
                write!(f, "{}", message)
            },
        }
    }
}

impl Value {
    // Method callers (these would be implemented to call the static methods)
    fn call_str_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("str method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        Value::call_str_method_static(String::new(), method_name, args[1..].to_vec())
    }

    fn call_list_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("list method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        // We can't actually call list methods without a mutable reference to the list
        // This is a limitation of this approach - in a real implementation, 
        // method calls would be handled by the VM with proper references
        Err(anyhow::anyhow!("List method '{}' cannot be called directly", method_name))
    }

    fn call_dict_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.len() < 2 {
            return Err(anyhow::anyhow!("dict method called with insufficient arguments"));
        }

        // First arg is method name, second is the dict itself (self)
        let method_name = if let Value::Str(name) = &args[0] {
            name.clone()
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };

        let dict = if let Value::Dict(d) = &args[1] {
            d.clone()
        } else {
            return Err(anyhow::anyhow!("Second argument must be a dict, got {}", args[1].type_name()));
        };

        // Remaining args are the actual method arguments
        let method_args = if args.len() > 2 {
            args[2..].to_vec()
        } else {
            vec![]
        };

        Self::call_dict_method_static(dict, &method_name, method_args)
    }

    fn call_set_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("set method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        // We can't actually call bytearray methods without a mutable reference to the bytearray
        // This is a limitation of this approach - in a real implementation, 
        // method calls would be handled by the VM with proper references
        Err(anyhow::anyhow!("Bytearray method '{}' cannot be called directly", method_name))
    }

    fn call_bytearray_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("bytearray method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        // We can't actually call bytearray methods without a mutable reference to the bytearray
        // This is a limitation of this approach - in a real implementation, 
        // method calls would be handled by the VM with proper references
        Err(anyhow::anyhow!("Bytearray method '{}' cannot be called directly", method_name))
    }

    /// Get method for this value (used for method resolution)
    pub fn get_method(&self, method_name: &str) -> Option<Value> {
        match self {
            Value::Str(_) => {
                // String methods
                match method_name {
                    "upper" | "lower" | "capitalize" | "title" | "swapcase" |
                    "strip" | "lstrip" | "rstrip" | "split" | "join" | "replace" |
                    "startswith" | "endswith" | "find" | "count" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_str_method))
                    }
                    _ => None,
                }
            }
            Value::List(_) => {
                // List methods
                match method_name {
                    "append" | "extend" | "insert" | "remove" | "pop" | "clear" |
                    "index" | "count" | "sort" | "reverse" | "copy" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_list_method))
                    }
                    _ => None,
                }
            }
            Value::Dict(_) => {
                // Dict methods
                match method_name {
                    "clear" | "copy" | "get" | "items" | "keys" | "values" |
                    "pop" | "popitem" | "setdefault" | "update" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_dict_method))
                    }
                    _ => None,
                }
            }
            Value::Set(_) => {
                // Set methods
                match method_name {
                    "add" | "clear" | "copy" | "discard" | "pop" | "remove" |
                    "union" | "intersection" | "difference" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_set_method))
                    }
                    _ => None,
                }
            }
            Value::Tuple(_) => {
                // Tuple methods
                match method_name {
                    "count" | "index" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_tuple_method))
                    }
                    _ => None,
                }
            }
            Value::Int(_) => {
                // Int methods
                match method_name {
                    "bit_length" | "to_bytes" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_int_method))
                    }
                    _ => None,
                }
            }
            Value::Float(_) => {
                // Float methods
                match method_name {
                    "is_integer" | "hex" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_float_method))
                    }
                    _ => None,
                }
            }
            Value::Bytes(_) => {
                // Bytes methods
                match method_name {
                    "decode" | "hex" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_bytes_method))
                    }
                    _ => None,
                }
            }
            Value::ByteArray(_) => {
                // Bytearray methods
                match method_name {
                    "append" | "clear" | "copy" | "extend" => {
                        Some(Value::BuiltinFunction(method_name.to_string(), Self::call_bytearray_method))
                    }
                    _ => None,
                }
            }
            Value::Object { class_methods, .. } => {
                // For custom objects, check class_methods for the method
                class_methods.get(method_name).cloned()
            }
            _ => None,
        }
    }

    fn call_tuple_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("tuple method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        Value::call_tuple_method_static(vec![], method_name, args[1..].to_vec())
    }

    fn call_int_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("int method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        Value::call_int_method_static(0, method_name, args[1..].to_vec())
    }

    fn call_float_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("float method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        Value::call_float_method_static(0.0, method_name, args[1..].to_vec())
    }

    fn call_bytes_method(args: Vec<Value>) -> anyhow::Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("bytes method called with no arguments"));
        }
        let method_name = if let Value::Str(name) = &args[0] {
            name
        } else {
            return Err(anyhow::anyhow!("First argument must be method name"));
        };
        Value::call_bytes_method_static(vec![], method_name, args[1..].to_vec())
    }
}

