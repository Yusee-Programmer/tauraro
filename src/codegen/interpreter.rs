//! Interactive Interpreter for TauraroLang
//! Handles REPL functionality and interactive execution

use super::{CodeGenerator, CodegenOptions, Target};
use crate::ast::{Program, Statement, Expr, Literal, BinaryOp, UnaryOp, Comprehension, FormatPart, CompareOp};
use crate::value::{Value, self};
use crate::lexer::{Lexer, Token};
use crate::parser::Parser;
use crate::ir::{Generator, IRModule, IRFunction, IRBlock, IRInstruction, IRValue, IRType};
use crate::vm::VM;
use crate::ast::{Param, ParamKind};
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use serde::{Serialize, Deserialize};
use std::io::{self, Write};
use rustyline::validate::{Validator, ValidationResult};
use rustyline::error::ReadlineError;
use rustyline::completion::{Completer, Pair};
use rustyline::highlight::Highlighter;
use rustyline::hint::Hinter;
use rustyline_derive::Helper;
use rustyline::Editor;

/// Bytecode instruction for the interpreter
#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum BytecodeInstruction {
    // Stack operations
    LoadConst(IRValue),
    LoadLocal(usize),
    StoreLocal(usize),
    LoadGlobal(String),
    StoreGlobal(String),
    
    // Arithmetic operations
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Pow,
    
    // Comparison operations
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
    
    // Logical operations
    And,
    Or,
    Not,
    
    // Control flow
    Jump(usize),
    JumpIfFalse(usize),
    JumpIfTrue(usize),
    Call(String, usize), // function name, arg count
    Return,
    
    // Object operations
    GetAttr(String),
    SetAttr(String),
    GetItem,
    SetItem,
    
    // Collection operations
    BuildList(usize),
    BuildDict(usize),
    BuildTuple(usize),
    
    // Async operations
    Await,
    Yield(Option<IRValue>),
    
    // Exception handling
    Raise,
    PushExceptionHandler(usize),
    PopExceptionHandler,
    
    // Debugging
    Nop,
    Print,
}

/// Bytecode function
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct BytecodeFunction {
    pub name: String,
    pub instructions: Vec<BytecodeInstruction>,
    pub local_count: usize,
    pub param_count: usize,
    pub params: Vec<Param>,
    pub is_async: bool,
    pub is_generator: bool,
}

/// Bytecode module
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct BytecodeModule {
    pub functions: HashMap<String, BytecodeFunction>,
    pub globals: HashMap<String, IRValue>,
    pub entry_point: Option<String>,
}

/// Interpreter code generator
pub struct InterpreterCodeGenerator {
    optimize: bool,
}

impl InterpreterCodeGenerator {
    pub fn new() -> Self {
        Self { optimize: false }
    }
    
    /// Compile IR module to bytecode
    fn compile_to_bytecode(&self, module: IRModule) -> Result<BytecodeModule> {
        let mut bytecode_module = BytecodeModule {
            functions: HashMap::new(),
            globals: HashMap::new(),
            entry_point: None,
        };
        
        // Compile functions
        for (name, function) in module.functions {
            let bytecode_func = self.compile_function(&function)?;
            bytecode_module.functions.insert(name.clone(), bytecode_func);
            
            // Set entry point if this is main
            if name == "main" {
                bytecode_module.entry_point = Some(name);
            }
        }
        
        // Set global variables
        for global in module.globals {
            if let Some(value) = global.value {
                bytecode_module.globals.insert(global.name, value);
            }
        }
        
        Ok(bytecode_module)
    }
    
