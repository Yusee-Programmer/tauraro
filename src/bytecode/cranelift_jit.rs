//! Cranelift JIT Compiler Implementation
//!
//! This module provides actual JIT compilation using Cranelift to generate native x86-64 code.

use cranelift::prelude::*;
use cranelift_jit::{JITBuilder, JITModule};
use cranelift_module::{DataDescription, FuncId, Linkage, Module};
use cranelift_codegen::ir::Value as ClifValue;
use std::collections::HashMap;
use anyhow::{Result, anyhow};

use crate::bytecode::instructions::{Instruction, OpCode};
use crate::bytecode::objects::RcValue;
use crate::value::Value as TauraroValue;

/// JIT-compiled function type: takes register array pointer, returns error code
pub type JitFunction = unsafe extern "C" fn(*mut RcValue, usize) -> i32;

/// Cranelift JIT compiler for hot loops
pub struct CraneliftJIT {
    /// Cranelift JIT module
    module: JITModule,

    /// Context for function compilation
    ctx: codegen::Context,

    /// Function builder context
    builder_ctx: FunctionBuilderContext,

    /// Data description for constants
    data_ctx: DataDescription,

    /// Runtime helper function IDs
    helpers: HashMap<String, FuncId>,

    /// Compiled function cache
    compiled_functions: HashMap<String, (*const u8, usize)>,
}

impl CraneliftJIT {
    /// Create a new Cranelift JIT compiler
    pub fn new() -> Result<Self> {
        // Create JIT builder with default settings
        let mut builder = JITBuilder::new(cranelift_module::default_libcall_names())?;

        // Declare runtime helper symbols
        Self::declare_runtime_helpers(&mut builder);

        // Create JIT module
        let module = JITModule::new(builder);

        let ctx = module.make_context();
        let builder_ctx = FunctionBuilderContext::new();

        Ok(Self {
            module,
            ctx,
            builder_ctx,
            data_ctx: DataDescription::new(),
            helpers: HashMap::new(),
            compiled_functions: HashMap::new(),
        })
    }

    /// Declare all runtime helper functions as external symbols
    fn declare_runtime_helpers(builder: &mut JITBuilder) {
        // List operations
        builder.symbol("tauraro_jit_subscr_load_list", crate::bytecode::jit_runtime::tauraro_jit_subscr_load_list as *const u8);
        builder.symbol("tauraro_jit_subscr_store_list", crate::bytecode::jit_runtime::tauraro_jit_subscr_store_list as *const u8);
        builder.symbol("tauraro_jit_list_append", crate::bytecode::jit_runtime::tauraro_jit_list_append as *const u8);
        builder.symbol("tauraro_jit_build_list", crate::bytecode::jit_runtime::tauraro_jit_build_list as *const u8);

        // String operations
        builder.symbol("tauraro_jit_string_concat", crate::bytecode::jit_runtime::tauraro_jit_string_concat as *const u8);
        builder.symbol("tauraro_jit_string_index", crate::bytecode::jit_runtime::tauraro_jit_string_index as *const u8);
        builder.symbol("tauraro_jit_string_slice", crate::bytecode::jit_runtime::tauraro_jit_string_slice as *const u8);
        builder.symbol("tauraro_jit_string_len", crate::bytecode::jit_runtime::tauraro_jit_string_len as *const u8);

        // Dictionary operations
        builder.symbol("tauraro_jit_dict_get", crate::bytecode::jit_runtime::tauraro_jit_dict_get as *const u8);
        builder.symbol("tauraro_jit_dict_set", crate::bytecode::jit_runtime::tauraro_jit_dict_set as *const u8);
        builder.symbol("tauraro_jit_build_dict", crate::bytecode::jit_runtime::tauraro_jit_build_dict as *const u8);

        // Tuple operations
        builder.symbol("tauraro_jit_build_tuple", crate::bytecode::jit_runtime::tauraro_jit_build_tuple as *const u8);
        builder.symbol("tauraro_jit_tuple_index", crate::bytecode::jit_runtime::tauraro_jit_tuple_index as *const u8);

        // Set operations
        builder.symbol("tauraro_jit_build_set", crate::bytecode::jit_runtime::tauraro_jit_build_set as *const u8);
        builder.symbol("tauraro_jit_set_add", crate::bytecode::jit_runtime::tauraro_jit_set_add as *const u8);

        // Loop variable storage
        builder.symbol("tauraro_jit_store_int", crate::bytecode::jit_runtime::tauraro_jit_store_int as *const u8);

        // Binary arithmetic operations
        builder.symbol("tauraro_jit_binary_add_rr", crate::bytecode::jit_runtime::tauraro_jit_binary_add_rr as *const u8);
        builder.symbol("tauraro_jit_binary_sub_rr", crate::bytecode::jit_runtime::tauraro_jit_binary_sub_rr as *const u8);
        builder.symbol("tauraro_jit_binary_mul_rr", crate::bytecode::jit_runtime::tauraro_jit_binary_mul_rr as *const u8);

        // Variable load/store operations
        builder.symbol("tauraro_jit_load_fast", crate::bytecode::jit_runtime::tauraro_jit_load_fast as *const u8);
        builder.symbol("tauraro_jit_store_fast", crate::bytecode::jit_runtime::tauraro_jit_store_fast as *const u8);
        builder.symbol("tauraro_jit_load_global", crate::bytecode::jit_runtime::tauraro_jit_load_global as *const u8);
        builder.symbol("tauraro_jit_store_global", crate::bytecode::jit_runtime::tauraro_jit_store_global as *const u8);

        // Note: Iterator and type conversion helpers will be added when needed
        // builder.symbol("tauraro_jit_get_iter", ...);
        // builder.symbol("tauraro_jit_for_iter", ...);
        // builder.symbol("tauraro_jit_isinstance", ...);
        // builder.symbol("tauraro_jit_to_str", ...);
        // builder.symbol("tauraro_jit_to_bool", ...);
    }

