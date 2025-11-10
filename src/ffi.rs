//! Foreign Function Interface for Tauraro
//!
//! This module provides comprehensive FFI support for loading and calling functions from
//! native libraries across multiple platforms:
//! - Windows (DLL)
//! - Linux (SO - Shared Objects)
//! - macOS (dylib)
//! - iOS (dylib/framework)
//! - Android (SO)
//! - Embedded systems
//! - Unix-like systems

use crate::value::Value;
use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex};
use anyhow::{Result, anyhow, Context};
use libloading::{Library, Symbol};
use std::ffi::{CString, CStr};
use std::os::raw::{c_char, c_int, c_long, c_double, c_float, c_void};

// Use libffi for more robust FFI function calling when available
#[cfg(feature = "ffi")]
use libffi::middle::{Cif, CodePtr, Arg, Type};

/// Represents different FFI types supported across platforms
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum FFIType {
    // Integer types
    Int,        // c_int (typically i32)
    Int8,       // i8
    Int16,      // i16
    Int32,      // i32
    Int64,      // i64
    UInt,       // c_uint
    UInt8,      // u8
    UInt16,     // u16
    UInt32,     // u32
    UInt64,     // u64

    // Floating point types
    Float,      // f32 / c_float
    Double,     // f64 / c_double

    // Character and string types
    Char,       // c_char
    String,     // *const c_char (null-terminated string)
    WString,    // Wide string (UTF-16 on Windows)

    // Pointer types
    Pointer,    // *mut c_void (generic pointer)
    ConstPointer, // *const c_void

    // Size types
    SizeT,      // size_t (usize)
    SSizeT,     // ssize_t (isize)

    // Long types
    Long,       // c_long
    ULong,      // c_ulong
    LongLong,   // c_longlong (i64)
    ULongLong,  // c_ulonglong (u64)

    // Special types
    Void,       // void (for return types)
    Bool,       // bool (c_bool / u8)

    // Structure and array types
    Struct(String),      // Named struct type
    Array(Box<FFIType>, usize), // Array type with length
}

/// Calling conventions for different platforms
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum CallingConvention {
    C,          // Standard C calling convention
    Stdcall,    // Windows stdcall (x86)
    Fastcall,   // Fastcall convention
    Cdecl,      // C declaration convention
    Thiscall,   // C++ this call
    Vectorcall, // Vector calling convention
}

/// Function signature for FFI functions
#[derive(Debug, Clone)]
pub struct FFISignature {
    pub name: String,
    pub return_type: FFIType,
    pub param_types: Vec<FFIType>,
    pub calling_convention: CallingConvention,
    pub is_variadic: bool,
}

/// Represents a loaded external function
pub struct FFIExternalFunction {
    pub signature: FFISignature,
    symbol_ptr: *mut c_void,
    #[cfg(feature = "ffi")]
    cif: Option<Cif>,
}

// Manual implementation of Send and Sync for FFIExternalFunction
// This is safe because we ensure proper synchronization through the FFIManager's mutex
unsafe impl Send for FFIExternalFunction {}
unsafe impl Sync for FFIExternalFunction {}

/// Represents a loaded dynamic library
pub struct FFILibrary {
    pub name: String,
    pub path: PathBuf,
    library: Arc<Library>,
    functions: HashMap<String, FFIExternalFunction>,
}

/// Main FFI manager for Tauraro
pub struct FFIManager {
    libraries: HashMap<String, Arc<Mutex<FFILibrary>>>,
    search_paths: Vec<PathBuf>,
}

impl FFIManager {
    /// Create a new FFI manager
    pub fn new() -> Self {
        let mut manager = Self {
            libraries: HashMap::new(),
            search_paths: Vec::new(),
        };

        // Add default system library paths
        manager.add_default_search_paths();
        manager
    }

