//! Runtime error handling for Tauraro

use anyhow::Error;
use std::fmt;

#[derive(Debug)]
pub struct RuntimeError {
    pub message: String,
    pub line: usize,
    pub column: usize,
    pub filename: String,
    pub traceback: Option<Vec<TracebackFrame>>,
}

#[derive(Debug, Clone)]
pub struct TracebackFrame {
    pub filename: String,
    pub line: usize,
    pub column: usize,
    pub function: String,
}

impl RuntimeError {
    pub fn new(message: String, line: usize, column: usize, filename: String) -> Self {
        Self {
            message,
            line,
            column,
            filename,
            traceback: None,
        }
    }
    
    pub fn with_traceback(message: String, line: usize, column: usize, filename: String, traceback: Vec<TracebackFrame>) -> Self {
        Self {
            message,
            line,
            column,
            filename,
            traceback: Some(traceback),
        }
    }
}

impl fmt::Display for RuntimeError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if let Some(traceback) = &self.traceback {
            writeln!(f, "Traceback (most recent call last):")?;
            for frame in traceback {
                writeln!(f, "  File \"{}\", line {}, in {}", frame.filename, frame.line, frame.function)?;
            }
        }
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

pub fn create_traceback_frame(filename: String, line: usize, column: usize, function: String) -> TracebackFrame {
    TracebackFrame {
        filename,
        line,
        column,
        function,
    }
}