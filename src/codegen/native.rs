use std::path::{Path, PathBuf};
use std::process::Command;
use std::env;

#[derive(Debug, Clone)]
pub enum TargetPlatform {
    Windows,
    Linux,
    MacOS,
    Android,
}

#[derive(Debug, Clone)]
pub enum OutputType {
    Executable,
    SharedLibrary,
    StaticLibrary,
    ObjectFile, // .o file
}

impl OutputType {
    pub fn from_target_string(target: &str) -> Self {
        match target {
            "so" | "dll" | "dylib" => OutputType::SharedLibrary,
            "a" | "lib" => OutputType::StaticLibrary,
            "o" | "obj" => OutputType::ObjectFile,
            _ => OutputType::Executable, // Default to executable
        }
    }
}

impl PartialEq for TargetPlatform {
    fn eq(&self, other: &Self) -> bool {
        matches!(
            (self, other),
            (TargetPlatform::Windows, TargetPlatform::Windows)
                | (TargetPlatform::Linux, TargetPlatform::Linux)
                | (TargetPlatform::MacOS, TargetPlatform::MacOS)
        )
    }
}

impl TargetPlatform {
    pub fn detect() -> Self {
        if cfg!(target_os = "windows") {
            TargetPlatform::Windows
        } else if cfg!(target_os = "linux") {
            TargetPlatform::Linux
        } else if cfg!(target_os = "macos") {
            TargetPlatform::MacOS
        } else {
            // Default to Linux for other Unix-like systems
            TargetPlatform::Linux
        }
    }

    pub fn library_extension(&self) -> &'static str {
        match self {
            TargetPlatform::Windows => "dll",
            TargetPlatform::Linux => "so",
            TargetPlatform::MacOS => "dylib",
            TargetPlatform::Android => "so",
        }
    }

    pub fn executable_extension(&self) -> &'static str {
        match self {
            TargetPlatform::Windows => "exe",
            _ => "",
        }
    }

    pub fn object_extension(&self) -> &'static str {
        match self {
            TargetPlatform::Windows => "obj",
            _ => "o",
        }
    }
}

pub struct NativeCompiler {
    platform: TargetPlatform,
}

impl NativeCompiler {
    pub fn new() -> Self {
        Self {
            platform: TargetPlatform::detect(),
        }
    }

    pub fn compile_multiple_c_to_native(
        &self,
        c_files: &[&Path],
        output_path: Option<&Path>,
        output_type: OutputType,
        export: bool,
    ) -> Result<PathBuf, Box<dyn std::error::Error>> {
        if c_files.is_empty() {
            return Err("No C files provided for compilation".into());
        }
        
        let compiler = self.detect_c_compiler()?;
        
        // Use the first file to determine the output path if not specified
        let first_file = c_files[0];
        let final_output_path = self.determine_output_path(first_file, output_path, &output_type)?;
        
        let mut cmd = Command::new(&compiler);
        
        // Add all input files
        for c_file in c_files {
            cmd.arg(c_file);
        }
        
        match compiler.as_str() {
            "cl" => {
                // MSVC compiler
                match output_type {
                    OutputType::Executable => {
                        cmd.arg(format!("/Fe:{}", final_output_path.display()));
                        cmd.arg("/O2"); // Optimization
                    },
                    OutputType::SharedLibrary => {
                        cmd.arg(format!("/Fe:{}", final_output_path.display()));
                        cmd.arg("/LD"); // Create DLL
                        cmd.arg("/O2"); // Optimization
                    },
                    OutputType::StaticLibrary => {
                        return Err("Static library compilation not yet implemented".into());
                    }
                    OutputType::ObjectFile => {
                        cmd.arg(format!("/Fo:{}", final_output_path.display()));
                        cmd.arg("/c"); // Compile only, don't link
                    }
                }
            },
            "gcc" | "clang" => {
                cmd.arg("-o").arg(&final_output_path);
                cmd.arg("-O2"); // Optimization
                
                match output_type {
                    OutputType::Executable => {
                        // No additional flags needed for executable
                    },
                    OutputType::SharedLibrary => {
                        cmd.arg("-shared");
                        if matches!(self.platform, TargetPlatform::Linux | TargetPlatform::MacOS) {
                            cmd.arg("-fPIC");
                        }
                        if self.platform == TargetPlatform::Windows {
                            cmd.arg("-Wl,--out-implib,lib.a"); // Create import library on Windows
                        }
                    },
                    OutputType::StaticLibrary => {
                        return Err("Static library compilation not yet implemented".into());
                    }
                    OutputType::ObjectFile => {
                        cmd.arg("-c"); // Compile only, don't link
                    }
                }
                
                // Platform-specific linking for gcc/clang
                match self.platform {
                    TargetPlatform::Windows => {
                        // For Windows with gcc/clang, use minimal linking
                        // The C runtime should handle basic functionality
                    },
                    TargetPlatform::Linux => {
                        cmd.args(&["-lm", "-lpthread"]);
                    },
                    TargetPlatform::MacOS => {
                        cmd.args(&["-lm", "-lpthread"]);
                    },
                    TargetPlatform::Android => {
                        // Android-specific linking
                        cmd.arg("-lm");
                    },
                }
            },
            _ => {
                return Err(format!("Unsupported compiler: {}", compiler).into());
            }
        }
        
        println!("Compiling multiple C files to native binary: {:?}", cmd);
        
        let output = cmd.output()?;
        
        if !output.status.success() {
            let stderr = String::from_utf8_lossy(&output.stderr);
            return Err(format!("C compilation failed: {}", stderr).into());
        }
        
        println!("Native compilation successful: {}", final_output_path.display());
        Ok(final_output_path)
    }

