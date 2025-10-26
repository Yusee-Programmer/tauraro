//! Intermediate Representation (IR) for Tauraro

use crate::ast::*;
use crate::value::Value;
use std::collections::HashMap;
use anyhow::Result;

#[derive(Debug, Clone)]
pub struct IRModule {
    pub functions: HashMap<String, IRFunction>,
    pub globals: Vec<IRInstruction>,
    pub type_info: IRTypeInfo, // Added to store type information
}

#[derive(Debug, Clone)]
pub struct IRFunction {
    pub name: String,
    pub params: Vec<String>,
    pub blocks: Vec<IRBlock>,
    pub return_type: Option<Type>,
    pub param_types: HashMap<String, Type>, // Added to store parameter types
}

#[derive(Debug, Clone)]
pub struct IRBlock {
    pub instructions: Vec<IRInstruction>,
}

#[derive(Debug, Clone)]
pub enum IRInstruction {
    // Constants
    LoadConst { value: Value, result: String },
    
    // Variables
    LoadLocal { name: String, result: String },
    StoreLocal { name: String, value: String },
    LoadGlobal { name: String, result: String },
    StoreGlobal { name: String, value: String },
    
    // Typed variables (new)
    LoadTypedLocal { name: String, result: String, type_info: Type },
    StoreTypedLocal { name: String, value: String, type_info: Type },
    LoadTypedGlobal { name: String, result: String, type_info: Type },
    StoreTypedGlobal { name: String, value: String, type_info: Type },
    
    // Binary operations
    BinaryOp { op: BinaryOp, left: String, right: String, result: String },
    
    // Typed binary operations (new)
    TypedBinaryOp { op: BinaryOp, left: String, right: String, result: String, type_info: Type },
    
    // Function calls
    Call { func: String, args: Vec<String>, result: Option<String> },
    Return { value: Option<String> },
    
    // Control flow
    Jump { target: usize },
    JumpIf { condition: String, target: usize },
    JumpIfNot { condition: String, target: usize },
    
    // Data structures
    ListCreate { elements: Vec<String>, result: String },
    DictCreate { pairs: Vec<(String, String)>, result: String },
    
    // Import statements
    Import { module: String },
    ImportFrom { module: String, names: Vec<String> },
    
    // OOP instructions
    ObjectCreate { class_name: String, result: String },
    ObjectSetAttr { object: String, attr: String, value: String },
    ObjectGetAttr { object: String, attr: String, result: String },
    SuperCall { args: Vec<String>, result: String }, // Added for super() calls
}

// Added struct to store type information in IR
#[derive(Debug, Clone)]
pub struct IRTypeInfo {
    pub variable_types: HashMap<String, Type>,
    pub function_types: HashMap<String, FunctionType>,
}

#[derive(Debug, Clone)]
pub struct FunctionType {
    pub param_types: HashMap<String, Type>,
    pub return_type: Option<Type>,
}

#[derive(Debug)]
pub struct Generator {
    // IR generation state
    object_types: HashMap<String, String>, // Maps variable names to class names
    type_info: IRTypeInfo, // Added to store type information during IR generation
    class_inheritance: HashMap<String, Vec<String>>, // Maps class names to their base classes
}

impl Generator {
    pub fn new() -> Self {
        Self {
            object_types: HashMap::new(),
            type_info: IRTypeInfo {
                variable_types: HashMap::new(),
                function_types: HashMap::new(),
            },
            class_inheritance: HashMap::new(),
        }
    }

    pub fn generate(&mut self, ast: Program) -> Result<IRModule> {
        let mut module = IRModule {
            functions: HashMap::new(),
            globals: Vec::new(),
            type_info: IRTypeInfo {
                variable_types: HashMap::new(),
                function_types: HashMap::new(),
            },
        };
        
        // Process each statement in the AST
        for statement in ast.statements {
            self.process_statement(&mut module, statement)?;
        }
        
        // Transfer collected type information to the module
        module.type_info = self.type_info.clone();
        
        Ok(module)
    }
    
