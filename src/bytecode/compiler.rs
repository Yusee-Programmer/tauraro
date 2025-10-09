//! SuperCompiler - Register-based bytecode compiler with advanced optimizations

use crate::ast::*;
use crate::value::Value;
use crate::bytecode::instructions::{OpCode, Instruction};
use crate::bytecode::arithmetic::CodeObject;
use anyhow::{Result, anyhow};
use std::collections::HashMap;

/// SuperCompiler - Register-based bytecode compiler with advanced optimizations
pub struct SuperCompiler {
    pub code: CodeObject,
    next_register: u32,
    current_line: u32,
}

impl SuperCompiler {
    pub fn new(filename: String) -> Self {
        Self {
            code: CodeObject::new(filename, "<module>".to_string(), 1),
            next_register: 0,
            current_line: 0,  // Start at 0, will increment to 1 for first statement
        }
    }
    
    /// Compile a function with the given name, parameters, and body
    pub fn compile_function(name: String, params: &[Param], body: &[Statement]) -> Result<CodeObject> {
        // Create a new compiler for the function
        let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));

        // Set the code name to the function name so is_in_function_scope() works
        func_compiler.code.name = format!("<fn:{}>", name);

        // Add parameters to the function's varnames
        for param in params {
            func_compiler.code.argcount = func_compiler.code.argcount + 1;
            func_compiler.code.add_varname(param.name.clone());
        }

        // Compile the function body
        for stmt in body.iter().cloned() {
            func_compiler.compile_statement(stmt)?;
        }
        
        // Add implicit return None at end of function if no return statement
        let none_const = func_compiler.code.add_constant(Value::None);
        let reg = func_compiler.allocate_register(); // Allocate a register
        func_compiler.emit(OpCode::LoadConst, none_const, reg, 0, 0);
        func_compiler.emit(OpCode::ReturnValue, reg, 0, 0, 0);
        
        // Set the number of registers needed for the function
        func_compiler.code.registers = func_compiler.next_register;
        func_compiler.code.nlocals = func_compiler.code.varnames.len() as u32;
        
        Ok(func_compiler.code)
    }
    
    /// Check if we're currently compiling a function (not a module)
    fn is_in_function_scope(&self) -> bool {
        // We're in a function if the code object name is not "<module>"
        self.code.name != "<module>"
    }
    
    /// Get or create local variable index (only for function scope)
    fn get_local_index(&mut self, name: &str) -> u32 {
        if let Some(pos) = self.code.varnames.iter().position(|n| n == name) {
            pos as u32
        } else {
            let pos = self.code.varnames.len() as u32;
            self.code.varnames.push(name.to_string());
            pos
        }
    }
    
    pub fn compile(&mut self, program: Program) -> Result<CodeObject> {
        for stmt in program.statements {
            // Increment line number for each statement
            self.current_line += 1;
            self.compile_statement(stmt)?;
        }

        // Set the number of registers needed for the module
        self.code.registers = self.next_register;

        Ok(self.code.clone())
    }
    
    fn emit(&mut self, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32, line: u32) -> usize {
        let pos = self.code.instructions.len();
        self.code.add_instruction(opcode, arg1, arg2, arg3, line);
        pos
    }
    
    fn allocate_register(&mut self) -> u32 {
        let reg = self.next_register;
        self.next_register += 1;
        reg
    }
    
    pub fn compile_statement(&mut self, stmt: Statement) -> Result<()> {
        match stmt {
            Statement::Expression(expr) => {
                let reg = self.compile_expression(expr)?;
                // In module scope, save expression result to special global for REPL
                if !self.is_in_function_scope() {
                    let name_idx = self.code.add_name("__last_expr__".to_string());
                    self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);
                }
                Ok(())
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let value_reg = self.compile_expression(expr)?;
                    // Store in local variable if in function scope, otherwise global
                    if self.is_in_function_scope() {
                        // We're in a function scope, use fast local access
                        let local_idx = self.get_local_index(&name);
                        self.emit(OpCode::StoreFast, local_idx, value_reg, 0, self.current_line);
                    } else {
                        // Global scope - use StoreGlobal
                        let name_idx = self.code.add_name(name);
                        self.emit(OpCode::StoreGlobal, name_idx, value_reg, 0, self.current_line);
                    }
                } else {
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    if self.is_in_function_scope() {
                        // We're in a function scope, use fast local access
                        let local_idx = self.get_local_index(&name);
                        self.emit(OpCode::StoreFast, local_idx, reg, 0, self.current_line);
                    } else {
                        // Global scope - use StoreGlobal
                        let name_idx = self.code.add_name(name);
                        self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);
                    }
                }
                Ok(())
            }
            Statement::SubscriptAssignment { object, index, value } => {
                let object_reg = self.compile_expression(object)?;
                let index_reg = self.compile_expression(index)?;
                let value_reg = self.compile_expression(value)?;
                
                // Emit SubscrStore instruction to store item to sequence
                self.emit(OpCode::SubscrStore, object_reg, index_reg, value_reg, self.current_line);
                Ok(())
            }
            Statement::Return(expr) => {
                if let Some(expr) = expr {
                    let value_reg = self.compile_expression(expr)?;
                    self.emit(OpCode::ReturnValue, value_reg, 0, 0, self.current_line);
                } else {
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    self.emit(OpCode::ReturnValue, reg, 0, 0, self.current_line);
                }
                Ok(())
            }
            Statement::FunctionDef { name, params, body, .. } => {
                // Create a new compiler for the function
                let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));

                // CRITICAL: Set code.name so is_in_function_scope() works correctly
                func_compiler.code.name = format!("<fn:{}>", name);

                // Add parameters to the function's varnames
                for param in &params {
                    func_compiler.code.argcount = func_compiler.code.argcount + 1;
                    func_compiler.code.add_varname(param.name.clone());
                }
                
                // Compile the function body
                for stmt in body.clone() {
                    func_compiler.compile_statement(stmt)?;
                }
                
                // Add implicit return None at end of function if no return statement
                let none_const = func_compiler.code.add_constant(Value::None);
                let reg = func_compiler.allocate_register(); // Allocate a register
                func_compiler.emit(OpCode::LoadConst, none_const, reg, 0, 0);
                func_compiler.emit(OpCode::ReturnValue, reg, 0, 0, 0);
                
                // Set the number of registers needed for the function
                func_compiler.code.registers = func_compiler.next_register;
                func_compiler.code.nlocals = func_compiler.code.varnames.len() as u32;
                
                // Get the compiled function code
                let func_code = func_compiler.code;
                
                // Debug output to see the compiled code
                // eprintln!("DEBUG: Compiled function '{}' with {} instructions", name, func_code.instructions.len());
                
                // Create a closure value for the function with the compiled code
                let closure_value = Value::Closure {
                    name: name.clone(),
                    params: params.clone(),
                    body: vec![], // Body is encoded in the bytecode, not stored as AST
                    captured_scope: HashMap::new(), // No captured scope for now
                    docstring: None,
                    compiled_code: Some(Box::new(func_code.clone())), // Store the compiled code directly in the Closure
                };
                
                // Debug output to see if compiled_code is set
                if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = closure_value {
                    // eprintln!("DEBUG: Created Closure '{}' with compiled_code: {}", name, compiled_code.is_some());
                    if let Some(ref code) = compiled_code {
                        // eprintln!("DEBUG: Created Closure '{}' has {} instructions", name, code.instructions.len());
                    }
                }
                
                // Store the closure in constants
                let closure_const_idx = self.code.add_constant(closure_value);
                
                // Load the closure
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, closure_const_idx, reg, 0, self.current_line);
                
                // Store the function in global namespace
                let name_idx = self.code.add_name(name.clone());
                self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);
                
                // Debug output to see what's stored in constants
                // eprintln!("DEBUG: Stored Closure '{}' in constants at index {}", name, closure_const_idx);
                if let Some(stored_value) = self.code.constants.get(closure_const_idx as usize) {
                    if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = stored_value {
                        // eprintln!("DEBUG: Stored Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                        if let Some(ref code) = compiled_code {
                            // eprintln!("DEBUG: Stored Closure '{}' has {} instructions", name, code.instructions.len());
                        }
                    }
                }
                Ok(())
            }
            Statement::For { variable, iterable, body, .. } => {
                // Compile for loop: for variable in iterable:
                
                // 1. Compile the iterable expression
                let iterable_reg = self.compile_expression(iterable)?;
                
                // 2. Get an iterator from the iterable
                let iter_reg = self.allocate_register();
                self.emit(OpCode::GetIter, iterable_reg, iter_reg, 0, self.current_line);
                
                // 3. Set up the loop structure
                let loop_var_idx = if self.is_in_function_scope() {
                    // In function scope, use fast local access
                    self.get_local_index(&variable)
                } else {
                    // In global scope, we don't add to varnames, just get the name index
                    self.code.add_name(variable.clone())
                };
                
                // 4. Create jump targets
                let loop_start = self.code.instructions.len(); // Start of loop body
                
                // 5. Emit ForIter instruction with placeholder for end target
                let value_reg = self.allocate_register();
                let for_iter_pos = self.emit(OpCode::ForIter, iter_reg, value_reg, 0, self.current_line); // arg3 will be updated later
                
                // 6. Store the iterated value in the loop variable
                if self.is_in_function_scope() {
                    // In function scope, use fast local access
                    self.emit(OpCode::StoreFast, loop_var_idx, value_reg, 0, self.current_line);
                } else {
                    // In global scope, use StoreGlobal
                    self.emit(OpCode::StoreGlobal, loop_var_idx, value_reg, 0, self.current_line);
                }
                
                // 7. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 8. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, self.current_line);
                
                // 9. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                // Update the ForIter instruction with the correct target
                self.code.instructions[for_iter_pos].arg3 = loop_end_pos as u32;
                Ok(())
            }
            Statement::While { condition, body, .. } => {
                // Compile while loop: while condition: body
                
                // 1. Create jump targets
                let loop_start = self.code.instructions.len(); // Start of condition check
                let loop_body = loop_start + 1; // Start of loop body (after condition check)
                
                // 2. Compile the condition
                let cond_reg = self.compile_expression(condition)?;
                
                // 3. Emit conditional jump to end of loop if condition is false
                let loop_end_pos_ref = self.emit(OpCode::JumpIfFalse, cond_reg, 0, 0, self.current_line); // arg2 will be updated later
                
                // 4. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 5. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, self.current_line);
                
                // 6. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                self.code.instructions[loop_end_pos_ref].arg2 = loop_end_pos as u32;
                Ok(())
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                // Compile if statement: if condition: then_branch elif ... else: else_branch
                
                // 1. Compile the condition
                let cond_reg = self.compile_expression(condition)?;
                
                // 2. Emit conditional jump to else branch if condition is false
                let else_jump_pos = self.emit(OpCode::JumpIfFalse, cond_reg, 0, 0, self.current_line); // arg2 will be updated later
                
                // 3. Compile the then branch
                for stmt in then_branch {
                    self.compile_statement(stmt)?;
                }
                
                // 4. Emit jump to end of if statement (after then branch)
                let end_jump_pos = self.emit(OpCode::Jump, 0, 0, 0, self.current_line); // arg1 will be updated later
                
                // 5. Fix the else jump placeholder
                let else_start_pos = self.code.instructions.len();
                self.code.instructions[else_jump_pos].arg2 = else_start_pos as u32;
                
                // 6. Compile elif branches and else branch
                let mut elif_jump_positions = Vec::new();
                
                // Compile elif branches
                for (elif_cond, elif_body) in elif_branches {
                    // Compile elif condition
                    let elif_cond_reg = self.compile_expression(elif_cond)?;
                    
                    // Emit conditional jump to next elif/else branch if condition is false
                    let elif_else_jump_pos = self.emit(OpCode::JumpIfFalse, elif_cond_reg, 0, 0, self.current_line);
                    elif_jump_positions.push(elif_else_jump_pos);
                    
                    // Compile elif body
                    for stmt in elif_body {
                        self.compile_statement(stmt)?;
                    }
                    
                    // Emit jump to end of if statement
                    let elif_end_jump_pos = self.emit(OpCode::Jump, 0, 0, 0, self.current_line);
                    elif_jump_positions.push(elif_end_jump_pos);
                }
                
                // Compile else branch if it exists
                if let Some(else_body) = else_branch {
                    for stmt in else_body {
                        self.compile_statement(stmt)?;
                    }
                }
                
                // 7. Fix all the jump placeholders
                let end_pos = self.code.instructions.len();
                self.code.instructions[end_jump_pos].arg1 = end_pos as u32;
                
                for jump_pos in elif_jump_positions {
                    if self.code.instructions[jump_pos].opcode == OpCode::JumpIfFalse {
                        self.code.instructions[jump_pos].arg2 = end_pos as u32;
                    } else {
                        self.code.instructions[jump_pos].arg1 = end_pos as u32;
                    }
                }
                Ok(())
            }
            Statement::ClassDef { name, bases, body, decorators: _, metaclass, docstring: _ } => {
                // Create class object with methods
                let mut class_methods = HashMap::new();

                // Process class body to extract methods and attributes
                for stmt in body {
                    if let Statement::FunctionDef { name: method_name, params, return_type: _, body: method_body, is_async: _, decorators: _, docstring } = stmt {
                        // Compile the method using the compile_function helper
                        let method_code = SuperCompiler::compile_function(method_name.clone(), &params, &method_body)?;

                        let method_value = Value::Closure {
                            name: method_name.clone(),
                            params: params.clone(),
                            body: vec![], // Body is encoded in the bytecode, not stored as AST
                            captured_scope: HashMap::new(),
                            docstring: docstring.clone(),
                            compiled_code: Some(Box::new(method_code)), // Store the compiled code directly in the Closure
                        };
                        class_methods.insert(method_name.clone(), method_value);
                    }
                }

                // Extract base class names
                // For now, we only support simple identifier bases
                let mut base_names = Vec::new();
                for base_expr in bases {
                    if let Expr::Identifier(base_name) = base_expr {
                        base_names.push(base_name.clone());
                    }
                }

                // If no bases specified, inherit from object
                if base_names.is_empty() {
                    base_names.push("object".to_string());
                }

                // Build MRO (Method Resolution Order) using C3 linearization
                // For now, simple linearization: [self, bases..., object]
                let mut mro_list = vec![name.clone()];
                for base in &base_names {
                    if base != "object" && !mro_list.contains(base) {
                        mro_list.push(base.clone());
                    }
                }
                if !mro_list.contains(&"object".to_string()) {
                    mro_list.push("object".to_string());
                }

                // Extract metaclass name if provided
                let metaclass_name = if let Some(mc_expr) = metaclass {
                    if let Expr::Identifier(mc_name) = mc_expr {
                        Some(mc_name.clone())
                    } else {
                        None
                    }
                } else {
                    Some("type".to_string()) // Default metaclass is 'type'
                };

                // Create the class using the new Class variant
                let class_obj = Value::Class {
                    name: name.clone(),
                    bases: base_names.clone(),
                    methods: class_methods,
                    metaclass: metaclass_name,
                    mro: crate::base_object::MRO::from_linearization(mro_list.clone()),
                    base_object: crate::base_object::BaseObject::new(name.clone(), base_names.clone()),
                };

                // Store class as a constant, load it, and store in global namespace
                let class_const_idx = self.code.add_constant(class_obj);
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, class_const_idx, reg, 0, self.current_line);

                let name_idx = self.code.add_name(name.clone());
                self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);

                Ok(())
            }
            _ => {
                // For unimplemented statements, we'll just return Ok for now
                // In a complete implementation, we would handle all statement types
                Ok(())
            }
        }
    }
    
    fn compile_expression(&mut self, expr: Expr) -> Result<u32> {
        match expr {
            Expr::Literal(literal) => {
                let value = match literal {
                    Literal::Int(n) => Value::Int(n),
                    Literal::Float(n) => Value::Float(n),
                    Literal::String(s) => Value::Str(s),
                    Literal::Bool(b) => Value::Bool(b),
                    Literal::None => Value::None,
                    _ => return Err(anyhow!("Unsupported literal type")),
                };
                let const_idx = self.code.add_constant(value);
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, const_idx, reg, 0, self.current_line);
                Ok(reg)
            }
            Expr::Identifier(name) => {
                let reg = self.allocate_register();

                // Check if this is a local variable (only in function scope)
                if self.is_in_function_scope() {
                    // In function scope, check if this is a local variable
                    if let Some(local_idx) = self.code.varnames.iter().position(|n| n == &name) {
                        // For local variables in function scope, use fast access
                        self.emit(OpCode::LoadFast, local_idx as u32, reg, 0, self.current_line);
                    } else {
                        // Not a local variable, treat as global
                        let name_idx = self.code.add_name(name);
                        let cache_idx = self.code.add_inline_cache();
                        self.emit(OpCode::LoadGlobal, name_idx, cache_idx, reg, self.current_line);
                    }
                } else {
                    // Global scope - always treat as global variable
                    let name_idx = self.code.add_name(name);
                    let cache_idx = self.code.add_inline_cache();
                    self.emit(OpCode::LoadGlobal, name_idx, cache_idx, reg, self.current_line);
                }
                Ok(reg)
            }
            Expr::BinaryOp { left, op, right } => {
                let left_reg = self.compile_expression(*left)?;
                let right_reg = self.compile_expression(*right)?;
                let result_reg = self.allocate_register();
                
                // Check if both operands are likely to be integers for fast path
                let use_fast_int = match op {
                    BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                        // Use fast path for common arithmetic operations
                        true
                    }
                    _ => false
                };
                
                if use_fast_int {
                    let opcode = match op {
                        BinaryOp::Add => OpCode::FastIntAdd,
                        BinaryOp::Sub => OpCode::FastIntSub,
                        BinaryOp::Mul => OpCode::FastIntMul,
                        BinaryOp::Div => OpCode::BinaryDivRRFastInt,
                        BinaryOp::Mod => OpCode::BinaryModRRFastInt,
                        _ => OpCode::BinaryAddRR, // fallback
                    };
                    self.emit(opcode, left_reg, right_reg, result_reg, self.current_line);
                } else {
                    let opcode = match op {
                        BinaryOp::Add => OpCode::BinaryAddRR,
                        BinaryOp::Sub => OpCode::BinarySubRR,
                        BinaryOp::Mul => OpCode::BinaryMulRR,
                        BinaryOp::Div => OpCode::BinaryDivRR,
                        BinaryOp::Mod => OpCode::BinaryModRR,
                        BinaryOp::Pow => OpCode::BinaryPowRR,
                        BinaryOp::Eq => OpCode::CompareEqualRR,
                        BinaryOp::Ne | BinaryOp::Neq => OpCode::CompareNotEqualRR,
                        BinaryOp::Lt => OpCode::CompareLessRR,
                        BinaryOp::Gt => OpCode::CompareGreaterRR,
                        BinaryOp::Le | BinaryOp::Lte => OpCode::CompareLessEqualRR,
                        BinaryOp::Ge | BinaryOp::Gte => OpCode::CompareGreaterEqualRR,
                        BinaryOp::And => {
                            // Short-circuit AND: if left is false, return left, otherwise return right
                            // This is a simplified implementation
                            self.emit(OpCode::BinaryMulRR, left_reg, right_reg, result_reg, self.current_line);
                            return Ok(result_reg);
                        },
                        BinaryOp::Or => {
                            // Short-circuit OR: if left is true, return left, otherwise return right
                            // This is a simplified implementation
                            self.emit(OpCode::BinaryAddRR, left_reg, right_reg, result_reg, self.current_line);
                            return Ok(result_reg);
                        },
                        _ => return Err(anyhow!("Unsupported binary operation: {:?}", op)),
                    };
                    
                    self.emit(opcode, left_reg, right_reg, result_reg, self.current_line);
                }
                Ok(result_reg)
            }
            Expr::Call { func, args, .. } => {
                let func_reg = self.compile_expression(*func)?;

                // Compile all arguments first
                let mut compiled_arg_regs = Vec::new();
                for arg in args {
                    let arg_reg = self.compile_expression(arg)?;
                    compiled_arg_regs.push(arg_reg);
                }

                // CRITICAL: Move arguments to consecutive registers starting from func_reg + 1
                // The CallFunction handler expects arguments in consecutive registers
                for (i, &arg_reg) in compiled_arg_regs.iter().enumerate() {
                    let target_reg = func_reg + 1 + i as u32;
                    if arg_reg != target_reg {
                        // Only emit LoadLocal if the register is different
                        self.emit(OpCode::LoadLocal, arg_reg, target_reg, 0, self.current_line);
                    }
                }

                let result_reg = self.allocate_register();
                self.emit(OpCode::CallFunction, func_reg, compiled_arg_regs.len() as u32, result_reg, self.current_line);

                Ok(result_reg)
            }
            Expr::Compare { left, ops, comparators } => {
                // For now, we'll just handle the first comparison operation
                // A full implementation would handle chained comparisons
                if ops.len() != 1 || comparators.len() != 1 {
                    return Err(anyhow!("Chained comparisons not yet supported"));
                }
                
                let left_reg = self.compile_expression(*left)?;
                let right_reg = self.compile_expression(comparators[0].clone())?;
                let result_reg = self.allocate_register();
                
                let opcode = match ops[0] {
                    CompareOp::Eq => OpCode::CompareEqualRR,
                    CompareOp::NotEq => OpCode::CompareNotEqualRR,
                    CompareOp::Lt => OpCode::CompareLessRR,
                    CompareOp::LtE => OpCode::CompareLessEqualRR,
                    CompareOp::Gt => OpCode::CompareGreaterRR,
                    CompareOp::GtE => OpCode::CompareGreaterEqualRR,
                    _ => return Err(anyhow!("Unsupported comparison operation: {:?}", ops[0])),
                };
                
                self.emit(opcode, left_reg, right_reg, result_reg, self.current_line);
                Ok(result_reg)
            }
            Expr::List(items) => {
                // Compile each item and store in consecutive registers
                let mut item_regs = Vec::new();
                for item in items {
                    let item_reg = self.compile_expression(item)?;
                    item_regs.push(item_reg);
                }

                let result_reg = self.allocate_register();

                // Use the BuildList opcode to create a list with the items
                // arg1 = number of items, arg2 = first item register, arg3 = result register
                let first_reg = if item_regs.is_empty() { 0 } else { item_regs[0] };
                self.emit(OpCode::BuildList, item_regs.len() as u32, first_reg, result_reg, self.current_line);

                Ok(result_reg)
            }
            Expr::UnaryOp { op, operand } => {
                let operand_reg = self.compile_expression(*operand)?;
                let result_reg = self.allocate_register();
                
                match op {
                    UnaryOp::USub => {
                        // For unary minus, we multiply by -1
                        let neg_one_const = self.code.add_constant(Value::Int(-1));
                        let neg_one_reg = self.allocate_register();
                        self.emit(OpCode::LoadConst, neg_one_const, neg_one_reg, 0, self.current_line);
                        self.emit(OpCode::BinaryMulRR, operand_reg, neg_one_reg, result_reg, self.current_line);
                    }
                    UnaryOp::UAdd => {
                        // For unary plus, we just return the operand
                        self.emit(OpCode::LoadLocal, operand_reg, result_reg, 0, self.current_line);
                    }
                    UnaryOp::Not => {
                        // For logical not, we need to implement this
                        // For now, we'll just load the operand as a placeholder
                        self.emit(OpCode::LoadLocal, operand_reg, result_reg, 0, self.current_line);
                    }
                    _ => {
                        return Err(anyhow!("Unsupported unary operation: {:?}", op));
                    }
                }
                
                Ok(result_reg)
            }
            Expr::Subscript { object, index } => {
                let object_reg = self.compile_expression(*object)?;
                let index_reg = self.compile_expression(*index)?;
                let result_reg = self.allocate_register();
                
                // Emit SubscrLoad instruction to load item from sequence
                self.emit(OpCode::SubscrLoad, object_reg, index_reg, result_reg, self.current_line);
                
                Ok(result_reg)
            }
            Expr::MethodCall { object, method, args, .. } => {
                // Check if object is an identifier (variable) so we can update it after mutating method calls
                let object_var_name = if let Expr::Identifier(ref name) = *object {
                    Some(name.clone())
                } else {
                    None
                };

                let object_reg = self.compile_expression(*object)?;

                // Compile all arguments first
                let mut compiled_arg_regs = Vec::new();
                for arg in args.iter() {
                    let arg_reg = self.compile_expression(arg.clone())?;
                    compiled_arg_regs.push(arg_reg);
                }

                // CRITICAL: Move arguments to consecutive registers starting from object_reg + 1
                // The CallMethod handler expects arguments in consecutive registers
                for (i, &arg_reg) in compiled_arg_regs.iter().enumerate() {
                    let target_reg = object_reg + 1 + i as u32;
                    if arg_reg != target_reg {
                        // Only emit LoadLocal if the register is different
                        self.emit(OpCode::LoadLocal, arg_reg, target_reg, 0, self.current_line);
                    }
                }

                let method_name_idx = self.code.add_name(method.clone());

                // Allocate result register BEFORE calling the method
                let result_reg = self.allocate_register();

                // Emit CallMethod instruction
                // We'll use a new calling convention where the result is stored in a dedicated register
                // For now, we'll store the object_reg and method will update it, then we'll load the result
                // arg1: object register (also used for storing result temporarily)
                // arg2: number of arguments
                // arg3: method name index
                self.emit(OpCode::CallMethod, object_reg, compiled_arg_regs.len() as u32, method_name_idx, self.current_line);

                // CRITICAL FIX: For mutating methods, store the modified object back to the variable
                // This ensures that mutations persist (e.g., list.append modifies the list)
                if let Some(var_name) = object_var_name {
                    let mutating_methods = vec!["append", "extend", "insert", "remove", "pop", "clear", "sort", "reverse"];
                    if mutating_methods.contains(&method.as_str()) {
                        let name_idx = self.code.add_name(var_name);
                        self.emit(OpCode::StoreGlobal, name_idx, object_reg, 0, self.current_line);
                    }
                }

                // Load the result from the object register (CallMethod stores result there)
                // We use LoadLocal to copy it to the result register
                self.emit(OpCode::LoadLocal, object_reg, result_reg, 0, self.current_line);
                Ok(result_reg)
            }
            Expr::Attribute { object, name } => {
                // Attribute access: object.name
                // For now, we compile the object and return a placeholder
                // TODO: Implement proper LoadAttr opcode and runtime support
                let _object_reg = self.compile_expression(*object)?;

                // For now, just return None as a placeholder
                // This allows class methods to compile even if they reference self.x
                let result_reg = self.allocate_register();
                let none_const = self.code.add_constant(Value::None);
                self.emit(OpCode::LoadConst, none_const, result_reg, 0, self.current_line);
                Ok(result_reg)
            }
            _ => Err(anyhow!("Unsupported expression type: {:?}", expr)),
        }
    }
}