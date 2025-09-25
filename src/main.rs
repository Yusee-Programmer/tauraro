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
mod vm;
mod runtime;
mod ffi;
mod modules;
mod object_system;

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
    },
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    env_logger::init();
    
    let cli = Cli::parse();

    match cli.command {
        Commands::Repl => {
            println!("Starting TauraroLang REPL...");
            let mut interpreter = codegen::interpreter::Interpreter::new();
            interpreter.repl()?;
        }
        Commands::Run { file, backend, optimization } => {
            let source = std::fs::read_to_string(&file)?;
            vm::run_file(&source, &backend, optimization)?;
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
        } => {
            compile_file(
                &file, 
                output.as_ref().map(|p| p.as_path()).map(|p| PathBuf::from(p)).as_ref(), 
                &backend, 
                &target, 
                optimization, 
                export, 
                generate_header,
                strict_types,
            )?;
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
                    .compile(ir_module, &output_path, optimization, export)?;
            }
            #[cfg(not(feature = "llvm"))]
            return Err("LLVM backend not enabled".into());
        }
        "c" => {
            let output_path = output.map_or_else(|| PathBuf::from("output.c"), |p| p.clone());
            codegen::c_abi::CCodeGen::new()
                .compile(ir_module, &output_path, export, generate_header)?;
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