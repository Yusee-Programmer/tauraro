//! Intermediate Representation (IR) for TauraroLang - Platform-independent code representation
use crate::ast::*;
use anyhow::Result;
use std::collections::HashMap;

/// IR Types - Simplified for code generation
#[derive(Debug, Clone, PartialEq)]
pub enum IRType {
    I8, I16, I32, I64,     // Integer types
    F32, F64,              // Floating point
    Bool,                  // Boolean
    Pointer(Box<IRType>),  // Pointer to type
    Array(Box<IRType>, usize), // Array type with size
    Struct(String),        // Struct type by name
    Function(Vec<IRType>, Box<IRType>), // Function type
    Void,                  // No return type
}

/// IR Values - SSA form
#[derive(Debug, Clone)]
pub enum IRValue {
    ConstantInt(i64),
    ConstantFloat(f64),
    ConstantBool(bool),
    ConstantString(String),
    Null,
    Variable(String),      // SSA variable
    Temporary(usize),      // Temporary value
}

/// IR Instructions
#[derive(Debug, Clone)]
pub enum IRInstruction {
    // Arithmetic
    Add { result: String, left: IRValue, right: IRValue },
    Sub { result: String, left: IRValue, right: IRValue },
    Mul { result: String, left: IRValue, right: IRValue },
    Div { result: String, left: IRValue, right: IRValue },
    
    // Comparison
    Eq { result: String, left: IRValue, right: IRValue },
    Neq { result: String, left: IRValue, right: IRValue },
    Lt { result: String, left: IRValue, right: IRValue },
    Gt { result: String, left: IRValue, right: IRValue },
    
    // Control flow
    Jump { target: String },                           // Unconditional jump
    Branch { condition: IRValue, true_target: String, false_target: String }, // Conditional branch
    Return { value: Option<IRValue> },
    
    // Memory
    Alloca { result: String, ir_type: IRType },        // Stack allocation
    Load { result: String, pointer: IRValue },         // Load from pointer
    Store { pointer: IRValue, value: IRValue },        // Store to pointer
    
    // Function calls
    Call { result: Option<String>, function: String, args: Vec<IRValue> },
    
    // Type conversion
    Truncate { result: String, value: IRValue, target_type: IRType },
    Extend { result: String, value: IRValue, target_type: IRType },
    FloatToInt { result: String, value: IRValue, target_type: IRType },
    IntToFloat { result: String, value: IRValue, target_type: IRType },
}

/// Basic Block in control flow graph
#[derive(Debug, Clone)]
pub struct BasicBlock {
    pub name: String,
    pub instructions: Vec<IRInstruction>,
    pub terminator: Option<IRInstruction>, // Final instruction (jump, branch, return)
}

/// IR Function
#[derive(Debug, Clone)]
pub struct IRFunction {
    pub name: String,
    pub parameters: Vec<(String, IRType)>,
    pub return_type: IRType,
    pub basic_blocks: Vec<BasicBlock>,
    pub variables: HashMap<String, IRType>, // Local variables
    pub is_async: bool,
}

/// IR Module (compilation unit)
#[derive(Debug, Clone)]
pub struct IRModule {
    pub name: String,
    pub functions: Vec<IRFunction>,
    pub globals: HashMap<String, (IRType, Option<IRValue>)>,
    pub types: HashMap<String, IRType>, // Struct types
}

/// IR Generator - Converts AST to IR
pub struct IRGenerator {
    current_module: IRModule,
    current_function: Option<IRFunction>,
    current_block: Option<BasicBlock>,
    variable_count: usize,
    block_count: usize,
    temp_count: usize,
}

impl IRGenerator {
    pub fn new(module_name: String) -> Self {
        Self {
            current_module: IRModule {
                name: module_name,
                functions: Vec::new(),
                globals: HashMap::new(),
                types: HashMap::new(),
            },
            current_function: None,
            current_block: None,
            variable_count: 0,
            block_count: 0,
            temp_count: 0,
        }
    }
    
    /// Main entry point - generate IR from AST
    pub fn generate(program: Program) -> Result<IRModule> {
        let mut generator = Self::new("main".to_string());
        generator.generate_program(program)?;
        Ok(generator.current_module)
    }
    
