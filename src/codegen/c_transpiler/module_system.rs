//! Module Compilation and Linking System
//!
//! Handles compilation of built-in modules to object files and user modules to header files

use std::path::{Path, PathBuf};
use std::fs;
use std::process::Command;
use std::collections::HashMap;
use anyhow::{Result, Context};

/// Module compilation system
pub struct ModuleCompiler {
    /// Build directory for compiled artifacts
    build_dir: PathBuf,
    /// Object files to link
    object_files: Vec<PathBuf>,
    /// Header files for user modules
    header_files: Vec<PathBuf>,
    /// Native libraries to link (.so, .dll, .dylib)
    native_libraries: Vec<PathBuf>,
}

impl ModuleCompiler {
    pub fn new(build_dir: PathBuf) -> Self {
        Self {
            build_dir,
            object_files: Vec::new(),
            header_files: Vec::new(),
            native_libraries: Vec::new(),
        }
    }

    /// Initialize build directory
    pub fn init_build_dir(&self) -> Result<()> {
        fs::create_dir_all(&self.build_dir)
            .context("Failed to create build directory")?;

        // Create subdirectories
        fs::create_dir_all(self.build_dir.join("modules"))?;
        fs::create_dir_all(self.build_dir.join("include"))?;
        fs::create_dir_all(self.build_dir.join("lib"))?;

        Ok(())
    }

    /// Compile a built-in module to an object file
    pub fn compile_builtin_module(&mut self, module_name: &str) -> Result<PathBuf> {
        let c_source = self.generate_builtin_module_source(module_name)?;
        let source_path = self.build_dir.join(format!("{}_module.c", module_name));
        let object_path = self.build_dir.join("modules").join(format!("{}_module.o", module_name));

        // Write C source
        fs::write(&source_path, c_source)
            .context("Failed to write module source")?;

        // Compile to object file
        self.compile_c_to_object(&source_path, &object_path)?;

        self.object_files.push(object_path.clone());
        Ok(object_path)
    }

    /// Generate C source for a built-in module
    fn generate_builtin_module_source(&self, module_name: &str) -> Result<String> {
        let mut code = String::new();

        code.push_str("#include <stdio.h>\n");
        code.push_str("#include <stdlib.h>\n");
        code.push_str("#include <string.h>\n");
        code.push_str("#include <math.h>\n");
        code.push_str("#include <time.h>\n");
        code.push_str("#include <stdint.h>\n");
        code.push_str("#include <stdbool.h>\n\n");

        // Add fallback type definition if not already defined
        code.push_str("#ifndef TAURARO_VALUE_T_DEFINED\n");
        code.push_str("#define TAURARO_VALUE_T_DEFINED\n");
        code.push_str("typedef void tauraro_value_t;\n");
        code.push_str("#endif\n\n");

        match module_name {
            "math" => code.push_str(&self.generate_math_module()),
            "sys" => code.push_str(&self.generate_sys_module()),
            "os" => code.push_str(&self.generate_os_module()),
            "time" => code.push_str(&self.generate_time_module()),
            "random" => code.push_str(&self.generate_random_module()),
            "json" => code.push_str(&self.generate_json_module()),
            _ => return Err(anyhow::anyhow!("Unknown built-in module: {}", module_name)),
        }

        Ok(code)
    }

    /// Generate math module implementation
    fn generate_math_module(&self) -> String {
        r#"
// Math module constants
double tauraro_math_pi = 3.14159265358979323846;
double tauraro_math_e = 2.71828182845904523536;

// sqrt function
double tauraro_math_sqrt_native(double x) {
    return sqrt(x);
}

// pow function
double tauraro_math_pow_native(double x, double y) {
    return pow(x, y);
}

// sin function
double tauraro_math_sin_native(double x) {
    return sin(x);
}

// cos function
double tauraro_math_cos_native(double x) {
    return cos(x);
}

// tan function
double tauraro_math_tan_native(double x) {
    return tan(x);
}

// log function
double tauraro_math_log_native(double x) {
    return log(x);
}

// exp function
double tauraro_math_exp_native(double x) {
    return exp(x);
}

// floor function
double tauraro_math_floor_native(double x) {
    return floor(x);
}

// ceil function
double tauraro_math_ceil_native(double x) {
    return ceil(x);
}
"#.to_string()
    }

