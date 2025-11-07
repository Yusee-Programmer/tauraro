//! Bridge module for gradual migration from Value to TaggedValue
//!
//! This module provides conversion utilities to enable gradual migration
//! from the old Value enum to the new TaggedValue system.

use crate::value::Value;
use crate::tagged_value::TaggedValue;

/// Convert Value to TaggedValue (fast path for common types)
#[inline]
pub fn value_to_tagged(value: &Value) -> Option<TaggedValue> {
    match value {
        Value::Int(n) => Some(TaggedValue::new_int(*n)),
        Value::Bool(b) => Some(TaggedValue::new_bool(*b)),
        Value::None => Some(TaggedValue::new_none()),
        Value::Float(f) => Some(TaggedValue::new_float(*f)),
        _ => None, // Complex types stay as Value for now
    }
}

/// Convert TaggedValue to Value (for compatibility)
#[inline]
pub fn tagged_to_value(tagged: &TaggedValue) -> Value {
    if let Some(n) = tagged.as_int() {
        Value::Int(n)
    } else if let Some(b) = tagged.as_bool() {
        Value::Bool(b)
    } else if tagged.is_none() {
        Value::None
    } else if let Some(f) = tagged.as_float() {
        Value::Float(f)
    } else {
        // Heap object - not yet implemented
        Value::None
    }
}

/// Hybrid value that can be either tagged or full Value
/// This enables gradual migration
#[derive(Clone)]
pub enum HybridValue {
    Tagged(TaggedValue),
    Full(Value),
}

impl HybridValue {
    /// Create from integer (uses tagged fast path)
    #[inline]
    pub fn from_int(n: i64) -> Self {
        HybridValue::Tagged(TaggedValue::new_int(n))
    }

    /// Create from bool (uses tagged fast path)
    #[inline]
    pub fn from_bool(b: bool) -> Self {
        HybridValue::Tagged(TaggedValue::new_bool(b))
    }

    /// Create from Value (tries tagged first)
    #[inline]
    pub fn from_value(value: Value) -> Self {
        if let Some(tagged) = value_to_tagged(&value) {
            HybridValue::Tagged(tagged)
        } else {
            HybridValue::Full(value)
        }
    }

    /// Try to get as integer (fast path)
    #[inline]
    pub fn as_int(&self) -> Option<i64> {
        match self {
            HybridValue::Tagged(t) => t.as_int(),
            HybridValue::Full(Value::Int(n)) => Some(*n),
            _ => None,
        }
    }

    /// Try to get as bool (fast path)
    #[inline]
    pub fn as_bool(&self) -> Option<bool> {
        match self {
            HybridValue::Tagged(t) => t.as_bool(),
            HybridValue::Full(Value::Bool(b)) => Some(*b),
            _ => None,
        }
    }

    /// Convert to full Value
    pub fn to_value(&self) -> Value {
        match self {
            HybridValue::Tagged(t) => tagged_to_value(t),
            HybridValue::Full(v) => v.clone(),
        }
    }

    /// Fast addition (uses tagged fast path if possible)
    #[inline]
    pub fn fast_add(&self, other: &HybridValue) -> Option<HybridValue> {
        match (self, other) {
            (HybridValue::Tagged(a), HybridValue::Tagged(b)) => {
                a.add(b).map(HybridValue::Tagged)
            }
            _ => None, // Fall back to slow path
        }
    }

    /// Fast subtraction (uses tagged fast path if possible)
    #[inline]
    pub fn fast_sub(&self, other: &HybridValue) -> Option<HybridValue> {
        match (self, other) {
            (HybridValue::Tagged(a), HybridValue::Tagged(b)) => {
                a.sub(b).map(HybridValue::Tagged)
            }
            _ => None,
        }
    }

    /// Fast multiplication (uses tagged fast path if possible)
    #[inline]
    pub fn fast_mul(&self, other: &HybridValue) -> Option<HybridValue> {
        match (self, other) {
            (HybridValue::Tagged(a), HybridValue::Tagged(b)) => {
                a.mul(b).map(HybridValue::Tagged)
            }
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_value_to_tagged_int() {
        let v = Value::Int(42);
        let t = value_to_tagged(&v).unwrap();
        assert_eq!(t.as_int(), Some(42));
    }

    #[test]
    fn test_tagged_to_value_int() {
        let t = TaggedValue::new_int(42);
        let v = tagged_to_value(&t);
        assert_eq!(v, Value::Int(42));
    }

    #[test]
    fn test_hybrid_fast_add() {
        let a = HybridValue::from_int(10);
        let b = HybridValue::from_int(32);
        let c = a.fast_add(&b).unwrap();
        assert_eq!(c.as_int(), Some(42));
    }

    #[test]
    fn test_hybrid_from_value() {
        let v = Value::Int(42);
        let h = HybridValue::from_value(v);
        assert_eq!(h.as_int(), Some(42));
    }
}
