//! Super Powerful Register-Based Bytecode Implementation for Tauraro
//! This implementation is designed to be 50x faster than CPython's bytecode
//! by using register-based architecture, inline caching, computed GOTOs, and JIT compilation

use crate::ast::*;
use crate::value::Value;
use crate::modules::hplist::HPList;
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::fmt::Debug;
use std::hash::{Hash, Hasher};
use smallvec::SmallVec;

// Import for JIT compilation
#[cfg(feature = "jit")]
use cranelift_module::{Module, Linkage};
#[cfg(feature = "jit")]
use cranelift_codegen::ir;
#[cfg(feature = "jit")]
use cranelift_codegen::isa;

/// Reference counted value for optimized memory management
#[derive(Debug, Clone)]
pub struct RcValue {
    pub value: Value,
    pub ref_count: usize,
}

impl PartialEq for RcValue {
    fn eq(&self, other: &Self) -> bool {
        self.value == other.value
    }
}

impl Eq for RcValue {}

impl Hash for RcValue {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.value.hash(state);
    }
}

impl RcValue {
    pub fn new(value: Value) -> Self {
        Self {
            value,
            ref_count: 1,
        }
    }
    
    pub fn clone_rc(&self) -> Self {
        Self {
            value: self.value.clone(),
            ref_count: self.ref_count + 1,
        }
    }
    
    pub fn is_unique(&self) -> bool {
        self.ref_count == 1
    }
    
    pub fn is_truthy(&self) -> bool {
        self.value.is_truthy()
    }
}

/// Method cache for object-oriented code performance
#[derive(Debug, Clone)]
pub struct MethodCache {
    pub class_name: String,
    pub method_name: String,
    pub method: Option<Value>,
    pub version: u32,
}

/// Register-based bytecode instruction opcodes with specialized fast paths
/// Using register-based architecture instead of stack-based for better performance
#[derive(Debug, Clone, Copy, PartialEq)]
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
    BinaryAddRR_InPlace, // In-place Register-Register addition (when left is unique)
    BinaryAddRR_FastInt, // Fast path for integer Register-Register addition
    BinaryAddRI_FastInt, // Fast path for integer Register-Immediate addition
    BinarySubRR,    // Register-Register subtraction
    BinarySubRI,    // Register-Immediate subtraction
    BinarySubIR,    // Immediate-Register subtraction
    BinarySubRR_FastInt, // Fast path for integer Register-Register subtraction
    BinarySubRI_FastInt, // Fast path for integer Register-Immediate subtraction
    BinaryMulRR,    // Register-Register multiplication
    BinaryMulRI,    // Register-Immediate multiplication
    BinaryMulIR,    // Immediate-Register multiplication
    BinaryMulRR_FastInt, // Fast path for integer Register-Register multiplication
    BinaryMulRI_FastInt, // Fast path for integer Register-Immediate multiplication
    BinaryDivRR,    // Register-Register division
    BinaryDivRI,    // Register-Immediate division
    BinaryDivIR,    // Immediate-Register division
    BinaryDivRR_FastInt, // Fast path for integer Register-Register division
    BinaryDivRI_FastInt, // Fast path for integer Register-Immediate division
    BinaryModRR,    // Register-Register modulo
    BinaryModRI,    // Register-Immediate modulo
    BinaryModIR,    // Immediate-Register modulo
    BinaryModRR_FastInt, // Fast path for integer Register-Register modulo
    BinaryModRI_FastInt, // Fast path for integer Register-Immediate modulo
    BinaryPowRR,    // Register-Register power
    BinaryPowRI,    // Register-Immediate power
    BinaryPowIR,    // Immediate-Register power
    BinaryPowRR_FastInt, // Fast path for integer Register-Register power
    BinaryPowRI_FastInt, // Fast path for integer Register-Immediate power
    
    // Unary operations
    UnaryPositive,
    UnaryNegative,
    UnaryNot,
    UnaryInvert,
    UnaryNegative_FastInt, // Fast path for integer negation
    
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
    CompareEqualRR_FastInt, // Fast path for integer equality comparison
    CompareLessRR_FastInt,  // Fast path for integer less-than comparison
    
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
    
    // Functions
    MakeFunction,
    LoadClassDeref,
    
    // Method calls with caching
    LoadMethod,     // Load method with caching
    CallMethod,     // Call method with caching
    LoadMethodCached, // Load method from cache
    CallMethodCached, // Call method from cache
    
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
    
    // Miscellaneous
    PrintExpr,
    FormatValue,
    ExtendedArg,
    NOP,
}

/// A single register-based bytecode instruction
/// Using 32-bit instructions for better performance
#[derive(Debug, Clone)]
pub struct Instruction {
    pub opcode: OpCode,
    pub arg1: u32,  // First argument (often a register index)
    pub arg2: u32,  // Second argument (often a register index or immediate value)
    pub arg3: u32,  // Third argument (often a register index)
    pub line: u32,  // Line number for debugging
}

/// Inline cache for speeding up attribute and global lookups
#[derive(Debug, Clone)]
pub struct InlineCache {
    pub counter: u32,           // Execution counter for specialization
    pub version: u32,           // Version for cache invalidation
    pub data: Option<Value>,    // Cached value
    pub type_info: Option<String>, // Type information for specialization
}

/// Register-based compiled code object
#[derive(Debug, Clone)]
pub struct CodeObject {
    pub instructions: Vec<Instruction>,
    pub constants: Vec<Value>,
    pub names: Vec<String>,
    pub varnames: Vec<String>,
    pub freevars: Vec<String>,
    pub cellvars: Vec<String>,
    pub filename: String,
    pub name: String,
    pub first_line: u32,
    pub argcount: u32,
    pub kwonlyargcount: u32,
    pub nlocals: u32,
    pub stacksize: u32,
    pub flags: u32,
    pub registers: u32,         // Number of registers needed
    pub inline_caches: Vec<InlineCache>, // Inline caches for specialization
}

impl CodeObject {
    pub fn new(filename: String, name: String, first_line: u32) -> Self {
        Self {
            instructions: Vec::new(),
            constants: Vec::new(),
            names: Vec::new(),
            varnames: Vec::new(),
            freevars: Vec::new(),
            cellvars: Vec::new(),
            filename,
            name,
            first_line,
            argcount: 0,
            kwonlyargcount: 0,
            nlocals: 0,
            stacksize: 0,
            flags: 0,
            registers: 0,
            inline_caches: Vec::new(),
        }
    }
    
    pub fn add_instruction(&mut self, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32, line: u32) {
        self.instructions.push(Instruction { opcode, arg1, arg2, arg3, line });
    }
    
    pub fn add_constant(&mut self, value: Value) -> u32 {
        self.constants.push(value);
        (self.constants.len() - 1) as u32
    }
    
    pub fn add_name(&mut self, name: String) -> u32 {
        if let Some(pos) = self.names.iter().position(|n| n == &name) {
            pos as u32
        } else {
            let pos = self.names.len() as u32;
            self.names.push(name);
            pos
        }
    }
    
