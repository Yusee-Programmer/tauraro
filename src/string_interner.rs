//! String interning for performance optimization
//! Reduces memory allocations and speeds up string comparisons

use std::collections::HashMap;
use std::rc::Rc;

/// String interner that deduplicates strings for performance
pub struct StringInterner {
    /// Maps string content to its interned Rc<String>
    strings: HashMap<String, Rc<String>>,
    /// Statistics for monitoring
    pub total_interns: usize,
    pub cache_hits: usize,
}

impl StringInterner {
    pub fn new() -> Self {
        Self {
            strings: HashMap::new(),
            total_interns: 0,
            cache_hits: 0,
        }
    }

    /// Intern a string, returning an Rc<String>
    /// If the string is already interned, returns the existing Rc
    #[inline]
    pub fn intern(&mut self, s: &str) -> Rc<String> {
        self.total_interns += 1;

        if let Some(existing) = self.strings.get(s) {
            self.cache_hits += 1;
            return Rc::clone(existing);
        }

        let rc = Rc::new(s.to_string());
        self.strings.insert(s.to_string(), Rc::clone(&rc));
        rc
    }

    /// Intern a String, consuming it if it's new
    #[inline]
    pub fn intern_owned(&mut self, s: String) -> Rc<String> {
        self.total_interns += 1;

        if let Some(existing) = self.strings.get(&s) {
            self.cache_hits += 1;
            return Rc::clone(existing);
        }

        let rc = Rc::new(s.clone());
        self.strings.insert(s, Rc::clone(&rc));
        rc
    }

    /// Get the hit rate as a percentage
    pub fn hit_rate(&self) -> f64 {
        if self.total_interns == 0 {
            return 0.0;
        }
        (self.cache_hits as f64 / self.total_interns as f64) * 100.0
    }

    /// Clear the interner (useful for testing)
    pub fn clear(&mut self) {
        self.strings.clear();
        self.total_interns = 0;
        self.cache_hits = 0;
    }

    /// Get the number of unique strings
    pub fn unique_count(&self) -> usize {
        self.strings.len()
    }
}

impl Default for StringInterner {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_string_interning() {
        let mut interner = StringInterner::new();

        let s1 = interner.intern("hello");
        let s2 = interner.intern("hello");
        let s3 = interner.intern("world");

        // Same string should return same Rc
        assert!(Rc::ptr_eq(&s1, &s2));
        // Different strings should return different Rc
        assert!(!Rc::ptr_eq(&s1, &s3));

        assert_eq!(interner.unique_count(), 2);
        assert_eq!(interner.cache_hits, 1);
        assert_eq!(interner.total_interns, 3);
    }

    #[test]
    fn test_hit_rate() {
        let mut interner = StringInterner::new();

        interner.intern("a");
        interner.intern("a");
        interner.intern("a");
        interner.intern("b");

        // 3 hits out of 4 total = 75%
        assert_eq!(interner.hit_rate(), 50.0);
    }
}
