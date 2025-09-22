//! COMPLETE recursive descent parser for TauraroLang - Builds AST from token stream
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
        write!(f, "Parse error: {} (found {:?}, expected {:?})", 
               self.message, self.found, self.expected)
    }
}

impl std::error::Error for ParseError {}

/// Main parser structure
pub struct Parser {
    tokens: Peekable<IntoIter<Token>>,
    current_span: Span,
    current_token: Option<Token>,
}

impl Parser {
    pub fn new(tokens: Vec<Token>) -> Self {
        let mut iter = tokens.into_iter().peekable();
        let current_token = iter.next();
        Self {
            tokens: iter,
            current_span: Span::unknown(),
            current_token,
        }
    }

    /// Main entry point - parse entire program with implicit main support
    pub fn parse_with_implicit_main(&mut self) -> Result<Program> {
        let mut statements = Vec::new();
        let mut has_explicit_main = false;
        
        // First pass: check if there's an explicit main function
        let tokens_clone = self.tokens.clone();
        let mut check_parser = Parser { 
            tokens: tokens_clone, 
            current_span: Span::unknown(),
            current_token: self.current_token.clone(),
        };
        
        while check_parser.current_token.is_some() {
            if let Ok(Stmt::Function { name, .. }) = check_parser.parse_statement() {
                if name == "main" {
                    has_explicit_main = true;
                    break;
                }
            }
            check_parser.advance()?;
        }
        
        // Parse all statements
        while self.current_token.is_some() {
            if let Ok(stmt) = self.parse_statement() {
                statements.push(stmt);
            }
            self.advance()?;
        }
        
        // Add implicit main function if no explicit main exists and we have top-level code
        if !has_explicit_main && !statements.is_empty() && !self.is_only_function_defs(&statements) {
            statements = vec![Stmt::Function {
                name: "main".to_string(),
                parameters: Vec::new(),
                return_type: Some(Type::Int),
                body: statements,
                span: Span::unknown(),
                is_async: false,
                is_export: false,
            }];
        }
        
        Ok(Program::new(statements))
    }
    
    /// Check if statements contain only function/class definitions
    fn is_only_function_defs(&self, statements: &[Stmt]) -> bool {
        statements.iter().all(|stmt| {
            matches!(stmt, Stmt::Function { .. } | Stmt::Class { .. })
        })
    }

