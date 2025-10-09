//! Runtime error with file and line information for Python-like tracebacks

use std::fmt;

#[derive(Debug, Clone)]
pub struct RuntimeError {
    pub message: String,
    pub filename: String,
    pub line: u32,
    pub function_name: String,
}

impl RuntimeError {
    pub fn new(message: String, filename: String, line: u32, function_name: String) -> Self {
        Self {
            message,
            filename,
            line,
            function_name,
        }
    }

    /// Format the error like Python's traceback
    pub fn format_traceback(&self) -> String {
        format!(
            "Traceback (most recent call last):\n  File \"{}\", line {}, in {}\n{}",
            self.filename, self.line, self.function_name, self.message
        )
    }
}

impl fmt::Display for RuntimeError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.format_traceback())
    }
}

impl std::error::Error for RuntimeError {}

/// Convert anyhow::Error to RuntimeError with file/line context
pub fn wrap_error(err: anyhow::Error, filename: &str, line: u32, function_name: &str) -> RuntimeError {
    RuntimeError::new(
        err.to_string(),
        filename.to_string(),
        line,
        function_name.to_string(),
    )
}
