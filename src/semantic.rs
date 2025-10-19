//! Semantic analysis (type checking, scope analysis, etc.)

use crate::ast::*;
use std::collections::HashMap;
use anyhow::Result;

#[derive(Debug)]
pub struct SemanticError {
    pub message: String,
    pub line: usize,
    pub column: usize,
}

pub struct Analyzer {
    strict_types: bool,
    scopes: Vec<Scope>,
}

#[derive(Debug, Clone)]
struct Scope {
    variables: HashMap<String, Type>,
    is_function_scope: bool,
}

impl Analyzer {
    pub fn new(strict_types: bool) -> Self {
        Self {
            strict_types,
            scopes: vec![Scope {
                variables: HashMap::new(),
                is_function_scope: false,
            }],
        }
    }

    pub fn analyze(&mut self, program: Program) -> Result<Program> {
        // For now, we'll just return the program as-is
        // In a full implementation, this would perform semantic analysis
        Ok(program)
    }

    fn enter_scope(&mut self, is_function_scope: bool) {
        self.scopes.push(Scope {
            variables: HashMap::new(),
            is_function_scope,
        });
    }

    fn exit_scope(&mut self) -> Option<Scope> {
        self.scopes.pop()
    }

    fn declare_variable(&mut self, name: String, type_info: Type) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.variables.insert(name, type_info);
        }
    }

    fn lookup_variable(&self, name: &str) -> Option<&Type> {
        // Search scopes from innermost to outermost
        for scope in self.scopes.iter().rev() {
            if let Some(type_info) = scope.variables.get(name) {
                return Some(type_info);
            }
        }
        None
    }
}