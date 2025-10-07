//! Tauraro Language Runtime Library

pub mod ast;
pub mod lexer;
pub mod parser;
pub mod semantic;
pub mod ir;
pub mod codegen;
pub mod value;
pub mod builtins;
pub mod builtins_super;
pub mod vm;
pub mod runtime;
pub mod ffi;
pub mod modules;
pub mod module_system;
pub mod module_cache; // Add module cache
pub mod object_system;
pub mod package_manager;
pub mod base_object;
pub mod bytecode; // Export our new bytecode module

// Re-export commonly used items
pub use value::Value;
// Update the re-exports to use the new modular structure
pub use bytecode::arithmetic::{SuperBytecodeVM, SuperCompiler, CodeObject, Frame, RcValue};
pub use bytecode::instructions::{Instruction, OpCode};
pub use vm::memory::Scope; // Add this export

// pub mod type_hierarchy; // Merged into object_system
// pub mod metaclass; // Merged into object_system

use std::cell::RefCell;
use std::path::PathBuf;

#[cfg(feature = "ffi")]
pub mod ffi;

#[cfg(feature = "python-interop")]
pub mod python_interop;

// Remove ambiguous re-exports and be more specific
pub use lexer::Lexer;
pub use parser::Parser;
// pub use ast::*;  // This causes conflicts
pub use semantic::Analyzer;
pub use ir::Generator as IRGenerator;
// pub use vm::*;  // This causes conflicts
pub use runtime::Runtime;
// pub use builtins::*;  // This causes conflicts
// pub use modules::*;  // This causes conflicts
// pub use module_system::*;  // This causes conflicts
// pub use object_system::*;  // This causes conflicts
// Update the bytecode re-export
// pub use bytecode::arithmetic::*;  // This causes conflicts
// pub use bytecode::instructions::*;  // This causes conflicts
// Update the VM re-export
// pub use vm::core::VM;  // This causes conflicts
// pub use vm::frame::ExecutionFrame;
// pub use vm::stack::StackFrame;
// pub use vm::memory::Scope;
// pub use metaclass::*; // Merged into object_system
#[cfg(feature = "ffi")]
pub use ffi::*;