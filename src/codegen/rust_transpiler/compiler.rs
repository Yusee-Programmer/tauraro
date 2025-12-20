//! Main compiler for Rust transpiler

use crate::ir::IRModule;
use anyhow::Result;
use super::RustTranspiler;

/// Rust compilation options
#[derive(Debug, Clone)]
pub struct RustCompileOptions {
    pub target: String,
    pub optimize: bool,
    pub include_runtime: bool,
    pub include_stdlib: bool,
    pub async_runtime: bool,
    pub generate_tests: bool,
}

impl Default for RustCompileOptions {
    fn default() -> Self {
        Self {
            target: "native".to_string(),
            optimize: true,
            include_runtime: true,
            include_stdlib: true,
            async_runtime: true,
            generate_tests: false,
        }
    }
}

/// Rust compiler interface
pub struct RustCompiler {
    options: RustCompileOptions,
}

impl RustCompiler {
    pub fn new(options: RustCompileOptions) -> Self {
        Self { options }
    }

    /// Compile IR module to Rust code
    pub fn compile(&self, module: IRModule) -> Result<String> {
        let mut transpiler = RustTranspiler::new("main".to_string());

        // Add stdlib modules if needed
        if self.options.include_stdlib {
            transpiler.context.gen_all_stdlib_modules()?;
        }

        // Transpile the module
        transpiler.transpile(module)
    }

    /// Compile and return formatted code
    pub fn compile_formatted(&self, module: IRModule) -> Result<String> {
        let code = self.compile(module)?;
        
        // Format the code (could use rustfmt via command line)
        Ok(code)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_compiler_creation() {
        let options = RustCompileOptions::default();
        let compiler = RustCompiler::new(options);
        assert_eq!(compiler.options.target, "native");
    }
}
