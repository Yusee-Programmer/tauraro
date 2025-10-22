//! Memory management (alloc, free, refcount, GC hooks)

use crate::value::Value;
use crate::bytecode::instructions::{OpCode, Instruction};
use crate::bytecode::objects::RcValue;
use crate::ast::{Param, Type};
use std::collections::HashMap;
use std::fmt::Debug;
use smallvec::SmallVec;

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
    pub params: Vec<Param>,     // Parameter information with type annotations
    pub var_types: HashMap<String, Type>,  // Variable type annotations
    pub return_type: Option<Type>,         // Function return type annotation
}

impl PartialEq for CodeObject {
    fn eq(&self, other: &Self) -> bool {
        // Simplified comparison for now - just compare name and instruction count
        self.name == other.name && self.instructions.len() == other.instructions.len()
    }
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
            params: Vec::new(),  // Initialize the params field
            var_types: HashMap::new(),  // Initialize variable type annotations
            return_type: None,           // Initialize return type annotation
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

/// Execution frame for register-based VM with reference counting and method caching
#[derive(Clone)]
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
    pub is_property_setter: bool,           // True if this frame is executing a property setter
    pub vars_to_update: Vec<String>,        // Variables to update after property setter completes
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

impl Frame {
    pub fn new(code: CodeObject, globals: HashMap<String, Value>, builtins: HashMap<String, Value>) -> Self {
        // Initialize registers
        let mut registers = SmallVec::new();
        registers.resize(code.registers as usize, RcValue::new(Value::None));
        
        // If registers count is 0, log a warning
        if code.registers == 0 && !code.instructions.is_empty() {
            // For debugging, let's allocate some registers if there are instructions
            if code.instructions.len() > 0 {
                registers.resize(64, RcValue::new(Value::None));
            }
        }
        
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
            is_property_setter: false,
            vars_to_update: Vec::new(),
        }
    }

    /// Create a frame optimized for function calls with pre-allocated registers
    pub fn new_function_frame(code: CodeObject, globals: HashMap<String, Value>, builtins: HashMap<String, Value>, args: Vec<Value>, kwargs: HashMap<String, Value>) -> Self {
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
        
        // Copy arguments to locals, handling *args, **kwargs, and keyword arguments
        let mut arg_index = 0;
        
        // Process each parameter in order
        for (param_index, param_name) in code.varnames.iter().enumerate() {
            // Check if this parameter is *args or **kwargs by looking at the params in code object
            let param_info = code.params.iter().find(|param| &param.name == param_name);
            
            if let Some(param) = param_info {
                match param.kind {
                    crate::ast::ParamKind::VarArgs => {
                        // This is a *args parameter
                        // Collect all remaining positional arguments into a tuple
                        let remaining_args: Vec<Value> = args.iter().skip(arg_index).cloned().collect();
                        let tuple_value = Value::Tuple(remaining_args);
                        let rc_arg = RcValue::new(tuple_value.clone());
                        if param_index < locals.len() {
                            locals[param_index] = rc_arg;
                        }
                        break; // All remaining args are collected, so we're done
                    }
                    crate::ast::ParamKind::VarKwargs => {
                        // This is a **kwargs parameter
                        // Create a dict with all remaining keyword arguments
                        // Convert KwargsMarker back to regular Dict when used as parameter
                        let dict_value = Value::Dict(kwargs.clone());
                        let rc_arg = RcValue::new(dict_value);
                        if param_index < locals.len() {
                            locals[param_index] = rc_arg;
                        }
                        break; // **kwargs should be the last parameter, so we're done
                    }
                    _ => {
                        // Regular parameter
                        // First check if it's provided as a keyword argument
                        if let Some(value) = kwargs.get(param_name) {
                            let rc_arg = RcValue::new(value.clone());
                            if param_index < locals.len() {
                                locals[param_index] = rc_arg;
                            }
                        } else if arg_index < args.len() {
                            // Use positional argument
                            let rc_arg = RcValue::new(args[arg_index].clone());
                            if param_index < locals.len() {
                                locals[param_index] = rc_arg;
                            }
                            arg_index += 1;
                        }
                    }
                }
            } else {
                // Regular parameter
                // First check if it's provided as a keyword argument
                if let Some(value) = kwargs.get(param_name) {
                    let rc_arg = RcValue::new(value.clone());
                    if param_index < locals.len() {
                        locals[param_index] = rc_arg;
                    }
                } else if arg_index < args.len() {
                    // Use positional argument
                    let rc_arg = RcValue::new(args[arg_index].clone());
                    if param_index < locals.len() {
                        locals[param_index] = rc_arg;
                    }
                    arg_index += 1;
                }
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
            is_property_setter: false,
            vars_to_update: Vec::new(),
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

/// Method cache for object-oriented code performance
#[derive(Debug, Clone)]
pub struct MethodCache {
    pub class_name: String,
    pub method_name: String,
    pub method: Option<Value>,
    pub version: u32,
}

/// Memory management operations
pub struct MemoryOps;

// Memory-related opcodes are already defined in instructions.rs
// The implementation for these opcodes is in arithmetic.rs in the execute_instruction_fast method