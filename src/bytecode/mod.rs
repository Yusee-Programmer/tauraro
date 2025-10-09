//! Central entry point for the bytecode module
//! Re-exports everything from the submodules

// Re-export all submodules
pub mod instructions;
pub mod arithmetic;
pub mod control;
pub mod functions;
pub mod objects;
pub mod memory;
pub mod io;
pub mod module;
pub mod vm;
pub mod values;
pub mod builtins;
pub mod compiler;

// Re-export commonly used items
pub use crate::bytecode::instructions::{OpCode, Instruction};
pub use crate::bytecode::vm::{RcValue, CodeObject, Frame, SuperBytecodeVM};
pub use crate::bytecode::compiler::SuperCompiler;