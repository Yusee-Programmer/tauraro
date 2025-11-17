//! Object handling (dict, list, string, etc.)

use crate::value::Value;
use std::fmt::Debug;
use std::rc::Rc;
use std::cell::RefCell;

/// OPTIMIZED: Reference counted value using Rust's Rc (zero-cost abstraction)
/// This eliminates manual reference counting and enables true copy-on-write
#[derive(Debug, Clone)]
pub struct RcValue {
    inner: Rc<RefCell<Value>>,
}

/// Simple iterator for Range values
#[derive(Debug, Clone)]
pub struct RangeIterator {
    pub start: i64,
    pub stop: i64,
    pub step: i64,
    pub current: i64,
}

impl RcValue {
    /// Create a new reference counted value
    #[inline(always)]
    pub fn new(value: Value) -> Self {
        Self {
            inner: Rc::new(RefCell::new(value)),
        }
    }
    
    /// Get the value with copy-on-write optimization for primitives
    /// CRITICAL: For Int/Float/Bool/None, this copies (8 bytes, no allocation)
    /// For heap types (Str/List/Dict), this references the Rc
    #[inline(always)]
    pub fn get_value(&self) -> Value {
        self.inner.borrow().clone()
    }
    
    /// Fast access for read-only primitives (avoids RefCell borrow overhead)
    #[inline(always)]
    pub fn try_get_int(&self) -> Option<i64> {
        match &*self.inner.borrow() {
            Value::Int(i) => Some(*i),
            _ => None,
        }
    }
    
    #[inline(always)]
    pub fn try_get_float(&self) -> Option<f64> {
        match &*self.inner.borrow() {
            Value::Float(f) => Some(*f),
            _ => None,
        }
    }
    
    #[inline(always)]
    pub fn try_get_bool(&self) -> Option<bool> {
        match &*self.inner.borrow() {
            Value::Bool(b) => Some(*b),
            _ => None,
        }
    }
    
    /// Set the value with copy-on-write optimization
    /// If this is the only reference (strong_count == 1), mutate in-place
    /// Otherwise, create a new Rc (automatic COW)
    #[inline(always)]
    pub fn set_value(&mut self, value: Value) {
        if Rc::strong_count(&self.inner) == 1 {
            // OPTIMIZATION: We're the only owner, mutate in-place (no allocation!)
            *self.inner.borrow_mut() = value;
        } else {
            // Multiple references, need to create new Rc
            self.inner = Rc::new(RefCell::new(value));
        }
    }
    
    /// Check if this is the only reference (for copy-on-write)
    #[inline(always)]
    pub fn is_unique(&self) -> bool {
        Rc::strong_count(&self.inner) == 1
    }
    
    /// Get reference count for debugging
    #[inline(always)]
    pub fn ref_count(&self) -> usize {
        Rc::strong_count(&self.inner)
    }
    
    /// Check truthiness without cloning
    #[inline(always)]
    pub fn is_truthy(&self) -> bool {
        self.inner.borrow().is_truthy()
    }
    
    /// Access the underlying value (for compatibility with old code)
    #[inline(always)]
    pub fn value(&self) -> Value {
        self.get_value()
    }
}

// Implement PartialEq by comparing values
impl PartialEq for RcValue {
    fn eq(&self, other: &Self) -> bool {
        *self.inner.borrow() == *other.inner.borrow()
    }
}

impl Eq for RcValue {}

// Implement Hash by hashing the value
impl std::hash::Hash for RcValue {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        // For hash tables, we need to hash the value
        // This is expensive but necessary for dict keys
        format!("{:?}", self.inner.borrow()).hash(state);
    }
}



/// Object handling operations
pub struct ObjectOps;

// Object-related opcodes are already defined in instructions.rs
// The implementation for these opcodes is in arithmetic.rs in the execute_instruction_fast method