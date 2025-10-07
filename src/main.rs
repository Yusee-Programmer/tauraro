//! Main entry point for Tauraro with full bytecode implementation

use clap::{Parser, Subcommand};
use std::path::PathBuf;
use anyhow::{Result, anyhow}; // Add this line

mod lexer;
mod parser;
mod ast;
mod semantic;
mod ir;
mod codegen;
mod value;
mod builtins;
mod builtins_super;
mod vm;
mod runtime;
mod ffi;
mod modules;
mod module_system;
mod module_cache;
mod object_system;
mod package_manager;
mod base_object;
mod bytecode; // Our new full bytecode implementation

#[cfg(feature = "test-llvm")]
mod test_llvm;

use crate::value::Value;
use crate::codegen::{CodeGen, CodegenOptions, Target, CodeGenerator};
use crate::codegen::interpreter::Interpreter;

#[derive(Parser)]
#[command(name = "tauraro")]
#[command(about = "TauraroLang: A flexible programming language", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Start the REPL
    Repl,
    
    /// Run a TauraroLang file
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
    
    /// Compile a TauraroLang file
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
    },
    
    /// Debug AST parsing
    DebugAst {
        /// Input file
        file: PathBuf,
    },
    
    /// Clear the module cache
    ClearCache,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    env_logger::init();
    
    let cli = Cli::parse();

    // Add a test command for LLVM backend
    #[cfg(feature = "test-llvm")]
    {
        if let Err(e) = test_llvm::test_simple_llvm_backend() {
            eprintln!("LLVM test failed: {}", e);
            std::process::exit(1);
        }
        return Ok(());
    }
    
    match cli.command {
        Commands::Repl => {
            // Use the enhanced REPL from interpreter module
            crate::codegen::interpreter::run_repl()?;
        }
        Commands::Run { file, backend, optimization, strict_types } => {
            let source = std::fs::read_to_string(&file)?;
            
            // Use our new full bytecode compiler for better performance when backend is "vm" and optimization > 0
            if backend == "vm" && optimization > 0 {
                if let Err(e) = run_file_bytecode(&source) {
                    eprintln!("Traceback (most recent call last):");
                    eprintln!("  File \"{}\", line 1, in <module>", file.display());
                    eprintln!("{}", e);
                    std::process::exit(1);
                }
            } else {
                if let Err(e) = crate::vm::core::VM::run_file_with_options(&source, &backend, optimization, strict_types) {
                    eprintln!("Traceback (most recent call last):");
                    eprintln!("  File \"{}\", line 1, in <module>", file.display());
                    eprintln!("{}", e);
                    std::process::exit(1);
                }
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
            )?;
        }
        Commands::DebugAst { file } => {
            debug_ast(&file)?;
        }
        Commands::ClearCache => {
            clear_cache()?;
        }
    }

    Ok(())
}

/// Run a TauraroLang file with our new full bytecode implementation
pub fn run_file_bytecode(source: &str) -> Result<()> {
    use crate::lexer::Lexer;
    use crate::parser::Parser;
    use crate::bytecode::arithmetic::{SuperCompiler, SuperBytecodeVM}; // Updated to use our new implementation
    
    // Debug output to see if this function is being called
    eprintln!("DEBUG: run_file_bytecode called");
    
    // Parse the source code
    let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| anyhow!("Lexer error: {}", e))?;
    let mut parser = Parser::new(tokens);
    let program = parser.parse()?;
    
    // Compile to bytecode using our new full implementation
    let mut compiler = SuperCompiler::new("<stdin>".to_string()); // Updated to use our new compiler
    let code = compiler.compile(program)?;
    
    // Debug output to see the compiled code
    eprintln!("DEBUG: Main module compiled with {} instructions", code.instructions.len());
    for (i, instruction) in code.instructions.iter().enumerate() {
        eprintln!("DEBUG: Instruction {}: {:?}", i, instruction);
    }
    
    // Execute bytecode using our new VM
    eprintln!("DEBUG: Creating VM and executing code");
    let mut vm = SuperBytecodeVM::new(); // Updated to use our new VM
    vm.execute(code)?;
    
    Ok(())
}

