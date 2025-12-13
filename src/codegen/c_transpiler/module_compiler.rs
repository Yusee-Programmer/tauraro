//! Module Compilation System for Tauraro C Backend
//! 
//! This module handles:
//! - Compiling built-in modules (Rust FFI) to object files using rustc
//! - Generating user-defined modules as header files (stored in build/headers/)
//! - Managing module dependencies during C compilation
//!
//! Built-in modules are implemented in src/builtins_ffi/*.rs as #![no_std] Rust code
//! with C-compatible FFI exports. When an import is detected, rustc compiles the
//! corresponding FFI module to an object file that gets linked with the generated C code.

use std::collections::HashSet;
use std::path::{Path, PathBuf};
use std::fs;
use std::process::Command;

/// Built-in module names that have Rust FFI implementations in builtins_ffi/
pub const BUILTIN_MODULES: &[&str] = &[
    "abc", "asyncio", "base64", "collections", "copy", "csv", "datetime",
    "exceptions", "functools", "gc", "hashlib", "httptools", "httpx",
    "io", "itertools", "json", "logging", "math", "memory", "os",
    "pickle", "random", "re", "socket", "sys", "threading", "time",
    "unittest", "urllib", "websockets"
];

/// Check if a module name is a built-in module
pub fn is_builtin_module(name: &str) -> bool {
    BUILTIN_MODULES.contains(&name)
}

/// Module compilation context
pub struct ModuleCompiler {
    /// Build directory root
    build_dir: PathBuf,
    /// Directory for compiled built-in module objects
    builtins_dir: PathBuf,
    /// Directory for generated user module headers
    headers_dir: PathBuf,
    /// Path to the tauraro source directory (where builtins_ffi/ is located)
    tauraro_src_dir: PathBuf,
    /// Set of modules that have been processed
    processed_modules: HashSet<String>,
    /// Object files generated for linking
    object_files: Vec<PathBuf>,
}

impl ModuleCompiler {
    /// Create a new module compiler with the given build directory
    pub fn new<P: AsRef<Path>>(build_dir: P) -> Self {
        let build_dir = build_dir.as_ref().to_path_buf();
        let builtins_dir = build_dir.join("builtins");
        let headers_dir = build_dir.join("headers");
        
        // Try to find the tauraro source directory
        let tauraro_src_dir = Self::find_tauraro_src_dir();
        
        Self {
            build_dir,
            builtins_dir,
            headers_dir,
            tauraro_src_dir,
            processed_modules: HashSet::new(),
            object_files: Vec::new(),
        }
    }
    
    /// Find the tauraro source directory containing builtins_ffi/
    fn find_tauraro_src_dir() -> PathBuf {
        // Try to find the source directory in common locations
        let possible_paths = [
            // When running from cargo - use CARGO_MANIFEST_DIR at compile time
            PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("src"),
            // Current directory
            PathBuf::from(".").join("src"),
            // Parent directory (if in build/)
            PathBuf::from("..").join("src"),
        ];
        
        for path in &possible_paths {
            if path.join("builtins_ffi").exists() {
                return path.clone();
            }
        }
        
        // Default to the compile-time path
        PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("src")
    }
    
    /// Initialize the build directory structure
    pub fn init_directories(&self) -> std::io::Result<()> {
        fs::create_dir_all(&self.build_dir)?;
        fs::create_dir_all(&self.builtins_dir)?;
        fs::create_dir_all(&self.headers_dir)?;
        Ok(())
    }
    
    /// Get the build directory path
    pub fn build_dir(&self) -> &Path {
        &self.build_dir
    }
    
    /// Get the builtins directory path
    pub fn builtins_dir(&self) -> &Path {
        &self.builtins_dir
    }
    
    /// Get the headers directory path  
    pub fn headers_dir(&self) -> &Path {
        &self.headers_dir
    }
    
    /// Get all object files that need to be linked
    pub fn object_files(&self) -> &[PathBuf] {
        &self.object_files
    }
    
    /// Process a module import - compile builtin or generate user header
    pub fn process_module(&mut self, module_name: &str) -> std::io::Result<()> {
        if self.processed_modules.contains(module_name) {
            return Ok(());
        }
        
        if is_builtin_module(module_name) {
            self.compile_builtin_module_from_rust(module_name)?;
        } else {
            // User-defined module - will be handled separately
            // The transpiler will generate the header when processing the module file
        }
        
        self.processed_modules.insert(module_name.to_string());
        Ok(())
    }
    
