// Semantic analysis: type checking, scope, symbol table
//! Semantic analysis - Type checking, symbol resolution, and validation
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
}

/// Scope containing symbols
#[derive(Debug, Clone)]
pub struct Scope {
    pub symbols: HashMap<String, Symbol>,
    pub parent: Option<usize>, // Index to parent scope
}

/// Semantic analyzer context
pub struct SemanticAnalyzer {
    scopes: Vec<Scope>,
    current_scope: usize,
    errors: Vec<SemanticError>,
}

/// Semantic error
#[derive(Debug, Clone)]
pub struct SemanticError {
    pub message: String,
    pub span: Span,
}

impl SemanticAnalyzer {
    pub fn new() -> Self {
        let global_scope = Scope {
            symbols: HashMap::new(),
            parent: None,
        };
        
        Self {
            scopes: vec![global_scope],
            current_scope: 0,
            errors: Vec::new(),
        }
    }
    
    /// Main entry point - analyze entire program
    pub fn analyze(program: Program) -> Result<Program> {
        let mut analyzer = Self::new();
        analyzer.analyze_program(program)
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
                        });
                        None
                    }
                }
            })
            .collect();
        
        // Report errors if any
        if !self.errors.is_empty() {
            return Err(anyhow!("Semantic analysis found {} errors", self.errors.len()));
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
            _ => Ok(stmt), // Placeholder for other statement types
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
        // Create function scope
        self.enter_scope();
        
        // Add parameters to scope
        for param in &parameters {
            let param_type = param.type_annotation.clone().unwrap_or(Type::Any);
            self.add_symbol(param.name.clone(), param_type, true, param.span)?;
        }
        
        // Analyze function body
        let analyzed_body: Result<Vec<Stmt>> = body
            .into_iter()
            .map(|stmt| self.analyze_statement(stmt))
            .collect();
        
        let analyzed_body = analyzed_body?;
        
        // Infer return type if not specified
        let final_return_type = return_type.unwrap_or_else(|| {
            // Simple type inference: look for return statements
            self.infer_return_type(&analyzed_body).unwrap_or(Type::Any)
        });
        
        self.exit_scope();
        
        // Add function to current scope
        let func_type = Type::Function(
            parameters.iter()
                .map(|p| p.type_annotation.clone().unwrap_or(Type::Any))
                .collect(),
            Box::new(final_return_type),
        );
        
        self.add_symbol(name.clone(), func_type, false, span)?;
        
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
        self.enter_scope();
        
        // Analyze class body
        let analyzed_body: Result<Vec<Stmt>> = body
            .into_iter()
            .map(|stmt| self.analyze_statement(stmt))
            .collect();
        
        let analyzed_body = analyzed_body?;
        
        self.exit_scope();
        
        // Add class type to current scope
        self.add_symbol(name.clone(), Type::Custom(name.clone()), false, span)?;
        
        Ok(Stmt::Class {
            name,
            bases: analyzed_bases,
            body: analyzed_body,
            span,
            is_export,
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
        self.expect_type(&analyzed_condition, &Type::Bool)?;
        
        // Analyze then branch
        self.enter_scope();
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
                self.expect_type(&analyzed_cond, &Type::Bool)?;
                
                self.enter_scope();
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
            self.enter_scope();
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
                    Ok(Expr::Identifier(name, span))
                } else {
                    Err(anyhow!("Undefined variable: {}", name))
                }
            }
            Expr::Binary { left, op, right, span } => {
                let analyzed_left = self.analyze_expression(*left)?;
                let analyzed_right = self.analyze_expression(*right)?;
                
                // Type checking for binary operations
                let result_type = self.check_binary_op(&analyzed_left, &op, &analyzed_right)?;
                
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
                
                // TODO: Check function call types
                Ok(Expr::Call {
                    callee: Box::new(analyzed_callee),
                    arguments: analyzed_args,
                    span,
                })
            }
            _ => Ok(expr), // Placeholder for other expression types
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
                    // TODO: Check type compatibility
                } else {
                    // Implicit variable declaration - infer type
                    let value_type = self.infer_expression_type(&analyzed_value)?;
                    self.add_symbol(name.clone(), value_type, true, target_span)?;
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
    
    // --- Utility methods ---
    
    fn enter_scope(&mut self) {
        let new_scope = Scope {
            symbols: HashMap::new(),
            parent: Some(self.current_scope),
        };
        self.scopes.push(new_scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    fn exit_scope(&mut self) {
        if let Some(parent) = self.scopes[self.current_scope].parent {
            self.current_scope = parent;
        }
    }
    
    fn add_symbol(&mut self, name: String, symbol_type: Type, is_mutable: bool, span: Span) -> Result<()> {
        let current_scope = &mut self.scopes[self.current_scope];
        
        if current_scope.symbols.contains_key(&name) {
            Err(anyhow!("Duplicate symbol: {}", name))
        } else {
            current_scope.symbols.insert(name.clone(), Symbol {
                name,
                symbol_type,
                is_mutable,
                span,
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
                    Err(anyhow!("Unknown variable: {}", name))
                }
            }
            _ => Ok(Type::Any), // TODO: Implement for other expression types
        }
    }
    
    fn types_compatible(&self, actual: &Type, expected: &Type) -> bool {
        // Simple type compatibility check
        match (actual, expected) {
            (Type::Any, _) | (_, Type::Any) => true, // Any type is compatible with any
            (a, b) => a == b,
        }
    }
    
    fn check_binary_op(&self, left: &Expr, op: &BinaryOp, right: &Expr) -> Result<Type> {
        let left_type = self.infer_expression_type(left)?;
        let right_type = self.infer_expression_type(right)?;
        
        match op {
            BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                if left_type == Type::Int && right_type == Type::Int {
                    Ok(Type::Int)
                } else if matches!(left_type, Type::Int | Type::Float) && 
                          matches!(right_type, Type::Int | Type::Float) {
                    Ok(Type::Float)
                } else {
                    Err(anyhow!("Arithmetic operation not supported for types {} and {}", left_type, right_type))
                }
            }
            BinaryOp::Eq | BinaryOp::Neq | BinaryOp::Gt | BinaryOp::Lt | BinaryOp::Gte | BinaryOp::Lte => {
                if self.types_compatible(&left_type, &right_type) {
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
            _ => Ok(Type::Any), // TODO: Implement for other operators
        }
    }
    
    fn infer_return_type(&self, body: &[Stmt]) -> Option<Type> {
        // Simple return type inference: look for return statements
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
        // Add built-in functions
        let builtins = [
            ("print", Type::Function(vec![Type::Any], Box::new(Type::Any))),
            ("len", Type::Function(vec![Type::Any], Box::new(Type::Int))),
            ("range", Type::Function(vec![Type::Int, Type::Int], Box::new(Type::List(Box::new(Type::Int))))),
        ];
        
        for (name, func_type) in builtins {
            self.add_symbol(name.to_string(), func_type, false, Span::unknown()).unwrap();
        }
    }
}