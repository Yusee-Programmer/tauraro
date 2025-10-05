//! Test module for LLVM backend

use crate::ir::{IRModule, IRFunction, IRType, IRInstruction, IRValue, IRParam, IRBlock, IRGlobal};
use crate::codegen::simple_llvm::SimpleLLVMCodeGenerator;
use crate::codegen::{CodegenOptions, Target};

pub fn test_simple_llvm_backend() -> anyhow::Result<()> {
    // Create a simple IR module
    let mut module = IRModule {
        name: "test_module".to_string(),
        functions: std::collections::HashMap::new(),
        globals: Vec::new(),
        types: std::collections::HashMap::new(),
    };
    
    // Create a simple function: add(a: int, b: int) -> int { return a + b; }
    let add_function = IRFunction {
        name: "add".to_string(),
        params: vec![
            IRParam {
                name: "a".to_string(),
                ty: IRType::Int64,
            },
            IRParam {
                name: "b".to_string(),
                ty: IRType::Int64,
            },
        ],
        return_type: IRType::Int64,
        blocks: vec![
            IRBlock {
                label: "entry".to_string(),
                instructions: vec![
                    IRInstruction::Add {
                        dest: "result".to_string(),
                        left: IRValue::Variable("a".to_string()),
                        right: IRValue::Variable("b".to_string()),
                    },
                    IRInstruction::Ret {
                        value: Some(IRValue::Variable("result".to_string())),
                    },
                ],
            },
        ],
        is_exported: true,
        is_extern: false,
        is_async: false,
    };
    
    module.functions.insert("add".to_string(), add_function);
    
    // Create main function that calls add
    let main_function = IRFunction {
        name: "main".to_string(),
        params: vec![],
        return_type: IRType::Int32,
        blocks: vec![
            IRBlock {
                label: "entry".to_string(),
                instructions: vec![
                    IRInstruction::Call {
                        dest: Some("result".to_string()),
                        func: "add".to_string(),
                        args: vec![
                            IRValue::ImmediateInt(10),
                            IRValue::ImmediateInt(20),
                        ],
                    },
                    IRInstruction::Ret {
                        value: Some(IRValue::Variable("result".to_string())),
                    },
                ],
            },
        ],
        is_exported: true,
        is_extern: false,
        is_async: false,
    };
    
    module.functions.insert("main".to_string(), main_function);
    
    // Generate LLVM IR
    let codegen = SimpleLLVMCodeGenerator::new();
    let options = CodegenOptions {
        target: Target::Native,
        ..Default::default()
    };
    
    let llvm_ir = codegen.generate(module, &options)?;
    println!("Generated LLVM IR:");
    println!("{}", String::from_utf8(llvm_ir)?);
    
    Ok(())
}