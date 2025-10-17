//! SuperCompiler - Register-based bytecode compiler with advanced optimizations

use crate::ast::{Program, Statement, Expr, Literal, BinaryOp, UnaryOp, Param, ParamKind, Type, Pattern, MatchCase, ExceptHandler, CompareOp};
use crate::bytecode::instructions::{OpCode, Instruction};
use crate::bytecode::memory::CodeObject;
use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;

/// SuperCompiler - Register-based bytecode compiler with advanced optimizations
pub struct SuperCompiler {
    pub code: CodeObject,
    next_register: u32,
    current_line: u32,
}

// Helper function to check if a statement contains yield expressions
fn contains_yield(stmt: &Statement) -> bool {
    match stmt {
        Statement::Expression(expr) => contains_yield_in_expr(expr),
        Statement::VariableDef { value: Some(expr), .. } => contains_yield_in_expr(expr),
        Statement::Return(Some(expr)) => contains_yield_in_expr(expr),
        Statement::If { condition, then_branch, elif_branches, else_branch } => {
            contains_yield_in_expr(condition) ||
            then_branch.iter().any(contains_yield) ||
            elif_branches.iter().any(|(_, body)| body.iter().any(contains_yield)) ||
            else_branch.as_ref().map_or(false, |body| body.iter().any(contains_yield))
        },
        Statement::While { condition, body, .. } => {
            contains_yield_in_expr(condition) || body.iter().any(contains_yield)
        },
        Statement::For { iterable, body, .. } => {
            contains_yield_in_expr(iterable) || body.iter().any(contains_yield)
        },
        Statement::FunctionDef { body, .. } => {
            body.iter().any(contains_yield)
        },
        Statement::ClassDef { body, .. } => {
            body.iter().any(contains_yield)
        },
        Statement::Try { body, except_handlers, else_branch, finally } => {
            body.iter().any(contains_yield) ||
            except_handlers.iter().any(|handler| handler.body.iter().any(contains_yield)) ||
            else_branch.as_ref().map_or(false, |body| body.iter().any(contains_yield)) ||
            finally.as_ref().map_or(false, |body| body.iter().any(contains_yield))
        },
        Statement::SubscriptAssignment { object, index, value } => {
            contains_yield_in_expr(object) || contains_yield_in_expr(index) || contains_yield_in_expr(value)
        },
        Statement::AttributeAssignment { object, value, .. } => {
            contains_yield_in_expr(object) || contains_yield_in_expr(value)
        },
        _ => false,
    }
}

