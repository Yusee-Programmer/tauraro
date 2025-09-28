use crate::ast::*;
use crate::value::Value;
use crate::vm::{VM, Scope};
use anyhow::{Result, anyhow};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};
use std::fs;
use std::cell::RefCell;

// Thread-local storage for additional module system paths
thread_local! {
    static MODULE_SYSTEM_PATHS: RefCell<Vec<PathBuf>> = RefCell::new(Vec::new());
}

/// Module system manager for handling imports, packages, and module resolution
pub struct ModuleSystem {
    /// Cache of loaded modules (module_path -> module_object)
    loaded_modules: HashMap<String, Value>,
    /// Module search paths
    search_paths: Vec<PathBuf>,
    /// Currently importing modules (to detect circular imports)
    importing: HashSet<String>,
    /// Package cache (package_path -> package_info)
    packages: HashMap<String, PackageInfo>,
}

/// Information about a package
#[derive(Debug, Clone)]
pub struct PackageInfo {
    pub path: PathBuf,
    pub init_module: Option<Value>,
    pub submodules: HashMap<String, String>, // name -> full_path
}

/// Import specification for different import types
#[derive(Debug, Clone)]
pub enum ImportSpec {
    /// import module
    Simple { module: String, alias: Option<String> },
    /// from module import name1, name2
    FromImport { module: String, names: Vec<(String, Option<String>)> },
    /// from module import *
    FromImportAll { module: String },
}

impl Default for ModuleSystem {
    fn default() -> Self {
        Self::new()
    }
}

impl ModuleSystem {
    pub fn new() -> Self {
        let mut search_paths = vec![
            PathBuf::from("."),                         // Current directory
            PathBuf::from("tauraro_packages"),          // Built-in packages
            PathBuf::from("tauraro_packages/externals"), // Downloaded packages
        ];
        
        // Add system paths if they exist
        if let Ok(tauraro_path) = std::env::var("TAURARO_PATH") {
            for path in tauraro_path.split(';') {
                search_paths.push(PathBuf::from(path));
            }
        }
        
        // Add paths from sys.path thread-local storage
        MODULE_SYSTEM_PATHS.with(|paths| {
            if let Ok(paths) = paths.try_borrow() {
                for path in paths.iter() {
                    if !search_paths.contains(path) {
                        search_paths.push(path.clone());
                    }
                }
            }
        });
        
        Self {
            loaded_modules: HashMap::new(),
            search_paths,
            importing: HashSet::new(),
            packages: HashMap::new(),
        }
    }
    
    /// Add a search path for modules
    pub fn add_search_path(&mut self, path: PathBuf) {
        if !self.search_paths.contains(&path) {
            self.search_paths.push(path);
        }
    }
    
