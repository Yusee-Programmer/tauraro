use crate::lexer::{Token, TokenInfo};
use crate::ast::*;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum ParseError {
    #[error("Unexpected token: expected {expected}, found {found}")]
    UnexpectedToken { expected: String, found: String },
    #[error("Unexpected end of input")]
    UnexpectedEof,
    #[error("Invalid syntax: {message}")]
    InvalidSyntax { message: String },
    #[error("Indentation error: {message}")]
    IndentationError { message: String },
}

pub struct Parser {
    tokens: Vec<TokenInfo>,
    current: usize,
}

impl Parser {
    pub fn new(mut tokens: Vec<TokenInfo>) -> Self {
        // Ensure there's always an EOF token at the end
        if tokens.is_empty() || !matches!(tokens.last().unwrap().token, Token::Eof) {
            tokens.push(TokenInfo {
                token: Token::Eof,
                span: (0, 0),
                line: 1,
                column: 1,
            });
        }
        Self { tokens, current: 0 }
    }

    pub fn parse(&mut self) -> Result<Program, ParseError> {
        let mut statements = Vec::new();
        
        while !self.is_at_end() && !self.check(&Token::Eof) {
            // Skip newlines at the top level
            if self.check(&Token::Newline) {
                self.advance();
                continue;
            }
            statements.push(self.statement()?);
        }
        
        Ok(Program { statements })
    }

    /// Parse with implicit main function support for scripts
    pub fn parse_with_implicit_main(&mut self) -> Result<Program, ParseError> {
        let mut statements = Vec::new();
        let mut main_body = Vec::new();
        let mut has_main_function = false;
        
        while !self.is_at_end() && !self.check(&Token::Eof) {
            // Skip newlines at the top level
            if self.check(&Token::Newline) {
                self.advance();
                continue;
            }
            
            let stmt = self.statement()?;
            
            // Check if this is a main function definition
            if let Statement::FunctionDef { name, .. } = &stmt {
                if name == "main" {
                    has_main_function = true;
                }
            }
            
            // If it's a top-level expression or variable definition, add to main body
            match &stmt {
                Statement::Expression(_) | Statement::VariableDef { .. } => {
                    main_body.push(stmt);
                }
                _ => {
                    statements.push(stmt);
                }
            }
        }
        
        // If we have main body statements and no explicit main function, create one
        if !main_body.is_empty() && !has_main_function {
            let main_function = Statement::FunctionDef {
                name: "main".to_string(),
                params: Vec::new(),
                return_type: None,
                body: main_body,
                is_async: false,
                decorators: Vec::new(),
            };
            statements.push(main_function);
        } else {
            // Add main body statements to the program
            statements.extend(main_body);
        }
        
        Ok(Program { statements })
    }

    fn statement(&mut self) -> Result<Statement, ParseError> {
        match &self.peek().token {
            Token::KwFunc | Token::KwAsync => self.function_def(),
            Token::KwClass => self.class_def(),
            Token::KwIf => self.if_statement(),
            Token::KwFor => self.for_statement(),
            Token::KwWhile => self.while_statement(),
            Token::KwReturn => self.return_statement(),
            Token::KwBreak => self.break_statement(),
            Token::KwContinue => self.continue_statement(),
            Token::KwImport => self.import_statement(),
            Token::KwFrom => self.from_import_statement(),
            Token::KwExtern => self.extern_statement(),
            Token::KwExport => self.export_statement(),
            Token::KwTry => self.try_statement(),
            Token::KwRaise => self.raise_statement(),
            Token::KwWith => self.with_statement(),
            Token::KwMatch => self.match_statement(),
            Token::KwDel => self.del_statement(),
            Token::KwAssert => self.assert_statement(),
            Token::KwGlobal => self.global_statement(),
            Token::KwNonlocal => self.nonlocal_statement(),
            Token::KwPass => self.pass_statement(),
            Token::At => self.decorated_statement(),
            _ => {
                // Try expression statement or variable definition
                let expr = self.expression()?;
                if self.match_token(&[Token::Assign]) {
                    self.variable_def(expr)
                } else {
                    // Optional semicolon or newline for expression statements
                    self.match_token(&[Token::Semicolon, Token::Newline]);
                    Ok(Statement::Expression(expr))
                }
            }
        }
    }

