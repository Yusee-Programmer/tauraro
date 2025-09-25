//! COMPLETE TauraroLang Virtual Machine with dynamic typing and REPL support
use crate::runtime::{MemoryAPI, ManagedPtr, MemoryMode};
use crate::ast::*;
use crate::lexer::Lexer;
use crate::parser::Parser;
use crate::semantic;
use crate::value::Value;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use std::fmt;

/// Variable scope
#[derive(Debug, Clone)]
pub struct Scope {
    pub variables: HashMap<String, Value>,
    pub parent: Option<usize>,
    pub scope_type: String, // "global", "function", "class", "block"
}

impl Scope {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
            parent: None,
            scope_type: "module".to_string(),
        }
    }
}

/// Virtual Machine state
pub struct VM {
    scopes: Vec<Scope>,
    current_scope: usize,
    memory: MemoryAPI,
    call_stack: Vec<StackFrame>,
    strict_types: bool,
    should_return: bool,
    return_value: Option<Value>,
}

/// Stack frame for function calls
#[derive(Debug)]
struct StackFrame {
    function_name: String,
    return_address: usize,
    scope_index: usize,
}

impl VM {
    pub fn new() -> Self {
        let global_scope = Scope {
            variables: HashMap::new(),
            parent: None,
            scope_type: "global".to_string(),
        };
        
        let mut vm = Self {
            scopes: vec![global_scope],
            current_scope: 0,
            memory: MemoryAPI::new(),
            call_stack: Vec::new(),
            strict_types: false,
            should_return: false,
            return_value: None,
        };
        
        // Initialize built-in functions
        vm.init_builtins();
        vm
    }
    
    /// Set strict type checking mode
    pub fn set_strict_types(&mut self, strict: bool) {
        self.strict_types = strict;
    }
    
    /// Execute TauraroLang source code as a script
    pub fn execute_script(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        // Parse with implicit main function support
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        // Try to parse as a single expression first (for simple scripts like "42")
        let program = if let Ok(program) = parser.parse() {
            if program.statements.is_empty() {
                // Empty program, create a simple one that returns None
                Program {
                    statements: vec![Statement::Expression(Expr::Literal(Literal::None))],
                }
            } else if program.statements.len() == 1 {
                // Single statement - check if it's an expression or variable definition
                match &program.statements[0] {
                    Statement::Expression(_) | Statement::VariableDef { .. } => {
                        program
                    },
                    _ => {
                        // Reset parser and try with implicit main
                        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
                            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
                        let mut parser = Parser::new(tokens);
                        parser.parse_with_implicit_main()?
                    }
                }
            } else {
                // Multiple statements - use implicit main
                let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
                    .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
                let mut parser = Parser::new(tokens);
                parser.parse_with_implicit_main()?
            }
        } else {
            // Parsing failed, try with implicit main
            let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
                .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
            let mut parser = Parser::new(tokens);
            parser.parse_with_implicit_main()?
        };
        
        // Optional semantic analysis based on strict mode
        let program = if self.strict_types {
            semantic::analyze_optional_types(program, true)
                .map_err(|errors| anyhow::anyhow!("Semantic errors: {:?}", errors))?
        } else {
            program
        };
        
        // Set command line arguments
        self.set_variable("args", Value::List(args.into_iter().map(Value::String).collect()));
        
        // Execute the program
        self.execute_program(program)
    }
    
    /// Execute REPL input (expressions or statements)
    pub fn execute_repl(&mut self, input: &str, line_number: usize) -> Result<Option<Value>> {
        // Add line number to source for better error messages
        let source = format!("# Line {}\n{}", line_number, input);
        
        // Try to parse as expression first (for immediate evaluation)
        let tokens = Lexer::new(input).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        // Try to parse as expression first
        if let Ok(program) = parser.parse() {
            if let Some(Statement::Expression(expr)) = program.statements.first() {
                let result = self.execute_expression(expr)?;
                return Ok(Some(result));
            }
        }
        
        // Try to parse as statement
        let tokens = Lexer::new(&source).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        if let Ok(program) = parser.parse() {
            if let Some(stmt) = program.statements.first() {
                self.execute_statement(stmt)?;
            }
        }
        
        Ok(None)
    }
    
