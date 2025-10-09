//! Arithmetic + logical ops (ADD, SUB, MUL, DIV, AND, OR, etc.)

use crate::ast::*;
use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::{OpCode, Instruction};
use anyhow::{Result, anyhow};
use std::collections::{HashMap, HashSet};
use std::fmt::Debug;
use std::hash::{Hash, Hasher};
use smallvec::SmallVec;
use std::cell::RefCell;
use std::rc::Rc;

/// Reference counted value for optimized memory management
#[derive(Debug, Clone)]
pub struct RcValue {
    pub value: Value,
    pub ref_count: usize,
}

/// Simple iterator for Range values
#[derive(Debug, Clone)]
pub struct RangeIterator {
    pub start: i64,
    pub stop: i64,
    pub step: i64,
    pub current: i64,
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

/// Register-based compiled code object
#[derive(Debug, Clone, PartialEq, Eq)]
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

/// Inline cache for speeding up attribute and global lookups
#[derive(Debug, Clone)]
pub struct InlineCache {
    pub counter: u32,           // Execution counter for specialization
    pub version: u32,           // Version for cache invalidation
    pub data: Option<Value>,    // Cached value
    pub type_info: Option<String>, // Type information for specialization
}

impl PartialEq for InlineCache {
    fn eq(&self, other: &Self) -> bool {
        self.counter == other.counter &&
        self.version == other.version &&
        self.type_info == other.type_info
        // We don't compare data because Value may not be Eq
    }
}

impl Eq for InlineCache {}

/// Execution frame for register-based VM with reference counting and method caching
pub struct Frame {
    pub code: CodeObject,
    pub pc: usize,                          // Program counter
    pub registers: SmallVec<[RcValue; 64]>, // Register file with reference counting
    pub locals: Vec<RcValue>,               // Local variables with direct indexing (faster than HashMap)
    pub locals_map: HashMap<String, usize>, // Maps variable names to indices for debugging
    pub globals: HashMap<String, RcValue>,  // Global variables with reference counting
    pub builtins: HashMap<String, RcValue>, // Builtin functions with reference counting
    pub free_vars: Vec<RcValue>,            // Free variables for closures with reference counting
    pub block_stack: Vec<Block>,            // Block stack for control flow
    pub cache_version: u32,                 // Current cache version
    pub method_cache: HashMap<(String, String), MethodCache>, // Method cache for object-oriented code
    pub return_register: Option<(usize, u32)>, // (caller_frame_idx, result_reg) where return value should be stored
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
        // Debug output to see the code object being used
        // eprintln!("DEBUG: Frame::new creating frame with code object '{}' with {} instructions", code.name, code.instructions.len());
        
        // Initialize registers
        let mut registers = SmallVec::new();
        registers.resize(code.registers as usize, RcValue::new(Value::None));
        
        // Initialize locals vector with None values
        let mut locals = Vec::new();
        locals.resize(code.varnames.len(), RcValue::new(Value::None));
        
        // Create mapping from variable names to indices
        let mut locals_map = HashMap::new();
        for (i, name) in code.varnames.iter().enumerate() {
            locals_map.insert(name.clone(), i);
        }
        
        // Convert globals and builtins to RcValue
        let rc_globals = globals.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        let rc_builtins = builtins.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        
        Self {
            code,
            pc: 0,
            registers,
            locals,
            locals_map,
            globals: rc_globals,
            builtins: rc_builtins,
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
            return_register: None,
        }
    }

