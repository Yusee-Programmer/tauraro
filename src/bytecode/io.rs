//! Input/output related bytecodes (PRINT, READ, etc.)

/// I/O operations
pub struct IoOps;

// I/O-related opcodes are already defined in instructions.rs
// The implementation for these opcodes is in arithmetic.rs in the execute_instruction_fast method

// The builtin_print function in arithmetic.rs already implements basic print functionality