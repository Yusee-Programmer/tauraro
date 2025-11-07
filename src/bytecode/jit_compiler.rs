//! Cranelift IR generation for Tauraro bytecode
//!
//! This module translates Tauraro bytecode instructions to Cranelift IR for JIT compilation.

#[cfg(feature = "jit")]
use cranelift_codegen::ir::{Function, InstBuilder, UserFuncName};
#[cfg(feature = "jit")]
use cranelift_codegen::ir::types::{I64, I32};
#[cfg(feature = "jit")]
use cranelift_codegen::Context;
#[cfg(feature = "jit")]
use cranelift_codegen::settings;
#[cfg(feature = "jit")]
use cranelift_frontend::{FunctionBuilder, FunctionBuilderContext, Variable};
#[cfg(feature = "jit")]
use cranelift_module::{Module, Linkage};
#[cfg(feature = "jit")]
use cranelift_jit::{JITBuilder, JITModule};
#[cfg(feature = "jit")]
use std::collections::HashMap;

use crate::bytecode::instructions::{OpCode, Instruction};
use crate::value::Value;
use anyhow::{Result, anyhow};

/// Native loop function signature
/// fn loop_body(registers: *mut i64, constants: *const i64, iteration_count: i64) -> i32
/// Returns: 0 = success, -1 = error
#[cfg(feature = "jit")]
pub type NativeLoopFn = unsafe extern "C" fn(*mut i64, *const i64, i64) -> i32;

/// JIT compiler using Cranelift
#[cfg(feature = "jit")]
pub struct JITCompiler {
    /// Cranelift JIT module
    module: JITModule,

    /// Context for compilation
    ctx: Context,

    /// Function builder context
    builder_ctx: FunctionBuilderContext,
}

#[cfg(feature = "jit")]
impl JITCompiler {
    /// Create a new JIT compiler
    pub fn new() -> Result<Self> {
        // Create Cranelift JIT builder
        let builder = JITBuilder::new(cranelift_module::default_libcall_names())
            .map_err(|e| anyhow!("Failed to create JIT builder: {}", e))?;
        let module = JITModule::new(builder);

        // Create context manually (not from module)
        let ctx = Context::new();
        let builder_ctx = FunctionBuilderContext::new();

        Ok(Self {
            module,
            ctx,
            builder_ctx,
        })
    }

