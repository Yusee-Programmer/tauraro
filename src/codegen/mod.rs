//! COMPLETE Code generation utilities and shared functionality
pub mod llvm;
pub mod c_abi;
pub mod wasm;

use crate::ir::IRModule;
use anyhow::Result;

/// Code generation target
#[derive(Debug, Clone, PartialEq)]
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
    pub output_format: OutputFormat,
    pub target_triple: Option<String>,
    pub features: Vec<String>, // Target-specific features
}

/// Output format for code generation
#[derive(Debug, Clone)]
pub enum OutputFormat {
    Executable,    // Standalone executable
    SharedLibrary, // .so, .dll, .dylib
    StaticLibrary, // .a, .lib
    ObjectFile,    // .o, .obj
    Assembly,      // .s, .asm
    IR,           // LLVM IR, C source, WASM text
}

/// Optimization level
#[derive(Debug, Clone)]
pub enum OptimizationLevel {
    O0, // No optimization
    O1, // Basic optimization
    O2, // Standard optimization  
    O3, // Aggressive optimization
    Os, // Optimize for size
    Oz, // Aggressive size optimization
}

impl From<u8> for OptimizationLevel {
    fn from(level: u8) -> Self {
        match level {
            0 => OptimizationLevel::O0,
            1 => OptimizationLevel::O1,
            2 => OptimizationLevel::O2,
            3 => OptimizationLevel::O3,
            _ => OptimizationLevel::O2, // Default
        }
    }
}

/// Main code generator trait
pub trait CodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>>;
    fn get_target(&self) -> Target;
    fn supports_optimization(&self) -> bool {
        true
    }
    fn get_supported_features(&self) -> Vec<&'static str> {
        Vec::new()
    }
}

/// Code generation result
#[derive(Debug)]
pub struct CodegenResult {
    pub binary: Vec<u8>,
    pub warnings: Vec<String>,
    pub statistics: CodegenStats,
}

/// Code generation statistics
#[derive(Debug, Default)]
pub struct CodegenStats {
    pub compilation_time: std::time::Duration,
    pub binary_size: usize,
    pub function_count: usize,
    pub instruction_count: usize,
    pub optimization_count: usize,
}

