//! Code generation utilities and shared functionality
pub mod llvm;
pub mod c_abi;
pub mod wasm;

use crate::ir::IRModule;
use anyhow::Result;

/// Code generation target
#[derive(Debug, Clone)]
pub enum Target {
    Native,    // Machine code via LLVM
    WASM,      // WebAssembly
    C,         // C source code
}

/// Code generation options
#[derive(Debug, Clone)]
pub struct CodegenOptions {
    pub target: Target,
    pub opt_level: u8,      // 0-3
    pub debug_info: bool,
    pub memory_mode: crate::runtime::MemoryMode,
}

/// Main code generator trait
pub trait CodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>>;
    fn get_target(&self) -> Target;
}

/// Dispatch to appropriate code generator
pub fn compile(module: IRModule, output_path: &std::path::Path, opt_level: u8) -> Result<()> {
    let options = CodegenOptions {
        target: Target::Native,
        opt_level,
        debug_info: true,
        memory_mode: crate::runtime::MemoryMode::Automatic,
    };
    
    let generator: Box<dyn CodeGenerator> = match options.target {
        Target::Native => Box::new(llvm::LLVMCodeGenerator::new()),
        Target::WASM => Box::new(wasm::WASMCodeGenerator::new()),
        Target::C => Box::new(c_abi::CABICodeGenerator::new()),
    };
    
    let binary = generator.generate(module, &options)?;
    std::fs::write(output_path, binary)?;
    
    Ok(())
}