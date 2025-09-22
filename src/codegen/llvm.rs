//! LLVM backend for native code generation with async/await support
use inkwell::context::Context;
use inkwell::module::Module;
use inkwell::builder::Builder;
use inkwell::values::{FunctionValue, BasicValueEnum};
use inkwell::types::{BasicTypeEnum, FunctionType};
use inkwell::IntPredicate;
use crate::ir::{IRModule, IRFunction, IRType, IRInstruction, IRValue};
use crate::codegen::{CodeGenerator, CodegenOptions, Target};
use anyhow::{Result, anyhow};

/// LLVM-based code generator
pub struct LLVMCodeGenerator {
    context: Context,
}

impl LLVMCodeGenerator {
    pub fn new() -> Self {
        Self {
            context: Context::create(),
        }
    }
    
    /// Generate LLVM IR from Tauraro IR
    fn generate_llvm_ir(&self, module: &IRModule, options: &CodegenOptions) -> Result<Module> {
        let llvm_module = self.context.create_module(&module.name);
        let builder = self.context.create_builder();
        
        // Generate type definitions
        self.generate_types(&llvm_module, module)?;
        
        // Generate global variables
        self.generate_globals(&llvm_module, module)?;
        
        // Generate functions
        for function in &module.functions {
            self.generate_function(&llvm_module, &builder, function, options)?;
        }
        
        // Generate async runtime if needed
        if module.functions.iter().any(|f| f.is_async) {
            self.generate_async_runtime(&llvm_module, &builder)?;
        }
        
        Ok(llvm_module)
    }
    
    /// Generate function with async support
    fn generate_function(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        options: &CodegenOptions,
    ) -> Result<FunctionValue> {
        // Convert parameter types to LLVM types
        let param_types: Vec<BasicTypeEnum> = function.parameters
            .iter()
            .map(|(_, ty)| self.ir_type_to_llvm(ty))
            .collect();
        
        let return_type = self.ir_type_to_llvm(&function.return_type);
        
        // Create function type
        let function_type = return_type.fn_type(&param_types, false);
        let llvm_function = module.add_function(&function.name, function_type, None);
        
        if function.is_async {
            // Generate async function (state machine)
            self.generate_async_function(module, builder, function, llvm_function)
        } else {
            // Generate normal function
            self.generate_sync_function(module, builder, function, llvm_function)
        }
    }
    
    /// Generate synchronous function
    fn generate_sync_function(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        llvm_function: FunctionValue,
    ) -> Result<FunctionValue> {
        let entry_block = self.context.append_basic_block(llvm_function, "entry");
        builder.position_at_end(entry_block);
        
        // Set up parameters
        for (i, (param_name, param_type)) in function.parameters.iter().enumerate() {
            let param_value = llvm_function.get_nth_param(i as u32).unwrap();
            let alloca = builder.build_alloca(self.ir_type_to_llvm(param_type), param_name);
            builder.build_store(alloca, param_value);
        }
        
        // Generate basic blocks
        for basic_block in &function.basic_blocks {
            self.generate_basic_block(module, builder, basic_block, llvm_function)?;
        }
        
        Ok(llvm_function)
    }
    
    /// Generate asynchronous function (state machine)
    fn generate_async_function(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        llvm_function: FunctionValue,
    ) -> Result<FunctionValue> {
        // Async functions are lowered to state machines
        let state_type = self.context.struct_type(&[
            self.context.i32_type().into(), // state
            self.context.i8_type().ptr_type(Default::default()).into(), // data
        ], false);
        
        let async_context_type = self.context.struct_type(&[
            state_type.into(), // current state
            self.context.i1_type().into(), // completed flag
            self.ir_type_to_llvm(&function.return_type).into(), // result
        ], false);
        
        // Create async function signature
        let async_function_type = async_context_type.ptr_type(Default::default())
            .fn_type(&[], false);
        
        let async_impl = module.add_function(&format!("{}_async", function.name), async_function_type, None);
        
        // Generate state machine implementation
        self.generate_async_state_machine(module, builder, function, async_impl, async_context_type)?;
        
        Ok(llvm_function)
    }
    
    /// Generate async state machine
    fn generate_async_state_machine(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        async_impl: FunctionValue,
        context_type: inkwell::types::StructType,
    ) -> Result<()> {
        let entry_block = self.context.append_basic_block(async_impl, "entry");
        builder.position_at_end(entry_block);
        
        // Get context parameter
        let context_arg = async_impl.get_nth_param(0).unwrap();
        
        // Load current state
        let state_ptr = builder.build_struct_gep(context_type, context_arg.into_pointer_value(), 0, "state_ptr");
        let state = builder.build_load(self.context.i32_type(), state_ptr, "state");
        
        // Create state switch
        let switch = builder.build_switch(state, entry_block, function.basic_blocks.len() as u32);
        
        // Generate code for each state (basic block)
        for (i, basic_block) in function.basic_blocks.iter().enumerate() {
            let state_block = self.context.append_basic_block(async_impl, &format!("state_{}", i));
            builder.position_at_end(state_block);
            
            // Generate code for this state
            self.generate_async_state(module, builder, basic_block, i as i32, context_arg.into_pointer_value(), context_type)?;
            
            switch.add_case(self.context.i32_type().const_int(i as u64, false), state_block);
        }
        
        Ok(())
    }
    
