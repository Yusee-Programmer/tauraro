

//! Type Checker Module - Runtime type enforcement for static typing
//!
//! This module provides runtime type checking capabilities for Tauraro,
//! enabling both dynamic and static typing in the same codebase.
//!
//! When variables, functions, or objects are declared with type annotations,
//! this module enforces those types at runtime, similar to Java's type system.

use crate::ast::Type;
use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::fmt;

/// Type information storage for variables, functions, and objects
#[derive(Debug, Clone)]
pub struct TypeInfo {
    pub declared_type: Type,
    pub is_mutable: bool,
}

/// Type environment for tracking declared types
#[derive(Debug, Clone)]
pub struct TypeEnvironment {
    /// Maps variable names to their declared types
    pub variable_types: HashMap<String, TypeInfo>,
    /// Maps function names to their parameter and return types
    pub function_types: HashMap<String, FunctionTypeInfo>,
    /// Maps class names to their attribute types
    pub class_types: HashMap<String, ClassTypeInfo>,
    /// Enable type inference for variables without explicit annotations
    pub enable_type_inference: bool,
    /// Inferred types from initial assignments
    pub inferred_types: HashMap<String, Type>,
}

#[derive(Debug, Clone)]
pub struct FunctionTypeInfo {
    pub param_types: Vec<(String, Option<Type>)>,
    pub return_type: Option<Type>,
}

#[derive(Debug, Clone)]
pub struct ClassTypeInfo {
    pub attribute_types: HashMap<String, Type>,
}

impl TypeEnvironment {
    pub fn new() -> Self {
        Self {
            variable_types: HashMap::new(),
            function_types: HashMap::new(),
            class_types: HashMap::new(),
            enable_type_inference: true,
            inferred_types: HashMap::new(),
        }
    }

    /// Infer type from a value and store it
    pub fn infer_type(&mut self, name: String, value: &Value) -> Type {
        let inferred = infer_type_from_value(value);
        self.inferred_types.insert(name, inferred.clone());
        inferred
    }

    /// Get the type of a variable (declared or inferred)
    pub fn get_variable_type_or_inferred(&self, name: &str) -> Option<&Type> {
        self.variable_types
            .get(name)
            .map(|info| &info.declared_type)
            .or_else(|| self.inferred_types.get(name))
    }

    /// Register a variable with its type annotation
    pub fn register_variable(&mut self, name: String, type_annotation: Type) {
        self.variable_types.insert(
            name,
            TypeInfo {
                declared_type: type_annotation,
                is_mutable: true,
            },
        );
    }

    /// Register a function with its parameter and return types
    pub fn register_function(
        &mut self,
        name: String,
        param_types: Vec<(String, Option<Type>)>,
        return_type: Option<Type>,
    ) {
        self.function_types.insert(
            name,
            FunctionTypeInfo {
                param_types,
                return_type,
            },
        );
    }

    /// Register a class with its attribute types
    pub fn register_class(&mut self, name: String, attribute_types: HashMap<String, Type>) {
        self.class_types.insert(
            name,
            ClassTypeInfo { attribute_types },
        );
    }

    /// Get the declared type of a variable
    pub fn get_variable_type(&self, name: &str) -> Option<&Type> {
        self.variable_types.get(name).map(|info| &info.declared_type)
    }

    /// Get the function type information
    pub fn get_function_type(&self, name: &str) -> Option<&FunctionTypeInfo> {
        self.function_types.get(name)
    }

    /// Get the class type information
    pub fn get_class_type(&self, name: &str) -> Option<&ClassTypeInfo> {
        self.class_types.get(name)
    }
}

impl Default for TypeEnvironment {
    fn default() -> Self {
        Self::new()
    }
}

/// Type checker for runtime type enforcement
pub struct TypeChecker {
    pub type_env: TypeEnvironment,
}

impl TypeChecker {
    pub fn new() -> Self {
        Self {
            type_env: TypeEnvironment::new(),
        }
    }

    /// Check if a value matches the expected type
    pub fn check_type(&self, value: &Value, expected_type: &Type) -> Result<()> {
        if self.value_matches_type(value, expected_type) {
            Ok(())
        } else {
            Err(anyhow!(
                "TypeError: Expected type '{}', but got value of type '{}'",
                format_type(expected_type),
                value.type_name()
            ))
        }
    }