    /// Create a frame optimized for function calls with pre-allocated registers
    pub fn new_function_frame(code: CodeObject, globals: HashMap<String, Value>, builtins: HashMap<String, Value>, args: Vec<Value>) -> Self {
        // Initialize registers
        let mut registers = SmallVec::new();
        registers.resize(code.registers as usize, RcValue::new(Value::None));
        
        // Initialize locals vector
        let mut locals = Vec::new();
        locals.resize(code.varnames.len(), RcValue::new(Value::None));
        
        // Create mapping from variable names to indices
        let mut locals_map = HashMap::new();
        for (i, name) in code.varnames.iter().enumerate() {
            locals_map.insert(name.clone(), i);
        }
        
        // Copy arguments to locals
        for (i, (arg, param_name)) in args.into_iter().zip(code.varnames.iter()).enumerate() {
            if i < locals.len() {
                let rc_arg = RcValue::new(arg);
                locals[i] = rc_arg;
            }
        }
        
        // Convert globals and builtins to RcValue
        let rc_globals = globals.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        let rc_builtins = builtins.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();
        
        Self {
            code,
            pc: 0,
            registers,
            locals,
            locals_map,
            globals: rc_globals,
            builtins: rc_builtins,
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
            return_register: None,
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
    
    /// Get local variable by index (fast path)
    pub fn get_local(&self, index: usize) -> &RcValue {
        &self.locals[index]
    }
    
    /// Set local variable by index (fast path)
    pub fn set_local(&mut self, index: usize, value: RcValue) {
        self.locals[index] = value;
    }
    
    /// Get local variable index by name (for compiler use)
    pub fn get_local_index(&self, name: &str) -> Option<usize> {
        self.locals_map.get(name).copied()
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

/// Method cache for object-oriented code performance
#[derive(Debug, Clone)]
pub struct MethodCache {
    pub class_name: String,
    pub method_name: String,
    pub method: Option<Value>,
    pub version: u32,
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
        // Load all builtins from the centralized registry
        let builtins_values = crate::builtins::init_builtins();

        // Convert to RcValue for the builtins HashMap
        let mut builtins = HashMap::new();
        for (name, value) in &builtins_values {
            builtins.insert(name.clone(), RcValue::new(value.clone()));
        }

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

        // Create builtins module
        let builtins_module = Value::Module("builtins".to_string(), builtins_values.clone());

        // Initialize globals with __name__, __builtins__, and ALL builtins
        let mut globals = HashMap::new();
        globals.insert("__name__".to_string(), RcValue::new(Value::Str("__main__".to_string())));
        globals.insert("__builtins__".to_string(), RcValue::new(builtins_module.clone()));
        globals.insert("builtins".to_string(), RcValue::new(builtins_module));

        // Add all builtins to global scope (so input(), ord(), etc. are directly accessible)
        for (name, value) in &builtins_values {
            globals.insert(name.clone(), RcValue::new(value.clone()));
        }

        Self {
            frames: Vec::new(),
            builtins,
            globals,
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
    /// Get a global variable by name (for REPL)
    pub fn get_global(&self, name: &str) -> Option<&RcValue> {
        self.globals.get(name)
    }

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
        // Debug output to see the code object being executed
        // eprintln!("DEBUG: execute called with code object '{}' with {} instructions", code.name, code.instructions.len());

        // Convert globals and builtins from RcValue to Value for Frame::new
        let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
        let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();

        let frame = Frame::new(code, globals_values, builtins_values);
        self.frames.push(frame);

        // eprintln!("DEBUG: About to call run_frame, frames.len(): {}", self.frames.len());
        let result = self.run_frame()?;
        // eprintln!("DEBUG: run_frame completed with result: {:?}", result);

        // Update globals from the executed frame and pop it
        if let Some(frame) = self.frames.pop() {
            self.globals = frame.globals;
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
                // eprintln!("DEBUG: run_frame - no frames, returning None");
                return Ok(Value::None);
            }
            
            // Update frame index in case frames were added/removed
            frame_idx = self.frames.len() - 1;
            
            // Debug output to see the frame being executed
            // eprintln!("DEBUG: run_frame executing frame {} with code object '{}' with {} instructions",
            //           frame_idx, self.frames[frame_idx].code.name, self.frames[frame_idx].code.instructions.len());
            
            // Safety check: if there are no instructions, return None immediately
            if self.frames[frame_idx].code.instructions.is_empty() {
                // eprintln!("DEBUG: Code object '{}' has no instructions, returning None", self.frames[frame_idx].code.name);
                // Update globals before popping any frame (REPL needs this)
                if let Some(frame) = self.frames.last() {
                    self.globals = frame.globals.clone();
                }
                self.frames.pop();
                return Ok(Value::None);
            }
            
            // Fast path: check bounds
            let pc = self.frames[frame_idx].pc;
            let instructions_len = self.frames[frame_idx].code.instructions.len();
            
            // eprintln!("DEBUG: PC: {}, Instructions len: {}", pc, instructions_len);

            if pc >= instructions_len {
                // eprintln!("DEBUG: PC >= instructions_len, breaking");
                break;
            }
            
            // Direct access to instruction without cloning when possible
            // Get the instruction details without borrowing self
            let (opcode, arg1, arg2, arg3, function_name, line_num, filename) = {
                let frame = &self.frames[frame_idx];
                let instruction = &frame.code.instructions[pc];
                (instruction.opcode, instruction.arg1, instruction.arg2, instruction.arg3,
                 frame.code.name.clone(), instruction.line, frame.code.filename.clone())
            };

            // Track instruction execution for profiling and JIT compilation
            self.track_instruction_execution(&function_name, pc);

            // eprintln!("DEBUG: Executing instruction at pc {}: {:?} (arg1: {}, arg2: {}, arg3: {})", pc, opcode, arg1, arg2, arg3);
            // Execute instruction using computed GOTOs for maximum performance
            match self.execute_instruction_fast(frame_idx, opcode, arg1, arg2, arg3) {
                Ok(Some(value)) => {
                    // eprintln!("DEBUG: Instruction returned value: {:?}", value);
                    // Check if we have more frames to execute
                    if self.frames.is_empty() {
                        return Ok(value);
                    }
                    // If we still have frames, continue execution with the next frame
                    continue;
                },
                Ok(None) => {
                    // eprintln!("DEBUG: Instruction completed, frame_idx: {}, frames.len(): {}", frame_idx, self.frames.len());
                    // Check if a new frame was pushed during execution
                    if self.frames.len() > frame_idx + 1 {
                        // eprintln!("DEBUG: New frame was pushed, continuing");
                        // A new frame was pushed, continue execution with the new frame
                        continue;
                    }
                    // eprintln!("DEBUG: No new frame, checking PC");
                    // Only increment PC if frame still exists and PC hasn't changed
                    if frame_idx < self.frames.len() {
                        // Check if PC has changed (e.g., due to a jump)
                        // eprintln!("DEBUG: PC before: {}, PC after: {}", pc, self.frames[frame_idx].pc);
                        if self.frames[frame_idx].pc == pc {
                            self.frames[frame_idx].pc += 1;
                            // eprintln!("DEBUG: Incremented PC to {}", self.frames[frame_idx].pc);
                        } else {
                            // eprintln!("DEBUG: PC was changed by instruction, now {}", self.frames[frame_idx].pc);
                        }
                        // If PC has changed, we don't increment it
                    }
                },
                Err(e) => {
                    // eprintln!("DEBUG: Instruction failed with error: {}", e);
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
                        // Format error with line number context for Python-like traceback
                        let error_msg = format!(
                            "Traceback (most recent call last):\n  File \"{}\", line {}, in {}\n{}",
                            filename, line_num, function_name, e
                        );
                        return Err(anyhow!(error_msg));
                    }
                },
            }
        }
        
        // eprintln!("DEBUG: run_frame - completed, returning None");
        Ok(Value::None)
    }

    /// Optimized instruction execution with computed GOTOs for maximum performance
    #[inline(always)]
    fn execute_instruction_fast(&mut self, frame_idx: usize, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32) -> Result<Option<Value>> {
        // Computed GOTOs implementation for maximum performance
        // eprintln!("DEBUG: execute_instruction_fast - opcode: {:?}", opcode);
        match opcode {
            OpCode::NOP => {
                // No operation; proceed to next instruction
                Ok(None)
            }
            OpCode::LoadFast => {
                // Fast load from local variable by index - no bounds checking for maximum performance
                let local_idx = arg1 as usize;
                let result_reg = arg2;
                
                // Direct access without bounds checking for maximum performance
                // The compiler ensures indices are valid
                let value = self.frames[frame_idx].locals[local_idx].clone();
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::StoreFast => {
                // Fast store to local variable by index - no bounds checking for maximum performance
                let local_idx = arg1 as usize;
                let value = self.frames[frame_idx].get_register(arg2).clone();
                
                // Direct access without bounds checking for maximum performance
                // The compiler ensures indices are valid
                self.frames[frame_idx].locals[local_idx] = value;
                Ok(None)
            }
            OpCode::LoadConst => {
                let const_idx = arg1 as usize;
                // eprintln!("DEBUG: LoadConst - const_idx: {}, result_reg: {}", const_idx, arg2);
                // Check bounds before accessing
                if const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadConst: constant index {} out of bounds (len: {})", const_idx, self.frames[frame_idx].code.constants.len()));
                }
                let value = RcValue::new(self.frames[frame_idx].code.constants[const_idx].clone());
                // eprintln!("DEBUG: LoadConst - loaded value: {:?}", value.value);
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
                
                // eprintln!("DEBUG: LoadGlobal - loading '{}' (name_idx: {}, cache_idx: {}, result_reg: {})", name, name_idx, arg2, arg3);

                // Skip local variable check for global frame (frame 0)
                // For function frames, we've already handled local variables with LoadFast/StoreFast
                if frame_idx > 0 {
                    // In function frames, all local variables should be accessed with LoadFast
                    // If we reach here, it's likely a global variable
                }
                
                // Try inline cache next
                let cache_idx = arg2 as usize;
                if cache_idx < self.frames[frame_idx].code.inline_caches.len() {
                    let should_use_cache = {
                        let cache = &self.frames[frame_idx].code.inline_caches[cache_idx];
                        cache.version == self.frames[frame_idx].cache_version && cache.data.is_some()
                    };
                    
                    if should_use_cache {
                        let cache_data = self.frames[frame_idx].code.inline_caches[cache_idx].data.clone().unwrap();
                        let value = RcValue::new(cache_data);
                        // Debug output for inline cache
                        // eprintln!("DEBUG: Using inline cache for '{}': {:?}", name, value.value);
                        // Additional debug output for Closure values from cache
                        if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = value.value {
                            // eprintln!("DEBUG: Cached Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                            if let Some(ref code) = compiled_code {
                                // eprintln!("DEBUG: Cached Closure '{}' has {} instructions", name, code.instructions.len());
                            }
                        }
                        self.frames[frame_idx].set_register(arg3, value);
                        return Ok(None);
                    }
                }
                
                // Fallback to global lookup
                let global_value = self.frames[frame_idx].globals.get(name).cloned();
                if let Some(value) = global_value {
                    // Debug output to see what global value we found
                    // eprintln!("DEBUG: Found '{}' in globals: {:?}", name, value.value);
                    // Additional debug output for Closure values
                    if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = value.value {
                        // eprintln!("DEBUG: Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                        if let Some(ref code) = compiled_code {
                            // eprintln!("DEBUG: Closure '{}' has {} instructions", name, code.instructions.len());
                        }
                    }
                    // Update inline cache
                    if cache_idx < self.frames[frame_idx].code.inline_caches.len() {
                        self.frames[frame_idx].code.inline_caches[cache_idx] = InlineCache {
                            counter: 0,
                            version: self.frames[frame_idx].cache_version,
                            data: Some(value.value.clone()),
                            type_info: None::<String>,
                        };
                    }
                    self.frames[frame_idx].set_register(arg3, value);
                    Ok(None)
                } else {
                    // Check built-ins if global not found
                    let builtin_value = self.frames[frame_idx].builtins.get(name).cloned();
                    if let Some(value) = builtin_value {
                        // Debug output to see what builtin value we found
                        // eprintln!("DEBUG: Found '{}' in built-ins: {:?}", name, value.value);
                        // Update inline cache
                        if cache_idx < self.frames[frame_idx].code.inline_caches.len() {
                            self.frames[frame_idx].code.inline_caches[cache_idx] = InlineCache {
                                counter: 0,
                                version: self.frames[frame_idx].cache_version,
                                data: Some(value.value.clone()),
                                type_info: None::<String>,
                            };
                        }
                        self.frames[frame_idx].set_register(arg3, value);
                        Ok(None)
                    } else {
                        // Debug output to see what built-ins are available
                        let available_builtins: Vec<String> = self.frames[frame_idx].builtins.keys().cloned().collect();
                        // eprintln!("DEBUG: '{}' not found in locals, globals or built-ins. Available built-ins: {:?}", name, available_builtins);
                        Err(anyhow!("Global '{}' is not defined", name))
                    }
                }
            }
            OpCode::StoreGlobal => {
                let name_idx = arg1 as usize;
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("StoreGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                let name = self.frames[frame_idx].code.names[name_idx].clone();
                let value = self.frames[frame_idx].get_register(arg2).clone();

                // Store in globals (module-level variables or true globals)
                if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = value.value {
                    // eprintln!("DEBUG: Storing Closure '{}' with compiled_code: {}", name, compiled_code.is_some());
                    if let Some(ref code) = compiled_code {
                        // eprintln!("DEBUG: Storing Closure '{}' has {} instructions", name, code.instructions.len());
                    }
                }

                self.frames[frame_idx].globals.insert(name, value);
                // Always invalidate caches to ensure correctness
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
            OpCode::BinarySubRR => {
                // Register-Register subtraction
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinarySubRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a - b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a - b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.sub_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinarySubRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareEqualRR => {
                // Register-Register equality comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                let left = self.frames[frame_idx].registers[left_reg as usize].clone();
                let right = self.frames[frame_idx].registers[right_reg as usize].clone();
                let result = left == right;
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::Bool(result));
                Ok(None)
            }
            OpCode::BinaryMulRR => {
                // Register-Register multiplication
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryMulRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a * b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a * b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.mul_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryMulRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryDivRR => {
                // Register-Register division
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryDivRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0f64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / b)
                    },
                    (Value::Int(a), Value::Float(b)) => {
                        if *b == 0.0f64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(*a as f64 / b)
                    },
                    (Value::Float(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / *b as f64)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.div_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryDivRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryModRR => {
                // Register-Register modulo
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryModRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Int(a % b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0f64 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Float(a % b)
                    },
                    (Value::Int(a), Value::Float(b)) => {
                        if *b == 0.0f64 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Float(*a as f64 % b)
                    },
                    (Value::Float(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Float(a % *b as f64)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.mod_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryModRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryPowRR => {
                // Register-Register power (exponentiation)
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;

                if left_reg as usize >= self.frames[frame_idx].registers.len() ||
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryPowRR: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];

                // Compute power using f64::powf for all numeric types
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        // For integer bases and exponents, try to keep as int if possible
                        if *b >= 0 && *b < 100 {
                            // Small positive integer exponents
                            let result_f64 = (*a as f64).powi(*b as i32);
                            // Check if result fits in i64 and is whole number
                            if result_f64.is_finite() && result_f64 == result_f64.floor() && result_f64.abs() < (i64::MAX as f64) {
                                Value::Int(result_f64 as i64)
                            } else {
                                Value::Float(result_f64)
                            }
                        } else {
                            Value::Float((*a as f64).powf(*b as f64))
                        }
                    },
                    (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
                    (Value::Int(a), Value::Float(b)) => Value::Float((*a as f64).powf(*b)),
                    (Value::Float(a), Value::Int(b)) => Value::Float(a.powf(*b as f64)),
                    _ => {
                        // For less common cases, use the general implementation
                        let result = match (&left.value, &right.value) {
                            (Value::Int(a), Value::Int(b)) => {
                                if *b >= 0 {
                                    Value::Int(a.pow(*b as u32))
                                } else {
                                    Value::Float((*a as f64).powf(*b as f64))
                                }
                            },
                            (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
                            (Value::Int(a), Value::Float(b)) => Value::Float((*a as f64).powf(*b)),
                            (Value::Float(a), Value::Int(b)) => Value::Float(a.powf(*b as f64)),
                            _ => return Err(anyhow!("Unsupported types for power operation"))
                        };
                        result
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareLessRR => {
                // Register-Register less than comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;

                if left_reg as usize >= self.frames[frame_idx].registers.len() ||
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareLessRR: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];

                // Perform comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a < b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a < b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64).lt(b)),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(a.lt(&(*b as f64))),
                    _ => {
                        // For less common cases, use the general implementation
                        self.lt_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareLessRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }

            OpCode::LoadFast => {
                // Fast load from local variable by index - no bounds checking for maximum performance
                let local_idx = arg1 as usize;
                let result_reg = arg2;
                
                // Direct access without bounds checking for maximum performance
                // The compiler ensures indices are valid
                let value = self.frames[frame_idx].locals[local_idx].clone();
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::StoreFast => {
                // Fast store to local variable by index - no bounds checking for maximum performance
                let local_idx = arg1 as usize;
                let value = self.frames[frame_idx].get_register(arg2).clone();
                
                // Direct access without bounds checking for maximum performance
                // The compiler ensures indices are valid
                self.frames[frame_idx].locals[local_idx] = value;
                Ok(None)
            }
            OpCode::BuildDict => {
                // Make a dictionary with `arg1` key-value pairs
                let num_pairs = arg1;
                let result_reg = arg2;
                
                if num_pairs < 0 {
                    return Err(anyhow!("MakeDict: number of pairs cannot be negative"));
                }
                
                let mut dict = HashMap::new();
                for i in 0..num_pairs {
                    let key_value = self.frames[frame_idx].get_register(i * 2).clone().value;
                    let value = self.frames[frame_idx].get_register(i * 2 + 1).clone().value;
                    // Convert key to string representation
                    let key_str = match key_value {
                        Value::Int(n) => n.to_string(),
                        Value::Float(f) => f.to_string(),
                        Value::Bool(b) => b.to_string(),
                        Value::Str(s) => s,
                        _ => format!("{:?}", key_value),
                    };
                    dict.insert(key_str, value);
                }
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::Dict(dict));
                Ok(None)
            }
            OpCode::BuildTuple => {
                // Make a tuple with `arg1` elements
                let num_elements = arg1;
                let result_reg = arg2;
                
                if num_elements < 0 {
                    return Err(anyhow!("MakeTuple: number of elements cannot be negative"));
                }
                
                let mut elements = Vec::with_capacity(num_elements as usize);
                for i in 0..num_elements {
                    elements.push(self.frames[frame_idx].get_register(i).clone().value);
                }
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::Tuple(elements));
                Ok(None)
            }
            OpCode::BuildSet => {
                // Make a set with `arg1` elements
                let num_elements = arg1;
                let result_reg = arg2;
                
                if num_elements < 0 {
                    return Err(anyhow!("MakeSet: number of elements cannot be negative"));
                }
                
                let mut set = Vec::new();
                for i in 0..num_elements {
                    let value = self.frames[frame_idx].get_register(i).clone().value;
                    // Check if value already exists in set (to maintain set semantics)
                    if !set.contains(&value) {
                        set.push(value);
                    }
                }
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(Value::Set(set));
                Ok(None)
            }
            OpCode::MakeFunction => {
                // Make a function with the code at `arg1` and the globals at `arg2`
                // This opcode is not fully implemented in our VM
                return Err(anyhow!("MakeFunction: not implemented"));
            }
            OpCode::Jump => {
                // Jump to the instruction at `arg1`
                self.frames[frame_idx].pc = arg1 as usize;
                Ok(None)
            }
            OpCode::LoadConst => {
                // Load the constant at `arg1` into register `arg2`
                let const_idx = arg1 as usize;
                let result_reg = arg2;
                
                if const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadConst: constant index out of bounds"));
                }
                
                let constant = RcValue::new(self.frames[frame_idx].code.constants[const_idx].clone());
                self.frames[frame_idx].registers[result_reg as usize] = constant;
                Ok(None)
            }
            OpCode::StoreGlobal => {
                // Store the value in register `arg1` into the global variable at `arg2`
                let value_reg = arg1;
                let global_idx = arg2 as usize;
                
                if value_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreGlobal: register index out of bounds"));
                }
                
                // For now, we'll just return an error as this is complex to implement correctly
                return Err(anyhow!("StoreGlobal: not implemented"));
            }

            OpCode::BinarySubRRFast => {
                // Fast path for integer Register-Register subtraction
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinarySubRRFast: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for integer subtraction
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a - b),
                    _ => {
                        // Fallback to regular subtraction
                        return self.execute_instruction_fast(frame_idx, OpCode::BinarySubRR, arg1, arg2, arg3);
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryMulRRFast => {
                // Fast path for integer Register-Register multiplication
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryMulRRFast: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for integer multiplication
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a * b),
                    _ => {
                        // Fallback to regular multiplication
                        return self.execute_instruction_fast(frame_idx, OpCode::BinaryMulRR, arg1, arg2, arg3);
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }











            OpCode::BinaryPowRRFast => {
                // Fast path for integer Register-Register power (exponentiation)
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;

                if left_reg as usize >= self.frames[frame_idx].registers.len() ||
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryPowRRFast: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];

                // Compute power using i64::pow for integer bases and exponents only
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b >= 0 {
                            Value::Int(a.pow(*b as u32))
                        } else {
                            Value::Float((*a as f64).powf(*b as f64))
                        }
                    },
                    _ => {
                        // Fallback to regular power
                        return self.execute_instruction_fast(frame_idx, OpCode::BinaryPowRR, arg1, arg2, arg3);
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareEqualRR => {
                // Register-Register equality comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a == b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a == b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64) == *b),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(*a == (*b as f64)),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a == b),
                    (Value::Bool(a), Value::Bool(b)) => Value::Bool(a == b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.eq_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareEqualRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareNotEqualRR => {
                // Register-Register not equal comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareNotEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a != b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a != b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64) != *b),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(*a != (*b as f64)),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a != b),
                    (Value::Bool(a), Value::Bool(b)) => Value::Bool(a != b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.ne_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareNotEqualRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareLessRR => {
                // Register-Register less than comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareLessRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a < b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a < b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64) < *b),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(*a < (*b as f64)),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a < b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.lt_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareLessRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareGreaterRR => {
                // Register-Register greater than comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareGreaterRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a > b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a > b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64) > *b),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(*a > (*b as f64)),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a > b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.gt_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareGreaterRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::SubscrLoad => {
                // Load item from sequence (obj[key])
                let object_reg = arg1;
                let index_reg = arg2;
                let result_reg = arg3;
                
                if object_reg as usize >= self.frames[frame_idx].registers.len() || 
                   index_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("SubscrLoad: register index out of bounds"));
                }
                
                let object = &self.frames[frame_idx].registers[object_reg as usize];
                let index = &self.frames[frame_idx].registers[index_reg as usize];
                
                // Handle list subscript access
                let result = match (&object.value, &index.value) {
                    (Value::List(list), Value::Int(idx)) => {
                        // Use HPList's get method which handles negative indexing
                        match list.get(*idx as isize) {
                            Some(value) => value.clone(),
                            None => return Err(anyhow!("list index out of range")),
                        }
                    },
                    _ => return Err(anyhow!("Subscript operation not supported for these types")),
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::SubscrStore => {
                // Store item to sequence (obj[key] = value)
                let object_reg = arg1;
                let index_reg = arg2;
                let value_reg = arg3;
                
                if object_reg as usize >= self.frames[frame_idx].registers.len() || 
                   index_reg as usize >= self.frames[frame_idx].registers.len() ||
                   value_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("SubscrStore: register index out of bounds"));
                }
                
                // Handle list subscript assignment
                // Clone all values first to avoid borrowing conflicts
                let object_value = self.frames[frame_idx].registers[object_reg as usize].value.clone();
                let index_value = self.frames[frame_idx].registers[index_reg as usize].value.clone();
                let value_to_set = self.frames[frame_idx].registers[value_reg as usize].value.clone();
                
                // Check if we can perform the assignment
                match (object_value, index_value) {
                    (Value::List(mut list), Value::Int(idx)) => {
                        // Perform the assignment
                        list.set(idx as isize, value_to_set)
                            .map_err(|e| anyhow!("Error in list assignment: {}", e))?;
                        // Update the register with the modified list
                        self.frames[frame_idx].registers[object_reg as usize].value = Value::List(list);
                    },
                    _ => return Err(anyhow!("Subscript assignment not supported for these types")),
                }
                
                Ok(None)
            }
            OpCode::CompareLessEqualRR => {
                // Register-Register less than or equal comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareLessEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a <= b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a <= b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64) <= *b),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(*a <= (*b as f64)),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a <= b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.le_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareLessEqualRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareGreaterEqualRR => {
                // Register-Register greater than or equal comparison
                let left_reg = arg1;
                let right_reg = arg2;
                let result_reg = arg3;
                
                if left_reg as usize >= self.frames[frame_idx].registers.len() || 
                   right_reg as usize >= self.frames[frame_idx].registers.len() ||
                   result_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareGreaterEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg as usize];
                let right = &self.frames[frame_idx].registers[right_reg as usize];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a >= b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a >= b),
                    (Value::Int(a), Value::Float(b)) => Value::Bool((*a as f64) >= *b),
                    (Value::Float(a), Value::Int(b)) => Value::Bool(*a >= (*b as f64)),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a >= b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.ge_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in CompareGreaterEqualRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::ReturnValue => {
                let reg = arg1;
                // eprintln!("DEBUG: ReturnValue - reg: {}, frame_idx: {}, frames.len(): {}", reg, frame_idx, self.frames.len());
                if reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("ReturnValue: register index out of bounds"));
                }
                let value = self.frames[frame_idx].registers[reg as usize].clone();
                // eprintln!("DEBUG: ReturnValue - value: {:?}", value);

                // Check if we need to store the return value in a caller's register
                let return_info = self.frames[frame_idx].return_register;
                // eprintln!("DEBUG: ReturnValue - return_info: {:?}", return_info);
                // Update globals before popping any frame (REPL needs this)
                self.globals = self.frames[frame_idx].globals.clone();
                self.frames.pop();
                // eprintln!("DEBUG: ReturnValue - after pop, frames.len(): {}", self.frames.len());

                if let Some((caller_frame_idx, result_reg)) = return_info {
                    // Store return value in caller's register
                    if caller_frame_idx < self.frames.len() {
                        self.frames[caller_frame_idx].set_register(result_reg, value);
                    }
                    Ok(None) // Continue execution in caller
                } else {
                    Ok(Some(value.value)) // Top-level return
                }
            }
            OpCode::CallFunction => {
                // Call a function with arguments
                let func_reg = arg1;
                let arg_count = arg2 as usize;
                let result_reg = arg3;

                // eprintln!("DEBUG: CallFunction - func_reg: {}, arg_count: {}, result_reg: {}", func_reg, arg_count, result_reg);

                // Get the function from the register
                if func_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunction: function register index out of bounds"));
                }
                let func_value = self.frames[frame_idx].registers[func_reg as usize].clone();
                // eprintln!("DEBUG: CallFunction - func_value: {:?}", func_value);

                // Collect arguments from consecutive registers
                let mut args = Vec::new();
                for i in 0..arg_count {
                    let arg_reg = func_reg + 1 + i as u32;
                    if arg_reg as usize >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallFunction: argument register index out of bounds"));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg as usize].value.clone());
                }
                // eprintln!("DEBUG: CallFunction - args: {:?}", args);

                // CRITICAL FIX: Check if a new frame was pushed BEFORE calling the function
                let frames_before = self.frames.len();

                // Call the function with caller frame info
                let result = self.call_function_fast(func_value.value, args, Some(frame_idx), Some(result_reg))?;

                // Check if a new frame was actually pushed (not just if result is None)
                let frames_after = self.frames.len();
                if frames_after > frames_before {
                    // A new frame was pushed (user-defined function), increment caller's PC
                    // This ensures when the function returns, execution continues at the next instruction
                    self.frames[frame_idx].pc += 1;
                } else {
                    // Builtin function returned directly (no frame pushed), store result
                    // PC will be incremented by main loop
                    self.frames[frame_idx].set_register(result_reg, RcValue::new(result));
                }
                Ok(None)
            }
            OpCode::CallMethod => {
                // Call a method on an object using the new comprehensive call_method implementation
                let object_reg = arg1;
                let arg_count = arg2 as usize;
                let method_name_idx = arg3 as usize;

                // Get the object from the register
                if object_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallMethod: object register index out of bounds"));
                }

                // Get the method name
                if method_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("CallMethod: method name index out of bounds"));
                }
                let method_name = self.frames[frame_idx].code.names[method_name_idx].clone();

                // Collect arguments from consecutive registers following the object register
                let mut args = Vec::new();
                for i in 0..arg_count {
                    let arg_reg = object_reg + 1 + (i as u32);
                    if arg_reg as usize >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallMethod: argument register {} out of bounds", arg_reg));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg as usize].value.clone());
                }

                // Clone the object value to call methods on it
                // For mutating methods, we'll update the register after the call
                let mut object_value = self.frames[frame_idx].registers[object_reg as usize].value.clone();

                // Call the method using the comprehensive call_method implementation
                // Note: call_method may mutate object_value for mutating methods like append()
                match object_value.call_method(&method_name, args) {
                    Ok(result) => {
                        // Determine if this is a mutating method that returns None
                        let is_pure_mutating = matches!(
                            (&object_value, method_name.as_str(), &result),
                            (Value::List(_), "append" | "extend" | "insert" | "remove" | "clear" | "sort" | "reverse", Value::None) |
                            (Value::Dict(_), "clear" | "update", Value::None) |
                            (Value::Set(_), "add" | "remove" | "discard" | "clear", Value::None) |
                            (Value::ByteArray(_), "append" | "extend", Value::None)
                        );

                        if is_pure_mutating {
                            // For pure mutating methods (mutate AND return None):
                            // Store the mutated object in the register so StoreGlobal can save it
                            self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(object_value);
                        } else {
                            // For all other methods:
                            // - Non-mutating methods: store the result (e.g., upper() returns new string)
                            // - Mutating methods that return a value: store the result (e.g., pop() returns value)
                            //   Note: For pop(), the mutated object will be saved by call_method, but we need to
                            //   also store it in the register for StoreGlobal. Let's do both:
                            if matches!((&object_value, method_name.as_str()),
                                (Value::List(_), "pop") |
                                (Value::Dict(_), "pop" | "setdefault" | "popitem") |
                                (Value::Set(_), "pop")
                            ) {
                                // First store the mutated object (will be saved by StoreGlobal)
                                // But wait - we need BOTH the mutated object AND the result!
                                // The trick: StoreGlobal happens AFTER this, so we need to store mutated object now
                                // Then after StoreGlobal in the generated code, LoadLocal will get the result
                                // But StoreGlobal reads the register, so it will see what we store here
                                // Solution: Store the mutated object for StoreGlobal to see
                                // The result will be "lost" but that's okay because these methods return a value
                                // Actually, the compiler should handle this differently. For now, let's store the mutated object
                                self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(object_value.clone());
                            }
                            // Now store the result for LoadLocal to retrieve
                            self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(result);
                        }

                        Ok(None)
                    }
                    Err(e) => Err(anyhow!("CallMethod error: {}", e)),
                }
            }
            OpCode::BuildList => {
                // Build a list from items in consecutive registers
                let item_count = arg1 as usize;
                let first_item_reg = arg2 as usize; // Starting register for items
                let result_reg = arg3;

                // Collect items from consecutive registers starting at first_item_reg
                let mut items = Vec::new();
                for i in 0..item_count {
                    let reg_idx = first_item_reg + i;
                    if reg_idx >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildList: register index {} out of bounds", reg_idx));
                    }
                    items.push(self.frames[frame_idx].registers[reg_idx].value.clone());
                }

                let list = crate::modules::hplist::HPList::from_values(items);
                let value = Value::List(list);

                self.frames[frame_idx].set_register(result_reg, RcValue::new(value));
                Ok(None)
            }
            OpCode::Jump => {
                // Unconditional jump
                let target = arg1 as usize;
                // eprintln!("DEBUG: Jump - jumping to target {}", target);
                self.frames[frame_idx].pc = target;
                Ok(None)
            }
            OpCode::JumpIfTrue => {
                // Jump if value is truthy
                let reg = arg1 as usize;
                let target = arg2 as usize;
                
                if reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("JumpIfTrue: register index out of bounds"));
                }
                
                let value = &self.frames[frame_idx].registers[reg];
                if value.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::JumpIfFalse => {
                // Jump if value is falsy
                let reg = arg1 as usize;
                let target = arg2 as usize;
                
                if reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("JumpIfFalse: register index out of bounds"));
                }
                
