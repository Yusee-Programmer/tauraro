//! Intermediate Representation (IR) for Tauraro

use crate::ast::*;
use crate::value::Value;
use std::collections::{HashMap, HashSet};
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
    pub defaults: HashMap<String, Value>, // Default values for parameters
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
    // Comments
    Comment(String),

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
    
    // Unary operations
    UnaryOp { op: crate::ast::UnaryOp, operand: String, result: String },
    
    // Typed binary operations (new)
    TypedBinaryOp { op: BinaryOp, left: String, right: String, result: String, type_info: Type },
    
    // Function calls
    Call { func: String, args: Vec<String>, result: Option<String> },
    Return { value: Option<String> },
    
    // Control flow
    Jump { target: usize },
    JumpIf { condition: String, target: usize },
    JumpIfNot { condition: String, target: usize },

    // High-level control flow (for easier transpilation)
    If {
        condition: String,
        then_body: Vec<IRInstruction>,
        elif_branches: Vec<(String, Vec<IRInstruction>)>,
        else_body: Option<Vec<IRInstruction>>,
    },
    While {
        condition: String,
        condition_instructions: Vec<IRInstruction>,  // Instructions to re-evaluate condition
        body: Vec<IRInstruction>,
    },
    For {
        variable: String,
        variables: Vec<crate::ast::AssignTarget>,
        iterable: String,
        body: Vec<IRInstruction>,
    },
    Break,
    Continue,
    Try {
        body: Vec<IRInstruction>,
        handlers: Vec<(Option<String>, Option<String>, Vec<IRInstruction>)>, // (exception_type, var_name, handler_body)
        else_body: Option<Vec<IRInstruction>>,
        finally_body: Option<Vec<IRInstruction>>,
    },
    Raise {
        exception: Option<String>,
    },

    // Data structures
    ListCreate { elements: Vec<String>, result: String },
    
    // Import statements
    Import { module: String },
    ImportFrom { module: String, names: Vec<String> },
    
    // OOP instructions
    ObjectCreate { class_name: String, result: String },
    ObjectSetAttr { object: String, attr: String, value: String },
    ObjectGetAttr { object: String, attr: String, result: String },
    SuperCall { args: Vec<String>, result: String }, // Added for super() calls
    
    // Dictionary instructions
    DictCreate { pairs: Vec<(String, String)>, result: String },
    DictSetItem { dict: String, key: String, value: String },
    DictGetItem { dict: String, key: String, result: String },
    
    // === NEW ADVANCED FEATURES ===
    
    // Lambda expressions
    Lambda {
        params: Vec<String>,
        body_instructions: Vec<IRInstruction>,
        captured_vars: Vec<String>,
        result: String,
        body_result_var: String, // Variable that holds the result of the body
    },
    
    // List comprehension
    ListComprehension {
        element_instrs: Vec<IRInstruction>,  // Instructions to compute element
        element_result: String,             // Variable holding element result
        variable: String,                   // Loop variable
        iterable: String,                   // Source iterable
        condition_instrs: Vec<IRInstruction>, // Instructions to compute condition
        condition_result: Option<String>,   // Variable holding condition result
        result: String,                     // Final list result
    },
    
    // Dict comprehension
    DictComprehension {
        key_instrs: Vec<IRInstruction>,
        key_result: String,
        value_instrs: Vec<IRInstruction>,
        value_result: String,
        variable: String,
        iterable: String,
        condition_instrs: Vec<IRInstruction>,
        condition_result: Option<String>,
        result: String,
    },
    
    // Slicing
    Slice {
        object: String,
        start: Option<String>,
        stop: Option<String>,
        step: Option<String>,
        result: String,
    },
    
    // Tuple operations
    TupleCreate { elements: Vec<String>, result: String },
    TupleUnpack { tuple: String, targets: Vec<String> },
    TupleGetItem { tuple: String, index: i64, result: String },
    
    // F-string / Format string
    FormatString {
        parts: Vec<IRFormatPart>,
        result: String,
    },
    
    // F-string with type preservation (preserves if single expression is numeric)
    FormatStringWithType {
        parts: Vec<IRFormatPart>,
        result: String,
        original_type: Option<Type>, // Preserves the type if single expression
        original_var: Option<String>, // Preserves reference to original typed variable
    },
    
    // Context manager (with statement)
    With {
        context_expr: String,
        alias: Option<String>,
        body: Vec<IRInstruction>,
    },
    
    // Generator/Yield
    Yield { value: Option<String> },
    YieldFrom { iterable: String },
    
    // Match statement
    Match {
        value: String,
        cases: Vec<MatchCase>,
        result: Option<String>,
    },
    
    // Variable arguments
    PackArgs { args: Vec<String>, result: String },      // Pack into *args tuple
    UnpackArgs { args: String, targets: Vec<String> },   // Unpack *args
    PackKwargs { pairs: Vec<(String, String)>, result: String },  // Pack into **kwargs dict
    UnpackKwargs { kwargs: String, targets: Vec<String> },        // Unpack **kwargs
}

// Format string part for f-strings (IR version)
#[derive(Debug, Clone)]
pub enum IRFormatPart {
    Literal(String),
    Expression { var: String, format_spec: Option<String> },
}

// Match case for pattern matching
#[derive(Debug, Clone)]
pub struct MatchCase {
    pub pattern: MatchPattern,
    pub guard: Option<String>,  // Optional guard variable
    pub body: Vec<IRInstruction>,
}