    fn process_statement(&mut self, module: &mut IRModule, statement: Statement) -> Result<()> {
        match statement {
            Statement::ClassDef { name, bases, body, .. } => {
                // Store inheritance information
                let mut base_classes = Vec::new();
                for base in &bases {
                    if let Expr::Identifier(base_name) = base {
                        base_classes.push(base_name.clone());
                    }
                }
                self.class_inheritance.insert(name.clone(), base_classes);
                
                // Process class definition
                // First, create a constructor method if __init__ doesn't exist
                let has_init = body.iter().any(|stmt| {
                    if let Statement::FunctionDef { name, .. } = stmt {
                        name == "__init__"
                    } else {
                        false
                    }
                });
                
                // Process all methods and class attributes in the class
                for item in body {
                    match item {
                        Statement::FunctionDef { name: method_name, params, body: method_body, return_type, .. } => {
                            let function_name = format!("{}__{}", name, method_name);
                            
                            // Store function type information
                            let mut param_types = HashMap::new();
                            for param in &params {
                                if let Some(type_annotation) = &param.type_annotation {
                                    param_types.insert(param.name.clone(), type_annotation.clone());
                                }
                            }
                            
                            self.type_info.function_types.insert(
                                function_name.clone(),
                                FunctionType {
                                    param_types: param_types.clone(),
                                    return_type: return_type.clone(),
                                },
                            );
                            
                            let ir_function = self.process_function(function_name, params, method_body)?;
                            module.functions.insert(format!("{}__{}", name, method_name), ir_function);
                        }
                        Statement::VariableDef { name: attr_name, value: Some(value), .. } => {
                            // Handle class attributes
                            // For now, we'll store them as global constants that can be referenced
                            // In a full implementation, these would be stored in the class object
                            let class_attr_name = format!("{}__{}", name, attr_name);
                            
                            // Process the value expression
                            self.process_expression(module, &value)?;
                            
                            // Store the attribute as a global
                            module.globals.push(IRInstruction::StoreGlobal {
                                name: class_attr_name,
                                value: "temp".to_string()
                            });
                        }
                        _ => {
                            // Ignore other statement types in class body for now
                        }
                    }
                }
                
                // Add a default constructor if none exists
                if !has_init {
                    // Create a simple constructor that just returns self
                    let constructor = IRFunction {
                        name: format!("{}__init__", name),
                        params: vec!["self".to_string()],
                        blocks: vec![IRBlock {
                            instructions: vec![
                                IRInstruction::Return { value: Some("self".to_string()) }
                            ]
                        }],
                        return_type: None,
                        param_types: HashMap::new(),
                    };
                    module.functions.insert(format!("{}__init__", name), constructor);
                }
            },
            Statement::VariableDef { name, type_annotation, value: Some(value), .. } => {
                // Check if we have explicit type annotation
                let has_explicit_type = type_annotation.is_some();
                
                // Store type information if available
                if let Some(type_annotation) = &type_annotation {
                    self.type_info.variable_types.insert(name.clone(), type_annotation.clone());
                }
                
                // Process variable assignment - generate a unique temp variable
                let temp_var = format!("var_{}_temp", name);

                // Generate instructions to evaluate the value
                match value {
                    Expr::Literal(lit) => {
                        let val = self.literal_to_value(&lit);
                        
                        // Infer type if no explicit annotation
                        if !has_explicit_type {
                            self.infer_type(name.clone(), &val);
                        }
                        
                        module.globals.push(IRInstruction::LoadConst {
                            value: val,
                            result: temp_var.clone()
                        });
                    },
                    Expr::Identifier(var_name) => {
                        // Check if we have type information for this variable
                        if let Some(var_type) = self.type_info.variable_types.get(&var_name) {
                            module.globals.push(IRInstruction::LoadTypedGlobal {
                                name: var_name.clone(),
                                result: temp_var.clone(),
                                type_info: var_type.clone(),
                            });
                            
                            // Copy type information if no explicit annotation
                            if !has_explicit_type {
                                self.type_info.variable_types.insert(name.clone(), var_type.clone());
                            }
                        } else {
                            module.globals.push(IRInstruction::LoadGlobal {
                                name: var_name.clone(),
                                result: temp_var.clone()
                            });
                        }
                    },
                    Expr::BinaryOp { op, left, right } => {
                        // For binary operations, we need to evaluate both sides
                        let left_temp = format!("{}_left", temp_var);
                        let right_temp = format!("{}_right", temp_var);

                        // Evaluate left side recursively
                        self.process_expression_to_result(module, &left, &left_temp)?;

                        // Evaluate right side recursively
                        self.process_expression_to_result(module, &right, &right_temp)?;

                        // Check if we have type information for optimization
                        let left_type = self.get_expression_type(&left);
                        let right_type = self.get_expression_type(&right);
                        
                        if let (Some(l_type), Some(r_type)) = (&left_type, &right_type) {
                            if *l_type == *r_type {
                                // Use typed binary operation
                                module.globals.push(IRInstruction::TypedBinaryOp {
                                    op: op.clone(),
                                    left: left_temp,
                                    right: right_temp,
                                    result: temp_var.clone(),
                                    type_info: l_type.clone(),
                                });
                                
                                // Infer result type if no explicit annotation
                                if !has_explicit_type {
                                    self.type_info.variable_types.insert(name.clone(), l_type.clone());
                                }
                            } else {
                                // Use generic binary operation
                                module.globals.push(IRInstruction::BinaryOp {
                                    op: op.clone(),
                                    left: left_temp,
                                    right: right_temp,
                                    result: temp_var.clone()
                                });
                            }
                        } else {
                            // Use generic binary operation
                            module.globals.push(IRInstruction::BinaryOp {
                                op: op.clone(),
                                left: left_temp,
                                right: right_temp,
                                result: temp_var.clone()
                            });
                        }
                    },
                    _ => {
                        // For other expressions, use the old method
                        self.process_expression(module, &value)?;
                        // The result will be in "temp", so we need to rename it
                        if let Some(var_type) = &type_annotation {
                            module.globals.push(IRInstruction::LoadTypedGlobal {
                                name: "temp".to_string(),
                                result: temp_var.clone(),
                                type_info: var_type.clone(),
                            });
                        } else {
                            module.globals.push(IRInstruction::LoadGlobal {
                                name: "temp".to_string(),
                                result: temp_var.clone()
                            });
                        }
                    }
                }

                // Store the result in the global variable
                if let Some(var_type) = &type_annotation {
                    module.globals.push(IRInstruction::StoreTypedGlobal {
                        name: name.clone(),
                        value: temp_var.clone(),
                        type_info: var_type.clone(),
                    });
                } else {
                    module.globals.push(IRInstruction::StoreGlobal {
                        name: name.clone(),
                        value: temp_var.clone()
                    });
                }
                
                // Copy object type if this is an object
                if let Some(class_name) = self.object_types.get(&temp_var) {
                    self.object_types.insert(name.clone(), class_name.clone());
                }
            },
            Statement::Expression(expr) => {
                self.process_expression(module, &expr)?;
            },
            Statement::AttributeAssignment { object, name: attr_name, value } => {
                // Process attribute assignment
                self.process_expression(module, &value)?;
                module.globals.push(IRInstruction::ObjectSetAttr { 
                    object: self.expression_to_string(&object), 
                    attr: attr_name, 
                    value: "temp".to_string() 
                });
            },
            Statement::FunctionDef { name, params, body, return_type, .. } => {
                // Store function type information
                let mut param_types = HashMap::new();
                for param in &params {
                    if let Some(type_annotation) = &param.type_annotation {
                        param_types.insert(param.name.clone(), type_annotation.clone());
                        // Also store in global type info
                        self.type_info.variable_types.insert(param.name.clone(), type_annotation.clone());
                    }
                }
                
                self.type_info.function_types.insert(
                    name.clone(),
                    FunctionType {
                        param_types: param_types.clone(),
                        return_type: return_type.clone(),
                    },
                );
                
                // Process global function definitions
                let func_name = name.clone();
                let ir_function = self.process_function(func_name, params, body)?;
                module.functions.insert(name, ir_function);
            },
            _ => {
                // For other statements, we'll add placeholder handling
            }
        }
        Ok(())
    }
    
