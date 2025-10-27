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

/// Compile C code to executable using available compilers
pub fn compile_to_executable(c_code: &str, output_path: &str, opt_level: u8) -> Result<()> {
    // Write C code to temporary file
    let temp_file = format!("{}.c", output_path);
    std::fs::write(&temp_file, c_code)?;

    // Check for builtin module dependencies and compile Rust FFI modules to object files
    let mut builtin_files = Vec::new();
    if c_code.contains("tauraro_math_") {
        // Compile Rust FFI module to object file
        match compile_rust_ffi_to_object("math", "build/builtin") {
            Ok(obj_file) => builtin_files.push(obj_file),
            Err(e) => {
                eprintln!("Warning: Failed to compile math FFI module: {}", e);
                // Fall back to C implementation if it exists
                let math_c = "build/builtin/tauraro_math.c";
                if std::path::Path::new(math_c).exists() {
                    builtin_files.push(math_c.to_string());
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
                    // Clean up temporary file
                    let _ = std::fs::remove_file(temp_file);
                    println!("Successfully compiled with {} {}", compiler, opt_flag);
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

    // Clean up temporary file
    let _ = std::fs::remove_file(temp_file);
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
