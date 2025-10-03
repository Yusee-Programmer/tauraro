//! High-Performance List Implementation for Tauraro
//! This module provides a high-performance list that maintains full Python list compatibility
//! while using Rust's efficient data structures internally.

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::hash::{Hash, Hasher};

/// High-performance list that wraps Vec for optimal performance
/// while maintaining Python list semantics
#[derive(Debug)]
pub struct HPList {
    /// Internal storage using Vec for maximum performance
    data: Vec<Value>,
}

impl HPList {
    /// Create a new empty high-performance list
    pub fn new() -> Self {
        Self {
            data: Vec::new(),
        }
    }

    /// Create a new high-performance list with initial capacity
    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: Vec::with_capacity(capacity),
        }
    }

    /// Create a new high-performance list from existing values
    pub fn from_values(values: Vec<Value>) -> Self {
        Self {
            data: values,
        }
    }

    /// Get the length of the list
    pub fn len(&self) -> usize {
        self.data.len()
    }

    /// Check if the list is empty
    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    /// Get an element at the specified index (Python-style negative indexing supported)
    pub fn get(&self, index: isize) -> Option<&Value> {
        self.normalize_index(index).and_then(|idx| self.data.get(idx))
    }

    /// Get a mutable reference to an element at the specified index
    pub fn get_mut(&mut self, index: isize) -> Option<&mut Value> {
        self.normalize_index(index).and_then(|idx| self.data.get_mut(idx))
    }

    /// Set an element at the specified index
    pub fn set(&mut self, index: isize, value: Value) -> Result<()> {
        let idx = self.normalize_index(index)
            .ok_or_else(|| anyhow!("list assignment index out of range"))?;
        self.data[idx] = value;
        Ok(())
    }

    /// Append an element to the end of the list (O(1) amortized)
    pub fn append(&mut self, value: Value) {
        self.data.push(value);
    }
    
    /// Alias for append to maintain compatibility with Vec interface
    pub fn push(&mut self, value: Value) {
        self.data.push(value);
    }

    /// Extend the list with elements from an iterator
    pub fn extend<I>(&mut self, iter: I) 
    where
        I: IntoIterator<Item = Value>,
    {
        self.data.extend(iter);
    }

    /// Insert an element at the specified index (O(n) in worst case)
    pub fn insert(&mut self, index: isize, value: Value) -> Result<()> {
        let len = self.data.len() as isize;
        let idx = if index < 0 {
            let normalized = len + index;
            if normalized < 0 { 0 } else { normalized as usize }
        } else if index > len {
            len as usize
        } else {
            index as usize
        };
        
        self.data.insert(idx, value);
        Ok(())
    }

    /// Remove and return the last element (O(1))
    pub fn pop(&mut self) -> Option<Value> {
        self.data.pop()
    }

    /// Remove and return the element at the specified index
    pub fn pop_at(&mut self, index: isize) -> Result<Value> {
        let idx = self.normalize_index(index)
            .ok_or_else(|| anyhow!("pop index out of range"))?;
        Ok(self.data.remove(idx))
    }

    /// Remove the first occurrence of a value
    pub fn remove(&mut self, value: &Value) -> Result<()> {
        let pos = self.data.iter().position(|x| x == value)
            .ok_or_else(|| anyhow!("list.remove(x): x not in list"))?;
        self.data.remove(pos);
        Ok(())
    }

    /// Get the index of the first occurrence of a value
    pub fn index(&self, value: &Value, start: Option<usize>, stop: Option<usize>) -> Result<usize> {
        let start_idx = start.unwrap_or(0);
        let stop_idx = stop.unwrap_or(self.data.len()).min(self.data.len());
        
        if start_idx >= self.data.len() {
            return Err(anyhow!("list.index(x): x not in list"));
        }
        
        for (i, item) in self.data[start_idx..stop_idx].iter().enumerate() {
            if item == value {
                return Ok(start_idx + i);
            }
        }
        
        Err(anyhow!("list.index(x): x not in list"))
    }

    /// Count the occurrences of a value
    pub fn count(&self, value: &Value) -> usize {
        self.data.iter().filter(|&x| x == value).count()
    }

    /// Reverse the list in place
    pub fn reverse(&mut self) {
        self.data.reverse();
    }

    /// Clear all elements from the list
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Sort the list in place using Python-style comparison
    pub fn sort(&mut self) {
        self.data.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
    }

    /// Create a shallow copy of the list
    pub fn copy(&self) -> Self {
        Self {
            data: self.data.clone(),
        }
    }

    /// Normalize a Python-style index to a Rust usize index
    fn normalize_index(&self, index: isize) -> Option<usize> {
        let len = self.data.len() as isize;
        if len == 0 {
            return None;
        }
        
        let idx = if index < 0 {
            len + index
        } else {
            index
        };
        
        if idx < 0 || idx >= len {
            None
        } else {
            Some(idx as usize)
        }
    }

    /// Get an iterator over the list elements
    pub fn iter(&self) -> impl Iterator<Item = &Value> {
        self.data.iter()
    }

    /// Get a mutable iterator over the list elements
    pub fn iter_mut(&mut self) -> impl Iterator<Item = &mut Value> {
        self.data.iter_mut()
    }

    /// Convert to a regular Vec<Value>
    pub fn to_vec(self) -> Vec<Value> {
        self.data
    }

    /// Convert to a reference Vec<Value>
    pub fn as_vec(&self) -> &Vec<Value> {
        &self.data
    }

    /// Get slice of the list
    pub fn slice(&self, start: Option<isize>, stop: Option<isize>, step: Option<isize>) -> Result<Vec<Value>> {
        let len = self.data.len() as isize;
        
        // Normalize start
        let start_idx = match start {
            Some(s) if s < 0 => {
                let normalized = len + s;
                if normalized < 0 { 0 } else { normalized as usize }
            },
            Some(s) if s > len => len as usize,
            Some(s) => s as usize,
            None => 0,
        };
        
        // Normalize stop
        let stop_idx = match stop {
            Some(s) if s < 0 => {
                let normalized = len + s;
                if normalized < 0 { 0 } else { normalized as usize }
            },
            Some(s) if s > len => len as usize,
            Some(s) => s as usize,
            None => len as usize,
        };
        
        // Handle step
        let step_val = step.unwrap_or(1);
        if step_val == 0 {
            return Err(anyhow!("slice step cannot be zero"));
        }
        
        let mut result = Vec::new();
        
        if step_val > 0 {
            let mut idx = start_idx;
            while idx < stop_idx && idx < self.data.len() {
                result.push(self.data[idx].clone());
                idx += step_val as usize;
            }
        } else {
            // Negative step
            let mut idx = if start_idx >= self.data.len() {
                self.data.len().saturating_sub(1)
            } else {
                start_idx
            };
            
            let stop_idx = if stop_idx > self.data.len() {
                0
            } else {
                stop_idx
            };
            
            while idx >= stop_idx && idx < self.data.len() {
                result.push(self.data[idx].clone());
                if idx < (-step_val) as usize {
                    break;
                }
                idx -= (-step_val) as usize;
            }
        }
        
        Ok(result)
    }
}