    /// Compile a loop to native code with iteration control
    pub fn compile_loop(
        &mut self,
        function_name: &str,
        instructions: &[Instruction],
        _constants: &[TauraroValue],
        result_reg: u32,
        start_value: i64,
        stop_value: i64,
        step: i64,
    ) -> Result<JitFunction> {
        // Clear previous context
        self.ctx.clear();
        // Note: FunctionBuilderContext doesn't need clearing, we create a new one each time
        self.builder_ctx = FunctionBuilderContext::new();

        // Set function signature: fn(registers_ptr: *mut RcValue, reg_count: usize) -> i32
        let ptr_type = self.module.target_config().pointer_type();
        self.ctx.func.signature.params.push(AbiParam::new(ptr_type)); // registers_ptr
        self.ctx.func.signature.params.push(AbiParam::new(types::I64)); // reg_count
        self.ctx.func.signature.returns.push(AbiParam::new(types::I32)); // error code

        // Create function builder
        let mut builder = FunctionBuilder::new(&mut self.ctx.func, &mut self.builder_ctx);

        // Create entry block
        let entry_block = builder.create_block();
        builder.append_block_params_for_function_params(entry_block);
        builder.switch_to_block(entry_block);
        builder.seal_block(entry_block);

        // Get function parameters
        let registers_ptr = builder.block_params(entry_block)[0];
        let _reg_count = builder.block_params(entry_block)[1];

        // Create loop blocks
        let loop_header = builder.create_block();
        let loop_body = builder.create_block();
        let loop_exit = builder.create_block();

        // Initialize loop variable
        let start_val = builder.ins().iconst(types::I64, start_value);
        let stop_val = builder.ins().iconst(types::I64, stop_value);
        let step_val = builder.ins().iconst(types::I64, step);

        // Jump to loop header
        builder.ins().jump(loop_header, &[start_val]);

        // Loop header: check condition
        builder.switch_to_block(loop_header);
        builder.append_block_param(loop_header, types::I64); // current iteration value
        let current = builder.block_params(loop_header)[0];

        // Check if current < stop (for positive step) or current > stop (for negative step)
        let condition = if step > 0 {
            builder.ins().icmp(IntCC::SignedLessThan, current, stop_val)
        } else {
            builder.ins().icmp(IntCC::SignedGreaterThan, current, stop_val)
        };

        builder.ins().brif(condition, loop_body, &[], loop_exit, &[]);

        // Loop body: execute instructions
        builder.switch_to_block(loop_body);

        // Store current iteration value in result_reg using runtime helper
        // Get or declare store_int helper
        let store_helper_id = if let Some(&id) = self.helpers.get("tauraro_jit_store_int") {
            id
        } else {
            let mut sig = self.module.make_signature();
            let ptr_type = self.module.target_config().pointer_type();
            sig.params.push(AbiParam::new(ptr_type)); // registers_ptr
            sig.params.push(AbiParam::new(types::I32)); // reg_index
            sig.params.push(AbiParam::new(types::I64)); // value
            sig.returns.push(AbiParam::new(types::I32)); // return code

            let id = self.module.declare_function("tauraro_jit_store_int", Linkage::Import, &sig)?;
            self.helpers.insert("tauraro_jit_store_int".to_string(), id);
            id
        };

        let store_ref = self.module.declare_func_in_func(store_helper_id, &mut builder.func);
        let result_reg_val = builder.ins().iconst(types::I32, result_reg as i64);
        builder.ins().call(store_ref, &[registers_ptr, result_reg_val, current]);

        // Compile loop body instructions
        for inst in instructions {
            Self::compile_instruction_static(&mut builder, inst, registers_ptr, &mut self.module, &mut self.helpers)?;
        }

        // Increment loop variable
        let next = builder.ins().iadd(current, step_val);

        // Jump back to header with updated value
        builder.ins().jump(loop_header, &[next]);

        // Loop exit
        builder.switch_to_block(loop_exit);
        builder.seal_block(loop_header); // Seal loop header after all predecessors defined
        builder.seal_block(loop_body);
        builder.seal_block(loop_exit);

        // Return success (0)
        let zero = builder.ins().iconst(types::I32, 0);
        builder.ins().return_(&[zero]);

        // Finalize function
        builder.finalize();

        // Define function in module
        let id = self.module
            .declare_function(function_name, Linkage::Export, &self.ctx.func.signature)?;

        self.module.define_function(id, &mut self.ctx)?;

        // Finalize and get function pointer
        self.module.finalize_definitions()?;

        let code_ptr = self.module.get_finalized_function(id);

        // Cache the compiled function (use 0 as placeholder for size)
        self.compiled_functions.insert(
            function_name.to_string(),
            (code_ptr, 0)
        );

        // Return function pointer
        Ok(unsafe { std::mem::transmute(code_ptr) })
    }

