//! Core VM data structures and main execution loop

use crate::ast::*;
use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::{OpCode, Instruction};
use anyhow::{Result, anyhow};
use std::collections::{HashMap, HashSet};
use std::fmt::Debug;
use std::hash::{Hash, Hasher};
use smallvec::SmallVec;

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
    pub frames: Vec<Frame>,
    pub builtins: HashMap<String, RcValue>,
    pub globals: HashMap<String, RcValue>,
    pub globals_version: u32,
    
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
}