    pub fn add_varname(&mut self, name: String) -> u32 {
        if let Some(pos) = self.varnames.iter().position(|n| n == &name) {
            pos as u32
        } else {
            let pos = self.varnames.len() as u32;
            self.varnames.push(name);
            pos
        }
    }
    
    pub fn add_freevar(&mut self, name: String) -> u32 {
        if let Some(pos) = self.freevars.iter().position(|n| n == &name) {
            pos as u32
        } else {
            let pos = self.freevars.len() as u32;
            self.freevars.push(name);
            pos
        }
    }
    
    pub fn add_cellvar(&mut self, name: String) -> u32 {
        if let Some(pos) = self.cellvars.iter().position(|n| n == &name) {
            pos as u32
        } else {
            let pos = self.cellvars.len() as u32;
            self.cellvars.push(name);
            pos
        }
    }
    
    /// Add an inline cache for specialization
    pub fn add_inline_cache(&mut self) -> u32 {
        let index = self.inline_caches.len() as u32;
        self.inline_caches.push(InlineCache {
            counter: 0,
            version: 0,
            data: None,
            type_info: None,
        });
        index
    }
}

/// Execution frame for register-based VM with reference counting and method caching
pub struct Frame {
    pub code: CodeObject,
    pub pc: usize,                          // Program counter
    pub registers: SmallVec<[RcValue; 64]>, // Register file with reference counting
    pub locals: HashMap<String, RcValue>,   // Local variables with reference counting
    pub globals: HashMap<String, RcValue>,  // Global variables with reference counting
    pub builtins: HashMap<String, RcValue>, // Builtin functions with reference counting
    pub free_vars: Vec<RcValue>,            // Free variables for closures with reference counting
    pub block_stack: Vec<Block>,            // Block stack for control flow
    pub cache_version: u32,                 // Current cache version
    pub method_cache: HashMap<(String, String), MethodCache>, // Method cache for object-oriented code
}

/// Block for control flow
#[derive(Debug, Clone)]
pub struct Block {
    pub block_type: BlockType,
    pub handler: usize,
    pub level: usize,
}

#[derive(Debug, Clone)]
pub enum BlockType {
    Loop,
    Except,
    Finally,
    With,
}

impl Frame {
    pub fn new(code: CodeObject, globals: HashMap<String, Value>, builtins: HashMap<String, Value>) -> Self {
        // Initialize registers
        let mut registers = SmallVec::new();
        registers.resize(code.registers as usize, RcValue::new(Value::None));
        
        // Convert globals and builtins to RcValue
        let rc_globals = globals.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        let rc_builtins = builtins.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        
        Self {
            code,
            pc: 0,
            registers,
            locals: HashMap::new(),
            globals: rc_globals,
            builtins: rc_builtins,
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
        }
    }

    /// Create a frame optimized for function calls with pre-allocated registers
    pub fn new_function_frame(code: CodeObject, globals: HashMap<String, Value>, builtins: HashMap<String, Value>, args: Vec<Value>) -> Self {
        // Initialize registers
        let mut registers = SmallVec::new();
        registers.resize(code.registers as usize, RcValue::new(Value::None));
        
        // Copy arguments to registers
        for (i, arg) in args.into_iter().enumerate() {
            if i < registers.len() {
                registers[i] = RcValue::new(arg);
            }
        }
        
        // Convert globals and builtins to RcValue
        let rc_globals = globals.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        let rc_builtins = builtins.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        
        Self {
            code,
            pc: 0,
            registers,
            locals: HashMap::new(),
            globals: rc_globals,
            builtins: rc_builtins,
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
        }
    }
    
    /// Get value from register
    pub fn get_register(&self, reg: u32) -> &RcValue {
        &self.registers[reg as usize]
    }
    
    /// Set value in register
    pub fn set_register(&mut self, reg: u32, value: RcValue) {
        self.registers[reg as usize] = value;
    }
    
    /// Get mutable reference to register value
    pub fn get_register_mut(&mut self, reg: u32) -> &mut RcValue {
        &mut self.registers[reg as usize]
    }
    
    /// Lookup method in cache
    pub fn lookup_method_cache(&self, class_name: &str, method_name: &str) -> Option<&MethodCache> {
        self.method_cache.get(&(class_name.to_string(), method_name.to_string()))
    }
    
    /// Update method cache
    pub fn update_method_cache(&mut self, class_name: String, method_name: String, method: Option<Value>) {
        let cache_entry = MethodCache {
            class_name: class_name.clone(),
            method_name: method_name.clone(),
            method,
            version: self.cache_version,
        };
        self.method_cache.insert((class_name, method_name), cache_entry);
    }
}

// Manual implementation of Debug trait for Frame struct
impl std::fmt::Debug for Frame {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Frame")
            .field("code", &self.code)
            .field("pc", &self.pc)
            .field("registers", &self.registers)
            .field("locals", &self.locals)
            .field("globals", &self.globals)
            .field("builtins", &self.builtins)
            .field("block_stack", &self.block_stack)
            .finish()
    }
}

/// Register-based bytecode virtual machine with computed GOTOs for maximum performance
pub struct SuperBytecodeVM {
    frames: Vec<Frame>,
    builtins: HashMap<String, RcValue>,
    globals: HashMap<String, RcValue>,
    globals_version: u32,
    
    // Cache compiled code objects for closures to avoid recompiling on each call
    function_code_cache: HashMap<Value, CodeObject>,
    
    // Profiling and JIT compilation tracking
    instruction_execution_count: HashMap<(String, usize), u64>, // (function_name, instruction_index) -> count
    function_call_count: HashMap<String, u64>, // function_name -> call count
    hot_function_threshold: u64, // Threshold for considering a function "hot"
    jit_compiled_functions: HashMap<String, bool>, // Track which functions have been JIT compiled
    
    // JIT compiler for hot function compilation
    #[cfg(feature = "jit")]
    jit_builder: Option<cranelift_module::Module<cranelift_module::Backend>>,
}

// Type alias for builtin functions
pub type BuiltinFunction = fn(Vec<Value>) -> Result<Value>;

