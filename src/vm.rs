//! TauraroLang Virtual Machine - Interpreter with integrated memory management
use crate::runtime::{MemoryAPI, ManagedPtr, MemoryMode};
use crate::ast::*;
use anyhow::Result;
use std::collections::HashMap;

/// VM value that can be stored in variables
#[derive(Debug, Clone)]
pub enum Value {
    Int(i64),
    Float(f64),
    Bool(bool),
    String(String),
    List(Vec<Value>),
    Dict(HashMap<String, Value>),
    Object(String, HashMap<String, Value>), // class name, fields
    Function(String, Vec<String>), // name, parameters
    NativeFunction(fn(Vec<Value>) -> Result<Value>),
    None,
    // Managed pointers for manual memory control
    ManagedPtr(ManagedPtr<Value>),
}

/// Variable scope
#[derive(Debug, Clone)]
pub struct Scope {
    pub variables: HashMap<String, Value>,
    pub parent: Option<usize>,
}

/// Virtual Machine state
pub struct VM {
    scopes: Vec<Scope>,
    current_scope: usize,
    memory: MemoryAPI,
    call_stack: Vec<StackFrame>,
}

/// Stack frame for function calls
#[derive(Debug)]
struct StackFrame {
    function_name: String,
    return_address: usize,
    variables: HashMap<String, Value>,
}

impl VM {
    pub fn new() -> Self {
        let global_scope = Scope {
            variables: HashMap::new(),
            parent: None,
        };
        
        Self {
            scopes: vec![global_scope],
            current_scope: 0,
            memory: MemoryAPI::new(),
            call_stack: Vec::new(),
        }
    }
    
    /// Execute TauraroLang source code
    pub fn execute(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        // For now, just parse and execute a simple program
        // In full implementation, this would use the lexer and parser
        
        // Set up command line arguments
        self.set_variable("args", Value::List(args.into_iter().map(Value::String).collect()));
        
        // Execute built-in example for demonstration
        self.execute_builtin_example()
    }
    
    /// Execute a statement
    fn execute_statement(&mut self, stmt: &Stmt) -> Result<Option<Value>> {
        match stmt {
            Stmt::Expression(expr, _) => {
                let value = self.execute_expression(expr)?;
                Ok(Some(value))
            }
            Stmt::Assignment { target, value, .. } => {
                self.execute_assignment(target, value)?;
                Ok(None)
            }
            Stmt::Function { name, parameters, body, .. } => {
                // Store function definition
                let param_names: Vec<String> = parameters.iter().map(|p| p.name.clone()).collect();
                let function_value = Value::Function(name.clone(), param_names);
                self.set_variable(name, function_value);
                Ok(None)
            }
            Stmt::Return { value, .. } => {
                let return_value = if let Some(expr) = value {
                    self.execute_expression(expr)?
                } else {
                    Value::None
                };
                Ok(Some(return_value))
            }
            _ => Ok(None), // TODO: Implement other statement types
        }
    }
    
    /// Execute an expression
    fn execute_expression(&mut self, expr: &Expr) -> Result<Value> {
        match expr {
            Expr::Int(n, _) => Ok(Value::Int(*n)),
            Expr::Float(n, _) => Ok(Value::Float(*n)),
            Expr::String(s, _) => Ok(Value::String(s.clone())),
            Expr::Bool(b, _) => Ok(Value::Bool(*b)),
            Expr::None(_) => Ok(Value::None),
            Expr::Identifier(name, _) => {
                self.get_variable(name).ok_or_else(|| anyhow::anyhow!("Undefined variable: {}", name))
            }
            Expr::Binary { left, op, right, .. } => {
                self.execute_binary_op(left, op, right)
            }
            Expr::Call { callee, arguments, .. } => {
                self.execute_function_call(callee, arguments)
            }
            _ => Ok(Value::None), // TODO: Implement other expression types
        }
    }
    
    /// Execute binary operation
    fn execute_binary_op(&mut self, left: &Expr, op: &BinaryOp, right: &Expr) -> Result<Value> {
        let left_val = self.execute_expression(left)?;
        let right_val = self.execute_expression(right)?;
        
        match op {
            BinaryOp::Add => self.add_values(left_val, right_val),
            BinaryOp::Sub => self.sub_values(left_val, right_val),
            BinaryOp::Mul => self.mul_values(left_val, right_val),
            BinaryOp::Div => self.div_values(left_val, right_val),
            BinaryOp::Eq => Ok(Value::Bool(self.values_equal(left_val, right_val))),
            BinaryOp::Neq => Ok(Value::Bool(!self.values_equal(left_val, right_val))),
            _ => Err(anyhow::anyhow!("Operator not implemented: {:?}", op)),
        }
    }
    
    /// Execute function call
    fn execute_function_call(&mut self, callee: &Expr, arguments: &[Expr]) -> Result<Value> {
        let callee_val = self.execute_expression(callee)?;
        let arg_values: Result<Vec<Value>> = arguments
            .iter()
            .map(|arg| self.execute_expression(arg))
            .collect();
        let arg_values = arg_values?;
        
        match callee_val {
            Value::Function(name, params) => {
                self.call_function(&name, &params, arg_values)
            }
            Value::NativeFunction(func) => {
                func(arg_values)
            }
            _ => Err(anyhow::anyhow!("Not a function: {:?}", callee_val)),
        }
    }
    
    /// Execute assignment
    fn execute_assignment(&mut self, target: &AssignTarget, value: &Expr) -> Result<()> {
        let value = self.execute_expression(value)?;
        
        match target {
            AssignTarget::Identifier(name, _) => {
                self.set_variable(name, value);
                Ok(())
            }
            _ => Err(anyhow::anyhow!("Complex assignment not supported")),
        }
    }
    
