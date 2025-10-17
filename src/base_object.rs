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
    
    pub fn find_method(&self, method_name: &str, class_methods: &HashMap<String, Value>) -> Option<DunderMethod> {
        // First check if the method exists in the current class methods
        if class_methods.contains_key(method_name) {
            // This is a simplified check - in a full implementation we would return the actual method
            return Some(DunderMethod::Call); // Placeholder
        }
        
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
        // C3 linearization algorithm:
        // L(Class) = Class + merge(L(parent1), L(parent2), ..., parent1, parent2, ...)
        
        // Create lists for merge operation
        let mut merge_lists: Vec<Vec<String>> = Vec::new();
        
        // Add MRO of each base class
        for base in bases {
            if let Some(base_mro) = class_mros.get(base) {
                merge_lists.push(base_mro.get_linearization().clone());
            } else {
                // If base class MRO is not available, create a simple one
                merge_lists.push(vec![base.clone(), "object".to_string()]);
            }
        }
        
        // Add the base classes themselves as a list
        let mut bases_list = bases.to_vec();
        bases_list.push("object".to_string()); // All classes ultimately inherit from object
        merge_lists.push(bases_list);
        
        // Start with the class itself
        let mut result = vec![class_name.to_string()];
        
        // Merge operation
        while !merge_lists.is_empty() {
            let mut _found = false;
            let candidate = {
                // Find a candidate class that is not in the tail of any list
                let mut candidate_opt = None;
                for list in &merge_lists {
                    if !list.is_empty() {
                        let candidate = &list[0];
                        let mut is_valid = true;
                        
                        // Check if candidate is in the tail of any other list
                        for other_list in &merge_lists {
                            if other_list.len() > 1 && other_list[1..].contains(candidate) {
                                is_valid = false;
                                break;
                            }
                        }
                        
                        if is_valid {
                            candidate_opt = Some(candidate.clone());
                            break;
                        }
                    }
                }
                if let Some(c) = candidate_opt {
                    c
                } else {
                    // Inconsistent hierarchy - this should not happen in valid Python code
                    return Err(anyhow::anyhow!("Cannot create a consistent method resolution order (MRO) for class {}", class_name));
                }
            };
            
            // Add candidate to result
            result.push(candidate.clone());
            
            // Remove candidate from all lists
            for list in merge_lists.iter_mut() {
                list.retain(|item| item != &candidate);
            }
            
            // Remove empty lists
            merge_lists.retain(|list| !list.is_empty());
        }
        
        Ok(result)
    }
    
    /// Find a method in the MRO chain
    pub fn find_method_in_mro(&self, method_name: &str, class_registry: &HashMap<String, Value>) -> Option<Value> {
        // Search through the linearization for the method
        for class_name in &self.linearization {
            // Look up the class in the registry
            if let Some(class_value) = class_registry.get(class_name) {
                // Check if it's a Class value
                if let Value::Class { methods, .. } = class_value {
                    // Check if the method exists in this class
                    if let Some(method) = methods.get(method_name) {
                        return Some(method.clone());
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