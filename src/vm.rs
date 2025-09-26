//! COMPLETE TauraroLang Virtual Machine with dynamic typing and REPL support
use crate::runtime::{MemoryAPI, ManagedPtr, MemoryMode};
use crate::ast::*;
use crate::lexer::Lexer;
use crate::parser::Parser;
use crate::ast::*;
use crate::value::Value;
use crate::object_system::{call_dunder_method, resolve_dunder_method};
use crate::semantic;
use crate::modules;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;
use std::cell::RefCell;
use std::fmt;

/// Variable scope
#[derive(Debug, Clone)]
pub struct Scope {
    pub variables: HashMap<String, Value>,
    pub parent: Option<usize>,
    pub scope_type: String, // "global", "function", "class", "block"
}

impl Scope {
    pub fn new() -> Self {
        Self {
            variables: HashMap::new(),
            parent: None,
            scope_type: "module".to_string(),
        }
    }
}

/// Virtual Machine state
pub struct VM {
    scopes: Vec<Scope>,
    current_scope: usize,
    memory: MemoryAPI,
    call_stack: Vec<StackFrame>,
    strict_types: bool,
    should_return: bool,
    return_value: Option<Value>,
}

/// Stack frame for function calls
#[derive(Debug)]
struct StackFrame {
    function_name: String,
    return_address: usize,
    scope_index: usize,
}

impl VM {
    pub fn new() -> Self {
        let global_scope = Scope {
            variables: HashMap::new(),
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
        };
        
        // Initialize built-in functions
        vm.init_builtins();
        vm
    }
    
    /// Set strict type checking mode
    pub fn set_strict_types(&mut self, strict: bool) {
        self.strict_types = strict;
    }
    
