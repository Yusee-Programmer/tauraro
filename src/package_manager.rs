use std::path::{Path, PathBuf};
use std::fs;
use std::process::Command;
use serde::{Deserialize, Serialize};
use anyhow::{Result, anyhow};

/// Package manager for Tauraro
/// Handles external Tauraro packages and Python packages from PyPI
#[derive(Debug, Clone)]
pub struct PackageManager {
    /// Base packages directory
    packages_dir: PathBuf,
    /// External packages directory (for Tauraro packages)
    externals_dir: PathBuf,
    /// Python packages directory (for PyPI packages)
    pysites_dir: PathBuf,
}

/// Package metadata information
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PackageInfo {
    pub name: String,
    pub version: String,
    pub description: Option<String>,
    pub author: Option<String>,
    pub dependencies: Vec<String>,
    pub package_type: PackageType,
}

/// Type of package
#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum PackageType {
    /// Native Tauraro package
    Tauraro,
    /// Python package from PyPI
    Python,
}

impl PackageManager {
    /// Create a new package manager
    pub fn new() -> Self {
        let packages_dir = PathBuf::from("tauraro_packages");
        let externals_dir = packages_dir.join("externals");
        let pysites_dir = packages_dir.join("pysites");
        
        Self {
            packages_dir,
            externals_dir,
            pysites_dir,
        }
    }
    
    /// Initialize package directories
    pub fn init(&self) -> Result<()> {
        fs::create_dir_all(&self.packages_dir)?;
        fs::create_dir_all(&self.externals_dir)?;
        fs::create_dir_all(&self.pysites_dir)?;
        Ok(())
    }
    
    /// Install a Tauraro package from a URL or local path
    pub fn install_tauraro_package(&self, source: &str, name: Option<&str>) -> Result<()> {
        let package_name = name.unwrap_or_else(|| {
            Path::new(source)
                .file_stem()
                .and_then(|s| s.to_str())
                .unwrap_or("unnamed_package")
        });
        
        let destination = self.externals_dir.join(package_name);
        
        if source.starts_with("http://") || source.starts_with("https://") {
            // Download package from URL
            self.download_package(source, &destination)?;
        } else {
            // Copy from local path
            self.copy_package(source, &destination)?;
        }
        
        // Validate package structure
        self.validate_tauraro_package(&destination)?;
        
        println!("Successfully installed Tauraro package: {}", package_name);
        Ok(())
    }
    
    /// Install a Python package from PyPI
    pub fn install_python_package(&self, package_name: &str, version: Option<&str>) -> Result<()> {
        let package_spec = if let Some(ver) = version {
            format!("{}=={}", package_name, ver)
        } else {
            package_name.to_string()
        };
        
        let output = Command::new("pip")
            .args(&[
                "install",
                "--target",
                &self.pysites_dir.to_string_lossy(),
                &package_spec,
                "--no-deps", // We'll handle dependencies separately
            ])
            .output()?;
        
        if !output.status.success() {
            let error = String::from_utf8_lossy(&output.stderr);
            return Err(anyhow!("Failed to install Python package {}: {}", package_name, error));
        }
        
        println!("Successfully installed Python package: {}", package_name);
        Ok(())
    }
    
    /// Uninstall a package
    pub fn uninstall_package(&self, package_name: &str) -> Result<()> {
        // Try to remove from externals first
        let external_path = self.externals_dir.join(package_name);
        if external_path.exists() {
            fs::remove_dir_all(&external_path)?;
            println!("Uninstalled Tauraro package: {}", package_name);
            return Ok(());
        }
        
        // Try to remove from pysites
        let python_path = self.pysites_dir.join(package_name);
        if python_path.exists() {
            fs::remove_dir_all(&python_path)?;
            println!("Uninstalled Python package: {}", package_name);
            return Ok(());
        }
        
        Err(anyhow!("Package '{}' not found", package_name))
    }
    