    fn process_function(&mut self, name: String, params: Vec<Param>, body: Vec<Statement>) -> Result<IRFunction> {
        let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
        
        // Create parameter type mapping
        let mut param_types = HashMap::new();
        for param in &params {
            if let Some(type_annotation) = &param.type_annotation {
                param_types.insert(param.name.clone(), type_annotation.clone());
                // Also store in global type info
                self.type_info.variable_types.insert(param.name.clone(), type_annotation.clone());
            }
        }
        
        let mut blocks = Vec::new();
        let mut instructions = Vec::new();
        
        // Process function body
        for statement in body {
            match statement {
                Statement::Return(Some(expr)) => {
                    // Process return expression
                    self.process_expression_for_instructions(&mut instructions, &expr)?;
                    instructions.push(IRInstruction::Return { 
                        value: Some("temp_result".to_string()) 
                    });
                },
                Statement::Return(None) => {
                    instructions.push(IRInstruction::Return { value: None });
                },
                Statement::AttributeAssignment { object, name: attr_name, value } => {
                    // Process attribute assignment
                    self.process_expression_for_instructions(&mut instructions, &value)?;
                    instructions.push(IRInstruction::ObjectSetAttr { 
                        object: self.expression_to_string(&object), 
                        attr: attr_name, 
                        value: "temp_result".to_string() 
                    });
                },
                Statement::Expression(expr) => {
                    // Process general expressions
                    self.process_expression_for_instructions(&mut instructions, &expr)?;
                },
                _ => {
                    // For other statements, add placeholder
                    instructions.push(IRInstruction::LoadConst { 
                        value: Value::None, 
                        result: "_".to_string() 
                    });
                }
            }
        }
        
        blocks.push(IRBlock { instructions });
        
        // Get return type from function type info if available
        let return_type = self.type_info.function_types.get(&name)
            .and_then(|func_type| func_type.return_type.clone());
        
        Ok(IRFunction {
            name,
            params: param_names,
            blocks,
            return_type,
            param_types,
        })
    }
    
