/// Collections module - provides specialized container datatypes
/// Similar to Python's collections module

use crate::value::Value;
use crate::modules::hplist::HPList;
use std::collections::{HashMap, VecDeque};
use std::rc::Rc;
use std::cell::RefCell;
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

    // Add Counter class
    namespace.insert("Counter".to_string(), Value::BuiltinFunction("Counter".to_string(), counter_builtin));

    // Add defaultdict class
    namespace.insert("defaultdict".to_string(), Value::BuiltinFunction("defaultdict".to_string(), defaultdict_builtin));

    // Add deque class
    namespace.insert("deque".to_string(), Value::BuiltinFunction("deque".to_string(), deque_builtin));

    Value::Module("collections".to_string(), namespace)
}

/// Counter implementation
fn counter_builtin(args: Vec<Value>) -> Result<Value> {
    // Counter() or Counter(iterable) or Counter(mapping)
    let mut counts: HashMap<String, Value> = HashMap::new();

    if !args.is_empty() {
        // Initialize from iterable or mapping
        match &args[0] {
            Value::List(items) => {
                // Count items in list
                for item in items {
                    let key = format!("{}", item);
                    let count = counts.entry(key.clone()).or_insert(Value::Int(0));
                    if let Value::Int(n) = count {
                        *count = Value::Int(*n + 1);
                    }
                }
            }
            Value::Dict(map) => {
                // Initialize from dict
                for (k, v) in map.borrow().iter() {
                    counts.insert(k.clone(), v.clone());
                }
            }
            Value::Str(s) => {
                // Count characters in string
                for ch in s.chars() {
                    let key = ch.to_string();
                    let count = counts.entry(key.clone()).or_insert(Value::Int(0));
                    if let Value::Int(n) = count {
                        *count = Value::Int(*n + 1);
                    }
                }
            }
            _ => {}
        }
    }

    // Add methods to Counter class
    let mut class_methods = HashMap::new();
    class_methods.insert("most_common".to_string(), Value::NativeFunction(counter_most_common));
    class_methods.insert("elements".to_string(), Value::NativeFunction(counter_elements));
    class_methods.insert("subtract".to_string(), Value::NativeFunction(counter_subtract));
    class_methods.insert("update".to_string(), Value::NativeFunction(counter_update));
    class_methods.insert("__add__".to_string(), Value::NativeFunction(counter_add));
    class_methods.insert("__sub__".to_string(), Value::NativeFunction(counter_subtract_op));
    class_methods.insert("__or__".to_string(), Value::NativeFunction(counter_or));
    class_methods.insert("__and__".to_string(), Value::NativeFunction(counter_and));

    Ok(Value::Object {
        class_name: "Counter".to_string(),
        fields: Rc::new(counts),
        class_methods,
        base_object: crate::base_object::BaseObject::new("Counter".to_string(), vec!["dict".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["Counter".to_string(), "dict".to_string(), "object".to_string()]),
    })
}

/// most_common method for Counter
fn counter_most_common(args: Vec<Value>) -> Result<Value> {
    if args.len() < 1 || args.len() > 2 {
        return Err(anyhow::anyhow!("most_common() takes 1 or 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    
    // Second argument is n (optional)
    let n = if args.len() == 2 {
        match &args[1] {
            Value::Int(i) => *i as usize,
            Value::None => usize::MAX, // None means return all
            _ => return Err(anyhow::anyhow!("most_common() argument must be an integer or None")),
        }
    } else {
        0 // Default is return all
    };

    // Get the fields from the Counter object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid Counter object")),
    };

    // Convert to vector of (key, count) pairs
    let mut items: Vec<(String, i64)> = Vec::new();
    for (key, value) in fields.iter() {
        if let Value::Int(count) = value {
            items.push((key.clone(), *count));
        }
    }

    // Sort by count in descending order
    items.sort_by(|a, b| b.1.cmp(&a.1));

    // Take the first n items or all if n is 0
    let result_items = if n > 0 && n < items.len() {
        &items[..n]
    } else {
        &items[..]
    };

    // Convert to list of tuples
    let mut result = Vec::new();
    for (key, count) in result_items {
        result.push(Value::Tuple(vec![Value::Str(key.clone()), Value::Int(*count)]));
    }

    Ok(Value::List(HPList::from_values(result)))
}

/// elements method for Counter
fn counter_elements(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("elements() takes exactly 1 argument"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];

    // Get the fields from the Counter object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid Counter object")),
    };

    // Create a list with elements repeated according to their counts
    let mut result = Vec::new();
    for (key, value) in fields.iter() {
        if let Value::Int(count) = value {
            for _ in 0..*count {
                result.push(Value::Str(key.clone()));
            }
        }
    }

    Ok(Value::List(HPList::from_values(result)))
}

/// subtract method for Counter
fn counter_subtract(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("subtract() takes exactly 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    let other = &args[1];

    // Get the fields from the Counter object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid Counter object")),
    };

    // For now, we'll just return None as this would require mutable access to fields
    // In a full implementation, we would subtract counts from other
    Ok(Value::None)
}

/// update method for Counter
fn counter_update(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("update() takes exactly 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    let other = &args[1];

    // Get the fields from the Counter object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid Counter object")),
    };

    // For now, we'll just return None as this would require mutable access to fields
    // In a full implementation, we would add counts from other
    Ok(Value::None)
}

/// __add__ method for Counter
fn counter_add(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("__add__ takes exactly 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    let other = &args[1];

    // For now, we'll just return self as this would require combining counters
    Ok(self_obj.clone())
}

/// __sub__ method for Counter
fn counter_subtract_op(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("__sub__ takes exactly 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    let other = &args[1];

    // For now, we'll just return self as this would require subtracting counters
    Ok(self_obj.clone())
}

/// __or__ method for Counter (union)
fn counter_or(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("__or__ takes exactly 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    let other = &args[1];

    // For now, we'll just return self as this would require combining counters with max counts
    Ok(self_obj.clone())
}

/// __and__ method for Counter (intersection)
fn counter_and(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("__and__ takes exactly 2 arguments"));
    }

    // First argument is self (the Counter object)
    let self_obj = &args[0];
    let other = &args[1];

    // For now, we'll just return self as this would require combining counters with min counts
    Ok(self_obj.clone())
}

/// defaultdict implementation
fn defaultdict_builtin(args: Vec<Value>) -> Result<Value> {
    // defaultdict(default_factory) or defaultdict(default_factory, mapping)
    let default_factory = if !args.is_empty() {
        Some(args[0].clone())
    } else {
        None
    };

    let mut fields = HashMap::new();
    fields.insert("default_factory".to_string(), default_factory.unwrap_or(Value::None));

    // If there's a second argument, initialize from it
    if args.len() > 1 {
        if let Value::Dict(map) = &args[1] {
            for (k, v) in map.borrow().iter() {
                fields.insert(k.clone(), v.clone());
            }
        }
    }

    Ok(Value::Object {
        class_name: "defaultdict".to_string(),
        fields: Rc::new(fields),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("defaultdict".to_string(), vec!["dict".to_string(), "object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["defaultdict".to_string(), "dict".to_string(), "object".to_string()]),
    })
}

/// deque implementation
fn deque_builtin(args: Vec<Value>) -> Result<Value> {
    // deque() or deque(iterable) or deque(iterable, maxlen)
    let mut items = Vec::new();
    let mut maxlen = None;

    if !args.is_empty() {
        match &args[0] {
            Value::List(list) => {
                items.extend(list.iter().cloned());
            }
            _ => {}
        }
    }

    if args.len() > 1 {
        if let Value::Int(n) = &args[1] {
            maxlen = Some(*n);
        }
    }

    let mut fields = HashMap::new();
    fields.insert("items".to_string(), Value::List(HPList::from_values(items)));
    if let Some(max) = maxlen {
        fields.insert("maxlen".to_string(), Value::Int(max));
    } else {
        fields.insert("maxlen".to_string(), Value::None);
    }
    
    // Add iterator methods to fields (like in itertools)
    fields.insert("__iter__".to_string(), Value::NativeFunction(deque_iter));
    fields.insert("__next__".to_string(), Value::NativeFunction(deque_next));

    // Add deque methods
    let mut class_methods = HashMap::new();
    class_methods.insert("append".to_string(), Value::NativeFunction(deque_append));
    class_methods.insert("appendleft".to_string(), Value::NativeFunction(deque_appendleft));
    class_methods.insert("pop".to_string(), Value::NativeFunction(deque_pop));
    class_methods.insert("popleft".to_string(), Value::NativeFunction(deque_popleft));
    class_methods.insert("clear".to_string(), Value::NativeFunction(deque_clear));
    class_methods.insert("extend".to_string(), Value::NativeFunction(deque_extend));
    class_methods.insert("extendleft".to_string(), Value::NativeFunction(deque_extendleft));
    class_methods.insert("rotate".to_string(), Value::NativeFunction(deque_rotate));

    Ok(Value::Object {
        class_name: "deque".to_string(),
        fields: Rc::new(fields),
        class_methods,
        base_object: crate::base_object::BaseObject::new("deque".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["deque".to_string(), "object".to_string()]),
    })
}

/// deque append method
fn deque_append(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("append() takes exactly one argument"));
    }

    let self_obj = &args[0];
    let value = &args[1];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Get maxlen if it exists
    let maxlen = match fields.get("maxlen") {
        Some(Value::Int(n)) => Some(*n as usize),
        _ => None,
    };

    // Clone the list to modify it
    let mut new_items = items.clone();
    new_items.append(value.clone());

    // Apply maxlen constraint if needed
    if let Some(max) = maxlen {
        while new_items.len() > max {
            new_items.pop_at(0)?; // Remove from left when exceeding maxlen
        }
    }

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(Value::None)
}

/// deque appendleft method
fn deque_appendleft(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("appendleft() takes exactly one argument"));
    }

    let self_obj = &args[0];
    let value = &args[1];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Get maxlen if it exists
    let maxlen = match fields.get("maxlen") {
        Some(Value::Int(n)) => Some(*n as usize),
        _ => None,
    };

    // Clone the list to modify it
    let mut new_items = items.clone();
    new_items.insert(0, value.clone())?;

    // Apply maxlen constraint if needed
    if let Some(max) = maxlen {
        while new_items.len() > max {
            new_items.pop(); // Remove from right when exceeding maxlen
        }
    }

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(Value::None)
}

/// deque pop method
fn deque_pop(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("pop() takes no arguments"));
    }

    let self_obj = &args[0];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Check if deque is empty
    if items.is_empty() {
        return Err(anyhow::anyhow!("pop from an empty deque"));
    }

    // Clone the list to modify it
    let mut new_items = items.clone();
    let popped = new_items.pop().unwrap();

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(popped)
}

