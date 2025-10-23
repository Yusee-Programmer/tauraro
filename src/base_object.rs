//! Base object implementation for Tauraro

use crate::value::Value;
use std::collections::HashMap;
use anyhow::Result;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BaseObject {
    pub class_name: String,
    pub bases: Vec<String>,
    pub methods: HashMap<String, DunderMethod>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DunderMethod {
    // Common dunder methods
    New,
    Init,
    Str,
    Repr,
    Len,
    GetItem,
    SetItem,
    DelItem,
    Contains,
    Iter,
    Next,
    Call,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Pow,
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
    And,
    Or,
    Xor,
    Invert,
    Neg,
    Pos,
    Abs,
    Index,
    Hash,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MRO {
    linearization: Vec<String>,
}

impl BaseObject {
    pub fn new(class_name: String, bases: Vec<String>) -> Self {
        Self {
            class_name,
            bases,
            methods: HashMap::new(),
        }
    }
    
    pub fn add_method(&mut self, name: String, method: DunderMethod) {
        self.methods.insert(name, method);
    }
    
    pub fn get_method(&self, name: &str) -> Option<DunderMethod> {
        self.methods.get(name).copied()
    }
    
    pub fn get_base_methods() -> HashMap<String, DunderMethod> {
        let mut methods = HashMap::new();
        methods.insert("__new__".to_string(), DunderMethod::New);
        methods.insert("__init__".to_string(), DunderMethod::Init);
        methods.insert("__str__".to_string(), DunderMethod::Str);
        methods.insert("__repr__".to_string(), DunderMethod::Repr);
        methods.insert("__len__".to_string(), DunderMethod::Len);
        methods.insert("__getitem__".to_string(), DunderMethod::GetItem);
        methods.insert("__setitem__".to_string(), DunderMethod::SetItem);
        methods.insert("__delitem__".to_string(), DunderMethod::DelItem);
        methods.insert("__contains__".to_string(), DunderMethod::Contains);
        methods.insert("__iter__".to_string(), DunderMethod::Iter);
        methods.insert("__next__".to_string(), DunderMethod::Next);
        methods.insert("__call__".to_string(), DunderMethod::Call);
        methods.insert("__add__".to_string(), DunderMethod::Add);
        methods.insert("__sub__".to_string(), DunderMethod::Sub);
        methods.insert("__mul__".to_string(), DunderMethod::Mul);
        methods.insert("__div__".to_string(), DunderMethod::Div);
        methods.insert("__mod__".to_string(), DunderMethod::Mod);
        methods.insert("__pow__".to_string(), DunderMethod::Pow);
        methods.insert("__eq__".to_string(), DunderMethod::Eq);
        methods.insert("__ne__".to_string(), DunderMethod::Ne);
        methods.insert("__lt__".to_string(), DunderMethod::Lt);
        methods.insert("__le__".to_string(), DunderMethod::Le);
        methods.insert("__gt__".to_string(), DunderMethod::Gt);
        methods.insert("__ge__".to_string(), DunderMethod::Ge);
        methods.insert("__and__".to_string(), DunderMethod::And);
        methods.insert("__or__".to_string(), DunderMethod::Or);
        methods.insert("__xor__".to_string(), DunderMethod::Xor);
        methods.insert("__invert__".to_string(), DunderMethod::Invert);
        methods.insert("__neg__".to_string(), DunderMethod::Neg);
        methods.insert("__pos__".to_string(), DunderMethod::Pos);
        methods.insert("__abs__".to_string(), DunderMethod::Abs);
        methods.insert("__index__".to_string(), DunderMethod::Index);
        methods.insert("__hash__".to_string(), DunderMethod::Hash);
        methods
    }
}

impl MRO {
    pub fn new() -> Self {
        Self {
            linearization: vec!["object".to_string()],
        }
    }
    
    pub fn from_linearization(linearization: Vec<String>) -> Self {
        Self { linearization }
    }
    
    pub fn get_linearization(&self) -> &Vec<String> {
        &self.linearization
    }
    
    pub fn find_method(&self, method_name: &str, _class_methods: &HashMap<String, Value>) -> Option<DunderMethod> {
        // For now, we'll just check if the method exists in base methods
        let base_methods = BaseObject::get_base_methods();
        base_methods.get(method_name).copied()
    }
    
    /// Compute C3 linearization for method resolution order
    pub fn compute_c3_linearization(
        class_name: &str,
        bases: &[String],
        class_mros: &HashMap<String, MRO>,
    ) -> Result<Vec<String>> {
        // Implement proper C3 linearization algorithm
        
        // Create lists for each base class MRO + the base class itself
        let mut sequences = Vec::new();
        
        // Add each base class MRO as a sequence
        for base_name in bases {
            if let Some(base_mro) = class_mros.get(base_name) {
                // Add the MRO sequence
                sequences.push(base_mro.get_linearization().clone());
            } else {
                // If we don't have the base MRO, just add the base class
                sequences.push(vec![base_name.clone()]);
            }
        }
        
        // Add the bases themselves as the final sequence
        sequences.push(bases.to_vec());
        
        // Start with the class itself
        let mut result = vec![class_name.to_string()];
        
        // Apply C3 merge algorithm
        while !sequences.is_empty() {
            // Find a good head (a class that appears at the head of at least one sequence
            // and doesn't appear in the tail of any sequence)
            let mut found = false;
            let mut i = 0;
            while i < sequences.len() {
                if !sequences[i].is_empty() {
                    let head = sequences[i][0].clone();
                    // Check if this head appears in any tail
                    let mut in_tail = false;
                    for seq in &sequences {
                        if seq.len() > 1 && seq[1..].contains(&head) {
                            in_tail = true;
                            break;
                        }
                    }
                    
                    if !in_tail {
                        // This is a good head, add it to result
                        result.push(head.clone());
                        
                        // Remove this head from all sequences
                        let mut j = 0;
                        while j < sequences.len() {
                            if !sequences[j].is_empty() && sequences[j][0] == head {
                                sequences[j].remove(0);
                                // If sequence is now empty, remove it
                                if sequences[j].is_empty() {
                                    sequences.remove(j);
                                } else {
                                    j += 1;
                                }
                            } else {
                                j += 1;
                            }
                        }
                        
                        found = true;
                        break;
                    }
                }
                i += 1;
            }
            
            if !found {
                // Cannot create consistent MRO
                return Err(anyhow::anyhow!("Cannot create a consistent method resolution order (MRO) for class {}", class_name));
            }
        }
        
        // Add object as the ultimate base class if not already present
        if !result.contains(&"object".to_string()) {
            result.push("object".to_string());
        }
        
        Ok(result)
    }
    
    /// Find a method in the MRO chain
    pub fn find_method_in_mro(&self, method_name: &str, class_registry: &HashMap<String, Value>) -> Option<Value> {
        self.find_method_in_mro_with_visited(method_name, class_registry, &mut std::collections::HashSet::new())
    }

    fn find_method_in_mro_with_visited(
        &self,
        method_name: &str,
        class_registry: &HashMap<String, Value>,
        visited: &mut std::collections::HashSet<String>
    ) -> Option<Value> {
        // Search through the linearization for the method
        for class_name in &self.linearization {
            // Skip if we've already visited this class (prevents infinite recursion)
            if visited.contains(class_name) {
                continue;
            }
            visited.insert(class_name.clone());

            // Look up the class in the registry
            if let Some(class_value) = class_registry.get(class_name) {
                // Check if it's a Class value
                if let Value::Class { methods, mro, .. } = class_value {
                    // First check if the method exists in this class's immediate methods
                    if let Some(method) = methods.get(method_name) {
                        return Some(method.clone());
                    }
                    // If not found, recursively search through this class's MRO
                    if let Some(method) = mro.find_method_in_mro_with_visited(method_name, class_registry, visited) {
                        return Some(method);
                    }
                }
                // Also check Object values (instances) for class methods
                else if let Value::Object { class_methods, mro, .. } = class_value {
                    // Check if the method exists in this object's class methods
                    if let Some(method) = class_methods.get(method_name) {
                        return Some(method.clone());
                    }
                    // If not found, recursively search through this object's MRO
                    if let Some(method) = mro.find_method_in_mro_with_visited(method_name, class_registry, visited) {
                        return Some(method);
                    }
                }
            }
        }
        None
    }
}

impl Default for MRO {
    fn default() -> Self {
        Self::new()
    }
}