#[test]
fn test_debug_comparison() {
    use crate::ast::*;
    
    println!("=== DEBUG COMPARISON TEST ===");
    
    // Create a simple comparison expression: x < y
    let comparison_expr = Expr::Compare {
        left: Box::new(Expr::Identifier("x".to_string())),
        ops: vec![CompareOp::Lt],
        comparators: vec![Expr::Identifier("y".to_string())],
    };
    
    let mut compiler = SuperCompiler::new("debug_test.py".to_string());
    
    // Add variables to varnames so they use LoadFast
    compiler.code.add_varname("x".to_string());
    compiler.code.add_varname("y".to_string());
    
    println!("Compiling comparison: x < y");
    match compiler.compile_expression(comparison_expr) {
        Ok(()) => {
            println!("✅ Comparison compiled successfully");
            println!("Generated instructions:");
            for (i, instr) in compiler.code.instructions.iter().enumerate() {
                println!("  {}: {:?} arg={:?}", i, instr.opcode, instr.arg);
            }
            
            // Test execution
            let mut vm = BytecodeVM::new();
            
            // Set up variables
            vm.globals.insert("x".to_string(), Value::Int(5));
            vm.globals.insert("y".to_string(), Value::Int(10));
            
            println!("Executing with x=5, y=10");
            match vm.execute(compiler.code.clone()) {
                Ok(result) => {
                    println!("✅ Execution successful, result: {:?}", result);
                }
                Err(e) => {
                    println!("❌ Execution failed: {}", e);
                }
            }
        }
        Err(e) => {
            println!("❌ Compilation failed: {}", e);
        }
    }
    
    // Test with subscript comparison: arr[i] < x
    println!("\n=== TESTING SUBSCRIPT COMPARISON ===");
    let subscript_comparison = Expr::Compare {
        left: Box::new(Expr::Subscript {
            object: Box::new(Expr::Identifier("arr".to_string())),
            index: Box::new(Expr::Identifier("i".to_string())),
        }),
        ops: vec![CompareOp::Lt],
        comparators: vec![Expr::Identifier("x".to_string())],
    };
    
    let mut compiler2 = SuperCompiler::new("debug_test2.py".to_string());
    compiler2.code.add_varname("arr".to_string());
    compiler2.code.add_varname("i".to_string());
    compiler2.code.add_varname("x".to_string());
    
    println!("Compiling comparison: arr[i] < x");
    match compiler2.compile_expression(subscript_comparison) {
        Ok(()) => {
            println!("✅ Subscript comparison compiled successfully");
            println!("Generated instructions:");
            for (i, instr) in compiler2.code.instructions.iter().enumerate() {
                println!("  {}: {:?} arg={:?}", i, instr.opcode, instr.arg);
            }
            
            // Test execution
            let mut vm = BytecodeVM::new();
            
            // Set up variables
            vm.globals.insert("arr".to_string(), Value::List(vec![
                Value::Int(1), Value::Int(3), Value::Int(5)
            ]));
            vm.globals.insert("i".to_string(), Value::Int(1)); // index 1 = value 3
            vm.globals.insert("x".to_string(), Value::Int(4)); // 3 < 4 = true
            
            println!("Executing with arr=[1,3,5], i=1, x=4 (should be true)");
            match vm.execute(compiler2.code.clone()) {
                Ok(result) => {
                    println!("✅ Execution successful, result: {:?}", result);
                    assert_eq!(result, Value::Bool(true));
                }
                Err(e) => {
                    println!("❌ Execution failed: {}", e);
                }
            }
        }
        Err(e) => {
            println!("❌ Compilation failed: {}", e);
        }
    }
    
    // Test the exact expression from your benchmark: x < sorted_list[i]
    println!("\n=== TESTING BENCHMARK EXPRESSION ===");
    let benchmark_expr = Expr::Compare {
        left: Box::new(Expr::Identifier("x".to_string())),
        ops: vec![CompareOp::Lt],
        comparators: vec![Expr::Subscript {
            object: Box::new(Expr::Identifier("sorted_list".to_string())),
            index: Box::new(Expr::Identifier("i".to_string())),
        }],
    };
    
    let mut compiler3 = SuperCompiler::new("debug_test3.py".to_string());
    compiler3.code.add_varname("x".to_string());
    compiler3.code.add_varname("sorted_list".to_string());
    compiler3.code.add_varname("i".to_string());
    
    println!("Compiling benchmark expression: x < sorted_list[i]");
    match compiler3.compile_expression(benchmark_expr) {
        Ok(()) => {
            println!("✅ Benchmark expression compiled successfully");
            println!("Generated instructions:");
            for (i, instr) in compiler3.code.instructions.iter().enumerate() {
                println!("  {}: {:?} arg={:?}", i, instr.opcode, instr.arg);
            }
            
            // Test execution
            let mut vm = BytecodeVM::new();
            
            // Set up variables like in your benchmark
            vm.globals.insert("x".to_string(), Value::Int(3));
            vm.globals.insert("sorted_list".to_string(), Value::List(vec![
                Value::Int(1), Value::Int(2), Value::Int(4), Value::Int(5)
            ]));
            vm.globals.insert("i".to_string(), Value::Int(2)); // index 2 = value 4
            
            println!("Executing with x=3, sorted_list=[1,2,4,5], i=2 (3 < 4 = true)");
            match vm.execute(compiler3.code.clone()) {
                Ok(result) => {
                    println!("✅ Execution successful, result: {:?}", result);
                    assert_eq!(result, Value::Bool(true));
                }
                Err(e) => {
                    println!("❌ Execution failed: {}", e);
                }
            }
        }
        Err(e) => {
            println!("❌ Compilation failed: {}", e);
        }
    }
    
    println!("\n=== DEBUG TEST COMPLETE ===");
}