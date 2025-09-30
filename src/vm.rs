//! COMPLETE TauraroLang Virtual Machine with dynamic typing and REPL support
use crate::runtime::{MemoryAPI, ManagedPtr, MemoryMode};
use crate::ast::*;
use crate::lexer::Lexer;
use crate::parser::Parser;
use crate::ast::*;
use crate::value::Value;
use crate::object_system::{call_dunder_method_with_vm, call_dunder_method, TypeRegistry};
use crate::semantic;
use crate::modules;
use crate::module_system::{ModuleSystem, ImportSpec};
use crate::package_manager::{PackageManager, PackageType};
use crate::builtins_super::builtin_super;
use crate::base_object::MRO;
use crate::object_system::{MROComputer, TypeCreator, MetaClass};
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use std::fmt;

/// Frame object inspired by CPython's PyFrameObject
/// Properly manages local variables and execution context
#[derive(Debug, Clone)]
pub struct ExecutionFrame {
    /// Local variables (like CPython's f_locals)
    pub locals: HashMap<String, Value>,
    /// Global variables reference
    pub globals: HashMap<String, Value>,
    /// Built-in variables
    pub builtins: HashMap<String, Value>,
    /// Code being executed
    pub code_name: String,
    /// Parent frame for nested calls
    pub parent: Option<Box<ExecutionFrame>>,
}

impl ExecutionFrame {
    pub fn new(code_name: String) -> Self {
        Self {
            locals: HashMap::new(),
            globals: HashMap::new(),
            builtins: HashMap::new(),
            code_name,
            parent: None,
        }
    }
    
    /// Set variable with proper scoping (like CPython's STORE_FAST/STORE_NAME)
    pub fn set_local(&mut self, name: &str, value: Value) {
        self.locals.insert(name.to_string(), value);
    }
    
    /// Get variable with proper lookup order (like CPython's LOAD_FAST/LOAD_NAME)
    pub fn get_variable(&self, name: &str) -> Option<Value> {
        // LEGB rule: Local, Enclosing, Global, Built-in
        if let Some(value) = self.locals.get(name) {
            return Some(value.clone());
        }
        if let Some(value) = self.globals.get(name) {
            return Some(value.clone());
        }
        if let Some(value) = self.builtins.get(name) {
            return Some(value.clone());
        }
        None
    }
}

/// Variable scope
#[derive(Debug, Clone)]
pub struct Scope {
    pub variables: HashMap<String, Value>,
    pub variable_types: HashMap<String, Type>, // Track declared types for strict typing
    pub parent: Option<usize>,
    pub scope_type: String, // "global", "function", "class", "block"
}

impl Scope {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
            variable_types: HashMap::new(),
            parent: None,
            scope_type: "module".to_string(),
        }
    }
}

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
}

impl fmt::Debug for VM {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("VM")
            .field("current_scope", &self.current_scope)
            .field("strict_types", &self.strict_types)
            .field("should_return", &self.should_return)
            .field("current_executing_class", &self.current_executing_class)
            .finish()
    }
}

/// Stack frame for function calls
#[derive(Debug, Clone)]
struct StackFrame {
    function_name: String,
    return_address: usize,
    scope_index: usize,
}

impl VM {
    pub fn new() -> Self {
        let global_scope = Scope {
            variables: HashMap::new(),
            variable_types: HashMap::new(),
            parent: None,
            scope_type: "global".to_string(),
        };
        
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
        };
        
        // Initialize built-in modules with memory management capabilities
        vm.init_builtins();
        
        // Initialize package manager and integrate with module system
        vm.init_package_system();
        
        vm
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
        self.set_variable("args", Value::List(args.into_iter().map(Value::Str).collect()))?;
        
