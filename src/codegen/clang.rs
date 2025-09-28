use crate::ir::{IRModule, IRFunction, IRType, IRValue, IRInstruction, IRBlock};
use crate::codegen::{CodeGenerator, Target, CodegenOptions};
use anyhow::{Result, anyhow};
use std::process::Command;
use std::fs;
use std::path::Path;

/// Clang-based native code generator
pub struct ClangCodeGenerator {
    temp_dir: String,
}

impl ClangCodeGenerator {
    pub fn new() -> Self {
        Self {
            temp_dir: std::env::temp_dir().join("tauraro_clang").to_string_lossy().to_string(),
        }
    }

    /// Check if Clang is available on the system
    pub fn is_available() -> bool {
        Command::new("clang")
            .arg("--version")
            .output()
            .map(|output| output.status.success())
            .unwrap_or(false)
    }

    /// Generate C code from IR module (reuses logic from GCC generator)
    fn generate_c_code(&self, module: &IRModule) -> Result<String> {
        let mut c_code = String::new();
        
        // Add standard includes
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <stdint.h>\n");
        c_code.push_str("#include <stdbool.h>\n\n");

        // Generate type definitions
        for (name, ty) in &module.types {
            match ty {
                IRType::Struct(fields) => {
                    c_code.push_str(&format!("typedef struct {{\n"));
                    for (i, field_type) in fields.iter().enumerate() {
                        c_code.push_str(&format!("    {} field{};\n", 
                            self.ir_type_to_c_type(field_type)?, i));
                    }
                    c_code.push_str(&format!("}} {};\n\n", name));
                }
                _ => {} // Other types don't need typedef
            }
        }

        // Generate function declarations
        for (_, function) in &module.functions {
            c_code.push_str(&self.generate_function_declaration(function)?);
        }

        // Generate function implementations
        for (_, function) in &module.functions {
            c_code.push_str(&self.generate_function_implementation(function)?);
        }

        Ok(c_code)
    }

    fn generate_function_declaration(&self, function: &IRFunction) -> Result<String> {
        let return_type = self.ir_type_to_c_type(&function.return_type)?;
        let mut params = Vec::new();
        
        for param in &function.params {
            params.push(format!("{} {}", 
                self.ir_type_to_c_type(&param.ty)?, param.name));
        }
        
        let params_str = if params.is_empty() { "void".to_string() } else { params.join(", ") };
        Ok(format!("{} {}({});\n", return_type, function.name, params_str))
    }

    fn generate_function_implementation(&self, function: &IRFunction) -> Result<String> {
        let return_type = self.ir_type_to_c_type(&function.return_type)?;
        let mut params = Vec::new();
        
        for param in &function.params {
            params.push(format!("{} {}", 
                self.ir_type_to_c_type(&param.ty)?, param.name));
        }
        
        let params_str = if params.is_empty() { "void".to_string() } else { params.join(", ") };
        let mut impl_code = format!("{} {}({}) {{\n", return_type, function.name, params_str);
        
        // Generate local variables (remove this section since IRFunction doesn't have locals field)
        
        // Generate basic blocks
        for block in &function.blocks {
            impl_code.push_str(&self.generate_block(block)?);
        }
        
        impl_code.push_str("}\n\n");
        Ok(impl_code)
    }

    fn generate_block(&self, block: &IRBlock) -> Result<String> {
        let mut block_code = format!("{}:\n", block.label);
        
        for instruction in &block.instructions {
            block_code.push_str(&self.generate_instruction(instruction)?);
        }
        
        Ok(block_code)
    }

    fn generate_instruction(&self, instruction: &IRInstruction) -> Result<String> {
        match instruction {
            IRInstruction::LoadConst { dest, value } => {
                Ok(format!("    {} = {};\n", dest, self.generate_value(value)?))
            }
            IRInstruction::Call { dest, func, args } => {
                let args_str = args.iter()
                    .map(|arg| self.generate_value(arg))
                    .collect::<Result<Vec<_>>>()?
                    .join(", ");
                
                if let Some(dest) = dest {
                    Ok(format!("    {} = {}({});\n", dest, func, args_str))
                } else {
                    Ok(format!("    {}({});\n", func, args_str))
                }
            }
            IRInstruction::Ret { value } => {
                if let Some(value) = value {
                    Ok(format!("    return {};\n", self.generate_value(value)?))
                } else {
                    Ok("    return;\n".to_string())
                }
            }
            IRInstruction::Br { cond, then_label, else_label } => {
                Ok(format!("    if ({}) goto {}; else goto {};\n", 
                    self.generate_value(cond)?, then_label, else_label))
            }
            IRInstruction::Jmp { label } => {
                Ok(format!("    goto {};\n", label))
            }
            _ => Ok("    // Unsupported instruction\n".to_string()),
        }
    }

