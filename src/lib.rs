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
pub mod object_system;
pub mod package_manager;
pub mod base_object;
pub mod bytecode; // Export our new bytecode module

// Re-export commonly used items
pub use value::Value;
pub use bytecode::{SuperBytecodeVM, SuperCompiler, CodeObject, Instruction, OpCode};
// pub mod type_hierarchy; // Merged into object_system
// pub mod metaclass; // Merged into object_system

use std::cell::RefCell;
use std::path::PathBuf;

#[cfg(feature = "ffi")]
pub mod ffi;

#[cfg(feature = "python-interop")]
pub mod python_interop;

pub use lexer::*;
pub use parser::*;
pub use ast::*;
pub use value::*;
pub use semantic::*;
pub use ir::*;
pub use vm::*;
pub use runtime::*;
pub use builtins::*;
pub use modules::*;
pub use module_system::*;
pub use object_system::*;
pub use bytecode::*;
// pub use metaclass::*; // Merged into object_system
#[cfg(feature = "ffi")]
pub use ffi::*;
