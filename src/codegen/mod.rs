pub mod target;
pub mod interpreter;
pub mod native;

#[cfg(feature = "llvm")]
pub mod llvm;

#[cfg(feature = "gcc")]
pub mod gcc;

#[cfg(feature = "clang")]
pub mod clang;

#[cfg(feature = "jit")]
pub mod jit;

pub mod wasm;
pub mod c_abi;
pub mod c_transpiler;
pub mod simple_llvm; // Add our new simple LLVM backend

use crate::ir::IRModule;
use anyhow::Result;
use std::path::Path;

/// Code generation target
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum Target {
    Native,
    WASM,
    C,
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
            target: Target::Native,
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

/// Unified code generation interface
pub struct CodeGen {
    generators: std::collections::HashMap<Target, Box<dyn CodeGenerator>>,
}

impl CodeGen {
    pub fn new() -> Self {
        let mut generators: std::collections::HashMap<Target, Box<dyn CodeGenerator>> = std::collections::HashMap::new();
        
        // Register available generators with automatic fallback
        
        // Try LLVM first if available
        #[cfg(feature = "llvm")]
        {
            generators.insert(Target::Native, Box::new(llvm::LLVMCodeGenerator::new()));
        }
        
        // Try GCC if LLVM is not available and GCC is installed
        #[cfg(all(not(feature = "llvm"), feature = "gcc"))]
        if gcc::GCCCodeGenerator::is_available() {
            generators.insert(Target::Native, Box::new(gcc::GCCCodeGenerator::new()));
            generators.insert(Target::GCC, Box::new(gcc::GCCCodeGenerator::new()));
        }
        
        // Try Clang if neither LLVM nor GCC are available
        #[cfg(feature = "clang")]
        if !generators.contains_key(&Target::Native) && clang::ClangCodeGenerator::is_available() {
            generators.insert(Target::Native, Box::new(clang::ClangCodeGenerator::new()));
        }
        
        // Always register specific backends if available
        #[cfg(feature = "gcc")]
        if gcc::GCCCodeGenerator::is_available() {
            generators.insert(Target::GCC, Box::new(gcc::GCCCodeGenerator::new()));
        }
        
        #[cfg(feature = "clang")]
        if clang::ClangCodeGenerator::is_available() {
            generators.insert(Target::Clang, Box::new(clang::ClangCodeGenerator::new()));
        }
        
        #[cfg(feature = "wasm")]
        generators.insert(Target::WASM, Box::new(wasm::WASMCodeGenerator::new()));
        
        generators.insert(Target::C, Box::new(c_abi::CCodeGenerator::new()));
        generators.insert(Target::Interpreter, Box::new(interpreter::InterpreterCodeGenerator::new()));
        
        // Register JIT compiler if available
        #[cfg(feature = "jit")]
        {
            generators.insert(Target::JIT, Box::new(jit::JITCodeGenerator::new()));
        }
        
        // Add our simple LLVM backend as a fallback when full LLVM is not available
        if !generators.contains_key(&Target::Native) {
            generators.insert(Target::Native, Box::new(simple_llvm::SimpleLLVMCodeGenerator::new()));
        }
        
        // Add C transpiler as a fallback for native code generation
        if !generators.contains_key(&Target::Native) {
            generators.insert(Target::Native, Box::new(c_transpiler::CTranspilerGenerator::new()));
        }
        
        Self { generators }
    }
    
    /// Generate code for the specified target
    pub fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        if let Some(generator) = self.generators.get(&options.target) {
            generator.generate(module, options)
        } else {
            Err(anyhow::anyhow!("Unsupported target: {:?}", options.target))
        }
    }
    
    /// Compile to file
    pub fn compile_to_file(&self, module: IRModule, options: &CodegenOptions, output_path: &Path) -> Result<()> {
        let code = self.generate(module, options)?;
        std::fs::write(output_path, code)?;
        Ok(())
    }
    
    /// Get available targets
    pub fn get_available_targets(&self) -> Vec<Target> {
        self.generators.keys().cloned().collect()
    }
    
    /// Check if target is supported
    pub fn supports_target(&self, target: &Target) -> bool {
        self.generators.contains_key(target)
    }
}

impl Default for CodeGen {
    fn default() -> Self {
        Self::new()
    }
}

/// Legacy trait for backward compatibility
pub trait LegacyCodeGenerator {
    fn compile(&self, module: IRModule, output_path: &Path, optimization: u8, export: bool) 
        -> Result<(), Box<dyn std::error::Error>>;
}

/// Configuration for code generation (legacy)
pub struct CodeGenConfig {
    pub optimization_level: u8,
    pub target_triple: String,
    pub export_symbols: bool,
    pub generate_debug_info: bool,
}

impl Default for CodeGenConfig {
    fn default() -> Self {
        Self {
            optimization_level: 0,
            target_triple: target::get_default_target_triple(),
            export_symbols: false,
            generate_debug_info: false,
        }
    }
}