//! COMPLETE semantic analysis with optional static typing
use crate::ast::*;
use anyhow::{Result, anyhow};
use std::collections::HashMap;

/// Symbol table entry
#[derive(Debug, Clone)]
pub struct Symbol {
    pub name: String,
    pub symbol_type: Type,
    pub is_mutable: bool,
    pub span: Span,
    pub is_defined: bool,
}

/// Scope containing symbols
#[derive(Debug, Clone)]
pub struct Scope {
    pub symbols: HashMap<String, Symbol>,
    pub parent: Option<usize>, // Index to parent scope
    pub scope_type: ScopeType,
}

/// Type of scope
#[derive(Debug, Clone, PartialEq)]
pub enum ScopeType {
    Global,
    Function,
    Class,
    Loop,
    Block,
}

/// Semantic analyzer context
pub struct SemanticAnalyzer {
    scopes: Vec<Scope>,
    current_scope: usize,
    errors: Vec<SemanticError>,
    enforce_types: bool,
    in_async_context: bool,
}

/// Semantic error
#[derive(Debug, Clone)]
pub struct SemanticError {
    pub message: String,
    pub span: Span,
    pub error_type: SemanticErrorType,
}

/// Type of semantic error
#[derive(Debug, Clone)]
pub enum SemanticErrorType {
    UndefinedVariable,
    TypeMismatch,
    InvalidOperation,
    DuplicateSymbol,
    InvalidAsync,
}

impl SemanticAnalyzer {
    pub fn new() -> Self {
        let global_scope = Scope {
            symbols: HashMap::new(),
            parent: None,
            scope_type: ScopeType::Global,
        };
        
        Self {
            scopes: vec![global_scope],
            current_scope: 0,
            errors: Vec::new(),
            enforce_types: false,
            in_async_context: false,
        }
    }
    
    /// Main entry point - analyze entire program with optional type checking
    pub fn analyze_optional_types(program: Program, enforce_types: bool) -> Result<Program> {
        let mut analyzer = Self::new();
        analyzer.enforce_types = enforce_types;
        analyzer.analyze_program(program)
    }
    
    /// Legacy function for backward compatibility
    pub fn analyze(program: Program) -> Result<Program> {
        Self::analyze_optional_types(program, false) // Default to dynamic typing
    }
    
    /// Analyze program
    fn analyze_program(&mut self, program: Program) -> Result<Program> {
        // Add built-in types and functions
        self.add_builtins();
        
        // Analyze each statement
        let analyzed_statements: Vec<Stmt> = program.statements
            .into_iter()
            .filter_map(|stmt| {
                match self.analyze_statement(stmt) {
                    Ok(stmt) => Some(stmt),
                    Err(e) => {
                        self.errors.push(SemanticError {
                            message: e.to_string(),
                            span: Span::unknown(),
                            error_type: SemanticErrorType::InvalidOperation,
                        });
                        None
                    }
                }
            })
            .collect();
        
        // Report errors if any
        if !self.errors.is_empty() {
            let error_messages: Vec<String> = self.errors
                .iter()
                .map(|e| format!("{} at line {}", e.message, e.span.line))
                .collect();
            return Err(anyhow!("Semantic analysis found {} errors:\n{}", 
                self.errors.len(), error_messages.join("\n")));
        }
        
        Ok(Program {
            statements: analyzed_statements,
            span: program.span,
        })
    }
    
