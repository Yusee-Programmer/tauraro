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
        eprintln!("DEBUG: Starting C transpilation with {} functions", module.functions.len());
        
        // Generate includes
        writeln!(self.output, "#include <stdio.h>")?;
        writeln!(self.output, "#include <stdlib.h>")?;
        writeln!(self.output, "#include <string.h>")?;
        writeln!(self.output, "#include <stdbool.h>")?;
        writeln!(self.output, "#include <stdint.h>")?;
        writeln!(self.output, "#include <math.h>")?;
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
        eprintln!("DEBUG: Processing instruction: {:?}", instruction);
        
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
                code.push_str(&format!("    {} = {} + {};\n", dest, left_str, right_str));
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
                code.push_str(&format!("    {} = {} * {};\n", dest, left_str, right_str));
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
            IRInstruction::Call { dest, func, args } => {
                let args_str: Vec<String> = args.iter().map(|arg| self.format_ir_value(arg)).collect();
                if let Some(dest_var) = dest {
                    // Declare destination variable if it's a temporary
                    if dest_var.starts_with("tmp_") {
                        let c_type = self.infer_return_type_from_function(func, module);
                        eprintln!("DEBUG: Declaring temp var {} with type {}", dest_var, c_type);
                        code.push_str(&format!("    {} {} = {}({});\n", c_type, dest_var, func, args_str.join(", ")));
                    } else {
                        code.push_str(&format!("    {} = {}({});\n", dest_var, func, args_str.join(", ")));
                    }
                } else {
                    code.push_str(&format!("    {}({});\n", func, args_str.join(", ")));
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
                        code.push_str(&format!("    printf(\"%p\\n\", (void*){});\n", val_str));
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
                format!("\"{}\"", s.replace("\"", "\\\""))
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
