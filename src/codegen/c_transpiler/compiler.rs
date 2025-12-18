//! C Compiler Integration Module
//!
//! This module handles detecting and invoking C compilers (GCC, Clang, MSVC)
//! to compile generated C code into executables.

use anyhow::Result;
use std::process::Command;

/// Detect available C compilers on the system
pub fn detect_compilers() -> Vec<String> {
    let mut compilers = Vec::new();

    // Check for GCC
    if Command::new("gcc").arg("--version").output().is_ok() {
        compilers.push("gcc".to_string());
    }

    // Check for Clang
    if Command::new("clang").arg("--version").output().is_ok() {
        compilers.push("clang".to_string());
    }

    // On Windows, check for Clang-CL
    if cfg!(windows) {
        if Command::new("clang-cl").arg("--version").output().is_ok() {
            compilers.push("clang-cl".to_string());
        }
    }

    // Check for MSVC (cl.exe)
    if cfg!(windows) {
        if Command::new("cl").arg("/?").output().is_ok() {
            compilers.push("cl".to_string());
        }
    }

    compilers
}

/// Detect if FFI (Foreign Function Interface) is used in the C code
fn detect_ffi_usage(c_code: &str) -> bool {
    // Check for FFI function calls
    c_code.contains("load_library(") ||
    c_code.contains("define_function(") ||
    c_code.contains("call_function(") ||
    c_code.contains("ffi_library_t") ||
    c_code.contains("ffi_function_t")
}

/// Compile Rust FFI module to object file
fn compile_rust_ffi_to_object(module_name: &str, output_dir: &str) -> Result<String> {
    let rust_source = format!("src/builtins_ffi/{}_ffi.rs", module_name);
    let object_file = format!("{}/{}_ffi.o", output_dir, module_name);

    // Create output directory if it doesn't exist
    std::fs::create_dir_all(output_dir)?;

    // Compile Rust to object file using rustc
    // Use panic=abort for #![no_std] compatibility
    let output = Command::new("rustc")
        .args(&[
            "--crate-type", "staticlib",
            "--emit", "obj",
            "-C", "panic=abort",
            "-O",
            &rust_source,
            "-o", &object_file,
        ])
        .output();

    match output {
        Ok(result) => {
            if result.status.success() {
                println!("Compiled Rust FFI module '{}' to object file: {}", module_name, object_file);
                Ok(object_file)
            } else {
                let stderr = String::from_utf8_lossy(&result.stderr);
                Err(anyhow::anyhow!("Failed to compile Rust FFI module: {}", stderr))
            }
        }
        Err(e) => Err(anyhow::anyhow!("Failed to run rustc: {}", e)),
    }
}