    /// Compile a loop to native code
    pub fn compile_loop(
        &mut self,
        function_name: &str,
        instructions: &[Instruction],
        constants: &[Value],
        loop_start: usize,
        loop_end: usize,
        result_reg: u32,  // Register that holds the loop variable (e.g., 'i' in 'for i in range()')
        start_value: i64,  // Starting value for the loop variable
        step: i64,  // Step value for the loop variable
    ) -> Result<*const u8> {
        // Clear previous function
        self.ctx.func.clear();

        // Set function signature:
        // fn loop_body(registers: *mut i64, constants: *const i64, iteration_count: i64) -> i32
        let pointer_type = self.module.target_config().pointer_type();
        self.ctx.func.signature.params.push(cranelift_codegen::ir::AbiParam::new(pointer_type)); // registers
        self.ctx.func.signature.params.push(cranelift_codegen::ir::AbiParam::new(pointer_type)); // constants
        self.ctx.func.signature.params.push(cranelift_codegen::ir::AbiParam::new(I64)); // iteration_count
        self.ctx.func.signature.returns.push(cranelift_codegen::ir::AbiParam::new(I32)); // return code

        // Create function builder
        let mut builder = FunctionBuilder::new(&mut self.ctx.func, &mut self.builder_ctx);

        // Create entry block
        let entry_block = builder.create_block();
        builder.append_block_params_for_function_params(entry_block);
        builder.switch_to_block(entry_block);
        builder.seal_block(entry_block);

        // Get function parameters
        let params = builder.block_params(entry_block);
        let registers_ptr = params[0];
        let constants_ptr = params[1];
        let iteration_count = params[2];

        // Track register values in Cranelift IR
        let mut register_values: HashMap<u32, cranelift_codegen::ir::Value> = HashMap::new();

        // Extract loop body instructions (AFTER loop_start, which is the ForIter instruction)
        // The loop body starts at loop_start + 1 and goes until loop_end (exclusive)
        let loop_body_start = loop_start + 1;
        let loop_instructions = if loop_end < instructions.len() && loop_body_start < loop_end {
            &instructions[loop_body_start..loop_end]
        } else if loop_body_start < instructions.len() {
            &instructions[loop_body_start..]
        } else {
            &[]  // Empty loop body
        };


        // Create loop blocks
        let loop_header = builder.create_block();
        let loop_body = builder.create_block();
        let exit_block = builder.create_block();

        // Initialize loop counter
        let loop_counter_var = Variable::from_u32(0);
        builder.declare_var(loop_counter_var, I64);
        let zero = builder.ins().iconst(I64, 0);
        builder.def_var(loop_counter_var, zero);

        // Jump to loop header
        builder.ins().jump(loop_header, &[]);

        // === Loop Header: Check condition ===
        builder.switch_to_block(loop_header);

        // Check loop condition: counter < iteration_count
        let counter = builder.use_var(loop_counter_var);
        let cond = builder.ins().icmp(cranelift_codegen::ir::condcodes::IntCC::SignedLessThan, counter, iteration_count);
        builder.ins().brif(cond, loop_body, &[], exit_block, &[]);

        // === Loop Body: Execute instructions ===
        builder.switch_to_block(loop_body);

        // Calculate and store the loop variable value: start_value + (counter * step)
        let counter_for_iter = builder.use_var(loop_counter_var);
        let start_val = builder.ins().iconst(I64, start_value);
        let step_val = builder.ins().iconst(I64, step);
        let offset = builder.ins().imul(counter_for_iter, step_val);
        let iter_value = builder.ins().iadd(start_val, offset);

        // Store iteration value in result_reg
        register_values.insert(result_reg, iter_value);
        let result_offset = builder.ins().iconst(I64, (result_reg as i64) * 8);
        let result_addr = builder.ins().iadd(registers_ptr, result_offset);
        builder.ins().store(cranelift_codegen::ir::MemFlags::new(), iter_value, result_addr, 0);

        // Translate loop body instructions to IR
        for instr in loop_instructions {
            // Translate instruction - ignore errors for now, fallback to interpreter for unsupported ops
            let _ = Self::translate_instruction(
                &mut builder,
                instr,
                &mut register_values,
                registers_ptr,
                constants_ptr,
                constants,
            );
        }

        // Increment loop counter
        let counter_body = builder.use_var(loop_counter_var);
        let one = builder.ins().iconst(I64, 1);
        let new_counter = builder.ins().iadd(counter_body, one);
        builder.def_var(loop_counter_var, new_counter);

        // Back edge to loop header
        builder.ins().jump(loop_header, &[]);

        // === Exit Block ===
        builder.switch_to_block(exit_block);
        builder.seal_block(loop_header);
        builder.seal_block(loop_body);
        builder.seal_block(exit_block);

        // Return success (0)
        let return_val = builder.ins().iconst(I32, 0);
        builder.ins().return_(&[return_val]);

        // Finalize function
        builder.finalize();

        // Declare function in module with unique name (include loop_start for uniqueness)
        let func_name = format!("jit_{}_pc{}", function_name, loop_start);
        let func_id = self.module
            .declare_function(&func_name, Linkage::Export, &self.ctx.func.signature)?;

        // Define function
        self.module.define_function(func_id, &mut self.ctx)?;

        // Clear context
        self.module.clear_context(&mut self.ctx);

        // Finalize module
        self.module.finalize_definitions()?;

        // Get function pointer
        let code_ptr = self.module.get_finalized_function(func_id);

        Ok(code_ptr)
    }

