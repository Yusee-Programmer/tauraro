// build.rs - Build script for Tauraro C Transpiler with external library support

use std::env;
use std::path::{Path, PathBuf};
use std::process::Command;

fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();
    let out_path = Path::new(&out_dir);
    let project_root = env::var("CARGO_MANIFEST_DIR").unwrap();
    let project_path = Path::new(&project_root);
    let build_dir = project_path.join("build");
    
    println!("cargo:rerun-if-changed=src/codegen/c_transpiler/builtin_modules/");
    println!("cargo:rerun-if-changed=build/CMakeLists.txt");
    
    // Detect platform
    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap_or_default();
    
    println!("cargo:warning=Building Tauraro C Transpiler Builtins");
    println!("cargo:warning=Platform: {}", target_os);
    
    // Step 1: Run CMake to detect and configure libraries
    println!("cargo:warning=Running CMake configuration...");
    
    let mut cmake_cmd = if target_os == "windows" {
        let mut cmd = Command::new("cmake");
        cmd.current_dir(&build_dir);
        cmd.arg("-G").arg("Visual Studio 17 2022");
        cmd.arg("-DCMAKE_BUILD_TYPE=Release");
        cmd
    } else {
        let mut cmd = Command::new("cmake");
        cmd.current_dir(&build_dir);
        cmd.arg("-DCMAKE_BUILD_TYPE=Release");
        cmd.arg("-DCMAKE_C_COMPILER=gcc");
        cmd
    };
    
    cmake_cmd.arg("..");
    
    let cmake_output = cmake_cmd.output();
    match cmake_output {
        Ok(output) => {
            if !output.status.success() {
                println!("cargo:warning=CMake output: {}", String::from_utf8_lossy(&output.stdout));
                println!("cargo:warning=CMake errors: {}", String::from_utf8_lossy(&output.stderr));
                
                // Fallback if CMake is not available
                println!("cargo:warning=CMake not found or failed - falling back to manual configuration");
                configure_libraries_fallback(&project_path, &out_path);
            } else {
                println!("cargo:warning=CMake configuration succeeded");
            }
        }
        Err(_) => {
            println!("cargo:warning=CMake not found - using fallback configuration");
            configure_libraries_fallback(&project_path, &out_path);
        }
    }
    
    // Step 2: Configure library linking
    link_external_libraries(&target_os, &out_path);
}

/// Fallback library configuration when CMake is not available
fn configure_libraries_fallback(project_path: &Path, _out_path: &Path) {
    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap_or_default();
    
    println!("cargo:warning=Configuring libraries for: {}", target_os);
    
    // Detect and add system library paths
    match target_os.as_str() {
        "windows" => {
            // Windows: Check for OpenSSL, SQLite, CURL in common locations
            let common_paths = vec![
                "C:\\Program Files\\OpenSSL-Win64",
                "C:\\Program Files (x86)\\OpenSSL-Win32",
                "C:\\OpenSSL",
                "C:\\Program Files\\SQLite",
                "C:\\Program Files\\curl",
            ];
            
            for path in common_paths {
                let lib_path = Path::new(path).join("lib");
                let include_path = Path::new(path).join("include");
                
                if lib_path.exists() {
                    println!("cargo:rustc-link-search=native={}", lib_path.display());
                }
                if include_path.exists() {
                    println!("cargo:rustc-link-search=native={}", include_path.display());
                }
            }
            
            // Link Windows libraries
            println!("cargo:rustc-link-lib=Crypt32");
            println!("cargo:rustc-link-lib=Advapi32");
        }
        "macos" => {
            // macOS: Check Homebrew paths
            let homebrew_paths = vec![
                "/usr/local/opt/openssl",
                "/usr/local/opt/curl",
                "/usr/local/opt/sqlite",
                "/opt/homebrew/opt/openssl",  // Apple Silicon
                "/opt/homebrew/opt/curl",
                "/opt/homebrew/opt/sqlite",
            ];
            
            for path in homebrew_paths {
                let lib_path = Path::new(path).join("lib");
                let include_path = Path::new(path).join("include");
                
                if lib_path.exists() {
                    println!("cargo:rustc-link-search=native={}", lib_path.display());
                }
                if include_path.exists() {
                    println!("cargo:rustc-link-search=native={}", include_path.display());
                }
            }
            
            // Link macOS frameworks
            println!("cargo:rustc-link-lib=framework=CoreFoundation");
            println!("cargo:rustc-link-lib=framework=Cocoa");
            println!("cargo:rustc-link-lib=framework=Security");
        }
        "linux" => {
            // Linux: Check standard library paths
            let lib_paths = vec![
                "/usr/lib/x86_64-linux-gnu",
                "/usr/lib64",
                "/usr/lib",
                "/lib/x86_64-linux-gnu",
            ];
            
            for path in lib_paths {
                if Path::new(path).exists() {
                    println!("cargo:rustc-link-search=native={}", path);
                }
            }
        }
        _ => {}
    }
}

