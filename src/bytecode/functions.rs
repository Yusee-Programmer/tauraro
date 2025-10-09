//! Function call handling (CALL, RETURN, METHOD)

use crate::ast::*;
use crate::value::Value;
use crate::modules::hplist::HPList;
use crate::bytecode::instructions::{OpCode, Instruction};
use crate::bytecode::arithmetic::{RcValue, Frame, SuperBytecodeVM};
use anyhow::{Result, anyhow};
use std::collections::HashMap;

impl SuperBytecodeVM {
    /// Execute function-related opcodes
    #[inline(always)]
    pub fn execute_function_op(&mut self, frame_idx: usize, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32) -> Result<Option<Value>> {
        match opcode {
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

                // For custom objects, we need to use getattr to get the method (which creates a BoundMethod)
                // and then call it. For built-in types, we can use call_method directly.
                let method_result = if matches!(object_value, Value::Object { .. }) {
                    // Get the method using getattr_impl
                    match self.getattr_impl(&object_value, &method_name) {
                        Ok(Value::BoundMethod { object, method_name: bound_method_name }) => {
                            // The method is a bound method - look it up in the class_methods and call it
                            if let Value::Object { class_methods, .. } = object.as_ref() {
                                if let Some(method) = class_methods.get(&bound_method_name) {
                                    // Call the method with the object as the first argument (self)
                                    let mut method_args = vec![object.as_ref().clone()];
                                    method_args.extend(args.clone());
                                    // Don't pass frame_idx to avoid side effects
                                    self.call_function_fast(method.clone(), method_args, None, None)
                                } else {
                                    Err(anyhow!("Method '{}' not found in class_methods", bound_method_name))
                                }
                            } else {
                                Err(anyhow!("BoundMethod object is not an Object"))
                            }
                        }
                        Ok(_) => Err(anyhow!("getattr did not return a BoundMethod")),
                        Err(e) => Err(e),
                    }
                } else {
                    // For built-in types, use call_method
                    object_value.call_method(&method_name, args.clone())
                };

                // Call the method using the comprehensive call_method implementation
                // Note: call_method may mutate object_value for mutating methods like append()
                match method_result {
                    Ok(result) => {
                        // For custom objects, we don't overwrite the object register
                        // unless it's a mutating built-in type method
                        let is_custom_object = matches!(object_value, Value::Object { .. });

                        if is_custom_object {
                            // For custom objects, store the method result in object_reg
                            // The bytecode expects the result to be in object_reg so it can LoadLocal it to result_reg
                            // The original object is stored in a variable (globals/locals), so this doesn't lose the object
                            self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(result);
                        } else {
                            // For built-in types, handle mutating methods
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
                                if matches!((&object_value, method_name.as_str()),
                                    (Value::List(_), "pop") |
                                    (Value::Dict(_), "pop" | "setdefault" | "popitem") |
                                    (Value::Set(_), "pop")
                                ) {
                                    // Store the mutated object first
                                    self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(object_value.clone());
                                }
                                // Now store the result for LoadLocal to retrieve
                                self.frames[frame_idx].registers[object_reg as usize] = RcValue::new(result);
                            }
                        }

                        Ok(None)
                    }
                    Err(e) => Err(anyhow!("CallMethod error: {}", e)),
                }
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
            _ => Err(anyhow!("Opcode {:?} not implemented in function operations", opcode)),
        }
    }
}