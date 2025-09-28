/// Target platform utilities for code generation
use std::env;

/// Get the default target triple for the current platform
pub fn get_default_target_triple() -> String {
    // Try to get from environment first
    if let Ok(target) = env::var("TARGET") {
        return target;
    }
    
    // Detect based on current platform
    let arch = if cfg!(target_arch = "x86_64") {
        "x86_64"
    } else if cfg!(target_arch = "aarch64") {
        "aarch64"
    } else if cfg!(target_arch = "x86") {
        "i686"
    } else if cfg!(target_arch = "arm") {
        "arm"
    } else {
        "unknown"
    };
    
    let vendor = if cfg!(target_vendor = "apple") {
        "apple"
    } else if cfg!(target_vendor = "pc") {
        "pc"
    } else {
        "unknown"
    };
    
    let os = if cfg!(target_os = "windows") {
        "windows"
    } else if cfg!(target_os = "macos") {
        "darwin"
    } else if cfg!(target_os = "linux") {
        "linux"
    } else if cfg!(target_os = "android") {
        "android"
    } else if cfg!(target_os = "ios") {
        "ios"
    } else {
        "unknown"
    };
    
    let env = if cfg!(target_env = "msvc") {
        "-msvc"
    } else if cfg!(target_env = "gnu") {
        "-gnu"
    } else if cfg!(target_env = "musl") {
        "-musl"
    } else {
        ""
    };
    
    format!("{}-{}-{}{}", arch, vendor, os, env)
}

/// Get supported target triples
pub fn get_supported_targets() -> Vec<&'static str> {
    vec![
        // x86_64 targets
        "x86_64-unknown-linux-gnu",
        "x86_64-unknown-linux-musl",
        "x86_64-pc-windows-msvc",
        "x86_64-pc-windows-gnu",
        "x86_64-apple-darwin",
        
        // aarch64 targets
        "aarch64-unknown-linux-gnu",
        "aarch64-unknown-linux-musl",
        "aarch64-pc-windows-msvc",
        "aarch64-apple-darwin",
        "aarch64-apple-ios",
        "aarch64-linux-android",
        
        // ARM targets
        "arm-unknown-linux-gnueabihf",
        "arm-linux-androideabi",
        "armv7-unknown-linux-gnueabihf",
        "armv7-linux-androideabi",
        
        // WebAssembly
        "wasm32-unknown-unknown",
        "wasm32-wasi",
        
        // i686 targets
        "i686-unknown-linux-gnu",
        "i686-pc-windows-msvc",
        "i686-pc-windows-gnu",
    ]
}

/// Check if a target triple is supported
pub fn is_supported_target(target: &str) -> bool {
    get_supported_targets().contains(&target)
}

/// Get the file extension for executables on the target platform
pub fn get_executable_extension(target: &str) -> &'static str {
    if target.contains("windows") {
        ".exe"
    } else if target.contains("wasm") {
        ".wasm"
    } else {
        ""
    }
}

/// Get the file extension for dynamic libraries on the target platform
pub fn get_dylib_extension(target: &str) -> &'static str {
    if target.contains("windows") {
        ".dll"
    } else if target.contains("darwin") || target.contains("ios") {
        ".dylib"
    } else {
        ".so"
    }
}

/// Get the file extension for static libraries on the target platform
pub fn get_staticlib_extension(target: &str) -> &'static str {
    if target.contains("windows") {
        ".lib"
    } else {
        ".a"
    }
}

/// Target architecture information
#[derive(Debug, Clone, PartialEq)]
pub enum Architecture {
    X86_64,
    AArch64,
    X86,
    ARM,
    WASM32,
    Unknown,
}

impl Architecture {
    pub fn from_target(target: &str) -> Self {
        if target.starts_with("x86_64") {
            Architecture::X86_64
        } else if target.starts_with("aarch64") {
            Architecture::AArch64
        } else if target.starts_with("i686") {
            Architecture::X86
        } else if target.starts_with("arm") {
            Architecture::ARM
        } else if target.starts_with("wasm32") {
            Architecture::WASM32
        } else {
            Architecture::Unknown
        }
    }
    
    pub fn pointer_size(&self) -> usize {
        match self {
            Architecture::X86_64 | Architecture::AArch64 => 8,
            Architecture::X86 | Architecture::ARM | Architecture::WASM32 => 4,
            Architecture::Unknown => 8, // Default to 64-bit
        }
    }
    
    pub fn is_64bit(&self) -> bool {
        matches!(self, Architecture::X86_64 | Architecture::AArch64)
    }
}

/// Operating system information
#[derive(Debug, Clone, PartialEq)]
pub enum OperatingSystem {
    Linux,
    Windows,
    MacOS,
    iOS,
    Android,
    WASI,
    Unknown,
}

impl OperatingSystem {
    pub fn from_target(target: &str) -> Self {
        if target.contains("linux") {
            OperatingSystem::Linux
        } else if target.contains("windows") {
            OperatingSystem::Windows
        } else if target.contains("darwin") {
            OperatingSystem::MacOS
        } else if target.contains("ios") {
            OperatingSystem::iOS
        } else if target.contains("android") {
            OperatingSystem::Android
        } else if target.contains("wasi") {
            OperatingSystem::WASI
        } else {
            OperatingSystem::Unknown
        }
    }
    
    pub fn supports_dynamic_linking(&self) -> bool {
        !matches!(self, OperatingSystem::WASI)
    }
    
    pub fn supports_threads(&self) -> bool {
        !matches!(self, OperatingSystem::WASI)
    }
}

/// Target information
#[derive(Debug, Clone)]
pub struct TargetInfo {
    pub triple: String,
    pub arch: Architecture,
    pub os: OperatingSystem,
}

impl TargetInfo {
    pub fn new(triple: String) -> Self {
        let arch = Architecture::from_target(&triple);
        let os = OperatingSystem::from_target(&triple);
        
        Self { triple, arch, os }
    }
    
    pub fn current() -> Self {
        Self::new(get_default_target_triple())
    }
}