    /// Analyze a statement
    fn analyze_statement(&mut self, stmt: Stmt) -> Result<Stmt> {
        match stmt {
            Stmt::Function { name, parameters, return_type, body, span, is_async, is_export } => {
                self.analyze_function(name, parameters, return_type, body, span, is_async, is_export)
            }
            Stmt::Class { name, bases, body, span, is_export } => {
                self.analyze_class(name, bases, body, span, is_export)
            }
            Stmt::Variable { name, type_annotation, value, span } => {
                self.analyze_variable(name, type_annotation, value, span)
            }
            Stmt::If { condition, then_branch, elif_branches, else_branch, span } => {
                self.analyze_if_statement(condition, then_branch, elif_branches, else_branch, span)
            }
            Stmt::Expression(expr, span) => {
                let analyzed_expr = self.analyze_expression(expr)?;
                Ok(Stmt::Expression(analyzed_expr, span))
            }
            Stmt::Assignment { target, value, span } => {
                self.analyze_assignment(target, value, span)
            }
            Stmt::Return { value, span } => {
                self.analyze_return(value, span)
            }
            Stmt::Import { module, alias, span } => {
                Ok(Stmt::Import { module, alias, span })
            }
            Stmt::Extern { library, span } => {
                Ok(Stmt::Extern { library, span })
            }
            _ => Ok(stmt), // TODO: Implement other statement types
        }
    }
    
    /// Analyze function definition
    fn analyze_function(
        &mut self, 
        name: String, 
        parameters: Vec<Parameter>, 
        return_type: Option<Type>, 
        body: Vec<Stmt>, 
        span: Span,
        is_async: bool,
        is_export: bool,
    ) -> Result<Stmt> {
        let previous_async_context = self.in_async_context;
        self.in_async_context = is_async;
        
        // Create function scope
        self.enter_scope(ScopeType::Function);
        
        // Add parameters to scope
        for param in &parameters {
            let param_type = param.type_annotation.clone().unwrap_or(Type::Any);
            self.add_symbol(param.name.clone(), param_type, true, param.span, true)?;
        }
        
        // Analyze function body
        let analyzed_body: Result<Vec<Stmt>> = body
            .into_iter()
            .map(|stmt| self.analyze_statement(stmt))
            .collect();
        
        let analyzed_body = analyzed_body?;
        
        // Infer return type if not specified
        let final_return_type = if let Some(rt) = return_type {
            rt
        } else {
            self.infer_return_type(&analyzed_body).unwrap_or(Type::Any)
        };
        
        // For async functions, wrap return type in Future
        let final_return_type = if is_async {
            Type::Custom(format!("Future[{}]", final_return_type))
        } else {
            final_return_type
        };
        
        self.exit_scope();
        self.in_async_context = previous_async_context;
        
        // Add function to current scope
        let param_types: Vec<Type> = parameters.iter()
            .map(|p| p.type_annotation.clone().unwrap_or(Type::Any))
            .collect();
        
        let func_type = Type::Function(param_types, Box::new(final_return_type.clone()));
        self.add_symbol(name.clone(), func_type, false, span, true)?;
        
        Ok(Stmt::Function {
            name,
            parameters,
            return_type: Some(final_return_type),
            body: analyzed_body,
            span,
            is_async,
            is_export,
        })
    }
    
    /// Analyze class definition
    fn analyze_class(
        &mut self,
        name: String,
        bases: Vec<Expr>,
        body: Vec<Stmt>,
        span: Span,
        is_export: bool,
    ) -> Result<Stmt> {
        // Analyze base classes
        let analyzed_bases: Result<Vec<Expr>> = bases
            .into_iter()
            .map(|base| self.analyze_expression(base))
            .collect();
        
        let analyzed_bases = analyzed_bases?;
        
        // Enter class scope
        self.enter_scope(ScopeType::Class);
        
        // Add special variables: self, cls
        self.add_symbol("self".to_string(), Type::Custom(name.clone()), true, span, true)?;
        self.add_symbol("cls".to_string(), Type::Custom(format!("Type[{}]", name)), true, span, true)?;
        
        // Analyze class body
        let analyzed_body: Result<Vec<Stmt>> = body
            .into_iter()
            .map(|stmt| self.analyze_statement(stmt))
            .collect();
        
        let analyzed_body = analyzed_body?;
        
        self.exit_scope();
        
        // Add class type to current scope
        self.add_symbol(name.clone(), Type::Custom(name.clone()), false, span, true)?;
        
        Ok(Stmt::Class {
            name,
            bases: analyzed_bases,
            body: analyzed_body,
            span,
            is_export,
        })
    }
    
