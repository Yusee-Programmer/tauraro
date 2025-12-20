//! Type system for Rust code generation

use std::collections::HashMap;

/// Native Rust type representation
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum RustType {
    Unit,
    Bool,
    I8,
    I16,
    I32,
    I64,
    I128,
    U8,
    U16,
    U32,
    U64,
    U128,
    F32,
    F64,
    String,
    Str,
    Vec(Box<RustType>),
    HashMap(Box<RustType>, Box<RustType>),
    Tuple(Vec<RustType>),
    Option(Box<RustType>),
    Result(Box<RustType>, Box<RustType>),
    Pointer(Box<RustType>),
    MutPointer(Box<RustType>),
    Reference(Box<RustType>),
    MutReference(Box<RustType>),
    Custom(String),
    TauObject,
    Trait(String),
    Generic(String),
}

impl RustType {
    /// Convert to Rust type string
    pub fn to_string(&self) -> String {
        match self {
            RustType::Unit => "()".to_string(),
            RustType::Bool => "bool".to_string(),
            RustType::I8 => "i8".to_string(),
            RustType::I16 => "i16".to_string(),
            RustType::I32 => "i32".to_string(),
            RustType::I64 => "i64".to_string(),
            RustType::I128 => "i128".to_string(),
            RustType::U8 => "u8".to_string(),
            RustType::U16 => "u16".to_string(),
            RustType::U32 => "u32".to_string(),
            RustType::U64 => "u64".to_string(),
            RustType::U128 => "u128".to_string(),
            RustType::F32 => "f32".to_string(),
            RustType::F64 => "f64".to_string(),
            RustType::String => "String".to_string(),
            RustType::Str => "&str".to_string(),
            RustType::Vec(inner) => format!("Vec<{}>", inner.to_string()),
            RustType::HashMap(key, val) => format!("HashMap<{}, {}>", key.to_string(), val.to_string()),
            RustType::Tuple(types) => {
                let type_strs = types.iter().map(|t| t.to_string()).collect::<Vec<_>>();
                format!("({})", type_strs.join(", "))
            }
            RustType::Option(inner) => format!("Option<{}>", inner.to_string()),
            RustType::Result(ok, err) => format!("Result<{}, {}>", ok.to_string(), err.to_string()),
            RustType::Pointer(inner) => format!("*const {}", inner.to_string()),
            RustType::MutPointer(inner) => format!("*mut {}", inner.to_string()),
            RustType::Reference(inner) => format!("&{}", inner.to_string()),
            RustType::MutReference(inner) => format!("&mut {}", inner.to_string()),
            RustType::Custom(name) => name.clone(),
            RustType::TauObject => "TauObject".to_string(),
            RustType::Trait(name) => format!("dyn {}", name),
            RustType::Generic(name) => name.clone(),
        }
    }

    /// Create a Vec of a type
    pub fn vec_of(inner: RustType) -> Self {
        RustType::Vec(Box::new(inner))
    }

    /// Create a reference to a type
    pub fn ref_of(inner: RustType) -> Self {
        RustType::Reference(Box::new(inner))
    }

    /// Create a mutable reference to a type
    pub fn mut_ref_of(inner: RustType) -> Self {
        RustType::MutReference(Box::new(inner))
    }

    /// Create an Option of a type
    pub fn option_of(inner: RustType) -> Self {
        RustType::Option(Box::new(inner))
    }

    /// Create a Result of a type
    pub fn result_of(ok: RustType, err: RustType) -> Self {
        RustType::Result(Box::new(ok), Box::new(err))
    }

    /// Map Python types to Rust types
    pub fn from_python_type(py_type: &str) -> Self {
        match py_type {
            "int" => RustType::I64,
            "float" => RustType::F64,
            "bool" => RustType::Bool,
            "str" => RustType::String,
            "list" => RustType::vec_of(RustType::TauObject),
            "dict" => RustType::HashMap(
                Box::new(RustType::String),
                Box::new(RustType::TauObject)
            ),
            "tuple" => RustType::Tuple(vec![RustType::TauObject]),
            "set" => RustType::Custom("HashSet<TauObject>".to_string()),
            "None" => RustType::Unit,
            _ => RustType::Custom(py_type.to_string()),
        }
    }
}

impl std::fmt::Display for RustType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.to_string())
    }
}

/// Type context for tracking types during code generation
#[derive(Debug, Clone)]
pub struct TypeContext {
    /// Variable type bindings
    pub bindings: HashMap<String, RustType>,
    /// Function signatures
    pub functions: HashMap<String, (Vec<RustType>, RustType)>,
    /// Class fields
    pub classes: HashMap<String, Vec<(String, RustType)>>,
}

impl TypeContext {
    pub fn new() -> Self {
        Self {
            bindings: HashMap::new(),
            functions: HashMap::new(),
            classes: HashMap::new(),
        }
    }

    pub fn bind_variable(&mut self, name: String, ty: RustType) {
        self.bindings.insert(name, ty);
    }

    pub fn bind_function(&mut self, name: String, params: Vec<RustType>, return_type: RustType) {
        self.functions.insert(name, (params, return_type));
    }

    pub fn bind_class(&mut self, name: String, fields: Vec<(String, RustType)>) {
        self.classes.insert(name, fields);
    }

    pub fn get_variable_type(&self, name: &str) -> Option<RustType> {
        self.bindings.get(name).cloned()
    }

    pub fn get_function_signature(&self, name: &str) -> Option<(Vec<RustType>, RustType)> {
        self.functions.get(name).cloned()
    }

    pub fn get_class_fields(&self, name: &str) -> Option<Vec<(String, RustType)>> {
        self.classes.get(name).cloned()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_type_to_string() {
        assert_eq!(RustType::Bool.to_string(), "bool");
        assert_eq!(RustType::I64.to_string(), "i64");
        assert_eq!(RustType::String.to_string(), "String");
        assert_eq!(RustType::vec_of(RustType::I64).to_string(), "Vec<i64>");
    }

    #[test]
    fn test_python_to_rust_type_mapping() {
        assert_eq!(RustType::from_python_type("int"), RustType::I64);
        assert_eq!(RustType::from_python_type("float"), RustType::F64);
        assert_eq!(RustType::from_python_type("bool"), RustType::Bool);
        assert_eq!(RustType::from_python_type("str"), RustType::String);
    }
}
