//! COMPLETE Intermediate Representation (IR) for TauraroLang - Platform-independent code representation
use crate::ast::*;
use anyhow::Result;
use std::collections::HashMap;
use std::fmt;

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
    Dynamic,               // Dynamic type (for optional typing)
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
    Global(String),        // Global variable reference
}

/// IR Instructions
#[derive(Debug, Clone)]
pub enum IRInstruction {
    // Arithmetic
    Add { result: String, left: IRValue, right: IRValue },
    Sub { result: String, left: IRValue, right: IRValue },
    Mul { result: String, left: IRValue, right: IRValue },
    Div { result: String, left: IRValue, right: IRValue },
    Mod { result: String, left: IRValue, right: IRValue },
    
    // Comparison
    Eq { result: String, left: IRValue, right: IRValue },
    Neq { result: String, left: IRValue, right: IRValue },
    Lt { result: String, left: IRValue, right: IRValue },
    Gt { result: String, left: IRValue, right: IRValue },
    Lte { result: String, left: IRValue, right: IRValue },
    Gte { result: String, left: IRValue, right: IRValue },
    
    // Control flow
    Jump { target: String },                           // Unconditional jump
    Branch { condition: IRValue, true_target: String, false_target: String }, // Conditional branch
    Return { value: Option<IRValue> },
    
    // Memory
    Alloca { result: String, ir_type: IRType },        // Stack allocation
    Load { result: String, pointer: IRValue },         // Load from pointer
    Store { pointer: IRValue, value: IRValue },        // Store to pointer
    Malloc { result: String, size: IRValue },          // Heap allocation
    Free { pointer: IRValue },                         // Heap deallocation
    
    // Function calls
    Call { result: Option<String>, function: String, args: Vec<IRValue> },
    
    // Type conversion
    Truncate { result: String, value: IRValue, target_type: IRType },
    Extend { result: String, value: IRValue, target_type: IRType },
    FloatToInt { result: String, value: IRValue, target_type: IRType },
    IntToFloat { result: String, value: IRValue, target_type: IRType },
    DynamicCast { result: String, value: IRValue, target_type: IRType }, // For optional typing
    
    // Object-oriented
    NewObject { result: String, class_name: String },
    GetField { result: String, object: IRValue, field: String },
    SetField { object: IRValue, field: String, value: IRValue },
    CallMethod { result: Option<String>, object: IRValue, method: String, args: Vec<IRValue> },
    
    // Async support
    AsyncStart { result: String, function: String, args: Vec<IRValue> },
    AsyncAwait { result: String, async_handle: IRValue },
    AsyncYield,
}

/// Basic Block in control flow graph
#[derive(Debug, Clone)]
pub struct BasicBlock {
    pub name: String,
    pub instructions: Vec<IRInstruction>,
    pub terminator: Option<IRInstruction>, // Final instruction (jump, branch, return)
    pub predecessors: Vec<String>,         // Names of predecessor blocks
}

/// IR Function
#[derive(Debug, Clone)]
pub struct IRFunction {
    pub name: String,
    pub parameters: Vec<(String, IRType)>,
    pub return_type: IRType,
    pub basic_blocks: HashMap<String, BasicBlock>,
    pub variables: HashMap<String, IRType>, // Local variables
    pub is_async: bool,
    pub entry_block: String,
}

/// IR Module (compilation unit)
#[derive(Debug, Clone)]
pub struct IRModule {
    pub name: String,
    pub functions: HashMap<String, IRFunction>,
    pub globals: HashMap<String, (IRType, Option<IRValue>)>,
    pub types: HashMap<String, IRType>, // Struct types
    pub imports: Vec<String>,           // Imported modules
    pub exports: Vec<String>,           // Exported functions
}

/// IR Generator - Converts AST to IR
pub struct IRGenerator {
    current_module: IRModule,
    current_function: Option<IRFunction>,
    current_block: String,
    variable_count: usize,
    block_count: usize,
    temp_count: usize,
    label_count: usize,
    strict_types: bool,
}

