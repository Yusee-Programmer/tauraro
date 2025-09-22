//! WebAssembly code generator
use crate::ir::{IRModule, IRFunction, IRType, IRValue};
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
        
        // Generate type definitions
        wat.push_str("  ;; Type definitions\n");
        for function in &module.functions {
            wat.push_str(&self.function_type_definition(function));
        }
        wat.push_str("\n");
        
        // Generate function implementations
        wat.push_str("  ;; Function implementations\n");
        for function in &module.functions {
            wat.push_str(&self.function_implementation(function, options));
        }
        wat.push_str("\n");
        
        // Generate memory section
        wat.push_str("  ;; Memory configuration\n");
        wat.push_str(&self.generate_memory_section());
        wat.push_str("\n");
        
        // Generate async support if needed
        if module.functions.iter().any(|f| f.is_async) {
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
            .map(|(_, ty)| self.ir_type_to_wasm(ty))
            .collect();
        
        let results = if function.return_type == IRType::Void {
            vec![]
        } else {
            vec![self.ir_type_to_wasm(&function.return_type)]
        };
        
        if function.is_async {
            // Async functions use the async extension
            format!("  (type ${}_type (func (result i32)))\n", function.name)
        } else {
            format!("  (type ${}_type (func (param {}) (result {})))\n", 
                   function.name, 
                   params.join(" "),
                   results.join(" "))
        }
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
        impl_code
    }
    
    /// Generate synchronous function
    fn generate_sync_function(&self, function: &IRFunction, options: &CodegenOptions) -> String {
        let mut body = String::new();
        
        // Add type annotations for static typing
        if options.memory_mode == crate::runtime::MemoryMode::Manual {
            body.push_str("    ;; Manual memory management enabled\n");
        }
        
        // Generate local variables
        for (var_name, var_type) in &function.variables {
            body.push_str(&format!("    (local ${} {})\n", var_name, self.ir_type_to_wasm(var_type)));
        }
        
        // Generate basic blocks (simplified)
        for basic_block in &function.basic_blocks {
            body.push_str(&format!("    ;; Basic block: {}\n", basic_block.name));
            
            for instruction in &basic_block.instructions {
                body.push_str(&self.instruction_to_wasm(instruction));
            }
            
            if let Some(terminator) = &basic_block.terminator {
                body.push_str(&self.terminator_to_wasm(terminator));
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
    
    block $async_block
      loop $async_loop
        (local.set $state (call $async_get_state))
        
        ;; State machine dispatch
        (block
          (block
            (block
              (br_table $state
                (case 0 $state_0)
                (case 1 $state_1)
                (case 2 $state_2)
                (default $async_complete)
              )
            )
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
      end $async_loop
    end $async_block
"#)
    }
    
    /// Convert IR instruction to WASM
    fn instruction_to_wasm(&self, instruction: &crate::ir::IRInstruction) -> String {
        match instruction {
            crate::ir::IRInstruction::Add { result, left, right } => {
                format!("    (local.set ${} ({} (local.get ${}) (local.get ${})))\n", 
                       result, 
                       self.operation_to_wasm("add", left, right),
                       self.value_to_wasm(left),
                       self.value_to_wasm(right))
            }
            crate::ir::IRInstruction::Call { result, function, args } => {
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
            _ => "    ;; Instruction not implemented\n".to_string(),
        }
    }
    
    /// Convert terminator to WASM
    fn terminator_to_wasm(&self, terminator: &crate::ir::IRInstruction) -> String {
        match terminator {
            crate::ir::IRInstruction::Return { value } => {
                if let Some(val) = value {
                    format!("    (return (local.get ${}))\n", self.value_to_wasm(val))
                } else {
                    "    (return)\n".to_string()
                }
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
            _ => "0".to_string(), // Default
        }
    }
    
    /// Convert operation to WASM opcode
    fn operation_to_wasm(&self, op: &str, left: &IRValue, right: &IRValue) -> String {
        // Determine the type prefix based on operand types
        let type_prefix = match (left, right) {
            (IRValue::ConstantInt(_), _) | (_, IRValue::ConstantInt(_)) => "i32.",
            (IRValue::ConstantFloat(_), _) | (_, IRValue::ConstantFloat(_)) => "f64.",
            _ => "i32.",
        };
        
        format!("{}{}", type_prefix, op)
    }
    
    /// Generate memory section
    fn generate_memory_section(&self) -> String {
        r#"  (memory $memory 1)  ;; 1 page = 64KB
  (export "memory" (memory $memory))
  
  ;; Memory management functions
  (func $malloc (param $size i32) (result i32)
    ;; Simplified malloc implementation
    (i32.const 0)  ;; Return null pointer for now
  )
  
  (func $free (param $ptr i32)
    ;; Simplified free implementation
    nop
  )
"#.to_string()
    }
    
    /// Generate async support functions
    fn generate_async_support(&self) -> String {
        r#"  ;; Async runtime functions
  (func $async_get_state (result i32)
    ;; Get current async state
    (i32.const 0)
  )
  
  (func $async_set_state (param $state i32)
    ;; Set async state
    nop
  )
  
  (func $async_yield)
    ;; Yield execution to scheduler
    nop
  )
  
  (func $async_resume (param $task i32) (result i32)
    ;; Resume async task
    (i32.const 1)  ;; Return completed status
  )
"#.to_string()
    }
    
    /// Generate export section
    fn generate_exports(&self, module: &IRModule) -> String {
        let mut exports = String::new();
        
        for function in &module.functions {
            if function.name.starts_with("export_") || !function.name.starts_with("_") {
                exports.push_str(&format!("  (export \"{}\" (func ${}))\n", function.name, function.name));
            }
        }
        
        exports
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
}

impl WASMCodeGenerator {
    /// Convert WAT text to WASM binary
    fn wat_to_wasm(&self, wat: &str) -> Result<Vec<u8>> {
        // In a real implementation, this would use a WAT parser like wat::parse_str
        // For now, return a simple WASM module structure
        
        // Minimal WASM module (empty for demonstration)
        let mut wasm = Vec::new();
        
        // WASM magic number and version
        wasm.extend_from_slice(b"\x00asm");
        wasm.extend_from_slice(b"\x01\x00\x00\x00");
        
        // For now, return the WAT as bytes (in real implementation, compile to binary)
        wasm.extend_from_slice(wat.as_bytes());
        
        Ok(wasm)
    }
}