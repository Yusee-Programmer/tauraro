//! Interpreter for Tauraro

use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;

use rustyline::error::ReadlineError;
use rustyline::Editor;
use rustyline::completion::{Completer, Pair};
use rustyline::validate::{Validator, ValidationContext, ValidationResult};
use rustyline::highlight::Highlighter;
use rustyline::hint::{Hinter, HistoryHinter};
use rustyline::Helper;

use crate::vm::VM;
use crate::value::Value;
use crate::lexer::Lexer;
use crate::parser::{Parser, ParseError};
use crate::codegen::{CodegenOptions, Target};
use anyhow::Result;

/// Memory manager (auto + optional manual GC)
pub struct MemoryManager {
    allocations: usize,
}

impl MemoryManager {
    pub fn new() -> Self {
        MemoryManager { allocations: 0 }
    }

    pub fn alloc(&mut self) {
        self.allocations += 1;
    }

    pub fn free(&mut self) {
        if self.allocations > 0 {
            self.allocations -= 1;
        }
    }
}

/// VM-based Interpreter with full language feature support
pub struct Interpreter {
    vm: VM,
    memory: Rc<RefCell<MemoryManager>>,
}

impl Interpreter {
    pub fn new() -> Self {
        Interpreter {
            vm: VM::new(),
            memory: Rc::new(RefCell::new(MemoryManager::new())),
        }
    }

    /// Run a single line (used by REPL) - supports all language features
    pub fn run_line(&mut self, code: String) -> Result<Option<Value>> {
        // Use VM's execute_repl for full language support
        match self.vm.execute_repl(&code, vec![]) {
            Ok(result) => Ok(Some(result)),
            Err(e) => Err(e),
        }
    }

    /// Run a full file (used by `tauraro run`)
    pub fn run_file(&mut self, source: String) -> Result<()> {
        self.vm.execute_script(&source, vec![])?;
        Ok(())
    }

    /// Get variable from VM
    pub fn get_variable(&self, name: &str) -> Option<Value> {
        // For the simplified VM, we don't have direct variable access
        None
    }

    /// Set variable in VM
    pub fn set_variable(&mut self, name: &str, value: Value) -> Result<()> {
        // For the simplified VM, we don't have direct variable access
        Ok(())
    }
    
    /// Run REPL with standard Python-like interface
    pub fn run_repl() -> Result<()> {
        run_repl()
    }
    
    /// Run REPL with multilingual support
    pub fn run_repl_with_multilingual(_multilingual: bool) -> Result<()> {
        run_repl()
    }
}

impl Default for Interpreter {
    fn default() -> Self {
        Self::new()
    }
}

/// REPL Helper for rustyline integration
struct REPLHelper;

impl REPLHelper {
    fn new() -> Self {
        Self
    }
}

impl Helper for REPLHelper {}

impl Validator for REPLHelper {
    fn validate(&self, _ctx: &mut ValidationContext) -> rustyline::Result<ValidationResult> {
        Ok(ValidationResult::Valid(None))
    }
}

impl Completer for REPLHelper {
    type Candidate = Pair;

    fn complete(
        &self,
        _line: &str,
        _pos: usize,
        _ctx: &rustyline::Context<'_>,
    ) -> rustyline::Result<(usize, Vec<Self::Candidate>)> {
        // Disable completion to ensure Tab inserts 4 spaces
        Ok((0, vec![]))
    }
}

