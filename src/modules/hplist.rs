//! High-Performance List Implementation for Tauraro
//! This module provides a high-performance list that maintains full Python list compatibility
//! while using Rust's efficient data structures internally.

use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::hash::{Hash, Hasher};
use std::rc::Rc;
use std::cell::RefCell;

/// High-performance list that wraps Vec for optimal performance
/// while maintaining Python list semantics with reference semantics like Python
#[derive(Debug, Clone)]
pub struct HPList {
    /// Internal storage using Rc<RefCell<Vec>> for Python-like reference semantics
    pub(crate) data: Rc<RefCell<Vec<Value>>>, 
}

impl HPList {
    /// Create a new empty high-performance list
    pub fn new() -> Self {
        Self {
            data: Rc::new(RefCell::new(Vec::new())),
        }
    }

    /// Create a new high-performance list with initial capacity
    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: Rc::new(RefCell::new(Vec::with_capacity(capacity))),
        }
    }

    /// Create a new high-performance list from existing values
    pub fn from_values(values: Vec<Value>) -> Self {
        Self {
            data: Rc::new(RefCell::new(values)),
        }
    }

    /// Get the length of the list
    pub fn len(&self) -> usize {
        self.data.borrow().len()
    }

    /// Check if the list is empty
    pub fn is_empty(&self) -> bool {
        self.data.borrow().is_empty()
    }

    /// Get an element at the specified index (Python-style negative indexing supported)
    pub fn get(&self, index: isize) -> Option<Value> {
        let data = self.data.borrow();
        self.normalize_index_with_len(index, data.len()).and_then(|idx| data.get(idx).cloned())
    }

    /// Get a mutable reference to an element at the specified index
    pub fn get_mut(&self, index: isize) -> Option<Value> {
        self.get(index)
    }

    /// Set an element at the specified index
    pub fn set(&self, index: isize, value: Value) -> Result<()> {
        let mut data = self.data.borrow_mut();
        let idx = self.normalize_index_with_len(index, data.len())
            .ok_or_else(|| anyhow!("list assignment index out of range"))?;
        data[idx] = value;
        Ok(())
    }

    /// Append an element to the end of the list (O(1) amortized)
    pub fn append(&self, value: Value) {
        self.data.borrow_mut().push(value);
    }

    /// Alias for append to maintain compatibility with Vec interface
    pub fn push(&self, value: Value) {
        self.data.borrow_mut().push(value);
    }

    /// Extend the list with elements from an iterator
    pub fn extend<I>(&self, iter: I)
    where
        I: IntoIterator<Item = Value>,
    {
        self.data.borrow_mut().extend(iter);
    }

    /// Insert an element at the specified index (O(n) in worst case)
    pub fn insert(&self, index: isize, value: Value) -> Result<()> {
        let mut data = self.data.borrow_mut();
        let len = data.len() as isize;
        let idx = if index < 0 {
            let normalized = len + index;
            if normalized < 0 { 0 } else { normalized as usize }
        } else if index > len {
            len as usize
        } else {
            index as usize
        };

        data.insert(idx, value);
        Ok(())
    }

    /// Remove and return the last element (O(1))
    pub fn pop(&self) -> Option<Value> {
        self.data.borrow_mut().pop()
    }

    /// Remove and return the element at the specified index
    pub fn pop_at(&self, index: isize) -> Result<Value> {
        let mut data = self.data.borrow_mut();
        let len = data.len();
        let idx = self.normalize_index_with_len(index, len)
            .ok_or_else(|| anyhow!("pop index out of range"))?;
        Ok(data.remove(idx))
    }

    /// Remove the first occurrence of a value
    pub fn remove(&self, value: &Value) -> Result<()> {
        let mut data = self.data.borrow_mut();
        let pos = data.iter().position(|x| x == value)
            .ok_or_else(|| anyhow!("list.remove(x): x not in list"))?;
        data.remove(pos);
        Ok(())
    }

    /// Get the index of the first occurrence of a value
    pub fn index(&self, value: &Value, start: Option<usize>, stop: Option<usize>) -> Result<usize> {
        let data = self.data.borrow();
        let start_idx = start.unwrap_or(0);
        let stop_idx = stop.unwrap_or(data.len()).min(data.len());

        if start_idx >= data.len() {
            return Err(anyhow!("list.index(x): x not in list"));
        }

        for (i, item) in data[start_idx..stop_idx].iter().enumerate() {
            if item == value {
                return Ok(start_idx + i);
            }
        }

        Err(anyhow!("list.index(x): x not in list"))
    }

    /// Count the occurrences of a value
    pub fn count(&self, value: &Value) -> usize {
        self.data.borrow().iter().filter(|&x| x == value).count()
    }

    /// Reverse the list in place
    pub fn reverse(&self) {
        self.data.borrow_mut().reverse();
    }

    /// Clear all elements from the list
    pub fn clear(&self) {
        self.data.borrow_mut().clear();
    }

    /// Sort the list in place using Python-style comparison
    pub fn sort(&self) {
        self.data.borrow_mut().sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
    }

    /// Create a shallow copy of the list
    pub fn copy(&self) -> Self {
        Self {
            data: Rc::new(RefCell::new(self.data.borrow().clone())),
        }
    }

    /// Normalize a Python-style index to a Rust usize index (helper without borrowing)
    fn normalize_index_with_len(&self, index: isize, len: usize) -> Option<usize> {
        let len = len as isize;
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

    /// Get an iterator over cloned values (necessary due to RefCell borrowing)
    pub fn iter(&self) -> std::vec::IntoIter<Value> {
        self.data.borrow().clone().into_iter()
    }

    /// Convert to a regular Vec<Value>
    pub fn to_vec(&self) -> Vec<Value> {
        self.data.borrow().clone()
    }

    /// Get a copy of the internal Vec
    pub fn as_vec(&self) -> Vec<Value> {
        self.data.borrow().clone()
    }

    /// Get a reference to the internal Rc for identity comparison
    pub fn data_ptr(&self) -> &Rc<RefCell<Vec<Value>>> {
        &self.data
    }

    /// Get slice of the list
    pub fn slice(&self, start: Option<isize>, stop: Option<isize>, step: Option<isize>) -> Result<Vec<Value>> {
        let data = self.data.borrow();
        let len = data.len() as isize;
        
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
            while idx < stop_idx && idx < data.len() {
                result.push(data[idx].clone());
                idx += step_val as usize;
            }
        } else {
            // Negative step
            let mut idx = if start_idx >= data.len() {
                data.len().saturating_sub(1)
            } else {
                start_idx
            };

            let stop_idx = if stop_idx > data.len() {
                0
            } else {
                stop_idx
            };

            while idx >= stop_idx && idx < data.len() {
                result.push(data[idx].clone());
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
        // Hash based on contents
        for item in self.data.borrow().iter() {
            // Note: Value must implement Hash for this to work properly
            // For now we'll just hash the type_name
            item.type_name().hash(state);
        }
    }
}

// Implement IntoIterator for HPList
impl IntoIterator for HPList {
    type Item = Value;
    type IntoIter = std::vec::IntoIter<Value>;

    fn into_iter(self) -> Self::IntoIter {
        // Get ownership of the inner Vec
        let vec = match Rc::try_unwrap(self.data) {
            Ok(refcell) => refcell.into_inner(),
            Err(rc) => rc.borrow().clone(),
        };
        vec.into_iter()
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
        let items: Vec<String> = self.data.borrow().iter().map(|v| format!("{}", v)).collect();
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
