//! Module cache for Tauraro

use std::collections::HashMap;
use std::path::PathBuf;
use anyhow::Result;
use std::fs;

pub struct ModuleCache {
    cache: HashMap<String, CachedModule>,
    cache_dir: PathBuf,
}

#[derive(Debug, Clone)]
pub struct CachedModule {
    pub name: String,
    pub path: PathBuf,
    pub compiled_code: Vec<u8>,
    pub timestamp: std::time::SystemTime,
}

impl ModuleCache {
    pub fn new() -> Result<Self> {
        let cache_dir = PathBuf::from(".tauraro_cache");
        
        // Create cache directory if it doesn't exist
        if !cache_dir.exists() {
            fs::create_dir_all(&cache_dir)?;
        }
        
        Ok(Self {
            cache: HashMap::new(),
            cache_dir,
        })
    }
    
    pub fn get(&self, module_name: &str) -> Option<&CachedModule> {
        self.cache.get(module_name)
    }
    
    pub fn insert(&mut self, module_name: String, module: CachedModule) {
        self.cache.insert(module_name, module);
    }
    
    pub fn remove(&mut self, module_name: &str) -> Option<CachedModule> {
        self.cache.remove(module_name)
    }
    
    pub fn contains(&self, module_name: &str) -> bool {
        self.cache.contains_key(module_name)
    }
    
    pub fn clear_cache(&self) -> Result<()> {
        // Remove all files in cache directory
        for entry in fs::read_dir(&self.cache_dir)? {
            let entry = entry?;
            if entry.file_type()?.is_file() {
                fs::remove_file(entry.path())?;
            }
        }
        Ok(())
    }
    
    pub fn save_module(&self, module_name: &str, compiled_code: &[u8]) -> Result<()> {
        let cache_file = self.cache_dir.join(format!("{}.tcache", module_name));
        fs::write(cache_file, compiled_code)?;
        Ok(())
    }
    
    pub fn load_module(&self, module_name: &str) -> Result<Option<Vec<u8>>> {
        let cache_file = self.cache_dir.join(format!("{}.tcache", module_name));
        if cache_file.exists() {
            let compiled_code = fs::read(cache_file)?;
            Ok(Some(compiled_code))
        } else {
            Ok(None)
        }
    }
    
    pub fn is_module_outdated(&self, module_name: &str, source_path: &PathBuf) -> Result<bool> {
        let cache_file = self.cache_dir.join(format!("{}.tcache", module_name));
        
        if !cache_file.exists() {
            return Ok(true);
        }
        
        let source_modified = fs::metadata(source_path)?.modified()?;
        let cache_modified = fs::metadata(cache_file)?.modified()?;
        
        Ok(source_modified > cache_modified)
    }
}

impl CachedModule {
    pub fn new(name: String, path: PathBuf, compiled_code: Vec<u8>) -> Self {
        Self {
            name,
            path,
            compiled_code,
            timestamp: std::time::SystemTime::now(),
        }
    }
    
    pub fn age(&self) -> Result<std::time::Duration> {
        self.timestamp.elapsed().map_err(|e| anyhow::anyhow!("Failed to calculate module age: {}", e))
    }
}