/// deque popleft method
fn deque_popleft(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("popleft() takes no arguments"));
    }

    let self_obj = &args[0];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Check if deque is empty
    if items.is_empty() {
        return Err(anyhow::anyhow!("pop from an empty deque"));
    }

    // Clone the list to modify it
    let mut new_items = items.clone();
    let popped = new_items.pop_at(0)?;

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(popped)
}

/// deque clear method
fn deque_clear(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("clear() takes no arguments"));
    }

    let self_obj = &args[0];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Update the items in the fields with an empty list
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(HPList::new()));

    Ok(Value::None)
}

/// deque extend method
fn deque_extend(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("extend() takes exactly one argument"));
    }

    let self_obj = &args[0];
    let iterable = &args[1];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Get maxlen if it exists
    let maxlen = match fields.get("maxlen") {
        Some(Value::Int(n)) => Some(*n as usize),
        _ => None,
    };

    // Clone the list to modify it
    let mut new_items = items.clone();

    // Extend with items from iterable
    match iterable {
        Value::List(list) => {
            for item in list.iter() {
                new_items.append(item.clone());
            }
        }
        Value::Tuple(tuple) => {
            for item in tuple.iter() {
                new_items.append(item.clone());
            }
        }
        _ => return Err(anyhow::anyhow!("extend() argument must be iterable")),
    }

    // Apply maxlen constraint if needed
    if let Some(max) = maxlen {
        while new_items.len() > max {
            new_items.pop_at(0)?; // Remove from left when exceeding maxlen
        }
    }

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(Value::None)
}

