# std.test

Lightweight unit-testing framework for Tauraro programs.

## Import

```tauraro
from std.test import TestRunner
```

## Overview

`std.test` provides a single `TestRunner` class.  Create one instance per test suite, call
assertion methods, then call `summary()` at the end to print a result line and check whether
all tests passed.

There is no test-discovery magic.  Test functions are plain Tauraro functions that you call
explicitly.  This keeps the model simple and fast.

## `TestRunner` class

### Construction

```tauraro
TestRunner.init(suite_name: str) -> TestRunner
```

Creates a new runner with the given suite name.  Pass/fail counters start at zero.

### Boolean assertions

```tauraro
t.assert_true(cond: bool, msg: str)
t.assert_false(cond: bool, msg: str)
```

### Integer assertions

```tauraro
t.assert_eq_int(got: int,  want: int, msg: str)
t.assert_ne_int(a: int,    b: int,    msg: str)
t.assert_gt_int(a: int,    b: int,    msg: str)   # a > b
t.assert_lt_int(a: int,    b: int,    msg: str)   # a < b
```

### String assertions

```tauraro
t.assert_eq_str(got: str, want: str, msg: str)
t.assert_ne_str(a: str,   b: str,    msg: str)
```

### Float assertions

```tauraro
t.assert_eq_float(got: float, want: float, eps: float, msg: str)  # |got - want| <= eps
t.assert_ne_float(a: float,   b: float,    eps: float, msg: str)  # |a - b| > eps
```

### Unconditional failure

```tauraro
t.fail(msg: str)   # always records a FAIL — use in unreachable branches
```

### Grouping

```tauraro
t.section(label: str)   # prints "  [label]" — cosmetic only, no effect on counts
```

### Summary

```tauraro
t.summary() -> bool
```

Prints a one-line result (`all N tests passed.` or `F/N tests FAILED.`) and returns `true`
iff all assertions passed.

## Behaviour

- Every passing assertion increments `t.passed`.
- Every failing assertion increments `t.failed` **and** immediately prints a `FAIL:` line with
  the message and the actual vs. expected values.
- `summary()` reads `t.passed` and `t.failed` at call time — you can call it multiple times if
  you want mid-suite checkpoints.

## Example

```tauraro
from std.test import TestRunner

def test_arithmetic(t: TestRunner):
    t.section("arithmetic")
    t.assert_eq_int(1 + 1,     2,    "1+1=2")
    t.assert_eq_int(10 - 3,    7,    "10-3=7")
    t.assert_eq_int(3 * 4,     12,   "3*4=12")
    t.assert_gt_int(10,        5,    "10>5")
    t.assert_lt_int(2,         100,  "2<100")

def test_strings(t: TestRunner):
    t.section("strings")
    t.assert_eq_str("hello" + " world", "hello world", "concatenation")
    t.assert_ne_str("foo", "bar", "distinct strings differ")

def test_floats(t: TestRunner):
    t.section("floats")
    t.assert_eq_float(1.0 / 3.0, 0.333333, 0.0001, "1/3 approx")

def main():
    mut t = TestRunner.init("my_module")
    test_arithmetic(t)
    test_strings(t)
    test_floats(t)
    if not t.summary():
        return   # non-zero conceptually; process exit code not yet enforced
```

Output on success:
```
  [arithmetic]
  [strings]
  [floats]
my_module: all 8 tests passed.
```

Output when a test fails:
```
  [arithmetic]
  FAIL: 1+1=2 — expected 2, got 3
  ...
my_module: 1/8 tests FAILED.
```

## Extending the framework

Because `TestRunner` is a plain class, you can embed it in larger runners or add domain-specific
helper methods via `extend TestRunner:` in your own code.

---

*See `TAURARO_MASTER_SPECIFICATION.md` for the full language specification.*
