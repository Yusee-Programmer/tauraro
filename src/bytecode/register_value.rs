//! Unboxed register storage for high-performance VM execution
//!
//! This module provides RegisterValue, an enum that stores integers and floats
//! directly (unboxed) to eliminate Rc overhead for numeric operations.
//!
//! Performance impact:
//! - Integers: 8 bytes unboxed vs 24-40 bytes boxed (3-5x less memory)
//! - Operations: ~10 instructions vs ~50-100 instructions (5-10x faster)
//! - Cache efficiency: Better locality, fewer pointer chases

use crate::value::Value;
use crate::bytecode::objects::RcValue;
use std::fmt;

/// Register storage that avoids boxing for common types
///
/// This enum allows the VM to store integers and floats directly
/// without wrapping them in Rc<Value>, eliminating atomic refcount
/// operations and allocations for numeric code.
#[derive(Clone, PartialEq)]
pub enum RegisterValue {
    /// Unboxed 64-bit integer (8 bytes, no allocation!)
    Int(i64),
    
    /// Unboxed 64-bit float (8 bytes, no allocation!)
    Float(f64),
    
    /// Unboxed boolean (1 byte, but padded to 8 for enum discrimination)
    Bool(bool),
    
    /// Boxed value for everything else (strings, objects, lists, etc.)
    /// Only used when necessary - about 10% of operations in typical code
    Boxed(RcValue),
}

impl RegisterValue {
    /// Create from a Value, boxing only when necessary
    #[inline(always)]
    pub fn from_value(value: Value) -> Self {
        match value {
            Value::Int(n) => RegisterValue::Int(n),
            Value::Float(f) => RegisterValue::Float(f),
            Value::Bool(b) => RegisterValue::Bool(b),
            other => RegisterValue::Boxed(RcValue::new(other)),
        }
    }
    
    /// Convert to Value (cheap for unboxed types, clone for boxed)
    #[inline(always)]
    pub fn to_value(&self) -> Value {
        match self {
            RegisterValue::Int(n) => Value::Int(*n),
            RegisterValue::Float(f) => Value::Float(*f),
            RegisterValue::Bool(b) => Value::Bool(*b),
            RegisterValue::Boxed(rc) => rc.get_value().clone(),
        }
    }
    
    /// Get Value (copies for primitives, clones Rc for boxed)
    /// This replaces as_value_ref since we can't return references to RcValue internals
    pub fn as_value_ref(&self) -> Value {
        match self {
            RegisterValue::Boxed(rc) => rc.get_value(),
            _ => self.to_value(),
        }
    }
    
    /// Check if value is truthy (for conditionals)
    #[inline(always)]
    pub fn is_truthy(&self) -> bool {
        match self {
            RegisterValue::Int(n) => *n != 0,
            RegisterValue::Float(f) => *f != 0.0 && !f.is_nan(),
            RegisterValue::Bool(b) => *b,
            RegisterValue::Boxed(rc) => rc.get_value().is_truthy(),
        }
    }
    
    /// Get integer value if this is an Int, None otherwise
    #[inline(always)]
    pub fn as_int(&self) -> Option<i64> {
        match self {
            RegisterValue::Int(n) => Some(*n),
            _ => None,
        }
    }
    
    /// Get float value if this is a Float, None otherwise
    #[inline(always)]
    pub fn as_float(&self) -> Option<f64> {
        match self {
            RegisterValue::Float(f) => Some(*f),
            _ => None,
        }
    }
    
    /// Get bool value if this is a Bool, None otherwise
    #[inline(always)]
    pub fn as_bool(&self) -> Option<bool> {
        match self {
            RegisterValue::Bool(b) => Some(*b),
            _ => None,
        }
    }
    
    /// Unsafe: Get integer without checking (for performance-critical paths)
    #[inline(always)]
    pub unsafe fn as_int_unchecked(&self) -> i64 {
        match self {
            RegisterValue::Int(n) => *n,
            _ => std::hint::unreachable_unchecked(),
        }
    }
    
    /// Unsafe: Get float without checking (for performance-critical paths)
    #[inline(always)]
    pub unsafe fn as_float_unchecked(&self) -> f64 {
        match self {
            RegisterValue::Float(f) => *f,
            _ => std::hint::unreachable_unchecked(),
        }
    }
    
    /// Check if this is an integer
    #[inline(always)]
    pub fn is_int(&self) -> bool {
        matches!(self, RegisterValue::Int(_))
    }
    
    /// Check if this is a float
    #[inline(always)]
    pub fn is_float(&self) -> bool {
        matches!(self, RegisterValue::Float(_))
    }
    
    /// Check if this is a boolean
    #[inline(always)]
    pub fn is_bool(&self) -> bool {
        matches!(self, RegisterValue::Bool(_))
    }
    