    /// Compile a single instruction (static to avoid borrow checker issues)
    fn compile_instruction_static(
        builder: &mut FunctionBuilder,
        inst: &Instruction,
        registers_ptr: ClifValue,
        module: &mut JITModule,
        helpers: &mut HashMap<String, FuncId>,
    ) -> Result<()> {
        match inst.opcode {
            // Collection operations - call runtime helpers
            OpCode::SubscrLoad => {
                Self::compile_helper_call_static(builder, "tauraro_jit_subscr_load_list", inst, registers_ptr, module, helpers)?;
            }
            OpCode::SubscrStore => {
                Self::compile_helper_call_static(builder, "tauraro_jit_subscr_store_list", inst, registers_ptr, module, helpers)?;
            }
            OpCode::ListAppend => {
                Self::compile_helper_call_static(builder, "tauraro_jit_list_append", inst, registers_ptr, module, helpers)?;
            }
            OpCode::BuildList => {
                Self::compile_helper_call_static(builder, "tauraro_jit_build_list", inst, registers_ptr, module, helpers)?;
            }
            OpCode::BuildDict => {
                Self::compile_helper_call_static(builder, "tauraro_jit_build_dict", inst, registers_ptr, module, helpers)?;
            }
            OpCode::BuildTuple => {
                Self::compile_helper_call_static(builder, "tauraro_jit_build_tuple", inst, registers_ptr, module, helpers)?;
            }

            // Binary arithmetic operations
            OpCode::BinaryAddRR | OpCode::FastIntAdd => {
                Self::compile_helper_call_static(builder, "tauraro_jit_binary_add_rr", inst, registers_ptr, module, helpers)?;
            }
            OpCode::BinarySubRR => {
                Self::compile_helper_call_static(builder, "tauraro_jit_binary_sub_rr", inst, registers_ptr, module, helpers)?;
            }
            OpCode::BinaryMulRR => {
                Self::compile_helper_call_static(builder, "tauraro_jit_binary_mul_rr", inst, registers_ptr, module, helpers)?;
            }

            // Variable load/store operations
            OpCode::LoadFast | OpCode::LoadGlobal => {
                Self::compile_helper_call_static(builder, "tauraro_jit_load_global", inst, registers_ptr, module, helpers)?;
            }
            OpCode::StoreFast | OpCode::StoreGlobal => {
                Self::compile_helper_call_static(builder, "tauraro_jit_store_global", inst, registers_ptr, module, helpers)?;
            }

            // Type inference and control flow - can be skipped in JIT
            OpCode::InferType | OpCode::Jump => {
                // These are no-ops in JIT context
                // InferType is compile-time only
                // Jump is handled by loop control flow
            }

            _ => {
                // Skip unsupported opcodes (will be handled by interpreter)
            }
        }

        Ok(())
    }

