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
    StoreFast,      // Store to fast local variable (indexed access)
    
    // Stack manipulation (for compatibility)
    PopTop,
    RotTwo,
    RotThree,
    DupTop,
    DupTopTwo,
    
    // Function calls
    CallFunction,
    CallFunctionKw,
    CallFunctionEx,
    ReturnValue,
    YieldValue,
    YieldFrom,
    
    // Binary operations (register-based) with reference counting optimizations
    BinaryAddRR,    // Register-Register addition
    BinaryAddRI,    // Register-Immediate addition
    BinaryAddIR,    // Immediate-Register addition
    BinaryAddRRInPlace, // In-place Register-Register addition (when left is unique)
    BinaryAddRRFastInt, // Fast path for integer Register-Register addition
    BinaryAddRIFastInt, // Fast path for integer Register-Immediate addition
    BinaryAddRRFast,    // Fast path for Register-Register addition
    BinarySubRR,    // Register-Register subtraction
    BinarySubRI,    // Register-Immediate subtraction
    BinarySubIR,    // Immediate-Register subtraction
    BinarySubRRFastInt, // Fast path for integer Register-Register subtraction
    BinarySubRIFastInt, // Fast path for integer Register-Immediate subtraction
    BinarySubRRFast,    // Fast path for Register-Register subtraction
    BinaryMulRR,    // Register-Register multiplication
    BinaryMulRI,    // Register-Immediate multiplication
    BinaryMulIR,    // Immediate-Register multiplication
    BinaryMulRRFastInt, // Fast path for integer Register-Register multiplication
    BinaryMulRIFastInt, // Fast path for integer Register-Immediate multiplication
    BinaryMulRRFast,    // Fast path for Register-Register multiplication
    BinaryDivRR,    // Register-Register division
    BinaryDivRI,    // Register-Immediate division
    BinaryDivIR,    // Immediate-Register division
    BinaryDivRRFastInt, // Fast path for integer Register-Register division
    BinaryDivRIFastInt, // Fast path for integer Register-Immediate division
    BinaryModRR,    // Register-Register modulo
    BinaryModRI,    // Register-Immediate modulo
    BinaryModIR,    // Immediate-Register modulo
    BinaryModRRFastInt, // Fast path for integer Register-Register modulo
    BinaryModRIFastInt, // Fast path for integer Register-Immediate modulo
    BinaryPowRR,    // Register-Register power
    BinaryPowRI,    // Register-Immediate power
    BinaryPowIR,    // Immediate-Register power
    BinaryPowRRFastInt, // Fast path for integer Register-Register power
    BinaryPowRIFastInt, // Fast path for integer Register-Immediate power
    BinaryPowRRFast,    // Fast path for Register-Register power
    
    // Unary operations
    UnaryPositive,
    UnaryNegative,
    UnaryNot,
    UnaryInvert,
    UnaryNegativeFastInt, // Fast path for integer negation
    
    // Comparisons
    CompareEqualRR,
    CompareEqualRI,
    CompareNotEqualRR,
    CompareNotEqualRI,
    CompareLessRR,
    CompareLessRI,
    CompareGreaterRR,
    CompareGreaterRI,
    CompareLessEqualRR,
    CompareLessEqualRI,
    CompareGreaterEqualRR,
    CompareGreaterEqualRI,
    CompareEqualRRFastInt, // Fast path for integer equality comparison
    CompareLessRRFastInt,  // Fast path for integer less-than comparison
    
    // Control flow
    Jump,
    JumpIfTrue,
    JumpIfFalse,
    JumpIfNotExhausted,
    PopJumpIfTrue,
    PopJumpIfFalse,
    
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
    
    // Exceptions
    Raise,
    
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
    LoopCond,       // Loop condition check
    
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
    
    // Miscellaneous
    PrintExpr,
    FormatValue,
    ExtendedArg,
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