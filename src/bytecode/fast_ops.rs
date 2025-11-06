//! Ultra-fast operations that bypass error handling entirely
//! These are used in FastInt opcodes where we KNOW the types are correct

use crate::value::Value;
use crate::bytecode::objects::RcValue;

/// Direct integer addition with NO error handling
/// SAFETY: Caller must ensure both values are Int
#[inline(always)]
pub unsafe fn unchecked_int_add(left: &Value, right: &Value) -> Value {
    match (left, right) {
        (Value::Int(a), Value::Int(b)) => Value::Int(a.wrapping_add(*b)),
        _ => std::hint::unreachable_unchecked(), // UB if not integers!
    }
}

/// Direct integer subtraction with NO error handling
/// SAFETY: Caller must ensure both values are Int
#[inline(always)]
pub unsafe fn unchecked_int_sub(left: &Value, right: &Value) -> Value {
    match (left, right) {
        (Value::Int(a), Value::Int(b)) => Value::Int(a.wrapping_sub(*b)),
        _ => std::hint::unreachable_unchecked(),
    }
}

/// Direct integer multiplication with NO error handling
/// SAFETY: Caller must ensure both values are Int
#[inline(always)]
pub unsafe fn unchecked_int_mul(left: &Value, right: &Value) -> Value {
    match (left, right) {
        (Value::Int(a), Value::Int(b)) => Value::Int(a.wrapping_mul(*b)),
        _ => std::hint::unreachable_unchecked(),
    }
}

/// Extract integer value directly
/// SAFETY: Caller must ensure value is Int
#[inline(always)]
pub unsafe fn extract_int(val: &Value) -> i64 {
    match val {
        Value::Int(n) => *n,
        _ => std::hint::unreachable_unchecked(),
    }
}

/// Create integer value directly
#[inline(always)]
pub fn make_int(val: i64) -> Value {
    Value::Int(val)
}

/// Fast integer addition that avoids ALL allocations
#[inline(always)]
pub fn fast_add_ints(a: i64, b: i64) -> i64 {
    a.wrapping_add(b)
}

/// Fast integer subtraction that avoids ALL allocations
#[inline(always)]
pub fn fast_sub_ints(a: i64, b: i64) -> i64 {
    a.wrapping_sub(b)
}

/// Fast integer multiplication that avoids ALL allocations
#[inline(always)]
pub fn fast_mul_ints(a: i64, b: i64) -> i64 {
    a.wrapping_mul(b)
}
