//! Runtime error handling for Tauraro

use anyhow::Error;
use std::fmt;

#[derive(Debug)]
pub struct RuntimeError {
    pub message: String,
    pub line: usize,
    pub column: usize,
    pub filename: String,
}

impl RuntimeError {
    pub fn new(message: String, line: usize, column: usize, filename: String) -> Self {
        Self {
            message,
            line,
            column,
            filename,
        }
    }
}

impl fmt::Display for RuntimeError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "RuntimeError: {} at {}:{}:{}", self.message, self.filename, self.line, self.column)
    }
}

impl std::error::Error for RuntimeError {}

pub fn format_traceback(error: &Error, filename: &str, line: usize, _column: usize) -> String {
    format!(
        "Traceback (most recent call last):\n  File \"{}\", line {}, in <module>\n{}",
        filename, line, error.to_string()
    )
}