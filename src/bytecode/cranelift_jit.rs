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
type JitFunction = unsafe extern "C" fn(*mut RcValue, usize) -> i32;

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

        // Note: Iterator and type conversion helpers will be added when needed
        // builder.symbol("tauraro_jit_get_iter", ...);
        // builder.symbol("tauraro_jit_for_iter", ...);
        // builder.symbol("tauraro_jit_isinstance", ...);
        // builder.symbol("tauraro_jit_to_str", ...);
        // builder.symbol("tauraro_jit_to_bool", ...);
    }

    /// Compile a loop to native code
    pub fn compile_loop(
        &mut self,
        function_name: &str,
        instructions: &[Instruction],
        _constants: &[TauraroValue],
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

        // Compile instructions
        for inst in instructions {
            Self::compile_instruction_static(&mut builder, inst, registers_ptr, &mut self.module, &mut self.helpers)?;
        }

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
