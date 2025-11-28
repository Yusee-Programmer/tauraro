//! Module Compilation System for Tauraro C Backend
//! 
//! This module handles:
//! - Compiling built-in modules to object files (stored in build/builtins/)
//! - Generating user-defined modules as header files (stored in build/headers/)
//! - Managing module dependencies during C compilation

use std::collections::HashSet;
use std::path::{Path, PathBuf};
use std::fs;
use std::io::Write;

/// Built-in module names that have FFI C implementations
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
        
        Self {
            build_dir,
            builtins_dir,
            headers_dir,
            processed_modules: HashSet::new(),
            object_files: Vec::new(),
        }
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
            self.compile_builtin_module(module_name)?;
        } else {
            // User-defined module - will be handled separately
            // The transpiler will generate the header when processing the module file
        }
        
        self.processed_modules.insert(module_name.to_string());
        Ok(())
    }
    
    /// Compile a built-in module to an object file
    fn compile_builtin_module(&mut self, module_name: &str) -> std::io::Result<()> {
        // Generate C source for the module
        let c_source = self.generate_builtin_module_c(module_name);
        
        // Write C source to file
        let c_file = self.builtins_dir.join(format!("{}_module.c", module_name));
        let mut file = fs::File::create(&c_file)?;
        file.write_all(c_source.as_bytes())?;
        
        // Also generate header file for the module
        let header = self.generate_builtin_module_header(module_name);
        let h_file = self.builtins_dir.join(format!("{}_module.h", module_name));
        let mut header_file = fs::File::create(&h_file)?;
        header_file.write_all(header.as_bytes())?;
        
        // Compile to object file
        let obj_file = self.builtins_dir.join(format!("{}_module.o", module_name));
        
        // Try to compile using available compiler
        if let Err(e) = self.compile_c_to_object(&c_file, &obj_file) {
            eprintln!("Warning: Could not compile {} to object file: {}", module_name, e);
            eprintln!("C source available at: {}", c_file.display());
        } else {
            self.object_files.push(obj_file);
        }
        
        Ok(())
    }
    
    /// Compile a C source file to an object file
    fn compile_c_to_object(&self, c_file: &Path, obj_file: &Path) -> std::io::Result<()> {
        use std::process::Command;
        
        // Try GCC first
        let gcc_result = Command::new("gcc")
            .args(&["-c", "-O2", "-fPIC"])
            .arg(c_file)
            .arg("-o")
            .arg(obj_file)
            .arg(format!("-I{}", self.builtins_dir.display()))
            .output();
            
        if let Ok(output) = gcc_result {
            if output.status.success() {
                return Ok(());
            }
        }
        
        // Try Clang
        let clang_result = Command::new("clang")
            .args(&["-c", "-O2", "-fPIC"])
            .arg(c_file)
            .arg("-o")
            .arg(obj_file)
            .arg(format!("-I{}", self.builtins_dir.display()))
            .output();
            
        if let Ok(output) = clang_result {
            if output.status.success() {
                return Ok(());
            }
        }
        
        // Try MSVC (cl.exe) on Windows
        #[cfg(windows)]
        {
            let cl_result = Command::new("cl")
                .args(&["/c", "/O2"])
                .arg(c_file)
                .arg(format!("/Fo{}", obj_file.display()))
                .arg(format!("/I{}", self.builtins_dir.display()))
                .output();
                
            if let Ok(output) = cl_result {
                if output.status.success() {
                    return Ok(());
                }
            }
        }
        
        Err(std::io::Error::new(
            std::io::ErrorKind::NotFound,
            "No C compiler found (tried gcc, clang, cl)"
        ))
    }
    
    /// Generate C source code for a built-in module
    fn generate_builtin_module_c(&self, module_name: &str) -> String {
        match module_name {
            "math" => self.generate_math_module_c(),
            "os" => self.generate_os_module_c(),
            "sys" => self.generate_sys_module_c(),
            "json" => self.generate_json_module_c(),
            "time" => self.generate_time_module_c(),
            "datetime" => self.generate_datetime_module_c(),
            "io" => self.generate_io_module_c(),
            "random" => self.generate_random_module_c(),
            "collections" => self.generate_collections_module_c(),
            "functools" => self.generate_functools_module_c(),
            "itertools" => self.generate_itertools_module_c(),
            "re" => self.generate_re_module_c(),
            "hashlib" => self.generate_hashlib_module_c(),
            "base64" => self.generate_base64_module_c(),
            "copy" => self.generate_copy_module_c(),
            "gc" => self.generate_gc_module_c(),
            "memory" => self.generate_memory_module_c(),
            "threading" => self.generate_threading_module_c(),
            "socket" => self.generate_socket_module_c(),
            "csv" => self.generate_csv_module_c(),
            "logging" => self.generate_logging_module_c(),
            "unittest" => self.generate_unittest_module_c(),
            "pickle" => self.generate_pickle_module_c(),
            "urllib" => self.generate_urllib_module_c(),
            "exceptions" => self.generate_exceptions_module_c(),
            _ => self.generate_stub_module_c(module_name),
        }
    }
    
    /// Generate header file for a built-in module
    fn generate_builtin_module_header(&self, module_name: &str) -> String {
        let guard = format!("TAURARO_{}_MODULE_H", module_name.to_uppercase());
        format!(
r#"// Tauraro {} Module Header
// Auto-generated - DO NOT EDIT

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

// Module initialization
void tauraro_{}_module_init(void);

// Module attribute access
tauraro_value_t* tauraro_{}_get_attr(const char* name);

// Module function declarations
{}

#endif // {}
"#,
            module_name,
            guard, guard,
            module_name, module_name,
            self.get_module_function_declarations(module_name),
            guard
        )
    }
    
    /// Get function declarations for a module
    fn get_module_function_declarations(&self, module_name: &str) -> String {
        match module_name {
            "math" => r#"
extern double tauraro_math_pi;
extern double tauraro_math_e;
extern double tauraro_math_tau;
extern double tauraro_math_inf;
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
tauraro_value_t* tauraro_math_abs(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_fabs(int argc, tauraro_value_t** argv);
"#.to_string(),
            "os" => r#"
tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_chdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_listdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_mkdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rmdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_remove(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rename(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_getenv(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_putenv(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_system(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_exists(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isfile(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_join(int argc, tauraro_value_t** argv);
"#.to_string(),
            "sys" => r#"
tauraro_value_t* tauraro_sys_argv;
tauraro_value_t* tauraro_sys_exit(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_sys_platform(void);
tauraro_value_t* tauraro_sys_version(void);
"#.to_string(),
            "json" => r#"
tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_dump(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_load(int argc, tauraro_value_t** argv);
"#.to_string(),
            "time" => r#"
tauraro_value_t* tauraro_time_time(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_time_sleep(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_time_localtime(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_time_gmtime(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_time_strftime(int argc, tauraro_value_t** argv);
"#.to_string(),
            "random" => r#"
tauraro_value_t* tauraro_random_random(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_randint(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_choice(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_shuffle(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_random_seed(int argc, tauraro_value_t** argv);
"#.to_string(),
            _ => "// No specific functions declared\n".to_string(),
        }
    }
    
    // Module C source generators
    
    fn generate_math_module_c(&self) -> String {
        r#"// Tauraro Math Module Implementation
// Auto-generated C implementation for the math built-in module

#include "math_module.h"
#include <math.h>

// Math constants
double tauraro_math_pi = 3.141592653589793;
double tauraro_math_e = 2.718281828459045;
double tauraro_math_tau = 6.283185307179586;
double tauraro_math_inf = 1.0 / 0.0;  // INFINITY

static int math_initialized = 0;

void tauraro_math_module_init(void) {
    if (math_initialized) return;
    math_initialized = 1;
}

// Helper to get numeric value
static double get_number(tauraro_value_t* val) {
    if (val == NULL) return 0.0;
    if (val->type == 0) return (double)val->data.int_val;  // TAURARO_INT
    if (val->type == 1) return val->data.float_val;        // TAURARO_FLOAT
    return 0.0;
}

tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x < 0) return tauraro_none();
    return tauraro_float(sqrt(x));
}

tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv) {
    if (argc < 2 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    double y = get_number(argv[1]);
    return tauraro_float(pow(x, y));
}

tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(sin(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(cos(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_tan(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(tan(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_log(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x <= 0) return tauraro_none();
    return tauraro_float(log(x));
}

tauraro_value_t* tauraro_math_log10(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x <= 0) return tauraro_none();
    return tauraro_float(log10(x));
}

tauraro_value_t* tauraro_math_log2(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x <= 0) return tauraro_none();
    return tauraro_float(log2(x));
}

tauraro_value_t* tauraro_math_exp(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(exp(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_floor(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(floor(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_ceil(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(ceil(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_abs(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    return tauraro_float(fabs(x));
}

tauraro_value_t* tauraro_math_fabs(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(fabs(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_get_attr(const char* name) {
    if (strcmp(name, "pi") == 0) return tauraro_float(tauraro_math_pi);
    if (strcmp(name, "e") == 0) return tauraro_float(tauraro_math_e);
    if (strcmp(name, "tau") == 0) return tauraro_float(tauraro_math_tau);
    if (strcmp(name, "inf") == 0) return tauraro_float(tauraro_math_inf);
    // For functions, return a function pointer wrapper
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_os_module_c(&self) -> String {
        r#"// Tauraro OS Module Implementation
// Auto-generated C implementation for the os built-in module

#include "os_module.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#define mkdir(path, mode) _mkdir(path)
#define rmdir _rmdir
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

static int os_initialized = 0;

void tauraro_os_module_init(void) {
    if (os_initialized) return;
    os_initialized = 1;
}

tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv) {
    (void)argc; (void)argv;
    char buffer[4096];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return tauraro_string(buffer);
    }
    return tauraro_none();
}

tauraro_value_t* tauraro_os_chdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);  // TAURARO_STRING
    int result = chdir(argv[0]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_mkdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
    int result = mkdir(argv[0]->data.str_val, 0755);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_rmdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
    int result = rmdir(argv[0]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_remove(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
    int result = remove(argv[0]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_rename(int argc, tauraro_value_t** argv) {
    if (argc < 2 || argv == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3 || argv[1]->type != 3) return tauraro_bool(0);
    int result = rename(argv[0]->data.str_val, argv[1]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_getenv(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    if (argv[0]->type != 3) return tauraro_none();
    char* value = getenv(argv[0]->data.str_val);
    if (value != NULL) {
        return tauraro_string(value);
    }
    return tauraro_none();
}

tauraro_value_t* tauraro_os_system(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_int(-1);
    if (argv[0]->type != 3) return tauraro_int(-1);
    int result = system(argv[0]->data.str_val);
    return tauraro_int(result);
}

tauraro_value_t* tauraro_os_path_exists(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(argv[0]->data.str_val);
    return tauraro_bool(attrs != INVALID_FILE_ATTRIBUTES);
#else
    struct stat st;
    return tauraro_bool(stat(argv[0]->data.str_val, &st) == 0);
#endif
}

tauraro_value_t* tauraro_os_path_isfile(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(argv[0]->data.str_val);
    return tauraro_bool(attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    if (stat(argv[0]->data.str_val, &st) != 0) return tauraro_bool(0);
    return tauraro_bool(S_ISREG(st.st_mode));
#endif
}

tauraro_value_t* tauraro_os_path_isdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(argv[0]->data.str_val);
    return tauraro_bool(attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    if (stat(argv[0]->data.str_val, &st) != 0) return tauraro_bool(0);
    return tauraro_bool(S_ISDIR(st.st_mode));
#endif
}

tauraro_value_t* tauraro_os_get_attr(const char* name) {
    if (strcmp(name, "name") == 0) {
#ifdef _WIN32
        return tauraro_string("nt");
#else
        return tauraro_string("posix");
#endif
    }
    if (strcmp(name, "sep") == 0) {
#ifdef _WIN32
        return tauraro_string("\\");
#else
        return tauraro_string("/");
#endif
    }
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_sys_module_c(&self) -> String {
        r#"// Tauraro Sys Module Implementation
// Auto-generated C implementation for the sys built-in module

#include "sys_module.h"

static int sys_initialized = 0;
static tauraro_value_t* sys_argv_val = NULL;

void tauraro_sys_module_init(void) {
    if (sys_initialized) return;
    sys_initialized = 1;
}

void tauraro_sys_set_argv(int argc, char** argv) {
    // Create list of arguments (simplified)
    sys_argv_val = tauraro_value_new();
    sys_argv_val->type = 4;  // TAURARO_LIST
    // In full implementation, would populate list with arguments
}

tauraro_value_t* tauraro_sys_exit(int argc, tauraro_value_t** argv) {
    int code = 0;
    if (argc >= 1 && argv != NULL && argv[0] != NULL) {
        if (argv[0]->type == 0) {  // TAURARO_INT
            code = (int)argv[0]->data.int_val;
        }
    }
    exit(code);
    return tauraro_none();  // Never reached
}

tauraro_value_t* tauraro_sys_platform(void) {
#ifdef _WIN32
    return tauraro_string("win32");
#elif __APPLE__
    return tauraro_string("darwin");
#elif __linux__
    return tauraro_string("linux");
#else
    return tauraro_string("unknown");
#endif
}

tauraro_value_t* tauraro_sys_version(void) {
    return tauraro_string("Tauraro 1.0.0 (C Backend)");
}

tauraro_value_t* tauraro_sys_get_attr(const char* name) {
    if (strcmp(name, "platform") == 0) return tauraro_sys_platform();
    if (strcmp(name, "version") == 0) return tauraro_sys_version();
    if (strcmp(name, "argv") == 0) return sys_argv_val ? sys_argv_val : tauraro_none();
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_json_module_c(&self) -> String {
        r#"// Tauraro JSON Module Implementation
// Auto-generated C implementation for the json built-in module

#include "json_module.h"

static int json_initialized = 0;

void tauraro_json_module_init(void) {
    if (json_initialized) return;
    json_initialized = 1;
}

// Simple JSON string builder (basic implementation)
static void append_char(char** buf, int* len, int* cap, char c) {
    if (*len >= *cap - 1) {
        *cap *= 2;
        *buf = (char*)realloc(*buf, *cap);
    }
    (*buf)[(*len)++] = c;
    (*buf)[*len] = '\0';
}

static void append_str(char** buf, int* len, int* cap, const char* s) {
    while (*s) append_char(buf, len, cap, *s++);
}

// Serialize value to JSON string
static void value_to_json(tauraro_value_t* val, char** buf, int* len, int* cap) {
    if (val == NULL) {
        append_str(buf, len, cap, "null");
        return;
    }
    
    switch (val->type) {
        case 0:  // INT
            {
                char num[32];
                snprintf(num, sizeof(num), "%lld", val->data.int_val);
                append_str(buf, len, cap, num);
            }
            break;
        case 1:  // FLOAT
            {
                char num[64];
                snprintf(num, sizeof(num), "%g", val->data.float_val);
                append_str(buf, len, cap, num);
            }
            break;
        case 2:  // BOOL
            append_str(buf, len, cap, val->data.bool_val ? "true" : "false");
            break;
        case 3:  // STRING
            append_char(buf, len, cap, '"');
            if (val->data.str_val) {
                const char* s = val->data.str_val;
                while (*s) {
                    if (*s == '"' || *s == '\\') append_char(buf, len, cap, '\\');
                    append_char(buf, len, cap, *s++);
                }
            }
            append_char(buf, len, cap, '"');
            break;
        case 8:  // NONE
            append_str(buf, len, cap, "null");
            break;
        default:
            append_str(buf, len, cap, "null");
            break;
    }
}

tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) {
        return tauraro_string("null");
    }
    
    int cap = 256;
    int len = 0;
    char* buf = (char*)malloc(cap);
    buf[0] = '\0';
    
    value_to_json(argv[0], &buf, &len, &cap);
    
    tauraro_value_t* result = tauraro_string(buf);
    free(buf);
    return result;
}

tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    if (argv[0]->type != 3) return tauraro_none();  // Must be string
    
    const char* json = argv[0]->data.str_val;
    if (json == NULL) return tauraro_none();
    
    // Skip whitespace
    while (*json == ' ' || *json == '\t' || *json == '\n' || *json == '\r') json++;
    
    // Parse based on first character
    if (*json == 'n' && strncmp(json, "null", 4) == 0) {
        return tauraro_none();
    }
    if (*json == 't' && strncmp(json, "true", 4) == 0) {
        return tauraro_bool(1);
    }
    if (*json == 'f' && strncmp(json, "false", 5) == 0) {
        return tauraro_bool(0);
    }
    if (*json == '"') {
        // Parse string
        json++;
        char* end = strchr(json, '"');
        if (end) {
            int len = end - json;
            char* str = (char*)malloc(len + 1);
            strncpy(str, json, len);
            str[len] = '\0';
            tauraro_value_t* result = tauraro_string(str);
            free(str);
            return result;
        }
        return tauraro_none();
    }
    if (*json == '-' || (*json >= '0' && *json <= '9')) {
        // Parse number
        char* endptr;
        double val = strtod(json, &endptr);
        if (strchr(json, '.') || strchr(json, 'e') || strchr(json, 'E')) {
            return tauraro_float(val);
        }
        return tauraro_int((long long)val);
    }
    
    return tauraro_none();
}

tauraro_value_t* tauraro_json_get_attr(const char* name) {
    (void)name;
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_time_module_c(&self) -> String {
        r#"// Tauraro Time Module Implementation
// Auto-generated C implementation for the time built-in module

#include "time_module.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static int time_initialized = 0;

void tauraro_time_module_init(void) {
    if (time_initialized) return;
    time_initialized = 1;
}

tauraro_value_t* tauraro_time_time(int argc, tauraro_value_t** argv) {
    (void)argc; (void)argv;
    return tauraro_float((double)time(NULL));
}

tauraro_value_t* tauraro_time_sleep(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    
    double seconds = 0;
    if (argv[0]->type == 0) seconds = (double)argv[0]->data.int_val;
    else if (argv[0]->type == 1) seconds = argv[0]->data.float_val;
    
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000));
#else
    usleep((useconds_t)(seconds * 1000000));
#endif
    
    return tauraro_none();
}

tauraro_value_t* tauraro_time_get_attr(const char* name) {
    (void)name;
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_datetime_module_c(&self) -> String {
        self.generate_stub_module_c("datetime")
    }
    
    fn generate_io_module_c(&self) -> String {
        r#"// Tauraro IO Module Implementation
// Auto-generated C implementation for the io built-in module

#include "io_module.h"

static int io_initialized = 0;

void tauraro_io_module_init(void) {
    if (io_initialized) return;
    io_initialized = 1;
}

tauraro_value_t* tauraro_io_open(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    if (argv[0]->type != 3) return tauraro_none();
    
    const char* path = argv[0]->data.str_val;
    const char* mode = "r";
    if (argc >= 2 && argv[1] != NULL && argv[1]->type == 3) {
        mode = argv[1]->data.str_val;
    }
    
    FILE* f = fopen(path, mode);
    if (f == NULL) return tauraro_none();
    
    // Return file handle as pointer
    tauraro_value_t* result = tauraro_value_new();
    result->type = 9;  // TAURARO_OBJECT
    result->data.ptr_val = f;
    return result;
}

tauraro_value_t* tauraro_io_get_attr(const char* name) {
    (void)name;
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_random_module_c(&self) -> String {
        r#"// Tauraro Random Module Implementation
// Auto-generated C implementation for the random built-in module

#include "random_module.h"
#include <time.h>

static int random_initialized = 0;

void tauraro_random_module_init(void) {
    if (random_initialized) return;
    srand((unsigned int)time(NULL));
    random_initialized = 1;
}

tauraro_value_t* tauraro_random_random(int argc, tauraro_value_t** argv) {
    (void)argc; (void)argv;
    return tauraro_float((double)rand() / RAND_MAX);
}

tauraro_value_t* tauraro_random_randint(int argc, tauraro_value_t** argv) {
    if (argc < 2 || argv == NULL) return tauraro_int(0);
    
    long long a = 0, b = 0;
    if (argv[0]->type == 0) a = argv[0]->data.int_val;
    if (argv[1]->type == 0) b = argv[1]->data.int_val;
    
    if (b <= a) return tauraro_int(a);
    return tauraro_int(a + rand() % (b - a + 1));
}

tauraro_value_t* tauraro_random_seed(int argc, tauraro_value_t** argv) {
    if (argc >= 1 && argv != NULL && argv[0] != NULL && argv[0]->type == 0) {
        srand((unsigned int)argv[0]->data.int_val);
    }
    return tauraro_none();
}

tauraro_value_t* tauraro_random_get_attr(const char* name) {
    (void)name;
    return tauraro_none();
}
"#.to_string()
    }
    
    fn generate_collections_module_c(&self) -> String {
        self.generate_stub_module_c("collections")
    }
    
    fn generate_functools_module_c(&self) -> String {
        self.generate_stub_module_c("functools")
    }
    
    fn generate_itertools_module_c(&self) -> String {
        self.generate_stub_module_c("itertools")
    }
    
    fn generate_re_module_c(&self) -> String {
        self.generate_stub_module_c("re")
    }
    
    fn generate_hashlib_module_c(&self) -> String {
        self.generate_stub_module_c("hashlib")
    }
    
    fn generate_base64_module_c(&self) -> String {
        self.generate_stub_module_c("base64")
    }
    
    fn generate_copy_module_c(&self) -> String {
        self.generate_stub_module_c("copy")
    }
    
    fn generate_gc_module_c(&self) -> String {
        self.generate_stub_module_c("gc")
    }
    
    fn generate_memory_module_c(&self) -> String {
        self.generate_stub_module_c("memory")
    }
    
    fn generate_threading_module_c(&self) -> String {
        self.generate_stub_module_c("threading")
    }
    
    fn generate_socket_module_c(&self) -> String {
        self.generate_stub_module_c("socket")
    }
    
    fn generate_csv_module_c(&self) -> String {
        self.generate_stub_module_c("csv")
    }
    
    fn generate_logging_module_c(&self) -> String {
        self.generate_stub_module_c("logging")
    }
    
    fn generate_unittest_module_c(&self) -> String {
        self.generate_stub_module_c("unittest")
    }
    
    fn generate_pickle_module_c(&self) -> String {
        self.generate_stub_module_c("pickle")
    }
    
    fn generate_urllib_module_c(&self) -> String {
        self.generate_stub_module_c("urllib")
    }
    
    fn generate_exceptions_module_c(&self) -> String {
        self.generate_stub_module_c("exceptions")
    }
    
    fn generate_stub_module_c(&self, module_name: &str) -> String {
        format!(
r#"// Tauraro {} Module Implementation
// Auto-generated stub C implementation

#include "{}_module.h"

static int {}_initialized = 0;

void tauraro_{}_module_init(void) {{
    if ({}_initialized) return;
    {}_initialized = 1;
}}

tauraro_value_t* tauraro_{}_get_attr(const char* name) {{
    (void)name;
    return tauraro_none();
}}
"#,
            module_name, module_name,
            module_name.replace("-", "_"),
            module_name,
            module_name.replace("-", "_"),
            module_name.replace("-", "_"),
            module_name
        )
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
        
        let mut file = fs::File::create(&header_path)?;
        file.write_all(content.as_bytes())?;
        
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
