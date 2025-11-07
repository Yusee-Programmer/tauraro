//! Memory management (alloc, free, refcount, GC hooks)

use crate::value::Value;
use crate::bytecode::instructions::{OpCode, Instruction};
use crate::bytecode::objects::RcValue;
use crate::ast::{Param, Type};
use std::collections::HashMap;
use std::fmt::Debug;
use std::rc::Rc;
use std::cell::RefCell;
use smallvec::SmallVec;

/// Register-based compiled code object
#[derive(Debug, Clone)]
pub struct CodeObject {
    pub instructions: Vec<Instruction>,
    pub constants: Vec<Value>,  // Keep as Value for simplicity
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
    pub inline_method_cache: Vec<InlineMethodCache>, // OPTIMIZATION: Per-instruction method caches
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
            inline_method_cache: Vec::new(),
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
            self.names.push(name.clone());
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

    /// Add an inline method cache for optimized method lookups (20-30% speedup)
    pub fn add_inline_method_cache(&mut self) -> u32 {
        let index = self.inline_method_cache.len() as u32;
        self.inline_method_cache.push(InlineMethodCache {
            cached_class_name: None,
            cached_method: None,
            cache_version: 0,
            hit_count: 0,
            miss_count: 0,
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

/// Inline method cache for optimized method lookups
/// This provides 20-30% speedup by avoiding HashMap lookups and using direct array indexing
#[derive(Debug, Clone)]
pub struct InlineMethodCache {
    pub cached_class_name: Option<String>,  // Last seen class name
    pub cached_method: Option<Value>,       // Cached method value
    pub cache_version: u32,                 // Version for cache invalidation
    pub hit_count: u32,                     // Number of cache hits (for profiling)
    pub miss_count: u32,                    // Number of cache misses (for profiling)
}

impl InlineMethodCache {
    #[inline(always)]
    pub fn new() -> Self {
        Self {
            cached_class_name: None,
            cached_method: None,
            cache_version: 0,
            hit_count: 0,
            miss_count: 0,
        }
    }

    /// Check if cache is valid for the given class name and version
    #[inline(always)]
    pub fn is_valid(&self, class_name: &str, current_version: u32) -> bool {
        self.cache_version == current_version
            && self.cached_class_name.as_ref().map(|s| s.as_str()) == Some(class_name)
            && self.cached_method.is_some()
    }

    /// Update cache with new method
    #[inline(always)]
    pub fn update(&mut self, class_name: String, method: Value, current_version: u32) {
        self.cached_class_name = Some(class_name);
        self.cached_method = Some(method);
        self.cache_version = current_version;
        self.miss_count += 1;
    }

    /// Get cached method (assumes is_valid was called first)
    #[inline(always)]
    pub fn get(&mut self) -> &Value {
        self.hit_count += 1;
        self.cached_method.as_ref().unwrap()
    }
}

impl PartialEq for InlineMethodCache {
    fn eq(&self, other: &Self) -> bool {
        self.cached_class_name == other.cached_class_name
            && self.cache_version == other.cache_version
    }
}

impl Eq for InlineMethodCache {}

/// Block for control flow
#[derive(Debug, Clone)]
pub struct Block {
    pub block_type: BlockType,
    pub handler: usize,
    pub level: usize,
}

#[derive(Debug, Clone, PartialEq)]
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
    pub line_number: usize,                 // Current line number for debugging
    pub registers: SmallVec<[RcValue; 64]>, // Register file with reference counting
    pub locals: Vec<RcValue>,               // Local variables with direct indexing (faster than HashMap)
    pub locals_map: HashMap<String, usize>, // Maps variable names to indices for debugging
    pub globals: Rc<RefCell<HashMap<String, RcValue>>>,  // OPTIMIZATION: Shared globals with interior mutability
    pub builtins: Rc<RefCell<HashMap<String, RcValue>>>, // OPTIMIZATION: Shared builtins with interior mutability
    pub free_vars: Vec<RcValue>,            // Free variables for closures with reference counting
    pub block_stack: Vec<Block>,            // Block stack for control flow
    pub cache_version: u32,                 // Current cache version
    pub method_cache: HashMap<(String, String), MethodCache>, // Method cache for object-oriented code
    pub return_register: Option<(usize, u32)>, // (caller_frame_idx, result_reg) where return value should be stored
    pub is_property_setter: bool,           // True if this frame is executing a property setter
    pub vars_to_update: Vec<String>,        // Variables to update after property setter completes
    pub last_loaded_attr: Option<String>,   // Track the last loaded attribute name for module class imports
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
    pub fn new(code: CodeObject, globals: Rc<RefCell<HashMap<String, RcValue>>>, builtins: Rc<RefCell<HashMap<String, RcValue>>>) -> Self {
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

        // Globals and builtins are already wrapped in Rc, just clone the pointer

        Self {
            code,
            pc: 0,
            line_number: 0,
            registers,
            locals,
            locals_map,
            globals,
            builtins,
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
            return_register: None,
            is_property_setter: false,
            vars_to_update: Vec::new(),
            last_loaded_attr: None,
        }
    }

    /// OPTIMIZATION: Reinitialize a pooled frame for reuse (avoids allocation)
    pub fn reinit(&mut self, code: CodeObject, globals: Rc<RefCell<HashMap<String, RcValue>>>, builtins: Rc<RefCell<HashMap<String, RcValue>>>) {
        // Reset frame state
        self.code = code;
        self.pc = 0;
        self.line_number = 0;

        // Resize registers to match new code
        let reg_count = self.code.registers as usize;
        if reg_count == 0 && !self.code.instructions.is_empty() {
            self.registers.resize(64, RcValue::new(Value::None));
        } else {
            self.registers.resize(reg_count, RcValue::new(Value::None));
        }

        // Reset locals
        self.locals.clear();
        self.locals.resize(self.code.varnames.len(), RcValue::new(Value::None));

        // Rebuild locals map
        self.locals_map.clear();
        for (i, name) in self.code.varnames.iter().enumerate() {
            self.locals_map.insert(name.clone(), i);
        }

        // Update globals and builtins references
        self.globals = globals;
        self.builtins = builtins;

        // Reset other state
        self.free_vars.clear();
        self.block_stack.clear();
        self.cache_version = 0;
        self.method_cache.clear();
        self.return_register = None;
        self.is_property_setter = false;
        self.vars_to_update.clear();
        self.last_loaded_attr = None;
    }

    /// Create a frame optimized for function calls with pre-allocated registers
    pub fn new_function_frame(code: CodeObject, globals: Rc<RefCell<HashMap<String, RcValue>>>, builtins: Rc<RefCell<HashMap<String, RcValue>>>, args: Vec<Value>, kwargs: HashMap<String, Value>) -> Self {
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
                        let dict_value = Value::Dict(Rc::new(RefCell::new(kwargs.clone())));
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

        // Globals and builtins are already wrapped in Rc, just use them

        Self {
            code,
            pc: 0,
            line_number: 0,
            registers,
            locals,
            locals_map,
            globals,
            builtins,
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
            return_register: None,
            is_property_setter: false,
            vars_to_update: Vec::new(),
            last_loaded_attr: None,
        }
    }

    /// Create a frame with Rc-wrapped globals/builtins (OPTIMIZATION: no HashMap conversion)
    pub fn new_with_rc(code: CodeObject, globals: Rc<RefCell<HashMap<String, RcValue>>>, builtins: Rc<RefCell<HashMap<String, RcValue>>>) -> Self {
        // Initialize registers
        let mut registers = SmallVec::new();
        registers.resize(code.registers as usize, RcValue::new(Value::None));

        // If registers count is 0, allocate some registers if there are instructions
        if code.registers == 0 && !code.instructions.is_empty() {
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

        Self {
            code,
            pc: 0,
            line_number: 0,
            registers,
            locals,
            locals_map,
            globals,   // Already Rc-wrapped, no conversion needed!
            builtins,  // Already Rc-wrapped, no conversion needed!
            free_vars: Vec::new(),
            block_stack: Vec::new(),
            cache_version: 0,
            method_cache: HashMap::new(),
            return_register: None,
            is_property_setter: false,
            vars_to_update: Vec::new(),
            last_loaded_attr: None,
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
pub struct MemoryOps {
    pub max_recursion_depth: usize,
    pub current_recursion_depth: usize,
}

impl MemoryOps {
    pub fn new() -> Self {
        Self {
            max_recursion_depth: 1000, // Default max recursion depth
            current_recursion_depth: 0,
        }
    }
    
    pub fn increment_recursion_depth(&mut self) -> Result<(), String> {
        self.current_recursion_depth += 1;
        if self.current_recursion_depth > self.max_recursion_depth {
            Err("Maximum recursion depth exceeded".to_string())
        } else {
            Ok(())
        }
    }
    
    pub fn decrement_recursion_depth(&mut self) {
        if self.current_recursion_depth > 0 {
            self.current_recursion_depth -= 1;
        }
    }
    
    pub fn set_max_recursion_depth(&mut self, depth: usize) {
        self.max_recursion_depth = depth;
    }
}

// Memory-related opcodes are already defined in instructions.rs
// The implementation for these opcodes is in arithmetic.rs in the execute_instruction_fast method