    /// Compile a built-in module from Rust FFI source to an object file
    fn compile_builtin_module_from_rust(&mut self, module_name: &str) -> std::io::Result<()> {
        // Path to the Rust FFI source file
        let ffi_source = self.tauraro_src_dir
            .join("builtins_ffi")
            .join(format!("{}_ffi.rs", module_name));
        
        // Output object file path
        let obj_file = self.builtins_dir.join(format!("{}_ffi.o", module_name));
        
        // Check if source file exists
        if !ffi_source.exists() {
            return Err(std::io::Error::new(
                std::io::ErrorKind::NotFound,
                format!("FFI source not found: {}", ffi_source.display())
            ));
        }
        
        // Generate C header for the module (for documentation/reference)
        let header = self.generate_module_header(module_name);
        let h_file = self.builtins_dir.join(format!("{}_ffi.h", module_name));
        fs::write(&h_file, header)?;
        
        // Compile using rustc
        match self.compile_rust_to_object(&ffi_source, &obj_file) {
            Ok(()) => {
                println!("  Compiled {} FFI module to {}", module_name, obj_file.display());
                self.object_files.push(obj_file);
            }
            Err(e) => {
                eprintln!("Warning: Could not compile {} FFI module: {}", module_name, e);
                eprintln!("  Source: {}", ffi_source.display());
            }
        }
        
        Ok(())
    }
    
    /// Compile a Rust source file to an object file using rustc
    fn compile_rust_to_object(&self, rs_file: &Path, obj_file: &Path) -> std::io::Result<()> {
        // Try rustc with staticlib output (produces .o or .obj depending on platform)
        let rustc_result = Command::new("rustc")
            .arg("--crate-type=staticlib")
            .arg("--emit=obj")
            .arg("-C").arg("opt-level=2")
            .arg("-C").arg("panic=abort")
            .arg("-o").arg(obj_file)
            .arg(rs_file)
            .output();
        
        match rustc_result {
            Ok(output) => {
                if output.status.success() {
                    return Ok(());
                }
                
                // Try again without some options that might cause issues
                let fallback_result = Command::new("rustc")
                    .arg("--crate-type=staticlib")
                    .arg("--emit=obj")
                    .arg("-o").arg(obj_file)
                    .arg(rs_file)
                    .output();
                
                match fallback_result {
                    Ok(output2) => {
                        if output2.status.success() {
                            return Ok(());
                        }
                        let stderr = String::from_utf8_lossy(&output2.stderr);
                        Err(std::io::Error::new(
                            std::io::ErrorKind::Other,
                            format!("rustc compilation failed: {}", stderr)
                        ))
                    }
                    Err(e) => Err(e)
                }
            }
            Err(e) => Err(e)
        }
    }
    
    /// Generate a C header file documenting the FFI exports for a module
    fn generate_module_header(&self, module_name: &str) -> String {
        let guard = format!("TAURARO_{}_FFI_H", module_name.to_uppercase());
        format!(
r#"// Tauraro {} FFI Module Header
// Auto-generated - Documents the Rust FFI exports
// The actual implementation is compiled from src/builtins_ffi/{}_ffi.rs

#ifndef {}
#define {}

#include <stdint.h>

// Tauraro value type enumeration
typedef enum {{
    TAURARO_INT = 0,
    TAURARO_FLOAT = 1,
    TAURARO_BOOL = 2,
    TAURARO_STRING = 3,
    TAURARO_LIST = 4,
    TAURARO_DICT = 5,
    TAURARO_TUPLE = 6,
    TAURARO_SET = 7,
    TAURARO_NONE = 8,
    TAURARO_OBJECT = 9,
    TAURARO_FUNCTION = 10,
    TAURARO_BYTES = 11,
    TAURARO_COMPLEX = 12,
    TAURARO_RANGE = 13,
    TAURARO_FROZENSET = 14,
}} tauraro_type_t;

// Tauraro value data union
typedef union {{
    int64_t int_val;
    double float_val;
    int bool_val;
    char* str_val;
    void* ptr_val;
}} tauraro_data_t;

// Tauraro value structure
typedef struct tauraro_value {{
    tauraro_type_t type;
    int ref_count;
    tauraro_data_t data;
}} tauraro_value_t;

// External: value allocation (must be provided by main program)
extern tauraro_value_t* tauraro_value_new(void);

// Module-specific FFI exports
{}

#endif // {}
"#,
            module_name,
            module_name,
            guard, guard,
            self.get_module_ffi_declarations(module_name),
            guard
        )
    }
    