    /// Compile a single function to bytecode
    fn compile_function(&self, function: &IRFunction) -> Result<BytecodeFunction> {
        let mut instructions = Vec::new();
        let mut local_map = HashMap::new();
        let mut local_count = 0;
        let mut labels = HashMap::new();
        let mut jumps = Vec::new();

        // Map parameters to locals
        for (i, param) in function.params.iter().enumerate() {
            local_map.insert(param.name.clone(), i);
            local_count = i + 1;
        }

        // First pass: find all labels
        let mut offset = 0;
        for block in &function.blocks {
            for instruction in &block.instructions {
                if let IRInstruction::Label(name) = instruction {
                    labels.insert(name.clone(), offset);
                }
                offset += 1;
            }
        }

        // Second pass: compile instructions
        for block in &function.blocks {
            for instruction in &block.instructions {
                let current_offset = instructions.len();
                match instruction {
                    IRInstruction::Jmp { label } => {
                        instructions.push(BytecodeInstruction::Jump(0));
                    }
                    IRInstruction::Br { cond: _, then_label, else_label } => {
                        jumps.push((current_offset, then_label.clone()));
                        instructions.push(BytecodeInstruction::JumpIfTrue(0));
                        jumps.push((current_offset + 1, else_label.clone()));
                        instructions.push(BytecodeInstruction::Jump(0));
                    }
                    _ => {
                        self.compile_instruction(instruction, &mut instructions, &mut local_map, &mut local_count)?;
                    }
                }
            }
        }

        // Third pass: patch jumps
        for (offset, label) in jumps {
            let target = *labels.get(&label).unwrap();
            match &mut instructions[offset] {
                BytecodeInstruction::Jump(addr) => *addr = target,
                BytecodeInstruction::JumpIfFalse(addr) => *addr = target,
                BytecodeInstruction::JumpIfTrue(addr) => *addr = target,
                _ => unreachable!(),
            }
        }

        Ok(BytecodeFunction {
            name: function.name.clone(),
            instructions,
            local_count,
            param_count: function.params.len(),
            params: function.params.iter().map(|p| Param { name: p.name.clone(), type_annotation: None, default: None, kind: ParamKind::Positional }).collect(),
            is_async: function.is_async,
            is_generator: false, // Remove is_generator field reference
        })
    }
    