    /// Generate code for async state
    fn generate_async_state(
        &self,
        module: &Module,
        builder: &Builder,
        basic_block: &crate::ir::BasicBlock,
        state_id: i32,
        context: inkwell::values::PointerValue,
        context_type: inkwell::types::StructType,
    ) -> Result<()> {
        // Handle await points and state transitions
        for instruction in &basic_block.instructions {
            if self.is_await_point(instruction) {
                // For await instructions, we return from the function and will resume later
                let next_state = self.context.i32_type().const_int((state_id + 1) as u64, false);
                let state_ptr = builder.build_struct_gep(context_type, context, 0, "state_ptr");
                builder.build_store(state_ptr, next_state);
                
                // Return from async function (will resume later)
                builder.build_return(Some(&context));
                return Ok(());
            }
            
            // Generate normal instruction
            self.generate_instruction(builder, instruction)?;
        }
        
        // Handle terminator
        if let Some(terminator) = &basic_block.terminator {
            self.generate_terminator(builder, terminator, context, context_type)?;
        }
        
        Ok(())
    }
    
    /// Check if instruction is an await point
    fn is_await_point(&self, instruction: &IRInstruction) -> bool {
        // In real implementation, this would detect await operations
        matches!(instruction, IRInstruction::Call { function, .. } if function.contains("await"))
    }
    
    /// Generate instruction
    fn generate_instruction(&self, builder: &Builder, instruction: &IRInstruction) -> Result<Option<BasicValueEnum>> {
        match instruction {
            IRInstruction::Add { result, left, right } => {
                let left_val = self.ir_value_to_llvm(builder, left)?;
                let right_val = self.ir_value_to_llvm(builder, right)?;
                
                let sum = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_add(l, r, result).as_basic_value_enum()
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_add(l, r, result).as_basic_value_enum()
                    }
                    _ => return Err(anyhow!("Invalid types for addition")),
                };
                
                Ok(Some(sum))
            }
            IRInstruction::Call { result, function, args } => {
                let function_value = builder.get_module().get_function(function)
                    .ok_or_else(|| anyhow!("Function not found: {}", function))?;
                
                let arg_values: Result<Vec<BasicValueEnum>> = args
                    .iter()
                    .map(|arg| self.ir_value_to_llvm(builder, arg))
                    .collect();
                
                let call_result = builder.build_call(function_value, &arg_values?, &result.clone().unwrap_or("".to_string()));
                
                if let Some(result_var) = result {
                    let alloca = builder.build_alloca(call_result.get_type(), result_var);
                    builder.build_store(alloca, call_result.try_as_basic_value().left().unwrap());
                    Ok(None)
                } else {
                    Ok(None)
                }
            }
            _ => Ok(None), // TODO: Implement other instructions
        }
    }
    
    /// Generate terminator instruction
    fn generate_terminator(
        &self,
        builder: &Builder,
        terminator: &IRInstruction,
        context: inkwell::values::PointerValue,
        context_type: inkwell::types::StructType,
    ) -> Result<()> {
        match terminator {
            IRInstruction::Return { value } => {
                if let Some(val) = value {
                    let llvm_val = self.ir_value_to_llvm(builder, val)?;
                    
                    // Store result in async context if this is an async function
                    let result_ptr = builder.build_struct_gep(context_type, context, 2, "result_ptr");
                    builder.build_store(result_ptr, llvm_val);
                    
                    // Set completed flag
                    let completed_ptr = builder.build_struct_gep(context_type, context, 1, "completed_ptr");
                    builder.build_store(completed_ptr, self.context.bool_type().const_int(1, false));
                }
                
                builder.build_return(Some(&context));
            }
            _ => {} // TODO: Implement other terminators
        }
        Ok(())
    }
    
    /// Convert IR type to LLVM type
    fn ir_type_to_llvm(&self, ir_type: &IRType) -> BasicTypeEnum {
        match ir_type {
            IRType::I8 => self.context.i8_type().into(),
            IRType::I16 => self.context.i16_type().into(),
            IRType::I32 => self.context.i32_type().into(),
            IRType::I64 => self.context.i64_type().into(),
            IRType::F32 => self.context.f32_type().into(),
            IRType::F64 => self.context.f64_type().into(),
            IRType::Bool => self.context.bool_type().into(),
            IRType::Pointer(inner) => self.ir_type_to_llvm(inner).ptr_type(Default::default()).into(),
            IRType::Void => self.context.void_type().into(),
            _ => self.context.i64_type().into(), // Default fallback
        }
    }
    
    /// Convert IR value to LLVM value
    fn ir_value_to_llvm(&self, builder: &Builder, value: &IRValue) -> Result<BasicValueEnum> {
        match value {
            IRValue::ConstantInt(n) => Ok(self.context.i64_type().const_int(*n as u64, false).into()),
            IRValue::ConstantFloat(n) => Ok(self.context.f64_type().const_float(*n).into()),
            IRValue::ConstantBool(b) => Ok(self.context.bool_type().const_int(*b as u64, false).into()),
            IRValue::Variable(name) => {
                // Look up variable in current function
                let current_function = builder.get_insert_block().unwrap().get_parent().unwrap();
                if let Some(param) = current_function.get_nth_param(0) {
                    // Simplified: assume it's a parameter
                    Ok(param)
                } else {
                    Err(anyhow!("Variable not found: {}", name))
                }
            }
            _ => Err(anyhow!("Unsupported IR value: {:?}", value)),
        }
    }
    
    /// Generate type definitions
    fn generate_types(&self, module: &Module, ir_module: &IRModule) -> Result<()> {
        for (name, ir_type) in &ir_module.types {
            match ir_type {
                IRType::Struct(_) => {
                    // Generate struct type
                    let struct_type = self.context.opaque_struct_type(name);
                    module.add_struct_type(name, struct_type);
                }
                _ => {}
            }
        }
        Ok(())
    }
    
    /// Generate global variables
    fn generate_globals(&self, module: &Module, ir_module: &IRModule) -> Result<()> {
        for (name, (ir_type, initial_value)) in &ir_module.globals {
            let llvm_type = self.ir_type_to_llvm(ir_type);
            let global = module.add_global(llvm_type, None, name);
            
            if let Some(value) = initial_value {
                let llvm_value = self.ir_value_to_llvm(&self.context.create_builder(), value)?;
                global.set_initializer(&llvm_value.into());
            }
        }
        Ok(())
    }
    
    /// Generate async runtime support
    fn generate_async_runtime(&self, module: &Module, builder: &Builder) -> Result<()> {
        // Generate async task structure
        let task_type = self.context.struct_type(&[
            self.context.i32_type().into(), // state
            self.context.i8_type().ptr_type(Default::default()).into(), // function pointer
            self.context.i8_type().ptr_type(Default::default()).into(), // context
        ], false);
        
        module.add_struct_type("AsyncTask", task_type);
        
        // Generate async scheduler functions
        let scheduler_type = self.context.void_type().fn_type(&[], false);
        module.add_function("async_schedule", scheduler_type, None);
        module.add_function("async_yield", scheduler_type, None);
        module.add_function("async_resume", scheduler_type, None);
        
        Ok(())
    }
}