/// deque extendleft method
fn deque_extendleft(args: Vec<Value>) -> Result<Value> {
    if args.len() != 2 {
        return Err(anyhow::anyhow!("extendleft() takes exactly one argument"));
    }

    let self_obj = &args[0];
    let iterable = &args[1];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Get maxlen if it exists
    let maxlen = match fields.get("maxlen") {
        Some(Value::Int(n)) => Some(*n as usize),
        _ => None,
    };

    // Clone the list to modify it
    let mut new_items = items.clone();

    // Extend with items from iterable (in reverse order to match Python behavior)
    match iterable {
        Value::List(list) => {
            // Collect items and then insert them in reverse order
            let mut items_to_insert = Vec::new();
            for item in list.iter() {
                items_to_insert.push(item.clone());
            }
            
            // Insert items in reverse order (last item first)
            for item in items_to_insert.iter().rev() {
                new_items.insert(0, item.clone())?;
            }
        }
        Value::Tuple(tuple) => {
            // Collect items and then insert them in reverse order
            let mut items_to_insert = Vec::new();
            for item in tuple.iter() {
                items_to_insert.push(item.clone());
            }
            
            // Insert items in reverse order (last item first)
            for item in items_to_insert.iter().rev() {
                new_items.insert(0, item.clone())?;
            }
        }
        _ => return Err(anyhow::anyhow!("extendleft() argument must be iterable")),
    }

    // Apply maxlen constraint if needed
    if let Some(max) = maxlen {
        while new_items.len() > max {
            new_items.pop(); // Remove from right when exceeding maxlen
        }
    }

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(Value::None)
}