    /// Compile a single IR instruction to bytecode
    fn compile_instruction(
        &self,
        instruction: &IRInstruction,
        bytecode: &mut Vec<BytecodeInstruction>,
        local_map: &mut HashMap<String, usize>,
        local_count: &mut usize,
    ) -> Result<()> {
        match instruction {
            IRInstruction::LoadConst { dest: _, value } => {
                bytecode.push(BytecodeInstruction::LoadConst(value.clone()));
            }
            IRInstruction::LoadLocal { dest: _, name } => {
                if let Some(&local_idx) = local_map.get(name) {
                    bytecode.push(BytecodeInstruction::LoadLocal(local_idx));
                } else {
                    bytecode.push(BytecodeInstruction::LoadGlobal(name.clone()));
                }
            }
            IRInstruction::StoreLocal { name, value: _ } => {
                if let Some(&local_idx) = local_map.get(name) {
                    bytecode.push(BytecodeInstruction::StoreLocal(local_idx));
                } else {
                    // Create new local if not exists
                    let local_idx = *local_count;
                    local_map.insert(name.clone(), local_idx);
                    *local_count += 1;
                    bytecode.push(BytecodeInstruction::StoreLocal(local_idx));
                }
            }
            IRInstruction::LoadGlobal { dest: _, name } => {
                bytecode.push(BytecodeInstruction::LoadGlobal(name.clone()));
            }
            IRInstruction::StoreGlobal { name, value: _ } => {
                bytecode.push(BytecodeInstruction::StoreGlobal(name.clone()));
            }
            IRInstruction::Add { .. } => bytecode.push(BytecodeInstruction::Add),
            IRInstruction::Sub { .. } => bytecode.push(BytecodeInstruction::Sub),
            IRInstruction::Mul { .. } => bytecode.push(BytecodeInstruction::Mul),
            IRInstruction::Div { .. } => bytecode.push(BytecodeInstruction::Div),
            IRInstruction::Mod { .. } => bytecode.push(BytecodeInstruction::Mod),
            IRInstruction::Pow { .. } => bytecode.push(BytecodeInstruction::Pow),
            IRInstruction::CmpEq { .. } => bytecode.push(BytecodeInstruction::Eq),
            IRInstruction::CmpNe { .. } => bytecode.push(BytecodeInstruction::Ne),
            IRInstruction::CmpLt { .. } => bytecode.push(BytecodeInstruction::Lt),
            IRInstruction::CmpLe { .. } => bytecode.push(BytecodeInstruction::Le),
            IRInstruction::CmpGt { .. } => bytecode.push(BytecodeInstruction::Gt),
            IRInstruction::CmpGe { .. } => bytecode.push(BytecodeInstruction::Ge),
            IRInstruction::And { .. } => bytecode.push(BytecodeInstruction::And),
            IRInstruction::Or { .. } => bytecode.push(BytecodeInstruction::Or),
            IRInstruction::Not { .. } => bytecode.push(BytecodeInstruction::Not),
            IRInstruction::Call { func, args, .. } => {
                bytecode.push(BytecodeInstruction::Call(func.clone(), args.len()));
            }
            IRInstruction::Ret { .. } => bytecode.push(BytecodeInstruction::Return),
            IRInstruction::Jmp { label } => {
                // Placeholder, needs label resolution
                bytecode.push(BytecodeInstruction::Jump(0));
            }
            IRInstruction::Br { cond: _, then_label, else_label: _ } => {
                // Placeholder, needs label resolution
                bytecode.push(BytecodeInstruction::JumpIfFalse(0));
            }
            IRInstruction::GetAttr { attr, .. } => {
                bytecode.push(BytecodeInstruction::GetAttr(attr.clone()));
            }
            IRInstruction::SetAttr { attr, .. } => {
                bytecode.push(BytecodeInstruction::SetAttr(attr.clone()));
            }
            IRInstruction::GetItem { .. } => bytecode.push(BytecodeInstruction::GetItem),
            IRInstruction::SetItem { .. } => bytecode.push(BytecodeInstruction::SetItem),
            IRInstruction::BuildList { elements, .. } => {
                bytecode.push(BytecodeInstruction::BuildList(elements.len()));
            }
            IRInstruction::BuildDict { pairs, .. } => {
                bytecode.push(BytecodeInstruction::BuildDict(pairs.len()));
            }
            IRInstruction::BuildTuple { elements, .. } => {
                bytecode.push(BytecodeInstruction::BuildTuple(elements.len()));
            }
            IRInstruction::Await { .. } => bytecode.push(BytecodeInstruction::Await),
            IRInstruction::Yield { value } => {
                bytecode.push(BytecodeInstruction::Yield(Some(value.clone())));
            }
            IRInstruction::Raise { .. } => bytecode.push(BytecodeInstruction::Raise),
            IRInstruction::Print { .. } => bytecode.push(BytecodeInstruction::Print),
            _ => {
                bytecode.push(BytecodeInstruction::Nop);
            }
        }
        Ok(())
    }
    
    /// Optimize bytecode (basic optimizations)
    fn optimize_bytecode(&self, module: &mut BytecodeModule) {
        if !self.optimize {
            return;
        }
        
        for function in module.functions.values_mut() {
            self.optimize_function(function);
        }
    }
    
    /// Optimize a single function
    fn optimize_function(&self, function: &mut BytecodeFunction) {
        // Remove consecutive nops
        function.instructions.retain(|inst| !matches!(inst, BytecodeInstruction::Nop));
        
        // Peephole optimizations
        let mut i = 0;
        while i < function.instructions.len().saturating_sub(1) {
            match (&function.instructions[i], &function.instructions[i + 1]) {
                // LoadConst followed by StoreLocal can be optimized
                (BytecodeInstruction::LoadConst(val), BytecodeInstruction::StoreLocal(idx)) => {
                    // Could combine into a single instruction in a more advanced implementation
                }
                _ => {}
            }
            i += 1;
        }
    }
}

