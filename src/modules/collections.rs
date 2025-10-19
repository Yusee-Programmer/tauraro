/// Collections module - provides specialized container datatypes
/// Similar to Python's collections module

use crate::value::Value;
use std::collections::{HashMap, VecDeque};
use std::rc::Rc;
use std::fmt;
use anyhow::Result;

/// High-performance list implementation that maintains Python list semantics
/// Uses VecDeque internally for better performance with insertions/deletions
#[derive(Clone, PartialEq)]
pub struct HighPerfList {
    /// Internal storage using VecDeque for O(1) operations at both ends
    data: VecDeque<Value>,
}

impl HighPerfList {
    /// Create a new empty high-performance list
    pub fn new() -> Self {
        Self {
            data: VecDeque::new(),
        }
    }

    /// Create a new high-performance list with initial capacity
    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            data: VecDeque::with_capacity(capacity),
        }
    }

    /// Create a new high-performance list from existing values
    pub fn from_values(values: Vec<Value>) -> Self {
        Self {
            data: VecDeque::from(values),
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

    /// Get an element at the specified index
    pub fn get(&self, index: isize) -> Option<&Value> {
        let idx = self.normalize_index(index)?;
        self.data.get(idx)
    }

    /// Get a mutable reference to an element at the specified index
    pub fn get_mut(&mut self, index: isize) -> Option<&mut Value> {
        let idx = self.normalize_index(index)?;
        self.data.get_mut(idx)
    }

    /// Set an element at the specified index
    pub fn set(&mut self, index: isize, value: Value) -> Result<()> {
        let idx = self.normalize_index(index)
            .ok_or_else(|| anyhow::anyhow!("list assignment index out of range"))?;
        *self.data.get_mut(idx).unwrap() = value;
        Ok(())
    }

    /// Append an element to the end of the list
    pub fn append(&mut self, value: Value) {
        self.data.push_back(value);
    }

    /// Extend the list with elements from an iterator
    pub fn extend<I>(&mut self, iter: I) 
    where
        I: IntoIterator<Item = Value>,
    {
        self.data.extend(iter);
    }

    /// Insert an element at the specified index
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

    /// Remove and return the last element
    pub fn pop(&mut self) -> Option<Value> {
        self.data.pop_back()
    }

    /// Remove and return the element at the specified index
    pub fn pop_at(&mut self, index: isize) -> Result<Value> {
        let idx = self.normalize_index(index)
            .ok_or_else(|| anyhow::anyhow!("pop index out of range"))?;
        Ok(self.data.remove(idx).unwrap())
    }

    /// Remove the first occurrence of a value
    pub fn remove(&mut self, value: &Value) -> Result<()> {
        let pos = self.data.iter().position(|x| x == value)
            .ok_or_else(|| anyhow::anyhow!("list.remove(x): x not in list"))?;
        self.data.remove(pos);
        Ok(())
    }

    /// Get the index of the first occurrence of a value
    pub fn index(&self, value: &Value) -> Result<usize> {
        self.data.iter().position(|x| x == value)
            .ok_or_else(|| anyhow::anyhow!("list.index(x): x not in list"))
    }

    /// Count the occurrences of a value
    pub fn count(&self, value: &Value) -> usize {
        self.data.iter().filter(|&x| x == value).count()
    }

    /// Reverse the list in place
    pub fn reverse(&mut self) {
        self.data.make_contiguous().reverse();
    }

    /// Clear all elements from the list
    pub fn clear(&mut self) {
        self.data.clear();
    }

    /// Sort the list in place
    pub fn sort(&mut self) {
        let mut vec: Vec<Value> = self.data.drain(..).collect();
        vec.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
        self.data.extend(vec);
    }

    /// Create a copy of the list
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
    pub fn to_vec(&self) -> Vec<Value> {
        self.data.iter().cloned().collect()
    }
}

impl fmt::Debug for HighPerfList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "HighPerfList({:?})", self.data)
    }
}

impl fmt::Display for HighPerfList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let items: Vec<String> = self.data.iter().map(|v| format!("{}", v)).collect();
        write!(f, "[{}]", items.join(", "))
    }
}

/// Create the collections module with high-performance list support
pub fn create_collections_module() -> Value {
    let mut namespace = HashMap::new();
    
    // Add high-performance list constructor
    namespace.insert("HighPerfList".to_string(), Value::NativeFunction(|args| {
        if args.is_empty() {
            Ok(Value::Object {
                class_name: "HighPerfList".to_string(),
                fields: Rc::new(HashMap::new()),
                class_methods: HashMap::new(),
                base_object: crate::base_object::BaseObject::new("HighPerfList".to_string(), vec!["object".to_string()]),
                mro: crate::base_object::MRO::from_linearization(vec!["HighPerfList".to_string(), "object".to_string()]),
            })
        } else {
            Err(anyhow::anyhow!("HighPerfList constructor not implemented with arguments"))
        }
    }));
    
    Value::Module("collections".to_string(), namespace)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basic_operations() {
        let mut list = HighPerfList::new();
        assert_eq!(list.len(), 0);
        assert!(list.is_empty());

        list.append(Value::Int(1));
        list.append(Value::Int(2));
        list.append(Value::Int(3));
        
        assert_eq!(list.len(), 3);
        assert!(!list.is_empty());
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(2)));
        assert_eq!(list.get(2), Some(&Value::Int(3)));
    }

    #[test]
    fn test_index_normalization() {
        let mut list = HighPerfList::new();
        list.append(Value::Int(10));
        list.append(Value::Int(20));
        list.append(Value::Int(30));
        
        assert_eq!(list.get(0), Some(&Value::Int(10)));
        assert_eq!(list.get(-1), Some(&Value::Int(30)));
        assert_eq!(list.get(-3), Some(&Value::Int(10)));
        assert_eq!(list.get(3), None);
        assert_eq!(list.get(-4), None);
    }

    #[test]
    fn test_insert_and_remove() {
        let mut list = HighPerfList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(3));
        
        // Insert in the middle
        list.insert(1, Value::Int(2)).unwrap();
        assert_eq!(list.len(), 3);
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(2)));
        assert_eq!(list.get(2), Some(&Value::Int(3)));
        
        // Remove from middle
        list.remove(&Value::Int(2)).unwrap();
        assert_eq!(list.len(), 2);
        assert_eq!(list.get(0), Some(&Value::Int(1)));
        assert_eq!(list.get(1), Some(&Value::Int(3)));
    }

    #[test]
    fn test_pop_operations() {
        let mut list = HighPerfList::new();
        list.append(Value::Int(1));
        list.append(Value::Int(2));
        list.append(Value::Int(3));
        
        // Pop from end
        assert_eq!(list.pop(), Some(Value::Int(3)));
        assert_eq!(list.len(), 2);
        
        // Pop from specific index
        assert_eq!(list.pop_at(0).unwrap(), Value::Int(1));
        assert_eq!(list.len(), 1);
        assert_eq!(list.get(0), Some(&Value::Int(2)));
    }
}