impl Highlighter for REPLHelper {
    fn highlight<'l>(&self, line: &'l str, _pos: usize) -> std::borrow::Cow<'l, str> {
        // Implement syntax highlighting with ANSI color codes
        use std::borrow::Cow;

        // Force highlighting to update immediately
        let keywords = [
            "def", "class", "if", "elif", "else", "for", "while", "return",
            "import", "from", "as", "try", "except", "finally", "with",
            "break", "continue", "pass", "raise", "assert", "yield",
            "async", "await", "lambda", "in", "is", "not", "and", "or",
        ];

        let builtins = [
            "print", "len", "range", "type", "int", "float", "str", "list",
            "dict", "set", "tuple", "bool", "None", "True", "False",
            "input", "open", "help", "dir", "vars", "globals", "locals",
        ];

        let mut highlighted = String::new();
        let mut chars = line.chars().peekable();

        while let Some(ch) = chars.next() {
            if ch == '"' || ch == '\'' {
                // String literal
                highlighted.push_str("\x1b[32m"); // Green
                highlighted.push(ch);
                let quote = ch;
                while let Some(ch) = chars.next() {
                    highlighted.push(ch);
                    if ch == '\\' {
                        if let Some(next) = chars.next() {
                            highlighted.push(next);
                        }
                    } else if ch == quote {
                        break;
                    }
                }
                highlighted.push_str("\x1b[0m"); // Reset
            } else if ch == '#' {
                // Comment
                highlighted.push_str("\x1b[90m"); // Gray
                highlighted.push(ch);
                for ch in chars.by_ref() {
                    highlighted.push(ch);
                }
                highlighted.push_str("\x1b[0m"); // Reset
                break;
            } else if ch.is_ascii_digit() {
                // Number
                highlighted.push_str("\x1b[36m"); // Cyan
                highlighted.push(ch);
                while let Some(&next_ch) = chars.peek() {
                    if next_ch.is_ascii_digit() || next_ch == '.' || next_ch == '_' {
                        highlighted.push(chars.next().unwrap());
                    } else {
                        break;
                    }
                }
                highlighted.push_str("\x1b[0m"); // Reset
            } else if ch.is_alphabetic() || ch == '_' {
                // Identifier or keyword
                let mut word = String::new();
                word.push(ch);
                while let Some(&next_ch) = chars.peek() {
                    if next_ch.is_alphanumeric() || next_ch == '_' {
                        word.push(chars.next().unwrap());
                    } else {
                        break;
                    }
                }

                if keywords.contains(&word.as_str()) {
                    highlighted.push_str("\x1b[35m"); // Magenta for keywords
                    highlighted.push_str(&word);
                    highlighted.push_str("\x1b[0m");
                } else if builtins.contains(&word.as_str()) {
                    highlighted.push_str("\x1b[33m"); // Yellow for builtins
                    highlighted.push_str(&word);
                    highlighted.push_str("\x1b[0m");
                } else {
                    highlighted.push_str(&word);
                }
            } else {
                highlighted.push(ch);
            }
        }

        Cow::Owned(highlighted)
    }

    fn highlight_char(&self, _line: &str, _pos: usize) -> bool {
        // Return true to trigger highlighting on every character
        true
    }
}

impl Hinter for REPLHelper {
    type Hint = String;

    fn hint(&self, _line: &str, _pos: usize, _ctx: &rustyline::Context<'_>) -> Option<Self::Hint> {
        None
    }
}