impl CodeGenerator for InterpreterCodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        // Compile to bytecode
        let mut bytecode_module = self.compile_to_bytecode(module)?;
        
        // Apply optimizations if requested
        if options.opt_level > 0 {
            let mut generator = Self { optimize: true };
            generator.optimize_bytecode(&mut bytecode_module);
        }
        
        // Serialize bytecode to bytes
        let serialized = bincode::serialize(&bytecode_module)
            .map_err(|e| anyhow!("Failed to serialize bytecode: {}", e))?;
        
        Ok(serialized)
    }
    
    fn get_target(&self) -> Target {
        Target::Interpreter
    }
    
    fn supports_optimization(&self) -> bool {
        true
    }
    
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec![
            "async/await",
            "generators", 
            "exceptions",
            "dynamic_typing",
            "reflection",
            "hot_reload",
        ]
    }
}

impl Default for InterpreterCodeGenerator {
    fn default() -> Self {
        Self::new()
    }
}

/// Runtime interpreter for executing bytecode
#[derive(Default, Clone)]
pub struct BytecodeInterpreter {
    stack: Vec<Value>,
    globals: HashMap<String, Value>,
    functions: HashMap<String, BytecodeFunction>,
}

impl BytecodeInterpreter {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn execute(&mut self, module: &BytecodeModule) -> Result<Value> {
        self.functions = module.functions.clone();
        if let Some(entry_point) = &module.entry_point {
            if let Some(function) = module.functions.get(entry_point) {
                self.globals = module.globals.iter().map(|(k, v)| (k.clone(), self.ir_value_to_value(v))).collect();
                return self.execute_function(function);
            }
        }
        Err(anyhow!("No entry point found"))
    }

    fn call_user_function(&mut self, name: &str, args: Vec<Value>, kwargs: HashMap<String, Value>) -> Result<Value> {
        if let Some(function) = self.functions.get(name).cloned() {
            let mut new_interpreter = self.clone();
            let mut new_locals = vec![Value::None; function.local_count];

            let mut arg_index = 0;
            let mut varargs: Vec<Value> = Vec::new();
            let mut varkwargs: HashMap<String, Value> = HashMap::new();

            for param in &function.params {
                // match param.kind {
                //     ParamKind::VarArgs => {
                //         while arg_index < args.len() {
                //             varargs.push(args[arg_index].clone());
                //             arg_index += 1;
                //         }
                //         // new_locals[param.index] = Value::List(varargs.clone());
                //     }
                //     ParamKind::VarKwargs => {
                //         for (name, value) in &kwargs {
                //             if !function.params.iter().any(|p| p.name == *name) {
                //                 varkwargs.insert(name.clone(), value.clone());
                //             }
                //         }
                //         // new_locals[param.index] = Value::Dict(varkwargs.clone());
                //     }
                //     _ => {
                //         // if let Some(value) = kwargs.get(&param.name) {
                //         //     new_locals[param.index] = value.clone();
                //         // } else if arg_index < args.len() {
                //         //     new_locals[param.index] = args[arg_index].clone();
                //         //     arg_index += 1;
                //         // } else if let Some(default_expr) = &param.default {
                //         //     let default_value = self.execute_function(default_expr)?;
                //         //     new_locals[param.index] = default_value;
                //         // } else {
                //         //     new_locals[param.index] = Value::None;
                //         // }
                //     }
                // }
            }

            new_interpreter.execute_function(&function)
        } else {
            Err(anyhow!("Function not found: {}", name))
        }
    }

    fn execute_function(&mut self, function: &BytecodeFunction) -> Result<Value> {
        let locals = vec![Value::None; function.local_count];
        // self.execute_function(function)
        Ok(Value::None)
    }

