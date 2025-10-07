///! Module loader + imports for VM execution
use anyhow::{Result, anyhow};
use crate::value::Value;
use crate::modules;
use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::fs;

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

    /// Check if a module is a builtin module
    fn is_builtin_module(&self, module_name: &str) -> bool {
        modules::is_builtin_module(module_name)
    }

    /// Load a builtin module
    fn load_builtin_module(&mut self, module_name: &str) -> Result<Value> {
        // Get the builtin module from the modules crate
        match modules::get_builtin_module(module_name) {
            Some(module) => {
                // Cache the module
                self.loaded_modules.insert(module_name.to_string(), module.clone());
                Ok(module)
            }
            None => Err(anyhow!("Built-in module '{}' not found", module_name)),
        }
    }

    /// Load a non-builtin module from file system
    fn load_file_module(&mut self, vm: &mut crate::vm::VM, module_name: &str) -> Result<Value> {
        // Convert module name to file path
        // e.g., "mymodule" -> "mymodule.tr"
        // e.g., "package.submodule" -> "package/submodule.tr"
        let file_path = if module_name.contains('.') {
            let parts: Vec<&str> = module_name.split('.').collect();
            let mut path = PathBuf::from(parts[0]);
            for part in &parts[1..] {
                path = path.join(part);
            }
            path.with_extension("tr")
        } else {
            PathBuf::from(format!("{}.tr", module_name))
        };

        // Try to find the module in the current directory or module search paths
        let module_path = self.find_module_file(&file_path)?;

        // Read and execute the module file
        let source = fs::read_to_string(&module_path)
            .map_err(|e| anyhow!("Failed to read module '{}': {}", module_name, e))?;

        // Parse and execute the module
        let tokens = crate::lexer::Lexer::new(&source)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in module '{}': {:?}", module_name, e))?;

        let mut parser = crate::parser::Parser::new(tokens);
        let ast = parser
            .parse()
            .map_err(|e| anyhow!("Parser error in module '{}': {:?}", module_name, e))?;

        // Save current __name__
        let old_name = vm.get_variable("__name__");

        // Set __name__ to the module name for this execution
        vm.set_variable("__name__", Value::Str(module_name.to_string()))?;

        // Execute the module in a new scope to capture its namespace
        let mut module_namespace = HashMap::new();

        // Execute each statement and capture definitions
        for stmt in ast.statements {
            vm.execute_statement(&stmt)?;
        }

        // Get all variables from the current scope as the module's namespace
        if let Some(scope) = vm.scopes.last() {
            for (name, value) in &scope.variables {
                // Don't export private variables (starting with _)
                // unless they're special like __name__
                if !name.starts_with('_') || name.starts_with("__") {
                    module_namespace.insert(name.clone(), value.clone());
                }
            }
        }

        // Restore __name__
        if let Some(name) = old_name {
            vm.set_variable("__name__", name)?;
        }

        // Create module value
        let module_value = Value::Module(module_name.to_string(), module_namespace);

        // Cache the module
        self.loaded_modules.insert(module_name.to_string(), module_value.clone());

        Ok(module_value)
    }

    /// Find a module file in the search paths
    fn find_module_file(&self, file_path: &Path) -> Result<PathBuf> {
        // Try current directory first
        if file_path.exists() {
            return Ok(file_path.to_path_buf());
        }

        // Try as package (__init__.tr)
        if file_path.is_dir() || !file_path.exists() {
            let init_path = file_path.with_file_name(
                format!("{}/__init__.tr", file_path.file_stem().unwrap().to_string_lossy())
            );
            if init_path.exists() {
                return Ok(init_path);
            }
        }

        // Could add more search paths here (e.g., TAURARO_PATH environment variable)

        Err(anyhow!(
            "Module file not found: {}",
            file_path.display()
        ))
    }

    /// Load a module by name (auto-detects builtin vs file module)
    pub fn load_module(&mut self, vm: &mut crate::vm::VM, module_name: &str) -> Result<Value> {
        // Check if module is already loaded
        if let Some(module) = self.loaded_modules.get(module_name) {
            return Ok(module.clone());
        }

        // Check if it's a builtin module
        if self.is_builtin_module(module_name) {
            return self.load_builtin_module(module_name);
        }

        // Try to load as a file module
        self.load_file_module(vm, module_name)
    }

    /// Import a module by name
    pub fn import_module(&mut self, vm: &mut crate::vm::VM, module_name: &str) -> Result<Value> {
        self.load_module(vm, module_name)
    }

    /// Import specific items from a module
    pub fn import_from(&mut self, vm: &mut crate::vm::VM, module_name: &str, items: &[String]) -> Result<HashMap<String, Value>> {
        // Load the module first
        let module = self.load_module(vm, module_name)?;

        // Extract the requested items
        let mut imported_items = HashMap::new();

        match module {
            Value::Module(_, ref namespace) => {
                for item in items {
                    if let Some(value) = namespace.get(item) {
                        imported_items.insert(item.clone(), value.clone());
                    } else {
                        return Err(anyhow!(
                            "cannot import name '{}' from '{}' (module has no attribute '{}')",
                            item,
                            module_name,
                            item
                        ));
                    }
                }
            }
            _ => {
                return Err(anyhow!("'{}' is not a module", module_name));
            }
        }

        Ok(imported_items)
    }

    /// Clear the module cache
    pub fn clear_cache(&mut self) {
        self.loaded_modules.clear();
    }

    /// Get a list of loaded modules
    pub fn loaded_module_names(&self) -> Vec<String> {
        self.loaded_modules.keys().cloned().collect()
    }
}

impl Default for ModuleLoader {
    fn default() -> Self {
        Self::new()
    }
}
