//! Python-like traceback formatting with colors and source context
//!
//! This module provides comprehensive error reporting that matches Python's behavior:
//! - Full traceback chain with file names and line numbers
//! - Source code context with syntax highlighting
//! - Caret (^) pointing to error location
//! - Color-coded error types and messages
//! - SyntaxError special formatting

use std::fmt;
use colored::*;

/// Represents a single frame in the traceback
#[derive(Debug, Clone)]
pub struct TracebackFrame {
    pub filename: String,
    pub line: usize,
    pub column: usize,
    pub function: String,
    pub source_line: Option<String>,  // The actual source code line
}

/// Represents an exception with full traceback information
#[derive(Debug, Clone)]
pub struct TauraroException {
    pub exception_type: String,
    pub message: String,
    pub filename: String,
    pub line: usize,
    pub column: usize,
    pub source_line: Option<String>,
    pub traceback: Vec<TracebackFrame>,
}

impl TracebackFrame {
    pub fn new(filename: String, line: usize, column: usize, function: String) -> Self {
        Self {
            filename,
            line,
            column,
            function,
            source_line: None,
        }
    }

    pub fn with_source(mut self, source: String) -> Self {
        self.source_line = Some(source);
        self
    }
}

impl TauraroException {
    pub fn new(
        exception_type: String,
        message: String,
        filename: String,
        line: usize,
        column: usize,
    ) -> Self {
        Self {
            exception_type,
            message,
            filename,
            line,
            column,
            source_line: None,
            traceback: Vec::new(),
        }
    }

    pub fn with_source(mut self, source: String) -> Self {
        self.source_line = Some(source);
        self
    }

    pub fn with_traceback(mut self, traceback: Vec<TracebackFrame>) -> Self {
        self.traceback = traceback;
        self
    }

    pub fn add_frame(&mut self, frame: TracebackFrame) {
        self.traceback.push(frame);
    }

    /// Format the exception in Python style with colors
    pub fn format_colored(&self) -> String {
        let mut output = String::new();

        // Print traceback header
        if !self.traceback.is_empty() {
            output.push_str(&format!("{}\n", "Traceback (most recent call last):".bold()));

            // Print each frame
            for frame in &self.traceback {
                output.push_str(&format!(
                    "  File \"{}\", line {}, in {}\n",
                    frame.filename.cyan(),
                    frame.line.to_string().yellow(),
                    frame.function.green()
                ));

                // Print source line if available
                if let Some(source) = &frame.source_line {
                    let trimmed = source.trim();
                    if !trimmed.is_empty() {
                        output.push_str(&format!("    {}\n", trimmed));
                    }
                }
            }
        }

        // Print error location
        output.push_str(&format!(
            "  File \"{}\", line {}\n",
            self.filename.cyan(),
            self.line.to_string().yellow()
        ));

        // Print source line with caret if available
        if let Some(source) = &self.source_line {
            let trimmed = source.trim_end();
            if !trimmed.is_empty() {
                output.push_str(&format!("    {}\n", trimmed));

                // Add caret pointing to the error location
                if self.column > 0 {
                    let leading_spaces = source.len() - source.trim_start().len();
                    let caret_position = if self.column > leading_spaces {
                        self.column - leading_spaces
                    } else {
                        0
                    };
                    let caret_line = format!("{}^", " ".repeat(caret_position + 4));
                    output.push_str(&format!("{}\n", caret_line.red().bold()));
                }
            }
        }

        // Print exception type and message with color
        let exception_line = format!("{}: {}", self.exception_type, self.message);
        output.push_str(&exception_line.red().bold().to_string());

        output
    }

    /// Format the exception in Python style without colors (for file output)
    pub fn format_plain(&self) -> String {
        let mut output = String::new();

        // Print traceback header
        if !self.traceback.is_empty() {
            output.push_str("Traceback (most recent call last):\n");

            // Print each frame
            for frame in &self.traceback {
                output.push_str(&format!(
                    "  File \"{}\", line {}, in {}\n",
                    frame.filename, frame.line, frame.function
                ));

                // Print source line if available
                if let Some(source) = &frame.source_line {
                    let trimmed = source.trim();
                    if !trimmed.is_empty() {
                        output.push_str(&format!("    {}\n", trimmed));
                    }
                }
            }
        }

        // Print error location
        output.push_str(&format!(
            "  File \"{}\", line {}\n",
            self.filename, self.line
        ));

        // Print source line with caret if available
        if let Some(source) = &self.source_line {
            let trimmed = source.trim_end();
            if !trimmed.is_empty() {
                output.push_str(&format!("    {}\n", trimmed));

                // Add caret pointing to the error location
                if self.column > 0 {
                    let leading_spaces = source.len() - source.trim_start().len();
                    let caret_position = if self.column > leading_spaces {
                        self.column - leading_spaces
                    } else {
                        0
                    };
                    output.push_str(&format!("    {}^\n", " ".repeat(caret_position)));
                }
            }
        }

        // Print exception type and message
        output.push_str(&format!("{}: {}", self.exception_type, self.message));

        output
    }
}

impl fmt::Display for TauraroException {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        // Check if colors are supported (terminal output)
        if atty::is(atty::Stream::Stderr) {
            write!(f, "{}", self.format_colored())
        } else {
            write!(f, "{}", self.format_plain())
        }
    }
}

/// Helper functions for creating common exception types

pub fn create_syntax_error(
    message: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let mut exc = TauraroException::new("SyntaxError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_name_error(
    name: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let message = format!("name '{}' is not defined", name);
    let mut exc = TauraroException::new("NameError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_type_error(
    message: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let mut exc = TauraroException::new("TypeError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_value_error(
    message: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let mut exc = TauraroException::new("ValueError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_zero_division_error(
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let message = "division by zero".to_string();
    let mut exc = TauraroException::new("ZeroDivisionError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_index_error(
    message: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let mut exc = TauraroException::new("IndexError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_key_error(
    key: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let message = format!("'{}'", key);
    let mut exc = TauraroException::new("KeyError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_attribute_error(
    obj_type: String,
    attr: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let message = format!("'{}' object has no attribute '{}'", obj_type, attr);
    let mut exc = TauraroException::new("AttributeError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_import_error(
    message: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let mut exc = TauraroException::new("ImportError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_runtime_error(
    message: String,
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let mut exc = TauraroException::new("RuntimeError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

pub fn create_recursion_error(
    filename: String,
    line: usize,
    column: usize,
    source: Option<String>,
) -> TauraroException {
    let message = "maximum recursion depth exceeded".to_string();
    let mut exc = TauraroException::new("RecursionError".to_string(), message, filename, line, column);
    if let Some(src) = source {
        exc = exc.with_source(src);
    }
    exc
}

/// Helper to extract source line from source code
pub fn get_source_line(source: &str, line_number: usize) -> Option<String> {
    source.lines().nth(line_number.saturating_sub(1)).map(|s| s.to_string())
}