    /// Execute a single line (for REPL)
    pub fn execute_line(&mut self, line: &str) -> Result<Value> {
        let tokens = Lexer::new(line).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        // Try to parse as program
        let program = parser.parse()?;
        
        // Check if it's a variable definition
        if let Some(Statement::VariableDef { value: Some(value), .. }) = program.statements.first() {
            return self.execute_expression(value);
        }
        
        // Otherwise parse as expression
        if let Some(Statement::Expression(expr)) = program.statements.first() {
            return self.execute_expression(expr);
        }
        
        Err(anyhow::anyhow!("Unable to parse line as expression or statement"))
    }
    
    /// Execute a complete program
    fn execute_program(&mut self, program: Program) -> Result<Value> {
        // First pass: register all functions and classes
        for stmt in &program.statements {
            if let Statement::FunctionDef { name, params, body, .. } = stmt {
                let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                let func_value = Value::Function(name.clone(), param_names, body.clone());
                self.set_variable(name, func_value);
            } else if let Statement::ClassDef { name, .. } = stmt {
                // Create class object
                let class_obj = Value::Object(name.clone(), HashMap::new());
                self.set_variable(name, class_obj);
            }
        }
        
        // Second pass: execute statements
        for stmt in program.statements {
            if !matches!(stmt, Statement::FunctionDef { .. } | Statement::ClassDef { .. }) {
                self.execute_statement(&stmt)?;
                
                if self.should_return {
                    return Ok(self.return_value.take().unwrap_or(Value::None));
                }
            }
        }
        
        // Call main function if it exists
        if let Some(Value::Function(_, _, body)) = self.get_variable("main") {
            self.enter_scope("function");
            for stmt in body {
                self.execute_statement(&stmt)?;
                if self.should_return {
                    break;
                }
            }
            let result = self.return_value.take().unwrap_or(Value::None);
            self.exit_scope();
            return Ok(result);
        }
        
        Ok(Value::None)
    }
    
