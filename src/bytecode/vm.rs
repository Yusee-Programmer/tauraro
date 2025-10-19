//! Virtual machine implementation

use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::OpCode;
use crate::bytecode::objects::RcValue;
use crate::bytecode::memory::{CodeObject, Frame, Block, BlockType};
// Import the arithmetic module
use crate::bytecode::arithmetic;
// Import necessary types for Closure handling
use anyhow::{Result, anyhow};
use std::collections::HashMap;
use std::rc::Rc;

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
        // Convert globals and builtins from RcValue to Value for Frame::new
        let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
        let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();

        let frame = Frame::new(code, globals_values, builtins_values);
        self.frames.push(frame);

        let result = self.run_frame()?;
        
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
                return Ok(Value::None);
            }
            
            // Update frame index in case frames were added/removed
            frame_idx = self.frames.len() - 1;
            
            // Safety check: if there are no instructions, return None immediately
            if self.frames[frame_idx].code.instructions.is_empty() {
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
                    println!("DEBUG: Function returned value: {:?}", value);
                    // A function has returned a value, pop the current frame
                    if let Some(returned_frame) = self.frames.pop() {
                        println!("DEBUG: Popped frame: {}", returned_frame.code.name);
                        // Check if this is an __init__ frame by looking at the function name
                        let is_init_frame = returned_frame.code.name == "__init__" || returned_frame.code.name == "<fn:__init__>";
                        println!("DEBUG: is_init_frame: {}", is_init_frame);

                        // If this is an __init__ frame, we want to return the instance instead of None
                        let return_value = if is_init_frame {
                            // For __init__ methods, we should return the instance that was passed as self
                            // The instance should be in the first local variable (self parameter)
                            if !returned_frame.locals.is_empty() {
                                returned_frame.locals[0].value.clone()
                            } else {
                                value // Fallback to the actual return value
                            }
                        } else {
                            // For regular functions, return the actual return value
                            println!("DEBUG: Returning actual value from {}: {:?}", returned_frame.code.name, value);
                            value
                        };
                        
                        // If there are no more frames, return the value
                        if self.frames.is_empty() {
                            return Ok(return_value);
                        }
                        
                        // Update frame index to point to the calling frame
                        frame_idx = self.frames.len() - 1;
                        
                        // Update globals from the returned frame
                        self.globals = returned_frame.globals;
                        
                        // Store the return value in the calling frame if return_register is set
                        println!("DEBUG: returned_frame.return_register: {:?}", returned_frame.return_register);
                        if let Some((caller_frame_idx, result_reg)) = returned_frame.return_register {
                            println!("DEBUG: Storing return value {:?} in caller frame {} register {}", return_value, caller_frame_idx, result_reg);
                            // Make sure the caller frame index is valid
                            if caller_frame_idx < self.frames.len() {
                                self.frames[caller_frame_idx].set_register(result_reg, RcValue::new(return_value.clone()));
                                println!("DEBUG: Stored return value successfully");
                            } else {
                                println!("DEBUG: ERROR: caller_frame_idx {} >= frames.len() {}", caller_frame_idx, self.frames.len());
                            }
                        } else {
                            println!("DEBUG: No return_register set for frame {}", returned_frame.code.name);
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
                    // Handle the exception
                    if let Some(handler_pos) = handler_pos_opt {
                        // Unwind the stack to the handler position
                        self.frames[frame_idx].pc = handler_pos;
                        // Push the exception value onto the stack
                        if let Some(top_exc) = top_exc {
                            self.frames[frame_idx].registers.push(top_exc);
                        }
                    } else {
                        // No handler found, propagate the exception
                        return Err(e);
                    }
                }
            }
        }
    }

    /// Execute a single instruction (placeholder implementation)
    fn execute_instruction(&mut self, _opcode: OpCode, _arg1: u32, _arg2: u32, _arg3: u32, _frame_idx: usize, _frame_len: usize) -> Result<Option<Value>> {
        // This is a placeholder implementation - in a real VM, this would dispatch to the appropriate handler
        Err(anyhow!("execute_instruction not implemented"))
    }
    
    /// Optimized instruction execution with computed GOTOs for maximum performance
    #[inline(always)]
    fn execute_instruction_fast(&mut self, frame_idx: usize, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32) -> Result<Option<Value>> {
        println!("DEBUG: Executing opcode: {:?}", opcode);
        // For now, we'll just return an error
        // In a complete implementation, this would dispatch to the appropriate handler
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
                                let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                                let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                                Frame::new_function_frame(*code, globals_values, builtins_values, vec![], HashMap::new())
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
                let _cache_idx = arg2 as usize; // Not used in this simple implementation
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
                // Load value from register to register
                let source_reg = arg1 as usize;
                let result_reg = arg2;
                
                if source_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadLocal: register index {} out of bounds (len: {})", source_reg, self.frames[frame_idx].registers.len()));
                }
                
                let value = self.frames[frame_idx].registers[source_reg].clone();
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
            OpCode::CallFunction => {
                // Function call: arg1 = function register, arg2 = argument count, arg3 = result register
                let func_reg = arg1 as usize;
                let arg_count = arg2 as usize;
                let result_reg = arg3 as usize;

                println!("DEBUG: CallFunction - func_reg={}, arg_count={}, result_reg={}, frame_idx={}", func_reg, arg_count, result_reg, frame_idx);

                if func_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunction: function register index {} out of bounds (len: {})", func_reg, self.frames[frame_idx].registers.len()));
                }

                // Get the function value
                let func_value = self.frames[frame_idx].registers[func_reg].value.clone();
                println!("DEBUG: CallFunction - func_value type: {}", func_value.type_name());
                // Debug info removed
                
                // Collect arguments from registers
                let mut args = Vec::new();
                for i in 0..arg_count {
                    // Arguments are stored in consecutive registers after the function register
                    let arg_reg = func_reg + 1 + i;
                    if arg_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("CallFunction: argument register index {} out of bounds (len: {})", arg_reg, self.frames[frame_idx].registers.len()));
                    }
                    let arg_value = self.frames[frame_idx].registers[arg_reg].value.clone();
                    // Debug info removed
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
                            Value::Closure { name: _, params: _, body: _, captured_scope: _, docstring: _, compiled_code } => {
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
                println!("DEBUG: CallFunction - calling call_function_fast with frame_idx=Some({}), result_reg=Some({})", frame_idx, result_reg);
                let result = self.call_function_fast(func_value, processed_args, kwargs, Some(frame_idx), Some(result_reg as u32))?;

                println!("DEBUG: CallFunction - call_function_fast returned: {:?}", result);
                // If the function returned a value directly, store it in the result register
                if !matches!(result, Value::None) {
                    println!("DEBUG: CallFunction - storing non-None result in register {}", result_reg);
                    self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(result));
                } else {
                    println!("DEBUG: CallFunction - result was None, expecting frame return_register to handle it");
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
            OpCode::BuildDict => {
                // Build a dict from key-value pairs on the stack/register
                // Keys and values are interleaved: key1, value1, key2, value2, ...
                let pair_count = arg1 as usize;
                let first_key_reg = arg2 as usize;
                let result_reg = arg3 as u32;
                
                // Create a new dict
                let mut dict = std::collections::HashMap::new();
                
                // Get key-value pairs from consecutive registers starting from first_key_reg
                // Keys and values are interleaved
                for i in 0..pair_count {
                    let key_reg = first_key_reg + (i * 2);
                    let value_reg = first_key_reg + (i * 2) + 1;
                    
                    if key_reg >= self.frames[frame_idx].registers.len() || value_reg >= self.frames[frame_idx].registers.len() {
                        return Err(anyhow!("BuildDict: register index out of bounds (len: {})", self.frames[frame_idx].registers.len()));
                    }
                    
                    // Keys must be strings in our implementation
                    let key_value = &self.frames[frame_idx].registers[key_reg].value;
                    let value_value = &self.frames[frame_idx].registers[value_reg].value;
                    
                    match key_value {
                        Value::Str(key_str) => {
                            dict.insert(key_str.clone(), value_value.clone());
                        },
                        _ => {
                            return Err(anyhow!("BuildDict: dictionary keys must be strings, got {}", key_value.type_name()));
                        }
                    }
                }
                
                let dict_value = Value::Dict(dict);
                self.frames[frame_idx].set_register(result_reg as u32, RcValue::new(dict_value));
                Ok(None)
            }
            OpCode::BinaryDivRR => {
                // Register-Register division
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryDivRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
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
            OpCode::BinaryDivIR => {
                // Immediate-Register division
                let left_imm = arg1 as usize; // Immediate value index in constants
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_imm >= self.frames[frame_idx].code.constants.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryDivIR: constant or register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].code.constants[left_imm];
                let right = &self.frames[frame_idx].registers[right_reg];
                
                // Fast path for common operations
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
                        Value::Float(a / b)
                    },
                    _ => {
                        // For less common cases, use the general implementation
                        self.div_values(left.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryDivIR: {}", e))?
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
                    _ => {
                        // For less common cases, use the general implementation
                        self.pow_values(left.value.clone(), right.value.clone())
                            .map_err(|e| anyhow!("Error in BinaryPowRR: {}", e))?
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
                    (Value::Dict(dict), key) => {
                        // For dictionaries, convert key to string for lookup
                        let key_str = match key {
                            Value::Str(s) => s.clone(),
                            Value::Int(n) => n.to_string(),
                            _ => format!("{}", key),
                        };
                        
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
                    Value::Dict(dict) => {
                        // For dictionaries, convert key to string for lookup
                        let key_str = match index_value {
                            Value::Str(s) => s,
                            Value::Int(n) => n.to_string(),
                            _ => format!("{}", index_value),
                        };
                        
                        dict.insert(key_str, value_to_store);
                    },
                    _ => {
                        return Err(anyhow!("Subscript assignment not supported for type {}", 
                                          self.frames[frame_idx].registers[object_reg].value.type_name()));
                    }
                };
                
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
                        println!("DEBUG: CallMethod - Super object, method_name={}", method_name);
                        
                        if let Some(instance_value) = instance {
                            // Look for the method in the parent class methods
                            if let Some(parent_methods_map) = parent_methods {
                                println!("DEBUG: CallMethod - Super object, parent_methods count: {}", parent_methods_map.len());
                                if let Some(method) = parent_methods_map.get(&method_name) {
                                    println!("DEBUG: CallMethod - Found method {} in parent_methods", method_name);
                                    // Create arguments with self as the first argument
                                    let mut method_args = vec![*instance_value.clone()];
                                    method_args.extend(args);
                                    
                                    // Call the method through the VM and capture the return value
                                    // Pass object_reg as the result register so the return value is stored correctly
                                    self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?
                                } else {
                                    println!("DEBUG: CallMethod - Method {} not found in parent_methods", method_name);
                                    // Print all available methods for debugging
                                    for (method_name, _) in parent_methods_map {
                                        println!("DEBUG: CallMethod - Available method: {}", method_name);
                                    }
                                    // If not found in parent_methods, return an error
                                    return Err(anyhow!("super(): method '{}' not found in parent class", method_name));
                                }
                            } else {
                                // If not found in parent_methods, return an error
                                return Err(anyhow!("super(): method '{}' not found in parent class", method_name));
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
                            _ => return Err(anyhow!("Bound method called on non-object type '{}'", object.as_ref().type_name())),
                        }
                    },
                    Value::Object { class_methods, .. } => {
                        // For regular objects, we need to handle method calls through the VM
                        // Extract the object and find the method in class_methods
                        if let Some(method) = class_methods.get(&method_name) {
                            // Create arguments with self as the first argument
                            let mut method_args = vec![self.frames[frame_idx].registers[object_reg].value.clone()];
                            method_args.extend(args.clone());
                            
                            // Call the method through the VM and capture the return value
                            // Pass object_reg as the result register so the return value is stored correctly
                            let method_result = self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?;
                            method_result
                        } else {
                            return Err(anyhow!("Method '{}' not found in class methods", method_name));
                        }
                    },
                    Value::Class { name, methods, .. } => {
                        // For Class objects, we need to handle method calls by looking up the method in the class
                        println!("DEBUG: CallMethod - Class object, method_name={}", method_name);
                        println!("DEBUG: CallMethod - Class name: {}", name);
                        
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
                            
                            // Call the method through the VM
                            // Pass object_reg as the result register so the return value is stored correctly
                            self.call_function_fast(method.clone(), method_args, HashMap::new(), Some(frame_idx), Some(object_reg as u32))?
                        } else {
                            return Err(anyhow!("Method '{}' not found in class methods", method_name));
                        }
                    },
                    _ => {
                        // For builtin types, try to get method and call it
                        if let Some(method) = object_value.get_method(&method_name) {
                            // Create arguments with self as the first argument
                            let mut method_args = vec![self.frames[frame_idx].registers[object_reg].value.clone()];
                            method_args.extend(args.clone());
                            
                            // Call the method
                            match method {
                                Value::BuiltinFunction(_, func) => func(method_args)?,
                                Value::NativeFunction(func) => func(method_args)?,
                                _ => return Err(anyhow!("Method '{}' cannot be called directly", method_name)),
                            }
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), method_name));
                        }
                    }
                };

                // Store the result back in the object register (this is where the VM expects it)
                // IMPORTANT: If result_value is None and the object may have been modified by the method,
                // preserve the current object_reg value instead of overwriting with None
                if matches!(result_value, Value::None) {
                    // Method returned None - the object_reg may have been updated by StoreAttr during method execution
                    // Don't overwrite it with None; keep the potentially modified object
                    println!("DEBUG: CallMethod - Method returned None, preserving object_reg");
                } else {
                    // Method returned an actual value - store it
                    println!("DEBUG: CallMethod - Method returned value, storing in object_reg");
                    self.frames[frame_idx].registers[object_reg] = RcValue::new(result_value);
                }
                Ok(None)
            }
            OpCode::BinaryMulRR => {
                // Register-Register multiplication
                let left_reg = arg1 as usize;
                let right_reg = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if left_reg >= self.frames[frame_idx].registers.len() || right_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("BinaryMulRR: register index out of bounds"));
                }
                
                let left = &self.frames[frame_idx].registers[left_reg];
                let right = &self.frames[frame_idx].registers[right_reg];
                
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
                
                self.frames[frame_idx].registers[result_reg] = RcValue::new(result);
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
                        let kwargs_marker = Value::KwargsMarker(dict.clone());
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
                
                eprintln!("DEBUG: Setting up exception handler at PC {} with stack level {}", handler_pc, stack_level);
                
                // Push an exception handler block onto the block stack
                let except_block = Block {
                    block_type: BlockType::Except,
                    handler: handler_pc,
                    level: stack_level,
                };
                self.frames[frame_idx].block_stack.push(except_block);
                eprintln!("DEBUG: Block stack now has {} entries", self.frames[frame_idx].block_stack.len());
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
                        self.frames[frame_idx].globals.insert(var_name, exception_value);
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
                
                // Debug: Print block stack information
                eprintln!("DEBUG: Block stack has {} entries", self.frames[frame_idx].block_stack.len());
                for (i, block) in self.frames[frame_idx].block_stack.iter().enumerate() {
                    eprintln!("DEBUG: Block {}: {:?} at handler PC {}", i, block.block_type, block.handler);
                }
                
                // Search for exception handlers in the block stack
                // Find the innermost exception handler
                let except_block_idx_opt = self.frames[frame_idx].block_stack.iter().rposition(|b| matches!(b.block_type, BlockType::Except));
                
                if let Some(except_block_idx) = except_block_idx_opt {
                    eprintln!("DEBUG: Found exception handler at index {}", except_block_idx);
                    // Get the exception handler block
                    let handler_pc = self.frames[frame_idx].block_stack[except_block_idx].handler;
                    eprintln!("DEBUG: Jumping to exception handler at PC {}", handler_pc);
                    // Remove the exception handler block from the stack (it's been used)
                    self.frames[frame_idx].block_stack.remove(except_block_idx);
                    // Jump to the exception handler
                    self.frames[frame_idx].pc = handler_pc;
                    // Push the exception value onto the stack for the handler to access
                    self.frames[frame_idx].registers.push(RcValue::new(exception_value));
                    Ok(None) // Continue execution, don't return an error
                } else {
                    eprintln!("DEBUG: No exception handler found");
                    // No exception handler found, print the exception and stop execution
                    eprintln!("{}", exception_value);
                    Err(anyhow!("Unhandled exception: {}", exception_value))
                }
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
                        if dict.len() >= key_count {
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
                        if dict.len() >= pair_count {
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
                println!("DEBUG: LoadAttr opcode called");
                // Load attribute from object (obj.attr)
                let object_reg = arg1 as usize;
                let attr_name_idx = arg2 as usize;
                let result_reg = arg3 as usize;
                
                if object_reg >= self.frames[frame_idx].registers.len() {
                    return Err(anyhow!("LoadAttr: object register index {} out of bounds (len: {})", object_reg, self.frames[frame_idx].registers.len()));
                }
                
                if attr_name_idx >= self.frames[frame_idx].code.names.len() {
                    return Err(anyhow!("LoadAttr: attribute name index {} out of bounds (len: {})", attr_name_idx, self.frames[frame_idx].code.names.len()));
                }
                
                println!("DEBUG: LoadAttr - object_reg={}, attr_name_idx={}, result_reg={}", object_reg, attr_name_idx, result_reg);
                println!("DEBUG: LoadAttr - frame registers len: {}", self.frames[frame_idx].registers.len());
                println!("DEBUG: LoadAttr - frame names len: {}", self.frames[frame_idx].code.names.len());
                // Print object ID to track if we're working with the same object
                println!("DEBUG: LoadAttr - object value ID: {:p}", &self.frames[frame_idx].registers[object_reg].value);
                
                // Clone values to avoid borrowing issues
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let attr_name = self.frames[frame_idx].code.names[attr_name_idx].clone();
                
                println!("DEBUG: LoadAttr - object_value: {:?}, attr_name: {}", object_value, attr_name);
                
                // Try to get the attribute from the object
                println!("DEBUG: LoadAttr - object_value type: {}", object_value.type_name());
                println!("DEBUG: LoadAttr - checking if object_value is Super");
                println!("DEBUG: LoadAttr - object_value debug: {:?}", object_value);
                println!("DEBUG: LoadAttr - object_value variant: {:?}", std::mem::discriminant(&object_value));
                println!("DEBUG: LoadAttr - object_value is Super: {}", matches!(object_value, Value::Super(_, _, _, _)));
                
                // Additional debug to check the actual type
                if let Value::Super(current_class, parent_class, instance, parent_methods) = &object_value {
                    println!("DEBUG: LoadAttr - Confirmed Super object: current_class={}, parent_class={}", current_class, parent_class);
                    if let Some(instance_val) = instance {
                        println!("DEBUG: LoadAttr - Super has instance of type: {}", instance_val.type_name());
                    } else {
                        println!("DEBUG: LoadAttr - Super has no instance");
                    }
                    if let Some(methods) = parent_methods {
                        println!("DEBUG: LoadAttr - Super has {} parent methods", methods.len());
                        for (method_name, _) in methods {
                            println!("DEBUG: LoadAttr - Parent method: {}", method_name);
                        }
                    } else {
                        println!("DEBUG: LoadAttr - Super has no parent methods");
                    }
                } else {
                    println!("DEBUG: LoadAttr - object_value is NOT a Super object");
                    // Let's check what variant it actually is
                    match &object_value {
                        Value::Int(_) => println!("DEBUG: LoadAttr - object_value is Int"),
                        Value::Float(_) => println!("DEBUG: LoadAttr - object_value is Float"),
                        Value::Bool(_) => println!("DEBUG: LoadAttr - object_value is Bool"),
                        Value::Str(_) => println!("DEBUG: LoadAttr - object_value is Str"),
                        Value::List(_) => println!("DEBUG: LoadAttr - object_value is List"),
                        Value::Dict(_) => println!("DEBUG: LoadAttr - object_value is Dict"),
                        Value::Tuple(_) => println!("DEBUG: LoadAttr - object_value is Tuple"),
                        Value::Object { .. } => println!("DEBUG: LoadAttr - object_value is Object"),
                        Value::Class { .. } => println!("DEBUG: LoadAttr - object_value is Class"),
                        Value::Super(_, _, _, _) => println!("DEBUG: LoadAttr - object_value is Super (this should have matched above!)"),
                        Value::Closure { .. } => println!("DEBUG: LoadAttr - object_value is Closure"),
                        Value::BuiltinFunction(_, _) => println!("DEBUG: LoadAttr - object_value is BuiltinFunction"),
                        Value::NativeFunction(_) => println!("DEBUG: LoadAttr - object_value is NativeFunction"),
                        Value::Module(_, _) => println!("DEBUG: LoadAttr - object_value is Module"),
                        Value::BoundMethod { .. } => println!("DEBUG: LoadAttr - object_value is BoundMethod"),
                        Value::None => println!("DEBUG: LoadAttr - object_value is None"),
                        _ => println!("DEBUG: LoadAttr - object_value is some other variant"),
                    }
                }
                
                let result = match &object_value {
                    Value::Super(current_class, parent_class, instance, parent_methods) => {
                        // Handle super() object - delegate to parent class
                        println!("DEBUG: LoadAttr - Super object, current_class={}, attr_name={}", current_class, attr_name);
                        
                        if let Some(instance_value) = instance {
                            // Debug information
                            println!("DEBUG: Super object - current_class={}, parent_class={}, attr_name={}", current_class, parent_class, attr_name);
                            println!("DEBUG: Instance value type: {}", instance_value.type_name());
                            
                            // For super() objects, we need to look up the method in the parent class hierarchy
                            
                            // First, try to find the current class in globals to get its MRO
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            
                            println!("DEBUG: globals_values keys: {:?}", globals_values.keys().collect::<Vec<_>>());
                            
                            // Look for the current class in globals
                            println!("DEBUG: Looking for class {} in globals", current_class);
                            if let Some(class_value) = globals_values.get(current_class) {
                                println!("DEBUG: Found class_value for {}", current_class);
                                if let Value::Class { name, mro, .. } = class_value {
                                    println!("DEBUG: Class name={}, MRO={:?}", name, mro.get_linearization());
                                    
                                    // Use MRO to find the method in parent classes
                                    println!("DEBUG: Looking for method {} in MRO", attr_name);
                                    if let Some(method) = mro.find_method_in_mro(&attr_name, &globals_values) {
                                        println!("DEBUG: Found method {} in MRO", attr_name);
                                        // Found the method, create a BoundMethod
                                        let bound_method = Value::BoundMethod {
                                            object: instance_value.clone(),
                                            method_name: attr_name.clone(),
                                        };
                                        self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                        return Ok(None);
                                    } else {
                                        println!("DEBUG: Method {} not found in MRO", attr_name);
                                        println!("DEBUG: Will check parent_methods as fallback");
                                    }
                                }
                            } else {
                                println!("DEBUG: Class {} not found in globals", current_class);
                                println!("DEBUG: Will check parent_methods as fallback");
                            }
                            
                            // If not found through MRO, check parent_methods as fallback
                            if let Some(methods) = parent_methods {
                                println!("DEBUG: Checking parent_methods, methods count: {}", methods.len());
                                println!("DEBUG: Looking for method: {}", attr_name);
                                if let Some(method) = methods.get(&attr_name) {
                                    println!("DEBUG: Found method {} in parent_methods", attr_name);
                                    // Found the method in parent methods, create a BoundMethod
                                    let bound_method = Value::BoundMethod {
                                        object: instance_value.clone(),
                                        method_name: attr_name.clone(),
                                    };
                                    self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                    return Ok(None);
                                } else {
                                    println!("DEBUG: Method {} not found in parent_methods", attr_name);
                                    // Print all available methods for debugging
                                    for (method_name, _) in methods {
                                        println!("DEBUG: Available method: {}", method_name);
                                    }
                                    
                                    // If still not found, check if this is a special case
                                    // For methods that might not be in the class methods, try to find them in the instance
                                    if let Value::Object { class_methods, .. } = instance_value.as_ref() {
                                        println!("DEBUG: Checking instance class_methods, methods count: {}", class_methods.len());
                                        if let Some(method) = class_methods.get(&attr_name) {
                                            println!("DEBUG: Found method {} in instance class_methods", attr_name);
                                            // Found the method in the instance's class methods, create a BoundMethod
                                            let bound_method = Value::BoundMethod {
                                                object: instance_value.clone(),
                                                method_name: attr_name.clone(),
                                            };
                                            self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                            return Ok(None);
                                        } else {
                                            println!("DEBUG: Method {} not found in instance class_methods", attr_name);
                                        }
                                    }
                                    
                                    // If still not found, create a BoundMethod but it will fail at call time
                                    // This maintains compatibility with the existing approach
                                    println!("DEBUG: Creating BoundMethod for {} (will fail at call time)", attr_name);
                                    let bound_method = Value::BoundMethod {
                                        object: instance_value.clone(),
                                        method_name: attr_name.clone(),
                                    };
                                    self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                    return Ok(None);
                                }
                            } else {
                                println!("DEBUG: No parent_methods provided");
                                
                                // If no parent_methods provided, check if this is a special case
                                // For methods that might not be in the class methods, try to find them in the instance
                                if let Value::Object { class_methods, .. } = instance_value.as_ref() {
                                    println!("DEBUG: Checking instance class_methods, methods count: {}", class_methods.len());
                                    if let Some(method) = class_methods.get(&attr_name) {
                                        println!("DEBUG: Found method {} in instance class_methods", attr_name);
                                        // Found the method in the instance's class methods, create a BoundMethod
                                        let bound_method = Value::BoundMethod {
                                            object: instance_value.clone(),
                                            method_name: attr_name.clone(),
                                        };
                                        self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                        return Ok(None);
                                    } else {
                                        println!("DEBUG: Method {} not found in instance class_methods", attr_name);
                                    }
                                }
                                
                                // If still not found, create a BoundMethod but it will fail at call time
                                // This maintains compatibility with the existing approach
                                println!("DEBUG: Creating BoundMethod for {} (will fail at call time)", attr_name);
                                let bound_method = Value::BoundMethod {
                                    object: instance_value.clone(),
                                    method_name: attr_name.clone(),
                                };
                                self.frames[frame_idx].registers[result_reg] = RcValue::new(bound_method);
                                return Ok(None);
                            }
                        } else {
                            return Err(anyhow!("super(): unbound super object has no attribute '{}'", attr_name));
                        }
                    },
                    Value::Object { fields, class_methods, mro, .. } => {
                        println!("DEBUG: LoadAttr - Object has {} fields: {:?}", fields.len(), fields.as_ref().keys().collect::<Vec<_>>());
                        // First check fields
                        if let Some(value) = fields.as_ref().get(&attr_name) {
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
                        // Then check methods - return as BoundMethod so self is bound
                        else if let Some(_method) = class_methods.get(&attr_name) {
                            // Create a BoundMethod to bind self to the method
                            Value::BoundMethod {
                                object: Box::new(object_value.clone()),
                                method_name: attr_name.clone(),
                            }
                        }
                        // Then check MRO for inherited methods
                        else {
                            // Convert globals from RcValue to Value for MRO lookup
                            let globals_values: HashMap<String, Value> = self.globals
                                .iter()
                                .map(|(k, v)| (k.clone(), v.value.clone()))
                                .collect();
                            if let Some(method) = mro.find_method_in_mro(&attr_name, &globals_values) {
                                method.clone()
                            } else {
                                return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                            }
                        }
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
                                .iter()
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
                        // For dictionaries, treat keys as attributes
                        if let Some(value) = dict.get(&attr_name) {
                            value.clone()
                        } else {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_value.type_name(), attr_name));
                        }
                    },
                    _ => {
                        // For other objects, try to get method
                        println!("DEBUG: LoadAttr - default case, object_value type: {}", object_value.type_name());
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
                println!("DEBUG: StoreAttr opcode called");
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
                
                println!("DEBUG: StoreAttr - object_reg={}, attr_name_idx={}, value_reg={}", object_reg, attr_name_idx, value_reg);
                println!("DEBUG: StoreAttr - frame registers len: {}", self.frames[frame_idx].registers.len());
                println!("DEBUG: StoreAttr - frame names len: {}", self.frames[frame_idx].code.names.len());
                
                // Clone the values first to avoid borrowing issues
                let attr_name = self.frames[frame_idx].code.names[attr_name_idx].clone();
                let value_to_store = self.frames[frame_idx].registers[value_reg].value.clone();
                let object_value = self.frames[frame_idx].registers[object_reg].value.clone();
                let object_type_name = object_value.type_name();
                
                println!("DEBUG: StoreAttr - attr_name: {}, value_to_store: {:?}, object_type: {}", attr_name, value_to_store, object_type_name);
                println!("DEBUG: StoreAttr - object value ID: {:p}", &self.frames[frame_idx].registers[object_reg].value);
                // Check if we're in an __init__ frame
                if self.frames[frame_idx].code.name == "__init__" || self.frames[frame_idx].code.name == "<fn:__init__>" {
                    println!("DEBUG: StoreAttr called in __init__ frame");
                    // Check if this is the self parameter (locals[0])
                    if object_reg < self.frames[frame_idx].registers.len() {
                        // Get the self instance from locals[0]
                        if !self.frames[frame_idx].locals.is_empty() {
                            println!("DEBUG: self instance from locals[0]: {:?}", self.frames[frame_idx].locals[0].value);
                        }
                    }
                }
                
                // Check if we're dealing with an Object that might have descriptors
                let is_object_with_fields = matches!(object_value, Value::Object { .. });
                
                if is_object_with_fields {
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
                
                println!("DEBUG: StoreAttr - About to store value in object");
                // Normal assignment for all other cases
                match &mut self.frames[frame_idx].registers[object_reg].value {
                    Value::Object { fields, .. } => {
                        // Store in fields using Rc::make_mut to get a mutable reference
                        println!("DEBUG: StoreAttr - Storing {} = {:?} in object fields", attr_name, value_to_store);
                        Rc::make_mut(fields).insert(attr_name.clone(), value_to_store.clone());
                        println!("DEBUG: StoreAttr - Stored successfully");

                        // CRITICAL FIX: Update locals[0] (self) with the modified object
                        // This ensures that subsequent loads of 'self' see the updated fields
                        // This applies to ALL methods, not just __init__
                        if !self.frames[frame_idx].locals.is_empty() {
                            // Update locals[0] with the modified object from the register
                            let updated_object = self.frames[frame_idx].registers[object_reg].clone();
                            self.frames[frame_idx].locals[0] = updated_object;
                            println!("DEBUG: StoreAttr - Updated locals[0] (self) with modified object in {}", self.frames[frame_idx].code.name);
                        }

                        // For ALL methods (not just __init__), update the instance in the caller frame's register
                        // This ensures that when methods modify self, the changes are visible to the caller
                        if let Some((caller_frame_idx, result_reg)) = self.frames[frame_idx].return_register {
                            if caller_frame_idx < self.frames.len() {
                                // Update the instance in the caller frame's register with all modified fields
                                println!("DEBUG: StoreAttr - Updating caller frame {} register {} with modified instance", caller_frame_idx, result_reg);
                                // Clone the entire modified object from current frame to caller frame
                                let modified_object = self.frames[frame_idx].registers[object_reg].clone();
                                self.frames[caller_frame_idx].registers[result_reg as usize] = modified_object;
                                println!("DEBUG: StoreAttr - Successfully updated caller frame's object register");
                            }
                        }

                // Let's verify that the value was actually stored
                if let Value::Object { fields, .. } = &self.frames[frame_idx].registers[object_reg].value {
                    println!("DEBUG: StoreAttr - After storing, object now has {} fields: {:?}", fields.len(), fields.as_ref().keys().collect::<Vec<_>>());
                }
                    },
                    Value::Dict(dict) => {
                        // For dictionaries, treat keys as attributes
                        dict.insert(attr_name, value_to_store);
                    },
                    Value::Module(_, namespace) => {
                        // For modules, store in namespace
                        namespace.insert(attr_name, value_to_store);
                    },
                    _ => {
                        return Err(anyhow!("'{}' object does not support attribute assignment", object_type_name));
                    }
                };
                
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
                    // Get the current value of the field to check if it's a descriptor
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
                        if !dict.contains_key(&attr_name) {
                            return Err(anyhow!("'{}' object has no attribute '{}'", object_type_name, attr_name));
                        }
                        dict.remove(&attr_name);
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
            _ => {
                // For unimplemented opcodes, we'll just return an error
                // In a complete implementation, we would handle all opcodes
                Err(anyhow!("Unimplemented opcode: {:?}", opcode))
            }
        }
    }
    
    /// Call a function with optimized fast path
    fn call_function_fast(&mut self, func_value: Value, args: Vec<Value>, kwargs: HashMap<String, Value>, frame_idx: Option<usize>, result_reg: Option<u32>) -> Result<Value> {
        match func_value {
            Value::BuiltinFunction(_, func) => {
                // For builtin functions, we should not pass kwargs as they don't expect them
                // Concatenate args and kwargs values if needed, or handle them appropriately
                // For now, let's just pass the args to builtin functions
                func(args.clone())
            }
            Value::NativeFunction(func) => {
                // Call native function directly
                func(args.clone())
            }
            Value::Closure { name, params, body: _, captured_scope: _, docstring: _, compiled_code } => {
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
                        println!("DEBUG: Closure - creating frame for function {}, frame_idx={:?}, result_reg={:?}", name, frame_idx, result_reg);
                        let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                        let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();

                        let mut frame = Frame::new_function_frame(*code_obj, globals_values, builtins_values, args, kwargs);

                        // Set the return register information if provided
                        if let (Some(caller_frame_idx), Some(result_reg)) = (frame_idx, result_reg) {
                            println!("DEBUG: Closure - setting return_register to ({}, {})", caller_frame_idx, result_reg);
                            frame.return_register = Some((caller_frame_idx, result_reg));
                        } else {
                            println!("DEBUG: Closure - NOT setting return_register, frame_idx={:?}, result_reg={:?}", frame_idx, result_reg);
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
                
                // If the instance has an __init__ method, call it
                if let Some(init_method) = methods.get("__init__") {
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
                        Value::Closure { name: method_name, params, body: _, captured_scope: _, docstring: _, compiled_code } => {
                            // For user-defined __init__ methods, we need to call them in a way that
                            // ensures modifications to the instance are visible
                            if let Some(code_obj) = compiled_code {
                                // Convert globals and builtins from RcValue to Value for Frame::new_function_frame
                                let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                                let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                                
                                // Create arguments with self as the first argument
                                let mut init_args = vec![instance.clone()];
                                init_args.extend(args.clone());
                                
                                // Create a new frame for the __init__ method
                                let mut init_frame = Frame::new_function_frame((**code_obj).clone(), globals_values, builtins_values, init_args, HashMap::new());
                                
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
                        Value::Closure { name: method_name, params, body: _, captured_scope: _, docstring: _, compiled_code } => {
                            // For user-defined __init__ methods, we need to call them in a way that
                            // ensures modifications to the instance are visible
                            if let Some(code_obj) = compiled_code {
                                // Convert globals and builtins from RcValue to Value for Frame::new_function_frame
                                let globals_values: HashMap<String, Value> = self.globals.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();
                                let builtins_values: HashMap<String, Value> = self.builtins.iter().map(|(k, v)| (k.clone(), v.value.clone())).collect();

                                // Create arguments with self as the first argument
                                let mut init_args = vec![instance.clone()];
                                init_args.extend(args.clone());

                                // Create a new frame for the __init__ method
                                let mut init_frame = Frame::new_function_frame((**code_obj).clone(), globals_values, builtins_values, init_args, HashMap::new());

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
                println!("DEBUG: BoundMethod handling - method_name={}, frame_idx={:?}, result_reg={:?}", method_name, frame_idx, result_reg);
                // Handle bound method calls
                // Get the method from the object
                match object.as_ref() {
                    Value::Object { class_name, class_methods, mro, .. } => {
                        println!("DEBUG: BoundMethod - found Object with class_name={}", class_name);
                        // First try to find the method in class_methods (normal case)
                        if let Some(method) = class_methods.get(&method_name) {
                            println!("DEBUG: BoundMethod - found method {} in class_methods", method_name);
                            // For bound methods, we need to call the method with the object as the first argument (self)
                            // But we need to do this through the VM properly

                            // Create arguments with self as the first argument
                            let mut method_args = vec![*object.clone()];
                            method_args.extend(args);

                            println!("DEBUG: BoundMethod - calling call_function_fast with frame_idx={:?}, result_reg={:?}", frame_idx, result_reg);
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
                    _ => return Err(anyhow!("Bound method called on non-object type '{}'", object.type_name()))
                }
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

}