use std::collections::HashMap;
use crate::value::Value;
use crate::modules::hplist::HPList;

/// Method Resolution Order implementation for multiple inheritance
#[derive(Debug, Clone, PartialEq)]
pub struct MRO {
    pub linearization: Vec<String>, // Class names in resolution order
}

impl MRO {
    pub fn new() -> Self {
        Self {
            linearization: vec!["object".to_string()],
        }
    }

    /// Create MRO from a computed linearization
    pub fn from_linearization(linearization: Vec<String>) -> Self {
        Self { linearization }
    }

    /// Compute C3 linearization for multiple inheritance
    /// Based on Python's C3 linearization algorithm
    pub fn compute_c3_linearization(
        class_name: &str,
        bases: &[String],
        class_mros: &HashMap<String, Vec<String>>
    ) -> Result<Vec<String>, String> {
        // C3 linearization algorithm implementation
        // L(C) = C + merge(L(B1), L(B2), ..., L(Bn), B1B2...Bn)
        
        if bases.is_empty() {
            // Base case: only inherits from object
            let result = vec![class_name.to_string(), "object".to_string()];
            return Ok(result);
        }

        // Get linearizations of all base classes
        let mut base_linearizations = Vec::new();
        for base in bases {
            if let Some(base_mro) = class_mros.get(base) {
                base_linearizations.push(base_mro.clone());
            } else {
                // If base MRO not found, create appropriate fallback
                let fallback_mro = if base == "object" {
                    // Special case: object class has only itself in MRO
                    vec!["object".to_string()]
                } else {
                    // Other classes inherit from object
                    vec![base.clone(), "object".to_string()]
                };
                base_linearizations.push(fallback_mro);
            }
        }

        // Add the list of bases as the last sequence to merge
        base_linearizations.push(bases.to_vec());

        // Perform C3 merge
        let merged = Self::c3_merge(base_linearizations)?;
        
        // Prepend the current class
        let mut result = vec![class_name.to_string()];
        result.extend(merged);
        
        Ok(result)
    }

    /// C3 merge algorithm
    /// Merges multiple linearizations following C3 rules:
    /// 1. Take the head of the first list if it doesn't appear in the tail of any other list
    /// 2. Otherwise, look at the head of the next list
    /// 3. If no head can be taken, the hierarchy is inconsistent
    fn c3_merge(mut sequences: Vec<Vec<String>>) -> Result<Vec<String>, String> {
        let mut result = Vec::new();
        
        // Add a guard to prevent infinite loops
        let mut iterations = 0;
        let max_iterations = 1000; // Prevent infinite loops
        
        while !sequences.is_empty() && iterations < max_iterations {
            iterations += 1;
            
            // Remove empty sequences
            sequences.retain(|seq| !seq.is_empty());
            
            if sequences.is_empty() {
                break;
            }

            let mut candidate_found = false;
            let mut selected_candidate: Option<String> = None;
            
            // Try each sequence's head as a candidate
            for (_i, seq) in sequences.iter().enumerate() {
                if seq.is_empty() {
                    continue;
                }
                
                let candidate = &seq[0];
                
                // Check if this candidate appears in the tail of any other sequence
                let mut appears_in_tail = false;
                for (_j, other_seq) in sequences.iter().enumerate() {
                    if _i == _j {
                        continue; // Skip the same sequence
                    }
                    if other_seq.len() > 1 && other_seq[1..].contains(candidate) {
                        appears_in_tail = true;
                        break;
                    }
                }
                
                if !appears_in_tail {
                    selected_candidate = Some(candidate.clone());
                    candidate_found = true;
                    break;
                }
            }
            
            if let Some(candidate) = selected_candidate {
                result.push(candidate.clone());
                
                // Remove this candidate from all sequences
                for seq in &mut sequences {
                    if !seq.is_empty() && seq[0] == candidate {
                        seq.remove(0);
                    }
                }
            }
            
            if !candidate_found {
                return Err("Cannot create a consistent method resolution order (MRO)".to_string());
            }
        }
        
        if iterations >= max_iterations {
            return Err("MRO computation exceeded maximum iterations - possible cycle".to_string());
        }
        
        Ok(result)
    }

    /// Get the method resolution order
    pub fn get_linearization(&self) -> &[String] {
        &self.linearization
    }

