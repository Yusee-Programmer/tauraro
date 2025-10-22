//! Core bytecode instruction set (opcodes, enums)

use std::fmt::Debug;

/// Register-based bytecode instruction opcodes with specialized fast paths
/// Using register-based architecture instead of stack-based for better performance
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum OpCode {
    // Constants
    LoadConst,
    
    // Variables - register-based versions
    LoadLocal,      // Load from local register
    StoreLocal,     // Store to local register
    LoadGlobal,     // Load from global namespace
    StoreGlobal,    // Store to global namespace
    LoadClosure,    // Load from closure
    StoreClosure,   // Store to closure
    LoadFast,       // Load from fast local variable (indexed access)
    StoreFast,      // Store to fast local variable (indexed access),
    
    // Loop control
    SetupLoop,      // Setup loop block in block stack
    
    // Control flow
    Jump,           // Unconditional jump
    JumpIfTrue,     // Jump if value is true
    JumpIfFalse,    // Jump if value is false
    ReturnValue,    // Return a value from function
    BreakLoop,      // Break out of loop
    ContinueLoop,   // Continue to next loop iteration
    
    // Function calls
    CallFunction,   // Call a function
    CallFunctionKw, // Call a function with keyword arguments
    CallFunctionEx, // Call a function with extended arguments
    
    // Binary operations
    BinaryAddRR,    // Register-Register addition
    BinaryAddRI,    // Register-Immediate addition
    BinaryAddIR,    // Immediate-Register addition
    BinarySubRR,    // Register-Register subtraction
    BinarySubRI,    // Register-Immediate subtraction
    BinarySubIR,    // Immediate-Register subtraction
    BinaryMulRR,    // Register-Register multiplication
    BinaryMulRI,    // Register-Immediate multiplication
    BinaryMulIR,    // Immediate-Register multiplication
    BinaryDivRR,    // Register-Register division
    BinaryDivRI,    // Register-Immediate division
    BinaryDivIR,    // Immediate-Register division
    BinaryModRR,    // Register-Register modulo
    BinaryModRI,    // Register-Immediate modulo
    BinaryModIR,    // Immediate-Register modulo
    BinaryPowRR,    // Register-Register power
    BinaryPowRI,    // Register-Immediate power
    BinaryPowIR,    // Immediate-Register power
    BinaryBitAndRR, // Register-Register bitwise AND
    BinaryBitOrRR,  // Register-Register bitwise OR
    
    // Fast integer operations
    BinaryDivRRFastInt, // Fast integer division
    BinaryModRRFastInt, // Fast integer modulo
    
    // Comparison operations
    CompareEqualRR,     // Register-Register equality comparison
    CompareNotEqualRR,  // Register-Register inequality comparison
    CompareLessRR,      // Register-Register less than comparison
    CompareLessEqualRR, // Register-Register less than or equal comparison
    CompareGreaterRR,   // Register-Register greater than comparison
    CompareGreaterEqualRR, // Register-Register greater than or equal comparison
    
    // Exception handling
    SetupExcept,    // Setup exception handler block
    SetupFinally,   // Setup finally block
    EndFinally,     // End finally block
    PopBlock,       // Pop a block from the block stack
    Raise,          // Raise an exception
    StoreException, // Store exception value in variable (used in except handlers)
    
    // Assertions
    Assert,         // Assert statement
    Match,          // Pattern matching
    MatchKeys,      // Match keys in mapping
    MatchClass,     // Match class pattern
    MatchSequence,  // Match sequence pattern
    MatchMapping,   // Match mapping pattern
    MatchOr,        // Match or pattern
    
    // Generator operations
    YieldValue,     // Yield a value from generator
    YieldFrom,      // Yield from an iterable
    
    // Async operations
    Await,          // Await a coroutine/future
    
    // Data structures
    BuildList,
    BuildTuple,
    BuildDict,
    BuildSet,
    ListAppend,
    SetAdd,
    MapAdd,
    
    // Iteration
    GetIter,
    ForIter,
    
    // Subscript operations
    SubscrLoad,     // Load item from sequence (obj[key])
    SubscrStore,    // Store item to sequence (obj[key] = value)
    SubscrDelete,   // Delete item from sequence (del obj[key])
    
    // Attribute operations
    LoadAttr,       // Load attribute from object (obj.attr)
    StoreAttr,      // Store attribute to object (obj.attr = value)
    DeleteAttr,     // Delete attribute from object (del obj.attr)
    
    // Functions
    MakeFunction,
    LoadClassDeref,
    
    // Method calls with caching
    LoadMethod,     // Load method with caching
    CallMethod,     // Call method with caching
    LoadMethodCached, // Load method from cache
    CallMethodCached, // Call method from cache
    
    // Import operations
    ImportModule,   // Import a module
    ImportFrom,     // Import specific names from a module
    
    // Super-instructions for common patterns
    LoadAddStore,   // Load + Add + Store in one instruction
    LoadMulStore,   // Load + Mul + Store in one instruction
    LoadSubStore,   // Load + Sub + Store in one instruction
    LoadDivStore,   // Load + Div + Store in one instruction
    LoadAndAdd,     // Load + Add in one instruction
    LoadAndMul,     // Load + Mul in one instruction
    LoadAndStore,   // Load + Store in one instruction
    IncLocal,       // Increment local variable
    DecLocal,       // Decrement local variable
    LoopCond,       // Loop condition check,
    
    // Reference counting operations
    IncRef,         // Increment reference count
    DecRef,         // Decrement reference count
    CloneIfNotUnique, // Clone value if not unique
    
    // Method caching operations
    UpdateMethodCache, // Update method cache
    
    // Specialized fast paths
    FastLoop,       // Fast loop implementation
    FastRangeIter,  // Fast range iteration
    FastListAppend, // Fast list append
    FastIntCompare, // Fast integer comparison
    FastIntArithmetic, // Fast integer arithmetic operations
    FastIntAdd,     // Ultra-fast integer addition
    FastIntSub,     // Ultra-fast integer subtraction
    FastIntMul,     // Ultra-fast integer multiplication
    FastIntDiv,     // Ultra-fast integer division
    FastIntMod,     // Ultra-fast integer modulo
    
    // Super() support
    LoadZeroArgSuper, // Load super object with zero arguments (special handling)
    
    // Iterator operations
    Next,           // Call next() on iterator and update iterator variable
    
    // Unary operations
    UnaryNot,       // Logical NOT operation
    UnaryNegate,    // Unary negation (-)

    // Miscellaneous
    PrintExpr,
    FormatValue,
    ExtendedArg,
    WrapKwargs,     // Wrap a dictionary in a KwargsMarker
    NOP,
}

/// A single register-based bytecode instruction
/// Using 32-bit instructions for better performance
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Instruction {
    pub opcode: OpCode,
    pub arg1: u32,  // First argument (often a register index)
    pub arg2: u32,  // Second argument (often a register index or immediate value)
    pub arg3: u32,  // Third argument (often a register index)
    pub line: u32,  // Line number for debugging
}