    /// Infer the type of a value and store it
    fn infer_type(&mut self, name: String, value: &Value) -> Type {
        let inferred_type = match value {
            Value::Int(_) => Type::Simple("int".to_string()),
            Value::Float(_) => Type::Simple("float".to_string()),
            Value::Str(_) => Type::Simple("str".to_string()),
            Value::Bool(_) => Type::Simple("bool".to_string()),
            Value::None => Type::Simple("None".to_string()),
            Value::List(_) => Type::Simple("list".to_string()),
            Value::Dict(_) => Type::Simple("dict".to_string()),
            Value::Tuple(_) => Type::Simple("tuple".to_string()),
            Value::Set(_) => Type::Simple("set".to_string()),
            Value::Object { class_name, .. } => Type::Simple(class_name.clone()),
            _ => Type::Any,
        };
        
        // Store the inferred type
        self.type_info.variable_types.insert(name, inferred_type.clone());
        inferred_type
    }
    
    /// Get the type of an expression if available
    fn get_expression_type(&self, expr: &Expr) -> Option<Type> {
        match expr {
            Expr::Identifier(name) => {
                self.type_info.variable_types.get(name).cloned()
            },
            Expr::Literal(lit) => {
                match lit {
                    Literal::Int(_) => Some(Type::Simple("int".to_string())),
                    Literal::Float(_) => Some(Type::Simple("float".to_string())),
                    Literal::String(_) => Some(Type::Simple("str".to_string())),
                    Literal::Bool(_) => Some(Type::Simple("bool".to_string())),
                    Literal::None => Some(Type::Simple("None".to_string())),
                    _ => None,
                }
            },
            _ => None,
        }
    }

    fn literal_to_value(&self, literal: &Literal) -> Value {
        match literal {
            Literal::Int(n) => Value::Int(*n),
            Literal::Float(n) => Value::Float(*n),
            Literal::String(s) => Value::Str(s.clone()),
            Literal::Bool(b) => Value::Bool(*b),
            Literal::None => Value::None,
            _ => Value::None,
        }
    }
    
