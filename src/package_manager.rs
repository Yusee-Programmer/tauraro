//! Package manager for Tauraro

use std::collections::HashMap;
use std::path::PathBuf;
use anyhow::Result;
use std::fs;

pub struct PackageManager {
    installed_packages: HashMap<String, PackageInfo>,
    packages_dir: PathBuf,
}

#[derive(Debug, Clone)]
pub struct PackageInfo {
    pub name: String,
    pub version: String,
    pub description: String,
    pub dependencies: Vec<String>,
    pub installed_path: PathBuf,
}

impl PackageManager {
    pub fn new() -> Result<Self> {
        let packages_dir = PathBuf::from("./tauraro_packages");
        
        // Create packages directory if it doesn't exist
        if !packages_dir.exists() {
            fs::create_dir_all(&packages_dir)?;
        }
        
        Ok(Self {
            installed_packages: HashMap::new(),
            packages_dir,
        })
    }
    
    pub fn install_package(&mut self, package_name: &str) -> Result<()> {
        // For now, we'll just simulate package installation
        println!("Installing package: {}", package_name);
        
        let package_info = PackageInfo {
            name: package_name.to_string(),
            version: "1.0.0".to_string(),
            description: format!("Package {}", package_name),
            dependencies: vec![],
            installed_path: self.packages_dir.join(package_name),
        };
        
        self.installed_packages.insert(package_name.to_string(), package_info);
        Ok(())
    }
    
    pub fn uninstall_package(&mut self, package_name: &str) -> Result<()> {
        if self.installed_packages.remove(package_name).is_some() {
            println!("Uninstalled package: {}", package_name);
            Ok(())
        } else {
            Err(anyhow::anyhow!("Package '{}' is not installed", package_name))
        }
    }
    
    pub fn list_packages(&self) -> Vec<&PackageInfo> {
        self.installed_packages.values().collect()
    }
    
    pub fn get_package(&self, package_name: &str) -> Option<&PackageInfo> {
        self.installed_packages.get(package_name)
    }
    
    pub fn is_package_installed(&self, package_name: &str) -> bool {
        self.installed_packages.contains_key(package_name)
    }
    
    pub fn update_package(&mut self, package_name: &str) -> Result<()> {
        if self.installed_packages.contains_key(package_name) {
            println!("Updating package: {}", package_name);
            // In a real implementation, this would download and install the latest version
            Ok(())
        } else {
            Err(anyhow::anyhow!("Package '{}' is not installed", package_name))
        }
    }
    
    pub fn resolve_dependencies(&self, package_name: &str) -> Result<Vec<String>> {
        // For now, we'll just return an empty list
        Ok(vec![])
    }
}

impl PackageInfo {
    pub fn new(name: String, version: String, description: String, dependencies: Vec<String>, installed_path: PathBuf) -> Self {
        Self {
            name,
            version,
            description,
            dependencies,
            installed_path,
        }
    }
}

impl Default for PackageManager {
    fn default() -> Self {
        Self::new().unwrap()
    }
}