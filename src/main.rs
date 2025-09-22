//! TauraroLang CLI - Complete MVP with REPL and implicit main
use clap::{Parser, Subcommand};
use std::path::PathBuf;
use anyhow::Result;
use std::io::{self, Write};

mod lexer;
mod parser;
mod ast;
mod semantic;
mod ir;
mod codegen;
mod vm;
mod runtime;
mod ffi;
mod python_interop;

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
        
        /// Enable strict type checking
        #[arg(long)]
        strict_types: bool,
    },
    
    /// Run Tauraro source directly (interpreted)
    Run {
        /// Input source file (.tr)
        input: PathBuf,
        
        /// Program arguments
        args: Vec<String>,
        
        /// Enable strict type checking
        #[arg(long)]
        strict_types: bool,
    },
    
    /// Start REPL interactive shell
    Repl {
        /// Enable strict type checking
        #[arg(long)]
        strict_types: bool,
    },
    
    /// Check syntax without compiling
    Check {
        /// Input source file (.tr)
        input: PathBuf,
        
        /// Enable strict type checking
        #[arg(long)]
        strict_types: bool,
    },
    
    /// Export Tauraro functions as C ABI
    Export {
        /// Input source file (.tr)
        input: PathBuf,
        
        /// Output header file (.h)
        #[arg(short, long)]
        header: Option<PathBuf>,
    },
    
    /// Run Python interoperability test
    PythonTest,
}

fn main() -> Result<()> {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Compile { input, output, target, opt_level, strict_types } => {
            compile_command(input, output, target, opt_level, strict_types)
        }
        Commands::Run { input, args, strict_types } => {
            run_command(input, args, strict_types)
        }
        Commands::Repl { strict_types } => {
            repl_command(strict_types)
        }
        Commands::Check { input, strict_types } => {
            check_command(input, strict_types)
        }
        Commands::Export { input, header } => {
            export_command(input, header)
        }
        Commands::PythonTest => {
            python_test_command()
        }
    }
}

/// Handle compilation command
fn compile_command(
    input: PathBuf, 
    output: Option<PathBuf>, 
    target: String, 
    opt_level: u8,
    strict_types: bool
) -> Result<()> {
    println!("Compiling {} for target {} with O{}", input.display(), target, opt_level);
    
    // Read source code
    let source = std::fs::read_to_string(&input)?;
    
    // Lexical analysis
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    println!("✓ Lexed {} tokens", tokens.len());
    
    // Parsing
    let mut parser = parser::Parser::new(tokens);
    let ast = parser.parse_with_implicit_main()?;
    println!("✓ Parsed AST with {} nodes", ast.nodes_count());
    
    // Semantic analysis
    let semantic_ast = semantic::analyze_optional_types(ast, strict_types)?;
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
fn run_command(input: PathBuf, args: Vec<String>, strict_types: bool) -> Result<()> {
    println!("Running {} with args: {:?}", input.display(), args);
    
    let source = std::fs::read_to_string(&input)?;
    
    // For interpreted execution, use VM
    let mut vm = vm::VM::new();
    vm.set_strict_types(strict_types);
    vm.execute_script(&source, args)?;
    
    Ok(())
}

/// Handle REPL command
fn repl_command(strict_types: bool) -> Result<()> {
    println!("TauraroLang REPL - Type 'exit' to quit, 'help' for help");
    if strict_types {
        println!("Strict type checking: ENABLED");
    }
    
    let mut vm = vm::VM::new();
    vm.set_strict_types(strict_types);
    let mut line_number = 1;
    
    loop {
        print!(">>> ");
        io::stdout().flush()?;
        
        let mut input = String::new();
        io::stdin().read_line(&mut input)?;
        
        let input = input.trim();
        if input.is_empty() {
            continue;
        }
        
        match input {
            "exit" | "quit" => break,
            "help" => {
                println!("TauraroLang REPL commands:");
                println!("  exit/quit - Exit REPL");
                println!("  help      - Show this help");
                println!("  stats     - Show memory statistics");
                println!("  clear     - Clear screen");
                println!("  types     - Toggle strict type checking");
                continue;
            }
            "stats" => {
                println!("{}", vm.memory_stats());
                continue;
            }
            "clear" => {
                print!("{esc}[2J{esc}[1;1H", esc = 27 as char);
                continue;
            }
            "types" => {
                let new_setting = !strict_types;
                vm.set_strict_types(new_setting);
                println!("Strict type checking: {}", if new_setting { "ENABLED" } else { "DISABLED" });
                continue;
            }
            _ => {}
        }
        
        // Handle both expressions and statements
        match vm.execute_repl(input, line_number) {
            Ok(result) => {
                if let Some(value) = result {
                    println!("{}", value);
                }
            }
            Err(e) => {
                eprintln!("Error: {}", e);
            }
        }
        
        line_number += 1;
    }
    
    Ok(())
}

/// Handle syntax check command
fn check_command(input: PathBuf, strict_types: bool) -> Result<()> {
    let source = std::fs::read_to_string(&input)?;
    
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    let mut parser = parser::Parser::new(tokens);
    let ast = parser.parse_with_implicit_main()?;
    let _ = semantic::analyze_optional_types(ast, strict_types)?;
    
    println!("✓ Syntax check passed for {}", input.display());
    Ok(())
}

/// Handle export command
fn export_command(input: PathBuf, header: Option<PathBuf>) -> Result<()> {
    let source = std::fs::read_to_string(&input)?;
    
    let tokens = lexer::Lexer::new(&source).collect::<Result<Vec<_>, _>>()?;
    let mut parser = parser::Parser::new(tokens);
    let ast = parser.parse_with_implicit_main()?;
    let semantic_ast = semantic::analyze(ast)?;
    
    // Generate C ABI exports
    let exports = codegen::c_abi::generate_exports(semantic_ast)?;
    
    let header_path = header.unwrap_or_else(|| input.with_extension("h"));
    std::fs::write(&header_path, exports)?;
    
    println!("✓ Generated C header: {}", header_path.display());
    Ok(())
}

/// Handle Python test command
fn python_test_command() -> Result<()> {
    println!("Testing Python interoperability...");
    
    match python_interop::PythonInterop::new() {
        Ok(interop) => {
            println!("✓ Python runtime initialized");
            
            // Test basic Python integration
            let math_module = interop.import_module("math")?;
            println!("✓ Imported Python math module");
            
            println!("Python interoperability: WORKING");
        }
        Err(e) => {
            eprintln!("Python interoperability: FAILED - {}", e);
            eprintln!("Note: Python integration requires pyo3 and Python 3.7+ installed");
        }
    }
    
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