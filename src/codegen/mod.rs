//! Code generation module

pub mod interpreter;

#[cfg(any(feature = "c-backend", feature = "clang", feature = "gcc"))]
pub mod c_transpiler;

pub mod rust_transpiler;

// Re-export commonly used items
pub use crate::codegen::interpreter::{Interpreter, InterpreterCodeGenerator};

#[cfg(any(feature = "c-backend", feature = "clang", feature = "gcc"))]
pub use crate::codegen::c_transpiler::CTranspiler;

pub use crate::codegen::rust_transpiler::RustTranspiler;

use crate::ir::IRModule;
use anyhow::Result;

/// Code generation target
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum Target {
    Native,
    WASM,
    C,
    Rust,
    Interpreter,
    GCC,
    Clang,
    JIT,
}

/// Optimization levels
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum OptimizationLevel {
    None = 0,
    Less = 1,
    Default = 2,
    Aggressive = 3,
}

impl From<u8> for OptimizationLevel {
    fn from(level: u8) -> Self {
        match level {
            0 => OptimizationLevel::None,
            1 => OptimizationLevel::Less,
            2 => OptimizationLevel::Default,
            3 => OptimizationLevel::Aggressive,
            _ => OptimizationLevel::Default,
        }
    }
}

/// Code generation options
#[derive(Debug, Clone)]
pub struct CodegenOptions {
    pub target: Target,
    pub opt_level: u8,
    pub target_triple: Option<String>,
    pub export_symbols: bool,
    pub generate_debug_info: bool,
    pub enable_async: bool,
    pub enable_ffi: bool,
    pub strict_types: bool,
    pub output_path: Option<String>,
}

impl Default for CodegenOptions {
    fn default() -> Self {
        Self {
            target: Target::Interpreter,
            opt_level: 0,
            target_triple: None,
            export_symbols: false,
            generate_debug_info: false,
            enable_async: true,
            enable_ffi: true,
            strict_types: false,
            output_path: None,
        }
    }
}

/// Main code generator trait
pub trait CodeGenerator {
    /// Generate code from IR module
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>>;
    
    /// Get the target this generator supports
    fn get_target(&self) -> Target;
    
    /// Check if this generator supports optimization
    fn supports_optimization(&self) -> bool {
        false
    }
    
    /// Get supported features
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec![]
    }
}