use crate::ir::{IRModule, IRFunction, IRInstruction, IRValue, IRType};
use anyhow::anyhow;
use std::fmt::Write;
use std::collections::HashMap;

pub struct CTranspiler {
    output: String,
    indent_level: usize,
    temp_counter: usize,
    label_counter: usize,
    function_declarations: Vec<String>,
}

impl CTranspiler {
    pub fn new() -> Self {
        Self {
            output: String::new(),
            indent_level: 0,
            temp_counter: 0,
            label_counter: 0,
            function_declarations: Vec::new(),
        }
    }

    pub fn transpile(&mut self, module: &IRModule) -> anyhow::Result<String> {
        
        
        // Generate includes
        writeln!(self.output, "#include <stdio.h>")?;
        writeln!(self.output, "#include <stdlib.h>")?;
        writeln!(self.output, "#include <string.h>")?;
        writeln!(self.output, "#include <stdbool.h>")?;
        writeln!(self.output, "#include <stdint.h>")?;
        writeln!(self.output, "#include <math.h>")?;
        writeln!(self.output, "#include <setjmp.h>")?;
        writeln!(self.output)?;

        // Generate runtime support functions
        writeln!(self.output, "// Runtime support functions")?;
        writeln!(self.output, "void* tauraro_null() {{ return NULL; }}")?;
        writeln!(self.output, "int tauraro_len(void* obj) {{ return 0; /* TODO: implement */ }}")?;
        writeln!(self.output, "char* tauraro_type(void* obj) {{ return \"object\"; /* TODO: implement */ }}")?;
        writeln!(self.output, "void tauraro_print(const char* str) {{ printf(\"%s\\n\", str); }}")?;
        writeln!(self.output, "void tauraro_print_int(int64_t val) {{ printf(\"%lld\\n\", val); }}")?;
        writeln!(self.output, "void tauraro_print_float(double val) {{ printf(\"%f\\n\", val); }}")?;
        writeln!(self.output, "void tauraro_print_bool(bool val) {{ printf(\"%s\\n\", val ? \"true\" : \"false\"); }}")?;
        writeln!(self.output, "void* tauraro_super() {{ return (void*)0x1; /* Special value for super calls */ }}")?;
        writeln!(self.output, "void* tauraro_super_method_call(const char* method_name, void* self_obj, void** args, int arg_count) {{")?;
        writeln!(self.output, "    // In a full implementation, this would resolve the method according to the MRO")?;
        writeln!(self.output, "    // For now, we'll just return NULL to indicate the method wasn't found")?;
        writeln!(self.output, "    printf(\"Super method call: %s\\n\", method_name);")?;
        writeln!(self.output, "    return NULL;")?;
        writeln!(self.output, "}}")?;
        
        // Data structure runtime functions
        writeln!(self.output, "// List support")?;
        writeln!(self.output, "typedef struct {{")?;
        writeln!(self.output, "    int size;")?;
        writeln!(self.output, "    int capacity;")?;
        writeln!(self.output, "    void** elements;")?;
        writeln!(self.output, "}} TauraroList;")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_create_list(int size) {{")?;
        writeln!(self.output, "    TauraroList* list = (TauraroList*)malloc(sizeof(TauraroList));")?;
        writeln!(self.output, "    list->size = size;")?;
        writeln!(self.output, "    list->capacity = size > 4 ? size : 4;")?;
        writeln!(self.output, "    list->elements = (void**)malloc(list->capacity * sizeof(void*));")?;
        writeln!(self.output, "    return (void*)list;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_list_set(void* list_ptr, int index, void* value) {{")?;
        writeln!(self.output, "    TauraroList* list = (TauraroList*)list_ptr;")?;
        writeln!(self.output, "    if (index >= 0 && index < list->size) {{")?;
        writeln!(self.output, "        list->elements[index] = value;")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_list_get(void* list_ptr, int index) {{")?;
        writeln!(self.output, "    TauraroList* list = (TauraroList*)list_ptr;")?;
        writeln!(self.output, "    if (index >= 0 && index < list->size) {{")?;
        writeln!(self.output, "        return list->elements[index];")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    return NULL;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_list_append(void* list_ptr, void* value) {{")?;
        writeln!(self.output, "    TauraroList* list = (TauraroList*)list_ptr;")?;
        writeln!(self.output, "    if (list->size >= list->capacity) {{")?;
        writeln!(self.output, "        list->capacity *= 2;")?;
        writeln!(self.output, "        list->elements = (void**)realloc(list->elements, list->capacity * sizeof(void*));")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    list->elements[list->size] = value;")?;
        writeln!(self.output, "    list->size++;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        writeln!(self.output, "// Dictionary support")?;
        writeln!(self.output, "typedef struct {{")?;
        writeln!(self.output, "    int size;")?;
        writeln!(self.output, "    int capacity;")?;
        writeln!(self.output, "    void** keys;")?;
        writeln!(self.output, "    void** values;")?;
        writeln!(self.output, "}} TauraroDict;")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_create_dict(int size) {{")?;
        writeln!(self.output, "    TauraroDict* dict = (TauraroDict*)malloc(sizeof(TauraroDict));")?;
        writeln!(self.output, "    dict->size = 0;")?;
        writeln!(self.output, "    dict->capacity = size > 4 ? size : 4;")?;
        writeln!(self.output, "    dict->keys = (void**)malloc(dict->capacity * sizeof(void*));")?;
        writeln!(self.output, "    dict->values = (void**)malloc(dict->capacity * sizeof(void*));")?;
        writeln!(self.output, "    return (void*)dict;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_dict_set(void* dict_ptr, void* key, void* value) {{")?;
        writeln!(self.output, "    TauraroDict* dict = (TauraroDict*)dict_ptr;")?;
        writeln!(self.output, "    // Simple linear search for key (inefficient but works for now)")?;
        writeln!(self.output, "    int index = -1;")?;
        writeln!(self.output, "    for (int i = 0; i < dict->size; i++) {{")?;
        writeln!(self.output, "        if (dict->keys[i] == key) {{")?;
        writeln!(self.output, "            index = i;")?;
        writeln!(self.output, "            break;")?;
        writeln!(self.output, "        }}")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    if (index == -1) {{")?;
        writeln!(self.output, "        // Key not found, add new entry")?;
        writeln!(self.output, "        if (dict->size >= dict->capacity) {{")?;
        writeln!(self.output, "            dict->capacity *= 2;")?;
        writeln!(self.output, "            dict->keys = (void**)realloc(dict->keys, dict->capacity * sizeof(void*));")?;
        writeln!(self.output, "            dict->values = (void**)realloc(dict->values, dict->capacity * sizeof(void*));")?;
        writeln!(self.output, "        }}")?;
        writeln!(self.output, "        index = dict->size;")?;
        writeln!(self.output, "        dict->size++;")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    dict->keys[index] = key;")?;
        writeln!(self.output, "    dict->values[index] = value;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_dict_get(void* dict_ptr, void* key) {{")?;
        writeln!(self.output, "    TauraroDict* dict = (TauraroDict*)dict_ptr;")?;
        writeln!(self.output, "    for (int i = 0; i < dict->size; i++) {{")?;
        writeln!(self.output, "        if (dict->keys[i] == key) {{")?;
        writeln!(self.output, "            return dict->values[i];")?;
        writeln!(self.output, "        }}")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    return NULL;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        writeln!(self.output, "// Tuple support")?;
        writeln!(self.output, "typedef struct {{")?;
        writeln!(self.output, "    int size;")?;
        writeln!(self.output, "    void** elements;")?;
        writeln!(self.output, "}} TauraroTuple;")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_create_tuple(int size) {{")?;
        writeln!(self.output, "    TauraroTuple* tuple = (TauraroTuple*)malloc(sizeof(TauraroTuple));")?;
        writeln!(self.output, "    tuple->size = size;")?;
        writeln!(self.output, "    tuple->elements = (void**)malloc(size * sizeof(void*));")?;
        writeln!(self.output, "    return (void*)tuple;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_tuple_set(void* tuple_ptr, int index, void* value) {{")?;
        writeln!(self.output, "    TauraroTuple* tuple = (TauraroTuple*)tuple_ptr;")?;
        writeln!(self.output, "    if (index >= 0 && index < tuple->size) {{")?;
        writeln!(self.output, "        tuple->elements[index] = value;")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_tuple_get(void* tuple_ptr, int index) {{")?;
        writeln!(self.output, "    TauraroTuple* tuple = (TauraroTuple*)tuple_ptr;")?;
        writeln!(self.output, "    if (index >= 0 && index < tuple->size) {{")?;
        writeln!(self.output, "        return tuple->elements[index];")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    return NULL;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        writeln!(self.output, "// Set support")?;
        writeln!(self.output, "typedef struct {{")?;
        writeln!(self.output, "    int size;")?;
        writeln!(self.output, "    int capacity;")?;
        writeln!(self.output, "    void** elements;")?;
        writeln!(self.output, "}} TauraroSet;")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_create_set(int size) {{")?;
        writeln!(self.output, "    TauraroSet* set = (TauraroSet*)malloc(sizeof(TauraroSet));")?;
        writeln!(self.output, "    set->size = 0;")?;
        writeln!(self.output, "    set->capacity = size > 4 ? size : 4;")?;
        writeln!(self.output, "    set->elements = (void**)malloc(set->capacity * sizeof(void*));")?;
        writeln!(self.output, "    return (void*)set;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_set_add(void* set_ptr, void* value) {{")?;
        writeln!(self.output, "    TauraroSet* set = (TauraroSet*)set_ptr;")?;
        writeln!(self.output, "    // Check if value already exists")?;
        writeln!(self.output, "    for (int i = 0; i < set->size; i++) {{")?;
        writeln!(self.output, "        if (set->elements[i] == value) {{")?;
        writeln!(self.output, "            return; // Already exists")?;
        writeln!(self.output, "        }}")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    // Add new value")?;
        writeln!(self.output, "    if (set->size >= set->capacity) {{")?;
        writeln!(self.output, "        set->capacity *= 2;")?;
        writeln!(self.output, "        set->elements = (void**)realloc(set->elements, set->capacity * sizeof(void*));")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    set->elements[set->size] = value;")?;
        writeln!(self.output, "    set->size++;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "int tauraro_set_contains(void* set_ptr, void* value) {{")?;
        writeln!(self.output, "    TauraroSet* set = (TauraroSet*)set_ptr;")?;
        writeln!(self.output, "    for (int i = 0; i < set->size; i++) {{")?;
        writeln!(self.output, "        if (set->elements[i] == value) {{")?;
        writeln!(self.output, "            return 1;")?;
        writeln!(self.output, "        }}")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    return 0;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        // Object-oriented programming support
        writeln!(self.output, "// OOP support")?;
        writeln!(self.output, "typedef struct {{")?;
        writeln!(self.output, "    char* class_name;")?;
        writeln!(self.output, "    TauraroDict* attributes;")?;
        writeln!(self.output, "    TauraroDict* methods;")?;
        writeln!(self.output, "}} TauraroObject;")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_create_object(const char* class_name) {{")?;
        writeln!(self.output, "    TauraroObject* obj = (TauraroObject*)malloc(sizeof(TauraroObject));")?;
        writeln!(self.output, "    obj->class_name = strdup(class_name);")?;
        writeln!(self.output, "    obj->attributes = (TauraroDict*)tauraro_create_dict(4);")?;
        writeln!(self.output, "    obj->methods = (TauraroDict*)tauraro_create_dict(4);")?;
        writeln!(self.output, "    return (void*)obj;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_set_attr(void* obj_ptr, const char* attr, void* value) {{")?;
        writeln!(self.output, "    TauraroObject* obj = (TauraroObject*)obj_ptr;")?;
        writeln!(self.output, "    char* attr_key = strdup(attr);")?;
        writeln!(self.output, "    tauraro_dict_set((void*)obj->attributes, (void*)attr_key, value);")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_get_attr(void* obj_ptr, const char* attr) {{")?;
        writeln!(self.output, "    TauraroObject* obj = (TauraroObject*)obj_ptr;")?;
        writeln!(self.output, "    // In a real implementation, we would search the class hierarchy")?;
        writeln!(self.output, "    // For now, just search in the object's attributes")?;
        writeln!(self.output, "    for (int i = 0; i < obj->attributes->size; i++) {{")?;
        writeln!(self.output, "        char* key = (char*)obj->attributes->keys[i];")?;
        writeln!(self.output, "        if (strcmp(key, attr) == 0) {{")?;
        writeln!(self.output, "            return obj->attributes->values[i];")?;
        writeln!(self.output, "        }}")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    return NULL;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void* tauraro_get_item(void* obj_ptr, void* index) {{")?;
        writeln!(self.output, "    // For now, assume obj is a list or similar")?;
        writeln!(self.output, "    return tauraro_list_get(obj_ptr, (int)(intptr_t)index);")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_set_item(void* obj_ptr, void* index, void* value) {{")?;
        writeln!(self.output, "    // For now, assume obj is a list or similar")?;
        writeln!(self.output, "    tauraro_list_set(obj_ptr, (int)(intptr_t)index, value);")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        // Exception handling support
        writeln!(self.output, "// Exception handling support")?;
        writeln!(self.output, "jmp_buf tauraro_exception_jmp;")?;
        writeln!(self.output, "void* tauraro_current_exception = NULL;")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_exception_setup() {{")?;
        writeln!(self.output, "    // Initialize exception handling")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        writeln!(self.output, "void tauraro_raise_exception(void* exception) {{")?;
        writeln!(self.output, "    tauraro_current_exception = exception;")?;
        writeln!(self.output, "    longjmp(tauraro_exception_jmp, 1);")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        // String operations
        writeln!(self.output, "// String operations")?;
        writeln!(self.output, "char* tauraro_str_concat(const char* s1, const char* s2) {{")?;
        writeln!(self.output, "    if (s1 == NULL || s2 == NULL) return NULL;")?;
        writeln!(self.output, "    size_t len1 = strlen(s1);")?;
        writeln!(self.output, "    size_t len2 = strlen(s2);")?;
        writeln!(self.output, "    char* result = (char*)malloc(len1 + len2 + 1);")?;
        writeln!(self.output, "    strcpy(result, s1);")?;
        writeln!(self.output, "    strcat(result, s2);")?;
        writeln!(self.output, "    return result;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        writeln!(self.output, "char* tauraro_str_multiply(const char* s, int n) {{")?;
        writeln!(self.output, "    if (s == NULL || n <= 0) return strdup(\"\");")?;
        writeln!(self.output, "    size_t len = strlen(s);")?;
        writeln!(self.output, "    char* result = (char*)malloc(len * n + 1);")?;
        writeln!(self.output, "    result[0] = '\\0';")?;
        writeln!(self.output, "    for (int i = 0; i < n; i++) {{")?;
        writeln!(self.output, "        strcat(result, s);")?;
        writeln!(self.output, "    }}")?;
        writeln!(self.output, "    return result;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output, "")?;
        
        writeln!(self.output)?;

        // Collect function declarations first
        for (_, function) in &module.functions {
            self.collect_function_declaration(function)?;
        }

        // Write function declarations
        for decl in &self.function_declarations {
            writeln!(self.output, "{};", decl)?;
        }
        writeln!(self.output)?;

        // Generate function implementations
        for (_, function) in &module.functions {
            self.transpile_function(function, &module)?;
            writeln!(self.output)?;
        }

        Ok(self.output.clone())
    }

    fn collect_function_declaration(&mut self, function: &IRFunction) -> anyhow::Result<()> {
        let return_type = self.ir_type_to_c_type(&function.return_type);
        let mut params = Vec::new();
        
        for param in &function.params {
            let param_type = self.ir_type_to_c_type(&param.ty);
            params.push(format!("{} {}", param_type, param.name));
        }
        
        let params_str = if params.is_empty() {
            "void".to_string()
        } else {
            params.join(", ")
        };
        
        let declaration = format!("{} {}({})", return_type, function.name, params_str);
        self.function_declarations.push(declaration);
        Ok(())
    }

    fn transpile_function(&mut self, function: &IRFunction, module: &IRModule) -> anyhow::Result<()> {
        let return_type = self.ir_type_to_c_type(&function.return_type);
        let mut params = Vec::new();
        
        for param in &function.params {
            let param_type = self.ir_type_to_c_type(&param.ty);
            params.push(format!("{} {}", param_type, param.name));
        }
        
        let params_str = if params.is_empty() {
            "void".to_string()
        } else {
            params.join(", ")
        };
        
        writeln!(self.output, "{} {}({}) {{", return_type, function.name, params_str)?;
        self.indent_level += 1;

        // Generate function body from blocks
        for block in &function.blocks {
            writeln!(self.output, "{}:", block.label)?;
            for instruction in &block.instructions {
                let instruction_code = self.transpile_instruction(instruction, module)?;
                self.output.push_str(&instruction_code);
            }
        }

        self.indent_level -= 1;
        writeln!(self.output, "}}")?;
        Ok(())
    }

    fn transpile_instruction(&mut self, instruction: &IRInstruction, module: &IRModule) -> anyhow::Result<String> {
        let mut code = String::new();
        
        
        match instruction {
            IRInstruction::Alloca { dest, ty } => {
                let c_type = self.ir_type_to_c_type(ty);
                code.push_str(&format!("    {} {};\n", c_type, dest));
            }
            IRInstruction::Load { dest, ptr, ty } => {
                let c_type = self.ir_type_to_c_type(ty);
                code.push_str(&format!("    {} {} = *{};\n", c_type, dest, ptr));
            }
            IRInstruction::Store { ptr, value } => {
                let value_str = self.format_ir_value(value);
                // Check if ptr is a temporary variable that needs declaration
                if ptr.starts_with("tmp_") {
                    // Declare the temporary variable first
                    let c_type = self.infer_c_type_from_value(value);
                    code.push_str(&format!("    {} {} = {};\n", c_type, ptr, value_str));
                } else {
                    code.push_str(&format!("    {} = {};\n", ptr, value_str));
                }
            }
            IRInstruction::Add { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                
                // Check if this is string concatenation (string + string)
                match (left, right) {
                    (IRValue::String(_) | IRValue::Str(_) | IRValue::ImmediateString(_) | IRValue::ConstantString(_), 
                     IRValue::String(_) | IRValue::Str(_) | IRValue::ImmediateString(_) | IRValue::ConstantString(_)) => {
                        // String + string case
                        code.push_str(&format!("    {} = tauraro_str_concat({}, {});\n", dest, left_str, right_str));
                    }
                    _ => {
                        // Regular addition
                        code.push_str(&format!("    {} = {} + {};\n", dest, left_str, right_str));
                    }
                }
            }
            IRInstruction::Sub { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} - {};\n", dest, left_str, right_str));
            }
            IRInstruction::Mul { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                
                // Check if this is string multiplication (string * int or int * string)
                match (left, right) {
                    (IRValue::String(_) | IRValue::Str(_) | IRValue::ImmediateString(_) | IRValue::ConstantString(_), 
                     IRValue::Int(_) | IRValue::ImmediateInt(_) | IRValue::ConstantInt(_)) => {
                        // String * int case
                        code.push_str(&format!("    {} = tauraro_str_multiply({}, {});\n", dest, left_str, right_str));
                    }
                    (IRValue::Int(_) | IRValue::ImmediateInt(_) | IRValue::ConstantInt(_),
                     IRValue::String(_) | IRValue::Str(_) | IRValue::ImmediateString(_) | IRValue::ConstantString(_)) => {
                        // int * String case
                        code.push_str(&format!("    {} = tauraro_str_multiply({}, {});\n", dest, right_str, left_str));
                    }
                    _ => {
                        // Regular multiplication
                        code.push_str(&format!("    {} = {} * {};\n", dest, left_str, right_str));
                    }
                }
            }
            IRInstruction::Div { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} / {};\n", dest, left_str, right_str));
            }
            IRInstruction::Mod { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} % {};\n", dest, left_str, right_str));
            }
            IRInstruction::Pow { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = pow({}, {});\n", dest, left_str, right_str));
            }
            IRInstruction::FloorDiv { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = floor({} / {});\n", dest, left_str, right_str));
            }
            // Function calls
            IRInstruction::Call { dest, func, args } => {
                let args_str: Vec<String> = args.iter().map(|arg| self.format_ir_value(arg)).collect();
                let args_list = args_str.join(", ");
                
                // Check if this is a super method call
                if func.starts_with("super_") {
                    // Handle super method calls
                    let method_name = &func[6..]; // Remove "super_" prefix
                    code.push_str(&format!("    // Super method call: {}\n", method_name));
                    if let Some(dest_var) = dest {
                        // Declare destination variable if it's a temporary
                        if dest_var.starts_with("tmp_") {
                            code.push_str(&format!("    void* {};\n", dest_var));
                        }
                        code.push_str(&format!("    {} = tauraro_super_method_call(\"{}\", NULL, NULL, 0);\n", dest_var, method_name));
                    } else {
                        code.push_str(&format!("    tauraro_super_method_call(\"{}\", NULL, NULL, 0);\n", method_name));
                    }
                } else {
                    // Regular function call
                    if let Some(dest_var) = dest {
                        // Declare destination variable if it's a temporary
                        if dest_var.starts_with("tmp_") {
                            let return_type = self.infer_return_type_from_function(func, &IRModule::default()); // TODO: Pass actual module
                            code.push_str(&format!("    {} {};\n", return_type, dest_var));
                        }
                        code.push_str(&format!("    {} = {}({});\n", dest_var, func, args_list));
                    } else {
                        code.push_str(&format!("    {}({});\n", func, args_list));
                    }
                }
            }
            IRInstruction::Ret { value } => {
                if let Some(val) = value {
                    let val_str = self.format_ir_value(val);
                    code.push_str(&format!("    return {};\n", val_str));
                } else {
                    code.push_str("    return;\n");
                }
            }
            IRInstruction::Br { cond, then_label, else_label } => {
                let cond_str = self.format_ir_value(cond);
                code.push_str(&format!("    if ({}) goto {}; else goto {};\n", cond_str, then_label, else_label));
            }
            IRInstruction::Jmp { label } => {
                code.push_str(&format!("    goto {};\n", label));
            }
            IRInstruction::Label(label) => {
                code.push_str(&format!("{}:\n", label));
            }
            IRInstruction::CmpEq { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = ({} == {});\n", dest, left_str, right_str));
            }
            IRInstruction::CmpNe { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = ({} != {});\n", dest, left_str, right_str));
            }
            IRInstruction::CmpLt { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = ({} < {});\n", dest, left_str, right_str));
            }
            IRInstruction::CmpGt { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = ({} > {});\n", dest, left_str, right_str));
            }
            IRInstruction::CmpLe { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = ({} <= {});\n", dest, left_str, right_str));
            }
            IRInstruction::CmpGe { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = ({} >= {});\n", dest, left_str, right_str));
            }
            IRInstruction::Not { dest, operand } => {
                let operand_str = self.format_ir_value(operand);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = !{};\n", dest, operand_str));
            }
            IRInstruction::Neg { dest, operand } => {
                let operand_str = self.format_ir_value(operand);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_value(operand);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = -{};\n", dest, operand_str));
            }
            IRInstruction::Pos { dest, operand } => {
                let operand_str = self.format_ir_value(operand);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_value(operand);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = +{};\n", dest, operand_str));
            }
            IRInstruction::BitNot { dest, operand } => {
                let operand_str = self.format_ir_value(operand);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_value(operand);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = ~{};\n", dest, operand_str));
            }
            IRInstruction::DeclareVar { name, ty, value } => {
                let c_type = self.ir_type_to_c_type(ty);
                if let Some(val) = value {
                    let val_str = self.format_ir_value(val);
                    code.push_str(&format!("    {} {} = {};\n", c_type, name, val_str));
                } else {
                    code.push_str(&format!("    {} {};\n", c_type, name));
                }
            }
            IRInstruction::Print { value } => {
                let val_str = self.format_ir_value(value);
                // Generate appropriate print call based on value type
                match value {
                    IRValue::ImmediateString(_) | IRValue::ConstantString(_) | IRValue::Str(_) => {
                        code.push_str(&format!("    tauraro_print({});\n", val_str));
                    }
                    IRValue::ImmediateInt(_) | IRValue::ConstantInt(_) | IRValue::Int(_) => {
                        code.push_str(&format!("    tauraro_print_int({});\n", val_str));
                    }
                    IRValue::ImmediateFloat(_) | IRValue::ConstantFloat(_) | IRValue::Float(_) => {
                        code.push_str(&format!("    tauraro_print_float({});\n", val_str));
                    }
                    IRValue::ImmediateBool(_) | IRValue::ConstantBool(_) | IRValue::Bool(_) => {
                        code.push_str(&format!("    tauraro_print_bool({});\n", val_str));
                    }
                    _ => {
                        // For variables and other values, try to infer the type
                        // If it's a temporary variable, it might be an int
                        if val_str.starts_with("tmp_") {
                            code.push_str(&format!("    tauraro_print_int({});\n", val_str));
                        } else {
                            code.push_str(&format!("    printf(\"%p\\n\", (void*){});\n", val_str));
                        }
                    }
                 }
             }
            // Logical operations
            IRInstruction::And { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = {} && {};\n", dest, left_str, right_str));
            }
            IRInstruction::Or { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    bool {};\n", dest));
                }
                code.push_str(&format!("    {} = {} || {};\n", dest, left_str, right_str));
            }
            // Bitwise operations
            IRInstruction::BitAnd { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} & {};\n", dest, left_str, right_str));
            }
            IRInstruction::BitOr { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} | {};\n", dest, left_str, right_str));
            }
            IRInstruction::BitXor { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} ^ {};\n", dest, left_str, right_str));
            }
            IRInstruction::Shl { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} << {};\n", dest, left_str, right_str));
            }
            IRInstruction::Shr { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_binary_op(left, right);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {} >> {};\n", dest, left_str, right_str));
            }
            // Memory operations
            IRInstruction::Alloca { dest, ty } => {
                let c_type = self.ir_type_to_c_type(ty);
                code.push_str(&format!("    {} {};\n", c_type, dest));
            }
            IRInstruction::Load { dest, ptr, ty } => {
                let c_type = self.ir_type_to_c_type(ty);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = *{};\n", dest, ptr));
            }
            // Type conversions
            IRInstruction::Trunc { dest, value, to_type } => {
                let val_str = self.format_ir_value(value);
                let c_type = self.ir_type_to_c_type(to_type);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = ({}){};\n", dest, c_type, val_str));
            }
            IRInstruction::ZExt { dest, value, to_type } => {
                let val_str = self.format_ir_value(value);
                let c_type = self.ir_type_to_c_type(to_type);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = ({}){};\n", dest, c_type, val_str));
            }
            IRInstruction::FpToSi { dest, value, to_type } => {
                let val_str = self.format_ir_value(value);
                let c_type = self.ir_type_to_c_type(to_type);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = ({}){};\n", dest, c_type, val_str));
            }
            IRInstruction::SiToFp { dest, value, to_type } => {
                let val_str = self.format_ir_value(value);
                let c_type = self.ir_type_to_c_type(to_type);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = ({}){};\n", dest, c_type, val_str));
            }
            // Additional instructions
            IRInstruction::LoadConst { dest, value } => {
                let val_str = self.format_ir_value(value);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    let c_type = self.infer_c_type_from_value(value);
                    code.push_str(&format!("    {} {};\n", c_type, dest));
                }
                code.push_str(&format!("    {} = {};\n", dest, val_str));
            }
            IRInstruction::LoadLocal { dest, name } => {
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = {};\n", dest, name));
            }
            IRInstruction::StoreLocal { name, value } => {
                let val_str = self.format_ir_value(value);
                code.push_str(&format!("    {} = {};\n", name, val_str));
            }
            IRInstruction::LoadGlobal { dest, name } => {
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = {};\n", dest, name));
            }
            IRInstruction::StoreGlobal { name, value } => {
                let val_str = self.format_ir_value(value);
                code.push_str(&format!("    {} = {};\n", name, val_str));
            }
            // Built-in functions
            IRInstruction::Len { dest, obj } => {
                let obj_str = self.format_ir_value(obj);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    int64_t {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_len({});\n", dest, obj_str));
            }
            IRInstruction::Type { dest, obj } => {
                let obj_str = self.format_ir_value(obj);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    char* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_type({});\n", dest, obj_str));
            }
            // Control flow
            IRInstruction::Break => {
                code.push_str("    break;\n");
            }
            IRInstruction::Continue => {
                code.push_str("    continue;\n");
            }
            // Comments
            IRInstruction::Comment { text } => {
                code.push_str(&format!("    // {}\n", text));
            }
            IRInstruction::DocString { text } => {
                code.push_str(&format!("    /* {} */\n", text));
            }
            
            // Data structure instructions
            IRInstruction::BuildList { dest, elements } => {
                let elements_str: Vec<String> = elements.iter().map(|e| self.format_ir_value(e)).collect();
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_create_list({});\n", dest, elements.len()));
                for (i, element_str) in elements_str.iter().enumerate() {
                    code.push_str(&format!("    tauraro_list_set({}, {}, {});\n", dest, i, element_str));
                }
            }
            IRInstruction::BuildDict { dest, pairs } => {
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_create_dict({});\n", dest, pairs.len()));
                for (key, value) in pairs {
                    let key_str = self.format_ir_value(key);
                    let value_str = self.format_ir_value(value);
                    code.push_str(&format!("    tauraro_dict_set({}, {}, {});\n", dest, key_str, value_str));
                }
            }
            IRInstruction::BuildTuple { dest, elements } => {
                let elements_str: Vec<String> = elements.iter().map(|e| self.format_ir_value(e)).collect();
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_create_tuple({});\n", dest, elements.len()));
                for (i, element_str) in elements_str.iter().enumerate() {
                    code.push_str(&format!("    tauraro_tuple_set({}, {}, {});\n", dest, i, element_str));
                }
            }
            IRInstruction::BuildSet { dest, elements } => {
                let elements_str: Vec<String> = elements.iter().map(|e| self.format_ir_value(e)).collect();
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_create_set({});\n", dest, elements.len()));
                for (i, element_str) in elements_str.iter().enumerate() {
                    code.push_str(&format!("    tauraro_set_add({}, {});\n", dest, element_str));
                }
            }
            
            // Object-oriented programming instructions
            IRInstruction::GetAttr { dest, obj, attr } => {
                let obj_str = self.format_ir_value(obj);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                // For simple attribute access like self.name, we use the runtime function
                code.push_str(&format!("    {} = tauraro_get_attr({}, \"{}\");\n", dest, obj_str, attr));
            }
            IRInstruction::SetAttr { obj, attr, value } => {
                let obj_str = self.format_ir_value(obj);
                let value_str = self.format_ir_value(value);
                code.push_str(&format!("    tauraro_set_attr({}, \"{}\", {});\n", obj_str, attr, value_str));
            }
            IRInstruction::GetItem { dest, obj, index } => {
                let obj_str = self.format_ir_value(obj);
                let index_str = self.format_ir_value(index);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    void* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_get_item({}, {});\n", dest, obj_str, index_str));
            }
            IRInstruction::SetItem { obj, index, value } => {
                let obj_str = self.format_ir_value(obj);
                let index_str = self.format_ir_value(index);
                let value_str = self.format_ir_value(value);
                code.push_str(&format!("    tauraro_set_item({}, {}, {});\n", obj_str, index_str, value_str));
            }
            
            // Exception handling instructions
            IRInstruction::Try { body_label, except_label, finally_label } => {
                code.push_str("    tauraro_exception_setup();\n");
                code.push_str(&format!("    if (setjmp(tauraro_exception_jmp) == 0) {{\n"));
                code.push_str(&format!("        goto {};\n", body_label));
                code.push_str("    } else {\n");
                code.push_str(&format!("        goto {};\n", except_label));
                code.push_str("    }\n");
                if let Some(finally) = finally_label {
                    code.push_str(&format!("    goto {};\n", finally));
                }
            }
            IRInstruction::Raise { exception } => {
                let exception_str = self.format_ir_value(exception);
                code.push_str(&format!("    tauraro_raise_exception({});\n", exception_str));
            }
            
            // String operations
            IRInstruction::StrConcat { dest, left, right } => {
                let left_str = self.format_ir_value(left);
                let right_str = self.format_ir_value(right);
                // Declare destination variable if it's a temporary
                if dest.starts_with("tmp_") {
                    code.push_str(&format!("    char* {};\n", dest));
                }
                code.push_str(&format!("    {} = tauraro_str_concat({}, {});\n", dest, left_str, right_str));
            }
            
            // Super call support
            IRInstruction::SuperCall { dest } => {
                // For super calls, we need to generate appropriate C code
                // In Python, super() returns a proxy object that can be used to call parent methods
                // For our implementation, we'll create a function that returns a special value
                // that the method call handler can recognize and resolve appropriately
                code.push_str(&format!("    {} = tauraro_super();\n", dest));
            }
            
            _ => {
                code.push_str(&format!("    // TODO: Implement {:?}\n", instruction));
            }
        }
        
        Ok(code)
    }

    fn write_indent(&mut self) -> anyhow::Result<()> {
        for _ in 0..self.indent_level {
            write!(self.output, "    ")?;
        }
        Ok(())
    }

    fn ir_type_to_c_type(&self, ty: &IRType) -> String {
        match ty {
            IRType::Void => "void".to_string(),
            IRType::Bool => "bool".to_string(),
            IRType::Int | IRType::Int32 | IRType::I32 => "int32_t".to_string(),
            IRType::Int8 | IRType::I8 => "int8_t".to_string(),
            IRType::Int16 | IRType::I16 => "int16_t".to_string(),
            IRType::Int64 | IRType::I64 => "int64_t".to_string(),
            IRType::Float | IRType::Float32 | IRType::F32 => "float".to_string(),
            IRType::Float64 | IRType::F64 => "double".to_string(),
            IRType::String => "char*".to_string(),
            IRType::Pointer(inner) => format!("{}*", self.ir_type_to_c_type(inner)),
            IRType::Array(inner, size) => format!("{}[{}]", self.ir_type_to_c_type(inner), size),
            IRType::Struct(_) => "struct".to_string(),
            IRType::Function { .. } => "void*".to_string(),
            IRType::Dynamic => "void*".to_string(),
            IRType::List(_) => "void*".to_string(),
            IRType::Dict(_, _) => "void*".to_string(),
            IRType::Any => "void*".to_string(),
        }
    }

    fn format_ir_value(&self, value: &IRValue) -> String {
        match value {
            IRValue::ImmediateInt(i) | IRValue::ConstantInt(i) | IRValue::Int(i) => i.to_string(),
            IRValue::ImmediateFloat(f) | IRValue::ConstantFloat(f) | IRValue::Float(f) => f.to_string(),
            IRValue::ImmediateBool(b) | IRValue::ConstantBool(b) | IRValue::Bool(b) => {
                if *b { "true" } else { "false" }.to_string()
            },
            IRValue::ImmediateString(s) | IRValue::ConstantString(s) | IRValue::Str(s) | IRValue::String(s) => {
                // Properly escape string literals for C
                let escaped = s.replace("\\", "\\\\")
                              .replace("\"", "\\\"")
                              .replace("\n", "\\n")
                              .replace("\r", "\\r")
                              .replace("\t", "\\t");
                format!("\"{}\"", escaped)
            },
            IRValue::Variable(name) => name.clone(),
            IRValue::Null | IRValue::None => "NULL".to_string(),
            IRValue::List(_) => "NULL".to_string(), // TODO: Implement list formatting
            IRValue::Dict(_) => "NULL".to_string(), // TODO: Implement dict formatting
        }
    }

    fn validate_identifier(&self, name: &str) -> String {
        // Check if the identifier is a C keyword
        if C_KEYWORDS.contains(&name) {
            format!("tauraro_{}", name)
        } else {
            name.to_string()
        }
    }

    // Helper function to infer C type from IR value
    fn infer_c_type_from_value(&self, value: &IRValue) -> String {
        match value {
            IRValue::ImmediateInt(_) | IRValue::ConstantInt(_) | IRValue::Int(_) => "int64_t".to_string(),
            IRValue::ImmediateFloat(_) | IRValue::ConstantFloat(_) | IRValue::Float(_) => "double".to_string(),
            IRValue::ImmediateBool(_) | IRValue::ConstantBool(_) | IRValue::Bool(_) => "bool".to_string(),
            IRValue::ImmediateString(_) | IRValue::ConstantString(_) | IRValue::Str(_) | IRValue::String(_) => "char*".to_string(),
            IRValue::Variable(_) => "void*".to_string(), // Default for variables
            IRValue::Null | IRValue::None => "void*".to_string(),
            IRValue::List(_) => "void*".to_string(),
            IRValue::Dict(_) => "void*".to_string(),
        }
    }

    // Helper function to infer C type from binary operation operands
    fn infer_c_type_from_binary_op(&self, left: &IRValue, right: &IRValue) -> String {
        // Promote to the "larger" type
        let left_type = self.infer_c_type_from_value(left);
        let right_type = self.infer_c_type_from_value(right);
        
        // Simple type promotion rules
        if left_type == "double" || right_type == "double" {
            "double".to_string()
        } else if left_type == "float" || right_type == "float" {
            "float".to_string()
        } else if left_type == "int64_t" || right_type == "int64_t" {
            "int64_t".to_string()
        } else if left_type == "bool" && right_type == "bool" {
            "bool".to_string()
        } else {
            "int64_t".to_string() // Default
        }
    }

    // Helper function to infer return type from function name
    fn infer_return_type_from_function(&self, func_name: &str, module: &IRModule) -> String {
        // First check if it's a function defined in the module
        if let Some(function) = module.functions.get(func_name) {
            return self.ir_type_to_c_type(&function.return_type);
        }
        
        // Fall back to built-in function types
        match func_name {
            "print" => "void".to_string(),
            "printf" => "int32_t".to_string(),
            "main" => "int32_t".to_string(),
            "malloc" => "void*".to_string(),
            "free" => "void".to_string(),
            _ => "void*".to_string(), // Default for unknown functions
        }
    }
}

pub struct CTranspilerGenerator {
    transpiler: CTranspiler,
}

impl CTranspilerGenerator {
    pub fn new() -> Self {
        Self {
            transpiler: CTranspiler::new(),
        }
    }
}

impl crate::codegen::CodeGenerator for CTranspilerGenerator {
    fn generate(&self, module: crate::ir::IRModule, _options: &crate::codegen::CodegenOptions) -> anyhow::Result<Vec<u8>> {
        let mut transpiler = CTranspiler::new();
        let c_code = transpiler.transpile(&module)?;
        Ok(c_code.into_bytes())
    }
    
    fn get_target(&self) -> crate::codegen::Target {
        crate::codegen::Target::C
    }
}

const C_KEYWORDS: &[&str] = &[
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Static_assert",
    "_Noreturn", "_Thread_local", "_Generic", "_Complex", "_Imaginary"
];