fn compile_file(
    file: &PathBuf,
    output: Option<&PathBuf>,
    backend: &str,
    target: &str,
    _optimization: u8,
    export: bool,
    generate_header: bool,
    strict_types: bool,
    native: bool,
) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;
    
    // Lexical analysis
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\", line 1", file.display());
            e
        })?;
    
    // Parsing
    let mut parser = parser::Parser::new(tokens);
    let ast = parser.parse().map_err(|e| {
        // Create a more detailed error message with location information
        eprintln!("Error in parser:");
        // For now, we'll just show a generic location since we don't have detailed line info in the error
        eprintln!("  File \"{}\", line 1", file.display());
        e
    })?;
    
    // Semantic analysis
    let semantic_ast = semantic::Analyzer::new(strict_types).analyze(ast)
        .map_err(|errors| format!("Semantic errors: {:?}", errors))?;
    
    // Generate IR
    let ir_module = ir::Generator::new().generate(semantic_ast)?;
    
    // Check if IR module has imports
    let has_imports = ir_module.functions.iter().any(|(_, function)| {
        function.blocks.iter().any(|block| {
            block.instructions.iter().any(|instruction| {
                matches!(instruction, crate::ir::IRInstruction::Import { .. } | 
                                  crate::ir::IRInstruction::ImportFrom { .. })
            })
        })
    });
    
    // Code generation based on backend
    match backend {
        "llvm" => {
            #[cfg(feature = "llvm")]
            {
                let output_path = output.map_or_else(|| PathBuf::from("a.out"), |p| p.clone());
                codegen::llvm::LLVMCodeGen::new()
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
                
                use crate::codegen::{CodeGen, CodegenOptions, Target};
                let codegen = CodeGen::new();
                let options = CodegenOptions {
                    target: Target::Native,
                    export_symbols: export,
                    ..Default::default()
                };
                
                let llvm_ir = codegen.generate(ir_module, &options)?;
                std::fs::write(&output_path, llvm_ir)?;
                println!("Generated LLVM IR to {}", output_path.display());
            }
        }
        "c" => {
            // Determine output path based on whether there are imports and if destination is specified
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
            
            // Use import-aware compilation if requested
            if native {
                use codegen::native::{NativeCompiler, OutputType, TargetPlatform};
                use codegen::{CodegenOptions, Target};
                use codegen::imports::ImportCompiler;
                
                // Determine build directory based on whether there are imports
                let build_dir = if has_imports {
                    PathBuf::from("build")
                } else {
                    // For files without imports, we don't need a build directory
                    std::env::current_dir()?
                };
                
                let mut import_compiler = ImportCompiler::new(build_dir.clone());
                
                let options = CodegenOptions {
                    target: Target::C,
                    export_symbols: export,
                    enable_async: false,
                    enable_ffi: false,
                    output_path: Some(output_path.to_string_lossy().to_string()),
                    ..Default::default()
                };
                
                // Compile main module and all imports
                let compiled_files = import_compiler.compile_with_imports(&file, &options)?;
                
                // Determine output type based on target and export flag
                let output_type = if !target.is_empty() && target != "native" {
                    // If target is specified, use it to determine output type
                    OutputType::from_target_string(target)
                } else if export {
                    // If export flag is set, create shared library
                    OutputType::SharedLibrary
                } else {
                    // Default to executable when --native is used without --target
                    OutputType::Executable
                };
                
                // Compile to native
                let main_module_name = file.file_stem()
                    .and_then(|s| s.to_str())
                    .unwrap_or("main")
                    .to_string();
                    
                let executable_path = import_compiler.compile_to_native_with_options(
                    &compiled_files, 
                    &main_module_name, 
                    &options,
                    output_type,
                    target,
                )?;
                
                if has_imports {
                    println!("Generated C files in {:?}", PathBuf::from("build"));
                }
                println!("Compiled to native target: {}", executable_path.display());
            } else {
                // Original single-file compilation
                codegen::c_abi::CCodeGen::new()
                    .compile(ir_module.clone(), &output_path, export, generate_header)?;
                    
                // Print appropriate message based on whether build directory was created
                if has_imports {
                    println!("Generated C file in {:?}", PathBuf::from("build"));
                } else {
                    println!("Generated C file: {}", output_path.display());
                }
            }
        }
        "wasm" => {
            #[cfg(feature = "wasm")]
            {
                let output_path = output.map_or_else(|| PathBuf::from("output.wasm"), |p| p.clone());
                codegen::wasm::WasmCodeGen::new()
                    .compile(ir_module, &output_path, export)?;
            }
            #[cfg(not(feature = "wasm"))]
            return Err("WASM backend not enabled".into());
        }
        _ => return Err(format!("Unsupported backend: {}", backend).into()),
    }
    
    println!("Compilation successful!");
    Ok(())
}

fn debug_ast(file: &PathBuf) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;
    
    // Lexical analysis
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()
        .map_err(|e| {
            eprintln!("Error in lexer:");
            eprintln!("  File \"{}\", line 1", file.display());
            e
        })?;
    
    // Parsing
    let mut parser = parser::Parser::new(tokens);
    let ast = parser.parse().map_err(|e| {
        // Create a more detailed error message with location information
        eprintln!("Error in parser:");
        // For now, we'll just show a generic location since we don't have detailed line info in the error
        eprintln!("  File \"{}\", line 1", file.display());
        e
    })?;
    
    // Print AST for debugging
    println!("AST: {:#?}", ast);
    
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