    fn ir_value_to_value(&self, ir_value: &IRValue) -> Value {
        match ir_value {
            IRValue::ImmediateInt(n) => Value::Int(*n),
            IRValue::ImmediateFloat(n) => Value::Float(*n),
            IRValue::ImmediateString(s) => Value::Str(s.clone()),
            IRValue::ImmediateBool(b) => Value::Bool(*b),
            IRValue::Null => Value::None,
            _ => Value::None,
        }
    }

    fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 + b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Str(a + &b)),
            _ => Err(anyhow!("Invalid types for addition")),
        }
    }

    fn sub_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 - b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - b as f64)),
            _ => Err(anyhow!("Invalid types for subtraction")),
        }
    }

    fn mul_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 * b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a * b as f64)),
            (Value::Str(a), Value::Int(b)) => Ok(Value::Str(a.repeat(b as usize))),
            _ => Err(anyhow!("Invalid types for multiplication")),
        }
    }

    fn div_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b as f64))
                }
            }
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b as f64))
                }
            }
            _ => Err(anyhow!("Invalid types for division")),
        }
    }

    fn mod_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a % b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a % b)),
            _ => Err(anyhow!("Invalid types for modulo")),
        }
    }

    fn pow_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a.pow(b as u32))),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a.powf(b))),
            _ => Err(anyhow!("Invalid types for power")),
        }
    }

    fn values_equal(&self, left: Value, right: Value) -> bool {
        left == right
    }

    fn lt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            _ => Err(anyhow!("Invalid types for less than comparison")),
        }
    }

    fn lte_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            _ => Err(anyhow!("Invalid types for less than or equal comparison")),
        }
    }

    fn gt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            _ => Err(anyhow!("Invalid types for greater than comparison")),
        }
    }

    fn gte_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            _ => Err(anyhow!("Invalid types for greater than or equal comparison")),
        }
    }

    fn get_attr(&self, obj: Value, name: &str) -> Result<Value> {
        match obj {
            Value::Object { fields, .. } => {
                if let Some(value) = fields.get(name) {
                    Ok(value.clone())
                } else {
                    Err(anyhow!("Attribute not found"))
                }
            }
            _ => Err(anyhow!("Not an object")),
        }
    }

    fn set_attr(&mut self, obj: Value, name: &str, value: Value) -> Result<()> {
        match obj {
            Value::Object { mut fields, .. } => {
                fields.insert(name.to_string(), value);
                Ok(())
            }
            _ => Err(anyhow!("Not an object")),
        }
    }

    fn get_item(&self, obj: Value, index: Value) -> Result<Value> {
        match (obj, index) {
            (Value::List(list), Value::Int(i)) => Ok(list[i as usize].clone()),
            (Value::Dict(dict), Value::Str(s)) => {
                if let Some(value) = dict.get(&s) {
                    Ok(value.clone())
                } else {
                    Err(anyhow!("Key not found"))
                }
            }
            _ => Err(anyhow!("Invalid types for get item")),
        }
    }

    fn set_item(&mut self, obj: Value, index: Value, value: Value) -> Result<()> {
        match (obj, index) {
            (Value::List(mut list), Value::Int(i)) => {
                list[i as usize] = value;
                Ok(())
            }
            (Value::Dict(mut dict), Value::Str(s)) => {
                dict.insert(s, value);
                Ok(())
            }
            _ => Err(anyhow!("Invalid types for set item")),
        }
    }
}

#[derive(Helper)]
struct REPLHelper {
    vm: VM,
}

impl Validator for REPLHelper {
    fn validate(&self, _ctx: &mut rustyline::validate::ValidationContext) -> rustyline::Result<ValidationResult> {
        Ok(ValidationResult::Valid(None))
    }
}

impl Completer for REPLHelper {
    type Candidate = Pair;

