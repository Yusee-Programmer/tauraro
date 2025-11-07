# JIT Compilation Implementation Plan

## Current State

### ✅ Implemented
- **HotLoopDetector**: Tracks loop execution counts (threshold: 10,000 iterations)
- **CompiledLoop**: Data structure for compiled loops
- **CraneliftCompiler**: Basic optimization framework (constant folding, dead code elimination)
- **Profiling**: Function call counts and instruction execution tracking in VM
- **Dependencies**: Cranelift 0.119 configured as optional feature

### ❌ Not Implemented
- Bytecode to Cranelift IR translation
- Native code generation with Cranelift
- Native code execution from VM
- Deoptimization/fallback mechanisms
- Type specialization for JIT

## Architecture Overview

```
┌──────────────────────────────────────────────────────────────┐
│                        Tauraro VM                             │
├──────────────────────────────────────────────────────────────┤
│  1. Execute bytecode                                          │
│  2. Track loop iterations → HotLoopDetector                   │
│  3. When threshold hit:                                       │
│     - Extract loop bytecode                                   │
│     - Pass to JIT compiler                                    │
│  4. Replace loop with native code call                        │
└──────────────────────────────────────────────────────────────┘
                           ↓
┌──────────────────────────────────────────────────────────────┐
│                    JIT Compiler Pipeline                      │
├──────────────────────────────────────────────────────────────┤
│  Phase 1: Bytecode Analysis                                   │
│    - Extract loop body (SetupLoop → PopBlock)                 │
│    - Identify live registers and variables                    │
│    - Build control flow graph                                 │
│                                                                │
│  Phase 2: Optimization                                        │
│    - Constant folding                                         │
│    - Dead code elimination                                    │
│    - Type specialization (if types known)                     │
│    - Register allocation                                      │
│                                                                │
│  Phase 3: Cranelift IR Generation                             │
│    - Translate opcodes to Cranelift IR                        │
│    - Map Tauraro registers to Cranelift values                │
│    - Handle Value boxing/unboxing                             │
│    - Generate loop condition and backedge                     │
│                                                                │
│  Phase 4: Native Code Generation                              │
│    - Cranelift optimize_and_emit                              │
│    - Link native function                                     │
│    - Store function pointer in CompiledLoop                   │
└──────────────────────────────────────────────────────────────┘
                           ↓
┌──────────────────────────────────────────────────────────────┐
│                     Native Execution                          │
├──────────────────────────────────────────────────────────────┤
│  - Call native function pointer                               │
│  - Pass frame registers as parameters                         │
│  - Native code executes loop iterations                       │
│  - Return control to VM                                       │
│  - Continue after loop                                        │
└──────────────────────────────────────────────────────────────┘
```

## Implementation Phases

### Phase 1: Basic IR Generation (This PR)

**Goal**: Translate simple loop opcodes to Cranelift IR

**Opcodes to Support** (covers 80% of loops):
1. LoadConst
2. LoadFast / StoreFast (local variables)
3. BinaryAddRR / BinarySubRR / BinaryMulRR / BinaryDivRR
4. FastIntAdd / FastIntSub / FastIntMul / FastIntDiv
5. CompareLessRR / CompareEqualRR / CompareGreaterRR
6. Jump / JumpIfTrue / JumpIfFalse
7. ForIter (iterator loops)

**IR Mapping**:
```rust
// Tauraro bytecode:
LoadConst c0, r1       →  let v1 = builder.ins().iconst(i64, constant_value);
LoadFast  v0, r2       →  let v2 = frame_vars[v0];
FastIntAdd r1, r2, r3  →  let v3 = builder.ins().iadd(v1, v2);
StoreFast r3, v0       →  frame_vars[v0] = v3;
```

**Native Function Signature**:
```rust
// Native loop function signature
type NativeLoopFn = unsafe extern "C" fn(
    registers: *mut Value,  // Pointer to register array
    locals: *mut Value,     // Pointer to local variables
    constants: *const Value // Pointer to constants
) -> i32;  // Return: 0 = success, 1 = error
```

