use crate::base_object::{BaseObject as OriginalBaseObject, MRO};
use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
// use std::collections::HashSet;
use std::rc::Rc;
use std::cell::RefCell;
use std::fmt;
// Import HPList
use crate::modules::hplist::HPList;

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
    pub methods: HashMap<String, fn(&[Value]) -> Result<Value, anyhow::Error>>,
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
    pub init: Option<Rc<dyn Fn(&[Value]) -> Result<Value, anyhow::Error>>>,
    pub repr: Option<Rc<dyn Fn(&Value) -> Result<String, anyhow::Error>>>,
    pub str: Option<Rc<dyn Fn(&Value) -> Result<String, anyhow::Error>>>,
    pub hash: Option<Rc<dyn Fn(&Value) -> Result<i64, anyhow::Error>>>,
    pub call: Option<Rc<dyn Fn(&Value, &[Value]) -> Result<Value, anyhow::Error>>>,
    pub bool_: Option<Rc<dyn Fn(&Value) -> Result<bool, anyhow::Error>>>,
    pub len: Option<Rc<dyn Fn(&Value) -> Result<usize, anyhow::Error>>>,
    
    /// Rich comparison slots
    pub eq: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub ne: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub lt: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub le: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub gt: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub ge: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    
    /// Attribute access slots
    pub getattr: Option<Rc<dyn Fn(&Value, &str) -> Result<Value, anyhow::Error>>>,
    pub setattr: Option<Rc<dyn Fn(&Value, &str, &Value) -> Result<(), anyhow::Error>>>,
    pub delattr: Option<Rc<dyn Fn(&Value, &str) -> Result<(), anyhow::Error>>>,
    
    /// Descriptor protocol slots
    pub get: Option<Rc<dyn Fn(&Value, &Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub set: Option<Rc<dyn Fn(&Value, &Value, &Value) -> Result<(), anyhow::Error>>>,
    pub delete: Option<Rc<dyn Fn(&Value, &Value) -> Result<(), anyhow::Error>>>,
}

/// Number protocol slots (like PyNumberMethods)
#[derive(Clone)]
pub struct NumberSlots {
    pub add: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub subtract: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub multiply: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub divide: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub remainder: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub power: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub negative: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub positive: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub absolute: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub invert: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub lshift: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub rshift: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub and: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub xor: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub or: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
}

/// Sequence protocol slots (like PySequenceMethods)
#[derive(Clone)]
pub struct SequenceSlots {
    pub length: Option<Rc<dyn Fn(&Value) -> Result<usize, anyhow::Error>>>,
    pub item: Option<Rc<dyn Fn(&Value, isize) -> Result<Value, anyhow::Error>>>,
    pub set_item: Option<Rc<dyn Fn(&Value, isize, &Value) -> Result<(), anyhow::Error>>>,
    pub contains: Option<Rc<dyn Fn(&Value, &Value) -> Result<bool, anyhow::Error>>>,
    pub concat: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub repeat: Option<Rc<dyn Fn(&Value, isize) -> Result<Value, anyhow::Error>>>,
}

/// Mapping protocol slots (like PyMappingMethods)
#[derive(Clone)]
pub struct MappingSlots {
    pub length: Option<Rc<dyn Fn(&Value) -> Result<usize, anyhow::Error>>>,
    pub subscript: Option<Rc<dyn Fn(&Value, &Value) -> Result<Value, anyhow::Error>>>,
    pub set_subscript: Option<Rc<dyn Fn(&Value, &Value, &Value) -> Result<(), anyhow::Error>>>,
}

/// Iterator protocol slots
#[derive(Clone)]
pub struct IteratorSlots {
    pub iter: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub next: Option<Rc<dyn Fn(&Value) -> Result<Option<Value>, anyhow::Error>>>,
}

/// Async protocol slots
#[derive(Clone)]
pub struct AsyncSlots {
    pub await_: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub aiter: Option<Rc<dyn Fn(&Value) -> Result<Value, anyhow::Error>>>,
    pub anext: Option<Rc<dyn Fn(&Value) -> Result<Option<Value>, anyhow::Error>>>,
}