    /// Add default system library search paths based on platform
    fn add_default_search_paths(&mut self) {
        #[cfg(target_os = "windows")]
        {
            // Current directory (highest priority)
            if let Ok(current_dir) = std::env::current_dir() {
                self.search_paths.push(current_dir);
            }

            // Windows system directories
            if let Ok(sys_dir) = std::env::var("SystemRoot") {
                self.search_paths.push(PathBuf::from(format!("{}\\System32", sys_dir)));
                self.search_paths.push(PathBuf::from(format!("{}\\SysWOW64", sys_dir)));
            }

            // Common MSYS2/MinGW paths
            let common_paths = vec![
                "C:\\msys64\\mingw64\\bin",
                "C:\\msys64\\mingw32\\bin",
                "C:\\msys64\\usr\\bin",
                "C:\\mingw64\\bin",
                "C:\\mingw32\\bin",
            ];
            for path in common_paths {
                let path_buf = PathBuf::from(path);
                if path_buf.exists() {
                    self.search_paths.push(path_buf);
                }
            }

            // Check PATH environment variable
            if let Ok(path_var) = std::env::var("PATH") {
                for path in path_var.split(';') {
                    let path_buf = PathBuf::from(path);
                    if path_buf.exists() && !self.search_paths.contains(&path_buf) {
                        self.search_paths.push(path_buf);
                    }
                }
            }
        }

        #[cfg(target_os = "linux")]
        {
            // Linux standard library paths
            self.search_paths.push(PathBuf::from("/lib"));
            self.search_paths.push(PathBuf::from("/usr/lib"));
            self.search_paths.push(PathBuf::from("/usr/local/lib"));
            self.search_paths.push(PathBuf::from("/lib64"));
            self.search_paths.push(PathBuf::from("/usr/lib64"));

            // Check LD_LIBRARY_PATH
            if let Ok(ld_path) = std::env::var("LD_LIBRARY_PATH") {
                for path in ld_path.split(':') {
                    self.search_paths.push(PathBuf::from(path));
                }
            }
        }

        #[cfg(target_os = "macos")]
        {
            // macOS standard library paths
            self.search_paths.push(PathBuf::from("/usr/lib"));
            self.search_paths.push(PathBuf::from("/usr/local/lib"));
            self.search_paths.push(PathBuf::from("/opt/homebrew/lib"));
            self.search_paths.push(PathBuf::from("/opt/local/lib"));

            // Check DYLD_LIBRARY_PATH
            if let Ok(dyld_path) = std::env::var("DYLD_LIBRARY_PATH") {
                for path in dyld_path.split(':') {
                    self.search_paths.push(PathBuf::from(path));
                }
            }
        }

        #[cfg(target_os = "android")]
        {
            // Android library paths
            self.search_paths.push(PathBuf::from("/system/lib"));
            self.search_paths.push(PathBuf::from("/system/lib64"));
            self.search_paths.push(PathBuf::from("/vendor/lib"));
            self.search_paths.push(PathBuf::from("/vendor/lib64"));
        }

        #[cfg(target_os = "ios")]
        {
            // iOS framework paths
            self.search_paths.push(PathBuf::from("/System/Library/Frameworks"));
            self.search_paths.push(PathBuf::from("/System/Library/PrivateFrameworks"));
        }
    }

    /// Add a custom search path for libraries
    pub fn add_search_path<P: AsRef<Path>>(&mut self, path: P) {
        self.search_paths.push(path.as_ref().to_path_buf());
    }

    /// Find a library in the search paths with platform-specific extensions
    fn find_library(&self, library_name: &str) -> Result<PathBuf> {
        // Check if it's already a full path (absolute)
        let path = Path::new(library_name);
        if path.is_absolute() {
            if path.exists() {
                return Ok(path.to_path_buf());
            } else {
                return Err(anyhow!("Absolute path does not exist: {}", library_name));
            }
        }

        // Check if it's a relative path with directory components
        if library_name.contains('/') || library_name.contains('\\') {
            if path.exists() {
                return Ok(path.to_path_buf());
            } else {
                return Err(anyhow!("Relative path does not exist: {}", library_name));
            }
        }

        // Check if the library name already has a known extension
        let has_extension = path.extension().is_some();
        let extensions = self.get_platform_extensions();

        // First, try the library name as-is in all search paths
        if has_extension {
            for search_path in &self.search_paths {
                let candidate = search_path.join(library_name);
                if candidate.exists() {
                    return Ok(candidate);
                }
            }
        }

        // Try with different extensions and prefixes
        for search_path in &self.search_paths {
            for ext in &extensions {
                // If no extension, try adding one
                if !has_extension {
                    let mut candidate = search_path.join(library_name);
                    candidate.set_extension(ext);
                    if candidate.exists() {
                        return Ok(candidate);
                    }

                    // Try with lib prefix (Unix-like systems)
                    #[cfg(not(target_os = "windows"))]
                    {
                        let lib_name = format!("lib{}", library_name);
                        candidate = search_path.join(&lib_name);
                        candidate.set_extension(ext);
                        if candidate.exists() {
                            return Ok(candidate);
                        }
                    }
                }
            }
        }

        // Last resort: try loading directly (system might find it via DLL search order)
        // But only if we haven't found it in our search paths
        Ok(PathBuf::from(library_name))
    }

    /// Get platform-specific library extensions
    fn get_platform_extensions(&self) -> Vec<&'static str> {
        #[cfg(target_os = "windows")]
        return vec!["dll", "DLL"];

        #[cfg(target_os = "linux")]
        return vec!["so", "so.1", "so.2", "so.3"];

        #[cfg(target_os = "macos")]
        return vec!["dylib", "so"];

        #[cfg(target_os = "android")]
        return vec!["so"];

        #[cfg(target_os = "ios")]
        return vec!["dylib", "framework"];