    fn complete(
        &self,
        line: &str,
        pos: usize,
        _ctx: &rustyline::Context<'_>,
    ) -> rustyline::Result<(usize, Vec<Pair>)> {
        let mut completions = Vec::new();
        let (start, word) = find_word(line, pos);

        // Keywords
        let keywords = vec![
            "func", "def", "aiki", "class", "iri", "if", "idan", "elif", "kokuma idan", "else", "akasi",
            "for", "duk", "while", "yayinda", "return", "maido", "break", "tsaya", "continue", "cigaba",
            "import", "shigoda", "from", "daga", "as", "dasunan", "extern", "waje", "export", "fitar",
            "async", "marasa_jira", "await", "jira", "try", "gwada", "except", "catch", "kama",
            "finally", "karshe", "raise", "throw", "jefa", "with", "tare", "yield", "bayar", "lambda",
            "dan_aiki", "match", "daidaita", "case", "yanayi", "in", "cikin", "is", "shine", "pass",
            "wuce", "global", "duniya", "nonlocal", "ba_gida", "del", "share", "assert", "tabbatar",
            "true", "True", "gaskiyane", "false", "False", "karyane", "none", "None", "null", "babu",
            "and", "dakuma", "or", "ko", "not", "ba",
        ];

        for keyword in keywords {
            if keyword.starts_with(word) {
                completions.push(Pair { display: keyword.to_string(), replacement: keyword.to_string() });
            }
        }

        // Variables
        let mut scope_index = Some(self.vm.get_current_scope());
        while let Some(idx) = scope_index {
            let scope = &self.vm.scopes[idx];
            for var in scope.variables.keys() {
                if var.starts_with(word) {
                    completions.push(Pair { display: var.clone(), replacement: var.clone() });
                }
            }
            scope_index = scope.parent;
        }

        Ok((start, completions))
    }
}

fn find_word(line: &str, pos: usize) -> (usize, &str) {
    let mut start = pos;
    while start > 0 && line.as_bytes()[start - 1].is_ascii_alphanumeric() {
        start -= 1;
    }
    (start, &line[start..pos])
}

impl Highlighter for REPLHelper {
    fn highlight<'l>(&self, line: &'l str, _pos: usize) -> std::borrow::Cow<'l, str> {
        let mut highlighted = String::new();
        let mut lexer = Lexer::new(line);
        let tokens = lexer.collect::<Result<Vec<_>, _>>();
        if let Ok(tokens) = tokens {
            for token_info in tokens {
                let color = match token_info.token {
                    Token::KwFunc | Token::KwClass | Token::KwIf | Token::KwElse | Token::KwFor | Token::KwWhile | Token::KwReturn | Token::KwBreak | Token::KwContinue | Token::KwImport | Token::KwFrom | Token::KwAs | Token::KwExtern | Token::KwExport | Token::KwAsync | Token::KwAwait | Token::KwTry | Token::KwExcept | Token::KwFinally | Token::KwRaise | Token::KwWith | Token::KwYield | Token::KwLambda | Token::KwMatch | Token::KwCase | Token::KwIn | Token::KwIs | Token::KwPass | Token::KwGlobal | Token::KwNonlocal | Token::KwDel | Token::KwAssert | Token::And | Token::Or | Token::Not => "\x1b[35m", // Magenta for keywords
                    Token::Int(_) | Token::Float(_) => "\x1b[33m", // Yellow for numbers
                    Token::StringLit(_) | Token::DocString(_) | Token::FString(_) => "\x1b[32m", // Green for strings
                    _ => "\x1b[0m", // Reset
                };
                highlighted.push_str(color);
                highlighted.push_str(&line[token_info.span.0..token_info.span.1]);
                highlighted.push_str("\x1b[0m");
            }
            return std::borrow::Cow::Owned(highlighted);
        }
        std::borrow::Cow::Borrowed(line)
    }
}

impl Hinter for REPLHelper {
    type Hint = String;
}

/// Interactive Interpreter for REPL functionality
pub struct Interpreter {
    vm: VM,
    line_number: usize,
}

