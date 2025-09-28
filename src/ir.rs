use crate::ast::*;
use std::collections::HashMap;
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone)]
pub struct IRModule {
    pub name: String,
    pub functions: HashMap<String, IRFunction>,
    pub globals: Vec<IRGlobal>,
    pub types: HashMap<String, IRType>,
}

#[derive(Debug, Clone)]
pub struct IRFunction {
    pub name: String,
    pub params: Vec<IRParam>,
    pub return_type: IRType,
    pub blocks: Vec<IRBlock>,
    pub is_exported: bool,
    pub is_extern: bool,
    pub is_async: bool,
}

#[derive(Debug, Clone)]
pub struct IRParam {
    pub name: String,
    pub ty: IRType,
}

#[derive(Debug, Clone)]
pub struct IRBlock {
    pub label: String,
    pub instructions: Vec<IRInstruction>,
}

#[derive(Debug, Clone)]
pub enum IRInstruction {
    // Arithmetic
    Add { dest: String, left: IRValue, right: IRValue },
    Sub { dest: String, left: IRValue, right: IRValue },
    Mul { dest: String, left: IRValue, right: IRValue },
    Div { dest: String, left: IRValue, right: IRValue },
    Mod { dest: String, left: IRValue, right: IRValue },
    Pow { dest: String, left: IRValue, right: IRValue },
    FloorDiv { dest: String, left: IRValue, right: IRValue },
    
    // Memory
    Alloca { dest: String, ty: IRType },
    Store { value: IRValue, ptr: String },
    Load { dest: String, ptr: String, ty: IRType },
    
    // Control flow
    Br { cond: IRValue, then_label: String, else_label: String },
    Jmp { label: String },
    Label(String),
    Ret { value: Option<IRValue> },
    
    // Function calls
    Call { dest: Option<String>, func: String, args: Vec<IRValue> },
    
    // Comparisons
    CmpEq { dest: String, left: IRValue, right: IRValue },
    CmpNe { dest: String, left: IRValue, right: IRValue },
    CmpLt { dest: String, left: IRValue, right: IRValue },
    CmpGt { dest: String, left: IRValue, right: IRValue },
    CmpLe { dest: String, left: IRValue, right: IRValue },
    CmpGe { dest: String, left: IRValue, right: IRValue },
    
    // Logical operations
    And { dest: String, left: IRValue, right: IRValue },
    Or { dest: String, left: IRValue, right: IRValue },
    Not { dest: String, operand: IRValue },
    
    // Bitwise operations
    BitAnd { dest: String, left: IRValue, right: IRValue },
    BitOr { dest: String, left: IRValue, right: IRValue },
    BitXor { dest: String, left: IRValue, right: IRValue },
    BitNot { dest: String, operand: IRValue },
    Shl { dest: String, left: IRValue, right: IRValue },
    Shr { dest: String, left: IRValue, right: IRValue },
    
    // Unary operations
    Neg { dest: String, operand: IRValue },
    Pos { dest: String, operand: IRValue },
    
    // Type conversions
    Trunc { dest: String, value: IRValue, to_type: IRType },
    ZExt { dest: String, value: IRValue, to_type: IRType },
    FpToSi { dest: String, value: IRValue, to_type: IRType },
    SiToFp { dest: String, value: IRValue, to_type: IRType },
    
    // Additional instructions for compatibility
    LoadConst { dest: String, value: IRValue },
    LoadLocal { dest: String, name: String },
    StoreLocal { name: String, value: IRValue },
    LoadGlobal { dest: String, name: String },
    StoreGlobal { name: String, value: IRValue },
    GetAttr { dest: String, obj: IRValue, attr: String },
    SetAttr { obj: IRValue, attr: String, value: IRValue },
    GetItem { dest: String, obj: IRValue, index: IRValue },
    SetItem { obj: IRValue, index: IRValue, value: IRValue },
    BuildList { dest: String, elements: Vec<IRValue> },
    BuildDict { dest: String, pairs: Vec<(IRValue, IRValue)> },
    BuildTuple { dest: String, elements: Vec<IRValue> },
    BuildSet { dest: String, elements: Vec<IRValue> },
    
    // Control flow enhancements
    Break,
    Continue,
    