/// Compile C code to executable with additional object files
pub fn compile_to_executable_with_objects(c_code: &str, output_path: &str, opt_level: u8, object_files: &[std::path::PathBuf]) -> Result<()> {
    // Write C code to temporary file (keep it for inspection)
    let temp_file = format!("{}.c", output_path);
    std::fs::write(&temp_file, c_code)?;
    println!("C source code written to: {}", temp_file);

    // Detect FFI usage for dynamic linking
    let uses_ffi = detect_ffi_usage(c_code);
    if uses_ffi {
        println!("FFI usage detected - dynamic library linking will be enabled");
    }

    // Detect available compilers
    let compilers = detect_compilers();
    if compilers.is_empty() {
        return Err(anyhow::anyhow!("No C compiler found. Please install GCC, Clang, or MSVC."));
    }

    // Determine optimization flags
    let opt_flag = match opt_level {
        0 => "-O0",
        1 => "-O1",
        2 => "-O2",
        3 => "-O3",
        _ => "-O2",
    };

    // Try each compiler in order
    let mut last_error = String::new();
    for compiler in &compilers {
        let output = match compiler.as_str() {
            "gcc" | "clang" => {
                let mut args = vec![temp_file.as_str()];
                // Add additional object files
                args.extend(object_files.iter().map(|p| p.to_str().unwrap_or("")));
                // Add include path for FFI headers and allow multiple definitions (for Rust panic handler)
                args.extend(&["-I.", "-Wl,--allow-multiple-definition", "-o", output_path, opt_flag, "-lm"]);

                // Add dynamic library linking if FFI is used
                if uses_ffi {
                    if !cfg!(target_os = "windows") {
                        args.push("-ldl");
                    }
                }

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "clang-cl" => {
                let opt_flag_clang = format!("-O{}", opt_level);

                let mut args = vec![temp_file.as_str()];
                // Add additional object files
                args.extend(object_files.iter().map(|p| p.to_str().unwrap_or("")));
                // Add include path for FFI headers
                args.push("-I.");
                args.push("-o");
                args.push(output_path);
                args.push(&opt_flag_clang);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "cl" => {
                // MSVC compilation
                let fe_flag = format!("/Fe:{}", output_path);
                let opt_flag_msvc = format!("/O{}", opt_level);

                let mut args = vec![temp_file.as_str()];
                // Add additional object files
                args.extend(object_files.iter().map(|p| p.to_str().unwrap_or("")));
                // Add include path for FFI headers
                args.push("/I.");
                args.push(&fe_flag);
                args.push(&opt_flag_msvc);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            _ => {
                // Fallback to basic compilation
                let mut args = vec![temp_file.as_str()];
                // Add additional object files
                args.extend(object_files.iter().map(|p| p.to_str().unwrap_or("")));
                args.extend(&["-o", output_path]);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
        };

        match output {
            Ok(output) => {
                if output.status.success() {
                    println!("Successfully compiled with {} {}", compiler, opt_flag);
                    println!("Executable created: {}", output_path);
                    return Ok(());
                } else {
                    let stderr = String::from_utf8_lossy(&output.stderr);
                    last_error = format!("{} compilation failed: {}", compiler, stderr);
                    eprintln!("{}", last_error);
                }
            }
            Err(e) => {
                last_error = format!("Failed to run {}: {}", compiler, e);
                eprintln!("{}", last_error);
            }
        }
    }

    Err(anyhow::anyhow!("Compilation failed with all available compilers. Last error: {}", last_error))
}

/// Compile C code to executable using available compilers
pub fn compile_to_executable(c_code: &str, output_path: &str, opt_level: u8) -> Result<()> {
    // Write C code to temporary file (keep it for inspection)
    let temp_file = format!("{}.c", output_path);
    std::fs::write(&temp_file, c_code)?;
    println!("C source code written to: {}", temp_file);

    // Detect FFI usage for dynamic linking
    let uses_ffi = detect_ffi_usage(c_code);
    if uses_ffi {
        println!("FFI usage detected - dynamic library linking will be enabled");
    }

    // Check for builtin module dependencies and compile Rust FFI modules to object files
    let mut builtin_files = Vec::new();

    // List of builtin modules to check
    let builtin_modules = [
        ("math", "tauraro_math_"),
        ("sys", "tauraro_sys_"),
        ("time", "tauraro_time_"),
        ("random", "tauraro_random_"),
        ("os", "tauraro_os_"),
        ("json", "tauraro_json_"),
        ("re", "tauraro_re_"),
        ("io", "tauraro_io_"),
        ("datetime", "tauraro_datetime_"),
        ("collections", "tauraro_collections_"),
        ("itertools", "tauraro_itertools_"),
        ("functools", "tauraro_functools_"),
        ("threading", "tauraro_threading_"),
        ("copy", "tauraro_copy_"),
        ("base64", "tauraro_base64_"),
        ("hashlib", "tauraro_hashlib_"),
        ("urllib", "tauraro_urllib_"),
        ("csv", "tauraro_csv_"),
        ("logging", "tauraro_logging_"),
        ("unittest", "tauraro_unittest_"),
        ("socket", "tauraro_socket_"),
        ("asyncio", "tauraro_asyncio_"),
        ("httptools", "tauraro_httptools_"),
        ("websockets", "tauraro_websockets_"),
        ("httpx", "tauraro_httpx_"),
        ("memory", "tauraro_memory_"),
        ("gc", "tauraro_gc_"),
        ("exceptions", "tauraro_exceptions_"),
        ("abc", "tauraro_abc_"),
        ("pickle", "tauraro_pickle_"),
        ("secrets", "tauraro_secrets_"),
        ("uuid", "tauraro_uuid_"),
        ("subprocess", "tauraro_subprocess_"),
    ];

    for (module_name, prefix) in &builtin_modules {
        if c_code.contains(prefix) {
            // Compile Rust FFI module to object file
            match compile_rust_ffi_to_object(module_name, "build/builtin") {
                Ok(obj_file) => builtin_files.push(obj_file),
                Err(e) => {
                    eprintln!("Warning: Failed to compile {} FFI module: {}", module_name, e);
                    // Fall back to C implementation if it exists
                    let fallback_c = format!("build/builtin/tauraro_{}.c", module_name);
                    if std::path::Path::new(&fallback_c).exists() {
                        builtin_files.push(fallback_c);
                    }
                }
            }
        }
    }

    // Detect available compilers
    let compilers = detect_compilers();
    if compilers.is_empty() {
        return Err(anyhow::anyhow!("No C compiler found. Please install GCC, Clang, or MSVC."));
    }

    // Determine optimization flags
    let opt_flag = match opt_level {
        0 => "-O0",
        1 => "-O1",
        2 => "-O2",
        3 => "-O3",
        _ => "-O2",
    };

    // Try each compiler in order
    let mut last_error = String::new();
    for compiler in &compilers {
        let output = match compiler.as_str() {
            "gcc" | "clang" => {
                let mut args = vec![temp_file.as_str()];
                args.extend(builtin_files.iter().map(|s| s.as_str()));
                args.extend(&["-o", output_path, opt_flag, "-lm"]);

                // Add dynamic library linking if FFI is used
                if uses_ffi {
                    if !cfg!(target_os = "windows") {
                        args.push("-ldl");
                    }
                }

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "clang-cl" => {
                Command::new(compiler)
                    .args(&[&temp_file, "-o", output_path, &format!("-O{}", opt_level)])
                    .output()
            }
            "cl" => {
                // MSVC compilation
                Command::new(compiler)
                    .args(&[&temp_file, &format!("/Fe:{}", output_path), &format!("/O{}", opt_level)])
                    .output()
            }
            _ => {
                // Fallback to basic compilation
                Command::new(compiler)
                    .args(&[&temp_file, "-o", output_path])
                    .output()
            }
        };

        match output {
            Ok(output) => {
                if output.status.success() {
                    // DON'T delete temporary file - keep it for inspection
                    // let _ = std::fs::remove_file(&temp_file);
                    println!("Successfully compiled with {} {}", compiler, opt_flag);
                    println!("Executable created: {}", output_path);
                    return Ok(());
                } else {
                    let stderr = String::from_utf8_lossy(&output.stderr);
                    last_error = format!("{} compilation failed: {}", compiler, stderr);
                    eprintln!("{}", last_error);
                }
            }
            Err(e) => {
                last_error = format!("Failed to run {}: {}", compiler, e);
                eprintln!("{}", last_error);
            }
        }
    }

    // DON'T delete temporary file even on failure - keep for debugging
    // let _ = std::fs::remove_file(&temp_file);
    Err(anyhow::anyhow!(
        "Compilation failed with all available compilers. Last error: {}",
        last_error
    ))
}

/// Get recommended compiler flags for different platforms
pub fn get_platform_flags() -> Vec<String> {
    let mut flags = Vec::new();

    if cfg!(target_os = "windows") {
        flags.push("-D_CRT_SECURE_NO_WARNINGS".to_string());
    }

    if cfg!(target_os = "macos") {
        flags.push("-Wno-deprecated-declarations".to_string());
    }

    flags
}

/// Check if a specific compiler is available
pub fn is_compiler_available(compiler_name: &str) -> bool {
    detect_compilers().contains(&compiler_name.to_string())
}

/// Compile C code to shared library with additional object files
pub fn compile_to_shared_library_with_objects(
    c_code: &str,
    output_path: &str,
    opt_level: u8,
    object_files: &[std::path::PathBuf],
    target: &str,
) -> Result<()> {
    // Write C code to temporary file (keep it for inspection)
    let temp_file = format!("{}.c", output_path.trim_end_matches(".dll").trim_end_matches(".so").trim_end_matches(".dylib"));
    std::fs::write(&temp_file, c_code)?;
    println!("C source code written to: {}", temp_file);

    // Detect FFI usage for dynamic linking
    let uses_ffi = detect_ffi_usage(c_code);
    if uses_ffi {
        println!("FFI usage detected - dynamic library linking will be enabled");
    }

    // Detect available compilers
    let compilers = detect_compilers();
    if compilers.is_empty() {
        return Err(anyhow::anyhow!("No C compiler found. Please install GCC, Clang, or MSVC."));
    }

    // Determine target platform
    let target_platform = if target != "native" {
        target
    } else {
        std::env::consts::OS
    };

    // Determine optimization flags
    let opt_flag = match opt_level {
        0 => "-O0",
        1 => "-O1",
        2 => "-O2",
        3 => "-O3",
        _ => "-O2",
    };

    // Try each compiler in order
    let mut last_error = String::new();
    for compiler in &compilers {
        let output = match compiler.as_str() {
            "gcc" | "clang" => {
                let mut args = vec!["-shared", "-fPIC", temp_file.as_str()];

                // Add object files
                for obj_file in object_files {
                    args.push(obj_file.to_str().unwrap());
                }

                args.extend(&["-o", output_path, opt_flag, "-lm"]);

                // Add dynamic library linking if FFI is used
                if uses_ffi {
                    if target_platform != "windows" {
                        args.push("-ldl");
                    }
                }

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "cl" => {
                // MSVC shared library compilation
                let fe_flag = format!("/Fe:{}", output_path);
                let opt_flag_msvc = format!("/O{}", opt_level);
                let mut args = vec!["/LD", temp_file.as_str()];

                // Add object files
                for obj_file in object_files {
                    args.push(obj_file.to_str().unwrap());
                }

                args.push(&fe_flag);
                args.push(&opt_flag_msvc);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "clang-cl" => {
                // Clang-CL shared library compilation
                let opt_flag_clang = format!("-O{}", opt_level);
                let mut args = vec!["-shared", temp_file.as_str()];

                // Add object files
                for obj_file in object_files {
                    args.push(obj_file.to_str().unwrap());
                }

                args.push("-o");
                args.push(output_path);
                args.push(&opt_flag_clang);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            _ => {
                // Fallback to basic shared library compilation
                let mut args = vec!["-shared", "-fPIC", temp_file.as_str()];

                // Add object files
                for obj_file in object_files {
                    args.push(obj_file.to_str().unwrap());
                }

                args.push("-o");
                args.push(output_path);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
        };

        match output {
            Ok(output) => {
                if output.status.success() {
                    println!("Successfully compiled shared library with {} {}", compiler, opt_flag);
                    println!("Shared library created: {}", output_path);
                    return Ok(());
                } else {
                    let stderr = String::from_utf8_lossy(&output.stderr);
                    last_error = format!("{} compilation failed: {}", compiler, stderr);
                    eprintln!("{}", last_error);
                }
            }
            Err(e) => {
                last_error = format!("Failed to run {}: {}", compiler, e);
                eprintln!("{}", last_error);
            }
        }
    }

    Err(anyhow::anyhow!("Compilation failed with all available compilers. Last error: {}", last_error))
}

/// Compile C code to shared library using available compilers
pub fn compile_to_shared_library(c_code: &str, output_path: &str, opt_level: u8, target: &str) -> Result<()> {
    // Write C code to temporary file (keep it for inspection)
    let temp_file = format!("{}.c", output_path.trim_end_matches(".dll").trim_end_matches(".so").trim_end_matches(".dylib"));
    std::fs::write(&temp_file, c_code)?;
    println!("C source code written to: {}", temp_file);

    // Detect FFI usage for dynamic linking
    let uses_ffi = detect_ffi_usage(c_code);
    if uses_ffi {
        println!("FFI usage detected - dynamic library linking will be enabled");
    }

    // Check for builtin module dependencies and compile Rust FFI modules to object files
    let mut builtin_files = Vec::new();

    // List of builtin modules to check
    let builtin_modules = [
        ("math", "tauraro_math_"),
        ("sys", "tauraro_sys_"),
        ("time", "tauraro_time_"),
        ("random", "tauraro_random_"),
        ("os", "tauraro_os_"),
        ("json", "tauraro_json_"),
        ("re", "tauraro_re_"),
        ("io", "tauraro_io_"),
        ("datetime", "tauraro_datetime_"),
        ("collections", "tauraro_collections_"),
        ("itertools", "tauraro_itertools_"),
        ("functools", "tauraro_functools_"),
        ("threading", "tauraro_threading_"),
        ("copy", "tauraro_copy_"),
        ("base64", "tauraro_base64_"),
        ("hashlib", "tauraro_hashlib_"),
        ("urllib", "tauraro_urllib_"),
        ("csv", "tauraro_csv_"),
        ("logging", "tauraro_logging_"),
        ("unittest", "tauraro_unittest_"),
        ("socket", "tauraro_socket_"),
        ("asyncio", "tauraro_asyncio_"),
        ("httptools", "tauraro_httptools_"),
        ("websockets", "tauraro_websockets_"),
        ("httpx", "tauraro_httpx_"),
        ("memory", "tauraro_memory_"),
        ("gc", "tauraro_gc_"),
        ("exceptions", "tauraro_exceptions_"),
        ("abc", "tauraro_abc_"),
        ("pickle", "tauraro_pickle_"),
        ("secrets", "tauraro_secrets_"),
        ("uuid", "tauraro_uuid_"),
        ("subprocess", "tauraro_subprocess_"),
    ];

    for (module_name, prefix) in &builtin_modules {
        if c_code.contains(prefix) {
            // Compile Rust FFI module to object file
            match compile_rust_ffi_to_object(module_name, "build/builtin") {
                Ok(obj_file) => builtin_files.push(obj_file),
                Err(e) => {
                    eprintln!("Warning: Failed to compile {} FFI module: {}", module_name, e);
                    // Fall back to C implementation if it exists
                    let fallback_c = format!("build/builtin/tauraro_{}.c", module_name);
                    if std::path::Path::new(&fallback_c).exists() {
                        builtin_files.push(fallback_c);
                    }
                }
            }
        }
    }

    // Detect available compilers
    let compilers = detect_compilers();
    if compilers.is_empty() {
        return Err(anyhow::anyhow!("No C compiler found. Please install GCC, Clang, or MSVC."));
    }

    // Determine target platform
    let target_platform = if target != "native" {
        target
    } else {
        std::env::consts::OS
    };

    // Determine optimization flags
    let opt_flag = match opt_level {
        0 => "-O0",
        1 => "-O1",
        2 => "-O2",
        3 => "-O3",
        _ => "-O2",
    };

    // Try each compiler in order
    let mut last_error = String::new();
    for compiler in &compilers {
        let output = match compiler.as_str() {
            "gcc" | "clang" => {
                let mut args = vec!["-shared", "-fPIC", temp_file.as_str()];
                args.extend(builtin_files.iter().map(|s| s.as_str()));
                args.extend(&["-o", output_path, opt_flag, "-lm"]);

                // Add dynamic library linking if FFI is used
                if uses_ffi {
                    if target_platform != "windows" {
                        args.push("-ldl");
                    }
                }

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "cl" => {
                // MSVC shared library compilation
                let fe_flag = format!("/Fe:{}", output_path);
                let opt_flag_msvc = format!("/O{}", opt_level);
                let mut args = vec!["/LD", temp_file.as_str()];
                args.extend(builtin_files.iter().map(|s| s.as_str()));
                args.push(&fe_flag);
                args.push(&opt_flag_msvc);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            "clang-cl" => {
                // Clang-CL shared library compilation
                let opt_flag_clang = format!("-O{}", opt_level);
                let mut args = vec!["-shared", temp_file.as_str()];
                args.extend(builtin_files.iter().map(|s| s.as_str()));
                args.push("-o");
                args.push(output_path);
                args.push(&opt_flag_clang);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
            _ => {
                // Fallback to basic shared library compilation
                let mut args = vec!["-shared", "-fPIC", temp_file.as_str()];
                args.extend(builtin_files.iter().map(|s| s.as_str()));
                args.push("-o");
                args.push(output_path);

                Command::new(compiler)
                    .args(&args)
                    .output()
            }
        };

        match output {
            Ok(output) => {
                if output.status.success() {
                    println!("Successfully compiled shared library with {} {}", compiler, opt_flag);
                    println!("Shared library created: {}", output_path);
                    return Ok(());
                } else {
                    let stderr = String::from_utf8_lossy(&output.stderr);
                    last_error = format!("{} compilation failed: {}", compiler, stderr);
                    eprintln!("{}", last_error);
                }
            }
            Err(e) => {
                last_error = format!("Failed to run {}: {}", compiler, e);
                eprintln!("{}", last_error);
            }
        }
    }

    Err(anyhow::anyhow!(
        "Compilation failed with all available compilers. Last error: {}",
        last_error
    ))
}
