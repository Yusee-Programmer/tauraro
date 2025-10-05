//! Arithmetic + logical ops (ADD, SUB, MUL, DIV, AND, OR, etc.)

use crate::ast::*;
use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::{OpCode, Instruction};
use anyhow::{Result, anyhow};
use std::collections::HashMap;
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
        // Debug output to see the code object being used
        eprintln!("DEBUG: Frame::new creating frame with code object '{}' with {} instructions", code.name, code.instructions.len());
        
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
        
        // Initialize locals map
        let mut locals = HashMap::new();
        
        // Copy arguments to registers and locals
        for (i, (arg, param_name)) in args.into_iter().zip(code.varnames.iter()).enumerate() {
            if i < registers.len() {
                let rc_arg = RcValue::new(arg);
                registers[i] = rc_arg.clone();
                locals.insert(param_name.clone(), rc_arg);
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
        let mut builtins = HashMap::new();
        builtins.insert("print".to_string(), RcValue::new(Value::BuiltinFunction("print".to_string(), Self::builtin_print)));
        builtins.insert("len".to_string(), RcValue::new(Value::BuiltinFunction("len".to_string(), Self::builtin_len)));
        builtins.insert("range".to_string(), RcValue::new(Value::BuiltinFunction("range".to_string(), Self::builtin_range)));
        builtins.insert("str".to_string(), RcValue::new(Value::BuiltinFunction("str".to_string(), Self::builtin_str)));
        builtins.insert("int".to_string(), RcValue::new(Value::BuiltinFunction("int".to_string(), Self::builtin_int)));
        builtins.insert("float".to_string(), RcValue::new(Value::BuiltinFunction("float".to_string(), Self::builtin_float)));
        builtins.insert("bool".to_string(), RcValue::new(Value::BuiltinFunction("bool".to_string(), Self::builtin_bool)));
        builtins.insert("list".to_string(), RcValue::new(Value::BuiltinFunction("list".to_string(), Self::builtin_list)));
        builtins.insert("sum".to_string(), RcValue::new(Value::BuiltinFunction("sum".to_string(), Self::builtin_sum)));
        builtins.insert("abs".to_string(), RcValue::new(Value::BuiltinFunction("abs".to_string(), Self::builtin_abs)));
        builtins.insert("chr".to_string(), RcValue::new(Value::BuiltinFunction("chr".to_string(), Self::builtin_chr)));
        builtins.insert("isinstance".to_string(), RcValue::new(Value::BuiltinFunction("isinstance".to_string(), Self::builtin_isinstance)));
        builtins.insert("type".to_string(), RcValue::new(Value::BuiltinFunction("type".to_string(), Self::builtin_type)));
        
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
        // Debug output to see the code object being executed
        eprintln!("DEBUG: execute called with code object '{}' with {} instructions", code.name, code.instructions.len());
        
        // Convert globals and builtins from RcValue to Value for Frame::new
        let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
        let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
        
        let frame = Frame::new(code, globals_values, builtins_values);
        self.frames.push(frame);
        
        eprintln!("DEBUG: About to call run_frame, frames.len(): {}", self.frames.len());
        let result = self.run_frame()?;
        eprintln!("DEBUG: run_frame completed with result: {:?}", result);
        
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
                eprintln!("DEBUG: run_frame - no frames, returning None");
                return Ok(Value::None);
            }
            
            // Update frame index in case frames were added/removed
            frame_idx = self.frames.len() - 1;
            
            // Debug output to see the frame being executed
            eprintln!("DEBUG: run_frame executing frame {} with code object '{}' with {} instructions", 
                      frame_idx, self.frames[frame_idx].code.name, self.frames[frame_idx].code.instructions.len());
            
            // Safety check: if there are no instructions, return None immediately
            if self.frames[frame_idx].code.instructions.is_empty() {
                eprintln!("DEBUG: Code object '{}' has no instructions, returning None", self.frames[frame_idx].code.name);
                self.frames.pop();
                return Ok(Value::None);
            }
            
            // Fast path: check bounds
            let pc = self.frames[frame_idx].pc;
            let instructions_len = self.frames[frame_idx].code.instructions.len();
            
            eprintln!("DEBUG: PC: {}, Instructions len: {}", pc, instructions_len);
            
            if pc >= instructions_len {
                eprintln!("DEBUG: PC >= instructions_len, breaking");
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
            
            eprintln!("DEBUG: Executing instruction at pc {}: {:?} (arg1: {}, arg2: {}, arg3: {})", pc, opcode, arg1, arg2, arg3);
            // Execute instruction using computed GOTOs for maximum performance
            match self.execute_instruction_fast(frame_idx, opcode, arg1, arg2, arg3) {
                Ok(Some(value)) => {
                    eprintln!("DEBUG: Instruction returned value: {:?}", value);
                    // Check if we have more frames to execute
                    if self.frames.is_empty() {
                        return Ok(value);
                    }
                    // If we still have frames, continue execution with the next frame
                    continue;
                },
                Ok(None) => {
                    eprintln!("DEBUG: Instruction completed, frame_idx: {}, frames.len(): {}", frame_idx, self.frames.len());
                    // Check if a new frame was pushed during execution
                    if self.frames.len() > frame_idx + 1 {
                        // A new frame was pushed, continue execution with the new frame
                        continue;
                    }
                    // Only increment PC if frame still exists and PC hasn't changed
                    if frame_idx < self.frames.len() {
                        // Check if PC has changed (e.g., due to a jump)
                        eprintln!("DEBUG: PC before: {}, PC after: {}", pc, self.frames[frame_idx].pc);
                        if self.frames[frame_idx].pc == pc {
                            self.frames[frame_idx].pc += 1;
                            eprintln!("DEBUG: Incremented PC to {}", self.frames[frame_idx].pc);
                        } else {
                            eprintln!("DEBUG: PC was changed by instruction, now {}", self.frames[frame_idx].pc);
                        }
                        // If PC has changed, we don't increment it
                    }
                },
                Err(e) => {
                    eprintln!("DEBUG: Instruction failed with error: {}", e);
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
        
        eprintln!("DEBUG: run_frame - completed, returning None");
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
                eprintln!("DEBUG: LoadConst - const_idx: {}, result_reg: {}", const_idx, arg2);
                // Check bounds before accessing
                if const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadConst: constant index {} out of bounds (len: {})", const_idx, self.frames[frame_idx].code.constants.len()));
                }
                let value = RcValue::new(self.frames[frame_idx].code.constants[const_idx].clone());
                eprintln!("DEBUG: LoadConst - loaded value: {:?}", value.value);
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
                
                eprintln!("DEBUG: LoadGlobal - loading '{}' (name_idx: {}, cache_idx: {}, result_reg: {})", name, name_idx, arg2, arg3);
                
                // First check locals (for function parameters and local variables)
                let local_value = self.frames[frame_idx].locals.get(name).cloned();
                if let Some(value) = local_value {
                    eprintln!("DEBUG: LoadGlobal - found '{}' in locals: {:?}", name, value.value);
                    self.frames[frame_idx].set_register(arg3, value);
                    return Ok(None);
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
                        eprintln!("DEBUG: Using inline cache for '{}': {:?}", name, value.value);
                        // Additional debug output for Closure values from cache
                        if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = value.value {
                            eprintln!("DEBUG: Cached Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                            if let Some(ref code) = compiled_code {
                                eprintln!("DEBUG: Cached Closure '{}' has {} instructions", name, code.instructions.len());
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
                    eprintln!("DEBUG: Found '{}' in globals: {:?}", name, value.value);
                    // Additional debug output for Closure values
                    if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = value.value {
                        eprintln!("DEBUG: Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                        if let Some(ref code) = compiled_code {
                            eprintln!("DEBUG: Closure '{}' has {} instructions", name, code.instructions.len());
                        }
                    }
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
                    // Check built-ins if global not found
                    let builtin_value = self.frames[frame_idx].builtins.get(name).cloned();
                    if let Some(value) = builtin_value {
                        // Debug output to see what builtin value we found
                        eprintln!("DEBUG: Found '{}' in built-ins: {:?}", name, value.value);
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
                        // Debug output to see what built-ins are available
                        let available_builtins: Vec<String> = self.frames[frame_idx].builtins.keys().cloned().collect();
                        eprintln!("DEBUG: '{}' not found in locals, globals or built-ins. Available built-ins: {:?}", name, available_builtins);
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
                // Debug output to see what's being stored
                eprintln!("DEBUG: Storing '{}' in globals: {:?}", name, value.value);
                if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = value.value {
                    eprintln!("DEBUG: Storing Closure '{}' with compiled_code: {}", name, compiled_code.is_some());
                    if let Some(ref code) = compiled_code {
                        eprintln!("DEBUG: Storing Closure '{}' has {} instructions", name, code.instructions.len());
                    }
                }
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
                if reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("ReturnValue: register index out of bounds"));
                }
                let value = self.frames[frame_idx].registers[reg as usize].clone();
                self.frames.pop();
                Ok(Some(value.value))
            }
            OpCode::CallFunction => {
                // Call a function with arguments
                let func_reg = arg1;
                let arg_count = arg2 as usize;
                let result_reg = arg3;
                
                eprintln!("DEBUG: CallFunction - func_reg: {}, arg_count: {}, result_reg: {}", func_reg, arg_count, result_reg);
                
                // Get the function from the register
                if func_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunction: function register index out of bounds"));
                }
                let func_value = self.frames[frame_idx].registers[func_reg as usize].clone();
                eprintln!("DEBUG: CallFunction - func_value: {:?}", func_value);
                
                // Collect arguments from consecutive registers
                let mut args = Vec::new();
                for i in 0..arg_count {
                    let arg_reg = func_reg + 1 + i as u32;
                    if arg_reg as usize >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallFunction: argument register index out of bounds"));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg as usize].value.clone());
                }
                eprintln!("DEBUG: CallFunction - args: {:?}", args);
                
                // Call the function
                let result = self.call_function_fast(func_value.value, args)?;
                eprintln!("DEBUG: CallFunction - result: {:?}", result);
                self.frames[frame_idx].set_register(result_reg, RcValue::new(result));
                Ok(None)
            }
            OpCode::CallMethod => {
                // Call a method on an object
                let object_reg = arg1;
                let arg_count = arg2 as usize;
                let method_name_idx = arg3 as usize;
                
                eprintln!("DEBUG: CallMethod - object_reg: {}, arg_count: {}, method_name_idx: {}", object_reg, arg_count, method_name_idx);
                
                // Get the object from the register
                if object_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallMethod: object register index out of bounds"));
                }
                
                // Clone the object value to avoid borrowing issues
                let object_value = self.frames[frame_idx].registers[object_reg as usize].value.clone();
                
                // Get the method name
                if method_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("CallMethod: method name index out of bounds"));
                }
                let method_name = self.frames[frame_idx].code.names[method_name_idx].clone();
                
                eprintln!("DEBUG: CallMethod - object_value: {:?}, method_name: {}", object_value, method_name);
                
                // Handle method calls on specific types
                match object_value {
                    Value::List(mut list) => {
                        match method_name.as_str() {
                            "append" => {
                                // For append, we need to get the argument
                                if arg_count != 1 {
                                    return Err(anyhow!("append method expects exactly 1 argument, got {}", arg_count));
                                }
                                
                                // The argument should be in the next register after the object register
                                let arg_reg = object_reg + 1;
                                if arg_reg as usize >= self.frames[frame_idx].registers.len() {
                                    return Err(anyhow!("CallMethod: append argument register out of bounds"));
                                }
                                
                                let arg_value = self.frames[frame_idx].registers[arg_reg as usize].value.clone();
                                eprintln!("DEBUG: CallMethod - arg_value: {:?}", arg_value);
                                list.append(arg_value);
                                eprintln!("DEBUG: CallMethod - list after append: {:?}", list);
                                
                                // Update the object register with the modified list
                                self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(Value::List(list));
                                eprintln!("DEBUG: CallMethod - updated register {} with new list", object_reg);
                                
                                // Method calls like append return None
                                Ok(None)
                            },
                            _ => Err(anyhow!("Method '{}' not found on List", method_name)),
                        }
                    },
                    _ => Err(anyhow!("Method calls not supported on this type")),
                }
            }
            OpCode::BuildList => {
                // Build a list from items
                let item_count = arg1 as usize;
                let result_reg = arg3;
                
                // Collect items from consecutive registers
                // For now, we'll assume items are in the registers just before this instruction
                // In a full implementation, we would track where the item registers are
                let mut list = crate::modules::hplist::HPList::new();
                
                // For now, we'll just create an empty list since we don't have a good way to track
                // where the item registers are without changing the compiler significantly
                // A full implementation would collect the actual items from registers
                
                let value = Value::List(list);
                
                self.frames[frame_idx].set_register(result_reg, RcValue::new(value));
                Ok(None)
            }
            OpCode::Jump => {
                // Unconditional jump
                let target = arg1 as usize;
                eprintln!("DEBUG: Jump - jumping to target {}", target);
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
                        eprintln!("DEBUG: ForIter - RangeIterator {{ start: {}, stop: {}, step: {}, current: {} }}", start, stop, step, current);
                        // Check if we've reached the end of the range
                        let should_continue = if step > 0 {
                            current < stop
                        } else if step < 0 {
                            current > stop
                        } else {
                            // step == 0 is invalid, but we'll treat it as end of iteration
                            false
                        };
                        eprintln!("DEBUG: ForIter - should_continue: {}", should_continue);
                        
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
                            eprintln!("DEBUG: ForIter - continuing loop");
                            Ok(None)
                        } else {
                            // End of iteration - jump to the target (after the loop)
                            eprintln!("DEBUG: ForIter - end of iteration, jumping to target {}", target);
                            self.frames[frame_idx].pc = target;
                            // Return Ok(None) to indicate that PC has changed
                            Ok(None)
                        }
                    },
                    _ => {
                        eprintln!("DEBUG: ForIter - not a RangeIterator, jumping to target {}", target);
                        // For other types, just jump to end for now
                        self.frames[frame_idx].pc = target;
                        // Return Ok(None) to indicate that PC has changed
                        Ok(None)
                    }
                }
            }
            _ => {
                // For unimplemented opcodes, return an error
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
    fn call_function_fast(&mut self, func: Value, args: Vec<Value>) -> Result<Value> {
        eprintln!("DEBUG: call_function_fast - func: {:?}", func);
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
                eprintln!("DEBUG: Function '{}' has compiled_code: {}", name, compiled_code.is_some());
                if let Some(ref code) = compiled_code {
                    eprintln!("DEBUG: Function '{}' has {} instructions", name, code.instructions.len());
                } else {
                    eprintln!("DEBUG: Function '{}' has no compiled code, will compile from body", name);
                    eprintln!("DEBUG: Function '{}' body length: {}", name, body.len());
                }

                // Check if we have compiled code directly in the Closure
                if let Some(func_code) = compiled_code {
                    // Fast path: use the compiled code directly from the Closure
                    let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());
                    
                    // Create optimized function frame
                    let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                    let frame = Frame::new_function_frame((**func_code).clone(), caller_globals_values, builtins_values, args);
                    
                    self.frames.push(frame);
                    // Instead of calling run_frame recursively, we return None to continue execution
                    // The main execution loop will handle the new frame
                    return Ok(Value::None);
                }

                // If we don't have compiled code, but we have a body, compile it now
                if !body.is_empty() {
                    eprintln!("DEBUG: Compiling function '{}' from body", name);
                    let func_code = SuperCompiler::compile_function(name.clone(), params, body)?;
                    
                    // Create optimized function frame
                    let caller_globals_values: HashMap<String, Value> = self.frames.last().map(|f| f.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()).unwrap_or_else(|| self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect());
                    let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                    let frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);
                    
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
                    let frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);
                    
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
                let frame = Frame::new_function_frame(func_code, caller_globals_values, builtins_values, args);
                
                self.frames.push(frame);
                // Instead of calling run_frame recursively, we return None to continue execution
                // The main execution loop will handle the new frame
                Ok(Value::None)
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
}