    /// Import a module with the given specification
    pub fn import_module(&mut self, vm: &mut VM, spec: ImportSpec) -> Result<Vec<(String, Value)>> {
        match spec {
            ImportSpec::Simple { module, alias } => {
                let module_obj = self.load_module(vm, &module)?;
                
                if let Some(alias) = alias {
                    // If there's an alias, just use it directly
                    Ok(vec![(alias, module_obj)])
                } else if module.contains('.') {
                    // Handle dotted imports by creating nested module structure
                    let parts: Vec<&str> = module.split('.').collect();
                    let mut variables = Vec::new();
                    
                    // Create the top-level module if it doesn't exist
                    let top_level = parts[0];
                    
                    if parts.len() == 2 {
                         // For math.algebra, we need to create/update the math module
                         // to contain the algebra submodule
                         
                         // Check if the top-level module already exists in the VM
                         if let Some(existing_math) = vm.get_variable(top_level) {
                             if let Value::Module(_, mut existing_namespace) = existing_math {
                                 // Add the submodule to the existing namespace
                                 existing_namespace.insert(parts[1].to_string(), module_obj);
                                 let updated_math = Value::Module(top_level.to_string(), existing_namespace);
                                 variables.push((top_level.to_string(), updated_math));
                             } else {
                                 // Existing variable is not a module, create new one
                                 let mut math_namespace = HashMap::new();
                                 math_namespace.insert(parts[1].to_string(), module_obj);
                                 let math_module = Value::Module(top_level.to_string(), math_namespace);
                                 variables.push((top_level.to_string(), math_module));
                             }
                         } else {
                             // No existing module, create new one
                             let mut math_namespace = HashMap::new();
                             math_namespace.insert(parts[1].to_string(), module_obj);
                             let math_module = Value::Module(top_level.to_string(), math_namespace);
                             variables.push((top_level.to_string(), math_module));
                         }
                    } else {
                        // For deeper nesting, we'd need more complex logic
                        // For now, just use the full name
                        variables.push((module.clone(), module_obj));
                    }
                    
                    Ok(variables)
                } else {
                    // Simple module name
                    Ok(vec![(module.clone(), module_obj)])
                }
            }
            ImportSpec::FromImport { module, names } => {
                let module_obj = self.load_module(vm, &module)?;
                let mut variables = Vec::new();
                
                if let Value::Module(_, namespace) = &module_obj {
                    for (name, alias) in names.iter() {
                        let var_name = alias.clone().unwrap_or(name.clone());
                        if let Some(value) = namespace.get(name) {
                            variables.push((var_name, value.clone()));
                        } else {
                            return Err(anyhow!("cannot import name '{}' from module '{}'", name, module));
                        }
                    }
                } else {
                    return Err(anyhow!("'{}' is not a module", module));
                }
                Ok(variables)
            }
            ImportSpec::FromImportAll { module } => {
                let module_obj = self.load_module(vm, &module)?;
                let mut variables = Vec::new();
                
                if let Value::Module(_, namespace) = &module_obj {
                    for (name, value) in namespace.iter() {
                        // Don't import private names (starting with _)
                        if !name.starts_with('_') {
                            variables.push((name.clone(), value.clone()));
                        }
                    }
                } else {
                    return Err(anyhow!("'{}' is not a module", module));
                }
                Ok(variables)
            }
        }
    }
    
    /// Load a module by name, handling packages and nested imports
    pub fn load_module(&mut self, vm: &mut VM, module_name: &str) -> Result<Value> {
        // Check if already loaded
        if let Some(cached) = self.loaded_modules.get(module_name) {
            return Ok(cached.clone());
        }
        
        // Check for circular imports
        if self.importing.contains(module_name) {
            return Err(anyhow!("Circular import detected: {}", module_name));
        }
        
        self.importing.insert(module_name.to_string());
        
        let result = self.load_module_internal(vm, module_name);
        
        self.importing.remove(module_name);
        
        result
    }
    
    /// Internal module loading logic
    fn load_module_internal(&mut self, vm: &mut VM, module_name: &str) -> Result<Value> {
        // Handle built-in modules first
        if let Some(builtin) = self.create_builtin_module(module_name) {
            self.loaded_modules.insert(module_name.to_string(), builtin.clone());
            return Ok(builtin);
        }
        
        // Try to find the module file or package
        let module_path = self.resolve_module_path(module_name)?;
        
        if module_path.is_dir() {
            // It's a package
            self.load_package(vm, module_name, &module_path)
        } else {
            // It's a single module file
            self.load_module_from_file(vm, &module_path, module_name)
        }
    }
    
    /// Resolve module name to file path
    fn resolve_module_path(&self, module_name: &str) -> Result<PathBuf> {
        let parts: Vec<&str> = module_name.split('.').collect();
        
        // Create a combined search path list including thread-local paths
        let mut all_search_paths = self.search_paths.clone();
        
        // Add current thread-local paths
        MODULE_SYSTEM_PATHS.with(|paths| {
            if let Ok(paths) = paths.try_borrow() {
                for path in paths.iter() {
                    if !all_search_paths.contains(path) {
                        all_search_paths.push(path.clone());
                    }
                }
            }
        });
        
        for search_path in &all_search_paths {
            // Try as a single file
            let mut file_path = search_path.clone();
            for part in &parts {
                file_path.push(part);
            }
            file_path.set_extension("tr");
            
            if file_path.exists() {
                return Ok(file_path);
            }
            
            // Try as a package directory
            let mut package_path = search_path.clone();
            for part in &parts {
                package_path.push(part);
            }
            
            if package_path.is_dir() {
                return Ok(package_path);
            }
        }
        
        Err(anyhow!("No module named '{}'", module_name))
    }
    
