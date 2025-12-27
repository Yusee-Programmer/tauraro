//! Main entry point for Tauraro with full bytecode implementation

use anyhow::Result;
use clap::{Parser, Subcommand};
use std::path::PathBuf;

// Import from the tauraro library crate instead of redeclaring modules
#[cfg(any(feature = "c-backend", feature = "clang", feature = "gcc"))]
use tauraro::codegen::c_transpiler::CTranspiler;
#[cfg(any(feature = "c-backend", feature = "clang", feature = "gcc"))]
use tauraro::codegen::c_transpiler::pure_native::PureNativeTranspiler;

use tauraro::codegen::CodeGenerator;

#[derive(Parser)]
#[command(name = "tauraro")]
#[command(about = "Tauraro Programming Language - A modern, high-performance programming language with 100% Python syntax compatibility and multilingual support", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Start the REPL
    Repl {
        /// Enable multilingual mode (Hausa/English)
        #[arg(long)]
        multilingual: bool,
    },

    /// Run a Tauraro file
    Run {
        /// Input file
        file: PathBuf,

        /// Backend to use (vm, rust, c, wasm)
        #[arg(short, long)]
        backend: Option<String>,

        /// Optimization level (0-3)
        #[arg(short, long)]
        optimization: Option<u8>,

        /// Enable strict type checking
        #[arg(long)]
        strict_types: bool,
    },

    /// Compile a Tauraro file
    Compile {
        /// Input file
        file: PathBuf,

        /// Output file
        #[arg(short, long)]
        output: Option<PathBuf>,

        /// Backend to use (vm, rust, c, wasm)
        #[arg(short, long)]
        backend: Option<String>,

        /// Target platform
        #[arg(long, default_value = "native")]
        target: String,

        /// Optimization level (0-3)
        #[arg(long, default_value = "2")]
        optimization: u8,

        /// Export as library
        #[arg(long)]
        export: bool,

        /// Generate header file
        #[arg(long)]
        generate_header: bool,

        /// Enable strict type checking
        #[arg(long)]
        strict_types: bool,

        /// Compile generated C code to native binary/library
        #[arg(long)]
        native: bool,

        /// Library type when using --native (executable, shared)
        #[arg(long, default_value = "executable")]
        lib_type: String,

        /// Use native type transpiler (generates optimized C with native types)
        #[arg(long)]
        use_native_transpiler: bool,

        /// Memory management strategy (auto, manual, arena)
        #[arg(long, default_value = "auto")]
        memory_strategy: String,

        /// Freestanding mode for bare-metal/OS development (no stdlib)
        #[arg(long)]
        freestanding: bool,

        /// Don't link C standard library
        #[arg(long)]
        no_stdlib: bool,

        /// Custom entry point name (default: main)
        #[arg(long)]
        entry_point: Option<String>,

        /// Target architecture for bare-metal (x86, x86_64, arm, arm64, riscv32, riscv64)
        #[arg(long)]
        target_arch: Option<String>,

        /// Enable inline assembly support
        #[arg(long)]
        inline_asm: bool,
    },

    /// Debug AST parsing
    DebugAst {
        /// Input file
        file: PathBuf,
    },

    /// Debug bytecode generation
    DebugBytecode {
        /// Input file
        file: PathBuf,
    },

    /// Debug IR generation
    DebugIr {
        /// Input file
        file: PathBuf,
    },

    /// Clear the module cache
    ClearCache,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    env_logger::init();

    let cli = Cli::parse();

    match cli.command {
        Commands::Repl { multilingual } => {
            // Use the enhanced REPL from interpreter module
            // Call the associated function directly
            tauraro::codegen::interpreter::Interpreter::run_repl_with_multilingual(multilingual)?;
        }
        Commands::Run {
            file,
            backend,
            optimization,
            strict_types,
        } => {
            // Read the file
            let source = match std::fs::read_to_string(&file) {
                Ok(s) => s,
                Err(e) => {
                    eprintln!("Error: Cannot read file '{}': {}", file.display(), e);
                    std::process::exit(1);
                }
            };

            // Use defaults: backend="vm", optimization=0
            let backend_str = backend.as_deref().unwrap_or("vm");
            let opt_level = optimization.unwrap_or(0);

            // Run with specified backend (defaults to "vm")
            if let Err(e) = tauraro::vm::core::VM::run_file_with_options(
                &source,
                &file.to_string_lossy(),
                backend_str,
                opt_level,
                strict_types,
            ) {
                // Error message already includes traceback information from the VM
                eprintln!("{}", e);
                std::process::exit(1);
            }
        }
        #[cfg(any(feature = "c-backend", feature = "clang", feature = "gcc"))]
        Commands::Compile {
            file,
            output,
            backend,
            target,
            optimization,
            export,
            generate_header,
            strict_types,
            native,
            lib_type,
            use_native_transpiler,
            memory_strategy,
            freestanding,
            no_stdlib,
            entry_point,
            target_arch,
            inline_asm,
        } => {
            // Validate that backend is provided
            let backend = match backend {
                Some(b) => b,
                None => {
                    eprintln!("Error: --backend flag is required");
                    eprintln!("\nAvailable backends:");
                    eprintln!("  - rust  : Transpile to Rust code");
                    eprintln!("  - c     : Transpile to C code");
                    std::process::exit(1);
                }
            };

            compile_file(
                &file,
                output.as_ref(),
                &backend,
                &target,
                optimization,
                export,
                generate_header,
                strict_types,
                native,
                &lib_type,
                use_native_transpiler,
                &memory_strategy,
                freestanding,
                no_stdlib,
                entry_point.as_deref(),
                target_arch.as_deref(),
                inline_asm,
            )?;
        }
        Commands::DebugAst { file } => {
            debug_ast(&file)?;
        }
        Commands::DebugBytecode { file } => {
            debug_bytecode(&file)?;
        }
        Commands::DebugIr { file } => {
            debug_ir(&file)?;
        }
        Commands::ClearCache => {
            clear_cache()?;
        }
    }

    Ok(())
}