    /// Check if a class is in the MRO
    pub fn contains_class(&self, class_name: &str) -> bool {
        self.linearization.contains(&class_name.to_string())
    }

    /// Get the next class in MRO after the given class
    pub fn get_next_class(&self, class_name: &str) -> Option<&String> {
        if let Some(pos) = self.linearization.iter().position(|c| c == class_name) {
            self.linearization.get(pos + 1)
        } else {
            None
        }
    }

    /// Find method in MRO order
    pub fn find_method(&self, method_name: &str, class_methods: &HashMap<String, HashMap<String, DunderMethod>>) -> Option<DunderMethod> {
        for class_name in &self.linearization {
            if let Some(methods) = class_methods.get(class_name) {
                if let Some(method) = methods.get(method_name) {
                    return Some(method.clone());
                }
            }
        }
        None
    }
}

/// Type alias for dunder method functions
pub type DunderMethod = fn(&Value, &[Value]) -> Result<Value, String>;

/// Base object class that all Python objects inherit from
#[derive(Debug, Clone)]
pub struct BaseObject {
    pub class_name: String,
    pub mro: MRO,
    pub attributes: HashMap<String, Value>,
    pub dunder_methods: HashMap<String, DunderMethod>,
}

impl BaseObject {
    pub fn new(class_name: String, bases: Vec<String>) -> Self {
        // Compute MRO using C3 linearization
        let class_mros = HashMap::new(); // This would be populated from a class registry
        let linearization = MRO::compute_c3_linearization(&class_name, &bases, &class_mros)
            .unwrap_or_else(|_| {
                // Fallback to simple linearization if C3 fails
                let mut result = vec![class_name.clone()];
                result.extend_from_slice(&bases);
                if !result.contains(&"object".to_string()) {
                    result.push("object".to_string());
                }
                result
            });
        
        let mro = MRO::from_linearization(linearization);
        let dunder_methods = Self::get_default_dunder_methods();
        
        Self {
            class_name,
            mro,
            attributes: HashMap::new(),
            dunder_methods,
        }
    }

    /// Get all base dunder methods that every object should have
    pub fn get_base_methods() -> HashMap<String, DunderMethod> {
        Self::get_default_dunder_methods()
    }

