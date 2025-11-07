//! Virtual machine implementation

use crate::value::Value;
use crate::value_pool as value_pool;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::OpCode;
use crate::bytecode::objects::RcValue;
use crate::bytecode::memory::{CodeObject, Frame, Block, BlockType, MemoryOps};
use crate::ast::Statement;
// Import the arithmetic module
// use crate::bytecode::arithmetic;
// Import necessary types for Closure handling
use anyhow::{Result, anyhow};
use std::collections::{HashMap, HashSet};
use std::rc::Rc;
use std::cell::RefCell;
// Import module system for dynamic module loading
use crate::modules;
// Import type checker for runtime type enforcement
use crate::type_checker::TypeChecker;
use crate::bytecode::memory::MethodCache;

/// Register-based bytecode virtual machine with computed GOTOs for maximum performance
pub struct SuperBytecodeVM {
    pub frames: Vec<Frame>,
    pub builtins: Rc<RefCell<HashMap<String, RcValue>>>,
    pub globals: Rc<RefCell<HashMap<String, RcValue>>>,
    pub globals_version: u32,

    // Memory management and stack overflow protection
    pub memory_ops: MemoryOps,

    // Cache compiled code objects for closures to avoid recompiling on each call
    function_code_cache: HashMap<Value, CodeObject>,

    // OPTIMIZATION: Global method cache shared across all frames for maximum performance
    // Key: (class_name, method_name) -> cached method
    global_method_cache: HashMap<(String, String), MethodCache>,
    method_cache_version: u32,

    // OPTIMIZATION: Global attribute cache for fast attribute lookups
    // Key: (class_name, attr_name) -> (offset, cached_value)
    global_attr_cache: HashMap<(String, String), (usize, Option<Value>)>,
    attr_cache_version: u32,

    // Profiling and JIT compilation tracking
    instruction_execution_count: HashMap<(String, usize), u64>, // (function_name, instruction_index) -> count
    function_call_count: HashMap<String, u64>, // function_name -> call count
    hot_function_threshold: u64, // Threshold for considering a function "hot"
    jit_compiled_functions: HashMap<String, bool>, // Track which functions have been JIT compiled

    // JIT compiler for hot function compilation
    #[cfg(feature = "jit")]
    jit_builder: Option<cranelift_module::Module<cranelift_module::Backend>>,

    // Type checker for runtime type enforcement
    pub type_checker: TypeChecker,
    pub enable_type_checking: bool, // Flag to enable/disable type checking

    // Module cache for preventing duplicate module loads (like Python's sys.modules)
    loaded_modules: HashMap<String, Value>,
    /// Stack of modules currently being loaded to detect circular imports
    pub loading_modules: std::collections::HashSet<String>,

    // Strong static typing: track variables with type annotations for enforcement
    // Maps variable name to its declared type string (e.g., "int", "str", "List[int]")
    typed_variables: HashMap<String, String>,
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
        let _jit_builder: Option<()> = None;

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

        // Initialize memory operations with stack overflow protection
        let memory_ops = MemoryOps::new();

        // Note: FFI builtins are automatically added through builtins::init_builtins()
        // when the 'ffi' feature is enabled

