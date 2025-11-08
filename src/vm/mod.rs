//! Virtual machine module

pub mod core;
pub mod frame;
pub mod memory;
pub mod memory_management;
pub mod stack;

// Re-export commonly used items
pub use crate::vm::core::VM;
pub use crate::vm::frame::ExecutionFrame;
pub use crate::vm::memory::Scope;
pub use crate::vm::memory_management::{VMMemoryContext, VMMemoryStrategy, ManagedBuffer, VMArena};
pub use crate::vm::stack::StackFrame;