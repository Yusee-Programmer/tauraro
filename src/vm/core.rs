use anyhow::{Result, anyhow};
use crate::bytecode::{SuperCompiler, SuperBytecodeVM};
use crate::lexer::Lexer;
use crate::parser::{Parser, ParseError};
use crate::semantic::Analyzer;
use crate::value::Value;
use std::path::Path;

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
        
        // Create a VM instance and use it for execution
        let mut vm = VM::new();
        
        // Normalize the module name by removing the extension
        let main_module_name = Path::new(filename)
            .file_stem()
            .and_then(|s| s.to_str())
            .unwrap_or(filename)
            .to_string();
            
        // Track main module loading to prevent circular imports
        if vm.bytecode_vm.loading_modules.contains(&main_module_name) {
            return Err(anyhow!("ImportError: cannot import name '{}' (circular import)", main_module_name));
        }
        vm.bytecode_vm.loading_modules.insert(main_module_name.clone());
        
        // Execute the code through the VM's execute_script method
        let result = vm.execute_script(source, vec![]);
        
        // Remove main module from loading set
        vm.bytecode_vm.loading_modules.remove(&main_module_name);
        
        result?;
        
        Ok(())
    }
    
    /// Execute TauraroLang source code as a script
    pub fn execute_script(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        self.execute_source(source, args, false)
    }
    
    /// Execute TauraroLang source code in REPL mode
    pub fn execute_repl(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        // Parse first to check if it's a single expression
        use crate::lexer::Lexer;
        use crate::parser::Parser;
        use crate::semantic::Analyzer;
        use crate::bytecode::SuperCompiler;
        use anyhow::anyhow;

        let tokens = Lexer::new(source).collect::<Result<Vec<_>, String>>()
            .map_err(|e| anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);

        let program = parser.parse()
            .map_err(|e| {
                let (line, column) = parser.current_token_location();
                let error_with_location = e.with_location(line, column, "<stdin>");
                anyhow!("{}", error_with_location)
            })?;

        // Check if this is a single expression statement (for REPL auto-print)
        let is_single_expr = program.statements.len() == 1 &&
            matches!(&program.statements[0], crate::ast::Statement::Expression(_));

        // Analyze
        let program = Analyzer::new(false).analyze(program)
            .map_err(|e| anyhow!("Semantic errors: {:?}", e))?;

        // Compile to bytecode
        let mut compiler = SuperCompiler::new("<stdin>".to_string());
        let code_object = compiler.compile(program)?;

        // Execute the program
        self.bytecode_vm.enable_type_checking = false;
        let result = self.bytecode_vm.execute(code_object)?;

        // In REPL mode, if it was a single expression, return the result
        // Otherwise return None (like Python REPL)
        if is_single_expr {
            Ok(result)
        } else {
            Ok(Value::None)
        }
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
        let mut compiler = SuperCompiler::new("<module>".to_string());
        let code_object = compiler.compile(program)?;
        
        // Execute the program
        // Disable type checking for better performance
        self.bytecode_vm.enable_type_checking = false;
        let result = self.bytecode_vm.execute(code_object)?;
        
        Ok(result)
    }
}