    /// Analyze variable declaration
    fn analyze_variable(
        &mut self,
        name: String,
        type_annotation: Option<Type>,
        value: Option<Expr>,
        span: Span,
    ) -> Result<Stmt> {
        let value_type = if let Some(expr) = &value {
            self.analyze_expression(expr.clone()).and_then(|e| self.infer_expression_type(&e))
        } else {
            Ok(Type::Any)
        }?;
        
        let final_type = type_annotation.unwrap_or(value_type);
        
        // Check type compatibility if we have both annotation and value
        if let (Some(annotation), Some(expr)) = (&type_annotation, &value) {
            if self.enforce_types {
                let actual_type = self.infer_expression_type(&self.analyze_expression(expr.clone())?)?;
                if !self.types_compatible(&actual_type, annotation) {
                    return Err(anyhow!("Type mismatch: variable '{}' declared as {} but assigned {}", 
                        name, annotation, actual_type));
                }
            }
        }
        
        // Add variable to current scope
        self.add_symbol(name.clone(), final_type.clone(), true, span, true)?;
        
        Ok(Stmt::Variable {
            name,
            type_annotation: Some(final_type),
            value: value.map(|e| self.analyze_expression(e)).transpose()?,
            span,
        })
    }
    
    /// Analyze if statement
    fn analyze_if_statement(
        &mut self,
        condition: Expr,
        then_branch: Vec<Stmt>,
        elif_branches: Vec<(Expr, Vec<Stmt>)>,
        else_branch: Option<Vec<Stmt>>,
        span: Span,
    ) -> Result<Stmt> {
        // Analyze condition - should be boolean
        let analyzed_condition = self.analyze_expression(condition)?;
        if self.enforce_types {
            self.expect_type(&analyzed_condition, &Type::Bool)?;
        }
        
        // Analyze then branch
        self.enter_scope(ScopeType::Block);
        let analyzed_then: Result<Vec<Stmt>> = then_branch
            .into_iter()
            .map(|stmt| self.analyze_statement(stmt))
            .collect();
        let analyzed_then = analyzed_then?;
        self.exit_scope();
        
        // Analyze elif branches
        let analyzed_elifs: Result<Vec<(Expr, Vec<Stmt>)>> = elif_branches
            .into_iter()
            .map(|(cond, branch)| {
                let analyzed_cond = self.analyze_expression(cond)?;
                if self.enforce_types {
                    self.expect_type(&analyzed_cond, &Type::Bool)?;
                }
                
                self.enter_scope(ScopeType::Block);
                let analyzed_branch: Result<Vec<Stmt>> = branch
                    .into_iter()
                    .map(|stmt| self.analyze_statement(stmt))
                    .collect();
                let analyzed_branch = analyzed_branch?;
                self.exit_scope();
                
                Ok((analyzed_cond, analyzed_branch))
            })
            .collect();
        let analyzed_elifs = analyzed_elifs?;
        
        // Analyze else branch
        let analyzed_else = if let Some(else_branch) = else_branch {
            self.enter_scope(ScopeType::Block);
            let analyzed: Result<Vec<Stmt>> = else_branch
                .into_iter()
                .map(|stmt| self.analyze_statement(stmt))
                .collect();
            let analyzed = analyzed?;
            self.exit_scope();
            Some(analyzed)
        } else {
            None
        };
        
        Ok(Stmt::If {
            condition: analyzed_condition,
            then_branch: analyzed_then,
            elif_branches: analyzed_elifs,
            else_branch: analyzed_else,
            span,
        })
    }
    