/// deque rotate method
fn deque_rotate(args: Vec<Value>) -> Result<Value> {
    if args.len() > 2 {
        return Err(anyhow::anyhow!("rotate() takes at most one argument"));
    }

    let self_obj = &args[0];
    let n = if args.len() == 2 {
        match &args[1] {
            Value::Int(i) => *i,
            _ => return Err(anyhow::anyhow!("rotate() argument must be an integer")),
        }
    } else {
        1 // Default rotation
    };

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    if items.is_empty() || n == 0 {
        return Ok(Value::None);
    }

    // Clone the list to modify it
    let mut new_items = items.clone();
    let len = new_items.len() as i64;
    
    // Normalize rotation steps to be within list length
    let steps = if n >= 0 {
        n % len
    } else {
        (n % len + len) % len
    };

    if steps != 0 {
        // Perform rotation
        if steps > 0 {
            // Rotate right: move last 'steps' elements to the front
            let split_point = (len - steps) as usize;
            let mut elements_to_move = Vec::new();
            
            // Collect elements to move from the end
            for i in split_point..new_items.len() {
                if let Some(item) = new_items.get(i as isize) {
                    elements_to_move.push(item.clone());
                }
            }
            
            // Remove elements from the end
            for _ in split_point..new_items.len() {
                new_items.pop();
            }
            
            // Insert elements at the beginning (in reverse order to maintain sequence)
            for item in elements_to_move.iter().rev() {
                new_items.insert(0, item.clone())?;
            }
        } else {
            // Rotate left: move first 'steps' elements to the end
            let steps_abs = (-steps) as usize;
            let mut elements_to_move = Vec::new();
            
            // Collect elements to move from the beginning
            for i in 0..steps_abs {
                if let Some(item) = new_items.get(i as isize) {
                    elements_to_move.push(item.clone());
                }
            }
            
            // Remove elements from the beginning
            for _ in 0..steps_abs {
                let _ = new_items.pop_at(0)?; // Ignore the returned value
            }
            
            // Append elements to the end
            for item in elements_to_move {
                new_items.append(item);
            }
        }
    }

    // Update the items in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("items".to_string(), Value::List(new_items));

    Ok(Value::None)
}

/// deque __iter__ method
fn deque_iter(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("__iter__() takes no arguments"));
    }

    let self_obj = &args[0];

    // Get the fields from the deque object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid deque object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid deque items")),
    };

    // Create an iterator object with the items and starting index
    let mut iterator_fields = HashMap::new();
    iterator_fields.insert("items".to_string(), Value::List(items.clone()));
    iterator_fields.insert("index".to_string(), Value::Int(0));

    Ok(Value::Object {
        class_name: "deque_iterator".to_string(),
        fields: Rc::new(iterator_fields),
        class_methods: HashMap::new(),
        base_object: crate::base_object::BaseObject::new("deque_iterator".to_string(), vec!["object".to_string()]),
        mro: crate::base_object::MRO::from_linearization(vec!["deque_iterator".to_string(), "object".to_string()]),
    })
}

/// deque __next__ method
fn deque_next(args: Vec<Value>) -> Result<Value> {
    if args.len() != 1 {
        return Err(anyhow::anyhow!("__next__() takes no arguments"));
    }

    let self_obj = &args[0];

    // Get the fields from the iterator object
    let fields = match self_obj {
        Value::Object { fields, .. } => fields,
        _ => return Err(anyhow::anyhow!("Invalid iterator object")),
    };

    // Get the items list
    let items = match fields.get("items") {
        Some(Value::List(list)) => list,
        _ => return Err(anyhow::anyhow!("Invalid iterator items")),
    };

    // Get the current index
    let index = match fields.get("index") {
        Some(Value::Int(i)) => *i,
        _ => return Err(anyhow::anyhow!("Invalid iterator index")),
    };

    // Check if we've reached the end
    if index as usize >= items.len() {
        return Err(anyhow::anyhow!("StopIteration"));
    }

    // Get the current item
    let item = items.get(index as isize).unwrap().clone();

    // Update the index in the fields
    let mut new_fields = (**fields).clone();
    new_fields.insert("index".to_string(), Value::Int(index + 1));

    Ok(item)
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