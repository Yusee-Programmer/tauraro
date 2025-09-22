//! TauraroLang CLI - Main entry point for compiler and interpreter
use clap::{Parser, Subcommand};
use std::path::PathBuf;
use anyhow::Result;

mod lexer;
mod parser;
mod ast;
mod semantic;
mod ir;
mod codegen;
mod vm;
mod runtime;
mod ffi;

/// TauraroLang - High-performance multi-paradigm programming language
#[derive(Parser)]
#[command(name = "tauraro")]
#[command(about = "TauraroLang compiler and interpreter", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

/// Supported subcommands
#[derive(Subcommand)]
enum Commands {
    /// Compile Tauraro source to executable
    Compile {
        /// Input source file (.tr)
        input: PathBuf,
        
        /// Output file (default: infer from input)
        #[arg(short, long)]
        output: Option<PathBuf>,
        
        /// Target platform (native, wasm, c)
        #[arg(short, long, default_value = "native")]
        target: String,
        
        /// Optimization level (0-3)
        #[arg(short, long, default_value_t = 2)]
        opt_level: u8,
    },
    
    /// Run Tauraro source directly (interpreted)
    Run {
        /// Input source file (.tr)
        input: PathBuf,
        
        /// Program arguments
        args: Vec<String>,
    },
    
    /// Check syntax without compiling
    Check {
        /// Input source file (.tr)
        input: PathBuf,
    },
    
    /// Export Tauraro functions as C ABI
    Export {
        /// Input source file (.tr)
        input: PathBuf,
        
        /// Output header file (.h)
        #[arg(short, long)]
        header: Option<PathBuf>,
    },
}

fn main() -> Result<()> {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Compile { input, output, target, opt_level } => {
            compile_command(input, output, target, opt_level)
        }
        Commands::Run { input, args } => {
            run_command(input, args)
        }
        Commands::Check { input } => {
            check_command(input)
        }
        Commands::Export { input, header } => {
            export_command(input, header)
        }
    }
}

/// Handle compilation command
fn compile_command(input: PathBuf, output: Option<PathBuf>, target: String, opt_level: u8) -> Result<()> {
    println!("Compiling {} for target {} with O{}", input.display(), target, opt_level);
    
    // Read source code
    let source = std::fs::read_to_string(&input)?;
    
    // Lexical analysis
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    println!("✓ Lexed {} tokens", tokens.len());
    
    // Parsing
    let ast = parser::Parser::new(tokens).parse()?;
    println!("✓ Parsed AST with {} nodes", ast.nodes_count());
    
    // Semantic analysis
    let semantic_ast = semantic::analyze(ast)?;
    println!("✓ Semantic analysis completed");
    
    // Generate IR
    let ir_module = ir::generate(semantic_ast)?;
    println!("✓ Generated IR with {} functions", ir_module.functions.len());
    
    // Code generation
    let output_path = output.unwrap_or_else(|| input.with_extension(target_ext(&target)));
    codegen::compile(ir_module, &output_path, opt_level)?;
    println!("✓ Compiled to {}", output_path.display());
    
    Ok(())
}

/// Handle run command (interpreted mode)
fn run_command(input: PathBuf, args: Vec<String>) -> Result<()> {
    println!("Running {} with args: {:?}", input.display(), args);
    
    let source = std::fs::read_to_string(&input)?;
    
    // For interpreted execution, use VM
    let mut vm = vm::VM::new();
    vm.execute(&source, args)?;
    
    Ok(())
}

/// Handle syntax check command
fn check_command(input: PathBuf) -> Result<()> {
    let source = std::fs::read_to_string(&input)?;
    
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    let _ast = parser::Parser::new(tokens).parse()?;
    
    println!("✓ Syntax check passed for {}", input.display());
    Ok(())
}

/// Handle export command
fn export_command(input: PathBuf, header: Option<PathBuf>) -> Result<()> {
    let source = std::fs::read_to_string(&input)?;
    
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    let ast = parser::Parser::new(tokens).parse()?;
    let semantic_ast = semantic::analyze(ast)?;
    
    // Generate C ABI exports
    let exports = codegen::c_abi::generate_exports(semantic_ast)?;
    
    let header_path = header.unwrap_or_else(|| input.with_extension("h"));
    std::fs::write(&header_path, exports)?;
    
    println!("✓ Generated C header: {}", header_path.display());
    Ok(())
}

/// Get file extension for target
fn target_ext(target: &str) -> &'static str {
    match target {
        "native" => "",
        "wasm" => ".wasm",
        "c" => ".c",
        _ => "",
    }
}