                let value = &self.frames[frame_idx].registers[reg];
                if !value.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::PopJumpIfTrue => {
                // Pop value from stack and jump if truthy
                // In our register-based VM, we'll just check the register
                let reg = arg1 as usize;
                let target = arg2 as usize;
                
                if reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("PopJumpIfTrue: register index out of bounds"));
                }
                
                let value = &self.frames[frame_idx].registers[reg];
                if value.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::PopJumpIfFalse => {
                // Pop value from stack and jump if falsy
                // In our register-based VM, we'll just check the register
                let reg = arg1 as usize;
                let target = arg2 as usize;
                
                if reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("PopJumpIfFalse: register index out of bounds"));
                }
                
                let value = &self.frames[frame_idx].registers[reg];
                if !value.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::GetIter => {
                // Get an iterator for a value
                let src_reg = arg1 as usize;
                let dst_reg = arg2 as usize;
                
                if src_reg >= self.frames[frame_idx].registers.len() || dst_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("GetIter: register index out of bounds"));
                }
                
                let src_value = &self.frames[frame_idx].registers[src_reg].value;
                let iterator_value = match src_value {
                    Value::Range { start, stop, step } => {
                        // Create a RangeIterator from a Range
                        Value::RangeIterator { 
                            start: *start, 
                            stop: *stop, 
                            step: *step, 
                            current: *start 
                        }
                    },
                    _ => {
                        // For other types, just copy the value for now
                        src_value.clone()
                    }
                };
                
                let value = RcValue::new(iterator_value);
                self.frames[frame_idx].set_register(dst_reg as u32, value);
                Ok(None)
            }
            OpCode::ForIter => {
                // Iterate over an iterator
                let iter_reg = arg1 as usize;
                let result_reg = arg2 as usize;
                let target = arg3 as usize;
                
                if iter_reg >= self.frames[frame_idx].registers.len() || result_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("ForIter: register index out of bounds"));
                }
                
                // Handle Range iteration
                // Clone the iterator value to avoid borrowing issues
                let iter_value = self.frames[frame_idx].registers[iter_reg].value.clone();
                match iter_value {
                    Value::RangeIterator { start, stop, step, current } => {
                        // eprintln!("DEBUG: ForIter - RangeIterator {{ start: {}, stop: {}, step: {}, current: {} }}", start, stop, step, current);
                        // Check if we've reached the end of the range
                        let should_continue = if step > 0 {
                            current < stop
                        } else if step < 0 {
                            current > stop
                        } else {
                            // step == 0 is invalid, but we'll treat it as end of iteration
                            false
                        };
                        // eprintln!("DEBUG: ForIter - should_continue: {}", should_continue);
                        
                        if should_continue {
                            // Store the current value in the result register
                            let value = RcValue::new(Value::Int(current));
                            self.frames[frame_idx].set_register(result_reg as u32, value);
                            
                            // Update the iterator's current position
                            let new_current = current + step;
                            let updated_iterator = Value::RangeIterator {
                                start,
                                stop,
                                step,
                                current: new_current,
                            };
                            self.frames[frame_idx].registers[iter_reg] = RcValue::new(updated_iterator);
                            
                            // Continue with the loop body
                            // eprintln!("DEBUG: ForIter - continuing loop");
                            Ok(None)
                        } else {
                            // End of iteration - jump to the target (after the loop)
                            // eprintln!("DEBUG: ForIter - end of iteration, jumping to target {}", target);
                            self.frames[frame_idx].pc = target;
                            // Return Ok(None) to indicate that PC has changed
                            Ok(None)
                        }
                    },
                    _ => {
                        // eprintln!("DEBUG: ForIter - not a RangeIterator, jumping to target {}", target);
                        // For other types, just jump to end for now
                        self.frames[frame_idx].pc = target;
                        // Return Ok(None) to indicate that PC has changed
                        Ok(None)
                    }
                }
            }
            OpCode::BinaryDivRRFastInt => {
                // eprintln!("DEBUG: BinaryDivRRFastInt - executing");
                // Fast path for integer Register-Register division
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // eprintln!("DEBUG: BinaryDivRRFastInt - left_reg: {}, right_reg: {}, result_reg: {}", left_reg, right_reg, result_reg);

                if left_reg >= self.frames[frame_idx].registers.len() ||
                   right_reg >= self.frames[frame_idx].registers.len() ||
                   result_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryDivRRFastInt: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];

                // eprintln!("DEBUG: BinaryDivRRFastInt - left: {:?}, right: {:?}", left.value, right.value);

                // Handle all division cases
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        // In Python, division always returns float
                        Value::Float(*a as f64 / *b as f64)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0f64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / b)
                    },
                    (Value::Int(a), Value::Float(b)) => {
                        if *b == 0.0f64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(*a as f64 / b)
                    },
                    (Value::Float(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / *b as f64)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.div_values(left.value.clone(), right.value.clone())?
                    }
                };

                // eprintln!("DEBUG: BinaryDivRRFastInt - result: {:?}", result);

                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryModRRFastInt => {
                // Fast path for integer Register-Register modulo
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || 
                   right_reg >= self.frames[frame_idx].registers.len() ||
                   result_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryModRRFastInt: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer modulo
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0i64 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Int(a % b)
                    },
                    _ => {
                        // Fallback to regular modulo
                        return self.execute_instruction_fast(frame_idx, OpCode::BinaryModRR, arg1, arg2, arg3);
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::FastIntAdd => {
                // Ultra-fast integer addition without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val + right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular addition if not integers
                return self.execute_instruction_fast(frame_idx, OpCode::BinaryAddRR, arg1, arg2, arg3);
            }
            OpCode::FastIntSub => {
                // Ultra-fast integer subtraction without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val - right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular subtraction if not integers
                return self.execute_instruction_fast(frame_idx, OpCode::BinarySubRR, arg1, arg2, arg3);
            }
            OpCode::FastIntMul => {
                // Ultra-fast integer multiplication without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val * right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular multiplication if not integers
                return self.execute_instruction_fast(frame_idx, OpCode::BinaryMulRR, arg1, arg2, arg3);
            }
            _ => {
                // For unimplemented opcodes, return an error
                // eprintln!("DEBUG: Unimplemented opcode: {:?}", opcode);
                Err(anyhow!("Unimplemented opcode: {:?}", opcode))
            }
        }
    }
    
    fn add_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 + b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a + b as f64)),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Str(format!("{}{}", a, b))),
            _ => Err(anyhow!("Unsupported types for addition")),
        }
    }
    
    fn sub_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a - b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a - b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 - b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a - b as f64)),
            _ => Err(anyhow!("Unsupported types for subtraction")),
        }
    }
    
    fn mul_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a * b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a * b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Float(a as f64 * b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Float(a * b as f64)),
            _ => Err(anyhow!("Unsupported types for multiplication")),
        }
    }
    
    fn div_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Int(a / b))
                }
            },
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b))
                }
            },
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a as f64 / b))
                }
            },
            (Value::Float(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Division by zero"))
                } else {
                    Ok(Value::Float(a / b as f64))
                }
            },
            _ => Err(anyhow!("Unsupported types for division")),
        }
    }
    
    fn mod_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Int(a % b))
                }
            },
            (Value::Float(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a % b))
                }
            },
            (Value::Int(a), Value::Float(b)) => {
                if b == 0.0f64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a as f64 % b))
                }
            },
            (Value::Float(a), Value::Int(b)) => {
                if b == 0i64 {
                    Err(anyhow!("Modulo by zero"))
                } else {
                    Ok(Value::Float(a % b as f64))
                }
            },
            _ => Err(anyhow!("Unsupported types for modulo")),
        }
    }
    
    fn eq_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a == b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a == b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) == b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a == (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a == b)),
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Bool(a == b)),
            _ => Ok(Value::Bool(false)), // Different types are not equal
        }
    }
    
    fn ne_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a != b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a != b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) != b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a != (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a != b)),
            (Value::Bool(a), Value::Bool(b)) => Ok(Value::Bool(a != b)),
            _ => Ok(Value::Bool(true)), // Different types are not equal
        }
    }
    
    fn lt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a < b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a < b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) < b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a < (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a < b)),
            _ => Err(anyhow!("Unsupported types for less than comparison")),
        }
    }
    
    fn gt_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a > b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a > b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) > b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a > (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a > b)),
            _ => Err(anyhow!("Unsupported types for greater than comparison")),
        }
    }
    
    fn le_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a <= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a <= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) <= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a <= (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a <= b)),
            _ => Err(anyhow!("Unsupported types for less than or equal comparison")),
        }
    }
    
    fn ge_values(&self, left: Value, right: Value) -> Result<Value> {
        match (left, right) {
            (Value::Int(a), Value::Int(b)) => Ok(Value::Bool(a >= b)),
            (Value::Float(a), Value::Float(b)) => Ok(Value::Bool(a >= b)),
            (Value::Int(a), Value::Float(b)) => Ok(Value::Bool((a as f64) >= b)),
            (Value::Float(a), Value::Int(b)) => Ok(Value::Bool(a >= (b as f64))),
            (Value::Str(a), Value::Str(b)) => Ok(Value::Bool(a >= b)),
            _ => Err(anyhow!("Unsupported types for greater than or equal comparison")),
        }
    }
    
    /// Optimized function call path with JIT compilation support
    fn call_function_fast(&mut self, func: Value, args: Vec<Value>, caller_frame_idx: Option<usize>, result_reg: Option<u32>) -> Result<Value> {
        // eprintln!("DEBUG: call_function_fast - func: {:?}", func);
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
            Value::BuiltinFunction(ref name, fptr) => {
                // Special handling for introspection builtins that need VM context
                match name.as_str() {
                    "dir" if args.is_empty() => {
                        // Return sorted list of names in current scope
                        // Always use frame.globals since it has the most up-to-date state
                        let mut names: Vec<String> = if let Some(frame) = self.frames.last() {
                            frame.globals.keys().cloned().collect()
                        } else {
                            self.globals.keys().cloned().collect()
                        };
                        names.sort();
                        let values: Vec<Value> = names.iter().map(|s| Value::Str(s.clone())).collect();
                        return Ok(Value::List(crate::modules::hplist::HPList::from_values(values)));
                    }
                    "globals" if args.is_empty() => {
                        // Return dictionary of global variables
                        // Use frame.globals for current state
                        let globals_dict: HashMap<String, Value> = if let Some(frame) = self.frames.last() {
                            frame.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()
                        } else {
                            self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()
                        };
                        return Ok(Value::Dict(globals_dict));
                    }
                    "locals" if args.is_empty() => {
                        // Return dictionary of local variables
                        // At module level, locals() == globals()
                        // In function scope, return the function's local variables
                        if let Some(frame) = self.frames.last() {
                            // Check if we're in a function by looking at the code object name
                            if frame.code.name.starts_with("<fn:") {
                                // We're in a function - return locals
                                let mut locals_dict = HashMap::new();
                                for (i, varname) in frame.code.varnames.iter().enumerate() {
                                    if i < frame.locals.len() {
                                        locals_dict.insert(varname.clone(), frame.locals[i].value.clone());
                                    }
                                }
                                return Ok(Value::Dict(locals_dict));
                            } else {
                                // Module level - return globals from current frame
                                let globals_dict: HashMap<String, Value> = frame.globals.iter()
                                    .map(|(k, v)| (k.clone(), v.value.clone()))
                                    .collect();
                                return Ok(Value::Dict(globals_dict));
                            }
                        } else {
                            return Ok(Value::Dict(HashMap::new()));
                        }
                    }
                    "vars" => {
                        // vars() with no args is same as locals()
                        if args.is_empty() {
                            if let Some(frame) = self.frames.last() {
                                if frame.code.name.starts_with("<fn:") {
                                    let mut locals_dict = HashMap::new();
                                    for (i, varname) in frame.code.varnames.iter().enumerate() {
                                        if i < frame.locals.len() {
                                            locals_dict.insert(varname.clone(), frame.locals[i].value.clone());
                                        }
                                    }
                                    return Ok(Value::Dict(locals_dict));
                                } else {
                                    // Module level - return globals from current frame
                                    let globals_dict: HashMap<String, Value> = frame.globals.iter()
                                        .map(|(k, v)| (k.clone(), v.value.clone()))
                                        .collect();
                                    return Ok(Value::Dict(globals_dict));
                                }
                            } else {
                                return Ok(Value::Dict(HashMap::new()));
                            }
                        }
                        // vars(object) would return object.__dict__, but we don't support that yet
                        // Fall through to default handler
                    }
                    _ => {}
                }

                // Direct call to builtin function - fastest path
                (fptr)(args)
            }
            Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } => {
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

                // Debug output to see if we have compiled code
                // eprintln!("DEBUG: Function '{}' has compiled_code: {}", name, compiled_code.is_some());
                if let Some(ref code) = compiled_code {
                    // eprintln!("DEBUG: Function '{}' has {} instructions", name, code.instructions.len());
                } else {
                    // eprintln!("DEBUG: Function '{}' has no compiled code, will compile from body", name);
                    // eprintln!("DEBUG: Function '{}' body length: {}", name, body.len());
                }

                // Check if we have compiled code directly in the Closure
                if let Some(func_code) = compiled_code {
                    // Fast path: use the compiled code directly from the Closure
                    let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());

                    // Create optimized function frame
                    let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                    let mut frame = Frame::new_function_frame((**func_code).clone(), caller_globals_values, builtins_values, args);

                    // Set return register if this is a function call from bytecode
                    if let (Some(caller_idx), Some(reg)) = (caller_frame_idx, result_reg) {
                        frame.return_register = Some((caller_idx, reg));
                    }

                    self.frames.push(frame);
                    // Instead of calling run_frame recursively, we return None to continue execution
                    // The main execution loop will handle the new frame
                    return Ok(Value::None);
                }

                // If we don't have compiled code, but we have a body, compile it now
                if !body.is_empty() {
                    // eprintln!("DEBUG: Compiling function '{}' from body", name);
                    let func_code = SuperCompiler::compile_function(name.clone(), params, body)?;

                    // Create optimized function frame
                    let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());
                    let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                    let mut frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);

                    // Set return register if this is a function call from bytecode
                    if let (Some(caller_idx), Some(reg)) = (caller_frame_idx, result_reg) {
                        frame.return_register = Some((caller_idx, reg));
                    }

                    self.frames.push(frame);
                    // Instead of calling run_frame recursively, we return None to continue execution
                    // The main execution loop will handle the new frame
                    return Ok(Value::None);
                }

                // Check if we have a cached code object
                if let Some(func_code) = self.function_code_cache.get(&func).cloned() {
                    // Fast path: use cached code object
                    let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());

                    // Create optimized function frame
                    let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                    let mut frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);

                    // Set return register if this is a function call from bytecode
                    if let (Some(caller_idx), Some(reg)) = (caller_frame_idx, result_reg) {
                        frame.return_register = Some((caller_idx, reg));
                    }

                    self.frames.push(frame);
                    // Instead of calling run_frame recursively, we return None to continue execution
                    // The main execution loop will handle the new frame
                    return Ok(Value::None);
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
                let mut frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);

                // Set return register if this is a function call from bytecode
                if let (Some(caller_idx), Some(reg)) = (caller_frame_idx, result_reg) {
                    frame.return_register = Some((caller_idx, reg));
                }

                self.frames.push(frame);
                // Instead of calling run_frame recursively, we return None to continue execution
                // The main execution loop will handle the new frame
                Ok(Value::None)
            }
            Value::Class { ref name, ref bases, ref methods, ref metaclass, ref mro, ref base_object } => {
                // Class instantiation - implements Python's object creation protocol
                // Step 1: Call __new__ to create the instance (if it exists)
                // Step 2: Call __init__ to initialize the instance (if it exists)

                // Create a blank instance
                let mut instance = Value::Object {
                    class_name: name.clone(),
                    fields: HashMap::new(),
                    base_object: base_object.clone(),
                    mro: mro.clone(),
                };

                // Call __new__ if it exists
                if let Some(new_method) = methods.get("__new__") {
                    // __new__ receives the class as first argument, then the args
                    let mut new_args = vec![func.clone()];
                    new_args.extend(args.clone());

                    // Call __new__
                    instance = self.call_function_fast(new_method.clone(), new_args, None, None)?;
                }

                // Call __init__ if it exists
                if let Some(init_method) = methods.get("__init__") {
                    // __init__ receives the instance as first argument (self), then the args
                    let mut init_args = vec![instance.clone()];
                    init_args.extend(args);

                    // Call __init__ - it should return None and modify self
                    // Note: __init__ returns None, but may have side effects
                    // For now, we don't have mutable references in our bytecode VM,
                    // so __init__ can't actually modify the instance
                    // TODO: Implement proper self modification or return updated instance
                    self.call_function_fast(init_method.clone(), init_args, None, None)?;
                }

                // Return the instance
                Ok(instance)
            }
            Value::Object { .. } => {
                // Objects themselves are not callable (unless they have __call__)
                // TODO: Check for __call__ method
                Err(anyhow!("'{}' object is not callable", func.type_name()))
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
    
    fn builtin_str(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("str() takes exactly one argument"));
        }
        
        let string_repr = match &args[0] {
            Value::Str(s) => s.clone(), // Don't add quotes for str() conversion
            _ => format!("{}", args[0]), // Use Display trait
        };
        Ok(Value::Str(string_repr))
    }
    
    fn builtin_int(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("int() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(*n)),
            Value::Float(f) => Ok(Value::Int(*f as i64)),
            Value::Bool(b) => Ok(Value::Int(if *b { 1 } else { 0 })),
            Value::Str(s) => {
                s.trim().parse::<i64>()
                    .map(Value::Int)
                    .map_err(|_| anyhow!("invalid literal for int() with base 10: '{}'", s))
            }
            _ => Err(anyhow!("int() argument must be a string, a bytes-like object or a number, not '{}'", args[0].type_name())),
        }
    }
    
    fn builtin_float(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("float() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Float(f) => Ok(Value::Float(*f)),
            Value::Int(n) => Ok(Value::Float(*n as f64)),
            Value::Bool(b) => Ok(Value::Float(if *b { 1.0 } else { 0.0 })),
            Value::Str(s) => {
                s.trim().parse::<f64>()
                    .map(Value::Float)
                    .map_err(|_| anyhow!("could not convert string to float: '{}'", s))
            }
            _ => Err(anyhow!("float() argument must be a string or a number, not '{}'", args[0].type_name())),
        }
    }
    
    fn builtin_bool(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("bool() takes exactly one argument"));
        }
        
        Ok(Value::Bool(args[0].is_truthy()))
    }
    
    fn builtin_list(args: Vec<Value>) -> Result<Value> {
        if args.len() > 1 {
            return Err(anyhow!("list() takes at most 1 argument"));
        }
        
        if args.is_empty() {
            Ok(Value::List(HPList::new()))
        } else {
            args[0].to_list()
        }
    }
    
    fn builtin_sum(args: Vec<Value>) -> Result<Value> {
        if args.len() < 1 || args.len() > 2 {
            return Err(anyhow!("sum() takes at least 1 argument and at most 2 arguments"));
        }
        
        let iterable = &args[0];
        let start = if args.len() == 2 { &args[1] } else { &Value::Int(0) };
        
        match iterable {
            Value::List(items) => {
                let mut sum = start.clone();
                for item in items.as_vec() {
                    sum = match (&sum, item) {
                        (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                        (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                        (Value::Int(a), Value::Float(b)) => Value::Float(*a as f64 + b),
                        (Value::Float(a), Value::Int(b)) => Value::Float(a + *b as f64),
                        _ => return Err(anyhow!("unsupported operand type(s) for +")),
                    };
                }
                Ok(sum)
            }
            _ => Err(anyhow!("'{}' object is not iterable", iterable.type_name())),
        }
    }
    
    fn builtin_abs(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("abs() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => Ok(Value::Int(n.abs())),
            Value::Float(f) => Ok(Value::Float(f.abs())),
            _ => Err(anyhow!("bad operand type for abs(): '{}'", args[0].type_name())),
        }
    }
    
    fn builtin_chr(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("chr() takes exactly one argument"));
        }
        
        match &args[0] {
            Value::Int(n) => {
                if *n < 0 || *n > 0x10FFFF {
                    return Err(anyhow!("chr() arg not in range(0x110000)"));
                }
                Ok(Value::Str(String::from_utf8(vec![*n as u8]).unwrap_or_else(|_| String::from("\u{FFFD}"))))
            }
            _ => Err(anyhow!("chr() requires an integer argument")),
        }
    }
    
    fn builtin_isinstance(args: Vec<Value>) -> Result<Value> {
        if args.len() != 2 {
            return Err(anyhow!("isinstance() takes exactly 2 arguments"));
        }
        
        let obj = &args[0];
        let class_info = &args[1];
        
        // Simple isinstance implementation - for full implementation we'd need TypeHierarchy
        let result = match class_info {
            Value::Str(type_name) => obj.type_name() == type_name,
            _ => false, // For now, we only support string type names
        };
        
        Ok(Value::Bool(result))
    }
    
    fn builtin_type(args: Vec<Value>) -> Result<Value> {
        if args.len() != 1 {
            return Err(anyhow!("type() takes exactly one argument"));
        }

        Ok(Value::Str(args[0].type_name().to_string()))
    }

    fn builtin_dir(_args: Vec<Value>) -> Result<Value> {
        // Return common builtin names
        // In a full implementation, this would inspect the current namespace
        let mut items = vec![
            "__name__", "__builtins__", "builtins",
            "print", "len", "range", "str", "int", "float", "bool",
            "list", "dict", "tuple", "set",
            "sum", "abs", "chr", "type", "isinstance",
            "dir", "globals", "locals", "vars", "help",
        ];
        items.sort();

        let values: Vec<Value> = items.iter().map(|s| Value::Str(s.to_string())).collect();
        Ok(Value::List(crate::modules::hplist::HPList::from_values(values)))
    }

    fn builtin_globals(_args: Vec<Value>) -> Result<Value> {
        // Return a simple dict representation
        // In a full implementation, this would return actual globals from the current frame
        let mut dict = HashMap::new();
        dict.insert("__name__".to_string(), Value::Str("__main__".to_string()));
        dict.insert("args".to_string(), Value::List(crate::modules::hplist::HPList::new()));
        Ok(Value::Dict(dict))
    }

    fn builtin_locals(_args: Vec<Value>) -> Result<Value> {
        // Return a simple dict representation
        // In a full implementation, this would return actual locals from the current frame
        let dict = HashMap::new();
        Ok(Value::Dict(dict))
    }

    fn builtin_vars(_args: Vec<Value>) -> Result<Value> {
        // vars() without arguments is equivalent to locals()
        if _args.is_empty() {
            return Err(anyhow!("vars() requires an argument"));
        }
        // With an argument, return the object's __dict__
        Ok(Value::Dict(HashMap::new()))
    }

    fn builtin_help(_args: Vec<Value>) -> Result<Value> {
        // Simple help message
        println!("\nWelcome to Tauraro!\n");
        println!("Tauraro is a Python-compatible programming language with Rust-like performance.\n");
        println!("Type help() for interactive help, or help(object) for help about object.\n");
        println!("Quick Reference:");
        println!("  Variables:    x = 10");
        println!("  Functions:    def greet(name): return f'Hello, {{name}}'");
        println!("  Classes:      class MyClass: pass");
        println!("  Loops:        for i in range(10): print(i)");
        println!("  Conditions:   if x > 5: print('big')\n");
        println!("  Import:       import math\n");
        println!("Built-in Functions:");
        println!("  print()       Print values to stdout");
        println!("  input()       Read input from stdin");
        println!("  len()         Get length of sequence");
        println!("  range()       Generate range of numbers");
        println!("  type()        Get type of object");
        println!("  dir()         List attributes");
        println!("  help()        Show this help");
        println!("  exit()        Exit the REPL\n");
        Ok(Value::None)
    }
}

