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
                // Check for tuple unpacking pattern: a, b = value
                let checkpoint = self.current;
                let first_expr = self.expression()?;

                // Check if this is a tuple (comma after first expression) before =
                if self.match_token(&[Token::Comma]) {
                    // This is potentially a tuple unpacking
                    let mut tuple_items = vec![first_expr];

                    // Parse remaining tuple elements
                    while !self.check(&Token::Assign) && !self.is_at_end() {
                        tuple_items.push(self.expression()?);
                        if !self.match_token(&[Token::Comma]) {
                            break;
                        }
                    }

                    // Now expect an assignment
                    if self.match_token(&[Token::Assign]) {
                        return self.variable_def(Expr::Tuple(tuple_items));
                    } else {
                        // Not an assignment, treat as expression statement
                        // This is an error case - tuples without parentheses in expression context
                        return Err(ParseError::InvalidSyntax {
                            message: "Tuple expression needs parentheses or assignment".to_string(),
                        });
                    }
                }

                // Not a tuple, check for regular assignment
                if self.match_token(&[Token::Assign]) {
                    self.variable_def(first_expr)
                } else if self.check_compound_assignment() {
                    self.advance(); // Advance past the compound assignment operator
                    self.compound_assignment(first_expr)
                } else {
                    // Optional semicolon or newline for expression statements
                    self.match_token(&[Token::Semicolon, Token::Newline]);
                    Ok(Statement::Expression(first_expr))
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
                    // Check for *args or **kwargs
                    if self.match_token(&[Token::Star]) {
                        if self.match_token(&[Token::Star]) {
                            // **kwargs
                            let param_name = self.consume_identifier_or_keyword()?;
                            params.push(Param {
                                name: param_name,
                                type_annotation: None,
                                default: None,
                                kind: ParamKind::VarKwargs,
                            });
                        } else {
                            // *args
                            let param_name = self.consume_identifier_or_keyword()?;
                            params.push(Param {
                                name: param_name,
                                type_annotation: None,
                                default: None,
                                kind: ParamKind::VarArgs,
                            });
                        }
                    } else if self.match_token(&[Token::Power]) {
                        // **kwargs (alternative syntax)
                        let param_name = self.consume_identifier_or_keyword()?;
                        params.push(Param {
                            name: param_name,
                            type_annotation: None,
                            default: None,
                            kind: ParamKind::VarKwargs,
                        });
                    } else {
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
                    }
                    
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

        // Parse variable(s) - can be a single variable or tuple unpacking
        let mut variables = Vec::new();
        variables.push(self.consume_identifier()?);

        // Check for tuple unpacking (comma-separated variables)
        while self.match_token(&[Token::Comma]) {
            variables.push(self.consume_identifier()?);
        }

        self.consume(Token::KwIn, "Expected 'in' or 'cikin' after for variable")?;
        let iterable = self.expression()?;
        self.consume(Token::Colon, "Expected ':' after for clause")?;
        let body = self.block()?;

        // For backwards compatibility, store first variable as 'variable'
        let variable = variables.get(0).unwrap_or(&String::new()).clone();

        Ok(Statement::For {
            variable,
            variables,
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
            let expr = Box::new(self.expression()?);
            Ok(Expr::Await(expr))
        } else if self.match_token(&[Token::KwLambda]) {
            // Parse lambda expression
            let mut params = Vec::new();
            
            // Lambda parameters (optional)
            if !self.check(&Token::Colon) {
                if self.match_token(&[Token::LParen]) {
                    // Lambda with parentheses
                    if !self.check(&Token::RParen) {
                        loop {
                            let param_name = self.consume_identifier()?;
                            params.push(Param {
                                name: param_name,
                                type_annotation: None,
                                default: None,
                                kind: ParamKind::Positional,
                            });
                            
                            if !self.match_token(&[Token::Comma]) {
                                break;
                            }
                        }
                    }
                    self.consume(Token::RParen, "Expected ')' after lambda parameters")?;
                } else {
                    // Simple lambda with single parameter
                    let param_name = self.consume_identifier()?;
                    params.push(Param {
                        name: param_name,
                        type_annotation: None,
                        default: None,
                        kind: ParamKind::Positional,
                    });
                }
            }
            
            self.consume(Token::Colon, "Expected ':' after lambda parameters")?;
            let body = Box::new(self.expression()?);
            
            Ok(Expr::Lambda { params, body })
        } else if self.is_number_token() {
            // Consume the number token first
            match self.advance().token {
                Token::Int(n) => Ok(Expr::Literal(Literal::Int(n))),
                Token::Float(n) => Ok(Expr::Literal(Literal::Float(n))),
                _ => unreachable!(),
            }
        } else if matches!(self.peek().token, Token::StringLit(_)) || matches!(self.peek().token, Token::FString(_)) {
            // Consume the string token first
            match self.advance().token.clone() {
                Token::StringLit(s) => Ok(Expr::Literal(Literal::String(s))),
                Token::FString(s) => {
                    // Parse f-string
                    let parts = self.parse_fstring(&s)?;
                    Ok(Expr::FormatString { parts })
                }
                _ => unreachable!(),
            }
        } else if matches!(self.peek().token, Token::DocString(_)) {
            // Consume the docstring token first
            match self.advance().token.clone() {
                Token::DocString(s) => Ok(Expr::DocString(s)),
                _ => unreachable!(),
            }
        } else if matches!(self.peek().token, Token::Identifier(_)) {
            // Consume the identifier token first
            match self.advance().token.clone() {
                Token::Identifier(name) => Ok(Expr::Identifier(name)),
                _ => unreachable!(),
            }
        } else if self.match_token(&[Token::LParen]) {
            // Check for empty tuple
            if self.match_token(&[Token::RParen]) {
                return Ok(Expr::Tuple(Vec::new()));
            }
            
            let first_expr = self.expression()?;
            
            // Check if this is a tuple (has comma) or just grouped expression
            if self.match_token(&[Token::Comma]) {
                // Tuple
                let mut items = vec![first_expr];
                while !self.check(&Token::RParen) {
                    if self.match_token(&[Token::Comma]) {
                        // Allow trailing comma
                        if self.check(&Token::RParen) {
                            break;
                        }
                        items.push(self.expression()?);
                    } else {
                        items.push(self.expression()?);
                        // After expression, we should have either comma or closing paren
                        if !self.check(&Token::RParen) && !self.check(&Token::Comma) {
                            return Err(ParseError::UnexpectedToken {
                                expected: "',' or ')'".to_string(),
                                found: format!("{:?}", self.peek().token),
                            });
                        }
                    }
                }
                self.consume(Token::RParen, "Expected ')' after tuple items")?;
                Ok(Expr::Tuple(items))
            } else {
                // Just grouped expression
                self.consume(Token::RParen, "Expected ')' after expression")?;
                Ok(first_expr)
            }
        } else if self.match_token(&[Token::LBracket]) {
            // List or list comprehension
            if self.check(&Token::RBracket) {
                // Empty list
                self.advance();
                Ok(Expr::List(Vec::new()))
            } else {
                let first_expr = self.expression()?;
                
                // Check if this is a list comprehension
                if self.match_token(&[Token::KwFor]) {
                    // List comprehension
                    let generators = self.parse_comprehension()?;
                    self.consume(Token::RBracket, "Expected ']' after list comprehension")?;
                    Ok(Expr::ListComp {
                        element: Box::new(first_expr),
                        generators,
                    })
                } else {
                    // Regular list
                    let mut items = vec![first_expr];
                    while self.match_token(&[Token::Comma]) {
                        if self.check(&Token::RBracket) {
                            break;
                        }
                        items.push(self.expression()?);
                    }
                    self.consume(Token::RBracket, "Expected ']' after list items")?;
                    Ok(Expr::List(items))
                }
            }
        } else if self.match_token(&[Token::LBrace]) {
            // Dict, set, or comprehension
            if self.check(&Token::RBrace) {
                // Empty dict
                self.advance();
                Ok(Expr::Dict(Vec::new()))
            } else {
                let first_expr = self.expression()?;
                
                // Check if it's a dict
                if self.match_token(&[Token::Colon]) {
                    // Dict or dict comprehension
                    let first_value = self.expression()?;
                    let mut pairs = vec![(first_expr.clone(), first_value.clone())];
                    
                    // Check if this is a dict comprehension
                    if self.match_token(&[Token::KwFor]) {
                        // Dict comprehension
                        let generators = self.parse_comprehension()?;
                        self.consume(Token::RBrace, "Expected '}' after dict comprehension")?;
                        Ok(Expr::DictComp {
                            key: Box::new(first_expr.clone()),
                            value: Box::new(first_value.clone()),
                            generators,
                        })
                    } else {
                        // Regular dict
                        while self.match_token(&[Token::Comma]) {
                            if self.check(&Token::RBrace) {
                                break;
                            }
                            let key = self.expression()?;
                            self.consume(Token::Colon, "Expected ':' in dict")?;
                            let value = self.expression()?;
                            pairs.push((key, value));
                        }
                        self.consume(Token::RBrace, "Expected '}' after dict items")?;
                        Ok(Expr::Dict(pairs))
                    }
                } else {
                    // Set or set comprehension
                    let mut items = vec![first_expr.clone()];
                    
                    // Check if this is a set comprehension
                    if self.match_token(&[Token::KwFor]) {
                        // Set comprehension
                        let generators = self.parse_comprehension()?;
                        self.consume(Token::RBrace, "Expected '}' after set comprehension")?;
                        Ok(Expr::SetComp {
                            element: Box::new(first_expr.clone()),
                            generators,
                        })
                    } else {
                        // Regular set
                        while self.match_token(&[Token::Comma]) {
                            if self.check(&Token::RBrace) {
                                break;
                            }
                            items.push(self.expression()?);
                        }
                        self.consume(Token::RBrace, "Expected '}' after set items")?;
                        Ok(Expr::Set(items))
                    }
                }
            }
        } else if self.match_token(&[Token::Ellipsis]) {
            Ok(Expr::Literal(Literal::Ellipsis))
        } else {
            Err(ParseError::UnexpectedToken {
                expected: "expression".to_string(),
                found: format!("{:?}", self.peek().token),
            })
        }
    }

    // Helper methods
    fn consume(&mut self, token: Token, message: &str) -> Result<&TokenInfo, ParseError> {
        if self.check(&token) {
            Ok(self.advance())
        } else {
            Err(ParseError::UnexpectedToken {
                expected: message.to_string(),
                found: format!("{:?}", self.peek().token),
            })
        }
    }

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
            return false;
        }
        &self.peek().token == token
    }

    fn advance(&mut self) -> &TokenInfo {
        if !self.is_at_end() {
            self.current += 1;
        }
        self.previous()
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.tokens.len() || matches!(self.peek().token, Token::Eof)
    }

    fn peek(&self) -> &TokenInfo {
        &self.tokens[self.current]
    }

    fn previous(&self) -> &TokenInfo {
        &self.tokens[self.current - 1]
    }

    fn consume_identifier(&mut self) -> Result<String, ParseError> {
        match &self.peek().token {
            Token::Identifier(name) => {
                let name = name.clone();
                self.advance();
                Ok(name)
            }
            _ => Err(ParseError::UnexpectedToken {
                expected: "identifier".to_string(),
                found: format!("{:?}", self.peek().token),
            }),
        }
    }

    /// Check if the current token is a number (Int or Float)
    fn is_number_token(&self) -> bool {
        matches!(self.peek().token, Token::Int(_) | Token::Float(_))
    }

    /// Check if the current token is a string literal
    fn is_string_token(&self) -> bool {
        matches!(self.peek().token, Token::StringLit(_) | Token::FString(_))
    }

    /// Check if the current token is a docstring
    fn is_docstring_token(&self) -> bool {
        matches!(self.peek().token, Token::DocString(_))
    }

    /// Check if the current token is an identifier
    fn is_identifier_token(&self) -> bool {
        matches!(self.peek().token, Token::Identifier(_))
    }

    fn consume_identifier_or_keyword(&mut self) -> Result<String, ParseError> {
        match &self.peek().token {
            Token::Identifier(name) => {
                let name = name.clone();
                self.advance();
                Ok(name)
            }
            // Allow keywords to be used as identifiers in certain contexts
            token if self.is_keyword_as_identifier() => {
                let name = format!("{:?}", token);
                self.advance();
                Ok(name)
            }
            _ => Err(ParseError::UnexpectedToken {
                expected: "identifier".to_string(),
                found: format!("{:?}", self.peek().token),
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
        let prev_token = &self.previous().token;
        let op = match prev_token {
            Token::PlusEq => BinaryOp::Add,
            Token::MinusEq => BinaryOp::Sub,
            Token::StarEq => BinaryOp::Mul,
            Token::SlashEq => BinaryOp::Div,
            Token::PercentEq => BinaryOp::Mod,
            Token::PowerEq => BinaryOp::Pow,
            Token::FloorDivEq => BinaryOp::FloorDiv,
            _ => {
                return Err(ParseError::UnexpectedToken {
                    expected: "compound assignment operator".to_string(),
                    found: format!("{:?}", prev_token),
                });
            }
        };
        let value = self.expression()?;
        self.match_token(&[Token::Semicolon, Token::Newline]);
        
        let binop_expr = Expr::BinaryOp {
            left: Box::new(target.clone()),
            op,
            right: Box::new(value),
        };
        
        match target {
            Expr::Identifier(name) => Ok(Statement::VariableDef {
                name,
                type_annotation: None,
                value: Some(binop_expr),
            }),
            Expr::Attribute { object, name } => Ok(Statement::AttributeAssignment {
                object: *object,
                name,
                value: binop_expr,
            }),
            Expr::Subscript { object, index } => Ok(Statement::SubscriptAssignment {
                object: *object,
                index: *index,
                value: binop_expr,
            }),
            _ => Err(ParseError::InvalidSyntax {
                message: "Invalid target for compound assignment".to_string(),
            }),
        }
    }

    fn variable_def(&mut self, target: Expr) -> Result<Statement, ParseError> {
        let value = self.expression()?;
        self.match_token(&[Token::Semicolon, Token::Newline]);

        match target {
            Expr::Identifier(name) => Ok(Statement::VariableDef {
                name,
                type_annotation: None,
                value: Some(value),
            }),
            Expr::Attribute { object, name } => Ok(Statement::AttributeAssignment {
                object: *object,
                name,
                value,
            }),
            Expr::Subscript { object, index } => Ok(Statement::SubscriptAssignment {
                object: *object,
                index: *index,
                value,
            }),
            Expr::Tuple(items) => {
                // Tuple unpacking: a, b = (1, 2)
                let mut targets = Vec::new();
                for item in items {
                    match item {
                        Expr::Identifier(name) => targets.push(name),
                        _ => return Err(ParseError::InvalidSyntax {
                            message: "Tuple unpacking targets must be identifiers".to_string(),
                        }),
                    }
                }
                Ok(Statement::TupleUnpack { targets, value })
            },
            _ => Err(ParseError::InvalidSyntax {
                message: "Invalid target for assignment".to_string(),
            }),
        }
    }

    fn block(&mut self) -> Result<Vec<Statement>, ParseError> {
        // The colon has already been consumed by the calling function
        // Handle single-line block
        if !self.check(&Token::Newline) && !self.check(&Token::Eof) {
            let stmt = self.statement()?;
            return Ok(vec![stmt]);
        }
        
        // Multi-line block
        self.consume(Token::Newline, "Expected newline after ':'")?;
        self.consume(Token::Indent, "Expected indented block")?;
        
        let mut statements = Vec::new();
        while !self.check(&Token::Dedent) && !self.is_at_end() {
            // Skip newlines within blocks
            if self.check(&Token::Newline) {
                self.advance();
                continue;
            }
            statements.push(self.statement()?);
        }
        
        self.consume(Token::Dedent, "Expected dedent after block")?;
        Ok(statements)
    }

    fn finish_call(&mut self, func: Expr) -> Result<Expr, ParseError> {
        let mut args = Vec::new();
        let mut kwargs = Vec::new();
        
        if !self.check(&Token::RParen) {
            loop {
                // Check for starred expressions (*args, **kwargs)
                if self.match_token(&[Token::Star]) {
                    if self.match_token(&[Token::Star]) {
                        // **kwargs in function call
                        let value = self.expression()?;
                        // For now, we'll add it as a special starred expression
                        // In a full implementation, we would need to handle this properly in the compiler/VM
                        args.push(Expr::Starred(Box::new(value)));
                    } else {
                        // *args in function call
                        let value = self.expression()?;
                        args.push(Expr::Starred(Box::new(value)));
                    }
                } else if let Token::Identifier(_) = &self.peek().token {
                    let checkpoint = self.current;
                    let name = self.consume_identifier()?;
                    
                    if self.match_token(&[Token::Assign]) {
                        // This is a keyword argument
                        let value = self.expression()?;
                        kwargs.push((name, value));
                    } else {
                        // This is a positional argument, backtrack
                        self.current = checkpoint;
                        args.push(self.expression()?);
                    }
                } else {
                    // Positional argument
                    args.push(self.expression()?);
                }
                
                if !self.match_token(&[Token::Comma]) {
                    break;
                }
            }
        }
        
        self.consume(Token::RParen, "Expected ')' after arguments")?;
        
        Ok(Expr::Call {
            func: Box::new(func),
            args,
            kwargs,
        })
    }

    fn finish_method_call(&mut self, object: Expr, method: String) -> Result<Expr, ParseError> {
        let mut args = Vec::new();
        let mut kwargs = Vec::new();
        
        if !self.check(&Token::RParen) {
            loop {
                // Check for starred expressions (*args, **kwargs)
                if self.match_token(&[Token::Star]) {
                    if self.match_token(&[Token::Star]) {
                        // **kwargs in function call
                        let value = self.expression()?;
                        // For now, we'll add it as a special starred expression
                        // In a full implementation, we would need to handle this properly in the compiler/VM
                        args.push(Expr::Starred(Box::new(value)));
                    } else {
                        // *args in function call
                        let value = self.expression()?;
                        args.push(Expr::Starred(Box::new(value)));
                    }
                } else if let Token::Identifier(_) = &self.peek().token {
                    let checkpoint = self.current;
                    let name = self.consume_identifier()?;
                    
                    if self.match_token(&[Token::Assign]) {
                        // This is a keyword argument
                        let value = self.expression()?;
                        kwargs.push((name, value));
                    } else {
                        // This is a positional argument, backtrack
                        self.current = checkpoint;
                        args.push(self.expression()?);
                    }
                } else {
                    // Positional argument
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

    fn parse_comprehension(&mut self) -> Result<Vec<Comprehension>, ParseError> {
        let mut generators = Vec::new();
        
        loop {
            let is_async = self.match_token(&[Token::KwAsync]);
            
            self.consume(Token::KwFor, "Expected 'for' in comprehension")?;
            let target = self.consume_identifier()?;
            self.consume(Token::KwIn, "Expected 'in' in comprehension")?;
            let iter = self.expression()?;
            
            let mut ifs = Vec::new();
            while self.match_token(&[Token::KwIf]) {
                ifs.push(self.expression()?);
            }
            
            generators.push(Comprehension {
                target,
                iter,
                ifs,
                is_async,
            });
            
            // Check if there's another for clause
            if !self.check(&Token::KwFor) && !self.check(&Token::KwAsync) {
                break;
            }
        }
        
        Ok(generators)
    }

    /// Parse an f-string into FormatPart components
    fn parse_fstring(&mut self, fstring_content: &str) -> Result<Vec<crate::ast::FormatPart>, ParseError> {
        let mut parts = Vec::new();
        let mut current = String::new();
        let mut chars = fstring_content.chars().peekable();
        let mut i = 0;
        
        while let Some(ch) = chars.next() {
            match ch {
                '{' => {
                    // Flush any accumulated string content
                    if !current.is_empty() {
                        parts.push(crate::ast::FormatPart::String(current.clone()));
                        current.clear();
                    }
                    
                    // Check for escaped brace
                    if chars.peek() == Some(&'{') {
                        chars.next(); // consume the second {
                        current.push('{');
                        continue;
                    }
                    
                    // Parse expression inside braces
                    let mut expr_content = String::new();
                    let mut brace_count = 1;
                    
                    while let Some(inner_ch) = chars.next() {
                        match inner_ch {
                            '{' => {
                                brace_count += 1;
                                expr_content.push(inner_ch);
                            }
                            '}' => {
                                brace_count -= 1;
                                if brace_count == 0 {
                                    break;
                                }
                                expr_content.push(inner_ch);
                            }
                            _ => {
                                expr_content.push(inner_ch);
                            }
                        }
                    }
                    
                    if brace_count != 0 {
                        return Err(ParseError::InvalidSyntax {
                            message: "Unmatched braces in f-string".to_string(),
                        });
                    }
                    
                    // Parse the expression content using a temporary lexer and parser
                    let expr_content = expr_content.trim();
                    if !expr_content.is_empty() {
                        // Use the lexer to tokenize the expression
                        use crate::lexer::Lexer;

                        let lexer = Lexer::new(expr_content);
                        match lexer.collect::<Result<Vec<_>, String>>() {
                            Ok(tokens) => {
                                // Create a temporary parser for the expression
                                let mut expr_parser = Parser::new(tokens);
                                match expr_parser.expression() {
                                    Ok(expr) => {
                                        parts.push(crate::ast::FormatPart::Expression {
                                            expr,
                                            format_spec: None,
                                            conversion: None,
                                        });
                                    }
                                    Err(_) => {
                                        // If parsing fails, treat as identifier (backwards compatibility)
                                        parts.push(crate::ast::FormatPart::Expression {
                                            expr: Expr::Identifier(expr_content.to_string()),
                                            format_spec: None,
                                            conversion: None,
                                        });
                                    }
                                }
                            }
                            Err(_) => {
                                // If lexing fails, treat as identifier (backwards compatibility)
                                parts.push(crate::ast::FormatPart::Expression {
                                    expr: Expr::Identifier(expr_content.to_string()),
                                    format_spec: None,
                                    conversion: None,
                                });
                            }
                        }
                    }
                }
                '}' => {
                    // Check for escaped brace
                    if chars.peek() == Some(&'}') {
                        chars.next(); // consume the second }
                        current.push('}');
                    } else {
                        return Err(ParseError::InvalidSyntax {
                            message: "Single '}' in f-string".to_string(),
                        });
                    }
                }
                _ => {
                    current.push(ch);
                }
            }
            i += 1;
        }
        
        // Flush any remaining string content
        if !current.is_empty() {
            parts.push(crate::ast::FormatPart::String(current));
        }
        
        Ok(parts)
    }

    fn type_annotation(&mut self) -> Result<Type, ParseError> {
        // Simplified type annotation parsing
        // In a full implementation, this would parse complex type annotations
        let name = self.consume_identifier()?;
        Ok(Type::Simple(name))
    }

    fn extract_docstring(&self, body: &[Statement]) -> Option<String> {
        if let Some(Statement::Expression(Expr::DocString(doc))) = body.first() {
            Some(doc.clone())
        } else {
            None
        }
    }

    fn import_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwImport, "Expected 'import' or 'shigoda'")?;
        let module = self.consume_identifier()?;
        let alias = if self.match_token(&[Token::KwAs]) {
            Some(self.consume_identifier()?)
        } else {
            None
        };
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Import { module, alias })
    }

    fn from_import_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwFrom, "Expected 'from' or 'daga'")?;
        let module = self.consume_identifier()?;
        self.consume(Token::KwImport, "Expected 'import' or 'shigoda'")?;
        
        let mut names = Vec::new();
        if self.match_token(&[Token::Star]) {
            // from module import *
            names.push(("*".to_string(), None));
        } else {
            loop {
                let name = self.consume_identifier()?;
                let alias = if self.match_token(&[Token::KwAs]) {
                    Some(self.consume_identifier()?)
                } else {
                    None
                };
                names.push((name, alias));
                
                if !self.match_token(&[Token::Comma]) {
                    break;
                }
            }
        }
        
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::FromImport { module, names })
    }

    fn extern_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwExtern, "Expected 'extern' or 'waje'")?;
        let name = self.consume_identifier()?;
        self.consume(Token::Colon, "Expected ':' after extern name")?;
        // For now, we'll just consume the signature as a string
        // In a full implementation, this would be parsed as a type signature
        let signature = if let Token::StringLit(s) = &self.peek().token {
            let s = s.clone();
            self.advance();
            s
        } else {
            return Err(ParseError::UnexpectedToken {
                expected: "string literal".to_string(),
                found: format!("{:?}", self.peek().token),
            });
        };
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Extern { name, signature })
    }

    fn export_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwExport, "Expected 'export' or 'fitar'")?;
        let name = self.consume_identifier()?;
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Export { name })
    }

    fn return_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwReturn, "Expected 'return' or 'maido'")?;
        let value = if self.check(&Token::Newline) || self.check(&Token::Semicolon) || self.is_at_end() {
            None
        } else {
            Some(self.expression()?)
        };
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Return(value))
    }

    fn break_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwBreak, "Expected 'break' or 'tsaya'")?;
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Break)
    }

    fn continue_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwContinue, "Expected 'continue' or 'cigaba'")?;
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Continue)
    }

    fn raise_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwRaise, "Expected 'raise', 'throw', or 'jefa'")?;
        let value = if self.check(&Token::Newline) || self.check(&Token::Semicolon) || self.is_at_end() {
            None
        } else {
            Some(self.expression()?)
        };
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Raise(value))
    }

    fn with_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwWith, "Expected 'with' or 'tare'")?;
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
        self.consume(Token::KwDel, "Expected 'del' or 'share'")?;
        let mut targets = Vec::new();
        loop {
            targets.push(self.expression()?);
            if !self.match_token(&[Token::Comma]) {
                break;
            }
        }
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Del { targets })
    }

    fn assert_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwAssert, "Expected 'assert' or 'tabbatar'")?;
        let condition = self.expression()?;
        let message = if self.match_token(&[Token::Comma]) {
            Some(self.expression()?)
        } else {
            None
        };
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Assert { condition, message })
    }

    fn global_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwGlobal, "Expected 'global' or 'duniya'")?;
        let mut names = Vec::new();
        loop {
            names.push(self.consume_identifier()?);
            if !self.match_token(&[Token::Comma]) {
                break;
            }
        }
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Global { names })
    }

    fn nonlocal_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwNonlocal, "Expected 'nonlocal' or 'ba_gida'")?;
        let mut names = Vec::new();
        loop {
            names.push(self.consume_identifier()?);
            if !self.match_token(&[Token::Comma]) {
                break;
            }
        }
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Nonlocal { names })
    }

    fn pass_statement(&mut self) -> Result<Statement, ParseError> {
        self.consume(Token::KwPass, "Expected 'pass' or 'wuce'")?;
        self.match_token(&[Token::Semicolon, Token::Newline]);
        Ok(Statement::Pass)
    }

    fn decorated_statement(&mut self) -> Result<Statement, ParseError> {
        let mut decorators = Vec::new();
        
        // Parse decorators
        while self.match_token(&[Token::At]) {
            let decorator = self.expression()?;
            decorators.push(decorator);
            self.consume(Token::Newline, "Expected newline after decorator")?;
        }
        
        // Parse the decorated statement
        let mut stmt = self.statement()?;
        
        // Attach decorators to the statement
        match &mut stmt {
            Statement::FunctionDef { decorators: ref mut func_decorators, .. } => {
                *func_decorators = decorators;
            }
            Statement::ClassDef { decorators: ref mut class_decorators, .. } => {
                *class_decorators = decorators;
            }
            _ => {
                return Err(ParseError::InvalidSyntax {
                    message: "Only functions and classes can be decorated".to_string(),
                });
            }
        }
        
        Ok(stmt)
    }

}
