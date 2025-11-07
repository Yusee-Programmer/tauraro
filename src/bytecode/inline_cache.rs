//! Inline caching for VM operations to accelerate repeated operations
//! This is a critical performance optimization that caches type information
//! and avoids expensive type checks on hot paths

use crate::value::Value;

/// Type combination for caching - extended to support more operations
#[derive(Copy, Clone, PartialEq, Eq, Hash, Debug)]
pub enum TypePair {
    IntInt,
    FloatFloat,
    IntFloat,
    FloatInt,
    StrStr,
    ListList,
    BoolBool,
    Other,
}

impl TypePair {
    #[inline(always)]
    pub fn from_values(left: &Value, right: &Value) -> Self {
        match (left, right) {
            (Value::Int(_), Value::Int(_)) => TypePair::IntInt,
            (Value::Float(_), Value::Float(_)) => TypePair::FloatFloat,
            (Value::Int(_), Value::Float(_)) => TypePair::IntFloat,
            (Value::Float(_), Value::Int(_)) => TypePair::FloatInt,
            (Value::Str(_), Value::Str(_)) => TypePair::StrStr,
            (Value::List(_), Value::List(_)) => TypePair::ListList,
            (Value::Bool(_), Value::Bool(_)) => TypePair::BoolBool,
            _ => TypePair::Other,
        }
    }
}

/// Inline cache entry for a single operation site
#[derive(Clone)]
pub struct InlineCache {
    pub cached_type_pair: TypePair,
    pub hit_count: u32,
}

impl InlineCache {
    #[inline]
    pub fn new() -> Self {
        Self {
            cached_type_pair: TypePair::Other,
            hit_count: 0,
        }
    }

    #[inline]
    pub fn check(&mut self, left: &Value, right: &Value) -> bool {
        let current_pair = TypePair::from_values(left, right);
        if current_pair == self.cached_type_pair {
            self.hit_count += 1;
            true
        } else {
            // Cache miss - update cache
            self.cached_type_pair = current_pair;
            self.hit_count = 1;
            false
        }
    }

    #[inline]
    pub fn get_cached_type(&self) -> TypePair {
        self.cached_type_pair
    }
}

/// Cached addition operation
#[inline]
pub fn cached_add(cache: &mut InlineCache, left: &Value, right: &Value) -> Option<Value> {
    let type_pair = cache.get_cached_type();

    // Fast path: types match cache
    match type_pair {
        TypePair::IntInt => {
            if let (Value::Int(a), Value::Int(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Int(a + b));
            }
        }
        TypePair::FloatFloat => {
            if let (Value::Float(a), Value::Float(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Float(a + b));
            }
        }
        TypePair::StrStr => {
            if let (Value::Str(a), Value::Str(b)) = (left, right) {
                cache.hit_count += 1;
                let mut s = String::with_capacity(a.len() + b.len());
                s.push_str(a);
                s.push_str(b);
                return Some(Value::Str(s));
            }
        }
        _ => {}
    }

    // Cache miss - update cache and fall back to slow path
    cache.cached_type_pair = TypePair::from_values(left, right);
    cache.hit_count = 1;
    None
}

/// Cached subtraction operation
#[inline]
pub fn cached_sub(cache: &mut InlineCache, left: &Value, right: &Value) -> Option<Value> {
    let type_pair = cache.get_cached_type();

    match type_pair {
        TypePair::IntInt => {
            if let (Value::Int(a), Value::Int(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Int(a - b));
            }
        }
        TypePair::FloatFloat => {
            if let (Value::Float(a), Value::Float(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Float(a - b));
            }
        }
        _ => {}
    }

    cache.cached_type_pair = TypePair::from_values(left, right);
    cache.hit_count = 1;
    None
}

/// Cached multiplication operation
#[inline]
pub fn cached_mul(cache: &mut InlineCache, left: &Value, right: &Value) -> Option<Value> {
    let type_pair = cache.get_cached_type();

    match type_pair {
        TypePair::IntInt => {
            if let (Value::Int(a), Value::Int(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Int(a * b));
            }
        }
        TypePair::FloatFloat => {
            if let (Value::Float(a), Value::Float(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Float(a * b));
            }
        }
        _ => {}
    }

    cache.cached_type_pair = TypePair::from_values(left, right);
    cache.hit_count = 1;
    None
}

/// Cached comparison (less than) operation
#[inline]
pub fn cached_compare_lt(cache: &mut InlineCache, left: &Value, right: &Value) -> Option<Value> {
    let type_pair = cache.get_cached_type();

    match type_pair {
        TypePair::IntInt => {
            if let (Value::Int(a), Value::Int(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a < b));
            }
        }
        TypePair::FloatFloat => {
            if let (Value::Float(a), Value::Float(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a < b));
            }
        }
        TypePair::StrStr => {
            if let (Value::Str(a), Value::Str(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a < b));
            }
        }
        _ => {}
    }

    cache.cached_type_pair = TypePair::from_values(left, right);
    cache.hit_count = 1;
    None
}

/// Cached equality comparison operation
#[inline]
pub fn cached_compare_eq(cache: &mut InlineCache, left: &Value, right: &Value) -> Option<Value> {
    let type_pair = cache.get_cached_type();

    match type_pair {
        TypePair::IntInt => {
            if let (Value::Int(a), Value::Int(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a == b));
            }
        }
        TypePair::FloatFloat => {
            if let (Value::Float(a), Value::Float(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a == b));
            }
        }
        TypePair::StrStr => {
            if let (Value::Str(a), Value::Str(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a == b));
            }
        }
        TypePair::BoolBool => {
            if let (Value::Bool(a), Value::Bool(b)) = (left, right) {
                cache.hit_count += 1;
                return Some(Value::Bool(a == b));
            }
        }
        _ => {}
    }

    cache.cached_type_pair = TypePair::from_values(left, right);
    cache.hit_count = 1;
    None
}
