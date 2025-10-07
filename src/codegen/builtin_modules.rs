//! Built-in module compilation and linking for Tauraro C backend
//!
//! This module handles:
//! 1. Detection of imported built-in modules during Tauraro script analysis
//! 2. Compilation of built-in Rust modules to object files (.o)
//! 3. Caching of compiled object files to avoid recompilation
//! 4. Generation of C extern declarations for built-in module functions
//! 5. Integration of built-in module object files into the linking process

use crate::ir::IRModule;
use crate::codegen::native::{NativeCompiler, TargetPlatform, OutputType};
use crate::module_cache::ModuleCache;
use crate::module_system::ModuleSystem;
use anyhow::{Result, anyhow};
use std::path::{Path, PathBuf};
use std::fs;
use std::collections::{HashSet, HashMap};
use std::process::Command;

/// Built-in module compiler and linker
pub struct BuiltinModuleCompiler {
    module_cache: ModuleCache,
    module_system: ModuleSystem,
    compiled_modules: HashSet<String>,
    built_in_object_files: HashMap<String, PathBuf>,
}

impl BuiltinModuleCompiler {
    /// Create a new built-in module compiler
    pub fn new() -> Result<Self> {
        let module_cache = ModuleCache::new()?;
        let mut module_system = ModuleSystem::new();
        
        // Add test_imports directory to the module search path
        module_system.add_search_path(PathBuf::from("test_imports"));
        
        Ok(Self {
            module_cache,
            module_system,
            compiled_modules: HashSet::new(),
            built_in_object_files: HashMap::new(),
        })
    }

