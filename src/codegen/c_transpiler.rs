//! C Code Transpiler - Converts TauraroLang IR to C code
use crate::ir::{IRModule, IRFunction, IRInstruction, IRValue, IRType};
use crate::ast::Type;
use crate::codegen::{CodeGenerator, Target, CodegenOptions};
use crate::vm::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::fmt::Write;

pub struct CTranspiler {
    output: String,
    indent_level: usize,
    temp_counter: usize,
    function_declarations: Vec<String>,
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

impl CodeGenerator for CTranspilerGenerator {
    fn generate(&self, module: IRModule, _options: &CodegenOptions) -> Result<Vec<u8>> {
        let mut transpiler = CTranspiler::new();
        let c_code = transpiler.transpile(&module)?;
        Ok(c_code.into_bytes())
    }

    fn get_target(&self) -> Target {
        Target::C
    }

    fn supports_optimization(&self) -> bool {
        false
    }

    fn get_supported_features(&self) -> Vec<&'static str> {
        vec!["basic_types", "functions", "control_flow"]
    }
}

impl CTranspiler {
    pub fn new() -> Self {
        Self {
            output: String::new(),
            indent_level: 0,
            temp_counter: 0,
            function_declarations: Vec::new(),
        }
    }

    pub fn transpile(&mut self, module: &IRModule) -> Result<String> {
        self.write_headers()?;
        self.write_runtime_support()?;
        
        // First pass: collect function declarations
        for (_, function) in &module.functions {
            self.collect_function_declaration(function)?;
        }
        
        // Write function declarations
        for decl in &self.function_declarations {
            writeln!(self.output, "{};", decl)?;
        }
        writeln!(self.output)?;
        
        // Second pass: write function implementations
        for (_, function) in &module.functions {
            self.transpile_function(function)?;
        }
        
        Ok(self.output.clone())
    }

    fn write_headers(&mut self) -> Result<()> {
        writeln!(self.output, "#include <stdio.h>")?;
        writeln!(self.output, "#include <stdlib.h>")?;
        writeln!(self.output, "#include <stdint.h>")?;
        writeln!(self.output, "#include <stdbool.h>")?;
        writeln!(self.output, "#include <string.h>")?;
        writeln!(self.output)?;
        Ok(())
    }

