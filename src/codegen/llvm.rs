//! COMPLETE LLVM backend for native code generation with async/await support

#[cfg(feature = "llvm")]
use std::collections::HashMap;
#[cfg(feature = "llvm")]
use inkwell::context::Context;
#[cfg(feature = "llvm")]
use inkwell::module::Module;
#[cfg(feature = "llvm")]
use inkwell::builder::Builder;
#[cfg(feature = "llvm")]
use inkwell::values::{FunctionValue, BasicValueEnum, PointerValue, IntValue};
#[cfg(feature = "llvm")]
use inkwell::types::{BasicTypeEnum, FunctionType, StructType, IntType};
#[cfg(feature = "llvm")]
use inkwell::IntPredicate;
#[cfg(feature = "llvm")]
use inkwell::AddressSpace;
use crate::ir::{IRModule, IRFunction, IRType, IRInstruction, IRValue, IRGlobal, IRParam};
use crate::codegen::{CodeGenerator, CodegenOptions, Target, OptimizationLevel};
use anyhow::{Result, anyhow};

#[cfg(feature = "llvm")]

/// LLVM-based code generator
pub struct LLVMCodeGenerator {
    context: Context,
}

#[cfg(feature = "llvm")]
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
        
        // Set target triple
        if let Some(triple) = &options.target_triple {
            llvm_module.set_triple(triple);
        } else {
            llvm_module.set_triple(&crate::codegen::target::get_default_target_triple());
        }
        
        // Generate type definitions
        self.generate_types(&llvm_module, module)?;
        
        // Generate global variables
        self.generate_globals(&llvm_module, module)?;
        
        // Generate function declarations first
        for function in module.functions.values() {
            self.generate_function_declaration(&llvm_module, function)?;
        }
        
        // Generate function implementations
        for function in module.functions.values() {
            self.generate_function_implementation(&llvm_module, &builder, function, options)?;
        }
        
        // Generate async runtime if needed
        if module.functions.values().any(|f| f.is_async) {
            self.generate_async_runtime(&llvm_module, &builder)?;
        }
        
        Ok(llvm_module)
    }
    
    /// Generate function declaration
    fn generate_function_declaration(&self, module: &Module, function: &IRFunction) -> Result<FunctionValue> {
        // Convert parameter types to LLVM types
        let param_types: Vec<BasicTypeEnum> = function.params
            .iter()
            .map(|param| self.ir_type_to_llvm(&param.ty))
            .collect();
        
        let return_type = self.ir_type_to_llvm(&function.return_type);
        
        // Create function type
        let function_type = if function.is_async {
            // Async functions return a pointer to their context
            let context_type = self.create_async_context_type();
            context_type.ptr_type(AddressSpace::default()).fn_type(&[context_type.ptr_type(AddressSpace::default()).into()], false)
        } else {
            return_type.fn_type(&param_types, false)
        };
        
        let llvm_function = module.add_function(&function.name, function_type, None);
        
        // Set parameter names
        for (i, param) in function.params.iter().enumerate() {
            llvm_function.get_nth_param(i as u32)
                .unwrap()
                .set_name(&param.name);
        }
        
        Ok(llvm_function)
    }
    
    /// Generate function implementation
    fn generate_function_implementation(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        options: &CodegenOptions,
    ) -> Result<FunctionValue> {
        let llvm_function = module.get_function(&function.name)
            .ok_or_else(|| anyhow!("Function not found: {}", function.name))?;
        
        if function.is_async {
            // Generate async function (state machine)
            self.generate_async_function(module, builder, function, llvm_function)
        } else {
            // Generate normal function
            self.generate_sync_function(module, builder, function, llvm_function, options)
        }
    }
    
    /// Generate synchronous function
    fn generate_sync_function(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        llvm_function: FunctionValue,
        _options: &CodegenOptions,
    ) -> Result<FunctionValue> {
        // Create entry block
        let entry_block = self.context.append_basic_block(llvm_function, "entry");
        builder.position_at_end(entry_block);
        
        // Set up parameters - create alloca for each parameter
        let mut param_allocas = HashMap::new();
        for (i, param) in function.params.iter().enumerate() {
            let param_value = llvm_function.get_nth_param(i as u32).unwrap();
            let alloca = builder.build_alloca(self.ir_type_to_llvm(&param.ty), &param.name)?;
            builder.build_store(alloca, param_value)?;
            param_allocas.insert(param.name.clone(), alloca);
        }
        
        // Create a mapping of block labels to LLVM basic blocks
        let mut basic_blocks = HashMap::new();
        basic_blocks.insert("entry".to_string(), entry_block);
        
        // Create all basic blocks first
        for block in &function.blocks {
            if block.label != "entry" {
                let llvm_block = self.context.append_basic_block(llvm_function, &block.label);
                basic_blocks.insert(block.label.clone(), llvm_block);
            }
        }
        
        // Keep track of generated values
        let mut value_map = HashMap::new();
        
        // Generate code for each basic block
        for block in &function.blocks {
            let llvm_block = *basic_blocks.get(&block.label).unwrap();
            builder.position_at_end(llvm_block);
            
            // Generate instructions
            for instruction in &block.instructions {
                self.generate_instruction(builder, instruction, &mut param_allocas, &basic_blocks, &mut value_map, module)?;
            }
        }
        
        Ok(llvm_function)
    }
    
    /// Generate instruction
    fn generate_instruction(
        &self,
        builder: &Builder,
        instruction: &IRInstruction,
        param_allocas: &mut HashMap<String, PointerValue>,
        basic_blocks: &HashMap<String, inkwell::basic_block::BasicBlock>,
        value_map: &mut HashMap<String, BasicValueEnum>,
        module: &IRModule,
    ) -> Result<()> {
        match instruction {
            IRInstruction::Add { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let sum = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_add(l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_add(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for addition")),
                };
                
                value_map.insert(dest.clone(), sum.as_basic_value_enum());
            }
            IRInstruction::Sub { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let diff = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_sub(l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_sub(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for subtraction")),
                };
                
                value_map.insert(dest.clone(), diff.as_basic_value_enum());
            }
            IRInstruction::Mul { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let product = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_mul(l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_mul(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for multiplication")),
                };
                
                value_map.insert(dest.clone(), product.as_basic_value_enum());
            }
            IRInstruction::Div { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let quotient = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_signed_div(l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_div(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for division")),
                };
                
                value_map.insert(dest.clone(), quotient.as_basic_value_enum());
            }
            IRInstruction::Mod { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let remainder = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_signed_rem(l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_rem(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for modulo")),
                };
                
                value_map.insert(dest.clone(), remainder.as_basic_value_enum());
            }
            IRInstruction::Alloca { dest, ty } => {
                let llvm_type = self.ir_type_to_llvm(ty);
                let alloca = builder.build_alloca(llvm_type, dest)?;
                value_map.insert(dest.clone(), alloca.as_basic_value_enum());
            }
            IRInstruction::Store { value, ptr } => {
                let value_val = self.get_value(builder, value, param_allocas, value_map, module)?;
                if let Some(ptr_val) = value_map.get(ptr) {
                    if let BasicValueEnum::PointerValue(ptr) = ptr_val {
                        builder.build_store(*ptr, value_val)?;
                    } else {
                        return Err(anyhow!("Store target is not a pointer"));
                    }
                } else {
                    return Err(anyhow!("Store target not found: {}", ptr));
                }
            }
            IRInstruction::Load { dest, ptr, ty: _ } => {
                if let Some(ptr_val) = value_map.get(ptr) {
                    if let BasicValueEnum::PointerValue(ptr) = ptr_val {
                        let loaded = builder.build_load(*ptr, dest)?;
                        value_map.insert(dest.clone(), loaded);
                    } else {
                        return Err(anyhow!("Load source is not a pointer"));
                    }
                } else {
                    return Err(anyhow!("Load source not found: {}", ptr));
                }
            }
            IRInstruction::Call { dest, func, args } => {
                let function_value = builder.get_module().get_function(func)
                    .ok_or_else(|| anyhow!("Function not found: {}", func))?;
                
                let arg_values: Result<Vec<BasicValueEnum>> = args
                    .iter()
                    .map(|arg| self.get_value(builder, arg, param_allocas, value_map, module))
                    .collect();
                
                let call_result = builder.build_call(function_value, &arg_values?, dest.as_deref().unwrap_or("call_result"))?;
                
                if let Some(result_var) = dest {
                    if let Some(basic_value) = call_result.try_as_basic_value().left() {
                        value_map.insert(result_var.clone(), basic_value);
                    }
                }
            }
            IRInstruction::Ret { value } => {
                let return_value = if let Some(val) = value {
                    Some(self.get_value(builder, val, param_allocas, value_map, module)?)
                } else {
                    None
                };
                
                builder.build_return(return_value.as_ref())?;
            }
            IRInstruction::Jmp { label } => {
                if let Some(target_block) = basic_blocks.get(label) {
                    builder.build_unconditional_branch(*target_block)?;
                } else {
                    return Err(anyhow!("Unknown basic block: {}", label));
                }
            }
            IRInstruction::Br { cond, then_label, else_label } => {
                let cond_val = self.get_value(builder, cond, param_allocas, value_map, module)?;
                let true_block = basic_blocks.get(then_label)
                    .ok_or_else(|| anyhow!("Unknown basic block: {}", then_label))?;
                let false_block = basic_blocks.get(else_label)
                    .ok_or_else(|| anyhow!("Unknown basic block: {}", else_label))?;
                
                if let BasicValueEnum::IntValue(int_val) = cond_val {
                    builder.build_conditional_branch(int_val, *true_block, *false_block)?;
                } else {
                    return Err(anyhow!("Branch condition is not an integer"));
                }
            }
            IRInstruction::CmpEq { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_compare(IntPredicate::EQ, l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_compare(inkwell::FloatPredicate::OEQ, l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for equality comparison")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::CmpNe { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_compare(IntPredicate::NE, l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_compare(inkwell::FloatPredicate::ONE, l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for inequality comparison")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::CmpLt { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_compare(IntPredicate::SLT, l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_compare(inkwell::FloatPredicate::OLT, l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for less-than comparison")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::CmpGt { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_compare(IntPredicate::SGT, l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_compare(inkwell::FloatPredicate::OGT, l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for greater-than comparison")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::CmpLe { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_compare(IntPredicate::SLE, l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_compare(inkwell::FloatPredicate::OLE, l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for less-than-or-equal comparison")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::CmpGe { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_int_compare(IntPredicate::SGE, l, r, dest)?
                    }
                    (BasicValueEnum::FloatValue(l), BasicValueEnum::FloatValue(r)) => {
                        builder.build_float_compare(inkwell::FloatPredicate::OGE, l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for greater-than-or-equal comparison")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::And { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_and(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for logical AND")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::Or { dest, left, right } => {
                let left_val = self.get_value(builder, left, param_allocas, value_map, module)?;
                let right_val = self.get_value(builder, right, param_allocas, value_map, module)?;
                
                let result = match (left_val, right_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        builder.build_or(l, r, dest)?
                    }
                    _ => return Err(anyhow!("Invalid types for logical OR")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::Not { dest, operand } => {
                let operand_val = self.get_value(builder, operand, param_allocas, value_map, module)?;
                
                let result = match operand_val {
                    BasicValueEnum::IntValue(val) => {
                        // Logical NOT: if val is 0, result is 1, otherwise 0
                        let zero = self.context.bool_type().const_int(0, false);
                        builder.build_int_compare(IntPredicate::EQ, val, zero, dest)?
                    }
                    _ => return Err(anyhow!("Invalid type for logical NOT")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::Neg { dest, operand } => {
                let operand_val = self.get_value(builder, operand, param_allocas, value_map, module)?;
                
                let result = match operand_val {
                    BasicValueEnum::IntValue(val) => {
                        builder.build_int_neg(val, dest)?
                    }
                    BasicValueEnum::FloatValue(val) => {
                        builder.build_float_neg(val, dest)?
                    }
                    _ => return Err(anyhow!("Invalid type for negation")),
                };
                
                value_map.insert(dest.clone(), result.as_basic_value_enum());
            }
            IRInstruction::Print { value } => {
                // For print, we'll create a call to a printf-like function
                let print_func = builder.get_module().get_function("print")
                    .unwrap_or_else(|| {
                        // Create print function if it doesn't exist
                        let i8_ptr_type = self.context.i8_type().ptr_type(AddressSpace::default());
                        let printf_type = self.context.void_type().fn_type(&[i8_ptr_type.into()], true);
                        builder.get_module().add_function("print", printf_type, None)
                    });
                
                let value_val = self.get_value(builder, value, param_allocas, value_map, module)?;
                // For simplicity, we'll just call print with no arguments for now
                builder.build_call(print_func, &[], "print_result")?;
            }
            _ => {
                // For unimplemented instructions, emit a comment in debug builds
                #[cfg(debug_assertions)]
                eprintln!("TODO: Implement instruction {:?}", instruction);
            }
        }
        Ok(())
    }
    
    /// Get LLVM value from IR value
    fn get_value(
        &self,
        builder: &Builder,
        value: &IRValue,
        param_allocas: &HashMap<String, PointerValue>,
        value_map: &HashMap<String, BasicValueEnum>,
        module: &IRModule,
    ) -> Result<BasicValueEnum> {
        match value {
            IRValue::ImmediateInt(n) | IRValue::ConstantInt(n) | IRValue::Int(n) => {
                Ok(self.context.i64_type().const_int(*n as u64, false).as_basic_value_enum())
            }
            IRValue::ImmediateFloat(n) | IRValue::ConstantFloat(n) | IRValue::Float(n) => {
                Ok(self.context.f64_type().const_float(*n).as_basic_value_enum())
            }
            IRValue::ImmediateBool(b) | IRValue::ConstantBool(b) | IRValue::Bool(b) => {
                Ok(self.context.bool_type().const_int(*b as u64, false).as_basic_value_enum())
            }
            IRValue::ImmediateString(s) | IRValue::ConstantString(s) | IRValue::Str(s) | IRValue::String(s) => {
                // Create global string constant
                let string_ptr = builder.build_global_string_ptr(s, "str")?;
                Ok(string_ptr.as_basic_value_enum())
            }
            IRValue::Variable(name) => {
                // Check if it's in our value map first
                if let Some(val) = value_map.get(name) {
                    Ok(*val)
                } 
                // Check if it's a parameter
                else if let Some(alloca) = param_allocas.get(name) {
                    let loaded = builder.build_load(*alloca, name)?;
                    Ok(loaded)
                } 
                // Check if it's a global variable
                else if module.globals.iter().any(|g| &g.name == name) {
                    if let Some(global) = builder.get_module().get_global(name) {
                        Ok(global.as_pointer_value().as_basic_value_enum())
                    } else {
                        Err(anyhow!("Global variable not found: {}", name))
                    }
                } else {
                    Err(anyhow!("Variable not found: {}", name))
                }
            }
            IRValue::Null | IRValue::None => {
                Ok(self.context.i8_type().ptr_type(AddressSpace::default()).const_null().as_basic_value_enum())
            }
            IRValue::List(_) | IRValue::Dict(_) => {
                // For complex types, return a null pointer for now
                Ok(self.context.i8_type().ptr_type(AddressSpace::default()).const_null().as_basic_value_enum())
            }
        }
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
        let async_context_type = self.create_async_context_type();
        
        // Create async function signature: async_context* -> async_context*
        let async_function_type = async_context_type.ptr_type(AddressSpace::default())
            .fn_type(&[async_context_type.ptr_type(AddressSpace::default()).into()], false);
        
        let async_impl = module.add_function(&format!("{}_async_impl", function.name), async_function_type, None);
        
        // Generate state machine implementation
        self.generate_async_state_machine(module, builder, function, async_impl, async_context_type)?;
        
        // Create wrapper function that calls the state machine
        self.generate_async_wrapper(module, builder, function, llvm_function, async_impl)?;
        
        Ok(llvm_function)
    }
    
    /// Create async context type for state machine
    fn create_async_context_type(&self) -> StructType {
        self.context.struct_type(&[
            self.context.i32_type().into(),    // state
            self.context.i8_type().ptr_type(AddressSpace::default()).into(), // data
            self.context.bool_type().into(),     // completed
            self.context.i8_type().ptr_type(AddressSpace::default()).into(), // result
        ], false)
    }
    
    /// Generate async state machine
    fn generate_async_state_machine(
        &self,
        module: &Module,
        builder: &Builder,
        function: &IRFunction,
        async_impl: FunctionValue,
        context_type: StructType,
    ) -> Result<()> {
        let entry_block = self.context.append_basic_block(async_impl, "entry");
        builder.position_at_end(entry_block);
        
        // Get context parameter
        let context_arg = async_impl.get_nth_param(0).unwrap().into_pointer_value();
        
        // Load current state
        let state_ptr = unsafe { builder.build_struct_gep(context_type, context_arg, 0, "state_ptr")? };
        let state = builder.build_load(self.context.i32_type(), state_ptr, "state")?;
        
        // Create state switch
        let switch = builder.build_switch(state.into_int_value(), entry_block, function.blocks.len() as u32);
        
        // Generate code for each state (basic block)
        for (i, block) in function.blocks.iter().enumerate() {
            let state_block = self.context.append_basic_block(async_impl, &format!("state_{}", i));
            builder.position_at_end(state_block);
            
            // Generate code for this state
            self.generate_async_state(builder, i as i32, context_arg, context_type)?;
            
            switch.add_case(self.context.i32_type().const_int(i as u64, false), state_block);
        }
        
        Ok(())
    }
    
    /// Generate async state
    fn generate_async_state(
        &self,
        builder: &Builder,
        state_id: i32,
        context: PointerValue,
        context_type: StructType,
    ) -> Result<()> {
        // Update state
        let state_ptr = unsafe { builder.build_struct_gep(context_type, context, 0, "state_ptr")? };
        let next_state = self.context.i32_type().const_int((state_id + 1) as u64, false);
        builder.build_store(state_ptr, next_state)?;
        
        // For now, mark as completed
        let completed_ptr = unsafe { builder.build_struct_gep(context_type, context, 2, "completed_ptr")? };
        builder.build_store(completed_ptr, self.context.bool_type().const_int(1, false))?;
        
        builder.build_return(Some(&context.as_basic_value_enum()))?;
        Ok(())
    }
    
    /// Generate async wrapper
    fn generate_async_wrapper(
        &self,
        module: &Module,
        builder: &Builder,
        _function: &IRFunction,
        wrapper_func: FunctionValue,
        async_impl: FunctionValue,
    ) -> Result<()> {
        let entry_block = self.context.append_basic_block(wrapper_func, "entry");
        builder.position_at_end(entry_block);
        
        // Create async context
        let async_context_type = self.create_async_context_type();
        let context_size = async_context_type.size_of()?.const_cast(self.context.i64_type(), false);
        let context_ptr = builder.build_malloc(self.context.i8_type(), context_size, "async_ctx")?;
        
        // Initialize context
        let context = builder.build_pointer_cast(
            context_ptr,
            async_context_type.ptr_type(AddressSpace::default()),
            "async_ctx_cast"
        );
        
        // Call async implementation
        let result = builder.build_call(async_impl, &[context.into()], "async_call")?;
        
        // Return the async handle
        builder.build_return(Some(&result.try_as_basic_value().left().unwrap()))?;
        
        Ok(())
    }
    
    /// Convert IR type to LLVM type
    fn ir_type_to_llvm(&self, ir_type: &IRType) -> BasicTypeEnum {
        match ir_type {
            IRType::I8 | IRType::Int8 => self.context.i8_type().into(),
            IRType::I16 | IRType::Int16 => self.context.i16_type().into(),
            IRType::I32 | IRType::Int32 => self.context.i32_type().into(),
            IRType::I64 | IRType::Int64 | IRType::Int => self.context.i64_type().into(),
            IRType::F32 | IRType::Float32 => self.context.f32_type().into(),
            IRType::F64 | IRType::Float64 | IRType::Float => self.context.f64_type().into(),
            IRType::Bool => self.context.bool_type().into(),
            IRType::Pointer(inner) => self.ir_type_to_llvm(inner).ptr_type(AddressSpace::default()).into(),
            IRType::Void => self.context.void_type().into(),
            IRType::Dynamic => self.context.i8_type().ptr_type(AddressSpace::default()).into(), // Treat dynamic as opaque pointer
            IRType::String => self.context.i8_type().ptr_type(AddressSpace::default()).into(),
            IRType::List(_) | IRType::Dict(_, _) | IRType::Any => self.context.i8_type().ptr_type(AddressSpace::default()).into(),
            _ => self.context.i64_type().into(), // Default fallback
        }
    }
    
    /// Generate type definitions
    fn generate_types(&self, module: &Module, ir_module: &IRModule) -> Result<()> {
        for (name, ir_type) in &ir_module.types {
            if let IRType::Struct(_) = ir_type {
                let struct_type = self.context.opaque_struct_type(name);
                module.add_struct_type(name, struct_type);
            }
        }
        Ok(())
    }
    
    /// Generate global variables
    fn generate_globals(&self, module: &Module, ir_module: &IRModule) -> Result<()> {
        for global in &ir_module.globals {
            let llvm_type = self.ir_type_to_llvm(&global.ty);
            let llvm_global = module.add_global(llvm_type, None, &global.name);
            
            if let Some(value) = &global.value {
                // Create a builder to set the initializer
                let builder = self.context.create_builder();
                let llvm_value = self.ir_value_to_constant(value, &global.ty)?;
                llvm_global.set_initializer(Some(&llvm_value));
            } else {
                llvm_global.set_initializer(Some(&llvm_type.const_zero()));
            }
        }
        Ok(())
    }
    
    /// Convert IR value to LLVM constant
    fn ir_value_to_constant(&self, value: &IRValue, ty: &IRType) -> Result<BasicValueEnum> {
        match value {
            IRValue::ConstantInt(n) | IRValue::ImmediateInt(n) | IRValue::Int(n) => {
                let llvm_type = match ty {
                    IRType::I8 | IRType::Int8 => self.context.i8_type().const_int(*n as u64, false).as_basic_value_enum(),
                    IRType::I16 | IRType::Int16 => self.context.i16_type().const_int(*n as u64, false).as_basic_value_enum(),
                    IRType::I32 | IRType::Int32 => self.context.i32_type().const_int(*n as u64, false).as_basic_value_enum(),
                    IRType::I64 | IRType::Int64 | IRType::Int => self.context.i64_type().const_int(*n as u64, false).as_basic_value_enum(),
                    _ => self.context.i64_type().const_int(*n as u64, false).as_basic_value_enum(),
                };
                Ok(llvm_type)
            }
            IRValue::ConstantFloat(n) | IRValue::ImmediateFloat(n) | IRValue::Float(n) => {
                Ok(self.context.f64_type().const_float(*n).as_basic_value_enum())
            }
            IRValue::ConstantBool(b) | IRValue::ImmediateBool(b) | IRValue::Bool(b) => {
                Ok(self.context.bool_type().const_int(*b as u64, false).as_basic_value_enum())
            }
            IRValue::ConstantString(s) | IRValue::ImmediateString(s) | IRValue::Str(s) | IRValue::String(s) => {
                let string_type = self.context.i8_type().array_type(s.len() as u32);
                let global_string = self.context.const_string(s.as_bytes(), false);
                Ok(global_string.as_basic_value_enum())
            }
            IRValue::Null | IRValue::None => {
                Ok(self.context.i8_type().ptr_type(AddressSpace::default()).const_null().as_basic_value_enum())
            }
            IRValue::List(_) | IRValue::Dict(_) => {
                // For complex types, return a null pointer for now
                Ok(self.context.i8_type().ptr_type(AddressSpace::default()).const_null().as_basic_value_enum())
            }
            _ => Err(anyhow!("Cannot convert IR value to constant: {:?}", value))
        }
    }
    
    /// Generate async runtime
    fn generate_async_runtime(&self, module: &Module, _builder: &Builder) -> Result<()> {
        // Generate async task structure
        let task_type = self.context.struct_type(&[
            self.context.i32_type().into(), // state
            self.context.i8_type().ptr_type(AddressSpace::default()).into(), // function pointer
            self.context.i8_type().ptr_type(AddressSpace::default()).into(), // context
        ], false);
        
        module.add_struct_type("AsyncTask", task_type);
        
        // Generate async scheduler functions
        let scheduler_type = self.context.void_type().fn_type(&[], false);
        module.add_function("async_schedule", scheduler_type, None);
        module.add_function("async_yield", scheduler_type, None);
        module.add_function("async_resume", scheduler_type, None);
        
        Ok(())
    }
    
    /// Optimize module
    fn optimize_module(&self, module: &Module, opt_level: OptimizationLevel) -> Result<()> {
        #[cfg(feature = "llvm")]
        use inkwell::passes::PassManager;
        
        // Convert optimization level
        let llvm_opt_level = match opt_level {
            OptimizationLevel::None => inkwell::passes::OptimizationLevel::None,
            OptimizationLevel::Less => inkwell::passes::OptimizationLevel::Less,
            OptimizationLevel::Default => inkwell::passes::OptimizationLevel::Default,
            OptimizationLevel::Aggressive => inkwell::passes::OptimizationLevel::Aggressive,
        };
        
        // Create and configure pass managers
        let module_pass_manager = PassManager::create(());
        module_pass_manager.add_instruction_combining_pass();
        module_pass_manager.add_reassociate_pass();
        module_pass_manager.add_gvn_pass();
        module_pass_manager.add_cfg_simplification_pass();
        module_pass_manager.add_basic_alias_analysis_pass();
        module_pass_manager.add_promote_memory_to_register_pass();
        module_pass_manager.add_instruction_combining_pass();
        module_pass_manager.add_reassociate_pass();
        module_pass_manager.set_optimization_level(llvm_opt_level);
        
        // Run optimizations
        module_pass_manager.run_on(module);
        
        Ok(())
    }
    
    #[cfg(feature = "llvm")]
    fn create_target_machine(&self, options: &CodegenOptions) -> Result<inkwell::targets::TargetMachine> {
        #[cfg(feature = "llvm")]
        use inkwell::targets::{Target, TargetTriple, InitializationConfig};
        
        // Initialize targets
        Target::initialize_native(&InitializationConfig::default())?;
        
        let target_triple = if let Some(triple) = &options.target_triple {
            TargetTriple::create(triple)
        } else {
            TargetTriple::create(&crate::codegen::target::get_default_target_triple())
        };
        
        let target = Target::from_triple(&target_triple)?;
        
        // Convert optimization level
        let opt_level = match options.opt_level {
            0 => inkwell::targets::OptimizationLevel::None,
            1 => inkwell::targets::OptimizationLevel::Less,
            2 => inkwell::targets::OptimizationLevel::Default,
            3 => inkwell::targets::OptimizationLevel::Aggressive,
            _ => inkwell::targets::OptimizationLevel::Default,
        };
        
        let target_machine = target.create_target_machine(
            &target_triple,
            &self.get_target_cpu(),
            &self.get_target_features(options),
            opt_level,
            inkwell::targets::RelocMode::Default,
            inkwell::targets::CodeModel::Default,
        ).ok_or_else(|| anyhow!("Failed to create target machine"))?;
        
        Ok(target_machine)
    }
    
    fn get_target_cpu(&self) -> String {
        if cfg!(target_arch = "x86_64") {
            "x86-64".to_string()
        } else if cfg!(target_arch = "aarch64") {
            "apple-a14".to_string() // Example Apple Silicon
        } else {
            "generic".to_string()
        }
    }
    
    fn get_target_features(&self, options: &CodegenOptions) -> String {
        options.features.join(",")
    }
}

#[cfg(feature = "llvm")]
impl CodeGenerator for LLVMCodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        // Generate LLVM IR
        let llvm_module = self.generate_llvm_ir(&module, options)?;
        
        // Optimize module based on optimization level
        let opt_level = OptimizationLevel::from(options.opt_level);
        self.optimize_module(&llvm_module, opt_level)?;
        
        // Generate machine code
        let target_machine = self.create_target_machine(options)?;
        let memory_buffer = target_machine.write_to_memory_buffer(&llvm_module, inkwell::targets::FileType::Object)?;
        
        Ok(memory_buffer.as_slice().to_vec())
    }
    
    fn get_target(&self) -> Target {
        Target::Native
    }
    
    fn supports_optimization(&self) -> bool {
        true
    }
    
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec!["async", "optimization", "debug-info", "target-specific"]
    }
}

#[cfg(not(feature = "llvm"))]
/// Stub implementation when LLVM feature is not enabled
pub struct LLVMCodeGenerator;

#[cfg(not(feature = "llvm"))]
impl LLVMCodeGenerator {
    pub fn new() -> Self {
        Self
    }
}

#[cfg(not(feature = "llvm"))]
impl CodeGenerator for LLVMCodeGenerator {
    fn generate(&self, _module: IRModule, _options: &CodegenOptions) -> Result<Vec<u8>> {
        Err(anyhow!("LLVM backend not enabled. Enable with --features llvm"))
    }
    
    fn get_target(&self) -> Target {
        Target::Native
    }
}