    /// Execute TauraroLang source code as a script
    pub fn execute_script(&mut self, source: &str, args: Vec<String>) -> Result<Value> {
        // Parse with implicit main function support
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
        let mut parser = Parser::new(tokens);
        
        // Try to parse as a single expression first (for simple scripts like "42")
        let program = if let Ok(program) = parser.parse() {
            if program.statements.is_empty() {
                // Empty program, create a simple one that returns None
                Program {
                    statements: vec![Statement::Expression(Expr::Literal(Literal::None))],
                }
            } else if program.statements.len() == 1 {
                // Single statement - check if it's an expression or variable definition
                match &program.statements[0] {
                    Statement::Expression(_) | Statement::VariableDef { .. } => {
                        program
                    },
                    _ => {
                        // Reset parser and try with implicit main
                        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
                            .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
                        let mut parser = Parser::new(tokens);
                        parser.parse_with_implicit_main()?
                    }
                }
            } else {
                // Multiple statements - use implicit main
                let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
                    .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
                let mut parser = Parser::new(tokens);
                parser.parse_with_implicit_main()?
            }
        } else {
            // Parsing failed, try with implicit main
            let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
                .map_err(|e| anyhow::anyhow!("Lexer error: {}", e))?;
            let mut parser = Parser::new(tokens);
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
        self.set_variable("args", Value::List(args.into_iter().map(Value::String).collect()));
        
        // Execute the program
        self.execute_program(program)
    }
    
    /// Execute a complete program
    fn execute_program(&mut self, program: Program) -> Result<Value> {
        // First pass: register all functions and classes
        for stmt in &program.statements {
            if let Statement::FunctionDef { name, params, return_type: _, body, is_async: _, decorators } = stmt {
                let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                let mut func_value = Value::Function(name.clone(), param_names, body.clone());
                
                // Apply decorators in reverse order (bottom to top)
                for decorator in decorators.iter().rev() {
                    func_value = self.apply_decorator(decorator, func_value)?;
                }
                
                self.set_variable(name, func_value);
            } else if let Statement::ClassDef { name, bases: _, body: _, decorators, metaclass: _ } = stmt {
                // Create class object
                let mut class_obj = Value::Object(name.clone(), HashMap::new());
                
                // Apply decorators to class
                for decorator in decorators.iter().rev() {
                    class_obj = self.apply_decorator(decorator, class_obj)?;
                }
                
                self.set_variable(name, class_obj);
            }
        }
        
        // Second pass: execute statements
        let mut last_expression_value = Value::None;
        for stmt in program.statements {
            if !matches!(stmt, Statement::FunctionDef { .. }) {
                if let Ok(Some(value)) = self.execute_statement(&stmt) {
                    // Capture the value from expressions for potential return
                    last_expression_value = value;
                }
                
                if self.should_return {
                    return Ok(self.return_value.take().unwrap_or(Value::None));
                }
            }
        }
        
        // Call main function if it exists
        if let Some(Value::Function(_, _, body)) = self.get_variable("main") {
            self.enter_scope("function");
            for stmt in body {
                self.execute_statement(&stmt)?;
                if self.should_return {
                    break;
                }
            }
            let result = self.return_value.take().unwrap_or(Value::None);
            self.exit_scope();
            return Ok(result);
        }
        
        // Return the last expression value if no main function exists
        Ok(last_expression_value)
    }
    
    /// Apply a decorator to a function or class
    fn apply_decorator(&mut self, decorator: &Expr, target: Value) -> Result<Value> {
        match decorator {
            // Simple decorator (just a name)
            Expr::Identifier(decorator_name) => {
                if let Some(decorator_func) = self.get_variable(decorator_name) {
                    match decorator_func {
                        Value::Function(name, params, body) => {
                            // Call the user-defined decorator function with the target as argument
                            let args = vec![target];
                            self.call_user_function(&name, &params, body, args)
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
                let decorator_result = self.execute_function_call(func, args)?;
                
                // Then apply the result to the target
                match decorator_result {
                    Value::Function(name, params, body) => {
                        // The decorator returned a function, call it with the target
                        let args = vec![target];
                        self.call_user_function(&name, &params, body, args)
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
                    Value::Function(name, params, body) => {
                        let args = vec![target];
                        self.call_user_function(&name, &params, body, args)
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
    pub fn execute_statement(&mut self, stmt: &Statement) -> Result<Option<Value>> {
        match stmt {
            Statement::Expression(expr) => {
                let value = self.execute_expression(expr)?;
                Ok(Some(value))
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let val = self.execute_expression(expr)?;
                    self.set_variable(name, val);
                } else {
                    self.set_variable(name, Value::None);
                }
                Ok(None)
            }
            Statement::FunctionDef { name: _, params: _, return_type: _, body: _, is_async: _, decorators: _ } => {
                // Function definition already handled in first pass
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
            Statement::ClassDef { name, bases: _, body, decorators: _, metaclass: _ } => {
                // Handle class definition in execute_statement
                let mut class_methods = HashMap::new();
                
                println!("Processing class '{}' with {} statements in body", name, body.len());
                
                // Process class body to extract methods - execute each statement in the class body
                for (i, stmt) in body.iter().enumerate() {
                    println!("Processing statement {} in class body: {:?}", i, stmt);
                    match stmt {
                        Statement::FunctionDef { name: method_name, params, return_type: _, body: method_body, is_async: _, decorators: _ } => {
                            let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                            let method_value = Value::Function(method_name.clone(), param_names, method_body.clone());
                            class_methods.insert(method_name.clone(), method_value);
                            println!("Added method '{}' to class '{}'", method_name, name);
                        }
                        _ => {
                            // Execute other statements in class body (like variable assignments)
                            println!("Executing non-function statement in class body");
                            self.execute_statement(stmt)?;
                        }
                    }
                }
                
                println!("Class '{}' created with {} methods", name, class_methods.len());
                for method_name in class_methods.keys() {
                    println!("  Method: {}", method_name);
                }
                let class_obj = Value::Object(name.clone(), class_methods);
                self.set_variable(name, class_obj);
                Ok(None)
            }
            Statement::AttributeAssignment { object, name, value } => {
                // Handle attribute assignment like self.x = value
                let val = self.execute_expression(value)?;
                
                // Get the object identifier to modify it in place
                if let Expr::Identifier(obj_name) = object {
                    if let Some(obj_value) = self.get_variable(obj_name) {
                        match obj_value {
                            Value::Object(class_name, mut fields) => {
                                // Store the attribute in the object's fields
                                fields.insert(name.clone(), val);
                                let updated_obj = Value::Object(class_name, fields);
                                self.set_variable(obj_name, updated_obj);
                                Ok(None)
                            }
                            _ => {
                                Err(anyhow::anyhow!("Cannot assign attribute '{}' to non-object", name))
                            }
                        }
                    } else {
                        Err(anyhow::anyhow!("Undefined variable: {}", obj_name))
                    }
                } else {
                    // For complex expressions, we can't modify in place yet
                    Err(anyhow::anyhow!("Complex attribute assignment not yet supported"))
                }
            }
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
            Expr::Literal(Literal::String(s)) => Ok(Value::String(s.clone())),
            Expr::Literal(Literal::Bool(b)) => Ok(Value::Bool(*b)),
            Expr::Literal(Literal::None) => Ok(Value::None),
            Expr::Literal(Literal::Bytes(bytes)) => Ok(Value::Bytes(bytes.clone())),
            Expr::Literal(Literal::Complex { real, imag }) => {
                // For now, return a tuple representing the complex number
                Ok(Value::Tuple(vec![Value::Float(*real), Value::Float(*imag)]))
            },
            Expr::Literal(Literal::Ellipsis) => {
                // Return a special marker for ellipsis
                Ok(Value::String("...".to_string()))
            },
            Expr::Identifier(name) => {
                self.get_variable(name).ok_or_else(|| anyhow::anyhow!("Undefined variable: {}", name))
            }
            Expr::BinaryOp { left, op, right } => {
                self.execute_binary_op(left, op, right)
            }
            Expr::UnaryOp { op, operand } => {
                self.execute_unary_op(op, operand)
            }
            Expr::Call { func, args, .. } => {
                self.execute_function_call(func, args)
            }
            Expr::MethodCall { object, method, args, kwargs: _ } => {
                self.execute_method_call(object.as_ref(), method, args)
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
                        Value::String(s) => s.clone(),
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
                    Value::Object(_, fields) => {
                        if let Some(value) = fields.get(name) {
                            Ok(value.clone())
                        } else {
                            Err(anyhow::anyhow!("'{}' object has no attribute '{}'", "object", name))
                        }
                    }
                    Value::Module(_, namespace) => {
                        if let Some(value) = namespace.get(name) {
                            Ok(value.clone())
                        } else {
                            Err(anyhow::anyhow!("'module' object has no method '{}'", name))
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
                            Value::String(s) => s.clone(),
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
                    (Value::String(s), Value::Int(index)) => {
                        let chars: Vec<char> = s.chars().collect();
                        let idx = if *index < 0 {
                            (chars.len() as i64 + index) as usize
                        } else {
                            *index as usize
                        };
                        
                        if idx < chars.len() {
                            Ok(Value::String(chars[idx].to_string()))
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
                let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                Ok(Value::Function("lambda".to_string(), param_names, vec![Statement::Return(Some((**body).clone()))]))
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
                        self.set_variable(name, evaluated_value.clone());
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
                Ok(Value::String(result))
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
    fn execute_function_call(&mut self, callee: &Expr, arguments: &[Expr]) -> Result<Value> {
        let callee_val = self.execute_expression(callee)?;
        let arg_values: Result<Vec<Value>> = arguments
            .iter()
            .map(|arg| self.execute_expression(arg))
            .collect();
        let arg_values = arg_values?;
        
        match callee_val {
            Value::Function(name, params, body) => {
                self.call_user_function(&name, &params, body, arg_values)
            }
            Value::BuiltinFunction(_, func) => {
                func(arg_values)
            }
            Value::NativeFunction(func) => {
                func(arg_values)
            }
            Value::Object(class_name, class_methods) => {
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
            (Value::String(a), Value::String(b)) => a == b,
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
                    Value::String(s) => s.clone(),
                    Value::Int(n) => n.to_string(),
                    Value::Float(n) => format!("{:.6}", n),
                    Value::Bool(b) => b.to_string(),
                    Value::None => "None".to_string(),
                    _ => format!("{}", value),
                };
                Ok(dict.contains_key(&key_string))
            }
            Value::String(s) => {
                match value {
                    Value::String(substr) => Ok(s.contains(substr)),
                    _ => Err(anyhow::anyhow!("'in' requires string on both sides for string containment")),
                }
            }
            _ => Err(anyhow::anyhow!("'{}' object is not iterable", container.type_name())),
        }
    }
    
    /// Execute method call
    fn execute_method_call(&mut self, object: &Expr, method: &str, args: &[Expr]) -> Result<Value> {
        let obj_value = self.execute_expression(object)?;
        let arg_values: Result<Vec<Value>> = args
            .iter()
            .map(|arg| self.execute_expression(arg))
            .collect();
        let arg_values = arg_values?;
        
        match &obj_value {
            Value::Object(class_name, class_methods) => {
                // Look for the method in the class
                if let Some(Value::Function(method_name, params, body)) = class_methods.get(method) {
                    // Create a new argument list with 'self' as the first argument
                    let mut method_args = vec![obj_value.clone()];
                    method_args.extend(arg_values);
                    
                    self.call_user_function(method_name, params, body.clone(), method_args)
                } else {
                    Err(anyhow::anyhow!("'{}' object has no method '{}'", class_name, method))
                }
            }
            Value::String(s) => {
                // Built-in string methods
                match method {
                    "upper" => Ok(Value::String(s.to_uppercase())),
                    "lower" => Ok(Value::String(s.to_lowercase())),
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
                        Ok(Value::String(result))
                    }
                    "capitalize" => {
                        let mut chars = s.chars();
                        match chars.next() {
                            None => Ok(Value::String(String::new())),
                            Some(first) => {
                                let capitalized = first.to_uppercase().collect::<String>() + &chars.as_str().to_lowercase();
                                Ok(Value::String(capitalized))
                            }
                        }
                    }
                    "strip" => Ok(Value::String(s.trim().to_string())),
                    "replace" => {
                        if arg_values.len() != 2 {
                            return Err(anyhow::anyhow!("replace() takes exactly 2 arguments"));
                        }
                        if let (Value::String(old), Value::String(new)) = (&arg_values[0], &arg_values[1]) {
                            Ok(Value::String(s.replace(old, new)))
                        } else {
                            Err(anyhow::anyhow!("replace() arguments must be strings"))
                        }
                    }
                    "split" => {
                        let delimiter = if arg_values.is_empty() {
                            None
                        } else if let Value::String(delim) = &arg_values[0] {
                            Some(delim.as_str())
                        } else {
                            return Err(anyhow::anyhow!("split() delimiter must be a string"));
                        };
                        
                        let parts: Vec<Value> = if let Some(delim) = delimiter {
                            s.split(delim).map(|part| Value::String(part.to_string())).collect()
                        } else {
                            s.split_whitespace().map(|part| Value::String(part.to_string())).collect()
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
                                    Value::String(s) => Ok(s.clone()),
                                    _ => Err(anyhow::anyhow!("join() argument must be a list of strings"))
                                }
                            }).collect();
                            Ok(Value::String(strings?.join(s)))
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
                    }
                    _ => Err(anyhow::anyhow!("'list' object has no method '{}'", method)),
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
             Value::String(s) => s.chars().map(|c| Value::String(c.to_string())).collect(),
             _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
         };
         
         for item in items {
             self.enter_scope("comprehension");
             self.set_variable(&gen.target, item);
             
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
                 Value::String(s) => s.clone(),
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
             Value::String(s) => s.chars().map(|c| Value::String(c.to_string())).collect(),
             _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
         };
         
         for item in items {
             self.enter_scope("comprehension");
             self.set_variable(&gen.target, item);
             
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
        self.set_variable(target, value);
        Ok(())
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
            Value::String(s) => s.chars().map(|c| Value::String(c.to_string())).collect(),
            _ => return Err(anyhow::anyhow!("'{}' object is not iterable", iter_value.type_name())),
        };
        
        for item in items {
            self.enter_scope("block");
            self.set_variable(variable, item);
            
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
        let name = alias.unwrap_or(module);
        
        // For now, create a mock module object
        let module_obj = Value::Object(module.to_string(), HashMap::new());
        self.set_variable(name, module_obj);
        
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
        // Create new instance with empty fields
        let mut instance_fields = HashMap::new();
        
        // Look for __init__ method in class methods
        if let Some(init_method) = class_methods.get("__init__") {
            match init_method {
                Value::Function(_, params, body) => {
                    // Create a temporary instance to pass as 'self'
                    let temp_instance = Value::Object(class_name.to_string(), instance_fields.clone());
                    
                    // Store the temporary instance in a variable so __init__ can modify it
                    let temp_var_name = format!("__temp_self_{}", class_name);
                    self.set_variable(&temp_var_name, temp_instance);
                    
                    // Prepare arguments: self + provided args
                    let mut init_args = vec![Value::Object(class_name.to_string(), instance_fields.clone())];
                    init_args.extend(args);
                    
                    // Check parameter count (including self)
                    if params.len() != init_args.len() {
                        return Err(anyhow::anyhow!("__init__() takes {} arguments but {} were given", 
                            params.len(), init_args.len()));
                    }
                    
                    // Call __init__ method
                    let modified_instance = self.call_user_function("__init__", params, body.clone(), init_args)?;
                    
                    // Return the modified instance
                    Ok(modified_instance)
                }
                _ => Err(anyhow::anyhow!("__init__ is not a function")),
            }
        } else {
            // No __init__ method, just create empty instance
            if !args.is_empty() {
                return Err(anyhow::anyhow!("{}() takes no arguments but {} were given", 
                    class_name, args.len()));
            }
            Ok(Value::Object(class_name.to_string(), instance_fields))
        }
    }

    /// Call user-defined function
    fn call_user_function(&mut self, name: &str, params: &[String], body: Vec<Statement>, args: Vec<Value>) -> Result<Value> {
        if params.len() != args.len() {
            return Err(anyhow::anyhow!("Function {} expects {} arguments, got {}", 
                name, params.len(), args.len()));
        }
        
        // Push new stack frame
        self.call_stack.push(StackFrame {
            function_name: name.to_string(),
            return_address: self.current_scope,
            scope_index: self.current_scope,
        });
        
        // Enter function scope
        self.enter_scope("function");
        
        // Set parameters
        for (param, arg) in params.iter().zip(args) {
            self.set_variable(param, arg);
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
            if let Some(modified_self) = self.get_variable(&params[0]) {
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
            Value::String(s) => s.clone(),
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
        Value::TypedValue {
            value: Box::new(value),
            type_info: Type::Any,
        }
    }
    
    /// Force garbage collection
    pub fn collect_garbage(&self) {
        self.memory.collect();
    }
    
    /// Get memory statistics
    pub fn memory_stats(&self) -> String {
        format!("Memory: {}", self.memory.stats())
    }
    
    // --- Scope Management ---
    
    fn enter_scope(&mut self, scope_type: &str) {
        let new_scope = Scope {
            variables: HashMap::new(),
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
    
    pub fn set_variable(&mut self, name: &str, value: Value) {
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
    
    pub fn get_global_variables(&self) -> HashMap<String, Value> {
        // Global scope is always at index 0
        self.scopes[0].variables.clone()
    }
    
    pub fn get_local_variables(&self) -> HashMap<String, Value> {
        // Current scope variables
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
            (Value::String(a), Value::String(b)) => Ok(Value::String(a + &b)),
            (Value::String(a), Value::Int(b)) => Ok(Value::String(a + &b.to_string())),
            (Value::String(a), Value::Float(b)) => Ok(Value::String(a + &b.to_string())),
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
            (Value::String(a), Value::Int(b)) => Ok(Value::String(a.repeat(b as usize))),
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
    
    fn gt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) > b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a > b as f64)),
            (Value::String(a), Value::String(b)) => Ok(Value::Bool(a > b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn lt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) < b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a < b as f64)),
            (Value::String(a), Value::String(b)) => Ok(Value::Bool(a < b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn gte_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) >= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a >= b as f64)),
            (Value::String(a), Value::String(b)) => Ok(Value::Bool(a >= b)),
            _ => Err(anyhow::anyhow!("Invalid types for comparison")),
        }
    }
    
    fn lte_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) <= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a <= b as f64)),
            (Value::String(a), Value::String(b)) => Ok(Value::Bool(a <= b)),
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
            (Value::String(a), Value::String(b)) => a == b,
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
            Value::String(s) => {
                match left {
                    Value::String(substr) => Ok(Value::Bool(s.contains(&substr))),
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
                    Value::String(key) => Ok(Value::Bool(dict.contains_key(&key))),
                    _ => Err(anyhow::anyhow!("Dictionary 'in' requires string key")),
                }
            }
            _ => Err(anyhow::anyhow!("'in' operator not supported for this type")),
        }
    }
    
    fn check_binary_op_types(&self, left: &Value, op: &BinaryOp, right: &Value) -> Result<()> {
        match op {
            BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                if !matches!((left, right), 
                    (Value::Int(_), Value::Int(_)) |
                    (Value::Float(_), Value::Float(_)) |
                    (Value::Int(_), Value::Float(_)) |
                    (Value::Float(_), Value::Int(_)) |
                    (Value::String(_), Value::String(_)) |
                    (Value::String(_), Value::Int(_)) |
                    (Value::String(_), Value::Float(_)) |
                    (Value::List(_), Value::List(_))
                ) {
                    return Err(anyhow::anyhow!("Invalid types for arithmetic operation: {} and {}", 
                        left.type_name(), right.type_name()));
                }
            }
            BinaryOp::Eq | BinaryOp::Neq | BinaryOp::Gt | BinaryOp::Lt | BinaryOp::Gte | BinaryOp::Lte => {
                if !matches!((left, right), 
                    (Value::Int(_), Value::Int(_)) |
                    (Value::Float(_), Value::Float(_)) |
                    (Value::Int(_), Value::Float(_)) |
                    (Value::Float(_), Value::Int(_)) |
                    (Value::Bool(_), Value::Bool(_)) |
                    (Value::String(_), Value::String(_)) |
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
            ("bool", Self::builtin_bool),
            ("range", Self::builtin_range),
            ("list", Self::builtin_list),
            ("sum", Self::builtin_sum),
            ("set", crate::builtins::builtin_set),
            ("dir", crate::builtins::builtin_dir),
        ];
        
        for (name, func) in builtins {
            self.set_variable(name, Value::BuiltinFunction(name.to_string(), func));
        }
        
        // Initialize built-in modules
        let builtin_modules = modules::init_builtin_modules();
        for (name, module) in builtin_modules {
            self.set_variable(&name, module);
        }
    }
    
    fn builtin_print(args: Vec<Value>) -> Result<Value> {
        for (i, arg) in args.iter().enumerate() {
            if i > 0 { print!(" "); }
            match arg {
                Value::String(s) => print!("{}", s), // Print strings without quotes
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
        
        match &args[0] {
            Value::String(s) => Ok(Value::Int(s.len() as i64)),
            Value::List(items) => Ok(Value::Int(items.len() as i64)),
            Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
            _ => Err(anyhow::anyhow!("object of type '{}' has no len()", args[0].type_name())),
        }
    }
    
    fn builtin_type(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("type() takes exactly one argument"));
        }
        
        Ok(Value::String(args[0].type_name().to_string()))
    }
    
    fn builtin_str(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("str() takes exactly one argument"));
        }
        
        let string_repr = match &args[0] {
            Value::String(s) => s.clone(), // Don't add quotes for str() conversion
            _ => format!("{}", args[0]), // Use Display trait
        };
        Ok(Value::String(string_repr))
    }
    
    fn builtin_int(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow::anyhow!("int() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(*n)),
            Value::Float(n) => Ok(Value::Int(*n as i64)),
            Value::String(s) => {
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
            Value::String(s) => {
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
        let (start, end) = match args.len() {
            1 => (Value::Int(0), Self::builtin_int(vec![args[0].clone()])?),
            2 => (Self::builtin_int(vec![args[0].clone()])?, Self::builtin_int(vec![args[1].clone()])?),
            _ => return Err(anyhow::anyhow!("range() takes 1 or 2 arguments")),
        };
        
        if let (Value::Int(start), Value::Int(end)) = (start, end) {
            let items: Vec<Value> = (start..end).map(Value::Int).collect();
            Ok(Value::List(items))
        } else {
            Err(anyhow::anyhow!("range() arguments must be integers"))
        }
    }
    
    fn builtin_list(args: Vec<Value>) -> Result<Value> {
        match args.len() {
            0 => Ok(Value::List(Vec::new())),
            1 => {
                match &args[0] {
                    Value::List(items) => Ok(Value::List(items.clone())),
                    Value::String(s) => {
                        let chars: Vec<Value> = s.chars()
                            .map(|c| Value::String(c.to_string()))
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
            Value::String(s) => {
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
                
                Ok(Value::String(result))
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
    match backend {
        "vm" => {
            let mut vm = VM::new();
            let result = vm.execute_script(source, vec![])?;
            if !matches!(result, Value::None) {
                println!("{}", result);
            }
            Ok(())
        }
        _ => {
            Err(anyhow::anyhow!("Backend '{}' not supported for running files", backend))
        }
    }
}