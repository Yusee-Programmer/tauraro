use crate::value::Value;
use crate::base_object::{BaseObject as OriginalBaseObject, MRO};
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

/// Debug implementations for complex function pointer types
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

impl fmt::Debug for NumberSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("NumberSlots")
            .field("add", &self.add.is_some())
            .field("subtract", &self.subtract.is_some())
            .field("multiply", &self.multiply.is_some())
            .field("divide", &self.divide.is_some())
            .field("remainder", &self.remainder.is_some())
            .field("power", &self.power.is_some())
            .field("negative", &self.negative.is_some())
            .field("positive", &self.positive.is_some())
            .field("absolute", &self.absolute.is_some())
            .field("invert", &self.invert.is_some())
            .field("lshift", &self.lshift.is_some())
            .field("rshift", &self.rshift.is_some())
            .field("and", &self.and.is_some())
            .field("xor", &self.xor.is_some())
            .field("or", &self.or.is_some())
            .finish()
    }
}

impl fmt::Debug for SequenceSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("SequenceSlots")
            .field("length", &self.length.is_some())
            .field("item", &self.item.is_some())
            .field("set_item", &self.set_item.is_some())
            .field("contains", &self.contains.is_some())
            .field("concat", &self.concat.is_some())
            .field("repeat", &self.repeat.is_some())
            .finish()
    }
}

impl fmt::Debug for MappingSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("MappingSlots")
            .field("length", &self.length.is_some())
            .field("subscript", &self.subscript.is_some())
            .field("set_subscript", &self.set_subscript.is_some())
            .finish()
    }
}

impl fmt::Debug for IteratorSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("IteratorSlots")
            .field("iter", &self.iter.is_some())
            .field("next", &self.next.is_some())
            .finish()
    }
}

impl fmt::Debug for AsyncSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("AsyncSlots")
            .field("await_", &self.await_.is_some())
            .field("aiter", &self.aiter.is_some())
            .field("anext", &self.anext.is_some())
            .finish()
    }
}

impl fmt::Debug for BufferSlots {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("BufferSlots")
            .field("get_buffer", &self.get_buffer.is_some())
            .field("release_buffer", &self.release_buffer.is_some())
            .finish()
    }
}

impl fmt::Debug for TypeRegistry {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("TypeRegistry")
            .field("types", &self.types.keys().collect::<Vec<_>>())
            .field("mro_computer", &self.mro_computer)
            .field("type_creator", &self.type_creator)
            .field("type_hierarchy", &self.type_hierarchy)
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

// Implementation of MetaClass
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

    pub fn set_custom_mro_method(&mut self, method_name: String) {
        self.custom_mro_method = Some(method_name);
        self.mro_cache = None;
    }

    pub fn has_custom_mro(&self) -> bool {
        self.custom_mro_method.is_some()
    }

    pub fn invoke_custom_mro(
        &self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Option<Vec<String>>, String> {
        if let Some(method_name) = &self.custom_mro_method {
            match method_name.as_str() {
                "__mro__" => Ok(None),
                "reverse_mro" => {
                    let mut reversed_bases = bases.to_vec();
                    reversed_bases.reverse();
                    let mut result = vec![class_name.to_string()];
                    result.extend(reversed_bases);
                    result.push("object".to_string());
                    Ok(Some(result))
                }
                "depth_first_mro" => {
                    let mut result = vec![class_name.to_string()];
                    for base in bases {
                        if let Some(base_mro) = class_registry.get(base) {
                            for class in base_mro {
                                if !result.contains(class) {
                                    result.push(class.clone());
                                }
                            }
                        } else if !result.contains(base) {
                            result.push(base.clone());
                        }
                    }
                    if !result.contains(&"object".to_string()) {
                        result.push("object".to_string());
                    }
                    Ok(Some(result))
                }
                _ => Err(format!("Unknown custom MRO method: {}", method_name))
            }
        } else {
            Ok(None)
        }
    }

    pub fn get_cached_mro(&self) -> Option<&Vec<String>> {
        self.mro_cache.as_ref()
    }

    pub fn cache_mro(&mut self, mro: Vec<String>) {
        self.mro_cache = Some(mro);
    }

    pub fn invalidate_mro_cache(&mut self) {
        self.mro_cache = None;
    }
}

// Implementation of MROComputer
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
        let cache_key = format!("{}:{}", class_name, bases.join(","));
        
        if let Some(cached_result) = self.cache.get(&cache_key) {
            return Ok(cached_result.clone());
        }

        self.validate_hierarchy(class_name, bases, class_registry)?;
        let result = self.compute_c3_with_optimizations(class_name, bases, class_registry)?;
        self.cache.insert(cache_key, result.clone());
        