    /// Check if a value matches the expected type (returns bool)
    pub fn value_matches_type(&self, value: &Value, expected_type: &Type) -> bool {
        match expected_type {
            // Simple types
            Type::Simple(type_name) => self.matches_simple_type(value, type_name),

            // Generic types (e.g., List[int], Dict[str, int])
            Type::Generic { name, args } => self.matches_generic_type(value, name, args),

            // Union types (e.g., int | str)
            Type::Union(types) => {
                types.iter().any(|t| self.value_matches_type(value, t))
            }

            // Optional types (e.g., Optional[int] or int?)
            Type::Optional(inner_type) => {
                matches!(value, Value::None) || self.value_matches_type(value, inner_type)
            }

            // Tuple types
            Type::Tuple(type_list) => {
                if let Value::Tuple(values) = value {
                    if values.len() != type_list.len() {
                        return false;
                    }
                    values.iter()
                        .zip(type_list.iter())
                        .all(|(val, ty)| self.value_matches_type(val, ty))
                } else {
                    false
                }
            }

            // Function types
            Type::Function { params, return_type } => {
                matches!(value, Value::Closure { .. } | Value::NativeFunction(_) | Value::BuiltinFunction(_, _))
            }

            // Any type matches everything
            Type::Any => true,

            // For other complex types, fall back to basic checking
            _ => true, // TypeVar and Protocol are advanced features
        }
    }

    /// Check if a value matches a simple type name
    fn matches_simple_type(&self, value: &Value, type_name: &str) -> bool {
        match (value, type_name) {
            (Value::Int(_), "int") => true,
            (Value::Float(_), "float") => true,
            (Value::Str(_), "str") => true,
            (Value::Bool(_), "bool") => true,
            (Value::List(_), "list") => true,
            (Value::Dict(_), "dict") => true,
            (Value::Tuple(_), "tuple") => true,
            (Value::Set(_), "set") => true,
            (Value::FrozenSet(_), "frozenset") => true,
            (Value::Bytes(_), "bytes") => true,
            (Value::ByteArray(_), "bytearray") => true,
            (Value::Complex { .. }, "complex") => true,
            (Value::None, "None" | "NoneType") => true,
            (Value::Range { .. }, "range") => true,

            // Check for numeric conversions
            (Value::Bool(_), "int") => true, // bool is subtype of int in Python
            (Value::Int(_), "float") => false, // int is not automatically float
            (Value::Float(_), "int") => false, // float is not int

            // Check class instances
            (Value::Object { class_name, .. }, expected) => {
                class_name == expected || self.is_subclass(class_name, expected)
            }

            // Check class types
            (Value::Class { name, .. }, expected) => {
                name == expected
            }

            _ => false,
        }
    }

    /// Check if a value matches a generic type
    fn matches_generic_type(&self, value: &Value, name: &str, args: &[Type]) -> bool {
        match (value, name) {
            // List[T]
            (Value::List(items), "List" | "list") => {
                if args.is_empty() {
                    return true; // List without type parameter
                }
                if let Some(element_type) = args.first() {
                    items.iter().all(|v| self.value_matches_type(v, element_type))
                } else {
                    true
                }
            }

            // Dict[K, V]
            (Value::Dict(dict), "Dict" | "dict") => {
                if args.len() < 2 {
                    return true; // Dict without full type parameters
                }
                let key_type = &args[0];
                let value_type = &args[1];

                // Check all keys and values match their types
                dict.iter().all(|(k, v)| {
                    let key_value = Value::Str(k.clone());
                    self.value_matches_type(&key_value, key_type) &&
                    self.value_matches_type(v, value_type)
                })
            }

            // Tuple[T1, T2, ...]
            (Value::Tuple(values), "Tuple" | "tuple") => {
                if args.is_empty() {
                    return true; // Tuple without type parameters
                }
                if values.len() != args.len() {
                    return false;
                }
                values.iter()
                    .zip(args.iter())
                    .all(|(val, ty)| self.value_matches_type(val, ty))
            }

            // Set[T] and FrozenSet[T]
            (Value::Set(items) | Value::FrozenSet(items), "Set" | "set" | "FrozenSet" | "frozenset") => {
                if args.is_empty() {
                    return true; // Set without type parameter
                }
                if let Some(element_type) = args.first() {
                    items.iter().all(|v| self.value_matches_type(v, element_type))
                } else {
                    true
                }
            }

            // Callable[[Arg1, Arg2, ...], ReturnType] - function types
            (Value::Closure { .. } | Value::NativeFunction(_) | Value::BuiltinFunction(_, _), "Callable") => {
                // Basic callable type matching
                true
            }

            _ => false,
        }
    }

    /// Check if a class is a subclass of another (simplified version)
    fn is_subclass(&self, class_name: &str, expected: &str) -> bool {
        // TODO: Implement proper class hierarchy checking using MRO
        // For now, just check if they're the same
        class_name == expected
    }

    /// Validate variable assignment with type checking
    pub fn check_variable_assignment(
        &self,
        var_name: &str,
        value: &Value,
    ) -> Result<()> {
        if let Some(expected_type) = self.type_env.get_variable_type(var_name) {
            self.check_type(value, expected_type)?;
        }
        Ok(())
    }

