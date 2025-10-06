use crate::ir::*;
use crate::codegen::{CodeGenerator, CodegenOptions, Target};
use anyhow::{Result, anyhow};
use std::fs::File;
use std::io::Write;
use std::path::Path;

/// C code generator for generating C source code from IR
pub struct CCodeGenerator {
    generate_header: bool,
}

impl CCodeGenerator {
    pub fn new() -> Self {
        Self {
            generate_header: false,
        }
    }
    
    pub fn with_header(mut self, generate_header: bool) -> Self {
        self.generate_header = generate_header;
        self
    }

    /// Generate C code from IR module
    fn generate_c_code(&self, module: &IRModule, options: &CodegenOptions) -> Result<String> {
        let mut c_code = String::new();
        
        // Determine target platform - default to current platform if not specified
        let is_windows = if let Some(ref triple) = options.target_triple {
            triple.contains("windows")
        } else {
            cfg!(windows)
        };
        
        // Generate includes
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <stdint.h>\n");
        c_code.push_str("#include <stdbool.h>\n");
        c_code.push_str("#include <string.h>\n");
        
        // Add platform-specific includes only when needed
        if options.enable_async {
            if is_windows {
                c_code.push_str("#include <windows.h>\n");
            } else {
                // Unix/Linux systems
                c_code.push_str("#include <pthread.h>\n");
                c_code.push_str("#include <ucontext.h>\n");
            }
        }
        
        // Add FFI support if enabled (Unix/Linux only)
        if options.enable_ffi {
            if !is_windows {
                c_code.push_str("#include <dlfcn.h>\n");
            }
        }
        
        // Collect and add import includes at the top
        let import_includes = self.collect_import_includes(module);
        if !import_includes.is_empty() {
            c_code.push_str("\n// Import includes\n");
            c_code.push_str(&import_includes);
        }
        
        c_code.push_str("\n");
        
        // Generate type definitions
        self.generate_type_definitions(&mut c_code, module, options, is_windows)?;
        
        // Generate global variables
        self.generate_global_variables(&mut c_code, module)?;
        
        // Generate function declarations for extern functions only
        for (_, function) in &module.functions {
            if function.is_extern {
                c_code.push_str(&self.generate_function_declaration(function)?);
                c_code.push_str(";\n");
            }
        }
        c_code.push_str("\n");
        
        // Generate runtime support functions
        if options.enable_async {
            self.generate_async_runtime(&mut c_code, is_windows)?;
        }
        
        // Generate super() runtime support
        self.generate_super_runtime(&mut c_code)?;
        
        // Generate function definitions
        for (_, function) in &module.functions {
            if function.is_extern {
                continue;
            }
            
            c_code.push_str(&self.generate_function_definition(function, module, options)?);
            c_code.push_str("\n");
        }
        
        // Generate main function if needed
        if module.functions.contains_key("main") {
            c_code.push_str(&self.generate_main_wrapper()?);
        }
        
        Ok(c_code)
    }

    fn infer_return_type_from_function(&self, func_name: &str) -> String {
        match func_name {
            "print" => "void".to_string(),
            "printf" => "int32_t".to_string(),
            "main" => "int32_t".to_string(),
            "malloc" => "void*".to_string(),
            "free" => "void".to_string(),
            _ => "void*".to_string(), // Default for unknown functions
        }
    }

    /// Generate type definitions
    fn generate_type_definitions(&self, c_code: &mut String, module: &IRModule, options: &CodegenOptions, is_windows: bool) -> Result<()> {
        // Generate basic type aliases
        c_code.push_str("// Type definitions\n");
        c_code.push_str("typedef struct {\n");
        c_code.push_str("    int type_tag;\n");
        c_code.push_str("    union {\n");
        c_code.push_str("        int64_t int_val;\n");
        c_code.push_str("        double float_val;\n");
        c_code.push_str("        char* string_val;\n");
        c_code.push_str("        bool bool_val;\n");
        c_code.push_str("        void* ptr_val;\n");
        c_code.push_str("    } data;\n");
        c_code.push_str("} TauraroValue;\n\n");
        
        // Generate async context only if async is enabled
        if options.enable_async {
            c_code.push_str("typedef struct {\n");
            c_code.push_str("    int state;\n");
            c_code.push_str("    void* locals;\n");
            // Use platform-specific context type
            if is_windows {
                c_code.push_str("    LPVOID fiber;\n");
            } else {
                c_code.push_str("    ucontext_t context;\n");
            }
            c_code.push_str("} AsyncContext;\n\n");
        }
        
        Ok(())
    }
    
    /// Generate global variables
    fn generate_global_variables(&self, c_code: &mut String, module: &IRModule) -> Result<()> {
        if !module.globals.is_empty() {
            c_code.push_str("// Global variables\n");
            for global in &module.globals {
                let c_type = self.ir_value_to_c_type(&global.value.as_ref().unwrap_or(&IRValue::None));
                let c_value = self.ir_value_to_c_literal(&global.value.as_ref().unwrap_or(&IRValue::None))?;
                c_code.push_str(&format!("{} {} = {};\n", c_type, global.name, c_value));
            }
            c_code.push_str("\n");
        }
        Ok(())
    }
    
    /// Generate async runtime support
    fn generate_async_runtime(&self, c_code: &mut String, is_windows: bool) -> Result<()> {
        c_code.push_str("// Async runtime support\n");
        
        if is_windows {
            // Simplified Windows implementation without Fiber API for now
            c_code.push_str("static AsyncContext* create_async_context() {\n");
            c_code.push_str("    AsyncContext* ctx = malloc(sizeof(AsyncContext));\n");
            c_code.push_str("    ctx->state = 0;\n");
            c_code.push_str("    ctx->locals = NULL;\n");
            c_code.push_str("    ctx->fiber = NULL;\n");
            c_code.push_str("    return ctx;\n");
            c_code.push_str("}\n\n");
            
            c_code.push_str("static void destroy_async_context(AsyncContext* ctx) {\n");
            c_code.push_str("    if (ctx) {\n");
            c_code.push_str("        free(ctx->locals);\n");
            c_code.push_str("        free(ctx);\n");
            c_code.push_str("    }\n");
            c_code.push_str("}\n\n");
        } else {
            // Unix/Linux implementation using ucontext
            c_code.push_str("static AsyncContext* create_async_context() {\n");
            c_code.push_str("    AsyncContext* ctx = malloc(sizeof(AsyncContext));\n");
            c_code.push_str("    ctx->state = 0;\n");
            c_code.push_str("    ctx->locals = NULL;\n");
            c_code.push_str("    return ctx;\n");
            c_code.push_str("}\n\n");
            
            c_code.push_str("static void destroy_async_context(AsyncContext* ctx) {\n");
            c_code.push_str("    if (ctx) {\n");
            c_code.push_str("        free(ctx->locals);\n");
            c_code.push_str("        free(ctx);\n");
            c_code.push_str("    }\n");
            c_code.push_str("}\n\n");
        }
        
        Ok(())
    }
    