impl Interpreter {
    /// Create a new interpreter instance
    pub fn new() -> Self {
        Self {
            vm: VM::new(),
            line_number: 1,
        }
    }

    /// Start the REPL (Read-Eval-Print Loop)
    pub fn repl(&mut self) -> Result<()> {
        println!("TauraroLang Interactive Interpreter");
        println!("Type 'exit' or 'quit' to exit, 'help()' for help");

        let helper = REPLHelper { vm: self.vm.clone() };
        let mut editor = Editor::<REPLHelper>::new()?;
        editor.set_helper(Some(helper));
        let mut buffer = String::new();
        
        loop {
            let prompt = if buffer.is_empty() { ">>> " } else { "... " };
            let readline = editor.readline(prompt);
            match readline {
                Ok(line) => {
                    editor.add_history_entry(line.as_str());
                    buffer.push_str(&line);
                    buffer.push('\n');

                    let tokens = Lexer::new(&buffer).collect::<Result<Vec<_>, _>>();
                    if let Ok(tokens) = tokens {
                        let mut parser = Parser::new(tokens);
                        match parser.parse() {
                            Ok(_) => {
                                self.process_input(&buffer)?;
                                buffer.clear();
                            }
                            Err(e) => {
                                if let crate::parser::ParseError::UnexpectedEof = e {
                                    // continue reading
                                } else {
                                    eprintln!("Error: {}", e);
                                    buffer.clear();
                                }
                            }
                        }
                    }
                }
                Err(ReadlineError::Interrupted) => {
                    println!("CTRL-C");
                    buffer.clear();
                }
                Err(ReadlineError::Eof) => {
                    println!("CTRL-D");
                    break;
                }
                Err(err) => {
                    println!("Error: {:?}", err);
                    break;
                }
            }
        }
        
        Ok(())
    }
    
    /// Process a complete input (single or multi-line)
    fn process_input(&mut self, input: &str) -> Result<()> {
        // Handle built-in commands
        if let Some(result) = self.handle_builtin_command(input)? {
            if !matches!(result, Value::None) {
                println!("{}", result);
            }
            self.line_number += 1;
            return Ok(());
        }
        
        match self.execute_repl_line(input) {
            Ok(Some(value)) => {
                if !matches!(value, Value::None) {
                    println!("{}", value);
                }
            }
            Ok(None) => {}
            Err(e) => {
                eprintln!("Error: {}", e);
            }
        }
        
        self.line_number += 1;
        Ok(())
    }

    /// Execute REPL input (expressions or statements) with proper function persistence
    pub fn execute_repl_line(&mut self, input: &str) -> Result<Option<Value>> {
        // Use the VM's built-in REPL execution which handles state persistence
        match self.vm.execute_repl(input, vec![]) {
            Ok(result) => {
                // Check if this was an expression by parsing
                let tokens = Lexer::new(input).collect::<Result<Vec<_>, _>>()
                    .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
                
                if !tokens.is_empty() {
                    let mut parser = Parser::new(tokens);
                    if let Ok(program) = parser.parse() {
                        // If it's a single expression statement, return its value
                        if program.statements.len() == 1 {
                            if let Statement::Expression(_) = &program.statements[0] {
                                return Ok(Some(result));
                            }
                        }
                    }
                }
                
                // For non-expression statements, don't show the result unless it's meaningful
                if !matches!(result, Value::None) {
                    Ok(Some(result))
                } else {
                    Ok(None)
                }
            }
            Err(e) => Err(e)
        }
    }