    /// Get all default dunder methods for the base object
    pub fn get_default_dunder_methods() -> HashMap<String, DunderMethod> {
        let mut methods = HashMap::new();
        
        // Object creation and initialization
        methods.insert("__new__".to_string(), dunder_new as DunderMethod);
        methods.insert("__init__".to_string(), dunder_init as DunderMethod);
        methods.insert("__del__".to_string(), dunder_del as DunderMethod);
        
        // String representation
        methods.insert("__str__".to_string(), dunder_str);
        methods.insert("__repr__".to_string(), dunder_repr);
        methods.insert("__format__".to_string(), dunder_format);
        methods.insert("__bytes__".to_string(), dunder_bytes);
        
        // Comparison operators
        methods.insert("__eq__".to_string(), dunder_eq);
        methods.insert("__ne__".to_string(), dunder_ne);
        methods.insert("__lt__".to_string(), dunder_lt);
        methods.insert("__le__".to_string(), dunder_le);
        methods.insert("__gt__".to_string(), dunder_gt);
        methods.insert("__ge__".to_string(), dunder_ge);
        
        // Hash and boolean conversion
        methods.insert("__hash__".to_string(), dunder_hash);
        methods.insert("__bool__".to_string(), dunder_bool);
        
        // Attribute access
        methods.insert("__getattribute__".to_string(), dunder_getattribute);
        methods.insert("__getattr__".to_string(), dunder_getattr);
        methods.insert("__setattr__".to_string(), dunder_setattr);
        methods.insert("__delattr__".to_string(), dunder_delattr);
        methods.insert("__dir__".to_string(), dunder_dir);
        
        // Container methods
        methods.insert("__len__".to_string(), dunder_len);
        methods.insert("__getitem__".to_string(), dunder_getitem);
        methods.insert("__setitem__".to_string(), dunder_setitem);
        methods.insert("__delitem__".to_string(), dunder_delitem);
        methods.insert("__iter__".to_string(), dunder_iter);
        methods.insert("__next__".to_string(), dunder_next);
        methods.insert("__reversed__".to_string(), dunder_reversed);
        methods.insert("__contains__".to_string(), dunder_contains);
        
        // Arithmetic operators
        methods.insert("__add__".to_string(), dunder_add);
        methods.insert("__sub__".to_string(), dunder_sub);
        methods.insert("__mul__".to_string(), dunder_mul);
        methods.insert("__truediv__".to_string(), dunder_truediv);
        methods.insert("__floordiv__".to_string(), dunder_floordiv);
        methods.insert("__mod__".to_string(), dunder_mod);
        methods.insert("__divmod__".to_string(), dunder_divmod);
        methods.insert("__pow__".to_string(), dunder_pow);
        methods.insert("__lshift__".to_string(), dunder_lshift);
        methods.insert("__rshift__".to_string(), dunder_rshift);
        methods.insert("__and__".to_string(), dunder_and);
        methods.insert("__xor__".to_string(), dunder_xor);
        methods.insert("__or__".to_string(), dunder_or);
        
        // Reverse arithmetic operators
        methods.insert("__radd__".to_string(), dunder_radd);
        methods.insert("__rsub__".to_string(), dunder_rsub);
        methods.insert("__rmul__".to_string(), dunder_rmul);
        methods.insert("__rtruediv__".to_string(), dunder_rtruediv);
        methods.insert("__rfloordiv__".to_string(), dunder_rfloordiv);
        methods.insert("__rmod__".to_string(), dunder_rmod);
        methods.insert("__rdivmod__".to_string(), dunder_rdivmod);
        methods.insert("__rpow__".to_string(), dunder_rpow);
        methods.insert("__rlshift__".to_string(), dunder_rlshift);
        methods.insert("__rrshift__".to_string(), dunder_rrshift);
        methods.insert("__rand__".to_string(), dunder_rand);
        methods.insert("__rxor__".to_string(), dunder_rxor);
        methods.insert("__ror__".to_string(), dunder_ror);
        
        // In-place arithmetic operators
        methods.insert("__iadd__".to_string(), dunder_iadd);
        methods.insert("__isub__".to_string(), dunder_isub);
        methods.insert("__imul__".to_string(), dunder_imul);
        methods.insert("__itruediv__".to_string(), dunder_itruediv);
        methods.insert("__ifloordiv__".to_string(), dunder_ifloordiv);
        methods.insert("__imod__".to_string(), dunder_imod);
        methods.insert("__ipow__".to_string(), dunder_ipow);
        methods.insert("__ilshift__".to_string(), dunder_ilshift);
        methods.insert("__irshift__".to_string(), dunder_irshift);
        methods.insert("__iand__".to_string(), dunder_iand);
        methods.insert("__ixor__".to_string(), dunder_ixor);
        methods.insert("__ior__".to_string(), dunder_ior);
        
        // Unary operators
        methods.insert("__neg__".to_string(), dunder_neg);
        methods.insert("__pos__".to_string(), dunder_pos);
        methods.insert("__abs__".to_string(), dunder_abs);
        methods.insert("__invert__".to_string(), dunder_invert);
        
        // Type conversion
        methods.insert("__complex__".to_string(), dunder_complex);
        methods.insert("__int__".to_string(), dunder_int);
        methods.insert("__float__".to_string(), dunder_float);
        methods.insert("__index__".to_string(), dunder_index);
        
        // Context managers
        methods.insert("__enter__".to_string(), dunder_enter);
        methods.insert("__exit__".to_string(), dunder_exit);
        
        // Callable objects
        methods.insert("__call__".to_string(), dunder_call);
        
        // Copy and pickle support
        methods.insert("__copy__".to_string(), dunder_copy);
        methods.insert("__deepcopy__".to_string(), dunder_deepcopy);
        methods.insert("__getstate__".to_string(), dunder_getstate);
        methods.insert("__setstate__".to_string(), dunder_setstate);
        methods.insert("__reduce__".to_string(), dunder_reduce);
        methods.insert("__reduce_ex__".to_string(), dunder_reduce_ex);
        
        methods
    }

    /// Get method from this object's dunder methods or MRO
    pub fn get_method(&self, method_name: &str, class_registry: &HashMap<String, HashMap<String, DunderMethod>>) -> Option<DunderMethod> {
        // First check this object's methods
        if let Some(method) = self.dunder_methods.get(method_name) {
            return Some(*method);
        }
        
        // Then check MRO
        self.mro.find_method(method_name, class_registry)
    }
}

// Default implementations for all dunder methods
// These provide basic Python-like behavior

