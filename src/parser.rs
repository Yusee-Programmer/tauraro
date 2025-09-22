//! Recursive descent parser for TauraroLang - Builds AST from token stream
use crate::lexer::{Token, LexError};
use crate::ast::*;
use anyhow::{Result, anyhow};
use std::iter::Peekable;
use std::vec::IntoIter;

/// Parser error type
#[derive(Debug, Clone)]
pub struct ParseError {
    pub message: String,
    pub expected: Vec<String>,
    pub found: Token,
    pub position: usize,
}

impl std::fmt::Display for ParseError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "Parse error at position {}: {}", self.position, self.message)
    }
}

impl std::error::Error for ParseError {}

/// Main parser structure
pub struct Parser {
    tokens: Peekable<IntoIter<Token>>,
    current_span: Span,
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Self {
        Self {
            tokens: tokens.into_iter().peekable(),
            current_span: Span::unknown(),
        }
    }

    /// Main entry point - parse entire program
    pub fn parse(&mut self) -> Result<Program> {
        let mut statements = Vec::new();
        
        while self.tokens.peek().is_some() {
            statements.push(self.parse_statement()?);
        }
        
        Ok(Program::new(statements))
    }

    /// Parse a statement
    fn parse_statement(&mut self) -> Result<Stmt> {
        let token = self.peek_token()?;
        
        match token {
            Token::KwFunc | Token::KwAsync => self.parse_function(),
            Token::KwClass => self.parse_class(),
            Token::KwIf => self.parse_if_statement(),
            Token::KwWhile => self.parse_while_statement(),
            Token::KwFor => self.parse_for_statement(),
            Token::KwReturn => self.parse_return_statement(),
            Token::KwBreak => self.parse_break_statement(),
            Token::KwContinue => self.parse_continue_statement(),
            Token::KwImport => self.parse_import_statement(),
            Token::KwExtern => self.parse_extern_statement(),
            Token::KwExport => self.parse_export_statement(),
            _ => self.parse_expression_statement(),
        }
    }

    /// Parse function definition
    fn parse_function(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        let is_async = self.consume_if(Token::KwAsync).is_some();
        self.expect_token(Token::KwFunc)?;
        
        let name = if let Token::Identifier(name) = self.expect_identifier()? {
            name
        } else {
            return Err(self.error("Expected function name"));
        };
        
        self.expect_token(Token::LParen)?;
        let parameters = self.parse_parameters()?;
        self.expect_token(Token::RParen)?;
        
        let return_type = if self.consume_if(Token::Arrow).is_some() {
            Some(self.parse_type()?)
        } else {
            None
        };
        
        self.expect_token(Token::Colon)?;
        let body = self.parse_block()?;
        
        Ok(Stmt::Function {
            name,
            parameters,
            return_type,
            body,
            span: self.merge_span(start_span),
            is_async,
            is_export: false, // Handled by export statement
        })
    }

    /// Parse class definition
    fn parse_class(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwClass)?;
        
        let name = if let Token::Identifier(name) = self.expect_identifier()? {
            name
        } else {
            return Err(self.error("Expected class name"));
        };
        
        let bases = if self.consume_if(Token::LParen).is_some() {
            let bases = self.parse_expression_list(Token::RParen)?;
            self.expect_token(Token::RParen)?;
            bases
        } else {
            Vec::new()
        };
        
        self.expect_token(Token::Colon)?;
        let body = self.parse_block()?;
        