        Ok(result)
    }

    fn compute_c3_with_optimizations(
        &mut self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Vec<String>, String> {
        if bases.is_empty() {
            return Ok(vec![class_name.to_string(), "object".to_string()]);
        }

        let mut base_linearizations = Vec::with_capacity(bases.len() + 1);
        
        for base in bases {
            let base_mro = if base == "object" {
                vec!["object".to_string()]
            } else if let Some(base_bases) = class_registry.get(base) {
                self.compute_optimized_c3_linearization(base, base_bases, class_registry)?
            } else {
                vec![base.clone(), "object".to_string()]
            };
            base_linearizations.push(base_mro);
        }

        base_linearizations.push(bases.to_vec());
        let merged = self.optimized_c3_merge(base_linearizations)?;
        
        let mut result = Vec::with_capacity(merged.len() + 1);
        result.push(class_name.to_string());
        result.extend(merged);
        
        Ok(result)
    }

    fn optimized_c3_merge(&self, mut sequences: Vec<Vec<String>>) -> Result<Vec<String>, String> {
        let mut result = Vec::new();
        
        while !sequences.is_empty() {
            sequences.retain(|seq| !seq.is_empty());
            
            if sequences.is_empty() {
                break;
            }

            let mut candidate_found = false;
            let mut selected_candidate: Option<String> = None;
            
            for seq in &sequences {
                if seq.is_empty() {
                    continue;
                }
                
                let candidate = &seq[0];
                let appears_in_tail = sequences.iter()
                    .any(|other_seq| other_seq.len() > 1 && other_seq[1..].contains(candidate));
                
                if !appears_in_tail {
                    result.push(candidate.clone());
                    selected_candidate = Some(candidate.clone());
                    candidate_found = true;
                    break;
                }
            }
            
            if let Some(candidate) = selected_candidate {
                for seq in &mut sequences {
                    if !seq.is_empty() && seq[0] == candidate {
                        seq.remove(0);
                    }
                }
            }
            
            if !candidate_found {
                return Err(format!(
                    "Cannot create consistent MRO - circular dependency detected in sequences: {:?}",
                    sequences
                ));
            }
        }
        
        Ok(result)
    }

    fn validate_hierarchy(
        &mut self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<(), String> {
        let validation_key = format!("{}:{}", class_name, bases.join(","));
        
        if let Some(cached_result) = self.validation_cache.get(&validation_key) {
            return cached_result.clone();
        }

        let mut validation_result = Ok(());

        if bases.contains(&class_name.to_string()) {
            validation_result = Err(format!("Class '{}' cannot inherit from itself", class_name));
        }

        if validation_result.is_ok() {
            let mut seen_bases = HashSet::new();
            for base in bases {
                if !seen_bases.insert(base) {
                    validation_result = Err(format!("Duplicate base class '{}' in inheritance list", base));
                    break;
                }
            }
        }

        if validation_result.is_ok() {
            validation_result = self.detect_circular_inheritance(class_name, bases, class_registry);
        }

        if validation_result.is_ok() {
            for base in bases {
                if base != "object" && !class_registry.contains_key(base) {
                    validation_result = Err(format!("Base class '{}' not found in registry", base));
                    break;
                }
            }
        }

        self.validation_cache.insert(validation_key, validation_result.clone());
        validation_result
    }

    fn detect_circular_inheritance(
        &self,
        class_name: &str,
        bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<(), String> {
        let mut visited = HashSet::new();
        let mut path = Vec::new();
        
        self.dfs_circular_check(class_name, class_name, bases, class_registry, &mut visited, &mut path)
    }

    fn dfs_circular_check(
        &self,
        current_class: &str,
        original_class: &str,
        original_bases: &[String],
        class_registry: &HashMap<String, Vec<String>>,
        visited: &mut HashSet<String>,
        path: &mut Vec<String>,
    ) -> Result<(), String> {
        if path.contains(&current_class.to_string()) {
            return Err(format!(
                "Circular inheritance detected: {} -> {}",
                path.join(" -> "),
                current_class
            ));
        }

        if visited.contains(current_class) {
            return Ok(());
        }

        visited.insert(current_class.to_string());
        path.push(current_class.to_string());

        let current_bases = if current_class == original_class {
            original_bases.to_vec()
        } else {
            if let Some(mro) = class_registry.get(current_class) {
                let bases_from_mro: Vec<String> = mro.iter()
                    .skip(1)
                    .take_while(|&class| class != "object")
                    .cloned()
                    .collect();
                bases_from_mro
            } else {
                vec![]
            }
        };

        for base in &current_bases {
            if base != "object" {
                self.dfs_circular_check(base, original_class, original_bases, class_registry, visited, path)?;
            }
        }

        path.pop();
        Ok(())
    }

    pub fn invalidate_cache(&mut self) {
        self.cache.clear();
        self.validation_cache.clear();
    }

    pub fn get_cache_stats(&self) -> (usize, usize) {
        (self.cache.len(), self.validation_cache.len())
    }
}

// Implementation of TypeCreator
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
        metaclass: Option<MetaClass>,
        existing_class_registry: &HashMap<String, Vec<String>>,
    ) -> Result<Value> {
        let mut class_registry = existing_class_registry.clone();
        
        if !class_registry.contains_key("object") {
            class_registry.insert("object".to_string(), vec!["object".to_string()]);
        }
        
        let mro_linearization = if let Some(ref meta) = metaclass {
            if meta.has_custom_mro() {
                match meta.invoke_custom_mro(&name, &bases, &class_registry) {
                    Ok(Some(custom_mro)) => custom_mro,
                    Ok(None) => {
                        self.mro_computer.compute_optimized_c3_linearization(
                            &name,
                            &bases,
                            &class_registry,
                        ).map_err(|e| anyhow!(e))?
                    }
                    Err(e) => {
                        return Err(anyhow!("Custom MRO method failed: {}", e));
                    }
                }
            } else {
                self.mro_computer.compute_optimized_c3_linearization(
                    &name,
                    &bases,
                    &class_registry,
                ).map_err(|e| anyhow!(e))?
            }
        } else {
            self.mro_computer.compute_optimized_c3_linearization(
                &name,
                &bases,
                &class_registry,
            ).map_err(|e| anyhow!(e))?
        };
        
        if let Some(mut meta) = metaclass {
            meta.cache_mro(mro_linearization.clone());
        }
        
        let class_value = Value::Object {
            class_name: name.clone(),
            fields: namespace,
            base_object: OriginalBaseObject::new(name.clone(), bases),
            mro: MRO::from_linearization(mro_linearization),
        };

        Ok(class_value)
    }

    pub fn mro_computer(&mut self) -> &mut MROComputer {
        &mut self.mro_computer
    }

    pub fn invalidate_caches(&mut self) {
        self.mro_computer.invalidate_cache();
    }

    pub fn get_performance_stats(&self) -> (usize, usize) {
        self.mro_computer.get_cache_stats()
    }
}

