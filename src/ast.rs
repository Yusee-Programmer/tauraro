//! COMPLETE Abstract Syntax Tree representation of TauraroLang programs
use std::fmt;
use std::rc::Rc;

/// Source code location for error reporting
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Span {
    pub start: usize,
    pub end: usize,
    pub line: usize,
    pub column: usize,
}

impl Span {
    pub fn new(start: usize, end: usize, line: usize, column: usize) -> Self {
        Self { start, end, line, column }
    }
    
    pub fn unknown() -> Self {
        Self { start: 0, end: 0, line: 0, column: 0 }
    }
}

impl Default for Span {
    fn default() -> Self {
        Self::unknown()
    }
}

/// Type representation with support for optional static typing
#[derive(Debug, Clone, PartialEq)]
pub enum Type {
    Int,
    Float,
    Bool,
    Str,
    List(Box<Type>),
    Dict(Box<Type>, Box<Type>), // key type, value type
    Tuple(Vec<Type>),
    Function(Vec<Type>, Box<Type>), // parameter types, return type
    Custom(String), // User-defined types
    Any, // Dynamic type (default)
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Type::Int => write!(f, "int"),
            Type::Float => write!(f, "float"),
            Type::Bool => write!(f, "bool"),
            Type::Str => write!(f, "str"),
            Type::List(inner) => write!(f, "list[{}]", inner),
            Type::Dict(key, value) => write!(f, "dict[{}, {}]", key, value),
            Type::Tuple(types) => {
                write!(f, "tuple[")?;
                for (i, ty) in types.iter().enumerate() {
                    if i > 0 { write!(f, ", ")?; }
                    write!(f, "{}", ty)?;
                }
                write!(f, "]")
            }
            Type::Function(params, ret) => {
                write!(f, "fn(")?;
                for (i, param) in params.iter().enumerate() {
                    if i > 0 { write!(f, ", ")?; }
                    write!(f, "{}", param)?;
                }
                write!(f, ") -> {}", ret)
            }
            Type::Custom(name) => write!(f, "{}", name),
            Type::Any => write!(f, "any"),
        }
    }
}

impl Default for Type {
    fn default() -> Self {
        Type::Any
    }
}

/// Expression nodes
#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    // Literals
    Int(i64, Span),
    Float(f64, Span),
    String(String, Span),
    Bool(bool, Span),
    None(Span),
    
    // Variables and access
    Identifier(String, Span),
    MemberAccess {
        object: Box<Expr>,
        member: String,
        span: Span,
    },
    Index {
        object: Box<Expr>,
        index: Box<Expr>,
        span: Span,
    },
    
    // Operations
    Binary {
        left: Box<Expr>,
        op: BinaryOp,
        right: Box<Expr>,
        span: Span,
    },
    Unary {
        op: UnaryOp,
        expr: Box<Expr>,
        span: Span,
    },
    Ternary {
        condition: Box<Expr>,
        then_expr: Box<Expr>,
        else_expr: Box<Expr>,
        span: Span,
    },
    
    // Function calls
    Call {
        callee: Box<Expr>,
        arguments: Vec<Expr>,
        span: Span,
    },
    
    // Collections
    List(Vec<Expr>, Span),
    Tuple(Vec<Expr>, Span),
    Dict(Vec<(Expr, Expr)>, Span),
    Set(Vec<Expr>, Span),
    
    // Async
    Await {
        expr: Box<Expr>,
        span: Span,
    },
    
    // Comprehensions (Python-like)
    ListComp {
        element: Box<Expr>,
        generators: Vec<Comprehension>,
        span: Span,
    },
    
    // Type annotation
    Typed {
        expr: Box<Expr>,
        type_annotation: Type,
        span: Span,
    },
}

/// Binary operators
#[derive(Debug, Clone, PartialEq)]
pub enum BinaryOp {
    Add, Sub, Mul, Div, Mod, Pow, FloorDiv,
    Eq, Neq, Gt, Lt, Gte, Lte,
    And, Or,
    BitAnd, BitOr, BitXor, Shl, Shr,
    In, NotIn, Is, IsNot,
}

/// Unary operators
#[derive(Debug, Clone, PartialEq)]
pub enum UnaryOp {
    Plus, Minus, Not, BitNot,
}

/// Comprehension generator (for list/dict comprehensions)
#[derive(Debug, Clone, PartialEq)]
pub struct Comprehension {
    pub target: String,
    pub iter: Expr,
    pub conditions: Vec<Expr>,
    pub span: Span,
}