// Helper function to check if an expression contains yield expressions
fn contains_yield_in_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Yield(_) | Expr::YieldFrom(_) => true,
        Expr::BinaryOp { left, op: _, right } => {
            contains_yield_in_expr(left) || contains_yield_in_expr(right)
        },
        Expr::UnaryOp { op: _, operand } => {
            contains_yield_in_expr(operand)
        },
        Expr::Call { func, args, kwargs } => {
            contains_yield_in_expr(func) || 
            args.iter().any(contains_yield_in_expr) || 
            kwargs.iter().any(|(_, expr)| contains_yield_in_expr(expr))
        },
        Expr::MethodCall { object, args, kwargs, .. } => {
            contains_yield_in_expr(object) || 
            args.iter().any(contains_yield_in_expr) || 
            kwargs.iter().any(|(_, expr)| contains_yield_in_expr(expr))
        },
        Expr::Attribute { object, .. } => {
            contains_yield_in_expr(object)
        },
        Expr::Subscript { object, index } => {
            contains_yield_in_expr(object) || contains_yield_in_expr(index)
        },
        Expr::Slice { object, start, stop, step } => {
            contains_yield_in_expr(object) || 
            start.as_ref().map_or(false, |expr| contains_yield_in_expr(expr)) ||
            stop.as_ref().map_or(false, |expr| contains_yield_in_expr(expr)) ||
            step.as_ref().map_or(false, |expr| contains_yield_in_expr(expr))
        },
        Expr::List(items) | Expr::Tuple(items) => {
            items.iter().any(contains_yield_in_expr)
        },
        Expr::Dict(pairs) => {
            pairs.iter().any(|(key, value)| contains_yield_in_expr(key) || contains_yield_in_expr(value))
        },
        Expr::Set(items) => {
            items.iter().any(contains_yield_in_expr)
        },
        Expr::ListComp { element, generators } | 
        Expr::SetComp { element, generators } => {
            contains_yield_in_expr(element) || 
            generators.iter().any(|gen| {
                contains_yield_in_expr(&gen.iter) || 
                gen.ifs.iter().any(contains_yield_in_expr)
            })
        },
        Expr::DictComp { key, value, generators } => {
            contains_yield_in_expr(key) || 
            contains_yield_in_expr(value) || 
            generators.iter().any(|gen| {
                contains_yield_in_expr(&gen.iter) || 
                gen.ifs.iter().any(contains_yield_in_expr)
            })
        },
        Expr::GeneratorExp { element, generators } => {
            contains_yield_in_expr(element) || 
            generators.iter().any(|gen| {
                contains_yield_in_expr(&gen.iter) || 
                gen.ifs.iter().any(contains_yield_in_expr)
            })
        },
        Expr::Lambda { body, .. } => {
            contains_yield_in_expr(body)
        },
        Expr::IfExp { condition, then_expr, else_expr } => {
            contains_yield_in_expr(condition) || 
            contains_yield_in_expr(then_expr) || 
            contains_yield_in_expr(else_expr)
        },
        Expr::Compare { left, comparators, .. } => {
            contains_yield_in_expr(left) || comparators.iter().any(contains_yield_in_expr)
        },
        Expr::FormatString { parts } => {
            parts.iter().any(|part| {
                match part {
                    crate::ast::FormatPart::String(_) => false,
                    crate::ast::FormatPart::Expression { expr, .. } => contains_yield_in_expr(expr),
                }
            })
        },
        _ => false,
    }
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
        for param in params.iter() {
            match param.kind {
                ParamKind::VarArgs | ParamKind::VarKwargs => {
                    // For *args and **kwargs, we still add them to varnames
                    // but we need to handle them specially in the frame creation
                    func_compiler.code.add_varname(param.name.clone());
                }
                _ => {
                    func_compiler.code.argcount = func_compiler.code.argcount + 1;
                    func_compiler.code.add_varname(param.name.clone());
                }
            }
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
        func_compiler.code.params = params.to_vec(); // Set the params field
        
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
            Statement::FunctionDef { name, params, body, decorators, .. } => {
                // Create a new compiler for the function
                let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));

                // CRITICAL: Set code.name so is_in_function_scope() works correctly
                func_compiler.code.name = format!("<fn:{}>", name);

                // Add parameters to the function's varnames
                for param in &params {
                    match param.kind {
                        ParamKind::VarArgs | ParamKind::VarKwargs => {
                            // For *args and **kwargs, we still add them to varnames
                            // but we need to handle them specially in the frame creation
                            func_compiler.code.add_varname(param.name.clone());
                        }
                        _ => {
                            func_compiler.code.argcount = func_compiler.code.argcount + 1;
                            func_compiler.code.add_varname(param.name.clone());
                        }
                    }
                }
                
                // Track if this function contains yield statements
                let mut has_yield = false;
                
                // Compile the function body
                for stmt in body.clone() {
                    // Check if statement contains yield expressions
                    if contains_yield(&stmt) {
                        has_yield = true;
                    }
                    func_compiler.compile_statement(stmt)?;
                }
                
                // Add implicit return None at end of function if no return statement
                let none_const = func_compiler.code.add_constant(Value::None);
                let reg = func_compiler.allocate_register(); // Allocate a register
                func_compiler.emit(OpCode::LoadConst, none_const, reg, 0, 0);
                
                // If function contains yield, use ReturnValue instead of implicit return
                if has_yield {
                    func_compiler.emit(OpCode::ReturnValue, reg, 0, 0, 0);
                } else {
                    func_compiler.emit(OpCode::ReturnValue, reg, 0, 0, 0);
                }
                
                // Set the number of registers needed for the function
                func_compiler.code.registers = func_compiler.next_register;
                func_compiler.code.nlocals = func_compiler.code.varnames.len() as u32;
                
                // Get the compiled function code
                let mut func_code = func_compiler.code;
                func_code.params = params.clone(); // Set the params field
                
                // Debug output to see the compiled code
                // eprintln!("DEBUG: Compiled function '{}' with {} instructions", name, func_code.instructions.len());
                
                // Create a closure value for the function with the compiled code
                let closure_value = Value::Closure {
                    name: name.clone(),
                    params: params.clone(),
                    body: vec![], // Body is encoded in the bytecode, not stored as AST
                    captured_scope: HashMap::new(),
                    docstring: None,
                    compiled_code: Some(Box::new(func_code)),
                };
                
                // Add the function to constants and create a LoadConst instruction
                let closure_const_idx = self.code.add_constant(closure_value.clone());
                
                // Load the closure
                let mut func_reg = self.allocate_register();
                self.emit(OpCode::LoadConst, closure_const_idx, func_reg, 0, self.current_line);
                
                // Apply decorators if any
                if !decorators.is_empty() {
                    // Apply decorators in reverse order (as in Python)
                    for decorator_expr in decorators.iter().rev() {
                        // Load the decorator
                        let decorator_reg = self.compile_expression(decorator_expr.clone())?;
                        
                        // Call the decorator with the function as argument
                        let result_reg = self.allocate_register();
                        self.emit(OpCode::CallFunction, decorator_reg, 1, result_reg, self.current_line);
                        
                        // Move the function to the next register for the call
                        let temp_reg = self.allocate_register();
                        self.emit(OpCode::LoadLocal, func_reg, temp_reg, 0, self.current_line);
                        
                        // Update the function register to the decorated result
                        func_reg = result_reg;
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
                    if let Value::Closure { name: ref _name, params: _, body: _, captured_scope: _, docstring: _, compiled_code: ref compiled_code } = stored_value {
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
                
                // 5. Emit SetupLoop instruction to set up the loop block
                // arg1 = end of loop PC (will be updated later)
                // arg2 = continue target PC (start of loop body)
                self.emit(OpCode::SetupLoop, 0, loop_start as u32, 0, self.current_line);
                
                // 6. Emit ForIter instruction with placeholder for end target
                let value_reg = self.allocate_register();
                let for_iter_pos = self.emit(OpCode::ForIter, iter_reg, value_reg, 0, self.current_line); // arg3 will be updated later
                
                // 7. Store the iterated value in the loop variable
                if self.is_in_function_scope() {
                    // In function scope, use fast local access
                    self.emit(OpCode::StoreFast, loop_var_idx, value_reg, 0, self.current_line);
                } else {
                    // In global scope, use StoreGlobal
                    self.emit(OpCode::StoreGlobal, loop_var_idx, value_reg, 0, self.current_line);
                }
                
                // 8. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 9. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, self.current_line);
                
                // 10. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                // Update the ForIter instruction with the correct target
                self.code.instructions[for_iter_pos].arg3 = loop_end_pos as u32;
                // Update the SetupLoop instruction with the correct end target
                self.code.instructions[loop_start].arg1 = loop_end_pos as u32;
                Ok(())
            }
            Statement::While { condition, body, .. } => {
                // Compile while loop: while condition: body
                
                // 1. Create jump targets
                let loop_start = self.code.instructions.len(); // Start of condition check
                
                // 2. Emit SetupLoop instruction to set up the loop block
                // arg1 = end of loop PC (will be updated later)
                // arg2 = continue target PC (start of loop body)
                self.emit(OpCode::SetupLoop, 0, (loop_start + 1) as u32, 0, self.current_line);
                
                // 3. Compile the condition
                let cond_reg = self.compile_expression(condition)?;
                
                // 4. Emit conditional jump to end of loop if condition is false
                let loop_end_pos_ref = self.emit(OpCode::JumpIfFalse, cond_reg, 0, 0, self.current_line); // arg2 will be updated later
                
                // 5. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 6. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, self.current_line);
                
                // 7. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                self.code.instructions[loop_end_pos_ref].arg2 = loop_end_pos as u32;
                // Update the SetupLoop instruction with the correct end target
                self.code.instructions[loop_start].arg1 = loop_end_pos as u32;
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
                let metaclass_value = if let Some(mc_expr) = metaclass {
                    if let Expr::Identifier(mc_name) = mc_expr {
                        Some(Box::new(Value::Str(mc_name.clone())))
                    } else {
                        None
                    }
                } else {
                    Some(Box::new(Value::Str("type".to_string()))) // Default metaclass is 'type'
                };

                // Create the class using the new Class variant
                let class_obj = Value::Class {
                    name: name.clone(),
                    bases: base_names.clone(),
                    methods: class_methods,
                    metaclass: metaclass_value,
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
            Statement::Break => {
                // Emit BreakLoop instruction
                self.emit(OpCode::BreakLoop, 0, 0, 0, self.current_line);
                Ok(())
            }
            Statement::Continue => {
                // Emit ContinueLoop instruction
                self.emit(OpCode::ContinueLoop, 0, 0, 0, self.current_line);
                Ok(())
            }
            Statement::Raise(expr) => {
                // Emit Raise instruction
                let exception_reg = if let Some(expr) = expr {
                    self.compile_expression(expr)?
                } else {
                    // No exception specified, use None
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    reg
                };
                self.emit(OpCode::Raise, exception_reg, 0, 0, self.current_line);
                Ok(())
            }
            Statement::Assert { condition, message } => {
                // Compile condition
                let condition_reg = self.compile_expression(condition)?;
                
                // Compile message if provided
                let message_reg = if let Some(msg) = message {
                    self.compile_expression(msg)?
                } else {
                    // No message specified, use None
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    reg
                };
                
                // Emit Assert instruction
                self.emit(OpCode::Assert, condition_reg, message_reg, 0, self.current_line);
                Ok(())
            }
            Statement::Try { body, except_handlers, else_branch, finally } => {
                // Compile try/except/finally statement
                
                // First, calculate where the exception handlers will start
                // This is after the try block and the jump to skip handlers
                let try_start = self.code.instructions.len();
                
                // Temporarily compile the try body to know its size
                let temp_code_start = self.code.instructions.len();
                for stmt in &body {
                    self.compile_statement(stmt.clone())?;
                }
                let try_block_size = self.code.instructions.len() - temp_code_start;
                
                // Remove the temporarily compiled try block
                self.code.instructions.truncate(temp_code_start);
                
                // Calculate where the handlers will start
                // It's after: try block + jump to end + SetupExcept instructions
                let handlers_start = try_start + try_block_size + 1 + except_handlers.len();
                
                // Set up exception handlers for each except clause
                let mut except_handler_positions = Vec::new();
                for handler in except_handlers.iter() {
                    // Create a jump target for this exception handler
                    let handler_start = handlers_start + except_handler_positions.len();
                    except_handler_positions.push(handler_start);
                }
                
                // Emit SetupExcept instructions to set up the exception handlers
                for &handler_start in &except_handler_positions {
                    // Emit SetupExcept instruction to set up the exception handler
                    // arg1 = handler PC
                    // arg2 = stack level (for now, we'll use 0)
                    self.emit(OpCode::SetupExcept, handler_start as u32, 0, 0, self.current_line);
                }
                
                // Now compile the try body for real
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // Jump to end of try/except/finally block (skip except handlers)
                let end_jump_pos = self.emit(OpCode::Jump, 0, 0, 0, self.current_line); // Will be fixed later
                
                // Compile exception handlers
                let mut except_end_jump_positions = Vec::new();
                for (i, handler) in except_handlers.iter().enumerate() {
                    // If handler has a specific exception type, we need to check it
                    if let Some(ref _exception_type) = handler.exception_type {
                        // For now, we'll just compile the handler body without type checking
                        // In a full implementation, we would check the exception type
                    }
                    
                    // If handler has a variable to bind the exception to, we need to handle that
                    if let Some(ref var_name) = handler.name {
                        // Emit StoreException instruction to bind the exception value to the variable
                        let var_idx = self.code.add_varname(var_name.clone());
                        if self.is_in_function_scope() {
                            // Store in fast local variable
                            self.emit(OpCode::StoreException, var_idx, 0, 0, self.current_line);
                        } else {
                            // Store in global namespace
                            self.emit(OpCode::StoreException, var_idx, 1, 0, self.current_line);
                        }
                    }
                    
                    // Compile the handler body
                    for stmt in &handler.body {
                        self.compile_statement(stmt.clone())?;
                    }
                    
                    // Jump to end of try/except/finally block after handling exception
                    let except_end_jump_pos = self.emit(OpCode::Jump, 0, 0, 0, self.current_line);
                    except_end_jump_positions.push(except_end_jump_pos);
                }
                
                // Fix the jump from try body to after except handlers
                let try_end_pos = self.code.instructions.len();
                self.code.instructions[end_jump_pos].arg1 = try_end_pos as u32;
                
                // Compile else branch if it exists (executed only if no exception occurred)
                if let Some(else_body) = else_branch {
                    for stmt in else_body {
                        self.compile_statement(stmt)?;
                    }
                }
                
                // If we have finally block, set it up
                if let Some(finally_body) = finally {
                    // Emit SetupFinally instruction
                    let finally_start = self.code.instructions.len();
                    self.emit(OpCode::SetupFinally, finally_start as u32, 0, 0, self.current_line);
                    
                    // Compile finally body
                    for stmt in finally_body {
                        self.compile_statement(stmt)?;
                    }
                    
                    // Emit EndFinally instruction
                    self.emit(OpCode::EndFinally, 0, 0, 0, self.current_line);
                }
                
                // Fix jumps from except handlers to end
                let final_end_pos = self.code.instructions.len();
                for jump_pos in except_end_jump_positions {
                    self.code.instructions[jump_pos].arg1 = final_end_pos as u32;
                }
                
                Ok(())
            }
            Statement::Match { value, cases } => {
                // Compile match statement: match value: cases
                
                // 1. Compile the value to match against
                let value_reg = self.compile_expression(value)?;
                
                // 2. Emit Match instruction to start pattern matching
                self.emit(OpCode::Match, value_reg, 0, 0, self.current_line);
                
                // 3. Compile each case
                let mut case_end_jump_positions = Vec::new();
                
                for case in cases {
                    // For each case, we need to:
                    // 1. Compile the pattern
                    // 2. Emit pattern matching opcodes
                    // 3. Compile the guard if present
                    // 4. Compile the body
                    // 5. Jump to end of match statement
                    
                    // Emit pattern matching opcodes based on pattern type
                    self.compile_pattern(case.pattern, value_reg)?;
                    
                    // Compile guard if present
                    if let Some(guard) = case.guard {
                        let guard_reg = self.compile_expression(guard)?;
                        // Emit conditional jump if guard fails
                        let guard_fail_jump = self.emit(OpCode::JumpIfFalse, guard_reg, 0, 0, self.current_line);
                        case_end_jump_positions.push(guard_fail_jump);
                    }
                    
                    // Compile the case body
                    for stmt in case.body {
                        self.compile_statement(stmt)?;
                    }
                    
                    // Jump to end of match statement after executing case body
                    let case_end_jump = self.emit(OpCode::Jump, 0, 0, 0, self.current_line);
                    case_end_jump_positions.push(case_end_jump);
                }
                
                // Fix all the jump placeholders
                let match_end_pos = self.code.instructions.len();
                for jump_pos in case_end_jump_positions {
                    self.code.instructions[jump_pos].arg2 = match_end_pos as u32;
                }
                
                Ok(())
            }
            Statement::AttributeAssignment { object, name, value } => {
                // Compile attribute assignment: object.name = value
                let object_reg = self.compile_expression(object)?;
                let value_reg = self.compile_expression(value)?;
                let name_idx = self.code.add_name(name);
                
                // Emit StoreAttr instruction to store attribute to object
                self.emit(OpCode::StoreAttr, object_reg, name_idx, value_reg, self.current_line);
                Ok(())
            }
            _ => {
                // For unimplemented statements, we'll just return Ok for now
                // In a complete implementation, we would handle all statement types
                Ok(())
            }
        }
    }
    
    /// Compile a pattern for pattern matching
    fn compile_pattern(&mut self, pattern: Pattern, value_reg: u32) -> Result<()> {
        match pattern {
            Pattern::Literal(expr) => {
                // Compile literal pattern
                let pattern_reg = self.compile_expression(expr)?;
                // Emit Match instruction to compare value with pattern
                self.emit(OpCode::Match, value_reg, pattern_reg, 0, self.current_line);
                Ok(())
            }
            Pattern::Variable(_name) => {
                // Variable pattern - always matches, binds value to variable
                // In a full implementation, we would store the matched value in the variable
                // For now, we'll just emit a Match instruction
                self.emit(OpCode::Match, value_reg, 0, 0, self.current_line);
                Ok(())
            }
            Pattern::Wildcard => {
                // Wildcard pattern - always matches
                self.emit(OpCode::Match, value_reg, 0, 0, self.current_line);
                Ok(())
            }
            Pattern::Tuple(patterns) => {
                // Tuple pattern - match against tuple
                self.emit(OpCode::MatchSequence, value_reg, patterns.len() as u32, 0, self.current_line);
                
                // Compile sub-patterns
                for (_i, pattern) in patterns.into_iter().enumerate() {
                    // For each sub-pattern, we would need to extract the i-th element
                    // and match it against the sub-pattern
                    // For now, we'll just emit the pattern matching opcodes
                    self.compile_pattern(pattern, value_reg)?;
                }
                Ok(())
            }
            Pattern::List(patterns) => {
                // List pattern - match against list
                self.emit(OpCode::MatchSequence, value_reg, patterns.len() as u32, 0, self.current_line);
                
                // Compile sub-patterns
                for pattern in patterns {
                    self.compile_pattern(pattern, value_reg)?;
                }
                Ok(())
            }
            Pattern::Dict(pairs) => {
                // Dict pattern - match against dictionary
                self.emit(OpCode::MatchMapping, value_reg, pairs.len() as u32, 0, self.current_line);
                
                // Compile key-value patterns
                for (key_pattern, value_pattern) in pairs {
                    // Compile key pattern
                    self.compile_pattern(key_pattern, value_reg)?;
                    // Compile value pattern
                    self.compile_pattern(value_pattern, value_reg)?;
                }
                Ok(())
            }
            Pattern::Class { name: _name, patterns } => {
                // Class pattern - match against class instance
                self.emit(OpCode::MatchClass, value_reg, patterns.len() as u32, 0, self.current_line);
                
                // Compile sub-patterns
                for pattern in patterns {
                    self.compile_pattern(pattern, value_reg)?;
                }
                Ok(())
            }
            Pattern::Or(patterns) => {
                // Or pattern - match against any of the patterns
                self.emit(OpCode::MatchOr, value_reg, patterns.len() as u32, 0, self.current_line);
                
                // Compile sub-patterns
                for pattern in patterns {
                    self.compile_pattern(pattern, value_reg)?;
                }
                Ok(())
            }
            Pattern::As { pattern, name: _name } => {
                // As pattern - match against pattern and bind to name
                self.compile_pattern(*pattern, value_reg)?;
                // In a full implementation, we would bind the matched value to the name
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
            Expr::Call { func, args, kwargs } => {
                let func_reg = self.compile_expression(*func)?;

                // Compile all positional arguments first
                let mut compiled_arg_regs = Vec::new();
                let mut starred_args = Vec::new();
                for (i, arg) in args.into_iter().enumerate() {
                    match arg {
                        Expr::Starred(expr) => {
                            // Mark this as a starred argument
                            let arg_reg = self.compile_expression(*expr)?;
                            starred_args.push((i, arg_reg));
                            compiled_arg_regs.push(arg_reg);
                        }
                        _ => {
                            let arg_reg = self.compile_expression(arg)?;
                            compiled_arg_regs.push(arg_reg);
                        }
                    }
                }

                // If we have keyword arguments, we need to pass them as a special argument
                // But only for user-defined functions that have **kwargs parameters
                if !kwargs.is_empty() {
                    // Debug info removed
                    // For now, we'll create the kwargs dictionary for all calls with kwargs
                    // In a more sophisticated implementation, we would check if the function
                    // actually accepts **kwargs parameters
                    // Create a dictionary with the keyword arguments
                    let mut dict_pairs = Vec::new();
                    for (name, value) in kwargs {
                        // Debug info removed
                        // Compile the key (name) as a string literal
                        let key_expr = Expr::Literal(Literal::String(name));
                        let key_reg = self.compile_expression(key_expr)?;
                        let value_reg = self.compile_expression(value)?;
                        dict_pairs.push((key_reg, value_reg));
                    }
                    
                    // Build the dictionary
                    if !dict_pairs.is_empty() {
                        // We need to interleave the keys and values
                        let mut dict_items = Vec::new();
                        for (key_reg, value_reg) in dict_pairs {
                            dict_items.push(key_reg);
                            dict_items.push(value_reg);
                        }
                        
                        // Use BuildDict to create the dictionary
                        let first_reg = dict_items[0];
                        let dict_reg = self.allocate_register();
                        self.emit(OpCode::BuildDict, (dict_items.len() / 2) as u32, first_reg, dict_reg, self.current_line);
                        
                        // Wrap the dictionary in a KwargsMarker
                        let kwargs_marker_reg = self.allocate_register();
                        self.emit(OpCode::WrapKwargs, dict_reg, kwargs_marker_reg, 0, self.current_line);
                        
                        // Add the KwargsMarker as a special argument
                        compiled_arg_regs.push(kwargs_marker_reg);
                    }
                }

                // CRITICAL: Move arguments to consecutive registers starting from func_reg + 1
                // The CallFunction handler expects arguments in consecutive registers
                for (i, &arg_reg) in compiled_arg_regs.iter().enumerate() {
                    let target_reg = func_reg + 1 + i as u32;
                    if arg_reg != target_reg {
                        // Only emit LoadLocal if the register is different
                        // Make sure both source and target registers are within bounds
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
            Expr::Dict(pairs) => {
                // Compile each key-value pair and store in consecutive registers
                // Keys and values are interleaved: key1, value1, key2, value2, ...
                let mut pair_regs = Vec::new();
                for (key, value) in pairs {
                    let key_reg = self.compile_expression(key)?;
                    let value_reg = self.compile_expression(value)?;
                    pair_regs.push(key_reg);
                    pair_regs.push(value_reg);
                }

                let result_reg = self.allocate_register();

                // Use the BuildDict opcode to create a dict with the key-value pairs
                // arg1 = number of pairs, arg2 = first key register, arg3 = result register
                let first_reg = if pair_regs.is_empty() { 0 } else { pair_regs[0] };
                self.emit(OpCode::BuildDict, (pair_regs.len() / 2) as u32, first_reg, result_reg, self.current_line);

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
                let object_reg = self.compile_expression(*object)?;
                let name_idx = self.code.add_name(name);
                let result_reg = self.allocate_register();
                
                // Emit LoadAttr instruction to load attribute from object
                self.emit(OpCode::LoadAttr, object_reg, name_idx, result_reg, self.current_line);
                Ok(result_reg)
            }
            Expr::FormatString { parts } => {
                // Handle f-string formatting by compiling each part and concatenating
                let mut part_regs = Vec::new();
                
                for part in parts {
                    match part {
                        crate::ast::FormatPart::String(s) => {
                            // Compile string literal part
                            let const_idx = self.code.add_constant(Value::Str(s));
                            let reg = self.allocate_register();
                            self.emit(OpCode::LoadConst, const_idx, reg, 0, self.current_line);
                            part_regs.push(reg);
                        }
                        crate::ast::FormatPart::Expression { expr, format_spec: _, conversion: _ } => {
                            // Compile expression part and convert to string
                            let expr_reg = self.compile_expression(expr)?;
                            
                            // Call str() builtin on the expression result
                            // For now, we'll use a simplified approach
                            // In a full implementation, we would call the str() builtin function
                            part_regs.push(expr_reg);
                        }
                    }
                }
                
                // Concatenate all parts
                if part_regs.is_empty() {
                    // Empty f-string
                    let result_reg = self.allocate_register();
                    let const_idx = self.code.add_constant(Value::Str(String::new()));
                    self.emit(OpCode::LoadConst, const_idx, result_reg, 0, self.current_line);
                    Ok(result_reg)
                } else if part_regs.len() == 1 {
                    // Single part, just return it
                    Ok(part_regs[0])
                } else {
                    // Multiple parts, concatenate them
                    let mut current_reg = part_regs[0];
                    
                    for &next_reg in &part_regs[1..] {
                        let result_reg = self.allocate_register();
                        self.emit(OpCode::BinaryAddRR, current_reg, next_reg, result_reg, self.current_line);
                        current_reg = result_reg;
                    }
                    
                    Ok(current_reg)
                }
            }
            Expr::Starred(expr) => {
                // For starred expressions in function calls, we compile the inner expression
                // The VM will need to handle unpacking these at call time
                self.compile_expression(*expr)
            }
            Expr::Yield(value) => {
                // Handle yield expression
                let value_reg = if let Some(expr) = value {
                    // Compile the yielded value
                    self.compile_expression(*expr)?
                } else {
                    // No value specified, yield None
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    reg
                };
                
                // Emit YieldValue instruction
                let result_reg = self.allocate_register();
                self.emit(OpCode::YieldValue, value_reg, 0, 0, self.current_line);
                Ok(result_reg)
            }
            Expr::YieldFrom(expr) => {
                // Handle yield from expression
                let iterable_reg = self.compile_expression(*expr)?;
                
                // Emit YieldFrom instruction
                let result_reg = self.allocate_register();
                self.emit(OpCode::YieldFrom, iterable_reg, 0, 0, self.current_line);
                Ok(result_reg)
            }
            Expr::Await(expr) => {
                // Handle await expression
                let value_reg = self.compile_expression(*expr)?;
                
                // Emit Await instruction
                let result_reg = self.allocate_register();
                self.emit(OpCode::Await, value_reg, result_reg, 0, self.current_line);
                Ok(result_reg)
            }
            _ => Err(anyhow!("Unsupported expression type: {:?}", expr)),
        }
    }
}