    /// Validate function call with type checking for parameters
    pub fn check_function_call(
        &self,
        func_name: &str,
        args: &[Value],
    ) -> Result<()> {
        if let Some(func_type) = self.type_env.get_function_type(func_name) {
            // Check parameter types
            for (i, (param_name, param_type)) in func_type.param_types.iter().enumerate() {
                if let Some(expected_type) = param_type {
                    if let Some(arg) = args.get(i) {
                        self.check_type(arg, expected_type)
                            .map_err(|_| anyhow!(
                                "TypeError: Parameter '{}' of function '{}' expects type '{}', but got '{}'",
                                param_name,
                                func_name,
                                format_type(expected_type),
                                arg.type_name()
                            ))?;
                    }
                }
            }
        }
        Ok(())
    }

    /// Validate function return value with type checking
    pub fn check_function_return(
        &self,
        func_name: &str,
        return_value: &Value,
    ) -> Result<()> {
        if let Some(func_type) = self.type_env.get_function_type(func_name) {
            if let Some(return_type) = &func_type.return_type {
                self.check_type(return_value, return_type)
                    .map_err(|_| anyhow!(
                        "TypeError: Function '{}' expects return type '{}', but got '{}'",
                        func_name,
                        format_type(return_type),
                        return_value.type_name()
                    ))?;
            }
        }
        Ok(())
    }

    /// Validate class attribute assignment with type checking
    pub fn check_attribute_assignment(
        &self,
        class_name: &str,
        attr_name: &str,
        value: &Value,
    ) -> Result<()> {
        if let Some(class_type) = self.type_env.get_class_type(class_name) {
            if let Some(expected_type) = class_type.attribute_types.get(attr_name) {
                self.check_type(value, expected_type)
                    .map_err(|_| anyhow!(
                        "TypeError: Attribute '{}' of class '{}' expects type '{}', but got '{}'",
                        attr_name,
                        class_name,
                        format_type(expected_type),
                        value.type_name()
                    ))?;
            }
        }
        Ok(())
    }
}

impl Default for TypeChecker {
    fn default() -> Self {
        Self::new()
    }
}

/// Format a type for error messages
pub fn format_type(ty: &Type) -> String {
    match ty {
        Type::Simple(name) => name.clone(),
        Type::Generic { name, args } => {
            let args_str = args.iter()
                .map(format_type)
                .collect::<Vec<_>>()
                .join(", ");
            format!("{}[{}]", name, args_str)
        }
        Type::Union(types) => {
            types.iter()
                .map(format_type)
                .collect::<Vec<_>>()
                .join(" | ")
        }
        Type::Optional(inner) => {
            format!("Optional[{}]", format_type(inner))
        }
        Type::Tuple(types) => {
            let types_str = types.iter()
                .map(format_type)
                .collect::<Vec<_>>()
                .join(", ");
            format!("Tuple[{}]", types_str)
        }
        Type::Any => "Any".to_string(),
        Type::Function { params, return_type } => {
            let params_str = params.iter()
                .map(format_type)
                .collect::<Vec<_>>()
                .join(", ");
            format!("({}) -> {}", params_str, format_type(return_type))
        }
        Type::Literal(expr) => format!("Literal({:?})", expr),
        Type::TypeVar { name, .. } => name.clone(),
        Type::Protocol { name, .. } => format!("Protocol({})", name),
    }
}

/// Create a nice error message for type mismatches
pub fn type_error_message(
    var_name: &str,
    expected_type: &Type,
    actual_value: &Value,
) -> String {
    format!(
        "TypeError: Cannot assign value of type '{}' to variable '{}' with declared type '{}'",
        actual_value.type_name(),
        var_name,
        format_type(expected_type)
    )
}