// Implementation of TypeHierarchy
impl TypeHierarchy {
    pub fn get_builtin_mro(type_name: &str) -> MRO {
        let class_mros = HashMap::new();
        match type_name {
            "int" => {
                let linearization = MRO::compute_c3_linearization("int", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["int".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "float" => {
                let linearization = MRO::compute_c3_linearization("float", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["float".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "str" => {
                let linearization = MRO::compute_c3_linearization("str", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["str".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "bool" => {
                let linearization = MRO::compute_c3_linearization("bool", &[String::from("int"), String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["bool".to_string(), "int".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "list" | "dict" | "tuple" | "set" | "bytes" | "bytearray" | "function" | "module" | "NoneType" => {
                let linearization = MRO::compute_c3_linearization(type_name, &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec![type_name.to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "object" => MRO::from_linearization(vec!["object".to_string()]),
            _ => MRO::from_linearization(vec!["object".to_string()]),
        }
    }

    pub fn get_builtin_base_object(type_name: &str) -> OriginalBaseObject {
        let parents = match type_name {
            "bool" => vec!["int".to_string(), "object".to_string()],
            _ => vec!["object".to_string()],
        };
        OriginalBaseObject::new(type_name.to_string(), parents)
    }

    pub fn is_subtype(subtype: &str, supertype: &str) -> bool {
        if subtype == supertype {
            return true;
        }
        
        let mro = Self::get_builtin_mro(subtype);
        mro.linearization.iter().any(|class| class == supertype)
    }

    pub fn get_parent_types(type_name: &str) -> Vec<String> {
        let mro = Self::get_builtin_mro(type_name);
        mro.linearization[1..].to_vec()
    }

    pub fn isinstance(value: &Value, expected_type: &str) -> bool {
        let value_type = value.type_name();
        Self::is_subtype(&value_type, expected_type)
    }

    pub fn get_type_info(value: &Value) -> String {
        let type_name = value.type_name();
        let mro = Self::get_builtin_mro(&type_name);
        format!("{} (MRO: {})", type_name, mro.linearization.join(" -> "))
    }
}

// Implementation of TypeRegistry
impl TypeRegistry {
    pub fn new() -> Self {
        let mut registry = Self {
            types: HashMap::new(),
            mro_computer: MROComputer::new(),
            type_creator: TypeCreator::new(),
            type_hierarchy: TypeHierarchy,
        };
        registry.init_builtin_types();
        registry
    }
    
    fn init_builtin_types(&mut self) {
        let int_type = self.create_int_type();
        self.types.insert("int".to_string(), Rc::new(int_type));
        
        let float_type = self.create_float_type();
        self.types.insert("float".to_string(), Rc::new(float_type));
        
        let str_type = self.create_str_type();
        self.types.insert("str".to_string(), Rc::new(str_type));
        
        let bool_type = self.create_bool_type();
        self.types.insert("bool".to_string(), Rc::new(bool_type));
        
        let list_type = self.create_list_type();
        self.types.insert("list".to_string(), Rc::new(list_type));
        
        let dict_type = self.create_dict_type();
        self.types.insert("dict".to_string(), Rc::new(dict_type));
    }
    
    fn create_int_type(&self) -> TauraroType {
        let mut int_type = TauraroType::builtin("int".to_string());
        
        int_type.slots.number = Some(NumberSlots {
            add: Some(Rc::new(|left, right| {
                match (left, right) {
                    (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
                    (Value::Int(a), Value::Float(b)) => Ok(Value::Float(*a as f64 + b)),
                    _ => Err(anyhow::anyhow!("Invalid types for int addition")),
                }
            })),
            subtract: Some(Rc::new(|left, right| {
                match (left, right) {
                    (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
                    (Value::Int(a), Value::Float(b)) => Ok(Value::Float(*a as f64 - b)),
                    _ => Err(anyhow::anyhow!("Invalid types for int subtraction")),
                }
            })),
            multiply: None,
            divide: None,
            remainder: None,
            power: None,
            negative: None,
            positive: None,
            absolute: None,
            invert: None,
            lshift: None,
            rshift: None,
            and: None,
            xor: None,
            or: None,
        });
        
        int_type
    }
    
    fn create_float_type(&self) -> TauraroType {
        let mut float_type = TauraroType::builtin("float".to_string());
        
        float_type.slots.number = Some(NumberSlots {
            add: Some(Rc::new(|left, right| {
                match (left, right) {
                    (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
                    (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + *b as f64)),
                    _ => Err(anyhow::anyhow!("Invalid types for float addition")),
                }
            })),
            subtract: None,
            multiply: None,
            divide: None,
            remainder: None,
            power: None,
            negative: None,
            positive: None,
            absolute: None,
            invert: None,
            lshift: None,
            rshift: None,
            and: None,
            xor: None,
            or: None,
        });
        
        float_type
    }
    
    fn create_str_type(&self) -> TauraroType {
        let mut str_type = TauraroType::builtin("str".to_string());
        
        str_type.slots.sequence = Some(SequenceSlots {
            length: Some(Rc::new(|value| {
                match value {
                    Value::Str(s) => Ok(s.len()),
                    _ => Err(anyhow::anyhow!("Invalid type for str length")),
                }
            })),
            item: None,
            set_item: None,
            contains: None,
            concat: None,
            repeat: None,
        });
        
        str_type
    }
    
    fn create_bool_type(&self) -> TauraroType {
        TauraroType::builtin("bool".to_string())
    }
    
    fn create_list_type(&self) -> TauraroType {
        let mut list_type = TauraroType::builtin("list".to_string());
        
        list_type.slots.sequence = Some(SequenceSlots {
            length: Some(Rc::new(|value| {
                match value {
                    Value::List(items) => Ok(items.len()),
                    _ => Err(anyhow::anyhow!("Invalid type for list length")),
                }
            })),
            item: None,
            set_item: None,
            contains: None,
            concat: None,
            repeat: None,
        });
        
        list_type
    }
    
    fn create_dict_type(&self) -> TauraroType {
        let mut dict_type = TauraroType::builtin("dict".to_string());
        
        dict_type.slots.mapping = Some(MappingSlots {
            length: Some(Rc::new(|value| {
                match value {
                    Value::Dict(map) => Ok(map.len()),
                    _ => Err(anyhow::anyhow!("Invalid type for dict length")),
                }
            })),
            subscript: None,
            set_subscript: None,
        });
        
        dict_type
    }
    
    pub fn get_type(&self, name: &str) -> Option<Rc<TauraroType>> {
        self.types.get(name).cloned()
    }
    
    pub fn register_type(&mut self, name: String, type_obj: TauraroType) {
        self.types.insert(name, Rc::new(type_obj));
    }
}

// Implementation of TauraroObject
impl TauraroObject {
    pub fn new(type_info: Rc<TauraroType>, value: Value) -> Self {
        Self {
            type_info,
            ref_count: RefCell::new(1),
            value,
        }
    }
    
    pub fn incref(&self) {
        *self.ref_count.borrow_mut() += 1;
    }
    
    pub fn decref(&self) -> bool {
        let mut count = self.ref_count.borrow_mut();
        *count -= 1;
        *count == 0
    }
    
    pub fn ref_count(&self) -> usize {
        *self.ref_count.borrow()
    }
    
    pub fn call_method(&self, method_name: &str, args: &[Value]) -> Result<Value> {
        if let Some(method) = self.type_info.methods.get(method_name) {
            return method(args);
        }
        
        Err(anyhow::anyhow!("'{}' object has no attribute '{}'", 
                           self.type_info.name, method_name))
    }
}

// Implementation of TauraroType
impl TauraroType {
    pub fn new(name: String) -> Self {
        Self {
            name,
            basic_size: std::mem::size_of::<Value>(),
            flags: TypeFlags {
                is_builtin: false,
                supports_gc: true,
                is_base_type: false,
                supports_weakrefs: false,
            },
            slots: TypeSlots::default(),
            mro: vec!["object".to_string()],
            bases: vec!["object".to_string()],
            methods: HashMap::new(),
        }
    }
    
    pub fn builtin(name: String) -> Self {
        let mut type_obj = Self::new(name);
        type_obj.flags.is_builtin = true;
        type_obj
    }
}



                }
            })),
            // TODO: Implement other sequence operations
            item: None,
            set_item: None,
            contains: None,
            concat: None,
            repeat: None,
        });
        
        str_type
    }
    
    fn create_bool_type(&self) -> TauraroType {
        TauraroType::builtin("bool".to_string())
    }
    
    fn create_list_type(&self) -> TauraroType {
        let mut list_type = TauraroType::builtin("list".to_string());
        
        // Set up sequence slots for list
        list_type.slots.sequence = Some(SequenceSlots {
            length: Some(Rc::new(|value| {
                match value {
                    Value::List(items) => Ok(items.len()),
                    _ => Err(anyhow::anyhow!("Invalid type for list length")),
                }
            })),
            // TODO: Implement other sequence operations
            item: None,
            set_item: None,
            contains: None,
            concat: None,
            repeat: None,
        });
        
        list_type
    }
    
    fn create_dict_type(&self) -> TauraroType {
        let mut dict_type = TauraroType::builtin("dict".to_string());
        
        // Set up mapping slots for dict
        dict_type.slots.mapping = Some(MappingSlots {
            length: Some(Rc::new(|value| {
                match value {
                    Value::Dict(map) => Ok(map.len()),
                    _ => Err(anyhow::anyhow!("Invalid type for dict length")),
                }
            })),
            // TODO: Implement other mapping operations
            subscript: None,
            set_subscript: None,
        });
        
        dict_type
    }
    
    pub fn get_type(&self, name: &str) -> Option<Rc<TauraroType>> {
        self.types.get(name).cloned()
    }
    
    pub fn register_type(&mut self, name: String, type_obj: TauraroType) {
        self.types.insert(name, Rc::new(type_obj));
    }
}

impl TauraroObject {
    /// Create a new object with reference count of 1 (like Py_INCREF)
    pub fn new(type_info: Rc<TauraroType>, value: Value) -> Self {
        Self {
            type_info,
            ref_count: RefCell::new(1),
            value,
        }
    }
    
    /// Increment reference count (like Py_INCREF)
    pub fn incref(&self) {
        *self.ref_count.borrow_mut() += 1;
    }
    
    /// Decrement reference count (like Py_DECREF)
    pub fn decref(&self) -> bool {
        let mut count = self.ref_count.borrow_mut();
        *count -= 1;
        *count == 0
    }
    
    /// Get current reference count
    pub fn ref_count(&self) -> usize {
        *self.ref_count.borrow()
    }
    
    /// Call a method on this object using the type's method resolution
    pub fn call_method(&self, method_name: &str, args: &[Value]) -> Result<Value> {
        // First check if the method exists in the type
        if let Some(method) = self.type_info.methods.get(method_name) {
            return method(args);
        }
        
        // Then check the method resolution order
        for base_type_name in &self.type_info.mro {
            // This would need integration with the global type registry
            // For now, we'll implement a simpler version
        }
        
        Err(anyhow::anyhow!("'{}' object has no attribute '{}'", 
                           self.type_info.name, method_name))
    }
}

impl TauraroType {
    /// Create a new type with default slots
    pub fn new(name: String) -> Self {
        Self {
            name,
            basic_size: std::mem::size_of::<Value>(),
            flags: TypeFlags {
                is_builtin: false,
                supports_gc: true,
                is_base_type: false,
                supports_weakrefs: false,
            },
            slots: TypeSlots::default(),
            mro: vec!["object".to_string()],
            bases: vec!["object".to_string()],
            methods: HashMap::new(),
        }
    }
    
    /// Create a builtin type with specific slots
    pub fn builtin(name: String) -> Self {
        let mut type_obj = Self::new(name);
        type_obj.flags.is_builtin = true;
        type_obj
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

/// Represents a dunder method that can be called on any object
pub type DunderMethod = fn(&Value, Vec<Value>) -> Option<Value>;

/// Base object methods that all types inherit from
pub struct BaseObject;

impl BaseObject {
    /// Get all base dunder methods that every object should have
    pub fn get_base_methods() -> HashMap<String, DunderMethod> {
        let mut methods = HashMap::new();
        
        // String representation methods
        methods.insert("__str__".to_string(), Self::dunder_str as DunderMethod);
        methods.insert("__repr__".to_string(), Self::dunder_repr as DunderMethod);
        methods.insert("__format__".to_string(), Self::dunder_format as DunderMethod);
        
        // Comparison methods
        methods.insert("__eq__".to_string(), Self::dunder_eq as DunderMethod);
        methods.insert("__ne__".to_string(), Self::dunder_ne as DunderMethod);
        methods.insert("__lt__".to_string(), Self::dunder_lt as DunderMethod);
        methods.insert("__le__".to_string(), Self::dunder_le as DunderMethod);
        methods.insert("__gt__".to_string(), Self::dunder_gt as DunderMethod);
        methods.insert("__ge__".to_string(), Self::dunder_ge as DunderMethod);
        
        // Hash and identity
        methods.insert("__hash__".to_string(), Self::dunder_hash as DunderMethod);
        methods.insert("__bool__".to_string(), Self::dunder_bool as DunderMethod);
        
        // Attribute access
        methods.insert("__getattribute__".to_string(), Self::dunder_getattribute as DunderMethod);
        methods.insert("__setattr__".to_string(), Self::dunder_setattr as DunderMethod);
        methods.insert("__delattr__".to_string(), Self::dunder_delattr as DunderMethod);
        methods.insert("__dir__".to_string(), Self::dunder_dir as DunderMethod);
        
        // Type and class methods
        methods.insert("__class__".to_string(), Self::dunder_class as DunderMethod);
        methods.insert("__sizeof__".to_string(), Self::dunder_sizeof as DunderMethod);
        
        // Initialization and destruction
        methods.insert("__new__".to_string(), Self::dunder_new as DunderMethod);
        methods.insert("__init__".to_string(), Self::dunder_init as DunderMethod);
        methods.insert("__del__".to_string(), Self::dunder_del as DunderMethod);
        
        methods
    }
    
    // Base implementations of dunder methods
    
    /// Default string representation
    fn dunder_str(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        let repr = match obj {
            Value::Int(n) => n.to_string(),
            Value::Float(n) => format!("{}", n),
            Value::Bool(b) => if *b { "True".to_string() } else { "False".to_string() },
            Value::Str(s) => s.clone(),
            Value::None => "None".to_string(),
            Value::List(items) => {
                let items_str: Vec<String> = items.iter().map(|v| {
                    match Self::dunder_str(v, vec![]) {
                        Some(Value::Str(s)) => s,
                        _ => format!("{:?}", v),
                    }
                }).collect();
                format!("[{}]", items_str.join(", "))
            },
            Value::Tuple(items) => {
                let items_str: Vec<String> = items.iter().map(|v| {
                    match Self::dunder_str(v, vec![]) {
                        Some(Value::Str(s)) => s,
                        _ => format!("{:?}", v),
                    }
                }).collect();
                if items.len() == 1 {
                    format!("({},)", items_str[0])
                } else {
                    format!("({})", items_str.join(", "))
                }
            },
            Value::Dict(dict) => {
                let pairs: Vec<String> = dict.iter().map(|(k, v)| {
                    let v_str = match Self::dunder_str(v, vec![]) {
                        Some(Value::Str(s)) => s,
                        _ => format!("{:?}", v),
                    };
                    format!("'{}': {}", k, v_str)
                }).collect();
                format!("{{{}}}", pairs.join(", "))
            },
            Value::Set(items) => {
                let items_str: Vec<String> = items.iter().map(|v| {
                    match Self::dunder_str(v, vec![]) {
                        Some(Value::Str(s)) => s,
                        _ => format!("{:?}", v),
                    }
                }).collect();
                format!("{{{}}}", items_str.join(", "))
            },
            Value::Object { class_name, .. } => {
                format!("<{} object>", class_name)
            },
            _ => format!("<{} object>", obj.type_name()),
        };
        Some(Value::Str(repr))
    }
    
    /// Default repr representation (more detailed than str)
    fn dunder_repr(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        let repr = match obj {
            Value::Str(s) => format!("'{}'", s),
            Value::Bytes(bytes) => format!("b'{}'", String::from_utf8_lossy(bytes)),
            Value::ByteArray(bytes) => format!("bytearray(b'{}')", String::from_utf8_lossy(bytes)),
            _ => {
                // For most types, repr is the same as str
                match Self::dunder_str(obj, vec![]) {
                    Some(Value::Str(s)) => s,
                    _ => format!("{:?}", obj),
                }
            }
        };
        Some(Value::Str(repr))
    }
    
    /// Default format method
    fn dunder_format(obj: &Value, args: Vec<Value>) -> Option<Value> {
        let _format_spec = if args.is_empty() {
            String::new()
        } else {
            match &args[0] {
                Value::Str(s) => s.clone(),
                _ => return None, // Invalid format spec
            }
        };
        
        // For now, just return str representation
        // TODO: Implement proper format specification parsing
        Self::dunder_str(obj, vec![])
    }
    
    /// Default equality comparison
    fn dunder_eq(obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.len() != 1 {
            return None;
        }
        
        let other = &args[0];
        let result = match (obj, other) {
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Int(a), Value::Float(b)) => *a as f64 == *b,
            (Value::Float(a), Value::Int(b)) => *a == *b as f64,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::Str(a), Value::Str(b)) => a == b,
            (Value::None, Value::None) => true,
            (Value::List(a), Value::List(b)) => a == b,
            (Value::Tuple(a), Value::Tuple(b)) => a == b,
            (Value::Dict(a), Value::Dict(b)) => a == b,
            (Value::Set(a), Value::Set(b)) => a == b,
            _ => false, // Different types are not equal
        };
        Some(Value::Bool(result))
    }
    
    /// Default not-equal comparison
    fn dunder_ne(obj: &Value, args: Vec<Value>) -> Option<Value> {
        let eq_result = Self::dunder_eq(obj, args)?;
        match eq_result {
            Value::Bool(b) => Some(Value::Bool(!b)),
            _ => None,
        }
    }
    
    /// Default less-than comparison
    fn dunder_lt(obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.len() != 1 {
            return None;
        }
        
        let other = &args[0];
        let result = match (obj, other) {
            (Value::Int(a), Value::Int(b)) => a < b,
            (Value::Float(a), Value::Float(b)) => a < b,
            (Value::Int(a), Value::Float(b)) => (*a as f64) < *b,
            (Value::Float(a), Value::Int(b)) => *a < (*b as f64),
            (Value::Str(a), Value::Str(b)) => a < b,
            (Value::Bool(a), Value::Bool(b)) => a < b,
            _ => return None, // Unsupported comparison
        };
        Some(Value::Bool(result))
    }
    
    /// Default less-than-or-equal comparison
    fn dunder_le(obj: &Value, args: Vec<Value>) -> Option<Value> {
        let lt_result = Self::dunder_lt(obj, args.clone())?;
        let eq_result = Self::dunder_eq(obj, args)?;
        
        match (lt_result, eq_result) {
            (Value::Bool(lt), Value::Bool(eq)) => Some(Value::Bool(lt || eq)),
            _ => None,
        }
    }
    
    /// Default greater-than comparison
    fn dunder_gt(obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.len() != 1 {
            return None;
        }
        
        // a > b is equivalent to b < a
        let other = &args[0];
        Self::dunder_lt(other, vec![obj.clone()])
    }
    
    /// Default greater-than-or-equal comparison
    fn dunder_ge(obj: &Value, args: Vec<Value>) -> Option<Value> {
        let gt_result = Self::dunder_gt(obj, args.clone())?;
        let eq_result = Self::dunder_eq(obj, args)?;
        
        match (gt_result, eq_result) {
            (Value::Bool(gt), Value::Bool(eq)) => Some(Value::Bool(gt || eq)),
            _ => None,
        }
    }
    
    /// Default hash method
    fn dunder_hash(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        use std::collections::hash_map::DefaultHasher;
        use std::hash::{Hash, Hasher};
        
        let hash_value = match obj {
            Value::Int(n) => *n as u64,
            Value::Float(n) => n.to_bits(),
            Value::Bool(b) => if *b { 1 } else { 0 },
            Value::Str(s) => {
                let mut hasher = DefaultHasher::new();
                s.hash(&mut hasher);
                hasher.finish()
            },
            Value::None => 0,
            Value::Tuple(items) => {
                let mut hasher = DefaultHasher::new();
                for item in items {
                    // Recursively hash tuple items
                    match Self::dunder_hash(item, vec![]) {
                        Some(Value::Int(h)) => (h as u64).hash(&mut hasher),
                        _ => return None, // unhashable type
                    }
                }
                hasher.finish()
            },
            _ => return None, // unhashable type
        };
        
        Some(Value::Int(hash_value as i64))
    }
    
    /// Default bool conversion
    fn dunder_bool(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        Some(Value::Bool(obj.is_truthy()))
    }
    
    /// Default attribute access
    fn dunder_getattribute(obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.len() != 1 {
            return None;
        }
        
        let attr_name = match &args[0] {
            Value::Str(s) => s,
            _ => return None,
        };
        
        match obj {
            Value::Object { fields, .. } => {
                fields.get(attr_name).cloned()
            },
            _ => None,
        }
    }
    
    /// Default attribute setting
    fn dunder_setattr(_obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.len() != 2 {
            return None;
        }
        
        // This is a placeholder - actual implementation would need mutable access
        None
    }
    
    /// Default attribute deletion
    fn dunder_delattr(_obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.len() != 1 {
            return None;
        }
        
        // This is a placeholder - actual implementation would need mutable access
        None
    }
    
    /// Default directory listing
    fn dunder_dir(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        let mut attrs = vec![];
        
        // Add base object methods
        for method_name in Self::get_base_methods().keys() {
            attrs.push(Value::Str(method_name.clone()));
        }
        
        // Add object-specific attributes
        match obj {
            Value::Object { fields, .. } => {
                for field_name in fields.keys() {
                    attrs.push(Value::Str(field_name.clone()));
                }
            },
            _ => {
                // Add type-specific methods
                if let Some(type_methods) = get_type_methods(obj) {
                    for method_name in type_methods.keys() {
                        attrs.push(Value::Str(method_name.clone()));
                    }
                }
            }
        }
        
        attrs.sort_by(|a, b| {
            match (a, b) {
                (Value::Str(s1), Value::Str(s2)) => s1.cmp(s2),
                _ => std::cmp::Ordering::Equal,
            }
        });
        
        Some(Value::List(attrs))
    }
    
    /// Default class method
    fn dunder_class(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        let class_name = match obj {
            Value::Object { class_name, .. } => class_name.clone(),
            _ => obj.type_name().to_string(),
        };
        Some(Value::Str(format!("<class '{}'>", class_name)))
    }
    
    /// Default sizeof method
    fn dunder_sizeof(obj: &Value, _args: Vec<Value>) -> Option<Value> {
        let size = match obj {
            Value::Int(_) => 8,
            Value::Float(_) => 8,
            Value::Bool(_) => 1,
            Value::Str(s) => s.len(),
            Value::List(items) => items.len() * 8, // Rough estimate
            Value::Dict(dict) => dict.len() * 16, // Rough estimate
            Value::Tuple(items) => items.len() * 8,
            Value::Set(items) => items.len() * 8,
            Value::Bytes(bytes) => bytes.len(),
            Value::ByteArray(bytes) => bytes.len(),
            Value::None => 0,
            _ => 64, // Default size for complex objects
        };
        Some(Value::Int(size as i64))
    }
    
    /// Default new method (object creation)
    fn dunder_new(_obj: &Value, args: Vec<Value>) -> Option<Value> {
        if args.is_empty() {
            return None;
        }
        
        // For now, just return a basic object
        // Real implementation would create instances based on the class
        Some(Value::Object { 
            class_name: "object".to_string(), 
            fields: HashMap::new(),
            base_object: crate::base_object::BaseObject::new("object".to_string(), vec![]),
            mro: crate::base_object::MRO::from_linearization(vec!["object".to_string()])
        })
    }
    
    /// Default init method (object initialization)
    fn dunder_init(_obj: &Value, _args: Vec<Value>) -> Option<Value> {
        // Default __init__ does nothing
        Some(Value::None)
    }
    
    /// Default destructor
    fn dunder_del(_obj: &Value, _args: Vec<Value>) -> Option<Value> {
        // Default destructor does nothing
        Some(Value::None)
    }
}

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
        Value::Object { class_name, .. } => {
            // For objects, we need to look up the class definition to find dunder methods
            // This is a simplified approach - in a full implementation, we'd need VM access
            // to look up the class definition properly
            Some(get_basic_object_methods())
        }
        _ => None,
    }
}

/// Object-specific dunder methods (for custom classes)
fn get_basic_object_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    
    // Provide basic implementations for common dunder methods
    methods.insert("__str__".to_string(), basic_object_str as DunderMethod);
    methods.insert("__repr__".to_string(), basic_object_repr as DunderMethod);
    
    // Include all base object methods (including comparison methods)
    let base_methods = BaseObject::get_base_methods();
    for (name, method) in base_methods {
        methods.insert(name, method);
    }
    
    methods
}

/// Basic __str__ implementation for objects
fn basic_object_str(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    match obj {
        Value::Object { class_name, .. } => {
            Some(Value::Str(format!("<{} object>", class_name)))
        }
        _ => None,
    }
}

/// Basic __repr__ implementation for objects
fn basic_object_repr(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    match obj {
        Value::Object { class_name, .. } => {
            Some(Value::Str(format!("<{} object>", class_name)))
        }
        _ => None,
    }
}

/// Wrapper function that calls object methods
fn object_method_wrapper(obj: &Value, args: Vec<Value>) -> Option<Value> {
    // This is a placeholder that provides basic implementations
    // The actual method calling would need VM access to execute user-defined functions
    
    match obj {
        Value::Object { class_name, fields, .. } => {
            // For now, provide reasonable defaults for common methods
            // In a full implementation, we would need to:
            // 1. Look up the specific method being called
            // 2. Execute it with proper VM context
            Some(Value::Str(format!("<{} object>", class_name)))
        }
        _ => None,
    }
}
fn get_int_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    
    // Arithmetic operations
    methods.insert("__add__".to_string(), int_add as DunderMethod);
    methods.insert("__sub__".to_string(), int_sub as DunderMethod);
    methods.insert("__mul__".to_string(), int_mul as DunderMethod);
    methods.insert("__truediv__".to_string(), int_truediv as DunderMethod);
    methods.insert("__floordiv__".to_string(), int_floordiv as DunderMethod);
    methods.insert("__mod__".to_string(), int_mod as DunderMethod);
    methods.insert("__pow__".to_string(), int_pow as DunderMethod);
    
    // Bitwise operations
    methods.insert("__and__".to_string(), int_and as DunderMethod);
    methods.insert("__or__".to_string(), int_or as DunderMethod);
    methods.insert("__xor__".to_string(), int_xor as DunderMethod);
    methods.insert("__lshift__".to_string(), int_lshift as DunderMethod);
    methods.insert("__rshift__".to_string(), int_rshift as DunderMethod);
    methods.insert("__invert__".to_string(), int_invert as DunderMethod);
    
    // Unary operations
    methods.insert("__neg__".to_string(), int_neg as DunderMethod);
    methods.insert("__pos__".to_string(), int_pos as DunderMethod);
    methods.insert("__abs__".to_string(), int_abs as DunderMethod);
    
    methods
}

// Placeholder implementations for integer methods
fn int_add(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        Some(Value::Int(a + b))
    } else if let (Value::Int(a), Some(Value::Float(b))) = (obj, args.get(0)) {
        Some(Value::Float(*a as f64 + b))
    } else {
        None
    }
}

fn int_sub(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        Some(Value::Int(a - b))
    } else if let (Value::Int(a), Some(Value::Float(b))) = (obj, args.get(0)) {
        Some(Value::Float(*a as f64 - b))
    } else {
        None
    }
}

fn int_mul(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        Some(Value::Int(a * b))
    } else if let (Value::Int(a), Some(Value::Float(b))) = (obj, args.get(0)) {
        Some(Value::Float(*a as f64 * b))
    } else {
        None
    }
}

fn int_truediv(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        if *b == 0 {
            return None; // Division by zero
        }
        Some(Value::Float(*a as f64 / *b as f64))
    } else if let (Value::Int(a), Some(Value::Float(b))) = (obj, args.get(0)) {
        if *b == 0.0 {
            return None; // Division by zero
        }
        Some(Value::Float(*a as f64 / b))
    } else {
        None
    }
}

fn int_floordiv(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        if *b == 0 {
            return None; // Division by zero
        }
        Some(Value::Int(a / b))
    } else {
        None
    }
}

fn int_mod(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        if *b == 0 {
            return None; // Division by zero
        }
        Some(Value::Int(a % b))
    } else {
        None
    }
}

