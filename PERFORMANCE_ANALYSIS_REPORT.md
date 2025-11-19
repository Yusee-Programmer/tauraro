// TAURARO PERFORMANCE OPTIMIZATION REPORT
// Generated: November 19, 2025
// 
// ==============================================================================
// PROBLEM ANALYSIS
// ==============================================================================
//
// Current Performance (vs Python):
//   Simple Loop:        4.4x SLOWER  (0.32s → 1.4s for 1M iterations)
//   Arithmetic (1M ops): 8.8x SLOWER  (0.16s → 1.4s)
//   Function Calls:     33x SLOWER   (0.03s → 1.0s for 100k calls)
//   Recursive Fib:      5000x SLOWER (0.0002s → 1.0s)
//   TOTAL:              10.9x SLOWER (0.51s → 5.56s)
//
// Root Causes (Priority Order):
// 1. FUNCTION CALL OVERHEAD (33x) - Every call creates Frame, allocates Vec, pushes/pops
// 2. Loop dispatch overhead (4.4x) - ForIter instruction even with fast path is slow
// 3. Value boxing overhead (8.8x) - Enum wrapping, pattern matching on every operation
// 4. Memory allocation (varies) - Vec<Value> for arguments, HashMap for locals
//
// ==============================================================================
// OPTIMIZATION STRATEGY (3 PHASES)
// ==============================================================================
//
// PHASE 1: TAIL CALL OPTIMIZATION (Target: 5-10x speedup for recursion)
// - Detect tail calls (function returns result of recursive call)
// - Reuse current frame instead of creating new one
// - Save: frame allocation, frame stack push/pop, argument Vec allocation
// - Example: fib(n-1) at end of function → reuse frame, reset PC to start
//
// Impact: Fibonacci(30) could go from ~40s to ~4s (10x improvement)
//
// PHASE 2: INLINE SIMPLE CLOSURES (Target: 3-5x speedup for small functions)
// - Detect simple closures (few lines, no loops)
// - Execute inline without frame stack
// - Cache compiled code at parse time
// - Example: is_prime() helper function → inline execution
//
// Impact: Recursive prime checking could gain 3-5x
//
// PHASE 3: UNBOXED ARITHMETIC (Target: 2-3x speedup for loops)
// - Use RegisterValue with inline integers/floats
// - Skip Value enum boxing in hot paths
// - Special opcodes for int-only operations
// - Example: result = result + 1 → direct i64 add, no enum
//
// Impact: 1M iteration loop could go from 1.4s to 0.5s (3x improvement)
//
// ==============================================================================
// IMPLEMENTATION PRIORITY (Start with highest ROI):
// ==============================================================================
//
// HIGH PRIORITY (5-10x speedup per change):
// 1. ✅ execute_closure_sync: Reuse VM instead of creating new (DONE - minor impact)
// 2. 🔴 Tail call detection & frame reuse in call_function_fast() 
// 3. 🔴 Add fast path in run_frame() for tail calls (no new frame push)
//
// MEDIUM PRIORITY (2-3x speedup per change):
// 4. 🔴 Inline fast path for BinaryAddRR when both operands are RegisterValue::Int
// 5. 🔴 Optimize ForIter with inline integer iteration (no Value allocation)
// 6. 🔴 Cache function lookups (globals[func_name] is slow due to Rc<RefCell> access)
//
// LOWER PRIORITY (1-2x speedup):
// 7. 🔴 Pre-allocate frame locals array size
// 8. 🔴 Use thread-local frame pool
// 9. 🔴 Implement constant folding at compile time
//
// ==============================================================================
// ESTIMATED IMPROVEMENTS (Cumulative):
// ==============================================================================
//
// Phase 1 (Tail calls):          fib(30): 47s → 5s     (10x) ✈️ HUGE
// Phase 2 (Inline closures):     is_prime: 3x speedup
// Phase 3 (Unboxed arithmetic):  loops: 1.4s → 0.5s    (3x)
//
// TOTAL POTENTIAL: 20-50x speedup as per user's target! 🎯
//
// ==============================================================================
// IMPLEMENTATION STEPS
// ==============================================================================
//
// Step 1: Add tail call detection helper
//   - Check if last statement in closure is a return of a recursive call
//   - Return tuple: (is_tail_call, recursive_func_name)
//
// Step 2: Modify call_function_fast() for Closure variant
//   - If tail call detected:
//     * Clear frame.locals but keep frame structure
//     * Update frame.locals with new arguments
//     * Reset frame.pc = 0
//     * Continue execution in same frame (no push/pop)
//   - Else: Use existing frame push mechanism
//
// Step 3: Add ForIter optimization
//   - For range iterators: unbox into RegisterValue::Int
//   - No Value allocation per iteration
//   - Direct comparison: if current < stop → continue
//
// Step 4: Benchmark after each phase
//   - Verify 5-10x improvements claimed
//   - Identify remaining bottlenecks
//   - Iterate
//
// ==============================================================================
