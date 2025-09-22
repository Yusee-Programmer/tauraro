//! Abstract Syntax Tree representation of TauraroLang programs
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

/// Type representation
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
    Any, // Dynamic type
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
    
    // Async
    Await {
        expr: Box<Expr>,
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
}

/// Unary operators
#[derive(Debug, Clone, PartialEq)]
pub enum UnaryOp {
    Plus, Minus, Not, BitNot,
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
    
    // Modules
    Import {
        module: String,
        alias: Option<String>,
        span: Span,
    },
    FromImport {
        module: String,
        items: Vec<(String, Option<String>)>,
        span: Span,
    },
    Extern {
        library: String,
        span: Span,
    },
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
        let mut count = 1; // Program node itself
        
        fn count_stmts(stmts: &[Stmt]) -> usize {
            let mut total = stmts.len();
            for stmt in stmts {
                total += count_stmt(stmt);
            }
            total
        }
        
        fn count_stmt(stmt: &Stmt) -> usize {
            match stmt {
                Stmt::Function { body, .. } => count_stmts(body),
                Stmt::Class { body, .. } => count_stmts(body),
                Stmt::If { then_branch, elif_branches, else_branch, .. } => {
                    let mut total = count_stmts(then_branch);
                    for (_, branch) in elif_branches {
                        total += count_stmts(branch);
                    }
                    if let Some(else_branch) = else_branch {
                        total += count_stmts(else_branch);
                    }
                    total
                }
                Stmt::While { body, .. } => count_stmts(body),
                Stmt::For { body, .. } => count_stmts(body),
                Stmt::Match { cases, else_branch, .. } => {
                    let mut total = 0;
                    for case in cases {
                        total += count_stmts(&case.body);
                    }
                    if let Some(else_branch) = else_branch {
                        total += count_stmts(else_branch);
                    }
                    total
                }
                _ => 0,
            }
        }
        
        count + count_stmts(&self.statements)
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
        }
    }
}