    fn process_expression_for_instructions(&mut self, instructions: &mut Vec<IRInstruction>, expr: &Expr) -> Result<()> {
        // Placeholder implementation for expression processing
        match expr {
            Expr::Literal(value) => {
                let converted_value = self.literal_to_value(value);
                instructions.push(IRInstruction::LoadConst { 
                    value: converted_value, 
                    result: "temp_result".to_string() 
                });
            },
            Expr::Identifier(name) => {
                instructions.push(IRInstruction::LoadGlobal { 
                    name: name.clone(), 
                    result: "temp_result".to_string() 
                });
                // Copy object type if this is an object
                if let Some(class_name) = self.object_types.get(name) {
                    self.object_types.insert("temp_result".to_string(), class_name.clone());
                }
            },
            Expr::Attribute { object, name } => {
                // Process attribute access
                let object_name = self.expression_to_string(&object);
                instructions.push(IRInstruction::ObjectGetAttr { 
                    object: object_name,
                    attr: name.clone(),
                    result: "temp_result".to_string() 
                });
            },
            Expr::MethodCall { object, method, args, .. } => {
                // Process method call
                let object_name = self.expression_to_string(&object);
                
                // Get the class name for this object
                let class_name = self.object_types.get(&object_name).cloned().unwrap_or_else(|| object_name.clone());
                
                // Process each argument and collect their result names
                let mut arg_names: Vec<String> = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_result = format!("method_arg_{}", i);
                    self.process_expression_for_instructions(instructions, arg)?;
                    // Move result to arg variable
                    instructions.push(IRInstruction::LoadGlobal { 
                        name: "temp_result".to_string(), 
                        result: arg_result.clone() 
                    });
                    arg_names.push(arg_result);
                }
                
                // Create the method name (class__method)
                let method_name = format!("{}__{}", class_name, method);
                
                // Call the method with object as first argument
                let mut method_args = vec![object_name.clone()];
                method_args.extend(arg_names);
                
                instructions.push(IRInstruction::Call { 
                    func: method_name,
                    args: method_args,
                    result: Some("temp_result".to_string())
                });
            },
            Expr::FormatString { parts } => {
                // Handle f-string by concatenating all parts
                if parts.is_empty() {
                    instructions.push(IRInstruction::LoadConst { 
                        value: Value::Str("".to_string()), 
                        result: "temp_result".to_string() 
                    });
                } else if parts.len() == 1 {
                    // Single part - either string literal or expression
                    match &parts[0] {
                        FormatPart::String(s) => {
                            instructions.push(IRInstruction::LoadConst { 
                                value: Value::Str(s.clone()), 
                                result: "temp_result".to_string() 
                            });
                        },
                        FormatPart::Expression { expr, .. } => {
                            // Evaluate the expression
                            self.process_expression_for_instructions(instructions, expr)?;
                            // The result is already in temp_result
                        },
                    }
                } else {
                    // Multiple parts - need to concatenate
                    // Start with first part
                    match &parts[0] {
                        FormatPart::String(s) => {
                            instructions.push(IRInstruction::LoadConst { 
                                value: Value::Str(s.clone()), 
                                result: "temp_result".to_string() 
                            });
                        },
                        FormatPart::Expression { expr, .. } => {
                            // Evaluate the expression
                            self.process_expression_for_instructions(instructions, expr)?;
                            // The result is already in temp_result
                        },
                    }
                    
                    // Concatenate remaining parts
                    for i in 1..parts.len() {
                        let temp_left = format!("fstring_left_{}", i);
                        let temp_right = format!("fstring_right_{}", i);
                        let temp_result_name = format!("fstring_result_{}", i);
                        
                        // Save current result to left temp
                        instructions.push(IRInstruction::LoadGlobal { 
                            name: "temp_result".to_string(), 
                            result: temp_left.clone() 
                        });
                        
                        // Evaluate next part to right temp
                        match &parts[i] {
                            FormatPart::String(s) => {
                                instructions.push(IRInstruction::LoadConst { 
                                    value: Value::Str(s.clone()), 
                                    result: temp_right.clone() 
                                });
                            },
                            FormatPart::Expression { expr, .. } => {
                                // Create a new temp result for this expression
                                let saved_result = "temp_result".to_string();
                                self.process_expression_for_instructions(instructions, expr)?;
                                // Move result to right temp
                                instructions.push(IRInstruction::LoadGlobal { 
                                    name: "temp_result".to_string(), 
                                    result: temp_right.clone() 
                                });
                                // Restore previous result name for consistency
                            },
                        }
                        
                        // Concatenate left and right
                        instructions.push(IRInstruction::BinaryOp { 
                            op: BinaryOp::Add,
                            left: temp_left.clone(),
                            right: temp_right.clone(),
                            result: temp_result_name.clone() 
                        });
                        
                        // Update temp_result to point to the new result
                        instructions.push(IRInstruction::LoadGlobal { 
                            name: temp_result_name, 
                            result: "temp_result".to_string() 
                        });
                    }
                }
            },
            Expr::Call { func, args, .. } => {
                // Process function call
                let func_name = self.expression_to_string(&func);

                // Process each argument and collect their result names
                let mut arg_names: Vec<String> = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_result = format!("arg_{}", i);
                    match arg {
                        Expr::Literal(lit) => {
                            let val = self.literal_to_value(lit);
                            instructions.push(IRInstruction::LoadConst {
                                value: val,
                                result: arg_result.clone()
                            });
                        },
                        Expr::Identifier(name) => {
                            instructions.push(IRInstruction::LoadGlobal {
                                name: name.clone(),
                                result: arg_result.clone()
                            });
                        },
                        Expr::BinaryOp { op, left, right } => {
                            // Evaluate binary operation
                            let left_temp = format!("{}_left", arg_result);
                            let right_temp = format!("{}_right", arg_result);

                            // Evaluate left side
                            match left.as_ref() {
                                Expr::Literal(lit) => {
                                    instructions.push(IRInstruction::LoadConst {
                                        value: self.literal_to_value(&lit),
                                        result: left_temp.clone()
                                    });
                                },
                                Expr::Identifier(n) => {
                                    instructions.push(IRInstruction::LoadGlobal {
                                        name: n.clone(),
                                        result: left_temp.clone()
                                    });
                                },
                                _ => {
                                    instructions.push(IRInstruction::LoadConst {
                                        value: Value::None,
                                        result: left_temp.clone()
                                    });
                                }
                            }

                            // Evaluate right side
                            match right.as_ref() {
                                Expr::Literal(lit) => {
                                    instructions.push(IRInstruction::LoadConst {
                                        value: self.literal_to_value(&lit),
                                        result: right_temp.clone()
                                    });
                                },
                                Expr::Identifier(n) => {
                                    instructions.push(IRInstruction::LoadGlobal {
                                        name: n.clone(),
                                        result: right_temp.clone()
                                    });
                                },
                                _ => {
                                    instructions.push(IRInstruction::LoadConst {
                                        value: Value::None,
                                        result: right_temp.clone()
                                    });
                                }
                            }

                            // Generate binary operation
                            instructions.push(IRInstruction::BinaryOp {
                                op: op.clone(),
                                left: left_temp,
                                right: right_temp,
                                result: arg_result.clone()
                            });
                        },
                        _ => {
                            // For other complex expressions, use a temp value
                            instructions.push(IRInstruction::LoadConst {
                                value: Value::None,
                                result: arg_result.clone()
                            });
                        }
                    }
                    arg_names.push(arg_result);
                }

                // Check if this looks like a class instantiation
                // For now, we'll add a simple heuristic: if the function name starts with uppercase letter,
                // treat it as object creation
                if func_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                    // This looks like object creation
                    let class_name = func_name.clone();
                    instructions.push(IRInstruction::ObjectCreate {
                        class_name: class_name.clone(),
                        result: "temp_result".to_string()
                    });
                    // Record the object type for method call resolution
                    self.object_types.insert("temp_result".to_string(), class_name);
                } else {
                    // Regular function call
                    instructions.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some("temp_result".to_string())
                    });
                }
            },
            _ => {
                // For other expressions, add placeholder
                instructions.push(IRInstruction::LoadConst { 
                    value: Value::None, 
                    result: "temp_result".to_string() 
                });
            }
        }
        Ok(())
    }
    
    fn process_expression(&mut self, module: &mut IRModule, expr: &Expr) -> Result<()> {
        // Placeholder implementation for expression processing
        match expr {
            Expr::Literal(value) => {
                let converted_value = self.literal_to_value(value);
                module.globals.push(IRInstruction::LoadConst { 
                    value: converted_value, 
                    result: "temp".to_string() 
                });
            },
            Expr::Identifier(name) => {
                module.globals.push(IRInstruction::LoadGlobal { 
                    name: name.clone(), 
                    result: "temp".to_string() 
                });
                // Copy object type if this is an object
                if let Some(class_name) = self.object_types.get(name) {
                    self.object_types.insert("temp".to_string(), class_name.clone());
                }
            },
            Expr::Attribute { object, name } => {
                // Process attribute access
                let object_name = self.expression_to_string(&object);
                module.globals.push(IRInstruction::ObjectGetAttr { 
                    object: object_name,
                    attr: name.clone(),
                    result: "temp".to_string() 
                });
            },
            Expr::MethodCall { object, method, args, .. } => {
                // Process method call
                let object_name = self.expression_to_string(&object);
                
                // Get the class name for this object
                let class_name = self.object_types.get(&object_name).cloned().unwrap_or_else(|| object_name.clone());
                
                // Process each argument and collect their result names
                let mut arg_names: Vec<String> = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_result = format!("method_arg_{}", i);
                    self.process_expression_to_result(module, arg, &arg_result)?;
                    arg_names.push(arg_result);
                }
                
                // Create the method name (class__method)
                let method_name = format!("{}__{}", class_name, method);
                
                // Call the method with object as first argument
                let mut method_args = vec![object_name.clone()];
                method_args.extend(arg_names);
                
                module.globals.push(IRInstruction::Call { 
                    func: method_name,
                    args: method_args,
                    result: Some("temp".to_string())
                });
            },
            Expr::FormatString { parts } => {
                // Handle f-string by concatenating all parts
                if parts.is_empty() {
                    module.globals.push(IRInstruction::LoadConst { 
                        value: Value::Str("".to_string()), 
                        result: "temp".to_string() 
                    });
                } else if parts.len() == 1 {
                    // Single part - either string literal or expression
                    match &parts[0] {
                        FormatPart::String(s) => {
                            module.globals.push(IRInstruction::LoadConst { 
                                value: Value::Str(s.clone()), 
                                result: "temp".to_string() 
                            });
                        },
                        FormatPart::Expression { expr, .. } => {
                            // Evaluate the expression
                            self.process_expression_to_result(module, expr, "temp")?;
                        },
                    }
                } else {
                    // Multiple parts - need to concatenate
                    // Start with first part
                    match &parts[0] {
                        FormatPart::String(s) => {
                            module.globals.push(IRInstruction::LoadConst { 
                                value: Value::Str(s.clone()), 
                                result: "temp".to_string() 
                            });
                        },
                        FormatPart::Expression { expr, .. } => {
                            // Evaluate the expression
                            self.process_expression_to_result(module, expr, "temp")?;
                        },
                    }
                    
                    // Concatenate remaining parts
                    for i in 1..parts.len() {
                        let temp_left = format!("fstring_left_{}", i);
                        let temp_right = format!("fstring_right_{}", i);
                        let temp_result_name = format!("fstring_result_{}", i);
                        
                        // Save current result to left temp
                        module.globals.push(IRInstruction::LoadGlobal { 
                            name: "temp".to_string(), 
                            result: temp_left.clone() 
                        });
                        
                        // Evaluate next part to right temp
                        match &parts[i] {
                            FormatPart::String(s) => {
                                module.globals.push(IRInstruction::LoadConst { 
                                    value: Value::Str(s.clone()), 
                                    result: temp_right.clone() 
                                });
                            },
                            FormatPart::Expression { expr, .. } => {
                                self.process_expression_to_result(module, expr, &temp_right)?;
                            },
                        }
                        
                        // Concatenate left and right
                        module.globals.push(IRInstruction::BinaryOp { 
                            op: BinaryOp::Add,
                            left: temp_left.clone(),
                            right: temp_right.clone(),
                            result: temp_result_name.clone() 
                        });
                        
                        // Update temp to point to the new result
                        module.globals.push(IRInstruction::LoadGlobal { 
                            name: temp_result_name, 
                            result: "temp".to_string() 
                        });
                    }
                }
            },
            Expr::Call { func, args, .. } => {
                // Process function call
                let func_name = self.expression_to_string(&func);

                // Process each argument and collect their result names
                let mut arg_names: Vec<String> = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_result = format!("arg_{}", i);
                    // Use recursive helper to handle any expression type
                    self.process_expression_to_result(module, arg, &arg_result)?;
                    arg_names.push(arg_result);
                }

                // Check if this is a super() call
                if func_name == "super" {
                    // Handle super() call - for now, we'll generate a special instruction
                    // In a full implementation, this would need more context about the current class
                    module.globals.push(IRInstruction::Call {
                        func: "tauraro_super_call".to_string(),
                        args: arg_names,
                        result: Some("temp".to_string())
                    });
                }
                // Check if this looks like a class instantiation
                // For now, we'll add a simple heuristic: if the function name starts with uppercase letter,
                // treat it as object creation
                else if func_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                    // This looks like object creation
                    let class_name = func_name.clone();
                    module.globals.push(IRInstruction::ObjectCreate {
                        class_name: class_name.clone(),
                        result: "temp".to_string()
                    });
                    // Record the object type for method call resolution
                    self.object_types.insert("temp".to_string(), class_name);
                } else {
                    // Regular function call
                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some("temp".to_string())
                    });
                }
            },
            _ => {
                // For other expressions, add placeholder
                module.globals.push(IRInstruction::LoadConst { 
                    value: Value::None, 
                    result: "temp".to_string() 
                });
            }
        }
        Ok(())
    }
    
    fn expression_to_string(&self, expr: &Expr) -> String {
        match expr {
            Expr::Identifier(name) => name.clone(),
            Expr::Attribute { object, name } => {
                format!("{}.{}", self.expression_to_string(&object), name)
            },
            _ => "temp_expr".to_string()
        }
    }

    
    /// Process an expression and store the result in a specific variable
    fn process_expression_to_result(&mut self, module: &mut IRModule, expr: &Expr, result_var: &str) -> Result<()> {
        match expr {
            Expr::Literal(lit) => {
                module.globals.push(IRInstruction::LoadConst {
                    value: self.literal_to_value(&lit),
                    result: result_var.to_string()
                });
            },
            Expr::Identifier(name) => {
                module.globals.push(IRInstruction::LoadGlobal {
                    name: name.clone(),
                    result: result_var.to_string()
                });
                // Copy object type if this is an object
                if let Some(class_name) = self.object_types.get(name) {
                    self.object_types.insert(result_var.to_string(), class_name.clone());
                }
            },
            Expr::BinaryOp { op, left, right } => {
                let left_temp = format!("{}_left", result_var);
                let right_temp = format!("{}_right", result_var);

                // Recursively evaluate left and right
                self.process_expression_to_result(module, &left, &left_temp)?;
                self.process_expression_to_result(module, &right, &right_temp)?;

                // Generate binary operation
                module.globals.push(IRInstruction::BinaryOp {
                    op: op.clone(),
                    left: left_temp,
                    right: right_temp,
                    result: result_var.to_string()
                });
            },
            Expr::Call { func, args, .. } => {
                let func_name = self.expression_to_string(&func);

                // Process arguments
                let mut arg_names: Vec<String> = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_result = format!("{}_arg_{}", result_var, i);
                    self.process_expression_to_result(module, arg, &arg_result)?;
                    arg_names.push(arg_result);
                }

                // Check if this is a class instantiation
                if func_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                    // This looks like object creation
                    let class_name = func_name.clone();
                    module.globals.push(IRInstruction::ObjectCreate {
                        class_name: class_name.clone(),
                        result: result_var.to_string()
                    });
                    // Record the object type for method call resolution
                    self.object_types.insert(result_var.to_string(), class_name);
                } else {
                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some(result_var.to_string())
                    });
                }
            },
            Expr::MethodCall { object, method, args, .. } => {
                // Process method call
                let object_name = self.expression_to_string(&object);
                
                // Get the class name for this object
                let class_name = self.object_types.get(&object_name).cloned().unwrap_or_else(|| object_name.clone());
                
                // Process each argument and collect their result names
                let mut arg_names: Vec<String> = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_result = format!("{}_method_arg_{}", result_var, i);
                    self.process_expression_to_result(module, arg, &arg_result)?;
                    arg_names.push(arg_result);
                }
                
                // Create the method name (class__method)
                let method_name = format!("{}__{}", class_name, method);
                
                // Call the method with object as first argument
                let mut method_args = vec![object_name.clone()];
                method_args.extend(arg_names);
                
                module.globals.push(IRInstruction::Call { 
                    func: method_name,
                    args: method_args,
                    result: Some(result_var.to_string())
                });
            },
            Expr::FormatString { parts } => {
                // Handle f-string by concatenating all parts
                if parts.is_empty() {
                    module.globals.push(IRInstruction::LoadConst { 
                        value: Value::Str("".to_string()), 
                        result: result_var.to_string() 
                    });
                } else if parts.len() == 1 {
                    // Single part - either string literal or expression
                    match &parts[0] {
                        FormatPart::String(s) => {
                            module.globals.push(IRInstruction::LoadConst { 
                                value: Value::Str(s.clone()), 
                                result: result_var.to_string() 
                            });
                        },
                        FormatPart::Expression { expr, .. } => {
                            // Evaluate the expression
                            self.process_expression_to_result(module, expr, result_var)?;
                        },
                    }
                } else {
                    // Multiple parts - need to concatenate
                    // Start with first part
                    match &parts[0] {
                        FormatPart::String(s) => {
                            module.globals.push(IRInstruction::LoadConst { 
                                value: Value::Str(s.clone()), 
                                result: result_var.to_string() 
                            });
                        },
                        FormatPart::Expression { expr, .. } => {
                            // Evaluate the expression
                            self.process_expression_to_result(module, expr, result_var)?;
                        },
                    }
                    
                    // Concatenate remaining parts
                    for i in 1..parts.len() {
                        let temp_left = format!("{}_left_{}", result_var, i);
                        let temp_right = format!("{}_right_{}", result_var, i);
                        let temp_result_name = format!("{}_result_{}", result_var, i);
                        
                        // Save current result to left temp
                        module.globals.push(IRInstruction::LoadGlobal { 
                            name: result_var.to_string(), 
                            result: temp_left.clone() 
                        });
                        
                        // Evaluate next part to right temp
                        match &parts[i] {
                            FormatPart::String(s) => {
                                module.globals.push(IRInstruction::LoadConst { 
                                    value: Value::Str(s.clone()), 
                                    result: temp_right.clone() 
                                });
                            },
                            FormatPart::Expression { expr, .. } => {
                                self.process_expression_to_result(module, expr, &temp_right)?;
                            },
                        }
                        
                        // Concatenate left and right
                        module.globals.push(IRInstruction::BinaryOp { 
                            op: BinaryOp::Add,
                            left: temp_left.clone(),
                            right: temp_right.clone(),
                            result: temp_result_name.clone() 
                        });
                        
                        // Update result_var to point to the new result
                        module.globals.push(IRInstruction::LoadGlobal { 
                            name: temp_result_name, 
                            result: result_var.to_string() 
                        });
                    }
                }
            },
            _ => {
                module.globals.push(IRInstruction::LoadConst {
                    value: Value::None,
                    result: result_var.to_string()
                });
            }
        }
        Ok(())
    }
}