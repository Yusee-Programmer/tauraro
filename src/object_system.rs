use crate::base_object::{BaseObject as OriginalBaseObject, MRO};
use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::{HashMap, HashSet};
use std::rc::Rc;
use std::cell::RefCell;
use std::fmt;

/// CPython-inspired object model for TauraroLang
/// Provides reference counting and proper type slots

/// Object inspired by CPython's PyObject
/// Every value in TauraroLang can be wrapped in this for enhanced functionality
#[derive(Debug, Clone)]
pub struct TauraroObject {
    /// Reference to the type information
    pub type_info: Rc<TauraroType>,
    /// Reference count for memory management (like CPython's ob_refcnt)
    pub ref_count: RefCell<usize>,
    /// The actual value data
    pub value: Value,
}

/// Type object inspired by CPython's PyTypeObject
/// Defines the behavior and characteristics of a type
#[derive(Debug, Clone)]
pub struct TauraroType {
    /// Type name (like CPython's tp_name)
    pub name: String,
    /// Basic size of instances (like CPython's tp_basicsize)
    pub basic_size: usize,
    /// Type flags (like CPython's tp_flags)
    pub flags: TypeFlags,
    /// Type slots containing function pointers (like CPython's tp_* slots)
    pub slots: TypeSlots,
    /// Method resolution order
    pub mro: Vec<String>,
    /// Base classes
    pub bases: Vec<String>,
    /// Methods defined on this type
    pub methods: HashMap<String, fn(&[Value]) -> Result<Value>>,
}

/// Type flags similar to CPython's tp_flags
#[derive(Debug, Clone)]
pub struct TypeFlags {
    pub is_builtin: bool,
    pub supports_gc: bool,
    pub is_base_type: bool,
    pub supports_weakrefs: bool,
}

/// Type slots structure containing function pointers for various operations
/// Enhanced with CPython's complete protocol support
#[derive(Clone)]
pub struct TypeSlots {
    /// Number protocol slots (like PyNumberMethods)
    pub number: Option<NumberSlots>,
    /// Sequence protocol slots (like PySequenceMethods)  
    pub sequence: Option<SequenceSlots>,
    /// Mapping protocol slots (like PyMappingMethods)
    pub mapping: Option<MappingSlots>,
    /// Iterator protocol slots
    pub iterator: Option<IteratorSlots>,
    /// Async protocol slots
    pub async_: Option<AsyncSlots>,
    /// Buffer protocol slots
    pub buffer: Option<BufferSlots>,
    
    /// Basic object slots
    pub init: Option<Rc<dyn Fn(&[Value]) -> Result<Value>>>,
    pub repr: Option<Rc<dyn Fn(&Value) -> Result<String>>>,
    pub str: Option<Rc<dyn Fn(&Value) -> Result<String>>>,
    pub hash: Option<Rc<dyn Fn(&Value) -> Result<i64>>>,
    pub call: Option<Rc<dyn Fn(&Value, &[Value]) -> Result<Value>>>,
    pub bool_: Option<Rc<dyn Fn(&Value) -> Result<bool>>>,
    pub len: Option<Rc<dyn Fn(&Value) -> Result<usize>>>,
    
    /// Rich comparison slots
    pub eq: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub ne: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub lt: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub le: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub gt: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub ge: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    
    /// Attribute access slots
    pub getattr: Option<Rc<dyn Fn(&Value, &str) -> Result<Value>>>,
    pub setattr: Option<Rc<dyn Fn(&Value, &str, &Value) -> Result<()>>>,
    pub delattr: Option<Rc<dyn Fn(&Value, &str) -> Result<()>>>,
    
    /// Descriptor protocol slots
    pub get: Option<Rc<dyn Fn(&Value, &Value, &Value) -> Result<Value>>>,
    pub set: Option<Rc<dyn Fn(&Value, &Value, &Value) -> Result<()>>>,
    pub delete: Option<Rc<dyn Fn(&Value, &Value) -> Result<()>>>,
}

/// Number protocol slots (like PyNumberMethods)
#[derive(Clone)]
pub struct NumberSlots {
    pub add: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub subtract: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub multiply: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub divide: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub remainder: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub power: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub negative: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub positive: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub absolute: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub invert: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub lshift: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub rshift: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub and: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub xor: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub or: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
}

/// Sequence protocol slots (like PySequenceMethods)
#[derive(Clone)]
pub struct SequenceSlots {
    pub length: Option<Rc<dyn Fn(&Value) -> Result<usize>>>,
    pub item: Option<Rc<dyn Fn(&Value, isize) -> Result<Value>>>,
    pub set_item: Option<Rc<dyn Fn(&Value, isize, &Value) -> Result<()>>>,
    pub contains: Option<Rc<dyn Fn(&Value, &Value) -> Result<bool>>>,
    pub concat: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub repeat: Option<Rc<dyn Fn(&Value, isize) -> Result<Value>>>,
}