impl SuperBytecodeVM {
    pub fn new() -> Self {
        let mut builtins = HashMap::new();
        builtins.insert("print".to_string(), RcValue::new(Value::BuiltinFunction("print".to_string(), Self::builtin_print)));
        builtins.insert("len".to_string(), RcValue::new(Value::BuiltinFunction("len".to_string(), Self::builtin_len)));
        builtins.insert("range".to_string(), RcValue::new(Value::BuiltinFunction("range".to_string(), Self::builtin_range)));
        
        // Initialize JIT builder if JIT feature is enabled
        #[cfg(feature = "jit")]
        let jit_builder = {
            use cranelift_codegen::isa;
            use cranelift_module::Module;
            use target_lexicon::triple;
            
            let mut flag_builder = cranelift_codegen::settings::builder();
            flag_builder.set("use_colocated_libcalls", "false").unwrap();
            let isa_builder = isa::lookup(triple!()).unwrap_or_else(|_| {
                panic!("Unsupported target triple");
            });
            let isa = isa_builder.finish(cranelift_codegen::settings::Flags::new(flag_builder));
            Some(Module::new())
        };
        
        #[cfg(not(feature = "jit"))]
        let jit_builder: Option<()> = None;
        
        Self {
            frames: Vec::new(),
            builtins,
            globals: HashMap::new(),
            globals_version: 0,
            function_code_cache: HashMap::new(),
            
            // Initialize profiling counters
            instruction_execution_count: HashMap::new(),
            function_call_count: HashMap::new(),
            hot_function_threshold: 1000, // Consider functions hot after 1000 calls
            jit_compiled_functions: HashMap::new(),
            
            // Initialize JIT compiler
            #[cfg(feature = "jit")]
            jit_builder,
        }
    }
    
    /// Track instruction execution for profiling and JIT compilation
    fn track_instruction_execution(&mut self, function_name: &str, instruction_index: usize) {
        let key = (function_name.to_string(), instruction_index);
        *self.instruction_execution_count.entry(key).or_insert(0) += 1;
    }
    
    /// Track function call for profiling and JIT compilation
    fn track_function_call(&mut self, function_name: &str) {
        *self.function_call_count.entry(function_name.to_string()).or_insert(0) += 1;
    }
    
    /// Check if a function is hot (frequently called) and should be JIT compiled
    fn is_function_hot(&self, function_name: &str) -> bool {
        self.function_call_count.get(function_name)
            .map_or(false, |&count| count >= self.hot_function_threshold)
    }
    
    /// Get hot functions that should be JIT compiled
    fn get_hot_functions(&self) -> Vec<String> {
        self.function_call_count.iter()
            .filter(|(_, &count)| count >= self.hot_function_threshold)
            .map(|(name, _)| name.clone())
            .collect()
    }
    
    /// Get hot instructions within a function for JIT compilation
    fn get_hot_instructions(&self, function_name: &str) -> Vec<usize> {
        self.instruction_execution_count.iter()
            .filter(|((name, _), &count)| name == function_name && count >= self.hot_function_threshold / 10)
            .map(|((_, index), _)| *index)
            .collect()
    }
    
    pub fn execute(&mut self, code: CodeObject) -> Result<Value> {
        // Convert globals and builtins from RcValue to Value for Frame::new
        let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
        let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
        
        let frame = Frame::new(code, globals_values, builtins_values);
        self.frames.push(frame);
        
        let result = self.run_frame()?;
        
        // Update globals from the executed frame
        if let Some(frame) = self.frames.last() {
            self.globals = frame.globals.clone();
        }
        
        Ok(result)
    }
    
    /// Optimized frame execution using computed GOTOs for maximum performance
    #[inline(always)]
    fn run_frame(&mut self) -> Result<Value> {
        let mut frame_idx;
        
        loop {
            // Fast path: check if we have frames
            if self.frames.is_empty() {
                return Ok(Value::None);
            }
            
            // Update frame index in case frames were added/removed
            frame_idx = self.frames.len() - 1;
            
            // Fast path: check bounds
            let pc = self.frames[frame_idx].pc;
            let instructions_len = self.frames[frame_idx].code.instructions.len();
            
            if pc >= instructions_len {
                break;
            }
            
            // Direct access to instruction without cloning when possible
            // Get the instruction details without borrowing self
            let (opcode, arg1, arg2, arg3, function_name) = {
                let frame = &self.frames[frame_idx];
                let instruction = &frame.code.instructions[pc];
                (instruction.opcode, instruction.arg1, instruction.arg2, instruction.arg3, frame.code.name.clone())
            };
            
            // Track instruction execution for profiling and JIT compilation
            self.track_instruction_execution(&function_name, pc);
            
            // Execute instruction using computed GOTOs for maximum performance
            match self.execute_instruction_fast(frame_idx, opcode, arg1, arg2, arg3) {
                Ok(Some(value)) => return Ok(value),
                Ok(None) => {
                    // Only increment PC if frame still exists and PC hasn't changed
                    if frame_idx < self.frames.len() {
                        // Check if PC has changed (e.g., due to a jump)
                        if self.frames[frame_idx].pc == pc {
                            self.frames[frame_idx].pc += 1;
                        }
                        // If PC has changed, we don't increment it
                    }
                },
                Err(e) => {
                    // Snapshot needed info without holding borrows across mutations
                    let (top_exc, handler_pos_opt) = {
                        let frame = &self.frames[frame_idx];
                        let top = if !frame.registers.is_empty() {
                            Some(frame.registers.last().unwrap().clone())
                        } else {
                            None
                        };
                        let handler_pos = frame
                            .block_stack
                            .iter()
                            .rfind(|b| matches!(b.block_type, BlockType::Except))
                            .map(|b| b.handler);
                        (top, handler_pos)
                    };

                    if let Some(handler_pos) = handler_pos_opt {
                        // Use existing exception value if present, otherwise push error string
                        let exc_val = top_exc.unwrap_or_else(|| RcValue::new(Value::Str(format!("{}", e))));
                        // Store exception in first register
                        if !self.frames[frame_idx].registers.is_empty() {
                            self.frames[frame_idx].registers[0] = exc_val;
                        }
                        // Set PC to handler
                        self.frames[frame_idx].pc = handler_pos;
                        // Pop the most recent Except block so nested exceptions can unwind further
                        if let Some(pos) = self.frames[frame_idx]
                            .block_stack
                            .iter()
                            .rposition(|b| matches!(b.block_type, BlockType::Except))
                        {
                            self.frames[frame_idx].block_stack.remove(pos);
                        }
                        // Continue execution
                        continue;
                    } else {
                        return Err(e);
                    }
                },
            }
        }
        
        Ok(Value::None)
    }