impl CodeGenerator for LLVMCodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        // Generate LLVM IR
        let llvm_module = self.generate_llvm_ir(&module, options)?;
        
        // Optimize module based on optimization level
        self.optimize_module(&llvm_module, options.opt_level)?;
        
        // Generate machine code
        let target_machine = self.create_target_machine()?;
        let memory_buffer = target_machine.write_to_memory_buffer(&llvm_module, inkwell::targets::FileType::Object)?;
        
        Ok(memory_buffer.as_slice().to_vec())
    }
    
    fn get_target(&self) -> Target {
        Target::Native
    }
}

impl LLVMCodeGenerator {
    fn optimize_module(&self, module: &Module, opt_level: u8) -> Result<()> {
        // Create pass manager
        let pass_manager_builder = inkwell::passes::PassManagerBuilder::create();
        pass_manager_builder.set_optimization_level(opt_level);
        
        let function_pass_manager = inkwell::passes::PassManager::create(());
        pass_manager_builder.populate_function_pass_manager(&function_pass_manager);
        
        let module_pass_manager = inkwell::passes::PassManager::create(());
        pass_manager_builder.populate_module_pass_manager(&module_pass_manager);
        
        // Run optimizations
        module_pass_manager.run_on(module);
        
        for function in module.get_functions() {
            function_pass_manager.run_on(&function);
        }
        
        Ok(())
    }
    
    fn create_target_machine(&self) -> Result<inkwell::targets::TargetMachine> {
        use inkwell::targets::{Target, TargetTriple, InitializationConfig};
        
        // Initialize targets
        Target::initialize_native(&InitializationConfig::default())?;
        
        let target_triple = TargetTriple::create(&TargetTriple::get_default_triple());
        let target = Target::from_triple(&target_triple)?;
        
        let target_machine = target.create_target_machine(
            &target_triple,
            "generic", // CPU
            "", // Features
            inkwell::targets::OptimizationLevel::Default,
            inkwell::targets::RelocMode::Default,
            inkwell::targets::CodeModel::Default,
        ).ok_or_else(|| anyhow!("Failed to create target machine"))?;
        
        Ok(target_machine)
    }
}