    /// Compile a call to a runtime helper function (static to avoid borrow checker issues)
    fn compile_helper_call_static(
        builder: &mut FunctionBuilder,
        helper_name: &str,
        inst: &Instruction,
        registers_ptr: ClifValue,
        module: &mut JITModule,
        helpers: &mut HashMap<String, FuncId>,
    ) -> Result<()> {
        // Get or declare helper function
        let helper_id = if let Some(&id) = helpers.get(helper_name) {
            id
        } else {
            // Declare helper signature: fn(registers_ptr, arg1, arg2, arg3) -> i32
            let mut sig = module.make_signature();
            let ptr_type = module.target_config().pointer_type();
            sig.params.push(AbiParam::new(ptr_type)); // registers_ptr
            sig.params.push(AbiParam::new(types::I32)); // arg1
            sig.params.push(AbiParam::new(types::I32)); // arg2
            sig.params.push(AbiParam::new(types::I32)); // arg3
            sig.returns.push(AbiParam::new(types::I32)); // error code

            let id = module.declare_function(helper_name, Linkage::Import, &sig)?;
            helpers.insert(helper_name.to_string(), id);
            id
        };

        // Get function reference
        let helper_ref = module.declare_func_in_func(helper_id, builder.func);

        // Prepare arguments
        let arg1_val = builder.ins().iconst(types::I32, inst.arg1 as i64);
        let arg2_val = builder.ins().iconst(types::I32, inst.arg2 as i64);
        let arg3_val = builder.ins().iconst(types::I32, inst.arg3 as i64);

        // Call helper
        let call_inst = builder.ins().call(helper_ref, &[registers_ptr, arg1_val, arg2_val, arg3_val]);
        let result = builder.inst_results(call_inst)[0];

        // Check for errors (result != 0 means error)
        let zero = builder.ins().iconst(types::I32, 0);
        let is_error = builder.ins().icmp(IntCC::NotEqual, result, zero);

        // Create deoptimization block
        let error_block = builder.create_block();
        let continue_block = builder.create_block();

        builder.ins().brif(is_error, error_block, &[], continue_block, &[]);

        // Error block: return error code to trigger deoptimization
        builder.switch_to_block(error_block);
        builder.seal_block(error_block);
        builder.ins().return_(&[result]);

        // Continue block: proceed with JIT execution
        builder.switch_to_block(continue_block);
        builder.seal_block(continue_block);

        Ok(())
    }

    /// Compile a loop with VM-compatible signature
    ///
    /// This wrapper method provides compatibility with the VM's hot loop detection system.
    /// It adapts the old jit_compiler.rs signature to work with the new CraneliftJIT implementation.
    ///
    /// # Parameters
    /// - `function_name`: Name of the function containing the loop
    /// - `instructions`: All bytecode instructions in the function
    /// - `constants`: Constant pool for the function
    /// - `loop_start`: PC of the loop start instruction (ForIter)
    /// - `loop_end`: PC of the loop end (jump target)
    /// - `result_reg`: Register holding the loop variable
    /// - `start_value`: Initial loop value (current iteration when JIT triggers)
    /// - `stop_value`: Loop stop value (exclusive)
    /// - `step`: Loop step value
    ///
    /// # Returns
    /// Raw function pointer for VM execution
    pub fn compile_loop_vm(
        &mut self,
        function_name: &str,
        instructions: &[Instruction],
        constants: &[TauraroValue],
        loop_start: usize,
        loop_end: usize,
        result_reg: u32,
        start_value: i64,
        stop_value: i64,
        step: i64,
    ) -> Result<*const u8> {
        // Extract loop body instructions
        // The loop body is between loop_start (ForIter) and loop_end (jump target)
        let loop_body = if loop_start + 1 < loop_end && loop_end <= instructions.len() {
            &instructions[loop_start + 1..loop_end]
        } else {
            // Empty or invalid loop
            return Err(anyhow!("Invalid loop range: start={}, end={}, len={}",
                loop_start, loop_end, instructions.len()));
        };

        // Generate unique function name for this loop
        let jit_function_name = format!("{}_loop_{}", function_name, loop_start);

        // Compile with Cranelift including loop control
        let jit_fn = self.compile_loop(
            &jit_function_name,
            loop_body,
            constants,
            result_reg,
            start_value,
            stop_value,
            step,
        )?;

        // Return raw function pointer for VM
        Ok(jit_fn as *const u8)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_cranelift_jit_creation() {
        let jit = CraneliftJIT::new();
        assert!(jit.is_ok(), "Should be able to create Cranelift JIT");
    }
}