    /// Parse a statement
    fn parse_statement(&mut self) -> Result<Stmt> {
        let token = self.current_token.clone().ok_or_else(|| self.error("Unexpected end of input"))?;
        
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
        let is_async = if let Token::KwAsync = self.current_token.as_ref().unwrap() {
            self.advance()?;
            true
        } else {
            false
        };
        
        self.expect_token(Token::KwFunc)?;
        
        let name = if let Some(Token::Identifier(name)) = self.current_token.take() {
            self.advance()?;
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
        
        let name = if let Some(Token::Identifier(name)) = self.current_token.take() {
            self.advance()?;
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
        self.parse_assignment()
    }

    /// Parse assignment expression
    fn parse_assignment(&mut self) -> Result<Expr> {
        let expr = self.parse_ternary()?;
        
        if let Some(op) = self.parse_assignment_op() {
            let value = self.parse_assignment()?;
            Ok(Expr::Binary {
                left: Box::new(expr),
                op,
                right: Box::new(value),
                span: self.current_span,
            })
        } else {
            Ok(expr)
        }
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
        if let Some(token) = self.current_token.as_ref() {
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
                self.advance().ok()?;
            }
            op
        } else {
            None
        }
    }

    /// Parse assignment operator
    fn parse_assignment_op(&mut self) -> Option<BinaryOp> {
        if let Some(token) = self.current_token.as_ref() {
            let op = match token {
                Token::PlusEq => Some(BinaryOp::Add),
                Token::MinusEq => Some(BinaryOp::Sub),
                Token::StarEq => Some(BinaryOp::Mul),
                Token::SlashEq => Some(BinaryOp::Div),
                Token::PercentEq => Some(BinaryOp::Mod),
                Token::PowerEq => Some(BinaryOp::Pow),
                Token::FloorDivEq => Some(BinaryOp::FloorDiv),
                _ => None,
            };
            if op.is_some() {
                self.advance().ok()?;
            }
            op
        } else {
            None
        }
    }

    /// Parse block of statements (indented or braced)
    fn parse_block(&mut self) -> Result<Vec<Stmt>> {
        if self.consume_if(Token::LBrace).is_some() {
            let mut statements = Vec::new();
            while self.current_token != Some(Token::RBrace) {
                statements.push(self.parse_statement()?);
                self.advance()?;
            }
            self.expect_token(Token::RBrace)?;
            Ok(statements)
        } else {
            // Single statement or look for indentation
            Ok(vec![self.parse_statement()?])
        }
    }

    /// Parse term (addition/subtraction)
    fn parse_term(&mut self) -> Result<Expr> {
        let mut left = self.parse_factor()?;
        
        while let Some(op) = self.parse_term_op() {
            let right = self.parse_factor()?;
            left = Expr::Binary {
                left: Box::new(left),
                op,
                right: Box::new(right),
                span: self.current_span,
            };
        }
        
        Ok(left)
    }

    /// Parse term operator
    fn parse_term_op(&mut self) -> Option<BinaryOp> {
        if let Some(token) = self.current_token.as_ref() {
            let op = match token {
                Token::Plus => Some(BinaryOp::Add),
                Token::Minus => Some(BinaryOp::Sub),
                _ => None,
            };
            if op.is_some() {
                self.advance().ok()?;
            }
            op
        } else {
            None
        }
    }

    /// Parse factor (multiplication/division)
    fn parse_factor(&mut self) -> Result<Expr> {
        let mut left = self.parse_power()?;
        
        while let Some(op) = self.parse_factor_op() {
            let right = self.parse_power()?;
            left = Expr::Binary {
                left: Box::new(left),
                op,
                right: Box::new(right),
                span: self.current_span,
            };
        }
        
        Ok(left)
    }

    /// Parse factor operator
    fn parse_factor_op(&mut self) -> Option<BinaryOp> {
        if let Some(token) = self.current_token.as_ref() {
            let op = match token {
                Token::Star => Some(BinaryOp::Mul),
                Token::Slash => Some(BinaryOp::Div),
                Token::Percent => Some(BinaryOp::Mod),
                Token::FloorDiv => Some(BinaryOp::FloorDiv),
                _ => None,
            };
            if op.is_some() {
                self.advance().ok()?;
            }
            op
        } else {
            None
        }
    }

    /// Parse power (exponentiation)
    fn parse_power(&mut self) -> Result<Expr> {
        let mut left = self.parse_unary()?;
        
        while self.consume_if(Token::Power).is_some() {
            let right = self.parse_unary()?;
            left = Expr::Binary {
                left: Box::new(left),
                op: BinaryOp::Pow,
                right: Box::new(right),
                span: self.current_span,
            };
        }
        
        Ok(left)
    }

    /// Parse unary expression
    fn parse_unary(&mut self) -> Result<Expr> {
        if let Some(op) = self.parse_unary_op() {
            let expr = self.parse_unary()?;
            Ok(Expr::Unary {
                op,
                expr: Box::new(expr),
                span: self.current_span,
            })
        } else {
            self.parse_primary()
        }
    }

    /// Parse unary operator
    fn parse_unary_op(&mut self) -> Option<UnaryOp> {
        if let Some(token) = self.current_token.as_ref() {
            let op = match token {
                Token::Plus => Some(UnaryOp::Plus),
                Token::Minus => Some(UnaryOp::Minus),
                Token::Not => Some(UnaryOp::Not),
                Token::BitNot => Some(UnaryOp::BitNot),
                _ => None,
            };
            if op.is_some() {
                self.advance().ok()?;
            }
            op
        } else {
            None
        }
    }

    /// Parse primary expression
    fn parse_primary(&mut self) -> Result<Expr> {
        let token = self.current_token.take().ok_or_else(|| self.error("Unexpected end of input"))?;
        
        match token {
            Token::Int(n) => Ok(Expr::Int(n, self.current_span)),
            Token::Float(n) => Ok(Expr::Float(n, self.current_span)),
            Token::StringLit(s) => Ok(Expr::String(s, self.current_span)),
            Token::True => Ok(Expr::Bool(true, self.current_span)),
            Token::False => Ok(Expr::Bool(false, self.current_span)),
            Token::None => Ok(Expr::None(self.current_span)),
            Token::Identifier(name) => {
                // Check if it's a function call or variable access
                if self.current_token == Some(Token::LParen) {
                    self.parse_function_call(name)
                } else {
                    Ok(Expr::Identifier(name, self.current_span))
                }
            }
            Token::LParen => {
                let expr = self.parse_expression()?;
                self.expect_token(Token::RParen)?;
                Ok(expr)
            }
            Token::LBracket => self.parse_list(),
            Token::LBrace => self.parse_dict(),
            _ => Err(self.error(&format!("Unexpected token: {:?}", token))),
        }
    }

    /// Parse function call
    fn parse_function_call(&mut self, name: String) -> Result<Expr> {
        self.expect_token(Token::LParen)?;
        let arguments = self.parse_expression_list(Token::RParen)?;
        self.expect_token(Token::RParen)?;
        
        Ok(Expr::Call {
            callee: Box::new(Expr::Identifier(name, self.current_span)),
            arguments,
            span: self.current_span,
        })
    }

    /// Parse list literal
    fn parse_list(&mut self) -> Result<Expr> {
        let elements = self.parse_expression_list(Token::RBracket)?;
        self.expect_token(Token::RBracket)?;
        Ok(Expr::List(elements, self.current_span))
    }

    /// Parse dictionary literal
    fn parse_dict(&mut self) -> Result<Expr> {
        let mut pairs = Vec::new();
        
        while self.current_token != Some(Token::RBrace) {
            let key = self.parse_expression()?;
            self.expect_token(Token::Colon)?;
            let value = self.parse_expression()?;
            pairs.push((key, value));
            
            if self.current_token != Some(Token::RBrace) {
                self.expect_token(Token::Comma)?;
            }
        }
        
        self.expect_token(Token::RBrace)?;
        Ok(Expr::Dict(pairs, self.current_span))
    }

    /// Parse expression list until end token
    fn parse_expression_list(&mut self, end: Token) -> Result<Vec<Expr>> {
        let mut exprs = Vec::new();
        
        while self.current_token != Some(end.clone()) {
            exprs.push(self.parse_expression()?);
            if self.current_token != Some(end.clone()) {
                self.expect_token(Token::Comma)?;
            }
        }
        
        Ok(exprs)
    }

    /// Parse parameters
    fn parse_parameters(&mut self) -> Result<Vec<Parameter>> {
        let mut params = Vec::new();
        
        while self.current_token != Some(Token::RParen) {
            let name = if let Some(Token::Identifier(name)) = self.current_token.take() {
                self.advance()?;
                name
            } else {
                return Err(self.error("Expected parameter name"));
            };
            
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
            
            if self.current_token != Some(Token::RParen) {
                self.expect_token(Token::Comma)?;
            }
        }
        
        Ok(params)
    }

    /// Parse type annotation
    fn parse_type(&mut self) -> Result<Type> {
        let token = self.current_token.take().ok_or_else(|| self.error("Expected type name"))?;
        
        match token {
            Token::Identifier(name) => {
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
                    "dict" => {
                        if self.consume_if(Token::LBracket).is_some() {
                            let key_type = self.parse_type()?;
                            self.expect_token(Token::Comma)?;
                            let value_type = self.parse_type()?;
                            self.expect_token(Token::RBracket)?;
                            Ok(Type::Dict(Box::new(key_type), Box::new(value_type)))
                        } else {
                            Ok(Type::Dict(Box::new(Type::Any), Box::new(Type::Any)))
                        }
                    }
                    "tuple" => {
                        if self.consume_if(Token::LBracket).is_some() {
                            let mut types = Vec::new();
                            while self.current_token != Some(Token::RBracket) {
                                types.push(self.parse_type()?);
                                if self.current_token != Some(Token::RBracket) {
                                    self.expect_token(Token::Comma)?;
                                }
                            }
                            self.expect_token(Token::RBracket)?;
                            Ok(Type::Tuple(types))
                        } else {
                            Ok(Type::Tuple(Vec::new()))
                        }
                    }
                    "any" => Ok(Type::Any),
                    _ => Ok(Type::Custom(name)),
                }
            }
            _ => Err(self.error("Expected type name")),
        }
    }