/// Enhanced REPL with full language support and Python-like behavior
pub fn run_repl() -> Result<()> {
    // Print Python-like banner
    println!("Tauraro 1.0.0 (main, Jan 2025)");
    println!("[Rust-based VM] on {}", std::env::consts::OS);
    println!("Type \"help\", \"copyright\", \"credits\" or \"license\" for more information.");

    let mut interpreter = Interpreter::new();
    let config = rustyline::Config::builder()
        .auto_add_history(true)
        .tab_stop(4)  // Use 4 spaces for tabs
        .completion_type(rustyline::CompletionType::Circular)
        .edit_mode(rustyline::EditMode::Emacs)
        .build();

    let helper = REPLHelper::new();
    let mut rl = Editor::with_config(config)
        .map_err(|e| anyhow::anyhow!("Failed to start REPL: {}", e))?;
    rl.set_helper(Some(helper));

    // Bind Tab key to insert literal tab character (4 spaces)
    rl.bind_sequence(rustyline::KeyEvent::new('\t', rustyline::Modifiers::NONE), rustyline::Cmd::Insert(1, "    ".to_string()));

    let mut buffer = String::new();
    let mut in_multiline = false;

    loop {
        let prompt = if buffer.is_empty() { ">>> " } else { "... " };
        let readline = rl.readline(prompt);

        match readline {
            Ok(line) => {
                // Check for exit commands
                if line.trim() == "exit()" || line.trim() == "quit()" || line.trim() == "exit" || line.trim() == "quit" {
                    break;
                }

                // Check for special commands
                if line.trim() == "help" || line.trim() == "help()" {
                    show_help();
                    continue;
                }

                if line.trim() == "copyright" {
                    println!("Copyright (c) 2025 Tauraro Programming Language");
                    println!("All rights reserved.");
                    continue;
                }

                if line.trim() == "credits" {
                    println!("Thanks to the Rust community and all contributors!");
                    continue;
                }

                if line.trim() == "license" {
                    println!("Tauraro is licensed under the MIT License");
                    continue;
                }

                // Handle multiline input like Python
                if buffer.is_empty() {
                    // Starting fresh - check if this line starts a multiline construct
                    let trimmed = line.trim();

                    // Check for multiline constructs
                    if trimmed.ends_with(':') ||
                       trimmed.starts_with('@') ||
                       trimmed.starts_with("def ") ||
                       trimmed.starts_with("class ") ||
                       trimmed.starts_with("if ") ||
                       trimmed.starts_with("elif ") ||
                       trimmed.starts_with("else:") ||
                       trimmed.starts_with("for ") ||
                       trimmed.starts_with("while ") ||
                       trimmed.starts_with("try:") ||
                       trimmed.starts_with("except ") ||
                       trimmed.starts_with("finally:") ||
                       trimmed.starts_with("with ") {
                        in_multiline = true;
                        buffer.push_str(&line);
                        buffer.push('\n');
                        continue;
                    } else if trimmed.is_empty() {
                        // Empty line at top level, ignore
                        continue;
                    } else {
                        // Single line - check if it's an expression or statement
                        buffer.push_str(&line);
                        buffer.push('\n');
                    }
                } else {
                    // We're in multiline mode
                    if line.trim().is_empty() {
                        // Empty line - end multiline input and execute
                        in_multiline = false;
                        // Don't add the empty line to buffer
                    } else {
                        // Add the line and continue
                        buffer.push_str(&line);
                        buffer.push('\n');
                        continue;
                    }
                }

                // Try to execute the buffer if we have content
                if !buffer.trim().is_empty() {
                    match interpreter.run_line(buffer.clone()) {
                        Ok(Some(value)) => {
                            // Pretty print the value like Python REPL
                            match &value {
                                Value::Str(s) => println!("'{}'", s), // Print strings with quotes
                                Value::List(_) => println!("{}", format_value(&value)),
                                Value::Dict(_) => println!("{}", format_value(&value)),
                                Value::Closure { name, .. } => {
                                    println!("<function {}>", name);
                                }
                                Value::Bool(b) => println!("{}", if *b { "True" } else { "False" }),
                                Value::None => {}, // Don't print None
                                // For objects, classes, and modules, show concise representation like Python
                                Value::Object { class_name, .. } => {
                                    println!("<{} object>", class_name);
                                }
                                Value::Class { name, .. } => {
                                    println!("<class '{}'>", name);
                                }
                                Value::Module(name, _) => {
                                    println!("<module '{}'>", name);
                                }
                                Value::BuiltinFunction(name, _) => {
                                    println!("<built-in function {}>", name);
                                }
                                _ => {
                                    // For other values, only print if they're not None
                                    if !matches!(value, Value::None) {
                                        println!("{}", format_value(&value));
                                    }
                                },
                            }
                        }
                        Ok(None) => {
                            // No value to print
                        }
                        Err(e) => {
                            // Print traceback-like error
                            eprintln!("Traceback (most recent call last):");
                            eprintln!("  File \"<stdin>\", line 1, in <module>");

                            // Parse error message to get error type
                            let error_str = e.to_string();
                            if error_str.contains("not found") || error_str.contains("not defined") {
                                eprintln!("NameError: {}", error_str);
                            } else if error_str.contains("type") {
                                eprintln!("TypeError: {}", error_str);
                            } else if error_str.contains("syntax") {
                                eprintln!("SyntaxError: {}", error_str);
                            } else if error_str.contains("division by zero") {
                                eprintln!("ZeroDivisionError: {}", error_str);
                            } else if error_str.contains("index") {
                                eprintln!("IndexError: {}", error_str);
                            } else if error_str.contains("key") {
                                eprintln!("KeyError: {}", error_str);
                            } else {
                                eprintln!("RuntimeError: {}", error_str);
                            }
                        }
                    }
                }
                buffer.clear();
                in_multiline = false;
            }
            Err(ReadlineError::Interrupted) => {
                println!("KeyboardInterrupt");
                buffer.clear();
                in_multiline = false;
                continue;
            }
            Err(ReadlineError::Eof) => {
                // Ctrl+D pressed
                break;
            }
            Err(err) => {
                eprintln!("Error: {:?}", err);
                break;
            }
        }
    }

    Ok(())
}