/// Link external C libraries
fn link_external_libraries(target_os: &str, _out_path: &Path) {
    match target_os {
        "windows" => {
            // Try to link to OpenSSL
            if cfg!(feature = "openssl") || env::var("CARGO_CFG_TARGET_OS").is_ok() {
                println!("cargo:rustc-link-lib=libssl");
                println!("cargo:rustc-link-lib=libcrypto");
            }
            
            // SQLite
            println!("cargo:rustc-link-lib=sqlite3");
            
            // CURL
            println!("cargo:rustc-link-lib=libcurl");
            
            // ZLIB
            println!("cargo:rustc-link-lib=zlib");
            
            println!("cargo:warning=Windows libraries configured (OpenSSL, SQLite, CURL, ZLIB)");
        }
        "macos" => {
            // macOS typically has these pre-installed
            println!("cargo:rustc-link-lib=ssl");
            println!("cargo:rustc-link-lib=crypto");
            println!("cargo:rustc-link-lib=sqlite3");
            println!("cargo:rustc-link-lib=curl");
            println!("cargo:rustc-link-lib=z");
            
            println!("cargo:warning=macOS libraries configured (using system/Homebrew)");
        }
        "linux" => {
            // Linux has these in system locations
            println!("cargo:rustc-link-lib=ssl");
            println!("cargo:rustc-link-lib=crypto");
            println!("cargo:rustc-link-lib=sqlite3");
            println!("cargo:rustc-link-lib=curl");
            println!("cargo:rustc-link-lib=z");
            
            // Regex libraries
            println!("cargo:rustc-link-lib=pcre2-8");
            
            println!("cargo:warning=Linux libraries configured");
        }
        _ => {}
    }
    
    // Try to link the compiled static library
    println!("cargo:rustc-link-search=native={}/build", 
             env::var("CARGO_MANIFEST_DIR").unwrap());
}

/// Print configuration summary
#[allow(dead_code)]
fn print_config_summary() {
    println!("\n");
    println!("╔═══════════════════════════════════════════════════════════════╗");
    println!("║      Tauraro C Transpiler - External Libraries Config        ║");
    println!("╚═══════════════════════════════════════════════════════════════╝");
    println!("");
    println!("Critical Libraries:");
    println!("  ✓ OpenSSL    - TLS/Cryptography");
    println!("  ✓ SQLite3    - Database operations");
    println!("  ✓ libcurl    - HTTP/FTP/SFTP");
    println!("  ✓ ZLIB       - Compression");
    println!("");
    println!("Optional Libraries:");
    println!("  ? PCRE/PCRE2 - Regular expressions");
    println!("  ? libffi     - Foreign function interface");
    println!("  ? libuv      - Async I/O");
    println!("");
    println!("Platform-Specific:");
    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap_or_default();
    match target_os.as_str() {
        "windows" => println!("  ◆ Windows Crypto API, WebView2"),
        "macos" => println!("  ◆ macOS Cocoa, CoreFoundation, Security"),
        "linux" => println!("  ◆ Linux GTK+3, system threading"),
        _ => println!("  ? Unknown platform"),
    }
    println!("");
    println!("To complete setup:");
    println!("  1. Install CMake (cmake.org)");
    println!("  2. Run: cmake --build build --config Release");
    println!("  3. cargo build --release");
    println!("");
}