        // Execute the program
        self.execute_program(program, is_repl)
    }
    
    /// Execute a complete program
    pub fn execute_program(&mut self, program: Program, is_repl: bool) -> Result<Value> {
        // Enter program scope
        self.enter_scope("program");
        
        // First pass: register all functions and classes
        for stmt in &program.statements {
            if let Statement::FunctionDef { name, params, body, is_async: _, decorators, docstring, return_type: _ } = stmt {
                let captured_scope = self.scopes[self.current_scope].variables.clone();
                let mut func_value = Value::Closure {
                    name: name.clone(),
                    params: params.clone(),
                    body: body.clone(),
                    captured_scope,
                    docstring: docstring.clone(),
                };
                
                // Apply decorators in reverse order (bottom to top)
                for decorator in decorators.iter().rev() {
                    func_value = self.apply_decorator(decorator, func_value)?;
                }
                
                self.set_variable(name, func_value)?;
            }
            // Skip class definitions in first pass - they will be handled in second pass
        }
        
        // Second pass: execute statements
        let mut last_expression_value = Value::None;
        for stmt in program.statements {
            if !matches!(stmt, Statement::FunctionDef { .. }) {
                match self.execute_statement(&stmt) {
                    Ok(Some(value)) => {
                        // In REPL mode, capture expression values for potential return
                        // In script mode, ignore return values to match Python behavior
                        if is_repl {
                            last_expression_value = value;
                        }
                    }
                    Ok(None) => {
                        // Statement executed successfully but returned no value
                    }
                    Err(err) => {
                        // Statement execution failed
                        self.exit_scope();
                        return Err(err);
                    }
                }
                
                if self.should_return {
                    let result = self.return_value.take().unwrap_or(Value::None);
                    self.exit_scope();
                    return Ok(result);
                }
            }
        }
        
        // Check if there's a main function and call it BEFORE exiting program scope
        if let Some(main_func) = self.get_variable("main") {
            match main_func {
                Value::Closure { name, params, body, captured_scope, .. } => {
                    if params.is_empty() {
                        // Call main function with no arguments
                        let result = self.call_user_function(&name, &params, body, vec![], HashMap::new(), Some(captured_scope))?;
                        self.exit_scope();
                        return Ok(result);
                    }
                }
                _ => {}
            }
        }
        
        // Exit program scope
        self.exit_scope();
        
        // In script mode, always return None (like Python)
        // In REPL mode, return the last expression value
        if is_repl {
            Ok(last_expression_value)
        } else {
            Ok(Value::None)
        }
    }
    
    /// Apply a decorator to a function or class
    fn apply_decorator(&mut self, decorator: &Expr, target: Value) -> Result<Value> {
        match decorator {
            // Simple decorator (just a name)
            Expr::Identifier(decorator_name) => {
                if let Some(decorator_func) = self.get_variable(decorator_name) {
                    match decorator_func {
                        Value::Closure { name, params, body, captured_scope, .. } => {
                            // Call the user-defined decorator function with the target as argument
                            let args = vec![target];
                            self.call_user_function(&name, &params, body, args, HashMap::new(), Some(captured_scope))
                        }
                        Value::BuiltinFunction(name, func) => {
                            // Call the builtin decorator function
                            let result = func(vec![target.clone()])?;
                            // For decorators, if the builtin function returns None, return the original target
                            if matches!(result, Value::None) {
                                Ok(target)
                            } else {
                                Ok(result)
                            }
                        }
                        Value::NativeFunction(func) => {
                            // Call the native decorator function
                            func(vec![target])
                        }
                        _ => {
                            // If it's not a function, just return the target unchanged
                            Ok(target)
                        }
                    }
                } else {
                    // Decorator not found, return target unchanged
                    Ok(target)
                }
            }
            // Decorator with arguments (function call)
            Expr::Call { func, args, .. } => {
                // First evaluate the decorator function with its arguments
                let decorator_result = self.execute_function_call(func, args, &[])?;;
                
                // Then apply the result to the target
                match decorator_result {
                    Value::Closure { name, params, body, captured_scope, .. } => {
                        // The decorator returned a function, call it with the target
                        let args = vec![target];
                        self.call_user_function(&name, &params, body, args, HashMap::new(), Some(captured_scope))
                    }
                    Value::BuiltinFunction(_, func) => {
                        func(vec![target])
                    }
                    Value::NativeFunction(func) => {
                        func(vec![target])
                    }
                    _ => {
                        // The decorator didn't return a function, return target unchanged
                        Ok(target)
                    }
                }
            }
            // Other decorator expressions (attributes, etc.)
            _ => {
                let decorator_value = self.execute_expression(decorator)?;
                match decorator_value {
                    Value::Closure { name, params, body, captured_scope, .. } => {
                        let args = vec![target];
                        self.call_user_function(&name, &params, body, args, HashMap::new(), Some(captured_scope))
                    }
                    Value::BuiltinFunction(_, func) => {
                        func(vec![target])
                    }
                    Value::NativeFunction(func) => {
                        func(vec![target])
                    }
                    _ => Ok(target)
                }
            }
        }
    }
    
    /// Execute try statement
    fn execute_try_statement(
        &mut self,
        body: &[Statement],
        except_handlers: &[ExceptHandler],
        else_branch: &Option<Vec<Statement>>,
        finally: &Option<Vec<Statement>>,
    ) -> Result<Option<Value>> {
        let mut exception_occurred = false;
        let mut exception_value = None;
        let mut result = None;
        
        // Execute try block
        for stmt in body {
            match self.execute_statement(stmt) {
                Ok(value) => result = value,
                Err(error) => {
                    exception_occurred = true;
                    exception_value = Some(error);
                    break;
                }
            }
            if self.should_return {
                break;
            }
        }
        
        // Handle exceptions
        if exception_occurred {
            let error = exception_value.unwrap();
            let mut handled = false;
            
            for handler in except_handlers {
                // Check if this handler matches the exception type
                let matches = if let Some(exception_type_expr) = &handler.exception_type {
                    // For now, we'll do simple string matching
                    // A proper implementation would check exception hierarchy
                    let error_str = error.to_string();
                    if let Ok(type_value) = self.execute_expression(exception_type_expr) {
                        match type_value {
                            Value::Str(type_name) => error_str.contains(&type_name),
                            _ => false,
                        }
                    } else {
                        false
                    }
                } else {
                    // Bare except clause catches all exceptions
                    true
                };
                
                if matches {
                    handled = true;
                    
                    // Set exception variable if specified
                    if let Some(name) = &handler.name {
                        let exception_obj = self.create_exception_object(&error);
                        self.set_variable(name, exception_obj)?;
                    }
                    
                    // Execute handler body
                    for stmt in &handler.body {
                        if let Ok(value) = self.execute_statement(stmt) {
                            result = value;
                        }
                        if self.should_return {
                            break;
                        }
                    }
                    break;
                }
            }
            
            // If no handler matched, re-raise the exception
            if !handled {
                // Execute finally block before re-raising
                if let Some(finally_body) = finally {
                    for stmt in finally_body {
                        let _ = self.execute_statement(stmt);
                    }
                }
                return Err(error);
            }
        } else {
            // No exception occurred, execute else block if present
            if let Some(else_body) = else_branch {
                for stmt in else_body {
                    if let Ok(value) = self.execute_statement(stmt) {
                        result = value;
                    }
                    if self.should_return {
                        break;
                    }
                }
            }
        }
        
        // Always execute finally block
        if let Some(finally_body) = finally {
            for stmt in finally_body {
                let _ = self.execute_statement(stmt);
            }
        }
        
        Ok(result)
    }
    
    /// Execute raise statement
    fn execute_raise_statement(&mut self, expr: &Option<Expr>) -> Result<Option<Value>> {
        if let Some(exception_expr) = expr {
            let exception_value = self.execute_expression(exception_expr)?;
            
            let error_message = match exception_value {
                Value::Str(msg) => msg,
                Value::Object { class_name, fields, .. } => {
                    // Extract message from exception object
                    if let Some(Value::Str(msg)) = fields.get("message") {
                        format!("{}: {}", class_name, msg)
                    } else {
                        class_name
                    }
                }
                _ => exception_value.to_string(),
            };
            
            Err(anyhow::anyhow!(error_message))
        } else {
            // Re-raise current exception (would need exception context)
            Err(anyhow::anyhow!("No active exception to re-raise"))
        }
    }
    
    /// Create an exception object from an error
    fn create_exception_object(&self, error: &anyhow::Error) -> Value {
        let mut fields = HashMap::new();
        fields.insert("message".to_string(), Value::Str(error.to_string()));
        fields.insert("args".to_string(), Value::Tuple(vec![Value::Str(error.to_string())]));
        
        Value::Object {
            class_name: "Exception".to_string(),
            fields,
            base_object: crate::base_object::BaseObject::new("Exception".to_string(), vec!["object".to_string()]),
            mro: crate::base_object::MRO::from_linearization(vec!["Exception".to_string(), "object".to_string()]),
        }
    }
    
    pub fn execute_statement(&mut self, stmt: &Statement) -> Result<Option<Value>> {
        match stmt {
            Statement::Expression(expr) => {
                let value = self.execute_expression(expr)?;
                Ok(Some(value))
            }
            Statement::VariableDef { name, value, type_annotation } => {
                // Handle type annotation for strict typing
                if let Some(type_info) = type_annotation {
                    self.declare_typed_variable(name, type_info.clone())?;
                }
                
                if let Some(expr) = value {
                    let val = self.execute_expression(expr)?;
                    
                    // Check type compatibility if type annotation exists
                    if let Some(type_info) = type_annotation {
                        if self.strict_types && !val.check_type(type_info) {
                            return Err(anyhow::anyhow!(
                                "Type mismatch: cannot assign {} to variable '{}' of type {}",
                                val.type_name(),
                                name,
                                type_info
                            ));
                        }
                    }
                    
                    self.set_variable(name, val)?;
                } else {
                    self.set_variable(name, Value::None)?;
                }
                Ok(None)
            }
            Statement::Return(value) => {
                let return_value = if let Some(expr) = value {
                    self.execute_expression(expr)?
                } else {
                    Value::None
                };
                self.should_return = true;
                self.return_value = Some(return_value);
                Ok(None)
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                self.execute_if_statement(condition, then_branch, elif_branches, else_branch)
            }
            Statement::While { condition, body, .. } => {
                self.execute_while_statement(condition, body)
            }
            Statement::For { variable, iterable, body, .. } => {
                self.execute_for_statement(variable, iterable, body)
            }
            Statement::Import { module, alias } => {
                self.execute_import_statement(module, alias.as_deref())
            }
            Statement::ClassDef { name, bases, body, decorators, metaclass, docstring: _ } => {
                // Use the new optimized metaclass system for class creation
                
                // Extract base class names
                let mut base_names = Vec::new();
                for base_expr in bases {
                    if let Expr::Identifier(base_name) = base_expr {
                        base_names.push(base_name.clone());
                        
                        // Ensure base class exists
                        if self.get_variable(base_name).is_none() {
                            return Err(anyhow::anyhow!("Base class '{}' not found", base_name));
                        }
                    } else {
                        return Err(anyhow::anyhow!("Complex base class expressions not yet supported"));
                    }
                }
                
                // If no explicit bases, inherit from object (Python default behavior)
                if base_names.is_empty() {
                    base_names.push("object".to_string());
                }
                
                // Process class body to extract methods and attributes
                let mut class_namespace = HashMap::new();
                let mut defined_methods = std::collections::HashSet::new();
                
                for stmt in body.iter() {
                    match stmt {
                        Statement::FunctionDef { name: method_name, params, body: method_body, decorators: _, is_async: _, docstring, return_type: _ } => {
                            let captured_scope = self.scopes[self.current_scope].variables.clone();
                            let method_value = Value::Closure {
                                name: method_name.clone(),
                                params: params.clone(),
                                body: method_body.clone(),
                                captured_scope,
                                docstring: docstring.clone(),
                            };
                            class_namespace.insert(method_name.clone(), method_value);
                            defined_methods.insert(method_name.clone());
                        }
                        Statement::VariableDef { name: attr_name, value, .. } => {
                            if let Some(expr) = value {
                                let attr_value = self.execute_expression(expr)?;
                                class_namespace.insert(attr_name.clone(), attr_value);
                            }
                        }
                        _ => {
                            // Execute other statements in class context
                            self.execute_statement(stmt)?;
                        }
                    }
                }
                
                // Handle metaclass if specified
                let metaclass_obj = if let Some(metaclass_expr) = metaclass {
                    match metaclass_expr {
                        Expr::Identifier(metaclass_name) => {
                            // Look up the metaclass
                            if let Some(metaclass_value) = self.get_variable(metaclass_name) {
                                Some(MetaClass::new(
                                    metaclass_name.clone(),
                                    vec!["type".to_string()],
                                    HashMap::new()
                                ))
                            } else {
                                return Err(anyhow::anyhow!("Metaclass '{}' not found", metaclass_name));
                            }
                        }
                        _ => {
                            return Err(anyhow::anyhow!("Complex metaclass expressions not yet supported"));
                        }
                    }
                } else {
                    None
                };
                
                // Create the class using the optimized type creator
                let mut class_value = self.type_creator.create_type(
                    name.clone(),
                    base_names.clone(),
                    class_namespace,
                    &self.class_registry,
                ).map_err(|e| anyhow::anyhow!("Class creation error: {}", e))?;
                
                // Apply decorators in reverse order (bottom to top)
                for decorator in decorators.iter().rev() {
                    class_value = self.apply_decorator(decorator, class_value)?;
                }
                
                // Store tracking information for compatibility with existing code
                self.class_defined_methods.insert(name.clone(), defined_methods);
                self.class_base_registry.insert(name.clone(), base_names.clone());
                
                // Register the class in the registry with its base classes (not MRO)
                // Store the actual base classes that were used for MRO computation
                self.class_registry.insert(name.clone(), base_names.clone());
                
                // Set the class variable
                self.set_variable(&name, class_value);
                
                Ok(Some(Value::None))
            }
            Statement::AttributeAssignment { object, name, value } => {
                // Handle attribute assignment like self.x = value
                let val = self.execute_expression(value)?;
                
                // Get the object identifier to modify it in place
                if let Expr::Identifier(obj_name) = object {
                    
                    // Find the scope containing the variable and modify it directly
                    let mut scope_index = Some(self.current_scope);
                    let mut found = false;
                    
                    while let Some(idx) = scope_index {
                        if self.scopes[idx].variables.contains_key(obj_name) {
                            // Found the variable, modify it directly
                            if let Some(obj_value) = self.scopes[idx].variables.get_mut(obj_name) {
                                match obj_value {
                                    Value::Object { ref mut fields, .. } => {
                                        // Store the attribute in the object's fields
                                        fields.insert(name.clone(), val);
                                        found = true;
                                        break;
                                    }
                                    _ => {
                                        return Err(anyhow::anyhow!("Cannot assign attribute '{}' to non-object", name));
                                    }
                                }
                            }
                        }
                        scope_index = self.scopes[idx].parent;
                    }
                    
                    if !found {
                        return Err(anyhow::anyhow!("Undefined variable: {}", obj_name));
                    }
                    
                    Ok(None)
                } else {
                    // For complex expressions, we can't modify in place yet
                    Err(anyhow::anyhow!("Complex attribute assignment not yet supported"))
                }
            }
            Statement::Try { body, except_handlers, else_branch, finally } => {
                self.execute_try_statement(body, except_handlers, else_branch, finally)
            },
            Statement::Raise(expr) => {
                self.execute_raise_statement(expr)
            },
            Statement::Match { value, cases } => {
                self.execute_match_statement(value, cases)
            },
            Statement::With { context, alias, body } => {
                self.execute_with_statement(context, alias.as_deref(), body)
            },
            _ => {
                // TODO: Implement other statement types
                Ok(None)
            }
        }
    }
    
    /// Execute an expression
    pub fn execute_expression(&mut self, expr: &Expr) -> Result<Value> {
        match expr {
            Expr::Literal(Literal::Int(n)) => Ok(Value::Int(*n)),
            Expr::Literal(Literal::Float(n)) => Ok(Value::Float(*n)),
            Expr::Literal(Literal::String(s)) => Ok(Value::Str(s.clone())),
            Expr::Literal(Literal::Bool(b)) => Ok(Value::Bool(*b)),
            Expr::Literal(Literal::None) => Ok(Value::None),
            Expr::Literal(Literal::Bytes(bytes)) => Ok(Value::Bytes(bytes.clone())),
            Expr::Literal(Literal::Complex { real, imag }) => {
                Ok(Value::Complex { real: *real, imag: *imag })
            },
            Expr::Literal(Literal::Ellipsis) => {
                Ok(Value::Ellipsis)
            },
            Expr::DocString(s) => Ok(Value::Str(s.clone())),
            Expr::Identifier(name) => {
                self.get_variable(name).ok_or_else(|| anyhow::anyhow!("Undefined variable: {}", name))
            }
            Expr::BinaryOp { left, op, right } => {
                self.execute_binary_op(left, op, right)
            }
            Expr::UnaryOp { op, operand } => {
                self.execute_unary_op(op, operand)
            }
            Expr::Call { func, args, kwargs } => {
                self.execute_function_call(func, args, kwargs)
            }
            Expr::MethodCall { object, method, args, kwargs } => {
                self.execute_method_call(object.as_ref(), method, args, kwargs)
            }
            Expr::List(elements) => {
                let values: Result<Vec<Value>> = elements.iter()
                    .map(|e| self.execute_expression(e))
                    .collect();
                Ok(Value::List(values?))
            }
            Expr::Tuple(elements) => {
                let values: Result<Vec<Value>> = elements.iter()
                    .map(|e| self.execute_expression(e))
                    .collect();
                Ok(Value::Tuple(values?))
            }
            Expr::Dict(pairs) => {
                let mut dict = HashMap::new();
                for (key_expr, value_expr) in pairs {
                    let key = self.execute_expression(key_expr)?;
                    let value = self.execute_expression(value_expr)?;
                    let key_string = match &key {
                        Value::Str(s) => s.clone(),
                        Value::Int(n) => n.to_string(),
                        Value::Float(n) => format!("{:.6}", n),
                        Value::Bool(b) => b.to_string(),
                        Value::None => "None".to_string(),
                        _ => format!("{}", key), // Use Display trait directly
                    };
                    dict.insert(key_string, value);
                }
                Ok(Value::Dict(dict))
            }
            Expr::Set(elements) => {
                let mut unique_values = Vec::new();
                for element in elements {
                    let value = self.execute_expression(element)?;
                    if !unique_values.iter().any(|existing: &Value| self.values_equal(existing.clone(), value.clone())) {
                        unique_values.push(value);
                    }
                }
                Ok(Value::Set(unique_values))
            }
            Expr::Attribute { object, name } => {
                let obj_value = self.execute_expression(object)?;
                
                match &obj_value {
                    Value::Object { fields, .. } => {
                        if let Some(value) = fields.get(name) {
                            Ok(value.clone())
                        } else {
                            Err(anyhow::anyhow!("'{}' object has no attribute '{}'", "object", name))
                        }
                    }
                    Value::Super(current_class, parent_class, self_obj) => {
                        // Look up the method in the parent class
                        if let Some(Value::Object { fields: parent_methods, .. }) = self.get_variable(parent_class) {
                            if let Some(method) = parent_methods.get(name) {
                                Ok(method.clone())
                            } else {
                                Err(anyhow::anyhow!("'super' object has no attribute '{}'", name))
                            }
                        } else {
                            Err(anyhow::anyhow!("Parent class '{}' not found", parent_class))
                        }
                    }
                    Value::Module(_module_name, namespace) => {
                        if let Some(value) = namespace.get(name) {
                            Ok(value.clone())
                        } else {
                            Err(anyhow::anyhow!("'module' object has no attribute '{}'", name))
                        }
                    }
                    _ => Err(anyhow::anyhow!("'{}' object has no attribute '{}'", obj_value.type_name(), name)),
                }
            }
            Expr::Subscript { object, index } => {
                let obj_value = self.execute_expression(object)?;
                let index_value = self.execute_expression(index)?;
                
                match (&obj_value, &index_value) {
                    (Value::Dict(dict), index_val) => {
                        let key_string = match index_val {
                            Value::Str(s) => s.clone(),
                            Value::Int(n) => n.to_string(),
                            Value::Float(n) => format!("{:.6}", n),
                            Value::Bool(b) => b.to_string(),
                            Value::None => "None".to_string(),
                            _ => format!("{}", index_val),
                        };
                        dict.get(&key_string)
                            .cloned()
                            .ok_or_else(|| anyhow::anyhow!("Key '{}' not found in dictionary", key_string))
                    }
                    (Value::List(list), Value::Int(index)) => {
                        let idx = if *index < 0 {
                            (list.len() as i64 + index) as usize
                        } else {
                            *index as usize
                        };
                        
                        if idx < list.len() {
                            Ok(list[idx].clone())
                        } else {
                            Err(anyhow::anyhow!("List index {} out of range", index))
                        }
                    }
                    (Value::Str(s), Value::Int(index)) => {
                        let chars: Vec<char> = s.chars().collect();
                        let idx = if *index < 0 {
                            (chars.len() as i64 + index) as usize
                        } else {
                            *index as usize
                        };
                        
                        if idx < chars.len() {
                            Ok(Value::Str(chars[idx].to_string()))
                        } else {
                            Err(anyhow::anyhow!("String index {} out of range", index))
                        }
                    }
                    _ => Err(anyhow::anyhow!("Invalid subscript operation: {:?}[{:?}]", obj_value, index_value)),
                }
            }
            Expr::ListComp { element, generators } => {
                self.execute_list_comprehension(element, generators)
            }
            Expr::DictComp { key, value, generators } => {
                self.execute_dict_comprehension(key, value, generators)
            }
            Expr::SetComp { element, generators } => {
                self.execute_set_comprehension(element, generators)
            }
            Expr::GeneratorExp { element, generators } => {
                // For now, we'll convert generator expressions to lists
                // In a full implementation, this would return a generator object
                self.execute_list_comprehension(element, generators)
            }
            Expr::Lambda { params, body } => {
                let captured_scope = self.scopes[self.current_scope].variables.clone();
                Ok(Value::Closure {
                    name: "lambda".to_string(),
                    params: params.clone(),
                    body: vec![Statement::Return(Some((**body).clone()))],
                    captured_scope,
                    docstring: None,
                })
            }
            Expr::IfExp { condition, then_expr, else_expr } => {
                let cond_value = self.execute_expression(condition)?;
                if cond_value.is_truthy() {
                    self.execute_expression(then_expr)
                } else {
                    self.execute_expression(else_expr)
                }
            }
            Expr::Yield(value) => {
                // For now, yield expressions will return the yielded value
                // In a full implementation, this would suspend execution and return control to the caller
                match value {
                    Some(expr) => {
                        let yielded_value = self.execute_expression(expr)?;
                        // TODO: Implement proper generator/coroutine suspension
                        Ok(yielded_value)
                    }
                    None => Ok(Value::None),
                }
            }
            Expr::YieldFrom(expr) => {
                // For now, yield from will just evaluate the expression
                // In a full implementation, this would delegate to another generator
                let value = self.execute_expression(expr)?;
                // TODO: Implement proper yield from delegation
                Ok(value)
            }
            Expr::Await(expr) => {
                // For now, await expressions will just evaluate the expression
                // In a full implementation, this would suspend execution until the awaitable completes
                let awaitable_value = self.execute_expression(expr)?;
                // TODO: Implement proper async/await suspension and resumption
                Ok(awaitable_value)
            }
            Expr::NamedExpr { target, value } => {
                // Walrus operator (:=) - evaluate the value and assign it to the target
                let evaluated_value = self.execute_expression(value)?;
                
                // Extract the variable name from the target expression
                match target.as_ref() {
                    Expr::Identifier(name) => {
                        self.set_variable(name, evaluated_value.clone())?;
                        Ok(evaluated_value)
                    }
                    _ => Err(anyhow::anyhow!("Invalid target for walrus operator: {:?}", target)),
                }
            }
            Expr::FormatString { parts } => {
                let mut result = String::new();
                for part in parts {
                    match part {
                        FormatPart::String(s) => {
                            result.push_str(s);
                        }
                        FormatPart::Expression { expr, format_spec, conversion } => {
                            let value = self.execute_expression(expr)?;
                            let formatted = if let Some(spec) = format_spec {
                                // Basic format specifier support
                                self.format_value_with_spec(&value, spec)?
                            } else {
                                self.format_value(&value)
                            };
                            result.push_str(&formatted);
                        }
                    }
                }
                Ok(Value::Str(result))
            }
            Expr::Compare { left, ops, comparators } => {
                self.execute_compare(left.as_ref(), ops, comparators)
            }
            Expr::Slice { object, start, stop, step } => {
                self.execute_slice(object.as_ref(), start.as_deref(), stop.as_deref(), step.as_deref())
            }
            Expr::Starred(expr) => {
                // Starred expressions are used for unpacking in function calls, assignments, etc.
                // For now, we'll evaluate the inner expression and return it as-is
                // In a full implementation, this would need context-aware handling
                let value = self.execute_expression(expr)?;
                // TODO: Implement proper starred expression unpacking based on context
                Ok(value)
            }
            // Typed expressions are not part of the current AST definition
            // This case has been removed as Expr::Typed doesn't exist
            _ => Err(anyhow::anyhow!("Expression not implemented: {:?}", expr)),
        }
    }
    
    /// Execute binary operation
    fn execute_binary_op(&mut self, left: &Expr, op: &BinaryOp, right: &Expr) -> Result<Value> {
        let left_val = self.execute_expression(left)?;
        let right_val = self.execute_expression(right)?;
        
        // Type checking in strict mode
        if self.strict_types {
            self.check_binary_op_types(&left_val, op, &right_val)?;
        }
        
        match op {
            BinaryOp::Add => self.add_values(left_val, right_val),
            BinaryOp::Sub => self.sub_values(left_val, right_val),
            BinaryOp::Mul => self.mul_values(left_val, right_val),
            BinaryOp::Div => self.div_values(left_val, right_val),
            BinaryOp::FloorDiv => self.floordiv_values(left_val, right_val),
            BinaryOp::Mod => self.mod_values(left_val, right_val),
            BinaryOp::Pow => self.pow_values(left_val, right_val),
            BinaryOp::MatMul => self.matmul_values(left_val, right_val),
            BinaryOp::BitAnd => self.bitand_values(left_val, right_val),
            BinaryOp::BitOr => self.bitor_values(left_val, right_val),
            BinaryOp::BitXor => self.bitxor_values(left_val, right_val),
            BinaryOp::LShift => self.lshift_values(left_val, right_val),
            BinaryOp::RShift => self.rshift_values(left_val, right_val),
            BinaryOp::Eq => Ok(Value::Bool(self.values_equal(left_val, right_val))),
            BinaryOp::Ne | BinaryOp::Neq => Ok(Value::Bool(!self.values_equal(left_val, right_val))),
            BinaryOp::Gt => self.gt_values(left_val, right_val),
            BinaryOp::Lt => self.lt_values(left_val, right_val),
            BinaryOp::Ge | BinaryOp::Gte => self.gte_values(left_val, right_val),
            BinaryOp::Le | BinaryOp::Lte => self.lte_values(left_val, right_val),
            BinaryOp::And => Ok(Value::Bool(left_val.is_truthy() && right_val.is_truthy())),
            BinaryOp::Or => Ok(Value::Bool(left_val.is_truthy() || right_val.is_truthy())),
            BinaryOp::Is => Ok(Value::Bool(self.is_same_object(&left_val, &right_val))),
            BinaryOp::IsNot => Ok(Value::Bool(!self.is_same_object(&left_val, &right_val))),
            BinaryOp::In => self.in_values(left_val, right_val),
            BinaryOp::NotIn => {
                let result = self.in_values(left_val, right_val)?;
                Ok(Value::Bool(!result.is_truthy()))
            }
        }
    }
    
    /// Execute unary operation
    fn execute_unary_op(&mut self, op: &UnaryOp, expr: &Expr) -> Result<Value> {
        let value = self.execute_expression(expr)?;
        
        match op {
            UnaryOp::UAdd => self.plus_value(value),
            UnaryOp::Minus | UnaryOp::USub => self.minus_value(value),
            UnaryOp::Not => Ok(Value::Bool(!value.is_truthy())),
            UnaryOp::BitNot | UnaryOp::Invert => self.bitnot_value(value),
        }
    }
    
    /// Execute function call
    fn execute_function_call(&mut self, callee: &Expr, arguments: &[Expr], kwargs: &[(String, Expr)]) -> Result<Value> {
        let callee_val = self.execute_expression(callee)?;
        let arg_values: Result<Vec<Value>> = arguments
            .iter()
            .map(|arg| self.execute_expression(arg))
            .collect();
        let arg_values = arg_values?;

        let kwarg_values: Result<HashMap<String, Value>> = kwargs
            .iter()
            .map(|(name, expr)| {
                let value = self.execute_expression(expr)?;
                Ok((name.clone(), value))
            })
            .collect();
        let kwarg_values = kwarg_values?;

        match callee_val {
            Value::Closure { name, params, body, captured_scope, .. } => {
                self.call_user_function(&name, &params, body, arg_values, kwarg_values, Some(captured_scope))
            }
            Value::BuiltinFunction(name, func) => {
                // Special handling for functions that need VM context
                match name.as_str() {
                    "str" => self.builtin_str_with_vm(arg_values),
                    "repr" => self.builtin_repr_with_vm(arg_values),
                    "len" => self.builtin_len_with_vm(arg_values),
                    "super" => builtin_super(arg_values, Some(self)),
                    "eval" => crate::builtins::builtin_eval_with_vm(self, arg_values),
                    "exec" => crate::builtins::builtin_exec_with_vm(self, arg_values),
                    _ => func(arg_values),
                }
            }
            Value::NativeFunction(func) => {
                func(arg_values)
            }
            Value::Object { class_name, fields: class_methods, .. } => {
                // Class instantiation - create new instance
                self.instantiate_class(&class_name, class_methods, arg_values)
            }
            _ => Err(anyhow::anyhow!("Not a function: {}", callee_val.type_name())),
        }
    }
    
    /// Execute comparison chain
    fn execute_compare(&mut self, left: &Expr, ops: &[CompareOp], comparators: &[Expr]) -> Result<Value> {
        if ops.len() != comparators.len() {
            return Err(anyhow::anyhow!("Mismatched comparison operators and comparators"));
        }
        
        let mut current_value = self.execute_expression(left)?;
        
        for (op, comparator) in ops.iter().zip(comparators.iter()) {
            let next_value = self.execute_expression(comparator)?;
            
            let comparison_result = match op {
                CompareOp::Eq => self.values_equal(current_value.clone(), next_value.clone()),
                CompareOp::NotEq => !self.values_equal(current_value.clone(), next_value.clone()),
                CompareOp::Lt => {
                    let result = self.lt_values(current_value.clone(), next_value.clone())?;
                    result.is_truthy()
                },
                CompareOp::LtE => {
                    let result = self.lte_values(current_value.clone(), next_value.clone())?;
                    result.is_truthy()
                },
                CompareOp::Gt => {
                    let result = self.gt_values(current_value.clone(), next_value.clone())?;
                    result.is_truthy()
                },
                CompareOp::GtE => {
                    let result = self.gte_values(current_value.clone(), next_value.clone())?;
                    result.is_truthy()
                },
                CompareOp::Is => self.is_same_object(&current_value, &next_value),
                CompareOp::IsNot => !self.is_same_object(&current_value, &next_value),
                CompareOp::In => self.value_in_container(&current_value, &next_value)?,
                CompareOp::NotIn => !self.value_in_container(&current_value, &next_value)?,
            };
            
            if !comparison_result {
                return Ok(Value::Bool(false));
            }
            
            current_value = next_value;
        }
        
        Ok(Value::Bool(true))
    }
    
    /// Check if two values are the same object (identity comparison)
    fn is_same_object(&self, left: &Value, right: &Value) -> bool {
        match (left, right) {
            (Value::None, Value::None) => true,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Str(a), Value::Str(b)) => a == b,
            _ => false, // For complex objects, we'd need reference comparison
        }
    }
    
    /// Check if a value is contained in a container
    fn value_in_container(&self, value: &Value, container: &Value) -> Result<bool> {
        match container {
            Value::List(list) => {
                Ok(list.iter().any(|item| self.values_equal(value.clone(), item.clone())))
            }
            Value::Tuple(tuple) => {
                Ok(tuple.iter().any(|item| self.values_equal(value.clone(), item.clone())))
            }
            Value::Set(set) => {
                Ok(set.iter().any(|item| self.values_equal(value.clone(), item.clone())))
            }
            Value::Dict(dict) => {
                let key_string = match value {
                    Value::Str(s) => s.clone(),
                    Value::Int(n) => n.to_string(),
                    Value::Float(n) => format!("{:.6}", n),
                    Value::Bool(b) => b.to_string(),
                    Value::None => "None".to_string(),
                    _ => format!("{}", value),
                };
                Ok(dict.contains_key(&key_string))
            }
            Value::Str(s) => {
                match value {
                    Value::Str(substr) => Ok(s.contains(substr)),
                    _ => Err(anyhow::anyhow!("'in' requires string on both sides for string containment")),
                }
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", container.type_name())),
        }
    }
    
    /// Execute method call
    fn execute_method_call(&mut self, object: &Expr, method: &str, args: &[Expr], kwargs: &[(String, Expr)]) -> Result<Value> {
        let obj_value = self.execute_expression(object)?;
        let arg_values: Result<Vec<Value>> = args
            .iter()
            .map(|arg| self.execute_expression(arg))
            .collect();
        let arg_values = arg_values?;

        let kwarg_values: Result<HashMap<String, Value>> = kwargs
            .iter()
            .map(|(name, expr)| {
                let value = self.execute_expression(expr)?;
                Ok((name.clone(), value))
            })
            .collect();
        let kwarg_values = kwarg_values?;
        
        // Store the original object for super() calls
        let original_self = if let Expr::Call { func, args: call_args, .. } = object {
            if let Expr::Identifier(func_name) = func.as_ref() {
                if func_name == "super" && !call_args.is_empty() {
                    // Get the current 'self' from the calling context
                    if let Some(self_val) = self.get_variable("self") {
                        Some(self_val.clone())
                    } else {
                        None
                    }
                } else {
                    None
                }
            } else {
                None
            }
        } else {
            None
        };
        
        match &obj_value {
            Value::Object { class_name, fields: instance_fields, base_object: _, mro } => {
                // First check if the method is in the instance fields (unlikely but possible)
                if let Some(Value::Closure { params, body, captured_scope, .. }) = instance_fields.get(method) {
                    let mut method_args = vec![obj_value.clone()];
                    method_args.extend(arg_values);
                    return self.call_user_function(method, &params, body.clone(), method_args, kwarg_values, Some(captured_scope.clone()));
                }
                
                // Use MRO to find the method in the class hierarchy
                println!("Looking for method '{}' in MRO: {:?}", method, mro.linearization);
                for class_in_mro in &mro.linearization {
                    println!("Checking class '{}' for method '{}'", class_in_mro, method);
                    if let Some(class_value) = self.get_variable(class_in_mro) {
                        if let Value::Object { fields: class_methods, .. } = class_value {
                            println!("Class '{}' has methods: {:?}", class_in_mro, class_methods.keys().collect::<Vec<_>>());
                            for (method_name, method_value) in class_methods.iter() {
                                println!("  Method '{}': {:?}", method_name, method_value);
                            }
                            if let Some(Value::Closure { params, body, captured_scope, .. }) = class_methods.get(method) {
                                println!("Found method '{}' in class '{}'", method, class_in_mro);
                                // Create a new argument list with 'self' as the first argument
                                let mut method_args = vec![obj_value.clone()];
                                method_args.extend(arg_values);
                                
                                // Track which class method is currently executing for super() calls
                                let previous_executing_class = self.current_executing_class.clone();
                                self.current_executing_class = Some(class_in_mro.clone());
                                
                                let result = self.call_user_function(method, &params, body.clone(), method_args, kwarg_values, Some(captured_scope.clone()))?;
                                
                                // Restore previous executing class
                                self.current_executing_class = previous_executing_class;
                                
                                return Ok(result);
                            } else if let Some(Value::NativeFunction(func)) = class_methods.get(method) {
                                // Handle native function methods
                                let mut method_args = vec![obj_value.clone()];
                                method_args.extend(arg_values);
                                return func(method_args);
                            }
                        }
                    } else {
                        println!("Class '{}' not found in variables", class_in_mro);
                    }
                }
                
                Err(anyhow::anyhow!("'{}' object has no method '{}'", class_name, method))
            }
            Value::Super(current_class, parent_class, self_obj) => {
                // Look up the method in the parent class
                if let Some(Value::Object { fields: parent_methods, .. }) = self.get_variable(parent_class) {
                    if let Some(Value::Closure { params, body, captured_scope, .. }) = parent_methods.get(method) {
                        let self_arg = if let Some(obj) = self_obj {
                            *obj.clone()
                        } else {
                             // Fallback to getting 'self' from current scope
                             if let Some(self_val) = self.get_variable("self") {
                                 self_val.clone()
                             } else {
                                 Value::Object {
                        class_name: current_class.clone(),
                        fields: HashMap::new(),
                        base_object: crate::base_object::BaseObject::new(current_class.clone(), vec!["object".to_string()]),
                        mro: crate::base_object::MRO::from_linearization(vec![current_class.clone(), "object".to_string()]),
                    }
                             }
                         };
                        
                        let mut method_args = vec![self_arg];
                        method_args.extend(arg_values);
                        
                        // Track which class method is currently executing for super() calls
                        let previous_executing_class = self.current_executing_class.clone();
                        self.current_executing_class = Some(parent_class.clone());
                        
                        let result = self.call_user_function(method, &params, body.clone(), method_args, kwarg_values, Some(captured_scope.clone()))?;
                        
                        // Restore previous executing class
                        self.current_executing_class = previous_executing_class;
                        
                        return Ok(result);
                    } else if let Some(Value::NativeFunction(func)) = parent_methods.get(method) {
                        // Handle native function methods
                        let self_arg = if let Some(obj) = self_obj {
                            *obj.clone()
                        } else {
                             if let Some(self_val) = self.get_variable("self") {
                                 self_val.clone()
                             } else {
                                 Value::Object {
                    class_name: current_class.clone(),
                    fields: HashMap::new(),
                    base_object: crate::base_object::BaseObject::new(current_class.clone(), vec!["object".to_string()]),
                    mro: crate::base_object::MRO::from_linearization(vec![current_class.clone(), "object".to_string()]),
                }
                             }
                         };
                        
                        let mut method_args = vec![self_arg];
                        method_args.extend(arg_values);
                        return func(method_args);
                    }
                }
                
                Err(anyhow::anyhow!("'super' object has no method '{}'", method))
            }
            Value::Str(s) => {
                // Built-in string methods
                match method {
                    "upper" => Ok(Value::Str(s.to_uppercase())),
                    "lower" => Ok(Value::Str(s.to_lowercase())),
                    "title" => {
                        let mut result = String::new();
                        let mut capitalize_next = true;
                        for c in s.chars() {
                            if c.is_alphabetic() {
                                if capitalize_next {
                                    result.push(c.to_uppercase().next().unwrap_or(c));
                                    capitalize_next = false;
                                } else {
                                    result.push(c.to_lowercase().next().unwrap_or(c));
                                }
                            } else {
                                result.push(c);
                                capitalize_next = true;
                            }
                        }
                        Ok(Value::Str(result))
                    }
                    "capitalize" => {
                        let mut chars = s.chars();
                        match chars.next() {
                            None => Ok(Value::Str(String::new())),
                            Some(first) => {
                                let capitalized = first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase();
                                Ok(Value::Str(capitalized))
                            }
                        }
                    }
                    "strip" => Ok(Value::Str(s.trim().to_string())),
                    "replace" => {
                        if arg_values.len() != 2 {
                            return Err(anyhow::anyhow!("replace() takes exactly 2 arguments"));
                        }
                        if let (Value::Str(old), Value::Str(new)) = (&arg_values[0], &arg_values[1]) {
                            Ok(Value::Str(s.replace(old, new)))
                        } else {
                            Err(anyhow::anyhow!("replace() arguments must be strings"))
                        }
                    }
                    "split" => {
                        let delimiter = if arg_values.is_empty() {
                            None
                        } else if let Value::Str(delim) = &arg_values[0] {
                            Some(delim.as_str())
                        } else {
                            return Err(anyhow::anyhow!("split() delimiter must be a string"));
                        };
                        
                        let parts: Vec<Value> = if let Some(delim) = delimiter {
                            s.split(delim).map(|part| Value::Str(part.to_string())).collect()
                        } else {
                            s.split_whitespace().map(|part| Value::Str(part.to_string())).collect()
                        };
                        Ok(Value::List(parts))
                    }
                    "join" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("join() takes exactly 1 argument"));
                        }
                        if let Value::List(items) = &arg_values[0] {
                            let strings: Result<Vec<String>, _> = items.iter().map(|item| {
                                match item {
                                    Value::Str(s) => Ok(s.clone()),
                                    _ => Err(anyhow::anyhow!("join() argument must be a list of strings"))
                                }
                            }).collect();
                            Ok(Value::Str(strings?.join(s)))
                        } else {
                            Err(anyhow::anyhow!("join() argument must be a list"))
                        }
                    }
                    _ => Err(anyhow::anyhow!("'str' object has no method '{}'", method)),
                }
            }
            Value::List(list) => {
                // Built-in list methods
                match method {
                    "append" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("append() takes exactly 1 argument"));
                        }
                        // Note: This creates a new list since we can't mutate in place easily
                        let mut new_list = list.clone();
                        new_list.push(arg_values[0].clone());
                        Ok(Value::List(new_list))
                    }
                    "extend" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("extend() takes exactly 1 argument"));
                        }
                        if let Value::List(other) = &arg_values[0] {
                            let mut new_list = list.clone();
                            new_list.extend(other.clone());
                            Ok(Value::List(new_list))
                        } else {
                            Err(anyhow::anyhow!("extend() argument must be a list"))
                        }
                    }
                    "pop" => {
                        let index = if arg_values.is_empty() {
                            list.len().saturating_sub(1)
                        } else if let Value::Int(i) = &arg_values[0] {
                            *i as usize
                        } else {
                            return Err(anyhow::anyhow!("pop() index must be an integer"));
                        };
                        
                        if index < list.len() {
                            Ok(list[index].clone())
                        } else {
                            Err(anyhow::anyhow!("pop index out of range"))
                        }
                    },
                    _ => Err(anyhow::anyhow!("'list' object has no method '{}'", method)),
                }
            }
            Value::Tuple(tuple) => {
                // Built-in tuple methods
                match method {
                    "count" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("count() takes exactly 1 argument"));
                        }
                        let mut count = 0;
                        for item in tuple {
                            if self.values_equal(item.clone(), arg_values[0].clone()) {
                                count += 1;
                            }
                        }
                        Ok(Value::Int(count))
                    }
                    "index" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("index() takes exactly 1 argument"));
                        }
                        for (i, item) in tuple.iter().enumerate() {
                            if self.values_equal(item.clone(), arg_values[0].clone()) {
                                return Ok(Value::Int(i as i64));
                            }
                        }
                        Err(anyhow::anyhow!("Item not found in tuple"))
                    }
                    _ => Err(anyhow::anyhow!("'tuple' object has no method '{}'", method)),
                }
            },
            Value::Set(set) => {
                // Built-in set methods
                match method {
                    "add" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("add() takes exactly 1 argument"));
                        }
                        let mut new_set = set.clone();
                        if !new_set.iter().any(|existing: &Value| self.values_equal(existing.clone(), arg_values[0].clone())) {
                            new_set.push(arg_values[0].clone());
                        }
                        Ok(Value::Set(new_set))
                    }
                    "remove" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("remove() takes exactly 1 argument"));
                        }
                        let mut new_set = set.clone();
                        let index = new_set.iter().position(|existing: &Value| self.values_equal(existing.clone(), arg_values[0].clone()));
                        if let Some(index) = index {
                            new_set.remove(index);
                            Ok(Value::Set(new_set))
                        } else {
                            Err(anyhow::anyhow!("Item not found in set"))
                        }
                    }
                    "discard" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("discard() takes exactly 1 argument"));
                        }
                        let mut new_set = set.clone();
                        let index = new_set.iter().position(|existing: &Value| self.values_equal(existing.clone(), arg_values[0].clone()));
                        if let Some(index) = index {
                            new_set.remove(index);
                        }
                        Ok(Value::Set(new_set))
                    }
                    "pop" => {
                        let mut new_set = set.clone();
                        if let Some(value) = new_set.pop() {
                            Ok(value)
                        } else {
                            Err(anyhow::anyhow!("pop from an empty set"))
                        }
                    }
                    "clear" => {
                        Ok(Value::Set(Vec::new()))
                    }
                    "union" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("union() takes exactly 1 argument"));
                        }
                        if let Value::Set(other) = &arg_values[0] {
                            let mut new_set = set.clone();
                            for item in other {
                                if !new_set.iter().any(|existing: &Value| self.values_equal(existing.clone(), item.clone())) {
                                    new_set.push(item.clone());
                                }
                            }
                            Ok(Value::Set(new_set))
                        } else {
                            Err(anyhow::anyhow!("union() argument must be a set"))
                        }
                    }
                    "intersection" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("intersection() takes exactly 1 argument"));
                        }
                        if let Value::Set(other) = &arg_values[0] {
                            let mut new_set = Vec::new();
                            for item in set {
                                if other.iter().any(|existing: &Value| self.values_equal(existing.clone(), item.clone())) {
                                    new_set.push(item.clone());
                                }
                            }
                            Ok(Value::Set(new_set))
                        } else {
                            Err(anyhow::anyhow!("intersection() argument must be a set"))
                        }
                    }
                    "difference" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("difference() takes exactly 1 argument"));
                        }
                        if let Value::Set(other) = &arg_values[0] {
                            let mut new_set = Vec::new();
                            for item in set {
                                if !other.iter().any(|existing: &Value| self.values_equal(existing.clone(), item.clone())) {
                                    new_set.push(item.clone());
                                }
                            }
                            Ok(Value::Set(new_set))
                        } else {
                            Err(anyhow::anyhow!("difference() argument must be a set"))
                        }
                    }
                    "symmetric_difference" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("symmetric_difference() takes exactly 1 argument"));
                        }
                        if let Value::Set(other) = &arg_values[0] {
                            let mut new_set = Vec::new();
                            for item in set {
                                if !other.iter().any(|existing: &Value| self.values_equal(existing.clone(), item.clone())) {
                                    new_set.push(item.clone());
                                }
                            }
                            for item in other {
                                if !set.iter().any(|existing: &Value| self.values_equal(existing.clone(), item.clone())) {
                                    new_set.push(item.clone());
                                }
                            }
                            Ok(Value::Set(new_set))
                        } else {
                            Err(anyhow::anyhow!("symmetric_difference() argument must be a set"))
                        }
                    }
                    _ => Err(anyhow::anyhow!("'set' object has no method '{}'", method)),
                }
            }
            Value::Dict(dict) => {
                // Built-in dictionary methods
                match method {
                    "keys" => {
                        let keys = dict.keys().map(|k| Value::Str(k.clone())).collect();
                        Ok(Value::List(keys))
                    }
                    "values" => {
                        let values = dict.values().cloned().collect();
                        Ok(Value::List(values))
                    }
                    "items" => {
                        let items = dict.iter().map(|(k, v)| Value::Tuple(vec![Value::Str(k.clone()), v.clone()])).collect();
                        Ok(Value::List(items))
                    }
                    "get" => {
                        if arg_values.len() < 1 || arg_values.len() > 2 {
                            return Err(anyhow::anyhow!("get() takes 1 or 2 arguments"));
                        }
                        let key = match &arg_values[0] {
                            Value::Str(s) => s.clone(),
                            _ => return Err(anyhow::anyhow!("get() key must be a string")),
                        };
                        let default = if arg_values.len() == 2 { &arg_values[1] } else { &Value::None };
                        Ok(dict.get(&key).cloned().unwrap_or_else(|| default.clone()))
                    }
                    "pop" => {
                        if arg_values.len() < 1 || arg_values.len() > 2 {
                            return Err(anyhow::anyhow!("pop() takes 1 or 2 arguments"));
                        }
                        let key = match &arg_values[0] {
                            Value::Str(s) => s.clone(),
                            _ => return Err(anyhow::anyhow!("pop() key must be a string")),
                        };
                        let mut new_dict = dict.clone();
                        if let Some(value) = new_dict.remove(&key) {
                            Ok(value)
                        } else if arg_values.len() == 2 {
                            Ok(arg_values[1].clone())
                        } else {
                            Err(anyhow::anyhow!("Key not found: {}", key))
                        }
                    }
                    "update" => {
                        if arg_values.len() != 1 {
                            return Err(anyhow::anyhow!("update() takes exactly 1 argument"));
                        }
                        if let Value::Dict(other) = &arg_values[0] {
                            let mut new_dict = dict.clone();
                            new_dict.extend(other.clone());
                            Ok(Value::Dict(new_dict))
                        } else {
                            Err(anyhow::anyhow!("update() argument must be a dictionary"))
                        }
                    }
                    "clear" => {
                        Ok(Value::Dict(HashMap::new()))
                    }
                    _ => Err(anyhow::anyhow!("'dict' object has no method '{}'", method)),
                }
            }
            Value::Module(_, namespace) => {
                // Module method calls - look up the function in the module's namespace
                if let Some(function_value) = namespace.get(method) {
                    match function_value {
                        Value::Closure { name, params, body, captured_scope, .. } => {
                            self.call_user_function(name, params, body.clone(), arg_values, kwarg_values, Some(captured_scope.clone()))
                        }
                        Value::BuiltinFunction(_, func) => {
                            func(arg_values)
                        }
                        Value::NativeFunction(func) => {
                            func(arg_values)
                        }
                        _ => Err(anyhow::anyhow!("'{}' is not a callable function", method)),
                    }
                } else {
                    Err(anyhow::anyhow!("module has no function '{}'", method))
                }
            }
            _ => Err(anyhow::anyhow!("'{}' object has no method '{}'", obj_value.type_name(), method)),
        }
    }
    
    /// Execute list comprehension
     fn execute_list_comprehension(&mut self, element: &Expr, generators: &[Comprehension]) -> Result<Value> {
         let mut result = Vec::new();
         self.execute_comprehension_recursive(element, generators, 0, &mut result)?;
         Ok(Value::List(result))
     }
     
     /// Execute dictionary comprehension
     fn execute_dict_comprehension(&mut self, key: &Expr, value: &Expr, generators: &[Comprehension]) -> Result<Value> {
         let mut result = HashMap::new();
         self.execute_dict_comprehension_recursive(key, value, generators, 0, &mut result)?;
         Ok(Value::Dict(result))
     }
     
     /// Execute set comprehension
     fn execute_set_comprehension(&mut self, element: &Expr, generators: &[Comprehension]) -> Result<Value> {
         let mut result = Vec::new();
         self.execute_comprehension_recursive(element, generators, 0, &mut result)?;
         
         // Remove duplicates to create a proper set
         let mut unique_values = Vec::new();
         for value in result {
             if !unique_values.iter().any(|existing: &Value| self.values_equal(existing.clone(), value.clone())) {
                 unique_values.push(value);
             }
         }
         
         Ok(Value::Set(unique_values))
     }
     
     /// Recursive helper for list comprehension
     fn execute_comprehension_recursive(
         &mut self,
         element: &Expr,
         generators: &[Comprehension],
         gen_index: usize,
         result: &mut Vec<Value>,
     ) -> Result<()> {
         if gen_index >= generators.len() {
             // Base case: evaluate element and add to result
             let value = self.execute_expression(element)?;
             result.push(value);
             return Ok(());
         }
         
         let gen = &generators[gen_index];
         let iter_value = self.execute_expression(&gen.iter)?;
         
         let items = match iter_value {
             Value::List(items) => items,
             Value::Str(s) => s.chars().map(|c| Value::Str(c.to_string())).collect(),
             _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
         };
         
         for item in items {
             self.enter_scope("comprehension");
             self.set_variable(&gen.target, item)?;
             
             // Check conditions
             let mut should_continue = true;
             for condition in &gen.ifs {
                 let cond_value = self.execute_expression(condition)?;
                 if !cond_value.is_truthy() {
                     should_continue = false;
                     break;
                 }
             }
             
             if should_continue {
                 self.execute_comprehension_recursive(element, generators, gen_index + 1, result)?;
             }
             
             self.exit_scope();
         }
         
         Ok(())
     }
     
     /// Recursive helper for dictionary comprehension
     fn execute_dict_comprehension_recursive(
         &mut self,
         key_expr: &Expr,
         value_expr: &Expr,
         generators: &[Comprehension],
         gen_index: usize,
         result: &mut HashMap<String, Value>,
     ) -> Result<()> {
         if gen_index >= generators.len() {
             // Base case: evaluate key and value and add to result
             let key = self.execute_expression(key_expr)?;
             let value = self.execute_expression(value_expr)?;
             let key_string = match &key {
                 Value::Str(s) => s.clone(),
                 Value::Int(n) => n.to_string(),
                 Value::Float(n) => format!("{:.6}", n),
                 Value::Bool(b) => b.to_string(),
                 Value::None => "None".to_string(),
                 _ => format!("{}", key),
             };
             result.insert(key_string, value);
             return Ok(());
         }
         
         let gen = &generators[gen_index];
         let iter_value = self.execute_expression(&gen.iter)?;
         
         let items = match iter_value {
             Value::List(items) => items,
             Value::Str(s) => s.chars().map(|c| Value::Str(c.to_string())).collect(),
             _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
         };
         
         for item in items {
             self.enter_scope("comprehension");
             self.set_variable(&gen.target, item)?;
             
             // Check conditions
             let mut should_continue = true;
             for condition in &gen.ifs {
                 let cond_value = self.execute_expression(condition)?;
                 if !cond_value.is_truthy() {
                     should_continue = false;
                     break;
                 }
             }
             
             if should_continue {
                 self.execute_dict_comprehension_recursive(key_expr, value_expr, generators, gen_index + 1, result)?;
             }
             
             self.exit_scope();
         }
         
         Ok(())
     }
    
    /// Execute assignment
    fn execute_assignment(&mut self, target: &str, value: &Expr) -> Result<()> {
        let value = self.execute_expression(value)?;
        self.set_variable(target, value)?;
        Ok(())
    }
    
    /// Execute match statement
    fn execute_match_statement(&mut self, value: &Expr, cases: &[MatchCase]) -> Result<Option<Value>> {
        let match_value = self.execute_expression(value)?;
        
        for case in cases {
            // Check if pattern matches
            if self.pattern_matches(&case.pattern, &match_value)? {
                // Check guard condition if present
                if let Some(guard) = &case.guard {
                    let guard_value = self.execute_expression(guard)?;
                    if !guard_value.is_truthy() {
                        continue; // Guard failed, try next case
                    }
                }
                
                // Execute case body
                self.enter_scope("match_case");
                let mut result = None;
                for stmt in &case.body {
                    result = self.execute_statement(stmt)?;
                    if self.should_return {
                        break;
                    }
                }
                self.exit_scope();
                return Ok(result);
            }
        }
        
        // No case matched
        Err(anyhow::anyhow!("No pattern matched in match statement"))
    }
    
    /// Check if a pattern matches a value
    fn pattern_matches(&mut self, pattern: &Pattern, value: &Value) -> Result<bool> {
        match pattern {
            Pattern::Wildcard => Ok(true),
            Pattern::Literal(expr) => {
                let pattern_value = self.execute_expression(expr)?;
                Ok(self.values_equal(pattern_value, value.clone()))
            }
            Pattern::Variable(name) => {
                // Variable patterns always match and bind the value
                self.set_variable(name, value.clone())?;
                Ok(true)
            }
            Pattern::Tuple(patterns) => {
                match value {
                    Value::Tuple(values) => {
                        if patterns.len() != values.len() {
                            return Ok(false);
                        }
                        for (pattern, val) in patterns.iter().zip(values.iter()) {
                            if !self.pattern_matches(pattern, val)? {
                                return Ok(false);
                            }
                        }
                        Ok(true)
                    }
                    _ => Ok(false),
                }
            }
            Pattern::List(patterns) => {
                match value {
                    Value::List(values) => {
                        if patterns.len() != values.len() {
                            return Ok(false);
                        }
                        for (pattern, val) in patterns.iter().zip(values.iter()) {
                            if !self.pattern_matches(pattern, val)? {
                                return Ok(false);
                            }
                        }
                        Ok(true)
                    }
                    _ => Ok(false),
                }
            }
            Pattern::Dict(pattern_pairs) => {
                match value {
                    Value::Dict(dict) => {
                        for (key_pattern, value_pattern) in pattern_pairs {
                            // For now, we only support literal keys in dict patterns
                            if let Pattern::Literal(key_expr) = key_pattern {
                                let key_value = self.execute_expression(key_expr)?;
                                let key_string = match &key_value {
                                    Value::Str(s) => s.clone(),
                                    Value::Int(n) => n.to_string(),
                                    Value::Float(n) => format!("{:.6}", n),
                                    Value::Bool(b) => b.to_string(),
                                    Value::None => "None".to_string(),
                                    _ => format!("{}", key_value),
                                };
                                
                                if let Some(dict_value) = dict.get(&key_string) {
                                    if !self.pattern_matches(value_pattern, dict_value)? {
                                        return Ok(false);
                                    }
                                } else {
                                    return Ok(false);
                                }
                            } else {
                                return Err(anyhow::anyhow!("Dictionary key patterns must be literals"));
                            }
                        }
                        Ok(true)
                    }
                    _ => Ok(false),
                }
            }
            Pattern::Class { name, patterns } => {
                // Class pattern matching - check if value is instance of class
                match value {
                    Value::Object { class_name, .. } => {
                        if class_name == name {
                            // For now, simple class matching without pattern destructuring
                            // TODO: Implement proper class pattern destructuring
                            Ok(true)
                        } else {
                            Ok(false)
                        }
                    }
                    _ => Ok(false),
                }
            }
            Pattern::Or(patterns) => {
                // OR pattern - any pattern can match
                for pattern in patterns {
                    if self.pattern_matches(pattern, value)? {
                        return Ok(true);
                    }
                }
                Ok(false)
            }
            Pattern::As { pattern, name } => {
                // AS pattern - pattern must match and value is bound to name
                if self.pattern_matches(pattern, value)? {
                    self.set_variable(name, value.clone())?;
                    Ok(true)
                } else {
                    Ok(false)
                }
            }
        }
    }
    
    /// Execute with statement (context manager)
    fn execute_with_statement(&mut self, context: &Expr, alias: Option<&str>, body: &[Statement]) -> Result<Option<Value>> {
        // Evaluate the context manager expression
        let context_manager = self.execute_expression(context)?;
        
        // Call __enter__ method
        let enter_result = self.call_context_manager_method(&context_manager, "__enter__", vec![])?;
        
        // Bind the result to alias if provided
        if let Some(alias_name) = alias {
            self.set_variable(alias_name, enter_result)?;
        }
        
        self.enter_scope("with");
        let mut result = None;
        let mut exception_occurred = false;
        let mut exception_value = None;
        
        // Execute the with body
        for stmt in body {
            match self.execute_statement(stmt) {
                Ok(value) => result = value,
                Err(error) => {
                    exception_occurred = true;
                    exception_value = Some(error);
                    break;
                }
            }
            if self.should_return {
                break;
            }
        }
        
        self.exit_scope();
        
        // Call __exit__ method with exception info
        let (exc_type, exc_value, exc_traceback) = if exception_occurred {
            // In a real implementation, we'd extract proper exception info
            let error = exception_value.as_ref().unwrap();
            (Value::Str("Exception".to_string()), Value::Str(error.to_string()), Value::None)
        } else {
            (Value::None, Value::None, Value::None)
        };
        
        let exit_args = vec![exc_type, exc_value, exc_traceback];
        let exit_result = self.call_context_manager_method(&context_manager, "__exit__", exit_args)?;
        
        // If __exit__ returns True, suppress the exception
        let suppress_exception = exit_result.is_truthy();
        
        if exception_occurred && !suppress_exception {
            return Err(exception_value.unwrap());
        }
        
        Ok(result)
    }
    
    /// Helper method to call context manager methods (__enter__ and __exit__)
    fn call_context_manager_method(&mut self, context_manager: &Value, method_name: &str, args: Vec<Value>) -> Result<Value> {
        match context_manager {
            Value::Object { fields, .. } => {
                if let Some(method) = fields.get(method_name) {
                    match method {
                        Value::Closure { name, params, body, captured_scope, .. } => {
                            let mut method_args = vec![context_manager.clone()];
                            method_args.extend(args);
                            self.call_user_function(name, params, body.clone(), method_args, HashMap::new(), Some(captured_scope.clone()))
                        }
                        Value::BuiltinFunction(_, func) => {
                            let mut method_args = vec![context_manager.clone()];
                            method_args.extend(args);
                            func(method_args)
                        }
                        Value::NativeFunction(func) => {
                            let mut method_args = vec![context_manager.clone()];
                            method_args.extend(args);
                            func(method_args)
                        }
                        _ => Err(anyhow::anyhow!("'{}' is not callable", method_name)),
                    }
                } else {
                    // Use default implementation or fall back to base object methods
                    match method_name {
                        "__enter__" => Ok(context_manager.clone()),
                        "__exit__" => Ok(Value::Bool(false)),
                        _ => Err(anyhow::anyhow!("'{}' object has no attribute '{}'", context_manager.type_name(), method_name)),
                    }
                }
            }
            _ => {
                // For non-object types, provide default behavior
                match method_name {
                    "__enter__" => Ok(context_manager.clone()),
                    "__exit__" => Ok(Value::Bool(false)),
                    _ => Err(anyhow::anyhow!("'{}' object has no attribute '{}'", context_manager.type_name(), method_name)),
                }
            }
        }
    }
    
    /// Execute if statement
    fn execute_if_statement(
        &mut self,
        condition: &Expr,
        then_branch: &[Statement],
        elif_branches: &[(Expr, Vec<Statement>)],
        else_branch: &Option<Vec<Statement>>,
    ) -> Result<Option<Value>> {
        let cond_value = self.execute_expression(condition)?;
        
        if cond_value.is_truthy() {
            self.enter_scope("block");
            for stmt in then_branch {
                self.execute_statement(stmt)?;
                if self.should_return {
                    break;
                }
            }
            self.exit_scope();
        } else {
            // Check elif branches
            let mut matched = false;
            for (elif_cond, elif_body) in elif_branches {
                let elif_value = self.execute_expression(elif_cond)?;
                if elif_value.is_truthy() {
                    self.enter_scope("block");
                    for stmt in elif_body {
                        self.execute_statement(stmt)?;
                        if self.should_return {
                            break;
                        }
                    }
                    self.exit_scope();
                    matched = true;
                    break;
                }
            }
            
            // Execute else branch if no elif matched
            if !matched {
                if let Some(else_body) = else_branch {
                    self.enter_scope("block");
                    for stmt in else_body {
                        self.execute_statement(stmt)?;
                        if self.should_return {
                            break;
                        }
                    }
                    self.exit_scope();
                }
            }
        }
        
        Ok(None)
    }
    
    /// Execute while statement
    fn execute_while_statement(&mut self, condition: &Expr, body: &[Statement]) -> Result<Option<Value>> {
        while {
            let cond_value = self.execute_expression(condition)?;
            cond_value.is_truthy()
        } {
            self.enter_scope("block");
            for stmt in body {
                self.execute_statement(stmt)?;
                if self.should_return {
                    self.exit_scope();
                    return Ok(None);
                }
            }
            self.exit_scope();
        }
        
        Ok(None)
    }
    
    /// Execute for statement
    fn execute_for_statement(&mut self, variable: &str, iterable: &Expr, body: &[Statement]) -> Result<Option<Value>> {
        let iter_value = self.execute_expression(iterable)?;
        
        let items = match iter_value {
            Value::List(items) => items,
            Value::Str(s) => s.chars().map(|c| Value::Str(c.to_string())).collect(),
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
        };
        
        for item in items {
            self.enter_scope("block");
            self.set_variable(variable, item)?;
            
            for stmt in body {
                self.execute_statement(stmt)?;
                if self.should_return {
                    self.exit_scope();
                    return Ok(None);
                }
            }
            
            self.exit_scope();
        }
        
        Ok(None)
    }
    
    /// Execute import statement
    fn execute_import_statement(&mut self, module: &str, alias: Option<&str>) -> Result<Option<Value>> {
        // Use the proper module system instead of creating mock objects
        let import_spec = ImportSpec::Simple {
            module: module.to_string(),
            alias: alias.map(|s| s.to_string()),
        };
        
        // We need to temporarily take ownership of the module system to avoid borrowing conflicts
        let mut module_system = std::mem::replace(&mut self.module_system, ModuleSystem::new());
        let variables = module_system.import_module(self, import_spec)?;
        self.module_system = module_system;
        
        for variable in variables {
            self.set_variable(&variable.0, variable.1)?;
        }
        
        Ok(None)
    }
    
    /// Execute extern statement
    fn execute_extern_statement(&mut self, library: &str) -> Result<Option<Value>> {
        println!("Loading external library: {}", library);
        // Actual library loading would be implemented here
        Ok(None)
    }
    
    /// Instantiate a class
    fn instantiate_class(&mut self, class_name: &str, class_methods: HashMap<String, Value>, args: Vec<Value>) -> Result<Value> {
        // Get the correct MRO from the type creator's MRO computation
        let base_classes = self.class_base_registry.get(class_name)
            .cloned()
            .unwrap_or_else(|| vec!["object".to_string()]);
        
        // Use the type creator to compute the correct MRO
        let mro_linearization = self.type_creator.mro_computer.compute_optimized_c3_linearization(
            class_name,
            &base_classes,
            &self.class_registry,
        ).map_err(|e| anyhow::anyhow!("Failed to compute MRO for instance: {}", e))?;
        
        println!("Creating instance of '{}' with MRO: {:?}", class_name, mro_linearization);
        
        // Look for __init__ method in class methods
        if let Some(init_method) = class_methods.get("__init__") {
            match init_method {
                Value::Closure { params, body, captured_scope, .. } => {
                    // Create a new instance with empty fields (not class methods) but correct MRO
                    let base_classes = self.class_base_registry.get(class_name)
                        .cloned()
                        .unwrap_or_else(|| vec!["object".to_string()]);
                    
                    let instance = Value::Object {
                        class_name: class_name.to_string(),
                        fields: HashMap::new(),
                        base_object: crate::base_object::BaseObject::new(class_name.to_string(), base_classes),
                        mro: crate::base_object::MRO::from_linearization(mro_linearization.clone()),
                    };
                    
                    // Prepare arguments: self + provided args
                    let mut init_args = vec![instance];
                    init_args.extend(args);
                    
                    // Call __init__ method
                    let modified_instance = self.call_user_function("__init__", &params, body.clone(), init_args, HashMap::new(), Some(captured_scope.clone()))?;
                    
                    // Return the modified instance
                    Ok(modified_instance)
                }
                _ => Err(anyhow::anyhow!("__init__ is not a function")),
            }
        } else {
            // No __init__ method, just create instance with empty fields but correct MRO
            if !args.is_empty() {
                return Err(anyhow::anyhow!("{}() takes no arguments but {} were given", 
                    class_name, args.len()));
            }
            
            let base_classes = self.class_base_registry.get(class_name)
                .cloned()
                .unwrap_or_else(|| vec!["object".to_string()]);
            
            Ok(Value::Object {
                class_name: class_name.to_string(),
                fields: HashMap::new(),
                base_object: crate::base_object::BaseObject::new(class_name.to_string(), base_classes),
                mro: MRO::from_linearization(mro_linearization),
            })
        }
    }

    /// Call user-defined function
    fn check_value_type(&self, value: &Value, expected_type: &Type) -> bool {
        match (value, expected_type) {
            (Value::Int(_), Type::Simple(name)) if name == "int" => true,
            (Value::Float(_), Type::Simple(name)) if name == "float" => true,
            (Value::Str(_), Type::Simple(name)) if name == "str" => true,
            (Value::Bool(_), Type::Simple(name)) if name == "bool" => true,
            (Value::List(_), Type::Simple(name)) if name == "list" => true,
            (Value::Dict(_), Type::Simple(name)) if name == "dict" => true,
            (Value::None, Type::Simple(name)) if name == "None" => true,
            (Value::Object { class_name, .. }, Type::Simple(expected_class)) => {
                class_name == expected_class
            }
            _ => false,
        }
    }

    fn get_value_type(&self, value: &Value) -> String {
        match value {
            Value::Int(_) => "int".to_string(),
            Value::Float(_) => "float".to_string(),
            Value::Str(_) => "str".to_string(),
            Value::Bool(_) => "bool".to_string(),
            Value::List(_) => "list".to_string(),
            Value::Dict(_) => "dict".to_string(),
            Value::None => "None".to_string(),
            Value::Object { class_name, .. } => class_name.clone(),
            Value::Closure { name, .. } => format!("function({})", name),
            Value::BuiltinFunction(name, _) => format!("builtin({})", name),
            Value::NativeFunction(_) => format!("native function"),
            Value::TypedValue { type_info, .. } => format!("{:?}", type_info),
            _ => "unknown".to_string(),
        }
    }



    pub fn call_user_function(&mut self, name: &str, params: &[Param], body: Vec<Statement>, args: Vec<Value>, kwargs: HashMap<String, Value>, captured_scope: Option<HashMap<String, Value>>) -> Result<Value> {
        // Push new stack frame
        self.call_stack.push(StackFrame {
            function_name: name.to_string(),
            return_address: self.current_scope,
            scope_index: self.current_scope,
        });
        
        // Enter function scope
        let mut new_scope = Scope::new();
        if let Some(captured) = captured_scope {
            new_scope.variables = captured;
        }
        new_scope.parent = Some(self.current_scope);
        self.scopes.push(new_scope);
        self.current_scope = self.scopes.len() - 1;

        // Set parameters
        let mut arg_index = 0;
        let mut varargs = Vec::new();
        let mut varkwargs = HashMap::new();

        for param in params {
            match param.kind {
                ParamKind::VarArgs => {
                    while arg_index < args.len() {
                        varargs.push(args[arg_index].clone());
                        arg_index += 1;
                    }
                    self.set_variable(&param.name, Value::List(varargs.clone()))?;
                }
                ParamKind::VarKwargs => {
                    for (name, value) in &kwargs {
                        if !params.iter().any(|p| p.name == *name) {
                            varkwargs.insert(name.clone(), value.clone());
                        }
                    }
                    self.set_variable(&param.name, Value::Dict(varkwargs.clone()))?;
                }
                _ => {
                    if let Some(value) = kwargs.get(&param.name) {
                        self.set_variable(&param.name, value.clone())?;
                    } else if arg_index < args.len() {
                        self.set_variable(&param.name, args[arg_index].clone())?;
                        arg_index += 1;
                    } else if let Some(default_expr) = &param.default {
                        let default_value = self.execute_expression(default_expr)?;
                        self.set_variable(&param.name, default_value)?;
                    } else {
                        self.set_variable(&param.name, Value::None)?;
                    }
                }
            }
        }
        
        // Execute function body
        let mut result = Value::None;
        for stmt in body {
            if let Some(value) = self.execute_statement(&stmt)? {
                result = value;
            }
            
            if self.should_return {
                result = self.return_value.take().unwrap_or(Value::None);
                self.should_return = false;
                break;
            }
        }
        
        // For __init__ methods, return the modified 'self' parameter
        if name == "__init__" && !params.is_empty() {
            if let Some(modified_self) = self.get_variable(&params[0].name) {
                result = modified_self;
            }
        }
        
        // Exit function scope
        self.exit_scope();
        
        // Pop stack frame
        self.call_stack.pop();
        
        Ok(result)
    }
    
    /// Format a value for display in f-strings
    fn format_value(&self, value: &Value) -> String {
        match value {
            Value::Str(s) => s.clone(),
            Value::Int(n) => n.to_string(),
            Value::Float(n) => n.to_string(),
            Value::Bool(b) => b.to_string(),
            Value::None => "None".to_string(),
            Value::List(items) => {
                let formatted_items: Vec<String> = items.iter()
                    .map(|item| self.format_value(item))
                    .collect();
                format!("[{}]", formatted_items.join(", "))
            }
            Value::Tuple(items) => {
                let formatted_items: Vec<String> = items.iter()
                    .map(|item| self.format_value(item))
                    .collect();
                if items.len() == 1 {
                    format!("({},)", formatted_items[0])
                } else {
                    format!("({})", formatted_items.join(", "))
                }
            }
            Value::Dict(dict) => {
                let formatted_pairs: Vec<String> = dict.iter()
                    .map(|(k, v)| format!("'{}': {}", k, self.format_value(v)))
                    .collect();
                format!("{{{}}}", formatted_pairs.join(", "))
            }
            _ => format!("{:?}", value),
        }
    }
    
    /// Format a value with a format specifier
    fn format_value_with_spec(&self, value: &Value, spec: &str) -> Result<String> {
        match value {
            Value::Int(n) => {
                if spec.is_empty() {
                    Ok(n.to_string())
                } else if spec == "d" {
                    Ok(n.to_string())
                } else if spec == "x" {
                    Ok(format!("{:x}", n))
                } else if spec == "X" {
                    Ok(format!("{:X}", n))
                } else if spec == "o" {
                    Ok(format!("{:o}", n))
                } else if spec == "b" {
                    Ok(format!("{:b}", n))
                } else if spec.starts_with('0') && spec.len() > 1 {
                    // Zero-padded format like "03d"
                    if let Ok(width) = spec[1..].parse::<usize>() {
                        Ok(format!("{:0width$}", n, width = width))
                    } else {
                        Ok(n.to_string())
                    }
                } else {
                    Ok(n.to_string())
                }
            }
            Value::Float(n) => {
                if spec.is_empty() {
                    Ok(n.to_string())
                } else if spec == "f" {
                    Ok(format!("{:.6}", n))
                } else if spec.starts_with('.') && spec.ends_with('f') {
                    // Precision format like ".2f"
                    if let Ok(precision) = spec[1..spec.len()-1].parse::<usize>() {
                        Ok(format!("{:.precision$}", n, precision = precision))
                    } else {
                        Ok(n.to_string())
                    }
                } else {
                    Ok(n.to_string())
                }
            }
            _ => Ok(self.format_value(value)),
        }
    }
    
    // --- Memory Management Integration ---
    
    /// Allocate value with automatic memory management
    pub fn allocate_auto(&self, value: Value) -> Value {
        // Use the memory API for automatic allocation
        let managed_ptr = self.memory.auto(value);
        Value::TypedValue {
            value: Box::new(managed_ptr.get().clone()),
            type_info: Type::Any,
        }
    }
    
    /// Allocate value with manual memory management
    pub fn allocate_manual(&self, value: Value) -> Value {
        // Use the memory API for manual allocation
        let managed_ptr = self.memory.manual(value);
        Value::TypedValue {
            value: Box::new(managed_ptr.get().clone()),
            type_info: Type::Any,
        }
    }
    
    /// Allocate value with hybrid memory management
    pub fn allocate_hybrid(&self, value: Value) -> Value {
        // Use the memory API for hybrid allocation
        let managed_ptr = self.memory.hybrid(value);
        Value::TypedValue {
            value: Box::new(managed_ptr.get().clone()),
            type_info: Type::Any,
        }
    }
    
    /// Force garbage collection
    pub fn collect_garbage(&self) {
        self.memory.collect();
    }
    
    /// Force tracing garbage collection
    pub fn collect_tracing_gc(&self) {
        self.memory.collect_tracing();
    }
    
    /// Get memory statistics
    pub fn memory_stats(&self) -> String {
        format!("Memory: {}", self.memory.stats())
    }
    
    /// Get memory usage
    pub fn memory_usage(&self) -> String {
        self.memory.memory_usage()
    }
    
    /// Set memory management mode
    pub fn set_memory_mode(&self, mode: MemoryMode) {
        self.memory.set_memory_mode(mode);
    }
    
    /// Get current memory management mode
    pub fn get_memory_mode(&self) -> MemoryMode {
        self.memory.get_memory_mode()
    }
    
    // --- Scope Management ---
    
    fn enter_scope(&mut self, scope_type: &str) {
        let new_scope = Scope {
            variables: HashMap::new(),
            variable_types: HashMap::new(),
            parent: Some(self.current_scope),
            scope_type: scope_type.to_string(),
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
    
    pub fn push_scope(&mut self, scope: Scope) {
        self.scopes.push(scope);
        self.current_scope = self.scopes.len() - 1;
    }
    
    pub fn pop_scope(&mut self) -> Scope {
        if self.scopes.len() > 1 {
            let scope = self.scopes.pop().unwrap();
            self.current_scope = self.scopes.len() - 1;
            scope
        } else {
            // Don't pop the global scope, return a copy instead
            self.scopes[0].clone()
        }
    }
    
    pub fn set_variable(&mut self, name: &str, value: Value) -> Result<()> {
        // Check for type restrictions in strict mode
        if self.strict_types {
            if let Some(expected_type) = self.get_variable_type(name) {
                if !value.check_type(&expected_type) {
                    // Try to convert the value to the expected type
                    match value.convert_to_type(&expected_type) {
                        Ok(converted_value) => {
                            self.set_variable_unchecked(name, converted_value);
                            return Ok(());
                        }
                        Err(_) => {
                            // If conversion fails, this is a type error
                            return Err(anyhow::anyhow!("Type error: cannot assign {} to variable '{}' of type {}", 
                                   value.type_name(), name, expected_type));
                        }
                    }
                }
            }
        }
        
        self.set_variable_unchecked(name, value);
        Ok(())
    }
    
    /// Set variable without type checking (internal use)
    fn set_variable_unchecked(&mut self, name: &str, value: Value) {
        
        // First check if the variable exists in any parent scope
        let mut scope_index = Some(self.current_scope);
        
        while let Some(idx) = scope_index {
            let scope = &self.scopes[idx];
            if scope.variables.contains_key(name) {
                // Variable exists in this scope, update it here
                self.scopes[idx].variables.insert(name.to_string(), value);
                return;
            }
            scope_index = scope.parent;
        }
        
        // Variable doesn't exist in any parent scope, create it in current scope
        self.scopes[self.current_scope].variables.insert(name.to_string(), value);
    }
    
    /// Declare a typed variable (for type annotations)
    pub fn declare_typed_variable(&mut self, name: &str, type_info: Type) -> Result<()> {
        self.scopes[self.current_scope].variable_types.insert(name.to_string(), type_info);
        Ok(())
    }
    
    /// Get the declared type of a variable
    pub fn get_variable_type(&self, name: &str) -> Option<Type> {
        let mut scope_index = Some(self.current_scope);
        
        while let Some(idx) = scope_index {
            let scope = &self.scopes[idx];
            if let Some(type_info) = scope.variable_types.get(name) {
                return Some(type_info.clone());
            }
            scope_index = scope.parent;
        }
        
        None
    }
    
    pub fn get_variable(&self, name: &str) -> Option<Value> {
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

    pub fn get_current_scope(&self) -> usize {
        self.current_scope
    }
    
    pub fn get_global_variables(&self) -> HashMap<String, Value> {
        // Global scope is always at index 0
        self.scopes[0].variables.clone()
    }
    
    pub fn get_local_variables(&self) -> HashMap<String, Value> {
        // Current scope variables
        self.scopes[self.current_scope].variables.clone()
    }
    
    pub fn get_current_scope_variables(&self) -> HashMap<String, Value> {
        // Same as get_local_variables - return current scope variables
        self.scopes[self.current_scope].variables.clone()
    }
    
    // --- Value Operations ---
    
    fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&left, "__add__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 + b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + b as f64)),
            (Value::Complex { real: a_real, imag: a_imag }, Value::Complex { real: b_real, imag: b_imag }) => {
                Ok(Value::Complex { real: a_real + b_real, imag: a_imag + b_imag })
            },
            (Value::Complex { real, imag }, Value::Int(n)) => {
                Ok(Value::Complex { real: real + n as f64, imag })
            },
            (Value::Complex { real, imag }, Value::Float(f)) => {
                Ok(Value::Complex { real: real + f, imag })
            },
            (Value::Int(n), Value::Complex { real, imag }) => {
                Ok(Value::Complex { real: n as f64 + real, imag })
            },
            (Value::Float(f), Value::Complex { real, imag }) => {
                Ok(Value::Complex { real: f + real, imag })
            },
            (Value::Str(a), Value::Str(b)) => Ok(Value::Str(a + &b)),
            (Value::Str(a), Value::Int(b)) => Ok(Value::Str(a + &b.to_string())),
            (Value::Str(a), Value::Float(b)) => Ok(Value::Str(a + &b.to_string())),
            (Value::List(mut a), Value::List(b)) => {
                a.extend(b);
                Ok(Value::List(a))
            }
            _ => Err(anyhow::anyhow!("Invalid types for addition")),
        }
    }
    
    fn sub_values(&self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&left, "__sub__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 - b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - b as f64)),
            (Value::Complex { real: a_real, imag: a_imag }, Value::Complex { real: b_real, imag: b_imag }) => {
                Ok(Value::Complex { real: a_real - b_real, imag: a_imag - b_imag })
            },
            (Value::Complex { real, imag }, Value::Int(n)) => {
                Ok(Value::Complex { real: real - n as f64, imag })
            },
            (Value::Complex { real, imag }, Value::Float(f)) => {
                Ok(Value::Complex { real: real - f, imag })
            },
            (Value::Int(n), Value::Complex { real, imag }) => {
                Ok(Value::Complex { real: n as f64 - real, imag: -imag })
            },
            (Value::Float(f), Value::Complex { real, imag }) => {
                Ok(Value::Complex { real: f - real, imag: -imag })
            },
            _ => Err(anyhow::anyhow!("Invalid types for subtraction")),
        }
    }
    
    fn mul_values(&self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&left, "__mul__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 * b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a * b as f64)),
            (Value::Complex { real: a_real, imag: a_imag }, Value::Complex { real: b_real, imag: b_imag }) => {
                // (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
                Ok(Value::Complex { 
                    real: a_real * b_real - a_imag * b_imag, 
                    imag: a_real * b_imag + a_imag * b_real 
                })
            },
            (Value::Complex { real, imag }, Value::Int(n)) => {
                Ok(Value::Complex { real: real * n as f64, imag: imag * n as f64 })
            },
            (Value::Complex { real, imag }, Value::Float(f)) => {
                Ok(Value::Complex { real: real * f, imag: imag * f })
            },
            (Value::Int(n), Value::Complex { real, imag }) => {
                Ok(Value::Complex { real: n as f64 * real, imag: n as f64 * imag })
            },
            (Value::Float(f), Value::Complex { real, imag }) => {
                Ok(Value::Complex { real: f * real, imag: f * imag })
            },
            (Value::Str(a), Value::Int(b)) => Ok(Value::Str(a.repeat(b as usize))),
            (Value::List(a), Value::Int(b)) => {
                let mut result = Vec::new();
                for _ in 0..b {
                    result.extend(a.clone());
                }
                Ok(Value::List(result))
            }
            _ => Err(anyhow::anyhow!("Invalid types for multiplication")),
        }
    }
    
    fn div_values(&self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&left, "__truediv__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b as f64))
                }
            }
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b as f64))
                }
            }
            _ => Err(anyhow::anyhow!("Invalid types for division")),
        }
    }
    
    fn mod_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Int(a % b))
                }
            }
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a % b))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float((a as f64) % b))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a % (b as f64)))
                }
            }
            _ => Err(anyhow::anyhow!("Invalid types for modulo")),
        }
    }
    
    fn gt_values(&mut self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method_with_vm(self, &left, "__gt__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) > b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a > b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a > b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn lt_values(&mut self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method_with_vm(self, &left, "__lt__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) < b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a < b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a < b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn gte_values(&mut self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method_with_vm(self, &left, "__ge__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) >= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a >= b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a >= b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn lte_values(&mut self, left: Value, right: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method_with_vm(self, &left, "__le__", vec![right.clone()]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) <= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a <= b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a <= b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn values_equal(&self, left: Value, right: Value) -> bool {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&left, "__eq__", vec![right.clone()]) {
            return result.is_truthy();
        }
        
        // Fallback to original implementation
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => a == b,
            (Value::Float(a), Value::Float(b)) => a == b,
            (Value::Bool(a), Value::Bool(b)) => a == b,
            (Value::Str(a), Value::Str(b)) => a == b,
            (Value::None, Value::None) => true,
            _ => false,
        }
    }
    
    fn plus_value(&self, value: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&value, "__pos__", vec![]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match value {
            Value::Int(n) => Ok(Value::Int(n)),
            Value::Float(n) => Ok(Value::Float(n)),
            _ => Err(anyhow::anyhow!("Invalid type for unary plus")),
        }
    }
    
    fn minus_value(&self, value: Value) -> Result<Value> {
        // Try dunder method first
        if let Some(result) = call_dunder_method(&value, "__neg__", vec![]) {
            return Ok(result);
        }
        
        // Fallback to original implementation
        match value {
            Value::Int(n) => Ok(Value::Int(-n)),
            Value::Float(n) => Ok(Value::Float(-n)),
            _ => Err(anyhow::anyhow!("Invalid type for unary minus")),
        }
    }
    
    fn bitnot_value(&self, value: Value) -> Result<Value> {
        match value {
            Value::Int(n) => Ok(Value::Int(!n)),
            _ => Err(anyhow::anyhow!("Invalid type for bitwise not")),
        }
    }
    
    // New operator implementations
    fn floordiv_values(&self, left: Value, right: Value) -> Result<Value> {
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
                    Ok(Value::Float((a / b).floor()))
                }
            }
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float((a as f64 / b).floor()))
                }
            }
            (Value::Float(a), Value::Int(b)) => {
                if b == 0 {
                    Err(anyhow::anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float((a / b as f64).floor()))
                }
            }
            _ => Err(anyhow::anyhow!("Invalid types for floor division")),
        }
    }
    
    fn pow_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b >= 0 {
                    Ok(Value::Int(a.pow(b as u32)))
                } else {
                    Ok(Value::Float((a as f64).powf(b as f64)))
                }
            }
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a.powf(b))),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float((a as f64).powf(b))),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a.powf(b as f64))),
            _ => Err(anyhow::anyhow!("Invalid types for power operation")),
        }
    }
    
    fn matmul_values(&self, left: Value, right: Value) -> Result<Value> {
        // Matrix multiplication - simplified implementation for now
        match (left, right) {
            (Value::List(a), Value::List(b)) => {
                // For now, treat as element-wise multiplication
                if a.len() != b.len() {
                    return Err(anyhow::anyhow!("Matrix dimensions don't match"));
                }
                let result: Result<Vec<Value>> = a.iter().zip(b.iter())
                    .map(|(x, y)| self.mul_values(x.clone(), y.clone()))
                    .collect();
                Ok(Value::List(result?))
            }
            _ => Err(anyhow::anyhow!("Matrix multiplication requires lists/matrices")),
        }
    }
    
    fn bitand_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a & b)),
            _ => Err(anyhow::anyhow!("Bitwise AND requires integers")),
        }
    }
    
    fn bitor_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a | b)),
            _ => Err(anyhow::anyhow!("Bitwise OR requires integers")),
        }
    }
    
    fn bitxor_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a ^ b)),
            _ => Err(anyhow::anyhow!("Bitwise XOR requires integers")),
        }
    }
    
    fn lshift_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b < 0 {
                    Err(anyhow::anyhow!("Negative shift count"))
                } else {
                    Ok(Value::Int(a << b))
                }
            }
            _ => Err(anyhow::anyhow!("Left shift requires integers")),
        }
    }
    
    fn rshift_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b < 0 {
                    Err(anyhow::anyhow!("Negative shift count"))
                } else {
                    Ok(Value::Int(a >> b))
                }
            }
            _ => Err(anyhow::anyhow!("Right shift requires integers")),
        }
    }
    
    fn in_values(&self, left: Value, right: Value) -> Result<Value> {
        match right {
            Value::List(items) => {
                for item in items {
                    if self.values_equal(left.clone(), item) {
                        return Ok(Value::Bool(true));
                    }
                }
                Ok(Value::Bool(false))
            }
            Value::Str(s) => {
                match left {
                    Value::Str(substr) => Ok(Value::Bool(s.contains(&substr))),
                    _ => Err(anyhow::anyhow!("'in' requires string for string search")),
                }
            }
            Value::Set(items) => {
                for item in items {
                    if self.values_equal(left.clone(), item) {
                        return Ok(Value::Bool(true));
                    }
                }
                Ok(Value::Bool(false))
            }
            Value::Dict(dict) => {
                match left {
                    Value::Str(key) => Ok(Value::Bool(dict.contains_key(&key))),
                    _ => Err(anyhow::anyhow!("Dictionary 'in' requires string key")),
                }
            }
            _ => Err(anyhow::anyhow!("'in' operator not supported for this type")),
        }
    }
    
    fn check_binary_op_types(&self, left: &Value, op: &BinaryOp, right: &Value) -> Result<()> {
        match op {
            BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                // Allow custom objects with dunder methods
                if matches!(left, Value::Object { .. }) || matches!(right, Value::Object { .. }) {
                    return Ok(());
                }
                
                if !matches!((left, right), 
                    (Value::Int(_), Value::Int(_)) |
                    (Value::Float(_), Value::Float(_)) |
                    (Value::Int(_), Value::Float(_)) |
                    (Value::Float(_), Value::Int(_)) |
                    (Value::Str(_), Value::Str(_)) |
                    (Value::Str(_), Value::Int(_)) |
                    (Value::Str(_), Value::Float(_)) |
                    (Value::List(_), Value::List(_))
                ) {
                    return Err(anyhow::anyhow!("Invalid types for arithmetic operation: {} and {}", 
                        left.type_name(), right.type_name()));
                }
            }
            BinaryOp::Eq | BinaryOp::Neq | BinaryOp::Gt | BinaryOp::Lt | BinaryOp::Gte | BinaryOp::Lte => {
                // Allow custom objects with dunder methods
                if matches!(left, Value::Object { .. }) || matches!(right, Value::Object { .. }) {
                    return Ok(());
                }
                
                if !matches!((left, right), 
                    (Value::Int(_), Value::Int(_)) |
                    (Value::Float(_), Value::Float(_)) |
                    (Value::Int(_), Value::Float(_)) |
                    (Value::Float(_), Value::Int(_)) |
                    (Value::Bool(_), Value::Bool(_)) |
                    (Value::Str(_), Value::Str(_)) |
                    (Value::None, Value::None)
                ) {
                    return Err(anyhow::anyhow!("Invalid types for comparison: {} and {}", 
                        left.type_name(), right.type_name()));
                }
            }
            BinaryOp::And | BinaryOp::Or => {
                if !matches!((left, right), (Value::Bool(_), Value::Bool(_))) {
                    return Err(anyhow::anyhow!("Logical operations require boolean operands"));
                }
            }
            _ => {}
        }
        Ok(())
    }
    
    // --- Built-in Functions ---
    
    fn init_builtins(&mut self) {
        let builtins: Vec<(&str, fn(Vec<Value>) -> Result<Value>)> = vec![
            ("print", Self::builtin_print),
            ("len", Self::builtin_len),
            ("type", Self::builtin_type),
            ("str", Self::builtin_str),
            ("int", Self::builtin_int),
            ("float", Self::builtin_float),
            ("complex", crate::builtins::builtin_complex),
            ("bool", Self::builtin_bool),
            ("range", Self::builtin_range),
            ("list", Self::builtin_list),
            ("tuple", crate::builtins::builtin_tuple),
            ("set", crate::builtins::builtin_set),
            ("dict", crate::builtins::builtin_dict),
            ("sum", Self::builtin_sum),
            ("dir", crate::builtins::builtin_dir),
            ("help", crate::builtins::builtin_help),
            ("open", crate::builtins::builtin_open),
            ("repr", crate::builtins::builtin_repr),
            ("super", |_args| Ok(Value::None)), // Placeholder - actual implementation handled in execute_function_call
            
            // Math functions
            ("abs", Self::builtin_abs),
            ("min", crate::builtins::builtin_min),
            ("max", crate::builtins::builtin_max),
            ("round", crate::builtins::builtin_round),
            ("pow", crate::builtins::builtin_pow),
            ("divmod", crate::builtins::builtin_divmod),
            
            // String functions
            ("chr", Self::builtin_chr),
            ("ord", crate::builtins::builtin_ord),
            ("hex", crate::builtins::builtin_hex),
            ("oct", crate::builtins::builtin_oct),
            ("bin", crate::builtins::builtin_bin),
            ("ascii", crate::builtins::builtin_ascii),
            
            // Object functions
            ("hasattr", crate::builtins::builtin_hasattr),
            ("getattr", crate::builtins::builtin_getattr),
            ("setattr", crate::builtins::builtin_setattr),
            ("delattr", crate::builtins::builtin_delattr),
            ("isinstance", Self::builtin_isinstance),
            ("issubclass", crate::builtins::builtin_issubclass),
            
            // Collection functions
            ("enumerate", crate::builtins::builtin_enumerate),
            ("zip", crate::builtins::builtin_zip),
            ("sorted", crate::builtins::builtin_sorted),
            ("reversed", crate::builtins::builtin_reversed),
            ("filter", crate::builtins::builtin_filter),
            ("map", crate::builtins::builtin_map),
            
            // Utility functions
            ("id", crate::builtins::builtin_id),
            ("hash", crate::builtins::builtin_hash),
            ("callable", crate::builtins::builtin_callable),
            ("format", crate::builtins::builtin_format),
            
            // Advanced collection functions
            ("all", crate::builtins::builtin_all),
            ("any", crate::builtins::builtin_any),
            
            // Collection creation functions
            ("frozenset", crate::builtins::builtin_frozenset),
            ("bytearray", crate::builtins::builtin_bytearray),
            ("bytes", crate::builtins::builtin_bytes),
            
            // Advanced functions
            ("iter", crate::builtins::builtin_iter),
            ("next", crate::builtins::builtin_next),
            ("slice", crate::builtins::builtin_slice),
            ("vars", crate::builtins::builtin_vars),
            ("globals", crate::builtins::builtin_globals),
            ("locals", crate::builtins::builtin_locals),
            ("eval", |_args| Ok(Value::None)), // Placeholder - actual implementation handled in execute_function_call
            ("exec", |_args| Ok(Value::None)), // Placeholder - actual implementation handled in execute_function_call
            ("compile", crate::builtins::builtin_compile),
            ("breakpoint", crate::builtins::builtin_breakpoint),
            ("input", crate::builtins::builtin_input),
            ("load_library", crate::builtins::builtin_load_library),
            
            // Special values
            ("Ellipsis", |_args| Ok(Value::Ellipsis)),
            ("NotImplemented", |_args| Ok(Value::NotImplemented)),
        ];
        
        for (name, func) in builtins {
            self.set_variable(name, Value::BuiltinFunction(name.to_string(), func));
        }
        
        // Initialize built-in modules (but don't auto-import them)
        // Built-in modules are only available when explicitly imported
        // self.module_system already handles built-in module creation on-demand
    }
    
    /// Initialize package system by integrating PackageManager with ModuleSystem
    fn init_package_system(&mut self) {
        // Initialize package directories
        if let Err(e) = self.package_manager.init() {
            eprintln!("Warning: Failed to initialize package directories: {}", e);
        }
        
        // Add package search paths to module system
        let package_paths = self.package_manager.get_search_paths();
        for path in package_paths {
            self.module_system.add_search_path(path);
        }
    }
    
    fn builtin_print(args: Vec<Value>) -> Result<Value> {
        for (i, arg) in args.iter().enumerate() {
            if i > 0 { print!(" "); }
            match arg {
                Value::Str(s) => print!("{}", s), // Print strings without quotes
                _ => print!("{}", arg),
            }
        }
        println!();
        Ok(Value::None)
    }
    
    fn builtin_len(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("len() takes exactly one argument"));
        }
        
        // Try calling __len__ dunder method first
        if let Some(result) = call_dunder_method(&args[0], "__len__", vec![]) {
            if let Value::Int(n) = result {
                return Ok(Value::Int(n));
            }
        }
        
        // Fallback to original implementation
        match &args[0] {
            Value::Str(s) => Ok(Value::Int(s.len() as i64)),
            Value::List(items) => Ok(Value::Int(items.len() as i64)),
            Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
            _ => Err(anyhow::anyhow!("object of type '{}' has no len()", args[0].type_name())),
        }
    }
    
    fn builtin_type(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("type() takes exactly one argument"));
        }
        
        Ok(Value::Str(args[0].type_name().to_string()))
    }
    
    fn builtin_str(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("str() takes exactly one argument"));
        }
        
        // Note: This is the static version without VM context
        // For VM-aware dunder method calls, use builtin_str_with_vm
        if let Some(result) = call_dunder_method(&args[0], "__str__", vec![]) {
            if let Value::Str(s) = result {
                return Ok(Value::Str(s));
            }
        }
        
        // Fallback to original implementation
        let string_repr = match &args[0] {
            Value::Str(s) => s.clone(), // Don't add quotes for str() conversion
            _ => format!("{}", args[0]), // Use Display trait
        };
        Ok(Value::Str(string_repr))
    }
    
    /// VM-aware version of str() that can call user-defined __str__ methods
    fn builtin_str_with_vm(&mut self, args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("str() takes exactly one argument"));
        }
        
        // Try to call __str__ dunder method with VM context first
        let value = &args[0];
        if let Value::Object { class_name, .. } = value {
            // Look up the class definition in the VM
            if let Some(Value::Object { fields: class_methods, .. }) = self.get_variable(class_name) {
                if let Some(method_value) = class_methods.get("__str__") {
                    match method_value {
                        Value::Closure { name: method_name, params, body, captured_scope, .. } => {
                            // Call the user-defined dunder method
                            let method_args = vec![value.clone()];
                            
                            match self.call_user_function(method_name, params, body.clone(), method_args, HashMap::new(), Some(captured_scope.clone())) {
                                Ok(result) => {
                                    if let Value::Str(s) = result {
                                        return Ok(Value::Str(s));
                                    }
                                }
                                Err(_) => {} // Fall through to default implementation
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        
        // Fallback to original implementation
        let string_repr = match &args[0] {
            Value::Str(s) => s.clone(), // Don't add quotes for str() conversion
            _ => format!("{}", args[0]), // Use Display trait
        };
        Ok(Value::Str(string_repr))
    }
    
    /// VM-aware version of repr() that can call user-defined __repr__ methods
    fn builtin_repr_with_vm(&mut self, args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("repr() takes exactly one argument"));
        }
        
        // Try to call __repr__ dunder method with VM context first
        let value = &args[0];
        if let Value::Object { class_name, .. } = value {
            // Look up the class definition in the VM
            if let Some(Value::Object { fields: class_methods, .. }) = self.get_variable(class_name) {
                if let Some(method_value) = class_methods.get("__repr__") {
                    match method_value {
                        Value::Closure { name: method_name, params, body, captured_scope, .. } => {
                            // Call the user-defined dunder method
                            let method_args = vec![value.clone()];
                            
                            match self.call_user_function(method_name, params, body.clone(), method_args, HashMap::new(), Some(captured_scope.clone())) {
                                Ok(result) => {
                                    if let Value::Str(s) = result {
                                        return Ok(Value::Str(s));
                                    }
                                }
                                Err(_) => {} // Fall through to default implementation
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        
        // Fallback to original implementation
        let repr_str = match &args[0] {
            Value::Str(s) => format!("'{}'", s),
            _ => format!("{}", args[0]),
        };
        Ok(Value::Str(repr_str))
    }
    
    /// VM-aware version of len() that can call user-defined __len__ methods
    fn builtin_len_with_vm(&mut self, args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("len() takes exactly one argument"));
        }
        
        // Try to call __len__ dunder method with VM context first
        let value = &args[0];
        if let Value::Object { class_name, .. } = value {
            // Look up the class definition in the VM
            if let Some(Value::Object { fields: class_methods, .. }) = self.get_variable(class_name) {
                if let Some(method_value) = class_methods.get("__len__") {
                    match method_value {
                        Value::Closure { name: method_name, params, body, captured_scope, .. } => {
                            // Call the user-defined dunder method
                            let method_args = vec![value.clone()];
                            
                            match self.call_user_function(method_name, params, body.clone(), method_args, HashMap::new(), Some(captured_scope.clone())) {
                                Ok(result) => return Ok(result),
                                Err(_) => {} // Fall through to default implementation
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        
        // Fallback to original implementation
        match &args[0] {
            Value::Str(s) => Ok(Value::Int(s.len() as i64)),
            Value::List(items) => Ok(Value::Int(items.len() as i64)),
            Value::Tuple(items) => Ok(Value::Int(items.len() as i64)),
            Value::Set(items) => Ok(Value::Int(items.len() as i64)),
            Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
            _ => Err(anyhow::anyhow!("object of type '{}' has no len()", args[0].type_name())),
        }
    }
     
     fn builtin_int(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("int() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(*n)),
            Value::Float(n) => Ok(Value::Int(*n as i64)),
            Value::Str(s) => {
                if let Ok(n) = s.parse::<i64>() {
                    Ok(Value::Int(n))
                } else {
                    Err(anyhow::anyhow!("invalid literal for int(): '{}'", s))
                }
            }
            Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
            _ => Err(anyhow::anyhow!("cannot convert '{}' to int", args[0].type_name())),
        }
    }
    
    fn builtin_float(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("float() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Float(*n as f64)),
            Value::Float(n) => Ok(Value::Float(*n)),
            Value::Str(s) => {
                if let Ok(n) = s.parse::<f64>() {
                    Ok(Value::Float(n))
                } else {
                    Err(anyhow::anyhow!("invalid literal for float(): '{}'", s))
                }
            }
            Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
            _ => Err(anyhow::anyhow!("cannot convert '{}' to float", args[0].type_name())),
        }
    }
    
    fn builtin_bool(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("bool() takes exactly one argument"));
        }
        
        Ok(Value::Bool(args[0].is_truthy()))
    }
    
    fn builtin_range(args: Vec<Value>) -> Result<Value> {
        crate::builtins::builtin_range(args)
    }
    
    fn builtin_list(args: Vec<Value>) -> Result<Value> {
        match args.len() {
            0 => Ok(Value::List(Vec::new())),
            1 => {
                match &args[0] {
                    Value::List(items) => Ok(Value::List(items.clone())),
                    Value::Str(s) => {
                        let chars: Vec<Value> = s.chars()
                            .map(|c| Value::Str(c.to_string()))
                            .collect();
                        Ok(Value::List(chars))
                    }
                    _ => Ok(Value::List(vec![args[0].clone()])),
                }
            }
            _ => Ok(Value::List(args)),
        }
    }
    
    fn builtin_sum(args: Vec<Value>) -> Result<Value> {
        if args.is_empty() {
            return Err(anyhow::anyhow!("sum expected at least 1 argument, got 0"));
        }
        
        let start = if args.len() > 1 { &args[1] } else { &Value::Int(0) };
        let mut result = start.clone();
        
        let iterable = &args[0];
        let items = match iterable {
            Value::List(items) => items,
            _ => return Err(anyhow::anyhow!("sum() can't sum {}", iterable.type_name())),
        };
        
        for item in items {
            match (&result, item) {
                (Value::Int(a), Value::Int(b)) => result = Value::Int(a + b),
                (Value::Float(a), Value::Float(b)) => result = Value::Float(a + b),
                (Value::Int(a), Value::Float(b)) => result = Value::Float(*a as f64 + b),
                (Value::Float(a), Value::Int(b)) => result = Value::Float(a + *b as f64),
                _ => return Err(anyhow::anyhow!("unsupported operand type(s) for +: '{}' and '{}'", result.type_name(), item.type_name())),
            }
        }
        
        Ok(result)
    }

    fn builtin_abs(args: Vec<Value>) -> Result<Value> {
        crate::builtins::builtin_abs(args)
    }

    fn builtin_chr(args: Vec<Value>) -> Result<Value> {
        crate::builtins::builtin_chr(args)
    }

    fn builtin_isinstance(args: Vec<Value>) -> Result<Value> {
        crate::builtins::builtin_isinstance(args)
    }
    
    /// Execute slice operation
    fn execute_slice(&mut self, object: &Expr, start: Option<&Expr>, stop: Option<&Expr>, step: Option<&Expr>) -> Result<Value> {
        let obj_value = self.execute_expression(object)?;
        
        // Evaluate slice parameters
        let start_val = if let Some(start_expr) = start {
            match self.execute_expression(start_expr)? {
                Value::Int(n) => Some(n),
                Value::None => None,
                _ => return Err(anyhow::anyhow!("slice indices must be integers or None")),
            }
        } else {
            None
        };
        
        let stop_val = if let Some(stop_expr) = stop {
            match self.execute_expression(stop_expr)? {
                Value::Int(n) => Some(n),
                Value::None => None,
                _ => return Err(anyhow::anyhow!("slice indices must be integers or None")),
            }
        } else {
            None
        };
        
        let step_val = if let Some(step_expr) = step {
            match self.execute_expression(step_expr)? {
                Value::Int(n) => {
                    if n == 0 {
                        return Err(anyhow::anyhow!("slice step cannot be zero"));
                    }
                    n
                }
                Value::None => 1,
                _ => return Err(anyhow::anyhow!("slice step must be an integer or None")),
            }
        } else {
            1
        };
        
        match obj_value {
            Value::List(list) => {
                let len = list.len() as i64;
                let (start_idx, stop_idx) = self.normalize_slice_indices(start_val, stop_val, step_val, len);
                
                let mut result = Vec::new();
                if step_val > 0 {
                    let mut i = start_idx;
                    while i < stop_idx && i < len {
                        if i >= 0 {
                            result.push(list[i as usize].clone());
                        }
                        i += step_val;
                    }
                } else {
                    let mut i = start_idx;
                    while i > stop_idx && i >= 0 {
                        if i < len {
                            result.push(list[i as usize].clone());
                        }
                        i += step_val; // step_val is negative
                    }
                }
                
                Ok(Value::List(result))
            }
            Value::Str(s) => {
                let chars: Vec<char> = s.chars().collect();
                let len = chars.len() as i64;
                let (start_idx, stop_idx) = self.normalize_slice_indices(start_val, stop_val, step_val, len);
                
                let mut result = String::new();
                if step_val > 0 {
                    let mut i = start_idx;
                    while i < stop_idx && i < len {
                        if i >= 0 {
                            result.push(chars[i as usize]);
                        }
                        i += step_val;
                    }
                } else {
                    let mut i = start_idx;
                    while i > stop_idx && i >= 0 {
                        if i < len {
                            result.push(chars[i as usize]);
                        }
                        i += step_val; // step_val is negative
                    }
                }
                
                Ok(Value::Str(result))
            }
            Value::Tuple(tuple) => {
                let len = tuple.len() as i64;
                let (start_idx, stop_idx) = self.normalize_slice_indices(start_val, stop_val, step_val, len);
                
                let mut result = Vec::new();
                if step_val > 0 {
                    let mut i = start_idx;
                    while i < stop_idx && i < len {
                        if i >= 0 {
                            result.push(tuple[i as usize].clone());
                        }
                        i += step_val;
                    }
                } else {
                    let mut i = start_idx;
                    while i > stop_idx && i >= 0 {
                        if i < len {
                            result.push(tuple[i as usize].clone());
                        }
                        i += step_val; // step_val is negative
                    }
                }
                
                Ok(Value::Tuple(result))
            }
            _ => Err(anyhow::anyhow!("'{}' object is not subscriptable", obj_value.type_name())),
        }
    }
    
    /// Normalize slice indices according to Python semantics
    fn normalize_slice_indices(&self, start: Option<i64>, stop: Option<i64>, step: i64, length: i64) -> (i64, i64) {
        let (default_start, default_stop) = if step > 0 {
            (0, length)
        } else {
            (length - 1, -1)
        };
        
        let start_idx = match start {
            Some(idx) => {
                if idx < 0 {
                    (length + idx).max(if step > 0 { 0 } else { -1 })
                } else {
                    idx.min(length - 1).max(if step > 0 { 0 } else { -1 })
                }
            }
            None => default_start,
        };
        
        let stop_idx = match stop {
            Some(idx) => {
                if idx < 0 {
                    (length + idx).max(if step > 0 { -1 } else { -1 })
                } else {
                    idx.min(length).max(if step > 0 { -1 } else { -1 })
                }
            }
            None => default_stop,
        };
        
        (start_idx, stop_idx)
    }
}

/// Run a TauraroLang file
pub fn run_file(source: &str, backend: &str, optimization: u8) -> Result<()> {
    run_file_with_options(source, backend, optimization, false)
}

pub fn run_file_with_options(source: &str, backend: &str, optimization: u8, strict_types: bool) -> Result<()> {
    match backend {
        "vm" => {
            let mut vm = VM::new();
            vm.strict_types = strict_types;
            match vm.execute_script(source, vec![]) {
                Ok(_) => {
                    // For scripts, don't print the result (like Python)
                    Ok(())
                }
                Err(e) => {
                    eprintln!("Error executing script: {}", e);
                    Err(e)
                }
            }
        }
        _ => {
            Err(anyhow::anyhow!("Backend '{}' not supported for running files", backend))
        }
    }
}