// Object creation and initialization
fn dunder_new(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    // Default __new__ creates a new instance
    Ok(crate::value::Value::Object {
            class_name: "object".to_string(),
            fields: HashMap::new(),
            base_object: BaseObject::new("object".to_string(), vec![]),
            mro: MRO::from_linearization(vec!["object".to_string()])
        })
}

fn dunder_init(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    // Default __init__ does nothing
    Ok(Value::None)
}

fn dunder_del(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    // Default __del__ does nothing
    Ok(Value::None)
}

// String representation
fn dunder_str(self_val: &Value, _args: &[Value]) -> Result<Value, String> {
    match self_val {
        Value::Str(s) => Ok(Value::Str(s.clone())),
        Value::Int(i) => Ok(Value::Str(i.to_string())),
        Value::Float(f) => Ok(Value::Str(f.to_string())),
        Value::Bool(b) => Ok(Value::Str(if *b { "True".to_string() } else { "False".to_string() })),
        Value::None => Ok(Value::Str("None".to_string())),
        Value::List(items) => {
            let item_strs: Result<Vec<String>, String> = items.iter()
                .map(|item| dunder_str(item, &[]).map(|v| match v {
                    Value::Str(s) => s,
                    _ => "".to_string(),
                }))
                .collect();
            match item_strs {
                Ok(strs) => Ok(Value::Str(format!("[{}]", strs.join(", ")))),
                Err(e) => Err(e),
            }
        },
        _ => Ok(Value::Str(format!("<{} object>", self_val.type_name()))),
    }
}

fn dunder_repr(self_val: &Value, _args: &[Value]) -> Result<Value, String> {
    // Default __repr__ is same as __str__ for most types
    dunder_str(self_val, _args)
}

fn dunder_format(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    // Default __format__ calls __str__
    dunder_str(_self, _args)
}

fn dunder_bytes(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: cannot convert to bytes".to_string())
}

// Comparison operators
fn dunder_eq(self_val: &Value, args: &[Value]) -> Result<Value, String> {
    if args.is_empty() {
        return Err("TypeError: __eq__ missing 1 required positional argument".to_string());
    }
    
    let other = &args[0];
    let result = match (self_val, other) {
        (Value::Int(a), Value::Int(b)) => a == b,
        (Value::Float(a), Value::Float(b)) => a == b,
        (Value::Str(a), Value::Str(b)) => a == b,
        (Value::Bool(a), Value::Bool(b)) => a == b,
        (Value::None, Value::None) => true,
        _ => false, // Different types are not equal by default
    };
    
    Ok(Value::Bool(result))
}

fn dunder_ne(self_val: &Value, args: &[Value]) -> Result<Value, String> {
    let eq_result = dunder_eq(self_val, args)?;
    match eq_result {
        Value::Bool(b) => Ok(Value::Bool(!b)),
        _ => Err("Internal error in __ne__".to_string()),
    }
}

fn dunder_lt(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: '<' not supported between instances".to_string())
}

fn dunder_le(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: '<=' not supported between instances".to_string())
}

fn dunder_gt(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: '>' not supported between instances".to_string())
}

fn dunder_ge(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: '>=' not supported between instances".to_string())
}

// Hash and boolean conversion
fn dunder_hash(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unhashable type".to_string())
}

fn dunder_bool(self_val: &Value, _args: &[Value]) -> Result<Value, String> {
    let result = match self_val {
        Value::Bool(b) => *b,
        Value::Int(i) => *i != 0,
        Value::Float(f) => *f != 0.0,
        Value::Str(s) => !s.is_empty(),
        Value::List(items) => !items.is_empty(),
        Value::Dict(items) => !items.is_empty(),
        Value::None => false,
        _ => true, // Most objects are truthy by default
    };
    
    Ok(Value::Bool(result))
}

// Attribute access (placeholder implementations)
fn dunder_getattribute(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("AttributeError: attribute not found".to_string())
}

fn dunder_getattr(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("AttributeError: attribute not found".to_string())
}

fn dunder_setattr(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(Value::None)
}

fn dunder_delattr(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(Value::None)
}

fn dunder_dir(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(Value::List(HPList::new()))
}

// Container methods (placeholder implementations)
fn dunder_len(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object has no len()".to_string())
}

fn dunder_getitem(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object is not subscriptable".to_string())
}

fn dunder_setitem(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object does not support item assignment".to_string())
}