    /// List all installed packages
    pub fn list_packages(&self) -> Result<Vec<PackageInfo>> {
        let mut packages = Vec::new();
        
        // List Tauraro packages
        if self.externals_dir.exists() {
            for entry in fs::read_dir(&self.externals_dir)? {
                let entry = entry?;
                if entry.file_type()?.is_dir() {
                    if let Some(name) = entry.file_name().to_str() {
                        if let Ok(info) = self.get_tauraro_package_info(&entry.path()) {
                            packages.push(info);
                        } else {
                            // Create basic info if metadata is missing
                            packages.push(PackageInfo {
                                name: name.to_string(),
                                version: "unknown".to_string(),
                                description: None,
                                author: None,
                                dependencies: vec![],
                                package_type: PackageType::Tauraro,
                            });
                        }
                    }
                }
            }
        }
        
        // List Python packages
        if self.pysites_dir.exists() {
            for entry in fs::read_dir(&self.pysites_dir)? {
                let entry = entry?;
                if entry.file_type()?.is_dir() {
                    if let Some(name) = entry.file_name().to_str() {
                        if !name.starts_with('.') && !name.ends_with(".dist-info") {
                            packages.push(PackageInfo {
                                name: name.to_string(),
                                version: "unknown".to_string(),
                                description: None,
                                author: None,
                                dependencies: vec![],
                                package_type: PackageType::Python,
                            });
                        }
                    }
                }
            }
        }
        
        packages.sort_by(|a, b| a.name.cmp(&b.name));
        Ok(packages)
    }
    
    /// Get package search paths for the module system
    pub fn get_search_paths(&self) -> Vec<PathBuf> {
        vec![
            self.packages_dir.clone(),
            self.externals_dir.clone(),
            self.pysites_dir.clone(),
        ]
    }
    
    /// Check if a package is installed
    pub fn is_package_installed(&self, package_name: &str) -> bool {
        self.externals_dir.join(package_name).exists() || 
        self.pysites_dir.join(package_name).exists()
    }
    
    /// Download package from URL
    fn download_package(&self, url: &str, destination: &Path) -> Result<()> {
        // This is a simplified implementation
        // In a real implementation, you'd use proper HTTP client like reqwest
        let output = Command::new("curl")
            .args(&["-L", "-o", "temp_package.zip", url])
            .output()?;
        
        if !output.status.success() {
            return Err(anyhow!("Failed to download package from: {}", url));
        }
        
        // Extract the downloaded package
        let extract_output = Command::new("unzip")
            .args(&["-q", "temp_package.zip", "-d", &destination.to_string_lossy()])
            .output()?;
        
        if !extract_output.status.success() {
            return Err(anyhow!("Failed to extract package"));
        }
        
        // Clean up
        let _ = fs::remove_file("temp_package.zip");
        
        Ok(())
    }
    
    /// Copy package from local path
    fn copy_package(&self, source: &str, destination: &Path) -> Result<()> {
        let source_path = Path::new(source);
        if !source_path.exists() {
            return Err(anyhow!("Source path does not exist: {}", source));
        }
        
        if source_path.is_dir() {
            self.copy_dir_all(source_path, destination)?;
        } else {
            return Err(anyhow!("Source must be a directory for Tauraro packages"));
        }
        
        Ok(())
    }
    
    /// Recursively copy directory
    fn copy_dir_all(&self, src: &Path, dst: &Path) -> Result<()> {
        fs::create_dir_all(dst)?;
        for entry in fs::read_dir(src)? {
            let entry = entry?;
            let ty = entry.file_type()?;
            if ty.is_dir() {
                self.copy_dir_all(&entry.path(), &dst.join(entry.file_name()))?;
            } else {
                fs::copy(entry.path(), dst.join(entry.file_name()))?;
            }
        }
        Ok(())
    }
    
    /// Validate Tauraro package structure
    fn validate_tauraro_package(&self, package_path: &Path) -> Result<()> {
        // Check for required files
        let init_file = package_path.join("__init__.tr");
        if !init_file.exists() {
            return Err(anyhow!("Invalid Tauraro package: missing __init__.tr"));
        }
        
        Ok(())
    }
    
    /// Get Tauraro package metadata
    fn get_tauraro_package_info(&self, package_path: &Path) -> Result<PackageInfo> {
        let metadata_file = package_path.join("package.toml");
        if metadata_file.exists() {
            let content = fs::read_to_string(&metadata_file)?;
            // Parse TOML metadata (simplified)
            // In a real implementation, you'd use a proper TOML parser
            let name = package_path.file_name()
                .and_then(|s| s.to_str())
                .unwrap_or("unknown")
                .to_string();
            
            Ok(PackageInfo {
                name,
                version: "1.0.0".to_string(), // Extract from TOML
                description: None,
                author: None,
                dependencies: vec![],
                package_type: PackageType::Tauraro,
            })
        } else {
            Err(anyhow!("No package metadata found"))
        }
    }
}

impl Default for PackageManager {
    fn default() -> Self {
        Self::new()
    }
}