/// SuperCompiler - Register-based bytecode compiler with advanced optimizations
pub struct SuperCompiler {
    pub code: CodeObject,
    next_register: u32,
    current_line: u32,
}

impl SuperCompiler {
    pub fn new(filename: String) -> Self {
        Self {
            code: CodeObject::new(filename, "<module>".to_string(), 1),
            next_register: 0,
            current_line: 0,  // Start at 0, will increment to 1 for first statement
        }
    }
    
    /// Compile a function with the given name, parameters, and body
    pub fn compile_function(name: String, params: &[Param], body: &[Statement]) -> Result<CodeObject> {
        // Create a new compiler for the function
        let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));

        // Set the code name to the function name so is_in_function_scope() works
        func_compiler.code.name = format!("<fn:{}>", name);

        // Add parameters to the function's varnames
        for param in params {
            func_compiler.code.argcount = func_compiler.code.argcount + 1;
            func_compiler.code.add_varname(param.name.clone());
        }

        // Compile the function body
        for stmt in body.iter().cloned() {
            func_compiler.compile_statement(stmt)?;
        }
        
        // Add implicit return None at end of function if no return statement
        let none_const = func_compiler.code.add_constant(Value::None);
        let reg = func_compiler.allocate_register(); // Allocate a register
        func_compiler.emit(OpCode::LoadConst, none_const, reg, 0, 0);
        func_compiler.emit(OpCode::ReturnValue, reg, 0, 0, 0);
        
        // Set the number of registers needed for the function
        func_compiler.code.registers = func_compiler.next_register;
        func_compiler.code.nlocals = func_compiler.code.varnames.len() as u32;
        
        Ok(func_compiler.code)
    }
    
    /// Check if we're currently compiling a function (not a module)
    fn is_in_function_scope(&self) -> bool {
        // We're in a function if the code object name is not "<module>"
        self.code.name != "<module>"
    }
    
    /// Get or create local variable index (only for function scope)
    fn get_local_index(&mut self, name: &str) -> u32 {
        if let Some(pos) = self.code.varnames.iter().position(|n| n == name) {
            pos as u32
        } else {
            let pos = self.code.varnames.len() as u32;
            self.code.varnames.push(name.to_string());
            pos
        }
    }
    
    pub fn compile(&mut self, program: Program) -> Result<CodeObject> {
        for stmt in program.statements {
            // Increment line number for each statement
            self.current_line += 1;
            self.compile_statement(stmt)?;
        }

        // Set the number of registers needed for the module
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
                let reg = self.compile_expression(expr)?;
                // In module scope, save expression result to special global for REPL
                if !self.is_in_function_scope() {
                    let name_idx = self.code.add_name("__last_expr__".to_string());
                    self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);
                }
                Ok(())
            }
            Statement::VariableDef { name, value, .. } => {
                if let Some(expr) = value {
                    let value_reg = self.compile_expression(expr)?;
                    // Store in local variable if in function scope, otherwise global
                    if self.is_in_function_scope() {
                        // We're in a function scope, use fast local access
                        let local_idx = self.get_local_index(&name);
                        self.emit(OpCode::StoreFast, local_idx, value_reg, 0, self.current_line);
                    } else {
                        // Global scope - use StoreGlobal
                        let name_idx = self.code.add_name(name);
                        self.emit(OpCode::StoreGlobal, name_idx, value_reg, 0, self.current_line);
                    }
                } else {
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    if self.is_in_function_scope() {
                        // We're in a function scope, use fast local access
                        let local_idx = self.get_local_index(&name);
                        self.emit(OpCode::StoreFast, local_idx, reg, 0, self.current_line);
                    } else {
                        // Global scope - use StoreGlobal
                        let name_idx = self.code.add_name(name);
                        self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);
                    }
                }
                Ok(())
            }
            Statement::SubscriptAssignment { object, index, value } => {
                let object_reg = self.compile_expression(object)?;
                let index_reg = self.compile_expression(index)?;
                let value_reg = self.compile_expression(value)?;
                
                // Emit SubscrStore instruction to store item to sequence
                self.emit(OpCode::SubscrStore, object_reg, index_reg, value_reg, self.current_line);
                Ok(())
            }
            Statement::Return(expr) => {
                if let Some(expr) = expr {
                    let value_reg = self.compile_expression(expr)?;
                    self.emit(OpCode::ReturnValue, value_reg, 0, 0, self.current_line);
                } else {
                    let none_const = self.code.add_constant(Value::None);
                    let reg = self.allocate_register();
                    self.emit(OpCode::LoadConst, none_const, reg, 0, self.current_line);
                    self.emit(OpCode::ReturnValue, reg, 0, 0, self.current_line);
                }
                Ok(())
            }
            Statement::FunctionDef { name, params, body, .. } => {
                // Create a new compiler for the function
                let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));

                // CRITICAL: Set code.name so is_in_function_scope() works correctly
                func_compiler.code.name = format!("<fn:{}>", name);

                // Add parameters to the function's varnames
                for param in &params {
                    func_compiler.code.argcount = func_compiler.code.argcount + 1;
                    func_compiler.code.add_varname(param.name.clone());
                }
                
                // Compile the function body
                for stmt in body.clone() {
                    func_compiler.compile_statement(stmt)?;
                }
                
                // Add implicit return None at end of function if no return statement
                let none_const = func_compiler.code.add_constant(Value::None);
                let reg = func_compiler.allocate_register(); // Allocate a register
                func_compiler.emit(OpCode::LoadConst, none_const, reg, 0, 0);
                func_compiler.emit(OpCode::ReturnValue, reg, 0, 0, 0);
                
                // Set the number of registers needed for the function
                func_compiler.code.registers = func_compiler.next_register;
                func_compiler.code.nlocals = func_compiler.code.varnames.len() as u32;
                
                // Get the compiled function code
                let func_code = func_compiler.code;
                
                // Debug output to see the compiled code
                // eprintln!("DEBUG: Compiled function '{}' with {} instructions", name, func_code.instructions.len());
                
                // Create a closure value for the function with the compiled code
                let closure_value = Value::Closure {
                    name: name.clone(),
                    params: params.clone(),
                    body: vec![], // Body is encoded in the bytecode, not stored as AST
                    captured_scope: HashMap::new(), // No captured scope for now
                    docstring: None,
                    compiled_code: Some(Box::new(func_code.clone())), // Store the compiled code directly in the Closure
                };
                
                // Debug output to see if compiled_code is set
                if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = closure_value {
                    // eprintln!("DEBUG: Created Closure '{}' with compiled_code: {}", name, compiled_code.is_some());
                    if let Some(ref code) = compiled_code {
                        // eprintln!("DEBUG: Created Closure '{}' has {} instructions", name, code.instructions.len());
                    }
                }
                
                // Store the closure in constants
                let closure_const_idx = self.code.add_constant(closure_value);
                
                // Load the closure
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, closure_const_idx, reg, 0, self.current_line);
                
                // Store the function in global namespace
                let name_idx = self.code.add_name(name.clone());
                self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);
                
                // Debug output to see what's stored in constants
                // eprintln!("DEBUG: Stored Closure '{}' in constants at index {}", name, closure_const_idx);
                if let Some(stored_value) = self.code.constants.get(closure_const_idx as usize) {
                    if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = stored_value {
                        // eprintln!("DEBUG: Stored Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                        if let Some(ref code) = compiled_code {
                            // eprintln!("DEBUG: Stored Closure '{}' has {} instructions", name, code.instructions.len());
                        }
                    }
                }
                Ok(())
            }
            Statement::For { variable, iterable, body, .. } => {
                // Compile for loop: for variable in iterable:
                
                // 1. Compile the iterable expression
                let iterable_reg = self.compile_expression(iterable)?;
                
                // 2. Get an iterator from the iterable
                let iter_reg = self.allocate_register();
                self.emit(OpCode::GetIter, iterable_reg, iter_reg, 0, self.current_line);
                
                // 3. Set up the loop structure
                let loop_var_idx = if self.is_in_function_scope() {
                    // In function scope, use fast local access
                    self.get_local_index(&variable)
                } else {
                    // In global scope, we don't add to varnames, just get the name index
                    self.code.add_name(variable.clone())
                };
                
                // 4. Create jump targets
                let loop_start = self.code.instructions.len(); // Start of loop body
                
                // 5. Emit ForIter instruction with placeholder for end target
                let value_reg = self.allocate_register();
                let for_iter_pos = self.emit(OpCode::ForIter, iter_reg, value_reg, 0, self.current_line); // arg3 will be updated later
                
                // 6. Store the iterated value in the loop variable
                if self.is_in_function_scope() {
                    // In function scope, use fast local access
                    self.emit(OpCode::StoreFast, loop_var_idx, value_reg, 0, self.current_line);
                } else {
                    // In global scope, use StoreGlobal
                    self.emit(OpCode::StoreGlobal, loop_var_idx, value_reg, 0, self.current_line);
                }
                
                // 7. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 8. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, self.current_line);
                
                // 9. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                // Update the ForIter instruction with the correct target
                self.code.instructions[for_iter_pos].arg3 = loop_end_pos as u32;
                Ok(())
            }
            Statement::While { condition, body, .. } => {
                // Compile while loop: while condition: body
                
                // 1. Create jump targets
                let loop_start = self.code.instructions.len(); // Start of condition check
                let loop_body = loop_start + 1; // Start of loop body (after condition check)
                
                // 2. Compile the condition
                let cond_reg = self.compile_expression(condition)?;
                
                // 3. Emit conditional jump to end of loop if condition is false
                let loop_end_pos_ref = self.emit(OpCode::JumpIfFalse, cond_reg, 0, 0, self.current_line); // arg2 will be updated later
                
                // 4. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 5. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, self.current_line);
                
                // 6. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                self.code.instructions[loop_end_pos_ref].arg2 = loop_end_pos as u32;
                Ok(())
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                // Compile if statement: if condition: then_branch elif ... else: else_branch
                
                // 1. Compile the condition
                let cond_reg = self.compile_expression(condition)?;
                
                // 2. Emit conditional jump to else branch if condition is false
                let else_jump_pos = self.emit(OpCode::JumpIfFalse, cond_reg, 0, 0, self.current_line); // arg2 will be updated later
                
                // 3. Compile the then branch
                for stmt in then_branch {
                    self.compile_statement(stmt)?;
                }
                
                // 4. Emit jump to end of if statement (after then branch)
                let end_jump_pos = self.emit(OpCode::Jump, 0, 0, 0, self.current_line); // arg1 will be updated later
                
                // 5. Fix the else jump placeholder
                let else_start_pos = self.code.instructions.len();
                self.code.instructions[else_jump_pos].arg2 = else_start_pos as u32;
                
                // 6. Compile elif branches and else branch
                let mut elif_jump_positions = Vec::new();
                
                // Compile elif branches
                for (elif_cond, elif_body) in elif_branches {
                    // Compile elif condition
                    let elif_cond_reg = self.compile_expression(elif_cond)?;
                    
                    // Emit conditional jump to next elif/else branch if condition is false
                    let elif_else_jump_pos = self.emit(OpCode::JumpIfFalse, elif_cond_reg, 0, 0, self.current_line);
                    elif_jump_positions.push(elif_else_jump_pos);
                    
                    // Compile elif body
                    for stmt in elif_body {
                        self.compile_statement(stmt)?;
                    }
                    
                    // Emit jump to end of if statement
                    let elif_end_jump_pos = self.emit(OpCode::Jump, 0, 0, 0, self.current_line);
                    elif_jump_positions.push(elif_end_jump_pos);
                }
                
                // Compile else branch if it exists
                if let Some(else_body) = else_branch {
                    for stmt in else_body {
                        self.compile_statement(stmt)?;
                    }
                }
                
                // 7. Fix all the jump placeholders
                let end_pos = self.code.instructions.len();
                self.code.instructions[end_jump_pos].arg1 = end_pos as u32;
                
                for jump_pos in elif_jump_positions {
                    if self.code.instructions[jump_pos].opcode == OpCode::JumpIfFalse {
                        self.code.instructions[jump_pos].arg2 = end_pos as u32;
                    } else {
                        self.code.instructions[jump_pos].arg1 = end_pos as u32;
                    }
                }
                Ok(())
            }
            Statement::ClassDef { name, bases, body, decorators: _, metaclass, docstring: _ } => {
                // Create class object with methods
                let mut class_methods = HashMap::new();

                // Process class body to extract methods and attributes
                for stmt in body {
                    if let Statement::FunctionDef { name: method_name, params, return_type: _, body: method_body, is_async: _, decorators: _, docstring } = stmt {
                        // Compile the method using the compile_function helper
                        let method_code = SuperCompiler::compile_function(method_name.clone(), &params, &method_body)?;

                        let method_value = Value::Closure {
                            name: method_name.clone(),
                            params: params.clone(),
                            body: vec![], // Body is encoded in the bytecode, not stored as AST
                            captured_scope: HashMap::new(),
                            docstring: docstring.clone(),
                            compiled_code: Some(Box::new(method_code)), // Store the compiled code directly in the Closure
                        };
                        class_methods.insert(method_name.clone(), method_value);
                    }
                }

                // Extract base class names
                // For now, we only support simple identifier bases
                let mut base_names = Vec::new();
                for base_expr in bases {
                    if let Expr::Identifier(base_name) = base_expr {
                        base_names.push(base_name.clone());
                    }
                }

                // If no bases specified, inherit from object
                if base_names.is_empty() {
                    base_names.push("object".to_string());
                }

                // Build MRO (Method Resolution Order) using C3 linearization
                // For now, simple linearization: [self, bases..., object]
                let mut mro_list = vec![name.clone()];
                for base in &base_names {
                    if base != "object" && !mro_list.contains(base) {
                        mro_list.push(base.clone());
                    }
                }
                if !mro_list.contains(&"object".to_string()) {
                    mro_list.push("object".to_string());
                }

                // Extract metaclass name if provided
                let metaclass_name = if let Some(mc_expr) = metaclass {
                    if let Expr::Identifier(mc_name) = mc_expr {
                        Some(mc_name.clone())
                    } else {
                        None
                    }
                } else {
                    Some("type".to_string()) // Default metaclass is 'type'
                };

                // Create the class using the new Class variant
                let class_obj = Value::Class {
                    name: name.clone(),
                    bases: base_names.clone(),
                    methods: class_methods,
                    metaclass: metaclass_name,
                    mro: crate::base_object::MRO::from_linearization(mro_list.clone()),
                    base_object: crate::base_object::BaseObject::new(name.clone(), base_names.clone()),
                };

                // Store class as a constant, load it, and store in global namespace
                let class_const_idx = self.code.add_constant(class_obj);
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, class_const_idx, reg, 0, self.current_line);

                let name_idx = self.code.add_name(name.clone());
                self.emit(OpCode::StoreGlobal, name_idx, reg, 0, self.current_line);

                Ok(())
            }
            _ => {
                // For unimplemented statements, we'll just return Ok for now
                // In a complete implementation, we would handle all statement types
                Ok(())
            }
        }
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
                self.emit(OpCode::LoadConst, const_idx, reg, 0, self.current_line);
                Ok(reg)
            }
            Expr::Identifier(name) => {
                let reg = self.allocate_register();

                // Check if this is a local variable (only in function scope)
                if self.is_in_function_scope() {
                    // In function scope, check if this is a local variable
                    if let Some(local_idx) = self.code.varnames.iter().position(|n| n == &name) {
                        // For local variables in function scope, use fast access
                        self.emit(OpCode::LoadFast, local_idx as u32, reg, 0, self.current_line);
                    } else {
                        // Not a local variable, treat as global
                        let name_idx = self.code.add_name(name);
                        let cache_idx = self.code.add_inline_cache();
                        self.emit(OpCode::LoadGlobal, name_idx, cache_idx, reg, self.current_line);
                    }
                } else {
                    // Global scope - always treat as global variable
                    let name_idx = self.code.add_name(name);
                    let cache_idx = self.code.add_inline_cache();
                    self.emit(OpCode::LoadGlobal, name_idx, cache_idx, reg, self.current_line);
                }
                Ok(reg)
            }
            Expr::BinaryOp { left, op, right } => {
                let left_reg = self.compile_expression(*left)?;
                let right_reg = self.compile_expression(*right)?;
                let result_reg = self.allocate_register();
                
                // Check if both operands are likely to be integers for fast path
                let use_fast_int = match op {
                    BinaryOp::Add | BinaryOp::Sub | BinaryOp::Mul | BinaryOp::Div | BinaryOp::Mod => {
                        // Use fast path for common arithmetic operations
                        true
                    }
                    _ => false
                };
                
                if use_fast_int {
                    let opcode = match op {
                        BinaryOp::Add => OpCode::FastIntAdd,
                        BinaryOp::Sub => OpCode::FastIntSub,
                        BinaryOp::Mul => OpCode::FastIntMul,
                        BinaryOp::Div => OpCode::BinaryDivRRFastInt,
                        BinaryOp::Mod => OpCode::BinaryModRRFastInt,
                        _ => OpCode::BinaryAddRR, // fallback
                    };
                    self.emit(opcode, left_reg, right_reg, result_reg, self.current_line);
                } else {
                    let opcode = match op {
                        BinaryOp::Add => OpCode::BinaryAddRR,
                        BinaryOp::Sub => OpCode::BinarySubRR,
                        BinaryOp::Mul => OpCode::BinaryMulRR,
                        BinaryOp::Div => OpCode::BinaryDivRR,
                        BinaryOp::Mod => OpCode::BinaryModRR,
                        BinaryOp::Pow => OpCode::BinaryPowRR,
                        BinaryOp::Eq => OpCode::CompareEqualRR,
                        BinaryOp::Ne | BinaryOp::Neq => OpCode::CompareNotEqualRR,
                        BinaryOp::Lt => OpCode::CompareLessRR,
                        BinaryOp::Gt => OpCode::CompareGreaterRR,
                        BinaryOp::Le | BinaryOp::Lte => OpCode::CompareLessEqualRR,
                        BinaryOp::Ge | BinaryOp::Gte => OpCode::CompareGreaterEqualRR,
                        BinaryOp::And => {
                            // Short-circuit AND: if left is false, return left, otherwise return right
                            // This is a simplified implementation
                            self.emit(OpCode::BinaryMulRR, left_reg, right_reg, result_reg, self.current_line);
                            return Ok(result_reg);
                        },
                        BinaryOp::Or => {
                            // Short-circuit OR: if left is true, return left, otherwise return right
                            // This is a simplified implementation
                            self.emit(OpCode::BinaryAddRR, left_reg, right_reg, result_reg, self.current_line);
                            return Ok(result_reg);
                        },
                        _ => return Err(anyhow!("Unsupported binary operation: {:?}", op)),
                    };
                    
                    self.emit(opcode, left_reg, right_reg, result_reg, self.current_line);
                }
                Ok(result_reg)
            }
            Expr::Call { func, args, .. } => {
                let func_reg = self.compile_expression(*func)?;

                // Compile all arguments first
                let mut compiled_arg_regs = Vec::new();
                for arg in args {
                    let arg_reg = self.compile_expression(arg)?;
                    compiled_arg_regs.push(arg_reg);
                }

                // CRITICAL: Move arguments to consecutive registers starting from func_reg + 1
                // The CallFunction handler expects arguments in consecutive registers
                for (i, &arg_reg) in compiled_arg_regs.iter().enumerate() {
                    let target_reg = func_reg + 1 + i as u32;
                    if arg_reg != target_reg {
                        // Only emit LoadLocal if the register is different
                        self.emit(OpCode::LoadLocal, arg_reg, target_reg, 0, self.current_line);
                    }
                }

                let result_reg = self.allocate_register();
                self.emit(OpCode::CallFunction, func_reg, compiled_arg_regs.len() as u32, result_reg, self.current_line);

                Ok(result_reg)
            }
            Expr::Compare { left, ops, comparators } => {
                // For now, we'll just handle the first comparison operation
                // A full implementation would handle chained comparisons
                if ops.len() != 1 || comparators.len() != 1 {
                    return Err(anyhow!("Chained comparisons not yet supported"));
                }
                
                let left_reg = self.compile_expression(*left)?;
                let right_reg = self.compile_expression(comparators[0].clone())?;
                let result_reg = self.allocate_register();
                
                let opcode = match ops[0] {
                    CompareOp::Eq => OpCode::CompareEqualRR,
                    CompareOp::NotEq => OpCode::CompareNotEqualRR,
                    CompareOp::Lt => OpCode::CompareLessRR,
                    CompareOp::LtE => OpCode::CompareLessEqualRR,
                    CompareOp::Gt => OpCode::CompareGreaterRR,
                    CompareOp::GtE => OpCode::CompareGreaterEqualRR,
                    _ => return Err(anyhow!("Unsupported comparison operation: {:?}", ops[0])),
                };
                
                self.emit(opcode, left_reg, right_reg, result_reg, self.current_line);
                Ok(result_reg)
            }
            Expr::List(items) => {
                // Compile each item and store in consecutive registers
                let mut item_regs = Vec::new();
                for item in items {
                    let item_reg = self.compile_expression(item)?;
                    item_regs.push(item_reg);
                }

                let result_reg = self.allocate_register();

                // Use the BuildList opcode to create a list with the items
                // arg1 = number of items, arg2 = first item register, arg3 = result register
                let first_reg = if item_regs.is_empty() { 0 } else { item_regs[0] };
                self.emit(OpCode::BuildList, item_regs.len() as u32, first_reg, result_reg, self.current_line);

                Ok(result_reg)
            }
            Expr::UnaryOp { op, operand } => {
                let operand_reg = self.compile_expression(*operand)?;
                let result_reg = self.allocate_register();
                
                match op {
                    UnaryOp::USub => {
                        // For unary minus, we multiply by -1
                        let neg_one_const = self.code.add_constant(Value::Int(-1));
                        let neg_one_reg = self.allocate_register();
                        self.emit(OpCode::LoadConst, neg_one_const, neg_one_reg, 0, self.current_line);
                        self.emit(OpCode::BinaryMulRR, operand_reg, neg_one_reg, result_reg, self.current_line);
                    }
                    UnaryOp::UAdd => {
                        // For unary plus, we just return the operand
                        self.emit(OpCode::LoadLocal, operand_reg, result_reg, 0, self.current_line);
                    }
                    UnaryOp::Not => {
                        // For logical not, we need to implement this
                        // For now, we'll just load the operand as a placeholder
                        self.emit(OpCode::LoadLocal, operand_reg, result_reg, 0, self.current_line);
                    }
                    _ => {
                        return Err(anyhow!("Unsupported unary operation: {:?}", op));
                    }
                }
                
                Ok(result_reg)
            }
            Expr::Subscript { object, index } => {
                let object_reg = self.compile_expression(*object)?;
                let index_reg = self.compile_expression(*index)?;
                let result_reg = self.allocate_register();
                
                // Emit SubscrLoad instruction to load item from sequence
                self.emit(OpCode::SubscrLoad, object_reg, index_reg, result_reg, self.current_line);
                
                Ok(result_reg)
            }
            Expr::MethodCall { object, method, args, .. } => {
                // Check if object is an identifier (variable) so we can update it after mutating method calls
                let object_var_name = if let Expr::Identifier(ref name) = *object {
                    Some(name.clone())
                } else {
                    None
                };

                let object_reg = self.compile_expression(*object)?;

                // Compile all arguments first
                let mut compiled_arg_regs = Vec::new();
                for arg in args.iter() {
                    let arg_reg = self.compile_expression(arg.clone())?;
                    compiled_arg_regs.push(arg_reg);
                }

                // CRITICAL: Move arguments to consecutive registers starting from object_reg + 1
                // The CallMethod handler expects arguments in consecutive registers
                for (i, &arg_reg) in compiled_arg_regs.iter().enumerate() {
                    let target_reg = object_reg + 1 + i as u32;
                    if arg_reg != target_reg {
                        // Only emit LoadLocal if the register is different
                        self.emit(OpCode::LoadLocal, arg_reg, target_reg, 0, self.current_line);
                    }
                }

                let method_name_idx = self.code.add_name(method.clone());

                // Allocate result register BEFORE calling the method
                let result_reg = self.allocate_register();

                // Emit CallMethod instruction
                // We'll use a new calling convention where the result is stored in a dedicated register
                // For now, we'll store the object_reg and method will update it, then we'll load the result
                // arg1: object register (also used for storing result temporarily)
                // arg2: number of arguments
                // arg3: method name index
                self.emit(OpCode::CallMethod, object_reg, compiled_arg_regs.len() as u32, method_name_idx, self.current_line);

                // CRITICAL FIX: For mutating methods, store the modified object back to the variable
                // This ensures that mutations persist (e.g., list.append modifies the list)
                if let Some(var_name) = object_var_name {
                    let mutating_methods = vec!["append", "extend", "insert", "remove", "pop", "clear", "sort", "reverse"];
                    if mutating_methods.contains(&method.as_str()) {
                        let name_idx = self.code.add_name(var_name);
                        self.emit(OpCode::StoreGlobal, name_idx, object_reg, 0, self.current_line);
                    }
                }

                // Load the result from the object register (CallMethod stores result there)
                // We use LoadLocal to copy it to the result register
                self.emit(OpCode::LoadLocal, object_reg, result_reg, 0, self.current_line);
                Ok(result_reg)
            }
            Expr::Attribute { object, name } => {
                // Attribute access: object.name
                // For now, we compile the object and return a placeholder
                // TODO: Implement proper LoadAttr opcode and runtime support
                let _object_reg = self.compile_expression(*object)?;

                // For now, just return None as a placeholder
                // This allows class methods to compile even if they reference self.x
                let result_reg = self.allocate_register();
                let none_const = self.code.add_constant(Value::None);
                self.emit(OpCode::LoadConst, none_const, result_reg, 0, self.current_line);
                Ok(result_reg)
            }
            _ => Err(anyhow!("Unsupported expression type: {:?}", expr)),
        }
    }
}
