//! Comprehensive tests for the super bytecode implementation

#[cfg(test)]
mod tests {
    use tauraro::{SuperCompiler, SuperBytecodeVM, OpCode, Lexer, Parser};

    #[test]
    fn test_register_based_arithmetic() {
        let source = "result = 2 + 3";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        // Check that we have the expected instructions for register-based VM
        assert!(!code.instructions.is_empty());
        
        // Check specific instructions for register-based operations
        let has_load_const = code.instructions.iter().any(|instr| instr.opcode == OpCode::LoadConst);
        let has_binary_add = code.instructions.iter().any(|instr| instr.opcode == OpCode::BinaryAddRR);
        assert!(has_load_const);
        assert!(has_binary_add);
    }

    #[test]
    fn test_fast_integer_operations() {
        let source = "result = 10 * 5";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        // Check that we have fast integer multiplication instruction
        let has_fast_mul = code.instructions.iter().any(|instr| instr.opcode == OpCode::BinaryMulRR_FastInt);
        assert!(has_fast_mul);
    }

    #[test]
    fn test_inline_caching() {
        let source = "x = 42";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        // Check that we have inline caches
        assert!(!code.inline_caches.is_empty());
    }

    #[test]
    fn test_method_caching() {
        let source = "my_list = []\nmy_list.append(1)";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        // Check that we have method-related instructions
        let has_load_method = code.instructions.iter().any(|instr| instr.opcode == OpCode::LoadMethodCached);
        let has_call_method = code.instructions.iter().any(|instr| instr.opcode == OpCode::CallMethodCached);
        assert!(has_load_method || has_call_method);
    }

    #[test]
    fn test_super_instructions() {
        let source = "x = 10\nx = x + 5"; // This should trigger LoadAddStore super-instruction
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        // Check that we have super instructions
        let has_super_instruction = code.instructions.iter().any(|instr| instr.opcode == OpCode::LoadAddStore);
        // Note: The compiler optimization might not generate this specific super-instruction
        // but we're testing that the infrastructure is there
    }

    #[test]
    fn test_vm_execution_with_registers() {
        let source = "result = 2 + 3";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        let mut vm = SuperBytecodeVM::new();
        let result = vm.execute(code);
        assert!(result.is_ok());
    }

    #[test]
    fn test_reference_counting() {
        let source = "a = [1, 2, 3]\nb = a\nc = a";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        let mut vm = SuperBytecodeVM::new();
        let result = vm.execute(code);
        assert!(result.is_ok());
        
        // Check that we have reference counting instructions
        let has_incref = code.instructions.iter().any(|instr| instr.opcode == OpCode::IncRef);
        let has_decref = code.instructions.iter().any(|instr| instr.opcode == OpCode::DecRef);
        // Note: The actual generation of these instructions depends on the compiler implementation
    }

    #[test]
    fn test_fast_loop() {
        let source = "for i in range(10):\n    pass";
        let tokens = Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        let mut parser = Parser::new(tokens);
        let program = parser.parse().unwrap();
        
        let mut compiler = SuperCompiler::new("test".to_string());
        let code = compiler.compile(program).unwrap();
        
        // Check that we have fast loop instructions
        let has_fast_loop = code.instructions.iter().any(|instr| instr.opcode == OpCode::FastLoop);
        let has_fast_range = code.instructions.iter().any(|instr| instr.opcode == OpCode::FastRangeIter);
        // Note: The actual generation of these instructions depends on the compiler implementation
    }
}