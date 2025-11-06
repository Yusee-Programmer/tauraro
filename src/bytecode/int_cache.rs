//! Small integer caching to avoid allocation for common values
//! Python caches integers from -5 to 256, we'll do the same

use crate::value::Value;
use crate::bytecode::objects::RcValue;
use std::cell::RefCell;

/// Range of cached integers (Python uses -5 to 256)
const CACHE_MIN: i64 = -5;
const CACHE_MAX: i64 = 256;
const CACHE_SIZE: usize = (CACHE_MAX - CACHE_MIN + 1) as usize;

/// Thread-local cache of small integers (VM is single-threaded)
thread_local! {
    static INT_CACHE: RefCell<Option<Vec<RcValue>>> = RefCell::new(None);
}

/// Initialize the integer cache
fn init_cache() -> Vec<RcValue> {
    let mut cache = Vec::with_capacity(CACHE_SIZE);
    for i in CACHE_MIN..=CACHE_MAX {
        cache.push(RcValue::new(Value::Int(i)));
    }
    cache
}

/// Get a cached integer if it's in range, otherwise create new
#[inline]
pub fn get_cached_int(val: i64) -> RcValue {
    if val >= CACHE_MIN && val <= CACHE_MAX {
        INT_CACHE.with(|cache| {
            let mut cache_ref = cache.borrow_mut();
            if cache_ref.is_none() {
                *cache_ref = Some(init_cache());
            }
            let cache_vec = cache_ref.as_ref().unwrap();
            let idx = (val - CACHE_MIN) as usize;
            cache_vec[idx].clone()
        })
    } else {
        RcValue::new(Value::Int(val))
    }
}

/// Fast path for integer addition with caching
#[inline]
pub fn fast_int_add(a: i64, b: i64) -> RcValue {
    let result = a.wrapping_add(b);
    get_cached_int(result)
}

/// Fast path for integer subtraction with caching
#[inline]
pub fn fast_int_sub(a: i64, b: i64) -> RcValue {
    let result = a.wrapping_sub(b);
    get_cached_int(result)
}

/// Fast path for integer multiplication with caching
#[inline]
pub fn fast_int_mul(a: i64, b: i64) -> RcValue {
    let result = a.wrapping_mul(b);
    get_cached_int(result)
}

/// Check if an integer is in cache range
#[inline]
pub fn is_cached_range(val: i64) -> bool {
    val >= CACHE_MIN && val <= CACHE_MAX
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_cache_range() {
        assert!(is_cached_range(0));
        assert!(is_cached_range(100));
        assert!(is_cached_range(-5));
        assert!(is_cached_range(256));
        assert!(!is_cached_range(-6));
        assert!(!is_cached_range(257));
    }

    #[test]
    fn test_cached_integers_are_same_object() {
        let a = get_cached_int(42);
        let b = get_cached_int(42);
        // Same cached object should have same pointer
        assert!(std::ptr::eq(&a.value, &b.value));
    }

    #[test]
    fn test_uncached_integers_are_different() {
        let a = get_cached_int(1000);
        let b = get_cached_int(1000);
        // Different objects for uncached integers
        assert!(!std::ptr::eq(&a.value, &b.value));
    }
}
