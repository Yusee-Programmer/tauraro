//! Object-Oriented Programming Support for C Transpiler
//!
//! Handles class definitions, inheritance, methods, and dynamic dispatch.

use std::collections::HashMap;
use crate::ast::*;

/// Class metadata for OOP
#[derive(Debug, Clone)]
pub struct ClassMeta {
    pub name: String,
    pub parent: Option<String>,
    pub methods: HashMap<String, String>,
    pub fields: HashMap<String, String>,
    pub is_abstract: bool,
}

/// OOP context for tracking classes and inheritance
pub struct OOPContext {
    pub classes: HashMap<String, ClassMeta>,
    pub inheritance_chain: HashMap<String, Vec<String>>,
}

impl OOPContext {
    pub fn new() -> Self {
        Self {
            classes: HashMap::new(),
            inheritance_chain: HashMap::new(),
        }
    }

    /// Register a class definition
    pub fn register_class(&mut self, name: String, parent: Option<String>, is_abstract: bool) {
        let meta = ClassMeta {
            name: name.clone(),
            parent: parent.clone(),
            methods: HashMap::new(),
            fields: HashMap::new(),
            is_abstract,
        };
        self.classes.insert(name.clone(), meta);

        // Build inheritance chain
        if let Some(parent_name) = parent {
            self.inheritance_chain
                .entry(parent_name)
                .or_insert_with(Vec::new)
                .push(name);
        }
    }

    /// Add method to class
    pub fn add_method(&mut self, class_name: String, method_name: String, signature: String) {
        if let Some(class) = self.classes.get_mut(&class_name) {
            class.methods.insert(method_name, signature);
        }
    }

    /// Add field to class
    pub fn add_field(&mut self, class_name: String, field_name: String, field_type: String) {
        if let Some(class) = self.classes.get_mut(&class_name) {
            class.fields.insert(field_name, field_type);
        }
    }

    /// Get class metadata
    pub fn get_class(&self, name: &str) -> Option<&ClassMeta> {
        self.classes.get(name)
    }

    /// Check if class is subclass of another
    pub fn is_subclass(&self, child: &str, parent: &str) -> bool {
        if let Some(class) = self.classes.get(child) {
            if let Some(ref p) = class.parent {
                if p == parent {
                    return true;
                }
                return self.is_subclass(p, parent);
            }
        }
        false
    }
}

/// Generate C struct for class definition
pub fn generate_class_struct(name: &str, fields: &HashMap<String, String>) -> String {
    let mut code = format!("typedef struct {{\n", );
    for (field_name, field_type) in fields {
        code.push_str(&format!("    {} {};\n", field_type, field_name));
    }
    code.push_str(&format!("}} {}_t;\n\n", name));
    code
}

/// Generate constructor function for class
pub fn generate_constructor(name: &str, fields: &HashMap<String, String>) -> String {
    let mut code = format!("{}_t* {}_new(", name, name);
    
    let field_params: Vec<_> = fields.iter()
        .map(|(n, t)| format!("{} {}", t, n))
        .collect();
    code.push_str(&field_params.join(", "));
    code.push_str(") {\n");
    
    code.push_str(&format!("    {}_t* obj = malloc(sizeof({}_t));\n", name, name));
    for field_name in fields.keys() {
        code.push_str(&format!("    obj->{} = {};\n", field_name, field_name));
    }
    code.push_str("    return obj;\n");
    code.push_str("}\n\n");
    code
}

/// Generate method wrapper
pub fn generate_method_wrapper(
    class_name: &str,
    method_name: &str,
    params: &[String],
    return_type: &str,
) -> String {
    let param_str = params.join(", ");
    format!(
        "{} {}_{}({}_t* self, {}) {{\n    // TODO: Implement method body\n}}\n\n",
        return_type, class_name, method_name, class_name, param_str
    )
}

/// Generate virtual method table for polymorphism
pub fn generate_vtable(class_name: &str, methods: &HashMap<String, String>) -> String {
    let mut code = format!("typedef struct {{\n");
    for (method_name, signature) in methods {
        code.push_str(&format!("    {} (*{})(void*, ...);\n", signature, method_name));
    }
    code.push_str(&format!("}} {}_vtable_t;\n\n", class_name));
    code
}

/// Generate inheritance chain setup
pub fn generate_inheritance_chain(class: &ClassMeta, parent: Option<&ClassMeta>) -> String {
    let mut code = String::new();
    
    if let Some(p) = parent {
        code.push_str(&format!("// {} inherits from {}\n", class.name, p.name));
        code.push_str(&format!("// Copy parent fields and methods\n\n"));
    }
    
    code
}

/// Generate destructor for class
pub fn generate_destructor(name: &str, fields: &HashMap<String, String>) -> String {
    let mut code = format!("void {}_free({}_t* obj) {{\n", name, name);
    
    // Free dynamically allocated fields
    for (field_name, field_type) in fields {
        if field_type.contains("char*") || field_type.contains("void*") {
            code.push_str(&format!("    if (obj->{}) free(obj->{});\n", field_name, field_name));
        }
    }
    
    code.push_str("    free(obj);\n");
    code.push_str("}\n\n");
    code
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_class_registration() {
        let mut ctx = OOPContext::new();
        ctx.register_class("Animal".to_string(), None, false);
        ctx.register_class("Dog".to_string(), Some("Animal".to_string()), false);
        
        assert!(ctx.is_subclass("Dog", "Animal"));
        assert!(!ctx.is_subclass("Animal", "Dog"));
    }

    #[test]
    fn test_method_registration() {
        let mut ctx = OOPContext::new();
        ctx.register_class("Dog".to_string(), None, false);
        ctx.add_method(
            "Dog".to_string(),
            "bark".to_string(),
            "void".to_string(),
        );
        
        let dog = ctx.get_class("Dog").unwrap();
        assert!(dog.methods.contains_key("bark"));
    }
}