/// Format a value for display (Python-like)
fn format_value(value: &Value) -> String {
    match value {
        Value::None => "None".to_string(),
        Value::Bool(b) => if *b { "True" } else { "False" }.to_string(),
        Value::Int(i) => i.to_string(),
        Value::Float(f) => {
            if f.fract() == 0.0 && f.abs() < 1e10 {
                format!("{:.1}", f)
            } else {
                f.to_string()
            }
        }
        Value::Str(s) => format!("'{}'", s),
        Value::List(list) => {
            let items: Vec<String> = (0..list.len())
                .filter_map(|i| list.get(i as isize))
                .map(|v| format_value(&v))
                .collect();
            format!("[{}]", items.join(", "))
        }
        Value::Dict(map) => {
            let items: Vec<String> = map
                .iter()
                .map(|(k, v)| format!("{}: {}", format_value(&Value::Str(k.clone())), format_value(v)))
                .collect();
            format!("{{{}}}", items.join(", "))
        }
        _ => format!("{:?}", value),
    }
}

/// Show help information
fn show_help() {
    println!("\nWelcome to Tauraro!");
    println!();
    println!("Tauraro is a Python-compatible programming language with Rust-like performance.");
    println!();
    println!("Type help() for interactive help, or help(object) for help about object.");
    println!();
    println!("Quick Reference:");
    println!("  Variables:    x = 10");
    println!("  Functions:    def greet(name): return f'Hello, {{name}}'");
    println!("  Classes:      class MyClass: pass");
    println!("  Loops:        for i in range(10): print(i)");
    println!("  Conditions:   if x > 5: print('big')");
    println!("  Import:       import math");
    println!();
    println!("Built-in Functions:");
    println!("  print()       Print values to stdout");
    println!("  input()       Read input from stdin");
    println!("  len()         Get length of sequence");
    println!("  range()       Generate range of numbers");
    println!("  type()        Get type of object");
    println!("  dir()         List attributes");
    println!("  help()        Show this help");
    println!("  exit()        Exit the REPL");
    println!();
    println!("REPL Commands:");
    println!("  copyright     Show copyright information");
    println!("  credits       Show credits");
    println!("  license       Show license information");
    println!();
}

/// Show variables in current scope
fn show_variables(_interpreter: &Interpreter) {
    // For the simplified VM, we don't have direct variable access
    println!("[]");
}

/// Show global variables
fn show_globals(_interpreter: &Interpreter) {
    // For the simplified VM, we don't have direct variable access
    println!("{{}}");
}

/// Show local variables
fn show_locals(_interpreter: &Interpreter) {
    // For the simplified VM, we don't have direct variable access
    println!("{{}}");
}

/// Code generator implementation for interpreter target
pub struct InterpreterCodeGenerator {
    target: Target,
}

impl InterpreterCodeGenerator {
    pub fn new() -> Self {
        Self {
            target: Target::Interpreter,
        }
    }
}

/// Implementation of CodeGenerator trait for interpreter
impl crate::codegen::CodeGenerator for InterpreterCodeGenerator {
    fn generate(
        &self,
        _module: crate::ir::IRModule,  // Fixed: removed & reference
        _options: &CodegenOptions,
    ) -> Result<Vec<u8>> {
        // The interpreter uses the VM directly, not compiled bytecode
        // This is just a stub to satisfy the codegen system
        Ok(vec![])
    }
    
    fn get_target(&self) -> Target {
        Target::Interpreter
    }
    
    fn supports_optimization(&self) -> bool {
        false
    }
    
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec![
            "async/await",
            "generators", 
            "exceptions",
            "dynamic_typing",
            "reflection",
            "hot_reload",
            "classes",
            "functions",
            "loops",
            "conditionals",
        ]
    }
}

impl Default for InterpreterCodeGenerator {
    fn default() -> Self {
        Self::new()
    }
}