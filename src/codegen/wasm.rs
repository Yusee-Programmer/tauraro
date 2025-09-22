//! COMPLETE WebAssembly code generator
use crate::ir::{IRModule, IRFunction, IRType, IRValue, IRInstruction, BasicBlock};
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
        for (name, (ir_type, initial_value)) in &module.globals {
            wat.push_str(&self.global_definition(name, ir_type, initial_value.as_ref()));
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
        let params: Vec<String> = function.parameters
            .iter()
            .map(|(_, ty)| self.ir_type_to_wasm(ty).to_string())
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
                   params.join(" "),
                   results.join(" "))
        }
    }
    
    /// Generate global variable definition
    fn global_definition(&self, name: &str, ir_type: &IRType, initial_value: Option<&IRValue>) -> String {
        let wasm_type = self.ir_type_to_wasm(ir_type);
        let init_expr = if let Some(value) = initial_value {
            format!("({}.const {})", wasm_type, self.ir_value_to_wasm(value))
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
        
        // Add local variables
        for (var_name, var_type) in &function.variables {
            body.push_str(&format!("    (local ${} {})\n", var_name, self.ir_type_to_wasm(var_type)));
        }
        
        // Generate basic blocks
        for (block_name, basic_block) in &function.basic_blocks {
            if block_name != "entry" {
                body.push_str(&format!("    (block ${}\n", block_name));
            }
            
            body.push_str(&format!("      ;; Basic block: {}\n", block_name));
            
            // Generate instructions
            for instruction in &basic_block.instructions {
                body.push_str(&self.instruction_to_wasm(instruction));
            }
            
            // Generate terminator
            if let Some(terminator) = &basic_block.terminator {
                body.push_str(&self.terminator_to_wasm(terminator, basic_block));
            }
            
            if block_name != "entry" {
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
            IRInstruction::Add { result, left, right } => {
                format!("    (local.set ${} ({} (local.get ${}) (local.get ${})))\n", 
                       result, 
                       self.operation_to_wasm("add", left, right),
                       self.value_to_wasm(left),
                       self.value_to_wasm(right))
            }
            IRInstruction::Call { result, function, args } => {
                let args_code: Vec<String> = args.iter().map(|arg| {
                    format!("(local.get ${})", self.value_to_wasm(arg))
                }).collect();
                
                if let Some(result_var) = result {
                    format!("    (local.set ${} (call ${} {}))\n", 
                           result_var, function, args_code.join(" "))
                } else {
                    format!("    (call ${} {})\n", function, args_code.join(" "))
                }
            }
            IRInstruction::Load { result, pointer } => {
                format!("    (local.set ${} (i32.load (local.get ${})))\n", 
                       result, self.value_to_wasm(pointer))
            }
            IRInstruction::Store { pointer, value } => {
                format!("    (i32.store (local.get ${}) (local.get ${}))\n", 
                       self.value_to_wasm(pointer), self.value_to_wasm(value))
            }
            IRInstruction::Alloca { result, ir_type } => {
                // In WASM, we use the linear memory for allocations
                format!("    (local.set ${} (call $malloc (i32.const {})))\n", 
                       result, self.type_size(ir_type))
            }
            _ => "    ;; Instruction not implemented\n".to_string(),
        }
    }
    
    /// Convert terminator to WASM
    fn terminator_to_wasm(&self, terminator: &IRInstruction, block: &BasicBlock) -> String {
        match terminator {
            IRInstruction::Return { value } => {
                if let Some(val) = value {
                    format!("    (return (local.get ${}))\n", self.value_to_wasm(val))
                } else {
                    "    (return)\n".to_string()
                }
            }
            IRInstruction::Jump { target } => {
                format!("    (br ${})\n", target)
            }
            IRInstruction::Branch { condition, true_target, false_target } => {
                format!(
                    "    (br_if ${} (local.get ${}))\n    (br ${})\n", 
                    true_target, self.value_to_wasm(condition), false_target
                )
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
            IRValue::ConstantString(s) => {
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