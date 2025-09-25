use std::fmt;
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Program {
    pub statements: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Statement {
    Expression(Expr),
    VariableDef {
        name: String,
        type_annotation: Option<Type>,
        value: Option<Expr>,
    },
    FunctionDef {
        name: String,
        params: Vec<Param>,
        return_type: Option<Type>,
        body: Vec<Statement>,
        is_async: bool,
        decorators: Vec<Expr>,
    },
    ClassDef {
        name: String,
        bases: Vec<Expr>,
        body: Vec<Statement>,
        decorators: Vec<Expr>,
        metaclass: Option<Expr>,
    },
    If {
        condition: Expr,
        then_branch: Vec<Statement>,
        elif_branches: Vec<(Expr, Vec<Statement>)>,
        else_branch: Option<Vec<Statement>>,
    },
    While {
        condition: Expr,
        body: Vec<Statement>,
        else_branch: Option<Vec<Statement>>,
    },
    For {
        variable: String,
        iterable: Expr,
        body: Vec<Statement>,
        else_branch: Option<Vec<Statement>>,
    },
    Match {
        value: Expr,
        cases: Vec<MatchCase>,
    },
    Try {
        body: Vec<Statement>,
        except_handlers: Vec<ExceptHandler>,
        else_branch: Option<Vec<Statement>>,
        finally: Option<Vec<Statement>>,
    },
    Return(Option<Expr>),
    Break,
    Continue,
    Raise(Option<Expr>),
    Import {
        module: String,
        alias: Option<String>,
    },
    FromImport {
        module: String,
        names: Vec<(String, Option<String>)>,
    },
    With {
        context: Expr,
        alias: Option<String>,
        body: Vec<Statement>,
    },
    Async(Box<Statement>),
    Await(Expr),
    Export {
        name: String,
    },
    Extern {
        name: String,
        signature: String,
    },
    Global {
        names: Vec<String>,
    },
    Nonlocal {
        names: Vec<String>,
    },
    Del {
        targets: Vec<Expr>,
    },
    Assert {
        condition: Expr,
        message: Option<Expr>,
    },
    Pass,
    TypeAlias {
        name: String,
        type_def: Type,
    },
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Param {
    pub name: String,
    pub type_annotation: Option<Type>,
    pub default: Option<Expr>,
    pub kind: ParamKind,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum ParamKind {
    Positional,
    PositionalOnly,
    KeywordOnly,
    VarArgs,    // *args
    VarKwargs,  // **kwargs
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct MatchCase {
    pub pattern: Pattern,
    pub guard: Option<Expr>,
    pub body: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Pattern {
    Literal(Expr),
    Variable(String),
    Wildcard,
    Tuple(Vec<Pattern>),
    List(Vec<Pattern>),
    Dict(Vec<(Pattern, Pattern)>),
    Class {
        name: String,
        patterns: Vec<Pattern>,
    },
    Or(Vec<Pattern>),
    As {
        pattern: Box<Pattern>,
        name: String,
    },
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ExceptHandler {
    pub exception_type: Option<Expr>,
    pub name: Option<String>,
    pub body: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Expr {
    Literal(Literal),
    Identifier(String),
    BinaryOp {
        left: Box<Expr>,
        op: BinaryOp,
        right: Box<Expr>,
    },
    UnaryOp {
        op: UnaryOp,
        operand: Box<Expr>,
    },
    Call {
        func: Box<Expr>,
        args: Vec<Expr>,
        kwargs: Vec<(String, Expr)>,
    },
    MethodCall {
        object: Box<Expr>,
        method: String,
        args: Vec<Expr>,
        kwargs: Vec<(String, Expr)>,
    },
    Attribute {
        object: Box<Expr>,
        name: String,
    },
    Subscript {
        object: Box<Expr>,
        index: Box<Expr>,
    },
    Slice {
        object: Box<Expr>,
        start: Option<Box<Expr>>,
        stop: Option<Box<Expr>>,
        step: Option<Box<Expr>>,
    },
    List(Vec<Expr>),
    Tuple(Vec<Expr>),
    Dict(Vec<(Expr, Expr)>),
    Set(Vec<Expr>),
    ListComp {
        element: Box<Expr>,
        generators: Vec<Comprehension>,
    },
    DictComp {
        key: Box<Expr>,
        value: Box<Expr>,
        generators: Vec<Comprehension>,
    },
    SetComp {
        element: Box<Expr>,
        generators: Vec<Comprehension>,
    },
    GeneratorExp {
        element: Box<Expr>,
        generators: Vec<Comprehension>,
    },
    Lambda {
        params: Vec<Param>,
        body: Box<Expr>,
    },
    IfExp {
        condition: Box<Expr>,
        then_expr: Box<Expr>,
        else_expr: Box<Expr>,
    },
    Yield(Option<Box<Expr>>),
    YieldFrom(Box<Expr>),
    Await(Box<Expr>),
    FormatString {
        parts: Vec<FormatPart>,
    },
    Starred(Box<Expr>),
    NamedExpr {
        target: Box<Expr>,
        value: Box<Expr>,
    },
    Compare {
        left: Box<Expr>,
        ops: Vec<CompareOp>,
        comparators: Vec<Expr>,
    },
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FormatPart {
    String(String),
    Expression {
        expr: Expr,
        format_spec: Option<String>,
        conversion: Option<char>,
    },
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Literal {
    Int(i64),
    Float(f64),
    String(String),
    Bool(bool),
    None,
    Bytes(Vec<u8>),
    Complex { real: f64, imag: f64 },
    Ellipsis,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    FloorDiv,
    Mod,
    Pow,
    LShift,
    RShift,
    BitOr,
    BitXor,
    BitAnd,
    MatMul,
    // Logical operators
    And,
    Or,
    // Comparison operators (moved to CompareOp)
    Eq,
    Ne,
    Neq, // Added for compatibility
    Lt,
    Le,
    Gt,
    Ge,
    Gte, // Added for compatibility
    Lte, // Added for compatibility
    Is,
    IsNot,
    In,
    NotIn,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum CompareOp {
    Eq,
    NotEq,
    Lt,
    LtE,
    Gt,
    GtE,
    Is,
    IsNot,
    In,
    NotIn,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AssignTarget {
    Identifier(String, Option<Type>),
    Attribute {
        object: Box<Expr>,
        name: String,
    },
    Subscript {
        object: Box<Expr>,
        index: Box<Expr>,
    },
    Tuple(Vec<AssignTarget>),
    List(Vec<AssignTarget>),
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum UnaryOp {
    Not,
    UAdd,
    USub,
    Minus, // Added for compatibility
    Invert,
    BitNot, // Added for compatibility
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Comprehension {
    pub target: String,
    pub iter: Expr,
    pub ifs: Vec<Expr>,
    pub is_async: bool,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Type {
    Simple(String),
    Generic {
        name: String,
        args: Vec<Type>,
    },
    Tuple(Vec<Type>),
    Union(Vec<Type>),
    Optional(Box<Type>),
    Function {
        params: Vec<Type>,
        return_type: Box<Type>,
    },
    Literal(Expr),
    TypeVar {
        name: String,
        bound: Option<Box<Type>>,
        constraints: Vec<Type>,
    },
    Protocol {
        name: String,
        members: Vec<ProtocolMember>,
    },
    Any, // Added for compatibility
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum ProtocolMember {
    Method {
        name: String,
        params: Vec<Type>,
        return_type: Type,
    },
    Property {
        name: String,
        type_annotation: Type,
    },
}

// Additional helper types for advanced features
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Decorator {
    pub name: String,
    pub args: Vec<Expr>,
    pub kwargs: Vec<(String, Expr)>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct AsyncContext {
    pub is_async: bool,
    pub await_expressions: Vec<Expr>,
}

// Trait implementations for better ergonomics
impl Default for ParamKind {
    fn default() -> Self {
        ParamKind::Positional
    }
}

impl From<String> for Expr {
    fn from(s: String) -> Self {
        Expr::Identifier(s)
    }
}

impl From<i64> for Expr {
    fn from(i: i64) -> Self {
        Expr::Literal(Literal::Int(i))
    }
}

impl From<f64> for Expr {
    fn from(f: f64) -> Self {
        Expr::Literal(Literal::Float(f))
    }
}

impl From<bool> for Expr {
    fn from(b: bool) -> Self {
        Expr::Literal(Literal::Bool(b))
    }
}

impl From<&str> for Expr {
    fn from(s: &str) -> Self {
        Expr::Literal(Literal::String(s.to_string()))
    }
}

// Helper methods for AST construction
impl Expr {
    pub fn call(self, args: Vec<Expr>) -> Self {
        Expr::Call {
            func: Box::new(self),
            args,
            kwargs: Vec::new(),
        }
    }

    pub fn attr(self, name: &str) -> Self {
        Expr::Attribute {
            object: Box::new(self),
            name: name.to_string(),
        }
    }

    pub fn index(self, index: Expr) -> Self {
        Expr::Subscript {
            object: Box::new(self),
            index: Box::new(index),
        }
    }

    pub fn add(self, other: Expr) -> Self {
        Expr::BinaryOp {
            left: Box::new(self),
            op: BinaryOp::Add,
            right: Box::new(other),
        }
    }

    pub fn sub(self, other: Expr) -> Self {
        Expr::BinaryOp {
            left: Box::new(self),
            op: BinaryOp::Sub,
            right: Box::new(other),
        }
    }

    pub fn mul(self, other: Expr) -> Self {
        Expr::BinaryOp {
            left: Box::new(self),
            op: BinaryOp::Mul,
            right: Box::new(other),
        }
    }

    pub fn div(self, other: Expr) -> Self {
        Expr::BinaryOp {
            left: Box::new(self),
            op: BinaryOp::Div,
            right: Box::new(other),
        }
    }
}

impl Statement {
    pub fn expr(expr: Expr) -> Self {
        Statement::Expression(expr)
    }

    pub fn var_def(name: &str, value: Expr) -> Self {
        Statement::VariableDef {
            name: name.to_string(),
            type_annotation: None,
            value: Some(value),
        }
    }

    pub fn func_def(name: &str, params: Vec<Param>, body: Vec<Statement>) -> Self {
        Statement::FunctionDef {
            name: name.to_string(),
            params,
            return_type: None,
            body,
            is_async: false,
            decorators: Vec::new(),
        }
    }
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Type::Simple(name) => write!(f, "{}", name),
            Type::Generic { name, args } => {
                write!(f, "{}<", name)?;
                for (i, arg) in args.iter().enumerate() {
                    if i > 0 {
                        write!(f, ", ")?;
                    }
                    write!(f, "{}", arg)?;
                }
                write!(f, ">")
            }
            Type::Tuple(types) => {
                write!(f, "(")?;
                for (i, ty) in types.iter().enumerate() {
                    if i > 0 { write!(f, ", ")?; }
                    write!(f, "{}", ty)?;
                }
                write!(f, ")")
            }
            Type::Union(types) => {
                for (i, ty) in types.iter().enumerate() {
                    if i > 0 { write!(f, " | ")?; }
                    write!(f, "{}", ty)?;
                }
                Ok(())
            }
            Type::Optional(inner) => write!(f, "{}?", inner),
            Type::Function { params, return_type } => {
                write!(f, "(")?;
                for (i, param) in params.iter().enumerate() {
                    if i > 0 { write!(f, ", ")?; }
                    write!(f, "{}", param)?;
                }
                write!(f, ") -> {}", return_type)
            }
            Type::Literal(expr) => write!(f, "Literal({:?})", expr),
            Type::TypeVar { name, bound, constraints } => {
                write!(f, "TypeVar({})", name)?;
                if let Some(bound) = bound {
                    write!(f, " bound {}", bound)?;
                }
                if !constraints.is_empty() {
                    write!(f, " constraints [")?;
                    for (i, constraint) in constraints.iter().enumerate() {
                        if i > 0 { write!(f, ", ")?; }
                        write!(f, "{}", constraint)?;
                    }
                    write!(f, "]")?;
                }
                Ok(())
            }
            Type::Protocol { name, .. } => write!(f, "Protocol({})", name),
            Type::Any => write!(f, "Any"),
        }
    }
}