/// Mapping protocol slots (like PyMappingMethods)
#[derive(Clone)]
pub struct MappingSlots {
    pub length: Option<Rc<dyn Fn(&Value) -> Result<usize>>>,
    pub subscript: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value>>>,
    pub set_subscript: Option<Rc<dyn Fn(&Value, &Value, &Value) -> Result<()>>>,
}

/// Iterator protocol slots
#[derive(Clone)]
pub struct IteratorSlots {
    pub iter: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub next: Option<Rc<dyn Fn(&Value) -> Result<Option<Value>>>>,
}

/// Async protocol slots
#[derive(Clone)]
pub struct AsyncSlots {
    pub await_: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub aiter: Option<Rc<dyn Fn(&Value) -> Result<Value>>>,
    pub anext: Option<Rc<dyn Fn(&Value) -> Result<Option<Value>>>>,
}

/// Buffer protocol slots
#[derive(Clone)]
pub struct BufferSlots {
    pub get_buffer: Option<Rc<dyn Fn(&Value) -> Result<Vec<u8>>>>,
    pub release_buffer: Option<Rc<dyn Fn(&Value) -> Result<()>>>,
}

/// CPython-inspired metaclass system for class creation and MRO computation
/// Implements type() builtin and advanced metaclass functionality
#[derive(Debug, Clone)]
pub struct MetaClass {
    pub name: String,
    pub bases: Vec<String>,
    pub namespace: HashMap<String, Value>,
    pub mro_cache: Option<Vec<String>>,
    pub custom_mro_method: Option<String>,
}

/// Optimized MRO computation system with caching and validation
/// Implements C3 linearization algorithm used by CPython
#[derive(Debug, Clone)]
pub struct MROComputer {
    /// Cache of computed MROs to avoid recomputation
    cache: HashMap<String, Vec<String>>,
    /// Validation cache to avoid re-validating the same hierarchies
    validation_cache: HashMap<String, Result<(), String>>,
}

/// Type creation system that mimics Python's type() builtin
/// Provides comprehensive class creation with metaclass support
#[derive(Debug, Clone)]
pub struct TypeCreator {
    pub mro_computer: MROComputer,
}

/// Built-in type hierarchy system - ensures all types inherit from object
/// Provides MRO computation for built-in types like int, str, list, etc.
#[derive(Debug, Clone)]
pub struct TypeHierarchy;

/// Type registry for managing all types in the system
/// This replaces the scattered type management we had before
#[derive(Clone)]
pub struct TypeRegistry {
    types: HashMap<String, Rc<TauraroType>>,
    /// MRO computer for class hierarchy operations
    pub mro_computer: MROComputer,
    /// Type creator for dynamic class creation
    pub type_creator: TypeCreator,
    /// Builtin type hierarchy manager
    pub type_hierarchy: TypeHierarchy,
}

impl fmt::Debug for TypeSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("TypeSlots")
            .field("number", &self.number.is_some())
            .field("sequence", &self.sequence.is_some())
            .field("mapping", &self.mapping.is_some())
            .field("iterator", &self.iterator.is_some())
            .field("async_", &self.async_.is_some())
            .field("buffer", &self.buffer.is_some())
            .field("init", &self.init.is_some())
            .field("repr", &self.repr.is_some())
            .field("str", &self.str.is_some())
            .field("hash", &self.hash.is_some())
            .field("call", &self.call.is_some())
            .field("bool_", &self.bool_.is_some())
            .field("len", &self.len.is_some())
            .field("eq", &self.eq.is_some())
            .field("ne", &self.ne.is_some())
            .field("lt", &self.lt.is_some())
            .field("le", &self.le.is_some())
            .field("gt", &self.gt.is_some())
            .field("ge", &self.ge.is_some())
            .field("getattr", &self.getattr.is_some())
            .field("setattr", &self.setattr.is_some())
            .field("delattr", &self.delattr.is_some())
            .field("get", &self.get.is_some())
            .field("set", &self.set.is_some())
            .field("delete", &self.delete.is_some())
            .finish()
    }
}

impl Default for TypeSlots {
    fn default() -> Self {
        Self {
            number: None,
            sequence: None,
            mapping: None,
            iterator: None,
            async_: None,
            buffer: None,
            init: None,
            repr: None,
            str: None,
            hash: None,
            call: None,
            bool_: None,
            len: None,
            eq: None,
            ne: None,
            lt: None,
            le: None,
            gt: None,
            ge: None,
            getattr: None,
            setattr: None,
            delattr: None,
            get: None,
            set: None,
            delete: None,
        }
    }
}

