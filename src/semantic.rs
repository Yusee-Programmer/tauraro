use crate::ast::*;
use std::collections::HashMap;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum SemanticError {
    #[error("Type error: {message}")]
    TypeError { message: String },
    #[error("Undefined variable: {name}")]
    UndefinedVariable { name: String },
    #[error("Duplicate definition: {name}")]
    DuplicateDefinition { name: String },
    #[error("Invalid operation: {operation}")]
    InvalidOperation { operation: String },
    #[error("Async error: {message}")]
    AsyncError { message: String },
    #[error("Pattern matching error: {message}")]
    PatternError { message: String },
    #[error("Decorator error: {message}")]
    DecoratorError { message: String },
    #[error("ModuleNotFoundError: No module named '{name}'")]
    ModuleNotFoundError { name: String },
}

#[derive(Debug, Clone)]
pub struct TypeInfo {
    pub name: String,
    pub fields: HashMap<String, Type>,
    pub methods: HashMap<String, FunctionType>,
    pub is_async: bool,
    pub decorators: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct FunctionType {
    pub params: Vec<Type>,
    pub return_type: Box<Type>,
    pub is_async: bool,
    pub is_generator: bool,
}

#[derive(Debug)]
pub struct SymbolTable {
    scopes: Vec<HashMap<String, TypeInfo>>,
    strict_types: bool,
    async_context: bool,
    loop_context: bool,
    function_context: bool,
}

impl SymbolTable {
    pub fn new(strict_types: bool) -> Self {
        Self {
            scopes: vec![HashMap::new()],
            strict_types,
            async_context: false,
            loop_context: false,
            function_context: false,
        }
    }

    pub fn enter_scope(&mut self) {
        self.scopes.push(HashMap::new());
    }

    pub fn exit_scope(&mut self) {
        self.scopes.pop();
    }

    pub fn enter_async_context(&mut self) {
        self.async_context = true;
    }

    pub fn exit_async_context(&mut self) {
        self.async_context = false;
    }

    pub fn enter_loop_context(&mut self) {
        self.loop_context = true;
    }

    pub fn exit_loop_context(&mut self) {
        self.loop_context = false;
    }

    pub fn enter_function_context(&mut self) {
        self.function_context = true;
    }

    pub fn exit_function_context(&mut self) {
        self.function_context = false;
    }

    pub fn define(&mut self, name: String, type_info: TypeInfo) -> Result<(), SemanticError> {
        let current_scope = self.scopes.last_mut().unwrap();
        // Allow redefinition in the same scope for dynamic typing
        current_scope.insert(name, type_info);
        Ok(())
    }

    pub fn lookup(&self, name: &str) -> Option<&TypeInfo> {
        for scope in self.scopes.iter().rev() {
            if let Some(type_info) = scope.get(name) {
                return Some(type_info);
            }
        }
        None
    }

    pub fn is_in_async_context(&self) -> bool {
        self.async_context
    }

    pub fn is_in_loop_context(&self) -> bool {
        self.loop_context
    }

    pub fn is_in_function_context(&self) -> bool {
        self.function_context
    }
}

pub struct Analyzer {
    symbol_table: SymbolTable,
    errors: Vec<SemanticError>,
}

impl Analyzer {
    pub fn new(strict_types: bool) -> Self {
        Self {
            symbol_table: SymbolTable::new(strict_types),
            errors: Vec::new(),
        }
    }

    pub fn analyze(&mut self, program: Program) -> Result<Program, Vec<SemanticError>> {
        // Add built-in types and functions
        self.add_builtins();
        
        // Analyze the program
        let analyzed_program = self.analyze_program(program);
        
        if self.errors.is_empty() {
            Ok(analyzed_program)
        } else {
            Err(std::mem::take(&mut self.errors))
        }
    }

