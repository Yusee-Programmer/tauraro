/// Interpreter code generator for runtime execution
use super::{CodeGenerator, CodegenOptions, Target};
use crate::ir::{IRModule, IRFunction, IRInstruction, IRValue, IRType};
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use serde::{Serialize, Deserialize};

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
        
        // Map parameters to locals
        for (i, param) in function.params.iter().enumerate() {
            local_map.insert(param.name.clone(), i);
            local_count = i + 1;
        }
        
        // Compile instructions
        for block in &function.blocks {
            for instruction in &block.instructions {
                self.compile_instruction(instruction, &mut instructions, &mut local_map, &mut local_count)?;
            }
        }

        Ok(BytecodeFunction {
            name: function.name.clone(),
            instructions,
            local_count,
            param_count: function.params.len(),
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
            
            IRInstruction::Add { dest: _, left: _, right: _ } => {
                bytecode.push(BytecodeInstruction::Add);
            }
            
            IRInstruction::Sub { dest: _, left: _, right: _ } => {
                bytecode.push(BytecodeInstruction::Sub);
            }
            
            IRInstruction::Mul { dest: _, left: _, right: _ } => {
                bytecode.push(BytecodeInstruction::Mul);
            }
            
            IRInstruction::Div { dest: _, left: _, right: _ } => {
                bytecode.push(BytecodeInstruction::Div);
            }
            
            IRInstruction::Call { dest: _, func, args } => {
                bytecode.push(BytecodeInstruction::Call(func.clone(), args.len()));
            }
            
            IRInstruction::Ret { value } => {
                if value.is_some() {
                    // Value should already be on stack
                }
                bytecode.push(BytecodeInstruction::Return);
            }
            
            IRInstruction::Jmp { label: _ } => {
                // For now, use placeholder - would need proper label resolution
                bytecode.push(BytecodeInstruction::Jump(0));
            }
            
            IRInstruction::Br { cond: _, then_label: _, else_label: _ } => {
                bytecode.push(BytecodeInstruction::JumpIfFalse(0));
            }
            
            IRInstruction::GetAttr { dest: _, obj: _, attr } => {
                bytecode.push(BytecodeInstruction::GetAttr(attr.clone()));
            }
            
            IRInstruction::SetAttr { obj: _, attr, value: _ } => {
                bytecode.push(BytecodeInstruction::SetAttr(attr.clone()));
            }
            
            IRInstruction::GetItem { dest: _, obj: _, index: _ } => {
                bytecode.push(BytecodeInstruction::GetItem);
            }
            
            IRInstruction::SetItem { obj: _, index: _, value: _ } => {
                bytecode.push(BytecodeInstruction::SetItem);
            }
            
            IRInstruction::BuildList { dest: _, elements } => {
                bytecode.push(BytecodeInstruction::BuildList(elements.len()));
            }
            
            IRInstruction::BuildDict { dest: _, pairs } => {
                bytecode.push(BytecodeInstruction::BuildDict(pairs.len()));
            }
            
            IRInstruction::Await { dest: _, expr: _ } => {
                bytecode.push(BytecodeInstruction::Await);
            }
            
            IRInstruction::Yield { value } => {
                bytecode.push(BytecodeInstruction::Yield(Some(value.clone())));
            }
            
            IRInstruction::Raise { exception: _ } => {
                bytecode.push(BytecodeInstruction::Raise);
            }
            
            IRInstruction::Print { value: _ } => {
                bytecode.push(BytecodeInstruction::Print);
            }
            
            _ => {
                // For unsupported instructions, add a nop
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
pub struct Interpreter {
    stack: Vec<IRValue>,
    globals: HashMap<String, IRValue>,
    locals: Vec<IRValue>,
    call_stack: Vec<CallFrame>,
    exception_handlers: Vec<ExceptionHandler>,
}

#[derive(Debug, Clone)]
struct CallFrame {
    function: String,
    pc: usize, // program counter
    locals_base: usize,
}

#[derive(Debug, Clone)]
struct ExceptionHandler {
    handler_pc: usize,
    stack_size: usize,
}

impl Interpreter {
    pub fn new() -> Self {
        Self {
            stack: Vec::new(),
            globals: HashMap::new(),
            locals: Vec::new(),
            call_stack: Vec::new(),
            exception_handlers: Vec::new(),
        }
    }
    
    /// Execute bytecode module
    pub fn execute(&mut self, bytecode: &[u8]) -> Result<IRValue> {
        // Deserialize bytecode
        let module: BytecodeModule = bincode::deserialize(bytecode)
            .map_err(|e| anyhow!("Failed to deserialize bytecode: {}", e))?;
        
        // Set up globals
        self.globals = module.globals;
        
        // Find and execute entry point
        if let Some(entry_point) = &module.entry_point {
            if let Some(main_func) = module.functions.get(entry_point) {
                self.execute_function(main_func, &module.functions)
            } else {
                Err(anyhow!("Entry point function '{}' not found", entry_point))
            }
        } else {
            Err(anyhow!("No entry point specified"))
        }
    }
    
    /// Execute a single function
    fn execute_function(
        &mut self,
        function: &BytecodeFunction,
        all_functions: &HashMap<String, BytecodeFunction>,
    ) -> Result<IRValue> {
        let mut pc = 0;
        let locals_base = self.locals.len();
        
        // Allocate locals
        self.locals.resize(locals_base + function.local_count, IRValue::None);
        
        // Create call frame
        self.call_stack.push(CallFrame {
            function: function.name.clone(),
            pc: 0,
            locals_base,
        });
        
        while pc < function.instructions.len() {
            match &function.instructions[pc] {
                BytecodeInstruction::LoadConst(value) => {
                    self.stack.push(value.clone());
                }
                
                BytecodeInstruction::LoadLocal(idx) => {
                    let value = self.locals[locals_base + idx].clone();
                    self.stack.push(value);
                }
                
                BytecodeInstruction::StoreLocal(idx) => {
                    let value = self.stack.pop().unwrap_or(IRValue::None);
                    self.locals[locals_base + idx] = value;
                }
                
                BytecodeInstruction::LoadGlobal(name) => {
                    let value = self.globals.get(name).cloned().unwrap_or(IRValue::None);
                    self.stack.push(value);
                }
                
                BytecodeInstruction::StoreGlobal(name) => {
                    let value = self.stack.pop().unwrap_or(IRValue::None);
                    self.globals.insert(name.clone(), value);
                }
                
                BytecodeInstruction::Add => {
                    let b = self.stack.pop().unwrap_or(IRValue::None);
                    let a = self.stack.pop().unwrap_or(IRValue::None);
                    let result = self.binary_op_add(a, b)?;
                    self.stack.push(result);
                }
                
                BytecodeInstruction::Print => {
                    let value = self.stack.pop().unwrap_or(IRValue::None);
                    println!("{}", self.value_to_string(&value));
                    self.stack.push(IRValue::None);
                }
                
                BytecodeInstruction::Return => {
                    let return_value = self.stack.pop().unwrap_or(IRValue::None);
                    
                    // Clean up locals
                    self.locals.truncate(locals_base);
                    self.call_stack.pop();
                    
                    return Ok(return_value);
                }
                
                _ => {
                    // For now, skip unsupported instructions
                }
            }
            
            pc += 1;
        }
        
        // If we reach here without explicit return, return None
        self.locals.truncate(locals_base);
        self.call_stack.pop();
        Ok(IRValue::None)
    }
    
    /// Perform binary addition
    fn binary_op_add(&self, a: IRValue, b: IRValue) -> Result<IRValue> {
        match (a, b) {
            (IRValue::Int(a), IRValue::Int(b)) => Ok(IRValue::Int(a + b)),
            (IRValue::Float(a), IRValue::Float(b)) => Ok(IRValue::Float(a + b)),
            (IRValue::Int(a), IRValue::Float(b)) => Ok(IRValue::Float(a as f64 + b)),
            (IRValue::Float(a), IRValue::Int(b)) => Ok(IRValue::Float(a + b as f64)),
            (IRValue::String(a), IRValue::String(b)) => Ok(IRValue::String(format!("{}{}", a, b))),
            _ => Err(anyhow!("Unsupported operand types for addition")),
        }
    }
    
    /// Convert value to string representation
    fn value_to_string(&self, value: &IRValue) -> String {
        match value {
            IRValue::None => "None".to_string(),
            IRValue::Bool(b) => b.to_string(),
            IRValue::Int(i) => i.to_string(),
            IRValue::Float(f) => f.to_string(),
            IRValue::String(s) => s.clone(),
            IRValue::List(items) => {
                let items_str: Vec<String> = items.iter().map(|v| self.value_to_string(v)).collect();
                format!("[{}]", items_str.join(", "))
            }
            IRValue::Dict(pairs) => {
                let pairs_str: Vec<String> = pairs.iter()
                    .map(|(k, v)| format!("{}: {}", self.value_to_string(k), self.value_to_string(v)))
                    .collect();
                format!("{{{}}}", pairs_str.join(", "))
            }
            IRValue::ImmediateInt(i) => i.to_string(),
            IRValue::ImmediateFloat(f) => f.to_string(),
            IRValue::ImmediateBool(b) => b.to_string(),
            IRValue::ImmediateString(s) => s.clone(),
            IRValue::Variable(name) => format!("${}", name),
            IRValue::Null => "null".to_string(),
            IRValue::ConstantInt(i) => i.to_string(),
            IRValue::ConstantFloat(f) => f.to_string(),
            IRValue::ConstantBool(b) => b.to_string(),
            IRValue::ConstantString(s) => s.clone(),
        }
    }
}

impl Default for Interpreter {
    fn default() -> Self {
        Self::new()
    }
}