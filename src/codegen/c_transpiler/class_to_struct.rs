//! Class to Struct Converter
//!
//! Converts Tauraro classes to high-performance C structs with vtables for methods

use crate::ast::{Statement, Expr};
use crate::codegen::c_transpiler::native_types::{NativeType, ClassInfo, MethodInfo};
use std::collections::HashMap;

/// Converts a Tauraro class definition to a C struct with vtable
pub struct ClassToStructConverter {
    /// Maps Tauraro type annotations to native C types
    type_map: HashMap<String, NativeType>,
}

impl ClassToStructConverter {
    pub fn new() -> Self {
        let mut type_map = HashMap::new();
        type_map.insert("int".to_string(), NativeType::Int);
        type_map.insert("float".to_string(), NativeType::Float);
        type_map.insert("bool".to_string(), NativeType::Bool);
        type_map.insert("str".to_string(), NativeType::String);

        Self { type_map }
    }

    /// Convert a class definition to C struct code
    pub fn convert_class(&self, class_def: &Statement) -> Result<String, String> {
        // Note: This is a placeholder - full implementation would extract from Statement::ClassDef
        if let Statement::ClassDef { name, .. } = class_def {
        let mut code = String::new();

            let struct_name = format!("{}_t", name);
            code.push_str(&format!("// Class {}\n", name));
            code.push_str(&format!("struct {} {{\n    int ref_count;\n}};\n", struct_name));
            Ok(code)
        } else {
            Err("Not a ClassDef statement".to_string())
        }
    }

    /// Map Tauraro type annotation to native type
    fn map_tauraro_type_to_native(&self, typ: &crate::ast::Type) -> Option<NativeType> {
        match typ {
            crate::ast::Type::Simple(s) => match s.as_str() {
                "int" => Some(NativeType::Int),
                "float" => Some(NativeType::Float),
                "bool" => Some(NativeType::Bool),
                "str" => Some(NativeType::String),
                name => Some(NativeType::Struct(name.to_string())),
            },
            crate::ast::Type::Generic { name, args } => {
                if name == "list" && !args.is_empty() {
                    let inner = self.map_tauraro_type_to_native(&args[0])
                        .unwrap_or(NativeType::Dynamic);
                    Some(NativeType::List(Box::new(inner)))
                } else {
                    None
                }
            }
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_class_converter_creation() {
        let converter = ClassToStructConverter::new();
        assert!(converter.type_map.contains_key("int"));
        assert!(converter.type_map.contains_key("str"));
    }
}