    /// Generate IR for entire program
    fn generate_program(&mut self, program: Program) -> Result<()> {
        for stmt in program.statements {
            self.generate_statement(stmt)?;
        }
        Ok(())
    }
    
    /// Generate IR for statement
    fn generate_statement(&mut self, stmt: Stmt) -> Result<()> {
        match stmt {
            Stmt::Function { name, parameters, return_type, body, .. } => {
                self.generate_function(name, parameters, return_type, body)?
            }
            Stmt::Expression(expr, _) => {
                self.generate_expression(expr)?;
            }
            Stmt::Assignment { target, value, .. } => {
                self.generate_assignment(target, value)?;
            }
            Stmt::Return { value, .. } => {
                self.generate_return(value)?;
            }
            _ => {
                // TODO: Implement other statement types
            }
        }
        Ok(())
    }
    
    /// Generate IR for function
    fn generate_function(
        &mut self,
        name: String,
        parameters: Vec<Parameter>,
        return_type: Option<Type>,
        body: Vec<Stmt>,
    ) -> Result<()> {
        // Convert parameters to IR types
        let ir_parameters: Vec<(String, IRType)> = parameters
            .into_iter()
            .map(|param| {
                let ir_type = self.type_to_ir(param.type_annotation.unwrap_or(Type::Any));
                (param.name, ir_type)
            })
            .collect();
        
        let ir_return_type = self.type_to_ir(return_type.unwrap_or(Type::Any));
        
        // Create function
        let function = IRFunction {
            name: name.clone(),
            parameters: ir_parameters,
            return_type: ir_return_type,
            basic_blocks: Vec::new(),
            variables: HashMap::new(),
            is_async: false,
        };
        
        self.current_function = Some(function);
        self.enter_block("entry".to_string());
        
        // Generate function body
        for stmt in body {
            self.generate_statement(stmt)?;
        }
        
        // Add implicit return if needed
        if self.current_block.as_ref().and_then(|b| b.terminator.as_ref()).is_none() {
            if matches!(self.current_function.as_ref().unwrap().return_type, IRType::Void) {
                self.add_instruction(IRInstruction::Return { value: None });
            }
        }
        
        // Finalize function
        if let Some(block) = self.current_block.take() {
            if let Some(mut func) = self.current_function.take() {
                func.basic_blocks.push(block);
                self.current_module.functions.push(func);
            }
        }
        
        Ok(())
    }
    
    /// Generate IR for expression
    fn generate_expression(&mut self, expr: Expr) -> Result<IRValue> {
        match expr {
            Expr::Int(value, _) => Ok(IRValue::ConstantInt(value)),
            Expr::Float(value, _) => Ok(IRValue::ConstantFloat(value)),
            Expr::String(value, _) => Ok(IRValue::ConstantString(value)),
            Expr::Bool(value, _) => Ok(IRValue::ConstantBool(value)),
            Expr::Identifier(name, _) => Ok(IRValue::Variable(name)),
            Expr::Binary { left, op, right, .. } => {
                self.generate_binary_operation(*left, op, *right)
            }
            Expr::Call { callee, arguments, .. } => {
                self.generate_function_call(*callee, arguments)
            }
            _ => {
                // TODO: Implement other expression types
                self.new_temp_value()
            }
        }
    }
    
