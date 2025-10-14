//! Module system for Tauraro

use crate::value::Value;
use std::collections::HashMap;
use std::path::PathBuf;
use anyhow::Result;

pub struct ModuleSystem {
    modules: HashMap<String, Value>,
    search_paths: Vec<PathBuf>,
}

impl ModuleSystem {
    pub fn new() -> Self {
        Self {
            modules: HashMap::new(),
            search_paths: vec![
                PathBuf::from("."),
                PathBuf::from("./modules"),
                PathBuf::from("./lib"),
            ],
        }
    }
    
    pub fn add_search_path(&mut self, path: PathBuf) {
        self.search_paths.push(path);
    }
    
    pub fn import_module(&mut self, module_name: &str) -> Result<Value> {
        // Check if module is already loaded
        if let Some(module) = self.modules.get(module_name) {
            return Ok(module.clone());
        }
        
        // Try to load built-in modules first
        if let Some(builtin_module) = self.load_builtin_module(module_name) {
            self.modules.insert(module_name.to_string(), builtin_module.clone());
            return Ok(builtin_module);
        }
        
        // Try to load from file system
        self.load_module_from_file(module_name)
    }
    
    fn load_builtin_module(&self, module_name: &str) -> Option<Value> {
        match module_name {
            "builtins" => Some(self.create_builtins_module()),
            "sys" => Some(self.create_sys_module()),
            "os" => Some(self.create_os_module()),
            "math" => Some(self.create_math_module()),
            "random" => Some(self.create_random_module()),
            "time" => Some(self.create_time_module()),
            "json" => Some(self.create_json_module()),
            "re" => Some(self.create_re_module()),
            "hplist" => Some(crate::modules::hplist::create_hplist_module()),
            _ => None,
        }
    }
    
    fn load_module_from_file(&self, module_name: &str) -> Result<Value> {
        // For now, we'll just return an empty module
        // In a full implementation, this would load and execute the module file
        let mut namespace = HashMap::new();
        Ok(Value::Module(module_name.to_string(), namespace))
    }
    
    fn create_builtins_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add built-in functions
        namespace.insert("print".to_string(), Value::BuiltinFunction("print".to_string(), |args| {
            let output = args.iter().map(|arg| format!("{}", arg)).collect::<Vec<_>>().join(" ");
            println!("{}", output);
            Ok(Value::None)
        }));
        
        namespace.insert("len".to_string(), Value::BuiltinFunction("len".to_string(), |args| {
            if args.len() != 1 {
                return Err(anyhow::anyhow!("len() takes exactly one argument ({} given)", args.len()));
            }
            
            match &args[0] {
                Value::Str(s) => Ok(Value::Int(s.len() as i64)),
                Value::List(items) => Ok(Value::Int(items.len() as i64)),
                Value::Tuple(items) => Ok(Value::Int(items.len() as i64)),
                Value::Dict(dict) => Ok(Value::Int(dict.len() as i64)),
                _ => Err(anyhow::anyhow!("object of type '{}' has no len()", args[0].type_name())),
            }
        }));
        
        Value::Module("builtins".to_string(), namespace)
    }
    
    fn create_sys_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add sys module attributes
        namespace.insert("version".to_string(), Value::Str(env!("CARGO_PKG_VERSION").to_string()));
        namespace.insert("platform".to_string(), Value::Str(std::env::consts::OS.to_string()));
        
        Value::Module("sys".to_string(), namespace)
    }
    
    fn create_os_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add os module functions
        namespace.insert("getcwd".to_string(), Value::BuiltinFunction("getcwd".to_string(), |_| {
            match std::env::current_dir() {
                Ok(path) => Ok(Value::Str(path.to_string_lossy().to_string())),
                Err(e) => Err(anyhow::anyhow!("getcwd() failed: {}", e)),
            }
        }));
        
        Value::Module("os".to_string(), namespace)
    }
    
    fn create_math_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add math module constants
        namespace.insert("pi".to_string(), Value::Float(std::f64::consts::PI));
        namespace.insert("e".to_string(), Value::Float(std::f64::consts::E));
        
        // Add math module functions
        namespace.insert("sqrt".to_string(), Value::BuiltinFunction("sqrt".to_string(), |args| {
            if args.len() != 1 {
                return Err(anyhow::anyhow!("sqrt() takes exactly one argument ({} given)", args.len()));
            }
            
            match &args[0] {
                Value::Int(n) => Ok(Value::Float((*n as f64).sqrt())),
                Value::Float(f) => Ok(Value::Float(f.sqrt())),
                _ => Err(anyhow::anyhow!("math.sqrt() argument must be a number")),
            }
        }));
        
        Value::Module("math".to_string(), namespace)
    }
    
    fn create_random_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add random module functions
        namespace.insert("random".to_string(), Value::BuiltinFunction("random".to_string(), |_| {
            Ok(Value::Float(rand::random::<f64>()))
        }));
        
        Value::Module("random".to_string(), namespace)
    }
    
    fn create_time_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add time module functions
        namespace.insert("time".to_string(), Value::BuiltinFunction("time".to_string(), |_| {
            Ok(Value::Float(std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .map(|d| d.as_secs_f64())
                .unwrap_or(0.0)))
        }));
        
        Value::Module("time".to_string(), namespace)
    }
    
    fn create_json_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add json module functions
        namespace.insert("loads".to_string(), Value::BuiltinFunction("loads".to_string(), |args| {
            if args.len() != 1 {
                return Err(anyhow::anyhow!("json.loads() takes exactly one argument ({} given)", args.len()));
            }
            
            match &args[0] {
                Value::Str(_s) => {
                    // In a full implementation, this would parse JSON
                    Ok(Value::Dict(HashMap::new()))
                }
                _ => Err(anyhow::anyhow!("json.loads() argument must be a string")),
            }
        }));
        
        Value::Module("json".to_string(), namespace)
    }
    
    fn create_re_module(&self) -> Value {
        let mut namespace = HashMap::new();
        
        // Add re module functions
        namespace.insert("search".to_string(), Value::BuiltinFunction("search".to_string(), |args| {
            if args.len() != 2 {
                return Err(anyhow::anyhow!("re.search() takes exactly two arguments ({} given)", args.len()));
            }
            
            // In a full implementation, this would perform regex search
            Ok(Value::None)
        }));
        
        Value::Module("re".to_string(), namespace)
    }
}

impl Default for ModuleSystem {
    fn default() -> Self {
        Self::new()
    }
}