    /// Handle built-in REPL commands
    fn handle_builtin_command(&mut self, input: &str) -> Result<Option<Value>> {
        match input.trim() {
            "dir" | "dir()" => {
                let global_vars = self.vm.get_global_variables();
                let local_vars = self.vm.get_local_variables();
                
                // Combine global and local variables
                let mut all_vars = global_vars;
                all_vars.extend(local_vars);
                
                let mut names: Vec<String> = all_vars.keys().cloned().collect();
                names.sort();
                
                println!("Available variables and functions:");
                for name in names {
                    if let Some(value) = all_vars.get(&name) {
                                            let type_name = match value {
                                                Value::Closure { .. } => "function",
                                                Value::Object { .. } => "class",
                            Value::Int(_) => "int",
                            Value::Float(_) => "float",
                            Value::Str(_) => "str",
                            Value::Bool(_) => "bool",
                            Value::List(_) => "list",
                            Value::Dict(_) => "dict",
                            Value::Tuple(_) => "tuple",
                            Value::Set(_) => "set",
                            Value::FrozenSet(_) => "frozenset",
                            Value::Range { .. } => "range",
                            Value::Bytes(_) => "bytes",
                            Value::ByteArray(_) => "bytearray",
                            Value::MemoryView { .. } => "memoryview",
                            Value::NativeFunction(_) => "native_function",
                            Value::BuiltinFunction(_, _) => "builtin_function",
                            Value::Module(_, _) => "module",
                            Value::Super(_, _, _) => "super",
                            Value::TypedValue { .. } => "typed_value",
                            Value::Complex { .. } => "complex",
                            Value::Ellipsis => "ellipsis",
                            Value::NotImplemented => "NotImplementedType",
                            #[cfg(feature = "ffi")]
                            Value::ExternFunction { .. } => "extern_function",
                            Value::None => "NoneType",
                        };
                        println!("  {} ({})", name, type_name);
                    }
                }
                Ok(Some(Value::None))
            }
            "cls" => {
                // Clear screen command
                if cfg!(target_os = "windows") {
                    std::process::Command::new("cmd")
                        .args(&["/c", "cls"])
                        .status()
                        .ok();
                } else {
                    std::process::Command::new("clear")
                        .status()
                        .ok();
                }
                Ok(Some(Value::None))
            }
            "help" | "help()" => {
                println!("TauraroLang Interactive Help");
                println!("===========================");
                println!("Available commands:");
                println!("  dir()     - List all variables and functions");
                println!("  cls       - Clear the screen");
                println!("  help()    - Show this help message");
                println!("  globals() - Show global variables");
                println!("  locals()  - Show local variables");
                println!("  exit      - Exit the interpreter");
                println!("  quit      - Exit the interpreter");
                println!();
                println!("You can define functions, variables, and execute expressions.");
                println!("Example:");
                println!("  >>> def greet(name):");
                println!("  ...     return \"Hello, \" + name");
                println!("  >>> greet(\"World\")");
                Ok(Some(Value::None))
            }
            "globals" | "globals()" => {
                let vars = self.vm.get_global_variables();
                let mut result = HashMap::new();
                for (name, value) in vars {
                    result.insert(name, value);
                }
                Ok(Some(Value::Dict(result)))
            }
            "locals" | "locals()" => {
                let vars = self.vm.get_local_variables();
                let mut result = HashMap::new();
                for (name, value) in vars {
                    result.insert(name, value);
                }
                Ok(Some(Value::Dict(result)))
            }
            _ => Ok(None)
        }
    }

    /// Execute a single line (for external use)
    pub fn execute_line(&mut self, line: &str) -> Result<Value> {
        let tokens = Lexer::new(line).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        // Try to parse as program
        let program = parser.parse()?;
        
        // Execute the first statement and return a default value
        if let Some(stmt) = program.statements.first() {
            self.vm.execute_statement(stmt)?;
            return Ok(Value::None);
        }
        
        Err(anyhow::anyhow!("Unable to parse line as expression or statement"))
    }

    /// Get access to the underlying VM for advanced operations
    pub fn vm(&mut self) -> &mut VM {
        &mut self.vm
    }
}

impl Default for Interpreter {
    fn default() -> Self {
        Self::new()
    }
}
