//! Tauraro Language Runtime Library

pub mod ast;
pub mod lexer;
pub mod parser;
pub mod semantic;
pub mod ir;
pub mod codegen;
pub mod value;
pub mod value_pool;

// FFI modules (before builtins so it can be imported)
#[cfg(feature = "ffi")]
pub mod ffi;

#[cfg(feature = "ffi")]
pub mod ffi_builtins;

pub mod builtins;
pub mod builtins_super;
pub mod vm;
pub mod runtime;
pub mod runtime_error;
pub mod modules;
pub mod module_system;
pub mod module_cache;
pub mod object_system;
pub mod package_manager;
pub mod base_object;
pub mod bytecode;
pub mod type_checker;

// Re-export commonly used items
pub use value::Value;
pub use bytecode::{SuperBytecodeVM, SuperCompiler, CodeObject, Frame, RcValue};
pub use bytecode::instructions::{Instruction, OpCode};
pub use vm::memory::Scope;
pub use type_checker::{TypeChecker, TypeEnvironment, TypeInfo};

#[cfg(feature = "python-interop")]
pub mod python_interop;

// Helper function to get FFI builtins (accessible from builtins.rs)
#[cfg(feature = "ffi")]
pub fn get_ffi_builtins() -> std::collections::HashMap<String, Value> {
    ffi_builtins::init_ffi_builtins()
}

pub use lexer::Lexer;
pub use parser::Parser;
pub use semantic::Analyzer;
pub use ir::Generator as IRGenerator;
pub use runtime::Runtime;