fn int_pow(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        if *b < 0 {
            Some(Value::Float((*a as f64).powf(*b as f64)))
        } else {
            Some(Value::Int(a.pow(*b as u32)))
        }
    } else {
        None
    }
}

fn int_and(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        Some(Value::Int(a & b))
    } else {
        None
    }
}

fn int_or(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        Some(Value::Int(a | b))
    } else {
        None
    }
}

fn int_xor(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        Some(Value::Int(a ^ b))
    } else {
        None
    }
}

fn int_lshift(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        if *b < 0 {
            return None; // Negative shift count
        }
        Some(Value::Int(a << b))
    } else {
        None
    }
}

fn int_rshift(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Int(a), Some(Value::Int(b))) = (obj, args.get(0)) {
        if *b < 0 {
            return None; // Negative shift count
        }
        Some(Value::Int(a >> b))
    } else {
        None
    }
}

fn int_invert(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Int(a) = obj {
        Some(Value::Int(!a))
    } else {
        None
    }
}

fn int_neg(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Int(a) = obj {
        Some(Value::Int(-a))
    } else {
        None
    }
}

fn int_pos(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Int(a) = obj {
        Some(Value::Int(*a))
    } else {
        None
    }
}

fn int_abs(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Int(a) = obj {
        Some(Value::Int(a.abs()))
    } else {
        None
    }
}

