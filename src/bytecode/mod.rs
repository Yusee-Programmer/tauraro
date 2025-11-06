//! Central entry point for the bytecode module
//! Re-exports everything from the submodules

// Re-export all submodules
pub mod instructions;
pub mod arithmetic;
pub mod control;
pub mod functions;
pub mod objects;
pub mod memory;
pub mod module;
pub mod vm;
pub mod builtins;
pub mod compiler;
pub mod type_checking;
pub mod jit;
pub mod int_cache;
pub mod inline_cache;
pub mod fast_ops;

// Re-export commonly used items
pub use crate::bytecode::instructions::{OpCode, Instruction};
pub use crate::bytecode::objects::RcValue;
pub use crate::bytecode::memory::{CodeObject, Frame};
pub use crate::bytecode::vm::SuperBytecodeVM;
pub use crate::bytecode::compiler::SuperCompiler;
pub use crate::bytecode::jit::{HotLoopDetector, CompiledLoop, JitStats};