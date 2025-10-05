//! Central entry point for the VM module
//! Re-exports everything from the submodules

// Re-export all submodules
pub mod core;
pub mod frame;
pub mod stack;
pub mod memory;
pub mod objects;
pub mod modules;
pub mod builtins;
pub mod errors;
pub mod debug;

// Re-export commonly used items
pub use crate::vm::core::VM;
pub use crate::vm::frame::ExecutionFrame;
pub use crate::vm::stack::StackFrame;
pub use crate::vm::memory::Scope;