/// Statement nodes
#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    // Declarations
    Function {
        name: String,
        parameters: Vec<Parameter>,
        return_type: Option<Type>,
        body: Vec<Stmt>,
        span: Span,
        is_async: bool,
        is_export: bool,
    },
    Class {
        name: String,
        bases: Vec<Expr>,
        body: Vec<Stmt>,
        span: Span,
        is_export: bool,
    },
    Variable {
        name: String,
        type_annotation: Option<Type>,
        value: Option<Expr>,
        span: Span,
    },
    
    // Control flow
    If {
        condition: Expr,
        then_branch: Vec<Stmt>,
        elif_branches: Vec<(Expr, Vec<Stmt>)>,
        else_branch: Option<Vec<Stmt>>,
        span: Span,
    },
    While {
        condition: Expr,
        body: Vec<Stmt>,
        span: Span,
    },
    For {
        variable: String,
        iterable: Expr,
        body: Vec<Stmt>,
        span: Span,
    },
    Match {
        expr: Expr,
        cases: Vec<MatchCase>,
        else_branch: Option<Vec<Stmt>>,
        span: Span,
    },
    
    // Jumps
    Return {
        value: Option<Expr>,
        span: Span,
    },
    Break(Span),
    Continue(Span),
    
    // Expressions
    Expression(Expr, Span),
    Assignment {
        target: AssignTarget,
        value: Expr,
        span: Span,
    },
    AugAssignment {
        target: AssignTarget,
        op: BinaryOp,
        value: Expr,
        span: Span,
    },
    
    // Modules
    Import {
        module: String,
        alias: Option<String>,
        span: Span,
    },
    FromImport {
        module: String,
        items: Vec<ImportItem>,
        span: Span,
    },
    Extern {
        library: String,
        span: Span,
    },
    
    // Async
    Async {
        stmt: Box<Stmt>,
        span: Span,
    },
    
    // Error handling
    Try {
        body: Vec<Stmt>,
        except_handlers: Vec<ExceptHandler>,
        else_body: Option<Vec<Stmt>>,
        finally_body: Option<Vec<Stmt>>,
        span: Span,
    },
    Raise {
        exc: Option<Expr>,
        span: Span,
    },
}

/// Import item for from-import statements
#[derive(Debug, Clone, PartialEq)]
pub struct ImportItem {
    pub name: String,
    pub alias: Option<String>,
    pub span: Span,
}

/// Exception handler
#[derive(Debug, Clone, PartialEq)]
pub struct ExceptHandler {
    pub type_: Option<Expr>,
    pub name: Option<String>,
    pub body: Vec<Stmt>,
    pub span: Span,
}

/// Assignment targets
#[derive(Debug, Clone, PartialEq)]
pub enum AssignTarget {
    Identifier(String, Span),
    Member {
        object: Box<Expr>,
        member: String,
        span: Span,
    },
    Index {
        object: Box<Expr>,
        index: Box<Expr>,
        span: Span,
    },
    Tuple(Vec<AssignTarget>, Span),
    List(Vec<AssignTarget>, Span),
}

/// Function parameters
#[derive(Debug, Clone, PartialEq)]
pub struct Parameter {
    pub name: String,
    pub type_annotation: Option<Type>,
    pub default_value: Option<Expr>,
    pub is_varargs: bool,    // *args
    pub is_kwargs: bool,     // **kwargs
    pub span: Span,
}

/// Match cases
#[derive(Debug, Clone, PartialEq)]
pub struct MatchCase {
    pub pattern: Pattern,
    pub guard: Option<Expr>,
    pub body: Vec<Stmt>,
    pub span: Span,
}

/// Pattern matching
#[derive(Debug, Clone, PartialEq)]
pub enum Pattern {
    Wildcard(Span),
    Literal(Expr, Span),
    Identifier(String, Span),
    Tuple(Vec<Pattern>, Span),
    List(Vec<Pattern>, Span),
    Class(String, Vec<Pattern>, Span),
}

/// Complete program AST
#[derive(Debug, Clone, PartialEq)]
pub struct Program {
    pub statements: Vec<Stmt>,
    pub span: Span,
}

impl Program {
    pub fn new(statements: Vec<Stmt>) -> Self {
        Self {
            statements,
            span: Span::unknown(),
        }
    }
    