#[cfg(any(feature = "c-backend", feature = "clang", feature = "gcc"))]
fn compile_file(
    file: &PathBuf,
    output: Option<&PathBuf>,
    backend: &str,
    target: &str,
    optimization: u8,
    export: bool,
    _generate_header: bool,
    strict_types: bool,
    native: bool,
    lib_type: &str,
    use_native_transpiler: bool,
    memory_strategy: &str,
    freestanding: bool,
    no_stdlib: bool,
    entry_point: Option<&str>,
    target_arch: Option<&str>,
    inline_asm: bool,
) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;

    // Lexical analysis
    let tokens = tauraro::lexer::Lexer::new(&source, file.to_string_lossy().to_string())
        .collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\"", file.display());
            e
        })?;

    // Parsing
    let mut parser = tauraro::parser::Parser::new(tokens);
    let ast = parser.parse().map_err(|e| {
        // Find the token that caused the error to get line/column info
        let (line, column) = parser.current_token_location();
        let error_with_location = e.with_location(line, column, &file.to_string_lossy());
        eprintln!("Error in parser:");
        Box::new(error_with_location) as Box<dyn std::error::Error>
    })?;

    // Semantic analysis
    let semantic_ast = tauraro::semantic::Analyzer::new(strict_types)
        .analyze(ast)
        .map_err(|errors| format!("Semantic errors: {:?}", errors))?;

    // Generate IR (clone semantic_ast if needed for native transpiler)
    let ir_module = if backend == "c" && use_native_transpiler {
        // Skip IR generation when using native transpiler
        tauraro::ir::IRModule {
            globals: vec![],
            functions: std::collections::HashMap::new(),
            type_info: tauraro::ir::IRTypeInfo {
                variable_types: std::collections::HashMap::new(),
                function_types: std::collections::HashMap::new(),
            },
        }
    } else {
        tauraro::ir::Generator::new().generate(semantic_ast.clone())?
    };

    // Check if IR module has imports (both in global instructions and function blocks)
    let has_imports = ir_module.globals.iter().any(|instruction| {
        matches!(
            instruction,
            tauraro::ir::IRInstruction::Import { .. }
                | tauraro::ir::IRInstruction::ImportFrom { .. }
        )
    }) || ir_module.functions.iter().any(|(_, function)| {
        function.blocks.iter().any(|block| {
            block.instructions.iter().any(|instruction| {
                matches!(
                    instruction,
                    tauraro::ir::IRInstruction::Import { .. }
                        | tauraro::ir::IRInstruction::ImportFrom { .. }
                )
            })
        })
    });

    // Code generation based on backend
    match backend {
        "llvm" => {
            #[cfg(feature = "llvm")]
            {
                let output_path = output.map_or_else(|| PathBuf::from("a.out"), |p| p.clone());
                tauraro::codegen::llvm::LLVMCodeGen::new().compile(
                    ir_module,
                    &output_path,
                    0,
                    export,
                )?;
            }
            #[cfg(not(feature = "llvm"))]
            {
                // Use our simple LLVM backend as a fallback
                let output_path = output.map_or_else(
                    || PathBuf::from("a.out.ll"),
                    |p| {
                        let mut ll_path = p.clone();
                        if ll_path.extension().is_none() {
                            ll_path.set_extension("ll");
                        }
                        ll_path
                    },
                );

                // Since we don't have the actual codegen, we'll just print a message
                println!("LLVM backend not available, using VM instead");
                tauraro::vm::core::VM::run_file_with_options(
                    &source,
                    "<main>",
                    "vm",
                    optimization,
                    strict_types,
                )?;
            }
        }
        "c" => {
            let mut object_files_to_link: Vec<PathBuf> = Vec::new();

            // Create baremetal options if any flags are set
            let baremetal_opts = if freestanding || no_stdlib || entry_point.is_some() || target_arch.is_some() || inline_asm {
                tauraro::codegen::c_transpiler::BaremetalOptions {
                    freestanding,
                    no_stdlib,
                    entry_point: entry_point.map(|s| s.to_string()),
                    target_arch: target_arch.map(|s| s.to_string()),
                    inline_asm,
                }
            } else {
                tauraro::codegen::c_transpiler::BaremetalOptions::default()
            };

            // Extract imported modules and process them
            let imported_modules = tauraro::codegen::c_transpiler::module_compiler::extract_imported_modules(&ir_module);
            let (builtin_modules, user_modules) = tauraro::codegen::c_transpiler::module_compiler::categorize_modules(&imported_modules);
            
            // Create module compiler if there are imports
            let mut module_compiler = if has_imports {
                let build_dir = PathBuf::from("build");
                let mut compiler = tauraro::codegen::c_transpiler::module_compiler::ModuleCompiler::new(&build_dir);
                compiler.init_directories()?;
                
                // Process builtin modules - compile to object files
                for module_name in &builtin_modules {
                    if let Err(e) = compiler.process_module(module_name) {
                        eprintln!("Warning: Failed to process builtin module '{}': {}", module_name, e);
                    }
                }
                
                // Collect object files for linking
                object_files_to_link.extend(compiler.object_files().iter().cloned());
                
                if !builtin_modules.is_empty() {
                    println!("Compiled {} builtin module(s) to object files in build/builtins/", builtin_modules.len());
                }
                
                if !user_modules.is_empty() {
                    println!("User module(s) detected: {:?}", user_modules);
                    println!("User module headers will be generated in build/headers/");
                }
                
                Some(compiler)
            } else {
                None
            };

            let (c_code_bytes, native_imported_modules) = if use_native_transpiler {
                // Use pure native C transpiler that generates native C code
                let mut transpiler = PureNativeTranspiler::new();
                let c_code = transpiler.transpile_program(&semantic_ast)
                    .map_err(|e| anyhow::anyhow!("Pure native transpiler error: {}", e))?;
                let imported_mods = transpiler.get_imported_modules();
                (c_code.into_bytes(), imported_mods)
            } else {
                // If not using native transpiler, generate IR-based C code with baremetal options
                let transpiler = tauraro::codegen::CTranspiler::with_baremetal_options(baremetal_opts);
                let c_code = transpiler.transpile(&ir_module)?;
                (c_code.into_bytes(), Vec::new())
            };

            // Process user-defined modules when using native transpiler
            if use_native_transpiler && !native_imported_modules.is_empty() {
                use tauraro::codegen::c_transpiler::module_compiler::{is_builtin_module, ModuleCompiler};

                // Create module compiler if it doesn't exist yet
                let module_comp = if let Some(ref mut mc) = module_compiler {
                    mc
                } else {
                    let build_dir = PathBuf::from("build");
                    let mut mc = ModuleCompiler::new(&build_dir);
                    mc.init_directories()?;
                    module_compiler = Some(mc);
                    module_compiler.as_mut().unwrap()
                };

                // Separate builtin and user modules
                let mut user_mods = Vec::new();
                let mut builtin_mods = Vec::new();
                for mod_name in &native_imported_modules {
                    if is_builtin_module(mod_name) {
                        builtin_mods.push(mod_name.clone());
                    } else {
                        user_mods.push(mod_name.clone());
                    }
                }

                // Process builtin modules
                for module_name in &builtin_mods {
                    if let Err(e) = module_comp.process_module(module_name) {
                        eprintln!("Warning: Failed to process builtin module '{}': {}", module_name, e);
                    }
                }
                object_files_to_link.extend(module_comp.object_files().iter().cloned());
                if !builtin_mods.is_empty() {
                    println!("Compiled {} builtin module(s) to object files in build/builtins/", builtin_mods.len());
                }

                // Process user-defined modules - compile them to header files
                for module_name in &user_mods {
                    // Find and compile the module file
                    let module_file = PathBuf::from(format!("{}.py", module_name));
                    if !module_file.exists() {
                        eprintln!("Warning: User module '{}' not found at {}", module_name, module_file.display());
                        continue;
                    }

                    println!("  Compiling user module '{}' to header file...", module_name);

                    // Parse and compile the user module
                    let module_content = std::fs::read_to_string(&module_file)?;

                    // Lexical analysis
                    let module_tokens = tauraro::lexer::Lexer::new(&module_content, module_file.to_string_lossy().to_string())
                        .collect::<Result<Vec<_>, _>>()
                        .map_err(|e| anyhow::anyhow!("Lexer error in module '{}': {}", module_name, e))?;

                    // Parsing
                    let mut module_parser = tauraro::parser::Parser::new(module_tokens);
                    let module_ast = module_parser.parse()
                        .map_err(|e| anyhow::anyhow!("Parser error in module '{}': {}", module_name, e))?;

                    // Semantic analysis
                    let module_semantic_ast = tauraro::semantic::Analyzer::new(strict_types)
                        .analyze(module_ast)
                        .map_err(|errors| anyhow::anyhow!("Semantic errors in module '{}': {:?}", module_name, errors))?;

                    // Transpile module to C code
                    let mut module_transpiler = PureNativeTranspiler::new();
                    let module_c_code = module_transpiler.transpile_program(&module_semantic_ast)
                        .map_err(|e| anyhow::anyhow!("Failed to transpile module '{}': {}", module_name, e))?;

                    // Write as header file
                    let header_path = module_comp.write_user_module_header(module_name, &module_c_code)?;
                    println!("  Generated header: {}", header_path.display());
                }

                if !user_mods.is_empty() {
                    println!("Generated {} user module header(s) in build/headers/", user_mods.len());
                }
            }

            // Determine output path
            let output_path = if let Some(output_file) = output {
                // Use specified output path
                output_file.clone()
            } else if has_imports {
                // If there are imports, create build directory automatically and compile there
                let build_dir = PathBuf::from("build");
                std::fs::create_dir_all(&build_dir)?;
                build_dir.join(format!(
                    "{}.c",
                    file.file_stem().and_then(|s| s.to_str()).unwrap_or("main")
                ))
            } else {
                // If no imports and no destination specified, compile in current directory
                PathBuf::from(format!(
                    "{}.c",
                    file.file_stem()
                        .and_then(|s| s.to_str())
                        .unwrap_or("output")
                ))
            };

            // Write C code to file
            std::fs::write(&output_path, c_code_bytes)?;
            println!("C code generated successfully: {}", output_path.display());

            // If --native flag is set, compile to executable or shared library
            if native {
                let is_shared_lib = lib_type == "shared";

                // Determine output file extension based on lib_type and target platform
                let output_ext = if is_shared_lib {
                    // Shared library extension based on target platform
                    let target_platform = if target != "native" {
                        target.to_string()
                    } else {
                        std::env::consts::OS.to_string()
                    };

                    match target_platform.as_str() {
                        "windows" => "dll",
                        "macos" | "darwin" => "dylib",
                        _ => "so", // Linux, Unix, etc.
                    }
                } else {
                    // Executable extension
                    std::env::consts::EXE_EXTENSION
                };

                // Determine output path
                let binary_path = if output_path.extension().and_then(|s| s.to_str()) == Some("c") {
                    // Change extension from .c to appropriate extension
                    let mut binary_path = output_path.clone();
                    binary_path.set_extension(output_ext);
                    binary_path
                } else {
                    // Append appropriate extension to the output path
                    let mut binary_path = output_path.clone();
                    binary_path.set_file_name(format!(
                        "{}.{}",
                        output_path
                            .file_stem()
                            .and_then(|s| s.to_str())
                            .unwrap_or("output"),
                        output_ext
                    ));
                    binary_path
                };

                // Use our compiler detection module for better error handling
                let compile_result = if is_shared_lib {
                    // Compile to shared library
                    if !object_files_to_link.is_empty() {
                        tauraro::codegen::c_transpiler::compiler::compile_to_shared_library_with_objects(
                            &std::fs::read_to_string(&output_path)?,
                            binary_path.to_str().unwrap(),
                            optimization,
                            &object_files_to_link,
                            target,
                        )
                    } else {
                        tauraro::codegen::c_transpiler::compiler::compile_to_shared_library(
                            &std::fs::read_to_string(&output_path)?,
                            binary_path.to_str().unwrap(),
                            optimization,
                            target,
                        )
                    }
                } else {
                    // Compile to executable
                    if !object_files_to_link.is_empty() {
                        tauraro::codegen::c_transpiler::compiler::compile_to_executable_with_objects(
                            &std::fs::read_to_string(&output_path)?,
                            binary_path.to_str().unwrap(),
                            optimization,
                            &object_files_to_link,
                        )
                    } else {
                        tauraro::codegen::c_transpiler::compiler::compile_to_executable(
                            &std::fs::read_to_string(&output_path)?,
                            binary_path.to_str().unwrap(),
                            optimization,
                        )
                    }
                };

                if let Err(e) = compile_result {
                    let output_type = if is_shared_lib { "shared library" } else { "executable" };
                    eprintln!("Warning: Could not compile to {}: {}", output_type, e);
                    println!("Please compile manually with one of the following:");

                    // Provide specific instructions based on detected compilers
                    let compilers = tauraro::codegen::c_transpiler::compiler::detect_compilers();
                    if compilers.is_empty() {
                        println!("  No C compilers detected. Please install GCC, Clang, or MSVC.");
                    } else {
                        for compiler in &compilers {
                            if is_shared_lib {
                                // Shared library compilation commands
                                match compiler.as_str() {
                                    "gcc" | "clang" => {
                                        println!(
                                            "  {} -shared -fPIC {} -o {} -lm",
                                            compiler,
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                    "cl" => {
                                        println!(
                                            "  cl /LD {} /Fe:{}",
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                    _ => {
                                        println!(
                                            "  {} -shared -fPIC {} -o {}",
                                            compiler,
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                }
                            } else {
                                // Executable compilation commands
                                match compiler.as_str() {
                                    "gcc" | "clang" => {
                                        println!(
                                            "  {} {} -o {} -lm",
                                            compiler,
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                    "cl" => {
                                        println!(
                                            "  cl {} /Fe:{}",
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                    "clang-cl" => {
                                        println!(
                                            "  clang-cl {} -o {}",
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                    _ => {
                                        println!(
                                            "  {} {} -o {}",
                                            compiler,
                                            output_path.display(),
                                            binary_path.display()
                                        );
                                    }
                                }
                            }
                        }
                    }
                } else {
                    let output_type = if is_shared_lib { "Shared library" } else { "Executable" };
                    println!("{} compiled successfully: {}", output_type, binary_path.display());
                }
            }
        }
        "rust" => {
            use tauraro::codegen::rust_transpiler::RustTranspiler;
            use std::path::Path;
            
            // Extract project name from input file
            let project_name = file
                .file_stem()
                .and_then(|s| s.to_str())
                .unwrap_or("tauraro_app");
            
            // Determine output directory
            let build_dir = if let Some(output_file) = output {
                // If output is specified, use its parent as build directory
                output_file.parent().unwrap_or_else(|| Path::new(".")).to_path_buf()
            } else {
                // Otherwise use default build/rust
                PathBuf::from("build")
            };
            
            // Transpile IR module to complete Rust project
            let mut rust_transpiler = RustTranspiler::new("tauraro_module".to_string());
            let project_root = rust_transpiler.transpile_to_project(ir_module, &build_dir, project_name)?;

            // If --native flag is set, build with Cargo
            if native {
                println!("\n→ Building Rust project with Cargo...");
                
                use std::process::Command;
                let cargo_build = Command::new("cargo")
                    .arg("build")
                    .arg("--release")
                    .current_dir(&project_root)
                    .output();
                
                match cargo_build {
                    Ok(output) => {
                        if output.status.success() {
                            println!("✓ Cargo build successful!");
                            
                            // Find and execute the binary
                            #[cfg(target_os = "windows")]
                            let exe_name = format!("{}.exe", project_name);
                            #[cfg(not(target_os = "windows"))]
                            let exe_name = project_name.to_string();
                            
                            let exe_path = project_root.join("target").join("release").join(&exe_name);
                            
                            if exe_path.exists() {
                                println!("→ Executing compiled binary...\n");
                                let exec_output = Command::new(&exe_path)
                                    .output()
                                    .map_err(|e| format!("Failed to execute binary: {}", e))?;
                                
                                print!("{}", String::from_utf8_lossy(&exec_output.stdout));
                                if !exec_output.stderr.is_empty() {
                                    eprint!("{}", String::from_utf8_lossy(&exec_output.stderr));
                                }
                            }
                        } else {
                            eprintln!("Cargo build failed!");
                            eprintln!("{}", String::from_utf8_lossy(&output.stderr));
                            return Err("Cargo build failed".into());
                        }
                    }
                    Err(e) => {
                        return Err(format!("Failed to run cargo: {}", e).into());
                    }
                }
            }
        }
        "wasm" => {
            #[cfg(feature = "wasm")]
            {
                // Use VM for now since WASM backend is not available
                println!("WASM backend not available, using VM instead");
                tauraro::vm::core::VM::run_file_with_options(
                    &source,
                    "<main>",
                    "vm",
                    optimization,
                    strict_types,
                )?;
            }
            #[cfg(not(feature = "wasm"))]
            {
                println!("WASM backend not available, using VM instead");
                tauraro::vm::core::VM::run_file_with_options(
                    &source,
                    "<main>",
                    "vm",
                    optimization,
                    strict_types,
                )?;
            }
        }
        "rust" => {
            // For Rust backend, generate a complete Rust Cargo project
            use tauraro::codegen::rust_transpiler::RustTranspiler;
            use std::path::Path;
            
            // Determine output directory (build/rust by default)
            let build_dir = PathBuf::from("build");
            
            // Extract project name from input file
            let project_name = file
                .file_stem()
                .and_then(|s| s.to_str())
                .unwrap_or("tauraro_app");
            
            // Transpile IR module to complete Rust project
            let mut rust_transpiler = RustTranspiler::new("tauraro_module".to_string());
            let project_root = rust_transpiler.transpile_to_project(ir_module, &build_dir, project_name)?;
            
            // Try to build the project with Cargo if --native flag is set
            if native {
                println!("\n→ Building Rust project with Cargo...");
                
                use std::process::Command;
                let cargo_build = Command::new("cargo")
                    .arg("build")
                    .arg("--release")
                    .current_dir(&project_root)
                    .output();
                
                match cargo_build {
                    Ok(output) => {
                        if output.status.success() {
                            println!("✓ Cargo build successful!");
                            
                            // Find and execute the binary
                            let exe_path = if cfg!(windows) {
                                project_root.join("target").join("release").join(format!("{}.exe", project_name))
                            } else {
                                project_root.join("target").join("release").join(project_name)
                            };
                            
                            if exe_path.exists() {
                                println!("→ Executing compiled binary...\n");
                                let exec_output = Command::new(&exe_path)
                                    .output()
                                    .map_err(|e| format!("Failed to execute binary: {}", e))?;
                                
                                print!("{}", String::from_utf8_lossy(&exec_output.stdout));
                                if !exec_output.stderr.is_empty() {
                                    eprint!("{}", String::from_utf8_lossy(&exec_output.stderr));
                                }
                            }
                        } else {
                            println!("⚠ Cargo build failed!");
                            println!("{}", String::from_utf8_lossy(&output.stderr));
                            println!("\n→ Falling back to VM execution...\n");
                            tauraro::vm::core::VM::run_file_with_options(
                                &source,
                                "<main>",
                                "vm",
                                optimization,
                                strict_types,
                            )?;
                        }
                    }
                    Err(e) => {
                        println!("⚠ Cargo build failed: {}", e);
                        println!("→ Falling back to VM execution...\n");
                        tauraro::vm::core::VM::run_file_with_options(
                            &source,
                            "<main>",
                            "vm",
                            optimization,
                            strict_types,
                        )?;
                    }
                }
            } else {
                // Without --native flag, execute via VM
                println!("→ Executing via VM backend (use --native to build and run Rust binary)...\n");
                tauraro::vm::core::VM::run_file_with_options(
                    &source,
                    "<main>",
                    "vm",
                    optimization,
                    strict_types,
                )?;
            }
        }
        _ => return Err(format!("Unsupported backend: {}", backend).into()),
    }

    println!("Compilation successful!");
    Ok(())
}

/// Compile a Rust source file to an executable using rustc
fn compile_rust_to_executable(
    rust_source: &PathBuf,
    executable_path: &PathBuf,
) -> Result<(), Box<dyn std::error::Error>> {
    use std::process::Command;

    // Try to compile using rustc with Rust 2021 edition to handle async/await
    let mut cmd = Command::new("rustc");
    cmd.arg("--edition")
        .arg("2021")
        .arg(rust_source)
        .arg("-o")
        .arg(executable_path);

    let output = cmd.output()?;

    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr);
        let stdout = String::from_utf8_lossy(&output.stdout);
        
        // Check if it's a dependency issue - we could use cargo build instead
        if stderr.contains("unresolved") || stderr.contains("no `Regex`") {
            eprintln!("Note: Generated Rust code has unresolved external dependencies.");
            eprintln!("For production use, create a Cargo.toml and use 'cargo build --release'");
            eprintln!("\nThe .rs file has been generated at: {}", rust_source.display());
        }
        
        return Err(format!(
            "Failed to compile Rust code with rustc:\n{}\n{}",
            stdout, stderr
        )
        .into());
    }

    Ok(())
}

fn debug_ast(file: &PathBuf) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;

    // Lexical analysis with debug output
    println!("=== TOKENS ===");
    let tokens: Vec<_> = tauraro::lexer::Lexer::new(&source, file.to_string_lossy().to_string())
        .collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\"", file.display());
            e
        })?;

    for (i, token_info) in tokens.iter().enumerate() {
        println!(
            "{}: {:?} (line: {}, col: {})",
            i, token_info.token, token_info.line, token_info.column
        );
    }

    // Parsing
    println!("\n=== PARSING ===");
    let mut parser = tauraro::parser::Parser::new(tokens);
    match parser.parse() {
        Ok(ast) => {
            println!("Successfully parsed AST:");
            println!("{:#?}", ast);
        }
        Err(e) => {
            // Find the token that caused the error to get line/column info
            let (line, column) = parser.current_token_location();
            let error_with_location = e.with_location(line, column, &file.to_string_lossy());
            eprintln!("Error in parser:");
            eprintln!("{}", error_with_location);
            return Err(Box::new(error_with_location));
        }
    }

    Ok(())
}

/// Clear the module cache
fn clear_cache() -> Result<(), Box<dyn std::error::Error>> {
    use tauraro::module_cache::ModuleCache;

    let cache = ModuleCache::new()?;
    cache.clear_cache()?;
    println!("Module cache cleared successfully");
    Ok(())
}

/// Debug IR generation
fn debug_ir(file: &PathBuf) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;

    // Lexical analysis
    let tokens = tauraro::lexer::Lexer::new(&source, file.to_string_lossy().to_string())
        .collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\"", file.display());
            e
        })?;

    // Parsing
    let mut parser = tauraro::parser::Parser::new(tokens);
    let ast = parser.parse().map_err(|e| {
        // Find the token that caused the error to get line/column info
        let (line, column) = parser.current_token_location();
        let error_with_location = e.with_location(line, column, &file.to_string_lossy());
        eprintln!("Error in parser:");
        Box::new(error_with_location) as Box<dyn std::error::Error>
    })?;

    // Semantic analysis
    let semantic_ast = tauraro::semantic::Analyzer::new(false)
        .analyze(ast)
        .map_err(|errors| format!("Semantic errors: {:?}", errors))?;

    // Generate IR
    let ir_module = tauraro::ir::Generator::new().generate(semantic_ast)?;

    // Print the generated IR
    println!("Generated IR:");
    println!("{:#?}", ir_module);

    Ok(())
}

/// Debug bytecode generation
fn debug_bytecode(file: &PathBuf) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;

    // Lexical analysis
    let tokens = tauraro::lexer::Lexer::new(&source, file.to_string_lossy().to_string())
        .collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\"", file.display());
            e
        })?;

    // Parsing
    let mut parser = tauraro::parser::Parser::new(tokens);
    let ast = parser.parse().map_err(|e| {
        // Find the token that caused the error to get line/column info
        let (line, column) = parser.current_token_location();
        let error_with_location = e.with_location(line, column, &file.to_string_lossy());
        eprintln!("Error in parser:");
        Box::new(error_with_location) as Box<dyn std::error::Error>
    })?;

    // Compile to bytecode
    use tauraro::bytecode::SuperCompiler;
    let mut compiler = SuperCompiler::new("<debug>".to_string());
    let code_object = compiler.compile(ast)?;

    // Print the generated bytecode
    println!("Generated bytecode:");
    for (i, instruction) in code_object.instructions.iter().enumerate() {
        println!(
            "  {}: {:?} (arg1: {}, arg2: {}, arg3: {})",
            i, instruction.opcode, instruction.arg1, instruction.arg2, instruction.arg3
        );
    }

    println!("\nConstants:");
    for (i, constant) in code_object.constants.iter().enumerate() {
        println!("  {}: {:?}", i, constant);
    }

    println!("\nNames:");
    for (i, name) in code_object.names.iter().enumerate() {
        println!("  {}: {}", i, name);
    }

    Ok(())
}