    // Enhanced control flow structures
    If { cond: IRValue, then_label: String, else_label: Option<String> },
    While { cond: IRValue, body_label: String, end_label: String },
    For { init: Option<IRValue>, cond: Option<IRValue>, update: Option<IRValue>, body_label: String, end_label: String },
    Loop { body_label: String, end_label: String },
    
    // Exception handling
    Try { body_label: String, except_label: String, finally_label: Option<String> },
    Except { exception_type: Option<String>, handler_label: String },
    Raise { exception: IRValue },
    
    // Async/await operations
    Await { dest: String, expr: IRValue },
    Yield { value: IRValue },
    
    // Function definition support
    FuncDef { name: String, params: Vec<String>, body_label: String },
    
    // Class definition support
    ClassDef { name: String, bases: Vec<String>, methods: Vec<String> },
    
    // Import/module support
    Import { module: String, alias: Option<String> },
    ImportFrom { module: String, names: Vec<String> },
    
    // String operations
    StrConcat { dest: String, left: IRValue, right: IRValue },
    StrFormat { dest: String, format_str: IRValue, args: Vec<IRValue> },
    
    // Comments and documentation (for preserving in output)
    Comment { text: String },
    DocString { text: String },
    
    // Built-in functions
    Print { value: IRValue },
    Len { dest: String, obj: IRValue },
    Type { dest: String, obj: IRValue },
    