    /// Get FFI function declarations for a specific module
    fn get_module_ffi_declarations(&self, module_name: &str) -> String {
        match module_name {
            "math" => r#"
// Math constants
extern double tauraro_math_pi;
extern double tauraro_math_e;
extern double tauraro_math_tau;
extern double tauraro_math_inf;
extern double tauraro_math_nan;

// Math functions
tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_tan(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_log(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_log10(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_log2(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_exp(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_floor(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_ceil(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_fabs(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_asin(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_acos(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_atan(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_atan2(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_sinh(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_cosh(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_tanh(int argc, tauraro_value_t** argv);
"#.to_string(),
            "os" => r#"
// OS functions
tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_chdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_listdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_mkdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rmdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_remove(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rename(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_getenv(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_system(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_exists(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isfile(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isdir(int argc, tauraro_value_t** argv);
"#.to_string(),
            "sys" => r#"
// Sys functions
tauraro_value_t* tauraro_sys_exit(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_sys_platform(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_sys_version(int argc, tauraro_value_t** argv);
"#.to_string(),
            "json" => r#"
// JSON functions
tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv);
"#.to_string(),
            "time" => r#"
// Time functions
tauraro_value_t* tauraro_time_time(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_time_sleep(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_time_localtime(int argc, tauraro_value_t** argv);
"#.to_string(),
            "random" => r#"
// Random functions
tauraro_value_t* tauraro_random_random(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_randint(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_choice(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_seed(int argc, tauraro_value_t** argv);
"#.to_string(),
            "io" => r#"
// IO functions
tauraro_value_t* tauraro_io_open(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_io_read(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_io_write(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_io_close(int argc, tauraro_value_t** argv);
"#.to_string(),
            _ => format!("// {} module FFI exports\n// See src/builtins_ffi/{}_ffi.rs for implementation\n", module_name, module_name),
        }
    }
    
    /// Generate a header file for a user-defined module
    pub fn generate_user_module_header(&self, module_name: &str, module_source: &str) -> String {
        let guard = format!("TAURARO_USER_{}_H", module_name.to_uppercase().replace("-", "_"));
        format!(
r#"// Tauraro User Module: {}
// Auto-generated header file - DO NOT EDIT

#ifndef {}
#define {}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations from main tauraro runtime
typedef struct tauraro_value tauraro_value_t;
extern tauraro_value_t* tauraro_value_new(void);
extern tauraro_value_t* tauraro_none(void);
extern tauraro_value_t* tauraro_int(long long val);
extern tauraro_value_t* tauraro_float(double val);
extern tauraro_value_t* tauraro_bool(int val);
extern tauraro_value_t* tauraro_string(const char* val);

// Module implementation
{}

#endif // {}
"#,
            module_name,
            guard, guard,
            module_source,
            guard
        )
    }
    
    /// Write a user module header to the headers directory
    pub fn write_user_module_header(&self, module_name: &str, c_source: &str) -> std::io::Result<PathBuf> {
        let header_path = self.headers_dir.join(format!("{}.h", module_name));
        let content = self.generate_user_module_header(module_name, c_source);
        fs::write(&header_path, content)?;
        Ok(header_path)
    }
}

/// Extract imported module names from IR
pub fn extract_imported_modules(ir_module: &crate::ir::IRModule) -> HashSet<String> {
    let mut modules = HashSet::new();
    
    // Check global instructions
    for instruction in &ir_module.globals {
        match instruction {
            crate::ir::IRInstruction::Import { module } => {
                modules.insert(module.clone());
            }
            crate::ir::IRInstruction::ImportFrom { module, .. } => {
                modules.insert(module.clone());
            }
            _ => {}
        }
    }
    
    // Check function instructions
    for (_, function) in &ir_module.functions {
        for block in &function.blocks {
            for instruction in &block.instructions {
                match instruction {
                    crate::ir::IRInstruction::Import { module } => {
                        modules.insert(module.clone());
                    }
                    crate::ir::IRInstruction::ImportFrom { module, .. } => {
                        modules.insert(module.clone());
                    }
                    _ => {}
                }
            }
        }
    }
    
    modules
}

/// Categorize modules into builtin and user-defined
pub fn categorize_modules(modules: &HashSet<String>) -> (Vec<String>, Vec<String>) {
    let mut builtin_modules = Vec::new();
    let mut user_modules = Vec::new();
    
    for module in modules {
        if is_builtin_module(module) {
            builtin_modules.push(module.clone());
        } else {
            user_modules.push(module.clone());
        }
    }
    
    (builtin_modules, user_modules)
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_builtin_module_detection() {
        assert!(is_builtin_module("math"));
        assert!(is_builtin_module("os"));
        assert!(is_builtin_module("json"));
        assert!(!is_builtin_module("my_module"));
        assert!(!is_builtin_module("custom"));
    }
    
    #[test]
    fn test_module_categorization() {
        let mut modules = HashSet::new();
        modules.insert("math".to_string());
        modules.insert("os".to_string());
        modules.insert("my_module".to_string());
        modules.insert("custom".to_string());
        
        let (builtin, user) = categorize_modules(&modules);
        
        assert!(builtin.contains(&"math".to_string()));
        assert!(builtin.contains(&"os".to_string()));
        assert!(user.contains(&"my_module".to_string()));
        assert!(user.contains(&"custom".to_string()));
    }
}