        Ok(Stmt::Class {
            name,
            bases,
            body,
            span: self.merge_span(start_span),
            is_export: false,
        })
    }

    /// Parse if statement
    fn parse_if_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwIf)?;
        
        let condition = self.parse_expression()?;
        self.expect_token(Token::Colon)?;
        let then_branch = self.parse_block()?;
        
        let mut elif_branches = Vec::new();
        while self.consume_if(Token::KwElif).is_some() {
            let elif_condition = self.parse_expression()?;
            self.expect_token(Token::Colon)?;
            let elif_body = self.parse_block()?;
            elif_branches.push((elif_condition, elif_body));
        }
        
        let else_branch = if self.consume_if(Token::KwElse).is_some() {
            self.expect_token(Token::Colon)?;
            Some(self.parse_block()?)
        } else {
            None
        };
        
        Ok(Stmt::If {
            condition,
            then_branch,
            elif_branches,
            else_branch,
            span: self.merge_span(start_span),
        })
    }

    /// Parse expression
    fn parse_expression(&mut self) -> Result<Expr> {
        self.parse_ternary()
    }

    /// Parse ternary expression: a if condition else b
    fn parse_ternary(&mut self) -> Result<Expr> {
        let condition = self.parse_or()?;
        
        if self.consume_if(Token::KwIf).is_some() {
            let then_expr = self.parse_or()?;
            self.expect_token(Token::KwElse)?;
            let else_expr = self.parse_ternary()?;
            
            return Ok(Expr::Ternary {
                condition: Box::new(condition),
                then_expr: Box::new(then_expr),
                else_expr: Box::new(else_expr),
                span: self.current_span,
            });
        }
        
        Ok(condition)
    }

    /// Parse logical OR
    fn parse_or(&mut self) -> Result<Expr> {
        let mut left = self.parse_and()?;
        
        while self.consume_if(Token::Or).is_some() {
            let right = self.parse_and()?;
            left = Expr::Binary {
                left: Box::new(left),
                op: BinaryOp::Or,
                right: Box::new(right),
                span: self.current_span,
            };
        }
        
        Ok(left)
    }

    /// Parse logical AND
    fn parse_and(&mut self) -> Result<Expr> {
        let mut left = self.parse_comparison()?;
        
        while self.consume_if(Token::And).is_some() {
            let right = self.parse_comparison()?;
            left = Expr::Binary {
                left: Box::new(left),
                op: BinaryOp::And,
                right: Box::new(right),
                span: self.current_span,
            };
        }
        
        Ok(left)
    }

    /// Parse comparison operators
    fn parse_comparison(&mut self) -> Result<Expr> {
        let mut left = self.parse_term()?;
        
        while let Some(op) = self.parse_comparison_op() {
            let right = self.parse_term()?;
            left = Expr::Binary {
                left: Box::new(left),
                op,
                right: Box::new(right),
                span: self.current_span,
            };
        }
        
        Ok(left)
    }

    /// Parse comparison operator
    fn parse_comparison_op(&mut self) -> Option<BinaryOp> {
        if let Some(token) = self.tokens.peek() {
            let op = match token {
                Token::Eq => Some(BinaryOp::Eq),
                Token::Neq => Some(BinaryOp::Neq),
                Token::Gt => Some(BinaryOp::Gt),
                Token::Lt => Some(BinaryOp::Lt),
                Token::Gte => Some(BinaryOp::Gte),
                Token::Lte => Some(BinaryOp::Lte),
                _ => None,
            };
            if op.is_some() {
                self.tokens.next();
            }
            op
        } else {
            None
        }
    }

    /// Parse block of statements (indented or braced)
    fn parse_block(&mut self) -> Result<Vec<Stmt>> {
        // For now, parse single statement or look for brace
        // In full implementation, this would handle indentation
        if self.consume_if(Token::LBrace).is_some() {
            let mut statements = Vec::new();
            while self.tokens.peek() != Some(&Token::RBrace) {
                statements.push(self.parse_statement()?);
            }
            self.expect_token(Token::RBrace)?;
            Ok(statements)
        } else {
            // Single statement
            Ok(vec![self.parse_statement()?])
        }
    }

    // --- Utility methods ---
    
    fn peek_token(&mut self) -> Result<&Token> {
        self.tokens.peek().ok_or_else(|| self.error("Unexpected end of input"))
    }
    
    fn next_token(&mut self) -> Result<Token> {
        self.tokens.next().ok_or_else(|| self.error("Unexpected end of input"))
    }
    
    fn expect_token(&mut self, expected: Token) -> Result<Token> {
        let token = self.next_token()?;
        if token == expected {
            Ok(token)
        } else {
            Err(self.error(&format!("Expected {:?}, found {:?}", expected, token)))
        }
    }
    
    fn expect_identifier(&mut self) -> Result<Token> {
        let token = self.next_token()?;
        if matches!(token, Token::Identifier(_)) {
            Ok(token)
        } else {
            Err(self.error(&format!("Expected identifier, found {:?}", token)))
        }
    }
    
    fn consume_if(&mut self, expected: Token) -> Option<Token> {
        if self.tokens.peek() == Some(&expected) {
            self.tokens.next()
        } else {
            None
        }
    }
    
    fn error(&self, message: &str) -> anyhow::Error {
        anyhow!("{} at position {:?}", message, self.current_span)
    }
    
    fn merge_span(&self, start: Span) -> Span {
        Span::new(start.start, self.current_span.end, start.line, start.column)
    }

    // Placeholder methods for remaining grammar rules
    fn parse_parameters(&mut self) -> Result<Vec<Parameter>> {
        // Simplified implementation
        let mut params = Vec::new();
        while self.tokens.peek() != Some(&Token::RParen) {
            if let Token::Identifier(name) = self.expect_identifier()? {
                let type_annotation = if self.consume_if(Token::Colon).is_some() {
                    Some(self.parse_type()?)
                } else {
                    None
                };
                
                let default_value = if self.consume_if(Token::Assign).is_some() {
                    Some(self.parse_expression()?)
                } else {
                    None
                };
                
                params.push(Parameter {
                    name,
                    type_annotation,
                    default_value,
                    is_varargs: false,
                    is_kwargs: false,
                    span: self.current_span,
                });
                
                if self.tokens.peek() != Some(&Token::RParen) {
                    self.expect_token(Token::Comma)?;
                }
            }
        }
        Ok(params)
    }
    
    fn parse_type(&mut self) -> Result<Type> {
        // Simplified type parsing
        if let Token::Identifier(name) = self.expect_identifier()? {
            match name.as_str() {
                "int" => Ok(Type::Int),
                "float" => Ok(Type::Float),
                "bool" => Ok(Type::Bool),
                "str" => Ok(Type::Str),
                "list" => {
                    if self.consume_if(Token::LBracket).is_some() {
                        let inner = self.parse_type()?;
                        self.expect_token(Token::RBracket)?;
                        Ok(Type::List(Box::new(inner)))
                    } else {
                        Ok(Type::List(Box::new(Type::Any)))
                    }
                }
                _ => Ok(Type::Custom(name)),
            }
        } else {
            Err(self.error("Expected type name"))
        }
    }
    
    fn parse_term(&mut self) -> Result<Expr> {
        // Simplified - just parse primary expressions for now
        self.parse_primary()
    }
    
    fn parse_primary(&mut self) -> Result<Expr> {
        let token = self.next_token()?;
        match token {
            Token::Int(n) => Ok(Expr::Int(n, self.current_span)),
            Token::Float(n) => Ok(Expr::Float(n, self.current_span)),
            Token::StringLit(s) => Ok(Expr::String(s, self.current_span)),
            Token::True => Ok(Expr::Bool(true, self.current_span)),
            Token::False => Ok(Expr::Bool(false, self.current_span)),
            Token::None => Ok(Expr::None(self.current_span)),
            Token::Identifier(name) => Ok(Expr::Identifier(name, self.current_span)),
            Token::LParen => {
                let expr = self.parse_expression()?;
                self.expect_token(Token::RParen)?;
                Ok(expr)
            }
            _ => Err(self.error(&format!("Unexpected token: {:?}", token))),
        }
    }
    
    fn parse_expression_list(&mut self, end: Token) -> Result<Vec<Expr>> {
        let mut exprs = Vec::new();
        while self.tokens.peek() != Some(&end) {
            exprs.push(self.parse_expression()?);
            if self.tokens.peek() != Some(&end) {
                self.expect_token(Token::Comma)?;
            }
        }
        Ok(exprs)
    }

    // Placeholder methods for other statement types
    fn parse_while_statement(&mut self) -> Result<Stmt> {
        todo!("Implement while statement parsing")
    }
    
    fn parse_for_statement(&mut self) -> Result<Stmt> {
        todo!("Implement for statement parsing")
    }
    
    fn parse_return_statement(&mut self) -> Result<Stmt> {
        todo!("Implement return statement parsing")
    }
    
    fn parse_break_statement(&mut self) -> Result<Stmt> {
        todo!("Implement break statement parsing")
    }
    
    fn parse_continue_statement(&mut self) -> Result<Stmt> {
        todo!("Implement continue statement parsing")
    }
    
    fn parse_import_statement(&mut self) -> Result<Stmt> {
        todo!("Implement import statement parsing")
    }
    
    fn parse_extern_statement(&mut self) -> Result<Stmt> {
        todo!("Implement extern statement parsing")
    }
    
    fn parse_export_statement(&mut self) -> Result<Stmt> {
        todo!("Implement export statement parsing")
    }
    
    fn parse_expression_statement(&mut self) -> Result<Stmt> {
        todo!("Implement expression statement parsing")
    }
}