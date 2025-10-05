//! Central entry point for the bytecode module
//! Re-exports everything from the submodules

// Re-export all submodules
pub mod instructions;
pub mod arithmetic;
pub mod control;
pub mod objects;
pub mod memory;
pub mod io;
pub mod module;
pub mod debug;

// Re-export commonly used items
pub use crate::bytecode::instructions::{OpCode, Instruction};
pub use crate::bytecode::arithmetic::{RcValue, CodeObject, Frame, SuperBytecodeVM, SuperCompiler};