/// Infer the type from a value at runtime
pub fn infer_type_from_value(value: &Value) -> Type {
    match value {
        Value::Int(_) => Type::Simple("int".to_string()),
        Value::Float(_) => Type::Simple("float".to_string()),
        Value::Str(_) => Type::Simple("str".to_string()),
        Value::Bool(_) => Type::Simple("bool".to_string()),
        Value::None => Type::Simple("None".to_string()),
        Value::Bytes(_) => Type::Simple("bytes".to_string()),
        Value::ByteArray(_) => Type::Simple("bytearray".to_string()),
        Value::Complex { .. } => Type::Simple("complex".to_string()),
        Value::Range { .. } => Type::Simple("range".to_string()),

        // For collections, infer the element types
        Value::List(items) => {
            if items.is_empty() {
                Type::Generic {
                    name: "List".to_string(),
                    args: vec![Type::Any],
                }
            } else {
                // Infer from first element
                let elem_type = infer_type_from_value(&items.get(0).unwrap_or(&Value::None));
                Type::Generic {
                    name: "List".to_string(),
                    args: vec![elem_type],
                }
            }
        }

        Value::Dict(dict) => {
            if dict.is_empty() {
                Type::Generic {
                    name: "Dict".to_string(),
                    args: vec![Type::Any, Type::Any],
                }
            } else {
                // All dict keys are strings in our implementation
                let key_type = Type::Simple("str".to_string());
                // Infer value type from first entry
                let value_type = dict.values().next()
                    .map(infer_type_from_value)
                    .unwrap_or(Type::Any);
                Type::Generic {
                    name: "Dict".to_string(),
                    args: vec![key_type, value_type],
                }
            }
        }

        Value::Tuple(items) => {
            if items.is_empty() {
                Type::Tuple(vec![])
            } else {
                let elem_types: Vec<Type> = items.iter()
                    .map(infer_type_from_value)
                    .collect();
                Type::Tuple(elem_types)
            }
        }

        Value::Set(items) | Value::FrozenSet(items) => {
            if items.is_empty() {
                Type::Generic {
                    name: "Set".to_string(),
                    args: vec![Type::Any],
                }
            } else {
                let elem_type = infer_type_from_value(&items[0]);
                Type::Generic {
                    name: "Set".to_string(),
                    args: vec![elem_type],
                }
            }
        }

        Value::Closure { .. } | Value::NativeFunction(_) | Value::BuiltinFunction(_, _) => {
            Type::Simple("Callable".to_string())
        }

        Value::Object { class_name, .. } => {
            Type::Simple(class_name.clone())
        }

        Value::Class { name, .. } => {
            Type::Simple(format!("type[{}]", name))
        }

        _ => Type::Any,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::ast::Type;
    use crate::value::Value;

    #[test]
    fn test_simple_type_checking() {
        let checker = TypeChecker::new();

        // int
        assert!(checker.value_matches_type(&Value::Int(42), &Type::Simple("int".to_string())));
        assert!(!checker.value_matches_type(&Value::Str("hello".to_string()), &Type::Simple("int".to_string())));

        // str
        assert!(checker.value_matches_type(&Value::Str("hello".to_string()), &Type::Simple("str".to_string())));
        assert!(!checker.value_matches_type(&Value::Int(42), &Type::Simple("str".to_string())));

        // bool (is subtype of int in Python)
        assert!(checker.value_matches_type(&Value::Bool(true), &Type::Simple("bool".to_string())));
        assert!(checker.value_matches_type(&Value::Bool(true), &Type::Simple("int".to_string())));
    }

    #[test]
    fn test_union_types() {
        let checker = TypeChecker::new();

        let union_type = Type::Union(vec![
            Type::Simple("int".to_string()),
            Type::Simple("str".to_string()),
        ]);

        assert!(checker.value_matches_type(&Value::Int(42), &union_type));
        assert!(checker.value_matches_type(&Value::Str("hello".to_string()), &union_type));
        assert!(!checker.value_matches_type(&Value::Float(3.14), &union_type));
    }

    #[test]
    fn test_optional_types() {
        let checker = TypeChecker::new();

        let optional_int = Type::Optional(Box::new(Type::Simple("int".to_string())));

        assert!(checker.value_matches_type(&Value::Int(42), &optional_int));
        assert!(checker.value_matches_type(&Value::None, &optional_int));
        assert!(!checker.value_matches_type(&Value::Str("hello".to_string()), &optional_int));
    }

    #[test]
    fn test_generic_list_type() {
        let checker = TypeChecker::new();

        let list_of_ints = Type::Generic {
            name: "List".to_string(),
            args: vec![Type::Simple("int".to_string())],
        };

        let valid_list = Value::new_list(vec![Value::Int(1), Value::Int(2), Value::Int(3)]);
        let invalid_list = Value::new_list(vec![Value::Int(1), Value::Str("two".to_string())]);

        assert!(checker.value_matches_type(&valid_list, &list_of_ints));
        assert!(!checker.value_matches_type(&invalid_list, &list_of_ints));
    }

    #[test]
    fn test_any_type() {
        let checker = TypeChecker::new();

        assert!(checker.value_matches_type(&Value::Int(42), &Type::Any));
        assert!(checker.value_matches_type(&Value::Str("hello".to_string()), &Type::Any));
        assert!(checker.value_matches_type(&Value::None, &Type::Any));
    }

    #[test]
    fn test_type_inference() {
        assert!(matches!(infer_type_from_value(&Value::Int(42)), Type::Simple(s) if s == "int"));
        assert!(matches!(infer_type_from_value(&Value::Str("hello".to_string())), Type::Simple(s) if s == "str"));

        let list = Value::new_list(vec![Value::Int(1), Value::Int(2)]);
        assert!(matches!(infer_type_from_value(&list), Type::Generic { name, .. } if name == "List"));
    }
}