// Implement Hash trait for HPList
impl Hash for HPList {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.data.hash(state);
    }
}

// Implement IntoIterator for HPList
impl IntoIterator for HPList {
    type Item = Value;
    type IntoIter = std::vec::IntoIter<Value>;
    
    fn into_iter(self) -> Self::IntoIter {
        self.data.into_iter()
    }
}

// Implement IntoIterator for &HPList
impl<'a> IntoIterator for &'a HPList {
    type Item = &'a Value;
    type IntoIter = std::slice::Iter<'a, Value>;
    
    fn into_iter(self) -> Self::IntoIter {
        self.data.iter()
    }
}

// Implement Clone for HPList
impl Clone for HPList {
    fn clone(&self) -> Self {
        HPList {
            data: self.data.clone(),
        }
    }
}

// Implement PartialEq for HPList
impl PartialEq for HPList {
    fn eq(&self, other: &Self) -> bool {
        self.data == other.data
    }
}

impl std::fmt::Display for HPList {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let items: Vec<String> = self.data.iter().map(|v| format!("{}", v)).collect();
        write!(f, "[{}]", items.join(", "))
    }
}

/// Create the high-performance list module
pub fn create_hplist_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Add high-performance list constructor
    namespace.insert("HPList".to_string(), Value::NativeFunction(|args| {
        if args.is_empty() {
            let hplist = HPList::new();
            Ok(Value::List(hplist))
        } else if args.len() == 1 {
            // Convert iterable to HPList
            match &args[0] {
                Value::List(items) => Ok(Value::List(items.clone())),
                Value::Tuple(items) => {
                    let mut hplist = HPList::new();
                    for item in items {
                        hplist.append(item.clone());
                    }
                    Ok(Value::List(hplist))
                },
                Value::Str(s) => {
                    let mut hplist = HPList::new();
                    for c in s.chars() {
                        hplist.append(Value::Str(c.to_string()));
                    }
                    Ok(Value::List(hplist))
                },
                _ => Err(anyhow!("'{}' object is not iterable", args[0].type_name())),
            }
        } else {
            Err(anyhow!("HPList() takes at most 1 argument ({} given)", args.len()))
        }
    }));
    
    Value::Module("hplist".to_string(), namespace)
}