impl MetaClass {
    pub fn new(name: String, bases: Vec<String>, namespace: HashMap<String, Value>) -> Self {
        Self {
            name,
            bases,
            namespace,
            mro_cache: None,
            custom_mro_method: None,
        }
    }
}

impl MROComputer {
    pub fn new() -> Self {
        Self {
            cache: HashMap::new(),
            validation_cache: HashMap::new(),
        }
    }
    
    pub fn compute_optimized_c3_linearization(
        &mut self,
        class_name: &str,
        bases: &[String],
        _class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Vec<String>, String> {
        // Simple implementation for now
        let mut result = vec![class_name.to_string()];
        result.extend_from_slice(bases);
        if !result.contains(&"object".to_string()) {
            result.push("object".to_string());
        }
        Ok(result)
    }
}

impl TypeCreator {
    pub fn new() -> Self {
        Self {
            mro_computer: MROComputer::new(),
        }
    }
    
    pub fn create_type(
        &mut self,
        name: String,
        bases: Vec<String>,
        namespace: HashMap<String, Value>,
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Value, String> {
        let mro = self.mro_computer.compute_optimized_c3_linearization(&name, &bases, class_registry)?;
        
        Ok(Value::Object {
            class_name: name.clone(),
            fields: namespace,
            base_object: crate::base_object::BaseObject::new(name, bases),
            mro: MRO::from_linearization(mro),
        })
    }
}

impl TypeHierarchy {
    pub fn isinstance(value: &Value, expected_type: &str) -> bool {
        match value {
            Value::Object { class_name, mro, .. } => {
                if class_name == expected_type {
                    return true;
                }
                mro.get_linearization().iter().any(|class| class == expected_type)
            }
            _ => {
                value.type_name() == expected_type
            }
        }
    }
}

impl TypeRegistry {
    pub fn new() -> Self {
        Self {
            types: HashMap::new(),
            mro_computer: MROComputer::new(),
            type_creator: TypeCreator::new(),
            type_hierarchy: TypeHierarchy,
        }
    }
}

impl fmt::Debug for TypeRegistry {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("TypeRegistry")
            .field("types", &self.types.keys().collect::<Vec<_>>())
            .field("mro_computer", &"MROComputer")
            .field("type_creator", &"TypeCreator")
            .field("type_hierarchy", &"TypeHierarchy")
            .finish()
    }
}

impl TauraroObject {
    pub fn new(type_info: Rc<TauraroType>, value: Value) -> Self {
        Self {
            type_info,
            ref_count: RefCell::new(1),
            value,
        }
    }
}

impl TauraroType {
    pub fn new(name: String) -> Self {
        Self {
            name,
            basic_size: 0,
            flags: TypeFlags {
                is_builtin: false,
                supports_gc: false,
                is_base_type: false,
                supports_weakrefs: false,
            },
            slots: TypeSlots::default(),
            mro: vec!["object".to_string()],
            bases: vec![],
            methods: HashMap::new(),
        }
    }
}

/// Represents a dunder method that can be called on any object
pub type DunderMethod = fn(&Value, &[Value]) -> Result<Value, String>;

/// Get type-specific methods for built-in types
pub fn get_type_methods(value: &Value) -> Option<HashMap<String, DunderMethod>> {
    match value {
        Value::Int(_) => Some(get_int_methods()),
        Value::Float(_) => Some(get_float_methods()),
        Value::Str(_) => Some(get_string_methods()),
        Value::List(_) => Some(get_list_methods()),
        Value::Dict(_) => Some(get_dict_methods()),
        Value::Tuple(_) => Some(get_tuple_methods()),
        Value::Set(_) => Some(get_set_methods()),
        Value::Bytes(_) => Some(get_bytes_methods()),
        Value::ByteArray(_) => Some(get_bytearray_methods()),
        Value::Object { .. } => Some(get_basic_object_methods()),
        _ => None,
    }
}

fn get_int_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("__str__".to_string(), int_str as DunderMethod);
    methods.insert("__repr__".to_string(), int_repr as DunderMethod);
    methods
}

fn get_float_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("__str__".to_string(), float_str as DunderMethod);
    methods.insert("__repr__".to_string(), float_repr as DunderMethod);
    methods
}

fn get_string_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("__str__".to_string(), str_str as DunderMethod);
    methods.insert("__repr__".to_string(), str_repr as DunderMethod);
    methods.insert("__len__".to_string(), str_len as DunderMethod);
    methods
}