    /// Load a package (directory with __init__.tr)
    fn load_package(&mut self, vm: &mut VM, package_name: &str, package_path: &PathBuf) -> Result<Value> {
        let init_path = package_path.join("__init__.tr");
        let mut namespace = HashMap::new();
        
        // Load __init__.tr if it exists
        if init_path.exists() {
            let init_module = self.load_module_from_file(vm, &init_path, &format!("{}.__init__", package_name))?;
            if let Value::Module(_, init_namespace) = init_module {
                namespace.extend(init_namespace);
            }
        }
        
        // Discover submodules
        let mut submodules = HashMap::new();
        if let Ok(entries) = fs::read_dir(package_path) {
            for entry in entries.flatten() {
                let path = entry.path();
                let name = path.file_stem()
                    .and_then(|s| s.to_str())
                    .unwrap_or("")
                    .to_string();
                
                if name.starts_with('_') {
                    continue; // Skip private modules
                }
                
                if path.extension().and_then(|s| s.to_str()) == Some("tr") {
                    let full_name = format!("{}.{}", package_name, name);
                    submodules.insert(name, full_name);
                } else if path.is_dir() {
                    let subpackage_init = path.join("__init__.tr");
                    if subpackage_init.exists() {
                        let full_name = format!("{}.{}", package_name, name);
                        submodules.insert(name, full_name);
                    }
                }
            }
        }
        
        // Store package info
        let package_info = PackageInfo {
            path: package_path.clone(),
            init_module: None,
            submodules,
        };
        self.packages.insert(package_name.to_string(), package_info);
        
        // Create package module object
        let package_obj = Value::Module(package_name.to_string(), namespace);
        self.loaded_modules.insert(package_name.to_string(), package_obj.clone());
        
        Ok(package_obj)
    }
    
