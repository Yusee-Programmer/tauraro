# Tauraro Generator Issue - Investigation Summary

## Problem Statement
Generators in Tauraro exhibit an infinite loop when used in a for loop. After yielding the correct values (1, 2, 3 in the test case), the generator continues to yield `None` indefinitely instead of stopping iteration.

## Test Case
```python
def simple_gen():
    yield 1
    yield 2
    yield 3

gen = simple_gen()
count = 0
for x in gen:
    print(x)          # Prints: 1, 2, 3, None, None, None...
    count = count + 1
    if count > 5:
        print("ERROR: Infinite loop detected!")
        break
```

## Root Cause Analysis (In Progress)

### Architecture Overview
- Generators are represented by `Value::Generator { code, frame: Option<Box<Frame>>, finished: bool }`
- When a generator is iterated in a for loop, `ForIterNext` opcode handler (line 2142-2183 in vm.rs) is responsible for resuming execution
- Generator functions yield values via `YieldValue` opcode (line 6455-6490 in vm.rs)

### Investigation Findings

1. **Correct Behavior (yields 1, 2, 3)**:
   - YieldValue handler correctly stores yielded values in caller's result_reg
   - Frame is saved with updated PC and restored to Generator value with `finished: false`
   - Frame is popped so control returns to ForIterNext

2. **Problem Starts (infinite None yields)**:
   - After 3 correct yields, None begins yielding repeatedly
   - This suggests either:
     a) An implicit `return None` at end of generator function is being yielded
     b) Generator frame is being reused/recreated instead of marked as finished
     c) ForIterNext is not checking `finished` flag correctly

3. **Code Changes Attempted**:
   - Modified `handle_return_value()` to detect generator frames and mark them as finished instead of returning
   - Modified PC bounds check (line 606) to detect generator frames at end of instructions and mark as finished
   - **Problem**: Neither debug output appears, suggesting these code paths aren't being executed

### Why Debug Output Didn't Appear

The absence of debug output suggests:
1. Generator frames never reach `PC >= instructions.len()` condition
2. Generator frames never execute `ReturnValue` instruction with `return_register` set
3. The `None` values are being yielded from some OTHER path not yet identified

Possible missing code paths:
- An explicit `YieldValue` with None argument added by compiler
- ForIterNext calling frame push/resume logic in a way that bypasses finalization
- Loop variable not being updated properly when generator finishes

## Next Steps

1. **Examine Compiled Bytecode**: Use `debug-bytecode` to see actual instructions for `simple_gen` function
2. **Add Comprehensive Logging**: Log every YieldValue execution, ForIterNext call, and Generator frame state change
3. **Consider Alternative Architecture**: Generator frame management might need restructuring to properly handle completion
4. **Implement Proper StopIteration**: Python generators should raise StopIteration when exhausted, not yield None

## Files Modified
- `src/bytecode/vm.rs`: 
  - Line 606-640: Enhanced PC bounds check for generator frames
  - Line 1854-1886: Modified handle_return_value to detect and finish generator frames

## Test File
- `tests/test_generator_yield.tr`: Reproduces the infinite loop issue with 5-iteration timeout

## Current Status
Build compiles successfully with enhancements. Issue persists but root cause not yet definitively identified. Requires bytecode-level analysis to proceed.
