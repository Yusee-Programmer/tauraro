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
    
    // Memory
    Alloca { dest: String, ty: IRType },
    Store { value: IRValue, ptr: String },
    Load { dest: String, ptr: String, ty: IRType },
    
    // Control flow
    Br { cond: IRValue, then_label: String, else_label: String },
    Jmp { label: String },
    Ret { value: Option<IRValue> },
    
    // Function calls
    Call { dest: Option<String>, func: String, args: Vec<IRValue> },
    
    // Comparisons
    CmpEq { dest: String, left: IRValue, right: IRValue },
    CmpNe { dest: String, left: IRValue, right: IRValue },
    CmpLt { dest: String, left: IRValue, right: IRValue },
    CmpGt { dest: String, left: IRValue, right: IRValue },
    
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
    Await { dest: String, expr: IRValue },
    Yield { value: IRValue },
    Raise { exception: IRValue },
    Print { value: IRValue },
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
    next_id: u32,
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

        for stmt in program.statements {
            self.generate_statement(stmt, &mut module)?;
        }

        Ok(module)
    }

    fn generate_statement(&mut self, stmt: Statement, module: &mut IRModule) -> Result<(), String> {
        match stmt {
            Statement::FunctionDef { name, params, return_type, body, is_async: _, decorators: _ } => {
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
            Statement::Expression(expr) => {
                // Generate main function for expression statements at top level
                self.generate_expression(expr, module)?;
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

        let ir_return_type = return_type
            .map(|t| self.type_to_ir(&t, module))
            .unwrap_or(Ok(IRType::Void))?;

        let mut blocks = Vec::new();
        let entry_block = IRBlock {
            label: "entry".to_string(),
            instructions: Vec::new(),
        };
        blocks.push(entry_block);

        // TODO: Generate instructions from body statements

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

        let ir_value = self.expr_to_ir_value(value, module)?;

        Ok(IRGlobal {
            name,
            ty,
            value: Some(ir_value),
            is_constant: true,
        })
    }

    fn generate_expression(&mut self, expr: Expr, module: &mut IRModule) -> Result<IRValue, String> {
        match expr {
            Expr::Literal(lit) => self.literal_to_ir_value(lit),
            Expr::Identifier(name) => Ok(IRValue::Variable(name)),
            Expr::BinaryOp { left, op, right } => {
                let left_val = self.generate_expression(*left, module)?;
                let right_val = self.generate_expression(*right, module)?;
                
                let temp_var = self.new_temp_var();
                // TODO: Generate appropriate instruction based on operation
                Ok(IRValue::Variable(temp_var))
            }
            Expr::Call { func, args, kwargs: _ } => {
                let func_name = match *func {
                    Expr::Identifier(name) => name,
                    _ => return Err("Complex function expressions not supported".to_string()),
                };
                
                let arg_values: Vec<IRValue> = args.into_iter()
                    .map(|arg| self.generate_expression(arg, module))
                    .collect::<Result<_, _>>()?;
                
                let temp_var = self.new_temp_var();
                // TODO: Add call instruction
                Ok(IRValue::Variable(temp_var))
            }
            _ => Err(format!("Unsupported expression: {:?}", expr)),
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

    fn expr_to_ir_value(&mut self, expr: Expr, module: &mut IRModule) -> Result<IRValue, String> {
        self.generate_expression(expr, module)
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

    fn infer_type_from_expr(&self, expr: &Expr, module: &IRModule) -> Result<IRType, String> {
        match expr {
            Expr::Literal(Literal::Int(_)) => Ok(IRType::Int64),
            Expr::Literal(Literal::Float(_)) => Ok(IRType::Float64),
            Expr::Literal(Literal::String(_)) => Ok(IRType::Pointer(Box::new(IRType::Int8))),
            Expr::Literal(Literal::Bool(_)) => Ok(IRType::Bool),
            Expr::Literal(Literal::None) => Ok(IRType::Int64), // TODO: Proper none type
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