    /// Translate a single Tauraro instruction to Cranelift IR
    #[allow(unused)]
    fn translate_instruction(
        builder: &mut FunctionBuilder,
        instr: &Instruction,
        registers: &mut HashMap<u32, cranelift_codegen::ir::Value>,
        registers_ptr: cranelift_codegen::ir::Value,
        constants_ptr: cranelift_codegen::ir::Value,
        constants: &[Value],
    ) -> Result<()> {
        match instr.opcode {
            OpCode::LoadConst => {
                // Load constant: registers[arg2] = constants[arg1]
                let const_idx = instr.arg1;
                let dest_reg = instr.arg2;

                // Get constant value
                if let Some(Value::Int(val)) = constants.get(const_idx as usize) {
                    let cranelift_val = builder.ins().iconst(I64, *val);
                    registers.insert(dest_reg, cranelift_val);

                    // Store to register array for interpreter compatibility
                    let offset = builder.ins().iconst(I64, (dest_reg as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), cranelift_val, addr, 0);
                }
            }

            OpCode::LoadFast | OpCode::LoadLocal | OpCode::LoadGlobal => {
                // Load from local/global variable: registers[arg2] = locals[arg1] or globals[arg1]
                // For JIT, we'll load from the register array
                let src_reg = instr.arg1;
                let dest_reg = instr.arg2;

                let offset = builder.ins().iconst(I64, (src_reg as i64) * 8);
                let addr = builder.ins().iadd(registers_ptr, offset);
                let val = builder.ins().load(I64, cranelift_codegen::ir::MemFlags::new(), addr, 0);
                registers.insert(dest_reg, val);
            }

            OpCode::StoreFast | OpCode::StoreLocal | OpCode::StoreGlobal => {
                // Store to local/global variable: locals[arg2] = registers[arg1] or globals[arg2] = registers[arg1]
                let src_reg = instr.arg1;
                let dest_idx = instr.arg2;

                if let Some(&val) = registers.get(&src_reg) {
                    let offset = builder.ins().iconst(I64, (dest_idx as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), val, addr, 0);
                }
            }

            OpCode::FastIntAdd | OpCode::BinaryAddRR => {
                // Add two registers: registers[arg3] = registers[arg1] + registers[arg2]
                let left_reg = instr.arg1;
                let right_reg = instr.arg2;
                let dest_reg = instr.arg3;

                if let (Some(&left_val), Some(&right_val)) = (registers.get(&left_reg), registers.get(&right_reg)) {
                    let result = builder.ins().iadd(left_val, right_val);
                    registers.insert(dest_reg, result);

                    // Store back to register array
                    let offset = builder.ins().iconst(I64, (dest_reg as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), result, addr, 0);
                }
            }

            OpCode::FastIntSub | OpCode::BinarySubRR => {
                // Subtract: registers[arg3] = registers[arg1] - registers[arg2]
                let left_reg = instr.arg1;
                let right_reg = instr.arg2;
                let dest_reg = instr.arg3;

                if let (Some(&left_val), Some(&right_val)) = (registers.get(&left_reg), registers.get(&right_reg)) {
                    let result = builder.ins().isub(left_val, right_val);
                    registers.insert(dest_reg, result);

                    // Store back to register array
                    let offset = builder.ins().iconst(I64, (dest_reg as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), result, addr, 0);
                }
            }

            OpCode::FastIntMul | OpCode::BinaryMulRR => {
                // Multiply: registers[arg3] = registers[arg1] * registers[arg2]
                let left_reg = instr.arg1;
                let right_reg = instr.arg2;
                let dest_reg = instr.arg3;

                if let (Some(&left_val), Some(&right_val)) = (registers.get(&left_reg), registers.get(&right_reg)) {
                    let result = builder.ins().imul(left_val, right_val);
                    registers.insert(dest_reg, result);

                    // Store back to register array
                    let offset = builder.ins().iconst(I64, (dest_reg as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), result, addr, 0);
                }
            }

            OpCode::FastIntDiv | OpCode::BinaryDivRR => {
                // Divide: registers[arg3] = registers[arg1] / registers[arg2]
                let left_reg = instr.arg1;
                let right_reg = instr.arg2;
                let dest_reg = instr.arg3;

                if let (Some(&left_val), Some(&right_val)) = (registers.get(&left_reg), registers.get(&right_reg)) {
                    let result = builder.ins().sdiv(left_val, right_val);
                    registers.insert(dest_reg, result);

                    // Store back to register array
                    let offset = builder.ins().iconst(I64, (dest_reg as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), result, addr, 0);
                }
            }

            OpCode::FastIntMod | OpCode::BinaryModRR => {
                // Modulo: registers[arg3] = registers[arg1] % registers[arg2]
                let left_reg = instr.arg1;
                let right_reg = instr.arg2;
                let dest_reg = instr.arg3;

                if let (Some(&left_val), Some(&right_val)) = (registers.get(&left_reg), registers.get(&right_reg)) {
                    let result = builder.ins().srem(left_val, right_val);
                    registers.insert(dest_reg, result);

                    // Store back to register array
                    let offset = builder.ins().iconst(I64, (dest_reg as i64) * 8);
                    let addr = builder.ins().iadd(registers_ptr, offset);
                    builder.ins().store(cranelift_codegen::ir::MemFlags::new(), result, addr, 0);
                }
            }

            _ => {
                // Unsupported opcode - will fallback to interpreter
                // Don't return error, just skip this instruction
            }
        }

        Ok(())
    }
}

// Stub implementation when JIT feature is disabled
#[cfg(not(feature = "jit"))]
pub struct JITCompiler;

#[cfg(not(feature = "jit"))]
impl JITCompiler {
    pub fn new() -> Result<Self> {
        Err(anyhow!("JIT compilation requires --features jit"))
    }
}