/// Buffer protocol slots
#[derive(Clone)]
pub struct BufferSlots {
    pub get_buffer: Option<Rc<dyn Fn(&Value) -> Result<Vec<u8>, anyhow::Error>>>,
    pub release_buffer: Option<Rc<dyn Fn(&Value) -> Result<(), anyhow::Error>>>,
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
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Vec<String>, String> {
        // Use the proper C3 linearization implementation from base_object
        let mut class_mros = HashMap::new();
        
        // Populate class_mros with existing MROs from registry
        for (class, base_classes) in class_registry {
            // For each base class, we need its MRO
            // For simplicity, we'll assume base classes have been processed already
            let base_mro = if class == "object" {
                vec!["object".to_string()]
            } else {
                let mut mro = vec![class.clone()];
                mro.extend_from_slice(base_classes);
                if !mro.contains(&"object".to_string()) && class != "object" {
                    mro.push("object".to_string());
                }
                mro
            };
            class_mros.insert(class.clone(), base_mro);
        }
        
        // Add the base classes themselves with their MROs
        for base in bases {
            if !class_mros.contains_key(base) {
                // If base class MRO not found, create appropriate fallback
                let fallback_mro = if base == "object" {
                    vec!["object".to_string()]
                } else {
                    // For other classes, assume they inherit from object
                    vec![base.clone(), "object".to_string()]
                };
                class_mros.insert(base.clone(), fallback_mro);
            }
        }
        
        // Compute the proper C3 linearization
        crate::base_object::MRO::compute_c3_linearization(class_name, bases, &class_mros)
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
        // Make sure all base classes exist in the registry or are "object"
        let mro = self.mro_computer.compute_optimized_c3_linearization(&name, &bases, class_registry)?;
        
        Ok(Value::Object {
            class_name: name.clone(),
            fields: namespace,
            class_methods: HashMap::new(),
            base_object: crate::base_object::BaseObject::new(name.clone(), bases.clone()),
            mro: crate::base_object::MRO::from_linearization(mro.clone()),
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
    methods.insert("to_bytes".to_string(), int_to_bytes as DunderMethod);
    methods.insert("bit_length".to_string(), int_bit_length as DunderMethod);
    methods.insert("from_bytes".to_string(), int_from_bytes as DunderMethod);
    methods
}

fn int_from_bytes(_obj: &Value, args: &[Value]) -> Result<Value, String> {
    if args.len() < 2 {
        return Err("from_bytes() takes at least 2 arguments".to_string());
    }
    
    let bytes = match &args[0] {
        Value::Bytes(b) => b.clone(),
        _ => return Err("first argument must be bytes".to_string()),
    };
    
    let byteorder = match &args[1] {
        Value::Str(s) => s.clone(),
        _ => return Err("byteorder must be a string".to_string()),
    };
    
    // Handle signed parameter (optional)
    let signed = if args.len() > 2 {
        match &args[2] {
            Value::Bool(b) => *b,
            _ => false,
        }
    } else {
        false
    };
    
    // Convert bytes to integer
    let result = if byteorder == "big" {
        if signed && !bytes.is_empty() && bytes[0] & 0x80 != 0 {
            // Negative signed integer (two's complement)
            let mut array = [0u8; 8];
            let start = 8usize.saturating_sub(bytes.len());
            array[start..].copy_from_slice(&bytes);
            i64::from_be_bytes(array)
        } else {
            // Positive integer
            let mut array = [0u8; 8];
            let start = 8usize.saturating_sub(bytes.len());
            array[start..].copy_from_slice(&bytes);
            i64::from_be_bytes(array)
        }
    } else if byteorder == "little" {
        if signed && !bytes.is_empty() && bytes.last().unwrap() & 0x80 != 0 {
            // Negative signed integer (two's complement)
            let mut array = [0u8; 8];
            let copy_len = std::cmp::min(bytes.len(), 8);
            array[..copy_len].copy_from_slice(&bytes[..copy_len]);
            i64::from_le_bytes(array)
        } else {
            // Positive integer
            let mut array = [0u8; 8];
            let copy_len = std::cmp::min(bytes.len(), 8);
            array[..copy_len].copy_from_slice(&bytes[..copy_len]);
            i64::from_le_bytes(array)
        }
    } else {
        return Err("byteorder must be 'big' or 'little'".to_string());
    };
    
    Ok(Value::Int(result))
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
    methods.insert("upper".to_string(), str_upper as DunderMethod);
    methods.insert("lower".to_string(), str_lower as DunderMethod);
    methods.insert("strip".to_string(), str_strip as DunderMethod);
    methods.insert("split".to_string(), str_split as DunderMethod);
    methods.insert("replace".to_string(), str_replace as DunderMethod);
    methods
}

fn get_list_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    methods.insert("append".to_string(), list_append as DunderMethod);
    methods.insert("extend".to_string(), list_extend as DunderMethod);
    methods.insert("pop".to_string(), list_pop as DunderMethod);
    methods.insert("insert".to_string(), list_insert as DunderMethod);
    methods.insert("remove".to_string(), list_remove as DunderMethod);
    methods.insert("index".to_string(), list_index as DunderMethod);
    methods.insert("count".to_string(), list_count as DunderMethod);
    methods.insert("sort".to_string(), list_sort as DunderMethod);
    methods.insert("reverse".to_string(), list_reverse as DunderMethod);
    methods.insert("__len__".to_string(), list_len as DunderMethod);
    methods
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

fn str_upper(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        Ok(Value::Str(s.to_uppercase()))
    } else {
        Err("Expected string".to_string())
    }
}

fn str_lower(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        Ok(Value::Str(s.to_lowercase()))
    } else {
        Err("Expected string".to_string())
    }
}

fn str_strip(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        let chars_to_strip = if args.is_empty() {
            None
        } else if let Value::Str(chars) = &args[0] {
            Some(chars.as_str())
        } else {
            return Err("strip() argument must be a string".to_string());
        };
        
        let stripped = if let Some(chars) = chars_to_strip {
            s.trim_matches(|c| chars.contains(c))
        } else {
            s.trim()
        };
        
        Ok(Value::Str(stripped.to_string()))
    } else {
        Err("Expected string".to_string())
    }
}