#[derive(Debug, Clone)]
pub enum MatchPattern {
    Literal(Value),
    Variable(String),
    Wildcard,
    Tuple(Vec<MatchPattern>),
    List(Vec<MatchPattern>),
    Dict(Vec<(String, MatchPattern)>),
    Class { name: String, patterns: Vec<MatchPattern> },
    Or(Vec<MatchPattern>),
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
    class_methods: HashMap<String, HashSet<String>>, // Maps class names to their methods
    imported_modules: HashSet<String>, // Track imported module names
    current_class: Option<String>, // Track current class being processed
    temp_var_counter: usize, // Counter for generating unique temporary variables
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
            class_methods: HashMap::new(),
            imported_modules: HashSet::new(),
            current_class: None,
            temp_var_counter: 0,
        }
    }

    /// Find which class defines a method by searching up the inheritance chain
    fn find_method_class(&self, class_name: &str, method_name: &str) -> String {
        // Check if the method is defined in this class
        if let Some(methods) = self.class_methods.get(class_name) {
            if methods.contains(method_name) {
                return class_name.to_string();
            }
        }

        // Search in parent classes
        if let Some(parents) = self.class_inheritance.get(class_name) {
            for parent in parents {
                // Recursively search in parent
                let result = self.find_method_class(parent, method_name);
                if result != *parent {
                    // Found in a parent's parent
                    return result;
                }
                // Check if parent has the method
                if let Some(parent_methods) = self.class_methods.get(parent) {
                    if parent_methods.contains(method_name) {
                        return parent.to_string();
                    }
                }
            }
        }

        // Method not found, return the original class name
        class_name.to_string()
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
            Statement::Comment(text) => {
                // Add comment to globals
                module.globals.push(IRInstruction::Comment(text));
            },
            Statement::ClassDef { name, bases, body, .. } => {
                // Store inheritance information
                let mut base_classes = Vec::new();
                for base in &bases {
                    if let Expr::Identifier(base_name) = base {
                        base_classes.push(base_name.clone());
                    }
                }
                self.class_inheritance.insert(name.clone(), base_classes);

                // Set current class context
                self.current_class = Some(name.clone());

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
                            // Create function name, handling dunder methods specially
                            // For __init__ and other dunder methods, avoid extra underscores
                            let function_name = if method_name.starts_with("__") && method_name.ends_with("__") {
                                format!("{}{}", name, method_name)
                            } else {
                                format!("{}__{}", name, method_name)
                            };

                            // Record that this method belongs to this class
                            self.class_methods.entry(name.clone())
                                .or_insert_with(HashSet::new)
                                .insert(method_name.clone());

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

                            let ir_function = self.process_function(function_name.clone(), params, method_body)?;
                            module.functions.insert(function_name, ir_function);
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
                
                // Add a default constructor if none exists and no parent class
                if !has_init && bases.is_empty() {
                    // Record the default __init__ method
                    self.class_methods.entry(name.clone())
                        .or_insert_with(HashSet::new)
                        .insert("__init__".to_string());

                    // Create a simple constructor that just returns self
                    let constructor = IRFunction {
                        name: format!("{}__init__", name),
                        params: vec!["self".to_string()],
                        defaults: HashMap::new(),
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

                // Reset current class context
                self.current_class = None;
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
                    Expr::ListComp { .. } | Expr::DictComp { .. } | Expr::SetComp { .. } | Expr::GeneratorExp { .. } | Expr::Lambda { .. } => {
                        // For comprehensions, generator expressions, and lambdas, use process_expression_to_result directly
                        self.process_expression_to_result(module, &value, &temp_var)?;
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
                        // Copy object type from "temp" to temp_var
                        if let Some(class_name) = self.object_types.get("temp") {
                            self.object_types.insert(temp_var.clone(), class_name.clone());
                        }
                    }
                }

                // Store the result in the global variable
                let var_type_from_info = self.type_info.variable_types.get(&name).cloned();
                let final_type = type_annotation.clone().or(var_type_from_info);
                
                if let Some(var_type) = &final_type {
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
            Statement::TupleUnpack { targets, value } => {
                // Handle tuple unpacking: a, b, c = (1, 2, 3)
                let value_var = "tuple_unpack_value".to_string();
                
                // Evaluate the right-hand side expression
                self.process_expression_to_result(module, &value, &value_var)?;
                
                // Generate TupleUnpack IR instruction
                module.globals.push(IRInstruction::TupleUnpack {
                    tuple: value_var,
                    targets: targets.clone(),
                });
            },
            Statement::Expression(expr) => {
                self.process_expression(module, &expr)?;
            },
            Statement::AttributeAssignment { object, name: attr_name, value, .. } => {
                // Process attribute assignment
                // First process the object expression to get a proper variable
                self.process_expression(module, &object)?;
                let object_var = "temp_setattr_object".to_string();
                module.globals.push(IRInstruction::LoadGlobal {
                    name: "temp".to_string(),
                    result: object_var.clone()
                });

                // Then process the value
                self.process_expression(module, &value)?;

                module.globals.push(IRInstruction::ObjectSetAttr {
                    object: object_var,
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
            Statement::Import { module: module_name, alias } => {
                // Track imported module
                let effective_name = alias.as_ref().unwrap_or(&module_name);
                self.imported_modules.insert(effective_name.clone());

                // Add import instruction to globals
                module.globals.push(IRInstruction::Import {
                    module: module_name.clone(),
                });
            },
            Statement::FromImport { module: module_name, names } => {
                // Extract just the names for the instruction
                let imported_names: Vec<String> = names.iter().map(|(name, _)| name.clone()).collect();

                // Add import from instruction to globals
                module.globals.push(IRInstruction::ImportFrom {
                    module: module_name.clone(),
                    names: imported_names,
                });
            },
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                // Process control flow at global scope
                self.process_expression(module, &condition)?;
                let condition_var = "temp".to_string();

                let mut then_instructions = Vec::new();
                for stmt in then_branch {
                    self.process_statement_in_function(&mut then_instructions, stmt)?;
                }

                let mut elif_ir_branches = Vec::new();
                for (elif_cond, elif_body) in elif_branches {
                    let mut elif_cond_instrs = Vec::new();
                    self.process_expression_for_instructions(&mut elif_cond_instrs, &elif_cond)?;
                    // The condition is evaluated into "temp_result" by process_expression_for_instructions
                    let elif_cond_var = "temp_result".to_string();

                    let mut elif_body_instrs = Vec::new();
                    for stmt in elif_body {
                        self.process_statement_in_function(&mut elif_body_instrs, stmt)?;
                    }

                    elif_cond_instrs.extend(elif_body_instrs);
                    elif_ir_branches.push((elif_cond_var, elif_cond_instrs));
                }

                let else_instructions = if let Some(else_stmts) = else_branch {
                    let mut else_instrs = Vec::new();
                    for stmt in else_stmts {
                        self.process_statement_in_function(&mut else_instrs, stmt)?;
                    }
                    Some(else_instrs)
                } else {
                    None
                };

                module.globals.push(IRInstruction::If {
                    condition: condition_var,
                    then_body: then_instructions,
                    elif_branches: elif_ir_branches,
                    else_body: else_instructions,
                });
            },
            Statement::While { condition, body, else_branch: _ } => {
                // Store the current length to capture condition instructions
                let start_len = module.globals.len();
                self.process_expression(module, &condition)?;
                let end_len = module.globals.len();

                // Extract the condition evaluation instructions (clone them, don't remove)
                let condition_instructions: Vec<IRInstruction> = module.globals[start_len..end_len].to_vec();
                let condition_var = "temp".to_string();

                let mut body_instructions = Vec::new();
                for stmt in body {
                    self.process_statement_in_function(&mut body_instructions, stmt)?;
                }

                module.globals.push(IRInstruction::While {
                    condition: condition_var,
                    condition_instructions,
                    body: body_instructions,
                });
            },
            Statement::For { variable, variables, iterable, body, else_branch: _, .. } => {
                self.process_expression(module, &iterable)?;
                // Get the result variable from the last instruction
                let iterable_var = module.globals.last()
                    .and_then(|instr| match instr {
                        IRInstruction::LoadConst { result, .. } => Some(result.clone()),
                        IRInstruction::Call { result, .. } => result.clone(),
                        IRInstruction::LoadGlobal { result, .. } => Some(result.clone()),
                        IRInstruction::LoadLocal { result, .. } => Some(result.clone()),
                        IRInstruction::ListCreate { result, .. } => Some(result.clone()),
                        IRInstruction::DictCreate { result, .. } => Some(result.clone()),
                        IRInstruction::BinaryOp { result, .. } => Some(result.clone()),
                        IRInstruction::ObjectCreate { result, .. } => Some(result.clone()),
                        _ => None,
                    })
                    .unwrap_or_else(|| "temp_iterable".to_string());

                let mut body_instructions = Vec::new();
                for stmt in body {
                    self.process_statement_in_function(&mut body_instructions, stmt)?;
                }

                module.globals.push(IRInstruction::For {
                    variable,
                    variables: variables.clone(),
                    iterable: iterable_var,
                    body: body_instructions,
                });
            },
            _ => {
                // For other statements, we'll add placeholder handling
            }
        }
        Ok(())
    }
    
    /// Process a single statement within a function (helper for recursion)
    fn process_statement_in_function(&mut self, instructions: &mut Vec<IRInstruction>, statement: Statement) -> Result<()> {
        match statement {
            Statement::Comment(text) => {
                instructions.push(IRInstruction::Comment(text));
            },
            Statement::Return(Some(expr)) => {
                self.process_expression_for_instructions(instructions, &expr)?;
                instructions.push(IRInstruction::Return {
                    value: Some("temp_result".to_string())
                });
            },
            Statement::Return(None) => {
                instructions.push(IRInstruction::Return { value: None });
            },
            Statement::AttributeAssignment { object, name: attr_name, value, .. } => {
                // First process the object expression to get a proper variable
                self.process_expression_for_instructions(instructions, &object)?;
                let object_var = "temp_setattr_object".to_string();
                instructions.push(IRInstruction::LoadGlobal {
                    name: "temp_result".to_string(),
                    result: object_var.clone()
                });

                // Then process the value
                self.process_expression_for_instructions(instructions, &value)?;

                instructions.push(IRInstruction::ObjectSetAttr {
                    object: object_var,
                    attr: attr_name,
                    value: "temp_result".to_string()
                });
            },
            Statement::Expression(expr) => {
                self.process_expression_for_instructions(instructions, &expr)?;
            },
            Statement::VariableDef { name, value: Some(value), type_annotation, .. } => {
                self.process_expression_for_instructions(instructions, &value)?;
                
                // Check if we have explicit type annotation or if the variable already has a known type
                let var_type = type_annotation.clone()
                    .or_else(|| self.type_info.variable_types.get(&name).cloned());
                
                // Store/update type information if available
                if let Some(type_annotation) = &var_type {
                    self.type_info.variable_types.insert(name.clone(), type_annotation.clone());
                    instructions.push(IRInstruction::StoreTypedLocal {
                        name: name.clone(),
                        value: "temp_result".to_string(),
                        type_info: type_annotation.clone(),
                    });
                } else {
                    instructions.push(IRInstruction::StoreLocal {
                        name: name.clone(),
                        value: "temp_result".to_string()
                    });
                }
                
                // Track object types when storing objects
                if let Some(obj_type) = self.object_types.get("temp_result") {
                    self.object_types.insert(name.clone(), obj_type.clone());
                }
            },
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                self.process_expression_for_instructions(instructions, &condition)?;
                let condition_var = "temp_result".to_string();

                let mut then_instructions = Vec::new();
                for stmt in then_branch {
                    self.process_statement_in_function(&mut then_instructions, stmt)?;
                }

                let mut elif_ir_branches = Vec::new();
                for (elif_cond, elif_body) in elif_branches {
                    let mut elif_cond_instrs = Vec::new();
                    self.process_expression_for_instructions(&mut elif_cond_instrs, &elif_cond)?;
                    // The condition is evaluated into "temp_result" by process_expression_for_instructions
                    let elif_cond_var = "temp_result".to_string();

                    let mut elif_body_instrs = Vec::new();
                    for stmt in elif_body {
                        self.process_statement_in_function(&mut elif_body_instrs, stmt)?;
                    }

                    elif_cond_instrs.extend(elif_body_instrs);
                    elif_ir_branches.push((elif_cond_var, elif_cond_instrs));
                }

                let else_instructions = if let Some(else_stmts) = else_branch {
                    let mut else_instrs = Vec::new();
                    for stmt in else_stmts {
                        self.process_statement_in_function(&mut else_instrs, stmt)?;
                    }
                    Some(else_instrs)
                } else {
                    None
                };

                instructions.push(IRInstruction::If {
                    condition: condition_var,
                    then_body: then_instructions,
                    elif_branches: elif_ir_branches,
                    else_body: else_instructions,
                });
            },
            Statement::While { condition, body, else_branch: _ } => {
                // Capture condition evaluation instructions
                let start_len = instructions.len();
                self.process_expression_for_instructions(instructions, &condition)?;
                let end_len = instructions.len();

                // Extract condition instructions (will be re-executed)
                let condition_instructions: Vec<IRInstruction> = instructions.drain(start_len..end_len).collect();
                let condition_var = "temp_result".to_string();

                let mut body_instructions = Vec::new();
                for stmt in body {
                    self.process_statement_in_function(&mut body_instructions, stmt)?;
                }

                // Re-add condition instructions before the While (for initial evaluation)
                for instr in &condition_instructions {
                    instructions.push(instr.clone());
                }

                instructions.push(IRInstruction::While {
                    condition: condition_var,
                    condition_instructions,
                    body: body_instructions,
                });
            },
            Statement::For { variable, variables, iterable, body, else_branch: _, .. } => {
                self.process_expression_for_instructions(instructions, &iterable)?;
                // Get the result variable from the last instruction
                let iterable_var = instructions.last()
                    .and_then(|instr| match instr {
                        IRInstruction::LoadConst { result, .. } => Some(result.clone()),
                        IRInstruction::Call { result, .. } => result.clone(),
                        IRInstruction::LoadGlobal { result, .. } => Some(result.clone()),
                        IRInstruction::LoadLocal { result, .. } => Some(result.clone()),
                        IRInstruction::ListCreate { result, .. } => Some(result.clone()),
                        IRInstruction::DictCreate { result, .. } => Some(result.clone()),
                        IRInstruction::BinaryOp { result, .. } => Some(result.clone()),
                        IRInstruction::ObjectCreate { result, .. } => Some(result.clone()),
                        _ => None,
                    })
                    .unwrap_or_else(|| "temp_iterable".to_string());

                let mut body_instructions = Vec::new();
                for stmt in body {
                    self.process_statement_in_function(&mut body_instructions, stmt)?;
                }

                instructions.push(IRInstruction::For {
                    variable,
                    variables: variables.clone(),
                    iterable: iterable_var,
                    body: body_instructions,
                });
            },
            Statement::Break => {
                instructions.push(IRInstruction::Break);
            },
            Statement::Continue => {
                instructions.push(IRInstruction::Continue);
            },
            _ => {
                instructions.push(IRInstruction::LoadConst {
                    value: Value::None,
                    result: "_".to_string()
                });
            }
        }
        Ok(())
    }

    fn process_function(&mut self, name: String, params: Vec<Param>, body: Vec<Statement>) -> Result<IRFunction> {
        let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
        
        // Extract default values from parameters
        let mut defaults = HashMap::new();
        for param in &params {
            if let Some(default_expr) = &param.default {
                // Convert the expression to a value (only works for literals)
                if let Expr::Literal(lit) = default_expr {
                    let default_val = self.literal_to_value(lit);
                    defaults.insert(param.name.clone(), default_val);
                }
            }
        }
        
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

        // Process function body using helper method
        for statement in body {
            self.process_statement_in_function(&mut instructions, statement)?;
        }
        
        blocks.push(IRBlock { instructions });
        
        // Get return type from function type info if available
        let return_type = self.type_info.function_types.get(&name)
            .and_then(|func_type| func_type.return_type.clone());
        
        Ok(IRFunction {
            name,
            params: param_names,
            defaults,
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
            Expr::BinaryOp { op, left, right } => {
                // Handle binary operations
                let left_temp = "binop_left".to_string();
                let right_temp = "binop_right".to_string();

                // Evaluate left operand
                match left.as_ref() {
                    Expr::Literal(lit) => {
                        instructions.push(IRInstruction::LoadConst {
                            value: self.literal_to_value(lit),
                            result: left_temp.clone()
                        });
                    },
                    Expr::Identifier(name) => {
                        instructions.push(IRInstruction::LoadLocal {
                            name: name.clone(),
                            result: left_temp.clone()
                        });
                    },
                    _ => {
                        // Recursively process complex left expressions
                        self.process_expression_for_instructions(instructions, left)?;
                        instructions.push(IRInstruction::LoadLocal {
                            name: "temp_result".to_string(),
                            result: left_temp.clone()
                        });
                    }
                }

                // Evaluate right operand
                match right.as_ref() {
                    Expr::Literal(lit) => {
                        instructions.push(IRInstruction::LoadConst {
                            value: self.literal_to_value(lit),
                            result: right_temp.clone()
                        });
                    },
                    Expr::Identifier(name) => {
                        instructions.push(IRInstruction::LoadLocal {
                            name: name.clone(),
                            result: right_temp.clone()
                        });
                    },
                    _ => {
                        // Recursively process complex right expressions
                        self.process_expression_for_instructions(instructions, right)?;
                        instructions.push(IRInstruction::LoadLocal {
                            name: "temp_result".to_string(),
                            result: right_temp.clone()
                        });
                    }
                }

                // Check if both operands have type information for optimization
                let left_type = self.get_expression_type(left);
                let right_type = self.get_expression_type(right);
                
                if let (Some(l_type), Some(r_type)) = (&left_type, &right_type) {
                    if *l_type == *r_type {
                        // Use typed binary operation
                        instructions.push(IRInstruction::TypedBinaryOp {
                            op: op.clone(),
                            left: left_temp,
                            right: right_temp,
                            result: "temp_result".to_string(),
                            type_info: l_type.clone(),
                        });
                    } else {
                        // Use generic binary operation
                        instructions.push(IRInstruction::BinaryOp {
                            op: op.clone(),
                            left: left_temp,
                            right: right_temp,
                            result: "temp_result".to_string()
                        });
                    }
                } else {
                    // Use generic binary operation
                    instructions.push(IRInstruction::BinaryOp {
                        op: op.clone(),
                        left: left_temp,
                        right: right_temp,
                        result: "temp_result".to_string()
                    });
                }
            },
            Expr::Attribute { object, name } => {
                // Process attribute access
                // First process the object expression to get a proper variable
                self.process_expression_for_instructions(instructions, &object)?;
                let object_var = "temp_attr_object".to_string();
                instructions.push(IRInstruction::LoadGlobal {
                    name: "temp_result".to_string(),
                    result: object_var.clone()
                });

                instructions.push(IRInstruction::ObjectGetAttr {
                    object: object_var,
                    attr: name.clone(),
                    result: "temp_result".to_string()
                });
            },
            Expr::MethodCall { object, method, args, .. } => {
                // Check if this is a super() method call
                let is_super_call = matches!(object.as_ref(), Expr::Call { func, .. }
                    if matches!(func.as_ref(), Expr::Identifier(name) if name == "super"));

                if is_super_call {
                    // Handle super().method() call
                    if let Some(current_class) = &self.current_class {
                        // Get the base class name
                        let base_classes = self.class_inheritance.get(current_class).cloned().unwrap_or_default();
                        let parent_class = base_classes.first().cloned().unwrap_or_else(|| "object".to_string());

                        // Process arguments
                        let mut arg_names: Vec<String> = Vec::new();
                        for (i, arg) in args.iter().enumerate() {
                            let arg_result = format!("method_arg_{}", i);
                            self.process_expression_for_instructions(instructions, arg)?;
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: arg_result.clone()
                            });
                            arg_names.push(arg_result);
                        }

                        // Create the parent method name (ParentClass__method)
                        let method_name = format!("{}__{}", parent_class, method);

                        // Call the parent method with self as first argument
                        let mut method_args = vec!["self".to_string()];
                        method_args.extend(arg_names);

                        instructions.push(IRInstruction::Call {
                            func: method_name,
                            args: method_args,
                            result: Some("temp_result".to_string())
                        });
                    } else {
                        // super() called outside of a class - error
                        instructions.push(IRInstruction::LoadConst {
                            value: Value::None,
                            result: "temp_result".to_string()
                        });
                    }
                } else {
                // Process method call
                let object_name = self.expression_to_string(&object);

                // Check if this is a module function call vs a method call
                if self.imported_modules.contains(&object_name) {
                    // Module function call: module.function() -> module_function()
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("method_arg_{}", i);
                        self.process_expression_for_instructions(instructions, arg)?;
                        instructions.push(IRInstruction::LoadGlobal {
                            name: "temp_result".to_string(),
                            result: arg_result.clone()
                        });
                        arg_names.push(arg_result);
                    }

                    // Call module function directly (no self argument)
                    let func_name = format!("{}_{}", object_name, method);
                    instructions.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some("temp_result".to_string())
                    });
                } else if ["upper", "lower", "strip", "lstrip", "rstrip", "split", "join", 
                    "replace", "startswith", "endswith", "find", "title", "capitalize", "swapcase",
                    "isdigit", "isalpha", "isalnum", "isspace", "isupper", "islower", "count", 
                    "center", "ljust", "rjust", "zfill",
                    "append", "pop", "insert", "remove", "extend", "index", "reverse", "sort", "copy", "clear"].contains(&method.as_str()) {
                    // Built-in methods for strings and lists
                    self.process_expression_for_instructions(instructions, &object)?;
                    let object_var = "temp_object".to_string();
                    instructions.push(IRInstruction::LoadGlobal {
                        name: "temp_result".to_string(),
                        result: object_var.clone()
                    });

                    // Process each argument and collect their result names
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("method_arg_{}", i);
                        self.process_expression_for_instructions(instructions, arg)?;
                        instructions.push(IRInstruction::LoadGlobal {
                            name: "temp_result".to_string(),
                            result: arg_result.clone()
                        });
                        arg_names.push(arg_result);
                    }

                    // Built-in methods use text__ or lst__ prefix based on method name
                    let func_prefix = if ["append", "pop", "insert", "remove", "extend", "index", "reverse", "sort", "copy", "clear"].contains(&method.as_str()) { "lst" } else { "text" };
                    let func_name = format!("{}__{}",  func_prefix, method);
                    let mut method_args = vec![object_var];
                    method_args.extend(arg_names);

                    instructions.push(IRInstruction::Call {
                        func: func_name,
                        args: method_args,
                        result: Some("temp_result".to_string())
                    });
                } else {
                    // Object method call: obj.method() -> ClassName__method(obj, ...)
                    // First process the object expression to get a proper variable
                    self.process_expression_for_instructions(instructions, &object)?;
                    let object_var = "temp_object".to_string();
                    instructions.push(IRInstruction::LoadGlobal {
                        name: "temp_result".to_string(),
                        result: object_var.clone()
                    });

                    // Copy object type tracking
                    if let Some(class) = self.object_types.get("temp_result") {
                        self.object_types.insert(object_var.clone(), class.clone());
                    }

                    // Get the class name for this object
                    // First try object_var (the processed variable), then fall back to object_name
                    let mut class_name = self.object_types.get(&object_var)
                        .or_else(|| self.object_types.get(&object_name))
                        .cloned()
                        .unwrap_or_else(|| object_name.clone());

                    // If the class name looks like a variable (starts with lowercase),
                    // try to find a matching class by capitalizing it
                    if class_name.chars().next().map(|c| c.is_lowercase()).unwrap_or(false) {
                        let capitalized = format!("{}{}", 
                            class_name.chars().next().unwrap().to_uppercase().to_string(),
                            &class_name[1..]);
                        if self.class_methods.contains_key(&capitalized) {
                            class_name = capitalized;
                        }
                    }

                    // Find which class actually defines this method (may be in a parent class)
                    let defining_class = self.find_method_class(&class_name, method);

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

                    // Create the method name using the class that actually defines it
                    let method_name = format!("{}__{}", defining_class, method);

                    // Call the method with object variable as first argument
                    let mut method_args = vec![object_var];
                    method_args.extend(arg_names);

                    instructions.push(IRInstruction::Call {
                        func: method_name,
                        args: method_args,
                        result: Some("temp_result".to_string())
                    });

                    // If this returns an object of a known type, track it
                    // For method chaining: builder.add(5) returns Builder
                    if class_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                        // If the class name looks like a class, assume method returns same type
                        self.object_types.insert("temp_result".to_string(), class_name.clone());
                    }
                }
                }
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
            Expr::Compare { left, ops, comparators } => {
                // Handle comparison operations (>, <, ==, etc.)
                if ops.len() == 1 && comparators.len() == 1 {
                    let left_temp = "comp_left".to_string();
                    let right_temp = "comp_right".to_string();

                    // Evaluate left operand
                    match left.as_ref() {
                        Expr::Literal(lit) => {
                            instructions.push(IRInstruction::LoadConst {
                                value: self.literal_to_value(lit),
                                result: left_temp.clone()
                            });
                        },
                        Expr::Identifier(name) => {
                            instructions.push(IRInstruction::LoadGlobal {
                                name: name.clone(),
                                result: left_temp.clone()
                            });
                        },
                        _ => {
                            // Recursively process complex expressions
                            self.process_expression_for_instructions(instructions, left)?;
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: left_temp.clone()
                            });
                        }
                    }

                    // Evaluate right operand
                    match &comparators[0] {
                        Expr::Literal(lit) => {
                            instructions.push(IRInstruction::LoadConst {
                                value: self.literal_to_value(lit),
                                result: right_temp.clone()
                            });
                        },
                        Expr::Identifier(name) => {
                            instructions.push(IRInstruction::LoadGlobal {
                                name: name.clone(),
                                result: right_temp.clone()
                            });
                        },
                        _ => {
                            // Recursively process complex expressions
                            self.process_expression_for_instructions(instructions, &comparators[0])?;
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: right_temp.clone()
                            });
                        }
                    }

                    // Convert CompareOp to BinaryOp
                    let binary_op = match &ops[0] {
                        CompareOp::Eq => BinaryOp::Eq,
                        CompareOp::NotEq => BinaryOp::Ne,
                        CompareOp::Lt => BinaryOp::Lt,
                        CompareOp::LtE => BinaryOp::Le,
                        CompareOp::Gt => BinaryOp::Gt,
                        CompareOp::GtE => BinaryOp::Ge,
                        _ => BinaryOp::Eq, // fallback
                    };

                    // Generate binary operation for comparison
                    instructions.push(IRInstruction::BinaryOp {
                        op: binary_op,
                        left: left_temp,
                        right: right_temp,
                        result: "temp_result".to_string()
                    });
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

                            // Recursively evaluate left and right using process_expression_for_instructions
                            // to handle complex expressions
                            let mut left_instrs = Vec::new();
                            self.process_expression_for_instructions(&mut left_instrs, left)?;
                            instructions.extend(left_instrs);
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: left_temp.clone()
                            });

                            let mut right_instrs = Vec::new();
                            self.process_expression_for_instructions(&mut right_instrs, right)?;
                            instructions.extend(right_instrs);
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: right_temp.clone()
                            });

                            // Generate binary operation
                            instructions.push(IRInstruction::BinaryOp {
                                op: op.clone(),
                                left: left_temp,
                                right: right_temp,
                                result: arg_result.clone()
                            });
                        },
                        Expr::Compare { left, ops, comparators } => {
                            // Handle comparison operations (>, <, ==, etc.)
                            if ops.len() == 1 && comparators.len() == 1 {
                                let left_temp = format!("{}_left", arg_result);
                                let right_temp = format!("{}_right", arg_result);

                                // Evaluate left and right operands
                                let mut left_instrs = Vec::new();
                                self.process_expression_for_instructions(&mut left_instrs, left)?;
                                instructions.extend(left_instrs);
                                instructions.push(IRInstruction::LoadGlobal {
                                    name: "temp_result".to_string(),
                                    result: left_temp.clone()
                                });

                                let mut right_instrs = Vec::new();
                                self.process_expression_for_instructions(&mut right_instrs, &comparators[0])?;
                                instructions.extend(right_instrs);
                                instructions.push(IRInstruction::LoadGlobal {
                                    name: "temp_result".to_string(),
                                    result: right_temp.clone()
                                });

                                // Convert CompareOp to BinaryOp
                                let binary_op = match &ops[0] {
                                    CompareOp::Eq => BinaryOp::Eq,
                                    CompareOp::NotEq => BinaryOp::Ne,
                                    CompareOp::Lt => BinaryOp::Lt,
                                    CompareOp::LtE => BinaryOp::Le,
                                    CompareOp::Gt => BinaryOp::Gt,
                                    CompareOp::GtE => BinaryOp::Ge,
                                    _ => BinaryOp::Eq, // fallback
                                };

                                // Generate binary operation for comparison
                                instructions.push(IRInstruction::BinaryOp {
                                    op: binary_op,
                                    left: left_temp,
                                    right: right_temp,
                                    result: arg_result.clone()
                                });
                            } else {
                            }
                        },
                        Expr::Attribute { object, name } => {
                            // Handle attribute access: object.attribute
                            let object_name = self.expression_to_string(&object);
                            instructions.push(IRInstruction::ObjectGetAttr {
                                object: object_name,
                                attr: name.clone(),
                                result: arg_result.clone()
                            });
                        },
                        Expr::List(elements) => {
                            // Handle list literal: [item1, item2, ...]
                            let mut element_names = Vec::new();
                            for (j, elem) in elements.iter().enumerate() {
                                let elem_result = format!("{}_elem_{}", arg_result, j);
                                match elem {
                                    Expr::Literal(lit) => {
                                        instructions.push(IRInstruction::LoadConst {
                                            value: self.literal_to_value(&lit),
                                            result: elem_result.clone()
                                        });
                                    },
                                    Expr::Identifier(name) => {
                                        instructions.push(IRInstruction::LoadGlobal {
                                            name: name.clone(),
                                            result: elem_result.clone()
                                        });
                                    },
                                    _ => {
                                        // For complex expressions, recursively process
                                        self.process_expression_for_instructions(instructions, elem)?;
                                        instructions.push(IRInstruction::LoadGlobal {
                                            name: "temp_result".to_string(),
                                            result: elem_result.clone()
                                        });
                                    }
                                }
                                element_names.push(elem_result);
                            }

                            // Create list from elements
                            instructions.push(IRInstruction::ListCreate {
                                elements: element_names,
                                result: arg_result.clone()
                            });
                        },
                        Expr::Tuple(elements) => {
                            // Handle tuple literal: (item1, item2, ...)
                            let mut element_names = Vec::new();
                            for (j, elem) in elements.iter().enumerate() {
                                let elem_result = format!("{}_elem_{}", arg_result, j);
                                match elem {
                                    Expr::Literal(lit) => {
                                        instructions.push(IRInstruction::LoadConst {
                                            value: self.literal_to_value(&lit),
                                            result: elem_result.clone()
                                        });
                                    },
                                    Expr::Identifier(name) => {
                                        instructions.push(IRInstruction::LoadGlobal {
                                            name: name.clone(),
                                            result: elem_result.clone()
                                        });
                                    },
                                    _ => {
                                        // For complex expressions, recursively process
                                        self.process_expression_for_instructions(instructions, elem)?;
                                        instructions.push(IRInstruction::LoadGlobal {
                                            name: "temp_result".to_string(),
                                            result: elem_result.clone()
                                        });
                                    }
                                }
                                element_names.push(elem_result);
                            }

                            // Create tuple from elements
                            instructions.push(IRInstruction::Call {
                                func: "tuple".to_string(),
                                args: element_names,
                                result: Some(arg_result.clone())
                            });
                        },
                        Expr::MethodCall { object, method, args: method_args, kwargs: _ } => {
                            // Handle method call as an argument
                            let object_name = self.expression_to_string(&object);
                            
                            // Determine the class name for method resolution
                            let class_name = self.object_types.get(&object_name)
                                .cloned()
                                .unwrap_or_else(|| {
                                    // Try to infer from the variable name
                                    if let Expr::Identifier(name) = object.as_ref() {
                                        // Capitalize first letter as a guess
                                        let mut chars = name.chars();
                                        match chars.next() {
                                            None => name.clone(),
                                            Some(first) => first.to_uppercase().collect::<String>() + chars.as_str(),
                                        }
                                    } else {
                                        object_name.clone()
                                    }
                                });
                            
                            // Prepare method arguments
                            let mut method_arg_names = vec![object_name.clone()];
                            for (j, method_arg) in method_args.iter().enumerate() {
                                let method_arg_result = format!("{}_method_arg_{}", arg_result, j);
                                match method_arg {
                                    Expr::Literal(lit) => {
                                        instructions.push(IRInstruction::LoadConst {
                                            value: self.literal_to_value(&lit),
                                            result: method_arg_result.clone()
                                        });
                                    },
                                    Expr::Identifier(name) => {
                                        instructions.push(IRInstruction::LoadGlobal {
                                            name: name.clone(),
                                            result: method_arg_result.clone()
                                        });
                                    },
                                    _ => {
                                        instructions.push(IRInstruction::LoadConst {
                                            value: Value::None,
                                            result: method_arg_result.clone()
                                        });
                                    }
                                }
                                method_arg_names.push(method_arg_result);
                            }
                            
                            // Generate method call with class name prefix
                            let method_call_name = format!("{}__{}",class_name, method);
                            instructions.push(IRInstruction::Call {
                                func: method_call_name,
                                args: method_arg_names,
                                result: Some(arg_result.clone())
                            });
                        },
                        Expr::FormatString { parts } => {
                            // Handle f-string as an argument
                            self.process_expression_for_instructions(instructions, arg)?;
                            // Move the result from temp_result to arg_result
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: arg_result.clone()
                            });
                        },
                        _ => {
                            // For other complex expressions (like comparisons, complex binary ops),
                            // process them using process_expression_for_instructions
                            self.process_expression_for_instructions(instructions, arg)?;
                            // Move the result from temp_result to arg_result
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: arg_result.clone()
                            });
                        }
                    }
                    arg_names.push(arg_result);
                }

                // Check if this is a setattr() call - special handling
                if func_name == "setattr" && arg_names.len() == 3 && args.len() == 3 {
                    // Check if the second argument is a string literal
                    if let Expr::Literal(Literal::String(attr_name)) = &args[1] {
                        // setattr(obj, 'attr_name', value) -> ObjectSetAttr (optimized case)
                        instructions.push(IRInstruction::ObjectSetAttr {
                            object: arg_names[0].clone(),
                            attr: attr_name.clone(),
                            value: arg_names[2].clone()
                        });
                        // setattr returns None
                        instructions.push(IRInstruction::LoadConst {
                            value: Value::None,
                            result: "temp_result".to_string()
                        });
                    } else {
                        // Dynamic attribute name - use regular function call
                        instructions.push(IRInstruction::Call {
                            func: func_name,
                            args: arg_names,
                            result: Some("temp_result".to_string())
                        });
                    }
                } else if func_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                    // This looks like object creation
                    let class_name = func_name.clone();
                    instructions.push(IRInstruction::ObjectCreate {
                        class_name: class_name.clone(),
                        result: "temp_result".to_string()
                    });
                    // Record the object type for method call resolution
                    self.object_types.insert("temp_result".to_string(), class_name.clone());
                    
                    // If there are arguments, call __init__ with them
                    if !arg_names.is_empty() {
                        // Create the __init__ method name (ClassName__init__)
                        let init_method_name = format!("{}__init__", class_name);
                        
                        // Call __init__ with object as first argument
                        // Note: __init__ returns None, so we don't capture the result
                        let mut init_args = vec!["temp_result".to_string()];
                        init_args.extend(arg_names);
                        
                        instructions.push(IRInstruction::Call {
                            func: init_method_name,
                            args: init_args,
                            result: None  // Don't capture __init__ return value
                        });
                    }
                } else {
                    // Regular function call
                    instructions.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some("temp_result".to_string())
                    });
                }
            },
            Expr::List(elements) => {
                // Handle list literal: [item1, item2, ...]
                let mut element_names = Vec::new();
                for (i, elem) in elements.iter().enumerate() {
                    let elem_result = format!("temp_elem_{}", i);
                    match elem {
                        Expr::Literal(lit) => {
                            instructions.push(IRInstruction::LoadConst {
                                value: self.literal_to_value(&lit),
                                result: elem_result.clone()
                            });
                        },
                        Expr::Identifier(name) => {
                            instructions.push(IRInstruction::LoadGlobal {
                                name: name.clone(),
                                result: elem_result.clone()
                            });
                        },
                        _ => {
                            // For complex expressions, recursively process
                            self.process_expression_for_instructions(instructions, elem)?;
                            instructions.push(IRInstruction::LoadGlobal {
                                name: "temp_result".to_string(),
                                result: elem_result.clone()
                            });
                        }
                    }
                    element_names.push(elem_result);
                }

                // Create list from elements
                instructions.push(IRInstruction::ListCreate {
                    elements: element_names,
                    result: "temp_result".to_string()
                });
            },
            Expr::Dict(pairs) => {
                // Handle dict literal: {key1: value1, key2: value2, ...}
                let mut pair_names = Vec::new();
                for (i, item) in pairs.iter().enumerate() {
                    match item {
                        crate::ast::DictItem::KeyValue(key, value) => {
                            let key_result = format!("temp_key_{}", i);
                            let value_result = format!("temp_value_{}", i);

                            // Process key
                            match key {
                                Expr::Literal(lit) => {
                                    instructions.push(IRInstruction::LoadConst {
                                        value: self.literal_to_value(&lit),
                                        result: key_result.clone()
                                    });
                                },
                                Expr::Identifier(name) => {
                                    instructions.push(IRInstruction::LoadGlobal {
                                        name: name.clone(),
                                        result: key_result.clone()
                                    });
                                },
                                _ => {
                                    self.process_expression_for_instructions(instructions, key)?;
                                    instructions.push(IRInstruction::LoadGlobal {
                                        name: "temp_result".to_string(),
                                        result: key_result.clone()
                                    });
                                }
                            }

                            // Process value
                            match value {
                                Expr::Literal(lit) => {
                                    instructions.push(IRInstruction::LoadConst {
                                        value: self.literal_to_value(&lit),
                                        result: value_result.clone()
                                    });
                                },
                                Expr::Identifier(name) => {
                                    instructions.push(IRInstruction::LoadGlobal {
                                        name: name.clone(),
                                        result: value_result.clone()
                                    });
                                },
                                _ => {
                                    self.process_expression_for_instructions(instructions, value)?;
                                    instructions.push(IRInstruction::LoadGlobal {
                                        name: "temp_result".to_string(),
                                        result: value_result.clone()
                                    });
                                }
                            }

                            pair_names.push((key_result, value_result));
                        },
                        crate::ast::DictItem::Unpacking(_expr) => {
                            // For now, skip unpacking
                        }
                    }
                }

                // Create dict from key-value pairs
                instructions.push(IRInstruction::DictCreate {
                    pairs: pair_names,
                    result: "temp_result".to_string()
                });
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
            Expr::BinaryOp { op, left, right } => {
                // Handle binary operations (arithmetic, etc.)
                let left_temp = "temp_left".to_string();
                let right_temp = "temp_right".to_string();

                // Evaluate left and right
                self.process_expression_to_result(module, left, &left_temp)?;
                self.process_expression_to_result(module, right, &right_temp)?;

                // Generate the binary operation
                module.globals.push(IRInstruction::BinaryOp {
                    op: op.clone(),
                    left: left_temp,
                    right: right_temp,
                    result: "temp".to_string()
                });
            },
            Expr::Compare { left, ops, comparators } => {
                // Handle comparison operations (e.g., i < 3, x == y)
                // For now, handle simple single comparisons
                if ops.len() == 1 && comparators.len() == 1 {
                    let left_temp = "temp_left".to_string();
                    let right_temp = "temp_right".to_string();

                    // Evaluate left and right
                    self.process_expression_to_result(module, left, &left_temp)?;
                    self.process_expression_to_result(module, &comparators[0], &right_temp)?;

                    // Convert CompareOp to BinaryOp
                    let binary_op = match &ops[0] {
                        CompareOp::Eq => BinaryOp::Eq,
                        CompareOp::NotEq => BinaryOp::Ne,
                        CompareOp::Lt => BinaryOp::Lt,
                        CompareOp::LtE => BinaryOp::Le,
                        CompareOp::Gt => BinaryOp::Gt,
                        CompareOp::GtE => BinaryOp::Ge,
                        _ => BinaryOp::Eq, // Fallback
                    };

                    // Generate the comparison operation
                    module.globals.push(IRInstruction::BinaryOp {
                        op: binary_op,
                        left: left_temp,
                        right: right_temp,
                        result: "temp".to_string()
                    });
                } else {
                    // Complex chained comparisons - generate None for now
                    module.globals.push(IRInstruction::LoadConst {
                        value: Value::None,
                        result: "temp".to_string()
                    });
                }
            },
            Expr::Attribute { object, name } => {
                // Process attribute access
                // First process the object expression to get a proper variable
                self.process_expression(module, &object)?;
                let object_var = "temp_attr_object".to_string();
                module.globals.push(IRInstruction::LoadGlobal {
                    name: "temp".to_string(),
                    result: object_var.clone()
                });

                module.globals.push(IRInstruction::ObjectGetAttr {
                    object: object_var,
                    attr: name.clone(),
                    result: "temp".to_string()
                });
            },
            Expr::MethodCall { object, method, args, .. } => {
                // Process method call
                let object_name = self.expression_to_string(&object);

                // Check if this is a module function call vs a method call
                if self.imported_modules.contains(&object_name) {
                    // Module function call: module.function() -> module_function()
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("method_arg_{}", i);
                        self.process_expression_to_result(module, arg, &arg_result)?;
                        arg_names.push(arg_result);
                    }

                    // Call module function directly (no self argument)
                    let func_name = format!("{}_{}", object_name, method);
                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some("temp".to_string())
                    });
                } else if ["upper", "lower", "strip", "lstrip", "rstrip", "split", "join", 
                    "replace", "startswith", "endswith", "find", "title", "capitalize", "swapcase",
                    "isdigit", "isalpha", "isalnum", "isspace", "isupper", "islower", "count", 
                    "center", "ljust", "rjust", "zfill",
                    "append", "pop", "insert", "remove", "extend", "index", "reverse", "sort", "copy", "clear"].contains(&method.as_str()) {
                    // Built-in methods for strings and lists
                    let object_var = "temp_object".to_string();
                    self.process_expression_to_result(module, &object, &object_var)?;

                    // Process each argument and collect their result names
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("method_arg_{}", i);
                        self.process_expression_to_result(module, arg, &arg_result)?;
                        arg_names.push(arg_result);
                    }

                    // Built-in methods use text__ or lst__ prefix based on method name
                    let func_prefix = if ["append", "pop", "insert", "remove", "extend", "index", "reverse", "sort", "copy", "clear"].contains(&method.as_str()) { "lst" } else { "text" };
                    let func_name = format!("{}__{}", func_prefix, method);
                    let mut method_args = vec![object_var];
                    method_args.extend(arg_names);

                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: method_args,
                        result: Some("temp".to_string())
                    });
                } else {
                    // Object method call: obj.method() -> ClassName__method(obj, ...)
                    // First process the object expression to get a proper variable
                    let object_var = "temp_object".to_string();
                    self.process_expression_to_result(module, &object, &object_var)?;

                    // Get the class name for this object
                    // First try object_var (the processed variable), then fall back to object_name
                    let class_name = self.object_types.get(&object_var)
                        .or_else(|| self.object_types.get(&object_name))
                        .cloned()
                        .unwrap_or_else(|| object_name.clone());

                    // Find which class actually defines this method (may be in a parent class)
                    let defining_class = self.find_method_class(&class_name, method);

                    // Process each argument and collect their result names
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("method_arg_{}", i);
                        self.process_expression_to_result(module, arg, &arg_result)?;
                        arg_names.push(arg_result);
                    }

                    // Create the method name using the class that actually defines it
                    let method_name = format!("{}__{}", defining_class, method);

                    // Call the method with object variable as first argument
                    let mut method_args = vec![object_var];
                    method_args.extend(arg_names);

                    module.globals.push(IRInstruction::Call {
                        func: method_name,
                        args: method_args,
                        result: Some("temp".to_string())
                    });

                    // If this returns an object of a known type, track it
                    // For method chaining: builder.add(5) returns Builder
                    if class_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                        // If the class name looks like a class, assume method returns same type
                        self.object_types.insert("temp".to_string(), class_name.clone());
                    }
                }
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

                // Check if this is a setattr() call - special handling
                if func_name == "setattr" && arg_names.len() == 3 && args.len() == 3 {
                    // Check if the second argument is a string literal
                    if let Expr::Literal(Literal::String(attr_name)) = &args[1] {
                        // setattr(obj, 'attr_name', value) -> ObjectSetAttr (optimized case)
                        module.globals.push(IRInstruction::ObjectSetAttr {
                            object: arg_names[0].clone(),
                            attr: attr_name.clone(),
                            value: arg_names[2].clone()
                        });
                        // setattr returns None
                        module.globals.push(IRInstruction::LoadConst {
                            value: Value::None,
                            result: "temp".to_string()
                        });
                    } else {
                        // Dynamic attribute name - use regular function call
                        module.globals.push(IRInstruction::Call {
                            func: func_name,
                            args: arg_names,
                            result: Some("temp".to_string())
                        });
                    }
                }
                // Check if this is a super() call
                else if func_name == "super" {
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
                    self.object_types.insert("temp".to_string(), class_name.clone());
                    
                    // Check if the class has an __init__ method defined (directly or inherited)
                    let init_class = self.find_method_class(&class_name, "__init__");
                    if init_class != class_name || self.class_methods.get(&class_name)
                        .map(|m| m.contains("__init__"))
                        .unwrap_or(false) {
                        // Only call __init__ if it exists somewhere in the hierarchy
                        // Method names use ClassName__method format (2 underscores)
                        let init_func = format!("{}__init__", init_class);
                        let mut init_args = vec!["temp".to_string()];
                        init_args.extend(arg_names);
                        
                        module.globals.push(IRInstruction::Call {
                            func: init_func,
                            args: init_args,
                            result: None
                        });
                    }
                } else {
                    // Regular function call
                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some("temp".to_string())
                    });
                }
            },
            Expr::List(elements) => {
                // Handle list literal: [item1, item2, ...]
                let mut element_names = Vec::new();
                for (i, elem) in elements.iter().enumerate() {
                    let elem_result = format!("temp_elem_{}", i);
                    self.process_expression_to_result(module, elem, &elem_result)?;
                    element_names.push(elem_result);
                }

                // Create list from elements
                module.globals.push(IRInstruction::ListCreate {
                    elements: element_names,
                    result: "temp".to_string()
                });
            },
            Expr::Dict(pairs) => {
                // Handle dict literal: {key1: value1, key2: value2, ...}
                let mut pair_names = Vec::new();
                for (i, item) in pairs.iter().enumerate() {
                    match item {
                        crate::ast::DictItem::KeyValue(key, value) => {
                            let key_result = format!("temp_key_{}", i);
                            let value_result = format!("temp_value_{}", i);

                            // Recursively process key and value to handle nested structures
                            self.process_expression_to_result(module, key, &key_result)?;
                            self.process_expression_to_result(module, value, &value_result)?;

                            pair_names.push((key_result, value_result));
                        },
                        crate::ast::DictItem::Unpacking(_expr) => {
                            // For now, skip unpacking - would need special handling
                            // This would require merging dicts at runtime
                        }
                    }
                }

                // Create dict from key-value pairs
                module.globals.push(IRInstruction::DictCreate {
                    pairs: pair_names,
                    result: "temp".to_string()
                });
            },
            Expr::Tuple(elements) => {
                // Handle tuple literal: (item1, item2, ...)
                let mut element_names = Vec::new();
                for (i, elem) in elements.iter().enumerate() {
                    let elem_result = format!("temp_elem_{}", i);
                    self.process_expression_to_result(module, elem, &elem_result)?;
                    element_names.push(elem_result);
                }

                // Create tuple from elements using tuple() function
                module.globals.push(IRInstruction::Call {
                    func: "tuple".to_string(),
                    args: element_names,
                    result: Some("temp".to_string())
                });
            },
            Expr::Subscript { object, index } => {
                // Handle subscript: obj[index]
                let object_var = "temp_object".to_string();
                let index_var = "temp_index".to_string();
                
                // Process object and index
                self.process_expression_to_result(module, object, &object_var)?;
                self.process_expression_to_result(module, index, &index_var)?;
                
                // Generate DictGetItem instruction (works for both dicts and list indexing)
                module.globals.push(IRInstruction::DictGetItem {
                    dict: object_var,
                    key: index_var,
                    result: "temp".to_string()
                });
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
            Expr::UnaryOp { op, operand } => {
                let operand_temp = format!("{}_operand", result_var);
                
                // Recursively evaluate the operand
                self.process_expression_to_result(module, operand, &operand_temp)?;
                
                // Generate unary operation
                module.globals.push(IRInstruction::UnaryOp {
                    op: op.clone(),
                    operand: operand_temp,
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

                // Check if this is a setattr() call - special handling
                if func_name == "setattr" && arg_names.len() == 3 && args.len() == 3 {
                    // Check if the second argument is a string literal
                    if let Expr::Literal(Literal::String(attr_name)) = &args[1] {
                        // setattr(obj, 'attr_name', value) -> ObjectSetAttr (optimized case)
                        module.globals.push(IRInstruction::ObjectSetAttr {
                            object: arg_names[0].clone(),
                            attr: attr_name.clone(),
                            value: arg_names[2].clone()
                        });
                        // setattr returns None
                        module.globals.push(IRInstruction::LoadConst {
                            value: Value::None,
                            result: result_var.to_string()
                        });
                    } else {
                        // Dynamic attribute name - use regular function call
                        module.globals.push(IRInstruction::Call {
                            func: func_name,
                            args: arg_names,
                            result: Some(result_var.to_string())
                        });
                    }
                }
                // Check if this is a class instantiation
                else if func_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                    // This looks like object creation
                    let class_name = func_name.clone();
                    module.globals.push(IRInstruction::ObjectCreate {
                        class_name: class_name.clone(),
                        result: result_var.to_string()
                    });
                    // Record the object type for method call resolution
                    self.object_types.insert(result_var.to_string(), class_name.clone());
                    
                    // Check if the class has an __init__ method defined (directly or inherited)
                    let init_class = self.find_method_class(&class_name, "__init__");
                    if init_class != class_name || self.class_methods.get(&class_name)
                        .map(|m| m.contains("__init__"))
                        .unwrap_or(false) {
                        // Only call __init__ if it exists somewhere in the hierarchy
                        // Method names use ClassName__method format (2 underscores)
                        let init_func = format!("{}__init__", init_class);
                        let mut init_args = vec![result_var.to_string()];
                        init_args.extend(arg_names);
                        
                        module.globals.push(IRInstruction::Call {
                            func: init_func,
                            args: init_args,
                            result: None
                        });
                    }
                } else {
                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: arg_names,
                        result: Some(result_var.to_string())
                    });
                }
            },
            Expr::MethodCall { object, method, args, .. } => {
                // Check if this is a super() method call
                let is_super_call = matches!(object.as_ref(), Expr::Call { func, .. }
                    if matches!(func.as_ref(), Expr::Identifier(name) if name == "super"));

                if is_super_call {
                    // Handle super().method() call
                    if let Some(current_class) = &self.current_class {
                        // Get the base class name
                        let base_classes = self.class_inheritance.get(current_class).cloned().unwrap_or_default();
                        let parent_class = base_classes.first().cloned().unwrap_or_else(|| "object".to_string());

                        // Process arguments
                        let mut arg_names: Vec<String> = Vec::new();
                        for (i, arg) in args.iter().enumerate() {
                            let arg_result = format!("{}_method_arg_{}", result_var, i);
                            self.process_expression_to_result(module, arg, &arg_result)?;
                            arg_names.push(arg_result);
                        }

                        // Create the parent method name (ParentClass__method)
                        let method_name = format!("{}__{}", parent_class, method);

                        // Call the parent method with self as first argument
                        let mut method_args = vec!["self".to_string()];
                        method_args.extend(arg_names);

                        module.globals.push(IRInstruction::Call {
                            func: method_name,
                            args: method_args,
                            result: Some(result_var.to_string())
                        });
                    } else {
                        // super() called outside of a class - error
                        module.globals.push(IRInstruction::LoadConst {
                            value: Value::None,
                            result: result_var.to_string()
                        });
                    }
                } else if ["upper", "lower", "strip", "lstrip", "rstrip", "split", "join", 
                    "replace", "startswith", "endswith", "find", "title", "capitalize", "swapcase",
                    "isdigit", "isalpha", "isalnum", "isspace", "isupper", "islower", "count", 
                    "center", "ljust", "rjust", "zfill",
                    "append", "pop", "insert", "remove", "extend", "index", "reverse", "sort", "copy", "clear"].contains(&method.as_str()) {
                    // Built-in methods for strings and lists
                    let object_var = format!("{}_object", result_var);
                    self.process_expression_to_result(module, &object, &object_var)?;

                    // Process each argument and collect their result names
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("{}_method_arg_{}", result_var, i);
                        self.process_expression_to_result(module, arg, &arg_result)?;
                        arg_names.push(arg_result);
                    }

                    // Built-in methods use text__ or lst__ prefix based on method name
                    let func_prefix = if ["append", "pop", "insert", "remove", "extend", "index", "reverse", "sort", "copy", "clear"].contains(&method.as_str()) { "lst" } else { "text" };
                    let func_name = format!("{}__{}", func_prefix, method);
                    let mut method_args = vec![object_var];
                    method_args.extend(arg_names);

                    module.globals.push(IRInstruction::Call {
                        func: func_name,
                        args: method_args,
                        result: Some(result_var.to_string())
                    });
                } else {
                    // Normal method call
                    // First process the object expression to get a proper variable
                    let object_var = format!("{}_object", result_var);
                    self.process_expression_to_result(module, &object, &object_var)?;

                    let object_name = self.expression_to_string(&object);

                    // Get the class name for this object
                    // First try object_var (the processed variable), then fall back to object_name
                    let class_name = self.object_types.get(&object_var)
                        .or_else(|| self.object_types.get(&object_name))
                        .cloned()
                        .unwrap_or_else(|| object_name.clone());

                    // Find which class actually defines this method (may be in a parent class)
                    let defining_class = self.find_method_class(&class_name, method);

                    // Process each argument and collect their result names
                    let mut arg_names: Vec<String> = Vec::new();
                    for (i, arg) in args.iter().enumerate() {
                        let arg_result = format!("{}_method_arg_{}", result_var, i);
                        self.process_expression_to_result(module, arg, &arg_result)?;
                        arg_names.push(arg_result);
                    }

                    // Create the method name using the class that actually defines it
                    let method_name = format!("{}__{}", defining_class, method);

                    // Call the method with object variable as first argument
                    let mut method_args = vec![object_var];
                    method_args.extend(arg_names);

                    module.globals.push(IRInstruction::Call {
                        func: method_name,
                        args: method_args,
                        result: Some(result_var.to_string())
                    });

                    // If this returns an object of a known type, track it
                    // For method chaining: builder.add(5) returns Builder
                    if let Some(obj_class) = self.object_types.get(&class_name) {
                        self.object_types.insert(result_var.to_string(), obj_class.clone());
                    } else if class_name.chars().next().map(|c| c.is_uppercase()).unwrap_or(false) {
                        // If the class name looks like a class, assume method returns same type
                        self.object_types.insert(result_var.to_string(), class_name.clone());
                    }
                }
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
            Expr::Attribute { object, name } => {
                // Handle attribute access: object.attribute
                // First process the object expression to get a proper variable
                let object_var = format!("{}_attr_object", result_var);
                self.process_expression_to_result(module, &object, &object_var)?;

                module.globals.push(IRInstruction::ObjectGetAttr {
                    object: object_var,
                    attr: name.clone(),
                    result: result_var.to_string()
                });
            },
            Expr::List(elements) => {
                // Handle list literal: [item1, item2, ...]
                let mut element_names = Vec::new();
                for (i, elem) in elements.iter().enumerate() {
                    let elem_result = format!("{}_elem_{}", result_var, i);
                    self.process_expression_to_result(module, elem, &elem_result)?;
                    element_names.push(elem_result);
                }

                // Create list from elements
                module.globals.push(IRInstruction::ListCreate {
                    elements: element_names,
                    result: result_var.to_string()
                });
            },
            Expr::Tuple(elements) => {
                // Handle tuple literal: (item1, item2, ...)
                let mut element_names = Vec::new();
                for (i, elem) in elements.iter().enumerate() {
                    let elem_result = format!("{}_elem_{}", result_var, i);
                    self.process_expression_to_result(module, elem, &elem_result)?;
                    element_names.push(elem_result);
                }

                // Create tuple from elements
                module.globals.push(IRInstruction::Call {
                    func: "tuple".to_string(),
                    args: element_names,
                    result: Some(result_var.to_string())
                });
            },
            Expr::Subscript { object, index } => {
                // Handle subscript: obj[index]
                let object_var = format!("{}_object", result_var);
                let index_var = format!("{}_index", result_var);
                
                // Process object and index
                self.process_expression_to_result(module, object, &object_var)?;
                self.process_expression_to_result(module, index, &index_var)?;
                
                // Generate DictGetItem instruction (works for both dicts and list indexing)
                // Transpiler will handle based on actual type
                module.globals.push(IRInstruction::DictGetItem {
                    dict: object_var,
                    key: index_var,
                    result: result_var.to_string()
                });
            },
            Expr::Slice { object, start, stop, step } => {
                // Handle slicing: obj[start:stop:step]
                let object_var = format!("{}_object", result_var);
                
                // Process object
                self.process_expression_to_result(module, object, &object_var)?;
                
                // Process start, stop, step if present
                let start_var = if let Some(s) = start {
                    let var = format!("{}_start", result_var);
                    self.process_expression_to_result(module, s, &var)?;
                    Some(var)
                } else {
                    None
                };
                
                let stop_var = if let Some(s) = stop {
                    let var = format!("{}_stop", result_var);
                    self.process_expression_to_result(module, s, &var)?;
                    Some(var)
                } else {
                    None
                };
                
                let step_var = if let Some(s) = step {
                    let var = format!("{}_step", result_var);
                    self.process_expression_to_result(module, s, &var)?;
                    Some(var)
                } else {
                    None
                };
                
                // Generate Slice instruction
                module.globals.push(IRInstruction::Slice {
                    object: object_var,
                    start: start_var,
                    stop: stop_var,
                    step: step_var,
                    result: result_var.to_string()
                });
            },
            Expr::ListComp { element, generators } => {
                // Handle list comprehension: [expr for var in iterable if condition]
                if generators.is_empty() {
                    module.globals.push(IRInstruction::ListCreate {
                        elements: vec![],
                        result: result_var.to_string()
                    });
                } else {
                    let first_gen = &generators[0];
                    
                    // Process the iterable
                    let iterable_var = format!("{}_iterable", result_var);
                    self.process_expression_to_result(module, &first_gen.iter, &iterable_var)?;
                    
                    // Compile element expression to IR instructions
                    // Create a temporary compiler state to compile element expression with loop variable available
                    let element_result_var = format!("{}_elem", result_var);
                    let mut element_instrs = Vec::new();
                    
                    // Compile element expression directly (loop variable will be available as a LoadGlobal in the loop)
                    let saved_len = module.globals.len();
                    self.process_expression_to_result(module, element, &element_result_var)?;
                    
                    // Extract the instructions that were just added
                    element_instrs = module.globals.drain(saved_len..).collect();
                    
                    // Compile condition if present
                    let mut condition_instrs = Vec::new();
                    let condition_result_var = if !first_gen.ifs.is_empty() {
                        let cond_var = format!("{}_cond", result_var);
                        let saved_len = module.globals.len();
                        self.process_expression_to_result(module, &first_gen.ifs[0], &cond_var)?;
                        condition_instrs = module.globals.drain(saved_len..).collect();
                        Some(cond_var)
                    } else {
                        None
                    };
                    
                    // Add the comprehension instruction
                    module.globals.push(IRInstruction::ListComprehension {
                        element_instrs,
                        element_result: element_result_var,
                        variable: first_gen.target.clone(),
                        iterable: iterable_var,
                        condition_instrs,
                        condition_result: condition_result_var,
                        result: result_var.to_string()
                    });
                }
            },
            Expr::DictComp { key, value, generators } => {
                // Handle dict comprehension: {k: v for var in iterable if condition}
                if generators.is_empty() {
                    module.globals.push(IRInstruction::DictCreate {
                        pairs: vec![],
                        result: result_var.to_string()
                    });
                } else {
                    let first_gen = &generators[0];
                    
                    // Process the iterable
                    let iterable_var = format!("{}_iterable", result_var);
                    self.process_expression_to_result(module, &first_gen.iter, &iterable_var)?;
                    
                    // Compile key expression
                    let key_result_var = format!("{}_key", result_var);
                    let saved_len = module.globals.len();
                    self.process_expression_to_result(module, key, &key_result_var)?;
                    let key_instrs = module.globals.drain(saved_len..).collect();
                    
                    // Compile value expression
                    let value_result_var = format!("{}_value", result_var);
                    let saved_len = module.globals.len();
                    self.process_expression_to_result(module, value, &value_result_var)?;
                    let value_instrs = module.globals.drain(saved_len..).collect();
                    
                    // Compile condition if present
                    let mut condition_instrs = Vec::new();
                    let condition_result_var = if !first_gen.ifs.is_empty() {
                        let cond_var = format!("{}_cond", result_var);
                        let saved_len = module.globals.len();
                        self.process_expression_to_result(module, &first_gen.ifs[0], &cond_var)?;
                        condition_instrs = module.globals.drain(saved_len..).collect();
                        Some(cond_var)
                    } else {
                        None
                    };
                    
                    // Add the comprehension instruction
                    module.globals.push(IRInstruction::DictComprehension {
                        key_instrs,
                        key_result: key_result_var,
                        value_instrs,
                        value_result: value_result_var,
                        variable: first_gen.target.clone(),
                        iterable: iterable_var,
                        condition_instrs,
                        condition_result: condition_result_var,
                        result: result_var.to_string()
                    });
                }
            },
            Expr::GeneratorExp { element, generators } => {
                // Handle generator expression: (expr for var in iterable if condition)
                // Generators in Python return an iterator that lazily evaluates elements
                // For now, implement as creating a list (eager evaluation) and wrapping it as an iterator
                // This is a simplification but matches the comprehension approach
                if generators.is_empty() {
                    module.globals.push(IRInstruction::ListCreate {
                        elements: vec![],
                        result: result_var.to_string()
                    });
                } else {
                    let first_gen = &generators[0];
                    
                    // Process the iterable
                    let iterable_var = format!("{}_iterable", result_var);
                    self.process_expression_to_result(module, &first_gen.iter, &iterable_var)?;
                    
                    // Compile element expression to IR instructions
                    let element_result_var = format!("{}_elem", result_var);
                    let mut element_instrs = Vec::new();
                    
                    let saved_len = module.globals.len();
                    self.process_expression_to_result(module, element, &element_result_var)?;
                    element_instrs = module.globals.drain(saved_len..).collect();
                    
                    // Compile condition if present
                    let mut condition_instrs = Vec::new();
                    let condition_result_var = if !first_gen.ifs.is_empty() {
                        let cond_var = format!("{}_cond", result_var);
                        let saved_len = module.globals.len();
                        self.process_expression_to_result(module, &first_gen.ifs[0], &cond_var)?;
                        condition_instrs = module.globals.drain(saved_len..).collect();
                        Some(cond_var)
                    } else {
                        None
                    };
                    
                    // For generators, we create them as iterators (for now, as lists)
                    // Use ListComprehension and mark result as iterator type
                    module.globals.push(IRInstruction::ListComprehension {
                        element_instrs,
                        element_result: element_result_var,
                        variable: first_gen.target.clone(),
                        iterable: iterable_var,
                        condition_instrs,
                        condition_result: condition_result_var,
                        result: result_var.to_string()
                    });
                }
            },
            Expr::Dict(pairs) => {
                // Handle dict literal: {key1: value1, key2: value2, ...}
                let mut pair_names = Vec::new();
                for (i, item) in pairs.iter().enumerate() {
                    match item {
                        crate::ast::DictItem::KeyValue(key, value) => {
                            let key_result = format!("{}_key_{}", result_var, i);
                            let value_result = format!("{}_value_{}", result_var, i);

                            // Recursively process key and value to handle nested structures
                            self.process_expression_to_result(module, key, &key_result)?;
                            self.process_expression_to_result(module, value, &value_result)?;

                            pair_names.push((key_result, value_result));
                        },
                        crate::ast::DictItem::Unpacking(_expr) => {
                            // For now, skip unpacking - would need special handling
                            // This would require merging dicts at runtime
                        }
                    }
                }

                // Create dict from key-value pairs
                module.globals.push(IRInstruction::DictCreate {
                    pairs: pair_names,
                    result: result_var.to_string()
                });
            },
            Expr::Compare { left, ops, comparators } => {
                // Handle comparison operations (>, <, ==, etc.)
                if ops.len() == 1 && comparators.len() == 1 {
                    let left_temp = format!("{}_left", result_var);
                    let right_temp = format!("{}_right", result_var);

                    // Recursively evaluate left and right
                    self.process_expression_to_result(module, left, &left_temp)?;
                    self.process_expression_to_result(module, &comparators[0], &right_temp)?;

                    // Convert CompareOp to BinaryOp
                    let binary_op = match &ops[0] {
                        CompareOp::Eq => BinaryOp::Eq,
                        CompareOp::NotEq => BinaryOp::Ne,
                        CompareOp::Lt => BinaryOp::Lt,
                        CompareOp::LtE => BinaryOp::Le,
                        CompareOp::Gt => BinaryOp::Gt,
                        CompareOp::GtE => BinaryOp::Ge,
                        _ => BinaryOp::Eq, // fallback
                    };

                    // Generate binary operation for comparison
                    module.globals.push(IRInstruction::BinaryOp {
                        op: binary_op,
                        left: left_temp,
                        right: right_temp,
                        result: result_var.to_string()
                    });
                } else {
                    // Multiple comparisons (chained) - not yet supported
                    // For now, fallback to None
                    module.globals.push(IRInstruction::LoadConst {
                        value: Value::None,
                        result: result_var.to_string()
                    });
                }
            },
            Expr::Lambda { params, body } => {
                // Lambda expression: lambda x, y: x + y
                // Store a simple Lambda IR instruction that the transpiler will handle
                
                // Record the current length to know what instructions are added for the body
                let start_len = module.globals.len();
                
                // Process the lambda body into IR instructions
                let body_result_var = format!("{}_result", result_var);
                self.process_expression_to_result(module, body, &body_result_var)?;
                
                // Drain the instructions that were added for the body
                let body_instrs: Vec<IRInstruction> = module.globals.drain(start_len..).collect();
                
                // Create the Lambda IR instruction
                module.globals.push(IRInstruction::Lambda {
                    params: params.iter().map(|p| p.name.clone()).collect(),
                    body_instructions: body_instrs,
                    captured_vars: Vec::new(), // No captured variables for now
                    result: result_var.to_string(),
                    body_result_var: body_result_var.to_string(),
                });
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