    fn write_runtime_support(&mut self) -> Result<()> {
        writeln!(self.output, "// TauraroLang Runtime Support")?;
        writeln!(self.output, "typedef enum {{")?;
        writeln!(self.output, "    TAURARO_INT,")?;
        writeln!(self.output, "    TAURARO_FLOAT,")?;
        writeln!(self.output, "    TAURARO_BOOL,")?;
        writeln!(self.output, "    TAURARO_STRING,")?;
        writeln!(self.output, "    TAURARO_NULL")?;
        writeln!(self.output, "}} TauraroValueType;")?;
        writeln!(self.output)?;
        
        writeln!(self.output, "typedef struct {{")?;
        writeln!(self.output, "    TauraroValueType type;")?;
        writeln!(self.output, "    union {{")?;
        writeln!(self.output, "        int64_t int_val;")?;
        writeln!(self.output, "        double float_val;")?;
        writeln!(self.output, "        bool bool_val;")?;
        writeln!(self.output, "        char* string_val;")?;
        writeln!(self.output, "    }} data;")?;
        writeln!(self.output, "}} TauraroValue;")?;
        writeln!(self.output)?;
        
        // Helper functions
        writeln!(self.output, "TauraroValue tauraro_make_int(int64_t val) {{")?;
        writeln!(self.output, "    TauraroValue v = {{TAURARO_INT, {{.int_val = val}}}};")?;
        writeln!(self.output, "    return v;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output)?;
        
        writeln!(self.output, "TauraroValue tauraro_make_float(double val) {{")?;
        writeln!(self.output, "    TauraroValue v = {{TAURARO_FLOAT, {{.float_val = val}}}};")?;
        writeln!(self.output, "    return v;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output)?;
        
        writeln!(self.output, "TauraroValue tauraro_make_bool(bool val) {{")?;
        writeln!(self.output, "    TauraroValue v = {{TAURARO_BOOL, {{.bool_val = val}}}};")?;
        writeln!(self.output, "    return v;")?;
        writeln!(self.output, "}}")?;
        writeln!(self.output)?;
        
        Ok(())
    }

    fn collect_function_declaration(&mut self, function: &IRFunction) -> Result<()> {
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

    fn transpile_function(&mut self, function: &IRFunction) -> Result<()> {
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
        
        // Transpile function body
        for block in &function.blocks {
            for instruction in &block.instructions {
                self.transpile_instruction(instruction)?;
            }
        }
        
        self.indent_level -= 1;
        writeln!(self.output, "}}")?;
        writeln!(self.output)?;
        Ok(())
    }

    fn transpile_instruction(&mut self, instruction: &IRInstruction) -> Result<()> {
        match instruction {
            IRInstruction::Load { dest, ptr, ty: _ } => {
                self.write_indent()?;
                writeln!(self.output, "{} = *{};", dest, ptr)?;
            }
            IRInstruction::Store { value, ptr } => {
                self.write_indent()?;
                writeln!(self.output, "*{} = {};", ptr, self.value_to_c_expr(value))?;
            }
            IRInstruction::Add { dest, left, right } => {
                self.write_indent()?;
                writeln!(self.output, "{} = {} + {};", dest, 
                    self.value_to_c_expr(left), self.value_to_c_expr(right))?;
            }
            IRInstruction::Sub { dest, left, right } => {
                self.write_indent()?;
                writeln!(self.output, "{} = {} - {};", dest, 
                    self.value_to_c_expr(left), self.value_to_c_expr(right))?;
            }
            IRInstruction::Mul { dest, left, right } => {
                self.write_indent()?;
                writeln!(self.output, "{} = {} * {};", dest, 
                    self.value_to_c_expr(left), self.value_to_c_expr(right))?;
            }
            IRInstruction::Div { dest, left, right } => {
                self.write_indent()?;
                writeln!(self.output, "{} = {} / {};", dest, 
                    self.value_to_c_expr(left), self.value_to_c_expr(right))?;
            }
            IRInstruction::Call { dest, func, args } => {
                self.write_indent()?;
                let args_str = args.iter()
                    .map(|arg| self.value_to_c_expr(arg))
                    .collect::<Vec<_>>()
                    .join(", ");
                
                if let Some(dest) = dest {
                    writeln!(self.output, "{} = {}({});", dest, func, args_str)?;
                } else {
                    writeln!(self.output, "{}({});", func, args_str)?;
                }
            }
            IRInstruction::Ret { value } => {
                self.write_indent()?;
                if let Some(val) = value {
                    writeln!(self.output, "return {};", self.value_to_c_expr(val))?;
                } else {
                    writeln!(self.output, "return;")?;
                }
            }
            IRInstruction::Br { cond, then_label, else_label } => {
                self.write_indent()?;
                writeln!(self.output, "if ({}) {{", self.value_to_c_expr(cond))?;
                self.indent_level += 1;
                self.write_indent()?;
                writeln!(self.output, "goto {};", then_label)?;
                self.indent_level -= 1;
                self.write_indent()?;
                writeln!(self.output, "}} else {{")?;
                self.indent_level += 1;
                self.write_indent()?;
                writeln!(self.output, "goto {};", else_label)?;
                self.indent_level -= 1;
                self.write_indent()?;
                writeln!(self.output, "}}")?;
            }
            IRInstruction::Jmp { label } => {
                self.write_indent()?;
                writeln!(self.output, "goto {};", label)?;
            }
            _ => {
                self.write_indent()?;
                writeln!(self.output, "// Unsupported instruction: {:?}", instruction)?;
            }
        }
        Ok(())
    }

    fn type_to_c_type(&self, ty: &Type) -> String {
        match ty {
            Type::Simple(name) => match name.as_str() {
                "int" => "int64_t".to_string(),
                "float" => "double".to_string(),
                "bool" => "bool".to_string(),
                "str" => "char*".to_string(),
                _ => "TauraroValue".to_string(),
            },
            Type::Generic { name, .. } => match name.as_str() {
                "list" => "TauraroValue".to_string(),
                "dict" => "TauraroValue".to_string(),
                _ => "TauraroValue".to_string(),
            },
            Type::Optional(_) => "TauraroValue".to_string(),
            Type::Function { .. } => "TauraroValue".to_string(),
            _ => "TauraroValue".to_string(),
        }
    }

    fn value_to_c_expr(&self, value: &IRValue) -> String {
        match value {
            IRValue::ImmediateInt(i) => i.to_string(),
            IRValue::ImmediateFloat(f) => f.to_string(),
            IRValue::ImmediateBool(b) => b.to_string(),
            IRValue::ImmediateString(s) => format!("\"{}\"", s.replace("\"", "\\\"")),
            IRValue::Variable(name) => name.clone(),
            IRValue::Null => "NULL".to_string(),
            IRValue::ConstantInt(i) => i.to_string(),
            IRValue::ConstantFloat(f) => f.to_string(),
            IRValue::ConstantBool(b) => b.to_string(),
            IRValue::ConstantString(s) => format!("\"{}\"", s.replace("\"", "\\\"")),
            IRValue::None => "NULL".to_string(),
            IRValue::Bool(b) => b.to_string(),
            IRValue::Int(i) => i.to_string(),
            IRValue::Float(f) => f.to_string(),
            IRValue::String(s) => format!("\"{}\"", s.replace("\"", "\\\"")),
            IRValue::List(_) => "/* TODO: List */".to_string(),
            IRValue::Dict(_) => "/* TODO: Dict */".to_string(),
        }
    }

    fn write_indent(&mut self) -> Result<()> {
        for _ in 0..self.indent_level {
            write!(self.output, "    ")?;
        }
        Ok(())
    }

    fn next_temp(&mut self) -> String {
        let temp = format!("temp_{}", self.temp_counter);
        self.temp_counter += 1;
        temp
    }

    fn ir_type_to_c_type(&self, ty: &IRType) -> String {
        match ty {
            IRType::Void => "void".to_string(),
            IRType::Int => "int64_t".to_string(),
            IRType::Float => "double".to_string(),
            IRType::Bool => "bool".to_string(),
            IRType::String => "char*".to_string(),
            IRType::List(_) => "TauraroValue*".to_string(),
            IRType::Dict(_, _) => "TauraroValue*".to_string(),
            IRType::Function { .. } => "void*".to_string(),
            IRType::Any => "TauraroValue".to_string(),
            // Additional type variants
            IRType::Int8 => "int8_t".to_string(),
            IRType::Int16 => "int16_t".to_string(),
            IRType::Int32 => "int32_t".to_string(),
            IRType::Int64 => "int64_t".to_string(),
            IRType::Float32 => "float".to_string(),
            IRType::Float64 => "double".to_string(),
            IRType::Pointer(_) => "void*".to_string(),
            IRType::Array(_, _) => "void*".to_string(),
            IRType::Struct(_) => "void*".to_string(),
            IRType::I8 => "int8_t".to_string(),
            IRType::I16 => "int16_t".to_string(),
            IRType::I32 => "int32_t".to_string(),
            IRType::I64 => "int64_t".to_string(),
            IRType::F32 => "float".to_string(),
            IRType::F64 => "double".to_string(),
            IRType::Dynamic => "TauraroValue".to_string(),
        }
    }
}