    // Variable declarations with type annotations
    DeclareVar { name: String, ty: IRType, value: Option<IRValue> },
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum IRValue {
    ImmediateInt(i64),
    ImmediateFloat(f64),
    ImmediateBool(bool),
    ImmediateString(String),
    Variable(String),
    Null,
    // Additional value types for compatibility
    ConstantInt(i64),
    ConstantFloat(f64),
    ConstantBool(bool),
    ConstantString(String),
    None,
    Bool(bool),
    Int(i64),
    Float(f64),
    String(String),
    Str(String),
    List(Vec<IRValue>),
    Dict(Vec<(IRValue, IRValue)>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum IRType {
    Void,
    Int8,
    Int16,
    Int32,
    Int64,
    Float32,
    Float64,
    Bool,
    Pointer(Box<IRType>),
    Array(Box<IRType>, usize),
    Struct(Vec<IRType>),
    Function { params: Vec<IRType>, return_type: Box<IRType> },
    // Additional types for compatibility
    I8,
    I16,
    I32,
    I64,
    F32,
    F64,
    Dynamic,
    Int,
    Float,
    String,
    List(Box<IRType>),
    Dict(Box<IRType>, Box<IRType>),
    Any,
}

#[derive(Debug, Clone)]
pub struct IRGlobal {
    pub name: String,
    pub ty: IRType,
    pub value: Option<IRValue>,
    pub is_constant: bool,
}

pub struct Generator {
    pub next_id: u32,
}

impl Generator {
    pub fn new() -> Self {
        Self { next_id: 0 }
    }

    pub fn generate(&mut self, program: Program) -> Result<IRModule, String> {
        let mut module = IRModule {
            name: "main".to_string(),
            functions: HashMap::new(),
            globals: Vec::new(),
            types: HashMap::new(),
        };

        // Add basic types
        module.types.insert("int".to_string(), IRType::Int64);
        module.types.insert("float".to_string(), IRType::Float64);
        module.types.insert("bool".to_string(), IRType::Bool);
        module.types.insert("str".to_string(), IRType::Pointer(Box::new(IRType::Int8)));

        // Add built-in functions
        self.add_builtin_functions(&mut module);

        // Collect all top-level statements for main function
        let mut main_statements = Vec::new();
        
        for stmt in program.statements {
            match stmt {
                Statement::FunctionDef { .. } | Statement::ClassDef { .. } => {
                    // Handle function and class definitions separately
                    self.generate_statement(stmt, &mut module)?;
                }
                _ => {
                    // All other statements go into main function
                    main_statements.push(stmt);
                }
            }
        }

        // Create main function if there are top-level statements
        if !main_statements.is_empty() {
            let main_function = self.generate_main_function_from_statements(main_statements, &mut module)?;
            module.functions.insert("main".to_string(), main_function);
        }

        Ok(module)
    }

    fn add_builtin_functions(&mut self, module: &mut IRModule) {
        // Add print function
        let print_function = IRFunction {
            name: "print".to_string(),
            params: vec![IRParam {
                name: "args".to_string(),
                ty: IRType::Pointer(Box::new(IRType::Pointer(Box::new(IRType::Int8)))), // char**
            }],
            return_type: IRType::Void,
            blocks: Vec::new(), // Built-in functions don't have implementations in IR
            is_exported: false,
            is_extern: true, // Mark as external/built-in
            is_async: false,
        };
        module.functions.insert("print".to_string(), print_function);
    }

    fn generate_main_function_from_statements(&mut self, statements: Vec<Statement>, module: &mut IRModule) -> Result<IRFunction, String> {
        // Create entry block
        let mut entry_block = IRBlock {
            label: "entry".to_string(),
            instructions: Vec::new(),
        };
        
        let mut all_blocks = Vec::new();
        
        // Process each statement in order
        for stmt in statements {
            self.generate_statement_in_function(&stmt, &mut entry_block, &mut all_blocks, module)?;
        }
        
        // Add return instruction to entry block
        entry_block.instructions.push(IRInstruction::Ret { 
            value: Some(IRValue::ImmediateInt(0)) 
        });
        
        // Combine all blocks
        let mut blocks = vec![entry_block];
        blocks.extend(all_blocks);
        
        Ok(IRFunction {
            name: "main".to_string(),
            params: Vec::new(),
            return_type: IRType::Int32,
            blocks,
            is_exported: true,
            is_extern: false,
            is_async: false,
        })
    }

    fn generate_statement(&mut self, stmt: Statement, module: &mut IRModule) -> Result<(), String> {
        match stmt {
            Statement::FunctionDef { name, params, return_type, body, is_async: _, decorators: _, docstring: _ } => {
                let ir_function = self.generate_function(name.clone(), params, return_type, body, module)?;
                module.functions.insert(name, ir_function);
            }
            Statement::VariableDef { name, type_annotation, value } => {
                if let Some(val) = value {
                    let ir_global = self.generate_global_variable(name, type_annotation, val, module)?;
                    module.globals.push(ir_global);
                } else {
                    // Handle variable declaration without initialization
                    let ty = if let Some(annot_type) = type_annotation {
                        self.type_to_ir(&annot_type, module)?
                    } else {
                        IRType::Dynamic // Default type for uninitialized variables
                    };
                    let ir_global = IRGlobal {
                        name,
                        ty,
                        value: None,
                        is_constant: false,
                    };
                    module.globals.push(ir_global);
                }
            }
            Statement::Expression(_) => {
                // Expression statements are now handled in generate_main_function_from_statements
                // This path should not be reached in the current implementation
            }
            Statement::If { condition: _, then_branch: _, elif_branches: _, else_branch: _ } => {
                // For now, we'll handle if statements in function context
                // This is a placeholder - proper implementation would need function context
                // TODO: Implement proper if statement handling in function generation
            }
            Statement::While { condition: _, body: _, else_branch: _ } => {
                // For now, we'll handle while loops in function context
                // This is a placeholder - proper implementation would need function context
                // TODO: Implement proper while loop handling in function generation
            }
            _ => {
                // TODO: Implement other statement types
            }
        }
        Ok(())
    }

    fn generate_function(
        &mut self, 
        name: String, 
        params: Vec<Param>, 
        return_type: Option<Type>, 
        body: Vec<Statement>,
        module: &mut IRModule
    ) -> Result<IRFunction, String> {
        let ir_params: Vec<IRParam> = params.into_iter()
            .map(|param| {
                let ty = param.type_annotation
                    .map(|t| self.type_to_ir(&t, module))
                    .unwrap_or(Ok(IRType::Int64))?; // Default to int64
                Ok(IRParam { name: param.name, ty })
            })
            .collect::<Result<_, String>>()?;

        // If no explicit return type, try to infer from function body
        let ir_return_type = if let Some(ret_type) = return_type {
            self.type_to_ir(&ret_type, module)?
        } else {
            // Try to infer return type from return statements in the body
            self.infer_return_type_from_body(&body, module).unwrap_or(IRType::Void)
        };

        let mut blocks = Vec::new();
        let mut entry_block = IRBlock {
            label: "entry".to_string(),
            instructions: Vec::new(),
        };

        // Generate instructions from body statements
        for stmt in body {
            self.generate_statement_in_function(&stmt, &mut entry_block, &mut blocks, module)?;
        }

        // Add the entry block first, then all additional blocks created during generation
        let mut all_blocks = vec![entry_block];
        all_blocks.extend(blocks);
        blocks = all_blocks;

        Ok(IRFunction {
            name,
            params: ir_params,
            return_type: ir_return_type,
            blocks,
            is_exported: false, // Set based on export statement
            is_extern: false,   // Set based on extern statement
            is_async: false,    // Set based on async statement
        })
    }

    fn generate_statement_in_function(
        &mut self,
        stmt: &Statement,
        current_block: &mut IRBlock,
        all_blocks: &mut Vec<IRBlock>,
        module: &mut IRModule
    ) -> Result<(), String> {
        match stmt {
            Statement::Expression(expr) => {
                // Generate expression and add to current block
                let ir_value = self.expr_to_ir_value(expr.clone(), current_block, module)?;
                // For function calls, the Call instruction already handles the result
                // No need to generate an additional Store instruction for temporary variables
                match &ir_value {
                    IRValue::Variable(v) if v.starts_with("tmp_") => {
                        // Skip Store instruction for temporary variables from function calls
                    }
                    IRValue::Null => {
                        // Skip Store instruction for null values
                    }
                    _ => {
                        current_block.instructions.push(IRInstruction::Store {
                            value: ir_value,
                            ptr: format!("tmp_{}", self.next_id),
                        });
                        self.next_id += 1;
                    }
                }
            }
            Statement::VariableDef { name, type_annotation: _, value } => {
                if let Some(val) = value {
                    let ir_value = self.expr_to_ir_value(val.clone(), current_block, module)?;
                    current_block.instructions.push(IRInstruction::Store {
                        value: ir_value,
                        ptr: name.clone(),
                    });
                }
            }
            Statement::If { condition, then_branch, elif_branches: _, else_branch } => {
                // Generate condition evaluation
                let condition_value = self.expr_to_ir_value(condition.clone(), current_block, module)?;
                let condition_var = format!("cond_{}", self.next_id);
                self.next_id += 1;
                
                current_block.instructions.push(IRInstruction::Store {
                    value: condition_value,
                    ptr: condition_var.clone(),
                });

                // Create labels for branches
                let then_label = format!("if_then_{}", self.next_id);
                let else_label = format!("if_else_{}", self.next_id);
                let end_label = format!("if_end_{}", self.next_id);
                self.next_id += 1;

                // Add conditional branch
                current_block.instructions.push(IRInstruction::Br {
                    cond: IRValue::Variable(condition_var),
                    then_label: then_label.clone(),
                    else_label: else_label.clone(),
                });

                // Create then block
                let mut then_block = IRBlock {
                    label: then_label.clone(),
                    instructions: vec![IRInstruction::Label(then_label.clone())],
                };
                
                for stmt in then_branch {
                    self.generate_statement_in_function(stmt, &mut then_block, all_blocks, module)?;
                }
                
                then_block.instructions.push(IRInstruction::Jmp { label: end_label.clone() });
                all_blocks.push(then_block);

                // Create else block
                let mut else_block = IRBlock {
                    label: else_label.clone(),
                    instructions: vec![IRInstruction::Label(else_label.clone())],
                };
                
                if let Some(else_stmts) = else_branch {
                    for stmt in else_stmts {
                        self.generate_statement_in_function(stmt, &mut else_block, all_blocks, module)?;
                    }
                }
                
                else_block.instructions.push(IRInstruction::Jmp { label: end_label.clone() });
                all_blocks.push(else_block);

                // Create end block for continuation
                let mut end_block = IRBlock {
                    label: end_label.clone(),
                    instructions: vec![IRInstruction::Label(end_label.clone())],
                };
                
                // Push the end block to all_blocks and create a new current block
                all_blocks.push(end_block);
                
                // Don't create a new continuation block - continue with the current block
                // This allows subsequent statements to be added to the current block
            }
            Statement::While { condition, body, else_branch: _ } => {
                // Create labels for loop
                let loop_start = format!("while_start_{}", self.next_id);
                let loop_body = format!("while_body_{}", self.next_id);
                let loop_end = format!("while_end_{}", self.next_id);
                self.next_id += 1;

                // Jump to loop start
                current_block.instructions.push(IRInstruction::Jmp { label: loop_start.clone() });

                // Create loop start block (condition check)
                let mut start_block = IRBlock {
                    label: loop_start.clone(),
                    instructions: vec![IRInstruction::Label(loop_start.clone())],
                };
                
                let condition_value = self.expr_to_ir_value(condition.clone(), &mut start_block, module)?;
                let condition_var = format!("while_cond_{}", self.next_id);
                self.next_id += 1;
                
                start_block.instructions.push(IRInstruction::Store {
                    value: condition_value,
                    ptr: condition_var.clone(),
                });
                
                start_block.instructions.push(IRInstruction::Br {
                    cond: IRValue::Variable(condition_var),
                    then_label: loop_body.clone(),
                    else_label: loop_end.clone(),
                });
                all_blocks.push(start_block);

                // Create loop body block
                let mut body_block = IRBlock {
                    label: loop_body.clone(),
                    instructions: vec![IRInstruction::Label(loop_body.clone())],
                };
                
                for stmt in body {
                    self.generate_statement_in_function(stmt, &mut body_block, all_blocks, module)?;
                }
                
                body_block.instructions.push(IRInstruction::Jmp { label: loop_start.clone() });
                all_blocks.push(body_block);

                // Create end block for continuation
                let mut end_block = IRBlock {
                    label: loop_end.clone(),
                    instructions: vec![IRInstruction::Label(loop_end.clone())],
                };
                
                // Push the end block to all_blocks and create a new current block
                all_blocks.push(end_block);
                
                // Don't create a new continuation block - continue with the current block
                // This allows subsequent statements to be added to the current block
            }
            Statement::Return(value) => {
                let return_value = if let Some(val) = value {
                    self.expr_to_ir_value(val.clone(), current_block, module)?
                } else {
                    IRValue::Null
                };
                current_block.instructions.push(IRInstruction::Ret { value: Some(return_value) });
            }
            _ => {
                // TODO: Implement other statement types
            }
        }
        Ok(())
    }

    fn generate_global_variable(
        &mut self,
        name: String,
        type_annotation: Option<Type>,
        value: Expr,
        module: &mut IRModule
    ) -> Result<IRGlobal, String> {
        let ty = if let Some(annot_type) = type_annotation {
            self.type_to_ir(&annot_type, module)?
        } else {
            // Infer type from value
            self.infer_type_from_expr(&value, module)?
        };

        // Create a temporary block for global variable initialization
        let mut temp_block = IRBlock {
            label: "global_init".to_string(),
            instructions: vec![],
        };
        let ir_value = self.expr_to_ir_value(value, &mut temp_block, module)?;

        Ok(IRGlobal {
            name,
            ty,
            value: Some(ir_value),
            is_constant: true,
        })
    }

    fn generate_expression(&mut self, expr: Expr, current_block: &mut IRBlock, module: &mut IRModule) -> Result<IRValue, String> {
        match expr {
            Expr::Literal(lit) => self.literal_to_ir_value(lit),
            Expr::Identifier(name) => Ok(IRValue::Variable(name)),
            Expr::DocString(s) => Ok(IRValue::ImmediateString(s)),
            Expr::BinaryOp { left, op, right } => {
                let left_val = self.generate_expression(*left, current_block, module)?;
                let right_val = self.generate_expression(*right, current_block, module)?;
                
                let temp_var = self.new_temp_var();
                // Generate appropriate instruction based on operation
                let instruction = match op {
                    BinaryOp::Add => IRInstruction::Add {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Sub => IRInstruction::Sub {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Mul => IRInstruction::Mul {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Div => IRInstruction::Div {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Mod => IRInstruction::Mod {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Or => IRInstruction::Or {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::And => IRInstruction::And {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Eq => IRInstruction::CmpEq {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Ne | BinaryOp::Neq => IRInstruction::CmpNe {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Lt => IRInstruction::CmpLt {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Le | BinaryOp::Lte => IRInstruction::CmpLe {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Gt => IRInstruction::CmpGt {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    BinaryOp::Ge | BinaryOp::Gte => IRInstruction::CmpGe {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    _ => return Err(format!("Unsupported binary operation: {:?}", op)),
                };
                // Add instruction to current block
                current_block.instructions.push(instruction);
                Ok(IRValue::Variable(temp_var))
            }
            Expr::Call { func, args, kwargs: _ } => {
                let func_name = match *func {
                    Expr::Identifier(name) => name,
                    _ => return Err("Complex function expressions not supported".to_string()),
                };
                
                let arg_values: Vec<IRValue> = args.into_iter()
                    .map(|arg| self.generate_expression(arg, current_block, module))
                    .collect::<Result<_, _>>()?;
                
                // Special handling for print function - it doesn't return a value
                if func_name == "print" {
                    current_block.instructions.push(IRInstruction::Print {
                        value: if arg_values.is_empty() {
                            IRValue::ImmediateString("".to_string())
                        } else {
                            arg_values[0].clone()
                        }
                    });
                    Ok(IRValue::Null) // print doesn't return a value
                } else {
                    let temp_var = self.new_temp_var();
                    eprintln!("DEBUG: Creating Call instruction with dest: {}", temp_var);
                    // Generate call instruction for function calls
                    current_block.instructions.push(IRInstruction::Call {
                        dest: Some(temp_var.clone()),
                        func: func_name,
                        args: arg_values,
                    });
                    Ok(IRValue::Variable(temp_var))
                }
            }
            Expr::Compare { left, ops, comparators } => {
                if ops.len() != 1 || comparators.len() != 1 {
                    return Err("Complex comparisons not yet supported".to_string());
                }
                
                let left_val = self.generate_expression(*left, current_block, module)?;
                let right_val = self.generate_expression(comparators[0].clone(), current_block, module)?;
                
                let temp_var = self.new_temp_var();
                let instruction = match &ops[0] {
                    CompareOp::Eq => IRInstruction::CmpEq {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    CompareOp::NotEq => IRInstruction::CmpNe {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    CompareOp::Lt => IRInstruction::CmpLt {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    CompareOp::LtE => IRInstruction::CmpLe {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    CompareOp::Gt => IRInstruction::CmpGt {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    CompareOp::GtE => IRInstruction::CmpGe {
                        dest: temp_var.clone(),
                        left: left_val,
                        right: right_val,
                    },
                    _ => return Err(format!("Unsupported comparison operation: {:?}", ops[0])),
                };
                // Add instruction to current block
                current_block.instructions.push(instruction);
                Ok(IRValue::Variable(temp_var))
            }
            Expr::UnaryOp { op, operand } => {
                let operand_val = self.generate_expression(*operand, current_block, module)?;
                let temp_var = self.new_temp_var();
                
                let instruction = match op {
                    UnaryOp::Not => IRInstruction::Not {
                        dest: temp_var.clone(),
                        operand: operand_val,
                    },
                    UnaryOp::Minus => IRInstruction::Neg {
                        dest: temp_var.clone(),
                        operand: operand_val,
                    },
                    UnaryOp::USub => IRInstruction::Neg {
                        dest: temp_var.clone(),
                        operand: operand_val,
                    },
                    UnaryOp::UAdd => IRInstruction::Pos {
                        dest: temp_var.clone(),
                        operand: operand_val,
                    },
                    UnaryOp::BitNot => IRInstruction::BitNot {
                        dest: temp_var.clone(),
                        operand: operand_val,
                    },
                    UnaryOp::Invert => IRInstruction::BitNot {
                        dest: temp_var.clone(),
                        operand: operand_val,
                    },
                };
                
                current_block.instructions.push(instruction);
                Ok(IRValue::Variable(temp_var))
            }
            Expr::MethodCall { object, method, args, kwargs: _ } => {
                // Method calls not yet supported in IR generation
                Err(format!("Method calls not yet supported in IR generation: {}.{}", 
                    format!("{:?}", object), method))
            }
            Expr::Attribute { object, name } => {
                // Attribute access not yet supported in IR generation
                Err(format!("Attribute access not yet supported in IR generation: {}.{}", 
                    format!("{:?}", object), name))
            }
            Expr::Subscript { object, index } => {
                // Subscript access not yet supported in IR generation
                Err(format!("Subscript access not yet supported in IR generation: {}[{}]", 
                    format!("{:?}", object), format!("{:?}", index)))
            }
            Expr::Slice { object, start, stop, step } => {
                // Slice operations not yet supported in IR generation
                Err(format!("Slice operations not yet supported in IR generation: {}[{}:{}:{}]", 
                    format!("{:?}", object), 
                    start.as_ref().map(|s| format!("{:?}", s)).unwrap_or_else(|| "".to_string()),
                    stop.as_ref().map(|s| format!("{:?}", s)).unwrap_or_else(|| "".to_string()),
                    step.as_ref().map(|s| format!("{:?}", s)).unwrap_or_else(|| "".to_string())))
            }
            Expr::List(elements) => {
                // List literals not yet supported in IR generation
                Err(format!("List literals not yet supported in IR generation: {} elements", elements.len()))
            }
            Expr::Tuple(elements) => {
                // Tuple literals not yet supported in IR generation
                Err(format!("Tuple literals not yet supported in IR generation: {} elements", elements.len()))
            }
            Expr::Dict(pairs) => {
                // Dictionary literals not yet supported in IR generation
                Err(format!("Dictionary literals not yet supported in IR generation: {} pairs", pairs.len()))
            }
            Expr::Set(elements) => {
                // Set literals not yet supported in IR generation
                Err(format!("Set literals not yet supported in IR generation: {} elements", elements.len()))
            }
            Expr::ListComp { element: _, generators } => {
                // List comprehensions not yet supported in IR generation
                Err(format!("List comprehensions not yet supported in IR generation: {} generators", generators.len()))
            }
            Expr::DictComp { key: _, value: _, generators } => {
                // Dictionary comprehensions not yet supported in IR generation
                Err(format!("Dictionary comprehensions not yet supported in IR generation: {} generators", generators.len()))
            }
            Expr::SetComp { element: _, generators } => {
                // Set comprehensions not yet supported in IR generation
                Err(format!("Set comprehensions not yet supported in IR generation: {} generators", generators.len()))
            }
            Expr::GeneratorExp { element: _, generators } => {
                // Generator expressions not yet supported in IR generation
                Err(format!("Generator expressions not yet supported in IR generation: {} generators", generators.len()))
            }
            Expr::Lambda { params, body: _ } => {
                // Lambda expressions not yet supported in IR generation
                Err(format!("Lambda expressions not yet supported in IR generation: {} params", params.len()))
            }
            Expr::IfExp { condition: _, then_expr: _, else_expr: _ } => {
                // Conditional expressions not yet supported in IR generation
                Err("Conditional expressions not yet supported in IR generation".to_string())
            }
            Expr::Yield(_) => {
                // Yield expressions not yet supported in IR generation
                Err("Yield expressions not yet supported in IR generation".to_string())
            }
            Expr::YieldFrom(_) => {
                // Yield from expressions not yet supported in IR generation
                Err("Yield from expressions not yet supported in IR generation".to_string())
            }
            Expr::Await(_) => {
                // Await expressions not yet supported in IR generation
                Err("Await expressions not yet supported in IR generation".to_string())
            }
            Expr::Starred(_) => {
                // Starred expressions not yet supported in IR generation
                Err("Starred expressions not yet supported in IR generation".to_string())
            }
            Expr::NamedExpr { target: _, value: _ } => {
                // Named expressions (walrus operator) not yet supported in IR generation
                Err("Named expressions (walrus operator) not yet supported in IR generation".to_string())
            }
            Expr::FormatString { parts: _ } => {
                // Format strings (f-strings) not yet supported in IR generation
                Err("Format strings (f-strings) not yet supported in IR generation".to_string())
            }
        }
    }

    fn literal_to_ir_value(&self, lit: Literal) -> Result<IRValue, String> {
        match lit {
            Literal::Int(n) => Ok(IRValue::ImmediateInt(n)),
            Literal::Float(n) => Ok(IRValue::ImmediateFloat(n)),
            Literal::String(s) => Ok(IRValue::ImmediateString(s)),
            Literal::Bool(b) => Ok(IRValue::ImmediateBool(b)),
            Literal::None => Ok(IRValue::Null),
            Literal::Bytes(_) => Err("Bytes literals not yet supported".to_string()),
            Literal::Complex { .. } => Err("Complex literals not yet supported".to_string()),
            Literal::Ellipsis => Err("Ellipsis literals not yet supported".to_string()),
        }
    }

    fn expr_to_ir_value(&mut self, expr: Expr, current_block: &mut IRBlock, module: &mut IRModule) -> Result<IRValue, String> {
        self.generate_expression(expr, current_block, module)
    }

    fn type_to_ir(&self, ty: &Type, module: &mut IRModule) -> Result<IRType, String> {
        match ty {
            Type::Simple(name) => {
                if let Some(ir_type) = module.types.get(name) {
                    Ok(ir_type.clone())
                } else {
                    // For unknown types, default to int64
                    Ok(IRType::Int64)
                }
            }
            Type::Generic { name, args } => {
                match name.as_str() {
                    "list" => {
                        if let Some(elem_type) = args.first() {
                            let ir_elem_type = self.type_to_ir(elem_type, module)?;
                            Ok(IRType::Pointer(Box::new(ir_elem_type)))
                        } else {
                            Ok(IRType::Pointer(Box::new(IRType::Int64))) // Default to int pointer
                        }
                    }
                    _ => Ok(IRType::Int64), // Fallback
                }
            }
            _ => Ok(IRType::Int64), // Fallback for complex types
        }
    }

    fn infer_return_type_from_body(&self, body: &[Statement], module: &IRModule) -> Option<IRType> {
        for stmt in body {
            if let Statement::Return(Some(expr)) = stmt {
                // Try to infer type from the return expression
                if let Ok(inferred_type) = self.infer_type_from_expr(expr, module) {
                    return Some(inferred_type);
                }
            }
        }
        None
    }

    fn infer_type_from_expr(&self, expr: &Expr, module: &IRModule) -> Result<IRType, String> {
        match expr {
            Expr::Literal(Literal::Int(_)) => Ok(IRType::Int64),
            Expr::Literal(Literal::Float(_)) => Ok(IRType::Float64),
            Expr::Literal(Literal::String(_)) => Ok(IRType::Pointer(Box::new(IRType::Int8))),
            Expr::Literal(Literal::Bool(_)) => Ok(IRType::Bool),
            Expr::Literal(Literal::None) => Ok(IRType::Int64), // TODO: Proper none type
            Expr::DocString(_) => Ok(IRType::Pointer(Box::new(IRType::Int8))), // Treat as string
            Expr::BinaryOp { left, op, right } => {
                // For binary operations, try to infer from operands
                match op {
                    BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div => {
                        // Arithmetic operations - check if operands are integers or floats
                        let left_type = self.infer_type_from_expr(left, module)?;
                        let right_type = self.infer_type_from_expr(right, module)?;
                        
                        // If either operand is float, result is float
                        if matches!(left_type, IRType::Float64) || matches!(right_type, IRType::Float64) {
                            Ok(IRType::Float64)
                        } else {
                            Ok(IRType::Int64)
                        }
                    }
                    _ => Ok(IRType::Int64), // Default for other operations
                }
            }
            _ => Ok(IRType::Int64), // Default fallback
        }
    }

    fn new_temp_var(&mut self) -> String {
        let id = self.next_id;
        self.next_id += 1;
        format!("tmp_{}", id)
    }
}

impl IRType {
    pub fn get_size(&self) -> usize {
        match self {
            IRType::Int8 => 1,
            IRType::Int32 => 4,
            IRType::Int64 => 8,
            IRType::Float32 => 4,
            IRType::Float64 => 8,
            IRType::Bool => 1,
            IRType::Pointer(_) => 8, // 64-bit pointers
            IRType::Array(elem_type, size) => elem_type.get_size() * size,
            IRType::Struct(fields) => fields.iter().map(|f| f.get_size()).sum(),
            _ => 0,
        }
    }
}