fn str_split(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        let separator = if args.is_empty() {
            " "
        } else if let Value::Str(sep) = &args[0] {
            sep.as_str()
        } else {
            return Err("split() separator must be a string".to_string());
        };
        
        let maxsplit = if args.len() > 1 {
            if let Value::Int(n) = &args[1] {
                *n as usize
            } else {
                return Err("split() maxsplit must be an integer".to_string());
            }
        } else {
            usize::MAX
        };
        
        let parts: Vec<Value> = s.split(separator)
            .take(maxsplit)
            .map(|part| Value::Str(part.to_string()))
            .collect();
        
        Ok(Value::List(HPList::from_values(parts)))
    } else {
        Err("Expected string".to_string())
    }
}

fn str_replace(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::Str(s) = obj {
        if args.len() < 2 {
            return Err("replace() requires at least 2 arguments".to_string());
        }
        
        let old_str = if let Value::Str(old) = &args[0] {
            old.as_str()
        } else {
            return Err("replace() old string must be a string".to_string());
        };
        
        let new_str = if let Value::Str(new) = &args[1] {
            new.as_str()
        } else {
            return Err("replace() new string must be a string".to_string());
        };
        
        let count = if args.len() > 2 {
            if let Value::Int(n) = &args[2] {
                *n as usize
            } else {
                return Err("replace() count must be an integer".to_string());
            }
        } else {
            usize::MAX
        };
        
        let replaced = if count == usize::MAX {
            s.replace(old_str, new_str)
        } else {
            let mut result = String::new();
            let mut remaining = s.as_str();
            let mut replacements = 0;
            
            while replacements < count {
                if let Some(pos) = remaining.find(old_str) {
                    result.push_str(&remaining[..pos]);
                    result.push_str(new_str);
                    remaining = &remaining[pos + old_str.len()..];
                    replacements += 1;
                } else {
                    break;
                }
            }
            result.push_str(remaining);
            result
        };
        
        Ok(Value::Str(replaced))
    } else {
        Err("Expected string".to_string())
    }
}

fn list_append(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        if args.is_empty() {
            return Err("append() requires an argument".to_string());
        }
        // For list.append(), we create a new list with the appended item
        // This is not ideal as it doesn't modify the original list in-place
        // But it's the best we can do with immutable values
        let mut new_list = list.clone();
        new_list.push(args[0].clone());
        Ok(Value::List(new_list))
    } else {
        Err("Expected list".to_string())
    }
}

fn list_extend(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        if args.is_empty() {
            return Err("extend() requires an argument".to_string());
        }
        let mut new_list = list.clone();
        if let Value::List(other_list) = &args[0] {
            new_list.extend(other_list.iter().cloned());
            Ok(Value::List(new_list))
        } else {
            Err("extend() argument must be a list".to_string())
        }
    } else {
        Err("Expected list".to_string())
    }
}

fn list_pop(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        let mut new_list = list.clone();
        let index = if args.is_empty() {
            new_list.len() as isize - 1
        } else if let Value::Int(i) = &args[0] {
            *i as isize
        } else {
            return Err("pop() index must be an integer".to_string());
        };
        
        if index >= new_list.len() as isize || index < -(new_list.len() as isize) {
            return Err("pop index out of range".to_string());
        }
        
        let normalized_index = if index < 0 {
            new_list.len() as isize + index
        } else {
            index
        } as usize;
        
        let popped = new_list.pop_at(normalized_index as isize);
        match popped {
            Ok(value) => Ok(value),
            Err(_) => Err("pop index out of range".to_string())
        }
    } else {
        Err("Expected list".to_string())
    }
}