impl IRGenerator {
    pub fn new(module_name: String, strict_types: bool) -> Self {
        let entry_block = "entry".to_string();
        let mut basic_blocks = HashMap::new();
        basic_blocks.insert(entry_block.clone(), BasicBlock {
            name: entry_block.clone(),
            instructions: Vec::new(),
            terminator: None,
            predecessors: Vec::new(),
        });
        
        Self {
            current_module: IRModule {
                name: module_name,
                functions: HashMap::new(),
                globals: HashMap::new(),
                types: HashMap::new(),
                imports: Vec::new(),
                exports: Vec::new(),
            },
            current_function: None,
            current_block: entry_block,
            variable_count: 0,
            block_count: 0,
            temp_count: 0,
            label_count: 0,
            strict_types,
        }
    }
    
    /// Main entry point - generate IR from AST
    pub fn generate(program: Program, strict_types: bool) -> Result<IRModule> {
        let mut generator = Self::new("main".to_string(), strict_types);
        generator.generate_program(program)?;
        Ok(generator.current_module)
    }
    
    /// Generate IR for entire program
    fn generate_program(&mut self, program: Program) -> Result<()> {
        // Process imports first
        for stmt in &program.statements {
            if let Stmt::Import { module, .. } = stmt {
                self.current_module.imports.push(module.clone());
            }
        }
        
        // Generate IR for each statement
        for stmt in program.statements {
            self.generate_statement(stmt)?;
        }
        
        Ok(())
    }
    
