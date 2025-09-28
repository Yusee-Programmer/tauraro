use crate::value::Value;
use std::collections::HashMap;

pub fn create_unittest_module() -> Value {
    let namespace = HashMap::new();
    // TODO: Implement unittest functions
    Value::Module("unittest".to_string(), namespace)
}
