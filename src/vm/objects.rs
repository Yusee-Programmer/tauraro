//! Object model (strings, lists, dicts, classes)
use crate::value::Value;
use std::collections::HashMap;
use std::rc::Rc;

/// String interning utilities
pub struct StringInterner {
    interner: HashMap<String, Rc<String>>,
}

impl StringInterner {
    pub fn new() -> Self {
        Self {
            interner: HashMap::new(),
        }
    }
    
    /// Intern a string for memory optimization
    pub fn intern(&mut self, s: &str) -> Rc<String> {
        if let Some(interned) = self.interner.get(s) {
            interned.clone()
        } else {
            let rc_string = Rc::new(s.to_string());
            self.interner.insert(s.to_string(), rc_string.clone());
            rc_string
        }
    }
    
    /// Get an interned string if it exists
    pub fn get(&self, s: &str) -> Option<Rc<String>> {
        self.interner.get(s).cloned()
    }
}

/// Object model utilities
pub struct ObjectModel;

impl ObjectModel {
    /// Create a new list object
    pub fn create_list(values: Vec<Value>) -> Value {
        Value::List(crate::modules::hplist::HPList::from_values(values))
    }
    
    /// Create a new dict object
    pub fn create_dict(pairs: Vec<(String, Value)>) -> Value {
        let mut map: HashMap<String, Value> = HashMap::new();
        for (key, value) in pairs {
            map.insert(key, value);
        }
        Value::Dict(map)
    }
    
    /// Create a new string object
    pub fn create_string(s: String) -> Value {
        Value::Str(s)
    }
    
    /// Create a new integer object
    pub fn create_integer(i: i64) -> Value {
        Value::Int(i)
    }
    
    /// Create a new float object
    pub fn create_float(f: f64) -> Value {
        Value::Float(f)
    }
    
    /// Create a new boolean object
    pub fn create_boolean(b: bool) -> Value {
        Value::Bool(b)
    }
}