    fn function_def(&mut self) -> Result<Statement, ParseError> {
        let is_async = self.match_token(&[Token::KwAsync]);
        self.consume(Token::KwFunc, "Expected 'func', 'def', or 'aiki'")?;
        
        let name = self.consume_identifier()?;
        
        // Optional parentheses for parameters
        let mut params = Vec::new();
        if self.match_token(&[Token::LParen]) {
            if !self.check(&Token::RParen) {
                loop {
                    let param_name = self.consume_identifier()?;
                    let type_annotation = if self.match_token(&[Token::Colon]) {
                        Some(self.type_annotation()?)
                    } else {
                        None
                    };
                    let default = if self.match_token(&[Token::Assign]) {
                        Some(self.expression()?)
                    } else {
                        None
                    };
                    
                    params.push(Param {
                        name: param_name,
                        type_annotation,
                        default,
                        kind: ParamKind::Positional,
                    });
                    
                    if !self.match_token(&[Token::Comma]) {
                        break;
                    }
                }
            }
            self.consume(Token::RParen, "Expected ')' after parameters")?;
        }
        
        let return_type = if self.match_token(&[Token::Arrow]) {
            Some(self.type_annotation()?)
        } else {
            None
        };
        
        self.consume(Token::Colon, "Expected ':' after function signature")?;
        let body = self.block()?;
        
        Ok(Statement::FunctionDef {
            name,
            params,
            return_type,
            body,
            is_async,
            decorators: Vec::new(),
        })
    }

    fn class_def(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwClass, "Expected 'class' or 'iri'")?;
        let name = self.consume_identifier()?;
        
        let mut bases = Vec::new();
        if self.match_token(&[Token::LParen]) {
            if !self.check(&Token::RParen) {
                loop {
                    bases.push(self.expression()?);
                    if !self.match_token(&[Token::Comma]) {
                        break;
                    }
                }
            }
            self.consume(Token::RParen, "Expected ')' after base classes")?;
        }
        
        self.consume(Token::Colon, "Expected ':' after class declaration")?;
        let body = self.block()?;
        