    fn add_builtins(&mut self) {
        let builtins = [
            ("print", TypeInfo {
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("fitoda", TypeInfo { // Hausa alias for print
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("len", TypeInfo {
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("tsawon", TypeInfo { // Hausa alias for len
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("input", TypeInfo {
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("sami", TypeInfo { // Hausa alias for input
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("type", TypeInfo {
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("nauin", TypeInfo { // Hausa alias for type
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("range", TypeInfo {
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
            ("jeri", TypeInfo { // Hausa alias for range
                name: "function".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            }),
        ];

        for (name, type_info) in builtins {
            self.symbol_table.define(name.to_string(), type_info).unwrap();
        }
    }

    fn analyze_program(&mut self, program: Program) -> Program {
        let mut statements = Vec::new();
        
        for stmt in program.statements {
            match self.analyze_statement(stmt) {
                Ok(analyzed_stmt) => statements.push(analyzed_stmt),
                Err(err) => {
                    self.errors.push(err);
                }
            }
        }
        
        Program { statements }
    }

    fn analyze_statement(&mut self, stmt: Statement) -> Result<Statement, SemanticError> {
        match stmt {
            Statement::Expression(expr) => {
                let analyzed_expr = self.analyze_expression(expr)?;
                Ok(Statement::Expression(analyzed_expr))
            }
            Statement::VariableDef { name, type_annotation, value } => {
                let analyzed_value = if let Some(val) = value {
                    Some(self.analyze_expression(val)?)
                } else {
                    None
                };
                
                let type_info = TypeInfo {
                    name: type_annotation.as_ref()
                        .map(|t| self.type_to_string(t))
                        .unwrap_or_else(|| {
                            if let Some(ref val) = analyzed_value {
                                self.infer_type(val).map(|t| self.type_to_string(&t)).unwrap_or("any".to_string())
                            } else {
                                "any".to_string()
                            }
                        }),
                    fields: HashMap::new(),
                    methods: HashMap::new(),
                    is_async: false,
                    decorators: Vec::new(),
                };
                
                self.symbol_table.define(name.clone(), type_info)?;
                Ok(Statement::VariableDef { name, type_annotation, value: analyzed_value })
            }
            Statement::AttributeAssignment { object, name, value } => {
                let analyzed_object = self.analyze_expression(object)?;
                let analyzed_value = self.analyze_expression(value)?;
                Ok(Statement::AttributeAssignment {
                    object: analyzed_object,
                    name,
                    value: analyzed_value,
                })
            }
            Statement::FunctionDef { name, params, return_type, body, is_async, decorators, docstring: _ } => {
                // Analyze decorators first
                let analyzed_decorators = decorators.into_iter()
                    .map(|d| self.analyze_expression(d))
                    .collect::<Result<Vec<_>, _>>()?;

                self.symbol_table.enter_scope();
                self.symbol_table.enter_function_context();
                
                if is_async {
                    self.symbol_table.enter_async_context();
                }

                // Define parameters in the function scope
                for param in &params {
                    let param_type = TypeInfo {
                        name: param.type_annotation.as_ref()
                            .map(|t| self.type_to_string(t))
                            .unwrap_or("any".to_string()),
                        fields: HashMap::new(),
                        methods: HashMap::new(),
                        is_async: false,
                        decorators: Vec::new(),
                    };
                    self.symbol_table.define(param.name.clone(), param_type)?;
                }

                let analyzed_body = body.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;

                if is_async {
                    self.symbol_table.exit_async_context();
                }
                
                self.symbol_table.exit_function_context();
                self.symbol_table.exit_scope();

                // Define the function in the outer scope
                let func_type = TypeInfo {
                    name: "function".to_string(),
                    fields: HashMap::new(),
                    methods: HashMap::new(),
                    is_async,
                    decorators: analyzed_decorators.iter()
                        .filter_map(|d| match d {
                            Expr::Identifier(name) => Some(name.clone()),
                            _ => None,
                        })
                        .collect(),
                };
                
                self.symbol_table.define(name.clone(), func_type)?;
                
                Ok(Statement::FunctionDef { 
                    name, 
                    params, 
                    return_type, 
                    body: analyzed_body, 
                    is_async, 
                    decorators: analyzed_decorators,
                    docstring: None, // TODO: Extract docstring during semantic analysis
                })
            }
            Statement::ClassDef { name, bases, body, decorators, metaclass, docstring: _ } => {
                let analyzed_decorators = decorators.into_iter()
                    .map(|d| self.analyze_expression(d))
                    .collect::<Result<Vec<_>, _>>()?;

                let analyzed_bases = bases.into_iter()
                    .map(|b| self.analyze_expression(b))
                    .collect::<Result<Vec<_>, _>>()?;

                let analyzed_metaclass = if let Some(mc) = metaclass {
                    Some(self.analyze_expression(mc)?)
                } else {
                    None
                };

                self.symbol_table.enter_scope();
                
                let analyzed_body = body.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;
                
                self.symbol_table.exit_scope();

                let class_type = TypeInfo {
                    name: "class".to_string(),
                    fields: HashMap::new(),
                    methods: HashMap::new(),
                    is_async: false,
                    decorators: analyzed_decorators.iter()
                        .filter_map(|d| match d {
                            Expr::Identifier(name) => Some(name.clone()),
                            _ => None,
                        })
                        .collect(),
                };
                
                self.symbol_table.define(name.clone(), class_type)?;
                
                Ok(Statement::ClassDef { 
                    name, 
                    bases: analyzed_bases, 
                    body: analyzed_body, 
                    decorators: analyzed_decorators,
                    metaclass: analyzed_metaclass,
                    docstring: None, // TODO: Extract docstring during semantic analysis
                })
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                let analyzed_condition = self.analyze_expression(condition)?;
                
                let analyzed_then = then_branch.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;

                let analyzed_elif = elif_branches.into_iter()
                    .map(|(cond, body)| {
                        let analyzed_cond = self.analyze_expression(cond)?;
                        let analyzed_body = body.into_iter()
                            .map(|s| self.analyze_statement(s))
                            .collect::<Result<Vec<_>, _>>()?;
                        Ok((analyzed_cond, analyzed_body))
                    })
                    .collect::<Result<Vec<_>, _>>()?;

                let analyzed_else = if let Some(else_body) = else_branch {
                    Some(else_body.into_iter()
                        .map(|s| self.analyze_statement(s))
                        .collect::<Result<Vec<_>, _>>()?)
                } else {
                    None
                };

                Ok(Statement::If {
                    condition: analyzed_condition,
                    then_branch: analyzed_then,
                    elif_branches: analyzed_elif,
                    else_branch: analyzed_else,
                })
            }
            Statement::While { condition, body, else_branch } => {
                let analyzed_condition = self.analyze_expression(condition)?;
                
                self.symbol_table.enter_loop_context();
                let analyzed_body = body.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;
                self.symbol_table.exit_loop_context();

                let analyzed_else = if let Some(else_body) = else_branch {
                    Some(else_body.into_iter()
                        .map(|s| self.analyze_statement(s))
                        .collect::<Result<Vec<_>, _>>()?)
                } else {
                    None
                };

                Ok(Statement::While {
                    condition: analyzed_condition,
                    body: analyzed_body,
                    else_branch: analyzed_else,
                })
            }
            Statement::For { variable, iterable, body, else_branch } => {
                let analyzed_iterable = self.analyze_expression(iterable)?;
                
                self.symbol_table.enter_scope();
                self.symbol_table.enter_loop_context();
                
                // Define the loop variable
                let var_type = TypeInfo {
                    name: "any".to_string(), // Could be inferred from iterable type
                    fields: HashMap::new(),
                    methods: HashMap::new(),
                    is_async: false,
                    decorators: Vec::new(),
                };
                self.symbol_table.define(variable.clone(), var_type)?;
                
                let analyzed_body = body.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;
                
                self.symbol_table.exit_loop_context();
                self.symbol_table.exit_scope();

                let analyzed_else = if let Some(else_body) = else_branch {
                    Some(else_body.into_iter()
                        .map(|s| self.analyze_statement(s))
                        .collect::<Result<Vec<_>, _>>()?)
                } else {
                    None
                };

                Ok(Statement::For {
                    variable,
                    iterable: analyzed_iterable,
                    body: analyzed_body,
                    else_branch: analyzed_else,
                })
            }
            Statement::Match { value, cases } => {
                let analyzed_value = self.analyze_expression(value)?;
                
                let analyzed_cases = cases.into_iter()
                    .map(|case| self.analyze_match_case(case))
                    .collect::<Result<Vec<_>, _>>()?;

                Ok(Statement::Match {
                    value: analyzed_value,
                    cases: analyzed_cases,
                })
            }
            Statement::Try { body, except_handlers, else_branch, finally } => {
                let analyzed_body = body.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;

                let analyzed_handlers = except_handlers.into_iter()
                    .map(|handler| self.analyze_except_handler(handler))
                    .collect::<Result<Vec<_>, _>>()?;

                let analyzed_else = if let Some(else_body) = else_branch {
                    Some(else_body.into_iter()
                        .map(|s| self.analyze_statement(s))
                        .collect::<Result<Vec<_>, _>>()?)
                } else {
                    None
                };

                let analyzed_finally = if let Some(finally_body) = finally {
                    Some(finally_body.into_iter()
                        .map(|s| self.analyze_statement(s))
                        .collect::<Result<Vec<_>, _>>()?)
                } else {
                    None
                };

                Ok(Statement::Try {
                    body: analyzed_body,
                    except_handlers: analyzed_handlers,
                    else_branch: analyzed_else,
                    finally: analyzed_finally,
                })
            }
            Statement::Return(expr) => {
                if !self.symbol_table.is_in_function_context() {
                    return Err(SemanticError::InvalidOperation {
                        operation: "return outside function".to_string(),
                    });
                }
                
                let analyzed_expr = if let Some(e) = expr {
                    Some(self.analyze_expression(e)?)
                } else {
                    None
                };
                
                Ok(Statement::Return(analyzed_expr))
            }
            Statement::Break => {
                if !self.symbol_table.is_in_loop_context() {
                    return Err(SemanticError::InvalidOperation {
                        operation: "break outside loop".to_string(),
                    });
                }
                Ok(Statement::Break)
            }
            Statement::Continue => {
                if !self.symbol_table.is_in_loop_context() {
                    return Err(SemanticError::InvalidOperation {
                        operation: "continue outside loop".to_string(),
                    });
                }
                Ok(Statement::Continue)
            }
            Statement::Raise(expr) => {
                let analyzed_expr = if let Some(e) = expr {
                    Some(self.analyze_expression(e)?)
                } else {
                    None
                };
                Ok(Statement::Raise(analyzed_expr))
            }
            Statement::With { context, alias, body } => {
                let analyzed_context = self.analyze_expression(context)?;
                
                self.symbol_table.enter_scope();
                
                // If there's an alias, define it in the scope
                if let Some(ref alias_name) = alias {
                    let context_type = TypeInfo {
                        name: "any".to_string(), // Could be inferred from context type
                        fields: HashMap::new(),
                        methods: HashMap::new(),
                        is_async: false,
                        decorators: Vec::new(),
                    };
                    self.symbol_table.define(alias_name.clone(), context_type)?;
                }
                
                let analyzed_body = body.into_iter()
                    .map(|s| self.analyze_statement(s))
                    .collect::<Result<Vec<_>, _>>()?;
                
                self.symbol_table.exit_scope();

                Ok(Statement::With {
                    context: analyzed_context,
                    alias,
                    body: analyzed_body,
                })
            }
            Statement::Async(stmt) => {
                self.symbol_table.enter_async_context();
                let analyzed_stmt = self.analyze_statement(*stmt)?;
                self.symbol_table.exit_async_context();
                Ok(Statement::Async(Box::new(analyzed_stmt)))
            }
            Statement::Await(expr) => {
                if !self.symbol_table.is_in_async_context() {
                    return Err(SemanticError::AsyncError {
                        message: "await outside async context".to_string(),
                    });
                }
                let analyzed_expr = self.analyze_expression(expr)?;
                Ok(Statement::Expression(Expr::Await(Box::new(analyzed_expr))))
            }
            // Handle other statements...
            _ => Ok(stmt), // For now, pass through other statements
        }
    }

    fn analyze_match_case(&mut self, case: MatchCase) -> Result<MatchCase, SemanticError> {
        self.symbol_table.enter_scope();
        
        // Analyze pattern and bind variables
        self.analyze_pattern(&case.pattern)?;
        
        let analyzed_guard = if let Some(guard) = case.guard {
            Some(self.analyze_expression(guard)?)
        } else {
            None
        };
        
        let analyzed_body = case.body.into_iter()
            .map(|s| self.analyze_statement(s))
            .collect::<Result<Vec<_>, _>>()?;
        
        self.symbol_table.exit_scope();
        
        Ok(MatchCase {
            pattern: case.pattern,
            guard: analyzed_guard,
            body: analyzed_body,
        })
    }

    fn analyze_pattern(&mut self, pattern: &Pattern) -> Result<(), SemanticError> {
        match pattern {
            Pattern::Variable(name) => {
                let var_type = TypeInfo {
                    name: "any".to_string(),
                    fields: HashMap::new(),
                    methods: HashMap::new(),
                    is_async: false,
                    decorators: Vec::new(),
                };
                self.symbol_table.define(name.clone(), var_type)?;
            }
            Pattern::Tuple(patterns) | Pattern::List(patterns) => {
                for p in patterns {
                    self.analyze_pattern(p)?;
                }
            }
            Pattern::Dict(pairs) => {
                for (key_pattern, value_pattern) in pairs {
                    self.analyze_pattern(key_pattern)?;
                    self.analyze_pattern(value_pattern)?;
                }
            }
            Pattern::As { pattern, name } => {
                self.analyze_pattern(pattern)?;
                let var_type = TypeInfo {
                    name: "any".to_string(),
                    fields: HashMap::new(),
                    methods: HashMap::new(),
                    is_async: false,
                    decorators: Vec::new(),
                };
                self.symbol_table.define(name.clone(), var_type)?;
            }
            Pattern::Or(patterns) => {
                for p in patterns {
                    self.analyze_pattern(p)?;
                }
            }
            _ => {} // Literal, Wildcard, Class patterns don't bind variables
        }
        Ok(())
    }

    fn analyze_except_handler(&mut self, handler: ExceptHandler) -> Result<ExceptHandler, SemanticError> {
        let analyzed_exception_type = if let Some(exc_type) = handler.exception_type {
            Some(self.analyze_expression(exc_type)?)
        } else {
            None
        };

        self.symbol_table.enter_scope();
        
        // If there's a name, define the exception variable
        if let Some(ref name) = handler.name {
            let exc_type = TypeInfo {
                name: "Exception".to_string(),
                fields: HashMap::new(),
                methods: HashMap::new(),
                is_async: false,
                decorators: Vec::new(),
            };
            self.symbol_table.define(name.clone(), exc_type)?;
        }
        
        let analyzed_body = handler.body.into_iter()
            .map(|s| self.analyze_statement(s))
            .collect::<Result<Vec<_>, _>>()?;
        
        self.symbol_table.exit_scope();

        Ok(ExceptHandler {
            exception_type: analyzed_exception_type,
            name: handler.name,
            body: analyzed_body,
        })
    }

    fn analyze_expression(&mut self, expr: Expr) -> Result<Expr, SemanticError> {
        match expr {
            Expr::Identifier(name) => {
                if self.symbol_table.lookup(&name).is_none() {
                    return Err(SemanticError::UndefinedVariable { name });
                }
                Ok(Expr::Identifier(name))
            }
            Expr::BinaryOp { left, op, right } => {
                let analyzed_left = self.analyze_expression(*left)?;
                let analyzed_right = self.analyze_expression(*right)?;
                
                // Type checking for binary operations
                let left_type = self.infer_type(&analyzed_left)?;
                let right_type = self.infer_type(&analyzed_right)?;
                
                if !self.is_valid_operation(&op, &left_type, &right_type) {
                    return Err(SemanticError::InvalidOperation {
                        operation: format!("{} {} {}", 
                            self.type_to_string(&left_type),
                            op_to_string(&op),
                            self.type_to_string(&right_type)
                        ),
                    });
                }
                
                Ok(Expr::BinaryOp {
                    left: Box::new(analyzed_left),
                    op,
                    right: Box::new(analyzed_right),
                })
            }
            Expr::Call { func, args, kwargs } => {
                let analyzed_func = self.analyze_expression(*func)?;
                let analyzed_args = args.into_iter()
                    .map(|arg| self.analyze_expression(arg))
                    .collect::<Result<Vec<_>, _>>()?;
                let analyzed_kwargs = kwargs.into_iter()
                    .map(|(k, v)| Ok((k, self.analyze_expression(v)?)))
                    .collect::<Result<Vec<_>, _>>()?;
                
                Ok(Expr::Call {
                    func: Box::new(analyzed_func),
                    args: analyzed_args,
                    kwargs: analyzed_kwargs,
                })
            }
            Expr::Await(expr) => {
                if !self.symbol_table.is_in_async_context() {
                    return Err(SemanticError::AsyncError {
                        message: "await outside async context".to_string(),
                    });
                }
                let analyzed_expr = self.analyze_expression(*expr)?;
                Ok(Expr::Await(Box::new(analyzed_expr)))
            }
            // Handle other expressions...
            _ => Ok(expr), // For now, pass through other expressions
        }
    }

    fn infer_type(&self, expr: &Expr) -> Result<Type, SemanticError> {
        match expr {
            Expr::Literal(lit) => Ok(self.literal_type(lit)),
            Expr::DocString(_) => Ok(Type::Simple("str".to_string())),
            Expr::Identifier(name) => {
                if let Some(type_info) = self.symbol_table.lookup(name) {
                    Ok(Type::Simple(type_info.name.clone()))
                } else {
                    Err(SemanticError::UndefinedVariable { name: name.clone() })
                }
            }
            Expr::BinaryOp { left, op, right } => {
                let left_type = self.infer_type(left)?;
                let right_type = self.infer_type(right)?;
                Ok(self.binary_op_result_type(&left_type, op, &right_type))
            }
            _ => Ok(Type::Simple("any".to_string())),
        }
    }

    fn literal_type(&self, lit: &Literal) -> Type {
        match lit {
            Literal::Int(_) => Type::Simple("int".to_string()),
            Literal::Float(_) => Type::Simple("float".to_string()),
            Literal::String(_) => Type::Simple("str".to_string()),
            Literal::Bool(_) => Type::Simple("bool".to_string()),
            Literal::None => Type::Simple("None".to_string()),
            Literal::Bytes(_) => Type::Simple("bytes".to_string()),
            Literal::Complex { .. } => Type::Simple("complex".to_string()),
            Literal::Ellipsis => Type::Simple("ellipsis".to_string()),
        }
    }

    fn binary_op_result_type(&self, left: &Type, op: &BinaryOp, right: &Type) -> Type {
        match (left, op, right) {
            (Type::Simple(l), BinaryOp::Add, Type::Simple(r)) if l == "int" && r == "int" => {
                Type::Simple("int".to_string())
            }
            (Type::Simple(l), BinaryOp::Add, Type::Simple(r)) if l == "float" || r == "float" => {
                Type::Simple("float".to_string())
            }
            (Type::Simple(l), BinaryOp::Add, Type::Simple(r)) if l == "str" && r == "str" => {
                Type::Simple("str".to_string())
            }
            _ => Type::Simple("any".to_string()),
        }
    }

    fn type_to_string(&self, ty: &Type) -> String {
        match ty {
            Type::Simple(name) => name.clone(),
            Type::Generic { name, args } => {
                format!("{}[{}]", name, args.iter().map(|t| self.type_to_string(t)).collect::<Vec<_>>().join(", "))
            }
            Type::Tuple(types) => {
                format!("({})", types.iter().map(|t| self.type_to_string(t)).collect::<Vec<_>>().join(", "))
            }
            Type::Union(types) => {
                types.iter().map(|t| self.type_to_string(t)).collect::<Vec<_>>().join(" | ")
            }
            Type::Optional(ty) => {
                format!("{}?", self.type_to_string(ty))
            }
            Type::Function { params, return_type } => {
                format!("({}) -> {}", 
                    params.iter().map(|t| self.type_to_string(t)).collect::<Vec<_>>().join(", "),
                    self.type_to_string(return_type)
                )
            }
            _ => "any".to_string(),
        }
    }

    fn is_boolean_type(&self, ty: &Type) -> bool {
        matches!(ty, Type::Simple(name) if name == "bool")
    }

    fn is_valid_operation(&self, op: &BinaryOp, left: &Type, right: &Type) -> bool {
        match op {
            BinaryOp::Add => {
                matches!((left, right), 
                    (Type::Simple(l), Type::Simple(r)) if 
                        (l == "int" && r == "int") ||
                        (l == "float" || r == "float") ||
                        (l == "str" && r == "str") ||
                        (l == "list" && r == "list") ||
                        l == "any" || r == "any"  // Allow any type for dynamic typing
                )
            }
            BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div => {
                matches!((left, right), 
                    (Type::Simple(l), Type::Simple(r)) if 
                        (l == "int" && r == "int") ||
                        (l == "float" || r == "float") ||
                        l == "any" || r == "any"  // Allow any type for dynamic typing
                )
            }
            BinaryOp::Eq | BinaryOp::Ne => true, // Any types can be compared for equality
            BinaryOp::Lt | BinaryOp::Le | BinaryOp::Gt | BinaryOp::Ge => {
                matches!((left, right), 
                    (Type::Simple(l), Type::Simple(r)) if 
                        (l == "int" && r == "int") ||
                        (l == "float" || r == "float") ||
                        (l == "str" && r == "str") ||
                        l == "any" || r == "any"  // Allow any type for dynamic typing
                )
            }
            BinaryOp::And | BinaryOp::Or => true, // Any types can be used in boolean context
            _ => true, // For now, allow other operations
        }
    }
}

fn op_to_string(op: &BinaryOp) -> String {
    match op {
        BinaryOp::Add => "+".to_string(),
        BinaryOp::Sub => "-".to_string(),
        BinaryOp::Mul => "*".to_string(),
        BinaryOp::Div => "/".to_string(),
        BinaryOp::FloorDiv => "//".to_string(),
        BinaryOp::Mod => "%".to_string(),
        BinaryOp::Pow => "**".to_string(),
        BinaryOp::Eq => "==".to_string(),
        BinaryOp::Ne => "!=".to_string(),
        BinaryOp::Neq => "!=".to_string(), // Added for compatibility
        BinaryOp::Lt => "<".to_string(),
        BinaryOp::Le => "<=".to_string(),
        BinaryOp::Gt => ">".to_string(),
        BinaryOp::Ge => ">=".to_string(),
        BinaryOp::Gte => ">=".to_string(), // Added for compatibility
        BinaryOp::Lte => "<=".to_string(), // Added for compatibility
        BinaryOp::And => "and".to_string(),
        BinaryOp::Or => "or".to_string(),
        BinaryOp::BitAnd => "&".to_string(),
        BinaryOp::BitOr => "|".to_string(),
        BinaryOp::BitXor => "^".to_string(),
        BinaryOp::LShift => "<<".to_string(),
        BinaryOp::RShift => ">>".to_string(),
        BinaryOp::Is => "is".to_string(),
        BinaryOp::IsNot => "is not".to_string(),
        BinaryOp::In => "in".to_string(),
        BinaryOp::NotIn => "not in".to_string(),
        BinaryOp::MatMul => "@".to_string(),
    }
}

/// Analyze optional types for a program
pub fn analyze_optional_types(program: Program, strict: bool) -> Result<Program, Vec<SemanticError>> {
    let mut analyzer = Analyzer::new(strict);
    analyzer.analyze(program)
}
