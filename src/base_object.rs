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
        _class_mros: &HashMap<String, MRO>,
    ) -> Result<Vec<String>> {
        // For now, we'll use a simple linearization
        let mut linearization = vec![class_name.to_string()];
        
        // Add all base classes
        for base in bases {
            linearization.push(base.clone());
        }
        
        // Add object as the ultimate base class if not already present
        if !linearization.contains(&"object".to_string()) {
            linearization.push("object".to_string());
        }
        
        Ok(linearization)
    }
}

impl Default for MRO {
    fn default() -> Self {
        Self::new()
    }
}