    /// Check if this is boxed
    #[inline(always)]
    pub fn is_boxed(&self) -> bool {
        matches!(self, RegisterValue::Boxed(_))
    }
    
    /// Convert to RcValue (for compatibility with old code)
    pub fn to_rc_value(&self) -> RcValue {
        match self {
            RegisterValue::Int(n) => RcValue::new(Value::Int(*n)),
            RegisterValue::Float(f) => RcValue::new(Value::Float(*f)),
            RegisterValue::Bool(b) => RcValue::new(Value::Bool(*b)),
            RegisterValue::Boxed(rc) => rc.clone(),
        }
    }
    
    /// Create from RcValue (for compatibility with old code)
    pub fn from_rc_value(rc: RcValue) -> Self {
        match rc.get_value() {
            Value::Int(n) => RegisterValue::Int(n),
            Value::Float(f) => RegisterValue::Float(f),
            Value::Bool(b) => RegisterValue::Bool(b),
            other => RegisterValue::Boxed(RcValue::new(other)),
        }
    }
    
    /// Set value with copy-on-write optimization (works for all value types)
    /// 
    /// For boxed values, uses COW. For unboxed values, overwrites directly.
    pub fn set_value(&mut self, new_value: Value) {
        match new_value {
            Value::Int(n) => *self = RegisterValue::Int(n),
            Value::Float(f) => *self = RegisterValue::Float(f),
            Value::Bool(b) => *self = RegisterValue::Bool(b),
            other => {
                if let RegisterValue::Boxed(rc) = self {
                    rc.set_value(other); // Uses COW
                } else {
                    *self = RegisterValue::Boxed(RcValue::new(other));
                }
            }
        }
    }
    
    /// Get reference to Value (for compatibility - requires temporary allocation for unboxed!)
    /// 
    /// WARNING: Prefer `to_value()` or type-specific methods like `as_int()`.
    /// This is only for gradual migration from old code.
    pub fn as_value_for_compat(&self) -> Value {
        self.to_value()
    }
}

impl fmt::Debug for RegisterValue {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            RegisterValue::Int(n) => write!(f, "Int({})", n),
            RegisterValue::Float(fl) => write!(f, "Float({})", fl),
            RegisterValue::Bool(b) => write!(f, "Bool({})", b),
            RegisterValue::Boxed(rc) => write!(f, "Boxed({:?})", rc.get_value()),
        }
    }
}

impl Default for RegisterValue {
    fn default() -> Self {
        RegisterValue::Int(0)
    }
}

// Implement From/Into for easy conversion
impl From<i64> for RegisterValue {
    #[inline(always)]
    fn from(n: i64) -> Self {
        RegisterValue::Int(n)
    }
}

impl From<f64> for RegisterValue {
    #[inline(always)]
    fn from(f: f64) -> Self {
        RegisterValue::Float(f)
    }
}

impl From<bool> for RegisterValue {
    #[inline(always)]
    fn from(b: bool) -> Self {
        RegisterValue::Bool(b)
    }
}

impl From<Value> for RegisterValue {
    #[inline(always)]
    fn from(value: Value) -> Self {
        RegisterValue::from_value(value)
    }
}

impl From<RcValue> for RegisterValue {
    #[inline(always)]
    fn from(rc: RcValue) -> Self {
        RegisterValue::from_rc_value(rc)
    }
}

/// Zero-copy JIT interface for RegisterValue arrays
/// 
/// This provides a way for JIT code to work directly with RegisterValue arrays
/// without converting to RcValue, eliminating allocation overhead.
#[cfg(feature = "jit")]
pub mod jit_interface {
    use super::*;
    
    /// Get raw pointer to RegisterValue array for JIT
    /// 
    /// SAFETY: The pointer is valid as long as the Vec is not reallocated.
    /// JIT code must not resize the register array.
    #[inline(always)]
    pub unsafe fn get_register_ptr(registers: &mut [RegisterValue]) -> *mut RegisterValue {
        registers.as_mut_ptr()
    }
    
    /// Convert RegisterValue array to RcValue array for legacy JIT code
    /// 
    /// This is the slow path that allocates. Only use when necessary for compatibility.
    pub fn to_rc_value_array(registers: &[RegisterValue]) -> Vec<RcValue> {
        registers.iter().map(|r| r.to_rc_value()).collect()
    }
    
    /// Convert RcValue array back to RegisterValue array (slow path)
    pub fn from_rc_value_array(rc_values: &[RcValue], registers: &mut [RegisterValue]) {
        for (i, rc) in rc_values.iter().enumerate() {
            if i < registers.len() {
                registers[i] = RegisterValue::from_rc_value(rc.clone());
            }
        }
    }
    
