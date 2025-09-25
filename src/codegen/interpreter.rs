//! Interactive Interpreter for TauraroLang
//! Handles REPL functionality and interactive execution

use super::{CodeGenerator, CodegenOptions, Target};
use crate::ir::{IRModule, IRFunction, IRInstruction, IRValue};
use crate::vm::VM;
use crate::value::Value;
use crate::lexer::Lexer;
use crate::parser::Parser;
use crate::ast::Statement;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use serde::{Serialize, Deserialize};
use std::io::{self, Write};

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
        
        let mut multi_line_buffer = String::new();
        let mut in_multi_line = false;
        
        loop {
            if in_multi_line {
                print!("... ");
            } else {
                print!(">>> ");
            }
            io::stdout().flush()?;
            
            let mut input = String::new();
            io::stdin().read_line(&mut input)?;
            
            let input_line = input.trim_end(); // Keep leading whitespace for indentation
            
            if input_line.trim() == "exit" || input_line.trim() == "quit" {
                break;
            }
            
            // Check if we need to start multi-line mode
            if !in_multi_line && (input_line.ends_with(':') || 
                                  input_line.trim_start().starts_with("def ") ||
                                  input_line.trim_start().starts_with("func ") ||
                                  input_line.trim_start().starts_with("aiki ") ||
                                  input_line.trim_start().starts_with("class ") ||
                                  input_line.trim_start().starts_with("if ") ||
                                  input_line.trim_start().starts_with("while ") ||
                                  input_line.trim_start().starts_with("for ")) {
                in_multi_line = true;
                multi_line_buffer.push_str(input_line);
                multi_line_buffer.push('\n');
                continue;
            }
            
            // If we're in multi-line mode
            if in_multi_line {
                if input_line.trim().is_empty() {
                    // Empty line ends multi-line input
                    in_multi_line = false;
                    let complete_input = multi_line_buffer.clone();
                    multi_line_buffer.clear();
                    
                    if !complete_input.trim().is_empty() {
                        self.process_input(&complete_input)?;
                    }
                    continue;
                } else {
                    multi_line_buffer.push_str(input_line);
                    multi_line_buffer.push('\n');
                    continue;
                }
            }
            
            // Single line input
            let input = input_line.trim();
            
            if input.is_empty() {
                continue;
            }
            
            self.process_input(input)?;
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
        // Try to parse as expression first (for immediate evaluation)
        let tokens = Lexer::new(input).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        // Try to parse as expression first
        if let Ok(program) = parser.parse() {
            if let Some(stmt) = program.statements.first() {
                match stmt {
                    Statement::Expression(expr) => {
                        // Handle expressions
                        let stmt = Statement::Expression(expr.clone());
                        match self.vm.execute_statement(&stmt) {
                            Ok(Some(value)) => {
                                return Ok(Some(value));
                            }
                            Ok(None) => return Ok(None),
                            Err(e) => return Err(e),
                        }
                    }
                    Statement::VariableDef { name, value, .. } => {
                        // Handle variable definitions directly
                        if let Some(expr) = value {
                            let val = self.vm.execute_expression(expr)?;
                            self.vm.set_variable(name, val);
                        } else {
                            self.vm.set_variable(name, Value::None);
                        }
                        return Ok(None);
                    }
                    _ => {
                        // Fall through to statement parsing below
                    }
                }
            }
        }
        
        // Try to parse as statement (including function definitions)
        let tokens = Lexer::new(input).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        if let Ok(program) = parser.parse() {
            if let Some(stmt) = program.statements.first() {
                // Handle function definitions properly in REPL
                match stmt {
                    Statement::FunctionDef { name, params, return_type: _, body, is_async: _, decorators: _ } => {
                        let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                        let func_value = Value::Function(name.clone(), param_names, body.clone());
                        self.vm.set_variable(name, func_value);
                        println!("Function '{}' defined", name);
                    }
                    Statement::ClassDef { name, bases: _, body, decorators: _, metaclass: _ } => {
                        // Create class object with methods
                        let mut class_methods = HashMap::new();
                        
                        // Process class body to extract methods
                        for stmt in body {
                            if let Statement::FunctionDef { name: method_name, params, return_type: _, body: method_body, is_async: _, decorators: _ } = stmt {
                                let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                                let method_value = Value::Function(method_name.clone(), param_names, method_body.clone());
                                class_methods.insert(method_name.clone(), method_value);
                            }
                        }
                        
                        let class_obj = Value::Object(name.clone(), class_methods);
                        self.vm.set_variable(name, class_obj);
                        println!("Class '{}' defined", name);
                    }
                    _ => {
                        match self.vm.execute_statement(stmt) {
                            Ok(Some(value)) => return Ok(Some(value)),
                            Ok(None) => {},
                            Err(e) => return Err(e),
                        }
                    }
                }
            }
        }
        
        Ok(None)
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
                            Value::Function(_, _, _) => "function",
                            Value::Object(_, _) => "class",
                            Value::Int(_) => "int",
                            Value::Float(_) => "float",
                            Value::String(_) => "str",
                            Value::Bool(_) => "bool",
                            Value::List(_) => "list",
                            Value::Dict(_) => "dict",
                            Value::Tuple(_) => "tuple",
                            Value::Set(_) => "set",
                            Value::Bytes(_) => "bytes",
                            Value::ByteArray(_) => "bytearray",
                            Value::NativeFunction(_) => "native_function",
                            Value::BuiltinFunction(_, _) => "builtin_function",
                            Value::Module(_, _) => "module",
                            Value::TypedValue { .. } => "typed_value",
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