        Ok(Statement::ClassDef { 
            name, 
            bases, 
            body,
            decorators: Vec::new(),
            metaclass: None,
        })
    }

    fn if_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwIf, "Expected 'if' or 'idan'")?;
        let condition = self.expression()?;
        self.consume(Token::Colon, "Expected ':' after if condition")?;
        let then_branch = self.block()?;
        
        let mut elif_branches = Vec::new();
        while self.match_token(&[Token::KwElif]) {
            let elif_condition = self.expression()?;
            self.consume(Token::Colon, "Expected ':' after elif condition")?;
            let elif_body = self.block()?;
            elif_branches.push((elif_condition, elif_body));
        }
        
        let else_branch = if self.match_token(&[Token::KwElse]) {
            self.consume(Token::Colon, "Expected ':' after else")?;
            Some(self.block()?)
        } else {
            None
        };
        
        Ok(Statement::If {
            condition,
            then_branch,
            elif_branches,
            else_branch,
        })
    }

    fn for_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwFor, "Expected 'for' or 'duk'")?;
        let variable = self.consume_identifier()?;
        self.consume(Token::KwIn, "Expected 'in' or 'cikin' after for variable")?;
        let iterable = self.expression()?;
        self.consume(Token::Colon, "Expected ':' after for clause")?;
        let body = self.block()?;
        
        Ok(Statement::For {
            variable,
            iterable,
            body,
            else_branch: None,
        })
    }

    fn while_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwWhile, "Expected 'while' or 'yayinda'")?;
        let condition = self.expression()?;
        self.consume(Token::Colon, "Expected ':' after while condition")?;
        let body = self.block()?;
        
        Ok(Statement::While { 
            condition, 
            body,
            else_branch: None,
        })
    }

    fn try_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwTry, "Expected 'try' or 'gwada'")?;
        self.consume(Token::Colon, "Expected ':' after try")?;
        let body = self.block()?;
        
        let mut except_handlers = Vec::new();
        while self.match_token(&[Token::KwExcept]) {
            let exception_type = if !self.check(&Token::Colon) {
                Some(self.expression()?)
            } else {
                None
            };
            
            let name = if self.match_token(&[Token::KwAs]) {
                Some(self.consume_identifier()?)
            } else {
                None
            };
            
            self.consume(Token::Colon, "Expected ':' after except clause")?;
            let handler_body = self.block()?;
            
            except_handlers.push(ExceptHandler {
                exception_type,
                name,
                body: handler_body,
            });
        }
        
        let finally = if self.match_token(&[Token::KwFinally]) {
            self.consume(Token::Colon, "Expected ':' after finally")?;
            Some(self.block()?)
        } else {
            None
        };
        
        Ok(Statement::Try {
            body,
            except_handlers,
            finally,
            else_branch: None,
        })
    }

    fn match_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwMatch, "Expected 'match' or 'daidaita'")?;
        let value = self.expression()?;
        self.consume(Token::Colon, "Expected ':' after match expression")?;
        
        self.consume(Token::Indent, "Expected indented block after match")?;
        let mut cases = Vec::new();
        
        while !self.check(&Token::Dedent) && !self.is_at_end() {
            self.consume(Token::KwCase, "Expected 'case' or 'yanayi'")?;
            let pattern = self.pattern()?;
            
            let guard = if self.match_token(&[Token::KwIf]) {
                Some(self.expression()?)
            } else {
                None
            };
            
            self.consume(Token::Colon, "Expected ':' after case pattern")?;
            let case_body = self.block()?;
            
            cases.push(MatchCase {
                pattern,
                guard,
                body: case_body,
            });
        }
        
        self.consume(Token::Dedent, "Expected dedent after match cases")?;
        
        Ok(Statement::Match { value, cases })
    }

    fn pattern(&mut self) -> Result<Pattern, ParseError> {
        match &self.peek().token {
            Token::Identifier(name) if name == "_" => {
                self.advance();
                Ok(Pattern::Wildcard)
            }
            Token::Identifier(_) => {
                let name = self.consume_identifier()?;
                Ok(Pattern::Variable(name))
            }
            Token::LParen => {
                self.advance();
                let mut patterns = Vec::new();
                if !self.check(&Token::RParen) {
                    loop {
                        patterns.push(self.pattern()?);
                        if !self.match_token(&[Token::Comma]) {
                            break;
                        }
                    }
                }
                self.consume(Token::RParen, "Expected ')' after tuple pattern")?;
                Ok(Pattern::Tuple(patterns))
            }
            _ => {
                let expr = self.primary()?;
                Ok(Pattern::Literal(expr))
            }
        }
    }

    fn expression(&mut self) -> Result<Expr, ParseError> {
        self.assignment()
    }

    fn assignment(&mut self) -> Result<Expr, ParseError> {
        let expr = self.or()?;
        
        if self.match_token(&[
            Token::PlusEq, Token::MinusEq, Token::StarEq,
            Token::SlashEq, Token::PercentEq, Token::PowerEq, Token::FloorDivEq,
        ]) {
            let op = self.previous().token.clone();
            let value = self.assignment()?;
            
            return match expr {
                Expr::Identifier(name) => Ok(Expr::BinaryOp {
                    left: Box::new(Expr::Identifier(name)),
                    op: match op {
                        Token::PlusEq => BinaryOp::Add,
                        Token::MinusEq => BinaryOp::Sub,
                        Token::StarEq => BinaryOp::Mul,
                        Token::SlashEq => BinaryOp::Div,
                        Token::PercentEq => BinaryOp::Mod,
                        Token::PowerEq => BinaryOp::Pow,
                        Token::FloorDivEq => BinaryOp::FloorDiv,
                        _ => unreachable!(),
                    },
                    right: Box::new(value),
                }),
                _ => Err(ParseError::InvalidSyntax {
                    message: "Invalid assignment target".to_string(),
                }),
            };
        }
        
        Ok(expr)
    }

    fn or(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.and()?;
        
        while self.match_token(&[Token::Or, Token::Or]) {
            let op = BinaryOp::Or;
            let right = self.and()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn and(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.equality()?;
        
        while self.match_token(&[Token::And, Token::And]) {
            let op = BinaryOp::And;
            let right = self.equality()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn equality(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.comparison()?;
        
        while self.match_token(&[Token::Eq, Token::Neq]) {
            let op = match self.previous().token {
                Token::Eq => BinaryOp::Eq,
                Token::Neq => BinaryOp::Ne,
                _ => unreachable!(),
            };
            let right = self.comparison()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn comparison(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.term()?;
        
        while self.match_token(&[Token::Gt, Token::Gte, Token::Lt, Token::Lte]) {
            let op = match self.previous().token {
                Token::Gt => BinaryOp::Gt,
                Token::Gte => BinaryOp::Ge,
                Token::Lt => BinaryOp::Lt,
                Token::Lte => BinaryOp::Le,
                _ => unreachable!(),
            };
            let right = self.term()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn term(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.factor()?;
        
        while self.match_token(&[Token::Plus, Token::Minus]) {
            let op = match self.previous().token {
                Token::Plus => BinaryOp::Add,
                Token::Minus => BinaryOp::Sub,
                _ => unreachable!(),
            };
            let right = self.factor()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn factor(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.unary()?;
        
        while self.match_token(&[Token::Star, Token::Slash, Token::Percent, Token::FloorDiv]) {
            let op = match self.previous().token {
                Token::Star => BinaryOp::Mul,
                Token::Slash => BinaryOp::Div,
                Token::Percent => BinaryOp::Mod,
                Token::FloorDiv => BinaryOp::FloorDiv,
                _ => unreachable!(),
            };
            let right = self.unary()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn unary(&mut self) -> Result<Expr, ParseError> {
        if self.match_token(&[Token::Not, Token::Minus, Token::Plus, Token::BitNot]) {
            let op = match self.previous().token {
                Token::Not => UnaryOp::Not,
                Token::Minus => UnaryOp::USub,
                Token::Plus => UnaryOp::UAdd,
                Token::BitNot => UnaryOp::Invert,
                _ => unreachable!(),
            };
            let expr = self.unary()?;
            return Ok(Expr::UnaryOp {
                op,
                operand: Box::new(expr),
            });
        }
        
        self.call()
    }

    fn call(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.primary()?;
        
        loop {
            if self.match_token(&[Token::LParen]) {
                expr = self.finish_call(expr)?;
            } else if self.match_token(&[Token::Dot]) {
                let attr = self.consume_identifier()?;
                expr = Expr::Attribute {
                    object: Box::new(expr),
                    name: attr,
                };
            } else if self.match_token(&[Token::LBracket]) {
                let index = self.expression()?;
                self.consume(Token::RBracket, "Expected ']' after index")?;
                expr = Expr::Subscript {
                    object: Box::new(expr),
                    index: Box::new(index),
                };
            } else {
                break;
            }
        }
        
        Ok(expr)
    }

    fn primary(&mut self) -> Result<Expr, ParseError> {
        if self.match_token(&[Token::True]) {
            Ok(Expr::Literal(Literal::Bool(true)))
        } else if self.match_token(&[Token::False]) {
            Ok(Expr::Literal(Literal::Bool(false)))
        } else if self.match_token(&[Token::None]) {
            Ok(Expr::Literal(Literal::None))
        } else if let Token::Int(n) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Literal(Literal::Int(n)))
        } else if let Token::Float(n) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Literal(Literal::Float(n)))
        } else if let Token::StringLit(s) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Literal(Literal::String(s)))
        } else if let Token::Identifier(name) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Identifier(name))
        } else if self.match_token(&[Token::LParen]) {
            let expr = self.expression()?;
            self.consume(Token::RParen, "Expected ')' after expression")?;
            Ok(expr)
        } else if self.match_token(&[Token::LBracket]) {
            self.list_or_comp()
        } else if self.match_token(&[Token::LBrace]) {
            self.dict_or_set()
        } else if self.is_at_end() || self.check(&Token::Eof) {
            Err(ParseError::UnexpectedEof)
        } else {
            Err(ParseError::UnexpectedToken {
                expected: "expression".to_string(),
                found: self.peek().to_string(),
            })
        }
    }

    // Helper methods
    fn match_token(&mut self, tokens: &[Token]) -> bool {
        for token in tokens {
            if self.check(token) {
                self.advance();
                return true;
            }
        }
        false
    }

    fn check(&self, token: &Token) -> bool {
        if self.is_at_end() {
            matches!(token, Token::Eof)
        } else {
            &self.peek().token == token
        }
    }

    fn advance(&mut self) -> &TokenInfo {
        if !self.is_at_end() {
            self.current += 1;
        }
        self.previous()
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.tokens.len()
    }

    fn peek(&self) -> &TokenInfo {
        if self.is_at_end() {
            // Return the last token (should be EOF) if we're at the end
            &self.tokens[self.tokens.len() - 1]
        } else {
            &self.tokens[self.current]
        }
    }

    fn previous(&self) -> &TokenInfo {
        &self.tokens[self.current - 1]
    }

    fn consume(&mut self, token: Token, message: &str) -> Result<&TokenInfo, ParseError> {
        if self.check(&token) {
            Ok(self.advance())
        } else {
            Err(ParseError::UnexpectedToken {
                expected: token.to_string(),
                found: self.peek().to_string(),
            })
        }
    }

    fn consume_identifier(&mut self) -> Result<String, ParseError> {
        if let Token::Identifier(name) = &self.peek().token {
            let name = name.clone();
            self.advance();
            Ok(name)
        } else {
            Err(ParseError::UnexpectedToken {
                expected: "identifier".to_string(),
                found: self.peek().to_string(),
            })
        }
    }

    fn type_annotation(&mut self) -> Result<Type, ParseError> {
        match &self.peek().token {
            Token::Identifier(name) => {
                let name = name.clone();
                self.advance();
                
                // Check for generic types
                if self.match_token(&[Token::LBracket]) {
                    let mut args = Vec::new();
                    if !self.check(&Token::RBracket) {
                        loop {
                            args.push(self.type_annotation()?);
                            if !self.match_token(&[Token::Comma]) {
                                break;
                            }
                        }
                    }
                    self.consume(Token::RBracket, "Expected ']' after generic type arguments")?;
                    Ok(Type::Generic { name, args })
                } else {
                    Ok(Type::Simple(name))
                }
            }
            Token::LParen => {
                // Tuple type
                self.advance();
                let mut types = Vec::new();
                if !self.check(&Token::RParen) {
                    loop {
                        types.push(self.type_annotation()?);
                        if !self.match_token(&[Token::Comma]) {
                            break;
                        }
                    }
                }
                self.consume(Token::RParen, "Expected ')' after tuple type")?;
                Ok(Type::Tuple(types))
            }
            _ => Err(ParseError::UnexpectedToken {
                expected: "type annotation".to_string(),
                found: format!("{:?}", self.peek().token),
            }),
        }
    }

    fn finish_call(&mut self, callee: Expr) -> Result<Expr, ParseError> {
        let mut args = Vec::new();
        let mut kwargs = Vec::new();
        
        if !self.check(&Token::RParen) {
            loop {
                // Check if this is a keyword argument
                if let Token::Identifier(_) = &self.peek().token {
                    let checkpoint = self.current;
                    let name = self.consume_identifier().unwrap();
                    
                    if self.match_token(&[Token::Assign]) {
                        // It's a keyword argument
                        let value = self.expression()?;
                        kwargs.push((name, value));
                    } else {
                        // It's a positional argument, backtrack
                        self.current = checkpoint;
                        args.push(self.expression()?);
                    }
                } else {
                    args.push(self.expression()?);
                }
                
                if !self.match_token(&[Token::Comma]) {
                    break;
                }
            }
        }
        
        self.consume(Token::RParen, "Expected ')' after arguments")?;
        
        Ok(Expr::Call {
            func: Box::new(callee),
            args,
            kwargs,
        })
    }

    fn variable_def(&mut self, target: Expr) -> Result<Statement, ParseError> {
        let value = self.expression()?;
        
        // Optional type annotation
        let type_annotation = if self.match_token(&[Token::Colon]) {
            Some(self.type_annotation()?)
        } else {
            None
        };
        
        self.match_token(&[Token::Semicolon, Token::Newline]);
        
        match target {
            Expr::Identifier(name) => Ok(Statement::VariableDef {
                name,
                type_annotation,
                value: Some(value),
            }),
            _ => Err(ParseError::InvalidSyntax {
                message: "Invalid assignment target".to_string(),
            }),
        }
    }

    // Additional statement parsing methods...

    fn return_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwReturn, "Expected 'return'")?;
        let value = if !self.check(&Token::Semicolon) && !self.check(&Token::RBrace) {
            Some(self.expression()?)
        } else {
            None
        };
        Ok(Statement::Return(value))
    }

    fn break_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwBreak, "Expected 'break'")?;
        Ok(Statement::Break)
    }

    fn continue_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwContinue, "Expected 'continue'")?;
        Ok(Statement::Continue)
    }

    fn import_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwImport, "Expected 'import'")?;
        let module = self.consume_identifier()?;
        let alias = if self.match_token(&[Token::KwAs]) {
            Some(self.consume_identifier()?)
        } else {
            None
        };
        Ok(Statement::Import { module, alias })
    }

    fn from_import_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwFrom, "Expected 'from'")?;
        let module = self.consume_identifier()?;
        self.consume(Token::KwImport, "Expected 'import'")?;
        
        let mut items = Vec::new();
        loop {
            let name = self.consume_identifier()?;
            let alias = if self.match_token(&[Token::KwAs]) {
                Some(self.consume_identifier()?)
            } else {
                None
            };
            items.push((name, alias));
            
            if !self.match_token(&[Token::Comma]) {
                break;
            }
        }
        
        Ok(Statement::FromImport { module, names: items })
    }

    fn extern_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwExtern, "Expected 'extern'")?;
        let name = self.consume_identifier()?;
        self.consume(Token::Colon, "Expected ':' after extern name")?;
        let signature = self.consume_identifier()?; // For now, just consume an identifier as signature
        Ok(Statement::Extern { name, signature })
    }

    fn export_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwExport, "Expected 'export'")?;
        let name = self.consume_identifier()?;
        Ok(Statement::Export { name })
    }

    fn raise_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwRaise, "Expected 'raise'")?;
        let exception = if self.check(&Token::Newline) || self.is_at_end() {
            None
        } else {
            Some(self.expression()?)
        };
        Ok(Statement::Raise(exception))
    }

    fn with_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwWith, "Expected 'with'")?;
        let context = self.expression()?;
        let alias = if self.match_token(&[Token::KwAs]) {
            Some(self.consume_identifier()?)
        } else {
            None
        };
        self.consume(Token::Colon, "Expected ':' after with clause")?;
        let body = self.block()?;
        Ok(Statement::With { context, alias, body })
    }

    fn del_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwDel, "Expected 'del'")?;
        let target = self.expression()?;
        Ok(Statement::Del { targets: vec![target] })
    }

    fn assert_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwAssert, "Expected 'assert'")?;
        let condition = self.expression()?;
        let message = if self.match_token(&[Token::Comma]) {
            Some(self.expression()?)
        } else {
            None
        };
        Ok(Statement::Assert { condition, message })
    }

    fn global_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwGlobal, "Expected 'global'")?;
        let mut names = vec![self.consume_identifier()?];
        while self.match_token(&[Token::Comma]) {
            names.push(self.consume_identifier()?);
        }
        Ok(Statement::Global { names })
    }

    fn nonlocal_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwNonlocal, "Expected 'nonlocal'")?;
        let mut names = vec![self.consume_identifier()?];
        while self.match_token(&[Token::Comma]) {
            names.push(self.consume_identifier()?);
        }
        Ok(Statement::Nonlocal { names })
    }

    fn pass_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwPass, "Expected 'pass'")?;
        Ok(Statement::Pass)
    }

    fn decorated_statement(&mut self) -> Result<Statement, ParseError> {
        let mut decorators = Vec::new();
        while self.match_token(&[Token::At]) {
            let decorator = self.expression()?;
            decorators.push(decorator);
            if !self.match_token(&[Token::Newline]) {
                break;
            }
        }
        
        // Parse the decorated statement (function or class)
        let stmt = match self.peek().token {
            Token::KwFunc => self.function_def()?,
            Token::KwClass => self.class_def()?,
            _ => return Err(ParseError::InvalidSyntax {
                message: "Decorators can only be applied to functions or classes".to_string(),
            }),
        };

        // Add decorators to the statement
        match stmt {
            Statement::FunctionDef { name, params, return_type, body, .. } => {
                Ok(Statement::FunctionDef {
                    name,
                    params,
                    return_type,
                    body,
                    decorators,
                    is_async: false, // Default to false for now
                })
            }
            Statement::ClassDef { name, bases, body, .. } => {
                Ok(Statement::ClassDef { 
                    name, 
                    bases, 
                    body,
                    decorators: Vec::new(),
                    metaclass: None,
                })
            }
            _ => unreachable!(),
        }
    }

    fn block(&mut self) -> Result<Vec<Statement>, ParseError> {
        let mut statements = Vec::new();
        
        // Expect an indent after colon
        if !self.match_token(&[Token::Indent]) {
            return Err(ParseError::IndentationError {
                message: "Expected indented block".to_string(),
            });
        }

        // Parse statements until dedent
        while !self.check(&Token::Dedent) && !self.is_at_end() {
            if self.match_token(&[Token::Newline]) {
                continue; // Skip empty lines
            }
            statements.push(self.statement()?);
        }

        // Consume the dedent
        if !self.match_token(&[Token::Dedent]) {
            return Err(ParseError::IndentationError {
                message: "Expected dedent to close block".to_string(),
            });
        }

        Ok(statements)
    }

    fn list_or_comp(&mut self) -> Result<Expr, ParseError> {
        let mut elements = Vec::new();
        
        if !self.check(&Token::RBracket) {
            elements.push(self.expression()?);
            
            while self.match_token(&[Token::Comma]) {
                if self.check(&Token::RBracket) {
                    break;
                }
                elements.push(self.expression()?);
            }
        }
        
        self.consume(Token::RBracket, "Expected ']' after list elements")?;
        Ok(Expr::List(elements))
    }

    fn dict_or_set(&mut self) -> Result<Expr, ParseError> {
        if self.check(&Token::RBrace) {
            self.consume(Token::RBrace, "Expected '}'")?;
            return Ok(Expr::Dict(Vec::new()));
        }
        
        let first_expr = self.expression()?;
        
        if self.match_token(&[Token::Colon]) {
            // Dictionary
            let mut pairs = vec![(first_expr, self.expression()?)];
            
            while self.match_token(&[Token::Comma]) {
                if self.check(&Token::RBrace) {
                    break;
                }
                let key = self.expression()?;
                self.consume(Token::Colon, "Expected ':' in dictionary")?;
                let value = self.expression()?;
                pairs.push((key, value));
            }
            
            self.consume(Token::RBrace, "Expected '}' after dictionary")?;
            Ok(Expr::Dict(pairs))
        } else {
            // Set
            let mut elements = vec![first_expr];
            
            while self.match_token(&[Token::Comma]) {
                if self.check(&Token::RBrace) {
                    break;
                }
                elements.push(self.expression()?);
            }
            
            self.consume(Token::RBrace, "Expected '}' after set")?;
            Ok(Expr::Set(elements))
        }
    }
}