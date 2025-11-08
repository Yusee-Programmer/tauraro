//! Native C Type System for High-Performance Transpilation
//!
//! This module implements a native C type system that uses direct C types
//! instead of boxed Tauraro values for maximum performance.

use std::collections::HashMap;

/// Represents a native C type (non-boxed)
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum NativeType {
    /// Native 64-bit integer
    Int,
    /// Native double-precision float
    Float,
    /// Native boolean
    Bool,
    /// Native C string (char*)
    String,
    /// Native void type
    Void,
    /// Native pointer to struct
    Struct(String),
    /// Native array with element type and size (if known)
    Array(Box<NativeType>, Option<usize>),
    /// Dynamic list (requires runtime support)
    List(Box<NativeType>),
    /// Dynamic dictionary (requires runtime support)
    Dict(Box<NativeType>, Box<NativeType>),
    /// Function pointer with parameters and return type
    Function(Vec<NativeType>, Box<NativeType>),
    /// Unknown/dynamic type (falls back to boxed)
    Dynamic,
}

impl NativeType {
    /// Get the C type string for this native type
    pub fn to_c_type(&self) -> String {
        match self {
            NativeType::Int => "int64_t".to_string(),
            NativeType::Float => "double".to_string(),
            NativeType::Bool => "bool".to_string(),
            NativeType::String => "char*".to_string(),
            NativeType::Void => "void".to_string(),
            NativeType::Struct(name) => format!("struct {}*", name),
            NativeType::Array(elem_type, Some(size)) => {
                format!("{}[{}]", elem_type.to_c_type(), size)
            }
            NativeType::Array(elem_type, None) => {
                format!("{}*", elem_type.to_c_type())
            }
            NativeType::List(_) => "tauraro_native_list_t*".to_string(),
            NativeType::Dict(_, _) => "tauraro_native_dict_t*".to_string(),
            NativeType::Function(_, _) => "void*".to_string(), // Function pointer
            NativeType::Dynamic => "tauraro_value_t*".to_string(), // Fallback to boxed
        }
    }

    /// Check if this type is a primitive that doesn't need heap allocation
    pub fn is_primitive(&self) -> bool {
        matches!(self, NativeType::Int | NativeType::Float | NativeType::Bool)
    }

    /// Check if this type can be used natively (no boxing required)
    pub fn is_native(&self) -> bool {
        !matches!(self, NativeType::Dynamic)
    }

    /// Get the default value for this type
    pub fn default_value(&self) -> String {
        match self {
            NativeType::Int => "0".to_string(),
            NativeType::Float => "0.0".to_string(),
            NativeType::Bool => "false".to_string(),
            NativeType::String => "NULL".to_string(),
            NativeType::Void => "".to_string(),
            _ => "NULL".to_string(),
        }
    }
}

/// Native list implementation
pub fn generate_native_list_type() -> String {
    r#"
// Native dynamic list (similar to C++ vector)
typedef struct tauraro_native_list {
    void** items;
    size_t size;
    size_t capacity;
    size_t item_size; // Size of each element
} tauraro_native_list_t;

tauraro_native_list_t* tauraro_native_list_new(size_t item_size) {
    tauraro_native_list_t* list = malloc(sizeof(tauraro_native_list_t));
    list->capacity = 8;
    list->size = 0;
    list->item_size = item_size;
    list->items = malloc(list->capacity * sizeof(void*));
    return list;
}

void tauraro_native_list_append(tauraro_native_list_t* list, void* item) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->items = realloc(list->items, list->capacity * sizeof(void*));
    }
    list->items[list->size++] = item;
}

void* tauraro_native_list_get(tauraro_native_list_t* list, int64_t index) {
    if (index < 0) index += list->size;
    if (index >= 0 && (size_t)index < list->size) {
        return list->items[index];
    }
    return NULL;
}

void tauraro_native_list_set(tauraro_native_list_t* list, int64_t index, void* item) {
    if (index < 0) index += list->size;
    if (index >= 0 && (size_t)index < list->size) {
        list->items[index] = item;
    }
}

void tauraro_native_list_free(tauraro_native_list_t* list) {
    if (list) {
        free(list->items);
        free(list);
    }
}
"#.to_string()
}

