//! Module system for Rust code generation

use super::RustCodegenContext;
use anyhow::Result;
use std::collections::HashMap;

/// Module information
#[derive(Debug, Clone)]
pub struct ModuleInfo {
    pub name: String,
    pub exports: Vec<String>,
    pub dependencies: Vec<String>,
}

/// Module compiler for Rust
pub struct RustModuleCompiler {
    pub modules: HashMap<String, ModuleInfo>,
}

impl RustModuleCompiler {
    pub fn new() -> Self {
        Self {
            modules: HashMap::new(),
        }
    }

    /// Register a module
    pub fn register_module(&mut self, info: ModuleInfo) {
        self.modules.insert(info.name.clone(), info);
    }

    /// Generate module imports
    pub fn gen_module_imports(&self, ctx: &mut RustCodegenContext, module_name: &str) -> Result<()> {
        if let Some(module) = self.modules.get(module_name) {
            for export in &module.exports {
                ctx.add_import(&format!("use crate::modules::{}::{};", module_name, export));
            }
        }
        Ok(())
    }

    /// Generate module export statement
    pub fn gen_module_export(&self, ctx: &mut RustCodegenContext, name: &str) -> Result<()> {
        ctx.emit(&format!("pub use self::{};", name));
        Ok(())
    }

    /// Generate visibility modifier
    pub fn gen_visibility(&self, public: bool) -> &str {
        if public { "pub" } else { "" }
    }
}

impl RustCodegenContext {
    /// Generate module definition
    pub fn gen_module(&mut self, name: &str, public: bool) -> Result<()> {
        let vis = if public { "pub " } else { "" };
        self.emit(&format!("{}mod {} {{", vis, name));
        self.indent();
        Ok(())
    }

    /// Close module definition
    pub fn close_module(&mut self) -> Result<()> {
        self.dedent();
        self.emit("}");
        self.emit("");
        Ok(())
    }

    /// Generate use statement
    pub fn gen_use(&mut self, path: &str, alias: Option<&str>) -> Result<()> {
        match alias {
            Some(a) => self.add_import(&format!("use {} as {};", path, a)),
            None => self.add_import(&format!("use {};", path)),
        }
        Ok(())
    }

    /// Generate pub use (re-export)
    pub fn gen_pub_use(&mut self, path: &str) -> Result<()> {
        self.emit(&format!("pub use {};", path));
        Ok(())
    }

    /// Generate super access
    pub fn gen_super_access(&self, item: &str) -> String {
        format!("super::{}", item)
    }

    /// Generate crate access
    pub fn gen_crate_access(&self, item: &str) -> String {
        format!("crate::{}", item)
    }

    /// Generate self access
    pub fn gen_self_access(&self, item: &str) -> String {
        format!("self::{}", item)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_module_compiler_registration() {
        let mut compiler = RustModuleCompiler::new();
        let module = ModuleInfo {
            name: "math".to_string(),
            exports: vec!["sin".to_string(), "cos".to_string()],
            dependencies: vec![],
        };
        compiler.register_module(module);
        assert!(compiler.modules.contains_key("math"));
    }
}