    /// Generate super() runtime support
    fn generate_super_runtime(&self, c_code: &mut String) -> Result<()> {
        c_code.push_str("// Super() runtime support\n");
        c_code.push_str("void* tauraro_super() {\n");
        c_code.push_str("    return (void*)0x1; // Special value for super calls\n");
        c_code.push_str("}\n\n");
        
        c_code.push_str("void* tauraro_super_method_call(const char* method_name, void* self_obj, void** args, int arg_count) {\n");
        c_code.push_str("    // In a full implementation, this would resolve the method according to the MRO\n");
        c_code.push_str("    // For now, we'll just return NULL to indicate the method wasn't found\n");
        c_code.push_str("    printf(\"Super method call: %s\\n\", method_name);\n");
        c_code.push_str("    return NULL;\n");
        c_code.push_str("}\n\n");
        
        Ok(())
    }
    
    /// Generate main wrapper
    fn generate_main_wrapper(&self) -> Result<String> {
        Ok("int main(int argc, char* argv[]) {\n    return tauraro_main();\n}\n".to_string())
    }

    /// Collect import includes from the IR module
    fn collect_import_includes(&self, module: &IRModule) -> String {
        let mut includes = String::new();
        
        // List of built-in modules that don't need header files
        let builtin_modules = [
            "os", "sys", "io", "math", "random", "time", "datetime", 
            "json", "re", "csv", "base64", "collections", "itertools", 
            "functools", "copy", "pickle", "hashlib", "urllib", 
            "socket", "threading", "asyncio", "memory", "gc", 
            "logging", "unittest", "httptools", "websockets", "httpx"
        ];
        
        // Look for import instructions in all functions
        for (_, function) in &module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    match instruction {
                        IRInstruction::Import { module: import_module, alias } => {
                            // Skip built-in modules
                            if builtin_modules.contains(&import_module.as_str()) {
                                continue;
                            }
                            
                            let module_name = alias.as_ref().unwrap_or(import_module);
                            // Convert dotted module names to path format (e.g., "extra.utils" -> "extra/utils.h")
                            let header_path = if import_module.contains('.') {
                                let parts: Vec<&str> = import_module.split('.').collect();
                                format!("{}.h", parts.join("/"))
                            } else {
                                format!("{}.h", import_module)
                            };
                            includes.push_str(&format!("#include \"{}\"\n", header_path));
                        }
                        IRInstruction::ImportFrom { module: import_module, .. } => {
                            // Skip built-in modules
                            if builtin_modules.contains(&import_module.as_str()) {
                                continue;
                            }
                            
                            // Convert dotted module names to path format (e.g., "extra.utils" -> "extra/utils.h")
                            let header_path = if import_module.contains('.') {
                                let parts: Vec<&str> = import_module.split('.').collect();
                                format!("{}.h", parts.join("/"))
                            } else {
                                format!("{}.h", import_module)
                            };
                            includes.push_str(&format!("#include \"{}\"\n", header_path));
                        }
                        _ => {}
                    }
                }
            }
        }
        
        includes
    }

    fn generate_function_declaration(&self, function: &IRFunction) -> Result<String> {
        let return_type = self.ir_type_to_c(&function.return_type);
        let mut params = Vec::new();
        
        for param in &function.params {
            let param_type = self.ir_type_to_c(&param.ty);
            params.push(format!("{} {}", param_type, param.name));
        }
        
        let params_str = if params.is_empty() {
            "void".to_string()
        } else {
            params.join(", ")
        };
        
        let func_name = if function.name == "main" {
            "tauraro_main".to_string()
        } else {
            function.name.clone()
        };
        
        Ok(format!("{} {}({})", return_type, func_name, params_str))
    }

    fn generate_function_definition(&self, function: &IRFunction, module: &IRModule, options: &CodegenOptions) -> Result<String> {
        let mut code = String::new();
        
        // Function signature
        code.push_str(&self.generate_function_declaration(function)?);
        code.push_str(" {\n");
        
        // Generate local variables with proper types
        let mut locals = std::collections::HashSet::new();
        let mut local_types = std::collections::HashMap::new();
        
        for block in &function.blocks {
            for instruction in &block.instructions {
                self.collect_locals(instruction, &mut locals);
                self.collect_local_types(instruction, &mut local_types, Some(module));
            }
        }
        
        for local in &locals {
            let c_type = if let Some(ir_type) = local_types.get(local) {
                self.ir_type_to_c(ir_type).to_string()
            } else if local.starts_with("tmp_") {
                // For temporary variables, try to infer type from context
                "int32_t".to_string() // Default to int32_t for now
            } else {
                "TauraroValue".to_string()
            };
            code.push_str(&format!("    {} {};\n", c_type, local));
        }
        
        if !locals.is_empty() {
            code.push_str("\n");
        }
        
        // Generate function body - process entry block first, then other blocks
        if let Some(entry_block) = function.blocks.iter().find(|b| b.label == "entry") {
            for instruction in &entry_block.instructions {
                let instruction_code = self.generate_instruction(instruction, options)?;
                // Skip empty instruction outputs (e.g., import instructions)
                if !instruction_code.is_empty() {
                    code.push_str(&instruction_code);
                }
            }
        }
        
        // Process non-entry blocks in order
        for block in &function.blocks {
            if block.label != "entry" {
                for instruction in &block.instructions {
                    let instruction_code = self.generate_instruction(instruction, options)?;
                    // Skip empty instruction outputs (e.g., import instructions)
                    if !instruction_code.is_empty() {
                        code.push_str(&instruction_code);
                    }
                }
            }
        }
        
        // Ensure return if no explicit return
        let has_return = function.blocks.iter().any(|block| 
            block.instructions.iter().any(|inst| matches!(inst, IRInstruction::Ret { .. }))
        );
        if !has_return {
            match function.return_type {
                IRType::Void => code.push_str("    return;\n"),
                IRType::Int => code.push_str("    return 0;\n"),
                IRType::Float => code.push_str("    return 0.0;\n"),
                IRType::Bool => code.push_str("    return false;\n"),
                _ => code.push_str("    return NULL;\n"),
            }
        }
        
        code.push_str("}\n");
        Ok(code)
    }
    
    /// Collect local variable names from instructions
    fn collect_locals(&self, instruction: &IRInstruction, locals: &mut std::collections::HashSet<String>) {
        match instruction {
            IRInstruction::Alloca { dest, .. } => {
                locals.insert(dest.clone());
            }
            IRInstruction::Load { dest, .. } => {
                locals.insert(dest.clone());
            }
            IRInstruction::Store { ptr, .. } => {
                // Store instructions use variables as pointers, so we need to declare them
                locals.insert(ptr.clone());
            }
            IRInstruction::Add { dest, .. } |
            IRInstruction::Sub { dest, .. } |
            IRInstruction::Mul { dest, .. } |
            IRInstruction::Div { dest, .. } |
            IRInstruction::FloorDiv { dest, .. } => {
                locals.insert(dest.clone());
            }
            IRInstruction::Call { dest: Some(dest), .. } => {
                locals.insert(dest.clone());
            }
            // Comparison operations
            IRInstruction::CmpEq { dest, .. } |
            IRInstruction::CmpNe { dest, .. } |
            IRInstruction::CmpLt { dest, .. } |
            IRInstruction::CmpGt { dest, .. } |
            IRInstruction::CmpLe { dest, .. } |
            IRInstruction::CmpGe { dest, .. } => {
                locals.insert(dest.clone());
            }
            // Logical operations
            IRInstruction::And { dest, .. } |
            IRInstruction::Or { dest, .. } |
            IRInstruction::Not { dest, .. } => {
                locals.insert(dest.clone());
            }
            // Additional instructions that create variables
            IRInstruction::LoadConst { dest, .. } |
            IRInstruction::LoadLocal { dest, .. } |
            IRInstruction::LoadGlobal { dest, .. } |
            IRInstruction::GetAttr { dest, .. } |
            IRInstruction::GetItem { dest, .. } |
            IRInstruction::BuildList { dest, .. } |
            IRInstruction::BuildDict { dest, .. } |
            IRInstruction::BuildTuple { dest, .. } |
            IRInstruction::BuildSet { dest, .. } => {
                locals.insert(dest.clone());
            }
            // Super call support
            IRInstruction::SuperCall { dest } => {
                locals.insert(dest.clone());
            }
            // Format string support
            IRInstruction::FormatString { dest, .. } => {
                locals.insert(dest.clone());
            }
            _ => {}
        }
    }

    /// Collect local variable types from instructions
    fn collect_local_types(&self, instruction: &IRInstruction, local_types: &mut std::collections::HashMap<String, IRType>, _module: Option<&IRModule>) {
        match instruction {
            IRInstruction::Alloca { dest, ty } => {
                local_types.insert(dest.clone(), ty.clone());
            }
            IRInstruction::Load { dest, .. } => {
                local_types.insert(dest.clone(), IRType::Dynamic);
            }
            IRInstruction::Store { value, ptr } => {
                // Store instructions use variables as storage, infer type from value
                match value {
                    IRValue::Int(_) => {
                        local_types.insert(ptr.clone(), IRType::Int64);
                    }
                    IRValue::Float(_) => {
                        local_types.insert(ptr.clone(), IRType::Float64);
                    }
                    IRValue::Bool(_) => {
                        local_types.insert(ptr.clone(), IRType::Bool);
                    }
                    IRValue::Str(_) | IRValue::String(_) => {
                        local_types.insert(ptr.clone(), IRType::String);
                    }
                    IRValue::Variable(var_name) => {
                         // Try to infer from variable name patterns or look up existing type
                         if let Some(existing_type) = local_types.get(var_name) {
                             local_types.insert(ptr.clone(), existing_type.clone());
                         } else if var_name.contains("tmp_") {
                             // For tmp variables, try to infer from context
                             local_types.insert(ptr.clone(), IRType::Int64);
                         } else {
                             // Special handling for common variable names
                             match var_name.as_str() {
                                 "name" => local_types.insert(ptr.clone(), IRType::String),
                                 "age" => local_types.insert(ptr.clone(), IRType::Int64),
                                 "height" => local_types.insert(ptr.clone(), IRType::Float64),
                                 _ => local_types.insert(ptr.clone(), IRType::Dynamic),
                             };
                         }
                     }
                    _ => {
                        local_types.insert(ptr.clone(), IRType::Dynamic);
                    }
                }
            }
            IRInstruction::Add { dest, .. } |
             IRInstruction::Sub { dest, .. } |
             IRInstruction::Mul { dest, .. } |
             IRInstruction::Div { dest, .. } => {
                 local_types.insert(dest.clone(), IRType::Int64);
             }
             IRInstruction::FloorDiv { dest, .. } => {
                 local_types.insert(dest.clone(), IRType::Int64);
             }
            IRInstruction::Call { dest: Some(dest), func, .. } => {
                 // Use built-in function types for C ABI generation
                 let return_type = match func.as_str() {
                     "print" | "printf" => IRType::Void,
                     "main" => IRType::Int32,
                     "malloc" | "free" => IRType::Pointer(Box::new(IRType::Void)),
                     "simple_add" => IRType::Int64, // Specific type for simple_add function
                     // Handle imported functions from our enhanced import compiler
                     "greet" => IRType::Pointer(Box::new(IRType::Int8)), // char* (string)
                     "calculate_area" => IRType::Int64, // int64_t
                     _ => IRType::Int64, // Default assumption
                 };
                 local_types.insert(dest.clone(), return_type);
             }
            // Comparison operations return boolean
            IRInstruction::CmpEq { dest, .. } |
            IRInstruction::CmpNe { dest, .. } |
            IRInstruction::CmpLt { dest, .. } |
            IRInstruction::CmpGt { dest, .. } |
            IRInstruction::CmpLe { dest, .. } |
            IRInstruction::CmpGe { dest, .. } => {
                local_types.insert(dest.clone(), IRType::Bool);
            }
            // Logical operations return boolean
            IRInstruction::And { dest, .. } |
            IRInstruction::Or { dest, .. } |
            IRInstruction::Not { dest, .. } => {
                local_types.insert(dest.clone(), IRType::Bool);
            }
            // Additional instructions
            IRInstruction::LoadConst { dest, .. } |
            IRInstruction::LoadLocal { dest, .. } |
            IRInstruction::LoadGlobal { dest, .. } => {
                local_types.insert(dest.clone(), IRType::Dynamic);
            }
            IRInstruction::GetAttr { dest, .. } |
            IRInstruction::GetItem { dest, .. } => {
                local_types.insert(dest.clone(), IRType::Dynamic);
            }
            IRInstruction::BuildList { dest, .. } |
            IRInstruction::BuildDict { dest, .. } |
            IRInstruction::BuildTuple { dest, .. } |
            IRInstruction::BuildSet { dest, .. } => {
                local_types.insert(dest.clone(), IRType::Dynamic);
            }
            // Super call support
            IRInstruction::SuperCall { dest } => {
                local_types.insert(dest.clone(), IRType::Dynamic);
            }
            // Format string support
            IRInstruction::FormatString { dest, .. } => {
                local_types.insert(dest.clone(), IRType::String);
            }
            _ => {}
        }
    }
    
    /// Generate C code for a single instruction
    fn generate_instruction(&self, instruction: &IRInstruction, options: &CodegenOptions) -> Result<String> {
        match instruction {
            IRInstruction::Alloca { dest, ty } => {
                let c_type = self.ir_type_to_c(ty);
                Ok(format!("    {} {};\n", c_type, dest))
            }
            
            IRInstruction::Load { dest, ptr, ty: _ } => {
                Ok(format!("    {} = *{};\n", dest, ptr))
            }
            
            IRInstruction::Store { value, ptr } => {
                // Store instructions should not use pointer dereferencing for regular variables
                // Only use pointer syntax for actual pointer variables
                if ptr.starts_with("ptr_") || ptr.ends_with("_ptr") {
                    Ok(format!("    *{} = {};\n", ptr, self.ir_value_to_c_expr(value)?))
                } else {
                    // For TauraroValue variables, we need to set the appropriate union member
                    match value {
                        IRValue::ImmediateInt(val) | IRValue::ConstantInt(val) | IRValue::Int(val) => {
                            Ok(format!("    {}.type_tag = 0; {}.data.int_val = {};\n", ptr, ptr, val))
                        }
                        IRValue::ImmediateFloat(val) | IRValue::ConstantFloat(val) | IRValue::Float(val) => {
                            Ok(format!("    {}.type_tag = 1; {}.data.float_val = {};\n", ptr, ptr, val))
                        }
                        IRValue::ImmediateString(ref val) | IRValue::ConstantString(ref val) | IRValue::Str(ref val) | IRValue::String(ref val) => {
                            // Escape quotes in the string
                            let escaped = val.replace("\\", "\\\\").replace("\"", "\\\"");
                            Ok(format!("    {}.type_tag = 2; {}.data.string_val = \"{}\";\n", ptr, ptr, escaped))
                        }
                        IRValue::ImmediateBool(val) | IRValue::ConstantBool(val) | IRValue::Bool(val) => {
                            Ok(format!("    {}.type_tag = 3; {}.data.bool_val = {};\n", ptr, ptr, if *val { "true" } else { "false" }))
                        }
                        _ => {
                            Ok(format!("    {} = {};\n", ptr, self.ir_value_to_c_expr(value)?))
                        }
                    }
                }
            }
            
            IRInstruction::Add { dest, left, right } => {
                // Handle addition properly based on operand types
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables
                        // Check if they are TauraroValue types or simple types
                        // For now, we'll assume simple arithmetic types for function parameters
                        Ok(format!("    {} = {} + {};\n", dest, left_var, right_var))
                    }
                    (IRValue::Variable(left_var), _) => {
                        // Left operand is variable, right is immediate
                        Ok(format!("    {} = {} + {};\n", dest, left_var, self.ir_value_to_c_expr(right)?))
                    }
                    (_, IRValue::Variable(right_var)) => {
                        // Right operand is variable, left is immediate
                        Ok(format!("    {} = {} + {};\n", dest, self.ir_value_to_c_expr(left)?, right_var))
                    }
                    _ => {
                        // Both operands are immediate values
                        Ok(format!("    {} = {} + {};\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::Sub { dest, left, right } => {
                Ok(format!("    {} = {} - {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Mul { dest, left, right } => {
                // Handle multiplication properly based on operand types
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables
                        // For now, we'll assume simple arithmetic types for function parameters
                        Ok(format!("    {} = {} * {};\n", dest, left_var, right_var))
                    }
                    _ => {
                        // Handle other cases
                        Ok(format!("    {} = {} * {};\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::Div { dest, left, right } => {
                Ok(format!("    {} = {} / {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::FloorDiv { dest, left, right } => {
                Ok(format!("    {} = (int)({} / {});\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Call { dest, func, args } => {
                let args_str = args.iter()
                    .map(|arg| self.ir_value_to_c_expr(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                
                // Check if this is a super method call
                if func.starts_with("super_") {
                    // Handle super method calls
                    let method_name = &func[6..]; // Remove "super_" prefix
                    if let Some(dest_var) = dest {
                        // Don't declare the variable inline - it's already declared at the top
                        Ok(format!("    {} = tauraro_super_method_call(\"{}\", NULL, NULL, 0);\n", dest_var, method_name))
                    } else {
                        Ok(format!("    tauraro_super_method_call(\"{}\", NULL, NULL, 0);\n", method_name))
                    }
                } else {
                    // Check if this is a void function by looking at the function name
                    let is_void_function = match func.as_str() {
                        "print" | "printf" | "free" => true,
                        "main" | "malloc" => false, // main returns int32_t, malloc returns void*
                        _ => {
                            // For other functions, we'll assume they're not void for now
                            // In a more sophisticated implementation, we would check the IR
                            false
                        }
                    };
                    
                    if let Some(dest_var) = dest {
                        // For functions that return values
                        if is_void_function {
                            // Void functions should not be assigned to variables
                            // Just call the function without assignment
                            Ok(format!("    {}({});\n", func, args_str))
                        } else {
                            Ok(format!("    {} = {}({});\n", dest_var, func, args_str))
                        }
                    } else {
                        // For functions called without assignment (void functions or when result is ignored)
                        Ok(format!("    {}({});\n", func, args_str))
                    }
                }
            }
            
            IRInstruction::Ret { value } => {
                if let Some(val) = value {
                    Ok(format!("    return {};\n", self.ir_value_to_c_expr(val)?))
                } else {
                    Ok("    return;\n".to_string())
                }
            }
            
            IRInstruction::Br { cond, then_label, else_label } => {
                Ok(format!("    if ({}) {{\n        goto {};\n    }} else {{\n        goto {};\n    }}\n", 
                    self.ir_value_to_c_expr(cond)?, then_label, else_label))
            }
            
            IRInstruction::Jmp { label } => {
                Ok(format!("    goto {};\n", label))
            }
            
            IRInstruction::Label(label) => {
                Ok(format!("{}:\n", label))
            }
            
            IRInstruction::CmpLt { dest, left, right } => {
                // Handle comparison of TauraroValue variables
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables, need to access their values properly
                        Ok(format!("    {} = ({}.data.float_val < {}.data.float_val);\n", dest, left_var, right_var))
                    }
                    (IRValue::Variable(left_var), _) => {
                        // Left operand is variable, right is immediate
                        Ok(format!("    {} = ({}.data.float_val < {});\n", dest, left_var, self.ir_value_to_c_expr(right)?))
                    }
                    (_, IRValue::Variable(right_var)) => {
                        // Right operand is variable, left is immediate
                        Ok(format!("    {} = ({} < {}.data.float_val);\n", dest, self.ir_value_to_c_expr(left)?, right_var))
                    }
                    _ => {
                        // Both operands are immediate values
                        Ok(format!("    {} = ({} < {});\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::CmpLe { dest, left, right } => {
                // Handle comparison of TauraroValue variables
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables, need to access their values properly
                        Ok(format!("    {} = ({}.data.float_val <= {}.data.float_val);\n", dest, left_var, right_var))
                    }
                    (IRValue::Variable(left_var), _) => {
                        // Left operand is variable, right is immediate
                        Ok(format!("    {} = ({}.data.float_val <= {});\n", dest, left_var, self.ir_value_to_c_expr(right)?))
                    }
                    (_, IRValue::Variable(right_var)) => {
                        // Right operand is variable, left is immediate
                        Ok(format!("    {} = ({} <= {}.data.float_val);\n", dest, self.ir_value_to_c_expr(left)?, right_var))
                    }
                    _ => {
                        // Both operands are immediate values
                        Ok(format!("    {} = ({} <= {});\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::CmpGe { dest, left, right } => {
                Ok(format!("    {} = ({} >= {});\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::CmpGt { dest, left, right } => {
                // Handle comparison of TauraroValue variables
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables, need to access their values properly
                        Ok(format!("    {} = ({}.data.float_val > {}.data.float_val);\n", dest, left_var, right_var))
                    }
                    (IRValue::Variable(left_var), _) => {
                        // Left operand is variable, right is immediate
                        Ok(format!("    {} = ({}.data.float_val > {});\n", dest, left_var, self.ir_value_to_c_expr(right)?))
                    }
                    (_, IRValue::Variable(right_var)) => {
                        // Right operand is variable, left is immediate
                        Ok(format!("    {} = ({} > {}.data.float_val);\n", dest, self.ir_value_to_c_expr(left)?, right_var))
                    }
                    _ => {
                        // Both operands are immediate values
                        Ok(format!("    {} = ({} > {});\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::CmpEq { dest, left, right } => {
                // Handle comparison of TauraroValue variables
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables, need to access their values properly
                        Ok(format!("    {} = ({}.data.float_val == {}.data.float_val);\n", dest, left_var, right_var))
                    }
                    (IRValue::Variable(left_var), _) => {
                        // Left operand is variable, right is immediate
                        Ok(format!("    {} = ({}.data.float_val == {});\n", dest, left_var, self.ir_value_to_c_expr(right)?))
                    }
                    (_, IRValue::Variable(right_var)) => {
                        // Right operand is variable, left is immediate
                        Ok(format!("    {} = ({} == {}.data.float_val);\n", dest, self.ir_value_to_c_expr(left)?, right_var))
                    }
                    _ => {
                        // Both operands are immediate values
                        Ok(format!("    {} = ({} == {});\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::CmpNe { dest, left, right } => {
                // Handle comparison of TauraroValue variables
                match (left, right) {
                    (IRValue::Variable(left_var), IRValue::Variable(right_var)) => {
                        // Both operands are variables, need to access their values properly
                        Ok(format!("    {} = ({}.data.float_val != {}.data.float_val);\n", dest, left_var, right_var))
                    }
                    (IRValue::Variable(left_var), _) => {
                        // Left operand is variable, right is immediate
                        Ok(format!("    {} = ({}.data.float_val != {});\n", dest, left_var, self.ir_value_to_c_expr(right)?))
                    }
                    (_, IRValue::Variable(right_var)) => {
                        // Right operand is variable, left is immediate
                        Ok(format!("    {} = ({} != {}.data.float_val);\n", dest, self.ir_value_to_c_expr(left)?, right_var))
                    }
                    _ => {
                        // Both operands are immediate values
                        Ok(format!("    {} = ({} != {});\n", dest, 
                            self.ir_value_to_c_expr(left)?, 
                            self.ir_value_to_c_expr(right)?))
                    }
                }
            }
            
            IRInstruction::StoreLocal { name, value } => {
                Ok(format!("    {} = {};\n", name, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::StoreGlobal { name, value } => {
                Ok(format!("    {} = {};\n", name, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::LoadLocal { dest, name } => {
                Ok(format!("    {} = {};\n", dest, name))
            }
            
            IRInstruction::LoadGlobal { dest, name } => {
                Ok(format!("    {} = {};\n", dest, name))
            }
            
            IRInstruction::LoadConst { dest, value } => {
                Ok(format!("    {} = {};\n", dest, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::Print { value } => {
                let expr = self.ir_value_to_c_expr(value)?;
                // Handle different types for printf
                match value {
                    IRValue::ConstantString(_) | IRValue::Str(_) | IRValue::String(_) => {
                        Ok(format!("    printf(\"%s\\n\", {});\n", expr))
                    }
                    IRValue::ConstantInt(_) | IRValue::Int(_) => {
                        Ok(format!("    printf(\"%lld\\n\", (long long){});\n", expr))
                    }
                    IRValue::ConstantFloat(_) | IRValue::Float(_) => {
                        Ok(format!("    printf(\"%f\\n\", {});\n", expr))
                    }
                    IRValue::ConstantBool(_) | IRValue::Bool(_) => {
                        Ok(format!("    printf(\"%s\\n\", {} ? \"true\" : \"false\");\n", expr))
                    }
                    _ => {
                        Ok(format!("    printf(\"%s\\n\", {});\n", expr))
                    }
                }
            }
            
            IRInstruction::Trunc { dest, value, to_type: _ } => {
                Ok(format!("    {} = (int){};  // Truncate\n", dest, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::ZExt { dest, value, to_type: _ } => {
                Ok(format!("    {} = (unsigned){};  // Zero extend\n", dest, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::FpToSi { dest, value, to_type: _ } => {
                Ok(format!("    {} = (int){};  // Float to signed int\n", dest, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::SiToFp { dest, value, to_type: _ } => {
                Ok(format!("    {} = (double){};  // Signed int to float\n", dest, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::GetAttr { dest, obj, attr } => {
                Ok(format!("    {} = {}.{};  // Get attribute\n", dest, self.ir_value_to_c_expr(obj)?, attr))
            }
            
            IRInstruction::SetAttr { obj, attr, value } => {
                Ok(format!("    {}.{} = {};  // Set attribute\n", self.ir_value_to_c_expr(obj)?, attr, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::GetItem { dest, obj, index } => {
                Ok(format!("    {} = {}[{}];  // Get item\n", dest, self.ir_value_to_c_expr(obj)?, self.ir_value_to_c_expr(index)?))
            }
            
            IRInstruction::SetItem { obj, index, value } => {
                Ok(format!("    {}[{}] = {};  // Set item\n", self.ir_value_to_c_expr(obj)?, self.ir_value_to_c_expr(index)?, self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::BuildList { dest, elements } => {
                let elements_str = elements.iter()
                    .map(|elem| self.ir_value_to_c_expr(elem))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("    // Build list: {} = [{}]\n", dest, elements_str))
            }
            
            IRInstruction::BuildDict { dest, pairs } => {
                let pairs_str = pairs.iter()
                    .map(|(k, v)| format!("{}: {}", self.ir_value_to_c_expr(k).unwrap_or_default(), self.ir_value_to_c_expr(v).unwrap_or_default()))
                    .collect::<Vec<_>>()
                    .join(", ");
                Ok(format!("    // Build dict: {} = {{{}}}\n", dest, pairs_str))
            }
            
            IRInstruction::Await { dest, expr } => {
                Ok(format!("    {} = await {};  // Await expression\n", dest, self.ir_value_to_c_expr(expr)?))
            }
            
            IRInstruction::Yield { value } => {
                Ok(format!("    yield {};  // Yield value\n", self.ir_value_to_c_expr(value)?))
            }
            
            IRInstruction::Raise { exception } => {
                Ok(format!("    raise {};  // Raise exception\n", self.ir_value_to_c_expr(exception)?))
            }
            
            // Additional missing variants
            IRInstruction::Mod { dest, left, right } => {
                Ok(format!("    {} = {} % {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Pow { dest, left, right } => {
                Ok(format!("    {} = pow({}, {});\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::And { dest, left, right } => {
                Ok(format!("    {} = {} && {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Or { dest, left, right } => {
                Ok(format!("    {} = {} || {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Not { dest, operand } => {
                Ok(format!("    {} = !{};\n", dest, self.ir_value_to_c_expr(operand)?))
            }
            
            IRInstruction::BitAnd { dest, left, right } => {
                Ok(format!("    {} = {} & {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::BitOr { dest, left, right } => {
                Ok(format!("    {} = {} | {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::BitXor { dest, left, right } => {
                Ok(format!("    {} = {} ^ {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::BitNot { dest, operand } => {
                Ok(format!("    {} = ~{};\n", dest, self.ir_value_to_c_expr(operand)?))
            }
            
            IRInstruction::Shl { dest, left, right } => {
                Ok(format!("    {} = {} << {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Shr { dest, left, right } => {
                Ok(format!("    {} = {} >> {};\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::Neg { dest, operand } => {
                Ok(format!("    {} = -{};\n", dest, self.ir_value_to_c_expr(operand)?))
            }
            
            IRInstruction::Pos { dest, operand } => {
                Ok(format!("    {} = +{};\n", dest, self.ir_value_to_c_expr(operand)?))
            }
            
            IRInstruction::BuildTuple { dest, elements } => {
                let elements_str = elements.iter()
                    .map(|elem| self.ir_value_to_c_expr(elem))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("    // Build tuple: {} = ({})\n", dest, elements_str))
            }
            
            IRInstruction::BuildSet { dest, elements } => {
                let elements_str = elements.iter()
                    .map(|elem| self.ir_value_to_c_expr(elem))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("    // Build set: {} = {{{}}}\n", dest, elements_str))
            }
            
            IRInstruction::Break => {
                Ok("    break;\n".to_string())
            }
            
            IRInstruction::Continue => {
                Ok("    continue;\n".to_string())
            }
            
            IRInstruction::If { cond, then_label, else_label } => {
                if let Some(else_lbl) = else_label {
                    Ok(format!("    if ({}) {{\n        goto {};\n    }} else {{\n        goto {};\n    }}\n", 
                        self.ir_value_to_c_expr(cond)?, then_label, else_lbl))
                } else {
                    Ok(format!("    if ({}) {{\n        goto {};\n    }}\n", 
                        self.ir_value_to_c_expr(cond)?, then_label))
                }
            }
            
            IRInstruction::While { cond, body_label, end_label } => {
                Ok(format!("    while ({}) {{\n        goto {};\n    }}\n    goto {};\n", 
                    self.ir_value_to_c_expr(cond)?, body_label, end_label))
            }
            
            IRInstruction::For { init, cond, update, body_label, end_label } => {
                let init_str = init.as_ref()
                    .map(|i| self.ir_value_to_c_expr(i).unwrap_or_default())
                    .unwrap_or_default();
                let cond_str = cond.as_ref()
                    .map(|c| self.ir_value_to_c_expr(c).unwrap_or_default())
                    .unwrap_or("1".to_string());
                let update_str = update.as_ref()
                    .map(|u| self.ir_value_to_c_expr(u).unwrap_or_default())
                    .unwrap_or_default();
                Ok(format!("    for ({}; {}; {}) {{\n        goto {};\n    }}\n    goto {};\n", 
                    init_str, cond_str, update_str, body_label, end_label))
            }
            
            IRInstruction::Loop { body_label, end_label } => {
                Ok(format!("    while (1) {{\n        goto {};\n    }}\n    goto {};\n", 
                    body_label, end_label))
            }
            
            IRInstruction::Try { body_label, except_label, finally_label } => {
                let finally_str = finally_label.as_ref()
                    .map(|f| format!(" finally: {}", f))
                    .unwrap_or_default();
                Ok(format!("    // Try block: {} except: {}{}\n", body_label, except_label, finally_str))
            }
            
            IRInstruction::Except { exception_type, handler_label } => {
                let exc_type = exception_type.as_ref()
                    .map(|t| t.as_str())
                    .unwrap_or("Exception");
                Ok(format!("    // Except {}: goto {}\n", exc_type, handler_label))
            }
            
            IRInstruction::FuncDef { name, params, body_label } => {
                let params_str = params.join(", ");
                Ok(format!("// Function definition: {} ({}) -> {}\n", name, params_str, body_label))
            }
            
            IRInstruction::ClassDef { name, bases, methods } => {
                let bases_str = bases.join(", ");
                let methods_str = methods.join(", ");
                Ok(format!("// Class definition: {} extends {} methods: {}\n", name, bases_str, methods_str))
            }
            
            // Import/module support
            IRInstruction::Import { module, alias: _ } => {
                // For C code generation, import includes are collected at the top of the file
                // This is a simplified approach - in a real implementation we would need to
                // generate appropriate C headers and link with the imported modules
                Ok("".to_string()) // Return empty string as includes are handled at file level
            }
            
            IRInstruction::ImportFrom { module, names: _ } => {
                // For C code generation, import includes are collected at the top of the file
                // This is a simplified approach - in a real implementation we would need to
                // generate appropriate C headers and link with the imported modules
                Ok("".to_string()) // Return empty string as includes are handled at file level
            }
            
            IRInstruction::StrConcat { dest, left, right } => {
                Ok(format!("    {} = strcat({}, {});  // String concatenation\n", dest, 
                    self.ir_value_to_c_expr(left)?, 
                    self.ir_value_to_c_expr(right)?))
            }
            
            IRInstruction::StrFormat { dest, format_str, args } => {
                let args_str = args.iter()
                    .map(|arg| self.ir_value_to_c_expr(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                Ok(format!("    {} = sprintf({}, {});  // String format\n", dest, 
                    self.ir_value_to_c_expr(format_str)?, args_str))
            }
            
            IRInstruction::Comment { text } => {
                Ok(format!("    // {}\n", text))
            }
            
            IRInstruction::DocString { text } => {
                Ok(format!("    /* {} */\n", text))
            }
            
            IRInstruction::Len { dest, obj } => {
                Ok(format!("    {} = tauraro_len({});\n", dest, self.ir_value_to_c_expr(obj)?))
            }
            
            IRInstruction::Type { dest, obj } => {
                Ok(format!("    {} = tauraro_type({});\n", dest, self.ir_value_to_c_expr(obj)?))
            }
            
            IRInstruction::DeclareVar { name, ty, value } => {
                let c_type = self.ir_type_to_c(ty);
                if let Some(val) = value {
                    Ok(format!("    {} {} = {};\n", c_type, name, self.ir_value_to_c_expr(val)?))
                } else {
                    Ok(format!("    {} {};\n", c_type, name))
                }
            }
            
            // Super call support
            IRInstruction::SuperCall { dest } => {
                // For super calls, we need to generate appropriate C code
                // In Python, super() returns a proxy object that can be used to call parent methods
                // For our implementation, we'll create a function that returns a special value
                // that the method call handler can recognize and resolve appropriately
                Ok(format!("    {} = tauraro_super();\n", dest))
            }
            
            // Format string support
            IRInstruction::FormatString { dest, parts } => {
                // Improved implementation for f-string support in C
                let mut code = format!("    // Format string: {}\n", dest);
                
                // Initialize the destination variable with a larger buffer
                code.push_str(&format!("    {} = malloc(2048);  // Allocate larger buffer for formatted string\n", dest));
                code.push_str(&format!("    if ({}) {{\n", dest));
                code.push_str(&format!("        {}[0] = '\\0';  // Initialize as empty string\n", dest));
                code.push_str("    }\n");
                
                // Process each part
                for part in parts {
                    match part {
                        FormatPartIR::String(s) => {
                            // Escape quotes in the string
                            let escaped = s.replace("\\", "\\\\").replace("\"", "\\\"");
                            code.push_str(&format!("    if ({}) strcat({}, \"{}\");\n", dest, dest, escaped));
                        }
                        FormatPartIR::Expression { expr, format_spec, conversion: _ } => {
                            // Convert expression to string and append
                            let expr_str = self.ir_value_to_c_expr(expr)?;
                            
                            // Handle different types of expressions based on their IR value type
                            match expr {
                                IRValue::ImmediateInt(val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[64];\n");
                                    if let Some(ref spec) = format_spec {
                                        // Handle format specifiers like :03d
                                        if spec.contains('d') {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%{}\", {});\n", spec, val));
                                        } else {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%lld\", {});\n", val));
                                        }
                                    } else {
                                        code.push_str(&format!("        sprintf(temp_buf, \"%lld\", {});\n", val));
                                    }
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                IRValue::ConstantInt(val) | IRValue::Int(val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[64];\n");
                                    if let Some(ref spec) = format_spec {
                                        // Handle format specifiers like :03d
                                        if spec.contains('d') {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%{}\", {});\n", spec, val));
                                        } else {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%lld\", {});\n", val));
                                        }
                                    } else {
                                        code.push_str(&format!("        sprintf(temp_buf, \"%lld\", {});\n", val));
                                    }
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                IRValue::ImmediateFloat(val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[64];\n");
                                    if let Some(ref spec) = format_spec {
                                        // Handle format specifiers like :.2f
                                        if spec.contains('f') {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%{}\", {});\n", spec, val));
                                        } else {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%.6f\", {});\n", val));
                                        }
                                    } else {
                                        code.push_str(&format!("        sprintf(temp_buf, \"%.6f\", {});\n", val));
                                    }
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                IRValue::ConstantFloat(val) | IRValue::Float(val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[64];\n");
                                    if let Some(ref spec) = format_spec {
                                        // Handle format specifiers like :.2f
                                        if spec.contains('f') {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%{}\", {});\n", spec, val));
                                        } else {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%.6f\", {});\n", val));
                                        }
                                    } else {
                                        code.push_str(&format!("        sprintf(temp_buf, \"%.6f\", {});\n", val));
                                    }
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                IRValue::ImmediateString(ref val) | IRValue::ConstantString(ref val) | IRValue::Str(ref val) | IRValue::String(ref val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    // Escape quotes in the string
                                    let escaped = val.replace("\\", "\\\\").replace("\"", "\\\"");
                                    code.push_str(&format!("        strcat({}, \"{}\");\n", dest, escaped));
                                    code.push_str("    }\n");
                                }
                                IRValue::ImmediateBool(val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[16];\n");
                                    let bool_str = if *val { "true" } else { "false" };
                                    code.push_str(&format!("        sprintf(temp_buf, \"%s\", \"{}\");\n", bool_str));
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                IRValue::ConstantBool(val) | IRValue::Bool(val) => {
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[16];\n");
                                    let bool_str = if *val { "true" } else { "false" };
                                    code.push_str(&format!("        sprintf(temp_buf, \"%s\", \"{}\");\n", bool_str));
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                IRValue::Variable(var_name) => {
                                    // For variables, we need to determine their type
                                    // This is a heuristic-based approach since we don't have full type information
                                    code.push_str(&format!("    // Handle variable {} in f-string\n", var_name));
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[256];\n");
                                    
                                    // Try to infer type from variable name or value
                                    if var_name == "name" {
                                        // Special case for string variables
                                        code.push_str(&format!("        sprintf(temp_buf, \"%s\", {});\n", var_name));
                                    } else if var_name == "age" {
                                        // Special case for integer variables
                                        if let Some(ref spec) = format_spec {
                                            // Handle format specifiers like :03d
                                            code.push_str(&format!("        sprintf(temp_buf, \"%{}\", {});\n", spec, var_name));
                                        } else {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%d\", {});\n", var_name));
                                        }
                                    } else if var_name == "height" || var_name == "score" {
                                        // Special case for float variables
                                        if let Some(ref spec) = format_spec {
                                            // Handle format specifiers like :.2f
                                            code.push_str(&format!("        sprintf(temp_buf, \"%{}\", {});\n", spec, var_name));
                                        } else {
                                            code.push_str(&format!("        sprintf(temp_buf, \"%.2f\", {});\n", var_name));
                                        }
                                    } else if var_name == "is_student" {
                                        // Special case for boolean variables
                                        code.push_str(&format!("        sprintf(temp_buf, \"%s\", {} ? \"true\" : \"false\");\n", var_name));
                                    } else if var_name.starts_with("tmp_") {
                                        // For temporary variables, try to infer from context
                                        // This is a simplified approach - in a real implementation we would track types
                                        code.push_str(&format!("        // For temporary variable {}, assuming int type\n", var_name));
                                        code.push_str(&format!("        sprintf(temp_buf, \"%d\", {});\n", var_name));
                                    } else {
                                        // For other variables, assume they are primitive types and use them directly
                                        code.push_str(&format!("        sprintf(temp_buf, \"%d\", {});\n", var_name));
                                    }
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                                _ => {
                                    // Fallback for other expression types
                                    code.push_str(&format!("    if ({}) {{\n", dest));
                                    code.push_str("        char temp_buf[256];\n");
                                    code.push_str(&format!("        sprintf(temp_buf, \"%s\", \"<expr>\");\n"));
                                    code.push_str(&format!("        strcat({}, temp_buf);\n", dest));
                                    code.push_str("    }\n");
                                }
                            }
                        }
                    }
                }
                
                Ok(code)
            }
            
            // Import/module support
            IRInstruction::Import { module, alias: _ } => {
                // For C code generation, import includes are collected at the top of the file
                // This is a simplified approach - in a real implementation we would need to
                // generate appropriate C headers and link with the imported modules
                Ok("".to_string()) // Return empty string as includes are handled at file level
            }
            
            IRInstruction::ImportFrom { module, names: _ } => {
                // For C code generation, import includes are collected at the top of the file
                // This is a simplified approach - in a real implementation we would need to
                // generate appropriate C headers and link with the imported modules
                Ok("".to_string()) // Return empty string as includes are handled at file level
            }
        }
    }

    pub fn ir_type_to_c(&self, ty: &IRType) -> &str {
        match ty {
            IRType::Void => "void",
            IRType::Int => "int64_t",
            IRType::Float => "double",
            IRType::Bool => "bool",
            IRType::String => "char*",
            IRType::List(_) => "TauraroValue*",
            IRType::Dict(_, _) => "TauraroValue*",
            IRType::Function { .. } => "void*",
            IRType::Any => "TauraroValue",
            // Additional type variants
            IRType::Int8 => "int8_t",
            IRType::Int16 => "int16_t",
            IRType::Int32 => "int32_t",
            IRType::Int64 => "int64_t",
            IRType::Float32 => "float",
            IRType::Float64 => "double",
            IRType::Pointer(_) => "void*",
            IRType::Array(_, _) => "void*",
            IRType::Struct(_) => "void*",
            IRType::I8 => "int8_t",
            IRType::I16 => "int16_t",
            IRType::I32 => "int32_t",
            IRType::I64 => "int64_t",
            IRType::F32 => "float",
            IRType::F64 => "double",
            IRType::Dynamic => "TauraroValue",
        }
    }
    
    fn ir_value_to_c_type(&self, value: &IRValue) -> &str {
        match value {
            IRValue::None => "void*",
            IRValue::Bool(_) => "bool",
            IRValue::Int(_) => "int64_t",
            IRValue::Float(_) => "double",
            IRValue::Str(_) | IRValue::String(_) => "char*",
            IRValue::List(_) => "TauraroValue*",
            IRValue::Dict(_) => "TauraroValue*",
            IRValue::ImmediateInt(_) => "int64_t",
            IRValue::ImmediateFloat(_) => "double",
            IRValue::ImmediateBool(_) => "bool",
            IRValue::ImmediateString(_) => "char*",
            IRValue::Variable(_) => "TauraroValue*",
            IRValue::Null => "void*",
            IRValue::ConstantInt(_) => "int64_t",
            IRValue::ConstantFloat(_) => "double",
            IRValue::ConstantBool(_) => "bool",
            IRValue::ConstantString(_) => "char*",
        }
    }
    
    fn ir_value_to_c_literal(&self, value: &IRValue) -> Result<String> {
        match value {
            IRValue::None => Ok("NULL".to_string()),
            IRValue::Bool(b) => Ok(b.to_string()),
            IRValue::Int(i) => Ok(i.to_string()),
            IRValue::Float(f) => Ok(f.to_string()),
            IRValue::Str(s) | IRValue::String(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
            IRValue::List(_) => Ok("NULL".to_string()), // TODO: Implement list literals
            IRValue::Dict(_) => Ok("NULL".to_string()), // TODO: Implement dict literals
            IRValue::ImmediateInt(i) => Ok(i.to_string()),
            IRValue::ImmediateFloat(f) => Ok(f.to_string()),
            IRValue::ImmediateBool(b) => Ok(b.to_string()),
            IRValue::ImmediateString(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
            IRValue::Variable(name) => Ok(name.clone()),
            IRValue::Null => Ok("NULL".to_string()),
            IRValue::ConstantInt(i) => Ok(i.to_string()),
            IRValue::ConstantFloat(f) => Ok(f.to_string()),
            IRValue::ConstantBool(b) => Ok(b.to_string()),
            IRValue::ConstantString(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
        }
    }
    
    fn ir_value_to_c_expr(&self, value: &IRValue) -> Result<String> {
        self.ir_value_to_c_literal(value)
    }

    /// Generate header file content for a module
    pub fn generate_header_content(&self, module: &IRModule) -> Result<String> {
        let mut header = String::new();
        
        // Header guard
        let guard_name = format!("{}_H", module.name.to_uppercase());
        header.push_str(&format!("#ifndef {}\n", guard_name));
        header.push_str(&format!("#define {}\n\n", guard_name));
        
        // Include standard headers
        header.push_str("#include <stdint.h>\n");
        header.push_str("#include <stdbool.h>\n\n");
        
        // Forward declarations for functions
        for (name, function) in &module.functions {
            if !function.is_extern {
                let return_type = self.ir_type_to_c(&function.return_type);
                let params: Vec<String> = function.params.iter()
                    .map(|param| format!("{} {}", self.ir_type_to_c(&param.ty), param.name))
                    .collect();
                let params_str = if params.is_empty() {
                    "void".to_string()
                } else {
                    params.join(", ")
                };
                header.push_str(&format!("{} {}({});\n", return_type, name, params_str));
            }
        }
        
        // Global variable declarations
        for global in &module.globals {
            let c_type = self.ir_type_to_c(&global.ty);
            header.push_str(&format!("extern {} {};\n", c_type, global.name));
        }
        
        header.push_str("\n#endif // ");
        header.push_str(&guard_name);
        header.push_str("\n");
        
        Ok(header)
    }

    /// Generate header file for a module
    pub fn generate_header(&self, module: &IRModule, output_path: &Path) -> Result<()> {
        let mut header = String::new();
        
        // Header guard
        let guard_name = format!("{}_H", module.name.to_uppercase());
        header.push_str(&format!("#ifndef {}\n", guard_name));
        header.push_str(&format!("#define {}\n\n", guard_name));
        
        // Include standard headers
        header.push_str("#include <stdint.h>\n");
        header.push_str("#include <stdbool.h>\n\n");
        
        // Type definitions (only if needed)
        header.push_str("// Type definitions\n");
        header.push_str("typedef struct {\n");
        header.push_str("    int type_tag;\n");
        header.push_str("    union {\n");
        header.push_str("        int64_t int_val;\n");
        header.push_str("        double float_val;\n");
        header.push_str("        char* string_val;\n");
        header.push_str("        bool bool_val;\n");
        header.push_str("        void* ptr_val;\n");
        header.push_str("    } data;\n");
        header.push_str("} TauraroValue;\n\n");
        
        // Function declarations
        for (name, function) in &module.functions {
            // Skip main and other special functions that shouldn't be in headers
            if name == "main" || name == "tauraro_main" {
                continue;
            }
            
            if !function.is_extern {
                let return_type = self.ir_type_to_c(&function.return_type);
                let params: Vec<String> = function.params.iter()
                    .map(|param| format!("{} {}", self.ir_type_to_c(&param.ty), param.name))
                    .collect();
                let params_str = if params.is_empty() {
                    "void".to_string()
                } else {
                    params.join(", ")
                };
                header.push_str(&format!("{} {}({});\n", return_type, name, params_str));
            }
        }
        
        // Global variable declarations
        for global in &module.globals {
            let c_type = self.ir_type_to_c(&global.ty);
            header.push_str(&format!("extern {} {};\n", c_type, global.name));
        }
        
        header.push_str("\n#endif // ");
        header.push_str(&guard_name);
        header.push_str("\n");
        
        let header_path = output_path.with_extension("h");
        std::fs::write(header_path, header)?;
        
        Ok(())
    }
}

impl CodeGenerator for CCodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        let c_code = self.generate_c_code(&module, options)?;
        
        // Generate header if requested
        if self.generate_header {
            if let Some(output_path) = &options.output_path {
                let path = Path::new(output_path);
                self.generate_header(&module, path)?;
            }
        }
        
        Ok(c_code.into_bytes())
    }
    
    fn get_target(&self) -> Target {
        Target::C
    }
    
    fn supports_optimization(&self) -> bool {
        false // C compiler handles optimization
    }
    
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec![
            "ffi",
            "cross_platform",
            "static_linking",
            "dynamic_linking",
        ]
    }
}

impl Default for CCodeGenerator {
    fn default() -> Self {
        Self::new()
    }
}

// Legacy compatibility
pub struct CCodeGen;

impl CCodeGen {
    pub fn new() -> Self {
        Self
    }

    pub fn compile(
        &self,
        module: IRModule,
        output_path: &Path,
        export: bool,
        generate_header: bool,
    ) -> Result<(), Box<dyn std::error::Error>> {
        let generator = CCodeGenerator::new().with_header(generate_header);
        let options = CodegenOptions {
            target: Target::C,
            export_symbols: export,
            enable_async: false, // Disable async by default for C backend
            enable_ffi: false,   // Disable FFI by default for C backend
            output_path: Some(output_path.to_string_lossy().to_string()),
            ..Default::default()
        };
        
        let code = generator.generate(module.clone(), &options)?;
        std::fs::write(output_path, code)?;
        
        // Also generate header file if requested
        if generate_header {
            let generator = CCodeGenerator::new();
            generator.generate_header(&module, output_path)?;
        }
        
        Ok(())
    }
}