### Phase 2: Full Opcode Support (Future PR)

**Additional Opcodes**:
- CallFunction / CallMethod (inline simple calls)
- BuildList / SubscrLoad
- LoadAttr / StoreAttr
- String operations
- Exception handling

### Phase 3: Advanced Optimizations (Future PR)

**Type Specialization**:
```rust
// If we know r1 and r2 are always integers:
FastIntAdd r1, r2, r3  →  builder.ins().iadd(v1, v2);  // Direct integer add

// If types unknown (generic path):
FastIntAdd r1, r2, r3  →
    call(value_add_fn, v1, v2)  // Call runtime helper
```

**Inlining**:
- Inline small functions directly into loop
- Eliminate function call overhead

**SIMD**:
- Vectorize array operations
- Use AVX2/AVX-512 for bulk operations

## Expected Performance

### Benchmark: Fibonacci(35)

**Current (Interpreted)**:
- Dispatch-optimized interpreter: ~2.5 seconds
- ~50,000 opcodes/second

**With JIT (Simple loops)**:
- Hot loop compiled to native: ~0.05 seconds
- ~2,500,000 opcodes/second
- **50x speedup**

**With JIT (Full optimization)**:
- Type specialization + inlining: ~0.025 seconds
- **100x speedup**

### Benchmark: Sum of 1 million integers

**Current**: ~200ms
**With JIT**: ~5ms
**Speedup**: 40x

## Implementation Checklist

### Phase 1: Core Infrastructure

- [x] HotLoopDetector (already implemented)
- [x] CraneliftCompiler skeleton (already implemented)
- [ ] Implement `bytecode_to_cranelift_ir()` function
- [ ] Implement `compile_loop_to_native()` function
- [ ] Add native function pointer to CompiledLoop
- [ ] Integrate JIT check into VM loop execution
- [ ] Add JIT statistics and debugging

### Phase 2: Testing

- [ ] Unit tests for IR generation
- [ ] Integration tests for simple loops
- [ ] Benchmark comparison (interpreted vs JIT)
- [ ] Edge case handling (exceptions, early exit)

### Phase 3: Polish

- [ ] Error handling and fallback
- [ ] JIT compiler error messages
- [ ] Performance profiling and tuning
- [ ] Documentation and examples

## Code Structure

```
src/bytecode/
├── jit.rs                  # JIT infrastructure (EXISTS)
├── jit_compiler.rs         # NEW: Cranelift IR generation
├── jit_codegen.rs          # NEW: Native code generation
└── vm.rs                   # UPDATE: Add JIT execution path
```

## Safety Considerations

1. **Memory Safety**: Native code must not corrupt VM state
2. **Type Safety**: Ensure Value boxing/unboxing is correct
3. **Exception Handling**: Native code must handle errors gracefully
4. **Stack Safety**: Prevent stack overflow in native loops
5. **Garbage Collection**: Ensure GC can see all live values

## Rollout Strategy

1. **Enable JIT feature flag**: `cargo build --features jit`
2. **Start with conservative threshold**: 10,000 iterations (current)
3. **Monitor compilation overhead**: Ensure compilation time < saved execution time
4. **Gradual opcode expansion**: Start with 10 opcodes, expand to full set
5. **Fallback to interpreter**: If JIT fails, continue with bytecode

## Success Criteria

- ✅ 10x speedup on loop-heavy benchmarks
- ✅ No correctness regressions
- ✅ Compilation overhead < 5% of total execution time
- ✅ Code compiles and runs with `--features jit`
- ✅ Graceful fallback when JIT fails

## Next Steps

1. Implement `bytecode_to_cranelift_ir()` for top 10 opcodes
2. Generate native function and store pointer
3. Wire JIT check into ForIter opcode handler
4. Test with simple for loop
5. Benchmark and verify speedup