// Placeholder functions for other types - these would be implemented similarly
fn get_float_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    
    // Arithmetic operations
    methods.insert("__add__".to_string(), float_add as DunderMethod);
    methods.insert("__sub__".to_string(), float_sub as DunderMethod);
    methods.insert("__mul__".to_string(), float_mul as DunderMethod);
    methods.insert("__truediv__".to_string(), float_truediv as DunderMethod);
    methods.insert("__floordiv__".to_string(), float_floordiv as DunderMethod);
    methods.insert("__mod__".to_string(), float_mod as DunderMethod);
    methods.insert("__pow__".to_string(), float_pow as DunderMethod);
    
    // Unary operations
    methods.insert("__neg__".to_string(), float_neg as DunderMethod);
    methods.insert("__pos__".to_string(), float_pos as DunderMethod);
    methods.insert("__abs__".to_string(), float_abs as DunderMethod);
    
    // Comparison operations
    methods.insert("__eq__".to_string(), float_eq as DunderMethod);
    methods.insert("__ne__".to_string(), float_ne as DunderMethod);
    methods.insert("__lt__".to_string(), float_lt as DunderMethod);
    methods.insert("__le__".to_string(), float_le as DunderMethod);
    methods.insert("__gt__".to_string(), float_gt as DunderMethod);
    methods.insert("__ge__".to_string(), float_ge as DunderMethod);
    
    methods
}