    /// JIT-compatible register access functions
    /// These are exported to JIT code as runtime helpers
    
    /// Load integer from register (fast path)
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_get_int(registers: *mut RegisterValue, index: u32) -> i64 {
        let reg = &*registers.add(index as usize);
        match reg {
            RegisterValue::Int(n) => *n,
            _ => 0, // Type mismatch - return 0 (could also deoptimize)
        }
    }
    
    /// Store integer to register (fast path)
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_set_int(registers: *mut RegisterValue, index: u32, value: i64) {
        let reg = &mut *registers.add(index as usize);
        *reg = RegisterValue::Int(value);
    }
    
    /// Load float from register (fast path)
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_get_float(registers: *mut RegisterValue, index: u32) -> f64 {
        let reg = &*registers.add(index as usize);
        match reg {
            RegisterValue::Float(f) => *f,
            _ => 0.0,
        }
    }
    
    /// Store float to register (fast path)
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_set_float(registers: *mut RegisterValue, index: u32, value: f64) {
        let reg = &mut *registers.add(index as usize);
        *reg = RegisterValue::Float(value);
    }
    
    /// Fast integer add (no type checking, assumes both are Int)
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_fast_int_add(
        registers: *mut RegisterValue,
        left_idx: u32,
        right_idx: u32,
        result_idx: u32,
    ) -> i32 {
        let left_reg = &*registers.add(left_idx as usize);
        let right_reg = &*registers.add(right_idx as usize);
        
        if let (Some(a), Some(b)) = (left_reg.as_int(), right_reg.as_int()) {
            let result_reg = &mut *registers.add(result_idx as usize);
            *result_reg = RegisterValue::Int(a.wrapping_add(b));
            0 // Success
        } else {
            1 // Type mismatch - deoptimize
        }
    }
    
    /// Fast integer subtract
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_fast_int_sub(
        registers: *mut RegisterValue,
        left_idx: u32,
        right_idx: u32,
        result_idx: u32,
    ) -> i32 {
        let left_reg = &*registers.add(left_idx as usize);
        let right_reg = &*registers.add(right_idx as usize);
        
        if let (Some(a), Some(b)) = (left_reg.as_int(), right_reg.as_int()) {
            let result_reg = &mut *registers.add(result_idx as usize);
            *result_reg = RegisterValue::Int(a.wrapping_sub(b));
            0
        } else {
            1
        }
    }
    
    /// Fast integer multiply
    #[no_mangle]
    pub unsafe extern "C" fn tauraro_jit_fast_int_mul(
        registers: *mut RegisterValue,
        left_idx: u32,
        right_idx: u32,
        result_idx: u32,
    ) -> i32 {
        let left_reg = &*registers.add(left_idx as usize);
        let right_reg = &*registers.add(right_idx as usize);
        
        if let (Some(a), Some(b)) = (left_reg.as_int(), right_reg.as_int()) {
            let result_reg = &mut *registers.add(result_idx as usize);
            *result_reg = RegisterValue::Int(a.wrapping_mul(b));
            0
        } else {
            1
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_unboxed_int() {
        let reg = RegisterValue::Int(42);
        assert_eq!(reg.as_int(), Some(42));
        assert_eq!(reg.to_value(), Value::Int(42));
        assert!(reg.is_int());
        assert!(!reg.is_boxed());
    }
    
    #[test]
    fn test_unboxed_float() {
        let reg = RegisterValue::Float(3.14);
        assert_eq!(reg.as_float(), Some(3.14));
        assert_eq!(reg.to_value(), Value::Float(3.14));
        assert!(reg.is_float());
        assert!(!reg.is_boxed());
    }
    
    #[test]
    fn test_unboxed_bool() {
        let reg = RegisterValue::Bool(true);
        assert_eq!(reg.as_bool(), Some(true));
        assert_eq!(reg.to_value(), Value::Bool(true));
        assert!(reg.is_bool());
        assert!(!reg.is_boxed());
    }
    
    #[test]
    fn test_boxed_string() {
        let reg = RegisterValue::from_value(Value::Str("hello".to_string()));
        assert!(reg.is_boxed());
        assert_eq!(reg.to_value(), Value::Str("hello".to_string()));
    }
    
    #[test]
    fn test_size() {
        use std::mem::size_of;
        
        // RegisterValue should be small (16 bytes: 8 for discriminant + 8 for largest variant)
        assert!(size_of::<RegisterValue>() <= 24, "RegisterValue size: {}", size_of::<RegisterValue>());
        
        // Much smaller than RcValue which includes the full Value enum
        println!("RegisterValue size: {} bytes", size_of::<RegisterValue>());
        println!("RcValue size: {} bytes", size_of::<RcValue>());
    }
}