    /// Generate sys module implementation
    fn generate_sys_module(&self) -> String {
        r#"
// Sys module constants
#ifdef _WIN32
const char* tauraro_sys_platform = "win32";
#elif __linux__
const char* tauraro_sys_platform = "linux";
#elif __APPLE__
const char* tauraro_sys_platform = "darwin";
#else
const char* tauraro_sys_platform = "unknown";
#endif

const char* tauraro_sys_version = "Tauraro 0.2.0";

// exit function
void tauraro_sys_exit_native(int code) {
    exit(code);
}
"#.to_string()
    }

    /// Generate os module implementation
    fn generate_os_module(&self) -> String {
        r#"
#include <unistd.h>
#include <dirent.h>

// getcwd function
char* tauraro_os_getcwd_native() {
    char* buf = malloc(4096);
    return getcwd(buf, 4096);
}

// listdir function - returns array of strings
typedef struct {
    char** items;
    size_t count;
} string_array_t;

string_array_t* tauraro_os_listdir_native(const char* path) {
    DIR* dir = opendir(path ? path : ".");
    if (!dir) return NULL;

    string_array_t* result = malloc(sizeof(string_array_t));
    result->count = 0;
    result->items = NULL;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        result->count++;
        result->items = realloc(result->items, result->count * sizeof(char*));
        result->items[result->count - 1] = strdup(entry->d_name);
    }

    closedir(dir);
    return result;
}
"#.to_string()
    }

    /// Generate time module implementation
    fn generate_time_module(&self) -> String {
        r#"
// time function - returns current time in seconds since epoch
double tauraro_time_time_native() {
    return (double)time(NULL);
}

// sleep function
void tauraro_time_sleep_native(double seconds) {
    #ifdef _WIN32
        Sleep((DWORD)(seconds * 1000));
    #else
        usleep((useconds_t)(seconds * 1000000));
    #endif
}
"#.to_string()
    }

    /// Generate random module implementation
    fn generate_random_module(&self) -> String {
        r#"
#include <stdlib.h>

// random function - returns random float between 0 and 1
double tauraro_random_random_native() {
    return (double)rand() / RAND_MAX;
}

// randint function - returns random integer in range [a, b]
int64_t tauraro_random_randint_native(int64_t a, int64_t b) {
    return a + rand() % (b - a + 1);
}

// seed function
void tauraro_random_seed_native(int64_t seed) {
    srand((unsigned int)seed);
}
"#.to_string()
    }

    /// Generate JSON module implementation
    fn generate_json_module(&self) -> String {
        r#"
// Simple JSON parsing - basic implementation
// This would need a full JSON parser for production use

char* tauraro_json_dumps_native(const char* data) {
    // TODO: Implement JSON serialization
    return strdup("{}");
}

void* tauraro_json_loads_native(const char* json_str) {
    // TODO: Implement JSON parsing
    return NULL;
}
"#.to_string()
    }

    /// Convert user module to header file
    pub fn convert_user_module_to_header(&mut self, module_path: &Path, module_name: &str) -> Result<PathBuf> {
        let header_path = self.build_dir.join("include").join(format!("{}.h", module_name));

        // Read the module source
        let source = fs::read_to_string(module_path)?;

        // Generate header (simplified - would need full transpilation)
        let mut header = String::new();
        header.push_str(&format!("#ifndef {}_H\n", module_name.to_uppercase()));
        header.push_str(&format!("#define {}_H\n\n", module_name.to_uppercase()));

        header.push_str("#include <stdint.h>\n");
        header.push_str("#include <stdbool.h>\n\n");

        // TODO: Parse module and generate function declarations

        header.push_str(&format!("#endif // {}_H\n", module_name.to_uppercase()));

        fs::write(&header_path, header)?;

        self.header_files.push(header_path.clone());
        Ok(header_path)
    }

    /// Compile C source to object file
    fn compile_c_to_object(&self, source: &Path, output: &Path) -> Result<()> {
        let compiler = self.detect_c_compiler()?;

        let mut cmd = Command::new(&compiler);
        cmd.arg("-c")
            .arg(source)
            .arg("-o")
            .arg(output)
            .arg("-O2")
            .arg("-fPIC"); // Position independent code for shared libraries

        #[cfg(target_os = "macos")]
        cmd.arg("-mmacosx-version-min=10.9");

        let output_result = cmd.output()
            .context("Failed to execute compiler")?;

        if !output_result.status.success() {
            let stderr = String::from_utf8_lossy(&output_result.stderr);
            return Err(anyhow::anyhow!("Compilation failed: {}", stderr));
        }

        Ok(())
    }

    /// Detect available C compiler
    fn detect_c_compiler(&self) -> Result<String> {
        // Try compilers in order of preference
        let compilers = ["gcc", "clang", "cc"];

        for compiler in &compilers {
            if Command::new(compiler).arg("--version").output().is_ok() {
                return Ok(compiler.to_string());
            }
        }

        Err(anyhow::anyhow!("No C compiler found. Please install GCC or Clang."))
    }

    /// Get list of object files to link
    pub fn get_object_files(&self) -> &[PathBuf] {
        &self.object_files
    }

    /// Get list of header files
    pub fn get_header_files(&self) -> &[PathBuf] {
        &self.header_files
    }

    /// Add native library to link
    pub fn add_native_library(&mut self, lib_path: PathBuf) {
        self.native_libraries.push(lib_path);
    }

    /// Link all object files into executable
    pub fn link_executable(&self, output: &Path, main_object: &Path) -> Result<()> {
        let compiler = self.detect_c_compiler()?;

        let mut cmd = Command::new(&compiler);
        cmd.arg(main_object);

        // Add all module object files
        for obj in &self.object_files {
            cmd.arg(obj);
        }

        // Add native libraries
        for lib in &self.native_libraries {
            cmd.arg(lib);
        }

        // Add standard math library
        cmd.arg("-lm");

        cmd.arg("-o").arg(output);

        let output_result = cmd.output()
            .context("Failed to link executable")?;

        if !output_result.status.success() {
            let stderr = String::from_utf8_lossy(&output_result.stderr);
            return Err(anyhow::anyhow!("Linking failed: {}", stderr));
        }

        Ok(())
    }
}