fn dunder_delitem(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object doesn't support item deletion".to_string())
}

fn dunder_iter(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object is not iterable".to_string())
}

fn dunder_next(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object is not an iterator".to_string())
}

fn dunder_reversed(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: argument to reversed() must be a sequence".to_string())
}

fn dunder_contains(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: argument of type 'object' is not iterable".to_string())
}

// Arithmetic operators (placeholder implementations that raise TypeError)
fn dunder_add(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for +".to_string())
}

fn dunder_sub(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for -".to_string())
}

fn dunder_mul(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for *".to_string())
}

fn dunder_truediv(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for /".to_string())
}

fn dunder_floordiv(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for //".to_string())
}

fn dunder_mod(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for %".to_string())
}

fn dunder_divmod(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for divmod()".to_string())
}

fn dunder_pow(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for **".to_string())
}

fn dunder_lshift(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for <<".to_string())
}

fn dunder_rshift(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for >>".to_string())
}

fn dunder_and(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for &".to_string())
}

fn dunder_xor(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for ^".to_string())
}

fn dunder_or(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for |".to_string())
}

// Reverse arithmetic operators
fn dunder_radd(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for +".to_string())
}

fn dunder_rsub(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for -".to_string())
}

fn dunder_rmul(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for *".to_string())
}

fn dunder_rtruediv(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for /".to_string())
}

fn dunder_rfloordiv(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for //".to_string())
}

fn dunder_rmod(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for %".to_string())
}

fn dunder_rdivmod(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for divmod()".to_string())
}

fn dunder_rpow(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for **".to_string())
}

fn dunder_rlshift(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for <<".to_string())
}

fn dunder_rrshift(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for >>".to_string())
}

fn dunder_rand(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for &".to_string())
}

fn dunder_rxor(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for ^".to_string())
}

fn dunder_ror(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: unsupported operand type(s) for |".to_string())
}

// In-place arithmetic operators
fn dunder_iadd(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_add(_self, _args)
}

fn dunder_isub(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_sub(_self, _args)
}

fn dunder_imul(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_mul(_self, _args)
}

fn dunder_itruediv(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_truediv(_self, _args)
}

fn dunder_ifloordiv(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_floordiv(_self, _args)
}

fn dunder_imod(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_mod(_self, _args)
}

fn dunder_ipow(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_pow(_self, _args)
}

fn dunder_ilshift(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_lshift(_self, _args)
}

fn dunder_irshift(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_rshift(_self, _args)
}

fn dunder_iand(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_and(_self, _args)
}

fn dunder_ixor(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_xor(_self, _args)
}

fn dunder_ior(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_or(_self, _args)
}

// Unary operators
fn dunder_neg(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: bad operand type for unary -".to_string())
}

fn dunder_pos(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: bad operand type for unary +".to_string())
}

fn dunder_abs(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: bad operand type for abs()".to_string())
}

fn dunder_invert(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: bad operand type for unary ~".to_string())
}

// Type conversion
fn dunder_complex(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: complex() argument must be a string or a number".to_string())
}

fn dunder_int(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: int() argument must be a string or a number".to_string())
}

fn dunder_float(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: float() argument must be a string or a number".to_string())
}

fn dunder_index(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object cannot be interpreted as an integer".to_string())
}

// Context managers
fn dunder_enter(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("AttributeError: __enter__".to_string())
}

fn dunder_exit(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("AttributeError: __exit__".to_string())
}

// Callable objects
fn dunder_call(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: object is not callable".to_string())
}

// Copy and pickle support
fn dunder_copy(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(_self.clone())
}

fn dunder_deepcopy(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(_self.clone())
}

fn dunder_getstate(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(Value::Dict(HashMap::new()))
}

fn dunder_setstate(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Ok(Value::None)
}

fn dunder_reduce(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    Err("TypeError: can't pickle object".to_string())
}

fn dunder_reduce_ex(_self: &Value, _args: &[Value]) -> Result<Value, String> {
    dunder_reduce(_self, _args)
}

impl PartialEq for BaseObject {
    fn eq(&self, other: &Self) -> bool {
        // Compare all fields except dunder_methods (function pointers can't be compared)
        self.class_name == other.class_name
            && self.mro == other.mro
            && self.attributes == other.attributes
            // Skip dunder_methods comparison since function pointers don't implement PartialEq
    }
}