    /// Call a function
    fn call_function(&mut self, name: &str, params: &[String], args: Vec<Value>) -> Result<Value> {
        // Handle built-in functions
        match name {
            "print" => {
                for arg in args {
                    print_value(&arg);
                }
                println!();
                Ok(Value::None)
            }
            "malloc" => {
                // Manual memory allocation example
                if let Some(arg) = args.first() {
                    if let Value::Int(size) = arg {
                        // Allocate manual memory
                        let manual_value = Value::Int(0); // Placeholder
                        let managed = self.memory.manual(manual_value);
                        Ok(Value::ManagedPtr(managed))
                    } else {
                        Err(anyhow::anyhow!("malloc requires integer size"))
                    }
                } else {
                    Err(anyhow::anyhow!("malloc requires size argument"))
                }
            }
            "free" => {
                // Manual memory deallocation
                if let Some(Value::ManagedPtr(ptr)) = args.first() {
                    // In real implementation, we'd free the memory
                    Ok(Value::None)
                } else {
                    Err(anyhow::anyhow!("free requires managed pointer"))
                }
            }
            _ => {
                // User-defined function
                self.call_user_function(name, params, args)
            }
        }
    }
    
    /// Call user-defined function (simplified)
    fn call_user_function(&mut self, name: &str, params: &[String], args: Vec<Value>) -> Result<Value> {
        // Enter new scope
        self.enter_scope();
        
        // Set parameters
        for (param, arg) in params.iter().zip(args) {
            self.set_variable(param, arg);
        }
        
        // In real implementation, we'd execute the function body
        let result = Value::None;
        
        // Exit scope
        self.exit_scope();
        
        Ok(result)
    }
    
    // --- Memory Management Integration ---
    
    /// Allocate value with automatic memory management
    pub fn allocate_auto(&self, value: Value) -> Value {
        Value::ManagedPtr(self.memory.auto(value))
    }
    
    /// Allocate value with manual memory management
    pub fn allocate_manual(&self, value: Value) -> Value {
        Value::ManagedPtr(self.memory.manual(value))
    }
    
    /// Force garbage collection
    pub fn collect_garbage(&self) {
        self.memory.collect();
    }
    
    /// Get memory statistics
    pub fn memory_stats(&self) -> String {
        format!("{:?}", self.memory.stats())
    }
    
    // --- Scope Management ---
    
    fn enter_scope(&mut self) {
        let new_scope = Scope {
            variables: HashMap::new(),
            parent: Some(self.current_scope),
        };
        self.scopes.push(new_scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    fn exit_scope(&mut self) {
        if let Some(parent) = self.scopes[self.current_scope].parent {
            self.current_scope = parent;
            self.scopes.pop(); // Remove the current scope
        }
    }
    
    fn set_variable(&mut self, name: &str, value: Value) {
        self.scopes[self.current_scope].variables.insert(name.to_string(), value);
    }
    
    fn get_variable(&self, name: &str) -> Option<Value> {
        let mut scope_index = Some(self.current_scope);
        
        while let Some(idx) = scope_index {
            let scope = &self.scopes[idx];
            if let Some(value) = scope.variables.get(name) {
                return Some(value.clone());
            }
            scope_index = scope.parent;
        }
        
        None
    }
    
    // --- Value Operations ---
    
    fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::String(a), Value::String(b)) => Ok(Value::String(a + &b)),
            (Value::String(a), Value::Int(b)) => Ok(Value::String(a + &b.to_string())),
            _ => Err(anyhow::anyhow!("Invalid types for addition")),
        }
    }
    
    fn sub_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            _ => Err(anyhow::anyhow!("Invalid types for subtraction")),
        }
    }
    
    fn mul_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            _ => Err(anyhow::anyhow!("Invalid types for multiplication")),
        }
    }
    
    fn div_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Int(a / b))
                }
            }
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            }
            _ => Err(anyhow::anyhow!("Invalid types for division")),
        }
    }
    
    fn values_equal(&self, left: Value, right: Value) -> bool {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::String(a), Value::String(b)) => a == b,
            (Value::None, Value::None) => true,
            _ => false,
        }
    }
    
    // --- Built-in Examples ---
    
    fn execute_builtin_example(&mut self) -> Result<Value> {
        // Demonstrate automatic vs manual memory management
        println!("=== TauraroLang Memory Management Demo ===");
        
        // Automatic memory management (default)
        println!("\n1. Automatic Memory Management:");
        let auto_value = self.allocate_auto(Value::String("Hello Auto".to_string()));
        println!("Allocated automatic value: {:?}", auto_value);
        // No need to free - automatically managed
        
        // Manual memory management (explicit control)
        println!("\n2. Manual Memory Management:");
        let manual_value = self.allocate_manual(Value::String("Hello Manual".to_string()));
        println!("Allocated manual value: {:?}", manual_value);
        // Developer must free this explicitly
        
        // Show memory statistics
        println!("\n3. Memory Statistics:");
        println!("{}", self.memory_stats());
        
        Ok(Value::None)
    }
}

/// Print value helper function
fn print_value(value: &Value) {
    match value {
        Value::Int(n) => print!("{}", n),
        Value::Float(n) => print!("{}", n),
        Value::Bool(b) => print!("{}", b),
        Value::String(s) => print!("{}", s),
        Value::None => print!("None"),
        Value::List(items) => {
            print!("[");
            for (i, item) in items.iter().enumerate() {
                if i > 0 { print!(", "); }
                print_value(item);
            }
            print!("]");
        }
        Value::ManagedPtr(_) => print!("<managed pointer>"),
        _ => print!("<complex value>"),
    }
}