fn get_string_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    
    // String operations
    methods.insert("__add__".to_string(), string_add as DunderMethod);
    methods.insert("__mul__".to_string(), string_mul as DunderMethod);
    methods.insert("__len__".to_string(), string_len as DunderMethod);
    methods.insert("__getitem__".to_string(), string_getitem as DunderMethod);
    methods.insert("__contains__".to_string(), string_contains as DunderMethod);
    methods.insert("__eq__".to_string(), string_eq as DunderMethod);
    methods.insert("__ne__".to_string(), string_ne as DunderMethod);
    methods.insert("__lt__".to_string(), string_lt as DunderMethod);
    methods.insert("__le__".to_string(), string_le as DunderMethod);
    methods.insert("__gt__".to_string(), string_gt as DunderMethod);
    methods.insert("__ge__".to_string(), string_ge as DunderMethod);
    
    methods
}

fn get_list_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    
    // List operations
    methods.insert("__add__".to_string(), list_add as DunderMethod);
    methods.insert("__mul__".to_string(), list_mul as DunderMethod);
    methods.insert("__len__".to_string(), list_len as DunderMethod);
    methods.insert("__getitem__".to_string(), list_getitem as DunderMethod);
    methods.insert("__setitem__".to_string(), list_setitem as DunderMethod);
    methods.insert("__contains__".to_string(), list_contains as DunderMethod);
    methods.insert("__eq__".to_string(), list_eq as DunderMethod);
    methods.insert("__ne__".to_string(), list_ne as DunderMethod);
    
    methods
}

