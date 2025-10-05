//! Module loader + imports
use anyhow::Result;
use crate::value::Value;
use std::collections::HashMap;

/// Module loader and importer
pub struct ModuleLoader {
    loaded_modules: HashMap<String, Value>,
}

impl ModuleLoader {
    pub fn new() -> Self {
        Self {
            loaded_modules: HashMap::new(),
        }
    }
    
    /// Load a module by name
    pub fn load_module(&mut self, _vm: &mut crate::vm::VM, module_name: &str) -> Result<Value> {
        // Check if module is already loaded
        if let Some(module) = self.loaded_modules.get(module_name) {
            return Ok(module.clone());
        }
        
        // For now, just return an error as we'll implement this properly later
        Err(anyhow::anyhow!("Module '{}' not found", module_name))
    }
    
    /// Import a module by name
    pub fn import_module(&mut self, vm: &mut crate::vm::VM, module_name: &str) -> Result<Value> {
        self.load_module(vm, module_name)
    }
}