        Self {
            frames: Vec::new(),
            builtins: Rc::new(RefCell::new(builtins)),
            globals: Rc::new(RefCell::new(globals)),
            globals_version: 0,
            memory_ops,
            function_code_cache: HashMap::new(),

            // Initialize global caches for maximum performance
            global_method_cache: HashMap::new(),
            method_cache_version: 0,
            global_attr_cache: HashMap::new(),
            attr_cache_version: 0,

            // Initialize profiling counters
            instruction_execution_count: HashMap::new(),
            function_call_count: HashMap::new(),
            hot_function_threshold: 1000, // Consider functions hot after 1000 calls
            jit_compiled_functions: HashMap::new(),

            // Initialize JIT compiler
            #[cfg(feature = "jit")]
            jit_builder,

            // Initialize type checker
            type_checker: TypeChecker::new(),
            enable_type_checking: true, // Enable type checking by default

            // Initialize module cache
            loaded_modules: HashMap::new(),
            loading_modules: std::collections::HashSet::new(),

            // Initialize typed variables map for strong static typing
            typed_variables: HashMap::new(),
        }
    }

    /// Helper method to compile and execute a module source file
    pub fn compile_and_execute_module(&mut self, source: &str, module_name: &str) -> Result<Value> {
        // eprintln!("DEBUG compile_and_execute_module: START for module '{}'", module_name);
        // eprintln!("DEBUG compile_and_execute_module: loading_modules before: {:?}", self.loading_modules);

        // Check for circular import
        if self.loading_modules.contains(module_name) {
            // eprintln!("DEBUG compile_and_execute_module: circular import detected for '{}'", module_name);
            return Err(anyhow!("ImportError: cannot import name '{}' (circular import)", module_name));
        }

        // Add module to loading set
        self.loading_modules.insert(module_name.to_string());
        // eprintln!("DEBUG compile_and_execute_module: added '{}' to loading_modules: {:?}", module_name, self.loading_modules);

        // Ensure we remove the module from loading set even if an error occurs
        // eprintln!("DEBUG compile_and_execute_module: calling compile_and_execute_module_inner for '{}'", module_name);
        let result = self.compile_and_execute_module_inner(source, module_name);
        // eprintln!("DEBUG compile_and_execute_module: compile_and_execute_module_inner returned for '{}'", module_name);

        // Remove module from loading set now that it's fully executed and cached
        self.loading_modules.remove(module_name);
        // eprintln!("DEBUG compile_and_execute_module: removed '{}' from loading_modules", module_name);

        result
    }

    /// Helper method to compile and execute a module source file
    fn compile_and_execute_module_inner(&mut self, source: &str, module_name: &str) -> Result<Value> {
        // eprintln!("DEBUG compile_and_execute_module_inner: START for module '{}'", module_name);

        // Compile the module
        // eprintln!("DEBUG compile_and_execute_module_inner: lexing module '{}'", module_name);
        let tokens = crate::lexer::Lexer::new(source)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in module '{}': {}", module_name, e))?;

        // eprintln!("DEBUG compile_and_execute_module_inner: parsing module '{}'", module_name);
        let mut parser = crate::parser::Parser::new(tokens);
        let ast = parser.parse()
            .map_err(|e| anyhow!("Parser error in module '{}': {}", module_name, e))?;

        // eprintln!("DEBUG compile_and_execute_module_inner: compiling module '{}'", module_name);
        let mut compiler = crate::bytecode::compiler::SuperCompiler::new(module_name.to_string());
        let code_object = compiler.compile(ast)
            .map_err(|e| anyhow!("Compiler error in module '{}': {}", module_name, e))?;

        // Save current globals to determine what the module adds
        let globals_before: std::collections::HashSet<String> = self.globals.borrow().keys().cloned().collect();

        // Execute the module
        // eprintln!("DEBUG compile_and_execute_module_inner: executing module '{}' with {} frames currently", module_name, self.frames.len());
        self.execute(code_object)
            .map_err(|e| anyhow!("Error executing module '{}': {}", module_name, e))?;
        // eprintln!("DEBUG compile_and_execute_module_inner: execution completed for module '{}'", module_name);

        // Get the module's globals (namespace) - only new names added by the module
        let mut module_namespace = HashMap::new();

        // Clone the current globals as module_globals for functions
        let module_globals_rc = Rc::clone(&self.globals);

        for (name, value) in self.globals.borrow().iter() {
            // Include new names except for builtins and special internal names
            // Allow: regular names, __version__ style names, _private names
            // Special case: allow __name__ to override even if it existed before
            // Exclude: builtins, __builtins__, __pycache__ etc
            let is_new_or_override = !globals_before.contains(name) || name == "__name__";
            if is_new_or_override &&
               name != "builtins" &&
               name != "__builtins__" &&
               !name.starts_with("__py") {
                // If this is a closure, attach the module's globals to it
                let value_to_store = match &value.value {
                    Value::Closure { name, params, body, captured_scope, docstring, compiled_code, .. } => {
                        Value::Closure {
                            name: name.clone(),
                            params: params.clone(),
                            body: body.clone(),
                            captured_scope: captured_scope.clone(),
                            docstring: docstring.clone(),
                            compiled_code: compiled_code.clone(),
                            module_globals: Some(Rc::clone(&module_globals_rc)),
                        }
                    },
                    _ => value.value.clone(),
                };
                module_namespace.insert(name.clone(), value_to_store);
            }
        }

        // Create the module and cache it
        let module = Value::Module(module_name.to_string(), module_namespace);
        self.loaded_modules.insert(module_name.to_string(), module.clone());
        
        Ok(module)
    }

    /// Load a module from a file in the filesystem
    /// Searches sys.path directories for module files with supported extensions
    /// Supported extensions: .py, .tr, .tau, .tauraro
    fn load_module_from_file(&mut self, module_name: &str) -> Result<Value> {
        // eprintln!("DEBUG load_module_from_file: attempting to load module '{}'", module_name);
        // eprintln!("DEBUG load_module_from_file: loaded_modules keys: {:?}", self.loaded_modules.keys().collect::<Vec<_>>());
        // eprintln!("DEBUG load_module_from_file: loading_modules: {:?}", self.loading_modules);

        // Check if module is already loaded (module caching like Python's sys.modules)
        if let Some(cached_module) = self.loaded_modules.get(module_name) {
            // eprintln!("DEBUG load_module_from_file: found cached module '{}'", module_name);
            return Ok(cached_module.clone());
        }

        // Check if module is currently being loaded (circular import detection)
        if self.loading_modules.contains(module_name) {
            // eprintln!("DEBUG load_module_from_file: circular import detected for '{}'", module_name);
            return Err(anyhow!("ImportError: cannot import name '{}' (circular import)", module_name));
        }

        let search_paths = vec![
            ".".to_string(),
            "tauraro_packages".to_string(),
            "lib".to_string(),
        ];

        let extensions = vec!["py", "tr", "tau", "tauraro"];

        // Handle hierarchical packages (e.g., "win32.constants" -> "win32/constants.tr")
        if module_name.contains('.') {
            let module_path_str = module_name.replace('.', std::path::MAIN_SEPARATOR_STR);
            for search_path in &search_paths {
                for ext in &extensions {
                    let full_path = std::path::Path::new(search_path).join(format!("{}.{}", module_path_str, ext));
                    if full_path.exists() {
                        let source = std::fs::read_to_string(&full_path)
                            .map_err(|e| anyhow!("Failed to read module file: {}", e))?;
                        return self.compile_and_execute_module(&source, module_name);
                    }
                }
            }
        }

        // Try to find the module file in search paths with any supported extension
        for search_path in &search_paths {
            for ext in &extensions {
                let module_path = std::path::Path::new(search_path).join(format!("{}.{}", module_name, ext));
                if module_path.exists() {
                    let source = std::fs::read_to_string(&module_path)
                        .map_err(|e| anyhow!("Failed to read module file: {}", e))?;
                    return self.compile_and_execute_module(&source, module_name);
                }
            }
        }

        // Try to load from package directories (with __init__ files)
        for search_path in &search_paths {
            for ext in &extensions {
                let package_path = std::path::Path::new(search_path)
                    .join(module_name)
                    .join(format!("__init__.{}", ext));
                if package_path.exists() {
                    let source = std::fs::read_to_string(&package_path)
                        .map_err(|e| anyhow!("Failed to read package __init__ file: {}", e))?;
                    return self.compile_and_execute_module(&source, module_name);
                }
            }
        }

        Err(anyhow!("Module file not found (searched for .py, .tr, .tau, .tauraro)"))
    }

    /// Track instruction execution for profiling and JIT compilation
    /// Get a global variable by name (for REPL)
    pub fn get_global(&self, name: &str) -> Option<RcValue> {
        self.globals.borrow().get(name).cloned()
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
        // Just clone the Rc pointers (cheap!) instead of cloning the entire HashMap
        let globals_rc = Rc::clone(&self.globals);
        let builtins_rc = Rc::clone(&self.builtins);

        let frame = Frame::new(code, globals_rc, builtins_rc);
        self.frames.push(frame);

        let result = self.run_frame()?;

        // Globals are shared via Rc<RefCell>, so no need to update
        // All modifications are already visible in self.globals
        self.frames.pop();

        // Check if there's a __last_expr__ global (for REPL expression evaluation)
        // If so, return it and remove it from globals
        if let Some(last_expr) = self.globals.borrow_mut().remove("__last_expr__") {
            return Ok(last_expr.value);
        }

        Ok(result)
    }
    
    /// Optimized frame execution using computed GOTOs for maximum performance
    pub fn run_frame(&mut self) -> Result<Value> {
        // Check for stack overflow using a simple counter
        if self.frames.len() > 1000 {
            return Err(anyhow!("Stack overflow: maximum recursion depth exceeded"));
        }
        
        let mut frame_idx;
        
        loop {
            // Fast path: check if we have frames
            if self.frames.is_empty() {
                return Ok(Value::None);
            }
            
            // Update frame index in case frames were added/removed
            frame_idx = self.frames.len() - 1;
            
            // Safety check: if there are no instructions, return None immediately
            if self.frames[frame_idx].code.instructions.is_empty() {
                // Globals are shared via Rc<RefCell>, no need to update
                self.frames.pop();
                return Ok(Value::None);
            }
            
            // Fast path: check bounds
            let pc = self.frames[frame_idx].pc;
            let instructions_len = self.frames[frame_idx].code.instructions.len();

            if pc >= instructions_len {
                // Return None when we've executed all instructions
                return Ok(Value::None);
            }
            
            // Direct access to instruction without cloning when possible
            // Get the instruction details without borrowing self
            let (opcode, arg1, arg2, arg3, function_name, _line_num, _filename) = {
                let frame = &self.frames[frame_idx];
                let instruction = &frame.code.instructions[pc]; // Use reference instead of moving
                (instruction.opcode, instruction.arg1, instruction.arg2, instruction.arg3,
                 frame.code.name.clone(), instruction.line, frame.code.filename.clone())
            };

            // Track instruction execution for profiling and JIT compilation
            self.track_instruction_execution(&function_name, pc);

            // Execute instruction using computed GOTOs for maximum performance
            match self.execute_instruction_fast(frame_idx, opcode, arg1, arg2, arg3) {
                Ok(Some(value)) => {
                    // A function has returned a value, pop the current frame
                    if let Some(returned_frame) = self.frames.pop() {
                        // Check if this is an __init__ frame by looking at the function name
                        let is_init_frame = returned_frame.code.name == "__init__" || returned_frame.code.name == "<fn:__init__>";

                        // Check if this is a property setter frame
                        let is_setter_frame = returned_frame.is_property_setter;

                        // If this is an __init__ or property setter frame, we want to return the instance instead of None
                        let return_value = if is_init_frame || is_setter_frame {
                            // For __init__ methods and property setters, we should return the modified instance that was passed as self
                            // The instance should be in the first local variable (self parameter)
                            if !returned_frame.locals.is_empty() {
                                returned_frame.locals[0].value.clone()
                            } else {
                                value // Fallback to the actual return value
                            }
                        } else {
                            // For regular functions, return the actual return value
                            value
                        };
                        
                        // If there are no more frames, return the value
                        if self.frames.is_empty() {
                            return Ok(return_value);
                        }
                        
                        // Update frame index to point to the calling frame
                        frame_idx = self.frames.len() - 1;

                        // Globals are shared via Rc<RefCell>, no need to update

                        // Store the return value in the calling frame if return_register is set
                        if let Some((caller_frame_idx, result_reg)) = returned_frame.return_register {
                            // Make sure the caller frame index is valid
                            if caller_frame_idx < self.frames.len() {
                                self.frames[caller_frame_idx].set_register(result_reg, RcValue::new(return_value.clone()));

                                // CRITICAL FIX: For object field persistence during inheritance
                                // When an __init__ frame returns, we need to ensure that any modifications
                                // to the object are properly propagated back to the caller's object registers AND locals[0]
                                // Check if this is an __init__ frame and update object registers if needed
                                if is_init_frame {
                                    // For __init__ frames, we also need to update locals[0] in the caller frame
                                    // This is critical for super() calls where the parent __init__ modifies the instance
                                    // and the child __init__ needs to see those modifications in its locals[0]

                                    // Update locals[0] with the modified instance from result_reg
                                    if !self.frames[caller_frame_idx].locals.is_empty() {
                                        // The modified instance is now in result_reg of the caller frame
                                        let modified_instance = self.frames[caller_frame_idx].registers[result_reg as usize].value.clone();
                                        self.frames[caller_frame_idx].locals[0] = RcValue::new(modified_instance);
                                    }
                                }

                                // CRITICAL FIX: For property setters, update all variables that referenced the object
                                if is_setter_frame && !returned_frame.vars_to_update.is_empty() {
                                    let modified_object = self.frames[caller_frame_idx].registers[result_reg as usize].clone();

                                    for var_spec in &returned_frame.vars_to_update {
                                        let parts: Vec<&str> = var_spec.split(':').collect();
                                        match parts[0] {
                                            "global" => {
                                                if self.globals.borrow().contains_key(parts[1]) {
                                                    self.globals.borrow_mut().insert(parts[1].to_string(), modified_object.clone());
                                                }
                                            }
                                            "frame_global" => {
                                                if caller_frame_idx < self.frames.len() {
                                                    if self.frames[caller_frame_idx].globals.borrow().contains_key(parts[1]) {
                                                        self.frames[caller_frame_idx].globals.borrow_mut().insert(parts[1].to_string(), modified_object.clone());
                                                    }
                                                }
                                            }
                                            "local" => {
                                                if let Ok(idx) = parts[1].parse::<usize>() {
                                                    if caller_frame_idx < self.frames.len() && idx < self.frames[caller_frame_idx].locals.len() {
                                                        self.frames[caller_frame_idx].locals[idx] = modified_object.clone();
                                                    }
                                                }
                                            }
                                            _ => {}
                                        }
                                    }
                                }
                            } else {
                            }
                        } else {
                        }
                        
                        // Continue execution with the calling frame
                        continue;
                    } else {
                        // No frames to pop, return the value
                        return Ok(value);
                    }
                },
                Ok(None) => {
                    // Check if a new frame was pushed during execution
                    if self.frames.len() > frame_idx + 1 {
                        // A new frame was pushed, continue execution with the new frame
                        // First, advance the PC in the calling frame
                        if frame_idx < self.frames.len() {
                            self.frames[frame_idx].pc += 1;
                        }
                        // Update frame index to point to the new frame
                        frame_idx = self.frames.len() - 1;
                        continue;
                    }
                    // Only increment PC if frame still exists and PC hasn't changed
                    if frame_idx < self.frames.len() {
                        // Check if PC has changed (e.g., due to a jump)
                        if self.frames[frame_idx].pc == pc {
                            self.frames[frame_idx].pc += 1;
                        }
                    }
                    // Continue the loop to execute the next instruction
                    continue;
                },
                Err(e) => {
                    // Handle the exception by checking for handlers without holding borrows across mutations
                    let handler_pos_opt = {
                        let frame = &self.frames[frame_idx];
                        frame
                            .block_stack
                            .iter()
                            .rfind(|b| matches!(b.block_type, BlockType::Except))
                            .map(|b| b.handler)
                    };
                                
                    // Handle the exception
                    if let Some(handler_pos) = handler_pos_opt {
                        // Unwind the stack to the handler position
                        self.frames[frame_idx].pc = handler_pos;

                        // Convert the Rust error to a Python exception object
                        let error_msg = format!("{}", e);
                        let error_msg_lower = error_msg.to_lowercase();
                        let exception_class = if error_msg_lower.contains("division by zero") || error_msg_lower.contains("divide by zero") {
                            "ZeroDivisionError"
                        } else if error_msg_lower.contains("assertionerror") {
                            "AssertionError"
                        } else if error_msg_lower.contains("nameerror") || error_msg_lower.contains("not defined") {
                            "NameError"
                        } else if error_msg_lower.contains("indexerror") || error_msg_lower.contains("index") && error_msg_lower.contains("out of") {
                            "IndexError"
                        } else if error_msg_lower.contains("keyerror") || (error_msg_lower.contains("key") && error_msg_lower.contains("not found")) {
                            "KeyError"
                        } else if error_msg_lower.contains("typeerror") {
                            "TypeError"
                        } else if error_msg_lower.contains("valueerror") || error_msg_lower.contains("invalid literal") || error_msg_lower.contains("could not convert") {
                            "ValueError"
                        } else if error_msg_lower.contains("attributeerror") || error_msg_lower.contains("attribute") {
                            "AttributeError"
                        } else {
                            "RuntimeError"
                        };

                        let exception = Value::new_exception(
                            exception_class.to_string(),
                            error_msg,
                            None
                        );

                        // Push the exception onto the registers stack
                        self.frames[frame_idx].registers.push(RcValue::new(exception));
                        // Continue execution at the exception handler
                        continue;
                    } else {
                        // No handler found, propagate the exception
                        return Err(e);
                    }
                }
            }
        }
    }

    /// Check if a value matches a declared type string (for strong static typing)
    fn check_type_match(&self, value: &Value, type_str: &str) -> bool {
        match type_str {
            "int" => matches!(value, Value::Int(_)),
            "float" => matches!(value, Value::Float(_)),
            "str" => matches!(value, Value::Str(_)),
            "bool" => matches!(value, Value::Bool(_)),
            "list" | "List" => matches!(value, Value::List(_)),
            "dict" | "Dict" => matches!(value, Value::Dict(_)),
            "tuple" | "Tuple" => matches!(value, Value::Tuple(_)),
            "set" | "Set" => matches!(value, Value::Set(_)),
            "None" | "NoneType" => matches!(value, Value::None),
            // For complex types like List[int], just check the base type for now
            s if s.starts_with("List[") || s.starts_with("list[") => matches!(value, Value::List(_)),
            s if s.starts_with("Dict[") || s.starts_with("dict[") => matches!(value, Value::Dict(_)),
            s if s.starts_with("Tuple[") || s.starts_with("tuple[") => matches!(value, Value::Tuple(_)),
            s if s.starts_with("Set[") || s.starts_with("set[") => matches!(value, Value::Set(_)),
            _ => true, // Unknown types are allowed (for custom classes, etc.)
        }
    }

    /// Optimized instruction execution with computed GOTOs for maximum performance
    #[inline(always)]
    fn execute_instruction_fast(&mut self, frame_idx: usize, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32) -> Result<Option<Value>> {
        // Debug output for instruction execution
        // eprintln!("DEBUG: Executing opcode {:?} with args {}, {}, {}", opcode, arg1, arg2, arg3);
        
        match opcode {
            OpCode::LoadConst => {
                let const_idx = arg1 as usize;
                let result_reg = arg2;
                
                if const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadConst: constant index {} out of bounds (len: {})", const_idx, self.frames[frame_idx].code.constants.len()));
                }
                let value = RcValue::new(self.frames[frame_idx].code.constants[const_idx].clone());
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::GetIter => {
                // Get an iterator from an iterable object
                let iterable_reg = arg1 as usize;
                let result_reg = arg2;
                
                if iterable_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("GetIter: register index {} out of bounds (len: {})", iterable_reg, self.frames[frame_idx].registers.len()));
                }
                
                let iterable_value = &self.frames[frame_idx].registers[iterable_reg];
                
                // Convert iterable to iterator based on its type
                let iterator = match &iterable_value.value {
                    Value::Generator { .. } => {
                        // For generators, we return the generator itself as an iterator
                        iterable_value.value.clone()
                    },
                    Value::Iterator { .. } => {
                        // For Iterator objects, we return the iterator itself
                        iterable_value.value.clone()
                    },
                    Value::Range { start, stop, step } => {
                        // For range, create a RangeIterator
                        Value::RangeIterator {
                            start: *start,
                            stop: *stop,
                            step: *step,
                            current: *start,
                        }
                    },
                    Value::List(items) => {
                        // For list, create an Iterator object
                        Value::Iterator {
                            items: items.as_vec().clone(),
                            current_index: 0,
                        }
                    },
                    Value::Tuple(items) => {
                        // For tuple, create an Iterator object
                        Value::Iterator {
                            items: items.clone(),
                            current_index: 0,
                        }
                    },
                    Value::Str(s) => {
                        // For string, create a StringIterator (using RangeIterator for indices)
                        Value::RangeIterator {
                            start: 0,
                            stop: s.len() as i64,
                            step: 1,
                            current: 0,
                        }
                    },
                    _ => {
                        // For other types, we'll just jump to end for now
                        // In a full implementation, we'd try to call the __iter__ method
                        return Err(anyhow!("GetIter: cannot create iterator for type {}", iterable_value.value.type_name()));
                    }
                };
                
                self.frames[frame_idx].set_register(result_reg, RcValue::new(iterator));
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
                
                // Handle different iterator types
                // Clone the iterator value to avoid borrowing issues
                let iter_value = self.frames[frame_idx].registers[iter_reg].value.clone();
                match iter_value {
                    Value::RangeIterator { start, stop, step, current } => {
                        // Check if we've reached the end of the range
                        let should_continue = if step > 0 {
                            current < stop
                        } else if step < 0 {
                            current > stop
                        } else {
                            // step == 0 is invalid, but we'll treat it as end of iteration
                            false
                        };
                        
                        if should_continue {
                            // Store the current value in the result register
                            let value = RcValue::new(value_pool::create_int(current));
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
                            Ok(None)
                        } else {
                            // End of iteration - jump to the target (after the loop)
                            self.frames[frame_idx].pc = target;
                            // Return Ok(None) to indicate that PC has changed
                            Ok(None)
                        }
                    },
                    Value::Generator { code, frame, finished } => {
                        // For generators, we need to resume execution and get the next value
                        if finished {
                            // Generator is finished, jump to the target (end of loop)
                            self.frames[frame_idx].pc = target;
                            Ok(None)
                        } else {
                            // Resume the generator execution
                            // If frame is None, this is the first time we're calling the generator
                            let generator_frame = if let Some(f) = frame {
                                *f
                            } else {
                                // Create a new frame for the generator
                                let globals_rc = Rc::clone(&self.globals);
                                let builtins_rc = Rc::clone(&self.builtins);
                                Frame::new_function_frame(*code, globals_rc, builtins_rc, vec![], HashMap::new())
                            };
                            
                            // Set up return register so the generator can return yielded values to this frame
                            let mut gen_frame = generator_frame;
                            gen_frame.return_register = Some((frame_idx, result_reg as u32));
                            
                            // Push the generator frame onto the stack
                            self.frames.push(gen_frame);
                            
                            // We'll handle the generator execution result in the main execution loop
                            // For now, we just continue execution which will run the generator frame
                            Ok(None)
                        }
                    },
                    Value::Iterator { ref items, ref current_index } => {
                        // For Iterator objects, check if we've reached the end
                        if *current_index < items.len() {
                            // Store the current value in the result register
                            let value = RcValue::new(items[*current_index].clone());
                            self.frames[frame_idx].set_register(result_reg as u32, value);
                            
                            // Update the iterator's current position
                            let updated_iterator = Value::Iterator {
                                items: items.clone(),
                                current_index: current_index + 1,
                            };
                            self.frames[frame_idx].registers[iter_reg] = RcValue::new(updated_iterator);
                            
                            // Continue with the loop body
                            Ok(None)
                        } else {
                            // End of iteration - jump to the target (after the loop)
                            self.frames[frame_idx].pc = target;
                            // Return Ok(None) to indicate that PC has changed
                            Ok(None)
                        }
                    },
                    _ => {
                        // For other types, we'll just jump to end for now
                        // In a full implementation, we'd try to call the __next__ method
                        self.frames[frame_idx].pc = target;
                        Ok(None)
                    }
                }
            }
            OpCode::Next => {
                // Call next() on an iterator and update the iterator variable
                let iter_reg = arg1 as usize;
                let result_reg = arg2 as usize;
                
                if iter_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("Next: iterator register index {} out of bounds", iter_reg));
                }
                
                // Clone the iterator value to avoid borrowing issues
                let iter_value = self.frames[frame_idx].registers[iter_reg].value.clone();
                match iter_value {
                    Value::RangeIterator { start, stop, step, current } => {
                        // Check if we've reached the end of the range
                        let should_continue = if step > 0 {
                            current < stop
                        } else if step < 0 {
                            current > stop
                        } else {
                            // step == 0 is invalid, but we'll treat it as end of iteration
                            false
                        };
                        
                        if should_continue {
                            // Store the current value in the result register
                            let value = RcValue::new(value_pool::create_int(current));
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
                            Ok(None)
                        } else {
                            // Iterator exhausted, raise StopIteration
                            Err(anyhow!("StopIteration"))
                        }
                    },
                    _ => {
                        // For other types, try to call the __next__ method
                        Err(anyhow!("'{}' object is not an iterator", iter_value.type_name()))
                    }
                }
            }
            OpCode::LoadAndAdd => {
                // Load + Add in one instruction
                // arg1 = load register, arg2 = add register, arg3 = result register
                let load_reg = arg1 as usize;
                let add_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                if load_reg >= self.frames[frame_idx].registers.len() || 
                   add_reg >= self.frames[frame_idx].registers.len() ||
                   result_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadAndAdd: register index out of bounds"));
                }

                // Load the value
                let load_value = &self.frames[frame_idx].registers[load_reg];
                // Get the value to add
                let add_value = &self.frames[frame_idx].registers[add_reg];

                // Perform addition (using value pool for integer results)
                let result = match (&load_value.value, &add_value.value) {
                    (Value::Int(a), Value::Int(b)) => value_pool::create_int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Str(a), Value::Str(b)) => Value::Str(format!("{}{}", a, b)),
                    _ => {
                        // For less common cases, use the general implementation
                        self.add_values(load_value.value.clone(), add_value.value.clone())
                            .map_err(|e| anyhow!("Error in LoadAndAdd: {}", e))?
                    }
                };

                // Store the result
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadAddStore => {
                // Load + Add + Store in one instruction
                // arg1 = load register, arg2 = add register, arg3 = store register
                let load_reg = arg1 as usize;
                let add_reg = arg2 as usize;
                let store_reg = arg3 as usize;

                if load_reg >= self.frames[frame_idx].registers.len() ||
                   add_reg >= self.frames[frame_idx].registers.len() ||
                   store_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadAddStore: register index out of bounds"));
                }

                // Load the value
                let load_value = &self.frames[frame_idx].registers[load_reg];
                // Get the value to add
                let add_value = &self.frames[frame_idx].registers[add_reg];

                // Perform addition (using value pool for integer results)
                let result = match (&load_value.value, &add_value.value) {
                    (Value::Int(a), Value::Int(b)) => value_pool::create_int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Str(a), Value::Str(b)) => Value::Str(format!("{}{}", a, b)),
                    _ => {
                        // For less common cases, use the general implementation
                        self.add_values(load_value.value.clone(), add_value.value.clone())
                            .map_err(|e| anyhow!("Error in LoadAddStore: {}", e))?
                    }
                };

                // Store the result
                self.frames[frame_idx].registers[store_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadSubStore => {
                // Load + Sub + Store in one instruction
                // arg1 = load register, arg2 = sub register, arg3 = store register
                let load_reg = arg1 as usize;
                let sub_reg = arg2 as usize;
                let store_reg = arg3 as usize;

                if load_reg >= self.frames[frame_idx].registers.len() || 
                   sub_reg >= self.frames[frame_idx].registers.len() ||
                   store_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadSubStore: register index out of bounds"));
                }

                // Load the value
                let load_value = &self.frames[frame_idx].registers[load_reg];
                // Get the value to subtract
                let sub_value = &self.frames[frame_idx].registers[sub_reg];

                // Perform subtraction
                let result = match (&load_value.value, &sub_value.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a - b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a - b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.sub_values(load_value.value.clone(), sub_value.value.clone())
                            .map_err(|e| anyhow!("Error in LoadSubStore: {}", e))?
                    }
                };

                // Store the result
                self.frames[frame_idx].registers[store_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadMulStore => {
                // Load + Mul + Store in one instruction
                // arg1 = load register, arg2 = mul register, arg3 = store register
                let load_reg = arg1 as usize;
                let mul_reg = arg2 as usize;
                let store_reg = arg3 as usize;

                if load_reg >= self.frames[frame_idx].registers.len() || 
                   mul_reg >= self.frames[frame_idx].registers.len() ||
                   store_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadMulStore: register index out of bounds"));
                }

                // Load the value
                let load_value = &self.frames[frame_idx].registers[load_reg];
                // Get the value to multiply
                let mul_value = &self.frames[frame_idx].registers[mul_reg];

                // Perform multiplication
                let result = match (&load_value.value, &mul_value.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a * b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a * b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.mul_values(load_value.value.clone(), mul_value.value.clone())
                            .map_err(|e| anyhow!("Error in LoadMulStore: {}", e))?
                    }
                };

                // Store the result
                self.frames[frame_idx].registers[store_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadDivStore => {
                // Load + Div + Store in one instruction
                // arg1 = load register, arg2 = div register, arg3 = store register
                let load_reg = arg1 as usize;
                let div_reg = arg2 as usize;
                let store_reg = arg3 as usize;

                if load_reg >= self.frames[frame_idx].registers.len() || 
                   div_reg >= self.frames[frame_idx].registers.len() ||
                   store_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadDivStore: register index out of bounds"));
                }

                // Load the value
                let load_value = &self.frames[frame_idx].registers[load_reg];
                // Get the value to divide
                let div_value = &self.frames[frame_idx].registers[div_reg];

                // Perform division
                let result = match (&load_value.value, &div_value.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.div_values(load_value.value.clone(), div_value.value.clone())
                            .map_err(|e| anyhow!("Error in LoadDivStore: {}", e))?
                    }
                };

                // Store the result
                self.frames[frame_idx].registers[store_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryAddRR => {
                // Register-Register addition with unsafe fast path in release mode
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BinaryAddRR: register index out of bounds"));
                    }
                }

                // SAFETY: In release builds, bytecode compiler guarantees register indices are valid
                // In debug builds, bounds checking above ensures safety
                let (left, right) = unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    (regs.get_unchecked(left_reg), regs.get_unchecked(right_reg))
                };

                // Fast path for integer addition
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Str(a), Value::Str(b)) => {
                        // Optimized string concatenation without format! overhead
                        let mut s = String::with_capacity(a.len() + b.len());
                        s.push_str(a);
                        s.push_str(b);
                        Value::Str(s)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.add_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddRR: {}", e))?
                    }
                };

                // SAFETY: Same as above - result_reg is guaranteed valid
                unsafe {
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::PopBlock => {
                self.frames[frame_idx].block_stack.pop();
                Ok(None)
            }
            OpCode::FastIntAdd => {
                // ULTRA-FAST integer addition - completely bypasses error handling
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("FastIntAdd: register index out of bounds"));
                    }
                }

                // SAFETY: Bounds checked in debug, guaranteed by compiler in release
                // This path does ZERO error handling for maximum speed
                unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    if let Value::Int(left_val) = regs.get_unchecked(left_reg).value {
                        if let Value::Int(right_val) = regs.get_unchecked(right_reg).value {
                            // Direct integer arithmetic - NO allocation, NO error handling
                            let result = left_val.wrapping_add(right_val);
                            self.frames[frame_idx].registers.get_unchecked_mut(result_reg).value = Value::Int(result);
                            return Ok(None);
                        }
                    }

                    // Fallback to regular addition (rare path)
                    let left_val = regs.get_unchecked(left_reg).value.clone();
                    let right_val = regs.get_unchecked(right_reg).value.clone();
                    let result = self.add_values(left_val, right_val)
                        .map_err(|e| anyhow!("Error in FastIntAdd: {}", e))?;
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::FastIntSub => {
                // ULTRA-FAST integer subtraction - zero allocation
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("FastIntSub: register index out of bounds"));
                    }
                }

                unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    if let Value::Int(left_val) = regs.get_unchecked(left_reg).value {
                        if let Value::Int(right_val) = regs.get_unchecked(right_reg).value {
                            // Direct integer arithmetic - NO allocation
                            let result = left_val.wrapping_sub(right_val);
                            self.frames[frame_idx].registers.get_unchecked_mut(result_reg).value = Value::Int(result);
                            return Ok(None);
                        }
                    }

                    // Fallback to regular subtraction
                    let left_val = regs.get_unchecked(left_reg).value.clone();
                    let right_val = regs.get_unchecked(right_reg).value.clone();
                    let result = self.sub_values(left_val, right_val)
                        .map_err(|e| anyhow!("Error in FastIntSub: {}", e))?;
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::FastIntMul => {
                // ULTRA-FAST integer multiplication - zero allocation
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("FastIntMul: register index out of bounds"));
                    }
                }

                unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    if let Value::Int(left_val) = regs.get_unchecked(left_reg).value {
                        if let Value::Int(right_val) = regs.get_unchecked(right_reg).value {
                            // Direct integer arithmetic - NO allocation
                            let result = left_val.wrapping_mul(right_val);
                            self.frames[frame_idx].registers.get_unchecked_mut(result_reg).value = Value::Int(result);
                            return Ok(None);
                        }
                    }

                    // Fallback to regular multiplication
                    let left_val = regs.get_unchecked(left_reg).value.clone();
                    let right_val = regs.get_unchecked(right_reg).value.clone();
                    let result = self.mul_values(left_val, right_val)
                        .map_err(|e| anyhow!("Error in FastIntMul: {}", e))?;
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::FastIntDiv => {
                // Ultra-fast integer division without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning for maximum performance
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Check for division by zero
                        if right_val == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val / right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular division using the arithmetic module
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.div_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in FastIntDiv: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::FastIntMod => {
                // Ultra-fast integer modulo without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning for maximum performance
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Check for modulo by zero
                        if right_val == 0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val % right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular modulo using the arithmetic module
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.mod_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in FastIntMod: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CallFunction => {
                // Function call: arg1 = function register, arg2 = argument count, arg3 = result register
                let func_reg = arg1 as usize;
                let arg_count = arg2 as usize;
                let result_reg = arg3 as usize;

                if func_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunction: function register index {} out of bounds (len: {})", func_reg, self.frames[frame_idx].registers.len()));
                }

                // Get the function value
                let func_value = self.frames[frame_idx].registers[func_reg].value.clone();

                // Collect arguments from registers
                let mut args = Vec::with_capacity(arg_count); // Pre-allocate capacity for better memory efficiency
                for i in 0..arg_count {
                    // Arguments are stored in consecutive registers after the function register
                    let arg_reg = func_reg + 1 + i;
                    if arg_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallFunction: argument register index {} out of bounds (len: {})", arg_reg, self.frames[frame_idx].registers.len()));
                    }
                    let arg_value = self.frames[frame_idx].registers[arg_reg].value.clone();
                    args.push(arg_value);
                }
                
                // Check if the last argument is a kwargs dictionary (our special marker)
                // But only process it for user-defined functions that accept **kwargs
                let mut kwargs_dict = None;
                let mut processed_arg_count = args.len();
                
                if !args.is_empty() {
                    // Check if the last argument is a kwargs dictionary (our special marker)
                    // But only process it for user-defined functions that accept **kwargs
                    // Only exclude KwargsMarker values, not regular Dict values
                    if let Value::KwargsMarker(dict) = &args[args.len() - 1] {
                        // Debug info removed
                        // For builtin functions, we don't pass the kwargs dictionary
                        // Only user-defined functions with **kwargs parameters should receive it
                        match &func_value {
                            Value::BuiltinFunction(_, _) | Value::NativeFunction(_) => {
                                // For builtin functions, don't pass the kwargs dictionary
                                // The kwargs dictionary was added as the last argument, so we exclude it
                                processed_arg_count = args.len() - 1;
                                // Debug info removed
                            }
                            Value::Closure { name: _, params: _, body: _, captured_scope: _, docstring: _, compiled_code, .. } => {
                                // For user-defined functions, check if they have **kwargs parameter
                                if let Some(code_obj) = compiled_code {
                                    // Check if any parameter is of kind VarKwargs
                                    let has_kwargs_param = code_obj.params.iter().any(|param| {
                                        matches!(param.kind, crate::ast::ParamKind::VarKwargs)
                                    });
                                    
                                    if has_kwargs_param {
                                        // This function accepts **kwargs, so pass the dictionary
                                        kwargs_dict = Some(dict.clone());
                                        processed_arg_count = args.len() - 1; // Exclude the kwargs dictionary from regular arguments
                                        // Debug info removed
                                    } else {
                                        // This function doesn't accept **kwargs, so don't pass the dictionary
                                        processed_arg_count = args.len() - 1;
                                        // Debug info removed
                                    }
                                } else {
                                    // No compiled code, don't pass the kwargs dictionary
                                    processed_arg_count = args.len() - 1;
                                    // Debug info removed
                                }
                            }
                            _ => {
                                // For other callable objects, don't pass the kwargs dictionary
                                processed_arg_count = args.len() - 1;
                                // Debug info removed
                            }
                        }
                    }
                }
                
                // Take only the regular arguments (excluding the kwargs dictionary if present)
                let regular_args = args[..processed_arg_count].to_vec();
                // Debug info removed
                
                // Process starred arguments in the args vector
                let processed_args = self.process_starred_arguments(regular_args)?;
                // Debug info removed
                
                // Create kwargs HashMap from the kwargs dictionary if present
                let kwargs = if let Some(dict) = kwargs_dict {
                    // Debug info removed
                    dict.clone()
                } else {
                    // Debug info removed
                    HashMap::new()
                };
                
                // Call the function using the fast path
                let result = self.call_function_fast(func_value, processed_args, kwargs, Some(frame_idx), Some(result_reg as u32))?;

                // If the function returned a value directly, store it in the result register
                if !matches!(result, Value::None) {
                    self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                } else {
                }

                Ok(None)
            }
            OpCode::CallFunctionKw => {
                // Call a function with keyword arguments
                // arg1 = function register, arg2 = positional argument count, arg3 = result register
                let func_reg = arg1 as usize;
                let pos_arg_count = arg2 as usize;
                let result_reg = arg3 as usize;

                if func_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunctionKw: function register index {} out of bounds (len: {})", func_reg, self.frames[frame_idx].registers.len()));
                }

                // Get the function value
                let func_value = self.frames[frame_idx].registers[func_reg].value.clone();

                // Collect positional arguments from registers
                let mut args = Vec::with_capacity(pos_arg_count);
                for i in 0..pos_arg_count {
                    // Arguments are stored in consecutive registers after the function register
                    let arg_reg = func_reg + 1 + i;
                    if arg_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallFunctionKw: argument register index {} out of bounds (len: {})", arg_reg, self.frames[frame_idx].registers.len()));
                    }
                    let arg_value = self.frames[frame_idx].registers[arg_reg].value.clone();
                    args.push(arg_value);
                }

                // The next register after positional arguments should contain the keyword arguments dict
                let kwargs_reg = func_reg + 1 + pos_arg_count;
                if kwargs_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunctionKw: kwargs register index {} out of bounds (len: {})", kwargs_reg, self.frames[frame_idx].registers.len()));
                }

                // Get the keyword arguments dictionary
                let kwargs_dict = match &self.frames[frame_idx].registers[kwargs_reg].value {
                    Value::Dict(dict_ref) => dict_ref.borrow().clone(),
                    Value::KwargsMarker(dict) => dict.clone(),
                    _ => return Err(anyhow!("CallFunctionKw: kwargs must be a dictionary, got {}", self.frames[frame_idx].registers[kwargs_reg].value.type_name())),
                };

                // Process starred arguments in the args vector
                let processed_args = self.process_starred_arguments(args)?;

                // Call the function using the fast path
                let result = self.call_function_fast(func_value, processed_args, kwargs_dict, Some(frame_idx), Some(result_reg as u32))?;

                // If the function returned a value directly, store it in the result register
                if !matches!(result, Value::None) {
                    self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                }

                Ok(None)
            }
            OpCode::CallFunctionEx => {
                // Call a function with extended arguments (positional args as tuple, keyword args as dict)
                // arg1 = function register, arg2 = flags (0 = no kwargs, 1 = has kwargs), arg3 = result register
                let func_reg = arg1 as usize;
                let has_kwargs = arg2 != 0;
                let result_reg = arg3 as usize;

                if func_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunctionEx: function register index {} out of bounds (len: {})", func_reg, self.frames[frame_idx].registers.len()));
                }

                // Get the function value
                let func_value = self.frames[frame_idx].registers[func_reg].value.clone();

                // The next register should contain the positional arguments as a tuple
                let args_reg = func_reg + 1;
                if args_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunctionEx: args register index {} out of bounds (len: {})", args_reg, self.frames[frame_idx].registers.len()));
                }

                // Extract arguments from the tuple
                let args = match &self.frames[frame_idx].registers[args_reg].value {
                    Value::Tuple(items) => items.clone(),
                    Value::List(list) => list.as_vec().clone(),
                    _ => return Err(anyhow!("CallFunctionEx: args must be a tuple or list, got {}", self.frames[frame_idx].registers[args_reg].value.type_name())),
                };

                // Get keyword arguments if present
                let kwargs_dict = if has_kwargs {
                    let kwargs_reg = args_reg + 1;
                    if kwargs_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallFunctionEx: kwargs register index {} out of bounds (len: {})", kwargs_reg, self.frames[frame_idx].registers.len()));
                    }

                    match &self.frames[frame_idx].registers[kwargs_reg].value {
                        Value::Dict(dict_ref) => dict_ref.borrow().clone(),
                        Value::KwargsMarker(dict) => dict.clone(),
                        _ => return Err(anyhow!("CallFunctionEx: kwargs must be a dictionary, got {}", self.frames[frame_idx].registers[kwargs_reg].value.type_name())),
                    }
                } else {
                    HashMap::new()
                };

                // Process starred arguments in the args vector
                let processed_args = self.process_starred_arguments(args)?;

                // Call the function using the fast path
                let result = self.call_function_fast(func_value, processed_args, kwargs_dict, Some(frame_idx), Some(result_reg as u32))?;

                // If the function returned a value directly, store it in the result register
                if !matches!(result, Value::None) {
                    self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                }

                Ok(None)
            }
            OpCode::BinaryDivRRFastInt => {
                // Fast path for integer Register-Register division
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning for maximum performance
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Check for division by zero
                        if right_val == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val / right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular division using the arithmetic module
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.div_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in BinaryDivRRFastInt: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryModRRFastInt => {
                // Fast path for integer Register-Register modulo
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // Direct access to integer values without cloning for maximum performance
                if let Value::Int(left_val) = self.frames[frame_idx].registers[left_reg].value {
                    if let Value::Int(right_val) = self.frames[frame_idx].registers[right_reg].value {
                        // Check for division by zero
                        if right_val == 0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        // Create result directly without intermediate allocations
                        self.frames[frame_idx].registers[result_reg] = RcValue {
                            value: Value::Int(left_val % right_val),
                            ref_count: 1,
                        };
                        return Ok(None);
                    }
                }
                // Fallback to regular modulo
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.mod_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in BinaryModRRFastInt: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareLessRR => {
                // Register-Register less than comparison
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareLessRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a < b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a < b),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a < b),
                    _ => {
                        // For other types, use the general comparison
                        match left.value.partial_cmp(&right.value) {
                            Some(std::cmp::Ordering::Less) => Value::Bool(true),
                            _ => Value::Bool(false),
                        }
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareLessEqualRR => {
                // Register-Register less than or equal comparison
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareLessEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a <= b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a <= b),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a <= b),
                    _ => {
                        // For other types, use the general comparison
                        match left.value.partial_cmp(&right.value) {
                            Some(std::cmp::Ordering::Less) | Some(std::cmp::Ordering::Equal) => Value::Bool(true),
                            _ => Value::Bool(false),
                        }
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareNotInRR => {
                // Register-Register non-membership test (not in)
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareNotInRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Check non-membership (opposite of membership)
                let result = match (&left.value, &right.value) {
                    // String non-membership
                    (Value::Str(item), Value::Str(container)) => Value::Bool(!container.contains(item)),
                    // List non-membership
                    (item, Value::List(container)) => {
                        let found = container.iter().any(|list_item| list_item == item);
                        Value::Bool(!found)
                    },
                    // Tuple non-membership
                    (item, Value::Tuple(container)) => {
                        let found = container.iter().any(|tuple_item| tuple_item == item);
                        Value::Bool(!found)
                    },
                    // Set non-membership
                    (item, Value::Set(container)) => {
                        let found = container.iter().any(|set_item| set_item == item);
                        Value::Bool(!found)
                    },
                    // Dict non-membership (check keys)
                    (item, Value::Dict(container)) => {
                        // For dict non-membership, we check if the item is NOT a key in the dict
                        match item {
                            Value::Str(key) => Value::Bool(!container.borrow().contains_key(key)),
                            _ => {
                                let key_str = format!("{}", item);
                                Value::Bool(!container.borrow().contains_key(&key_str))
                            }
                        }
                    },
                    _ => {
                        // For other types, try to convert to string and check string non-membership
                        let left_str = format!("{}", left.value);
                        let right_str = format!("{}", right.value);
                        Value::Bool(!right_str.contains(&left_str))
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BuildList => {
                // Build a list from items on the stack/register
                let item_count = arg1 as usize;
                let first_item_reg = arg2 as usize;
                let result_reg = arg3 as u32;
                
                // Create a new list
                let mut items = Vec::new();
                
                // Get items from consecutive registers starting from first_item_reg
                for i in 0..item_count {
                    let item_reg = first_item_reg + i;
                    if item_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildList: item register index {} out of bounds (len: {})", item_reg, self.frames[frame_idx].registers.len()));
                    }
                    items.push(self.frames[frame_idx].registers[item_reg].value.clone());
                }
                
                let list_value = Value::List(crate::modules::hplist::HPList::from_values(items));
                self.frames[frame_idx].set_register(result_reg, RcValue::new(list_value));
                Ok(None)
            }
            OpCode::BuildTuple => {
                // Build a tuple from items on the stack/register
                let item_count = arg1 as usize;
                let first_item_reg = arg2 as usize;
                let result_reg = arg3 as u32;

                // Create a new tuple
                let mut items = Vec::new();

                // Get items from consecutive registers starting from first_item_reg
                for i in 0..item_count {
                    let item_reg = first_item_reg + i;
                    if item_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildTuple: item register index {} out of bounds (len: {})", item_reg, self.frames[frame_idx].registers.len()));
                    }
                    items.push(self.frames[frame_idx].registers[item_reg].value.clone());
                }

                let tuple_value = Value::Tuple(items);
                self.frames[frame_idx].set_register(result_reg, RcValue::new(tuple_value));
                Ok(None)
            }
            OpCode::BuildDict => {
                // Build a dict from key-value pairs on the stack/register
                // Keys and values are interleaved: key1, value1, key2, value2, ...
                let pair_count = arg1 as usize;
                let first_key_reg = arg2 as usize;
                let result_reg = arg3 as u32;

                // eprintln!("DEBUG BuildDict: pair_count={}, first_key_reg={}, result_reg={}", pair_count, first_key_reg, result_reg);

                // Create a new dict
                let mut dict = HashMap::new();

                // Get items from consecutive registers starting from first_key_reg
                for i in 0..pair_count {
                    let key_reg = first_key_reg + 2 * i;
                    let value_reg = first_key_reg + 2 * i + 1;
                    if key_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildDict: key register index {} out of bounds (len: {})", key_reg, self.frames[frame_idx].registers.len()));
                    }
                    if value_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildDict: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                    }

                    // Keys must be strings - convert them or error
                    let key = &self.frames[frame_idx].registers[key_reg].value;
                    let key_str = match key {
                        Value::Str(s) => s.clone(),
                        _ => return Err(anyhow!("BuildDict: dictionary keys must be strings, got {}", key.type_name())),
                    };

                    dict.insert(key_str, self.frames[frame_idx].registers[value_reg].value.clone());
                }

                // eprintln!("DEBUG BuildDict: created dict with {} entries", dict.len());
                let dict_value = Value::Dict(Rc::new(RefCell::new(dict)));
                self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(dict_value));
                Ok(None)
            }
            OpCode::BuildSet => {
                // Build a set from items on the stack/register
                let item_count = arg1 as usize;
                let first_item_reg = arg2 as usize;
                let result_reg = arg3 as u32;
                
                // Create a new set
                let mut items = Vec::new();
                
                // Get items from consecutive registers starting from first_item_reg
                for i in 0..item_count {
                    let item_reg = first_item_reg + i;
                    if item_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildSet: item register index {} out of bounds (len: {})", item_reg, self.frames[frame_idx].registers.len()));
                    }
                    items.push(self.frames[frame_idx].registers[item_reg].value.clone());
                }
                
                let set_value = Value::Set(items);
                self.frames[frame_idx].set_register(result_reg, RcValue::new(set_value));
                Ok(None)
            }
            OpCode::BinaryDivRR => {
                // Register-Register division with unsafe fast path
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BinaryDivRR: register index out of bounds"));
                    }
                }

                let (left, right) = unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    (regs.get_unchecked(left_reg), regs.get_unchecked(right_reg))
                };

                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.div_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryDivRR: {}", e))?
                    }
                };

                unsafe {
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::BinaryFloorDivRR => {
                // Register-Register floor division
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryFloorDivRR: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];

                // Fast path for integer floor division
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        // Python-style floor division for integers
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        // Floor division for floats returns float
                        Value::Float((a / b).floor())
                    },
                    (Value::Int(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float((*a as f64 / b).floor())
                    },
                    (Value::Float(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float((a / *b as f64).floor())
                    },
                    _ => {
                        return Err(anyhow!("Unsupported operand types for floor division"));
                    }
                };

                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryFloorDivRI => {
                // Register-Immediate floor division
                let left_reg = arg1 as usize;
                let imm_idx = arg2 as usize;
                let result_reg = arg3 as usize;

                if left_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryFloorDivRI: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg];
                let right = self.frames[frame_idx].code.constants.get(imm_idx)
                    .ok_or_else(|| anyhow!("BinaryFloorDivRI: constant index out of bounds"))?;

                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float((a / b).floor())
                    },
                    _ => {
                        return Err(anyhow!("Unsupported operand types for floor division"));
                    }
                };

                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryFloorDivIR => {
                // Immediate-Register floor division
                let imm_idx = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                if right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryFloorDivIR: register index out of bounds"));
                }

                let left = self.frames[frame_idx].code.constants.get(imm_idx)
                    .ok_or_else(|| anyhow!("BinaryFloorDivIR: constant index out of bounds"))?;
                let right = &self.frames[frame_idx].registers[right_reg];

                let result = match (left, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float((a / b).floor())
                    },
                    _ => {
                        return Err(anyhow!("Unsupported operand types for floor division"));
                    }
                };

                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryDivRI => {
                // Register-Immediate division
                let left_reg = arg1 as usize;
                let right_imm = arg2 as usize; // Immediate value index in constants
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_imm >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryDivRI: register or constant index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].code.constants[right_imm];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Int(a / b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Division by zero"));
                        }
                        Value::Float(a / b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.div_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryDivRI: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryAddIR => {
                // Immediate-Register addition
                let left_imm = arg1 as usize; // Immediate value index in constants
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_imm >= self.frames[frame_idx].code.constants.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryAddIR: constant or register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].code.constants[left_imm];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (left, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Str(a), Value::Str(b)) => Value::Str(format!("{}{}", a, b)),
                    _ => {
                        // For less common cases, use the general implementation
                        self.add_values(left.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddIR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryAddRI => {
                // Register-Immediate addition
                let left_reg = arg1 as usize;
                let right_imm = arg2 as usize; // Immediate value index in constants
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_imm >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryAddRI: register or constant index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].code.constants[right_imm];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a + b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a + b),
                    (Value::Str(a), Value::Str(b)) => Value::Str(format!("{}{}", a, b)),
                    _ => {
                        // For less common cases, use the general implementation
                        self.add_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryAddRI: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinarySubRR => {
                // Register-Register subtraction with unsafe fast path
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BinarySubRR: register index out of bounds"));
                    }
                }

                let (left, right) = unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    (regs.get_unchecked(left_reg), regs.get_unchecked(right_reg))
                };

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

                unsafe {
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::BinarySubRI => {
                // Register-Immediate subtraction
                let left_reg = arg1 as usize;
                let right_imm = arg2 as usize; // Immediate value index in constants
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_imm >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinarySubRI: register or constant index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].code.constants[right_imm];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a - b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a - b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.sub_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinarySubRI: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinarySubIR => {
                // Immediate-Register subtraction
                let left_imm = arg1 as usize; // Immediate value index in constants
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_imm >= self.frames[frame_idx].code.constants.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinarySubIR: constant or register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].code.constants[left_imm];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (left, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a - b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a - b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.sub_values(left.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinarySubIR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryMulRI => {
                // Register-Immediate multiplication
                let left_reg = arg1 as usize;
                let right_imm = arg2 as usize; // Immediate value index in constants
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_imm >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryMulRI: register or constant index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].code.constants[right_imm];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a * b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a * b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.mul_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryMulRI: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryMulIR => {
                // Immediate-Register multiplication
                let left_imm = arg1 as usize; // Immediate value index in constants
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_imm >= self.frames[frame_idx].code.constants.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryMulIR: constant or register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].code.constants[left_imm];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (left, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a * b),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a * b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.mul_values(left.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryMulIR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryModRR => {
                // Register-Register modulo
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryModRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Int(a % b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Float(a % b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.mod_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryModRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryModRI => {
                // Register-Immediate modulo
                let left_reg = arg1 as usize;
                let right_imm = arg2 as usize; // Immediate value index in constants
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_imm >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryModRI: register or constant index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].code.constants[right_imm];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Int(a % b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Float(a % b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.mod_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryModRI: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryModIR => {
                // Immediate-Register modulo
                let left_imm = arg1 as usize; // Immediate value index in constants
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_imm >= self.frames[frame_idx].code.constants.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryModIR: constant or register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].code.constants[left_imm];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (left, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b == 0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Int(a % b)
                    },
                    (Value::Float(a), Value::Float(b)) => {
                        if *b == 0.0 {
                            return Err(anyhow!("Modulo by zero"));
                        }
                        Value::Float(a % b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.mod_values(left.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryModIR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryPowRR => {
                // Register-Register power
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryPowRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b >= 0 {
                            Value::Int(a.pow(*b as u32))
                        } else {
                            Value::Float((*a as f64).powf(*b as f64))
                        }
                    },
                    (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
                    (Value::Float(a), Value::Int(b)) => Value::Float(a.powf(*b as f64)),
                    (Value::Int(a), Value::Float(b)) => Value::Float((*a as f64).powf(*b)),
                    _ => {
                        return Err(anyhow!("Error in BinaryPowRR: Unsupported types for power: {} ** {}", left.value.type_name(), right.value.type_name()));
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryPowRI => {
                // Register-Immediate power
                let left_reg = arg1 as usize;
                let right_imm = arg2 as usize; // Immediate value index in constants
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_imm >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("BinaryPowRI: register or constant index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].code.constants[right_imm];
                
                // Fast path for common operations
                let result = match (&left.value, right) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b >= 0 {
                            Value::Int(a.pow(*b as u32))
                        } else {
                            Value::Float((*a as f64).powf(*b as f64))
                        }
                    },
                    (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
                    _ => {
                        // For less common cases, use the general implementation
                        self.pow_values(left.value.clone(), right.clone())
                            .map_err(|e| anyhow!("Error in BinaryPowRI: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryPowIR => {
                // Immediate-Register power
                let left_imm = arg1 as usize; // Immediate value index in constants
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_imm >= self.frames[frame_idx].code.constants.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryPowIR: constant or register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].code.constants[left_imm];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (left, &right.value) {
                    (Value::Int(a), Value::Int(b)) => {
                        if *b >= 0 {
                            Value::Int(a.pow(*b as u32))
                        } else {
                            Value::Float((*a as f64).powf(*b as f64))
                        }
                    },
                    (Value::Float(a), Value::Float(b)) => Value::Float(a.powf(*b)),
                    _ => {
                        // For less common cases, use the general implementation
                        self.pow_values(left.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryPowIR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryBitAndRR => {
                // Register-Register bitwise AND
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryBitAndRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a & b),
                    (Value::Bool(a), Value::Bool(b)) => Value::Bool(*a & *b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.bitand_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryBitAndRR: {}", e))?
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::BinaryBitOrRR => {
                // Register-Register bitwise OR
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // eprintln!("DEBUG BinaryBitOrRR: left_reg={}, right_reg={}, result_reg={}", left_reg, right_reg, result_reg);
                // eprintln!("DEBUG BinaryBitOrRR: registers.len()={}", self.frames[frame_idx].registers.len());

                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryBitOrRR: register index out of bounds"));
                }

                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];

                // eprintln!("DEBUG BinaryBitOrRR: left value = {:?}", left.value);
                // eprintln!("DEBUG BinaryBitOrRR: right value = {:?}", right.value);

                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int(a | b),
                    (Value::Bool(a), Value::Bool(b)) => Value::Bool(*a | *b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.bitor_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryBitOrRR: {}", e))?
                    }
                };

                // eprintln!("DEBUG BinaryBitOrRR: result = {:?}", result);
                // eprintln!("DEBUG BinaryBitOrRR: storing in result_reg={}", result_reg);

                // Ensure result register exists, expand if needed
                if result_reg >= self.frames[frame_idx].registers.len() {
                    // eprintln!("DEBUG BinaryBitOrRR: WARNING - result_reg {} >= registers.len() {}", result_reg, self.frames[frame_idx].registers.len());
                    // Expand the register array
                    self.frames[frame_idx].registers.resize(result_reg + 1, RcValue::new(Value::None));
                    // eprintln!("DEBUG BinaryBitOrRR: Expanded registers to {}", self.frames[frame_idx].registers.len());
                }
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result.clone());

                // eprintln!("DEBUG BinaryBitOrRR: Stored result, verifying...");
                // eprintln!("DEBUG BinaryBitOrRR: registers[{}] = {:?}", result_reg, self.frames[frame_idx].registers[result_reg].value);
                Ok(None)
            }
            OpCode::CompareEqualRR => {
                // Register-Register equality comparison
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a == b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a == b),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a == b),
                    (Value::Bool(a), Value::Bool(b)) => Value::Bool(a == b),
                    _ => {
                        // For other types, use the general comparison
                        Value::Bool(left.value == right.value)
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareNotEqualRR => {
                // Register-Register not equal comparison
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareNotEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a != b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a != b),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a != b),
                    (Value::Bool(a), Value::Bool(b)) => Value::Bool(a != b),
                    _ => {
                        // For other types, use the general comparison
                        Value::Bool(left.value != right.value)
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareGreaterRR => {
                // Register-Register greater than comparison
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareGreaterRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a > b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a > b),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a > b),
                    _ => {
                        // For other types, use the general comparison
                        match left.value.partial_cmp(&right.value) {
                            Some(std::cmp::Ordering::Greater) => Value::Bool(true),
                            _ => Value::Bool(false),
                        }
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareGreaterEqualRR => {
                // Register-Register greater than or equal comparison
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareGreaterEqualRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for integer comparison
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Bool(a >= b),
                    (Value::Float(a), Value::Float(b)) => Value::Bool(a >= b),
                    (Value::Str(a), Value::Str(b)) => Value::Bool(a >= b),
                    _ => {
                        // For other types, use the general comparison
                        match left.value.partial_cmp(&right.value) {
                            Some(std::cmp::Ordering::Greater) | Some(std::cmp::Ordering::Equal) => Value::Bool(true),
                            _ => Value::Bool(false),
                        }
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareInRR => {
                // Register-Register membership test (in)
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareInRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Check membership
                let result = match (&left.value, &right.value) {
                    // String membership
                    (Value::Str(item), Value::Str(container)) => Value::Bool(container.contains(item)),
                    // List membership
                    (item, Value::List(container)) => {
                        let found = container.iter().any(|list_item| list_item == item);
                        Value::Bool(found)
                    },
                    // Tuple membership
                    (item, Value::Tuple(container)) => {
                        let found = container.iter().any(|tuple_item| tuple_item == item);
                        Value::Bool(found)
                    },
                    // Set membership
                    (item, Value::Set(container)) => {
                        let found = container.iter().any(|set_item| set_item == item);
                        Value::Bool(found)
                    },
                    // Dict membership (check keys)
                    (item, Value::Dict(container)) => {
                        // For dict membership, we check if the item is a key in the dict
                        match item {
                            Value::Str(key) => Value::Bool(container.borrow().contains_key(key)),
                            _ => {
                                let key_str = format!("{}", item);
                                Value::Bool(container.borrow().contains_key(&key_str))
                            }
                        }
                    },
                    _ => {
                        // For other types, try to convert to string and check string membership
                        let left_str = format!("{}", left.value);
                        let right_str = format!("{}", right.value);
                        Value::Bool(right_str.contains(&left_str))
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CompareNotInRR => {
                // Register-Register non-membership test (not in)
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CompareNotInRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Check non-membership (opposite of membership)
                let result = match (&left.value, &right.value) {
                    // String non-membership
                    (Value::Str(item), Value::Str(container)) => Value::Bool(!container.contains(item)),
                    // List non-membership
                    (item, Value::List(container)) => {
                        let found = container.iter().any(|list_item| list_item == item);
                        Value::Bool(!found)
                    },
                    // Tuple non-membership
                    (item, Value::Tuple(container)) => {
                        let found = container.iter().any(|tuple_item| tuple_item == item);
                        Value::Bool(!found)
                    },
                    // Set non-membership
                    (item, Value::Set(container)) => {
                        let found = container.iter().any(|set_item| set_item == item);
                        Value::Bool(!found)
                    },
                    // Dict non-membership (check keys)
                    (item, Value::Dict(container)) => {
                        // For dict non-membership, we check if the item is NOT a key in the dict
                        match item {
                            Value::Str(key) => Value::Bool(!container.borrow().contains_key(key)),
                            _ => {
                                let key_str = format!("{}", item);
                                Value::Bool(!container.borrow().contains_key(&key_str))
                            }
                        }
                    },
                    _ => {
                        // For other types, try to convert to string and check string non-membership
                        let left_str = format!("{}", left.value);
                        let right_str = format!("{}", right.value);
                        Value::Bool(!right_str.contains(&left_str))
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::LoadFast => {
                // Load from fast local variable (indexed access)
                let local_idx = arg1 as usize;
                let result_reg = arg2 as u32;

                // eprintln!("DEBUG LoadFast: local_idx={}, result_reg={}", local_idx, result_reg);
                // eprintln!("DEBUG LoadFast: locals.len()={}", self.frames[frame_idx].locals.len());

                if local_idx >= self.frames[frame_idx].locals.len() {
                    return Err(anyhow!("LoadFast: local variable index {} out of bounds (len: {})", local_idx, self.frames[frame_idx].locals.len()));
                }

                let value = self.frames[frame_idx].locals[local_idx].clone();
                // eprintln!("DEBUG LoadFast: Loading locals[{}] = {:?} into register {}", local_idx, value.value, result_reg);
                self.frames[frame_idx].set_register(result_reg, value.clone());
                // eprintln!("DEBUG LoadFast: Loaded! registers[{}] = {:?}", result_reg, value.value);
                Ok(None)
            }
            OpCode::StoreFast => {
                // Store to fast local variable (indexed access)
                let value_reg = arg1 as usize;
                let local_idx = arg2 as usize;

                // eprintln!("DEBUG StoreFast: value_reg={}, local_idx={}", value_reg, local_idx);
                // eprintln!("DEBUG StoreFast: registers.len()={}, locals.len()={}",
                //     self.frames[frame_idx].registers.len(), self.frames[frame_idx].locals.len());

                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreFast: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }

                let value = self.frames[frame_idx].registers[value_reg].clone();
                // eprintln!("DEBUG StoreFast: Storing value {:?} from register {} to local {}", value.value, value_reg, local_idx);

                if local_idx >= self.frames[frame_idx].locals.len() {
                    // Extend locals if needed
                    // eprintln!("DEBUG StoreFast: Extending locals from {} to {}", self.frames[frame_idx].locals.len(), local_idx + 1);
                    self.frames[frame_idx].locals.resize(local_idx + 1, RcValue::new(Value::None));
                }

                self.frames[frame_idx].locals[local_idx] = value.clone();
                // eprintln!("DEBUG StoreFast: Stored! Verifying locals[{}] = {:?}", local_idx, self.frames[frame_idx].locals[local_idx].value);
                Ok(None)
            }
            OpCode::LoadClosure => {
                // Load from closure variable
                let closure_idx = arg1 as usize;
                let result_reg = arg2 as u32;
                
                if closure_idx >= self.frames[frame_idx].free_vars.len() {
                    return Err(anyhow!("LoadClosure: closure variable index {} out of bounds (len: {})", closure_idx, self.frames[frame_idx].free_vars.len()));
                }
                
                let value = self.frames[frame_idx].free_vars[closure_idx].clone();
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::StoreClosure => {
                // Store to closure variable
                let value_reg = arg1 as usize;
                let closure_idx = arg2 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreClosure: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                if closure_idx >= self.frames[frame_idx].free_vars.len() {
                    // Extend free_vars if needed
                    self.frames[frame_idx].free_vars.resize(closure_idx + 1, RcValue::new(Value::None));
                }
                
                let value = self.frames[frame_idx].registers[value_reg].clone();
                self.frames[frame_idx].free_vars[closure_idx] = value;
                Ok(None)
            }
            OpCode::LoadLocal => {
                // Load from local register
                let local_idx = arg1 as usize;
                let result_reg = arg2 as u32;

                // eprintln!("DEBUG LoadLocal: local_idx={}, result_reg={}", local_idx, result_reg);
                // eprintln!("DEBUG LoadLocal: locals.len()={}", self.frames[frame_idx].locals.len());

                if local_idx >= self.frames[frame_idx].locals.len() {
                    return Err(anyhow!("LoadLocal: local variable index {} out of bounds (len: {})", local_idx, self.frames[frame_idx].locals.len()));
                }

                // Clone the value to avoid borrowing conflicts
                let value = self.frames[frame_idx].locals[local_idx].clone();
                // eprintln!("DEBUG LoadLocal: Loading locals[{}] = {:?} into register {}", local_idx, value.value, result_reg);
                self.frames[frame_idx].set_register(result_reg, value.clone());
                // eprintln!("DEBUG LoadLocal: Loaded! registers[{}] = {:?}", result_reg, value.value);
                Ok(None)
            }
            OpCode::StoreLocal => {
                // Store to local register
                let value_reg = arg1 as usize;
                let local_idx = arg2 as usize;

                // eprintln!("DEBUG StoreLocal: value_reg={}, local_idx={}", value_reg, local_idx);
                // eprintln!("DEBUG StoreLocal: registers.len()={}, locals.len()={}",
                //     self.frames[frame_idx].registers.len(), self.frames[frame_idx].locals.len());

                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreLocal: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }

                // Clone the value to avoid borrowing conflicts
                let value = self.frames[frame_idx].registers[value_reg].clone();
                // eprintln!("DEBUG StoreLocal: Storing value {:?} from register {} to local {}", value.value, value_reg, local_idx);

                if local_idx >= self.frames[frame_idx].locals.len() {
                    // Extend locals if needed
                    // eprintln!("DEBUG StoreLocal: Extending locals from {} to {}", self.frames[frame_idx].locals.len(), local_idx + 1);
                    self.frames[frame_idx].locals.resize(local_idx + 1, RcValue::new(Value::None));
                }

                self.frames[frame_idx].locals[local_idx] = value.clone();
                // eprintln!("DEBUG StoreLocal: Stored! Verifying locals[{}] = {:?}", local_idx, self.frames[frame_idx].locals[local_idx].value);
                Ok(None)
            }
            OpCode::MoveReg => {
                // Move value from one register to another
                let source_reg = arg1 as usize;
                let target_reg = arg2 as usize;
                
                if source_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MoveReg: source register index {} out of bounds (len: {})", source_reg, self.frames[frame_idx].registers.len()));
                }
                
                if target_reg >= self.frames[frame_idx].registers.len() {
                    // Extend registers if needed
                    while self.frames[frame_idx].registers.len() <= target_reg {
                        self.frames[frame_idx].registers.push(RcValue::new(Value::None));
                    }
                }
                
                // Clone the value to avoid borrowing conflicts
                let value = self.frames[frame_idx].registers[source_reg].clone();
                self.frames[frame_idx].registers[target_reg] = value;
                Ok(None)
            }
            OpCode::StoreGlobal => {
                // Store to global namespace
                let value_reg = arg1 as usize;
                let name_idx = arg2 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreGlobal: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("StoreGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                let value = self.frames[frame_idx].registers[value_reg].clone();
                let name = self.frames[frame_idx].code.names[name_idx].clone();

                // Debug output
                // eprintln!("DEBUG StoreGlobal: storing '{}' = {:?}", name, value.value);

                // Strong static typing: check if variable has a declared type
                if let Some(declared_type) = self.typed_variables.get(&name) {
                    if !self.check_type_match(&value.value, declared_type) {
                        return Err(anyhow!(
                            "TypeError: Cannot assign value of type '{}' to variable '{}' of type '{}'",
                            value.value.type_name(),
                            name,
                            declared_type
                        ));
                    }
                }

                // Store in frame globals (which is shared with self.globals via Rc<RefCell>)
                self.frames[frame_idx].globals.borrow_mut().insert(name.clone(), value.clone());

                // Debug output
                // eprintln!("DEBUG StoreGlobal: stored '{}' in globals", name);
                Ok(None)
            }
            OpCode::LoadGlobal => {
                // Load from global namespace
                let name_idx = arg1 as usize;
                let result_reg = arg2 as u32;
                
                // DEBUG: Print the names vector for debugging
                // eprintln!("DEBUG: Names vector: {:?}", self.frames[frame_idx].code.names);
                // eprintln!("DEBUG: Trying to load name at index {}", name_idx);
                
                // Get the name first to avoid borrowing conflicts
                let name = {
                    if name_idx >= self.frames[frame_idx].code.names.len() {
                        return Err(anyhow!("LoadGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                    }
                    self.frames[frame_idx].code.names[name_idx].clone()
                };
                
                // DEBUG: Print the name being loaded
                // eprintln!("DEBUG: Loading name '{}' from index {}", name, name_idx);
                
                // Check if the name exists in any of the global scopes
                let value = {
                    // Check frame globals
                    if self.frames[frame_idx].globals.borrow().contains_key(&name) {
                        self.frames[frame_idx].globals.borrow().get(&name).cloned()
                    }
                    // Then check builtins
                    else if self.frames[frame_idx].builtins.borrow().contains_key(&name) {
                        // DEBUG: Print if found in builtins
                        // eprintln!("DEBUG: Found '{}' in builtins", name);
                        self.frames[frame_idx].builtins.borrow().get(&name).cloned()
                    }
                    // Then check VM globals
                    else if self.globals.borrow().contains_key(&name) {
                        self.globals.borrow().get(&name).cloned()
                    } else {
                        None
                    }
                };
                
                if let Some(value) = value {
                    self.frames[frame_idx].set_register(result_reg, value);
                    Ok(None)
                } else {
                    // More descriptive error message to help debugging
                    Err(anyhow!("NameError: name '{}' is not defined", name))
                }
            }
            OpCode::LoadClassDeref => {
                // Load from class dereference (for super() calls and class variables)
                // arg1 = name index, arg2 = result register
                let name_idx = arg1 as usize;
                let result_reg = arg2 as u32;
                
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadClassDeref: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                let name = self.frames[frame_idx].code.names[name_idx].clone();
                
                // First, try to find the name in the current class's namespace
                // This is used for accessing class variables from within methods
                if let Some(current_class_name) = &self.frames[frame_idx].code.name.strip_prefix("<fn:") {
                    if let Some(class_name) = current_class_name.strip_suffix(">") {
                        // Look for the class in globals
                        if let Some(class_value) = self.globals.borrow().get(class_name).cloned() {
                            if let Value::Class { methods, .. } = &class_value.value {
                                // Check if the name is a class method
                                if let Some(method) = methods.get(&name) {
                                    self.frames[frame_idx].set_register(result_reg, RcValue::new(method.clone()));
                                    return Ok(None);
                                }
                            }
                        }
                    }
                }
                
                // If not found in class, fall back to global lookup
                let value = {
                    // Check frame globals
                    if self.frames[frame_idx].globals.borrow().contains_key(&name) {
                        self.frames[frame_idx].globals.borrow().get(&name).cloned()
                    }
                    // Then check builtins
                    else if self.frames[frame_idx].builtins.borrow().contains_key(&name) {
                        self.frames[frame_idx].builtins.borrow().get(&name).cloned()
                    }
                    // Then check VM globals
                    else if self.globals.borrow().contains_key(&name) {
                        self.globals.borrow().get(&name).cloned()
                    } else {
                        None
                    }
                };
                
                if let Some(value) = value {
                    self.frames[frame_idx].set_register(result_reg, value);
                    Ok(None)
                } else {
                    Err(anyhow!("LoadClassDeref: name '{}' not found", name))
                }
            }
            OpCode::BuildList => {
                // Build a list from a set of registers
                let num_items = arg1 as usize;
                let start_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // Collect register indices first to avoid borrowing conflicts
                let register_indices: Vec<usize> = (0..num_items)
                    .map(|i| start_reg + i)
                    .collect();

                // Check bounds first
                for &reg_idx in &register_indices {
                    if reg_idx >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildList: register index {} out of bounds (len: {})", reg_idx, self.frames[frame_idx].registers.len()));
                    }
                }

                // Collect values
                let items: Vec<Value> = register_indices
                    .into_iter()
                    .map(|reg_idx| self.frames[frame_idx].registers[reg_idx].value.clone())
                    .collect();

                let list = Value::List(HPList::from_values(items));
                self.frames[frame_idx].registers[result_reg] = RcValue::new(list);
                Ok(None)
            }
            OpCode::BuildTuple => {
                // Build a tuple from a set of registers
                let num_items = arg1 as usize;
                let start_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // Collect register indices first to avoid borrowing conflicts
                let register_indices: Vec<usize> = (0..num_items)
                    .map(|i| start_reg + i)
                    .collect();

                // Check bounds first
                for &reg_idx in &register_indices {
                    if reg_idx >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildTuple: register index {} out of bounds (len: {})", reg_idx, self.frames[frame_idx].registers.len()));
                    }
                }

                // Collect values
                let items: Vec<Value> = register_indices
                    .into_iter()
                    .map(|reg_idx| self.frames[frame_idx].registers[reg_idx].value.clone())
                    .collect();

                let tuple = Value::Tuple(items);
                self.frames[frame_idx].registers[result_reg] = RcValue::new(tuple);
                Ok(None)
            }
            OpCode::BuildDict => {
                // Build a dictionary from a set of key-value pairs in registers
                let num_pairs = arg1 as usize;
                let start_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // Collect register indices first to avoid borrowing conflicts
                let register_pairs: Vec<(usize, usize)> = (0..num_pairs)
                    .map(|i| (start_reg + i * 2, start_reg + i * 2 + 1))
                    .collect();

                // Check bounds first
                for &(key_reg, value_reg) in &register_pairs {
                    if key_reg >= self.frames[frame_idx].registers.len() || value_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildDict: register index out of bounds"));
                    }
                }

                // Collect key-value pairs
                let mut items = HashMap::new();
                for &(key_reg, value_reg) in &register_pairs {
                    let key_value = &self.frames[frame_idx].registers[key_reg].value;
                    let value_value = &self.frames[frame_idx].registers[value_reg].value;

                    // Keys must be strings
                    let key_str = match key_value {
                        Value::Str(s) => s.clone(),
                        _ => format!("{}", key_value),
                    };

                    items.insert(key_str, value_value.clone());
                }

                let dict = Value::Dict(Rc::new(RefCell::new(items)));
                self.frames[frame_idx].registers[result_reg] = RcValue::new(dict);
                Ok(None)
            }
            OpCode::BuildSet => {
                // Build a set from a set of registers
                let num_items = arg1 as usize;
                let start_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                // Collect register indices first to avoid borrowing conflicts
                let register_indices: Vec<usize> = (0..num_items)
                    .map(|i| start_reg + i)
                    .collect();

                // Check bounds first
                for &reg_idx in &register_indices {
                    if reg_idx >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildSet: register index {} out of bounds (len: {})", reg_idx, self.frames[frame_idx].registers.len()));
                    }
                }

                // Collect values
                let items: Vec<Value> = register_indices
                    .into_iter()
                    .map(|reg_idx| self.frames[frame_idx].registers[reg_idx].value.clone())
                    .collect();

                let set = Value::Set(items);
                self.frames[frame_idx].registers[result_reg] = RcValue::new(set);
                Ok(None)
            }
            OpCode::Jump => {
                // Unconditional jump
                let target = arg1 as usize;
                self.frames[frame_idx].pc = target;
                Ok(None)
            }
            OpCode::JumpIfFalse => {
                // Jump if value is false
                let cond_reg = arg1 as usize;
                let target = arg2 as usize;
                
                if cond_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("JumpIfFalse: register index {} out of bounds (len: {})", cond_reg, self.frames[frame_idx].registers.len()));
                }
                
                let cond_value = &self.frames[frame_idx].registers[cond_reg];
                if !cond_value.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::JumpIfTrue => {
                // Jump if value is true
                let cond_reg = arg1 as usize;
                let target = arg2 as usize;
                
                if cond_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("JumpIfTrue: register index {} out of bounds (len: {})", cond_reg, self.frames[frame_idx].registers.len()));
                }
                
                let cond_value = &self.frames[frame_idx].registers[cond_reg];
                if cond_value.is_truthy() {
                    self.frames[frame_idx].pc = target;
                }
                Ok(None)
            }
            OpCode::UnaryNot => {
                // Logical NOT operation - negate boolean value
                let operand_reg = arg1 as usize;
                let result_reg = arg2 as usize;

                if operand_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("UnaryNot: operand register index {} out of bounds (len: {})", operand_reg, self.frames[frame_idx].registers.len()));
                }

                // Get the truthiness of the operand and negate it
                let operand_value = &self.frames[frame_idx].registers[operand_reg];
                let is_truthy = operand_value.is_truthy();
                let result = Value::Bool(!is_truthy);

                self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                Ok(None)
            }
            OpCode::UnaryNegate => {
                // Unary negation operation (-)
                let operand_reg = arg1 as usize;
                let result_reg = arg2 as usize;

                if operand_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("UnaryNegate: operand register index {} out of bounds (len: {})", operand_reg, self.frames[frame_idx].registers.len()));
                }

                let operand_value = &self.frames[frame_idx].registers[operand_reg].value;
                let result = match operand_value {
                    Value::Int(n) => Value::Int(-n),
                    Value::Float(f) => Value::Float(-f),
                    _ => return Err(anyhow!("UnaryNegate: unsupported operand type")),
                };

                self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                Ok(None)
            }
            OpCode::UnaryInvert => {
                // Bitwise NOT operation (~)
                // In Python: ~x == -(x + 1)
                let operand_reg = arg1 as usize;
                let result_reg = arg2 as usize;

                if operand_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("UnaryInvert: operand register index {} out of bounds (len: {})", operand_reg, self.frames[frame_idx].registers.len()));
                }

                let operand_value = &self.frames[frame_idx].registers[operand_reg].value;
                let result = match operand_value {
                    Value::Int(n) => Value::Int(!n),  // Bitwise NOT
                    Value::Bool(b) => Value::Int(if *b { -2 } else { -1 }),  // ~True == -2, ~False == -1
                    _ => return Err(anyhow!("UnaryInvert: unsupported operand type for bitwise NOT (expected int or bool, got {:?})", operand_value)),
                };

                self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                Ok(None)
            }
            OpCode::RegisterType => {
                // Register a variable's declared type for strong static typing enforcement
                // arg1 = variable name index, arg2 = type constant index
                let name_idx = arg1 as usize;
                let type_const_idx = arg2 as usize;

                let var_name = self.frames[frame_idx].code.names.get(name_idx)
                    .ok_or_else(|| anyhow!("RegisterType: name index {} out of bounds", name_idx))?;

                let type_str = match self.frames[frame_idx].code.constants.get(type_const_idx) {
                    Some(Value::Str(s)) => s.clone(),
                    _ => return Err(anyhow!("RegisterType: type constant is not a string")),
                };

                // Store the type in our typed_variables map for strong static typing
                self.typed_variables.insert(var_name.clone(), type_str.clone());

                // Also register with the type checker if type checking is enabled
                if self.enable_type_checking {
                    // Parse the type string and register it
                    let parsed_type = crate::bytecode::type_checking::parse_type_string(&type_str)
                        .map_err(|e| anyhow!("RegisterType: failed to parse type '{}': {}", type_str, e))?;

                    self.type_checker.type_env.register_variable(var_name.clone(), parsed_type);
                }
                Ok(None)
            }
            OpCode::CheckType => {
                // Check if a value matches a declared type
                // arg1 = variable name index, arg2 = value register, arg3 = type constant index
                if !self.enable_type_checking {
                    return Ok(None);
                }

                let name_idx = arg1 as usize;
                let value_reg = arg2 as usize;
                let type_const_idx = arg3 as usize;

                let var_name = self.frames[frame_idx].code.names.get(name_idx)
                    .ok_or_else(|| anyhow!("CheckType: name index {} out of bounds", name_idx))?;

                let value = &self.frames[frame_idx].registers[value_reg].value;

                let type_str = match self.frames[frame_idx].code.constants.get(type_const_idx) {
                    Some(Value::Str(s)) => s,
                    _ => return Err(anyhow!("CheckType: type constant is not a string")),
                };

                // Check the value against the type
                crate::bytecode::type_checking::check_value_against_type_string(value, type_str, &self.type_checker)
                    .map_err(|e| anyhow!("Type error in variable '{}': {}", var_name, e))?;

                Ok(None)
            }
            OpCode::CheckFunctionParam => {
                // Check function parameter type
                // arg1 = function name index, arg2 = param name index, arg3 = value register
                if !self.enable_type_checking {
                    return Ok(None);
                }

                // TODO: Implement parameter type checking
                // For now, this is a placeholder
                Ok(None)
            }
            OpCode::CheckFunctionReturn => {
                // Check function return type
                // arg1 = function name index, arg2 = return value register, arg3 = type constant index
                if !self.enable_type_checking {
                    return Ok(None);
                }

                let func_name_idx = arg1 as usize;
                let value_reg = arg2 as usize;
                let type_const_idx = arg3 as usize;

                let func_name = self.frames[frame_idx].code.names.get(func_name_idx)
                    .ok_or_else(|| anyhow!("CheckFunctionReturn: function name index {} out of bounds", func_name_idx))?;

                let value = &self.frames[frame_idx].registers[value_reg].value;

                let type_str = match self.frames[frame_idx].code.constants.get(type_const_idx) {
                    Some(Value::Str(s)) => s,
                    _ => return Err(anyhow!("CheckFunctionReturn: type constant is not a string")),
                };

                // Check the return value against the expected return type
                crate::bytecode::type_checking::check_value_against_type_string(value, type_str, &self.type_checker)
                    .map_err(|e| anyhow!("Type error in function '{}' return value: {}", func_name, e))?;

                Ok(None)
            }
            OpCode::CheckAttrType => {
                // Check attribute assignment type
                // arg1 = object register, arg2 = attribute name index, arg3 = value register
                if !self.enable_type_checking {
                    return Ok(None);
                }

                let object_reg = arg1 as usize;
                let attr_name_idx = arg2 as usize;
                let value_reg = arg3 as usize;

                let attr_name = self.frames[frame_idx].code.names.get(attr_name_idx)
                    .ok_or_else(|| anyhow!("CheckAttrType: attribute name index {} out of bounds", attr_name_idx))?;

                let object = &self.frames[frame_idx].registers[object_reg].value;
                let value = &self.frames[frame_idx].registers[value_reg].value;

                // Get the class name from the object
                if let Value::Object { class_name, .. } = object {
                    if let Some(class_type) = self.type_checker.type_env.get_class_type(class_name) {
                        if let Some(expected_type) = class_type.attribute_types.get(attr_name) {
                            self.type_checker.check_type(value, expected_type)
                                .map_err(|e| anyhow!("Type error in attribute '{}' of class '{}': {}", attr_name, class_name, e))?;
                        }
                    }
                }

                Ok(None)
            }
            OpCode::InferType => {
                // Infer type from a value and register it
                // arg1 = variable name index, arg2 = value register
                if !self.enable_type_checking || !self.type_checker.type_env.enable_type_inference {
                    return Ok(None);
                }

                let name_idx = arg1 as usize;
                let value_reg = arg2 as usize;

                let var_name = self.frames[frame_idx].code.names.get(name_idx)
                    .ok_or_else(|| anyhow!("InferType: name index {} out of bounds", name_idx))?;

                let value = &self.frames[frame_idx].registers[value_reg].value;

                // Infer and store the type
                self.type_checker.type_env.infer_type(var_name.clone(), value);

                Ok(None)
            }
            OpCode::ReturnValue => {
                // Return a value from the current function
                let value_reg = arg1 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("ReturnValue: register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                let return_value = self.frames[frame_idx].registers[value_reg].value.clone();
                Ok(Some(return_value))
            }
            OpCode::SubscrLoad => {
                // Load item from sequence (obj[key])
                let object_reg = arg1 as usize;
                let index_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() || index_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("SubscrLoad: register index out of bounds"));
                }

                let object_value = &self.frames[frame_idx].registers[object_reg];
                let index_value = &self.frames[frame_idx].registers[index_reg];

                // eprintln!("DEBUG SubscrLoad: object_reg={}, index={:?}, object_type={}",
                //          object_reg, index_value.value, object_value.value.type_name());

                // Handle different sequence types
                let result = match (&object_value.value, &index_value.value) {
                    (Value::List(items), Value::Int(index)) => {
                        let normalized_index = if *index < 0 {
                            items.len() as i64 + *index
                        } else {
                            *index
                        };

                        if normalized_index >= 0 && normalized_index < items.len() as i64 {
                            items.get(normalized_index as isize).unwrap().clone()
                        } else {
                            return Err(anyhow!("Index {} out of range for list of length {}", index, items.len()));
                        }
                    },
                    (Value::Tuple(items), Value::Int(index)) => {
                        let normalized_index = if *index < 0 {
                            items.len() as i64 + *index
                        } else {
                            *index
                        };

                        if normalized_index >= 0 && normalized_index < items.len() as i64 {
                            items[normalized_index as usize].clone()
                        } else {
                            return Err(anyhow!("Index {} out of range for tuple of length {}", index, items.len()));
                        }
                    },
                    (Value::Str(s), Value::Int(index)) => {
                        let normalized_index = if *index < 0 {
                            s.len() as i64 + *index
                        } else {
                            *index
                        };

                        if normalized_index >= 0 && normalized_index < s.len() as i64 {
                            Value::Str(s.chars().nth(normalized_index as usize).unwrap().to_string())
                        } else {
                            return Err(anyhow!("Index {} out of range for string of length {}", index, s.len()));
                        }
                    },
                    (Value::Dict(dict_ref), key) => {
                        // For dictionaries, convert key to string for lookup
                        let key_str = match key {
                            Value::Str(s) => s.clone(),
                            Value::Int(n) => n.to_string(),
                            _ => format!("{}", key),
                        };

                        let dict = dict_ref.borrow();
                        // eprintln!("DEBUG SubscrLoad: looking for key='{}', dict has {} entries, keys: {:?}",
                        //          key_str, dict.len(), dict_ref.borrow().keys().collect::<Vec<_>>());

                        if let Some(value) = dict.get(&key_str) {
                            value.clone()
                        } else {
                            return Err(anyhow!("Key '{}' not found in dictionary", key_str));
                        }
                    },
                    _ => {
                        return Err(anyhow!("Subscript not supported for types {} and {}",
                                          object_value.value.type_name(), index_value.value.type_name()));
                    }
                };

                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::SubscrDelete => {
                // Delete item from sequence (del obj[key])
                let object_reg = arg1 as usize;
                let index_reg = arg2 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() || index_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("SubscrDelete: register index out of bounds"));
                }

                // Clone the values we need to avoid borrowing issues
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let index_value = self.frames[frame_idx].registers[index_reg].value.clone();

                // Handle different sequence types
                match object_value {
                    Value::List(mut items) => {
                        if let Value::Int(index) = index_value {
                            // Use pop_at to remove the item at the specified index
                            // Convert i64 to isize for the HPList API
                            match items.pop_at(index as isize) {
                                Ok(_) => {
                                    self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(items));
                                    Ok(None)
                                }
                                Err(_) => Err(anyhow!("Index {} out of range for list of length {}", index, items.len()))
                            }
                        } else {
                            Err(anyhow!("List indices must be integers, not {}", index_value.type_name()))
                        }
                    },
                    Value::Dict(dict_ref) => {
                        // For dictionaries, convert key to string for lookup
                        let key_str = match index_value {
                            Value::Str(s) => s,
                            Value::Int(n) => n.to_string(),
                            _ => format!("{}", index_value),
                        };

                        dict_ref.borrow_mut().remove(&key_str);
                        Ok(None)
                    },
                    Value::Set(mut items) => {
                        // For sets, we remove the item if it exists
                        items.retain(|item| item != &index_value);
                        self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::Set(items));
                        Ok(None)
                    },
                    _ => {
                        Err(anyhow!("Subscript deletion not supported for type {}",
                                  object_value.type_name()))
                    }
                }
            }
            OpCode::MakeFunction => {
                // Create a function object from a code object
                let code_reg = arg1 as usize;
                let result_reg = arg2 as u32;
                
                if code_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MakeFunction: code register index {} out of bounds (len: {})", code_reg, self.frames[frame_idx].registers.len()));
                }
                
                let code_value = &self.frames[frame_idx].registers[code_reg];
                
                match &code_value.value {
                    Value::Code(code_obj) => {
                        // Create a closure with the code object
                        let closure = Value::Closure {
                            name: code_obj.name.clone(),
                            params: code_obj.params.clone(),
                            body: vec![], // Empty body since it's in the compiled code
                            captured_scope: HashMap::new(), // No captured scope for now
                            docstring: None, // No docstring for now
                            compiled_code: Some(Box::new(*code_obj.clone())),
                            module_globals: None,
                        };
                        
                        self.frames[frame_idx].set_register(result_reg, RcValue::new(closure));
                        Ok(None)
                    }
                    _ => Err(anyhow!("MakeFunction: expected code object, got {}", code_value.value.type_name())),

                }
            }
            OpCode::SubscrStore => {
                // Store item to sequence (obj[key] = value)
                let object_reg = arg1 as usize;
                let index_reg = arg2 as usize;
                let value_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() ||
                   index_reg >= self.frames[frame_idx].registers.len() ||
                   value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("SubscrStore: register index out of bounds"));
                }

                // Clone the values we need to avoid borrowing issues
                let index_value = self.frames[frame_idx].registers[index_reg].value.clone();
                let value_to_store = self.frames[frame_idx].registers[value_reg].value.clone();

                // eprintln!("DEBUG SubscrStore: object_reg={}, index={:?}, value={:?}",
                //          object_reg, index_value, value_to_store);

                // Handle different sequence types
                match &mut self.frames[frame_idx].registers[object_reg].value {
                    Value::List(items) => {
                        if let Value::Int(index) = index_value {
                            let normalized_index = if index < 0 {
                                items.len() as i64 + index
                            } else {
                                index
                            };

                            if normalized_index >= 0 && normalized_index < items.len() as i64 {
                                items.set(normalized_index as isize, value_to_store)
                                    .map_err(|e| anyhow!("Error setting list item: {}", e))?;
                            } else {
                                return Err(anyhow!("Index {} out of range for list of length {}", index, items.len()));
                            }
                        } else {
                            return Err(anyhow!("List indices must be integers, not {}", index_value.type_name()));
                        }
                    },
                    Value::Dict(dict_ref) => {
                        // For dictionaries, convert key to string for lookup
                        let key_str = match index_value {
                            Value::Str(s) => s,
                            Value::Int(n) => n.to_string(),
                            _ => format!("{}", index_value),
                        };

                        let mut dict = dict_ref.borrow_mut();
                        // eprintln!("DEBUG SubscrStore: inserting key='{}', dict had {} entries before", key_str, dict.len());
                        dict.insert(key_str.clone(), value_to_store);
                        // eprintln!("DEBUG SubscrStore: dict now has {} entries, contains key: {}", dict.len(), dict.contains_key(&key_str));
                    },
                    _ => {
                        return Err(anyhow!("Subscript assignment not supported for type {}",
                                          self.frames[frame_idx].registers[object_reg].value.type_name()));
                    }
                };

                Ok(None)
            }
            OpCode::Slice => {
                // Create a slice: object[start:stop:step]
                let object_reg = arg1 as usize;
                let start_reg = arg2 as usize;
                let stop_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() ||
                   start_reg >= self.frames[frame_idx].registers.len() ||
                   stop_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("Slice: register index out of bounds"));
                }

                let object_value = &self.frames[frame_idx].registers[object_reg].value;
                let start_value = &self.frames[frame_idx].registers[start_reg].value;
                let stop_value = &self.frames[frame_idx].registers[stop_reg].value;

                // Extract start and stop indices
                let start_idx = match start_value {
                    Value::Int(n) => Some(*n),
                    Value::None => None,
                    _ => return Err(anyhow!("Slice start must be an integer or None")),
                };

                let stop_idx = match stop_value {
                    Value::Int(n) => Some(*n),
                    Value::None => None,
                    _ => return Err(anyhow!("Slice stop must be an integer or None")),
                };

                // Perform slicing based on object type
                let result = match object_value {
                    Value::List(items) => {
                        let len = items.len() as i64;

                        // Normalize indices
                        let start = start_idx.unwrap_or(0);
                        let stop = stop_idx.unwrap_or(len);

                        let normalized_start = if start < 0 {
                            (len + start).max(0) as usize
                        } else {
                            start.min(len) as usize
                        };

                        let normalized_stop = if stop < 0 {
                            (len + stop).max(0) as usize
                        } else {
                            stop.min(len) as usize
                        };

                        // Extract slice
                        let slice: Vec<Value> = items.as_vec()
                            .iter()
                            .skip(normalized_start)
                            .take(normalized_stop.saturating_sub(normalized_start))
                            .cloned()
                            .collect();

                        Value::List(crate::modules::hplist::HPList::from_values(slice))
                    },
                    Value::Tuple(items) => {
                        let len = items.len() as i64;

                        // Normalize indices
                        let start = start_idx.unwrap_or(0);
                        let stop = stop_idx.unwrap_or(len);

                        let normalized_start = if start < 0 {
                            (len + start).max(0) as usize
                        } else {
                            start.min(len) as usize
                        };

                        let normalized_stop = if stop < 0 {
                            (len + stop).max(0) as usize
                        } else {
                            stop.min(len) as usize
                        };

                        // Extract slice
                        let slice: Vec<Value> = items
                            .iter()
                            .skip(normalized_start)
                            .take(normalized_stop.saturating_sub(normalized_start))
                            .cloned()
                            .collect();

                        Value::Tuple(slice)
                    },
                    Value::Str(s) => {
                        let len = s.len() as i64;

                        // Normalize indices
                        let start = start_idx.unwrap_or(0);
                        let stop = stop_idx.unwrap_or(len);

                        let normalized_start = if start < 0 {
                            (len + start).max(0) as usize
                        } else {
                            start.min(len) as usize
                        };

                        let normalized_stop = if stop < 0 {
                            (len + stop).max(0) as usize
                        } else {
                            stop.min(len) as usize
                        };

                        // Extract substring
                        let slice: String = s
                            .chars()
                            .skip(normalized_start)
                            .take(normalized_stop.saturating_sub(normalized_start))
                            .collect();

                        Value::Str(slice)
                    },
                    _ => {
                        return Err(anyhow!("Slicing not supported for type {}",
                                          object_value.type_name()));
                    }
                };

                // Store result back in object register
                self.frames[frame_idx].registers[object_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::CallMethod => {
                // Call a method on an object
                let object_reg = arg1 as usize;
                let arg_count = arg2 as usize;
                let method_name_idx = arg3 as usize;
                
                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallMethod: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }
                
                if method_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("CallMethod: method name index {} out of bounds (len: {})", method_name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                // Get the method name
                let method_name = self.frames[frame_idx].code.names[method_name_idx].clone();
                
                // Collect arguments from registers
                let mut args = Vec::new();
                for i in 0..arg_count {
                    // Arguments are stored in consecutive registers after the object register
                    let arg_reg = object_reg + 1 + i;
                    if arg_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallMethod: argument register index {} out of bounds (len: {})", arg_reg, self.frames[frame_idx].registers.len()));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg].value.clone());
                }
                
                // Get the object value
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                
                // Handle different types of method calls
                let result_value = match object_value {
                    Value::Super(current_class, parent_class, instance, parent_methods) => {
                        // Handle super() object method calls

                        if let Some(instance_value) = instance {
                            // Look up the parent class and search for the method
                            let globals_values: HashMap<String, Value> = self.globals.borrow().iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                            let method = if let Some(parent_class_value) = globals_values.get(&parent_class) {
                                if let Value::Class { methods, mro, .. } = parent_class_value {
                                    // First check the class's own methods
                                    if let Some(method) = methods.get(&method_name) {
                                        Some(method.clone())
                                    } else {
                                        // Then search through its MRO
                                        mro.find_method_in_mro(&method_name, &globals_values)
                                    }
                                } else {
                                    None
                                }
                            } else {
                                None
                            };

                            if let Some(method_value) = method {
                                // CRITICAL FIX for super() attribute propagation:
                                // The issue is that we need to pass locals[0] (the actual instance)
                                // to the parent method, not the instance from the Super object.
                                // This ensures that all modifications happen to the same instance object.

                                // Get the actual instance from locals[0] (this is 'self')
                                let current_instance = if !self.frames[frame_idx].locals.is_empty() {
                                    self.frames[frame_idx].locals[0].value.clone()
                                } else {
                                    *instance_value.clone()
                                };

                                // Create arguments with the current instance as self
                                let mut method_args = vec![current_instance];
                                method_args.extend(args);

                                // Call the parent method
                                // The key is NOT to extract anything after - just let StoreAttr do its job
                                // StoreAttr will update locals[0] in the child frame,
                                // and also update the result_reg in THIS frame (object_reg)
                                self.call_function_fast(
                                    method_value,
                                    method_args,
                                    HashMap::new(),
                                    Some(frame_idx),
                                    Some(object_reg as u32)
                                )?
                            } else {
                                // If the parent class is "object" and the method is not found,
                                // silently return None (object's methods are empty/noop)
                                if parent_class == "object" {
                                    Value::None
                                } else {
                                    return Err(anyhow!("super(): method '{}' not found in parent class '{}'", method_name, parent_class));
                                }
                            }
                        } else {
                            return Err(anyhow!("super(): unbound super object cannot be called directly"));
                        }
                    },
                    Value::BoundMethod { object, method_name: bound_method_name } => {
                        // For BoundMethod objects, we need to call the method from the class
                        match object.as_ref() {
                            Value::Object { class_methods, .. } => {
                                if let Some(method) = class_methods.get(&bound_method_name) {
                                    // Create arguments with self as the first argument
                                    let mut method_args = vec![*object.clone()];
                                    method_args.extend(args);

                                    // Call the method through the VM
                                    // Pass object_reg as the result register so the return value is stored correctly
                                    self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?
                                } else {
                                    return Err(anyhow!("Method '{}' not found in class methods", bound_method_name));
                                }
                            },
                            Value::Dict(_) | Value::List(_) | Value::Str(_) | Value::Set(_) | Value::Tuple(_) => {
                                // For builtin types (dict, list, str, set, tuple), get the method and call it
                                if let Some(method) = object.as_ref().get_method(&bound_method_name) {
                                    // Create arguments: method_name, self, then the actual args
                                    let mut method_args = vec![Value::Str(bound_method_name.clone()), *object.clone()];
                                    method_args.extend(args);

                                    // Call the method
                                    self.call_function_fast(method, method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?
                                } else {
                                    return Err(anyhow!("Method '{}' not found for type '{}'", bound_method_name, object.as_ref().type_name()));
                                }
                            },
                            _ => return Err(anyhow!("Bound method called on non-object type '{}'", object.as_ref().type_name())),
                        }
                    },
                    Value::Object { class_methods, mro, .. } => {
                        // For regular objects, we need to handle method calls through the VM
                        // First, try to find the method in class_methods
                        let method = if let Some(method) = class_methods.get(&method_name) {
                            Some(method.clone())
                        } else {
                            // Method not found in immediate class, search through MRO

                            // Use MRO to find the method in parent classes
                            // Convert globals from HashMap<String, RcValue> to HashMap<String, Value>
                            let globals_values: HashMap<String, Value> = self.frames[frame_idx].globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            mro.find_method_in_mro(&method_name, &globals_values)
                        };

                        if let Some(method) = method {
                            // Create arguments with self as the first argument
                            let mut method_args = vec![self.frames[frame_idx].registers[object_reg].value.clone()];
                            method_args.extend(args.clone());

                            // Call the method through the VM and capture the return value
                            // Pass object_reg as the result register so the return value is stored correctly
                            let method_result = self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                            method_result
                        } else {
                            return Err(anyhow!("Method '{}' not found in class or parent classes", method_name));
                        }
                    },
                    Value::Class { name, methods, .. } => {
                        // For Class objects, we need to handle method calls by looking up the method in the class
                        
                        if let Some(method) = methods.get(&method_name) {
                            // For class methods, the first argument should be the instance
                            // This is the correct Python semantics for calling class methods on instances
                            if args.is_empty() {
                                return Err(anyhow!("Method '{}' requires at least one argument (self)", method_name));
                            }
                            
                            // The first argument is the instance (self)
                            let instance = args[0].clone();
                            let remaining_args = args[1..].to_vec();
                            
                            // Create arguments with instance as self
                            let mut method_args = vec![instance];
                            method_args.extend(remaining_args);
                            
                            // Store the original object register value
                            let original_object_reg_value = self.frames[frame_idx].registers[object_reg].clone();
                            
                            // Call the method through the VM
                            // Pass object_reg as the result register so the return value is stored correctly
                            let method_result = self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                            
                            // CRITICAL FIX: For object field persistence during inheritance
                            // When calling parent class constructors, we need to ensure that any modifications
                            // to the object are properly propagated back to the current frame's object register
                            // Check if the object was modified during the method call and update if necessary
                            if let Some((caller_frame_idx, result_reg)) = self.frames[frame_idx].return_register {
                                if caller_frame_idx < self.frames.len() && result_reg as usize == object_reg {
                                    // Update the current frame's object register with the potentially modified object
                                    // from the caller frame (which was updated when the method frame returned)
                                    self.frames[frame_idx].registers[object_reg] = self.frames[caller_frame_idx].registers[object_reg].clone();
                                }
                            }
                            
                            method_result
                        } else {
                            return Err(anyhow!("Method '{}' not found in class methods", method_name));
                        }
                    },
                    Value::Module(_, namespace) => {
                        // For Module objects, get the function/value from the namespace
                        if let Some(value) = namespace.get(&method_name) {
                            // Call the function with the provided arguments (no self argument for module functions)
                            match value {
                                Value::BuiltinFunction(_, func) => func(args.clone())?,
                                Value::NativeFunction(func) => func(args.clone())?,
                                Value::Class { .. } | Value::Object { .. } => {
                                    // For classes and objects in modules, call through the VM
                                    // This is the critical fix for module class imports
                                    self.call_function_fast(value.clone(), args.clone(), HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                                    // For classes and objects, we don't return a value directly, the VM handles it
                                    return Ok(None);
                                },
                                Value::Closure { .. } => {
                                    // For closures, call through the VM
                                    self.call_function_fast(value.clone(), args.clone(), HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                                    // For closures, we don't return a value directly, the VM handles it
                                    return Ok(None);
                                },

                                _ => {
                                    // If it's not a callable, return an error
                                    return Err(anyhow!("'{}' in module is not callable", method_name));
                                }
                            }
                        } else {
                            return Err(anyhow!("module has no function '{}'", method_name));
                        }
                    },
                    Value::List(_) => {
                        // Handle list methods directly in the VM
                        match method_name.as_str() {
                            "append" => {
                                if args.len() != 1 {
                                    return Err(anyhow!("append() takes exactly one argument ({} given)", args.len()));
                                }
                                // CRITICAL FIX: Clone the list, modify it, and replace the register value
                                // The previous code used &mut which created a temporary borrow that didn't persist
                                if let Value::List(list) = &self.frames[frame_idx].registers[object_reg].value {
                                    let mut new_list = list.clone();
                                    new_list.push(args[0].clone());
                                    self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(new_list));
                                }
                                Value::None
                            }
                            "extend" => {
                                if args.len() != 1 {
                                    return Err(anyhow!("extend() takes exactly one argument ({} given)", args.len()));
                                }
                                // Get the iterable to extend with
                                let items_to_add = match &args[0] {
                                    Value::List(other_list) => other_list.as_vec().clone(),
                                    Value::Tuple(tuple) => tuple.clone(),
                                    _ => return Err(anyhow!("extend() argument must be iterable")),
                                };
                                // CRITICAL FIX: Clone the list, modify it, and replace the register value
                                if let Value::List(list) = &self.frames[frame_idx].registers[object_reg].value {
                                    let mut new_list = list.clone();
                                    for item in items_to_add {
                                        new_list.push(item);
                                    }
                                    self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(new_list));
                                }
                                Value::None
                            }
                            "pop" => {
                                let index = if args.is_empty() {
                                    -1i64  // Default to last element
                                } else if let Value::Int(idx) = args[0] {
                                    idx
                                } else {
                                    return Err(anyhow!("pop() argument must be an integer"));
                                };

                                // CRITICAL FIX: Clone the list, modify it, and replace the register value
                                if let Value::List(list) = &self.frames[frame_idx].registers[object_reg].value {
                                    let mut new_list = list.clone();
                                    match new_list.pop_at(index as isize) {
                                        Ok(value) => {
                                            self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(new_list));
                                            value
                                        },
                                        Err(_) => return Err(anyhow!("pop index out of range")),
                                    }
                                } else {
                                    Value::None
                                }
                            }
                            "copy" => {
                                if !args.is_empty() {
                                    return Err(anyhow!("copy() takes no arguments ({} given)", args.len()));
                                }
                                // Return a new list with the same elements
                                if let Value::List(list) = &self.frames[frame_idx].registers[object_reg].value {
                                    Value::List(list.clone())
                                } else {
                                    Value::None
                                }
                            }
                            "clear" => {
                                if !args.is_empty() {
                                    return Err(anyhow!("clear() takes no arguments ({} given)", args.len()));
                                }
                                // Clear the list (replace with empty list)
                                if let Value::List(_) = &self.frames[frame_idx].registers[object_reg].value {
                                    self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(HPList::new()));
                                }
                                Value::None
                            }
                            "reverse" => {
                                if !args.is_empty() {
                                    return Err(anyhow!("reverse() takes no arguments ({} given)", args.len()));
                                }
                                // Reverse the list in place
                                if let Value::List(list) = &self.frames[frame_idx].registers[object_reg].value {
                                    let mut new_list = list.clone();
                                    new_list.reverse();
                                    self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(new_list));
                                }
                                Value::None
                            }
                            "sort" => {
                                // sort() with optional key parameter and reverse parameter
                                // Usage: list.sort(key=None, reverse=False)
                                let mut key_func: Option<Value> = None;
                                let mut reverse = false;

                                // Parse arguments - for now, only support positional for key, named for reverse
                                // In full Python: list.sort(*, key=None, reverse=False)
                                if args.len() > 0 {
                                    if let Value::Str(arg_name) = &args[0] {
                                        if arg_name == "key" && args.len() > 1 {
                                            key_func = Some(args[1].clone());
                                        } else if arg_name == "reverse" && args.len() > 1 {
                                            if let Value::Bool(b) = args[1] {
                                                reverse = b;
                                            }
                                        }
                                    } else {
                                        // Positional argument assumed to be key function
                                        key_func = Some(args[0].clone());
                                    }
                                }

                                // Sort the list in place
                                if let Value::List(list) = &self.frames[frame_idx].registers[object_reg].value {
                                    let mut items: Vec<Value> = list.as_vec().clone();

                                    if let Some(key_fn) = key_func {
                                        // Sort with key function
                                        // We need to call the key function for each element
                                        // This requires VM access to call Python functions
                                        let mut keyed_items: Vec<(Value, Value)> = Vec::new();

                                        for item in items.iter() {
                                            // Call key_fn(item) to get the sort key
                                            let key_result = self.call_function_fast(
                                                key_fn.clone(),
                                                vec![item.clone()],
                                                HashMap::new(),
                                                Some(frame_idx),
                                                None,
                                            )?;
                                            keyed_items.push((key_result, item.clone()));
                                        }

                                        // Sort by the keys
                                        keyed_items.sort_by(|a, b| {
                                            match (&a.0, &b.0) {
                                                (Value::Int(x), Value::Int(y)) => x.cmp(y),
                                                (Value::Float(x), Value::Float(y)) => {
                                                    if x < y { std::cmp::Ordering::Less }
                                                    else if x > y { std::cmp::Ordering::Greater }
                                                    else { std::cmp::Ordering::Equal }
                                                }
                                                (Value::Str(x), Value::Str(y)) => x.cmp(y),
                                                (Value::Bool(x), Value::Bool(y)) => x.cmp(y),
                                                _ => std::cmp::Ordering::Equal,
                                            }
                                        });

                                        // Extract the original items in sorted order
                                        items = keyed_items.into_iter().map(|(_, item)| item).collect();
                                    } else {
                                        // Sort without key function (natural ordering)
                                        items.sort_by(|a, b| {
                                            match (a, b) {
                                                (Value::Int(x), Value::Int(y)) => x.cmp(y),
                                                (Value::Float(x), Value::Float(y)) => {
                                                    if x < y { std::cmp::Ordering::Less }
                                                    else if x > y { std::cmp::Ordering::Greater }
                                                    else { std::cmp::Ordering::Equal }
                                                }
                                                (Value::Str(x), Value::Str(y)) => x.cmp(y),
                                                (Value::Bool(x), Value::Bool(y)) => x.cmp(y),
                                                _ => std::cmp::Ordering::Equal,
                                            }
                                        });
                                    }

                                    // Reverse if requested
                                    if reverse {
                                        items.reverse();
                                    }

                                    // Replace the list with sorted items
                                    let mut new_list = HPList::new();
                                    for item in items {
                                        new_list.push(item);
                                    }
                                    self.frames[frame_idx].registers[object_reg] = RcValue::new(Value::List(new_list));
                                }
                                Value::None
                            }
                            _ => {
                                return Err(anyhow!("List has no method '{}'", method_name));
                            }
                        }
                    }
                    Value::Str(_) => {
                        // Handle string methods directly in the VM
                        let s_clone = if let Value::Str(s) = &object_value {
                            s.clone()
                        } else {
                            return Err(anyhow!("Internal error: expected string"));
                        };
                        match method_name.as_str() {
                            "upper" => Value::Str(s_clone.to_uppercase()),
                            "lower" => Value::Str(s_clone.to_lowercase()),
                            "capitalize" => {
                                let mut chars = s_clone.chars();
                                match chars.next() {
                                    None => Value::Str(String::new()),
                                    Some(first) => Value::Str(first.to_uppercase().collect::<String>() + chars.as_str().to_lowercase().as_str()),
                                }
                            }
                            "strip" => Value::Str(s_clone.trim().to_string()),
                            "lstrip" => Value::Str(s_clone.trim_start().to_string()),
                            "rstrip" => Value::Str(s_clone.trim_end().to_string()),
                            "encode" => {
                                // encode() returns bytes - for simplicity, use UTF-8 encoding
                                let encoding = if args.is_empty() {
                                    "utf-8"
                                } else if let Value::Str(enc) = &args[0] {
                                    enc.as_str()
                                } else {
                                    "utf-8"
                                };
                                if encoding != "utf-8" && encoding != "utf8" {
                                    return Err(anyhow!("Encoding '{}' not supported, only UTF-8 is supported", encoding));
                                }
                                Value::Bytes(s_clone.as_bytes().to_vec())
                            }
                            "isidentifier" => {
                                // Check if string is a valid Python identifier
                                let is_valid = !s_clone.is_empty()
                                    && (s_clone.chars().next().unwrap().is_alphabetic() || s_clone.starts_with('_'))
                                    && s_clone.chars().all(|c| c.is_alphanumeric() || c == '_');
                                Value::Bool(is_valid)
                            }
                            "isascii" => {
                                // Check if all characters are ASCII
                                Value::Bool(s_clone.is_ascii())
                            }
                            "partition" => {
                                // Partition string into (before, sep, after)
                                if args.is_empty() {
                                    return Err(anyhow!("partition() missing required argument: 'sep'"));
                                }
                                if let Value::Str(sep) = &args[0] {
                                    if let Some(pos) = s_clone.find(sep.as_str()) {
                                        let before = s_clone[..pos].to_string();
                                        let after = s_clone[pos + sep.len()..].to_string();
                                        Value::Tuple(vec![Value::Str(before), Value::Str(sep.clone()), Value::Str(after)])
                                    } else {
                                        Value::Tuple(vec![Value::Str(s_clone), Value::Str(String::new()), Value::Str(String::new())])
                                    }
                                } else {
                                    return Err(anyhow!("partition() argument must be a string"));
                                }
                            }
                            "rpartition" => {
                                // Reverse partition string into (before, sep, after)
                                if args.is_empty() {
                                    return Err(anyhow!("rpartition() missing required argument: 'sep'"));
                                }
                                if let Value::Str(sep) = &args[0] {
                                    if let Some(pos) = s_clone.rfind(sep.as_str()) {
                                        let before = s_clone[..pos].to_string();
                                        let after = s_clone[pos + sep.len()..].to_string();
                                        Value::Tuple(vec![Value::Str(before), Value::Str(sep.clone()), Value::Str(after)])
                                    } else {
                                        Value::Tuple(vec![Value::Str(String::new()), Value::Str(String::new()), Value::Str(s_clone)])
                                    }
                                } else {
                                    return Err(anyhow!("rpartition() argument must be a string"));
                                }
                            }
                            "expandtabs" => {
                                // Expand tabs to spaces (default tabsize=8)
                                let tabsize = if args.is_empty() {
                                    8
                                } else if let Value::Int(size) = args[0] {
                                    size as usize
                                } else {
                                    return Err(anyhow!("expandtabs() argument must be an integer"));
                                };
                                let expanded = s_clone.replace('\t', &" ".repeat(tabsize));
                                Value::Str(expanded)
                            }
                            "startswith" => {
                                // Check if string starts with prefix
                                if args.is_empty() {
                                    return Err(anyhow!("startswith() missing required argument: 'prefix'"));
                                }
                                if let Value::Str(prefix) = &args[0] {
                                    Value::Bool(s_clone.starts_with(prefix))
                                } else {
                                    return Err(anyhow!("startswith() argument must be a string"));
                                }
                            }
                            "endswith" => {
                                // Check if string ends with suffix
                                if args.is_empty() {
                                    return Err(anyhow!("endswith() missing required argument: 'suffix'"));
                                }
                                if let Value::Str(suffix) = &args[0] {
                                    Value::Bool(s_clone.ends_with(suffix))
                                } else {
                                    return Err(anyhow!("endswith() argument must be a string"));
                                }
                            }
                            "split" => {
                                // Split string by separator
                                if args.is_empty() {
                                    // Split by whitespace
                                    let parts: Vec<Value> = s_clone
                                        .split_whitespace()
                                        .map(|s| Value::Str(s.to_string()))
                                        .collect();
                                    Value::List(HPList::from_values(parts))
                                } else if let Value::Str(sep) = &args[0] {
                                    let parts: Vec<Value> = s_clone
                                        .split(sep.as_str())
                                        .map(|s| Value::Str(s.to_string()))
                                        .collect();
                                    Value::List(HPList::from_values(parts))
                                } else {
                                    return Err(anyhow!("split() separator must be a string"));
                                }
                            }
                            "rsplit" => {
                                // Split string from the right
                                if args.is_empty() {
                                    // Split by whitespace
                                    let parts: Vec<Value> = s_clone
                                        .split_whitespace()
                                        .map(|s| Value::Str(s.to_string()))
                                        .collect();
                                    Value::List(HPList::from_values(parts))
                                } else if let Value::Str(sep) = &args[0] {
                                    let mut parts: Vec<Value> = s_clone
                                        .rsplit(sep.as_str())
                                        .map(|s| Value::Str(s.to_string()))
                                        .collect();
                                    parts.reverse();
                                    Value::List(HPList::from_values(parts))
                                } else {
                                    return Err(anyhow!("rsplit() separator must be a string"));
                                }
                            }
                            "join" => {
                                // Join iterable with string as separator
                                if args.is_empty() {
                                    return Err(anyhow!("join() missing required argument: 'iterable'"));
                                }
                                match &args[0] {
                                    Value::List(list) => {
                                        let strings: Result<Vec<String>, _> = list.as_vec()
                                            .iter()
                                            .map(|v| {
                                                if let Value::Str(s) = v {
                                                    Ok(s.clone())
                                                } else {
                                                    Err(anyhow!("join() iterable must contain only strings"))
                                                }
                                            })
                                            .collect();
                                        Value::Str(strings?.join(&s_clone))
                                    }
                                    Value::Tuple(items) => {
                                        let strings: Result<Vec<String>, _> = items
                                            .iter()
                                            .map(|v| {
                                                if let Value::Str(s) = v {
                                                    Ok(s.clone())
                                                } else {
                                                    Err(anyhow!("join() iterable must contain only strings"))
                                                }
                                            })
                                            .collect();
                                        Value::Str(strings?.join(&s_clone))
                                    }
                                    _ => return Err(anyhow!("join() argument must be an iterable")),
                                }
                            }
                            "replace" => {
                                // Replace occurrences of old with new
                                if args.len() < 2 {
                                    return Err(anyhow!("replace() requires 2 arguments: old and new"));
                                }
                                if let (Value::Str(old), Value::Str(new)) = (&args[0], &args[1]) {
                                    Value::Str(s_clone.replace(old, new))
                                } else {
                                    return Err(anyhow!("replace() arguments must be strings"));
                                }
                            }
                            "find" => {
                                // Find first occurrence of substring
                                if args.is_empty() {
                                    return Err(anyhow!("find() missing required argument: 'sub'"));
                                }
                                if let Value::Str(sub) = &args[0] {
                                    match s_clone.find(sub.as_str()) {
                                        Some(pos) => Value::Int(pos as i64),
                                        None => Value::Int(-1),
                                    }
                                } else {
                                    return Err(anyhow!("find() argument must be a string"));
                                }
                            }
                            "rfind" => {
                                // Find last occurrence of substring
                                if args.is_empty() {
                                    return Err(anyhow!("rfind() missing required argument: 'sub'"));
                                }
                                if let Value::Str(sub) = &args[0] {
                                    match s_clone.rfind(sub.as_str()) {
                                        Some(pos) => Value::Int(pos as i64),
                                        None => Value::Int(-1),
                                    }
                                } else {
                                    return Err(anyhow!("rfind() argument must be a string"));
                                }
                            }
                            "index" => {
                                // Find first occurrence of substring (raises error if not found)
                                if args.is_empty() {
                                    return Err(anyhow!("index() missing required argument: 'sub'"));
                                }
                                if let Value::Str(sub) = &args[0] {
                                    match s_clone.find(sub.as_str()) {
                                        Some(pos) => Value::Int(pos as i64),
                                        None => return Err(anyhow!("substring not found")),
                                    }
                                } else {
                                    return Err(anyhow!("index() argument must be a string"));
                                }
                            }
                            "rindex" => {
                                // Find last occurrence of substring (raises error if not found)
                                if args.is_empty() {
                                    return Err(anyhow!("rindex() missing required argument: 'sub'"));
                                }
                                if let Value::Str(sub) = &args[0] {
                                    match s_clone.rfind(sub.as_str()) {
                                        Some(pos) => Value::Int(pos as i64),
                                        None => return Err(anyhow!("substring not found")),
                                    }
                                } else {
                                    return Err(anyhow!("rindex() argument must be a string"));
                                }
                            }
                            "count" => {
                                // Count occurrences of substring
                                if args.is_empty() {
                                    return Err(anyhow!("count() missing required argument: 'sub'"));
                                }
                                if let Value::Str(sub) = &args[0] {
                                    let count = s_clone.matches(sub.as_str()).count();
                                    Value::Int(count as i64)
                                } else {
                                    return Err(anyhow!("count() argument must be a string"));
                                }
                            }
                            _ => {
                                return Err(anyhow!("String has no method '{}'", method_name));
                            }
                        }
                    }
                    Value::Bytes(b) => {
                        // Bytes methods
                        let b_clone = b.clone();
                        match method_name.as_str() {
                            "decode" => {
                                // decode() converts bytes to string - for simplicity, use UTF-8 encoding
                                let encoding = if args.is_empty() {
                                    "utf-8"
                                } else if let Value::Str(enc) = &args[0] {
                                    enc.as_str()
                                } else {
                                    "utf-8"
                                };
                                if encoding != "utf-8" && encoding != "utf8" {
                                    return Err(anyhow!("Encoding '{}' not supported, only UTF-8 is supported", encoding));
                                }
                                match String::from_utf8(b_clone) {
                                    Ok(s) => Value::Str(s),
                                    Err(e) => return Err(anyhow!("Failed to decode bytes: {}", e)),
                                }
                            }
                            _ => {
                                return Err(anyhow!("Bytes has no method '{}'", method_name));
                            }
                        }
                    }
                    Value::Dict(_) => {
                        // Handle dict methods by calling them directly and storing the result immediately
                        // We need to bypass the None-preservation logic because dict.get() can return None as a valid result
                        if let Some(method) = object_value.get_method(&method_name) {
                            // Create arguments: method_name, self, then the actual args
                            let mut method_args = vec![Value::Str(method_name.clone()), object_value.clone()];
                            method_args.extend(args);

                            // Call the builtin function directly
                            let result = match method {
                                Value::BuiltinFunction(_, func) => func(method_args)?,
                                _ => {
                                    // Fallback to call_function_fast for non-builtin methods
                                    self.call_function_fast(method, method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?
                                }
                            };

                            // Store the result directly in object_reg and return early to bypass None-preservation logic
                            self.frames[frame_idx].registers[object_reg] = RcValue::new(result);
                            return Ok(None);
                        } else {
                            return Err(anyhow!("'dict' object has no attribute '{}'", method_name));
                        }
                    }
                    _ => {
                        // For other builtin types that don't have direct VM support yet
                        return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                    }
                };

                // Store the result back in the object register (this is where the VM expects it)
                // IMPORTANT: If result_value is None and the object may have been modified by the method,
                // preserve the current object_reg value instead of overwriting with None
                if matches!(result_value, Value::None) {
                    // Method returned None - the object_reg may have been updated by StoreAttr during method execution
                    // Don't overwrite it with None; keep the potentially modified object

                    // CRITICAL FIX: After a super() method call, sync locals[0] with object_reg
                    // This ensures that subsequent code in this method sees the modifications
                    // made by the parent method
                    if !self.frames[frame_idx].locals.is_empty() {
                        // Check if object_reg contains an Object (instance)
                        if matches!(self.frames[frame_idx].registers[object_reg].value, Value::Object { .. }) {
                            self.frames[frame_idx].locals[0] = self.frames[frame_idx].registers[object_reg].clone();
                        } else {
                        }
                    }
                } else {
                    // Method returned an actual value - store it
                    self.frames[frame_idx].registers[object_reg] = RcValue::new(result_value);
                }
                Ok(None)
            }
            OpCode::LoadMethod => {
                // Load method with caching
                // arg1 = object register, arg2 = method name index, arg3 = result register
                let object_reg = arg1 as usize;
                let method_name_idx = arg2 as usize;
                let result_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadMethod: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }

                if method_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadMethod: method name index {} out of bounds (len: {})", method_name_idx, self.frames[frame_idx].code.names.len()));
                }

                // Get the object and method name
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let method_name = self.frames[frame_idx].code.names[method_name_idx].clone();

                // Try to get the method from the object
                let method_value = match &object_value {
                    Value::Object { class_methods, mro, .. } => {
                        // First check class methods
                        if let Some(method) = class_methods.get(&method_name) {
                            // Create a BoundMethod to bind self to the method
                            Value::BoundMethod {
                                object: Box::new(object_value.clone()),
                                method_name: method_name.clone(),
                            }
                        } else {
                            // Then check MRO for inherited methods
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&method_name, &globals_values) {
                                // If we found a method in the MRO, create a BoundMethod
                                Value::BoundMethod {
                                    object: Box::new(object_value.clone()),
                                    method_name: method_name.clone(),
                                }
                            } else {
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                            }
                        }
                    },
                    Value::Class { methods, mro, .. } => {
                        // Check class methods
                        if let Some(method) = methods.get(&method_name) {
                            method.clone()
                        } else {
                            // Then check MRO for inherited methods
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&method_name, &globals_values) {
                                method.clone()
                            } else {
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                            }
                        }
                    },
                    Value::Module(_, namespace) => {
                        // Check module attributes
                        if let Some(value) = namespace.get(&method_name) {
                            value.clone()
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                        }
                    },
                    _ => {
                        // For other objects, try to get method
                        if let Some(method) = object_value.get_method(&method_name) {
                            method
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                        }
                    }
                };

                // Store the method in the result register
                self.frames[frame_idx].registers[result_reg] = RcValue::new(method_value);
                Ok(None)
            }
            OpCode::LoadMethodCached => {
                // Load method from GLOBAL cache for maximum performance
                // arg1 = object register, arg2 = method name index, arg3 = result register
                let object_reg = arg1 as usize;
                let method_name_idx = arg2 as usize;
                let result_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadMethodCached: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }

                if method_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadMethodCached: method name index {} out of bounds (len: {})", method_name_idx, self.frames[frame_idx].code.names.len()));
                }

                // Get the object and method name
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let method_name = self.frames[frame_idx].code.names[method_name_idx].clone();

                // Try to lookup method in GLOBAL cache first (much faster than per-frame cache)
                let class_name = object_value.type_name();
                let cache_key = (class_name.to_string(), method_name.clone());
                if let Some(cache_entry) = self.global_method_cache.get(&cache_key) {
                    if cache_entry.version == self.method_cache_version {
                        if let Some(method) = &cache_entry.method {
                            // CACHE HIT: Use cached method without any lookups
                            self.frames[frame_idx].registers[result_reg] = RcValue::new(method.clone());
                            return Ok(None);
                        }
                    }
                }

                // Not in cache, load method normally
                let method_value = match &object_value {
                    Value::Object { class_methods, mro, .. } => {
                        // First check class methods
                        if let Some(method) = class_methods.get(&method_name) {
                            // Create a BoundMethod to bind self to the method
                            Value::BoundMethod {
                                object: Box::new(object_value.clone()),
                                method_name: method_name.clone(),
                            }
                        } else {
                            // Then check MRO for inherited methods
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&method_name, &globals_values) {
                                // If we found a method in the MRO, create a BoundMethod
                                Value::BoundMethod {
                                    object: Box::new(object_value.clone()),
                                    method_name: method_name.clone(),
                                }
                            } else {
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                            }
                        }
                    },
                    Value::Class { methods, mro, .. } => {
                        // Check class methods
                        if let Some(method) = methods.get(&method_name) {
                            method.clone()
                        } else {
                            // Then check MRO for inherited methods
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&method_name, &globals_values) {
                                method.clone()
                            } else {
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                            }
                        }
                    },
                    Value::Module(_, namespace) => {
                        // Check module attributes
                        if let Some(value) = namespace.get(&method_name) {
                            value.clone()
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                        }
                    },
                    _ => {
                        // For other objects, try to get method
                        if let Some(method) = object_value.get_method(&method_name) {
                            method
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                        }
                    }
                };

                // Store method in GLOBAL cache for future use (much better performance)
                let cache_entry = MethodCache {
                    class_name: class_name.to_string(),
                    method_name: method_name.clone(),
                    method: Some(method_value.clone()),
                    version: self.method_cache_version,
                };
                self.global_method_cache.insert(cache_key, cache_entry);

                // Store the method in the result register
                self.frames[frame_idx].registers[result_reg] = RcValue::new(method_value);
                Ok(None)
            }
            OpCode::CallMethodCached => {
                // Call method from cache
                // arg1 = object register, arg2 = argument count, arg3 = result register
                let object_reg = arg1 as usize;
                let arg_count = arg2 as usize;
                let result_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallMethodCached: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }

                // Collect arguments from registers
                let mut args = Vec::new();
                for i in 0..arg_count {
                    // Arguments are stored in consecutive registers after the object register
                    let arg_reg = object_reg + 1 + i;
                    if arg_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("CallMethodCached: argument register index {} out of bounds (len: {})", arg_reg, self.frames[frame_idx].registers.len()));
                    }
                    args.push(self.frames[frame_idx].registers[arg_reg].value.clone());
                }

                // Get the method value (should be a BoundMethod or regular method)
                let method_value = self.frames[frame_idx].registers[object_reg].value.clone();

                // Call the method
                let result = match method_value {
                    Value::BoundMethod { object, method_name } => {
                        // Handle bound method calls
                        match object.as_ref() {
                            Value::Object { class_methods, .. } => {
                                if let Some(method) = class_methods.get(&method_name) {
                                    // Create arguments with self as the first argument
                                    let mut method_args = vec![*object.clone()];
                                    method_args.extend(args);

                                    // Call the method through the VM
                                    self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(result_reg as u32))?
                                } else {
                                    return Err(anyhow!("Method '{}' not found in class methods", method_name));
                                }
                            },
                            Value::Dict(_) | Value::List(_) | Value::Str(_) | Value::Set(_) | Value::Tuple(_) => {
                                // For builtin types, get the method and call it
                                if let Some(method) = object.as_ref().get_method(&method_name) {
                                    // Create arguments: method_name, self, then the actual args
                                    let mut method_args = vec![Value::Str(method_name.clone()), *object.clone()];
                                    method_args.extend(args);

                                    // Call the method
                                    self.call_function_fast(method, method_args, HashMap::new(), Some(frame_idx), Some(result_reg as u32))?
                                } else {
                                    return Err(anyhow!("Method '{}' not found for type '{}'", method_name, object.as_ref().type_name()));
                                }
                            },
                            _ => return Err(anyhow!("Bound method called on non-object type '{}'", object.as_ref().type_name())),
                        }
                    },
                    _ => {
                        // For regular methods, call directly
                        self.call_function_fast(method_value, args, HashMap::new(), Some(frame_idx), Some(result_reg as u32))?
                    }
                };

                // Store the result
                if !matches!(result, Value::None) {
                    self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                }
                Ok(None)
            }
            OpCode::BinaryMulRR => {
                // Register-Register multiplication with unsafe fast path
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;

                #[cfg(debug_assertions)]
                {
                    if left_reg >= self.frames[frame_idx].registers.len() ||
                       right_reg >= self.frames[frame_idx].registers.len() ||
                       result_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BinaryMulRR: register index out of bounds"));
                    }
                }

                let (left, right) = unsafe {
                    let regs = &self.frames[frame_idx].registers;
                    (regs.get_unchecked(left_reg), regs.get_unchecked(right_reg))
                };

                // Fast path for common operations
                let result = match (&left.value, &right.value) {
                    (Value::Int(a), Value::Int(b)) => Value::Int((*a).wrapping_mul(*b)),
                    (Value::Float(a), Value::Float(b)) => Value::Float(a * b),
                    _ => {
                        // For less common cases, use the general implementation
                        self.mul_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryMulRR: {}", e))?
                    }
                };

                unsafe {
                    *self.frames[frame_idx].registers.get_unchecked_mut(result_reg) = RcValue::new(result);
                }
                Ok(None)
            }
            OpCode::WrapKwargs => {
                // Wrap a dictionary in a KwargsMarker
                let dict_reg = arg1 as usize;
                let result_reg = arg2;
                
                if dict_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("WrapKwargs: dictionary register index {} out of bounds (len: {})", dict_reg, self.frames[frame_idx].registers.len()));
                }
                
                let dict_value = &self.frames[frame_idx].registers[dict_reg];
                match &dict_value.value {
                    Value::Dict(dict) => {
                        let kwargs_marker = Value::KwargsMarker(dict.borrow().clone());
                        self.frames[frame_idx].set_register(result_reg, RcValue::new(kwargs_marker));
                        Ok(None)
                    }
                    _ => Err(anyhow!("WrapKwargs: expected dictionary, got {}", dict_value.value.type_name())),
                }
            }
            OpCode::BreakLoop => {
                // Break out of the innermost loop
                // Find the innermost loop block in the block stack
                if let Some(loop_block_idx) = self.frames[frame_idx].block_stack.iter().rposition(|b| matches!(b.block_type, BlockType::Loop)) {
                    // Get the loop block
                    let loop_block = &self.frames[frame_idx].block_stack[loop_block_idx];
                    // Jump to the end of the loop (handler contains the target PC)
                    self.frames[frame_idx].pc = loop_block.handler;
                    // Pop all blocks up to and including the loop block
                    self.frames[frame_idx].block_stack.truncate(loop_block_idx);
                    Ok(None)
                } else {
                    Err(anyhow!("'break' outside loop"))
                }
            }
            OpCode::ContinueLoop => {
                // Continue to the next iteration of the innermost loop
                // Find the innermost loop block in the block stack
                if let Some(loop_block_idx) = self.frames[frame_idx].block_stack.iter().rposition(|b| matches!(b.block_type, BlockType::Loop)) {
                    // Get the loop block
                    let loop_block = &self.frames[frame_idx].block_stack[loop_block_idx];
                    // Jump to the continue target of the loop (level contains the target PC)
                    self.frames[frame_idx].pc = loop_block.level;
                    // Pop all blocks up to and including the loop block
                    self.frames[frame_idx].block_stack.truncate(loop_block_idx);
                    Ok(None)
                } else {
                    Err(anyhow!("'continue' not properly in loop"))
                }
            }
            OpCode::SetupLoop => {
                // Setup a loop block
                let handler_pc = arg1 as usize;  // End of loop PC
                let continue_pc = arg2 as usize; // Continue target PC (start of loop body)
                
                // Push a loop block onto the block stack
                let loop_block = Block {
                    block_type: BlockType::Loop,
                    handler: handler_pc,
                    level: continue_pc,
                };
                self.frames[frame_idx].block_stack.push(loop_block);
                Ok(None)
            }
            OpCode::SetupExcept => {
                // Setup an exception handler block
                let handler_pc = arg1 as usize;
                let stack_level = arg2 as usize;
                
                // Push an exception handler block onto the block stack
                let except_block = Block {
                    block_type: BlockType::Except,
                    handler: handler_pc,
                    level: stack_level,
                };
                self.frames[frame_idx].block_stack.push(except_block);
                Ok(None)
            }
            OpCode::SetupFinally => {
                // Setup a finally block
                let handler_pc = arg1 as usize;
                let stack_level = arg2 as usize;
                
                // Push a finally block onto the block stack
                let finally_block = crate::bytecode::memory::Block {
                    block_type: BlockType::Finally,
                    handler: handler_pc,
                    level: stack_level,
                };
                self.frames[frame_idx].block_stack.push(finally_block);
                Ok(None)
            }
            OpCode::EndFinally => {
                // End of finally block - just continue execution
                Ok(None)
            }
            OpCode::PopBlock => {
                // Pop a block from the block stack
                // This is used to remove exception handler blocks when try block completes successfully
                if self.frames[frame_idx].block_stack.is_empty() {
                    return Err(anyhow!("PopBlock: block stack is empty"));
                }
                self.frames[frame_idx].block_stack.pop();
                Ok(None)
            }
            OpCode::StoreException => {
                // Store exception value (from top of stack) in a variable
                // arg1: variable index
                // arg2: 0 for local/fast storage, 1 for global storage
                let var_idx = arg1 as usize;
                let storage_type = arg2;
                
                // Get the exception value from the top of the stack
                if self.frames[frame_idx].registers.is_empty() {
                    return Err(anyhow!("StoreException: no value on stack"));
                }
                
                let exception_value = self.frames[frame_idx].registers.pop().unwrap();
                
                match storage_type {
                    0 => {
                        // Store in fast local variable
                        if var_idx >= self.frames[frame_idx].locals.len() {
                            // Extend locals if needed
                            self.frames[frame_idx].locals.resize(var_idx + 1, RcValue::new(Value::None));
                        }
                        self.frames[frame_idx].locals[var_idx] = exception_value;
                    }
                    1 => {
                        // Store in global namespace
                        if var_idx >= self.frames[frame_idx].code.varnames.len() {
                            return Err(anyhow!("StoreException: varname index {} out of bounds", var_idx));
                        }
                        let var_name = self.frames[frame_idx].code.varnames[var_idx].clone();
                        self.frames[frame_idx].globals.borrow_mut().insert(var_name, exception_value);
                    }
                    _ => return Err(anyhow!("StoreException: invalid storage type {}", storage_type)),
                }
                
                Ok(None)
            }
            OpCode::Raise => {
                // Raise an exception
                let exception_reg = arg1 as usize;
                
                if exception_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("Raise: exception register index {} out of bounds (len: {})", exception_reg, self.frames[frame_idx].registers.len()));
                }
                
                // Clone the exception value to avoid borrowing issues
                let exception_value = self.frames[frame_idx].registers[exception_reg].value.clone();
                
                // Add traceback information to the exception
                let mut enhanced_exception = exception_value.clone();
                if let Value::Exception { class_name, message, traceback: _ } = &exception_value {
                    // Create traceback information
                    let mut traceback_info = format!("Traceback (most recent call last):\n");
                    traceback_info.push_str(&format!("  File \"{}\", line {}, in {}\n", 
                        self.frames[frame_idx].code.filename, 
                        self.frames[frame_idx].line_number,
                        self.frames[frame_idx].code.name));
                    traceback_info.push_str(&format!("{}: {}\n", class_name, message));
                    
                    enhanced_exception = Value::new_exception(
                        class_name.clone(),
                        message.clone(),
                        Some(traceback_info)
                    );
                }
                
                // Search for exception handlers in the block stack
                // Find the innermost exception handler
                let except_block_idx_opt = self.frames[frame_idx].block_stack.iter().rposition(|b| matches!(b.block_type, BlockType::Except));
                
                if let Some(except_block_idx) = except_block_idx_opt {
                    // Get the exception handler block
                    let handler_pc = self.frames[frame_idx].block_stack[except_block_idx].handler;
                    // Remove the exception handler block from the stack (it's been used)
                    self.frames[frame_idx].block_stack.remove(except_block_idx);
                    // Jump to the exception handler
                    self.frames[frame_idx].pc = handler_pc;
                    // Push the exception value onto the stack for the handler to access
                    self.frames[frame_idx].registers.push(RcValue::new(enhanced_exception));
                    Ok(None) // Continue execution, don't return an error
                } else {
                    // No exception handler found, print the exception with traceback and stop execution
                    if let Some(traceback) = enhanced_exception.get_traceback() {
                        eprintln!("{}", traceback);
                    } else {
                        eprintln!("{}", enhanced_exception);
                    }
                    Err(anyhow!("Unhandled exception: {}", enhanced_exception))
                }
            }
            OpCode::GetExceptionValue => {
                // Pop exception value from stack and store in register
                // arg1 = destination register
                let dest_reg = arg1 as usize;

                if self.frames[frame_idx].registers.is_empty() {
                    return Err(anyhow!("GetExceptionValue: no exception on stack"));
                }

                // Pop the exception value
                let exception_value = self.frames[frame_idx].registers.pop().unwrap();

                // Ensure we have enough registers
                while self.frames[frame_idx].registers.len() <= dest_reg {
                    self.frames[frame_idx].registers.push(RcValue::new(Value::None));
                }

                // Store in destination register
                self.frames[frame_idx].registers[dest_reg] = exception_value;
                Ok(None)
            }
            OpCode::MatchExceptionType => {
                // Check if exception matches a specific type
                // arg1 = exception register
                // arg2 = type name string index
                // arg3 = result register (will be set to Bool)
                let exc_reg = arg1 as usize;
                let type_name_idx = arg2 as usize;
                let result_reg = arg3 as usize;

                if exc_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MatchExceptionType: exception register {} out of bounds", exc_reg));
                }

                if type_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("MatchExceptionType: type name index {} out of bounds", type_name_idx));
                }

                let exception_value = &self.frames[frame_idx].registers[exc_reg].value;
                let expected_type_name = &self.frames[frame_idx].code.names[type_name_idx];

                // Check if exception matches the expected type
                let matches = if let Value::Exception { class_name, .. } = exception_value {
                    class_name == expected_type_name
                } else {
                    false
                };

                // Ensure result register exists
                while self.frames[frame_idx].registers.len() <= result_reg {
                    self.frames[frame_idx].registers.push(RcValue::new(Value::None));
                }

                self.frames[frame_idx].registers[result_reg] = RcValue::new(Value::Bool(matches));
                Ok(None)
            }
            OpCode::Assert => {
                // Assert statement
                let condition_reg = arg1 as usize;
                let message_reg = arg2 as usize;
                
                if condition_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("Assert: condition register index {} out of bounds (len: {})", condition_reg, self.frames[frame_idx].registers.len()));
                }
                
                let condition_value = &self.frames[frame_idx].registers[condition_reg];
                if !condition_value.is_truthy() {
                    // Assertion failed
                    if message_reg < self.frames[frame_idx].registers.len() {
                        let message_value = &self.frames[frame_idx].registers[message_reg];
                        return Err(anyhow!("AssertionError: {}", message_value.value));
                    } else {
                        return Err(anyhow!("AssertionError"));
                    }
                }
                // Assertion passed, continue execution
                Ok(None)
            }
            OpCode::Match => {
                // Pattern matching - match value against pattern
                let value_reg = arg1 as usize;
                let pattern_reg = arg2 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("Match: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                // If pattern_reg is 0, this is a wildcard or variable pattern that always matches
                if pattern_reg == 0 {
                    // Always match - continue execution
                    Ok(None)
                } else {
                    // Compare value with pattern
                    if pattern_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("Match: pattern register index {} out of bounds (len: {})", pattern_reg, self.frames[frame_idx].registers.len()));
                    }
                    
                    let value = &self.frames[frame_idx].registers[value_reg];
                    let pattern = &self.frames[frame_idx].registers[pattern_reg];
                    
                    // Simple equality check for now
                    if value.value == pattern.value {
                        // Match successful - continue execution
                        Ok(None)
                    } else {
                        // Match failed - jump to next case
                        // In a full implementation, we would jump to the next case
                        // For now, we'll just continue to the next instruction
                        Ok(None)
                    }
                }
            }
            OpCode::MatchKeys => {
                // Match keys in mapping pattern
                let mapping_reg = arg1 as usize;
                let key_count = arg2 as usize;
                
                if mapping_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MatchKeys: mapping register index {} out of bounds (len: {})", mapping_reg, self.frames[frame_idx].registers.len()));
                }
                
                let mapping_value = &self.frames[frame_idx].registers[mapping_reg];
                
                // Check if the value is a dictionary
                match &mapping_value.value {
                    Value::Dict(dict) => {
                        // For now, we'll just check that we have the right number of keys
                        // In a full implementation, we would check specific keys
                        if dict.borrow().len() >= key_count {
                            Ok(None) // Match successful
                        } else {
                            // Jump to next case
                            // In a full implementation, we would jump to the next case
                            Ok(None)
                        }
                    }
                    _ => {
                        // Not a dictionary - jump to next case
                        // In a full implementation, we would jump to the next case
                        Ok(None)
                    }
                }
            }
            OpCode::MatchClass => {
                // Match class pattern
                let object_reg = arg1 as usize;
                let pattern_count = arg2 as usize;
                
                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MatchClass: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }
                
                let object_value = &self.frames[frame_idx].registers[object_reg];
                
                // For now, we'll just check that the object is an object type
                // In a full implementation, we would check the class name and patterns
                match &object_value.value {
                    Value::Object { .. } => {
                        // Object type - continue with pattern matching
                        Ok(None)
                    }
                    _ => {
                        // Not an object - jump to next case
                        // In a full implementation, we would jump to the next case
                        Ok(None)
                    }
                }
            }
            OpCode::MatchSequence => {
                // Match sequence pattern
                let sequence_reg = arg1 as usize;
                let item_count = arg2 as usize;
                
                if sequence_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MatchSequence: sequence register index {} out of bounds (len: {})", sequence_reg, self.frames[frame_idx].registers.len()));
                }
                
                let sequence_value = &self.frames[frame_idx].registers[sequence_reg];
                
                // Check if the value is a sequence (list, tuple, etc.)
                match &sequence_value.value {
                    Value::List(items) => {
                        // Check if we have the right number of items
                        if items.len() >= item_count {
                            Ok(None) // Match successful
                        } else {
                            // Jump to next case
                            // In a full implementation, we would jump to the next case
                            Ok(None)
                        }
                    }
                    Value::Tuple(items) => {
                        // Check if we have the right number of items
                        if items.len() >= item_count {
                            Ok(None) // Match successful
                        } else {
                            // Jump to next case
                            // In a full implementation, we would jump to the next case
                            Ok(None)
                        }
                    }
                    _ => {
                        // Not a sequence - jump to next case
                        // In a full implementation, we would jump to the next case
                        Ok(None)
                    }
                }
            }
            OpCode::MakeFunction => {
                // Create a function object from a code object
                let code_reg = arg1 as usize;
                let result_reg = arg2 as u32;
                
                if code_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MakeFunction: code register index {} out of bounds (len: {})", code_reg, self.frames[frame_idx].registers.len()));
                }
                
                let code_value = &self.frames[frame_idx].registers[code_reg];
                
                match &code_value.value {
                    Value::Code(code_obj) => {
                        // Create a closure with the code object
                        let closure = Value::Closure {
                            name: code_obj.name.clone(),
                            params: code_obj.params.clone(),
                            body: vec![], // Empty body since it's in the compiled code
                            captured_scope: HashMap::new(), // No captured scope for now
                            docstring: None, // No docstring for now
                            compiled_code: Some(Box::new(*code_obj.clone())),
                            module_globals: None,
                        };
                        
                        self.frames[frame_idx].set_register(result_reg, RcValue::new(closure));
                        Ok(None)
                    }
                    _ => Err(anyhow!("MakeFunction: expected code object, got {}", code_value.value.type_name())),
                }
            }
            OpCode::MatchMapping => {
                // Match mapping pattern
                let mapping_reg = arg1 as usize;
                let pair_count = arg2 as usize;
                
                if mapping_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MatchMapping: mapping register index {} out of bounds (len: {})", mapping_reg, self.frames[frame_idx].registers.len()));
                }
                
                let mapping_value = &self.frames[frame_idx].registers[mapping_reg];
                
                // Check if the value is a dictionary
                match &mapping_value.value {
                    Value::Dict(dict) => {
                        // Check if we have at least the required number of key-value pairs
                        if dict.borrow().len() >= pair_count {
                            Ok(None) // Match successful
                        } else {
                            // Jump to next case
                            // In a full implementation, we would jump to the next case
                            Ok(None)
                        }
                    }
                    _ => {
                        // Not a dictionary - jump to next case
                        // In a full implementation, we would jump to the next case
                        Ok(None)
                    }
                }
            }
            OpCode::MatchOr => {
                // Match or pattern
                let value_reg = arg1 as usize;
                let _pattern_count = arg2 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("MatchOr: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                // For now, we'll just continue execution
                // In a full implementation, we would try each pattern in turn
                Ok(None)
            }
            OpCode::ListAppend => {
                // Append an item to a list
                let list_reg = arg1 as usize;
                let item_reg = arg2 as usize;
                
                // Check bounds first to avoid borrowing conflicts
                let frames_len = self.frames[frame_idx].registers.len();
                if list_reg >= frames_len || item_reg >= frames_len {
                    return Err(anyhow!("ListAppend: register index out of bounds"));
                }
                
                // Clone values to avoid borrowing conflicts
                let list_value = self.frames[frame_idx].registers[list_reg].value.clone();
                let item_value = self.frames[frame_idx].registers[item_reg].value.clone();
                
                match list_value {
                    Value::List(mut list) => {
                        list.push(item_value);
                        self.frames[frame_idx].registers[list_reg] = RcValue::new(Value::List(list));
                        Ok(None)
                    }
                    _ => Err(anyhow!("ListAppend: expected list, got {}", list_value.type_name())),
                }
            }
            OpCode::SetAdd => {
                // Add an item to a set
                let set_reg = arg1 as usize;
                let item_reg = arg2 as usize;
                
                // Check bounds first to avoid borrowing conflicts
                let frames_len = self.frames[frame_idx].registers.len();
                if set_reg >= frames_len || item_reg >= frames_len {
                    return Err(anyhow!("SetAdd: register index out of bounds"));
                }
                
                // Clone values to avoid borrowing conflicts
                let set_value = self.frames[frame_idx].registers[set_reg].value.clone();
                let item_value = self.frames[frame_idx].registers[item_reg].value.clone();
                
                match set_value {
                    Value::Set(mut items) => {
                        items.push(item_value);
                        self.frames[frame_idx].registers[set_reg] = RcValue::new(Value::Set(items));
                        Ok(None)
                    }
                    _ => Err(anyhow!("SetAdd: expected set, got {}", set_value.type_name())),
                }
            }
            OpCode::MapAdd => {
                // Add a key-value pair to a dictionary
                let dict_reg = arg1 as usize;
                let key_reg = arg2 as usize;
                let value_reg = arg3 as usize;
                
                // Check bounds first to avoid borrowing conflicts
                let frames_len = self.frames[frame_idx].registers.len();
                if dict_reg >= frames_len || key_reg >= frames_len || value_reg >= frames_len {
                    return Err(anyhow!("MapAdd: register index out of bounds"));
                }
                
                // Clone values to avoid borrowing conflicts
                let dict_value = self.frames[frame_idx].registers[dict_reg].value.clone();
                let key_value = self.frames[frame_idx].registers[key_reg].value.clone();
                let value_value = self.frames[frame_idx].registers[value_reg].value.clone();
                
                match dict_value {
                    Value::Dict(dict) => {
                        // Keys must be strings
                        let key_str = match &key_value {
                            Value::Str(s) => s.clone(),
                            _ => format!("{}", key_value),
                        };
                        
                        dict.borrow_mut().insert(key_str, value_value);
                        Ok(None)
                    }
                    _ => Err(anyhow!("MapAdd: expected dict, got {}", dict_value.type_name())),
                }
            }
            OpCode::YieldValue => {
                // Yield a value from a generator
                let value_reg = arg1 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("YieldValue: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                let yield_value = self.frames[frame_idx].registers[value_reg].value.clone();
                // For generator functions, we need to suspend execution and return to the caller
                // The caller will receive the yielded value and can resume the generator later
                // Check if this frame has a return register (meaning it's a generator)
                if let Some((caller_frame_idx, result_reg)) = self.frames[frame_idx].return_register {
                    // This is a generator frame, suspend it and return the yielded value to the caller
                    if caller_frame_idx < self.frames.len() {
                        // Store the yielded value in the caller's result register
                        self.frames[caller_frame_idx].set_register(result_reg, RcValue::new(yield_value.clone()));
                    }
                    
                    // Pop the current frame and return None to indicate we're suspending, not completing
                    // The generator's frame state has already been saved in the caller's generator object
                    self.frames.pop();
                    Ok(None)
                } else {
                    // Regular return from function
                    Ok(Some(yield_value))
                }
            }
            OpCode::YieldFrom => {
                // Yield from an iterable
                let iterable_reg = arg1 as usize;
                
                if iterable_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("YieldFrom: iterable register index {} out of bounds (len: {})", iterable_reg, self.frames[frame_idx].registers.len()));
                }
                
                let iterable_value = self.frames[frame_idx].registers[iterable_reg].value.clone();
                // For yield from, we would normally iterate through the iterable and yield each value
                // For now, we'll just return the iterable
                Ok(Some(iterable_value))
            }
            OpCode::Await => {
                // Await a coroutine/future
                let value_reg = arg1 as usize;
                let result_reg = arg2 as usize;
                
                if value_reg >= self.frames[frame_idx].registers.len() || result_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("Await: register index out of bounds"));
                }
                
                // For now, we'll just return the value as-is (no actual async support yet)
                // In a full implementation, we would suspend execution until the future completes
                let value = self.frames[frame_idx].registers[value_reg].value.clone();
                self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(value));
                Ok(None)
            }
            OpCode::LoadAttr => {
                // Load attribute from object (obj.attr) with FAST PATH caching
                let object_reg = arg1 as usize;
                let attr_name_idx = arg2 as usize;
                let result_reg = arg3 as usize;

                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadAttr: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }

                if attr_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadAttr: attribute name index {} out of bounds (len: {})", attr_name_idx, self.frames[frame_idx].code.names.len()));
                }

                // Clone values to avoid borrowing issues
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let attr_name = self.frames[frame_idx].code.names[attr_name_idx].clone();

                // OPTIMIZATION: Fast path for common Object attribute access
                // This bypasses the expensive match statements below for hot paths
                if let Value::Object { fields, .. } = &object_value {
                    if let Some(attr_value) = fields.get(&attr_name) {
                        // FAST PATH: Direct attribute access without cache lookup
                        self.frames[frame_idx].registers[result_reg] = RcValue::new(attr_value.clone());
                        return Ok(None);
                    }
                }

                // Try to get the attribute from the object
                
                let result = match &object_value {
                    Value::Super(current_class, parent_class, instance, parent_methods) => {
                        // Handle super() object - delegate to parent class
                        
                        if let Some(instance_value) = instance {
                            // For super() objects, we need to look up the method in the parent class hierarchy
                            
                            // First, try to find the current class in globals to get its MRO
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            
                            // Look for the current class in globals
                            if let Some(class_value) = globals_values.get(current_class) {
                                if let Value::Class { name, mro, .. } = class_value {
                                    
                                    // Use MRO to find the method in parent classes
                                    if let Some(method) = mro.find_method_in_mro(&attr_name, &globals_values) {
                                        // Found the method, create a BoundMethod
                                        let bound_method = Value::BoundMethod {
                                            object: instance_value.clone(),
                                            method_name: attr_name.clone(),
                                        };
                                        self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                        return Ok(None);
                                    }
                                }
                            }
                            
                            // If not found through MRO, check parent_methods as fallback
                            if let Some(methods) = parent_methods {
                                if let Some(method) = methods.get(&attr_name) {
                                    // Found the method in parent methods, create a BoundMethod
                                    let bound_method = Value::BoundMethod {
                                        object: instance_value.clone(),
                                        method_name: attr_name.clone(),
                                    };
                                    self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                    return Ok(None);
                                }
                            }
                            
                            // If still not found, check if this is a special case
                            // For methods that might not be in the class methods, try to find them in the instance
                            if let Value::Object { class_methods, .. } = instance_value.as_ref() {
                                if let Some(method) = class_methods.get(&attr_name) {
                                    // Found the method in the instance's class methods, create a BoundMethod
                                    let bound_method = Value::BoundMethod {
                                        object: instance_value.clone(),
                                        method_name: attr_name.clone(),
                                    };
                                    self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                    return Ok(None);
                                }
                            }
                            
                            // If still not found, create a BoundMethod but it will fail at call time
                            // This maintains compatibility with the existing approach
                            let bound_method = Value::BoundMethod {
                                object: instance_value.clone(),
                                method_name: attr_name.clone(),
                            };
                            self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                            return Ok(None);
                        } else {
                            return Err(anyhow!("super(): unbound super object has no attribute '{}'", attr_name));
                        }
                    },
                    Value::Object { fields, class_methods, mro, .. } => {
                        // First check fields (instance attributes)
                        let result = if let Some(value) = fields.as_ref().get(&attr_name) {
                            // Check if this is a descriptor (has __get__ method)
                            if let Some(getter) = value.get_method("__get__") {
                                // Call the descriptor's __get__ method
                                // __get__(self, obj, owner)
                                let args = vec![value.clone(), object_value.clone(), Value::None]; // Simplified - in full implementation we'd pass the owner
                                match getter {
                                    Value::BuiltinFunction(_, func) => func(args)?,
                                    Value::NativeFunction(func) => func(args)?,
                                    _ => value.clone(), // Fallback to returning the descriptor itself
                                }
                            } else {
                                value.clone()
                            }
                        }
                        // Then check class methods - return as BoundMethod so self is bound
                        else if let Some(method) = class_methods.get(&attr_name) {
                            // Check if this is a descriptor (has __get__ method)
                            // Descriptors take precedence over everything
                            if let Some(getter) = method.get_method("__get__") {
                                // Call the descriptor's __get__ method
                                // __get__(self, obj, owner)
                                let args = vec![method.clone(), object_value.clone(), Value::None];
                                match getter {
                                    Value::BuiltinFunction(_, func) => func(args)?,
                                    Value::NativeFunction(func) => func(args)?,
                                    Value::Closure { .. } => {
                                        // For closure, we need to call it through the VM
                                        self.call_function_fast(getter.clone(), args, HashMap::new(), Some(frame_idx), Some(result_reg as u32))?;
                                        return Ok(None);
                                    },
                                    _ => method.clone(), // Fallback
                                }
                            }
                            // Check if this is a property object that needs to be called
                            else if let Value::Object { class_name, fields, .. } = method {
                                if class_name == "property" {
                                    // This is a property, call its getter function if it exists
                                    if let Some(getter) = fields.as_ref().get("fget") {
                                        // Call the getter function with self as argument
                                        let args = vec![object_value.clone()];
                                        match getter {
                                            Value::BuiltinFunction(_, func) => func(args)?,
                                            Value::NativeFunction(func) => func(args)?,
                                            Value::Closure { .. } => {
                                                // For closure, we need to call it through the VM
                                                // Push a new frame for the property getter
                                                self.call_function_fast(getter.clone(), args, HashMap::new(), Some(frame_idx), Some(result_reg as u32))?;
                                                // Return None to let the main VM loop handle the new frame
                                                // The result will be stored in result_reg when the frame completes
                                                return Ok(None);
                                            },
                                            _ => method.clone() // Fallback
                                        }
                                    } else {
                                        method.clone()
                                    }
                                } else {
                                    // Create a BoundMethod to bind self to the method
                                    Value::BoundMethod {
                                        object: Box::new(object_value.clone()),
                                        method_name: attr_name.clone(),
                                    }
                                }
                            } else {
                                // Create a BoundMethod to bind self to the method
                                Value::BoundMethod {
                                    object: Box::new(object_value.clone()),
                                    method_name: attr_name.clone(),
                                }
                            }
                        }
                        // Then check MRO for inherited methods and attributes
                        else {
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&attr_name, &globals_values) {
                                // Check if this is a property object that needs to be called
                                if let Value::Object { class_name, fields, .. } = &method {
                                    if class_name == "property" {
                                        // This is a property, call its getter function if it exists
                                        if let Some(getter) = fields.as_ref().get("fget") {
                                            // Call the getter function with self as argument
                                            let args = vec![object_value.clone()];
                                            match getter {
                                                Value::BuiltinFunction(_, func) => func(args)?,
                                                Value::NativeFunction(func) => func(args)?,
                                                Value::Closure { .. } => {
                                                    // For closure, we need to call it through the VM
                                                    // Push a new frame for the property getter
                                                    self.call_function_fast(getter.clone(), args, HashMap::new(), Some(frame_idx), Some(result_reg as u32))?;
                                                    // Return None to let the main VM loop handle the new frame
                                                    // The result will be stored in result_reg when the frame completes
                                                    return Ok(None);
                                                },
                                                _ => method.clone(), // Fallback
                                            }
                                        } else {
                                            method.clone()
                                        }
                                    } else {
                                        // If we found a method in the MRO, create a BoundMethod
                                        Value::BoundMethod {
                                            object: Box::new(object_value.clone()),
                                            method_name: attr_name.clone(),
                                        }
                                    }
                                } else {
                                    // If we found a method in the MRO, create a BoundMethod
                                    Value::BoundMethod {
                                        object: Box::new(object_value.clone()),
                                        method_name: attr_name.clone(),
                                    }
                                }
                            } else {
                                // Check if any parent class has this as an attribute (not just a method)
                                // This handles the case where a parent class sets an attribute in __init__
                                for class_name in mro.get_linearization() {
                                    if let Some(class_value) = globals_values.get(class_name) {
                                        if let Value::Class { methods, .. } = class_value {
                                            // Check if this class has any instances with this attribute
                                            // This is a simplified approach - in a full implementation we'd need to search instances
                                            // For now, we'll just return an error if not found
                                            continue;
                                        }
                                    }
                                }
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                            }
                        };

                        self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                        return Ok(None);
                    },
                    Value::Class { methods, mro, .. } => {
                        // Check class methods
                        if let Some(method) = methods.get(&attr_name) {
                            method.clone()
                        }
                        // Then check MRO for inherited methods
                        else {
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&attr_name, &globals_values) {
                                method.clone()
                            } else {
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                            }
                        }
                    },
                    Value::Module(_, namespace) => {
                        // Check module attributes
                        if let Some(value) = namespace.get(&attr_name) {
                            value.clone()
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                        }
                    },
                    Value::Dict(dict) => {
                        // First check if this is a method name
                        if let Some(_method) = object_value.get_method(&attr_name) {
                            // Return a BoundMethod so the dict instance is available when called
                            Value::BoundMethod {
                                object: Box::new(object_value.clone()),
                                method_name: attr_name.clone(),
                            }
                        }
                        // Otherwise treat keys as attributes
                        else if let Some(value) = dict.borrow().get(&attr_name) {
                            value.clone()
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                        }
                    },
                    _ => {
                        // For other objects, try to get method
                        if let Some(method) = object_value.get_method(&attr_name) {
                            method
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                        }
                    }
                };
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::StoreAttr => {
                // Store attribute to object (obj.attr = value)
                let object_reg = arg1 as usize;
                let attr_name_idx = arg2 as usize;
                let value_reg = arg3 as usize;
                
                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreAttr: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }
                
                if attr_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("StoreAttr: attribute name index {} out of bounds (len: {})", attr_name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                if value_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreAttr: value register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                // Clone the values first to avoid borrowing issues
                let attr_name = self.frames[frame_idx].code.names[attr_name_idx].clone();
                let value_to_store = self.frames[frame_idx].registers[value_reg].value.clone();
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let object_type_name = object_value.type_name();

                // CRITICAL FIX: Track which variables reference this object before modification
                // so we can update them after modification to see the changes
                let mut vars_to_update: Vec<String> = Vec::new();

                // Check if this is an Object and get its Rc<HashMap> pointer for comparison
                if let Value::Object { fields: obj_fields, .. } = &object_value {
                    let obj_ptr = Rc::as_ptr(obj_fields);

                    // Check globals
                    for (name, global_value) in self.globals.borrow().iter() {
                        if let Value::Object { fields: global_fields, .. } = &global_value.value {
                            if Rc::as_ptr(&global_fields) == obj_ptr {
                                vars_to_update.push(format!("global:{}", name));
                            }
                        }
                    }

                    // Check frame globals
                    for (name, frame_global_value) in self.frames[frame_idx].globals.borrow().iter() {
                        if let Value::Object { fields: frame_fields, .. } = &frame_global_value.value {
                            if Rc::as_ptr(&frame_fields) == obj_ptr {
                                vars_to_update.push(format!("frame_global:{}", name));
                            }
                        }
                    }

                    // Check locals
                    for (idx, local_value) in self.frames[frame_idx].locals.iter().enumerate() {
                        if let Value::Object { fields: local_fields, .. } = &local_value.value {
                            if Rc::as_ptr(local_fields) == obj_ptr {
                                vars_to_update.push(format!("local:{}", idx));
                            }
                        }
                    }
                }
                
                // Check if we're in an __init__ frame
                if self.frames[frame_idx].code.name == "__init__" || self.frames[frame_idx].code.name == "<fn:__init__>" {
                    // Check if this is the self parameter (locals[0])
                    if object_reg < self.frames[frame_idx].registers.len() {
                        // Get the self instance from locals[0]
                        if !self.frames[frame_idx].locals.is_empty() {
                        }
                    }
                }
                
                // Check if we're dealing with an Object that might have properties or descriptors
                let is_object_with_fields = matches!(object_value, Value::Object { .. });

                if is_object_with_fields {
                    // First, check if this attribute is a descriptor in class_methods
                    let descriptor_setter_result = match &object_value {
                        Value::Object { class_methods, .. } => {
                            if let Some(descriptor_obj) = class_methods.get(&attr_name) {
                                // Check if it's a descriptor (has __set__ method)
                                if let Some(setter) = descriptor_obj.get_method("__set__") {
                                    // Call the descriptor's __set__ method
                                    // __set__(self, obj, value)
                                    let args = vec![descriptor_obj.clone(), object_value.clone(), value_to_store.clone()];
                                    match setter {
                                        Value::BuiltinFunction(_, func) => Some(func(args)),
                                        Value::NativeFunction(func) => Some(func(args)),
                                        Value::Closure { .. } => {
                                            // For closure, we need to call it through the VM
                                            self.call_function_fast(setter.clone(), args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                                            return Ok(None);
                                        },
                                        _ => None
                                    }
                                } else if let Value::Object { class_name, fields, .. } = descriptor_obj {
                                    // Check if it's a property object
                                    if class_name == "property" {
                                        // This is a property, check if it has a setter
                                        if let Some(setter) = fields.as_ref().get("fset") {
                                            // Call the setter with self and the value
                                            let args = vec![object_value.clone(), value_to_store.clone()];
                                            match setter {
                                                Value::BuiltinFunction(_, func) => Some(func(args.clone())),
                                                Value::NativeFunction(func) => Some(func(args.clone())),
                                                Value::Closure { compiled_code: Some(code_obj), .. } => {
                                                    // For property setters, we need to create a frame and mark it as a setter
                                                    let globals_rc = Rc::clone(&self.globals);
                                                    let builtins_rc = Rc::clone(&self.builtins);

                                                    let mut setter_frame = Frame::new_function_frame(code_obj.as_ref().clone(), globals_rc, builtins_rc, args, HashMap::new());

                                                    // Mark this frame as a property setter
                                                    setter_frame.is_property_setter = true;

                                                    // Store the vars_to_update list so we can update them after the setter completes
                                                    setter_frame.vars_to_update = vars_to_update.clone();

                                                    // Set the return register so the modified object gets stored back
                                                    setter_frame.return_register = Some((frame_idx, object_reg as u32));

                                                    // Push the setter frame onto the stack
                                                    self.frames.push(setter_frame);

                                                    // Return None to let the VM execute the setter frame
                                                    return Ok(None);
                                                },
                                                Value::Closure { .. } => {
                                                    // If there's no compiled code, fall back to normal call
                                                    self.call_function_fast(setter.clone(), args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                                                    return Ok(None);
                                                },
                                                _ => None
                                            }
                                        } else {
                                            // Property has no setter, it's read-only
                                            return Err(anyhow!("can't set attribute '{}' on '{}' object", attr_name, object_type_name));
                                        }
                                    } else {
                                        None
                                    }
                                } else {
                                    None
                                }
                            } else {
                                None
                            }
                        }
                        _ => None
                    };

                    // If we called a descriptor setter, return early
                    if let Some(result) = descriptor_setter_result {
                        result?;
                        return Ok(None);
                    }

                    // Get the current value of the field to check if it's a descriptor
                    let current_field_value = match &object_value {
                        Value::Object { fields, .. } => fields.as_ref().get(&attr_name).cloned(),
                        _ => None
                    };

                    // If the field exists and is a descriptor, call its __set__ method
                    if let Some(descriptor) = current_field_value {
                        if let Some(setter) = descriptor.get_method("__set__") {
                            // Call the descriptor's __set__ method
                            // __set__(self, obj, value)
                            let args = vec![descriptor.clone(), object_value.clone(), value_to_store.clone()];
                            match setter {
                                Value::BuiltinFunction(_, func) => {
                                    func(args.clone())?;
                                    return Ok(None); // Successfully called descriptor setter
                                },
                                Value::NativeFunction(func) => {
                                    func(args.clone())?;
                                    return Ok(None); // Successfully called descriptor setter
                                },
                                _ => {
                                    // Continue with normal assignment below
                                }
                            }
                        }
                    }
                }
                
                // Normal assignment for all other cases
                match &mut self.frames[frame_idx].registers[object_reg].value {
                    Value::Class { methods, .. } => {
                        // Store class attribute in methods HashMap
                        methods.insert(attr_name.clone(), value_to_store.clone());

                        // Update the class in globals to reflect the change
                        // Find the class in globals and update it
                        let updated_class = self.frames[frame_idx].registers[object_reg].clone();
                        for (var_name, var_value) in self.globals.borrow_mut().iter_mut() {
                            if let Value::Class { name: class_name, .. } = &var_value.value {
                                if let Value::Class { name: updated_class_name, .. } = &updated_class.value {
                                    if class_name == updated_class_name {
                                        *var_value = updated_class.clone();
                                        break;
                                    }
                                }
                            }
                        }
                    },
                    Value::Object { fields, .. } => {
                        // Store in fields using Rc::make_mut to get a mutable reference
                        Rc::make_mut(fields).insert(attr_name.clone(), value_to_store.clone());

                        // CRITICAL FIX: Update locals[0] (self) with the modified object
                        // This ensures that subsequent loads of 'self' see the updated fields
                        // This applies to ALL methods, not just __init__
                        if !self.frames[frame_idx].locals.is_empty() {
                            // Update locals[0] with the modified object from the register
                            let updated_object = self.frames[frame_idx].registers[object_reg].clone();
                            self.frames[frame_idx].locals[0] = updated_object;
                        }

                        // For ALL methods (not just __init__), update the instance in the caller frame's register
                        // This ensures that when methods modify self, the changes are visible to the caller
                        if let Some((caller_frame_idx, result_reg)) = self.frames[frame_idx].return_register {
                            if caller_frame_idx < self.frames.len() {
                                // Update the instance in the caller frame's register with all modified fields
                                // Clone the entire modified object from current frame to caller frame
                                let modified_object = self.frames[frame_idx].registers[object_reg].clone();
                                self.frames[caller_frame_idx].registers[result_reg as usize] = modified_object;
                            }
                        }

                // Let's verify that the value was actually stored
                if let Value::Object { fields, .. } = &self.frames[frame_idx].registers[object_reg].value {
                }
                    },
                    Value::Dict(dict) => {
                        // For dictionaries, treat keys as attributes
                        dict.borrow_mut().insert(attr_name, value_to_store);
                    },
                    Value::Module(_, namespace) => {
                        // For modules, store in namespace
                        namespace.insert(attr_name, value_to_store);
                    },
                    _ => {
                        return Err(anyhow!("'{}' object does not support attribute assignment", object_type_name));
                    }
                };

                // CRITICAL FIX: After modifying an object in a register, update all variables
                // that were referencing this object so they see the modifications
                let modified_object = self.frames[frame_idx].registers[object_reg].clone();

                for var_spec in vars_to_update {
                    let parts: Vec<&str> = var_spec.split(':').collect();
                    match parts[0] {
                        "global" => {
                            if self.globals.borrow().contains_key(parts[1]) {
                                self.globals.borrow_mut().insert(parts[1].to_string(), modified_object.clone());
                            }
                        }
                        "frame_global" => {
                            if self.frames[frame_idx].globals.borrow().contains_key(parts[1]) {
                                self.frames[frame_idx].globals.borrow_mut().insert(parts[1].to_string(), modified_object.clone());
                            }
                        }
                        "local" => {
                            if let Ok(idx) = parts[1].parse::<usize>() {
                                if idx < self.frames[frame_idx].locals.len() {
                                    self.frames[frame_idx].locals[idx] = modified_object.clone();
                                }
                            }
                        }
                        _ => {}
                    }
                }

                Ok(None)
            }
            OpCode::DeleteAttr => {
                // Delete attribute from object (del obj.attr)
                let object_reg = arg1 as usize;
                let attr_name_idx = arg2 as usize;
                
                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("DeleteAttr: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }
                
                if attr_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("DeleteAttr: attribute name index {} out of bounds (len: {})", attr_name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                // Clone the values first to avoid borrowing issues
                let attr_name = self.frames[frame_idx].code.names[attr_name_idx].clone();
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let object_type_name = object_value.type_name();
                
                // Check if we're dealing with an Object that might have descriptors
                let is_object_with_fields = matches!(object_value, Value::Object { .. });
                
                if is_object_with_fields {
                    // First check class methods for descriptors
                    let class_descriptor = match &object_value {
                        Value::Object { class_methods, .. } => class_methods.get(&attr_name).cloned(),
                        _ => None
                    };

                    if let Some(descriptor) = class_descriptor {
                        if let Some(deleter) = descriptor.get_method("__delete__") {
                            // Call the descriptor's __delete__ method
                            // __delete__(self, obj)
                            let args = vec![descriptor.clone(), object_value.clone()];
                            match deleter {
                                Value::BuiltinFunction(_, func) => {
                                    func(args)?;
                                    return Ok(None); // Successfully called descriptor deleter
                                },
                                Value::NativeFunction(func) => {
                                    func(args)?;
                                    return Ok(None); // Successfully called descriptor deleter
                                },
                                Value::Closure { .. } => {
                                    // For closure, we need to call it through the VM
                                    self.call_function_fast(deleter.clone(), args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                                    return Ok(None);
                                },
                                _ => {
                                    // Continue with normal deletion below
                                }
                            }
                        }
                    }

                    // Then check instance fields for descriptors
                    let current_field_value = match &object_value {
                        Value::Object { fields, .. } => fields.as_ref().get(&attr_name).cloned(),
                        _ => None
                    };

                    // If the field exists and is a descriptor, call its __delete__ method
                    if let Some(descriptor) = current_field_value {
                        if let Some(deleter) = descriptor.get_method("__delete__") {
                            // Call the descriptor's __delete__ method
                            // __delete__(self, obj)
                            let args = vec![descriptor.clone(), object_value.clone()];
                            match deleter {
                                Value::BuiltinFunction(_, func) => {
                                    func(args.clone())?;
                                    return Ok(None); // Successfully called descriptor deleter
                                },
                                Value::NativeFunction(func) => {
                                    func(args.clone())?;
                                    return Ok(None); // Successfully called descriptor deleter
                                },
                                _ => {
                                    // Continue with normal deletion below
                                }
                            }
                        }
                    }
                }
                
                // Normal deletion for all other cases
                match &mut self.frames[frame_idx].registers[object_reg].value {
                    Value::Object { fields, .. } => {
                        // Remove from fields
                        if !fields.as_ref().contains_key(&attr_name) {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_type_name, attr_name));
                        }
                        Rc::make_mut(fields).remove(&attr_name);
                    },
                    Value::Dict(dict) => {
                        // For dictionaries, treat keys as attributes
                        if !dict.borrow().contains_key(&attr_name) {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_type_name, attr_name));
                        }
                        dict.borrow_mut().remove(&attr_name);
                    },
                    Value::Module(_, namespace) => {
                        // For modules, remove from namespace
                        if !namespace.contains_key(&attr_name) {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_type_name, attr_name));
                        }
                        namespace.remove(&attr_name);
                    },
                    _ => {
                        return Err(anyhow!("'{}' object does not support attribute deletion", object_type_name));
                    }
                };
                
                Ok(None)
            }
            OpCode::LoadZeroArgSuper => {
                // Handle zero-argument super() calls
                let class_name_const_idx = arg1 as usize;
                let self_reg = arg2 as usize;
                let result_reg = arg3;
                
                if class_name_const_idx >= self.frames[frame_idx].code.constants.len() {
                    return Err(anyhow!("LoadZeroArgSuper: class name constant index {} out of bounds (len: {})", class_name_const_idx, self.frames[frame_idx].code.constants.len()));
                }
                
                if self_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadZeroArgSuper: self register index {} out of bounds (len: {})", self_reg, self.frames[frame_idx].registers.len()));
                }
                
                // Get the class name from constants
                let class_name = match &self.frames[frame_idx].code.constants[class_name_const_idx] {
                    Value::Str(s) => s.clone(),
                    _ => return Err(anyhow!("LoadZeroArgSuper: class name constant must be a string")),
                };
                
                // Get the self instance from registers (first local variable which should be 'self')
                let self_instance = if !self.frames[frame_idx].locals.is_empty() {
                    self.frames[frame_idx].locals[0].value.clone()
                } else {
                    self.frames[frame_idx].registers[self_reg].value.clone()
                };
                
                // Get the class methods from globals
                let super_obj = if let Some(class_value) = self.frames[frame_idx].globals.borrow().get(&class_name).cloned() {
                    match &class_value.value {
                        Value::Class { name, methods, mro, .. } => {
                            // CRITICAL FIX: For correct diamond inheritance, we need to use the instance's actual MRO,
                            // not the class's MRO. This ensures that super() follows the correct method resolution order
                            // for the actual runtime type of the instance.
                            let instance_mro = if let Value::Object { mro: instance_mro, .. } = &self_instance {
                                instance_mro.get_linearization()
                            } else {
                                mro.get_linearization()
                            };

                            // Find the current class in the instance's MRO and get the next class
                            let parent_class = if let Some(current_idx) = instance_mro.iter().position(|c| c == name) {
                                // Get the next class in the MRO
                                if current_idx + 1 < instance_mro.len() {
                                    instance_mro[current_idx + 1].clone()
                                } else {
                                    "object".to_string()
                                }
                            } else {
                                // Fallback to the class's own MRO if we can't find it in the instance's MRO
                                if let Some(second_class) = mro.get_linearization().get(1) {
                                    second_class.clone()
                                } else {
                                    "object".to_string()
                                }
                            };

                            // Get parent class and its MRO - use VM globals instead of frame globals
                            // to ensure we can find all classes defined in the module
                            let (parent_methods, parent_mro) = if let Some(parent_class_value) = self.globals.borrow().get(&parent_class).cloned() {
                                match &parent_class_value.value {
                                    Value::Class { methods, mro, .. } => {
                                        (Some(methods.clone()), Some(mro.clone()))
                                    },
                                    _ => (None, None)
                                }
                            } else {
                                (None, None)
                            };

                            // Create the super object with the current class, parent class, instance, and parent methods
                            Value::Super(name.clone(), parent_class, Some(Box::new(self_instance)), parent_methods)
                        },
                        _ => {
                            return Err(anyhow!("LoadZeroArgSuper: {} is not a class", class_name));
                        }
                    }
                } else {
                    return Err(anyhow!("LoadZeroArgSuper: class {} not found in globals", class_name));
                };
                
                // Store the super object in the result register
                self.frames[frame_idx].set_register(result_reg, RcValue::new(super_obj));
                Ok(None)
            }
            OpCode::ImportModule => {
                // Import a module and store it in the global namespace
                // arg1: name index (module name in names array)
                // arg2: result register (where to store the module)
                let name_idx = arg1 as usize;
                let result_reg = arg2;

                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("ImportModule: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }

                let module_name = self.frames[frame_idx].code.names[name_idx].clone();

                // Try to load the builtin module first
                let module_value = if let Some(module) = modules::get_builtin_module(&module_name) {
                    module
                } else {
                    // Try to load from file system
                    match self.load_module_from_file(&module_name) {
                        Ok(module) => module,
                        Err(e) => {
                            // If this is a circular import error, re-raise it without wrapping
                            if e.to_string().contains("circular import") {
                                return Err(e);
                            }
                            return Err(anyhow!("ImportModule: module '{}' not found: {}", module_name, e));
                        }
                    }
                };

                // Store the module in both globals and the result register
                let rc_module = RcValue::new(module_value.clone());
                self.globals.borrow_mut().insert(module_name.clone(), rc_module.clone());
                self.frames[frame_idx].globals.borrow_mut().insert(module_name.clone(), rc_module.clone());

                // Handle hierarchical packages (e.g., "win32.constants")
                if module_name.contains('.') {
                    let parts: Vec<&str> = module_name.split('.').collect();

                    // Create or get parent package modules
                    for i in 0..parts.len() - 1 {
                        let parent_name = parts[0..=i].join(".");
                        let child_name = parts[i + 1].to_string();
                        let child_full_name = parts[0..=i+1].join(".");

                        // Get the child module
                        let child_module = if i + 1 == parts.len() - 1 {
                            // This is the final submodule we're importing
                            rc_module.clone()
                        } else {
                            // This is an intermediate package - get or create it
                            let existing_child = self.globals.borrow().get(&child_full_name).cloned();
                            if let Some(child) = existing_child {
                                child
                            } else {
                                let new_mod = RcValue::new(Value::Module(child_full_name.clone(), std::collections::HashMap::new()));
                                self.globals.borrow_mut().insert(child_full_name.clone(), new_mod.clone());
                                self.frames[frame_idx].globals.borrow_mut().insert(child_full_name.clone(), new_mod.clone());
                                new_mod
                            }
                        };

                        // Get or create parent module with updated namespace
                        let _parent_module = {
                            let existing_parent = self.globals.borrow().get(&parent_name).cloned();
                            if let Some(existing) = existing_parent {
                                // Parent exists - need to create new version with updated namespace
                                if let Value::Module(name, mut namespace) = existing.value.clone() {
                                    namespace.insert(child_name, child_module.value.clone());
                                    let updated_parent = RcValue::new(Value::Module(name, namespace));
                                    self.globals.borrow_mut().insert(parent_name.clone(), updated_parent.clone());
                                    self.frames[frame_idx].globals.borrow_mut().insert(parent_name.clone(), updated_parent.clone());
                                    updated_parent
                                } else {
                                    existing.clone()
                                }
                            } else {
                                // Create new parent module
                                let mut namespace = std::collections::HashMap::new();
                                namespace.insert(child_name, child_module.value.clone());
                                let new_parent = RcValue::new(Value::Module(parent_name.clone(), namespace));
                                self.globals.borrow_mut().insert(parent_name.clone(), new_parent.clone());
                                self.frames[frame_idx].globals.borrow_mut().insert(parent_name.clone(), new_parent.clone());
                                new_parent
                            }
                        };
                    }

                    // Store the actual imported module in the result register
                    // (not the top-level package)
                    self.frames[frame_idx].set_register(result_reg, rc_module);
                } else {
                    self.frames[frame_idx].set_register(result_reg, rc_module);
                }

                Ok(None)
            }
            OpCode::ImportFrom => {
                // Import specific names from a module
                // arg1: module name index
                // arg2: name to import index
                // arg3: result register
                let module_name_idx = arg1 as usize;
                let import_name_idx = arg2 as usize;
                let result_reg = arg3;

                if module_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("ImportFrom: module name index {} out of bounds (len: {})", module_name_idx, self.frames[frame_idx].code.names.len()));
                }

                if import_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("ImportFrom: import name index {} out of bounds (len: {})", import_name_idx, self.frames[frame_idx].code.names.len()));
                }

                let module_name = self.frames[frame_idx].code.names[module_name_idx].clone();
                let import_name = self.frames[frame_idx].code.names[import_name_idx].clone();

                // Try to load the builtin module first
                let module_value = if let Some(module) = modules::get_builtin_module(&module_name) {
                    module
                } else {
                    // Try to load from file system
                    match self.load_module_from_file(&module_name) {
                        Ok(module) => module,
                        Err(e) => {
                            // If this is a circular import error, re-raise it without wrapping
                            if e.to_string().contains("circular import") {
                                return Err(e);
                            }
                            return Err(anyhow!("ImportFrom: module '{}' not found: {}", module_name, e));
                        }
                    }
                };

                // Check if this is a star import (from module import *)
                if import_name == "*" {
                    // Import all names from the module (star import)
                    match &module_value {
                        Value::Module(_, namespace) => {
                            // Import all names from the module namespace
                            for (name, value) in namespace {
                                // Skip private names (starting with _) unless they're special like __all__
                                if !name.starts_with("_") || name == "__all__" {
                                    let rc_value = RcValue::new(value.clone());
                                    self.globals.borrow_mut().insert(name.clone(), rc_value.clone());
                                    self.frames[frame_idx].globals.borrow_mut().insert(name.clone(), rc_value.clone());
                                }
                            }

                            // For star imports, we don't store anything in the result register
                            // Just put None there
                            self.frames[frame_idx].set_register(result_reg, RcValue::new(Value::None));
                        }
                        _ => {
                            return Err(anyhow!("ImportFrom: '{}' is not a module", module_name));
                        }
                    }
                } else {
                    // Regular from-import (specific name)
                    let imported_value = match &module_value {
                        Value::Module(_, namespace) => {
                            if let Some(value) = namespace.get(&import_name) {
                                value.clone()
                            } else {
                                return Err(anyhow!("ImportFrom: cannot import name '{}' from module '{}'", import_name, module_name));
                            }
                        }
                        _ => {
                            return Err(anyhow!("ImportFrom: '{}' is not a module", module_name));
                        }
                    };

                    // Store the imported value in globals and the result register
                    let rc_value = RcValue::new(imported_value);
                    self.globals.borrow_mut().insert(import_name.clone(), rc_value.clone());
                    self.frames[frame_idx].globals.borrow_mut().insert(import_name.clone(), rc_value.clone());
                    self.frames[frame_idx].set_register(result_reg, rc_value);
                }

                Ok(None)
            }
            OpCode::ImportFrom => {
                // Import specific names from a module
                // arg1: module name index
                // arg2: name to import index
                // arg3: result register
                let module_name_idx = arg1 as usize;
                let import_name_idx = arg2 as usize;
                let result_reg = arg3;

                if module_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("ImportFrom: module name index {} out of bounds (len: {})", module_name_idx, self.frames[frame_idx].code.names.len()));
                }

                if import_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("ImportFrom: import name index {} out of bounds (len: {})", import_name_idx, self.frames[frame_idx].code.names.len()));
                }

                let module_name = self.frames[frame_idx].code.names[module_name_idx].clone();
                let import_name = self.frames[frame_idx].code.names[import_name_idx].clone();

                // Try to load the builtin module first
                let module_value = if let Some(module) = modules::get_builtin_module(&module_name) {
                    module
                } else {
                    // Try to load from file system
                    match self.load_module_from_file(&module_name) {
                        Ok(module) => module,
                        Err(e) => {
                            // If this is a circular import error, re-raise it without wrapping
                            if e.to_string().contains("circular import") {
                                return Err(e);
                            }
                            return Err(anyhow!("ImportFrom: module '{}' not found: {}", module_name, e));
                        }
                    }
                };

                // Check if this is a star import (from module import *)
                if import_name == "*" {
                    // Import all names from the module (star import)
                    match &module_value {
                        Value::Module(_, namespace) => {
                            // Import all names from the module namespace
                            for (name, value) in namespace {
                                // Skip private names (starting with _) unless they're special like __all__
                                if !name.starts_with("_") || name == "__all__" {
                                    let rc_value = RcValue::new(value.clone());
                                    self.globals.borrow_mut().insert(name.clone(), rc_value.clone());
                                    self.frames[frame_idx].globals.borrow_mut().insert(name.clone(), rc_value.clone());
                                }
                            }

                            // For star imports, we don't store anything in the result register
                            // Just put None there
                            self.frames[frame_idx].set_register(result_reg, RcValue::new(Value::None));
                        }
                        _ => {
                            return Err(anyhow!("ImportFrom: '{}' is not a module", module_name));
                        }
                    }
                } else {
                    // Regular from-import (specific name)
                    let imported_value = match &module_value {
                        Value::Module(_, namespace) => {
                            if let Some(value) = namespace.get(&import_name) {
                                value.clone()
                            } else {
                                return Err(anyhow!("ImportFrom: cannot import name '{}' from module '{}'", import_name, module_name));
                            }
                        }
                        _ => {
                            return Err(anyhow!("ImportFrom: '{}' is not a module", module_name));
                        }
                    };

                    // Store the imported value in globals and the result register
                    let rc_value = RcValue::new(imported_value);
                    self.globals.borrow_mut().insert(import_name.clone(), rc_value.clone());
                    self.frames[frame_idx].globals.borrow_mut().insert(import_name.clone(), rc_value.clone());
                    self.frames[frame_idx].set_register(result_reg, rc_value);
                }

                Ok(None)
            }
            _ => {
                // For unimplemented opcodes, we'll just return an error
                // In a complete implementation, we would handle all opcodes
                Err(anyhow!("Unimplemented opcode: {:?}", opcode))
            }
        }
    }
    
    /// Call a function with optimized fast path
    pub fn call_function_fast(&mut self, func_value: Value, args: Vec<Value>, kwargs: HashMap<String, Value>, frame_idx: Option<usize>, result_reg: Option<u32>) -> Result<Value> {
        match func_value {
            Value::BuiltinFunction(name, func) => {
                // Special handling for eval/exec/compile - they need VM access
                if name == "eval" {
                    if args.is_empty() || args.len() > 3 {
                        return Err(anyhow!("eval() takes at most 3 arguments ({} given)", args.len()));
                    }

                    let source = match &args[0] {
                        Value::Str(s) => s.clone(),
                        Value::Code(code) => {
                            // If it's already a code object, execute it and retrieve __eval_result__
                            self.execute(*code.clone())?;

                            // Get the result from globals (set by compile in eval mode)
                            let result = self.globals.borrow().get("__eval_result__")
                                .map(|v| v.value.clone())
                                .unwrap_or(Value::None);

                            // Clean up the temporary variable
                            self.globals.borrow_mut().remove("__eval_result__");

                            return Ok(result);
                        }
                        _ => return Err(anyhow!("eval() arg 1 must be a string or code object")),
                    };

                    // Extract globals and locals if provided
                    let globals = if args.len() > 1 {
                        match &args[1] {
                            Value::Dict(d) => Some(d.borrow().clone()),
                            Value::None => None,
                            _ => return Err(anyhow!("eval() arg 2 must be a dict or None")),
                        }
                    } else {
                        None
                    };

                    let locals = if args.len() > 2 {
                        match &args[2] {
                            Value::Dict(d) => Some(d.borrow().clone()),
                            Value::None => None,
                            _ => return Err(anyhow!("eval() arg 3 must be a dict or None")),
                        }
                    } else {
                        None
                    };

                    return self.eval_impl(&source, globals, locals);
                }

                if name == "exec" {
                    if args.is_empty() || args.len() > 3 {
                        return Err(anyhow!("exec() takes at most 3 arguments ({} given)", args.len()));
                    }

                    let source = match &args[0] {
                        Value::Str(s) => s.clone(),
                        Value::Code(code) => {
                            // If it's already a code object, execute it directly
                            self.execute(*code.clone())?;
                            return Ok(Value::None);
                        }
                        _ => return Err(anyhow!("exec() arg 1 must be a string or code object")),
                    };

                    // Extract globals and locals if provided
                    let globals = if args.len() > 1 {
                        match &args[1] {
                            Value::Dict(d) => Some(d.borrow().clone()),
                            Value::None => None,
                            _ => return Err(anyhow!("exec() arg 2 must be a dict or None")),
                        }
                    } else {
                        None
                    };

                    let locals = if args.len() > 2 {
                        match &args[2] {
                            Value::Dict(d) => Some(d.borrow().clone()),
                            Value::None => None,
                            _ => return Err(anyhow!("exec() arg 3 must be a dict or None")),
                        }
                    } else {
                        None
                    };

                    self.exec_impl(&source, globals, locals)?;
                    return Ok(Value::None);
                }

                if name == "compile" {
                    if args.len() < 3 || args.len() > 5 {
                        return Err(anyhow!("compile() takes 3-5 arguments ({} given)", args.len()));
                    }

                    let source = match &args[0] {
                        Value::Str(s) => s.clone(),
                        _ => return Err(anyhow!("compile() arg 1 must be a string")),
                    };

                    let filename = match &args[1] {
                        Value::Str(s) => s.clone(),
                        _ => return Err(anyhow!("compile() arg 2 must be a string")),
                    };

                    let mode = match &args[2] {
                        Value::Str(s) => s.clone(),
                        _ => return Err(anyhow!("compile() arg 3 must be a string")),
                    };

                    return self.compile_impl(&source, &filename, &mode);
                }

                // Special handling for str() and repr() to support __str__ and __repr__ dunder methods
                if name == "str" && args.len() == 1 {
                    if let Some(str_method) = args[0].get_method("__str__") {
                        // Call __str__(self)
                        return self.call_function_fast(
                            str_method,
                            vec![args[0].clone()],
                            HashMap::new(),
                            frame_idx,
                            result_reg
                        );
                    }
                }

                if name == "repr" && args.len() == 1 {
                    if let Some(repr_method) = args[0].get_method("__repr__") {
                        // Call __repr__(self)
                        return self.call_function_fast(
                            repr_method,
                            vec![args[0].clone()],
                            HashMap::new(),
                            frame_idx,
                            result_reg
                        );
                    }
                }

                // Special handling for globals() - return actual globals from VM
                if name == "globals" {
                    if !args.is_empty() {
                        return Err(anyhow!("globals() takes no arguments ({} given)", args.len()));
                    }

                    // Convert globals to a regular dict that can be used in Python code
                    let globals_dict = Rc::new(RefCell::new(
                        self.globals.borrow().iter()
                            .map(|(k, v)| (k.clone(), v.value.clone()))
                            .collect::<HashMap<String, Value>>()
                    ));
                    return Ok(Value::Dict(globals_dict));
                }

                // Special handling for locals() - return actual locals from current frame
                if name == "locals" {
                    if !args.is_empty() {
                        return Err(anyhow!("locals() takes no arguments ({} given)", args.len()));
                    }

                    // Get the current frame's locals
                    if let Some(frame) = self.frames.last() {
                        let locals_dict = Rc::new(RefCell::new(
                            frame.locals_map.iter()
                                .filter_map(|(name, &idx)| {
                                    frame.locals.get(idx).map(|v| (name.clone(), v.value.clone()))
                                })
                                .collect::<HashMap<String, Value>>()
                        ));
                        return Ok(Value::Dict(locals_dict));
                    } else {
                        // No frame, return globals (like Python does at module level)
                        let globals_dict = Rc::new(RefCell::new(
                            self.globals.borrow().iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect::<HashMap<String, Value>>()
                        ));
                        return Ok(Value::Dict(globals_dict));
                    }
                }

                // Special handling for dir() - list names in current scope
                if name == "dir" {
                    if args.len() > 1 {
                        return Err(anyhow!("dir() takes at most 1 argument ({} given)", args.len()));
                    }

                    if args.is_empty() {
                        // dir() without arguments - return names in current scope
                        let mut names = Vec::new();

                        // Get names from current frame's locals
                        if let Some(frame) = self.frames.last() {
                            for name in frame.locals_map.keys() {
                                names.push(Value::Str(name.clone()));
                            }
                        }

                        // Add names from globals
                        for name in self.globals.borrow().keys() {
                            names.push(Value::Str(name.clone()));
                        }

                        // Sort and remove duplicates
                        names.sort_by(|a, b| {
                            if let (Value::Str(a_str), Value::Str(b_str)) = (a, b) {
                                a_str.cmp(b_str)
                            } else {
                                std::cmp::Ordering::Equal
                            }
                        });
                        names.dedup();

                        return Ok(Value::List(HPList::from_values(names)));
                    } else {
                        // dir(obj) - use the regular builtin implementation for objects
                        return func(args.clone());
                    }
                }

                // Special handling for help() - show docstrings and help info
                if name == "help" {
                    if args.len() > 1 {
                        return Err(anyhow!("help() takes at most 1 argument ({} given)", args.len()));
                    }

                    if args.is_empty() {
                        // help() without arguments - show general help
                        println!("\nWelcome to Tauraro!");
                        println!();
                        println!("Tauraro is a Python-compatible programming language with Rust-like performance.");
                        println!();
                        println!("Type help() for interactive help, or help(object) for help about object.");
                        println!();
                        println!("Quick Reference:");
                        println!("  Variables:    x = 10");
                        println!("  Functions:    def greet(name): return f'Hello, {{name}}'");
                        println!("  Classes:      class MyClass: pass");
                        println!("  Loops:        for i in range(10): print(i)");
                        println!("  Conditions:   if x > 5: print('big')");
                        println!("  Import:       import math");
                        println!();
                        return Ok(Value::None);
                    }

                    let obj = &args[0];

                    // Handle string arguments - lookup the name
                    let target = if let Value::Str(name_str) = obj {
                        // Try to lookup the name in current scope
                        if let Some(frame) = self.frames.last() {
                            if let Some(&idx) = frame.locals_map.get(name_str) {
                                frame.locals.get(idx).map(|v| v.value.clone())
                            } else {
                                self.globals.borrow().get(name_str).map(|v| v.value.clone())
                            }
                        } else {
                            self.globals.borrow().get(name_str).map(|v| v.value.clone())
                        }
                    } else {
                        Some(obj.clone())
                    };

                    if let Some(target_val) = target {
                        // Show help for the object
                        match &target_val {
                            Value::Closure { name: func_name, params, docstring, .. } => {
                                println!("\nHelp on function {}:", func_name);
                                println!();

                                // Show function signature
                                let param_names: Vec<String> = params.iter()
                                    .map(|p| {
                                        if let Some(ref default) = p.default {
                                            format!("{}={:?}", p.name, default)
                                        } else {
                                            p.name.clone()
                                        }
                                    })
                                    .collect();
                                println!("{}({})", func_name, param_names.join(", "));
                                println!();

                                // Show docstring if available
                                if let Some(doc) = docstring {
                                    println!("{}", doc);
                                } else {
                                    println!("No documentation available.");
                                }
                                println!();
                            }
                            Value::Class { name: class_name, methods, .. } => {
                                println!("\nHelp on class {}:", class_name);
                                println!();
                                println!("class {}", class_name);
                                println!();

                                // List methods
                                if !methods.is_empty() {
                                    println!("Methods:");
                                    let mut method_names: Vec<_> = methods.keys().collect();
                                    method_names.sort();
                                    for method_name in method_names {
                                        println!("  {}", method_name);
                                    }
                                    println!();
                                }
                            }
                            Value::BuiltinFunction(builtin_name, _) => {
                                println!("\nHelp on built-in function {}:", builtin_name);
                                println!();
                                println!("{}(...)", builtin_name);
                                println!("    Built-in function");
                                println!();
                            }
                            Value::Module(module_name, namespace) => {
                                println!("\nHelp on module {}:", module_name);
                                println!();
                                println!("NAME");
                                println!("    {}", module_name);
                                println!();

                                // List module contents
                                if !namespace.is_empty() {
                                    println!("CONTENTS:");
                                    let mut names: Vec<_> = namespace.keys().collect();
                                    names.sort();
                                    for name in names {
                                        println!("    {}", name);
                                    }
                                    println!();
                                }
                            }
                            Value::Object { class_name, .. } => {
                                println!("\nHelp on {} object:", class_name);
                                println!();
                                println!("Instance of class {}", class_name);
                                println!();
                            }
                            _ => {
                                println!("\nHelp on {}:", target_val.type_name());
                                println!();
                                println!("Type: {}", target_val.type_name());
                                println!();
                            }
                        }
                    } else if let Value::Str(name_str) = obj {
                        println!("\nNo Python documentation found for '{}'", name_str);
                        println!();
                    }

                    return Ok(Value::None);
                }

                // For builtin functions, we should not pass kwargs as they don't expect them
                // Concatenate args and kwargs values if needed, or handle them appropriately
                // For now, let's just pass the args to builtin functions
                func(args.clone())
            }
            Value::NativeFunction(func) => {
                // Call native function directly
                func(args.clone())
            }
            Value::Closure { name, params, body: _, captured_scope: _, docstring: _, compiled_code, module_globals } => {
                // Validate argument types if type checking is enabled
                #[cfg(feature = "type_checking")]
                {
                    // Check positional arguments against parameter types
                    for (i, (arg, param)) in args.iter().zip(params.iter()).enumerate() {
                        if let Some(ref type_annotation) = param.type_annotation {
                            if !arg.matches_type(type_annotation) {
                                return Err(anyhow!("TypeError: Argument {} of function '{}' must be of type '{}', got '{}'",
                                    i, name, type_annotation, arg.type_name()));
                            }
                        }
                    }
                }

                // Call user-defined function
                if let Some(code_obj) = compiled_code {
                    // Check if this is a generator function by looking at the instructions
                    let is_generator = code_obj.instructions.iter().any(|instr| {
                        matches!(instr.opcode, OpCode::YieldValue | OpCode::YieldFrom)
                    });

                    if is_generator {
                        // For generator functions, create a generator object instead of executing immediately
                        let mut generator_value = Value::Generator {
                            code: Box::new(*code_obj),
                            frame: None,
                            finished: false,
                        };

                        // If we have a return register, we need to set it up for the generator
                        if let (Some(caller_frame_idx), Some(result_reg)) = (frame_idx, result_reg) {
                            // We'll set up the generator frame when it's first called
                        }

                        Ok(generator_value)
                    } else {
                        // For regular functions, create a new frame for the function call
                        // Use module_globals if available (for functions from imported modules),
                        // otherwise use the current VM globals (for main script functions)
                        let globals_rc = if let Some(ref mod_globals) = module_globals {
                            Rc::clone(mod_globals)
                        } else {
                            Rc::clone(&self.globals)
                        };
                        let builtins_rc = Rc::clone(&self.builtins);

                        let mut frame = Frame::new_function_frame(*code_obj, globals_rc, builtins_rc, args, kwargs);

                        // Set the return register information if provided
                        if let (Some(caller_frame_idx), Some(result_reg)) = (frame_idx, result_reg) {
                            frame.return_register = Some((caller_frame_idx, result_reg));
                        } else {
                        }
                        
                        self.frames.push(frame);
                        
                        // Track function call for profiling
                        self.track_function_call(&name);
                        
                        // For user-defined functions, we don't return a value immediately
                        // The value will be returned when the function frame completes execution
                        Ok(Value::None)
                    }
                } else {
                    Err(anyhow!("Function '{}' has no compiled code", name))
                }
            }
            Value::Class { name: class_name, methods, mro, base_object, .. } => {
                // When a class is called, it creates a new instance of that class
                // Create the object instance
                let instance = Value::Object {
                    class_name: class_name.clone(),
                    fields: Rc::new(HashMap::new()),
                    class_methods: methods.clone(), // Use the class methods from the Class
                    base_object: base_object.clone(),
                    mro: mro.clone(),
                };

                // Look for __init__ method in the class or its parents via MRO
                let globals_values: HashMap<String, Value> = self.globals.borrow().iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                let init_method = methods.get("__init__").cloned().or_else(|| mro.find_method_in_mro("__init__", &globals_values));

                // If the instance has an __init__ method, call it
                if let Some(init_method) = init_method {
                    match init_method {
                        Value::BuiltinFunction(_, func) => {
                            let mut init_args = vec![instance.clone()];
                            init_args.extend(args.clone());
                            func(init_args)?;
                            // For builtin functions, we can return the instance directly
                            return Ok(instance);
                        },
                        Value::NativeFunction(func) => {
                            let mut init_args = vec![instance.clone()];
                            init_args.extend(args.clone());
                            func(init_args)?;
                            // For native functions, we can return the instance directly
                            return Ok(instance);
                        },
                        Value::Closure { name: method_name, params, body: _, captured_scope: _, docstring: _, compiled_code, .. } => {
                            // For user-defined __init__ methods, we need to call them in a way that
                            // ensures modifications to the instance are visible
                            if let Some(code_obj) = compiled_code {
                                // Use Rc-wrapped globals and builtins
                                let globals_rc = Rc::clone(&self.globals);
                                let builtins_rc = Rc::clone(&self.builtins);

                                // Create arguments with self as the first argument
                                let mut init_args = vec![instance.clone()];
                                init_args.extend(args.clone());

                                // Create a new frame for the __init__ method
                                let mut init_frame = Frame::new_function_frame(code_obj.as_ref().clone(), globals_rc, builtins_rc, init_args, HashMap::new());
                                
                                // Store the instance in the result register BEFORE creating the __init__ frame
                                // This ensures that the instance is available for modification during __init__ execution
                                if let (Some(caller_frame_idx), Some(result_reg)) = (frame_idx, result_reg) {
                                    if caller_frame_idx < self.frames.len() {
                                        self.frames[caller_frame_idx].set_register(result_reg, RcValue::new(instance.clone()));
                                    }
                                    init_frame.return_register = Some((caller_frame_idx, result_reg));
                                }
                                
                                // Push the frame onto the stack
                                self.frames.push(init_frame);
                                
                                // Return None to indicate that we've pushed a new frame and the main loop should handle it
                                return Ok(Value::None);
                            }
                        },
                        _ => {},
                    }
                }

                // Return the object instance for cases where there's no __init__ or __init__ is not handled above
                Ok(instance)
            }
            Value::Object {
                class_name,
                fields,
                class_methods,
                base_object,
                mro,
                ..
            } => {
                // Create the object instance
                let mut instance = Value::Object {
                    class_name: class_name.clone(),
                    fields: Rc::new(HashMap::new()),
                    class_methods: class_methods.clone(), // Use the class methods from the Object
                    base_object: base_object.clone(),
                    mro: mro.clone(),
                };
                
                // If the instance has an __init__ method, call it
                if let Some(init_method) = class_methods.get("__init__") {
                    match init_method {
                        Value::BuiltinFunction(_, func) => {
                            let mut init_args = vec![instance.clone()];
                            init_args.extend(args.clone());
                            func(init_args)?;
                        },
                        Value::NativeFunction(func) => {
                            let mut init_args = vec![instance.clone()];
                            init_args.extend(args.clone());
                            func(init_args)?;
                        },
                        Value::Closure { name: method_name, params, body: _, captured_scope: _, docstring: _, compiled_code, .. } => {
                            // For user-defined __init__ methods, we need to call them in a way that
                            // ensures modifications to the instance are visible
                            if let Some(code_obj) = compiled_code {
                                // Use Rc-wrapped globals and builtins
                                let globals_rc = Rc::clone(&self.globals);
                                let builtins_rc = Rc::clone(&self.builtins);

                                // Create arguments with self as the first argument
                                let mut init_args = vec![instance.clone()];
                                init_args.extend(args.clone());

                                // Create a new frame for the __init__ method
                                let mut init_frame = Frame::new_function_frame(code_obj.as_ref().clone(), globals_rc, builtins_rc, init_args, HashMap::new());

                                // Store the instance in the result register BEFORE creating the __init__ frame
                                // This ensures that the instance is available for modification during __init__ execution
                                if let (Some(caller_frame_idx), Some(result_reg)) = (frame_idx, result_reg) {
                                    if caller_frame_idx < self.frames.len() {
                                        self.frames[caller_frame_idx].set_register(result_reg, RcValue::new(instance.clone()));
                                    }
                                    init_frame.return_register = Some((caller_frame_idx, result_reg));
                                }

                                // Push the frame onto the stack
                                self.frames.push(init_frame);

                                // Return None to indicate that we've pushed a new frame and the main loop should handle it
                                return Ok(Value::None);
                            }
                        },
                        _ => {},
                    }
                }

                // Return the object instance (which may have been modified by __init__)
                Ok(instance)
            }
            Value::BoundMethod { object, method_name } => {
                // Handle bound method calls
                // Get the method from the object
                match object.as_ref() {
                    Value::Object { class_name, class_methods, mro, .. } => {
                        // First try to find the method in class_methods (normal case)
                        if let Some(method) = class_methods.get(&method_name) {
                            // For bound methods, we need to call the method with the object as the first argument (self)
                            // But we need to do this through the VM properly

                            // Create arguments with self as the first argument
                            let mut method_args = vec![*object.clone()];
                            method_args.extend(args);

                            // Call the method through the VM
                            return self.call_function_fast(method.clone(), method_args, kwargs, frame_idx, result_reg);
                        } else {
                            // If not found in class_methods, try to find it in the MRO (Method Resolution Order)
                            // This is important for super() calls
                            for class_name in mro.get_linearization() {
                                // In a full implementation, we would look up the class and find its methods
                                // For now, we'll just continue searching
                            }

                            // If not found in MRO, try to call it as a method on the object
                            let mut method_args = vec![*object.clone()];
                            method_args.extend(args);
                            // We can't call call_method on a non-mutable reference, so we'll return an error
                            return Err(anyhow!("Method '{}' not found in class methods", method_name));
                        }
                    }
                    Value::Dict(_) | Value::List(_) | Value::Str(_) | Value::Set(_) | Value::Tuple(_) => {
                        // For builtin types, get the method and call it
                        if let Some(method) = object.as_ref().get_method(&method_name) {
                            // Create arguments: method_name, self, then the actual args
                            let mut method_args = vec![Value::Str(method_name.clone()), *object.clone()];
                            method_args.extend(args);

                            // Call the method
                            return self.call_function_fast(method, method_args, kwargs, frame_idx, result_reg);
                        } else {
                            return Err(anyhow!("Method '{}' not found for type '{}'", method_name, object.type_name()));
                        }
                    }
                    _ => return Err(anyhow!("Bound method called on non-object type '{}'", object.type_name()))
                }
            }
            #[cfg(feature = "ffi")]
            Value::Object { class_name, fields, .. } if class_name == "FFIFunction" => {
                // Handle FFI function calls
                // Check if this is an FFI callable
                if let Some(Value::Bool(true)) = fields.get("__ffi_callable__") {
                    // Extract library and function names
                    let library_name = match fields.get("__ffi_library__") {
                        Some(Value::Str(s)) => s.clone(),
                        _ => return Err(anyhow!("FFI function missing library name")),
                    };

                    let function_name = match fields.get("__ffi_function__") {
                        Some(Value::Str(s)) => s.clone(),
                        _ => return Err(anyhow!("FFI function missing function name")),
                    };

                    // Call the FFI function through the global manager
                    let manager = crate::builtins::GLOBAL_FFI_MANAGER.lock().unwrap();
                    let result = manager.call_external_function(&library_name, &function_name, args)?;
                    Ok(result)
                } else {
                    Err(anyhow!("Object is not callable"))
                }
            }
            #[cfg(feature = "ffi")]
            Value::ExternFunction { library_name, name, .. } => {
                // Call the FFI function through the global manager
                let manager = crate::builtins::GLOBAL_FFI_MANAGER.lock().unwrap();
                let result = manager.call_external_function(&library_name, &name, args.clone())?;
                Ok(result)
            }
            _ => {
                Err(anyhow!("'{}' object is not callable", func_value.type_name()))
            }
        }
    }
    
    /// Validate that a value matches the expected type
    #[cfg(feature = "type_checking")]
    fn validate_type(&self, value: &Value, expected_type: &crate::ast::Type) -> Result<()> {
        if !value.matches_type(expected_type) {
            Err(anyhow!("TypeError: Expected type '{}', got '{}'", expected_type, value.type_name()))
        } else {
            Ok(())
        }
    }
    
    /// Process starred arguments (*args, **kwargs) in function calls
    fn process_starred_arguments(&self, args: Vec<Value>) -> Result<Vec<Value>> {
        let mut processed_args = Vec::new();

        for arg in args {
            match arg {
                Value::Starred(value) => {
                    // Handle *args - unpack the iterable
                    match *value {
                        Value::List(list) => {
                            // Add each item in the list as a separate argument
                            for item in list.as_vec() {
                                processed_args.push(item.clone());
                            }
                        }
                        Value::Tuple(items) => {
                            // Add each item in the tuple as a separate argument
                            for item in items {
                                processed_args.push(item);
                            }
                        }
                        _ => {
                            return Err(anyhow!("Value after * must be iterable, got {}", value.type_name()));
                        }
                    }
                }
                _ => {
                    // Regular argument
                    processed_args.push(arg);
                }
            }
        }

        Ok(processed_args)
    }

    /// Implement eval() - evaluate a Python expression
    pub fn eval_impl(&mut self, source: &str, globals: Option<HashMap<String, Value>>, _locals: Option<HashMap<String, Value>>) -> Result<Value> {
        // Wrap the source in a return statement to capture the expression value
        let wrapped_source = format!("__eval_result__ = ({})", source);

        // Parse the wrapped expression
        let tokens = crate::lexer::Lexer::new(&wrapped_source)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in eval(): {}", e))?;

        let mut parser = crate::parser::Parser::new(tokens);
        let program = parser.parse()
            .map_err(|e| anyhow!("Parser error in eval(): {}", e))?;

        // Set up the evaluation context
        if let Some(g) = globals {
            // Temporarily replace globals
            let old_globals = self.globals.clone();
            *self.globals.borrow_mut() = g.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();

            // Compile and execute
            let mut compiler = crate::bytecode::compiler::SuperCompiler::new("<eval>".to_string());
            let code_object = compiler.compile(program)
                .map_err(|e| anyhow!("Compiler error in eval(): {}", e))?;

            // Execute
            self.execute(code_object)?;

            // Get the result from globals
            let result = self.globals.borrow().get("__eval_result__")
                .map(|v| v.value.clone())
                .unwrap_or(Value::None);

            // Clean up the temporary variable
            self.globals.borrow_mut().remove("__eval_result__");

            // Restore old globals
            *self.globals.borrow_mut() = old_globals.borrow().clone();

            Ok(result)
        } else {
            // Use current globals
            let mut compiler = crate::bytecode::compiler::SuperCompiler::new("<eval>".to_string());
            let code_object = compiler.compile(program)
                .map_err(|e| anyhow!("Compiler error in eval(): {}", e))?;

            // Execute
            self.execute(code_object)?;

            // Get the result from globals
            let result = self.globals.borrow().get("__eval_result__")
                .map(|v| v.value.clone())
                .unwrap_or(Value::None);

            // Clean up the temporary variable
            self.globals.borrow_mut().remove("__eval_result__");

            Ok(result)
        }
    }

    /// Implement exec() - execute Python statements
    pub fn exec_impl(&mut self, source: &str, globals: Option<HashMap<String, Value>>, locals: Option<HashMap<String, Value>>) -> Result<Value> {
        // Parse the source as statements
        let tokens = crate::lexer::Lexer::new(source)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in exec(): {}", e))?;

        let mut parser = crate::parser::Parser::new(tokens);
        let program = parser.parse()
            .map_err(|e| anyhow!("Parser error in exec(): {}", e))?;

        // Set up the execution context
        if let Some(g) = globals {
            // Temporarily replace globals
            let old_globals = self.globals.clone();
            *self.globals.borrow_mut() = g.into_iter().map(|(k, v)| (k, RcValue::new(v))).collect();

            // Compile and execute
            let mut compiler = crate::bytecode::compiler::SuperCompiler::new("<exec>".to_string());
            let code_object = compiler.compile(program)
                .map_err(|e| anyhow!("Compiler error in exec(): {}", e))?;

            let result = self.execute(code_object);

            // Restore old globals
            *self.globals.borrow_mut() = old_globals.borrow().clone();

            result
        } else {
            // Use current globals
            let mut compiler = crate::bytecode::compiler::SuperCompiler::new("<exec>".to_string());
            let code_object = compiler.compile(program)
                .map_err(|e| anyhow!("Compiler error in exec(): {}", e))?;

            self.execute(code_object)
        }
    }

    /// Implement compile() - compile source to a code object
    pub fn compile_impl(&mut self, source: &str, filename: &str, mode: &str) -> Result<Value> {
        // For eval mode, wrap the source to capture the result
        let wrapped_source;
        let source_to_parse = if mode == "eval" {
            wrapped_source = format!("__eval_result__ = ({})", source);
            &wrapped_source
        } else {
            source
        };

        // Parse the source based on mode
        let tokens = crate::lexer::Lexer::new(source_to_parse)
            .collect::<Result<Vec<_>, _>>()
            .map_err(|e| anyhow!("Lexer error in compile(): {}", e))?;

        let mut parser = crate::parser::Parser::new(tokens);

        let program = match mode {
            "eval" => {
                // Parse the wrapped assignment
                parser.parse()
                    .map_err(|e| anyhow!("Parser error in compile(): {}", e))?
            }
            "exec" | "single" => {
                // Parse as statements
                parser.parse()
                    .map_err(|e| anyhow!("Parser error in compile(): {}", e))?
            }
            _ => {
                return Err(anyhow!("compile() mode must be 'eval', 'exec', or 'single'"));
            }
        };

        // Compile to bytecode
        let mut compiler = crate::bytecode::compiler::SuperCompiler::new(filename.to_string());
        let code_object = compiler.compile(program)
            .map_err(|e| anyhow!("Compiler error in compile(): {}", e))?;

        // Return the code object as a Value
        Ok(Value::Code(Box::new(code_object)))
    }
}