    // --- COMPLETE statement parsers ---

    fn parse_while_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwWhile)?;
        
        let condition = self.parse_expression()?;
        self.expect_token(Token::Colon)?;
        let body = self.parse_block()?;
        
        Ok(Stmt::While {
            condition,
            body,
            span: self.merge_span(start_span),
        })
    }

    fn parse_for_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwFor)?;
        
        let variable = if let Some(Token::Identifier(name)) = self.current_token.take() {
            self.advance()?;
            name
        } else {
            return Err(self.error("Expected variable name in for loop"));
        };
        
        self.expect_token(Token::KwIn)?;
        let iterable = self.parse_expression()?;
        self.expect_token(Token::Colon)?;
        let body = self.parse_block()?;
        
        Ok(Stmt::For {
            variable,
            iterable,
            body,
            span: self.merge_span(start_span),
        })
    }

    fn parse_return_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwReturn)?;
        
        let value = if self.current_token.as_ref().map_or(false, |t| t.can_start_expression()) {
            Some(self.parse_expression()?)
        } else {
            None
        };
        
        Ok(Stmt::Return {
            value,
            span: self.merge_span(start_span),
        })
    }

    fn parse_break_statement(&mut self) -> Result<Stmt> {
        let span = self.current_span;
        self.expect_token(Token::KwBreak)?;
        Ok(Stmt::Break(span))
    }

    fn parse_continue_statement(&mut self) -> Result<Stmt> {
        let span = self.current_span;
        self.expect_token(Token::KwContinue)?;
        Ok(Stmt::Continue(span))
    }

    fn parse_import_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwImport)?;
        
        let module = if let Some(Token::Identifier(name)) = self.current_token.take() {
            self.advance()?;
            name
        } else if let Some(Token::StringLit(s)) = self.current_token.take() {
            self.advance()?;
            s
        } else {
            return Err(self.error("Expected module name or string"));
        };
        
        let alias = if self.consume_if(Token::KwAs).is_some() {
            Some(if let Some(Token::Identifier(name)) = self.current_token.take() {
                self.advance()?;
                name
            } else {
                return Err(self.error("Expected alias name"));
            })
        } else {
            None
        };
        
        Ok(Stmt::Import {
            module,
            alias,
            span: self.merge_span(start_span),
        })
    }

    fn parse_extern_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwExtern)?;
        
        let library = if let Some(Token::StringLit(s)) = self.current_token.take() {
            self.advance()?;
            s
        } else {
            return Err(self.error("Expected library path string"));
        };
        
        Ok(Stmt::Extern {
            library,
            span: self.merge_span(start_span),
        })
    }

    fn parse_export_statement(&mut self) -> Result<Stmt> {
        let start_span = self.current_span;
        self.expect_token(Token::KwExport)?;
        
        let stmt = self.parse_statement()?;
        
        // Mark the statement as exported
        match stmt {
            Stmt::Function { name, parameters, return_type, body, span, is_async, .. } => {
                Ok(Stmt::Function {
                    name,
                    parameters,
                    return_type,
                    body,
                    span: self.merge_span(start_span),
                    is_async,
                    is_export: true,
                })
            }
            Stmt::Class { name, bases, body, span, .. } => {
                Ok(Stmt::Class {
                    name,
                    bases,
                    body,
                    span: self.merge_span(start_span),
                    is_export: true,
                })
            }
            _ => Err(self.error("Only functions and classes can be exported")),
        }
    }

    fn parse_expression_statement(&mut self) -> Result<Stmt> {
        let expr = self.parse_expression()?;
        Ok(Stmt::Expression(expr, self.current_span))
    }

    // --- Utility methods ---
    
    fn advance(&mut self) -> Result<()> {
        self.current_token = self.tokens.next();
        Ok(())
    }
    
    fn expect_token(&mut self, expected: Token) -> Result<Token> {
        if let Some(current) = self.current_token.take() {
            if current == expected {
                self.advance()?;
                Ok(current)
            } else {
                Err(self.error(&format!("Expected {:?}, found {:?}", expected, current)))
            }
        } else {
            Err(self.error(&format!("Expected {:?}, found end of input", expected)))
        }
    }
    
    fn consume_if(&mut self, expected: Token) -> Option<Token> {
        if self.current_token == Some(expected.clone()) {
            self.current_token.take()
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
}