/// Native library loader for ctypes-style functionality
pub struct NativeLibraryLoader {
    loaded_libraries: HashMap<String, *mut std::ffi::c_void>,
}

impl NativeLibraryLoader {
    pub fn new() -> Self {
        Self {
            loaded_libraries: HashMap::new(),
        }
    }

    /// Generate C code for loading a native library
    pub fn generate_load_library_code(lib_path: &str) -> String {
        let mut code = String::new();

        code.push_str("#ifdef _WIN32\n");
        code.push_str("#include <windows.h>\n");
        code.push_str("#define dlopen(name, flags) LoadLibraryA(name)\n");
        code.push_str("#define dlsym(handle, name) GetProcAddress((HMODULE)handle, name)\n");
        code.push_str("#define dlclose(handle) FreeLibrary((HMODULE)handle)\n");
        code.push_str("#else\n");
        code.push_str("#include <dlfcn.h>\n");
        code.push_str("#endif\n\n");

        code.push_str(&format!(r#"
void* load_library_{}_handle() {{
    void* handle = dlopen("{}", RTLD_LAZY);
    if (!handle) {{
        fprintf(stderr, "Failed to load library: {}\n");
        return NULL;
    }}
    return handle;
}}
"#,
            lib_path.replace("/", "_").replace(".", "_"),
            lib_path,
            lib_path
        ));

        code
    }

    /// Generate C code for loading a function from a library
    pub fn generate_load_function_code(lib_name: &str, func_name: &str, signature: &str) -> String {
        format!(r#"
{} (*load_function_{}_{})() {{
    static void* lib_handle = NULL;
    if (!lib_handle) {{
        lib_handle = load_library_{}_handle();
    }}
    if (!lib_handle) return NULL;

    return ({} (*)())dlsym(lib_handle, "{}");
}}
"#,
            signature,
            lib_name.replace("/", "_").replace(".", "_"),
            func_name,
            lib_name.replace("/", "_").replace(".", "_"),
            signature,
            func_name
        )
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::env;

    #[test]
    fn test_module_compiler_creation() {
        let temp_dir = env::temp_dir().join("tauraro_test");
        let compiler = ModuleCompiler::new(temp_dir);
        assert_eq!(compiler.object_files.len(), 0);
    }

    #[test]
    fn test_math_module_generation() {
        let temp_dir = env::temp_dir().join("tauraro_test");
        let compiler = ModuleCompiler::new(temp_dir);
        let math_code = compiler.generate_math_module();
        assert!(math_code.contains("tauraro_math_pi"));
        assert!(math_code.contains("tauraro_math_sqrt_native"));
    }
}