/// Dispatch to appropriate code generator
pub fn compile(module: IRModule, output_path: &std::path::Path, opt_level: u8) -> Result<()> {
    let options = CodegenOptions {
        target: Target::Native,
        opt_level,
        debug_info: true,
        memory_mode: crate::runtime::MemoryMode::Automatic,
        output_format: OutputFormat::Executable,
        target_triple: None,
        features: Vec::new(),
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

/// Advanced compilation with full options
pub fn compile_advanced(
    module: IRModule,
    options: CodegenOptions,
) -> Result<CodegenResult> {
    let generator: Box<dyn CodeGenerator> = match options.target {
        Target::Native => Box::new(llvm::LLVMCodeGenerator::new()),
        Target::WASM => Box::new(wasm::WASMCodeGenerator::new()),
        Target::C => Box::new(c_abi::CABICodeGenerator::new()),
    };
    
    let start_time = std::time::Instant::now();
    let binary = generator.generate(module, &options)?;
    let compilation_time = start_time.elapsed();
    
    let stats = CodegenStats {
        compilation_time,
        binary_size: binary.len(),
        function_count: 0, // Would be populated from module
        instruction_count: 0,
        optimization_count: 0,
    };
    
    Ok(CodegenResult {
        binary,
        warnings: Vec::new(),
        statistics: stats,
    })
}

/// Utility functions for code generation
pub mod utils {
    use super::*;
    
    /// Validate IR module before code generation
    pub fn validate_module(module: &IRModule) -> Result<Vec<String>> {
        let mut warnings = Vec::new();
        
        // Check for undefined functions
        for (func_name, function) in &module.functions {
            for block in function.basic_blocks.values() {
                for instruction in &block.instructions {
                    if let IRInstruction::Call { function: callee, .. } = instruction {
                        if !module.functions.contains_key(callee) && !is_builtin_function(callee) {
                            warnings.push(format!("Call to undefined function: {}", callee));
                        }
                    }
                }
            }
        }
        
        // Check for unused functions
        let called_functions: Vec<String> = module.functions.values()
            .flat_map(|f| f.basic_blocks.values())
            .flat_map(|b| &b.instructions)
            .filter_map(|inst| {
                if let IRInstruction::Call { function, .. } = inst {
                    Some(function.clone())
                } else {
                    None
                }
            })
            .collect();
        
        for func_name in module.functions.keys() {
            if func_name != "main" && !called_functions.contains(func_name) {
                warnings.push(format!("Unused function: {}", func_name));
            }
        }
        
        Ok(warnings)
    }
    
    /// Check if a function name is a builtin
    fn is_builtin_function(name: &str) -> bool {
        let builtins = [
            "print", "len", "type", "str", "int", "float", "bool", "range",
            "malloc", "free", "exit", "assert",
        ];
        builtins.contains(&name)
    }
    
    /// Optimize IR module based on optimization level
    pub fn optimize_module(module: &mut IRModule, opt_level: OptimizationLevel) {
        match opt_level {
            OptimizationLevel::O0 => {
                // No optimization
            }
            OptimizationLevel::O1 => {
                apply_basic_optimizations(module);
            }
            OptimizationLevel::O2 => {
                apply_basic_optimizations(module);
                apply_standard_optimizations(module);
            }
            OptimizationLevel::O3 => {
                apply_basic_optimizations(module);
                apply_standard_optimizations(module);
                apply_aggressive_optimizations(module);
            }
            OptimizationLevel::Os => {
                apply_size_optimizations(module);
            }
            OptimizationLevel::Oz => {
                apply_aggressive_size_optimizations(module);
            }
        }
    }
    
    fn apply_basic_optimizations(_module: &mut IRModule) {
        // Constant folding, dead code elimination, etc.
    }
    
    fn apply_standard_optimizations(_module: &mut IRModule) {
        // Inlining, loop optimizations, etc.
    }
    
    fn apply_aggressive_optimizations(_module: &mut IRModule) {
        // Vectorization, interprocedural optimizations, etc.
    }
    
    fn apply_size_optimizations(_module: &mut IRModule) {
        // Optimize for binary size
    }
    
    fn apply_aggressive_size_optimizations(_module: &mut IRModule) {
        // Aggressive size optimization
    }
}

/// Target-specific configuration
pub mod target {
    use super::*;
    
    /// Get default target triple for the current platform
    pub fn get_default_target_triple() -> String {
        if cfg!(target_os = "windows") {
            if cfg!(target_arch = "x86_64") {
                "x86_64-pc-windows-msvc".to_string()
            } else {
                "i686-pc-windows-msvc".to_string()
            }
        } else if cfg!(target_os = "linux") {
            if cfg!(target_arch = "x86_64") {
                "x86_64-unknown-linux-gnu".to_string()
            } else {
                "i686-unknown-linux-gnu".to_string()
            }
        } else if cfg!(target_os = "macos") {
            if cfg!(target_arch = "x86_64") {
                "x86_64-apple-darwin".to_string()
            } else {
                "aarch64-apple-darwin".to_string()
            }
        } else {
            "unknown-unknown-unknown".to_string()
        }
    }
    
    /// Get target-specific features
    pub fn get_target_features(target: &Target) -> Vec<&'static str> {
        match target {
            Target::Native => {
                if cfg!(target_arch = "x86_64") {
                    vec!["sse", "sse2", "sse3", "ssse3", "sse4.1", "sse4.2"]
                } else if cfg!(target_arch = "aarch64") {
                    vec!["neon", "crc", "lse"]
                } else {
                    Vec::new()
                }
            }
            Target::WASM => {
                vec!["simd128", "bulk-memory", "reference-types"]
            }
            Target::C => {
                Vec::new() // C target doesn't have specific features
            }
        }
    }
    
    /// Check if target supports specific feature
    pub fn supports_feature(target: &Target, feature: &str) -> bool {
        get_target_features(target).contains(&feature)
    }
}