    /// Analyze expression
    fn analyze_expression(&mut self, expr: Expr) -> Result<Expr> {
        match expr {
            Expr::Identifier(name, span) => {
                if let Some(symbol) = self.lookup_symbol(&name) {
                    if !symbol.is_defined {
                        return Err(anyhow!("Variable '{}' used before definition", name));
                    }
                    Ok(Expr::Identifier(name, span))
                } else if self.enforce_types {
                    Err(anyhow!("Undefined variable: {}", name))
                } else {
                    // In dynamic mode, allow undefined variables
                    Ok(Expr::Identifier(name, span))
                }
            }
            Expr::Binary { left, op, right, span } => {
                let analyzed_left = self.analyze_expression(*left)?;
                let analyzed_right = self.analyze_expression(*right)?;
                
                // Type checking for binary operations
                if self.enforce_types {
                    let _ = self.check_binary_op(&analyzed_left, &op, &analyzed_right)?;
                }
                
                Ok(Expr::Binary {
                    left: Box::new(analyzed_left),
                    op,
                    right: Box::new(analyzed_right),
                    span,
                })
            }
            Expr::Call { callee, arguments, span } => {
                let analyzed_callee = self.analyze_expression(*callee)?;
                let analyzed_args: Result<Vec<Expr>> = arguments
                    .into_iter()
                    .map(|arg| self.analyze_expression(arg))
                    .collect();
                let analyzed_args = analyzed_args?;
                
                // Type checking for function calls
                if self.enforce_types {
                    if let Expr::Identifier(name, _) = *analyzed_callee {
                        if let Some(symbol) = self.lookup_symbol(&name) {
                            if let Type::Function(param_types, _) = &symbol.symbol_type {
                                if param_types.len() != analyzed_args.len() {
                                    return Err(anyhow!("Function {} expects {} arguments, got {}", 
                                        name, param_types.len(), analyzed_args.len()));
                                }
                                
                                for (i, (arg, expected_type)) in analyzed_args.iter().zip(param_types).enumerate() {
                                    let arg_type = self.infer_expression_type(arg)?;
                                    if !self.types_compatible(&arg_type, expected_type) {
                                        return Err(anyhow!("Argument {} to {}: expected {}, got {}", 
                                            i + 1, name, expected_type, arg_type));
                                    }
                                }
                            }
                        }
                    }
                }
                
                Ok(Expr::Call {
                    callee: Box::new(analyzed_callee),
                    arguments: analyzed_args,
                    span,
                })
            }
            Expr::Typed { expr, type_annotation, span } => {
                let analyzed_expr = self.analyze_expression(*expr)?;
                
                if self.enforce_types {
                    let expr_type = self.infer_expression_type(&analyzed_expr)?;
                    if !self.types_compatible(&expr_type, &type_annotation) {
                        return Err(anyhow!("Type annotation mismatch: expected {}, got {}", 
                            type_annotation, expr_type));
                    }
                }
                
                Ok(Expr::Typed {
                    expr: Box::new(analyzed_expr),
                    type_annotation,
                    span,
                })
            }
            _ => Ok(expr), // TODO: Implement other expression types
        }
    }
    
    /// Analyze assignment
    fn analyze_assignment(&mut self, target: AssignTarget, value: Expr, span: Span) -> Result<Stmt> {
        let analyzed_value = self.analyze_expression(value)?;
        
        match target {
            AssignTarget::Identifier(name, target_span) => {
                // Check if variable exists or create new one
                if let Some(symbol) = self.lookup_symbol(&name) {
                    if !symbol.is_mutable {
                        return Err(anyhow!("Cannot assign to immutable variable: {}", name));
                    }
                    
                    // Type checking
                    if self.enforce_types {
                        let value_type = self.infer_expression_type(&analyzed_value)?;
                        if !self.types_compatible(&value_type, &symbol.symbol_type) {
                            return Err(anyhow!("Type mismatch for variable '{}': expected {}, got {}", 
                                name, symbol.symbol_type, value_type));
                        }
                    }
                } else {
                    // Implicit variable declaration - infer type
                    let value_type = self.infer_expression_type(&analyzed_value)?;
                    self.add_symbol(name.clone(), value_type, true, target_span, true)?;
                }
                
                Ok(Stmt::Assignment {
                    target: AssignTarget::Identifier(name, target_span),
                    value: analyzed_value,
                    span,
                })
            }
            _ => Err(anyhow!("Complex assignment targets not yet supported")),
        }
    }
    