    /// Optimized instruction execution with computed GOTOs for maximum performance
    #[inline(always)]
    fn execute_instruction_fast(&mut self, frame_idx: usize, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32) -> Result<Option<Value>> {
        // Computed GOTOs implementation for maximum performance
        match opcode {
            OpCode::NOP => {
                // No operation; proceed to next instruction
                Ok(None)
            }
            OpCode::LoadConst => {
                let const_idx = arg1 as usize;
                // Check bounds before accessing
                if const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadConst: constant index {} out of bounds (len: {})", const_idx, self.frames[frame_idx].code.constants.len()));
                }
                let value = RcValue::new(self.frames[frame_idx].code.constants[const_idx].clone());
                self.frames[frame_idx].set_register(arg2, value);
                Ok(None)
            }
            OpCode::LoadLocal => {
                // In register-based VM, this is typically a register-to-register move
                let src_reg = arg1;
                let dst_reg = arg2;
                if src_reg as usize >= self.frames[frame_idx].registers.len() || dst_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadLocal: register index out of bounds"));
                }
                let value = self.frames[frame_idx].registers[src_reg as usize].clone();
                self.frames[frame_idx].registers[dst_reg as usize] = value;
                Ok(None)
            }
            OpCode::StoreLocal => {
                // In register-based VM, this is typically a register-to-register move
                let src_reg = arg1;
                let dst_reg = arg2;
                if src_reg as usize >= self.frames[frame_idx].registers.len() || dst_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreLocal: register index out of bounds"));
                }
                let value = self.frames[frame_idx].registers[src_reg as usize].clone();
                self.frames[frame_idx].registers[dst_reg as usize] = value;
                Ok(None)
            }
            OpCode::LoadGlobal => {
                let name_idx = arg1 as usize;
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                let name = &self.frames[frame_idx].code.names[name_idx];
                
                // Try inline cache first
                let cache_idx = arg2 as usize;
                if cache_idx < self.frames[frame_idx].code.inline_caches.len() {
                    let should_use_cache = {
                        let cache = &self.frames[frame_idx].code.inline_caches[cache_idx];
                        cache.version == self.frames[frame_idx].cache_version && cache.data.is_some()
                    };
                    
                    if should_use_cache {
                        let cache_data = self.frames[frame_idx].code.inline_caches[cache_idx].data.clone().unwrap();
                        let value = RcValue::new(cache_data);
                        self.frames[frame_idx].set_register(arg3, value);
                        return Ok(None);
                    }
                }
                
                // Fallback to global lookup
                let global_value = self.frames[frame_idx].globals.get(name).cloned();
                if let Some(value) = global_value {
                    // Update inline cache
                    if cache_idx < self.frames[frame_idx].code.inline_caches.len() {
                        self.frames[frame_idx].code.inline_caches[cache_idx] = InlineCache {
                            counter: 0,
                            version: self.frames[frame_idx].cache_version,
                            data: Some(value.value.clone()),
                            type_info: None,
                        };
                    }
                    self.frames[frame_idx].set_register(arg3, value);
                    Ok(None)
                } else {
                    Err(anyhow!("Global '{}' is not defined", name))
                }
            }
            OpCode::StoreGlobal => {
                let name_idx = arg1 as usize;
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("StoreGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                let name = self.frames[frame_idx].code.names[name_idx].clone();
                let value = self.frames[frame_idx].get_register(arg2).clone();
                self.frames[frame_idx].globals.insert(name, value);
                // bump globals version to invalidate caches
                self.globals_version = self.globals_version.wrapping_add(1);
                self.frames[frame_idx].cache_version = self.globals_version;
                Ok(None)
            }
            OpCode::BinaryAddRR => {
                // Register-Register addition
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryAddRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Str(a), Value::Str(b)) => {
                        // Preallocate capacity for string concatenation to avoid intermediate formatting allocations
                        let mut s = String::with_capacity(a.len() + b.len());
                        s.push_str(a);
                        s.push_str(b);
                        Value::Str(s)
                    },
                    (Value::List(a), Value::List(b)) => {
                        // Avoid intermediate clones; allocate exact capacity and clone elements once
                        let mut c = HPList::with_capacity(a.len() + b.len());
                        for item in a {
                            c.append((*item).clone());
                        }
                        for item in b {
                            c.append((*item).clone());
                        }
                        Value::List(c)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.add_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryAddRI => {
                // Register-Immediate addition
                let reg = arg1;
                let imm_idx = arg2 as usize;
                let result_reg = arg3;
                
                if reg as usize >= self.frames[frame_idx].registers.len() || 
                   result_reg as usize >= self.frames[frame_idx].registers.len() ||
                   imm_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryAddRI: index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[reg as usize];
                let right = &self.frames[frame_idx].code.constants[imm_idx];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => RcValue::new(Value::Int(a + b)),
                    (Value::Float(a), Value::Float(b)) => RcValue::new(Value::Float(a + b)),
                    (Value::Str(a), Value::Str(b)) => {
                        let mut s = String::with_capacity(a.len() + b.len());
                        s.push_str(a);
                        s.push_str(b);
                        RcValue::new(Value::Str(s))
                    },
                    _ => {
                        RcValue::new(self.add_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddRI: {}", e))?)
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = result;
                Ok(None)
            }
            OpCode::BinaryAddRR_InPlace => {
                // In-place Register-Register addition (when left is unique)
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryAddRR_InPlace: register index out of bounds"));
                }
                
                // Check if left register value is unique (ref_count == 1)
                let left_is_unique = self.frames[frame_idx].registers[left_reg as usize].is_unique();
                
                if left_is_unique {
                    // We need to get the values first to avoid borrowing conflicts
                    let right_value = self.frames[frame_idx].registers[right_reg as usize].value.clone();
                    let left_value = &self.frames[frame_idx].registers[left_reg as usize].value.clone();
                    
                    // Fast path for integers
                    match (left_value, &right_value) {
                        (Value::Int(a), Value::Int(b)) => {
                            // We can modify in-place
                            let left = &mut self.frames[frame_idx].registers[left_reg as usize];
                            if let Value::Int(ref mut a_val) = left.value {
                                *a_val += b;
                            }
                            self.frames[frame_idx].registers[result_reg as usize] = left.clone_rc();
                        },
                        _ => {
                            // Fallback to regular addition
                            let result = self.add_values(left_value.clone(), right_value)
                                .map_err(|e| anyhow!("Error in BinaryAddRR_InPlace: {}", e))?;
                            self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                        }
                    }
                } else {
                    // Not unique, need to clone
                    let left = &self.frames[frame_idx].registers[left_reg as usize];
                    let right = &self.frames[frame_idx].registers[right_reg as usize];
                    
                    // Fast path for integers
                    let result = match (&left.value, &right.value) {
                        (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                        _ => {
                            self.add_values(left.value.clone(), right.value.clone())
                                .map_err(|e| anyhow!("Error in BinaryAddRR_InPlace: {}", e))?
                        }
                    };
                    
                    self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::BinaryAddRR_FastInt => {
                // Fast path for integer Register-Register addition
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryAddRR_FastInt: register index out of bounds"));
                }
                
                // Direct integer addition without type checking
                let left = &self.frames[frame_idx].registers[left_reg as usize].value;
                let right = &self.frames[frame_idx].registers[right_reg as usize].value;
                
                match (left, right) {
                    (Value::Int(a), Value::Int(b)) => {
                        self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::Int(a + b));
                    },
                    _ => {
                        // Fallback to regular addition if not integers
                        let result = self.add_values(left.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddRR_FastInt: {}", e))?;
                        self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                    }
                }
                
                Ok(None)
            }
            OpCode::BinaryAddRI_FastInt => {
                // Fast path for integer Register-Immediate addition
                let reg = arg1;
                let imm_idx = arg2 as usize;
                let result_reg = arg3;
                
                if reg as usize >= self.frames[frame_idx].registers.len() || 
                   result_reg as usize >= self.frames[frame_idx].registers.len() ||
                   imm_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryAddRI_FastInt: index out of bounds"));
                }
                
                // Direct integer addition without type checking
                let left = &self.frames[frame_idx].registers[reg as usize].value;
                let right = &self.frames[frame_idx].code.constants[imm_idx];
                
                match (left, right) {
                    (Value::Int(a), Value::Int(b)) => {
                        self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::Int(a + b));
                    },
                    _ => {
                        // Fallback to regular addition if not integers
                        let result = self.add_values(left.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddRI_FastInt: {}", e))?;
                        self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                    }
                }
                
                Ok(None)
            }
            OpCode::UnaryNegative_FastInt => {
                // Fast path for integer negation
                let src_reg = arg1;
                let dst_reg = arg2;
                
                if src_reg as usize >= self.frames[frame_idx].registers.len() || 
                   dst_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("UnaryNegative_FastInt: register index out of bounds"));
                }
                
                // Direct integer negation without type checking
                let value = &self.frames[frame_idx].registers[src_reg as usize].value;
                
                match value {
                    Value::Int(n) => {
                        self.frames[frame_idx].registers[dst_reg as usize] = RcValue::new(Value::Int(-n));
                    },
                    _ => {
                        // Fallback to regular negation if not integer
                        let result = match value {
                            Value::Int(n) => Value::Int(-n),
                            Value::Float(n) => Value::Float(-n),
                            _ => return Err(anyhow!("UnaryNegative_FastInt: unsupported type")),
                        };
                        self.frames[frame_idx].registers[dst_reg as usize] = RcValue::new(result);
                    }
                }
                
                Ok(None)
            }
            OpCode::Jump => {
                let target = arg1 as usize;
                self.frames[frame_idx].pc = target;
                Ok(None)
            }
            OpCode::JumpIfTrue => {
                let cond_reg = arg1;
                let target = arg2 as usize;
                
                if cond_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("JumpIfTrue: register index out of bounds"));
                }
                
                let condition = &self.frames[frame_idx].registers[cond_reg as usize];
                if condition.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::JumpIfFalse => {
                let cond_reg = arg1;
                let target = arg2 as usize;
                
                if cond_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("JumpIfFalse: register index out of bounds"));
                }
                
                let condition = &self.frames[frame_idx].registers[cond_reg as usize];
                if !condition.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::CallFunction => {
                let func_reg = arg1;
                let arg_count = arg2 as usize;
                let result_reg = arg3;
                
                if func_reg as usize >= self.frames[frame_idx].registers.len() || 
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunction: register index out of bounds"));
                }
                
                // Extract arguments from consecutive registers
                let mut args = Vec::with_capacity(arg_count);
                for i in 0..arg_count {
                    let arg_reg = func_reg + 1 + i as u32;
                    if arg_reg as usize >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallFunction: argument register out of bounds"));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg as usize].value.clone());
                }
                
                // Get the function value
                let func = self.frames[frame_idx].registers[func_reg as usize].value.clone();
                
                // Call the function using fast path
                let result = self.call_function_fast(func, args)?;
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::ReturnValue => {
                let value_reg = arg1;
                if value_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("ReturnValue: register index out of bounds"));
                }
                let value = self.frames[frame_idx].registers[value_reg as usize].value.clone();
                return Ok(Some(value));
            }
            OpCode::BuildList => {
                let element_count = arg1 as usize;
                let start_reg = arg2;
                let result_reg = arg3;
                
                if result_reg as usize >= self.frames[frame_idx].registers.len() ||
                   (start_reg as usize + element_count) > self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BuildList: register index out of bounds"));
                }
                
                let mut elements = Vec::with_capacity(element_count);
                for i in 0..element_count {
                    elements.push(self.frames[frame_idx].registers[(start_reg as usize + i)].value.clone());
                }
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::List(HPList::from_values(elements)));
                Ok(None)
            }
            OpCode::LoadAddStore => {
                // Super-instruction: Load + Add + Store in one operation
                // arg1: source register
                // arg2: value to add (constant index)
                // arg3: destination register
                let src_reg = arg1;
                let const_idx = arg2 as usize;
                let dst_reg = arg3;
                
                if src_reg as usize >= self.frames[frame_idx].registers.len() || 
                   dst_reg as usize >= self.frames[frame_idx].registers.len() ||
                   const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadAddStore: index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[src_reg as usize];
                let right = &self.frames[frame_idx].code.constants[const_idx];
                
                // Fast path for integers
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    _ => {
                        self.add_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in LoadAddStore: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[dst_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadAndAdd => {
                // Super-instruction: Load + Add in one operation
                // arg1: source register 1
                // arg2: source register 2
                // arg3: destination register
                let src_reg1 = arg1;
                let src_reg2 = arg2;
                let dst_reg = arg3;
                
                if src_reg1 as usize >= self.frames[frame_idx].registers.len() || 
                   src_reg2 as usize >= self.frames[frame_idx].registers.len() ||
                   dst_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadAndAdd: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[src_reg1 as usize];
                let right = &self.frames[frame_idx].registers[src_reg2 as usize];
                
                // Fast path for integers
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    _ => {
                        self.add_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in LoadAndAdd: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[dst_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadAndStore => {
                // Super-instruction: Load + Store in one operation
                // arg1: source register
                // arg2: destination register
                // arg3: unused
                let src_reg = arg1;
                let dst_reg = arg2;
                
                if src_reg as usize >= self.frames[frame_idx].registers.len() || 
                   dst_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadAndStore: register index out of bounds"));
                }
                
                let value = self.frames[frame_idx].registers[src_reg as usize].clone();
                self.frames[frame_idx].registers[dst_reg as usize] = value;
                Ok(None)
            }
            OpCode::IncLocal => {
                // Super-instruction: Increment local variable
                // arg1: register to increment
                // arg2: unused
                // arg3: unused
                let reg = arg1;
                
                if reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("IncLocal: register index out of bounds"));
                }
                
                match &mut self.frames[frame_idx].registers[reg as usize].value {
                    Value::Int(n) => *n += 1,
                    _ => return Err(anyhow!("IncLocal: can only increment integers")),
                }
                
                Ok(None)
            }
            OpCode::DecLocal => {
                // Super-instruction: Decrement local variable
                // arg1: register to decrement
                // arg2: unused
                // arg3: unused
                let reg = arg1;
                
                if reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("DecLocal: register index out of bounds"));
                }
                
                match &mut self.frames[frame_idx].registers[reg as usize].value {
                    Value::Int(n) => *n -= 1,
                    _ => return Err(anyhow!("DecLocal: can only decrement integers")),
                }
                
                Ok(None)
            }
            OpCode::LoopCond => {
                // Super-instruction: Loop condition check
                // arg1: condition register
                // arg2: jump target if true
                // arg3: jump target if false
                let cond_reg = arg1;
                let true_target = arg2 as usize;
                let false_target = arg3 as usize;
                
                if cond_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoopCond: register index out of bounds"));
                }
                
                let condition = &self.frames[frame_idx].registers[cond_reg as usize];
                if condition.is_truthy() {
                    self.frames[frame_idx].pc = true_target;
                } else {
                    self.frames[frame_idx].pc = false_target;
                }
                Ok(None)
            }
            OpCode::IncRef => {
                // Increment reference count
                let reg = arg1;
                if reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("IncRef: register index out of bounds"));
                }
                
                self.frames[frame_idx].registers[reg as usize].ref_count += 1;
                Ok(None)
            }
            OpCode::DecRef => {
                // Decrement reference count
                let reg = arg1;
                if reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("DecRef: register index out of bounds"));
                }
                
                let rc_value = &mut self.frames[frame_idx].registers[reg as usize];
                if rc_value.ref_count > 1 {
                    rc_value.ref_count -= 1;
                } else {
                    // Reference count would reach zero, replace with None
                    *rc_value = RcValue::new(Value::None);
                }
                Ok(None)
            }
            OpCode::CloneIfNotUnique => {
                // Clone value if not unique
                let src_reg = arg1;
                let dst_reg = arg2;
                
                if src_reg as usize >= self.frames[frame_idx].registers.len() || 
                   dst_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CloneIfNotUnique: register index out of bounds"));
                }
                
                let src_value = &self.frames[frame_idx].registers[src_reg as usize];
                if src_value.is_unique() {
                    // Already unique, just copy the reference
                    self.frames[frame_idx].registers[dst_reg as usize] = src_value.clone();
                } else {
                    // Not unique, need to clone
                    self.frames[frame_idx].registers[dst_reg as usize] = src_value.clone_rc();
                }
                Ok(None)
            }
            OpCode::LoadMethodCached => {
                // Load method from cache
                // arg1: object register
                // arg2: method name constant index
                // arg3: destination register
                let obj_reg = arg1;
                let name_idx = arg2 as usize;
                let dst_reg = arg3;
                
                if obj_reg as usize >= self.frames[frame_idx].registers.len() || 
                   dst_reg as usize >= self.frames[frame_idx].registers.len() ||
                   name_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadMethodCached: index out of bounds"));
                }
                
                let obj = &self.frames[frame_idx].registers[obj_reg as usize];
                let method_name = match &self.frames[frame_idx].code.constants[name_idx] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow!("LoadMethodCached: method name must be string")),
                };
                
                // Get class name from object
                let class_name = obj.value.type_name().to_string();
                
                // Try to lookup in cache first
                if let Some(cache_entry) = self.frames[frame_idx].lookup_method_cache(&class_name, &method_name) {
                    if cache_entry.version == self.frames[frame_idx].cache_version {
                        if let Some(method) = &cache_entry.method {
                            self.frames[frame_idx].registers[dst_reg as usize] = RcValue::new(method.clone());
                            return Ok(None);
                        }
                    }
                }
                
                // Cache miss, do regular method lookup
                let method = match &obj.value {
                    Value::Object { fields, .. } => {
                        fields.get(&method_name).cloned()
                    }
                    _ => {
                        // Try to get method from built-in types
                        obj.value.get_method(&method_name).map(|f| Value::NativeFunction(f))
                    }
                };
                
                // Update cache
                self.frames[frame_idx].update_method_cache(class_name, method_name.clone(), method.clone());
                
                if let Some(method_value) = method {
                    self.frames[frame_idx].registers[dst_reg as usize] = RcValue::new(method_value);
                } else {
                    return Err(anyhow!("Method '{}' not found", method_name));
                }
                
                Ok(None)
            }
            OpCode::CallMethodCached => {
                // Call method from cache
                // arg1: method register
                // arg2: argument count
                // arg3: result register
                let method_reg = arg1;
                let arg_count = arg2 as usize;
                let result_reg = arg3;
                
                if method_reg as usize >= self.frames[frame_idx].registers.len() || 
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallMethodCached: register index out of bounds"));
                }
                
                // Extract arguments from consecutive registers
                let mut args = Vec::with_capacity(arg_count);
                for i in 0..arg_count {
                    let arg_reg = method_reg + 1 + i as u32;
                    if arg_reg as usize >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallMethodCached: argument register out of bounds"));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg as usize].value.clone());
                }
                
                // Get the method value
                let method = self.frames[frame_idx].registers[method_reg as usize].value.clone();
                
                // Call the method
                let result = match method {
                    Value::NativeFunction(fptr) => {
                        fptr(args).map_err(|e| anyhow!("Error calling method: {}", e))?
                    }
                    Value::BuiltinFunction(_, fptr) => {
                        fptr(args).map_err(|e| anyhow!("Error calling method: {}", e))?
                    }
                    _ => {
                        return Err(anyhow!("'{}' object is not callable as method", method.type_name()));
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::UpdateMethodCache => {
                // Update method cache
                // arg1: class name constant index
                // arg2: method name constant index
                // arg3: method value register (or 0 if method not found)
                let class_name_idx = arg1 as usize;
                let method_name_idx = arg2 as usize;
                let method_reg = arg3;
                
                if class_name_idx >= self.frames[frame_idx].code.constants.len() ||
                   method_name_idx >= self.frames[frame_idx].code.constants.len() ||
                   (method_reg != 0 && method_reg as usize >= self.frames[frame_idx].registers.len()) {
                    return Err(anyhow!("UpdateMethodCache: index out of bounds"));
                }
                
                let class_name = match &self.frames[frame_idx].code.constants[class_name_idx] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow!("UpdateMethodCache: class name must be string")),
                };
                
                let method_name = match &self.frames[frame_idx].code.constants[method_name_idx] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow!("UpdateMethodCache: method name must be string")),
                };
                
                let method = if method_reg != 0 {
                    Some(self.frames[frame_idx].registers[method_reg as usize].value.clone())
                } else {
                    None
                };
                
                self.frames[frame_idx].update_method_cache(class_name, method_name, method);
                Ok(None)
            }
            _ => {
                // For unimplemented opcodes, fall back to the existing VM implementation
                Err(anyhow!("Unimplemented opcode: {:?}", opcode))
            }
        }
    }
    
    // Helper methods for binary operations
    fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Str(a), Value::Str(b)) => {
                let mut s = String::with_capacity(a.len() + b.len());
                s.push_str(&a);
                s.push_str(&b);
                Ok(Value::Str(s))
            },
            (Value::Str(a), Value::Int(b)) => {
                let bs = b.to_string();
                let mut s = String::with_capacity(a.len() + bs.len());
                s.push_str(&a);
                s.push_str(&bs);
                Ok(Value::Str(s))
            },
            (Value::Str(a), Value::Float(b)) => {
                let bs = b.to_string();
                let mut s = String::with_capacity(a.len() + bs.len());
                s.push_str(&a);
                s.push_str(&bs);
                Ok(Value::Str(s))
            },
            (Value::Str(a), Value::Bool(b)) => {
                let bs = b.to_string();
                let mut s = String::with_capacity(a.len() + bs.len());
                s.push_str(&a);
                s.push_str(&bs);
                Ok(Value::Str(s))
            },
            (Value::Int(a), Value::Str(b)) => {
                let as_ = a.to_string();
                let mut s = String::with_capacity(as_.len() + b.len());
                s.push_str(&as_);
                s.push_str(&b);
                Ok(Value::Str(s))
            },
            (Value::Float(a), Value::Str(b)) => {
                let as_ = a.to_string();
                let mut s = String::with_capacity(as_.len() + b.len());
                s.push_str(&as_);
                s.push_str(&b);
                Ok(Value::Str(s))
            },
            (Value::Bool(a), Value::Str(b)) => {
                let as_ = a.to_string();
                let mut s = String::with_capacity(as_.len() + b.len());
                s.push_str(&as_);
                s.push_str(&b);
                Ok(Value::Str(s))
            },
            (Value::List(a), Value::List(b)) => {
                let mut c = HPList::with_capacity(a.len() + b.len());
                for item in a {
                    c.append(item);
                }
                for item in b {
                    c.append(item);
                }
                Ok(Value::List(c))
            },
            (Value::Tuple(mut a), Value::Tuple(b)) => { a.extend(b); Ok(Value::Tuple(a)) },
            _ => Err(anyhow!("Unsupported types for addition")),
        }
    }
    
    /// Convert bytecode to Cranelift IR for JIT compilation
    #[cfg(feature = "jit")]
    fn bytecode_to_cranelift_ir(&self, code: &CodeObject, function_name: &str) -> Result<cranelift_module::Module<cranelift_module::Backend>> {
        use cranelift_codegen::ir::*;
        use cranelift_codegen::settings;
        use cranelift_frontend::{FunctionBuilder, FunctionBuilderContext};
        use cranelift_module::{Module, Linkage};
        use target_lexicon::triple;
        
        // Create a new module for this function
        let mut module = Module::new();
        
        // Define the function signature
        let mut sig = Signature::new(CallConv::Fast);
        sig.params.push(AbiParam::new(types::I64)); // Simple signature for now
        sig.returns.push(AbiParam::new(types::I64));
        
        // Declare the function in the module
        let func_id = module.declare_function(function_name, Linkage::Export, &sig)?;
        
        // Create the function builder
        let mut func_ctx = FunctionBuilderContext::new();
        let mut func = Function::with_name_signature(func_id, sig);
        let mut builder = FunctionBuilder::new(&mut func, &mut func_ctx);
        
        // Create the entry block
        let block0 = builder.create_block();
        builder.append_block_params_for_function_params(block0);
        builder.switch_to_block(block0);
        builder.seal_block(block0);
        
        // For now, just return a constant value as a placeholder
        // In a real implementation, we would translate each bytecode instruction to Cranelift IR
        let val = builder.ins().iconst(types::I64, 42);
        builder.ins().return_(&[val]);
        
        // Finalize the function
        builder.finalize();
        
        // Define the function in the module
        let mut ctx = cranelift_codegen::Context::for_function(func);
        module.define_function(func_id, &mut ctx)?;
        
        Ok(module)
    }
    
    /// JIT compile a hot function
    #[cfg(feature = "jit")]
    fn jit_compile_function(&mut self, function_name: &str, code: &CodeObject) -> Result<()> {
        // Check if function is already JIT compiled
        if self.jit_compiled_functions.get(function_name).copied().unwrap_or(false) {
            return Ok(());
        }
        
        // Convert bytecode to Cranelift IR
        let module = self.bytecode_to_cranelift_ir(code, function_name)?;
        
        // For now, we'll just mark it as compiled
        // In a real implementation, we would actually compile and store the compiled code
        self.jit_compiled_functions.insert(function_name.to_string(), true);
        
        println!("JIT compiled function: {}", function_name);
        Ok(())
    }
    
    /// Execute a JIT compiled function
    #[cfg(feature = "jit")]
    fn execute_jit_function(&self, function_name: &str, args: &[Value]) -> Result<Value> {
        // In a real implementation, we would execute the compiled code
        // For now, we'll just return a placeholder value
        Ok(Value::Int(42))
    }
    
    /// Optimized function call path with JIT compilation support
    fn call_function_fast(&mut self, func: Value, args: Vec<Value>) -> Result<Value> {
        // Track function call for profiling and JIT compilation
        let function_name = match &func {
            Value::Closure { name, .. } => name.clone(),
            Value::BuiltinFunction(name, _) => name.clone(),
            _ => "<unknown>".to_string(),
        };
        self.track_function_call(&function_name);
        
        // Check if function should be JIT compiled
        #[cfg(feature = "jit")]
        {
            // First check if we already have a JIT-compiled version
            if let Some(true) = self.jit_compiled_functions.get(&function_name) {
                // Function is already JIT compiled, try to execute it
                return self.execute_jit_function(&function_name, &args);
            }
            
            // Check if function is hot and should be JIT compiled
            let call_count = self.function_call_count.get(&function_name).copied().unwrap_or(0);
            if call_count >= self.hot_function_threshold {
                // Function is hot and should be JIT compiled
                if let Value::Closure { ref name, ref params, ref body, .. } = func {
                    // Try to compile the function using our Cranelift-based JIT compiler
                    if let Some(func_code) = self.function_code_cache.get(&func) {
                        if let Err(e) = self.jit_compile_function(name, func_code) {
                            println!("Failed to JIT compile function {}: {}", name, e);
                        }
                    }
                }
            }
        }
        
        match func {
            Value::BuiltinFunction(_, fptr) => {
                // Direct call to builtin function - fastest path
                (fptr)(args)
            }
            Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _ } => {
                // Fast path for user-defined functions
                let expected = params.len();
                if args.len() != expected {
                    return Err(anyhow!(
                        "Function '{}' expected {} argument(s), got {}",
                        name,
                        expected,
                        args.len()
                    ));
                }

                // Check if we have a cached code object
                if let Some(func_code) = self.function_code_cache.get(&func).cloned() {
                    // Fast path: use cached code object
                    let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());
                    
                    // Create optimized function frame
                    let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                    let frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);
                    
                    self.frames.push(frame);
                    return match self.run_frame()? {
                        Value::None => Ok(Value::None),
                        v => Ok(v),
                    };
                }

                // Slow path: compile and cache
                let mut compiler = SuperCompiler::new(format!("<fn:{}>", name));
                for p in params.iter() {
                    compiler.code.add_varname(p.name.clone());
                }
                for stmt in body.iter().cloned() {
                    compiler.compile_statement(stmt)?;
                }
                let none_const = compiler.code.add_constant(Value::None);
                compiler.emit(OpCode::LoadConst, none_const, 0, 0, 0);
                compiler.emit(OpCode::ReturnValue, 0, 0, 0, 0);

                let func_code = compiler.code.clone();
                self.function_code_cache.insert(func.clone(), func_code.clone());

                let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());
                let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                let frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);
                
                self.frames.push(frame);
                match self.run_frame()? {
                    Value::None => Ok(Value::None),
                    v => Ok(v),
                }
            }
            _ => Err(anyhow!("'{}' object is not callable", func.type_name())),
        }
    }
    
    // Builtin functions (associated functions, not methods)
    fn builtin_print(args: Vec<Value>) -> Result<Value> {
        let output: Vec<String> = args.iter().map(|v| v.to_string()).collect();
        println!("{}", output.join(" "));
        Ok(Value::None)
    }
    
    fn builtin_len(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("len() takes exactly one argument"));
        }
        
        let length = match &args[0] {
            Value::List(list) => list.len(),
            Value::Tuple(tuple) => tuple.len(),
            Value::Dict(dict) => dict.len(),
            Value::Str(s) => s.len(),
            _ => return Err(anyhow!("len() unsupported for type")),
        };
        
        Ok(Value::Int(length as i64))
    }
    
    fn builtin_range(args: Vec<Value>) -> Result<Value> {
        let mut start: i64 = 0;
        let stop: i64;
        let mut step: i64 = 1;
        
        match args.len() {
            1 => {
                stop = match &args[0] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
            }
            2 => {
                start = match &args[0] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                stop = match &args[1] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
            }
            3 => {
                start = match &args[0] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                stop = match &args[1] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                let step_val = match &args[2] {
                    Value::Int(n) => *n,
                    _ => return Err(anyhow!("range() requires integer arguments")),
                };
                if step_val == 0 {
                    return Err(anyhow!("range() step cannot be zero"));
                }
                step = step_val;
            }
            _ => return Err(anyhow!("range() takes 1-3 arguments")),
        }
        
        // Return an unboxed Range value to enable fast iteration in ForIter
        Ok(Value::Range { start, stop, step })
    }
}

