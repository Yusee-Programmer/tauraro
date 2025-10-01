use clap::{Parser, Subcommand};
use std::path::PathBuf;

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
mod object_system;
mod package_manager;
mod base_object;
// Merged into object_system
// mod type_hierarchy;
// mod metaclass;

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
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    env_logger::init();
    
    let cli = Cli::parse();

    match cli.command {
        Commands::Repl => {
            // Use the enhanced REPL from interpreter module
            crate::codegen::interpreter::run_repl()?;
        }
        Commands::Run { file, backend, optimization, strict_types } => {
            let source = std::fs::read_to_string(&file)?;
            vm::run_file_with_options(&source, &backend, optimization, strict_types)?;
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
    }

    Ok(())
}

fn compile_file(
    file: &PathBuf,
    output: Option<&PathBuf>,
    backend: &str,
    _target: &str,
    _optimization: u8,
    export: bool,
    generate_header: bool,
    strict_types: bool,
    native: bool,
) -> Result<(), Box<dyn std::error::Error>> {
    let source = std::fs::read_to_string(file)?;
    
    // Lexical analysis
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    
    // Parsing
    let ast = parser::Parser::new(tokens).parse()?;
    
    // Semantic analysis
    let semantic_ast = semantic::Analyzer::new(strict_types).analyze(ast)
        .map_err(|errors| format!("Semantic errors: {:?}", errors))?;
    
    // Generate IR
    let ir_module = ir::Generator::new().generate(semantic_ast)?;
    
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
            return Err("LLVM backend not enabled".into());
        }
        "c" => {
            let output_path = output.map_or_else(|| PathBuf::from("output.c"), |p| {
                if p.extension().is_none() || p.extension() != Some(std::ffi::OsStr::new("c")) {
                    let mut c_path = p.clone();
                    c_path.set_extension("c");
                    c_path
                } else {
                    p.clone()
                }
            });
            codegen::c_abi::CCodeGen::new()
                .compile(ir_module.clone(), &output_path, export, generate_header)?;
            
            if native {
                use codegen::native::{NativeCompiler, OutputType};
                use codegen::{CodegenOptions, Target};
                
                let compiler = NativeCompiler::new();
                let output_type = if export {
                    OutputType::SharedLibrary
                } else {
                    OutputType::Executable
                };
                
                let options = CodegenOptions {
                    target: Target::C,
                    export_symbols: export,
                    enable_async: false,
                    enable_ffi: false,
                    output_path: Some(output_path.to_string_lossy().to_string()),
                    ..Default::default()
                };
                
                let mut generator = codegen::c_abi::CCodeGenerator::new();
                if generate_header {
                    generator = generator.with_header(true);
                }
                let c_code = generator.generate(ir_module.clone(), &options)?;
                std::fs::write(&output_path, c_code)?;
                
                let native_output = if let Some(orig_output) = output {
                    let stem = orig_output.file_stem()
                        .unwrap_or_else(|| std::ffi::OsStr::new("output"))
                        .to_string_lossy();
                    
                    let platform = codegen::native::TargetPlatform::detect();
                    let extension = match output_type {
                        OutputType::Executable => platform.executable_extension(),
                        OutputType::SharedLibrary => platform.library_extension(),
                        _ => "",
                    };
                    
                    if extension.is_empty() {
                        PathBuf::from(stem.as_ref())
                    } else {
                        PathBuf::from(format!("{}.{}", stem, extension))
                    }
                } else {
                    PathBuf::new()
                };
                
                let final_output = if native_output == PathBuf::new() {
                    None
                } else {
                    Some(native_output.as_path())
                };
                
                compiler.compile_c_to_native(&output_path, final_output, output_type, export)?;
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