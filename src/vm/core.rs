//! Core virtual machine implementation

use anyhow::{Result, anyhow};
use crate::bytecode::{SuperCompiler, SuperBytecodeVM};
use crate::lexer::Lexer;
use crate::parser::{Parser, ParseError};
use crate::semantic::Analyzer;
use crate::value::Value;

pub struct VM {
    bytecode_vm: SuperBytecodeVM,
}

impl VM {
    pub fn new() -> Self {
        Self {
            bytecode_vm: SuperBytecodeVM::new(),
        }
    }
    
    pub fn run_file_with_options(source: &str, filename: &str, _backend: &str, _optimization: u8, strict_types: bool) -> Result<()> {
        println!("Running file with VM backend");
        
        // Lexical analysis
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, String>>()
            .map_err(|e| anyhow!("{}", e))?;
        
        // Parsing
        let mut parser = Parser::new(tokens);
        let ast = parser.parse()
            .map_err(|e| {
                // Find the token that caused the error to get line/column info
                let (line, column) = parser.current_token_location();
                let error_with_location = e.with_location(line, column, filename);
                anyhow!("{}", error_with_location)
            })?;
        
        // Semantic analysis
        let semantic_ast = Analyzer::new(strict_types).analyze(ast)
            .map_err(|e| anyhow!("Semantic errors: {:?}", e))?;
        
        // Compile to bytecode
        let mut compiler = SuperCompiler::new(filename.to_string());
        let code_object = compiler.compile(semantic_ast)?;
        
        // Execute with VM
        let mut vm = SuperBytecodeVM::new();
        // Disable type checking for better performance unless explicitly requested
        vm.enable_type_checking = strict_types;
        vm.execute(code_object)?;
        
        Ok(())
    }
    
    /// Execute TauraroLang source code as a script
    pub fn execute_script(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        self.execute_source(source, args, false)
    }
    
    /// Execute TauraroLang source code in REPL mode
    pub fn execute_repl(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        self.execute_source(source, args, true)
    }
    
    /// Get a variable from the VM
    pub fn get_variable(&self, _name: &str) -> Option<Value> {
        // For the simplified VM, we don't have direct variable access
        None
    }
    
    /// Set a variable in the VM
    pub fn set_variable(&mut self, _name: &str, _value: Value) -> Result<()> {
        // For the simplified VM, we don't have direct variable access
        Ok(())
    }
    
    /// Execute TauraroLang source code with mode specification
    fn execute_source(&mut self, source: &str, args: Vec<String>, _is_repl: bool) -> Result<Value> {
        // Parse the source code
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, String>>()
            .map_err(|e| anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);

        let program = parser.parse()
            .map_err(|e| {
                // Find the token that caused the error to get line/column info
                let (line, column) = parser.current_token_location();
                let error_with_location = e.with_location(line, column, "<main>");
                anyhow!("{}", error_with_location)
            })?;
        
        // Optional semantic analysis based on strict mode
        let program = Analyzer::new(false).analyze(program)
            .map_err(|e| anyhow!("Semantic errors: {:?}", e))?;
        
        // Compile to bytecode
        let mut compiler = SuperCompiler::new("<main>".to_string());
        let code_object = compiler.compile(program)?;
        
        // Execute the program
        // Disable type checking for better performance
        self.bytecode_vm.enable_type_checking = false;
        let result = self.bytecode_vm.execute(code_object)?;
        
        Ok(result)
    }
}

impl Default for VM {
    fn default() -> Self {
        Self::new()
    }
}