    pub fn compile_c_to_native(
        &self,
        c_file: &Path,
        output_path: Option<&Path>,
        output_type: OutputType,
        export: bool,
    ) -> Result<PathBuf, Box<dyn std::error::Error>> {
        self.compile_multiple_c_to_native(&[c_file], output_path, output_type, export)
    }

    fn detect_c_compiler(&self) -> Result<String, Box<dyn std::error::Error>> {
        let compilers = match self.platform {
            TargetPlatform::Windows => {
                // Check if we're in a Visual Studio environment first
                if env::var("VCINSTALLDIR").is_ok() || env::var("VCToolsInstallDir").is_ok() {
                    vec!["cl", "gcc", "clang"]
                } else {
                    vec!["gcc", "clang", "cl"]
                }
            },
            TargetPlatform::Android | TargetPlatform::Linux | TargetPlatform::MacOS => {
                vec!["clang", "gcc", "cc"]
            },
        };
        
        for compiler in compilers {
            if Command::new(compiler).arg("--version").output().is_ok() || 
               (compiler == "cl" && Command::new(compiler).output().is_ok()) {
                return Ok(compiler.to_string());
            }
        }
        
        // If no compiler found, provide helpful error message
        let install_msg = match self.platform {
            TargetPlatform::Windows => "Please install MinGW-w64, MSVC Build Tools, or Clang. You can install MinGW-w64 via MSYS2 or install Visual Studio Build Tools.",
            TargetPlatform::Linux => "Please install gcc or clang using your package manager (e.g., apt install gcc, yum install gcc, or pacman -S gcc)",
            TargetPlatform::MacOS => "Please install Xcode Command Line Tools by running 'xcode-select --install'",
            TargetPlatform::Android => "Please install Android NDK with Clang compiler",
        };
        
        Err(format!("No C compiler found. {}", install_msg).into())
    }

    fn determine_output_path(
        &self,
        c_file: &Path,
        output_path: Option<&Path>,
        output_type: &OutputType,
    ) -> Result<PathBuf, Box<dyn std::error::Error>> {
        if let Some(path) = output_path {
            return Ok(path.to_path_buf());
        }
        
        let stem = c_file.file_stem()
            .ok_or("Invalid input file name")?
            .to_string_lossy();
        
        let extension = match output_type {
            OutputType::Executable => self.platform.executable_extension(),
            OutputType::SharedLibrary => self.platform.library_extension(),
            OutputType::StaticLibrary => "a", // Static library extension
            OutputType::ObjectFile => self.platform.object_extension(), // Add this case
        };
        
        let filename = if extension.is_empty() {
            stem.to_string()
        } else {
            format!("{}.{}", stem, extension)
        };
        
        Ok(PathBuf::from(filename))
    }
}