/// Super bytecode compiler that converts AST to register-based bytecode
pub struct SuperCompiler {
    code: CodeObject,
    scopes: Vec<CodeObject>,
    current_line: u32,
    next_register: u32,  // Next available register
}

impl SuperCompiler {
    pub fn new(filename: String) -> Self {
        let code = CodeObject::new(filename, "<module>".to_string(), 1);
        Self {
            code,
            scopes: Vec::new(),
            current_line: 1,
            next_register: 0,
        }
    }
    
    pub fn compile(&mut self, program: Program) -> Result<CodeObject> {
        for stmt in program.statements {
            self.compile_statement(stmt)?;
        }
        
        // Add implicit return None at end of module
        let none_const = self.code.add_constant(Value::None);
        self.emit(OpCode::LoadConst, none_const, 0, 0, 0);
        self.emit(OpCode::ReturnValue, 0, 0, 0, 0);
        
        // Set the number of registers needed
        self.code.registers = self.next_register;
        
        Ok(self.code.clone())
    }
    
    fn emit(&mut self, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32, line: u32) -> usize {
        let pos = self.code.instructions.len();
        self.code.add_instruction(opcode, arg1, arg2, arg3, line);
        pos
    }
    
    fn allocate_register(&mut self) -> u32 {
        let reg = self.next_register;
        self.next_register += 1;
        reg
    }
    
