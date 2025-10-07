//! Module cache system for built-in Rust modules
//! This module handles caching of compiled built-in modules to improve compilation performance

use std::path::{Path, PathBuf};
use std::fs;
use std::collections::HashMap;
use anyhow::{Result, anyhow};
use std::time::SystemTime;
use std::fmt;

/// Cache manager for built-in modules
#[derive(Debug, Clone)]
pub struct ModuleCache {
    /// Path to the cache directory
    cache_dir: PathBuf,
    /// Map of module names to their cached object file paths
    cached_modules: HashMap<String, PathBuf>,
    /// Map of module names to their last compilation timestamps
    module_timestamps: HashMap<String, SystemTime>,
}

impl ModuleCache {
    /// Create a new module cache manager
    pub fn new() -> Result<Self> {
        let cache_dir = PathBuf::from("build").join("built-ins");
        
        // Create cache directory if it doesn't exist
        if !cache_dir.exists() {
            fs::create_dir_all(&cache_dir)?;
        }
        
        Ok(Self {
            cache_dir,
            cached_modules: HashMap::new(),
            module_timestamps: HashMap::new(),
        })
    }
    
    /// Get the cache directory path
    pub fn cache_dir(&self) -> &Path {
        &self.cache_dir
    }
    
    /// Check if a module is cached
    pub fn is_module_cached(&self, module_name: &str) -> bool {
        self.cached_modules.contains_key(module_name)
    }
    
    /// Get the cached object file path for a module
    pub fn get_cached_module_path(&self, module_name: &str) -> Option<&PathBuf> {
        self.cached_modules.get(module_name)
    }
    
    /// Cache a compiled module object file
    pub fn cache_module(&mut self, module_name: &str, obj_path: PathBuf) -> Result<()> {
        self.cached_modules.insert(module_name.to_string(), obj_path);
        self.module_timestamps.insert(module_name.to_string(), SystemTime::now());
        Ok(())
    }
    
    /// Mark a module as processed (even if we don't have a real object file)
    pub fn mark_module_processed(&mut self, module_name: &str) -> Result<()> {
        let obj_path = self.get_module_obj_path(module_name);
        self.cached_modules.insert(module_name.to_string(), obj_path);
        self.module_timestamps.insert(module_name.to_string(), SystemTime::now());
        Ok(())
    }
    
    /// Check if a built-in module should be cached
    pub fn should_cache_module(&self, module_name: &str) -> bool {
        // List of built-in modules that should be cached
        const BUILTIN_MODULES: &[&str] = &[
            "os", "sys", "threading", "time", "datetime", "io", "math", "random", "re", "json",
            "functools", "itertools", "collections", "copy", "pickle", "base64", "hashlib",
            "urllib", "csv", "logging", "unittest", "socket", "asyncio", "httptools",
            "websockets", "httpx", "memory", "gc", "exceptions"
        ];
        
        BUILTIN_MODULES.contains(&module_name)
    }
    
    /// Get the object file path for a cached module
    pub fn get_module_obj_path(&self, module_name: &str) -> PathBuf {
        self.cache_dir.join(format!("{}_module.o", module_name))
    }
    
    /// Clear the cache
    pub fn clear_cache(&self) -> Result<()> {
        if self.cache_dir.exists() {
            fs::remove_dir_all(&self.cache_dir)?;
            fs::create_dir_all(&self.cache_dir)?;
        }
        Ok(())
    }
    
    /// Check if cache directory exists
    pub fn cache_exists(&self) -> bool {
        self.cache_dir.exists()
    }
    
    /// Get the last compilation timestamp for a module
    pub fn get_module_timestamp(&self, module_name: &str) -> Option<SystemTime> {
        self.module_timestamps.get(module_name).copied()
    }
    
    /// Update the compilation timestamp for a module
    pub fn update_module_timestamp(&mut self, module_name: &str) -> Result<()> {
        self.module_timestamps.insert(module_name.to_string(), SystemTime::now());
        Ok(())
    }
}

impl Default for ModuleCache {
    fn default() -> Self {
        Self::new().unwrap_or_else(|_| Self {
            cache_dir: PathBuf::from(".cache"),
            cached_modules: HashMap::new(),
            module_timestamps: HashMap::new(),
        })
    }
}