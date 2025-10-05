//! Main VM loop (fetch-decode-execute)
use crate::vm::frame::ExecutionFrame;
use crate::vm::stack::StackFrame;
use crate::vm::memory::Scope;
use crate::ast::*;
use crate::value::Value;
use crate::parser::Parser;
use crate::lexer::Lexer;
use crate::semantic;
use crate::module_system::ModuleSystem;
use crate::package_manager::PackageManager;
use crate::object_system::{TypeRegistry, TypeCreator};
use crate::runtime::MemoryAPI;
use crate::modules::hplist::HPList;
use crate::vm::builtins::Builtins;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;

/// Virtual Machine state
#[derive(Clone)]
pub struct VM {
    pub scopes: Vec<Scope>,
    current_scope: usize,
    memory: MemoryAPI,
    call_stack: Vec<StackFrame>,
    strict_types: bool,
    should_return: bool,
    return_value: Option<Value>,
    module_system: ModuleSystem,
    
    // CPython-inspired execution model
    pub current_frame: ExecutionFrame,
    pub frame_stack: Vec<ExecutionFrame>,
    pub type_registry: TypeRegistry,

    package_manager: PackageManager,
    class_registry: HashMap<String, Vec<String>>, // Track class MROs for C3 linearization
    class_defined_methods: HashMap<String, std::collections::HashSet<String>>, // Track methods defined in each class
    class_base_registry: HashMap<String, Vec<String>>, // Track base classes for each class
    type_creator: TypeCreator, // Optimized metaclass and MRO system
    pub current_executing_class: Option<String>, // Track which class method is currently executing for super() calls
    
    // Performance optimization caches
    variable_cache: HashMap<String, Value>, // Cache for frequently accessed variables
    function_cache: HashMap<String, (Vec<Param>, Vec<Statement>)>, // Cache for function definitions
    
    // Super-optimized bytecode caching system
    bytecode_cache: HashMap<String, Vec<crate::bytecode::instructions::Instruction>>, // Cache compiled bytecode
    method_cache: HashMap<String, Value>, // Cache bound method objects
    
    // Predictive variable access optimization
    variable_access_patterns: HashMap<String, usize>, // Track variable access frequency
    fast_local_indices: HashMap<String, usize>, // Fast local variable indices
    global_cache: HashMap<String, Value>, // Global variable cache with versioning
    
    // String interning for memory optimization
    string_interner: HashMap<String, Rc<String>>, // Interned strings for reduced memory usage
}

impl VM {
    pub fn new() -> Self {
        let global_scope = Scope::new();
        
        let mut vm = Self {
            scopes: vec![global_scope],
            current_scope: 0,
            memory: MemoryAPI::new(),
            call_stack: Vec::new(),
            strict_types: false,
            should_return: false,
            return_value: None,
            module_system: ModuleSystem::new(),
            
            // CPython-inspired execution model
            current_frame: ExecutionFrame::new("__main__".to_string()),
            frame_stack: Vec::new(),
            type_registry: TypeRegistry::new(),

            package_manager: PackageManager::new(),
            class_registry: HashMap::new(),
            class_defined_methods: HashMap::new(),
            class_base_registry: HashMap::new(),
            type_creator: TypeCreator::new(),
            current_executing_class: None,
            
            // Performance optimization caches
            variable_cache: HashMap::new(),
            function_cache: HashMap::new(),
            
            // Super-optimized bytecode caching system
            bytecode_cache: HashMap::new(),
            method_cache: HashMap::new(),
            
            // Predictive variable access optimization
            variable_access_patterns: HashMap::new(),
            fast_local_indices: HashMap::new(),
            global_cache: HashMap::new(),
            
            // String interning for memory optimization
            string_interner: HashMap::new(),
        };
        
        // Initialize built-in modules with memory management capabilities
        vm.init_builtins();
        
        // Initialize package manager and integrate with module system
        vm.init_package_system();
        
        // Set __name__ to "__main__" by default (like Python)
        vm.set_variable_internal("__name__", Value::Str("__main__".to_string())).unwrap_or(());
        
        vm
    }
    
    /// Initialize built-in functions and constants
    fn init_builtins(&mut self) {
        let builtins = Builtins::init_builtins();
        for (name, value) in builtins {
            self.current_frame.builtins.insert(name, value);
        }
    }
    
    /// Initialize package system
    fn init_package_system(&mut self) {
        // This would contain the logic to initialize the package system
        // For now, we'll leave it empty
    }
    
    /// Set a variable in the current scope (internal method)
    fn set_variable_internal(&mut self, name: &str, value: Value) -> Result<()> {
        self.scopes[self.current_scope].variables.insert(name.to_string(), value);
        Ok(())
    }
    
    /// Set a variable in the current scope (public method)
    pub fn set_variable(&mut self, name: &str, value: Value) -> Result<()> {
        self.set_variable_internal(name, value)
    }
    
    /// Get a variable from the current scope
    pub fn get_variable(&self, name: &str) -> Option<Value> {
        self.scopes[self.current_scope].variables.get(name).cloned()
    }
    
    /// Execute a program
    fn execute_program(&mut self, _program: Program, _is_repl: bool) -> Result<Value> {
        // This is a simplified implementation
        // In a real VM, this would contain the full program execution logic
        Ok(Value::None)
    }
    
    /// Set strict type checking mode
    pub fn set_strict_types(&mut self, strict: bool) {
        self.strict_types = strict;
    }
    
    /// Execute TauraroLang source code as a script
    pub fn execute_script(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        self.execute_source(source, args, false)
    }
    
    /// Execute TauraroLang source code in REPL mode
    pub fn execute_repl(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        self.execute_source(source, args, true)
    }
    
    /// Execute TauraroLang source code with mode specification
    fn execute_source(&mut self, source: &str, args: Vec<String>, is_repl: bool) -> Result<Value> {
        // Parse the source code
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        let program = if is_repl {
            // In REPL mode, parse as-is (single line at a time)
            parser.parse()?
        } else {
            // In script mode, always use implicit main to handle multiple statements correctly
            parser.parse_with_implicit_main()?
        };
        
        // Optional semantic analysis based on strict mode
        let program = if self.strict_types {
            semantic::analyze_optional_types(program, true)
                .map_err(|errors| anyhow::anyhow!("Semantic errors: {:?}", errors))?
        } else {
            program
        };
        
        // Set command line arguments
        self.set_variable_internal("args", Value::List(HPList::from_values(args.into_iter().map(Value::Str).collect())))?;
        
        // Execute the program
        self.execute_program(program, is_repl)
    }
    
    /// Push a new scope onto the scope stack
    pub fn push_scope(&mut self, scope: Scope) {
        self.scopes.push(scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    /// Pop the current scope from the scope stack
    pub fn pop_scope(&mut self) -> Scope {
        if self.scopes.len() > 1 {
            self.current_scope = self.scopes.len() - 2;
        } else {
            self.current_scope = 0;
        }
        self.scopes.pop().unwrap_or_else(|| Scope::new())
    }
    
    /// Execute a single statement
    pub fn execute_statement(&mut self, _statement: &Statement) -> Result<()> {
        // This is a placeholder implementation
        // In a real VM, this would contain the logic to execute a single statement
        Ok(())
    }
    
    /// Run a file with options (replaces the old run_file_with_options function)
    pub fn run_file_with_options(source: &str, _backend: &str, _optimization: u8, strict_types: bool) -> Result<Value> {
        let mut vm = VM::new();
        vm.set_strict_types(strict_types);
        vm.execute_script(source, vec![])
    }
}