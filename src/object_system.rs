use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;

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
