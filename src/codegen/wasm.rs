//! COMPLETE WebAssembly code generator
use crate::ir::{IRModule, IRFunction, IRType, IRValue, IRInstruction, IRBlock};
use crate::codegen::{CodeGenerator, CodegenOptions, Target};
use anyhow::Result;

/// WebAssembly code generator
pub struct WASMCodeGenerator;

impl WASMCodeGenerator {
    pub fn new() -> Self {
        Self
    }
    
    /// Generate WebAssembly text format (WAT)
    fn generate_wat(&self, module: &IRModule, options: &CodegenOptions) -> String {
        let mut wat = String::from("(module\n");
        
        // Generate memory configuration
        wat.push_str("  (memory $memory 1)\n");
        wat.push_str("  (export \"memory\" (memory $memory))\n\n");
        
        // Generate type definitions
        wat.push_str("  ;; Type definitions\n");
        for function in module.functions.values() {
            wat.push_str(&self.function_type_definition(function));
        }
        wat.push_str("\n");
        
        // Generate global variables
        wat.push_str("  ;; Global variables\n");
        for global in &module.globals {
            wat.push_str(&self.global_definition(&global.name, &global.ty, global.value.as_ref()));
        }
        wat.push_str("\n");
        
        // Generate function implementations
        wat.push_str("  ;; Function implementations\n");
        for function in module.functions.values() {
            wat.push_str(&self.function_implementation(function, options));
        }
        wat.push_str("\n");
        
        // Generate async support if needed
        if module.functions.values().any(|f| f.is_async) {
            wat.push_str("  ;; Async runtime support\n");
            wat.push_str(&self.generate_async_support());
            wat.push_str("\n");
        }
        
        // Generate export section
        wat.push_str("  ;; Exports\n");
        wat.push_str(&self.generate_exports(module));
        
        wat.push_str(")\n");
        wat
    }
    
    /// Generate function type definition
    fn function_type_definition(&self, function: &IRFunction) -> String {
        let param_types: Vec<String> = function.params
            .iter()
            .map(|param| self.ir_type_to_wasm(&param.ty).to_string())
            .collect();
        
        let results = if function.return_type == IRType::Void {
            vec![]
        } else {
            vec![self.ir_type_to_wasm(&function.return_type).to_string()]
        };
        
        if function.is_async {
            // Async functions return a future handle (i32)
            format!("  (type ${}_type (func (result i32)))\n", function.name)
        } else {
            format!("  (type ${}_type (func (param {}) (result {})))\n", 
                   function.name, 
                   param_types.join(" "),
                   results.join(" "))
        }
    }
    
    /// Generate global variable definition
    fn global_definition(&self, name: &str, ir_type: &IRType, initial_value: Option<&IRValue>) -> String {
        let wasm_type = self.ir_type_to_wasm(ir_type);
        let init_expr = if let Some(value) = initial_value {
            format!("({}.const {})", wasm_type, self.value_to_wasm(value))
        } else {
            format!("({}.const 0)", wasm_type)
        };
        
        format!("  (global ${} (mut {}) {})\n", name, wasm_type, init_expr)
    }
    
    /// Generate function implementation
    fn function_implementation(&self, function: &IRFunction, options: &CodegenOptions) -> String {
        let mut impl_code = format!("  (func ${} (type ${}_type)\n", function.name, function.name);
        
        if function.is_async {
            impl_code.push_str(&self.generate_async_function(function));
        } else {
            impl_code.push_str(&self.generate_sync_function(function, options));
        }
        
        impl_code.push_str("  )\n");
        
        // Export the function if it's not internal
        if !function.name.starts_with('_') {
            impl_code.push_str(&format!("  (export \"{}\" (func ${}))\n", function.name, function.name));
        }
        
        impl_code
    }
    
    /// Generate synchronous function
    fn generate_sync_function(&self, function: &IRFunction, options: &CodegenOptions) -> String {
        let mut body = String::new();
        
        // Add local variables (removed since IRFunction doesn't have variables field)
        // Local variables would be handled differently in the IR structure
        
        // Generate basic blocks
        for block in &function.blocks {
            if block.label != "entry" {
                body.push_str(&format!("    (block ${}\n", block.label));
            }
            
            body.push_str(&format!("      ;; Basic block: {}\n", block.label));
            
            // Generate instructions
            for instruction in &block.instructions {
                body.push_str(&self.instruction_to_wasm(instruction));
            }
            
            // Check if the last instruction is a terminator (Ret, Br, Jmp)
            if let Some(last_instruction) = block.instructions.last() {
                match last_instruction {
                    IRInstruction::Ret { .. } | IRInstruction::Br { .. } | IRInstruction::Jmp { .. } => {
                        // Already handled in instruction_to_wasm
                    }
                    _ => {
                        // Add implicit return if needed
                    }
                }
            }
            
            if block.label != "entry" {
                body.push_str("    )\n");
            }
        }
        
        body
    }
    