fn get_list_methods() -> HashMap<String, DunderMethod> {
    HashMap::new()
}

fn get_dict_methods() -> HashMap<String, DunderMethod> {
    HashMap::new()
}

fn get_tuple_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("__str__".to_string(), tuple_str as DunderMethod);
    methods.insert("__repr__".to_string(), tuple_repr as DunderMethod);
    methods.insert("__len__".to_string(), tuple_len as DunderMethod);
    methods
}

fn get_set_methods() -> HashMap<String, DunderMethod> {
    HashMap::new()
}

fn get_bytes_methods() -> HashMap<String, DunderMethod> {
    HashMap::new()
}

fn get_bytearray_methods() -> HashMap<String, DunderMethod> {
    HashMap::new()
}

fn get_bool_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("__str__".to_string(), bool_str as DunderMethod);
    methods.insert("__repr__".to_string(), bool_repr as DunderMethod);
    methods
}

fn get_basic_object_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("__str__".to_string(), basic_object_str as DunderMethod);
    methods.insert("__repr__".to_string(), basic_object_repr as DunderMethod);
    methods
}

fn basic_object_str(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    match obj {
        Value::Object { class_name, .. } => {
            Ok(Value::Str(format!("<{} object>", class_name)))
        }
        _ => Ok(Value::Str(format!("<{} object>", obj.type_name())))
    }
}

fn basic_object_repr(obj: &Value, args: &[Value]) -> Result<Value, String> {
    basic_object_str(obj, args)
}

pub fn resolve_dunder_method(value: &Value, method_name: &str) -> Option<DunderMethod> {
    // Get methods for the specific value type
    let methods = match value {
        Value::Object { .. } => get_basic_object_methods(),
        Value::List(_) => get_list_methods(), 
        Value::Dict(_) => get_dict_methods(),
        Value::Str(_) => get_string_methods(),
        Value::Int(_) => get_int_methods(),
        Value::Float(_) => get_float_methods(),
        Value::Bool(_) => get_bool_methods(),
        Value::Tuple(_) => get_tuple_methods(),
        Value::Set(_) => get_set_methods(),
        Value::Bytes(_) => get_bytes_methods(),
        Value::ByteArray(_) => get_bytearray_methods(),
        _ => HashMap::new(),
    };
    
    methods.get(method_name).copied()
}



// Method implementations for built-in types
fn str_str(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        Ok(Value::Str(s.clone()))
    } else {
        Err("Expected string".to_string())
    }
}

fn str_repr(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        Ok(Value::Str(format!("\"{}\"", s)))
    } else {
        Err("Expected string".to_string())
    }
}

fn str_len(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        Ok(Value::Int(s.len() as i64))
    } else {
        Err("Expected string".to_string())
    }
}

fn int_str(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Int(n) = obj {
        Ok(Value::Str(n.to_string()))
    } else {
        Err("Expected int".to_string())
    }
}

fn int_repr(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Int(n) = obj {
        Ok(Value::Str(n.to_string()))
    } else {
        Err("Expected int".to_string())
    }
}

fn float_str(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Float(f) = obj {
        Ok(Value::Str(f.to_string()))
    } else {
        Err("Expected float".to_string())
    }
}

fn float_repr(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Float(f) = obj {
        Ok(Value::Str(f.to_string()))
    } else {
        Err("Expected float".to_string())
    }
}

fn bool_str(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Bool(b) = obj {
        Ok(Value::Str(if *b { "True".to_string() } else { "False".to_string() }))
    } else {
        Err("Expected bool".to_string())
    }
}

fn bool_repr(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Bool(b) = obj {
        Ok(Value::Str(if *b { "True".to_string() } else { "False".to_string() }))
    } else {
        Err("Expected bool".to_string())
    }
}

fn tuple_str(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Tuple(items) = obj {
        let items_str: Vec<String> = items.iter().map(|v| format!("{}", v)).collect();
        Ok(Value::Str(format!("({})", items_str.join(", "))))
    } else {
        Err("Expected tuple".to_string())
    }
}

fn tuple_repr(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    tuple_str(obj, _args)
}

fn tuple_len(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Tuple(items) = obj {
        Ok(Value::Int(items.len() as i64))
    } else {
        Err("Expected tuple".to_string())
    }
}

pub fn call_dunder_method(value: &Value, method_name: &str, args: Vec<Value>) -> Option<Value> {
    if let Some(method) = resolve_dunder_method(value, method_name) {
        method(value, &args).ok()
    } else {
        None
    }
}

pub fn call_dunder_method_with_vm(_vm: &mut crate::vm::VM, value: &Value, method_name: &str, args: Vec<Value>) -> Option<Value> {
    call_dunder_method(value, method_name, args)
}