fn get_dict_methods() -> HashMap<String, DunderMethod> {
    let mut methods = HashMap::new();
    
    // Dictionary operations
    methods.insert("__getitem__".to_string(), dict_getitem as DunderMethod);
    methods.insert("__setitem__".to_string(), dict_setitem as DunderMethod);
    methods.insert("__len__".to_string(), dict_len as DunderMethod);
    methods.insert("__contains__".to_string(), dict_contains as DunderMethod);
    methods.insert("__eq__".to_string(), dict_eq as DunderMethod);
    methods.insert("__ne__".to_string(), dict_ne as DunderMethod);
    
    methods
}

fn get_tuple_methods() -> HashMap<String, DunderMethod> {
    HashMap::new() // TODO: Implement tuple methods
}

fn get_set_methods() -> HashMap<String, DunderMethod> {
    HashMap::new() // TODO: Implement set methods
}

fn get_bytes_methods() -> HashMap<String, DunderMethod> {
    HashMap::new() // TODO: Implement bytes methods
}

fn get_bytearray_methods() -> HashMap<String, DunderMethod> {
    HashMap::new() // TODO: Implement bytearray methods
}

// String method implementations
fn string_add(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Str(format!("{}{}", a, b)))
    } else {
        None
    }
}

fn string_mul(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(s), Some(Value::Int(n))) = (obj, args.get(0)) {
        if *n < 0 {
            Some(Value::Str(String::new()))
        } else {
            Some(Value::Str(s.repeat(*n as usize)))
        }
    } else {
        None
    }
}

fn string_len(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Str(s) = obj {
        Some(Value::Int(s.len() as i64))
    } else {
        None
    }
}

fn string_getitem(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(s), Some(Value::Int(index))) = (obj, args.get(0)) {
        let chars: Vec<char> = s.chars().collect();
        let len = chars.len() as i64;
        let idx = if *index < 0 { len + index } else { *index };
        
        if idx >= 0 && idx < len {
            Some(Value::Str(chars[idx as usize].to_string()))
        } else {
            None // Index out of bounds
        }
    } else {
        None
    }
}

fn string_contains(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(s), Some(Value::Str(substr))) = (obj, args.get(0)) {
        Some(Value::Bool(s.contains(substr)))
    } else {
        None
    }
}

fn string_eq(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a == b))
    } else {
        Some(Value::Bool(false))
    }
}

fn string_ne(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a != b))
    } else {
        Some(Value::Bool(true))
    }
}

fn string_lt(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a < b))
    } else {
        None
    }
}

fn string_le(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a <= b))
    } else {
        None
    }
}

fn string_gt(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a > b))
    } else {
        None
    }
}

fn string_ge(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Str(a), Some(Value::Str(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a >= b))
    } else {
        None
    }
}

// List method implementations
fn list_add(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::List(a), Some(Value::List(b))) = (obj, args.get(0)) {
        let mut result = a.clone();
        result.extend(b.clone());
        Some(Value::List(result))
    } else {
        None
    }
}

fn list_mul(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::List(list), Some(Value::Int(n))) = (obj, args.get(0)) {
        if *n <= 0 {
            Some(Value::List(Vec::new()))
        } else {
            let mut result = Vec::new();
            for _ in 0..*n {
                result.extend(list.clone());
            }
            Some(Value::List(result))
        }
    } else {
        None
    }
}

fn list_len(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::List(list) = obj {
        Some(Value::Int(list.len() as i64))
    } else {
        None
    }
}

fn list_getitem(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::List(list), Some(Value::Int(index))) = (obj, args.get(0)) {
        let len = list.len() as i64;
        let idx = if *index < 0 { len + index } else { *index };
        
        if idx >= 0 && idx < len {
            Some(list[idx as usize].clone())
        } else {
            None // Index out of bounds
        }
    } else {
        None
    }
}

fn list_setitem(obj: &Value, args: Vec<Value>) -> Option<Value> {
    // Note: This would need mutable access to the list, which is challenging with the current design
    // For now, we'll return None to indicate this operation isn't supported in this context
    // In a real implementation, this would require a different approach to handle mutability
    None
}