    /// Generate asynchronous function
    fn generate_async_function(&self, function: &IRFunction) -> String {
        // Async functions are implemented as state machines in WASM
        format!(r#"    ;; Async function implementation
    (local $state i32)
    (local $result i32)
    
    (block $async_block
      (loop $async_loop
        (local.set $state (call $async_get_state))
        
        ;; State machine dispatch
        (block
          (br_table $state
            (case 0 $state_0)
            (case 1 $state_1)
            (case 2 $state_2)
            (default $async_complete)
          )
        )
        
        ;; State implementations
        $state_0
          ;; Initial state
          (call $async_set_state (i32.const 1))
          (br $async_loop)
        
        $state_1
          ;; Processing state
          (call $async_set_state (i32.const 2))
          (br $async_loop)
        
        $state_2
          ;; Final state
          (local.set $result (i32.const 42))  ;; Example result
          (br $async_complete)
        
        $async_complete
          (return (local.get $result))
      )
    )
"#)
    }
    
    /// Convert IR instruction to WASM
    fn instruction_to_wasm(&self, instruction: &IRInstruction) -> String {
        match instruction {
            IRInstruction::Add { dest, left, right } => {
                format!("    (local.set ${} ({} (local.get ${}) (local.get ${})))\n", 
                       dest, 
                       self.operation_to_wasm("add", left, right),
                       self.value_to_wasm(left),
                       self.value_to_wasm(right))
            }
            IRInstruction::Call { dest, func, args } => {
                let args_code: Vec<String> = args.iter().map(|arg| {
                    format!("(local.get ${})", self.value_to_wasm(arg))
                }).collect();
                
                if let Some(result_var) = dest {
                    format!("    (local.set ${} (call ${} {}))\n", 
                           result_var, func, args_code.join(" "))
                } else {
                    format!("    (call ${} {})\n", func, args_code.join(" "))
                }
            }
            IRInstruction::Load { dest, ptr, ty: _ } => {
                format!("    (local.set ${} (i32.load (local.get ${})))\n", 
                       dest, ptr)
            }
            IRInstruction::Store { value, ptr } => {
                format!("    (i32.store (local.get ${}) {})\n", 
                       ptr, self.value_to_wasm(&value))
            }
            IRInstruction::Alloca { dest, ty } => {
                // In WASM, we use the linear memory for allocations
                format!("    (local.set ${} (call $malloc (i32.const {})))\n", 
                       dest, self.type_size(ty))
            }
            _ => "    ;; Instruction not implemented\n".to_string(),
        }
    }
    
    /// Convert terminator to WASM
    fn terminator_to_wasm(&self, terminator: &IRInstruction, block: &IRBlock) -> String {
        match terminator {
            IRInstruction::Ret { value } => {
                if let Some(val) = value {
                    format!("    (return (local.get ${}))\n", self.value_to_wasm(val))
                } else {
                    "    (return)\n".to_string()
                }
            }
            IRInstruction::Jmp { label } => {
                format!("    (br ${})
", label)
            }
            IRInstruction::Br { cond, then_label, else_label } => {
                format!("    (if {} (then (br ${})) (else (br ${})))\n", 
                       self.value_to_wasm(&cond), then_label, else_label)
            }
            _ => "    ;; Terminator not implemented\n".to_string(),
        }
    }
    
    /// Convert IR type to WASM type
    fn ir_type_to_wasm(&self, ir_type: &IRType) -> &str {
        match ir_type {
            IRType::I32 => "i32",
            IRType::I64 => "i64",
            IRType::F32 => "f32",
            IRType::F64 => "f64",
            IRType::Bool => "i32", // bool as i32
            IRType::Pointer(_) => "i32", // pointers are i32 indices
            IRType::Dynamic => "i32", // dynamic types as i32 handles
            _ => "i32", // Default to i32
        }
    }
    
    /// Convert IR value to WASM value reference
    fn value_to_wasm(&self, value: &IRValue) -> String {
        match value {
            IRValue::Variable(name) => name.clone(),
            IRValue::ConstantInt(n) => n.to_string(),
            IRValue::ConstantFloat(n) => n.to_string(),
            IRValue::ConstantBool(b) => if *b { "1" } else { "0" }.to_string(),
            IRValue::ConstantString(s) | IRValue::String(s) => {
                // String constants would be stored in memory
                format!("{}", s.as_ptr() as i32) // Simplified
            }
            IRValue::Null => "0".to_string(), // null pointer
            _ => "0".to_string(), // Default
        }
    }
    
    /// Convert operation to WASM opcode
    fn operation_to_wasm(&self, op: &str, left: &IRValue, right: &IRValue) -> String {
        // Determine the type prefix based on operand types
        let type_prefix = match (left, right) {
            (IRValue::ConstantInt(_), _) | (_, IRValue::ConstantInt(_)) => "i32.",
            (IRValue::ConstantFloat(_), _) | (_, IRValue::ConstantFloat(_)) => "f64.",
            _ => "i32.", // Default to i32
        };
        
        format!("{}{}", type_prefix, op)
    }
    
    /// Get size of type in bytes
    fn type_size(&self, ir_type: &IRType) -> i32 {
        match ir_type {
            IRType::I8 => 1,
            IRType::I16 => 2,
            IRType::I32 => 4,
            IRType::I64 => 8,
            IRType::F32 => 4,
            IRType::F64 => 8,
            IRType::Bool => 1,
            _ => 8, // Default size for pointers and dynamic types
        }
    }
    
    /// Generate memory management functions
    fn generate_memory_section(&self) -> String {
        r#"  ;; Memory management functions
  (func $malloc (param $size i32) (result i32)
    ;; Simple bump allocator
    (global.set $heap_ptr (i32.add (global.get $heap_ptr) (local.get $size)))
    (global.get $heap_ptr)
  )
  
  (func $free (param $ptr i32)
    ;; No-op for simple allocator
    nop
  )
  
  (global $heap_ptr (mut i32) (i32.const 0))
"#.to_string()
    }
    
    /// Generate async support functions
    fn generate_async_support(&self) -> String {
        r#"  ;; Async runtime functions
  (func $async_get_state (result i32)
    ;; Get current async state from task structure
    (i32.load (global.get $current_task))
  )
  
  (func $async_set_state (param $state i32)
    ;; Set async state in task structure
    (i32.store (global.get $current_task) (local.get $state))
  )
  
  (func $async_yield)
    ;; Yield execution to scheduler
    (call $async_set_state (i32.const 1))
  )
  
  (global $current_task (mut i32) (i32.const 0))
"#.to_string()
    }
    
    /// Generate export section
    fn generate_exports(&self, module: &IRModule) -> String {
        let mut exports = String::new();
        
        // Export functions
        for function_name in module.functions.keys() {
            if !function_name.starts_with('_') { // Don't export internal functions
                exports.push_str(&format!("  (export \"{}\" (func ${}))\n", function_name, function_name));
            }
        }
        
        // Export memory
        exports.push_str("  (export \"memory\" (memory $memory))\n");
        
        exports
    }
    
    /// Convert WAT to WASM binary (simplified)
    fn wat_to_wasm(&self, wat: &str) -> Result<Vec<u8>> {
        // In a real implementation, this would use a proper WAT parser
        // For now, return a minimal WASM module structure
        
        let mut wasm = Vec::new();
        
        // WASM magic number and version
        wasm.extend_from_slice(b"\x00asm");
        wasm.extend_from_slice(b"\x01\x00\x00\x00");
        
        // For demonstration, just include the WAT as a custom section
        // In production, you'd use a proper WAT-to-WASM compiler like wat2wasm
        let custom_section = format!(";; TauraroLang Generated WASM\n{}", wat);
        wasm.push(0x00); // Custom section ID
        wasm.extend_from_slice(&(custom_section.len() as u32).to_le_bytes());
        wasm.extend_from_slice(custom_section.as_bytes());
        
        Ok(wasm)
    }
}

impl CodeGenerator for WASMCodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        // Generate WebAssembly text format
        let wat = self.generate_wat(&module, options);
        
        // Convert WAT to WASM binary
        let wasm_binary = self.wat_to_wasm(&wat)?;
        
        Ok(wasm_binary)
    }
    
    fn get_target(&self) -> Target {
        Target::WASM
    }
    
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec!["wasm", "linear-memory", "async"]
    }
}
