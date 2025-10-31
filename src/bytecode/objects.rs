//! Object handling (dict, list, string, etc.)

use crate::value::Value;
use std::fmt::Debug;

/// Reference counted value for optimized memory management
#[derive(Debug, PartialEq, Eq, Hash)]
pub struct RcValue {
    pub value: Value,
    pub ref_count: usize,
}

/// Simple iterator for Range values
#[derive(Debug, Clone)]
pub struct RangeIterator {
    pub start: i64,
    pub stop: i64,
    pub step: i64,
    pub current: i64,
}


// Remove the Hash implementation since Value doesn't implement Hash properly
// impl Hash for RcValue {
//     fn hash<H: Hasher>(&self, state: &mut H) {
//         self.value.hash(state);
//     }
// }

impl RcValue {
    pub fn new(value: Value) -> Self {
        Self {
            value,
            ref_count: 1,
        }
    }
    
    pub fn clone_rc(&self) -> Self {
        Self {
            value: self.value.clone(),
            ref_count: self.ref_count + 1,
        }
    }
    
    pub fn is_unique(&self) -> bool {
        self.ref_count == 1
    }
    
    pub fn is_truthy(&self) -> bool {
        self.value.is_truthy()
    }
}

impl Clone for RcValue {
    fn clone(&self) -> Self {
        Self {
            value: self.value.clone(),
            ref_count: self.ref_count + 1,
        }
    }
}



/// Object handling operations
pub struct ObjectOps;

// Object-related opcodes are already defined in instructions.rs
// The implementation for these opcodes is in arithmetic.rs in the execute_instruction_fast method