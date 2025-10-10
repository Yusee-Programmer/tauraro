//! Virtual machine implementation


use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::{OpCode, Instruction};
use crate::bytecode::objects::{RcValue, RangeIterator};
use crate::bytecode::memory::{CodeObject, Frame, InlineCache, MethodCache, Block, BlockType};
use crate::bytecode::arithmetic; // Import the arithmetic module
use crate::ast::{Param, Literal, Expr}; // Import necessary types for Closure handling
use anyhow::{Result, anyhow};
use std::collections::HashMap;

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
        // For now, we'll just return an error
        // In a complete implementation, this would dispatch to the appropriate handler
        match opcode {
            // Delegate function-related opcodes to the functions module
            OpCode::CallFunction | OpCode::CallMethod | OpCode::ReturnValue => {
                self.execute_function_op(frame_idx, opcode, arg1, arg2, arg3)
            }
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
                        // Try to convert values to strings if they're not already
                        let left_val = left.value.clone();
                        let right_val = right.value.clone();
                        
                        match (&left_val, &right_val) {
                            // If either is a string, convert both to strings
                            (Value::Str(_), _) | (_, Value::Str(_)) => {
                                let left_str = match left_val {
                                    Value::Str(s) => s,
                                    _ => format!("{}", left_val),
                                };
                                let right_str = match right_val {
                                    Value::Str(s) => s,
                                    _ => format!("{}", right_val),
                                };
                                let mut s = String::with_capacity(left_str.len() + right_str.len());
                                s.push_str(&left_str);
                                s.push_str(&right_str);
                                Value::Str(s)
                            },
                            // Otherwise, use the general arithmetic implementation
                            _ => {
                                self.add_values(left_val, right_val)
                                    .map_err(|e| anyhow!("Error in BinaryAddRR: {}", e))?
                            }
                        }
                    }
                };
                
                self.frames[frame_idx].registers[result_reg as usize] = RcValue::new(result);
                Ok(None)
            }
            OpCode::StoreGlobal => {
                // Store value from register to global namespace
                let name_idx = arg1 as usize;
                let value_reg = arg2;
                
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("StoreGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                if value_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreGlobal: register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                // Clone the values first to avoid borrowing issues
                let name = self.frames[frame_idx].code.names[name_idx].clone();
                let value = self.frames[frame_idx].registers[value_reg as usize].clone();
                
                // Store in both frame globals and VM globals
                self.frames[frame_idx].globals.insert(name.clone(), value.clone());
                self.globals.insert(name, value);
                
                Ok(None)
            }
            OpCode::LoadGlobal => {
                // Load value from global namespace to register
                let name_idx = arg1 as usize;
                let cache_idx = arg2 as usize; // Not used in this simple implementation
                let result_reg = arg3;
                
                if name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadGlobal: name index {} out of bounds (len: {})", name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                let name = &self.frames[frame_idx].code.names[name_idx];
                
                // Try to get from frame globals first, then VM globals
                let value = if let Some(value) = self.frames[frame_idx].globals.get(name) {
                    value.clone()
                } else if let Some(value) = self.globals.get(name) {
                    value.clone()
                } else {
                    return Err(anyhow!("LoadGlobal: name '{}' not found in global namespace", name));
                };
                
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::LoadFast => {
                // Load value from local variable (fast access) to register
                let local_idx = arg1 as usize;
                let result_reg = arg2;
                
                if local_idx >= self.frames[frame_idx].locals.len() {
                    return Err(anyhow!("LoadFast: local index {} out of bounds (len: {})", local_idx, self.frames[frame_idx].locals.len()));
                }
                
                let value = self.frames[frame_idx].locals[local_idx].clone();
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::LoadLocal => {
                // Load value from local register (same as LoadFast)
                let local_idx = arg1 as usize;
                let result_reg = arg2;
                
                if local_idx >= self.frames[frame_idx].locals.len() {
                    return Err(anyhow!("LoadLocal: local index {} out of bounds (len: {})", local_idx, self.frames[frame_idx].locals.len()));
                }
                
                let value = self.frames[frame_idx].locals[local_idx].clone();
                self.frames[frame_idx].set_register(result_reg, value);
                Ok(None)
            }
            OpCode::StoreFast => {
                // Store value from register to local variable (fast access)
                let local_idx = arg1 as usize;
                let value_reg = arg2;
                
                if local_idx >= self.frames[frame_idx].locals.len() {
                    return Err(anyhow!("StoreFast: local index {} out of bounds (len: {})", local_idx, self.frames[frame_idx].locals.len()));
                }
                
                if value_reg as usize >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("StoreFast: register index {} out of bounds (len: {})", value_reg, self.frames[frame_idx].registers.len()));
                }
                
                let value = self.frames[frame_idx].registers[value_reg as usize].clone();
                self.frames[frame_idx].locals[local_idx] = value;
                Ok(None)
            }
            OpCode::FastIntAdd => {
                // Ultra-fast integer addition without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning for maximum performance
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
                // Fallback to regular addition using the arithmetic module
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.add_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in FastIntAdd: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::FastIntSub => {
                // Ultra-fast integer subtraction without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning for maximum performance
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
                // Fallback to regular subtraction using the arithmetic module
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.sub_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in FastIntSub: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
                Ok(None)
            }
            OpCode::FastIntMul => {
                // Ultra-fast integer multiplication without cloning
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                // Direct access to integer values without cloning for maximum performance
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
                // Fallback to regular multiplication using the arithmetic module
                let left_val = self.frames[frame_idx].registers[left_reg].value.clone();
                let right_val = self.frames[frame_idx].registers[right_reg].value.clone();
                let result = self.mul_values(left_val, right_val)
                    .map_err(|e| anyhow!("Error in FastIntMul: {}", e))?;
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
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
            _ => {
                // For unimplemented opcodes, return an error
                Err(anyhow!("Unimplemented opcode: {:?}", opcode))
            }
        }
    }
    

    
    /// Optimized function call path
    pub fn call_function_fast(&mut self, func: Value, args: Vec<Value>, caller_frame_idx: Option<usize>, result_reg: Option<u32>) -> Result<Value> {
        match func {
            Value::BuiltinFunction(_, fptr) => {
                // Direct call to builtin function - fastest path
                (fptr)(args)
            }
            Value::Closure { name, params, body, captured_scope, docstring, compiled_code } => {
                // Handle user-defined functions (closures)
                if let Some(code_obj) = compiled_code {
                    // Create a new frame for the function execution
                    let mut locals = HashMap::new();
                    
                    // Set up function parameters
                    for (i, param) in params.iter().enumerate() {
                        if i < args.len() {
                            locals.insert(param.name.clone(), args[i].clone());
                        } else if let Some(default_expr) = &param.default {
                            // Evaluate default expression
                            match default_expr {
                                Expr::Literal(lit) => {
                                    match lit {
                                        Literal::Int(n) => {
                                            locals.insert(param.name.clone(), Value::Int(*n));
                                        }
                                        Literal::Float(n) => {
                                            locals.insert(param.name.clone(), Value::Float(*n));
                                        }
                                        Literal::String(s) => {
                                            locals.insert(param.name.clone(), Value::Str(s.clone()));
                                        }
                                        Literal::Bool(b) => {
                                            locals.insert(param.name.clone(), Value::Bool(*b));
                                        }
                                        Literal::None => {
                                            locals.insert(param.name.clone(), Value::None);
                                        }
                                        Literal::Bytes(b) => {
                                            locals.insert(param.name.clone(), Value::Bytes(b.clone()));
                                        }
                                        Literal::Complex { real, imag } => {
                                            locals.insert(param.name.clone(), Value::Complex { real: *real, imag: *imag });
                                        }
                                        Literal::Ellipsis => {
                                            locals.insert(param.name.clone(), Value::Ellipsis);
                                        }
                                    }
                                }
                                _ => {
                                    // For complex expressions, we'd need to evaluate them
                                    // For now, just use None as a fallback
                                    locals.insert(param.name.clone(), Value::None);
                                }
                            }
                        } else {
                            // No default value and no argument provided
                            locals.insert(param.name.clone(), Value::None);
                        }
                    }
                    
                    // Add captured scope variables to locals
                    for (var_name, var_value) in captured_scope {
                        locals.insert(var_name, var_value);
                    }
                    
                    // Get globals from the caller frame if available
                    let globals: HashMap<String, Value> = if let Some(frame_idx) = caller_frame_idx {
                        if frame_idx < self.frames.len() {
                            self.frames[frame_idx].globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()
                        } else {
                            HashMap::new()
                        }
                    } else {
                        HashMap::new()
                    };
                    
                    // Get builtins from the caller frame if available
                    let builtins: HashMap<String, Value> = if let Some(frame_idx) = caller_frame_idx {
                        if frame_idx < self.frames.len() {
                            self.frames[frame_idx].builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect()
                        } else {
                            HashMap::new()
                        }
                    } else {
                        HashMap::new()
                    };
                    
                    // Create new frame using the function frame constructor
                    let mut frame = Frame::new_function_frame(*code_obj, globals, builtins, vec![]);
                    
                    // Set up the locals in the frame
                    for (var_name, var_value) in locals {
                        if let Some(index) = frame.get_local_index(&var_name) {
                            frame.locals[index] = RcValue::new(var_value);
                        }
                    }
                    
                    // Set the return register so the return value can be stored in the caller frame
                    if let Some(caller_idx) = caller_frame_idx {
                        if let Some(result_reg_idx) = result_reg {
                            frame.return_register = Some((caller_idx, result_reg_idx));
                        }
                    }
                    
                    self.frames.push(frame);
                    // Return Ok(None) to indicate that execution should continue in the new frame
                    Ok(Value::None)
                } else {
                    Err(anyhow!("Closure '{}' has no compiled code", name))
                }
            }
            _ => Err(anyhow!("Function call not fully implemented for type: {:?}", func)),
        }
    }
    
    /// Get attribute implementation
    pub fn getattr_impl(&self, obj: &Value, attr: &str) -> Result<Value> {
        // Simple implementation for now
        Err(anyhow!("getattr not fully implemented"))
    }
    

}