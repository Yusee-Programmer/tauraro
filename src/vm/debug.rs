//! Debug hooks, tracing, disassembly
use crate::value::Value;
use crate::ast::Program;
use crate::bytecode::instructions::Instruction;
use std::collections::HashMap;

/// Debug utilities for the VM
pub struct Debugger {
    /// Enable/disable debugging
    enabled: bool,
    
    /// Trace function calls
    trace_calls: bool,
    
    /// Trace variable assignments
    trace_variables: bool,
    
    /// Trace execution steps
    trace_execution: bool,
    
    /// Call stack depth for tracing
    call_depth: usize,
}

impl Debugger {
    pub fn new() -> Self {
        Self {
            enabled: false,
            trace_calls: false,
            trace_variables: false,
            trace_execution: false,
            call_depth: 0,
        }
    }
    
    /// Enable debugging
    pub fn enable(&mut self) {
        self.enabled = true;
    }
    
    /// Disable debugging
    pub fn disable(&mut self) {
        self.enabled = false;
    }
    
    /// Enable call tracing
    pub fn enable_call_tracing(&mut self) {
        self.trace_calls = true;
    }
    
    /// Enable variable tracing
    pub fn enable_variable_tracing(&mut self) {
        self.trace_variables = true;
    }
    
    /// Enable execution tracing
    pub fn enable_execution_tracing(&mut self) {
        self.trace_execution = true;
    }
    
    /// Trace a function call
    pub fn trace_call(&mut self, function_name: &str, args: &[Value]) {
        if self.enabled && self.trace_calls {
            println!("{}Calling function '{}' with {} arguments", 
                     "  ".repeat(self.call_depth), 
                     function_name, 
                     args.len());
            self.call_depth += 1;
        }
    }
    
    /// Trace a function return
    pub fn trace_return(&mut self, function_name: &str, result: &Value) {
        if self.enabled && self.trace_calls {
            self.call_depth = self.call_depth.saturating_sub(1);
            println!("{}Function '{}' returned: {:?}", 
                     "  ".repeat(self.call_depth), 
                     function_name, 
                     result);
        }
    }
    
    /// Trace a variable assignment
    pub fn trace_variable_assignment(&self, name: &str, value: &Value) {
        if self.enabled && self.trace_variables {
            println!("Assigned variable '{}' = {:?}", name, value);
        }
    }
    
    /// Trace an execution step
    pub fn trace_execution_step(&self, instruction: &Instruction, pc: usize) {
        if self.enabled && self.trace_execution {
            println!("Executing instruction {}: {:?}", pc, instruction);
        }
    }
}

/// Disassembler for bytecode
pub struct Disassembler;

impl Disassembler {
    /// Disassemble a program
    pub fn disassemble_program(program: &Program) -> String {
        // This would contain the logic to disassemble a program
        format!("Disassembled program with {} statements", program.statements.len())
    }
    
    /// Disassemble bytecode instructions
    pub fn disassemble_bytecode(instructions: &[Instruction]) -> String {
        let mut result = String::new();
        for (i, instruction) in instructions.iter().enumerate() {
            result.push_str(&format!("{}: {:?}\n", i, instruction));
        }
        result
    }
}

/// Profiler for VM execution
pub struct Profiler {
    /// Function call counts
    function_calls: HashMap<String, usize>,
    
    /// Execution times for functions
    function_times: HashMap<String, std::time::Duration>,
    
    /// Total execution time
    total_time: std::time::Duration,
}

impl Profiler {
    pub fn new() -> Self {
        Self {
            function_calls: HashMap::new(),
            function_times: HashMap::new(),
            total_time: std::time::Duration::new(0, 0),
        }
    }
    
    /// Start profiling
    pub fn start(&mut self) -> std::time::Instant {
        std::time::Instant::now()
    }
    
    /// End profiling and record results
    pub fn end(&mut self, function_name: &str, start: std::time::Instant) {
        let duration = start.elapsed();
        *self.function_calls.entry(function_name.to_string()).or_insert(0) += 1;
        let current_time = self.function_times.entry(function_name.to_string()).or_insert(std::time::Duration::new(0, 0));
        *current_time += duration;
        self.total_time += duration;
    }
    
    /// Print profiling results
    pub fn print_results(&self) {
        println!("Profiling Results:");
        println!("Total execution time: {:?}", self.total_time);
        println!("Function call counts:");
        for (function, count) in &self.function_calls {
            println!("  {}: {} calls", function, count);
        }
        println!("Function execution times:");
        for (function, time) in &self.function_times {
            println!("  {}: {:?}", function, time);
        }
    }
}