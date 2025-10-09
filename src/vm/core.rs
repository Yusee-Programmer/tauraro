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
// Removed - using crate::builtins::init_builtins() instead
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;

/// Virtual Machine state
pub struct VM {
    pub scopes: Vec<Scope>,
    current_scope: usize,
    memory: MemoryAPI,
    call_stack: Vec<StackFrame>,
    strict_types: bool,
    should_return: bool,
    return_value: Option<Value>,
    should_break: bool,
    should_continue: bool,
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

    // Persistent bytecode VM for REPL mode
    bytecode_vm: Option<crate::bytecode::SuperBytecodeVM>,
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
            should_break: false,
            should_continue: false,
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

            // Persistent bytecode VM for REPL mode
            bytecode_vm: None,
        };

        // Initialize built-in modules with memory management capabilities
        vm.init_builtins();
        
        // Initialize package system and integrate with module system
        vm.init_package_system();
        
        // Set __name__ to "__main__" by default (like Python)
        vm.set_variable_internal("__name__", Value::Str("__main__".to_string())).unwrap_or(());
        
        vm
    }
    
    /// Initialize built-in functions and constants
    fn init_builtins(&mut self) {
        let builtins = crate::builtins::init_builtins();
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
    fn execute_program(&mut self, program: Program, is_repl: bool) -> Result<Value> {
        // Use SuperBytecodeVM for maximum performance (debug output now removed)
        use crate::bytecode::{SuperCompiler, SuperBytecodeVM};

        // Compile program to bytecode
        let mut compiler = SuperCompiler::new("<main>".to_string());
        let code_object = compiler.compile(program)?;

        // In REPL mode, maintain persistent bytecode VM to preserve state
        if is_repl {
            // Initialize bytecode_vm if not already created
            if self.bytecode_vm.is_none() {
                self.bytecode_vm = Some(SuperBytecodeVM::new());
            }

            // Get mutable reference to the persistent VM
            if let Some(ref mut bytecode_vm) = self.bytecode_vm {
                let result = bytecode_vm.execute(code_object)?;
                return Ok(result);
            }
        }

        // For non-REPL mode, use a fresh VM instance
        let mut bytecode_vm = SuperBytecodeVM::new();
        let result = bytecode_vm.execute(code_object)?;

        Ok(result)
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
        // Parse the source code with REPL-specific parsing
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);

        // Use special REPL parser that detects expressions
        let (program, is_expression) = parser.parse_repl_line()?;

        // Optional semantic analysis
        let program = if self.strict_types {
            semantic::analyze_optional_types(program, true)
                .map_err(|errors| anyhow::anyhow!("Semantic errors: {:?}", errors))?
        } else {
            program
        };

        // Set command line arguments
        self.set_variable_internal("args", Value::List(HPList::from_values(args.into_iter().map(Value::Str).collect())))?;

        // Execute with special REPL flag
        let _result = self.execute_program(program, true)?;

        // If it was an expression, retrieve the __last_expr__ value from bytecode VM
        if is_expression {
            if let Some(ref bytecode_vm) = self.bytecode_vm {
                // Try to get __last_expr__ from the bytecode VM's globals
                if let Some(value) = bytecode_vm.get_global("__last_expr__") {
                    return Ok(value.value.clone());
                }
            }
        }

        Ok(Value::None)
    }

    /// Execute TauraroLang source code with mode specification
    fn execute_source(&mut self, source: &str, args: Vec<String>, is_repl: bool) -> Result<Value> {
        // Parse the source code
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);

        let program = if is_repl {
            // This path shouldn't be used anymore (use execute_repl directly)
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
    pub fn execute_statement(&mut self, statement: &Statement) -> Result<()> {
        match statement {
            Statement::Expression(expr) => {
                // Evaluate the expression and if it's a function call like print(), execute it
                self.evaluate_expression(expr)?;
                Ok(())
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let val = self.evaluate_expression(expr)?;
                    self.set_variable_internal(name, val)?;
                } else {
                    self.set_variable_internal(name, Value::None)?;
                }
                Ok(())
            }
            Statement::SubscriptAssignment { object, index, value } => {
                // Handle subscript assignment like dict["key"] = value or list[0] = value
                let value_val = self.evaluate_expression(value)?;
                let index_val = self.evaluate_expression(index)?;

                // Handle case where object is an identifier
                if let Expr::Identifier(var_name) = object {
                    // Find the variable and mutate it in place - CRITICAL for performance
                    let mut var_found = false;
                    for scope_idx in (0..=self.current_scope).rev() {
                        if self.scopes[scope_idx].variables.contains_key(var_name) {
                            // Get mutable reference to the variable without cloning
                            let var_value = self.scopes[scope_idx].variables.get_mut(var_name).unwrap();

                            match (var_value, &index_val) {
                                (Value::Dict(dict), Value::Str(key)) => {
                                    dict.insert(key.clone(), value_val);
                                    var_found = true;
                                    break;
                                }
                                (Value::List(list), Value::Int(idx)) => {
                                    let actual_idx = if *idx < 0 {
                                        (list.len() as i64 + idx) as isize
                                    } else {
                                        *idx as isize
                                    };

                                    if let Err(e) = list.set(actual_idx, value_val) {
                                        return Err(anyhow::anyhow!("Index out of range: {}", e));
                                    }
                                    var_found = true;
                                    break;
                                }
                                (val, _) => {
                                    return Err(anyhow::anyhow!("Subscript assignment not supported for {} with index type {}", val.type_name(), index_val.type_name()));
                                }
                            }
                        }
                    }

                    if !var_found {
                        return Err(anyhow::anyhow!("Variable '{}' is not defined", var_name));
                    }
                } else {
                    return Err(anyhow::anyhow!("Subscript assignment only supported for identifiers"));
                }
                Ok(())
            }
            // Add more statement types as needed
            _ => {
                // For unimplemented statements, we'll just return Ok for now
                // In a complete implementation, we would handle all statement types
                Ok(())
            }
        }
    }
    
    /// Evaluate an expression and return its value
    fn evaluate_expression(&mut self, expr: &Expr) -> Result<Value> {
        match expr {
            Expr::Literal(literal) => {
                match literal {
                    Literal::Int(n) => Ok(Value::Int(*n)),
                    Literal::Float(n) => Ok(Value::Float(*n)),
                    Literal::String(s) => Ok(Value::Str(s.clone())),
                    Literal::Bool(b) => Ok(Value::Bool(*b)),
                    Literal::None => Ok(Value::None),
                    _ => Err(anyhow::anyhow!("Unsupported literal type")),
                }
            }
            Expr::Identifier(name) => {
                // Look up variable in scopes
                for scope in self.scopes.iter().rev() {
                    if let Some(value) = scope.variables.get(name) {
                        return Ok(value.clone());
                    }
                }
                // Check built-ins
                if let Some(value) = self.current_frame.builtins.get(name) {
                    return Ok(value.clone());
                }
                Err(anyhow::anyhow!("Name '{}' is not defined", name))
            }
            Expr::BinaryOp { left, op, right } => {
                let left_val = self.evaluate_expression(left)?;
                let right_val = self.evaluate_expression(right)?;
                
                match op {
                    BinaryOp::Add => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
                            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
                            (Value::Str(a), Value::Str(b)) => Ok(Value::Str(format!("{}{}", a, b))),
                            _ => Err(anyhow::anyhow!("Unsupported operand types for +")),
                        }
                    }
                    BinaryOp::Sub => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
                            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
                            _ => Err(anyhow::anyhow!("Unsupported operand types for -")),
                        }
                    }
                    BinaryOp::Mul => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
                            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
                            (Value::Str(s), Value::Int(n)) => {
                                if *n >= 0 {
                                    Ok(Value::Str(s.repeat(*n as usize)))
                                } else {
                                    Err(anyhow::anyhow!("String repetition count must be non-negative"))
                                }
                            }
                            (Value::Int(n), Value::Str(s)) => {
                                if *n >= 0 {
                                    Ok(Value::Str(s.repeat(*n as usize)))
                                } else {
                                    Err(anyhow::anyhow!("String repetition count must be non-negative"))
                                }
                            }
                            _ => Err(anyhow::anyhow!("Unsupported operand types for *")),
                        }
                    }
                    BinaryOp::Div => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => {
                                if *b == 0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    // In Python, division always returns float
                                    Ok(Value::Float(*a as f64 / *b as f64))
                                }
                            },
                            (Value::Float(a), Value::Float(b)) => {
                                if *b == 0.0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Float(a / b))
                                }
                            },
                            (Value::Int(a), Value::Float(b)) => {
                                if *b == 0.0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Float(*a as f64 / b))
                                }
                            },
                            (Value::Float(a), Value::Int(b)) => {
                                if *b == 0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Float(a / *b as f64))
                                }
                            },
                            _ => Err(anyhow::anyhow!("Unsupported operand types for /")),
                        }
                    }
                    BinaryOp::FloorDiv => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => {
                                if *b == 0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Int(a / b))
                                }
                            },
                            (Value::Float(a), Value::Float(b)) => {
                                if *b == 0.0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Float((a / b).floor()))
                                }
                            },
                            (Value::Int(a), Value::Float(b)) => {
                                if *b == 0.0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Float((*a as f64 / b).floor()))
                                }
                            },
                            (Value::Float(a), Value::Int(b)) => {
                                if *b == 0 {
                                    Err(anyhow::anyhow!("Division by zero"))
                                } else {
                                    Ok(Value::Float((a / *b as f64).floor()))
                                }
                            },
                            _ => Err(anyhow::anyhow!("Unsupported operand types for //")),
                        }
                    }
                    BinaryOp::Mod => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => {
                                if *b == 0 {
                                    Err(anyhow::anyhow!("Modulo by zero"))
                                } else {
                                    Ok(Value::Int(a % b))
                                }
                            },
                            (Value::Float(a), Value::Float(b)) => {
                                if *b == 0.0 {
                                    Err(anyhow::anyhow!("Modulo by zero"))
                                } else {
                                    Ok(Value::Float(a % b))
                                }
                            },
                            (Value::Int(a), Value::Float(b)) => {
                                if *b == 0.0 {
                                    Err(anyhow::anyhow!("Modulo by zero"))
                                } else {
                                    Ok(Value::Float(*a as f64 % b))
                                }
                            },
                            (Value::Float(a), Value::Int(b)) => {
                                if *b == 0 {
                                    Err(anyhow::anyhow!("Modulo by zero"))
                                } else {
                                    Ok(Value::Float(a % *b as f64))
                                }
                            },
                            _ => Err(anyhow::anyhow!("Unsupported operand types for %")),
                        }
                    }
                    BinaryOp::Pow => {
                        match (&left_val, &right_val) {
                            (Value::Int(a), Value::Int(b)) => {
                                if *b >= 0 {
                                    Ok(Value::Int(a.pow(*b as u32)))
                                } else {
                                    // Negative exponent, return float
                                    Ok(Value::Float((*a as f64).powf(*b as f64)))
                                }
                            },
                            (Value::Float(a), Value::Float(b)) => {
                                Ok(Value::Float(a.powf(*b)))
                            },
                            (Value::Int(a), Value::Float(b)) => {
                                Ok(Value::Float((*a as f64).powf(*b)))
                            },
                            (Value::Float(a), Value::Int(b)) => {
                                Ok(Value::Float(a.powf(*b as f64)))
                            },
                            _ => Err(anyhow::anyhow!("Unsupported operand types for **")),
                        }
                    }
                    BinaryOp::Eq => Ok(Value::Bool(left_val == right_val)),
                    BinaryOp::Ne | BinaryOp::Neq => Ok(Value::Bool(left_val != right_val)),
                    BinaryOp::Lt => Ok(Value::Bool(left_val < right_val)),
                    BinaryOp::Gt => Ok(Value::Bool(left_val > right_val)),
                    BinaryOp::Le | BinaryOp::Lte => Ok(Value::Bool(left_val <= right_val)),
                    BinaryOp::Ge | BinaryOp::Gte => Ok(Value::Bool(left_val >= right_val)),
                    BinaryOp::And => {
                        // Short-circuit evaluation for 'and'
                        if !left_val.is_truthy() {
                            Ok(left_val)
                        } else {
                            Ok(right_val)
                        }
                    }
                    BinaryOp::Or => {
                        // Short-circuit evaluation for 'or'
                        if left_val.is_truthy() {
                            Ok(left_val)
                        } else {
                            Ok(right_val)
                        }
                    }
                    // Add more operations as needed
                    _ => Err(anyhow::anyhow!("Unsupported binary operation")),
                }
            }
            Expr::UnaryOp { op, operand } => {
                let operand_val = self.evaluate_expression(operand)?;
                
                match op {
                    UnaryOp::Not => Ok(Value::Bool(!operand_val.is_truthy())),
                    UnaryOp::USub => {
                        match operand_val {
                            Value::Int(n) => Ok(Value::Int(-n)),
                            Value::Float(n) => Ok(Value::Float(-n)),
                            _ => Err(anyhow::anyhow!("Unsupported operand type for unary -")),
                        }
                    }
                    UnaryOp::UAdd => {
                        match operand_val {
                            Value::Int(n) => Ok(Value::Int(n)),
                            Value::Float(n) => Ok(Value::Float(n)),
                            _ => Err(anyhow::anyhow!("Unsupported operand type for unary +")),
                        }
                    }
                    _ => Err(anyhow::anyhow!("Unsupported unary operation")),
                }
            }
            Expr::Call { func, args, .. } => {
                let func_val = self.evaluate_expression(func)?;
                let mut arg_vals = Vec::new();
                for arg in args {
                    arg_vals.push(self.evaluate_expression(arg)?);
                }
                
                match func_val {
                    Value::BuiltinFunction(_, func_ptr) => {
                        func_ptr(arg_vals)
                    }
                    _ => Err(anyhow::anyhow!("'{}' object is not callable", func_val.type_name())),
                }
            }
            Expr::MethodCall { object, method, args, kwargs } => {
                // First evaluate arguments
                let mut arg_vals = Vec::new();
                for arg in args {
                    arg_vals.push(self.evaluate_expression(arg)?);
                }

                // Handle keyword arguments if any
                if !kwargs.is_empty() {
                    return Err(anyhow::anyhow!("Keyword arguments not yet supported in method calls"));
                }

                // Special handling for mutating methods (append, update, add, etc.)
                // These need to modify the object in place
                if matches!(method.as_str(), "append" | "extend" | "insert" | "remove" | "pop" | "clear" | "sort" | "reverse") {
                    // Get the variable name if object is an identifier
                    if let Expr::Identifier(var_name) = object.as_ref() {
                        // Get the current value
                        let obj_val = self.get_variable(var_name)
                            .ok_or_else(|| anyhow::anyhow!("Variable '{}' not found", var_name))?;

                        match (&obj_val, method.as_str()) {
                            (Value::List(list), "append") => {
                                if arg_vals.len() == 1 {
                                    let mut new_list = list.clone();
                                    new_list.append(arg_vals[0].clone());
                                    // Update the variable in scope
                                    self.set_variable_internal(var_name, Value::List(new_list))?;
                                    Ok(Value::None)
                                } else {
                                    Err(anyhow::anyhow!("append() takes exactly one argument"))
                                }
                            }
                            _ => Err(anyhow::anyhow!("Method '{}' not found on object of type '{}'", method, obj_val.type_name())),
                        }
                    } else {
                        // Object is not a simple identifier, can't mutate in place
                        Err(anyhow::anyhow!("Cannot call mutating method '{}' on non-identifier expression", method))
                    }
                } else {
                    // Non-mutating methods - evaluate object normally
                    let obj_val = self.evaluate_expression(object)?;

                    match (&obj_val, method.as_str()) {
                        // Non-mutating list methods would go here
                        _ => Err(anyhow::anyhow!("Method '{}' not found on object of type '{}'", method, obj_val.type_name())),
                    }
                }
            }
            Expr::Compare { left, ops, comparators } => {
                // Evaluate left operand
                let left_val = self.evaluate_expression(left)?;
                
                // For now, we'll handle simple comparisons (one operator, one comparator)
                if ops.len() == 1 && comparators.len() == 1 {
                    let right_val = self.evaluate_expression(&comparators[0])?;
                    let op = &ops[0];
                    
                    match op {
                        CompareOp::Eq => Ok(Value::Bool(left_val == right_val)),
                        CompareOp::NotEq => Ok(Value::Bool(left_val != right_val)),
                        CompareOp::Lt => Ok(Value::Bool(left_val < right_val)),
                        CompareOp::LtE => Ok(Value::Bool(left_val <= right_val)),
                        CompareOp::Gt => Ok(Value::Bool(left_val > right_val)),
                        CompareOp::GtE => Ok(Value::Bool(left_val >= right_val)),
                        _ => Err(anyhow::anyhow!("Unsupported comparison operator")),
                    }
                } else {
                    Err(anyhow::anyhow!("Chained comparisons not yet supported"))
                }
            }
            Expr::List(elements) => {
                let mut values = Vec::new();
                for element in elements {
                    values.push(self.evaluate_expression(element)?);
                }
                Ok(Value::List(HPList::from_values(values)))
            }
            Expr::Tuple(elements) => {
                let mut values = Vec::new();
                for element in elements {
                    values.push(self.evaluate_expression(element)?);
                }
                Ok(Value::Tuple(values))
            }
            Expr::Dict(pairs) => {
                let mut dict = std::collections::HashMap::new();
                for (key, value) in pairs {
                    let key_val = self.evaluate_expression(key)?;
                    let val = self.evaluate_expression(value)?;
                    // Convert key to string for simplicity
                    let key_str = format!("{}", key_val);
                    dict.insert(key_str, val);
                }
                Ok(Value::Dict(dict))
            }
            Expr::Set(elements) => {
                let mut values = Vec::new();
                for element in elements {
                    let val = self.evaluate_expression(element)?;
                    // Check if value is already in the set to maintain uniqueness
                    if !values.contains(&val) {
                        values.push(val);
                    }
                }
                Ok(Value::Set(values))
            }
            Expr::Subscript { object, index } => {
                let obj_val = self.evaluate_expression(object)?;
                let index_val = self.evaluate_expression(index)?;
                
                match (&obj_val, &index_val) {
                    (Value::List(list), Value::Int(idx)) => {
                        // Handle negative indices
                        let idx = if *idx < 0 {
                            list.len() as i64 + *idx
                        } else {
                            *idx
                        };
                        
                        // Check bounds and get value
                        // Convert i64 to isize safely
                        if let Ok(idx_isize) = idx.try_into() {
                            match list.get(idx_isize) {
                                Some(val) => Ok(val.clone()),
                                None => Err(anyhow::anyhow!("Index out of range")),
                            }
                        } else {
                            Err(anyhow::anyhow!("Index out of range"))
                        }
                    }
                    (Value::Tuple(items), Value::Int(idx)) => {
                        // Handle negative indices
                        let idx = if *idx < 0 {
                            items.len() as i64 + *idx
                        } else {
                            *idx
                        };
                        
                        // Check bounds and get value
                        if idx >= 0 && idx < items.len() as i64 {
                            Ok(items[idx as usize].clone())
                        } else {
                            Err(anyhow::anyhow!("Index out of range"))
                        }
                    }
                    (Value::Dict(dict), Value::Str(key)) => {
                        match dict.get(key) {
                            Some(val) => Ok(val.clone()),
                            None => Err(anyhow::anyhow!("Key '{}' not found", key)),
                        }
                    }
                    _ => Err(anyhow::anyhow!("Subscript operation not supported for these types")),
                }
            }
            // Add more expression types as needed
            _ => Err(anyhow::anyhow!("Unsupported expression type: {:?}", expr)),
        }
    }
    
    /// Run a file with options (replaces the old run_file_with_options function)
    pub fn run_file_with_options(source: &str, _backend: &str, _optimization: u8, strict_types: bool) -> Result<Value> {
        let mut vm = VM::new();
        vm.set_strict_types(strict_types);
        vm.execute_script(source, vec![])
    }
}