        #[cfg(not(any(
            target_os = "windows",
            target_os = "linux",
            target_os = "macos",
            target_os = "android",
            target_os = "ios"
        )))]
        return vec!["so", "a"];
    }

    /// Load a dynamic library
    ///
    /// # Arguments
    /// * `library_name` - Name or path of the library (e.g., "mylib", "libmath.so", "kernel32.dll")
    ///
    /// # Examples
    /// ```
    /// // Windows
    /// manager.load_library("kernel32.dll")?;
    /// manager.load_library("user32")?;
    ///
    /// // Linux
    /// manager.load_library("libm.so")?;
    /// manager.load_library("m")?;
    ///
    /// // macOS
    /// manager.load_library("libSystem.dylib")?;
    /// ```
    pub fn load_library(&mut self, library_name: &str) -> Result<()> {
        // Check if already loaded
        if self.libraries.contains_key(library_name) {
            return Ok(());
        }

        // Find the library file
        let library_path = self.find_library(library_name)
            .context(format!("Failed to find library: {}", library_name))?;

        // On Windows, add the library's directory to the DLL search path
        // so that dependency DLLs can be found
        #[cfg(target_os = "windows")]
        {
            if let Some(parent_dir) = library_path.parent() {
                if let Some(dir_str) = parent_dir.to_str() {
                    // Temporarily add to PATH so Windows can find dependency DLLs
                    if let Ok(current_path) = std::env::var("PATH") {
                        if !current_path.contains(dir_str) {
                            let new_path = format!("{};{}", dir_str, current_path);
                            std::env::set_var("PATH", new_path);
                        }
                    }
                }
            }
        }

        // Load the library
        let library = unsafe {
            Library::new(&library_path)
                .map_err(|e| anyhow!("Failed to load library from path: {:?}\nReason: {}\n\nThis usually means the DLL has missing dependencies. Try running:\n  - On Windows: Use 'Dependencies.exe' or 'dumpbin /dependents {}' to see missing DLLs\n  - Make sure all dependency DLLs are in the same directory or in your PATH",
                    library_path, e, library_path.display()))?
        };

        println!("Successfully loaded library: {} from {:?}", library_name, library_path);

        let ffi_library = FFILibrary {
            name: library_name.to_string(),
            path: library_path,
            library: Arc::new(library),
            functions: HashMap::new(),
        };

        self.libraries.insert(
            library_name.to_string(),
            Arc::new(Mutex::new(ffi_library))
        );

        Ok(())
    }

    /// Define a function signature for an external function
    ///
    /// # Arguments
    /// * `library_name` - Name of the library containing the function
    /// * `function_name` - Name of the function in the library
    /// * `return_type` - Return type of the function
    /// * `param_types` - Parameter types of the function
    /// * `calling_convention` - Optional calling convention (defaults to C)
    pub fn define_function(
        &mut self,
        library_name: &str,
        function_name: &str,
        return_type: FFIType,
        param_types: Vec<FFIType>,
        calling_convention: Option<CallingConvention>,
    ) -> Result<()> {
        let library = self.libraries.get(library_name)
            .ok_or_else(|| anyhow!("Library not loaded: {}", library_name))?;

        let mut lib = library.lock().unwrap();

        // Get the function symbol
        let symbol_ptr = unsafe {
            let symbol: Symbol<*mut c_void> = lib.library
                .get(function_name.as_bytes())
                .context(format!("Function not found: {}", function_name))?;
            *symbol
        };

        // Create CIF for libffi (if feature is enabled)
        #[cfg(feature = "ffi")]
        // Convert FFI types to libffi types
        let return_ffi_type = self.ffi_type_to_libffi(&return_type)?;
        #[cfg(feature = "ffi")]
        let param_ffi_types: Result<Vec<Type>> = param_types.iter()
            .map(|t| self.ffi_type_to_libffi(t))
            .collect();
        #[cfg(feature = "ffi")]
        let param_ffi_types = param_ffi_types?;
        
        // Create CIF for function calling
        // For now, we'll set cif to None and use manual transmutation
        // In a more complete implementation, we would use libffi for more robust function calling
        #[cfg(feature = "ffi")]
        let cif = None;  // This matches Option<Cif>
        
        #[cfg(not(feature = "ffi"))]
        let cif = None;

        let signature = FFISignature {
            name: function_name.to_string(),
            return_type,
            param_types,
            calling_convention: calling_convention.unwrap_or(CallingConvention::C),
            is_variadic: false,
        };

        let external_function = FFIExternalFunction {
            signature,
            symbol_ptr,
            #[cfg(feature = "ffi")]
            cif,
        };

        lib.functions.insert(function_name.to_string(), external_function);

        Ok(())
    }

    /// Convert FFIType to libffi Type
    #[cfg(feature = "ffi")]
    fn ffi_type_to_libffi(&self, ffi_type: &FFIType) -> Result<Type> {
        match ffi_type {
            FFIType::Void => Ok(Type::void()),
            FFIType::Int | FFIType::Int32 => Ok(Type::i32()),
            FFIType::Int8 => Ok(Type::i8()),
            FFIType::Int16 => Ok(Type::i16()),
            FFIType::Int64 => Ok(Type::i64()),
            FFIType::UInt | FFIType::UInt32 => Ok(Type::u32()),
            FFIType::UInt8 => Ok(Type::u8()),
            FFIType::UInt16 => Ok(Type::u16()),
            FFIType::UInt64 => Ok(Type::u64()),
            FFIType::Float => Ok(Type::f32()),
            FFIType::Double => Ok(Type::f64()),
            FFIType::Char => Ok(Type::i8()), // char is typically i8 in C
            FFIType::String => Ok(Type::pointer()), // String is a pointer to char
            FFIType::Pointer | FFIType::ConstPointer => Ok(Type::pointer()),
            FFIType::Bool => Ok(Type::u8()), // bool is typically u8 in C
            FFIType::SizeT => Ok(Type::usize()),
            FFIType::SSizeT => Ok(Type::isize()),
            // Use i64 for long types as a reasonable approximation
            FFIType::Long => Ok(Type::i64()),
            FFIType::ULong => Ok(Type::u64()),
            FFIType::LongLong => Ok(Type::i64()),
            FFIType::ULongLong => Ok(Type::u64()),
            _ => Err(anyhow!("Unsupported FFI type for libffi: {:?}", ffi_type)),
        }
    }

    /// Call an external function
    ///
    /// # Arguments
    /// * `library_name` - Name of the library
    /// * `function_name` - Name of the function
    /// * `args` - Arguments to pass to the function
    pub fn call_external_function(
        &self,
        library_name: &str,
        function_name: &str,
        args: Vec<Value>,
    ) -> Result<Value> {
        let library = self.libraries.get(library_name)
            .ok_or_else(|| anyhow!("Library not loaded: {}", library_name))?;

        let lib = library.lock().unwrap();
        let function = lib.functions.get(function_name)
            .ok_or_else(|| anyhow!("Function not defined: {}", function_name))?;

        // Validate argument count
        if args.len() != function.signature.param_types.len() {
            return Err(anyhow!(
                "Argument count mismatch: expected {}, got {}",
                function.signature.param_types.len(),
                args.len()
            ));
        }

        // Call the function based on signature
        self.call_function_by_signature(function, args)
    }

    /// Internal function to call an FFI function with type marshalling
    fn call_function_by_signature(
        &self,
        function: &FFIExternalFunction,
        args: Vec<Value>,
    ) -> Result<Value> {
        let sig = &function.signature;

        // Use libffi for function calling when available and CIF is available
        #[cfg(feature = "ffi")]
        if let Some(cif) = &function.cif {
            return self.call_function_with_libffi(cif, function, args);
        }

        // Fallback to manual transmutation for simple cases
        match (sig.return_type.clone(), sig.param_types.as_slice()) {
            // No arguments, void return
            (FFIType::Void, &[]) => {
                unsafe {
                    let func: unsafe extern "C" fn() = std::mem::transmute(function.symbol_ptr);
                    func();
                }
                Ok(Value::None)
            }

            // No arguments, int return
            (FFIType::Int | FFIType::Int32 | FFIType::UInt | FFIType::UInt32, &[]) => {
                unsafe {
                    let func: unsafe extern "C" fn() -> c_int = std::mem::transmute(function.symbol_ptr);
                    let result = func();
                    Ok(Value::Int(result as i64))
                }
            }

            // One int argument, int return
            (FFIType::Int | FFIType::Int32, &[FFIType::Int | FFIType::Int32]) => {
                let arg = self.value_to_int(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(arg);
                    Ok(Value::Int(result as i64))
                }
            }

            // Two int arguments, int return
            (FFIType::Int | FFIType::Int32, &[FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32]) => {
                let arg1 = self.value_to_int(&args[0])?;
                let arg2 = self.value_to_int(&args[1])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_int, c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(arg1, arg2);
                    Ok(Value::Int(result as i64))
                }
            }

            // Int argument, void return
            (FFIType::Void, &[FFIType::Int | FFIType::Int32 | FFIType::UInt | FFIType::UInt32]) => {
                let arg = self.value_to_int(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_int) = std::mem::transmute(function.symbol_ptr);
                    func(arg);
                }
                Ok(Value::None)
            }

            // Pointer argument, pointer return (e.g., GetModuleHandleA)
            (FFIType::Pointer | FFIType::ConstPointer, &[FFIType::Pointer | FFIType::ConstPointer]) => {
                let arg_ptr = self.value_to_pointer(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(*const c_void) -> *const c_void =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(arg_ptr);
                    Ok(Value::Int(result as usize as i64))
                }
            }

            // ShowWindow: (pointer, int) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Int | FFIType::Int32]) => {
                let hwnd = self.value_to_pointer(&args[0])?;
                let cmd = self.value_to_int(&args[1])?;
                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(hwnd, cmd);
                    Ok(Value::Int(result as i64))
                }
            }

            // UpdateWindow, DestroyWindow: (pointer) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer]) => {
                let hwnd = self.value_to_pointer(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(*const c_void) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(hwnd);
                    Ok(Value::Int(result as i64))
                }
            }

            // String argument, void return
            (FFIType::Void, &[FFIType::String]) => {
                let s = self.value_to_string(&args[0])?;
                let c_string = CString::new(s)?;
                unsafe {
                    let func: unsafe extern "C" fn(*const c_char) =
                        std::mem::transmute(function.symbol_ptr);
                    func(c_string.as_ptr());
                }
                Ok(Value::None)
            }

            // String argument, string return
            (FFIType::String, &[FFIType::String]) => {
                let s = self.value_to_string(&args[0])?;
                let c_string = CString::new(s)?;
                unsafe {
                    let func: unsafe extern "C" fn(*const c_char) -> *const c_char =
                        std::mem::transmute(function.symbol_ptr);
                    let result_ptr = func(c_string.as_ptr());
                    if result_ptr.is_null() {
                        Ok(Value::None)
                    } else {
                        let result = CStr::from_ptr(result_ptr).to_string_lossy().into_owned();
                        Ok(Value::Str(result))
                    }
                }
            }

            // Double argument, double return
            (FFIType::Double, &[FFIType::Double]) => {
                let arg = self.value_to_float(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_double) -> c_double =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(arg);
                    Ok(Value::Float(result))
                }
            }

            // Two double arguments, double return
            (FFIType::Double, &[FFIType::Double, FFIType::Double]) => {
                let arg1 = self.value_to_float(&args[0])?;
                let arg2 = self.value_to_float(&args[1])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_double, c_double) -> c_double =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(arg1, arg2);
                    Ok(Value::Float(result))
                }
            }

            // Two float arguments, void return
            (FFIType::Void, &[FFIType::Float, FFIType::Float]) => {
                let arg1 = self.value_to_float(&args[0])? as f32;
                let arg2 = self.value_to_float(&args[1])? as f32;
                unsafe {
                    let func: unsafe extern "C" fn(c_float, c_float) = std::mem::transmute(function.symbol_ptr);
                    func(arg1, arg2);
                }
                Ok(Value::None)
            }

            // Three float arguments, void return
            (FFIType::Void, &[FFIType::Float, FFIType::Float, FFIType::Float]) => {
                let arg1 = self.value_to_float(&args[0])? as f32;
                let arg2 = self.value_to_float(&args[1])? as f32;
                let arg3 = self.value_to_float(&args[2])? as f32;
                unsafe {
                    let func: unsafe extern "C" fn(c_float, c_float, c_float) = std::mem::transmute(function.symbol_ptr);
                    func(arg1, arg2, arg3);
                }
                Ok(Value::None)
            }

            // Four float arguments, void return (e.g., glClearColor, glColor4f)
            (FFIType::Void, &[FFIType::Float, FFIType::Float, FFIType::Float, FFIType::Float]) => {
                let arg1 = self.value_to_float(&args[0])? as f32;
                let arg2 = self.value_to_float(&args[1])? as f32;
                let arg3 = self.value_to_float(&args[2])? as f32;
                let arg4 = self.value_to_float(&args[3])? as f32;
                unsafe {
                    let func: unsafe extern "C" fn(c_float, c_float, c_float, c_float) = std::mem::transmute(function.symbol_ptr);
                    func(arg1, arg2, arg3, arg4);
                }
                Ok(Value::None)
            }

            // Four int arguments, void return (e.g., glViewport)
            (FFIType::Void, &[FFIType::Int32, FFIType::Int32, FFIType::Int32, FFIType::Int32]) => {
                let arg1 = self.value_to_int(&args[0])?;
                let arg2 = self.value_to_int(&args[1])?;
                let arg3 = self.value_to_int(&args[2])?;
                let arg4 = self.value_to_int(&args[3])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_int, c_int, c_int, c_int) = std::mem::transmute(function.symbol_ptr);
                    func(arg1, arg2, arg3, arg4);
                }
                Ok(Value::None)
            }

            // Four double arguments, void return (e.g., gluPerspective)
            (FFIType::Void, &[FFIType::Double, FFIType::Double, FFIType::Double, FFIType::Double]) => {
                let arg1 = self.value_to_float(&args[0])?;
                let arg2 = self.value_to_float(&args[1])?;
                let arg3 = self.value_to_float(&args[2])?;
                let arg4 = self.value_to_float(&args[3])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_double, c_double, c_double, c_double) = std::mem::transmute(function.symbol_ptr);
                    func(arg1, arg2, arg3, arg4);
                }
                Ok(Value::None)
            }

            // MessageBoxA: (pointer, pointer, pointer, int) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Pointer | FFIType::ConstPointer | FFIType::String, FFIType::Pointer | FFIType::ConstPointer | FFIType::String, FFIType::Int | FFIType::Int32]) => {
                let hwnd = self.value_to_pointer(&args[0])?;
                let text = self.value_to_string(&args[1])?;
                let title = self.value_to_string(&args[2])?;
                let style = self.value_to_int(&args[3])?;

                let text_cstring = CString::new(text)?;
                let title_cstring = CString::new(title)?;

                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, *const c_char, *const c_char, c_int) -> c_int = std::mem::transmute(function.symbol_ptr);
                    let result = func(hwnd, text_cstring.as_ptr(), title_cstring.as_ptr(), style);
                    Ok(Value::Int(result as i64))
                }
            }

            // CreateWindowExA: (int, pointer, pointer, int, int, int, int, int, pointer, pointer, pointer, pointer) -> pointer
            (FFIType::Pointer | FFIType::ConstPointer, params) if params.len() == 12 => {
                // Extract all 12 parameters
                let ex_style = self.value_to_int(&args[0])?;

                let class_name = self.value_to_string(&args[1])?;
                let class_name_cstr = CString::new(class_name)?;

                let window_name = self.value_to_string(&args[2])?;
                let window_name_cstr = CString::new(window_name)?;

                let style = self.value_to_int(&args[3])?;
                let x = self.value_to_int(&args[4])?;
                let y = self.value_to_int(&args[5])?;
                let width = self.value_to_int(&args[6])?;
                let height = self.value_to_int(&args[7])?;
                let parent = self.value_to_pointer(&args[8])?;
                let menu = self.value_to_pointer(&args[9])?;
                let instance = self.value_to_pointer(&args[10])?;
                let param = self.value_to_pointer(&args[11])?;

                unsafe {
                    let func: unsafe extern "C" fn(
                        c_int, *const c_char, *const c_char, c_int,
                        c_int, c_int, c_int, c_int,
                        *const c_void, *const c_void, *const c_void, *const c_void
                    ) -> *const c_void = std::mem::transmute(function.symbol_ptr);

                    let result = func(
                        ex_style,
                        class_name_cstr.as_ptr(),
                        window_name_cstr.as_ptr(),
                        style,
                        x, y, width, height,
                        parent, menu, instance, param
                    );

                    Ok(Value::Int(result as usize as i64))
                }
            }

            // GetSystemMetrics, MessageBeep: (int) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Int | FFIType::Int32]) => {
                let arg = self.value_to_int(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(arg);
                    Ok(Value::Int(result as i64))
                }
            }

            // SetWindowTextA: (pointer, pointer) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Pointer | FFIType::ConstPointer | FFIType::String]) => {
                let hwnd = self.value_to_pointer(&args[0])?;
                let text = self.value_to_string(&args[1])?;
                let text_cstring = CString::new(text)?;

                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, *const c_char) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(hwnd, text_cstring.as_ptr());
                    Ok(Value::Int(result as i64))
                }
            }

            // MoveWindow: (pointer, int, int, int, int, int) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32]) => {
                let hwnd = self.value_to_pointer(&args[0])?;
                let x = self.value_to_int(&args[1])?;
                let y = self.value_to_int(&args[2])?;
                let width = self.value_to_int(&args[3])?;
                let height = self.value_to_int(&args[4])?;
                let repaint = self.value_to_int(&args[5])?;

                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, c_int, c_int, c_int, c_int, c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(hwnd, x, y, width, height, repaint);
                    Ok(Value::Int(result as i64))
                }
            }

            // GetMessageA: (pointer, pointer, int, int) -> int
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Pointer | FFIType::ConstPointer, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32]) => {
                let msg_ptr = self.value_to_pointer(&args[0])?;
                let hwnd = self.value_to_pointer(&args[1])?;
                let msg_filter_min = self.value_to_int(&args[2])?;
                let msg_filter_max = self.value_to_int(&args[3])?;

                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, *const c_void, c_int, c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(msg_ptr, hwnd, msg_filter_min, msg_filter_max);
                    Ok(Value::Int(result as i64))
                }
            }

            // PeekMessageA: (pointer, pointer, int, int, int) -> int (has 5 params!)
            (FFIType::Int | FFIType::Int32, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Pointer | FFIType::ConstPointer, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32, FFIType::Int | FFIType::Int32]) => {
                let msg_ptr = self.value_to_pointer(&args[0])?;
                let hwnd = self.value_to_pointer(&args[1])?;
                let msg_filter_min = self.value_to_int(&args[2])?;
                let msg_filter_max = self.value_to_int(&args[3])?;
                let remove_msg = self.value_to_int(&args[4])?;

                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, *const c_void, c_int, c_int, c_int) -> c_int =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(msg_ptr, hwnd, msg_filter_min, msg_filter_max, remove_msg);
                    Ok(Value::Int(result as i64))
                }
            }

            // TranslateMessage, DispatchMessageA: (pointer) -> int
            (FFIType::Int | FFIType::Int32 | FFIType::Long, &[FFIType::Pointer | FFIType::ConstPointer]) if sig.return_type != FFIType::Pointer && sig.return_type != FFIType::ConstPointer => {
                let msg_ptr = self.value_to_pointer(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(*const c_void) -> c_long =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(msg_ptr);
                    Ok(Value::Int(result as i64))
                }
            }

            // PostQuitMessage: (int) -> void
            (FFIType::Void, &[FFIType::Int | FFIType::Int32]) => {
                let exit_code = self.value_to_int(&args[0])?;
                unsafe {
                    let func: unsafe extern "C" fn(c_int) = std::mem::transmute(function.symbol_ptr);
                    func(exit_code);
                }
                Ok(Value::None)
            }

            // DefWindowProcA: (pointer, int, int, int) -> long
            (FFIType::Long | FFIType::Int | FFIType::Int32 | FFIType::Int64, &[FFIType::Pointer | FFIType::ConstPointer, FFIType::Int | FFIType::Int32 | FFIType::UInt | FFIType::UInt32, FFIType::Int | FFIType::Int32 | FFIType::Int64 | FFIType::UInt64, FFIType::Int | FFIType::Int32 | FFIType::Int64]) => {
                let hwnd = self.value_to_pointer(&args[0])?;
                let msg = self.value_to_int(&args[1])?;
                let wparam = self.value_to_int(&args[2])?;
                let lparam = self.value_to_int(&args[3])?;

                unsafe {
                    let func: unsafe extern "C" fn(*const c_void, c_int, isize, isize) -> isize =
                        std::mem::transmute(function.symbol_ptr);
                    let result = func(hwnd, msg, wparam as isize, lparam as isize);
                    Ok(Value::Int(result as i64))
                }
            }

            _ => {
                Err(anyhow!(
                    "Unsupported function signature: {:?} with {} parameters",
                    sig.return_type,
                    sig.param_types.len()
                ))
            }
        }
    }

    /// Call function using libffi
    #[cfg(feature = "ffi")]
    fn call_function_with_libffi(
        &self,
        cif: &Cif,
        function: &FFIExternalFunction,
        args: Vec<Value>,
    ) -> Result<Value> {
        // Convert arguments to libffi compatible format
        let mut ffi_args: Vec<Arg> = Vec::with_capacity(args.len());
        for (i, arg) in args.iter().enumerate() {
            let param_type = &function.signature.param_types[i];
            let ffi_arg = self.value_to_ffi_arg(arg, param_type)?;
            ffi_args.push(ffi_arg);
        }

        // Call the function using libffi
        let code_ptr = CodePtr(function.symbol_ptr);
        let result = unsafe {
            match &function.signature.return_type {
                FFIType::Void => {
                    cif.call::<()>(code_ptr, &ffi_args);
                    Value::None
                },
                FFIType::Int | FFIType::Int32 => {
                    let result: i32 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::Int8 => {
                    let result: i8 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::Int16 => {
                    let result: i16 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::Int64 => {
                    let result: i64 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result)
                },
                FFIType::UInt | FFIType::UInt32 => {
                    let result: u32 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::UInt8 => {
                    let result: u8 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::UInt16 => {
                    let result: u16 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::UInt64 => {
                    let result: u64 = cif.call(code_ptr, &ffi_args);
                    Value::Int(result as i64)
                },
                FFIType::Float => {
                    let result: f32 = cif.call(code_ptr, &ffi_args);
                    Value::Float(result as f64)
                },
                FFIType::Double => {
                    let result: f64 = cif.call(code_ptr, &ffi_args);
                    Value::Float(result)
                },
                FFIType::Bool => {
                    let result: u8 = cif.call(code_ptr, &ffi_args);
                    Value::Bool(result != 0)
                },
                FFIType::String => {
                    let result: *const c_char = cif.call(code_ptr, &ffi_args);
                    if result.is_null() {
                        Value::None
                    } else {
                        let c_str = unsafe { CStr::from_ptr(result) };
                        let string = c_str.to_string_lossy().into_owned();
                        Value::Str(string)
                    }
                },
                FFIType::Pointer | FFIType::ConstPointer => {
                    let result: *mut c_void = cif.call(code_ptr, &ffi_args);
                    // For now, we'll just return None for pointer results
                    // In a more complete implementation, we might want to wrap pointers in a special type
                    Value::None
                },
                _ => {
                    return Err(anyhow!("Unsupported return type for libffi: {:?}", function.signature.return_type));
                }
            }
        };

        Ok(result)
    }

    /// Convert Tauraro Value to libffi Arg
    #[cfg(feature = "ffi")]
    fn value_to_ffi_arg(&self, value: &Value, ffi_type: &FFIType) -> Result<Arg> {
        match (value, ffi_type) {
            (Value::Int(i), FFIType::Int | FFIType::Int32) => Ok(Arg::new(&(*i as i32))),
            (Value::Int(i), FFIType::Int8) => Ok(Arg::new(&(*i as i8))),
            (Value::Int(i), FFIType::Int16) => Ok(Arg::new(&(*i as i16))),
            (Value::Int(i), FFIType::Int64) => Ok(Arg::new(&(*i))),
            (Value::Int(i), FFIType::UInt | FFIType::UInt32) => Ok(Arg::new(&(*i as u32))),
            (Value::Int(i), FFIType::UInt8) => Ok(Arg::new(&(*i as u8))),
            (Value::Int(i), FFIType::UInt16) => Ok(Arg::new(&(*i as u16))),
            (Value::Int(i), FFIType::UInt64) => Ok(Arg::new(&(*i as u64))),
            (Value::Int(i), FFIType::Bool) => Ok(Arg::new(&(if *i != 0 { 1u8 } else { 0u8 }))),
            (Value::Float(f), FFIType::Float) => Ok(Arg::new(&(*f as f32))),
            (Value::Float(f), FFIType::Double) => Ok(Arg::new(&(*f))),
            (Value::Bool(b), FFIType::Bool) => Ok(Arg::new(&(if *b { 1u8 } else { 0u8 }))),
            (Value::Str(s), FFIType::String) => {
                let c_string = CString::new(s.clone())?;
                // Note: This is a simplified approach. In a production system, you'd need
                // to ensure the CString lives long enough for the function call.
                Ok(Arg::new(&(c_string.as_ptr())))
            },
            (Value::None, FFIType::Pointer | FFIType::ConstPointer) => {
                let null_ptr: *const c_void = std::ptr::null();
                Ok(Arg::new(&null_ptr))
            },
            _ => Err(anyhow!("Cannot convert {:?} to FFI type {:?}", value, ffi_type)),
        }
    }

    /// Convert Tauraro Value to C int
    fn value_to_int(&self, value: &Value) -> Result<c_int> {
        match value {
            Value::Int(i) => Ok(*i as c_int),
            Value::Float(f) => Ok(*f as c_int),
            Value::Bool(b) => Ok(if *b { 1 } else { 0 }),
            _ => Err(anyhow!("Cannot convert {:?} to int", value)),
        }
    }

    /// Convert Tauraro Value to C double
    fn value_to_float(&self, value: &Value) -> Result<c_double> {
        match value {
            Value::Float(f) => Ok(*f),
            Value::Int(i) => Ok(*i as c_double),
            _ => Err(anyhow!("Cannot convert {:?} to float", value)),
        }
    }

    /// Convert Tauraro Value to String
    fn value_to_string(&self, value: &Value) -> Result<String> {
        match value {
            Value::Str(s) => Ok(s.clone()),
            Value::Int(i) => Ok(i.to_string()),
            Value::Float(f) => Ok(f.to_string()),
            Value::Bool(b) => Ok(b.to_string()),
            _ => Err(anyhow!("Cannot convert {:?} to string", value)),
        }
    }

    /// Convert Tauraro Value to pointer (for NULL or integer pointers)
    fn value_to_pointer(&self, value: &Value) -> Result<*const c_void> {
        match value {
            Value::None => Ok(std::ptr::null()),
            Value::Int(i) => Ok(*i as usize as *const c_void),
            _ => Err(anyhow!("Cannot convert {:?} to pointer", value)),
        }
    }

    /// Get information about a loaded library
    pub fn get_library_info(&self, library_name: &str) -> Option<(String, PathBuf, usize)> {
        self.libraries.get(library_name).map(|lib| {
            let l = lib.lock().unwrap();
            (l.name.clone(), l.path.clone(), l.functions.len())
        })
    }

    /// List all loaded libraries
    pub fn list_libraries(&self) -> Vec<String> {
        self.libraries.keys().cloned().collect()
    }

    /// Unload a library
    pub fn unload_library(&mut self, library_name: &str) -> Result<()> {
        self.libraries.remove(library_name)
            .ok_or_else(|| anyhow!("Library not loaded: {}", library_name))?;
        Ok(())
    }
}

impl Default for FFIManager {
    fn default() -> Self {
        Self::new()
    }
}

/// Helper function to create FFI manager and make it available globally
pub fn create_ffi_manager() -> FFIManager {
    FFIManager::new()
}

/// Parse FFI type from string representation
pub fn parse_ffi_type(type_str: &str) -> Result<FFIType> {
    match type_str.to_lowercase().as_str() {
        "int" | "int32" | "i32" => Ok(FFIType::Int32),
        "int8" | "i8" => Ok(FFIType::Int8),
        "int16" | "i16" => Ok(FFIType::Int16),
        "int64" | "i64" => Ok(FFIType::Int64),
        "uint" | "uint32" | "u32" => Ok(FFIType::UInt32),
        "uint8" | "u8" => Ok(FFIType::UInt8),
        "uint16" | "u16" => Ok(FFIType::UInt16),
        "uint64" | "u64" => Ok(FFIType::UInt64),
        "float" | "f32" => Ok(FFIType::Float),
        "double" | "f64" => Ok(FFIType::Double),
        "char" => Ok(FFIType::Char),
        "string" | "str" => Ok(FFIType::String),
        "pointer" | "ptr" => Ok(FFIType::Pointer),
        "void" => Ok(FFIType::Void),
        "bool" | "boolean" => Ok(FFIType::Bool),
        "size" | "size_t" | "usize" => Ok(FFIType::SizeT),
        "ssize" | "ssize_t" | "isize" => Ok(FFIType::SSizeT),
        "long" => Ok(FFIType::Long),
        "ulong" => Ok(FFIType::ULong),
        _ => Err(anyhow!("Unknown FFI type: {}", type_str)),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_ffi_manager_creation() {
        let manager = FFIManager::new();
        assert_eq!(manager.list_libraries().len(), 0);
    }

    #[test]
    fn test_parse_ffi_type() {
        assert_eq!(parse_ffi_type("int32").unwrap(), FFIType::Int32);
        assert_eq!(parse_ffi_type("double").unwrap(), FFIType::Double);
        assert_eq!(parse_ffi_type("string").unwrap(), FFIType::String);
        assert_eq!(parse_ffi_type("void").unwrap(), FFIType::Void);
    }
}