    /// Detect which built-in modules are imported in the IR module
    pub fn detect_imported_builtins(&self, ir_module: &IRModule) -> HashSet<String> {
        let mut imported_modules = HashSet::new();
        let builtin_modules = self.get_builtin_module_list();
        
        // Look for import instructions in all functions
        for (_, function) in &ir_module.functions {
            for block in &function.blocks {
                for instruction in &block.instructions {
                    match instruction {
                        crate::ir::IRInstruction::Import { module, .. } => {
                            if builtin_modules.iter().any(|&m| m == module) {
                                imported_modules.insert(module.clone());
                            }
                        }
                        crate::ir::IRInstruction::ImportFrom { module, .. } => {
                            if builtin_modules.iter().any(|&m| m == module) {
                                imported_modules.insert(module.clone());
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        
        imported_modules
    }

    /// Get the list of built-in modules
    fn get_builtin_module_list(&self) -> Vec<&'static str> {
        vec![
            "os", "sys", "io", "math", "random", "time", "datetime", 
            "json", "re", "csv", "base64", "collections", "itertools", 
            "functools", "copy", "pickle", "hashlib", "urllib", 
            "socket", "threading", "asyncio", "memory", "gc", 
            "logging", "unittest", "httptools", "websockets", "httpx"
        ]
    }

    /// Compile all imported built-in modules to object files
    pub fn compile_imported_builtins(&mut self, imported_modules: &HashSet<String>) -> Result<()> {
        // Ensure build directories exist
        self.ensure_build_dirs()?;
        
        // Compile each imported built-in module
        for module_name in imported_modules {
            if !self.compiled_modules.contains(module_name) {
                self.compile_builtin_module(module_name)?;
                self.compiled_modules.insert(module_name.clone());
            }
        }
        
        Ok(())
    }

    /// Ensure build directories exist
    fn ensure_build_dirs(&self) -> Result<()> {
        let build_dir = Path::new("build");
        let builtins_dir = build_dir.join("builtins");
        
        if !build_dir.exists() {
            fs::create_dir_all(build_dir)?;
        }
        
        if !builtins_dir.exists() {
            fs::create_dir_all(builtins_dir)?;
        }
        
        Ok(())
    }

    /// Compile a single built-in module to an object file
    fn compile_builtin_module(&mut self, module_name: &str) -> Result<()> {
        // Check if module is already cached and up to date
        if self.is_module_cached(module_name)? {
            let obj_path = self.module_cache.get_module_obj_path(module_name);
            self.built_in_object_files.insert(module_name.to_string(), obj_path);
            return Ok(());
        }
        
        // Get the object file path
        let obj_path = self.module_cache.get_module_obj_path(module_name);
        
        // Compile the built-in module to object file
        self.compile_module_to_obj(module_name, &obj_path)?;
        
        // Cache the module
        self.module_cache.cache_module(module_name, obj_path.clone())?;
        
        // Track the object file for linking
        self.built_in_object_files.insert(module_name.to_string(), obj_path);
        
        println!("✅ Compiled built-in module: {}", module_name);
        Ok(())
    }

    /// Check if a module is already cached and up to date
    fn is_module_cached(&self, module_name: &str) -> Result<bool> {
        // Check if module is in cache
        if !self.module_cache.is_module_cached(module_name) {
            return Ok(false);
        }
        
        // Get paths
        let module_source_path = PathBuf::from("src").join("modules").join(format!("{}.rs", module_name));
        let obj_path = self.module_cache.get_module_obj_path(module_name);
        
        // If object file doesn't exist, it's not cached
        if !obj_path.exists() {
            return Ok(false);
        }
        
        // Check if source file is newer than object file
        let source_metadata = fs::metadata(&module_source_path)?;
        let obj_metadata = fs::metadata(&obj_path)?;
        
        let source_modified = source_metadata.modified()?;
        let obj_modified = obj_metadata.modified()?;
        
        // If source is newer, cache is outdated
        Ok(source_modified <= obj_modified)
    }

    /// Compile a built-in module to an object file using cargo
    fn compile_module_to_obj(&self, module_name: &str, obj_path: &Path) -> Result<()> {
        // Create the cache directory if it doesn't exist
        if let Some(parent) = obj_path.parent() {
            fs::create_dir_all(parent)?;
        }
        
        // For built-in modules that are part of the Tauraro crate, we need to generate
        // a standalone C file that exports the functions and constants
        let c_code = self.generate_builtin_module_c_code(module_name)?;
        
        // Write C code to a temporary file
        let temp_dir = std::env::temp_dir();
        let c_file_path = temp_dir.join(format!("{}_module.c", module_name));
        fs::write(&c_file_path, c_code)?;
        
        // Use the native compiler to detect available C compiler and compile to object file
        let compiler = NativeCompiler::new();
        let c_compiler = compiler.detect_c_compiler()
            .map_err(|e| anyhow!("Failed to detect C compiler: {}", e))?;
        
        let output = Command::new(&c_compiler)
            .arg("-c")  // Compile only, don't link
            .arg(&c_file_path)
            .arg("-o")
            .arg(obj_path)
            .output()
            .map_err(|e| anyhow!("Failed to execute C compiler: {}", e))?;
            
        // Clean up temporary file
        let _ = fs::remove_file(&c_file_path);
        
        if !output.status.success() {
            let stderr = String::from_utf8_lossy(&output.stderr);
            return Err(anyhow!("Failed to compile built-in module to object file: {}", stderr));
        }
        
        Ok(())
    }

    /// Get the list of module functions and their signatures
    fn get_module_function_signatures(&self, module_name: &str) -> Vec<(&str, &str, Vec<&str>)> {
        // Returns (function_name, return_type, param_types)
        match module_name {
            "math" => vec![
                ("tauraro_math_sin", "double", vec!["double"]),
                ("tauraro_math_cos", "double", vec!["double"]),
                ("tauraro_math_tan", "double", vec!["double"]),
                ("tauraro_math_sqrt", "double", vec!["double"]),
                ("tauraro_math_asin", "double", vec!["double"]),
                ("tauraro_math_acos", "double", vec!["double"]),
                ("tauraro_math_atan", "double", vec!["double"]),
                ("tauraro_math_atan2", "double", vec!["double", "double"]),
                ("tauraro_math_sinh", "double", vec!["double"]),
                ("tauraro_math_cosh", "double", vec!["double"]),
                ("tauraro_math_tanh", "double", vec!["double"]),
                ("tauraro_math_pow", "double", vec!["double", "double"]),
                ("tauraro_math_exp", "double", vec!["double"]),
                ("tauraro_math_log", "double", vec!["double"]),
                ("tauraro_math_log2", "double", vec!["double"]),
                ("tauraro_math_log10", "double", vec!["double"]),
                ("tauraro_math_ceil", "double", vec!["double"]),
                ("tauraro_math_floor", "double", vec!["double"]),
                ("tauraro_math_fabs", "double", vec!["double"]),
                ("tauraro_math_fmod", "double", vec!["double", "double"]),
            ],
            "time" => vec![
                ("tauraro_time_time", "double", vec![]),
                ("tauraro_time_sleep", "void", vec!["double"]),
                ("tauraro_time_perf_counter", "double", vec![]),
            ],
            "random" => vec![
                ("tauraro_random_random", "double", vec![]),
                ("tauraro_random_uniform", "double", vec!["double", "double"]),
                ("tauraro_random_randint", "int64_t", vec!["int64_t", "int64_t"]),
            ],
            "os" => vec![
                ("tauraro_os_getenv", "const char*", vec!["const char*"]),
                ("tauraro_os_system", "int", vec!["const char*"]),
            ],
            "sys" => vec![
                ("tauraro_sys_exit", "void", vec!["int"]),
            ],
            "io" => vec![
                ("tauraro_io_open", "void*", vec!["const char*", "const char*"]),
                ("tauraro_io_read", "char*", vec!["void*"]),
                ("tauraro_io_write", "void", vec!["void*", "const char*"]),
                ("tauraro_io_close", "void", vec!["void*"]),
            ],
            "json" => vec![
                ("tauraro_json_dumps", "char*", vec!["void*"]),
                ("tauraro_json_loads", "void*", vec!["const char*"]),
            ],
            _ => vec![],
        }
    }

    /// Generate C code for a built-in module that exports functions and constants
    fn generate_builtin_module_c_code(&self, module_name: &str) -> Result<String> {
        let mut c_code = String::new();

        // Standard includes
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <stdint.h>\n");
        c_code.push_str("#include <stdbool.h>\n");
        c_code.push_str("#include <math.h>\n");
        c_code.push_str("#include <string.h>\n");
        c_code.push_str("#include <time.h>\n\n");

        // Platform detection for exports
        c_code.push_str("// Platform-specific export macros\n");
        c_code.push_str("#ifdef _WIN32\n");
        c_code.push_str("    #ifdef BUILD_DLL\n");
        c_code.push_str("        #define EXPORT __declspec(dllexport)\n");
        c_code.push_str("    #else\n");
        c_code.push_str("        #define EXPORT __declspec(dllimport)\n");
        c_code.push_str("    #endif\n");
        c_code.push_str("#else\n");
        c_code.push_str("    #define EXPORT __attribute__((visibility(\"default\")))\n");
        c_code.push_str("#endif\n\n");

        // Module-specific code
        match module_name {
            "math" => {
                // Math module constants - exported for C linkage
                c_code.push_str("// Math module constants (exported for C linkage)\n");
                c_code.push_str("EXPORT const double TAURARO_MATH_PI = 3.141592653589793;\n");
                c_code.push_str("EXPORT const double TAURARO_MATH_E = 2.718281828459045;\n");
                c_code.push_str("EXPORT const double TAURARO_MATH_TAU = 6.283185307179586;\n\n");

                // Math module functions - exported for C linkage
                c_code.push_str("// Math module functions (exported for C linkage)\n");
                c_code.push_str("EXPORT double tauraro_math_sin(double x) { return sin(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_cos(double x) { return cos(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_sqrt(double x) { return sqrt(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_tan(double x) { return tan(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_asin(double x) { return asin(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_acos(double x) { return acos(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_atan(double x) { return atan(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_atan2(double y, double x) { return atan2(y, x); }\n");
                c_code.push_str("EXPORT double tauraro_math_sinh(double x) { return sinh(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_cosh(double x) { return cosh(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_tanh(double x) { return tanh(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_pow(double x, double y) { return pow(x, y); }\n");
                c_code.push_str("EXPORT double tauraro_math_exp(double x) { return exp(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_log(double x) { return log(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_log2(double x) { return log2(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_log10(double x) { return log10(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_ceil(double x) { return ceil(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_floor(double x) { return floor(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_fabs(double x) { return fabs(x); }\n");
                c_code.push_str("EXPORT double tauraro_math_fmod(double x, double y) { return fmod(x, y); }\n");
            },
            "os" => {
                // OS module constants and functions - exported for C linkage
                c_code.push_str("// OS module constants (exported for C linkage)\n");
                c_code.push_str("#ifdef _WIN32\n");
                c_code.push_str("EXPORT const char* TAURARO_OS_NAME = \"Windows\";\n");
                c_code.push_str("#elif __APPLE__\n");
                c_code.push_str("EXPORT const char* TAURARO_OS_NAME = \"macOS\";\n");
                c_code.push_str("#elif __linux__\n");
                c_code.push_str("EXPORT const char* TAURARO_OS_NAME = \"Linux\";\n");
                c_code.push_str("#else\n");
                c_code.push_str("EXPORT const char* TAURARO_OS_NAME = \"Unknown\";\n");
                c_code.push_str("#endif\n\n");

                c_code.push_str("// OS module functions (exported for C linkage)\n");
                c_code.push_str("EXPORT const char* tauraro_os_getenv(const char* name) { return getenv(name); }\n");
                c_code.push_str("EXPORT int tauraro_os_system(const char* command) { return system(command); }\n");
            },
            "sys" => {
                // Sys module constants and functions - exported for C linkage
                c_code.push_str("// Sys module constants (exported for C linkage)\n");
                c_code.push_str("#ifdef _WIN32\n");
                c_code.push_str("EXPORT const char* TAURARO_SYS_PLATFORM = \"Windows\";\n");
                c_code.push_str("#elif __APPLE__\n");
                c_code.push_str("EXPORT const char* TAURARO_SYS_PLATFORM = \"Darwin\";\n");
                c_code.push_str("#elif __linux__\n");
                c_code.push_str("EXPORT const char* TAURARO_SYS_PLATFORM = \"Linux\";\n");
                c_code.push_str("#else\n");
                c_code.push_str("EXPORT const char* TAURARO_SYS_PLATFORM = \"Unknown\";\n");
                c_code.push_str("#endif\n\n");

                c_code.push_str("EXPORT const char* TAURARO_SYS_VERSION = \"Tauraro 1.0\";\n\n");

                c_code.push_str("// Sys module functions (exported for C linkage)\n");
                c_code.push_str("EXPORT void tauraro_sys_exit(int status) { exit(status); }\n");
            },
            "time" => {
                // Time module functions - exported for C linkage
                c_code.push_str("// Time module functions (exported for C linkage)\n");
                c_code.push_str("EXPORT double tauraro_time_time() {\n");
                c_code.push_str("    return (double)time(NULL);\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT void tauraro_time_sleep(double seconds) {\n");
                c_code.push_str("#ifdef _WIN32\n");
                c_code.push_str("    Sleep((DWORD)(seconds * 1000));\n");
                c_code.push_str("#else\n");
                c_code.push_str("    usleep((useconds_t)(seconds * 1000000));\n");
                c_code.push_str("#endif\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT double tauraro_time_perf_counter() {\n");
                c_code.push_str("    return (double)clock() / CLOCKS_PER_SEC;\n");
                c_code.push_str("}\n");
            },
            "random" => {
                // Random module functions - exported for C linkage
                c_code.push_str("// Random module functions (exported for C linkage)\n");
                c_code.push_str("EXPORT double tauraro_random_random() {\n");
                c_code.push_str("    return (double)rand() / RAND_MAX;\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT double tauraro_random_uniform(double a, double b) {\n");
                c_code.push_str("    return a + (b - a) * tauraro_random_random();\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT int64_t tauraro_random_randint(int64_t a, int64_t b) {\n");
                c_code.push_str("    return a + (int64_t)(tauraro_random_random() * (b - a + 1));\n");
                c_code.push_str("}\n");
            },
            "io" => {
                // IO module functions - exported for C linkage
                c_code.push_str("// IO module functions (exported for C linkage)\n");
                c_code.push_str("EXPORT void* tauraro_io_open(const char* filename, const char* mode) {\n");
                c_code.push_str("    return fopen(filename, mode);\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT char* tauraro_io_read(void* file) {\n");
                c_code.push_str("    if (!file) return NULL;\n");
                c_code.push_str("    fseek((FILE*)file, 0, SEEK_END);\n");
                c_code.push_str("    long size = ftell((FILE*)file);\n");
                c_code.push_str("    fseek((FILE*)file, 0, SEEK_SET);\n");
                c_code.push_str("    char* buffer = malloc(size + 1);\n");
                c_code.push_str("    fread(buffer, 1, size, (FILE*)file);\n");
                c_code.push_str("    buffer[size] = '\\0';\n");
                c_code.push_str("    return buffer;\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT void tauraro_io_write(void* file, const char* data) {\n");
                c_code.push_str("    if (file && data) fprintf((FILE*)file, \"%s\", data);\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT void tauraro_io_close(void* file) {\n");
                c_code.push_str("    if (file) fclose((FILE*)file);\n");
                c_code.push_str("}\n");
            },
            "json" => {
                // JSON module basic stubs - full implementation would require a JSON library
                c_code.push_str("// JSON module basic stubs (exported for C linkage)\n");
                c_code.push_str("EXPORT char* tauraro_json_dumps(void* obj) {\n");
                c_code.push_str("    // Basic stub - full implementation requires JSON library\n");
                c_code.push_str("    return strdup(\"{}\");\n");
                c_code.push_str("}\n\n");

                c_code.push_str("EXPORT void* tauraro_json_loads(const char* json_str) {\n");
                c_code.push_str("    // Basic stub - full implementation requires JSON library\n");
                c_code.push_str("    return NULL;\n");
                c_code.push_str("}\n");
            },
            _ => {
                // Generic module - implementations will be linked from object files
                c_code.push_str("// Generic built-in module - implementations will be linked from object files\n");
            }
        }

        // Add a module initialization function
        c_code.push_str("\n// Module initialization\n");
        c_code.push_str(&format!("EXPORT void tauraro_{}_init() {{\n", module_name));
        c_code.push_str("    // Module initialization code would go here\n");
        c_code.push_str("}\n");

        Ok(c_code)
    }

    /// Generate C extern declarations for built-in module functions
    pub fn generate_extern_declarations(&self, imported_modules: &HashSet<String>) -> Result<String> {
        let mut declarations = String::new();
        
        // Add comment header
        declarations.push_str("// Built-in module extern declarations\n");
        
        for module_name in imported_modules {
            match module_name.as_str() {
                "math" => {
                    // Math module extern declarations
                    declarations.push_str("\n// Math module extern declarations\n");
                    declarations.push_str("extern double TAURARO_MATH_PI;\n");
                    declarations.push_str("extern double TAURARO_MATH_E;\n");
                    declarations.push_str("extern double TAURARO_MATH_TAU;\n\n");
                    declarations.push_str("extern double tauraro_math_sin(double x);\n");
                    declarations.push_str("extern double tauraro_math_cos(double x);\n");
                    declarations.push_str("extern double tauraro_math_sqrt(double x);\n");
                    declarations.push_str("extern double tauraro_math_tan(double x);\n");
                    declarations.push_str("extern double tauraro_math_asin(double x);\n");
                    declarations.push_str("extern double tauraro_math_acos(double x);\n");
                    declarations.push_str("extern double tauraro_math_atan(double x);\n");
                    declarations.push_str("extern double tauraro_math_atan2(double y, double x);\n");
                    declarations.push_str("extern double tauraro_math_sinh(double x);\n");
                    declarations.push_str("extern double tauraro_math_cosh(double x);\n");
                    declarations.push_str("extern double tauraro_math_tanh(double x);\n");
                    declarations.push_str("extern double tauraro_math_pow(double x, double y);\n");
                    declarations.push_str("extern double tauraro_math_exp(double x);\n");
                    declarations.push_str("extern double tauraro_math_log(double x);\n");
                    declarations.push_str("extern double tauraro_math_log2(double x);\n");
                    declarations.push_str("extern double tauraro_math_log10(double x);\n");
                    declarations.push_str("extern double tauraro_math_ceil(double x);\n");
                    declarations.push_str("extern double tauraro_math_floor(double x);\n");
                    declarations.push_str("extern double tauraro_math_fabs(double x);\n");
                    declarations.push_str("extern double tauraro_math_fmod(double x, double y);\n");
                },
                "os" => {
                    // OS module extern declarations
                    declarations.push_str("\n// OS module extern declarations\n");
                    declarations.push_str("extern const char* TAURARO_OS_NAME;\n\n");
                    declarations.push_str("extern const char* tauraro_os_getenv(const char* name);\n");
                    declarations.push_str("extern int tauraro_os_system(const char* command);\n");
                },
                "sys" => {
                    // Sys module extern declarations
                    declarations.push_str("\n// Sys module extern declarations\n");
                    declarations.push_str("extern const char* TAURARO_SYS_PLATFORM;\n");
                    declarations.push_str("extern const char* TAURARO_SYS_VERSION;\n\n");
                    declarations.push_str("extern void tauraro_sys_exit(int status);\n");
                },
                _ => {
                    // Generic module - extern declarations will be linked from object files
                    declarations.push_str(&format!("\n// {} module extern declarations - will be linked from object files\n", module_name));
                }
            }
        }
        
        declarations.push_str("\n");
        Ok(declarations)
    }

    /// Get the object files for linking
    pub fn get_object_files(&self) -> Vec<PathBuf> {
        self.built_in_object_files.values().cloned().collect()
    }

    /// Clear the compilation cache
    pub fn clear_cache(&self) -> Result<()> {
        self.module_cache.clear_cache()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_builtin_module_compiler_creation() {
        let compiler = BuiltinModuleCompiler::new().unwrap();
        assert_eq!(compiler.compiled_modules.len(), 0);
        assert_eq!(compiler.built_in_object_files.len(), 0);
    }
}