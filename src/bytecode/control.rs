//! Control flow (JUMP, IF, CALL, RETURN)


use crate::value::Value;
use crate::bytecode::instructions::OpCode;
use crate::bytecode::vm::SuperBytecodeVM;
use crate::bytecode::objects::RcValue;
use anyhow::{Result, anyhow};

impl SuperBytecodeVM {
    /// Execute control flow opcodes
    #[inline(always)]
    pub fn execute_control_op(&mut self, frame_idx: usize, opcode: OpCode, arg1: u32, arg2: u32, arg3: u32) -> Result<Option<Value>> {
        match opcode {
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
            _ => Err(anyhow!("Opcode {:?} not implemented in control operations", opcode)),
        }
    }
}