/// Native dictionary implementation using hash table
pub fn generate_native_dict_type() -> String {
    r#"
// Native dictionary (hash table)
typedef struct tauraro_dict_entry {
    char* key;
    void* value;
    struct tauraro_dict_entry* next;
} tauraro_dict_entry_t;

typedef struct tauraro_native_dict {
    tauraro_dict_entry_t** buckets;
    size_t capacity;
    size_t size;
} tauraro_native_dict_t;

static unsigned long tauraro_hash_string(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

tauraro_native_dict_t* tauraro_native_dict_new() {
    tauraro_native_dict_t* dict = malloc(sizeof(tauraro_native_dict_t));
    dict->capacity = 16;
    dict->size = 0;
    dict->buckets = calloc(dict->capacity, sizeof(tauraro_dict_entry_t*));
    return dict;
}

void tauraro_native_dict_set(tauraro_native_dict_t* dict, const char* key, void* value) {
    unsigned long hash = tauraro_hash_string(key);
    size_t index = hash % dict->capacity;

    // Check if key exists
    tauraro_dict_entry_t* entry = dict->buckets[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    // Add new entry
    tauraro_dict_entry_t* new_entry = malloc(sizeof(tauraro_dict_entry_t));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = dict->buckets[index];
    dict->buckets[index] = new_entry;
    dict->size++;
}

void* tauraro_native_dict_get(tauraro_native_dict_t* dict, const char* key) {
    unsigned long hash = tauraro_hash_string(key);
    size_t index = hash % dict->capacity;

    tauraro_dict_entry_t* entry = dict->buckets[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void tauraro_native_dict_free(tauraro_native_dict_t* dict) {
    if (dict) {
        for (size_t i = 0; i < dict->capacity; i++) {
            tauraro_dict_entry_t* entry = dict->buckets[i];
            while (entry) {
                tauraro_dict_entry_t* next = entry->next;
                free(entry->key);
                free(entry);
                entry = next;
            }
        }
        free(dict->buckets);
        free(dict);
    }
}
"#.to_string()
}

/// Generate native type declarations
pub fn generate_native_type_declarations() -> String {
    let mut code = String::new();

    code.push_str("// Native type system for high-performance compilation\n");
    code.push_str("#include <stdint.h>\n");
    code.push_str("#include <stdbool.h>\n");
    code.push_str("#include <string.h>\n");
    code.push_str("#include <stdlib.h>\n\n");

    code.push_str(&generate_native_list_type());
    code.push_str(&generate_native_dict_type());

    code
}

/// Context for tracking variable types during transpilation
pub struct NativeTypeContext {
    /// Maps variable names to their inferred native types
    variables: HashMap<String, NativeType>,
    /// Maps function names to their signatures
    functions: HashMap<String, (Vec<NativeType>, NativeType)>,
    /// Maps class names to their struct definitions
    classes: HashMap<String, ClassInfo>,
}

/// Information about a class converted to a struct
#[derive(Debug, Clone)]
pub struct ClassInfo {
    pub name: String,
    pub fields: Vec<(String, NativeType)>,
    pub methods: Vec<MethodInfo>,
    pub base_class: Option<String>,
}

/// Information about a method
#[derive(Debug, Clone)]
pub struct MethodInfo {
    pub name: String,
    pub params: Vec<(String, NativeType)>,
    pub return_type: NativeType,
}

impl NativeTypeContext {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
            functions: HashMap::new(),
            classes: HashMap::new(),
        }
    }

    pub fn set_variable_type(&mut self, name: String, typ: NativeType) {
        self.variables.insert(name, typ);
    }

    pub fn get_variable_type(&self, name: &str) -> Option<&NativeType> {
        self.variables.get(name)
    }

    pub fn set_function_type(&mut self, name: String, params: Vec<NativeType>, return_type: NativeType) {
        self.functions.insert(name, (params, return_type));
    }

    pub fn get_function_type(&self, name: &str) -> Option<&(Vec<NativeType>, NativeType)> {
        self.functions.get(name)
    }

    pub fn add_class(&mut self, class_info: ClassInfo) {
        self.classes.insert(class_info.name.clone(), class_info);
    }

    pub fn get_class(&self, name: &str) -> Option<&ClassInfo> {
        self.classes.get(name)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_native_type_to_c() {
        assert_eq!(NativeType::Int.to_c_type(), "int64_t");
        assert_eq!(NativeType::Float.to_c_type(), "double");
        assert_eq!(NativeType::Bool.to_c_type(), "bool");
        assert_eq!(NativeType::String.to_c_type(), "char*");
    }

    #[test]
    fn test_is_primitive() {
        assert!(NativeType::Int.is_primitive());
        assert!(NativeType::Float.is_primitive());
        assert!(NativeType::Bool.is_primitive());
        assert!(!NativeType::String.is_primitive());
    }
}
