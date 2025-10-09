use crate::lexer::{Token, TokenInfo};
use crate::ast::*;
use thiserror::Error;
use std::fmt;

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

impl ParseError {
    pub fn with_location(self, line: usize, column: usize, file_name: &str) -> ParseErrorWithLocation {
        ParseErrorWithLocation {
            error: self,
            line,
            column,
            file_name: file_name.to_string(),
        }
    }
}

#[derive(Debug)]
pub struct ParseErrorWithLocation {
    pub error: ParseError,
    pub line: usize,
    pub column: usize,
    pub file_name: String,
}

impl fmt::Display for ParseErrorWithLocation {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "  File \"{}\", line {}, column {}\n{}", self.file_name, self.line, self.column, self.error)
    }
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

    fn is_keyword_as_identifier(&self) -> bool {
        matches!(self.peek().token, 
            Token::KwFunc | Token::KwClass | Token::KwIf | Token::KwElse | 
            Token::KwFor | Token::KwWhile | Token::KwReturn | Token::KwBreak | 
            Token::KwContinue | Token::KwImport | Token::KwFrom | Token::KwAs |
            Token::KwExtern | Token::KwExport | Token::KwAsync | Token::KwAwait |
            Token::KwTry | Token::KwExcept | Token::KwFinally | Token::KwRaise |
            Token::KwWith | Token::KwYield | Token::KwLambda | Token::KwMatch |
            Token::KwCase | Token::KwIn | Token::KwIs | Token::KwPass |
            Token::KwGlobal | Token::KwNonlocal | Token::KwDel | Token::KwAssert |
            Token::And | Token::Or | Token::Not
        )
    }

    fn is_keyword_assignment(&self) -> bool {
        // Look ahead to see if this keyword is followed by an assignment
        if self.current + 1 < self.tokens.len() {
            matches!(self.tokens[self.current + 1].token, Token::Assign)
        } else {
            false
        }
    }

    pub fn parse(&mut self) -> Result<Program, ParseError> {
        let mut statements = Vec::new();

        while !self.is_at_end() && !self.check(&Token::Eof) {
            // Skip newlines at the top level
            if self.check(&Token::Newline) {
                self.advance();
                continue;
            }
            let stmt = self.statement()?;
            statements.push(stmt);
        }

        Ok(Program { statements })
    }

    /// Parse a single REPL line - returns value for expressions like Python
    pub fn parse_repl_line(&mut self) -> Result<(Program, bool), ParseError> {
        let mut statements = Vec::new();
        let mut is_expression = false;

        // Skip leading newlines
        while self.check(&Token::Newline) {
            self.advance();
        }

        if self.is_at_end() || self.check(&Token::Eof) {
            return Ok((Program { statements }, false));
        }

        // Check if it's a statement keyword
        let is_statement_keyword = matches!(
            self.peek().token,
            Token::KwFunc | Token::KwClass | Token::KwIf | Token::KwFor | Token::KwWhile |
            Token::KwTry | Token::KwWith | Token::KwImport | Token::KwFrom | Token::KwReturn |
            Token::KwBreak | Token::KwContinue | Token::KwPass | Token::KwRaise | Token::KwAssert |
            Token::KwDel | Token::KwGlobal | Token::KwNonlocal | Token::KwAsync
        );

        // Check if it's an assignment (look ahead for = token)
        let mut is_assignment = false;
        let mut temp_pos = self.current;
        while temp_pos < self.tokens.len() {
            match &self.tokens[temp_pos].token {
                Token::Assign => {
                    is_assignment = true;
                    break;
                }
                Token::Newline | Token::Eof => break,
                Token::LParen | Token::LBracket | Token::Dot => {
                    temp_pos += 1;
                }
                _ => temp_pos += 1,
            }
        }

        if is_statement_keyword || is_assignment {
            // Parse as regular statement
            let stmt = self.statement()?;
            statements.push(stmt);
        } else {
            // It's an expression - parse and mark it for auto-print
            let expr = self.expression()?;

            // Consume optional trailing newline
            if self.check(&Token::Newline) {
                self.advance();
            }

            statements.push(Statement::Expression(expr));
            is_expression = true;
        }

        Ok((Program { statements }, is_expression))
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
            
            // If it's a top-level statement that should go in main body
            match &stmt {
                Statement::Expression(_) | Statement::VariableDef { .. } | 
                Statement::While { .. } | Statement::For { .. } | Statement::If { .. } |
                Statement::AttributeAssignment { .. } => {
                    main_body.push(stmt);
                }
                _ => {
                    statements.push(stmt);
                }
            }
        }
        
        // If we have main body statements and no explicit main function, execute them directly
        if !main_body.is_empty() && !has_main_function {
            // For scripts without explicit main, execute statements directly at global level
            statements.extend(main_body);
        } else if !main_body.is_empty() {
            // If there's an explicit main function, still add the main body statements
            statements.extend(main_body);
        }

        Ok(Program { statements })
    }

    fn statement(&mut self) -> Result<Statement, ParseError> {
        match &self.peek().token {
            Token::KwFunc | Token::KwAsync => {
                // For function definitions, always call function_def() directly
                // Don't check for keyword assignment for function definitions
                self.function_def()
            },
            Token::KwClass => {
                // Check if this is a keyword being used as an identifier in assignment
                if self.is_keyword_assignment() {
                    // Treat as expression/assignment
                    let expr = self.expression()?;
                    if self.match_token(&[Token::Assign]) {
                        self.variable_def(expr)
                    } else {
                        // Optional semicolon or newline for expression statements
                        self.match_token(&[Token::Semicolon, Token::Newline]);
                        Ok(Statement::Expression(expr))
                    }
                } else {
                    self.class_def()
                }
            },
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
                // Check if this is a typed variable declaration (identifier : type = value)
                if matches!(self.peek().token, Token::Identifier(_)) {
                    let checkpoint = self.current;
                    let name = self.consume_identifier()?;
                    
                    if self.match_token(&[Token::Colon]) {
                        // This is a typed variable declaration
                        let type_annotation = Some(self.type_annotation()?);
                        self.consume(Token::Assign, "Expected '=' after type annotation")?;
                        let value = self.expression()?;
                        self.match_token(&[Token::Semicolon, Token::Newline]);
                        
                        return Ok(Statement::VariableDef {
                            name,
                            type_annotation,
                            value: Some(value),
                        });
                    } else {
                        // Reset and try normal parsing
                        self.current = checkpoint;
                    }
                }
                
                // Try expression statement or variable definition
                let expr = self.expression()?;
                if self.match_token(&[Token::Assign]) {
                    self.variable_def(expr)
                } else if self.check_compound_assignment() {
                    self.compound_assignment(expr)
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
        
        let name = self.consume_identifier_or_keyword()?;
        
        // Optional parentheses for parameters
        let mut params = Vec::new();
        if self.match_token(&[Token::LParen]) {
            if !self.check(&Token::RParen) {
                loop {
                    let param_name = self.consume_identifier_or_keyword()?;
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
        
        // Extract docstring from the first statement if it's a string literal
        let docstring = self.extract_docstring(&body);
        
        Ok(Statement::FunctionDef {
            name,
            params,
            return_type,
            body,
            is_async,
            decorators: Vec::new(),
            docstring,
        })
    }

    fn class_def(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwClass, "Expected 'class' or 'iri'")?;
        let name = self.consume_identifier_or_keyword()?;
        
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
        
        // Extract docstring from the first statement if it's a string literal
        let docstring = self.extract_docstring(&body);
        
        Ok(Statement::ClassDef { 
            name, 
            bases, 
            body,
            decorators: Vec::new(),
            metaclass: None,
            docstring,
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
            Token::Identifier(name) if name.as_str() == "_" => {
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
        self.conditional()
    }

    fn conditional(&mut self) -> Result<Expr, ParseError> {
        let expr = self.assignment()?;
        
        // Check for conditional expression: expr if condition else else_expr
        if self.match_token(&[Token::KwIf]) {
            let condition = Box::new(self.assignment()?);
            self.consume(Token::KwElse, "Expected 'else' in conditional expression")?;
            let else_expr = Box::new(self.conditional()?);
            
            Ok(Expr::IfExp {
                condition,
                then_expr: Box::new(expr),
                else_expr,
            })
        } else {
            Ok(expr)
        }
    }

    fn assignment(&mut self) -> Result<Expr, ParseError> {
        let expr = self.or()?;
        
        // Check for walrus operator (:=) - named expression
        if self.match_token(&[Token::WalrusOp]) {
            let value = self.assignment()?;
            
            return match expr {
                Expr::Identifier(_) => Ok(Expr::NamedExpr {
                    target: Box::new(expr),
                    value: Box::new(value),
                }),
                _ => Err(ParseError::InvalidSyntax {
                    message: "Invalid target for walrus operator (:=)".to_string(),
                }),
            };
        }
        
        // Regular assignments (=) and compound assignments (+=, -=, etc.) 
        // should be handled by the statement parser, not here in the expression parser
        // Only handle them here if we're in an expression context where assignment is allowed
        
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
        let mut expr = self.bit_or()?;
        
        while self.match_token(&[Token::Eq, Token::Neq]) {
            let op = match self.previous().token {
                Token::Eq => BinaryOp::Eq,
                Token::Neq => BinaryOp::Ne,
                _ => unreachable!(),
            };
            let right = self.bit_or()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn bit_or(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.bit_xor()?;
        
        while self.match_token(&[Token::BitOr]) {
            let op = BinaryOp::BitOr;
            let right = self.bit_xor()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn bit_xor(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.bit_and()?;
        
        while self.match_token(&[Token::BitXor]) {
            let op = BinaryOp::BitXor;
            let right = self.bit_and()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn bit_and(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.shift()?;
        
        while self.match_token(&[Token::BitAnd]) {
            let op = BinaryOp::BitAnd;
            let right = self.shift()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn shift(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.comparison()?;
        
        while self.match_token(&[Token::Shl, Token::Shr]) {
            let op = match self.previous().token {
                Token::Shl => BinaryOp::LShift,
                Token::Shr => BinaryOp::RShift,
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
        
        // Check if we have comparison operators
        if self.match_token(&[Token::Gt, Token::Gte, Token::Lt, Token::Lte, Token::KwIn, Token::Not, Token::Eq, Token::Neq, Token::KwIs]) {
            let mut ops = Vec::new();
            let mut comparators = Vec::new();
            
            // Handle the first operator we just matched
            let first_op = match self.previous().token {
                Token::Gt => CompareOp::Gt,
                Token::Gte => CompareOp::GtE,
                Token::Lt => CompareOp::Lt,
                Token::Lte => CompareOp::LtE,
                Token::KwIn => CompareOp::In,
                Token::Eq => CompareOp::Eq,
                Token::Neq => CompareOp::NotEq,
                Token::KwIs => {
                    // Check for "is not"
                    if self.match_token(&[Token::Not]) {
                        CompareOp::IsNot
                    } else {
                        CompareOp::Is
                    }
                },
                Token::Not => {
                    // Handle "not in" operator
                    if self.match_token(&[Token::KwIn]) {
                        CompareOp::NotIn
                    } else {
                        return Err(ParseError::UnexpectedToken {
                            expected: "in".to_string(),
                            found: format!("{:?}", self.peek().token),
                        });
                    }
                }
                _ => unreachable!(),
            };
            
            ops.push(first_op);
            comparators.push(self.term()?);
            
            // Handle additional chained comparisons
            while self.match_token(&[Token::Gt, Token::Gte, Token::Lt, Token::Lte, Token::KwIn, Token::Not, Token::Eq, Token::Neq, Token::KwIs]) {
                let op = match self.previous().token {
                    Token::Gt => CompareOp::Gt,
                    Token::Gte => CompareOp::GtE,
                    Token::Lt => CompareOp::Lt,
                    Token::Lte => CompareOp::LtE,
                    Token::KwIn => CompareOp::In,
                    Token::Eq => CompareOp::Eq,
                    Token::Neq => CompareOp::NotEq,
                    Token::KwIs => {
                        // Check for "is not"
                        if self.match_token(&[Token::Not]) {
                            CompareOp::IsNot
                        } else {
                            CompareOp::Is
                        }
                    },
                    Token::Not => {
                        // Handle "not in" operator
                        if self.match_token(&[Token::KwIn]) {
                            CompareOp::NotIn
                        } else {
                            return Err(ParseError::UnexpectedToken {
                                expected: "in".to_string(),
                                found: format!("{:?}", self.peek().token),
                            });
                        }
                    }
                    _ => unreachable!(),
                };
                
                ops.push(op);
                comparators.push(self.term()?);
            }
            
            expr = Expr::Compare {
                left: Box::new(expr),
                ops,
                comparators,
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
        let mut expr = self.power()?;
        
        while self.match_token(&[Token::Star, Token::Slash, Token::Percent, Token::FloorDiv]) {
            let op = match self.previous().token {
                Token::Star => BinaryOp::Mul,
                Token::Slash => BinaryOp::Div,
                Token::Percent => BinaryOp::Mod,
                Token::FloorDiv => BinaryOp::FloorDiv,
                _ => unreachable!(),
            };
            let right = self.power()?;
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op,
                right: Box::new(right),
            };
        }
        
        Ok(expr)
    }

    fn power(&mut self) -> Result<Expr, ParseError> {
        let mut expr = self.unary()?;
        
        // Power operator is right-associative, so we handle it differently
        if self.match_token(&[Token::Power]) {
            let right = self.power()?; // Right-associative recursion
            expr = Expr::BinaryOp {
                left: Box::new(expr),
                op: BinaryOp::Pow,
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
                // Check if there's a parenthesis after the attribute for method calls
                if self.check(&Token::LParen) {
                    self.advance(); // consume the '('
                    expr = self.finish_method_call(expr, attr)?;
                } else {
                    // Just attribute access
                    expr = Expr::Attribute {
                        object: Box::new(expr),
                        name: attr,
                    };
                }
            } else if self.match_token(&[Token::LBracket]) {
                // Check if this is a slice or subscript
                let mut start = None;
                let mut stop = None;
                let mut step = None;
                
                // Check for empty slice [:] or [::step]
                if self.check(&Token::Colon) {
                    // This is a slice starting with :
                    self.consume(Token::Colon, "Expected ':'")?;
                    
                    // Check for stop value
                    if !self.check(&Token::Colon) && !self.check(&Token::RBracket) {
                        stop = Some(Box::new(self.expression()?));
                    }
                    
                    // Check for step
                    if self.match_token(&[Token::Colon]) {
                        if !self.check(&Token::RBracket) {
                            step = Some(Box::new(self.expression()?));
                        }
                    }
                    
                    self.consume(Token::RBracket, "Expected ']' after slice")?;
                    expr = Expr::Slice {
                        object: Box::new(expr),
                        start,
                        stop,
                        step,
                    };
                } else {
                    // Parse first expression (could be start of slice or simple index)
                    let first_expr = self.expression()?;
                    
                    if self.match_token(&[Token::Colon]) {
                        // This is a slice with start value
                        start = Some(Box::new(first_expr));
                        
                        // Check for stop value
                        if !self.check(&Token::Colon) && !self.check(&Token::RBracket) {
                            stop = Some(Box::new(self.expression()?));
                        }
                        
                        // Check for step
                        if self.match_token(&[Token::Colon]) {
                            if !self.check(&Token::RBracket) {
                                step = Some(Box::new(self.expression()?));
                            }
                        }
                        
                        self.consume(Token::RBracket, "Expected ']' after slice")?;
                        expr = Expr::Slice {
                            object: Box::new(expr),
                            start,
                            stop,
                            step,
                        };
                    } else {
                        // Simple subscript
                        self.consume(Token::RBracket, "Expected ']' after index")?;
                        expr = Expr::Subscript {
                            object: Box::new(expr),
                            index: Box::new(first_expr),
                        };
                    }
                }
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
        } else if self.match_token(&[Token::KwYield]) {
            // Parse yield expression: yield [value] or yield from expr
            if self.match_token(&[Token::KwFrom]) {
                // yield from expression
                let expr = Box::new(self.expression()?);
                Ok(Expr::YieldFrom(expr))
            } else {
                // yield [value]
                let value = if self.check(&Token::Semicolon) || self.check(&Token::Newline) || 
                              self.check(&Token::RBrace) || self.check(&Token::RParen) ||
                              self.check(&Token::RBracket) || self.is_at_end() {
                    None
                } else {
                    Some(Box::new(self.expression()?))
                };
                Ok(Expr::Yield(value))
            }
        } else if self.match_token(&[Token::KwAwait]) {
            // Parse await expression: await expr
            let expr = Box::new(self.expression()?);
            Ok(Expr::Await(expr))
        } else if self.match_token(&[Token::KwLambda]) {
            // Parse lambda function: lambda params: body
            let mut params = Vec::new();
            
            // Parse parameters (optional)
            if !self.check(&Token::Colon) {
                let name = self.consume_identifier()?;
                params.push(Param {
                    name,
                    type_annotation: None,
                    default: None,
                    kind: ParamKind::Positional,
                });
                while self.match_token(&[Token::Comma]) {
                    let name = self.consume_identifier()?;
                    params.push(Param {
                        name,
                        type_annotation: None,
                        default: None,
                        kind: ParamKind::Positional,
                    });
                }
            }
            
            self.consume(Token::Colon, "Expected ':' after lambda parameters")?;
            let body = Box::new(self.expression()?);
            
            Ok(Expr::Lambda { params, body })
        } else if let Token::Int(n) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Literal(Literal::Int(n)))
        } else if let Token::Float(n) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Literal(Literal::Float(n)))
        } else if let Token::StringLit(s) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Literal(Literal::String(s)))
        } else if let Token::DocString(s) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::DocString(s))
        } else if let Token::FString(s) = self.peek().token.clone() {
            self.advance();
            self.parse_fstring(s)
        } else if let Token::Identifier(name) = self.peek().token.clone() {
            self.advance();
            Ok(Expr::Identifier(name))
        } else if self.is_keyword_as_identifier() {
            // Handle keywords used as identifiers in expression contexts
            let name = self.consume_identifier_or_keyword()?;
            Ok(Expr::Identifier(name))
        } else if self.match_token(&[Token::LParen]) {
            // Handle empty tuple
            if self.check(&Token::RParen) {
                self.consume(Token::RParen, "Expected ')")?;
                return Ok(Expr::Tuple(Vec::new()));
            }
            
            let first_expr = self.assignment()?; // Use assignment instead of expression to avoid conditional parsing
            
            // Check if this is a generator expression
            if self.check(&Token::KwFor) {
                // This is a generator expression: (element for target in iter)
                let mut generators = Vec::new();
                
                // Parse all for clauses
                while self.check(&Token::KwFor) {
                    self.consume(Token::KwFor, "Expected 'for'")?;
                    let target = self.consume_identifier()?;
                    self.consume(Token::KwIn, "Expected 'in'")?;
                    let iter = self.assignment()?; // Use assignment instead of expression to avoid conditional parsing
                    
                    let mut ifs = Vec::new();
                    while self.check(&Token::KwIf) {
                        self.consume(Token::KwIf, "Expected 'if'")?;
                        ifs.push(self.assignment()?); // Use assignment instead of expression
                    }
                    
                    generators.push(crate::ast::Comprehension {
                        target,
                        iter,
                        ifs,
                        is_async: false,
                    });
                }
                
                self.consume(Token::RParen, "Expected ')' after generator expression")?;
                
                Ok(Expr::GeneratorExp {
                    element: Box::new(first_expr),
                    generators,
                })
            }
            // Check if this is a tuple (has comma) or just grouped expression
            else if self.match_token(&[Token::Comma]) {
                let mut elements = vec![first_expr];
                
                // Handle trailing comma case: (expr,)
                if self.check(&Token::RParen) {
                    self.consume(Token::RParen, "Expected ')' after tuple")?;
                    return Ok(Expr::Tuple(elements));
                }
                
                // Parse remaining elements
                loop {
                    elements.push(self.expression()?);
                    if !self.match_token(&[Token::Comma]) {
                        break;
                    }
                    // Allow trailing comma
                    if self.check(&Token::RParen) {
                        break;
                    }
                }
                
                self.consume(Token::RParen, "Expected ')' after tuple")?;
                Ok(Expr::Tuple(elements))
            } else {
                // Just a grouped expression
                self.consume(Token::RParen, "Expected ')' after expression")?;
                Ok(first_expr)
            }
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
                expected: format!("{}", token),
                found: format!("{}", self.peek()),
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

    fn consume_identifier_or_keyword(&mut self) -> Result<String, ParseError> {
        match &self.peek().token {
            Token::Identifier(name) => {
                let name = name.clone();
                self.advance();
                Ok(name)
            }
            // Allow keywords to be used as identifiers in parameter contexts
            Token::KwFunc => {
                self.advance();
                Ok("func".to_string())
            }
            Token::KwClass => {
                self.advance();
                Ok("class".to_string())
            }
            Token::KwIf => {
                self.advance();
                Ok("if".to_string())
            }
            Token::KwElse => {
                self.advance();
                Ok("else".to_string())
            }
            Token::KwFor => {
                self.advance();
                Ok("for".to_string())
            }
            Token::KwWhile => {
                self.advance();
                Ok("while".to_string())
            }
            Token::KwReturn => {
                self.advance();
                Ok("return".to_string())
            }
            Token::KwBreak => {
                self.advance();
                Ok("break".to_string())
            }
            Token::KwContinue => {
                self.advance();
                Ok("continue".to_string())
            }
            Token::KwImport => {
                self.advance();
                Ok("import".to_string())
            }
            Token::KwFrom => {
                self.advance();
                Ok("from".to_string())
            }
            Token::KwAs => {
                self.advance();
                Ok("as".to_string())
            }
            Token::KwExtern => {
                self.advance();
                Ok("extern".to_string())
            }
            Token::KwExport => {
                self.advance();
                Ok("export".to_string())
            }
            Token::KwAsync => {
                self.advance();
                Ok("async".to_string())
            }
            Token::KwAwait => {
                self.advance();
                Ok("await".to_string())
            }
            Token::KwTry => {
                self.advance();
                Ok("try".to_string())
            }
            Token::KwExcept => {
                self.advance();
                Ok("except".to_string())
            }
            Token::KwFinally => {
                self.advance();
                Ok("finally".to_string())
            }
            Token::KwRaise => {
                self.advance();
                Ok("raise".to_string())
            }
            Token::KwWith => {
                self.advance();
                Ok("with".to_string())
            }
            Token::KwYield => {
                self.advance();
                Ok("yield".to_string())
            }
            Token::KwLambda => {
                self.advance();
                Ok("lambda".to_string())
            }
            Token::KwMatch => {
                self.advance();
                Ok("match".to_string())
            }
            Token::KwCase => {
                self.advance();
                Ok("case".to_string())
            }
            Token::KwIn => {
                self.advance();
                Ok("in".to_string())
            }
            Token::KwIs => {
                self.advance();
                Ok("is".to_string())
            }
            Token::KwPass => {
                self.advance();
                Ok("pass".to_string())
            }
            Token::KwGlobal => {
                self.advance();
                Ok("global".to_string())
            }
            Token::KwNonlocal => {
                self.advance();
                Ok("nonlocal".to_string())
            }
            Token::KwDel => {
                self.advance();
                Ok("del".to_string())
            }
            Token::KwAssert => {
                self.advance();
                Ok("assert".to_string())
            }
            Token::And => {
                self.advance();
                Ok("and".to_string())
            }
            Token::Or => {
                self.advance();
                Ok("or".to_string())
            }
            Token::Not => {
                self.advance();
                Ok("not".to_string())
            }
            _ => Err(ParseError::UnexpectedToken {
                expected: "identifier or keyword".to_string(),
                found: self.peek().to_string(),
            })
        }
    }

    fn consume_dotted_name(&mut self) -> Result<String, ParseError> {
        let mut name = self.consume_identifier()?;
        
        while self.match_token(&[Token::Dot]) {
            name.push('.');
            name.push_str(&self.consume_identifier()?);
        }
        
        Ok(name)
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
            Token::None => {
                // Handle None as a type annotation
                self.advance();
                Ok(Type::Simple("None".to_string()))
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
                if matches!(self.peek().token, Token::Identifier(_)) {
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

    fn finish_method_call(&mut self, object: Expr, method: String) -> Result<Expr, ParseError> {
        let mut args = Vec::new();
        let mut kwargs = Vec::new();
        
        if !self.check(&Token::RParen) {
            loop {
                // Check if this is a keyword argument
                if matches!(self.peek().token, Token::Identifier(_)) {
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
        
        Ok(Expr::MethodCall {
            object: Box::new(object),
            method,
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
            Expr::Identifier(name) => {
                Ok(Statement::VariableDef {
                    name,
                    type_annotation,
                    value: Some(value),
                })
            },
            Expr::Attribute { object, name } => {
                // Handle attribute assignment like self.x = value
                Ok(Statement::AttributeAssignment {
                    object: *object,
                    name,
                    value,
                })
            },
            Expr::Subscript { object, index } => {
                // Handle subscript assignment like arr[index] = value
                Ok(Statement::SubscriptAssignment {
                    object: *object,
                    index: *index,
                    value,
                })
            },
            _ => Err(ParseError::InvalidSyntax {
                message: "Invalid assignment target".to_string(),
            }),
        }
    }
    
    fn check_compound_assignment(&self) -> bool {
        matches!(self.peek().token, 
            Token::PlusEq | Token::MinusEq | Token::StarEq | Token::SlashEq |
            Token::PercentEq | Token::PowerEq | Token::FloorDivEq
        )
    }
    
    fn compound_assignment(&mut self, target: Expr) -> Result<Statement, ParseError> {
        let op_token = self.advance().token.clone();
        let value = self.expression()?;
        
        self.match_token(&[Token::Semicolon, Token::Newline]);
        
        match target {
            Expr::Identifier(name) => {
                // Convert compound assignment to regular assignment with binary operation
                // e.g., x += 5 becomes x = x + 5
                let binary_op = match op_token {
                    Token::PlusEq => BinaryOp::Add,
                    Token::MinusEq => BinaryOp::Sub,
                    Token::StarEq => BinaryOp::Mul,
                    Token::SlashEq => BinaryOp::Div,
                    Token::PercentEq => BinaryOp::Mod,
                    Token::PowerEq => BinaryOp::Pow,
                    Token::FloorDivEq => BinaryOp::FloorDiv,
                    _ => return Err(ParseError::InvalidSyntax {
                        message: "Invalid compound assignment operator".to_string(),
                    }),
                };
                
                let combined_value = Expr::BinaryOp {
                    left: Box::new(Expr::Identifier(name.clone())),
                    op: binary_op,
                    right: Box::new(value),
                };
                
                Ok(Statement::VariableDef {
                    name,
                    type_annotation: None,
                    value: Some(combined_value),
                })
            },
            Expr::Attribute { object, name: attr_name } => {
                // Handle attribute compound assignment like self.x += value
                let binary_op = match op_token {
                    Token::PlusEq => BinaryOp::Add,
                    Token::MinusEq => BinaryOp::Sub,
                    Token::StarEq => BinaryOp::Mul,
                    Token::SlashEq => BinaryOp::Div,
                    Token::PercentEq => BinaryOp::Mod,
                    Token::PowerEq => BinaryOp::Pow,
                    Token::FloorDivEq => BinaryOp::FloorDiv,
                    _ => return Err(ParseError::InvalidSyntax {
                        message: "Invalid compound assignment operator".to_string(),
                    }),
                };
                
                let combined_value = Expr::BinaryOp {
                    left: Box::new(Expr::Attribute { object: object.clone(), name: attr_name.clone() }),
                    op: binary_op,
                    right: Box::new(value),
                };
                
                Ok(Statement::AttributeAssignment {
                    object: *object,
                    name: attr_name,
                    value: combined_value,
                })
            },
            Expr::Subscript { object, index } => {
                // Handle subscript compound assignment like arr[index] += value
                let binary_op = match op_token {
                    Token::PlusEq => BinaryOp::Add,
                    Token::MinusEq => BinaryOp::Sub,
                    Token::StarEq => BinaryOp::Mul,
                    Token::SlashEq => BinaryOp::Div,
                    Token::PercentEq => BinaryOp::Mod,
                    Token::PowerEq => BinaryOp::Pow,
                    Token::FloorDivEq => BinaryOp::FloorDiv,
                    _ => return Err(ParseError::InvalidSyntax {
                        message: "Invalid compound assignment operator".to_string(),
                    }),
                };
                
                let combined_value = Expr::BinaryOp {
                    left: Box::new(Expr::Subscript { object: object.clone(), index: index.clone() }),
                    op: binary_op,
                    right: Box::new(value),
                };
                
                Ok(Statement::SubscriptAssignment {
                    object: *object,
                    index: *index,
                    value: combined_value,
                })
            },
            _ => Err(ParseError::InvalidSyntax {
                message: "Invalid assignment target for compound assignment".to_string(),
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
        let module = self.consume_dotted_name()?;
        let alias = if self.match_token(&[Token::KwAs]) {
            Some(self.consume_identifier()?)
        } else {
            None
        };
        Ok(Statement::Import { module, alias })
    }

    fn from_import_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwFrom, "Expected 'from'")?;
        let module = self.consume_dotted_name()?;
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
            Statement::FunctionDef { name, params, return_type, body, is_async: _, decorators: _, docstring: _ } => {
                Ok(Statement::FunctionDef {
                    name,
                    params,
                    return_type,
                    body,
                    decorators,
                    is_async: false, // Default to false for now
                    docstring: None, // Decorators don't preserve docstrings for now
                })
            }
            Statement::ClassDef { name, bases, body, decorators: _, metaclass: _, docstring: _ } => {
                Ok(Statement::ClassDef { 
                    name, 
                    bases, 
                    body,
                    decorators,
                    metaclass: None,
                    docstring: None, // Decorators don't preserve docstrings for now
                })
            }
            _ => unreachable!(),
        }
    }

    fn block(&mut self) -> Result<Vec<Statement>, ParseError> {
        let mut statements = Vec::new();
        
        // Skip any newlines after colon
        while self.match_token(&[Token::Newline]) {
            // Continue skipping newlines
        }
        
        // Expect an indent after colon (and possible newlines)
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
        // Skip newlines after opening bracket
        while self.match_token(&[Token::Newline]) {}
        
        if self.check(&Token::RBracket) {
            self.consume(Token::RBracket, "Expected ']'")?;
            return Ok(Expr::List(vec![]));
        }
        
        let first_element = self.expression()?;
        
        // Check if this is a list comprehension
        if self.check(&Token::KwFor) {
            // This is a list comprehension: [element for target in iter]
            let mut generators = Vec::new();
            
            // Parse all for clauses
            while self.check(&Token::KwFor) {
                self.consume(Token::KwFor, "Expected 'for'")?;
                let target = self.consume_identifier()?;
                self.consume(Token::KwIn, "Expected 'in'")?;
                let iter = self.assignment()?;
                
                let mut ifs = Vec::new();
                while self.check(&Token::KwIf) {
                    self.consume(Token::KwIf, "Expected 'if'")?;
                    ifs.push(self.assignment()?);
                }
                
                generators.push(crate::ast::Comprehension {
                    target,
                    iter,
                    ifs,
                    is_async: false,
                });
            }
            
            self.consume(Token::RBracket, "Expected ']' after list comprehension")?;
            
            Ok(Expr::ListComp {
                element: Box::new(first_element),
                generators,
            })
        } else {
            // Regular list
            let mut elements = vec![first_element];
            
            while self.match_token(&[Token::Comma]) {
                // Skip newlines after comma
                while self.match_token(&[Token::Newline]) {}
                
                if self.check(&Token::RBracket) {
                    break; // Trailing comma
                }
                elements.push(self.expression()?);
            }
            
            // Skip newlines before closing bracket
            while self.match_token(&[Token::Newline]) {}
            self.consume(Token::RBracket, "Expected ']' after list")?;
            Ok(Expr::List(elements))
        }
    }

    fn dict_or_set(&mut self) -> Result<Expr, ParseError> {
        // Skip newlines after opening brace
        while self.match_token(&[Token::Newline]) {}
        
        if self.check(&Token::RBrace) {
            self.consume(Token::RBrace, "Expected '}'")?;
            return Ok(Expr::Dict(Vec::new()));
        }
        
        let first_expr = self.expression()?;
        
        if self.match_token(&[Token::Colon]) {
            // Check if this is a dictionary comprehension
            let value_expr = self.expression()?;
            
            if self.check(&Token::KwFor) {
                // This is a dictionary comprehension: {key: value for target in iter}
                self.consume(Token::KwFor, "Expected 'for'")?;
                let target = self.consume_identifier()?;
                self.consume(Token::KwIn, "Expected 'in'")?;
                let iter = self.assignment()?;
                
                let mut ifs = Vec::new();
                while self.check(&Token::KwIf) {
                    self.consume(Token::KwIf, "Expected 'if'")?;
                    ifs.push(self.assignment()?);
                }
                
                self.consume(Token::RBrace, "Expected '}' after dictionary comprehension")?;
                
                let generator = crate::ast::Comprehension {
                    target,
                    iter,
                    ifs,
                    is_async: false,
                };
                
                return Ok(Expr::DictComp {
                    key: Box::new(first_expr),
                    value: Box::new(value_expr),
                    generators: vec![generator],
                });
            }
            
            // Regular dictionary
            let mut pairs = vec![(first_expr, value_expr)];
            
            while self.match_token(&[Token::Comma]) {
                // Skip newlines after comma
                while self.match_token(&[Token::Newline]) {}
                
                if self.check(&Token::RBrace) {
                    break;
                }
                let key = self.expression()?;
                self.consume(Token::Colon, "Expected ':' in dictionary")?;
                let value = self.expression()?;
                pairs.push((key, value));
            }
            
            // Skip newlines before closing brace
            while self.match_token(&[Token::Newline]) {}
            self.consume(Token::RBrace, "Expected '}' after dictionary")?;
            Ok(Expr::Dict(pairs))
        } else {
            // Check if this is a set comprehension
            if self.check(&Token::KwFor) {
                // This is a set comprehension: {element for target in iter}
                let mut generators = Vec::new();
                
                // Parse all for clauses
                while self.check(&Token::KwFor) {
                    self.consume(Token::KwFor, "Expected 'for'")?;
                    let target = self.consume_identifier()?;
                    self.consume(Token::KwIn, "Expected 'in'")?;
                    let iter = self.assignment()?;
                    
                    let mut ifs = Vec::new();
                    while self.check(&Token::KwIf) {
                        self.consume(Token::KwIf, "Expected 'if'")?;
                        ifs.push(self.assignment()?);
                    }
                    
                    generators.push(crate::ast::Comprehension {
                        target,
                        iter,
                        ifs,
                        is_async: false,
                    });
                }
                
                self.consume(Token::RBrace, "Expected '}' after set comprehension")?;
                
                return Ok(Expr::SetComp {
                    element: Box::new(first_expr),
                    generators,
                });
            }
            
            // Set
            let mut elements = vec![first_expr];
            
            while self.match_token(&[Token::Comma]) {
                // Skip newlines after comma
                while self.match_token(&[Token::Newline]) {}
                
                if self.check(&Token::RBrace) {
                    break;
                }
                elements.push(self.expression()?);
            }
            
            // Skip newlines before closing brace
            while self.match_token(&[Token::Newline]) {}
            self.consume(Token::RBrace, "Expected '}' after set")?;
            Ok(Expr::Set(elements))
        }
    }

    fn parse_fstring(&mut self, content: String) -> Result<Expr, ParseError> {
        let mut parts = Vec::new();
        let mut current_string = String::new();
        let mut chars = content.chars().peekable();
        
        while let Some(ch) = chars.next() {
            if ch == '{' {
                // Check for escaped brace {{
                if chars.peek() == Some(&'{') {
                    chars.next(); // consume second {
                    current_string.push('{');
                    continue;
                }
                
                // If we have accumulated string content, add it as a string part
                if !current_string.is_empty() {
                    parts.push(crate::ast::FormatPart::String(current_string.clone()));
                    current_string.clear();
                }
                
                // Parse the expression inside {}
                let mut expr_content = String::new();
                let mut brace_count = 1;
                
                while let Some(ch) = chars.next() {
                    if ch == '{' {
                        brace_count += 1;
                    } else if ch == '}' {
                        brace_count -= 1;
                        if brace_count == 0 {
                            break;
                        }
                    }
                    expr_content.push(ch);
                }
                
                if brace_count > 0 {
                    return Err(ParseError::InvalidSyntax {
                        message: "Unclosed '{' in f-string".to_string(),
                    });
                }
                
                // Parse the expression content
                if !expr_content.is_empty() {
                    // Create a mini-parser for the expression
                    let mut expr_lexer = crate::lexer::Lexer::new(&expr_content);
                    let expr_tokens = expr_lexer.collect::<Result<Vec<_>, _>>()
                        .map_err(|_| ParseError::InvalidSyntax {
                            message: "Invalid expression in f-string".to_string(),
                        })?;
                    
                    let token_infos: Vec<crate::lexer::TokenInfo> = expr_tokens.into_iter()
                        .enumerate()
                        .map(|(i, token_info)| crate::lexer::TokenInfo {
                            token: token_info.token,
                            span: (i, i + 1),
                            line: 1,
                            column: i + 1,
                        })
                        .collect();
                    
                    let mut expr_parser = Parser::new(token_infos);
                    let expr = expr_parser.expression()?;
                    
                    parts.push(crate::ast::FormatPart::Expression {
                        expr,
                        format_spec: None,
                        conversion: None,
                    });
                }
            } else if ch == '}' {
                // Check for escaped brace }}
                if chars.peek() == Some(&'}') {
                    chars.next(); // consume second }
                    current_string.push('}');
                    continue;
                } else {
                    return Err(ParseError::InvalidSyntax {
                        message: "Unmatched '}' in f-string".to_string(),
                    });
                }
            } else {
                current_string.push(ch);
            }
        }
        
        // Add any remaining string content
        if !current_string.is_empty() {
            parts.push(crate::ast::FormatPart::String(current_string));
        }
        
        Ok(Expr::FormatString { parts })
    }

    /// Extract docstring from the first statement in a body if it's a string literal or DocString
    fn extract_docstring(&self, body: &[Statement]) -> Option<String> {
        if let Some(Statement::Expression(expr)) = body.first() {
            match expr {
                Expr::Literal(crate::ast::Literal::String(s)) => Some(s.clone()),
                Expr::DocString(s) => Some(s.clone()),
                _ => None,
            }
        } else {
            None
        }
    }
}