    /// Generate IR for binary operation
    fn generate_binary_operation(&mut self, left: Expr, op: BinaryOp, right: Expr) -> Result<IRValue> {
        let left_val = self.generate_expression(left)?;
        let right_val = self.generate_expression(right)?;
        let result = self.new_temp_var();
        
        let instruction = match op {
            BinaryOp::Add => IRInstruction::Add { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Sub => IRInstruction::Sub { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Mul => IRInstruction::Mul { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Div => IRInstruction::Div { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Eq => IRInstruction::Eq { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Neq => IRInstruction::Neq { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Lt => IRInstruction::Lt { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Gt => IRInstruction::Gt { result: result.clone(), left: left_val, right: right_val },
            _ => {
                // TODO: Implement other operators
                return self.new_temp_value();
            }
        };
        
        self.add_instruction(instruction);
        Ok(IRValue::Variable(result))
    }
    
    /// Generate IR for function call
    fn generate_function_call(&mut self, callee: Expr, arguments: Vec<Expr>) -> Result<IRValue> {
        let callee_name = if let Expr::Identifier(name, _) = callee {
            name
        } else {
            // TODO: Handle complex callee expressions
            "unknown".to_string()
        };
        
        let arg_values: Result<Vec<IRValue>> = arguments
            .into_iter()
            .map(|arg| self.generate_expression(arg))
            .collect();
        
        let arg_values = arg_values?;
        
        let result_var = self.new_temp_var();
        
        self.add_instruction(IRInstruction::Call {
            result: Some(result_var.clone()),
            function: callee_name,
            args: arg_values,
        });
        
        Ok(IRValue::Variable(result_var))
    }
    
    /// Generate IR for assignment
    fn generate_assignment(&mut self, target: AssignTarget, value: Expr) -> Result<()> {
        let value_ir = self.generate_expression(value)?;
        
        match target {
            AssignTarget::Identifier(name, _) => {
                // For now, just store the value (simplified)
                // In real implementation, we'd need to handle variable declaration
                self.add_instruction(IRInstruction::Store {
                    pointer: IRValue::Variable(name.clone()),
                    value: value_ir,
                });
            }
            _ => {
                // TODO: Implement complex assignment targets
            }
        }
        
        Ok(())
    }
    
    /// Generate IR for return statement
    fn generate_return(&mut self, value: Option<Expr>) -> Result<()> {
        let return_value = if let Some(expr) = value {
            Some(self.generate_expression(expr)?)
        } else {
            None
        };
        
        self.add_instruction(IRInstruction::Return { value: return_value });
        Ok(())
    }
    
    // --- Utility methods ---
    
    fn enter_block(&mut self, name: String) {
        if let Some(block) = self.current_block.take() {
            if let Some(func) = &mut self.current_function {
                func.basic_blocks.push(block);
            }
        }
        self.current_block = Some(BasicBlock {
            name,
            instructions: Vec::new(),
            terminator: None,
        });
    }
    
    fn add_instruction(&mut self, instruction: IRInstruction) {
        if let Some(block) = &mut self.current_block {
            // Check if this is a terminator instruction
            if matches!(instruction, 
                IRInstruction::Jump { .. } | 
                IRInstruction::Branch { .. } | 
                IRInstruction::Return { .. }
            ) {
                block.terminator = Some(instruction);
            } else {
                block.instructions.push(instruction);
            }
        }
    }
    
    fn new_temp_var(&mut self) -> String {
        self.variable_count += 1;
        format("%{}", self.variable_count)
    }
    
    fn new_temp_value(&mut self) -> Result<IRValue> {
        self.temp_count += 1;
        Ok(IRValue::Temporary(self.temp_count))
    }
    
    fn type_to_ir(&self, tauraro_type: Type) -> IRType {
        match tauraro_type {
            Type::Int => IRType::I64,    // Default to 64-bit integers
            Type::Float => IRType::F64,  // Default to 64-bit floats
            Type::Bool => IRType::Bool,
            Type::Str => IRType::Pointer(Box::new(IRType::I8)), // String as char*
            Type::List(_) => IRType::Pointer(Box::new(IRType::I8)), // Simplified
            Type::Any => IRType::I64,    // Treat Any as 64-bit value
            _ => IRType::I64,            // Default fallback
        }
    }
}

// Display implementations for debugging
impl std::fmt::Display for IRType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            IRType::I8 => write!(f, "i8"),
            IRType::I16 => write!(f, "i16"),
            IRType::I32 => write!(f, "i32"),
            IRType::I64 => write!(f, "i64"),
            IRType::F32 => write!(f, "f32"),
            IRType::F64 => write!(f, "f64"),
            IRType::Bool => write!(f, "i1"),
            IRType::Pointer(inner) => write!(f, "{}*", inner),
            IRType::Array(inner, size) => write!(f, "[{} x {}]", size, inner),
            IRType::Struct(name) => write!(f, "%{}", name),
            IRType::Function(params, ret) => {
                write!(f, "{} (", ret)?;
                for (i, param) in params.iter().enumerate() {
                    if i > 0 { write!(f, ", ")?; }
                    write!(f, "{}", param)?;
                }
                write!(f, ")")
            }
            IRType::Void => write!(f, "void"),
        }
    }
}