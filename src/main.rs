//! Main entry point for Tauraro with full bytecode implementation

use clap::{Parser, Subcommand};
use std::path::PathBuf;
use anyhow::Result;

// Import from the tauraro library crate instead of redeclaring modules
use tauraro::codegen::c_transpiler::CTranspiler;
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
        
        /// Backend to use (vm, llvm, c, wasm)
        #[arg(short, long, default_value = "vm")]
        backend: String,
        
        /// Optimization level (0-3)
        #[arg(short, long, default_value = "0")]
        optimization: u8,
        
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
        
        /// Backend to use (vm, llvm, c, wasm)
        #[arg(short, long, default_value = "llvm")]
        backend: String,
        
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

        /// Use native type transpiler (generates optimized C with native types)
        #[arg(long)]
        use_native_transpiler: bool,
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
        Commands::Run { file, backend, optimization, strict_types } => {
            let source = std::fs::read_to_string(&file)?;

            // Always use VM for now
            if let Err(e) = tauraro::vm::core::VM::run_file_with_options(&source, &file.to_string_lossy(), &backend, optimization, strict_types) {
                // Error message already includes traceback information from the VM
                eprintln!("{}", e);
                std::process::exit(1);
            }
        }
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
            use_native_transpiler,
        } => {
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
                use_native_transpiler,
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

fn compile_file(
    file: &PathBuf,
    output: Option<&PathBuf>,
    backend: &str,
    target: &str,
    optimization: u8,
    export: bool,
    generate_header: bool,
    strict_types: bool,
    native: bool,
    use_native_transpiler: bool,
) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;
    
    // Lexical analysis
    let tokens = tauraro::lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\", line 1", file.display());
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
    let semantic_ast = tauraro::semantic::Analyzer::new(strict_types).analyze(ast)
        .map_err(|errors| format!("Semantic errors: {:?}", errors))?;
    
    // Generate IR
    let ir_module = tauraro::ir::Generator::new().generate(semantic_ast)?;
    
    // Check if IR module has imports (both in global instructions and function blocks)
    let has_imports = ir_module.globals.iter().any(|instruction| {
        matches!(instruction, tauraro::ir::IRInstruction::Import { .. } |
                                  tauraro::ir::IRInstruction::ImportFrom { .. })
    }) || ir_module.functions.iter().any(|(_, function)| {
        function.blocks.iter().any(|block| {
            block.instructions.iter().any(|instruction| {
                matches!(instruction, tauraro::ir::IRInstruction::Import { .. } |
                tauraro::ir::IRInstruction::ImportFrom { .. })
            })
        })
    });
    
    // Code generation based on backend
    match backend {
        "llvm" => {
            #[cfg(feature = "llvm")]
            {
                let output_path = output.map_or_else(|| PathBuf::from("a.out"), |p| p.clone());
                        tauraro::codegen::llvm::LLVMCodeGen::new()
                    .compile(ir_module, &output_path, 0, export)?;
            }
            #[cfg(not(feature = "llvm"))]
            {
                // Use our simple LLVM backend as a fallback
                let output_path = output.map_or_else(|| PathBuf::from("a.out.ll"), |p| {
                    let mut ll_path = p.clone();
                    if ll_path.extension().is_none() {
                        ll_path.set_extension("ll");
                    }
                    ll_path
                });
                
                // Since we don't have the actual codegen, we'll just print a message
                println!("LLVM backend not available, using VM instead");
                tauraro::vm::core::VM::run_file_with_options(&source, "<main>", "vm", optimization, strict_types)?;

            }
        }
        "c" => {
            let c_code_bytes = if use_native_transpiler {
                // Use native type transpiler (works directly from AST)
                let mut native_transpiler = tauraro::codegen::c_transpiler::optimized_native::OptimizedNativeTranspiler::new()
                    .with_optimizations(optimization > 0);
                let c_code = native_transpiler.transpile_program(&semantic_ast)?;
                c_code.into_bytes()
            } else {
                // Use traditional IR-based C transpiler
                let c_transpiler = tauraro::codegen::CTranspiler::new();
                let options = tauraro::codegen::CodegenOptions {
                    target: tauraro::codegen::Target::C,
                    opt_level: optimization,
                    target_triple: Some(target.to_string()),
                    export_symbols: export,
                    generate_debug_info: false,
                    enable_async: true,
                    enable_ffi: true,
                    strict_types,
                    output_path: output.map(|p| p.to_string_lossy().to_string()),
                };
                c_transpiler.generate(ir_module, &options)?
            };
            
            // Determine output path
            let output_path = if let Some(output_file) = output {
                // Use specified output path
                output_file.clone()
            } else if has_imports {
                // If there are imports, create build directory automatically and compile there
                let build_dir = PathBuf::from("build");
                std::fs::create_dir_all(&build_dir)?;
                build_dir.join(format!("{}.c", file.file_stem().and_then(|s| s.to_str()).unwrap_or("main")))
            } else {
                // If no imports and no destination specified, compile in current directory
                PathBuf::from(format!("{}.c", file.file_stem().and_then(|s| s.to_str()).unwrap_or("output")))
            };
            
            // Write C code to file
            std::fs::write(&output_path, c_code_bytes)?;
            println!("C code generated successfully: {}", output_path.display());
            
            // If --native flag is set, compile to executable
            if native {
                // Determine executable path
                let exe_path = if output_path.extension().and_then(|s| s.to_str()) == Some("c") {
                    // Change extension to exe (or no extension on Unix)
                    let mut exe_path = output_path.clone();
                    exe_path.set_extension(std::env::consts::EXE_EXTENSION);
                    exe_path
                } else {
                    // Append .exe to the output path
                    let mut exe_path = output_path.clone();
                    exe_path.set_file_name(format!("{}.{}", 
                        output_path.file_stem().and_then(|s| s.to_str()).unwrap_or("output"),
                        std::env::consts::EXE_EXTENSION));
                    exe_path
                };
                
                // Use our compiler detection module for better error handling
                if let Err(e) = tauraro::codegen::c_transpiler::compiler::compile_to_executable(
                    &std::fs::read_to_string(&output_path)?, 
                    exe_path.to_str().unwrap(), 
                    optimization
                ) {
                    eprintln!("Warning: Could not compile to executable: {}", e);
                    println!("Please compile manually with one of the following:");
                    
                    // Provide specific instructions based on detected compilers
                    let compilers = tauraro::codegen::c_transpiler::compiler::detect_compilers();
                    if compilers.is_empty() {
                        println!("  No C compilers detected. Please install GCC, Clang, or MSVC.");
                    } else {
                        for compiler in &compilers {
                            match compiler.as_str() {
                                "gcc" | "clang" => {
                                    println!("  {} {} -o {} -lm", compiler, output_path.display(), exe_path.display());
                                }
                                "cl" => {
                                    println!("  cl {} /Fe:{}", output_path.display(), exe_path.display());
                                }
                                "clang-cl" => {
                                    println!("  clang-cl {} -o {}", output_path.display(), exe_path.display());
                                }
                                _ => {
                                    println!("  {} {} -o {}", compiler, output_path.display(), exe_path.display());
                                }
                            }
                        }
                    }
                } else {
                    println!("Executable compiled successfully: {}", exe_path.display());
                }
            }
        }
        "wasm" => {
            #[cfg(feature = "wasm")]
            {
                // Use VM for now since WASM backend is not available
                println!("WASM backend not available, using VM instead");
                tauraro::vm::core::VM::run_file_with_options(&source, "<main>", "vm", optimization, strict_types)?;
            }
            #[cfg(not(feature = "wasm"))]
            {
                println!("WASM backend not available, using VM instead");
                tauraro::vm::core::VM::run_file_with_options(&source, "<main>", "vm", optimization, strict_types)?;
            }
        }
        _ => return Err(format!("Unsupported backend: {}", backend).into()),
    }
    
    println!("Compilation successful!");
    Ok(())
}

fn debug_ast(file: &PathBuf) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;
    
    // Lexical analysis with debug output
    println!("=== TOKENS ===");
    let tokens: Vec<_> = tauraro::lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\", line 1", file.display());
            e
        })?;
    
    for (i, token_info) in tokens.iter().enumerate() {
        println!("{}: {:?} (line: {}, col: {})", i, token_info.token, token_info.line, token_info.column);
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
    let tokens = tauraro::lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\", line 1", file.display());
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
    let semantic_ast = tauraro::semantic::Analyzer::new(false).analyze(ast)
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
    let tokens = tauraro::lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\", line 1", file.display());
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
    use tauraro::bytecode::{SuperCompiler};
    let mut compiler = SuperCompiler::new("<debug>".to_string());
    let code_object = compiler.compile(ast)?;
    
    // Print the generated bytecode
    println!("Generated bytecode:");
    for (i, instruction) in code_object.instructions.iter().enumerate() {
        println!("  {}: {:?} (arg1: {}, arg2: {}, arg3: {})", 
                 i, instruction.opcode, instruction.arg1, instruction.arg2, instruction.arg3);
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