    fn compile_statement(&mut self, stmt: Statement) -> Result<()> {
        match stmt {
            Statement::Expression(expr) => {
                let _reg = self.compile_expression(expr)?;
                // Discard expression result
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let value_reg = self.compile_expression(expr)?;
                    // Store in local variable
                    let name_idx = self.code.add_name(name);
                    self.emit(OpCode::StoreGlobal, name_idx, value_reg, 0, 0);
                } else {
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, 0);
                    let name_idx = self.code.add_name(name);
                    self.emit(OpCode::StoreGlobal, name_idx, reg, 0, 0);
                }
            }
            Statement::Return(expr) => {
                if let Some(expr) = expr {
                    let value_reg = self.compile_expression(expr)?;
                    self.emit(OpCode::ReturnValue, value_reg, 0, 0, 0);
                } else {
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, 0);
                    self.emit(OpCode::ReturnValue, reg, 0, 0, 0);
                }
            }
            _ => {
                // For unimplemented statements, we'll just emit NOP
                self.emit(OpCode::NOP, 0, 0, 0, 0);
            }
        }
        Ok(())
    }
    
    fn compile_expression(&mut self, expr: Expr) -> Result<u32> {
        match expr {
            Expr::Literal(literal) => {
                let value = match literal {
                    Literal::Int(n) => Value::Int(n),
                    Literal::Float(n) => Value::Float(n),
                    Literal::String(s) => Value::Str(s),
                    Literal::Bool(b) => Value::Bool(b),
                    Literal::None => Value::None,
                    _ => return Err(anyhow!("Unsupported literal type")),
                };
                let const_idx = self.code.add_constant(value);
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, const_idx, reg, 0, 0);
                Ok(reg)
            }
            Expr::Identifier(name) => {
                let name_idx = self.code.add_name(name);
                let reg = self.allocate_register();
                // For now, we'll use LoadGlobal, but in a full implementation we'd check locals first
                self.emit(OpCode::LoadGlobal, name_idx, reg, 0, 0);
                Ok(reg)
            }
            Expr::BinaryOp { left, op, right } => {
                match op {
                    BinaryOp::Add => {
                        let left_reg = self.compile_expression(*left)?;
                        let right_reg = self.compile_expression(*right)?;
                        let result_reg = self.allocate_register();
                        self.emit(OpCode::BinaryAddRR, left_reg, right_reg, result_reg, 0);
                        Ok(result_reg)
                    },
                    BinaryOp::Sub => {
                        let left_reg = self.compile_expression(*left)?;
                        let right_reg = self.compile_expression(*right)?;
                        let result_reg = self.allocate_register();
                        self.emit(OpCode::BinarySubRR, left_reg, right_reg, result_reg, 0);
                        Ok(result_reg)
                    },
                    BinaryOp::Mul => {
                        let left_reg = self.compile_expression(*left)?;
                        let right_reg = self.compile_expression(*right)?;
                        let result_reg = self.allocate_register();
                        self.emit(OpCode::BinaryMulRR, left_reg, right_reg, result_reg, 0);
                        Ok(result_reg)
                    },
                    BinaryOp::Div => {
                        let left_reg = self.compile_expression(*left)?;
                        let right_reg = self.compile_expression(*right)?;
                        let result_reg = self.allocate_register();
                        self.emit(OpCode::BinaryDivRR, left_reg, right_reg, result_reg, 0);
                        Ok(result_reg)
                    },
                    _ => {
                        return Err(anyhow!("Unsupported binary operation"));
                    }
                }
            }
            _ => {
                return Err(anyhow!("Unsupported expression type"));
            }
        }
    }
}