    fn generate_value(&self, value: &IRValue) -> Result<String> {
        match value {
            IRValue::ImmediateInt(i) => Ok(i.to_string()),
            IRValue::ImmediateFloat(f) => Ok(f.to_string()),
            IRValue::ImmediateBool(b) => Ok(if *b { "true".to_string() } else { "false".to_string() }),
            IRValue::ImmediateString(s) => Ok(format!("\"{}\"", s)),
            IRValue::Variable(name) => Ok(name.clone()),
            IRValue::Null => Ok("NULL".to_string()),
            IRValue::ConstantInt(i) => Ok(i.to_string()),
            IRValue::ConstantFloat(f) => Ok(f.to_string()),
            IRValue::ConstantBool(b) => Ok(if *b { "true".to_string() } else { "false".to_string() }),
            IRValue::ConstantString(s) | IRValue::String(s) => Ok(format!("\"{}\"", s)),
            IRValue::None => Ok("NULL".to_string()),
            _ => Ok("0".to_string()), // Default fallback
        }
    }

    fn ir_type_to_c_type(&self, ir_type: &IRType) -> Result<String> {
        match ir_type {
            IRType::Void => Ok("void".to_string()),
            IRType::Bool => Ok("bool".to_string()),
            IRType::Int8 => Ok("int8_t".to_string()),
            IRType::Int16 => Ok("int16_t".to_string()),
            IRType::Int32 => Ok("int32_t".to_string()),
            IRType::Int64 => Ok("int64_t".to_string()),
            IRType::Float32 => Ok("float".to_string()),
            IRType::Float64 => Ok("double".to_string()),
            IRType::I8 => Ok("int8_t".to_string()),
            IRType::I16 => Ok("int16_t".to_string()),
            IRType::I32 => Ok("int32_t".to_string()),
            IRType::I64 => Ok("int64_t".to_string()),
            IRType::F32 => Ok("float".to_string()),
            IRType::F64 => Ok("double".to_string()),
            IRType::String => Ok("char*".to_string()),
            IRType::Pointer(pointee) => {
                Ok(format!("{}*", self.ir_type_to_c_type(pointee)?))
            }
            IRType::Array(element, size) => {
                Ok(format!("{}[{}]", self.ir_type_to_c_type(element)?, size))
            }
            IRType::Struct(fields) => Ok("struct".to_string()), // Will be handled by typedef
            IRType::Function { params, return_type } => Ok("void*".to_string()), // Function pointer
            _ => Ok("void".to_string()), // Default fallback
        }
    }

    fn compile_with_clang(&self, c_file: &Path, output_file: &Path, options: &CodegenOptions) -> Result<()> {
        let mut cmd = Command::new("clang");
        
        // Add optimization flags
        match options.opt_level {
            0 => cmd.arg("-O0"),
            1 => cmd.arg("-O1"),
            2 => cmd.arg("-O2"),
            3 => cmd.arg("-O3"),
            _ => cmd.arg("-O2"),
        };

        // Add debug info if requested
        if options.generate_debug_info {
            cmd.arg("-g");
        }

        // Add target triple if specified
        if let Some(ref triple) = options.target_triple {
            cmd.arg("-target").arg(triple);
        }

        // Enable modern C features
        cmd.arg("-std=c11");
        
        // Add warnings
        cmd.arg("-Wall").arg("-Wextra");

        // Input and output files
        cmd.arg(c_file)
           .arg("-o")
           .arg(output_file);

        let output = cmd.output()?;
        
        if !output.status.success() {
            let stderr = String::from_utf8_lossy(&output.stderr);
            return Err(anyhow!("Clang compilation failed: {}", stderr));
        }

        Ok(())
    }
}

impl CodeGenerator for ClangCodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        // Create temporary directory
        fs::create_dir_all(&self.temp_dir)?;
        
        // Generate C code
        let c_code = self.generate_c_code(&module)?;
        
        // Write C code to temporary file
        let c_file = Path::new(&self.temp_dir).join("output.c");
        fs::write(&c_file, c_code)?;
        
        // Compile with Clang
        let output_file = Path::new(&self.temp_dir).join("output");
        self.compile_with_clang(&c_file, &output_file, options)?;
        
        // Read compiled binary
        let binary = fs::read(&output_file)?;
        
        // Clean up temporary files
        let _ = fs::remove_file(&c_file);
        let _ = fs::remove_file(&output_file);
        
        Ok(binary)
    }

    fn get_target(&self) -> Target {
        Target::Native
    }

    fn supports_optimization(&self) -> bool {
        true
    }

    fn get_supported_features(&self) -> Vec<&'static str> {
        vec!["optimization", "debug_info", "cross_compilation", "modern_c", "warnings"]
    }
}

impl Default for ClangCodeGenerator {
    fn default() -> Self {
        Self::new()
    }
}
