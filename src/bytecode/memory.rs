//! Memory management (alloc, free, refcount, GC hooks)

/// Memory management operations
pub struct MemoryOps;

// Memory-related opcodes are already defined in instructions.rs
// The implementation for these opcodes is in arithmetic.rs in the execute_instruction_fast method