    /// Load a module from a specific file
    fn load_module_from_file(&mut self, vm: &mut VM, file_path: &PathBuf, module_name: &str) -> Result<Value> {
        // Read the module file
        let source = fs::read_to_string(file_path)
            .map_err(|e| anyhow!("Failed to read module file '{}': {}", file_path.display(), e))?;
        
        // Parse the module
        let mut lexer = crate::lexer::Lexer::new(&source);
        let tokens: Result<Vec<_>, _> = lexer.collect();
        let tokens = tokens
            .map_err(|e| anyhow!("Lexer error in module '{}': {}", module_name, e))?;
        
        let mut parser = crate::parser::Parser::new(tokens);
        let program = parser.parse()
            .map_err(|e| anyhow!("Parse error in module '{}': {}", module_name, e))?;
        
        // Create a new scope for the module
        vm.push_scope(Scope::new());
        
        // Set __name__ and __file__ variables
        vm.set_variable("__name__", Value::Str(module_name.to_string()))?;
        vm.set_variable("__file__", Value::Str(file_path.to_string_lossy().to_string()))?;
        
        // Execute the module in its own scope
        // First pass: register all functions and classes
        for statement in &program.statements {
            if let Statement::FunctionDef { name, params, return_type: _, body, is_async: _, decorators: _, docstring } = statement {
                let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                let func_value = Value::Function(name.clone(), param_names, body.clone(), docstring.clone());
                vm.set_variable(name, func_value)?;
            } else if let Statement::ClassDef { name, bases: _, body, decorators: _, metaclass: _, docstring: _ } = statement {
                // Create class object with methods
                let mut class_methods = HashMap::new();
                
                // Process class body to extract methods
                for stmt in body {
                    if let Statement::FunctionDef { name: method_name, params, return_type: _, body: method_body, is_async: _, decorators: _, docstring } = stmt {
                        let param_names: Vec<String> = params.iter().map(|p| p.name.clone()).collect();
                        let method_value = Value::Function(method_name.clone(), param_names, method_body.clone(), docstring.clone());
                        class_methods.insert(method_name.clone(), method_value);
                    }
                }
                
                let class_obj = Value::Object {
                    class_name: name.clone(),
                    fields: class_methods,
                    base_object: crate::base_object::BaseObject::new(name.clone(), vec!["object".to_string()]),
                    mro: crate::base_object::MRO::from_linearization(vec![name.clone(), "object".to_string()]),
                };
                vm.set_variable(name, class_obj)?;
            }
        }
        
        // Second pass: execute other statements
        for statement in program.statements {
            if !matches!(statement, Statement::FunctionDef { .. } | Statement::ClassDef { .. }) {
                // Handle imports within the module
                if let Statement::Import { module, alias } = &statement {
                    let import_spec = ImportSpec::Simple {
                        module: module.clone(),
                        alias: alias.clone(),
                    };
                    let variables = self.import_module(vm, import_spec)?;
                    for (name, value) in variables {
                        vm.set_variable(&name, value)?;
                    }
                } else if let Statement::FromImport { module, names } = &statement {
                    let import_spec = ImportSpec::FromImport {
                        module: module.clone(),
                        names: names.clone(),
                    };
                    let variables = self.import_module(vm, import_spec)?;
                    for (name, value) in variables {
                        vm.set_variable(&name, value)?;
                    }
                } else {
                    vm.execute_statement(&statement)?;
                }
            }
        }
        
        // Get the module's namespace (all variables defined in the module)
        let module_scope = vm.pop_scope();
        let mut module_namespace = module_scope.variables;
        
        // Remove internal variables
        module_namespace.remove("__name__");
        module_namespace.remove("__file__");
        
        // Create the module object
        let module_obj = Value::Module(module_name.to_string(), module_namespace);
        
        // Cache the loaded module
        self.loaded_modules.insert(module_name.to_string(), module_obj.clone());
        
        Ok(module_obj)
    }
    
    /// Create built-in modules
    fn create_builtin_module(&self, module_name: &str) -> Option<Value> {
        match module_name {
            "sys" => Some(crate::modules::sys::create_sys_module()),
            "os" => Some(crate::modules::os::create_os_module()),
            "thread" => Some(crate::modules::threading::create_thread_module()),
            "threading" => Some(crate::modules::threading::create_threading_module()),
            "time" => Some(crate::modules::time::create_time_module()),
            "datetime" => Some(crate::modules::datetime::create_datetime_module()),
            "socket" => Some(crate::modules::socket::create_socket_module()),
            "asyncio" => Some(crate::modules::asyncio::create_asyncio_module()),
            "httptools" => Some(crate::modules::httptools::create_httptools_module()),
            "websockets" => Some(crate::modules::websockets::create_websockets_module()),
            "httpx" => Some(crate::modules::httpx::create_httpx_module()),
            _ => None,
        }
    }
    
    /// Get all loaded modules
    pub fn get_loaded_modules(&self) -> &HashMap<String, Value> {
        &self.loaded_modules
    }
    
    /// Clear module cache
    pub fn clear_cache(&mut self) {
        self.loaded_modules.clear();
        self.packages.clear();
    }
}

/// Parse import statement into ImportSpec
pub fn parse_import_spec(statement: &Statement) -> Option<ImportSpec> {
    match statement {
        Statement::Import { module, alias } => {
            Some(ImportSpec::Simple {
                module: module.clone(),
                alias: alias.clone(),
            })
        }
        Statement::FromImport { module, names } => {
            if names.len() == 1 && names[0].0 == "*" {
                Some(ImportSpec::FromImportAll {
                    module: module.clone(),
                })
            } else {
                Some(ImportSpec::FromImport {
                    module: module.clone(),
                    names: names.clone(),
                })
            }
        }
        _ => None,
    }
}