fn list_insert(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        if args.len() < 2 {
            return Err("insert() requires 2 arguments".to_string());
        }
        
        let index = if let Value::Int(i) = &args[0] {
            *i as isize
        } else {
            return Err("insert() index must be an integer".to_string());
        };
        
        let mut new_list = list.clone();
        match new_list.insert(index, args[1].clone()) {
            Ok(()) => Ok(Value::List(new_list)),
            Err(_) => Err("insert index out of range".to_string())
        }
    } else {
        Err("Expected list".to_string())
    }
}

fn list_remove(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        if args.is_empty() {
            return Err("remove() requires an argument".to_string());
        }
        
        let mut new_list = list.clone();
        let value_to_remove = &args[0];
        
        match new_list.remove(value_to_remove) {
            Ok(()) => Ok(Value::List(new_list)),
            Err(_) => Err("value not found in list".to_string())
        }
    } else {
        Err("Expected list".to_string())
    }
}

fn list_index(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        if args.is_empty() {
            return Err("index() requires an argument".to_string());
        }
        
        let value_to_find = &args[0];
        
        if let Some(pos) = list.iter().position(|x| x == value_to_find) {
            Ok(Value::Int(pos as i64))
        } else {
            Err("value not found in list".to_string())
        }
    } else {
        Err("Expected list".to_string())
    }
}

fn list_count(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        if args.is_empty() {
            return Err("count() requires an argument".to_string());
        }
        
        let value_to_count = &args[0];
        let count = list.iter().filter(|&x| x == value_to_count).count();
        Ok(Value::Int(count as i64))
    } else {
        Err("Expected list".to_string())
    }
}

fn list_sort(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        let mut new_list = list.clone();
        new_list.sort();
        Ok(Value::List(new_list))
    } else {
        Err("Expected list".to_string())
    }
}

fn list_reverse(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        let mut new_list = list.clone();
        new_list.reverse();
        Ok(Value::List(new_list))
    } else {
        Err("Expected list".to_string())
    }
}

fn list_len(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::List(list) = obj {
        Ok(Value::Int(list.len() as i64))
    } else {
        Err("Expected list".to_string())
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

fn int_to_bytes(obj: &Value, args: &[Value]) -> Result<Value, String> {
    if let Value::Int(n) = obj {
        if args.len() < 2 {
            return Err("to_bytes() takes at least 2 arguments".to_string());
        }
        
        let length = match &args[0] {
            Value::Int(i) => *i as usize,
            _ => return Err("length must be an integer".to_string()),
        };
        
        let byteorder = match &args[1] {
            Value::Str(s) => s.clone(),
            _ => return Err("byteorder must be a string".to_string()),
        };
        
        // Handle signed parameter (optional)
        let signed = if args.len() > 2 {
            match &args[2] {
                Value::Bool(b) => *b,
                _ => false,
            }
        } else {
            false
        };
        
        // Convert integer to bytes
        let bytes = if byteorder == "big" {
            if signed && *n < 0 {
                // For negative signed integers, we need to use two's complement
                let unsigned_val = *n as u64;
                unsigned_val.to_be_bytes().to_vec()
            } else if signed {
                // For positive signed integers
                let unsigned_val = *n as u64;
                unsigned_val.to_be_bytes().to_vec()
            } else {
                // For unsigned integers
                let unsigned_val = (*n as u64) & 0xFFFFFFFFFFFFFFFF;
                unsigned_val.to_be_bytes().to_vec()
            }
        } else if byteorder == "little" {
            if signed && *n < 0 {
                // For negative signed integers, we need to use two's complement
                let unsigned_val = *n as u64;
                unsigned_val.to_le_bytes().to_vec()
            } else if signed {
                // For positive signed integers
                let unsigned_val = *n as u64;
                unsigned_val.to_le_bytes().to_vec()
            } else {
                // For unsigned integers
                let unsigned_val = (*n as u64) & 0xFFFFFFFFFFFFFFFF;
                unsigned_val.to_le_bytes().to_vec()
            }
        } else {
            return Err("byteorder must be 'big' or 'little'".to_string());
        };

        
        // Truncate or pad to desired length
        let result_bytes = if bytes.len() > length {
            // Truncate
            bytes[bytes.len() - length..].to_vec()
        } else if bytes.len() < length {
            // Pad
            let mut padded = vec![0; length - bytes.len()];
            if byteorder == "big" {
                padded.extend(&bytes);
            } else {
                padded = bytes;
                padded.extend(vec![0; length - padded.len()]);
            }
            padded
        } else {
            bytes
        };
        
        Ok(Value::Bytes(result_bytes))
    } else {
        Err("Expected int".to_string())
    }
}

fn int_bit_length(obj: &Value, _args: &[Value]) -> Result<Value, String> {
    if let Value::Int(n) = obj {
        if *n == 0 {
            Ok(Value::Int(0))
        } else {
            let abs_n = (*n).abs() as u64;
            Ok(Value::Int(64 - abs_n.leading_zeros() as i64))
        }
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