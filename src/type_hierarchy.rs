use crate::value::Value;
use crate::base_object::{BaseObject, MRO};
use std::collections::HashMap;

/// Type hierarchy system that makes all built-in types inherit from object
pub struct TypeHierarchy;

impl TypeHierarchy {
    /// Get the MRO for a built-in type
    pub fn get_builtin_mro(type_name: &str) -> MRO {
        let class_mros = HashMap::new(); // Empty for built-in types
        match type_name {
            "int" => {
                let linearization = MRO::compute_c3_linearization("int", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["int".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "float" => {
                let linearization = MRO::compute_c3_linearization("float", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["float".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "str" => {
                let linearization = MRO::compute_c3_linearization("str", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["str".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "bool" => {
                let linearization = MRO::compute_c3_linearization("bool", &[String::from("int"), String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["bool".to_string(), "int".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "list" => {
                let linearization = MRO::compute_c3_linearization("list", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["list".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "dict" => {
                let linearization = MRO::compute_c3_linearization("dict", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["dict".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "tuple" => {
                let linearization = MRO::compute_c3_linearization("tuple", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["tuple".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "set" => {
                let linearization = MRO::compute_c3_linearization("set", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["set".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "bytes" => {
                let linearization = MRO::compute_c3_linearization("bytes", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["bytes".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "bytearray" => {
                let linearization = MRO::compute_c3_linearization("bytearray", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["bytearray".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "function" => {
                let linearization = MRO::compute_c3_linearization("function", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["function".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "module" => {
                let linearization = MRO::compute_c3_linearization("module", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["module".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "NoneType" => {
                let linearization = MRO::compute_c3_linearization("NoneType", &[String::from("object")], &class_mros)
                    .unwrap_or_else(|_| vec!["NoneType".to_string(), "object".to_string()]);
                MRO::from_linearization(linearization)
            },
            "object" => MRO::from_linearization(vec!["object".to_string()]),
            _ => MRO::from_linearization(vec!["object".to_string()]), // Default to object
        }
    }

    /// Get the base object for a built-in type
    pub fn get_builtin_base_object(type_name: &str) -> BaseObject {
        let parents = match type_name {
            "bool" => vec!["int".to_string(), "object".to_string()],
            _ => vec!["object".to_string()],
        };
        BaseObject::new(type_name.to_string(), parents)
    }

    /// Check if a type is a subtype of another type using MRO
    pub fn is_subtype(subtype: &str, supertype: &str) -> bool {
        if subtype == supertype {
            return true;
        }
        
        let mro = Self::get_builtin_mro(subtype);
        mro.linearization.iter().any(|class| class == supertype)
    }

    /// Get all parent types for a given type
    pub fn get_parent_types(type_name: &str) -> Vec<String> {
        let mro = Self::get_builtin_mro(type_name);
        mro.linearization[1..].to_vec() // Skip the first element (the type itself)
    }

    /// Enhanced isinstance check that uses MRO
    pub fn isinstance(value: &Value, expected_type: &str) -> bool {
        let value_type = value.type_name();
        Self::is_subtype(&value_type, expected_type)
    }

    /// Get the type name with inheritance information
    pub fn get_type_info(value: &Value) -> String {
        let type_name = value.type_name();
        let mro = Self::get_builtin_mro(&type_name);
        format!("{} (MRO: {})", type_name, mro.linearization.join(" -> "))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_builtin_inheritance() {
        // Test that all types inherit from object
        assert!(TypeHierarchy::is_subtype("int", "object"));
        assert!(TypeHierarchy::is_subtype("str", "object"));
        assert!(TypeHierarchy::is_subtype("list", "object"));
        
        // Test bool inherits from int
        assert!(TypeHierarchy::is_subtype("bool", "int"));
        assert!(TypeHierarchy::is_subtype("bool", "object"));
        
        // Test self-inheritance
        assert!(TypeHierarchy::is_subtype("int", "int"));
    }

    #[test]
    fn test_isinstance() {
        let int_val = Value::Int(42);
        let str_val = Value::Str("hello".to_string());
        let bool_val = Value::Bool(true);
        
        assert!(TypeHierarchy::isinstance(&int_val, "int"));
        assert!(TypeHierarchy::isinstance(&int_val, "object"));
        assert!(!TypeHierarchy::isinstance(&int_val, "str"));
        
        assert!(TypeHierarchy::isinstance(&str_val, "str"));
        assert!(TypeHierarchy::isinstance(&str_val, "object"));
        
        assert!(TypeHierarchy::isinstance(&bool_val, "bool"));
        assert!(TypeHierarchy::isinstance(&bool_val, "int"));
        assert!(TypeHierarchy::isinstance(&bool_val, "object"));
    }
}
