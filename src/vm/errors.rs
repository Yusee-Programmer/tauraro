//! VM errors (stack overflow, type error, etc.)
use anyhow::Result;
use thiserror::Error;

/// Custom error types for the VM
#[derive(Error, Debug)]
pub enum VMError {
    #[error("Stack overflow")]
    StackOverflow,
    
    #[error("Type error: {0}")]
    TypeError(String),
    
    #[error("Name error: {0}")]
    NameError(String),
    
    #[error("Attribute error: {0}")]
    AttributeError(String),
    
    #[error("Index error: {0}")]
    IndexError(String),
    
    #[error("Value error: {0}")]
    ValueError(String),
    
    #[error("Runtime error: {0}")]
    RuntimeError(String),
    
    #[error("Import error: {0}")]
    ImportError(String),
    
    #[error("Syntax error: {0}")]
    SyntaxError(String),
    
    #[error("Memory error: {0}")]
    MemoryError(String),
    
    #[error("Overflow error: {0}")]
    OverflowError(String),
    
    #[error("Zero division error: {0}")]
    ZeroDivisionError(String),
}

/// Result type alias for VM operations
pub type VMResult<T> = Result<T, VMError>;

/// Stack overflow error
#[derive(Error, Debug)]
#[error("Stack overflow: maximum recursion depth exceeded")]
pub struct StackOverflowError;

/// Type error
#[derive(Error, Debug)]
#[error("Type error: {expected} expected, got {actual}")]
pub struct TypeError {
    pub expected: String,
    pub actual: String,
}

/// Name error
#[derive(Error, Debug)]
#[error("Name '{name}' is not defined")]
pub struct NameError {
    pub name: String,
}

/// Attribute error
#[derive(Error, Debug)]
#[error("'{obj_type}' object has no attribute '{attribute}'")]
pub struct AttributeError {
    pub obj_type: String,
    pub attribute: String,
}

/// Index error
#[derive(Error, Debug)]
#[error("Index error: list index out of range")]
pub struct IndexError;

/// Value error
#[derive(Error, Debug)]
#[error("Value error: {message}")]
pub struct ValueError {
    pub message: String,
}

/// Runtime error
#[derive(Error, Debug)]
#[error("Runtime error: {message}")]
pub struct RuntimeError {
    pub message: String,
}

/// Import error
#[derive(Error, Debug)]
#[error("Import error: {message}")]
pub struct ImportError {
    pub message: String,
}