    /// Count total nodes for debugging
    pub fn nodes_count(&self) -> usize {
        let mut count = 0;
        
        fn count_stmt(stmt: &Stmt) -> usize {
            let mut total = 1; // Count the statement itself
            
            match stmt {
                Stmt::Function { body, .. } => {
                    total += count_stmts(body);
                }
                Stmt::Class { body, .. } => {
                    total += count_stmts(body);
                }
                Stmt::If { then_branch, elif_branches, else_branch, .. } => {
                    total += count_stmts(then_branch);
                    for (_, branch) in elif_branches {
                        total += count_stmts(branch);
                    }
                    if let Some(else_branch) = else_branch {
                        total += count_stmts(else_branch);
                    }
                }
                Stmt::While { body, .. } => {
                    total += count_stmts(body);
                }
                Stmt::For { body, .. } => {
                    total += count_stmts(body);
                }
                Stmt::Match { cases, else_branch, .. } => {
                    for case in cases {
                        total += count_stmts(&case.body);
                    }
                    if let Some(else_branch) = else_branch {
                        total += count_stmts(else_branch);
                    }
                }
                Stmt::Try { body, except_handlers, else_body, finally_body, .. } => {
                    total += count_stmts(body);
                    for handler in except_handlers {
                        total += count_stmts(&handler.body);
                    }
                    if let Some(else_body) = else_body {
                        total += count_stmts(else_body);
                    }
                    if let Some(finally_body) = finally_body {
                        total += count_stmts(finally_body);
                    }
                }
                Stmt::Expression(expr, _) => {
                    total += count_expr(expr);
                }
                Stmt::Assignment { target: _, value, .. } => {
                    total += count_expr(value);
                }
                Stmt::AugAssignment { target: _, value, .. } => {
                    total += count_expr(value);
                }
                _ => {}
            }
            
            total
        }
        
        fn count_expr(expr: &Expr) -> usize {
            let mut total = 1; // Count the expression itself
            
            match expr {
                Expr::Binary { left, right, .. } => {
                    total += count_expr(left);
                    total += count_expr(right);
                }
                Expr::Unary { expr, .. } => {
                    total += count_expr(expr);
                }
                Expr::Ternary { condition, then_expr, else_expr, .. } => {
                    total += count_expr(condition);
                    total += count_expr(then_expr);
                    total += count_expr(else_expr);
                }
                Expr::Call { callee, arguments, .. } => {
                    total += count_expr(callee);
                    for arg in arguments {
                        total += count_expr(arg);
                    }
                }
                Expr::MemberAccess { object, .. } => {
                    total += count_expr(object);
                }
                Expr::Index { object, index, .. } => {
                    total += count_expr(object);
                    total += count_expr(index);
                }
                Expr::List(items, _) => {
                    for item in items {
                        total += count_expr(item);
                    }
                }
                Expr::Tuple(items, _) => {
                    for item in items {
                        total += count_expr(item);
                    }
                }
                Expr::Dict(pairs, _) => {
                    for (key, value) in pairs {
                        total += count_expr(key);
                        total += count_expr(value);
                    }
                }
                Expr::Set(items, _) => {
                    for item in items {
                        total += count_expr(item);
                    }
                }
                Expr::Await { expr, .. } => {
                    total += count_expr(expr);
                }
                Expr::ListComp { element, generators, .. } => {
                    total += count_expr(element);
                    for gen in generators {
                        total += count_expr(&gen.iter);
                        for cond in &gen.conditions {
                            total += count_expr(cond);
                        }
                    }
                }
                Expr::Typed { expr, .. } => {
                    total += count_expr(expr);
                }
                _ => {}
            }
            
            total
        }
        
        fn count_stmts(stmts: &[Stmt]) -> usize {
            let mut total = 0;
            for stmt in stmts {
                total += count_stmt(stmt);
            }
            total
        }
        
        count_stmts(&self.statements)
    }
}

// Display implementations for debugging
impl fmt::Display for BinaryOp {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            BinaryOp::Add => write!(f, "+"),
            BinaryOp::Sub => write!(f, "-"),
            BinaryOp::Mul => write!(f, "*"),
            BinaryOp::Div => write!(f, "/"),
            BinaryOp::Mod => write!(f, "%"),
            BinaryOp::Pow => write!(f, "**"),
            BinaryOp::FloorDiv => write!(f, "//"),
            BinaryOp::Eq => write!(f, "=="),
            BinaryOp::Neq => write!(f, "!="),
            BinaryOp::Gt => write!(f, ">"),
            BinaryOp::Lt => write!(f, "<"),
            BinaryOp::Gte => write!(f, ">="),
            BinaryOp::Lte => write!(f, "<="),
            BinaryOp::And => write!(f, "and"),
            BinaryOp::Or => write!(f, "or"),
            BinaryOp::BitAnd => write!(f, "&"),
            BinaryOp::BitOr => write!(f, "|"),
            BinaryOp::BitXor => write!(f, "^"),
            BinaryOp::Shl => write!(f, "<<"),
            BinaryOp::Shr => write!(f, ">>"),
            BinaryOp::In => write!(f, "in"),
            BinaryOp::NotIn => write!(f, "not in"),
            BinaryOp::Is => write!(f, "is"),
            BinaryOp::IsNot => write!(f, "is not"),
        }
    }
}

impl fmt::Display for UnaryOp {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            UnaryOp::Plus => write!(f, "+"),
            UnaryOp::Minus => write!(f, "-"),
            UnaryOp::Not => write!(f, "not"),
            UnaryOp::BitNot => write!(f, "~"),
        }
    }
}

// Helper trait for AST walking
pub trait Visitor {
    fn visit_expr(&mut self, expr: &Expr);
    fn visit_stmt(&mut self, stmt: &Stmt);
    
    fn visit_program(&mut self, program: &Program) {
        for stmt in &program.statements {
            self.visit_stmt(stmt);
        }
    }
}