// Implement the CompleteExecutor trait for VM
use crate::vm::executor::CompleteExecutor;

impl CompleteExecutor for VM {
    fn execute_statement_complete(&mut self, statement: &Statement) -> Result<()> {
        match statement {
            Statement::For { variable, iterable, body, else_branch } => {
                crate::vm::executor::execute_for_loop(self, variable, iterable, body)?;
                // Handle else branch if loop didn't break
                if !self.should_break {
                    if let Some(else_body) = else_branch {
                        for stmt in else_body {
                            self.execute_statement_complete(stmt)?;
                            if self.check_return() || self.check_break() || self.check_continue() {
                                break;
                            }
                        }
                    }
                }
                Ok(())
            }
            Statement::While { condition, body, else_branch } => {
                crate::vm::executor::execute_while_loop(self, condition, body)?;
                // Handle else branch if loop didn't break
                if !self.should_break {
                    if let Some(else_body) = else_branch {
                        for stmt in else_body {
                            self.execute_statement_complete(stmt)?;
                            if self.check_return() || self.check_break() || self.check_continue() {
                                break;
                            }
                        }
                    }
                }
                Ok(())
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                crate::vm::executor::execute_if_statement(
                    self, condition, then_branch, elif_branches, else_branch
                )
            }
            Statement::Return(expr) => {
                let value = if let Some(e) = expr {
                    self.evaluate_expression_complete(e)?
                } else {
                    Value::None
                };
                self.set_return(Some(value));
                Ok(())
            }
            Statement::Break => {
                self.set_break(true);
                Ok(())
            }
            Statement::Continue => {
                self.set_continue(true);
                Ok(())
            }
            Statement::Expression(expr) => {
                self.evaluate_expression_complete(expr)?;
                Ok(())
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let val = self.evaluate_expression_complete(expr)?;
                    self.set_variable_internal(name, val)?;
                } else {
                    self.set_variable_internal(name, Value::None)?;
                }
                Ok(())
            }
            Statement::SubscriptAssignment { object, index, value } => {
                let value_val = self.evaluate_expression_complete(value)?;
                let index_val = self.evaluate_expression_complete(index)?;

                // Handle case where object is an identifier
                if let Expr::Identifier(var_name) = object {
                    // Find the variable and mutate it in place - CRITICAL for performance
                    let mut var_found = false;
                    for scope_idx in (0..=self.current_scope).rev() {
                        if self.scopes[scope_idx].variables.contains_key(var_name) {
                            // Get mutable reference to the variable without cloning
                            let var_value = self.scopes[scope_idx].variables.get_mut(var_name).unwrap();

                            match (var_value, &index_val) {
                                (Value::Dict(dict), Value::Str(key)) => {
                                    dict.insert(key.clone(), value_val);
                                    var_found = true;
                                    break;
                                }
                                (Value::List(list), Value::Int(idx)) => {
                                    let actual_idx = if *idx < 0 {
                                        (list.len() as i64 + idx) as isize
                                    } else {
                                        *idx as isize
                                    };

                                    if let Err(e) = list.set(actual_idx, value_val) {
                                        return Err(anyhow!("Index out of range: {}", e));
                                    }
                                    var_found = true;
                                    break;
                                }
                                (val, _) => {
                                    return Err(anyhow!("Subscript assignment not supported for {} with index type {}", val.type_name(), index_val.type_name()));
                                }
                            }
                        }
                    }
                    if !var_found {
                        return Err(anyhow!("Variable '{}' is not defined", var_name));
                    }
                } else {
                    return Err(anyhow!("Subscript assignment only supported for identifiers"));
                }
                Ok(())
            }
            Statement::FunctionDef { name, params, body, .. } => {
                // Store function definition
                let func_value = Value::Closure {
                    name: name.clone(),
                    params: params.clone(),
                    body: body.clone(),
                    captured_scope: HashMap::new(),
                    docstring: None,
                    compiled_code: None,
                };
                self.set_variable_internal(name, func_value)?;
                Ok(())
            }
            _ => {
                // For other statements, use the existing implementation
                self.execute_statement(statement)
            }
        }
    }

    #[inline(always)]
    fn evaluate_expression_complete(&mut self, expr: &Expr) -> Result<Value> {
        match expr {
            // Fast path for literals - most common case
            Expr::Literal(literal) => {
                match literal {
                    Literal::Int(n) => Ok(Value::Int(*n)),
                    Literal::Float(n) => Ok(Value::Float(*n)),
                    Literal::String(s) => Ok(Value::Str(s.clone())),
                    Literal::Bool(b) => Ok(Value::Bool(*b)),
                    Literal::None => Ok(Value::None),
                    _ => self.evaluate_expression(expr),
                }
            }
            // Fast path for identifiers - very hot in loops
            Expr::Identifier(name) => {
                // Inline scope lookup for performance
                for scope in self.scopes[..=self.current_scope].iter().rev() {
                    if let Some(value) = scope.variables.get(name) {
                        return Ok(value.clone());
                    }
                }
                // Check built-ins
                self.evaluate_expression(expr)
            }
            // Fast path for subscript operations - critical for array access
            Expr::Subscript { object, index } => {
                let obj_val = self.evaluate_expression_complete(object)?;
                let index_val = self.evaluate_expression_complete(index)?;

                match (&obj_val, &index_val) {
                    (Value::List(list), Value::Int(idx)) => {
                        let actual_idx = if *idx < 0 {
                            (list.len() as i64 + idx) as isize
                        } else {
                            *idx as isize
                        };
                        list.get(actual_idx)
                            .cloned()
                            .ok_or_else(|| anyhow!("list index out of range"))
                    }
                    (Value::Tuple(items), Value::Int(idx)) => {
                        let actual_idx = if *idx < 0 {
                            (items.len() as i64 + idx) as usize
                        } else {
                            *idx as usize
                        };
                        items.get(actual_idx)
                            .cloned()
                            .ok_or_else(|| anyhow!("tuple index out of range"))
                    }
                    _ => self.evaluate_expression(expr),
                }
            }
            // Fast path for binary operations
            Expr::BinaryOp { left, op, right } => {
                let left_val = self.evaluate_expression_complete(left)?;
                let right_val = self.evaluate_expression_complete(right)?;

                // Ultra-fast path for integer operations (most common in loops)
                if let (Value::Int(a), Value::Int(b)) = (&left_val, &right_val) {
                    return Ok(match op {
                        BinaryOp::Add => Value::Int(a + b),
                        BinaryOp::Sub => Value::Int(a - b),
                        BinaryOp::Mul => Value::Int(a * b),
                        BinaryOp::FloorDiv => {
                            if *b == 0 {
                                return Err(anyhow!("Division by zero"));
                            }
                            Value::Int(a / b)
                        }
                        BinaryOp::Mod => {
                            if *b == 0 {
                                return Err(anyhow!("Modulo by zero"));
                            }
                            Value::Int(a % b)
                        }
                        BinaryOp::Lt => Value::Bool(a < b),
                        BinaryOp::Le | BinaryOp::Lte => Value::Bool(a <= b),
                        BinaryOp::Gt => Value::Bool(a > b),
                        BinaryOp::Ge | BinaryOp::Gte => Value::Bool(a >= b),
                        BinaryOp::Eq => Value::Bool(a == b),
                        BinaryOp::Ne | BinaryOp::Neq => Value::Bool(a != b),
                        _ => {
                            // Fall through to generic handler
                            return crate::vm::executor::evaluate_binop_fast(left_val, op, right_val);
                        }
                    });
                }

                crate::vm::executor::evaluate_binop_fast(left_val, op, right_val)
            }
            Expr::Call { func, args, kwargs } => {
                // Evaluate function
                let func_val = self.evaluate_expression_complete(func)?;

                // Evaluate arguments
                let arg_vals: Result<Vec<Value>> = args.iter()
                    .map(|arg| self.evaluate_expression_complete(arg))
                    .collect();
                let arg_vals = arg_vals?;

                // TODO: Handle kwargs

                // Call the function
                match func_val {
                    Value::BuiltinFunction(_, func) => {
                        func(arg_vals)
                    }
                    Value::Closure { name, params, body, captured_scope, .. } => {
                        // Create new scope for function execution
                        let mut func_scope = Scope::new();

                        // Bind parameters
                        for (param, arg) in params.iter().zip(arg_vals.iter()) {
                            func_scope.variables.insert(param.name.clone(), arg.clone());
                        }

                        // Add captured variables
                        for (k, v) in captured_scope {
                            func_scope.variables.insert(k, v);
                        }

                        // Push scope and execute
                        self.push_scope_internal(func_scope);

                        for stmt in &body {
                            self.execute_statement_complete(stmt)?;
                            if self.check_return() {
                                break;
                            }
                        }

                        // Get return value
                        let return_val = self.return_value.take().unwrap_or(Value::None);
                        self.set_return(None);

                        // Pop scope
                        self.pop_scope_internal();

                        Ok(return_val)
                    }
                    _ => Err(anyhow!("'{}' is not callable", func_val.type_name())),
                }
            }
            _ => {
                // Use existing evaluation for other expression types
                self.evaluate_expression(expr)
            }
        }
    }

    fn get_scope_mut(&mut self) -> &mut Scope {
        &mut self.scopes[self.current_scope]
    }

    fn get_scope(&self) -> &Scope {
        &self.scopes[self.current_scope]
    }

    fn push_scope_internal(&mut self, scope: Scope) {
        self.push_scope(scope);
    }

    fn pop_scope_internal(&mut self) -> Scope {
        self.pop_scope()
    }

    fn set_return(&mut self, value: Option<Value>) {
        if value.is_some() {
            self.should_return = true;
            self.return_value = value;
        } else {
            self.should_return = false;
            self.return_value = None;
        }
    }

    fn check_return(&self) -> bool {
        self.should_return
    }

    fn check_break(&self) -> bool {
        self.should_break
    }

    fn check_continue(&self) -> bool {
        self.should_continue
    }

    fn set_break(&mut self, val: bool) {
        self.should_break = val;
    }

    fn set_continue(&mut self, val: bool) {
        self.should_continue = val;
    }
}