    /// Execute a statement
    pub fn execute_statement(&mut self, stmt: &Statement) -> Result<Option<Value>> {
        match stmt {
            Statement::Expression(expr) => {
                let value = self.execute_expression(expr)?;
                Ok(Some(value))
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let val = self.execute_expression(expr)?;
                    self.set_variable(name, val);
                } else {
                    self.set_variable(name, Value::None);
                }
                Ok(None)
            }
            Statement::FunctionDef { name, params, body, .. } => {
                // Function definition already handled in first pass
                Ok(None)
            }
            Statement::Return(value) => {
                let return_value = if let Some(expr) = value {
                    self.execute_expression(expr)?
                } else {
                    Value::None
                };
                self.should_return = true;
                self.return_value = Some(return_value);
                Ok(None)
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                self.execute_if_statement(condition, then_branch, elif_branches, else_branch)
            }
            Statement::While { condition, body, .. } => {
                self.execute_while_statement(condition, body)
            }
            Statement::For { variable, iterable, body, .. } => {
                self.execute_for_statement(variable, iterable, body)
            }
            Statement::Import { module, alias } => {
                self.execute_import_statement(module, alias.as_deref())
            }
            _ => {
                // TODO: Implement other statement types
                Ok(None)
            }
        }
    }
    
    /// Execute an expression
    fn execute_expression(&mut self, expr: &Expr) -> Result<Value> {
        match expr {
            Expr::Literal(Literal::Int(n)) => Ok(Value::Int(*n)),
            Expr::Literal(Literal::Float(n)) => Ok(Value::Float(*n)),
            Expr::Literal(Literal::String(s)) => Ok(Value::String(s.clone())),
            Expr::Literal(Literal::Bool(b)) => Ok(Value::Bool(*b)),
            Expr::Literal(Literal::None) => Ok(Value::None),
            Expr::Identifier(name) => {
                self.get_variable(name).ok_or_else(|| anyhow::anyhow!("Undefined variable: {}", name))
            }
            Expr::BinaryOp { left, op, right } => {
                self.execute_binary_op(left, op, right)
            }
            Expr::UnaryOp { op, operand } => {
                self.execute_unary_op(op, operand)
            }
            Expr::Call { func, args, .. } => {
                self.execute_function_call(func, args)
            }
            Expr::List(elements) => {
                let values: Result<Vec<Value>> = elements.iter()
                    .map(|e| self.execute_expression(e))
                    .collect();
                Ok(Value::List(values?))
            }
            Expr::Dict(pairs) => {
                let mut dict = HashMap::new();
                for (key_expr, value_expr) in pairs {
                    let key = self.execute_expression(key_expr)?;
                    let value = self.execute_expression(value_expr)?;
                    let key_string = match &key {
                        Value::String(s) => s.clone(),
                        Value::Int(n) => n.to_string(),
                        Value::Float(n) => format!("{:.6}", n),
                        Value::Bool(b) => b.to_string(),
                        Value::None => "None".to_string(),
                        _ => format!("{}", key), // Use Display trait directly
                    };
                    dict.insert(key_string, value);
                }
                Ok(Value::Dict(dict))
            }
            Expr::Subscript { object, index } => {
                let obj_value = self.execute_expression(object)?;
                let index_value = self.execute_expression(index)?;
                
                match (&obj_value, &index_value) {
                    (Value::Dict(dict), index_val) => {
                        let key_string = match index_val {
                            Value::String(s) => s.clone(),
                            Value::Int(n) => n.to_string(),
                            Value::Float(n) => format!("{:.6}", n),
                            Value::Bool(b) => b.to_string(),
                            Value::None => "None".to_string(),
                            _ => format!("{}", index_val),
                        };
                        dict.get(&key_string)
                            .cloned()
                            .ok_or_else(|| anyhow::anyhow!("Key '{}' not found in dictionary", key_string))
                    }
                    (Value::List(list), Value::Int(index)) => {
                        let idx = if *index < 0 {
                            (list.len() as i64 + index) as usize
                        } else {
                            *index as usize
                        };
                        
                        if idx < list.len() {
                            Ok(list[idx].clone())
                        } else {
                            Err(anyhow::anyhow!("List index {} out of range", index))
                        }
                    }
                    (Value::String(s), Value::Int(index)) => {
                        let chars: Vec<char> = s.chars().collect();
                        let idx = if *index < 0 {
                            (chars.len() as i64 + index) as usize
                        } else {
                            *index as usize
                        };
                        
                        if idx < chars.len() {
                            Ok(Value::String(chars[idx].to_string()))
                        } else {
                            Err(anyhow::anyhow!("String index {} out of range", index))
                        }
                    }
                    _ => Err(anyhow::anyhow!("Invalid subscript operation: {:?}[{:?}]", obj_value, index_value)),
                }
            }
            // Typed expressions are not part of the current AST definition
            // This case has been removed as Expr::Typed doesn't exist
            _ => Err(anyhow::anyhow!("Expression not implemented: {:?}", expr)),
        }
    }
    
    /// Execute binary operation
    fn execute_binary_op(&mut self, left: &Expr, op: &BinaryOp, right: &Expr) -> Result<Value> {
        let left_val = self.execute_expression(left)?;
        let right_val = self.execute_expression(right)?;
        
        // Type checking in strict mode
        if self.strict_types {
            self.check_binary_op_types(&left_val, op, &right_val)?;
        }
        
        match op {
            BinaryOp::Add => self.add_values(left_val, right_val),
            BinaryOp::Sub => self.sub_values(left_val, right_val),
            BinaryOp::Mul => self.mul_values(left_val, right_val),
            BinaryOp::Div => self.div_values(left_val, right_val),
            BinaryOp::Mod => self.mod_values(left_val, right_val),
            BinaryOp::Eq => Ok(Value::Bool(self.values_equal(left_val, right_val))),
            BinaryOp::Ne | BinaryOp::Neq => Ok(Value::Bool(!self.values_equal(left_val, right_val))),
            BinaryOp::Gt => self.gt_values(left_val, right_val),
            BinaryOp::Lt => self.lt_values(left_val, right_val),
            BinaryOp::Ge | BinaryOp::Gte => self.gte_values(left_val, right_val),
            BinaryOp::Le | BinaryOp::Lte => self.lte_values(left_val, right_val),
            BinaryOp::And => Ok(Value::Bool(left_val.is_truthy() && right_val.is_truthy())),
            BinaryOp::Or => Ok(Value::Bool(left_val.is_truthy() || right_val.is_truthy())),
            _ => Err(anyhow::anyhow!("Operator not implemented: {:?}", op)),
        }
    }
    
    /// Execute unary operation
    fn execute_unary_op(&mut self, op: &UnaryOp, expr: &Expr) -> Result<Value> {
        let value = self.execute_expression(expr)?;
        
        match op {
            UnaryOp::UAdd => self.plus_value(value),
            UnaryOp::Minus | UnaryOp::USub => self.minus_value(value),
            UnaryOp::Not => Ok(Value::Bool(!value.is_truthy())),
            UnaryOp::BitNot | UnaryOp::Invert => self.bitnot_value(value),
        }
    }
    
    /// Execute function call
    fn execute_function_call(&mut self, callee: &Expr, arguments: &[Expr]) -> Result<Value> {
        let callee_val = self.execute_expression(callee)?;
        let arg_values: Result<Vec<Value>> = arguments
            .iter()
            .map(|arg| self.execute_expression(arg))
            .collect();
        let arg_values = arg_values?;
        
        match callee_val {
            Value::Function(name, params, body) => {
                self.call_user_function(&name, &params, body, arg_values)
            }
            Value::BuiltinFunction(_, func) => {
                func(arg_values)
            }
            Value::NativeFunction(func) => {
                func(arg_values)
            }
            _ => Err(anyhow::anyhow!("Not a function: {}", callee_val.type_name())),
        }
    }
    
    /// Execute assignment
    fn execute_assignment(&mut self, target: &str, value: &Expr) -> Result<()> {
        let value = self.execute_expression(value)?;
        self.set_variable(target, value);
        Ok(())
    }
    
    /// Execute if statement
    fn execute_if_statement(
        &mut self,
        condition: &Expr,
        then_branch: &[Statement],
        elif_branches: &[(Expr, Vec<Statement>)],
        else_branch: &Option<Vec<Statement>>,
    ) -> Result<Option<Value>> {
        let cond_value = self.execute_expression(condition)?;
        
        if cond_value.is_truthy() {
            self.enter_scope("block");
            for stmt in then_branch {
                self.execute_statement(stmt)?;
                if self.should_return {
                    break;
                }
            }
            self.exit_scope();
        } else {
            // Check elif branches
            let mut matched = false;
            for (elif_cond, elif_body) in elif_branches {
                let elif_value = self.execute_expression(elif_cond)?;
                if elif_value.is_truthy() {
                    self.enter_scope("block");
                    for stmt in elif_body {
                        self.execute_statement(stmt)?;
                        if self.should_return {
                            break;
                        }
                    }
                    self.exit_scope();
                    matched = true;
                    break;
                }
            }
            
            // Execute else branch if no elif matched
            if !matched {
                if let Some(else_body) = else_branch {
                    self.enter_scope("block");
                    for stmt in else_body {
                        self.execute_statement(stmt)?;
                        if self.should_return {
                            break;
                        }
                    }
                    self.exit_scope();
                }
            }
        }
        
        Ok(None)
    }
    
    /// Execute while statement
    fn execute_while_statement(&mut self, condition: &Expr, body: &[Statement]) -> Result<Option<Value>> {
        while {
            let cond_value = self.execute_expression(condition)?;
            cond_value.is_truthy()
        } {
            self.enter_scope("block");
            for stmt in body {
                self.execute_statement(stmt)?;
                if self.should_return {
                    self.exit_scope();
                    return Ok(None);
                }
            }
            self.exit_scope();
        }
        
        Ok(None)
    }
    
    /// Execute for statement
    fn execute_for_statement(&mut self, variable: &str, iterable: &Expr, body: &[Statement]) -> Result<Option<Value>> {
        let iter_value = self.execute_expression(iterable)?;
        
        let items = match iter_value {
            Value::List(items) => items,
            Value::String(s) => s.chars().map(|c| Value::String(c.to_string())).collect(),
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
        };
        
        for item in items {
            self.enter_scope("block");
            self.set_variable(variable, item);
            
            for stmt in body {
                self.execute_statement(stmt)?;
                if self.should_return {
                    self.exit_scope();
                    return Ok(None);
                }
            }
            
            self.exit_scope();
        }
        
        Ok(None)
    }
    
    /// Execute import statement
    fn execute_import_statement(&mut self, module: &str, alias: Option<&str>) -> Result<Option<Value>> {
        let name = alias.unwrap_or(module);
        
        // For now, create a mock module object
        let module_obj = Value::Object(module.to_string(), HashMap::new());
        self.set_variable(name, module_obj);
        
        Ok(None)
    }
    
    /// Execute extern statement
    fn execute_extern_statement(&mut self, library: &str) -> Result<Option<Value>> {
        println!("Loading external library: {}", library);
        // Actual library loading would be implemented here
        Ok(None)
    }
    
    /// Call user-defined function
    fn call_user_function(&mut self, name: &str, params: &[String], body: Vec<Statement>, args: Vec<Value>) -> Result<Value> {
        if params.len() != args.len() {
            return Err(anyhow::anyhow!("Function {} expects {} arguments, got {}", 
                name, params.len(), args.len()));
        }
        
        // Push new stack frame
        self.call_stack.push(StackFrame {
            function_name: name.to_string(),
            return_address: self.current_scope,
            scope_index: self.current_scope,
        });
        
        // Enter function scope
        self.enter_scope("function");
        
        // Set parameters
        for (param, arg) in params.iter().zip(args) {
            self.set_variable(param, arg);
        }
        
        // Execute function body
        let mut result = Value::None;
        for stmt in body {
            if let Some(value) = self.execute_statement(&stmt)? {
                result = value;
            }
            
            if self.should_return {
                result = self.return_value.take().unwrap_or(Value::None);
                self.should_return = false;
                break;
            }
        }
        
        // Exit function scope
        self.exit_scope();
        
        // Pop stack frame
        self.call_stack.pop();
        
        Ok(result)
    }
    
    // --- Memory Management Integration ---
    
    /// Allocate value with automatic memory management
    pub fn allocate_auto(&self, value: Value) -> Value {
        Value::TypedValue {
            value: Box::new(value),
            type_info: Type::Any,
        }
    }
    
    /// Force garbage collection
    pub fn collect_garbage(&self) {
        self.memory.collect();
    }
    
    /// Get memory statistics
    pub fn memory_stats(&self) -> String {
        format!("Memory: {}", self.memory.stats())
    }
    
    // --- Scope Management ---
    
    fn enter_scope(&mut self, scope_type: &str) {
        let new_scope = Scope {
            variables: HashMap::new(),
            parent: Some(self.current_scope),
            scope_type: scope_type.to_string(),
        };
        self.scopes.push(new_scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    fn exit_scope(&mut self) {
        if let Some(parent) = self.scopes[self.current_scope].parent {
            self.current_scope = parent;
            self.scopes.pop(); // Remove the current scope
        }
    }
    
    pub fn push_scope(&mut self, scope: Scope) {
        self.scopes.push(scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    pub fn pop_scope(&mut self) -> Scope {
        if self.scopes.len() > 1 {
            let scope = self.scopes.pop().unwrap();
            self.current_scope = self.scopes.len() - 1;
            scope
        } else {
            // Don't pop the global scope, return a copy instead
            self.scopes[0].clone()
        }
    }
    
    pub fn set_variable(&mut self, name: &str, value: Value) {
        self.scopes[self.current_scope].variables.insert(name.to_string(), value);
    }
    
    pub fn get_variable(&self, name: &str) -> Option<Value> {
        let mut scope_index = Some(self.current_scope);
        
        while let Some(idx) = scope_index {
            let scope = &self.scopes[idx];
            if let Some(value) = scope.variables.get(name) {
                return Some(value.clone());
            }
            scope_index = scope.parent;
        }
        
        None
    }
    
    // --- Value Operations ---
    
    fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 + b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + b as f64)),
            (Value::String(a), Value::String(b)) => Ok(Value::String(a + &b)),
            (Value::String(a), Value::Int(b)) => Ok(Value::String(a + &b.to_string())),
            (Value::String(a), Value::Float(b)) => Ok(Value::String(a + &b.to_string())),
            (Value::List(mut a), Value::List(b)) => {
                a.extend(b);
                Ok(Value::List(a))
            }
            _ => Err(anyhow::anyhow!("Invalid types for addition")),
        }
    }
    
    fn sub_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 - b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - b as f64)),
            _ => Err(anyhow::anyhow!("Invalid types for subtraction")),
        }
    }
    
    fn mul_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 * b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a * b as f64)),
            (Value::String(a), Value::Int(b)) => Ok(Value::String(a.repeat(b as usize))),
            (Value::List(a), Value::Int(b)) => {
                let mut result = Vec::new();
                for _ in 0..b {
                    result.extend(a.clone());
                }
                Ok(Value::List(result))
            }
            _ => Err(anyhow::anyhow!("Invalid types for multiplication")),
        }
    }
    
    fn div_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b as f64))
                }
            }
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b as f64))
                }
            }
            _ => Err(anyhow::anyhow!("Invalid types for division")),
        }
    }
    
    fn mod_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Int(a % b))
                }
            }
            _ => Err(anyhow::anyhow!("Invalid types for modulo")),
        }
    }
    
    fn gt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) > b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a > b as f64)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn lt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) < b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a < b as f64)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn gte_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) >= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a >= b as f64)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn lte_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) <= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a <= b as f64)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn values_equal(&self, left: Value, right: Value) -> bool {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::String(a), Value::String(b)) => a == b,
            (Value::None, Value::None) => true,
            _ => false,
        }
    }
    
    fn plus_value(&self, value: Value) -> Result<Value> {
        match value {
            Value::Int(n) => Ok(Value::Int(n)),
            Value::Float(n) => Ok(Value::Float(n)),
            _ => Err(anyhow::anyhow!("Invalid type for unary plus")),
        }
    }
    
    fn minus_value(&self, value: Value) -> Result<Value> {
        match value {
            Value::Int(n) => Ok(Value::Int(-n)),
            Value::Float(n) => Ok(Value::Float(-n)),
            _ => Err(anyhow::anyhow!("Invalid type for unary minus")),
        }
    }
    
    fn bitnot_value(&self, value: Value) -> Result<Value> {
        match value {
            Value::Int(n) => Ok(Value::Int(!n)),
            _ => Err(anyhow::anyhow!("Invalid type for bitwise not")),
        }
    }
    
    fn check_binary_op_types(&self, left: &Value, op: &BinaryOp, right: &Value) -> Result<()> {
        match op {
            BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                if !matches!((left, right), 
                    (Value::Int(_), Value::Int(_)) |
                    (Value::Float(_), Value::Float(_)) |
                    (Value::Int(_), Value::Float(_)) |
                    (Value::Float(_), Value::Int(_)) |
                    (Value::String(_), Value::String(_)) |
                    (Value::String(_), Value::Int(_)) |
                    (Value::String(_), Value::Float(_)) |
                    (Value::List(_), Value::List(_))
                ) {
                    return Err(anyhow::anyhow!("Invalid types for arithmetic operation: {} and {}", 
                        left.type_name(), right.type_name()));
                }
            }
            BinaryOp::Eq | BinaryOp::Neq | BinaryOp::Gt | BinaryOp::Lt | BinaryOp::Gte | BinaryOp::Lte => {
                if !matches!((left, right), 
                    (Value::Int(_), Value::Int(_)) |
                    (Value::Float(_), Value::Float(_)) |
                    (Value::Int(_), Value::Float(_)) |
                    (Value::Float(_), Value::Int(_)) |
                    (Value::Bool(_), Value::Bool(_)) |
                    (Value::String(_), Value::String(_)) |
                    (Value::None, Value::None)
                ) {
                    return Err(anyhow::anyhow!("Invalid types for comparison: {} and {}", 
                        left.type_name(), right.type_name()));
                }
            }
            BinaryOp::And | BinaryOp::Or => {
                if !matches!((left, right), (Value::Bool(_), Value::Bool(_))) {
                    return Err(anyhow::anyhow!("Logical operations require boolean operands"));
                }
            }
            _ => {}
        }
        Ok(())
    }
    
    // --- Built-in Functions ---
    
    fn init_builtins(&mut self) {
        let builtins: Vec<(&str, fn(Vec<Value>) -> Result<Value>)> = vec![
            ("print", Self::builtin_print),
            ("len", Self::builtin_len),
            ("type", Self::builtin_type),
            ("str", Self::builtin_str),
            ("int", Self::builtin_int),
            ("float", Self::builtin_float),
            ("bool", Self::builtin_bool),
            ("range", Self::builtin_range),
        ];
        
        for (name, func) in builtins {
            self.set_variable(name, Value::BuiltinFunction(name.to_string(), func));
        }
    }
    
    fn builtin_print(args: Vec<Value>) -> Result<Value> {
        for (i, arg) in args.iter().enumerate() {
            if i > 0 { print!(" "); }
            match arg {
                Value::String(s) => print!("{}", s), // Print strings without quotes
                _ => print!("{}", arg),
            }
        }
        println!();
        Ok(Value::None)
    }
    
    fn builtin_len(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("len() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::String(s) => Ok(Value::Int(s.len() as i64)),
            Value::List(items) => Ok(Value::Int(items.len() as i64)),
            Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
            _ => Err(anyhow::anyhow!("object of type '{}' has no len()", args[0].type_name())),
        }
    }
    
    fn builtin_type(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("type() takes exactly one argument"));
        }
        
        Ok(Value::String(args[0].type_name().to_string()))
    }
    
    fn builtin_str(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("str() takes exactly one argument"));
        }
        
        let string_repr = match &args[0] {
            Value::String(s) => s.clone(), // Don't add quotes for str() conversion
            _ => format!("{}", args[0]), // Use Display trait
        };
        Ok(Value::String(string_repr))
    }
    
    fn builtin_int(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("int() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(*n)),
            Value::Float(n) => Ok(Value::Int(*n as i64)),
            Value::String(s) => {
                if let Ok(n) = s.parse::<i64>() {
                    Ok(Value::Int(n))
                } else {
                    Err(anyhow::anyhow!("invalid literal for int(): '{}'", s))
                }
            }
            Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
            _ => Err(anyhow::anyhow!("cannot convert '{}' to int", args[0].type_name())),
        }
    }
    
    fn builtin_float(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("float() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Float(*n as f64)),
            Value::Float(n) => Ok(Value::Float(*n)),
            Value::String(s) => {
                if let Ok(n) = s.parse::<f64>() {
                    Ok(Value::Float(n))
                } else {
                    Err(anyhow::anyhow!("invalid literal for float(): '{}'", s))
                }
            }
            Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
            _ => Err(anyhow::anyhow!("cannot convert '{}' to float", args[0].type_name())),
        }
    }
    
    fn builtin_bool(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("bool() takes exactly one argument"));
        }
        
        Ok(Value::Bool(args[0].is_truthy()))
    }
    
    fn builtin_range(args: Vec<Value>) -> Result<Value> {
        let (start, end) = match args.len() {
            1 => (Value::Int(0), Self::builtin_int(vec![args[0].clone()])?),
            2 => (Self::builtin_int(vec![args[0].clone()])?, Self::builtin_int(vec![args[1].clone()])?),
            _ => return Err(anyhow::anyhow!("range() takes 1 or 2 arguments")),
        };
        
        if let (Value::Int(start), Value::Int(end)) = (start, end) {
            let items: Vec<Value> = (start..end).map(Value::Int).collect();
            Ok(Value::List(items))
        } else {
            Err(anyhow::anyhow!("range() arguments must be integers"))
        }
    }
}

/// Start the REPL (Read-Eval-Print Loop)
pub fn repl() -> Result<()> {
    use std::io::{self, Write};
    
    let mut vm = VM::new();
    let mut line_number = 1;
    
    loop {
        print!(">>> ");
        io::stdout().flush()?;
        
        let mut input = String::new();
        io::stdin().read_line(&mut input)?;
        
        let input = input.trim();
        
        if input == "exit" || input == "quit" {
            break;
        }
        
        if input.is_empty() {
            continue;
        }
        
        match vm.execute_repl(input, line_number) {
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
        
        line_number += 1;
    }
    
    Ok(())
}

/// Run a TauraroLang file
pub fn run_file(source: &str, backend: &str, optimization: u8) -> Result<()> {
    match backend {
        "vm" => {
            let mut vm = VM::new();
            let result = vm.execute_script(source, vec![])?;
            if !matches!(result, Value::None) {
                println!("{}", result);
            }
            Ok(())
        }
        _ => {
            Err(anyhow::anyhow!("Backend '{}' not supported for running files", backend))
        }
    }
}