fn list_contains(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::List(list), Some(item)) = (obj, args.get(0)) {
        Some(Value::Bool(list.contains(item)))
    } else {
        None
    }
}

fn list_eq(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::List(a), Some(Value::List(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a == b))
    } else {
        Some(Value::Bool(false))
    }
}

fn list_ne(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::List(a), Some(Value::List(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a != b))
    } else {
        Some(Value::Bool(true))
    }
}

// Dictionary method implementations
fn dict_getitem(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Dict(dict), Some(key)) = (obj, args.get(0)) {
        // Convert key to string for HashMap lookup
        let key_str = match key {
            Value::Str(s) => s,
            _ => return None, // Only string keys are supported
        };
        dict.get(key_str).cloned()
    } else {
        None
    }
}

fn dict_setitem(obj: &Value, args: Vec<Value>) -> Option<Value> {
    // Note: This would need mutable access to the dictionary, which is challenging with the current design
    // For now, we'll return None to indicate this operation isn't supported in this context
    // In a real implementation, this would require a different approach to handle mutability
    None
}

fn dict_len(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Dict(dict) = obj {
        Some(Value::Int(dict.len() as i64))
    } else {
        None
    }
}

fn dict_contains(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Dict(dict), Some(key)) = (obj, args.get(0)) {
        // Convert key to string for HashMap lookup
        let key_str = match key {
            Value::Str(s) => s,
            _ => return Some(Value::Bool(false)), // Only string keys are supported
        };
        Some(Value::Bool(dict.contains_key(key_str)))
    } else {
        None
    }
}

fn dict_eq(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Dict(a), Some(Value::Dict(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a == b))
    } else {
        Some(Value::Bool(false))
    }
}

fn dict_ne(obj: &Value, args: Vec<Value>) -> Option<Value> {
    if let (Value::Dict(a), Some(Value::Dict(b))) = (obj, args.get(0)) {
        Some(Value::Bool(a != b))
    } else {
        Some(Value::Bool(true))
    }
}

// Float method implementations
fn float_add(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Float(a + b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Float(a + *b as f64)),
        _ => None,
    }
}

fn float_sub(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Float(a - b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Float(a - *b as f64)),
        _ => None,
    }
}

fn float_mul(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Float(a * b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Float(a * *b as f64)),
        _ => None,
    }
}

fn float_truediv(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => {
            if *b == 0.0 {
                None // Division by zero
            } else {
                Some(Value::Float(a / b))
            }
        }
        (Value::Float(a), Some(Value::Int(b))) => {
            if *b == 0 {
                None // Division by zero
            } else {
                Some(Value::Float(a / *b as f64))
            }
        }
        _ => None,
    }
}

fn float_floordiv(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => {
            if *b == 0.0 {
                None // Division by zero
            } else {
                Some(Value::Float((a / b).floor()))
            }
        }
        (Value::Float(a), Some(Value::Int(b))) => {
            if *b == 0 {
                None // Division by zero
            } else {
                Some(Value::Float((a / *b as f64).floor()))
            }
        }
        _ => None,
    }
}

fn float_mod(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => {
            if *b == 0.0 {
                None // Division by zero
            } else {
                Some(Value::Float(a % b))
            }
        }
        (Value::Float(a), Some(Value::Int(b))) => {
            if *b == 0 {
                None // Division by zero
            } else {
                Some(Value::Float(a % *b as f64))
            }
        }
        _ => None,
    }
}

fn float_pow(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Float(a.powf(*b))),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Float(a.powf(*b as f64))),
        _ => None,
    }
}

fn float_neg(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Float(a) = obj {
        Some(Value::Float(-a))
    } else {
        None
    }
}

fn float_pos(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Float(a) = obj {
        Some(Value::Float(*a))
    } else {
        None
    }
}

fn float_abs(obj: &Value, _args: Vec<Value>) -> Option<Value> {
    if let Value::Float(a) = obj {
        Some(Value::Float(a.abs()))
    } else {
        None
    }
}

fn float_eq(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Bool((a - b).abs() < f64::EPSILON)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Bool((*a - *b as f64).abs() < f64::EPSILON)),
        _ => Some(Value::Bool(false)),
    }
}

fn float_ne(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Bool((a - b).abs() >= f64::EPSILON)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Bool((*a - *b as f64).abs() >= f64::EPSILON)),
        _ => Some(Value::Bool(true)),
    }
}

fn float_lt(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Bool(a < b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Bool(*a < *b as f64)),
        _ => None,
    }
}

fn float_le(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Bool(a <= b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Bool(*a <= *b as f64)),
        _ => None,
    }
}

fn float_gt(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Bool(a > b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Bool(*a > *b as f64)),
        _ => None,
    }
}

fn float_ge(obj: &Value, args: Vec<Value>) -> Option<Value> {
    match (obj, args.get(0)) {
        (Value::Float(a), Some(Value::Float(b))) => Some(Value::Bool(a >= b)),
        (Value::Float(a), Some(Value::Int(b))) => Some(Value::Bool(*a >= *b as f64)),
        _ => None,
    }
}

/// Method resolution order - finds the appropriate dunder method for a value
pub fn resolve_dunder_method(value: &Value, method_name: &str) -> Option<DunderMethod> {
    // First check type-specific methods
    if let Some(type_methods) = get_type_methods(value) {
        if let Some(method) = type_methods.get(method_name) {
            return Some(*method);
        }
    }
    
    // Then check base object methods
    let base_methods = BaseObject::get_base_methods();
    base_methods.get(method_name).copied()
}

/// Call a dunder method on a value with VM context for user-defined methods
pub fn call_dunder_method_with_vm(vm: &mut crate::vm::VM, value: &Value, method_name: &str, args: Vec<Value>) -> Option<Value> {
    // For objects, try to find user-defined dunder methods in the class definition
    if let Value::Object { class_name, .. } = value {
        // Look up the class definition in the VM
        if let Some(Value::Object { fields: class_methods, .. }) = vm.get_variable(class_name) {
            if let Some(method_value) = class_methods.get(method_name) {
                match method_value {
                    Value::Closure { name: method_name, params, body, captured_scope, .. } => {
                        // Call the user-defined dunder method
                        let mut method_args = vec![value.clone()];
                        method_args.extend(args.clone());
                        
                        match vm.call_user_function(method_name, params, body.clone(), method_args, HashMap::new(), Some(captured_scope.clone())) {
                            Ok(result) => return Some(result),
                            Err(_) => {} // Fall through to default implementation
                        }
                    }
                    _ => {}
                }
            }
        }
    }
    
    // Fall back to the original implementation for built-in types and default behavior
    call_dunder_method(value, method_name, args)
}

/// Call a dunder method on a value (original implementation for backward compatibility)
pub fn call_dunder_method(value: &Value, method_name: &str, args: Vec<Value>) -> Option<Value> {
    if let Some(method) = resolve_dunder_method(value, method_name) {
        method(value, args)
    } else {
        None
    }
}
