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
        
        // Generate includes
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <stdint.h>\n");
        c_code.push_str("#include <stdbool.h>\n");
        c_code.push_str("#include <string.h>\n");
        
        // Add async support if enabled
        if options.enable_async {
            c_code.push_str("#include <pthread.h>\n");
            c_code.push_str("#include <ucontext.h>\n");
        }
        
        // Add FFI support if enabled
        if options.enable_ffi {
            c_code.push_str("#include <dlfcn.h>\n");
        }
        
        c_code.push_str("\n");
        
        // Generate type definitions
        self.generate_type_definitions(&mut c_code, module)?;
        
        // Generate global variables
        self.generate_global_variables(&mut c_code, module)?;
        
        // Generate function declarations
        for (_, function) in &module.functions {
            if function.is_extern {
                continue; // Skip extern functions
            }
            
            c_code.push_str(&self.generate_function_declaration(function)?);
            c_code.push_str(";\n");
        }
        c_code.push_str("\n");
        
        // Generate runtime support functions
        if options.enable_async {
            self.generate_async_runtime(&mut c_code)?;
        }
        
        // Generate function definitions
        for (_, function) in &module.functions {
            if function.is_extern {
                continue;
            }
            
            c_code.push_str(&self.generate_function_definition(function, options)?);
            c_code.push_str("\n");
        }
        
        // Generate main function if needed
        if module.functions.contains_key("main") {
            c_code.push_str(&self.generate_main_wrapper()?);
        }
        
        Ok(c_code)
    }
    
    /// Generate type definitions
    fn generate_type_definitions(&self, c_code: &mut String, module: &IRModule) -> Result<()> {
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
        
        // Generate async context if needed
        c_code.push_str("typedef struct {\n");
        c_code.push_str("    int state;\n");
        c_code.push_str("    void* locals;\n");
        c_code.push_str("    ucontext_t context;\n");
        c_code.push_str("} AsyncContext;\n\n");
        
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
    fn generate_async_runtime(&self, c_code: &mut String) -> Result<()> {
        c_code.push_str("// Async runtime support\n");
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
        
        Ok(())
    }
    
    /// Generate main wrapper
    fn generate_main_wrapper(&self) -> Result<String> {
        Ok("int main(int argc, char* argv[]) {\n    return tauraro_main();\n}\n".to_string())
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

    fn generate_function_definition(&self, function: &IRFunction, options: &CodegenOptions) -> Result<String> {
        let mut code = String::new();
        
        // Function signature
        code.push_str(&self.generate_function_declaration(function)?);
        code.push_str(" {\n");
        
        // Generate local variables
        let mut locals = std::collections::HashSet::new();
        for block in &function.blocks {
            for instruction in &block.instructions {
                self.collect_locals(instruction, &mut locals);
            }
        }
        
        for local in &locals {
            code.push_str(&format!("    TauraroValue {};\n", local));
        }
        
        if !locals.is_empty() {
            code.push_str("\n");
        }
        
        // Generate function body
        for block in &function.blocks {
            for instruction in &block.instructions {
                code.push_str(&self.generate_instruction(instruction, options)?);
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
            _ => {}
        }
    }
    
    /// Generate C code for a single instruction
    fn generate_instruction(&self, instruction: &IRInstruction, options: &CodegenOptions) -> Result<String> {
        match instruction {
            IRInstruction::LoadConst { dest, value } => {
                Ok(format!("    // Load constant: {} = {:?}\n", dest, value))
            }
            
            IRInstruction::Load { dest, ptr, ty: _ } => {
                Ok(format!("    // Load variable: {} = *{}\n", dest, ptr))
            }
            
            IRInstruction::Store { value, ptr } => {
                Ok(format!("    // Store variable: *{} = {:?}\n", ptr, value))
            }
            
            IRInstruction::Add { dest, left, right } => {
                Ok(format!("    // Add: {} = {:?} + {:?}\n", dest, left, right))
            }
            
            IRInstruction::Call { dest, func, args } => {
                let args_str = args.iter().map(|_| "arg").collect::<Vec<_>>().join(", ");
                if let Some(dest_var) = dest {
                    Ok(format!("    {} = {}({});\n", dest_var, func, args_str))
                } else {
                    Ok(format!("    {}({});\n", func, args_str))
                }
            }
            
            IRInstruction::Ret { value } => {
                if let Some(val) = value {
                    Ok(format!("    return {};\n", self.ir_value_to_c_expr(val)?))
                } else {
                    Ok("    return;\n".to_string())
                }
            }
            
            IRInstruction::Print { value } => {
                Ok(format!("    printf(\"%s\\n\", {});\n", self.ir_value_to_c_expr(value)?))
            }
            
            _ => Ok("    // Unsupported instruction\n".to_string()),
        }
    }

    fn ir_type_to_c(&self, ty: &IRType) -> &str {
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
            IRValue::String(_) => "char*",
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
            IRValue::String(s) => Ok(format!("\"{}\"", s.replace("\"", "\\\""))),
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

    fn generate_header(&self, module: &IRModule, output_path: &Path) -> Result<()> {
        let mut header = String::new();
        
        header.push_str("#ifndef TAURARO_GENERATED_H\n");
        header.push_str("#define TAURARO_GENERATED_H\n\n");
        header.push_str("#include <stdint.h>\n");
        header.push_str("#include <stdbool.h>\n\n");
        
        // Generate function declarations
        for (_, function) in &module.functions {
            if !function.is_extern {
                header.push_str(&self.generate_function_declaration(function)?);
                header.push_str(";\n");
            }
        }
        
        header.push_str("\n#endif // TAURARO_GENERATED_H\n");
        
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
            output_path: Some(output_path.to_string_lossy().to_string()),
            ..Default::default()
        };
        
        let code = generator.generate(module, &options)?;
        std::fs::write(output_path, code)?;
        
        Ok(())
    }
}