    /// Analyze return statement
    fn analyze_return(&mut self, value: Option<Expr>, span: Span) -> Result<Stmt> {
        let analyzed_value = value.map(|v| self.analyze_expression(v)).transpose()?;
        
        // Check return type compatibility with function return type
        if self.enforce_types {
            if let Some(current_function) = self.get_current_function_return_type() {
                if let Some(expr) = &analyzed_value {
                    let return_type = self.infer_expression_type(expr)?;
                    if !self.types_compatible(&return_type, &current_function) {
                        return Err(anyhow!("Return type mismatch: expected {}, got {}", 
                            current_function, return_type));
                    }
                } else if current_function != Type::Any {
                    return Err(anyhow!("Function expects return type {} but returns nothing", 
                        current_function));
                }
            }
        }
        
        Ok(Stmt::Return {
            value: analyzed_value,
            span,
        })
    }
    
    // --- Utility methods ---
    
    fn enter_scope(&mut self, scope_type: ScopeType) {
        let new_scope = Scope {
            symbols: HashMap::new(),
            parent: Some(self.current_scope),
            scope_type,
        };
        self.scopes.push(new_scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    fn exit_scope(&mut self) {
        if let Some(parent) = self.scopes[self.current_scope].parent {
            self.current_scope = parent;
        }
    }
    
    fn add_symbol(&mut self, name: String, symbol_type: Type, is_mutable: bool, span: Span, is_defined: bool) -> Result<()> {
        let current_scope = &mut self.scopes[self.current_scope];
        
        if current_scope.symbols.contains_key(&name) {
            Err(anyhow!("Duplicate symbol: {}", name))
        } else {
            current_scope.symbols.insert(name.clone(), Symbol {
                name,
                symbol_type,
                is_mutable,
                span,
                is_defined,
            });
            Ok(())
        }
    }
    
    fn lookup_symbol(&self, name: &str) -> Option<&Symbol> {
        let mut scope_index = Some(self.current_scope);
        
        while let Some(idx) = scope_index {
            let scope = &self.scopes[idx];
            if let Some(symbol) = scope.symbols.get(name) {
                return Some(symbol);
            }
            scope_index = scope.parent;
        }
        
        None
    }
    
    fn get_current_function_return_type(&self) -> Option<Type> {
        let mut scope_index = Some(self.current_scope);
        
        while let Some(idx) = scope_index {
            let scope = &self.scopes[idx];
            if scope.scope_type == ScopeType::Function {
                // Look for function return type in parent scope
                if let Some(parent_idx) = scope.parent {
                    if let Some(symbol) = self.scopes[parent_idx].symbols.values().find(|s| {
                        if let Type::Function(_, return_type) = &s.symbol_type {
                            true
                        } else {
                            false
                        }
                    }) {
                        if let Type::Function(_, return_type) = &symbol.symbol_type {
                            return Some(*return_type.clone());
                        }
                    }
                }
            }
            scope_index = scope.parent;
        }
        
        None
    }
    
    fn expect_type(&self, expr: &Expr, expected: &Type) -> Result<()> {
        let actual = self.infer_expression_type(expr)?;
        if self.types_compatible(&actual, expected) {
            Ok(())
        } else {
            Err(anyhow!("Type mismatch: expected {}, found {}", expected, actual))
        }
    }
    
    fn infer_expression_type(&self, expr: &Expr) -> Result<Type> {
        match expr {
            Expr::Int(_, _) => Ok(Type::Int),
            Expr::Float(_, _) => Ok(Type::Float),
            Expr::String(_, _) => Ok(Type::Str),
            Expr::Bool(_, _) => Ok(Type::Bool),
            Expr::None(_) => Ok(Type::Any),
            Expr::Identifier(name, _) => {
                if let Some(symbol) = self.lookup_symbol(name) {
                    Ok(symbol.symbol_type.clone())
                } else {
                    Ok(Type::Any) // Dynamic type for undefined variables
                }
            }
            Expr::Binary { left, op, right, .. } => {
                let left_type = self.infer_expression_type(left)?;
                let right_type = self.infer_expression_type(right)?;
                self.check_binary_op_type(&left_type, op, &right_type)
            }
            Expr::Typed { type_annotation, .. } => Ok(type_annotation.clone()),
            _ => Ok(Type::Any), // Default for complex expressions
        }
    }
    
    fn types_compatible(&self, actual: &Type, expected: &Type) -> bool {
        match (actual, expected) {
            (Type::Any, _) | (_, Type::Any) => true, // Any type is compatible with any
            (Type::Int, Type::Float) | (Type::Float, Type::Int) => true, // Numeric compatibility
            (a, b) => a == b, // Exact match
        }
    }
    
    fn check_binary_op(&self, left: &Expr, op: &BinaryOp, right: &Expr) -> Result<Type> {
        let left_type = self.infer_expression_type(left)?;
        let right_type = self.infer_expression_type(right)?;
        self.check_binary_op_type(&left_type, op, &right_type)
    }
    
    fn check_binary_op_type(&self, left_type: &Type, op: &BinaryOp, right_type: &Type) -> Result<Type> {
        match op {
            BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                if left_type == Type::Int && right_type == Type::Int {
                    Ok(Type::Int)
                } else if matches!(left_type, Type::Int | Type::Float) && 
                          matches!(right_type, Type::Int | Type::Float) {
                    Ok(Type::Float)
                } else if left_type == Type::Str && right_type == Type::Str && *op == BinaryOp::Add {
                    Ok(Type::Str) // String concatenation
                } else {
                    Err(anyhow!("Operation {} not supported for types {} and {}", op, left_type, right_type))
                }
            }
            BinaryOp::Eq | BinaryOp::Neq | BinaryOp::Gt | BinaryOp::Lt | BinaryOp::Gte | BinaryOp::Lte => {
                if self.types_compatible(left_type, right_type) {
                    Ok(Type::Bool)
                } else {
                    Err(anyhow!("Comparison operation not supported for types {} and {}", left_type, right_type))
                }
            }
            BinaryOp::And | BinaryOp::Or => {
                if left_type == Type::Bool && right_type == Type::Bool {
                    Ok(Type::Bool)
                } else {
                    Err(anyhow!("Logical operation requires boolean operands"))
                }
            }
            _ => Ok(Type::Any), // Default for unsupported operations
        }
    }
    
    fn infer_return_type(&self, body: &[Stmt]) -> Option<Type> {
        for stmt in body {
            if let Stmt::Return { value, .. } = stmt {
                if let Some(expr) = value {
                    return self.infer_expression_type(expr).ok();
                } else {
                    return Some(Type::Any); // void return
                }
            }
        }
        None
    }
    
    fn add_builtins(&mut self) {
        // Add built-in functions with type signatures
        let builtins = [
            ("print", Type::Function(vec![Type::Any], Box::new(Type::Any))),
            ("len", Type::Function(vec![Type::Any], Box::new(Type::Int))),
            ("range", Type::Function(vec![Type::Int, Type::Int], Box::new(Type::List(Box::new(Type::Int))))),
            ("type", Type::Function(vec![Type::Any], Box::new(Type::Str))),
            ("str", Type::Function(vec![Type::Any], Box::new(Type::Str))),
            ("int", Type::Function(vec![Type::Any], Box::new(Type::Int))),
            ("float", Type::Function(vec![Type::Any], Box::new(Type::Float))),
            ("bool", Type::Function(vec![Type::Any], Box::new(Type::Bool))),
        ];
        
        for (name, func_type) in builtins {
            self.add_symbol(name.to_string(), func_type, false, Span::unknown(), true).unwrap();
        }
        
        // Add built-in constants
        let constants = [
            ("true", Type::Bool),
            ("false", Type::Bool),
            ("none", Type::Any),
        ];
        
        for (name, const_type) in constants {
            self.add_symbol(name.to_string(), const_type, false, Span::unknown(), true).unwrap();
        }
    }
}

// Error reporting
impl fmt::Display for SemanticError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.message)
    }
}