/// SuperCompiler - Register-based bytecode compiler with advanced optimizations
pub struct SuperCompiler {
    pub code: CodeObject,
    next_register: u32,
}

impl SuperCompiler {
    pub fn new(filename: String) -> Self {
        Self {
            code: CodeObject::new(filename, "<module>".to_string(), 1),
            next_register: 0,
        }
    }
    
    /// Compile a function with the given name, parameters, and body
    pub fn compile_function(name: String, params: &[Param], body: &[Statement]) -> Result<CodeObject> {
        // Create a new compiler for the function
        let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));
        
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
    
    pub fn compile(&mut self, program: Program) -> Result<CodeObject> {
        for stmt in program.statements {
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
            Statement::SubscriptAssignment { object, index, value } => {
                let object_reg = self.compile_expression(object)?;
                let index_reg = self.compile_expression(index)?;
                let value_reg = self.compile_expression(value)?;
                
                // Emit SubscrStore instruction to store item to sequence
                self.emit(OpCode::SubscrStore, object_reg, index_reg, value_reg, 0);
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
            Statement::FunctionDef { name, params, body, .. } => {
                // Create a new compiler for the function
                let mut func_compiler = SuperCompiler::new(format!("<fn:{}>", name));
                
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
                eprintln!("DEBUG: Compiled function '{}' with {} instructions", name, func_code.instructions.len());
                
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
                    eprintln!("DEBUG: Created Closure '{}' with compiled_code: {}", name, compiled_code.is_some());
                    if let Some(ref code) = compiled_code {
                        eprintln!("DEBUG: Created Closure '{}' has {} instructions", name, code.instructions.len());
                    }
                }
                
                // Store the closure in constants
                let closure_const_idx = self.code.add_constant(closure_value);
                
                // Load the closure
                let reg = self.allocate_register();
                self.emit(OpCode::LoadConst, closure_const_idx, reg, 0, 0);
                
                // Store the function in global namespace
                let name_idx = self.code.add_name(name.clone());
                self.emit(OpCode::StoreGlobal, name_idx, reg, 0, 0);
                
                // Debug output to see what's stored in constants
                eprintln!("DEBUG: Stored Closure '{}' in constants at index {}", name, closure_const_idx);
                if let Some(stored_value) = self.code.constants.get(closure_const_idx as usize) {
                    if let Value::Closure { ref name, ref params, ref body, captured_scope: _, docstring: _, ref compiled_code } = stored_value {
                        eprintln!("DEBUG: Stored Closure '{}' has compiled_code: {}", name, compiled_code.is_some());
                        if let Some(ref code) = compiled_code {
                            eprintln!("DEBUG: Stored Closure '{}' has {} instructions", name, code.instructions.len());
                        }
                    }
                }
            }
            Statement::For { variable, iterable, body, .. } => {
                // Compile for loop: for variable in iterable:
                
                // 1. Compile the iterable expression
                let iterable_reg = self.compile_expression(iterable)?;
                
                // 2. Get an iterator from the iterable
                let iter_reg = self.allocate_register();
                self.emit(OpCode::GetIter, iterable_reg, iter_reg, 0, 0);
                
                // 3. Set up the loop structure
                let loop_var_idx = self.code.add_varname(variable.clone());
                
                // 4. Create jump targets
                let loop_start = self.code.instructions.len(); // Start of loop body
                
                // 5. Emit ForIter instruction with placeholder for end target
                let value_reg = self.allocate_register();
                let for_iter_pos = self.emit(OpCode::ForIter, iter_reg, value_reg, 0, 0); // arg3 will be updated later
                
                // 6. Store the iterated value in the loop variable
                // For loop variables, we should use StoreGlobal to store in the local scope
                let name_idx = self.code.add_name(variable.clone());
                self.emit(OpCode::StoreGlobal, name_idx, value_reg, 0, 0);
                
                // 7. Compile the loop body
                for stmt in body {
                    self.compile_statement(stmt)?;
                }
                
                // 8. Jump back to the start of the loop
                self.emit(OpCode::Jump, loop_start as u32, 0, 0, 0);
                
                // 9. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                // Update the ForIter instruction with the correct target
                // 9. Fix the loop end placeholder
                let loop_end_pos = self.code.instructions.len();
                // Update the ForIter instruction with the correct target
                self.code.instructions[for_iter_pos].arg3 = loop_end_pos as u32;
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
                let name_idx = self.code.add_name(name.clone());
                let reg = self.allocate_register();
                
                // Check if this is a local variable (parameter or local)
                if self.code.varnames.contains(&name) {
                    // For now, we'll still use LoadGlobal but a full implementation would use LoadLocal
                    // and store the local index instead of the name index
                    let cache_idx = self.code.add_inline_cache();
                    self.emit(OpCode::LoadGlobal, name_idx, cache_idx, reg, 0);
                } else {
                    // Global variable
                    let cache_idx = self.code.add_inline_cache();
                    self.emit(OpCode::LoadGlobal, name_idx, cache_idx, reg, 0);
                }
                Ok(reg)
            }
            Expr::BinaryOp { left, op, right } => {
                let left_reg = self.compile_expression(*left)?;
                let right_reg = self.compile_expression(*right)?;
                let result_reg = self.allocate_register();
                
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
                        self.emit(OpCode::BinaryMulRR, left_reg, right_reg, result_reg, 0);
                        return Ok(result_reg);
                    },
                    BinaryOp::Or => {
                        // Short-circuit OR: if left is true, return left, otherwise return right
                        // This is a simplified implementation
                        self.emit(OpCode::BinaryAddRR, left_reg, right_reg, result_reg, 0);
                        return Ok(result_reg);
                    },
                    _ => return Err(anyhow!("Unsupported binary operation: {:?}", op)),
                };
                
                self.emit(opcode, left_reg, right_reg, result_reg, 0);
                Ok(result_reg)
            }
            Expr::Call { func, args, .. } => {
                let func_reg = self.compile_expression(*func)?;
                let mut arg_regs = Vec::new();
                
                for arg in args {
                    let arg_reg = self.compile_expression(arg)?;
                    arg_regs.push(arg_reg);
                }
                
                // For now, we'll just emit a Call instruction
                // In a full implementation, we'd need to handle different call types
                let result_reg = self.allocate_register();
                self.emit(OpCode::CallFunction, func_reg, arg_regs.len() as u32, result_reg, 0);
                
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
                
                self.emit(opcode, left_reg, right_reg, result_reg, 0);
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
                self.emit(OpCode::BuildList, item_regs.len() as u32, 0, result_reg, 0);
                
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
                        self.emit(OpCode::LoadConst, neg_one_const, neg_one_reg, 0, 0);
                        self.emit(OpCode::BinaryMulRR, operand_reg, neg_one_reg, result_reg, 0);
                    }
                    UnaryOp::UAdd => {
                        // For unary plus, we just return the operand
                        self.emit(OpCode::LoadLocal, operand_reg, result_reg, 0, 0);
                    }
                    UnaryOp::Not => {
                        // For logical not, we need to implement this
                        // For now, we'll just load the operand as a placeholder
                        self.emit(OpCode::LoadLocal, operand_reg, result_reg, 0, 0);
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
                self.emit(OpCode::SubscrLoad, object_reg, index_reg, result_reg, 0);
                
                Ok(result_reg)
            }
            Expr::MethodCall { object, method, args, .. } => {
                let object_reg = self.compile_expression(*object)?;
                
                // Compile arguments and ensure they are in consecutive registers after the object register
                let mut arg_regs = Vec::new();
                for (i, arg) in args.iter().enumerate() {
                    let arg_reg = self.compile_expression(arg.clone())?;
                    eprintln!("DEBUG: MethodCall - arg{} reg: {}", i, arg_reg);
                    arg_regs.push(arg_reg);
                }
                
                // The VM expects arguments to be in consecutive registers starting from object_reg + 1
                // We need to make sure this is the case by potentially moving registers around
                // For now, we'll assume the compiler has organized them correctly
                
                let method_name_idx = self.code.add_name(method);
                eprintln!("DEBUG: MethodCall - method_name_idx: {}", method_name_idx);
                
                // Emit CallMethod instruction
                // arg1: object register
                // arg2: number of arguments
                // arg3: method name index
                self.emit(OpCode::CallMethod, object_reg, arg_regs.len() as u32, method_name_idx, 0);
                
                // Return a register with None value (most method calls return None)
                let result_reg = self.allocate_register();
                let none_const = self.code.add_constant(Value::None);
                self.emit(OpCode::LoadConst, none_const, result_reg, 0, 0);
                Ok(result_reg)
            }
            _ => Err(anyhow!("Unsupported expression type: {:?}", expr)),
        }
    }
}