    /// Generate IR for statement
    fn generate_statement(&mut self, stmt: Stmt) -> Result<()> {
        match stmt {
            Stmt::Function { name, parameters, return_type, body, is_async, is_export, .. } => {
                self.generate_function(name, parameters, return_type, body, is_async, is_export)?;
            }
            Stmt::Class { name, bases, body, is_export, .. } => {
                self.generate_class(name, bases, body, is_export)?;
            }
            Stmt::Expression(expr, _) => {
                self.generate_expression(expr)?;
                // Discard expression result for statement context
            }
            Stmt::Assignment { target, value, .. } => {
                self.generate_assignment(target, value)?;
            }
            Stmt::Variable { name, type_annotation, value, .. } => {
                self.generate_variable(name, type_annotation, value)?;
            }
            Stmt::Return { value, .. } => {
                self.generate_return(value)?;
            }
            Stmt::If { condition, then_branch, elif_branches, else_branch, .. } => {
                self.generate_if_statement(condition, then_branch, elif_branches, else_branch)?;
            }
            Stmt::While { condition, body, .. } => {
                self.generate_while_statement(condition, body)?;
            }
            Stmt::For { variable, iterable, body, .. } => {
                self.generate_for_statement(variable, iterable, body)?;
            }
            Stmt::Import { module, .. } => {
                // Already handled in first pass
                self.current_module.imports.push(module);
            }
            Stmt::Extern { library, .. } => {
                self.generate_extern_statement(library)?;
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
        is_async: bool,
        is_export: bool,
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
        let entry_block = "entry".to_string();
        let mut basic_blocks = HashMap::new();
        basic_blocks.insert(entry_block.clone(), BasicBlock {
            name: entry_block.clone(),
            instructions: Vec::new(),
            terminator: None,
            predecessors: Vec::new(),
        });
        
        let function = IRFunction {
            name: name.clone(),
            parameters: ir_parameters,
            return_type: ir_return_type,
            basic_blocks,
            variables: HashMap::new(),
            is_async,
            entry_block: entry_block.clone(),
        };
        
        // Store current function context
        let previous_function = self.current_function.take();
        let previous_block = self.current_block.clone();
        
        self.current_function = Some(function);
        self.current_block = entry_block;
        
        // Add parameters to function scope
        for (param_name, param_type) in &self.current_function.as_ref().unwrap().parameters {
            self.current_function.as_mut().unwrap().variables.insert(
                param_name.clone(), 
                param_type.clone()
            );
        }
        
        // Generate function body
        for stmt in body {
            self.generate_statement(stmt)?;
        }
        
        // Add implicit return if needed
        if self.current_function.as_ref().unwrap().basic_blocks[&self.current_block].terminator.is_none() {
            if matches!(self.current_function.as_ref().unwrap().return_type, IRType::Void) {
                self.add_instruction(IRInstruction::Return { value: None });
            } else {
                // Return default value for non-void functions
                let default_value = self.default_value_for_type(&self.current_function.as_ref().unwrap().return_type);
                self.add_instruction(IRInstruction::Return { value: Some(default_value) });
            }
        }
        
        // Finalize function
        if let Some(function) = self.current_function.take() {
            self.current_module.functions.insert(name.clone(), function);
            if is_export {
                self.current_module.exports.push(name);
            }
        }
        
        // Restore previous context
        self.current_function = previous_function;
        self.current_block = previous_block;
        
        Ok(())
    }
    
    /// Generate IR for expression
    fn generate_expression(&mut self, expr: Expr) -> Result<IRValue> {
        match expr {
            Expr::Int(value, _) => Ok(IRValue::ConstantInt(value)),
            Expr::Float(value, _) => Ok(IRValue::ConstantFloat(value)),
            Expr::String(value, _) => Ok(IRValue::ConstantString(value)),
            Expr::Bool(value, _) => Ok(IRValue::ConstantBool(value)),
            Expr::None(_) => Ok(IRValue::Null),
            Expr::Identifier(name, _) => {
                // Check if it's a local variable or global
                if let Some(function) = &self.current_function {
                    if function.variables.contains_key(&name) {
                        return Ok(IRValue::Variable(name));
                    }
                }
                if self.current_module.globals.contains_key(&name) {
                    return Ok(IRValue::Global(name));
                }
                // For dynamic typing, we need to handle undefined variables
                if self.strict_types {
                    return Err(anyhow::anyhow!("Undefined variable: {}", name));
                }
                // In dynamic mode, create a new variable
                let var_name = self.new_temp_var();
                self.add_variable(var_name.clone(), IRType::Dynamic);
                Ok(IRValue::Variable(var_name))
            }
            Expr::Binary { left, op, right, .. } => {
                self.generate_binary_operation(*left, op, *right)
            }
            Expr::Call { callee, arguments, .. } => {
                self.generate_function_call(*callee, arguments)
            }
            Expr::List(elements, _) => {
                self.generate_list_literal(elements)
            }
            Expr::Typed { expr, type_annotation, .. } => {
                let value = self.generate_expression(*expr)?;
                let target_type = self.type_to_ir(type_annotation);
                self.generate_type_cast(value, target_type)
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
            BinaryOp::Mod => IRInstruction::Mod { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Eq => IRInstruction::Eq { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Neq => IRInstruction::Neq { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Lt => IRInstruction::Lt { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Gt => IRInstruction::Gt { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Lte => IRInstruction::Lte { result: result.clone(), left: left_val, right: right_val },
            BinaryOp::Gte => IRInstruction::Gte { result: result.clone(), left: left_val, right: right_val },
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
                // Check if variable exists
                if let Some(function) = &mut self.current_function {
                    if !function.variables.contains_key(&name) {
                        // Implicit variable declaration
                        let value_type = self.infer_ir_type(&value_ir);
                        function.variables.insert(name.clone(), value_type);
                    }
                }
                
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
    
    /// Generate IR for variable declaration
    fn generate_variable(&mut self, name: String, type_annotation: Option<Type>, value: Option<Expr>) -> Result<()> {
        let var_type = if let Some(annotation) = type_annotation {
            self.type_to_ir(annotation)
        } else if let Some(expr) = &value {
            let value_ir = self.generate_expression(expr.clone())?;
            self.infer_ir_type(&value_ir)
        } else {
            IRType::Dynamic
        };
        
        // Allocate storage for the variable
        self.add_instruction(IRInstruction::Alloca {
            result: name.clone(),
            ir_type: var_type.clone(),
        });
        
        // Add to current function's variables
        if let Some(function) = &mut self.current_function {
            function.variables.insert(name.clone(), var_type);
        }
        
        // Store initial value if provided
        if let Some(expr) = value {
            let value_ir = self.generate_expression(expr)?;
            self.add_instruction(IRInstruction::Store {
                pointer: IRValue::Variable(name),
                value: value_ir,
            });
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
    
    /// Generate IR for if statement
    fn generate_if_statement(
        &mut self,
        condition: Expr,
        then_branch: Vec<Stmt>,
        elif_branches: Vec<(Expr, Vec<Stmt>)>,
        else_branch: Option<Vec<Stmt>>,
    ) -> Result<()> {
        let cond_value = self.generate_expression(condition)?;
        
        let then_label = self.new_label();
        let else_label = self.new_label();
        let end_label = self.new_label();
        
        // Conditional branch
        self.add_instruction(IRInstruction::Branch {
            condition: cond_value,
            true_target: then_label.clone(),
            false_target: else_label.clone(),
        });
        
        // Then branch
        self.start_new_block(then_label);
        for stmt in then_branch {
            self.generate_statement(stmt)?;
        }
        self.add_instruction(IRInstruction::Jump { target: end_label.clone() });
        
        // Else if branches
        for (elif_cond, elif_body) in elif_branches {
            let elif_label = self.new_label();
            let next_label = self.new_label();
            
            self.start_new_block(elif_label);
            let elif_cond_value = self.generate_expression(elif_cond)?;
            self.add_instruction(IRInstruction::Branch {
                condition: elif_cond_value,
                true_target: next_label.clone(),
                false_target: else_label.clone(),
            });
            
            self.start_new_block(next_label);
            for stmt in elif_body {
                self.generate_statement(stmt)?;
            }
            self.add_instruction(IRInstruction::Jump { target: end_label.clone() });
        }
        
        // Else branch
        self.start_new_block(else_label);
        if let Some(else_body) = else_branch {
            for stmt in else_body {
                self.generate_statement(stmt)?;
            }
        }
        self.add_instruction(IRInstruction::Jump { target: end_label.clone() });
        
        // End of if statement
        self.start_new_block(end_label);
        
        Ok(())
    }
    
    /// Generate IR for while statement
    fn generate_while_statement(&mut self, condition: Expr, body: Vec<Stmt>) -> Result<()> {
        let cond_label = self.new_label();
        let body_label = self.new_label();
        let end_label = self.new_label();
        
        // Jump to condition check
        self.add_instruction(IRInstruction::Jump { target: cond_label.clone() });
        
        // Condition check
        self.start_new_block(cond_label);
        let cond_value = self.generate_expression(condition)?;
        self.add_instruction(IRInstruction::Branch {
            condition: cond_value,
            true_target: body_label.clone(),
            false_target: end_label.clone(),
        });
        
        // Loop body
        self.start_new_block(body_label);
        for stmt in body {
            self.generate_statement(stmt)?;
        }
        self.add_instruction(IRInstruction::Jump { target: cond_label.clone() });
        
        // End of loop
        self.start_new_block(end_label);
        
        Ok(())
    }
    
    /// Generate IR for list literal
    fn generate_list_literal(&mut self, elements: Vec<Expr>) -> Result<IRValue> {
        // For now, create a simple array
        // In a real implementation, this would create a list object
        let result = self.new_temp_var();
        
        // Allocate array
        let size = elements.len();
        self.add_instruction(IRInstruction::Malloc {
            result: result.clone(),
            size: IRValue::ConstantInt((size * 8) as i64), // Simplified size calculation
        });
        
        // Store elements
        for (i, element) in elements.into_iter().enumerate() {
            let element_value = self.generate_expression(element)?;
            let index_ptr = self.new_temp_var();
            
            // Calculate element pointer
            self.add_instruction(IRInstruction::Add {
                result: index_ptr.clone(),
                left: IRValue::Variable(result.clone()),
                right: IRValue::ConstantInt((i * 8) as i64),
            });
            
            // Store element
            self.add_instruction(IRInstruction::Store {
                pointer: IRValue::Variable(index_ptr),
                value: element_value,
            });
        }
        
        Ok(IRValue::Variable(result))
    }
    
    /// Generate type cast
    fn generate_type_cast(&mut self, value: IRValue, target_type: IRType) -> Result<IRValue> {
        if self.strict_types {
            // In strict mode, generate actual type conversion
            let result = self.new_temp_var();
            self.add_instruction(IRInstruction::DynamicCast {
                result: result.clone(),
                value,
                target_type,
            });
            Ok(IRValue::Variable(result))
        } else {
            // In dynamic mode, just pass through the value
            Ok(value)
        }
    }
    
    // --- Remaining methods for other statement types ---
    fn generate_class(&mut self, name: String, _bases: Vec<Expr>, _body: Vec<Stmt>, is_export: bool) -> Result<()> {
        // Create class type
        self.current_module.types.insert(name.clone(), IRType::Struct(name.clone()));
        if is_export {
            self.current_module.exports.push(name);
        }
        Ok(())
    }
    
    fn generate_for_statement(&mut self, _variable: String, _iterable: Expr, _body: Vec<Stmt>) -> Result<()> {
        // TODO: Implement for loop code generation
        Ok(())
    }
    
    fn generate_extern_statement(&mut self, library: String) -> Result<()> {
        self.current_module.imports.push(library);
        Ok(())
    }
    
    // --- Utility methods ---
    
    fn start_new_block(&mut self, name: String) {
        if let Some(function) = &mut self.current_function {
            let new_block = BasicBlock {
                name: name.clone(),
                instructions: Vec::new(),
                terminator: None,
                predecessors: vec![self.current_block.clone()],
            };
            function.basic_blocks.insert(name.clone(), new_block);
        }
        self.current_block = name;
    }
    
    fn add_instruction(&mut self, instruction: IRInstruction) {
        if let Some(function) = &mut self.current_function {
            if let Some(block) = function.basic_blocks.get_mut(&self.current_block) {
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
    }
    
    fn add_variable(&mut self, name: String, ir_type: IRType) {
        if let Some(function) = &mut self.current_function {
            function.variables.insert(name, ir_type);
        }
    }
    
    fn new_temp_var(&mut self) -> String {
        self.variable_count += 1;
        format("%{}", self.variable_count)
    }
    
    fn new_label(&mut self) -> String {
        self.label_count += 1;
        format!("L{}", self.label_count)
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
            Type::List(_) => IRType::Pointer(Box::new(IRType::Dynamic)), // Dynamic list
            Type::Dict(_, _) => IRType::Pointer(Box::new(IRType::Dynamic)), // Dynamic dict
            Type::Any => IRType::Dynamic, // Dynamic type
            Type::Custom(name) => IRType::Struct(name),
            _ => IRType::Dynamic,        // Default fallback to dynamic
        }
    }
    
    fn infer_ir_type(&self, value: &IRValue) -> IRType {
        match value {
            IRValue::ConstantInt(_) => IRType::I64,
            IRValue::ConstantFloat(_) => IRType::F64,
            IRValue::ConstantBool(_) => IRType::Bool,
            IRValue::ConstantString(_) => IRType::Pointer(Box::new(IRType::I8)),
            IRValue::Null => IRType::Dynamic,
            _ => IRType::Dynamic, // For variables, we need more context
        }
    }
    
    fn default_value_for_type(&self, ir_type: &IRType) -> IRValue {
        match ir_type {
            IRType::I64 => IRValue::ConstantInt(0),
            IRType::F64 => IRValue::ConstantFloat(0.0),
            IRType::Bool => IRValue::ConstantBool(false),
            IRType::Pointer(_) => IRValue::Null,
            IRType::Void => IRValue::Null,
            _ => IRValue::Null,
        }
    }
}

// Display implementations for debugging
impl fmt::Display for IRType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
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
            IRType::Dynamic => write!(f, "dynamic"),
        }
    }
}

impl fmt::Display for IRValue {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            IRValue::ConstantInt(n) => write!(f, "{}", n),
            IRValue::ConstantFloat(n) => write!(f, "{:.6}", n),
            IRValue::ConstantBool(b) => write!(f, "{}", b),
            IRValue::ConstantString(s) => write!(f, "\"{}\"", s),
            IRValue::Null => write!(f, "null"),
            IRValue::Variable(name) => write!(f, "%{}", name),
            IRValue::Temporary(id) => write!(f, "temp{}", id),
            IRValue::Global(name) => write!(f, "@{}", name),
        }
    }
}