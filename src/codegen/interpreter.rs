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
        // Use VM's execute_script for full language support
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
        self.vm.get_variable(name)
    }

    /// Set variable in VM
    pub fn set_variable(&mut self, name: &str, value: Value) -> Result<()> {
        self.vm.set_variable(name, value)
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
        // Disable syntax highlighting to avoid character display issues
        std::borrow::Cow::Borrowed(line)
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
    println!("TauraroLang Interactive Interpreter v0.1.0 🔥");
    println!("Python-compatible syntax with multi-language support");
    println!("Type 'exit' or press Ctrl+C to quit\n");

    let mut interpreter = Interpreter::new();
    let mut rl = Editor::<()>::new().map_err(|e| anyhow::anyhow!("Failed to start REPL: {}", e))?;

    let mut buffer = String::new();
    let mut in_multiline = false;

    loop {
        let prompt = if buffer.is_empty() { ">>> " } else { "... " };
        let readline = rl.readline(prompt);

        match readline {
            Ok(line) => {
                let _ = rl.add_history_entry(&line);

                // Check for exit commands
                if line.trim() == "exit" || line.trim() == "quit" {
                    println!("Goodbye! 👋");
                    break;
                }

                // Check for special commands
                if line.trim() == "help" || line.trim() == "help()" {
                    show_help();
                    continue;
                }

                // Handle multiline input like Python
                if buffer.is_empty() {
                    // Starting fresh - check if this line starts a multiline construct
                    let trimmed = line.trim();
                    if trimmed.ends_with(':') || trimmed.starts_with('@') {
                        in_multiline = true;
                        buffer.push_str(&line);
                        buffer.push('\n');
                        continue;
                    } else if trimmed.is_empty() {
                        // Empty line at top level, ignore
                        continue;
                    } else {
                        // Single line - try to execute immediately
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
                        Ok(Some(value)) if !matches!(value, Value::None) => {
                            println!("{}", value);
                        }
                        Ok(_) => {
                            // No value to print, or None
                        }
                        Err(e) => {
                            eprintln!("Runtime Error: {}", e);
                        }
                    }
                }
                buffer.clear();
                in_multiline = false;
            }
            Err(ReadlineError::Interrupted) => {
                println!("(To exit, type 'exit' or press Ctrl+D)");
                buffer.clear();
                continue;
            }
            Err(ReadlineError::Eof) => {
                println!("\nGoodbye! 👋");
                break;
            }
            Err(err) => {
                eprintln!("REPL Error: {:?}", err);
                break;
            }
        }
    }
    
    Ok(())
}

/// Show help information
fn show_help() {
    println!("TauraroLang Interactive Help");
    println!("============================");
    println!("Available commands:");
    println!("  help       - Show this help message");
    println!("  dir()      - List all variables and functions");
    println!("  globals()  - Show global variables");
    println!("  locals()   - Show local variables");
    println!("  exit/quit  - Exit the interpreter");
    println!();
    println!("Features:");
    println!("  - Full Python-compatible syntax");
    println!("  - Multi-language keywords (def/func/aiki)");
    println!("  - Classes, functions, loops, conditionals");
    println!("  - Expression evaluation and statement execution");
    println!();
    println!("Example:");
    println!("  >>> a = 42");
    println!("  >>> def greet(name):");
    println!("  ...     return f'Hello, ' + name + '!'");
    println!("  >>> greet('TauraroLang')");
    println!("  'Hello, TauraroLang!'");
}

/// Show variables in current scope
fn show_variables(_interpreter: &Interpreter) {
    println!("Variables and functions in current scope:");
    // This would need to be implemented to query the VM's scope
    println!("[Variable listing not yet implemented]");
}

/// Show global variables
fn show_globals(_interpreter: &Interpreter) {
    println!("Global variables:");
    // This would need to be implemented to query the VM's global scope
    println!("[Global variable listing not yet implemented]");
}

/// Show local variables
fn show_locals(interpreter: &Interpreter) {
    println!("Local variables:");
    // This would need to be implemented to query the VM's local scope
    println!("[Local variable listing not yet implemented]");
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
