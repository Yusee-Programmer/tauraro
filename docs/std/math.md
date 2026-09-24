# std.math — Integer, Float, Bitwise, Random, Statistics, and BigInt

```tauraro
from std.math.int    import Math          # integer math (static methods)
from std.math.float  import FloatMath     # floating-point math (static methods)
from std.math.bits   import Bits          # bitwise operations (static methods)
from std.math.random import Random        # pseudo-random number generation
from std.math.stats  import Stats         # descriptive statistics (static methods)
from std.math.bigint import BigInt        # arbitrary-precision integers
```

---

## std.math.int — Integer mathematics

**When**: Arithmetic helpers that are error-prone to write by hand — GCD, primality, factorial, clamping.
**Why**: `Math` groups common number-theory and combinatorics functions into a single static-method class.

### Arithmetic

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.abs` | `(x: int) -> int` | `int` | Absolute value. |
| `Math.min` | `(a: int, b: int) -> int` | `int` | Smaller of two values. |
| `Math.max` | `(a: int, b: int) -> int` | `int` | Larger of two values. |
| `Math.clamp` | `(x: int, lo: int, hi: int) -> int` | `int` | Clamp `x` to `[lo, hi]`. |
| `Math.sign` | `(x: int) -> int` | `int` | `-1`, `0`, or `1`. |

### Powers and roots

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.pow` | `(base: int, exp: int) -> int` | `int` | `base^exp` via repeated squaring. |
| `Math.isqrt` | `(n: int) -> int` | `int` | Integer square root (floor). |

### Number theory

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.gcd` | `(a: int, b: int) -> int` | `int` | Greatest common divisor. |
| `Math.lcm` | `(a: int, b: int) -> int` | `int` | Least common multiple. |
| `Math.is_prime` | `(n: int) -> bool` | `bool` | Primality test. |

### Combinatorics

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.factorial` | `(n: int) -> int` | `int` | `n!` |
| `Math.choose` | `(n: int, k: int) -> int` | `int` | Binomial coefficient `C(n, k)`. |
| `Math.fibonacci` | `(n: int) -> int` | `int` | `n`-th Fibonacci number (0-indexed, iterative). |

### Division helpers

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.mod_pos` | `(a: int, b: int) -> int` | `int` | Always-positive modulo (useful for ring arithmetic). |
| `Math.div_ceil` | `(a: int, b: int) -> int` | `int` | Ceiling integer division. |

### Digit utilities

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.sum_digits` | `(n: int) -> int` | `int` | Sum of all decimal digits of `n`. |
| `Math.num_digits` | `(n: int) -> int` | `int` | Number of decimal digits of `n`. |
| `Math.is_palindrome` | `(n: int) -> bool` | `bool` | `true` if decimal digits read the same forwards and backwards. |
| `Math.digits` | `(n: int) -> Vec[int]` | `Vec[int]` | Decimal digits of `n` as a vector, most-significant first. |

### Fast modular exponentiation

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Math.pow_mod` | `(base: int, exp: int, mod: int) -> int` | `int` | `base^exp mod m` without overflow; uses binary exponentiation. |
| `Math.pow_fast` | `(base: int, exp: int) -> int` | `int` | Fast integer exponentiation (same as `Math.pow`). |

### Example

```tauraro
from std.math.int import Math

print(str(Math.gcd(12, 8)))          # 4
print(str(Math.lcm(4, 6)))           # 12
print(str(Math.is_prime(17)))        # true
print(str(Math.factorial(5)))        # 120
print(str(Math.clamp(15, 0, 10)))    # 10
print(str(Math.fibonacci(8)))        # 21
print(str(Math.sum_digits(1234)))    # 10
print(str(Math.is_palindrome(121)))  # true
print(str(Math.pow_mod(2, 10, 100))) # 24
```

---

## std.math.float — Floating-point mathematics

**When**: Trigonometry, logarithms, roots, and other operations on `float` (64-bit `double`).
**Why**: `FloatMath` wraps C `<math.h>` without requiring you to declare `extern "C"` manually.

### Constants

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatMath.pi` | `() -> float` | `float` | π ≈ 3.14159265358979 |
| `FloatMath.tau` | `() -> float` | `float` | 2π |
| `FloatMath.e` | `() -> float` | `float` | Euler's number |
| `FloatMath.inf` | `() -> float` | `float` | Positive infinity |

### Trigonometry

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatMath.sin` | `(x: float) -> float` | `float` | Sine (radians). |
| `FloatMath.cos` | `(x: float) -> float` | `float` | Cosine (radians). |
| `FloatMath.tan` | `(x: float) -> float` | `float` | Tangent (radians). |
| `FloatMath.asin` | `(x: float) -> float` | `float` | Arcsine. |
| `FloatMath.acos` | `(x: float) -> float` | `float` | Arccosine. |
| `FloatMath.atan` | `(x: float) -> float` | `float` | Arctangent. |
| `FloatMath.atan2` | `(y: float, x: float) -> float` | `float` | Two-argument arctangent. |
| `FloatMath.sinh` | `(x: float) -> float` | `float` | Hyperbolic sine. |
| `FloatMath.cosh` | `(x: float) -> float` | `float` | Hyperbolic cosine. |
| `FloatMath.tanh` | `(x: float) -> float` | `float` | Hyperbolic tangent. |

### Exponential / logarithm

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatMath.sqrt` | `(x: float) -> float` | `float` | Square root. |
| `FloatMath.cbrt` | `(x: float) -> float` | `float` | Cube root. |
| `FloatMath.pow` | `(base: float, exp: float) -> float` | `float` | `base^exp`. |
| `FloatMath.exp` | `(x: float) -> float` | `float` | `e^x`. |
| `FloatMath.exp2` | `(x: float) -> float` | `float` | `2^x`. |
| `FloatMath.log` | `(x: float) -> float` | `float` | Natural logarithm. |
| `FloatMath.log2` | `(x: float) -> float` | `float` | Base-2 logarithm. |
| `FloatMath.log10` | `(x: float) -> float` | `float` | Base-10 logarithm. |

### Rounding

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatMath.floor` | `(x: float) -> float` | `float` | Round down. |
| `FloatMath.ceil` | `(x: float) -> float` | `float` | Round up. |
| `FloatMath.round` | `(x: float) -> float` | `float` | Round to nearest. |
| `FloatMath.trunc` | `(x: float) -> float` | `float` | Truncate toward zero. |

### Utilities

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatMath.abs` | `(x: float) -> float` | `float` | Absolute value. |
| `FloatMath.mod` | `(x: float, y: float) -> float` | `float` | Floating-point remainder of `x / y` (`fmod`). |
| `FloatMath.hypot` | `(x: float, y: float) -> float` | `float` | Euclidean distance √(x²+y²). |
| `FloatMath.deg_to_rad` | `(deg: float) -> float` | `float` | Degrees → radians. |
| `FloatMath.rad_to_deg` | `(rad: float) -> float` | `float` | Radians → degrees. |
| `FloatMath.clamp` | `(x: float, lo: float, hi: float) -> float` | `float` | Clamp to `[lo, hi]`. |
| `FloatMath.lerp` | `(a: float, b: float, t: float) -> float` | `float` | Linear interpolation `a + t*(b-a)`. |
| `FloatMath.is_nan` | `(x: float) -> bool` | `bool` | `true` if `x` is NaN. |
| `FloatMath.is_inf` | `(x: float) -> bool` | `bool` | `true` if `x` is ±infinity. |

### Example

```tauraro
from std.math.float import FloatMath

mut sq    = FloatMath.sqrt(9.0)               # 3.0
mut angle = FloatMath.deg_to_rad(90.0)        # π/2
mut hyp   = FloatMath.hypot(3.0, 4.0)         # 5.0
mut p     = FloatMath.pi()
print(str(p > 3.0))                           # true
```

---

## std.math.bits — Bitwise operations

**When**: Low-level bit manipulation — popcount, power-of-two tests, bit-field extraction.
**Why**: Wraps compiler intrinsics (popcnt, rotate) that would otherwise require `extern "C"`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Bits.popcount` | `(x: int) -> int` | `int` | Number of set bits (population count). |
| `Bits.is_pow2` | `(x: int) -> bool` | `bool` | `true` if `x` is an exact power of two. |
| `Bits.next_pow2` | `(x: int) -> int` | `int` | Smallest power of two `>= x`. |
| `Bits.log2_floor` | `(x: int) -> int` | `int` | ⌊log₂(x)⌋. Returns `-1` for `x <= 0`. |
| `Bits.rotl64` | `(x: int, n: int) -> int` | `int` | Rotate `x` left by `n` bits. |
| `Bits.rotr64` | `(x: int, n: int) -> int` | `int` | Rotate `x` right by `n` bits. |
| `Bits.bswap32` | `(x: int) -> int` | `int` | Reverse byte order of the low 32 bits. |
| `Bits.bit_field` | `(x: int, lo: int, hi: int) -> int` | `int` | Extract bits `[lo, hi]` (inclusive) from `x`. |

### Example

```tauraro
from std.math.bits import Bits

print(str(Bits.popcount(7)))     # 3  (0b111)
print(str(Bits.is_pow2(8)))      # true
print(str(Bits.next_pow2(5)))    # 8
print(str(Bits.log2_floor(15)))  # 3
```

---

## std.math.random — Pseudo-random number generation

**When**: You need random integers, floats, shuffled sequences, or random sampling in tests, simulations, or games.
**Why**: Uses a 64-bit LCG (Knuth multiplicative hash) stored inside the struct — no OS handles, no `Pointer` fields, no allocation.

### Construction

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Random.new` | `(seed: int) -> Random` | `Random` | Create a deterministic RNG from `seed`. Non-zero seeds are used as-is; `0` is replaced with `12345`. |
| `Random.from_time` | `() -> Random` | `Random` | Seed from the current clock — non-deterministic. Use this in production; use `new(seed)` in tests. |

### Core generation

| Method | Signature | Returns | Description |
|---|---|---|---|
| `next_int` | `() -> int` | `int` | Advance the LCG; return a non-negative pseudo-random `int`. |
| `next_float` | `() -> float` | `float` | Return a float in `[0.0, 1.0)`. |

### Bounded integers

| Method | Signature | Returns | Description |
|---|---|---|---|
| `randint` | `(lo: int, hi: int) -> int` | `int` | Random integer in `[lo, hi]` inclusive. |
| `randrange` | `(n: int) -> int` | `int` | Random integer in `[0, n)` — same as Python `random.randrange(n)`. |

### Bounded floats

| Method | Signature | Returns | Description |
|---|---|---|---|
| `uniform` | `(lo: float, hi: float) -> float` | `float` | Random float in `[lo, hi)`. |

### Sequences

| Method | Signature | Returns | Description |
|---|---|---|---|
| `choice` | `(v: Vec[int]) -> int` | `int` | Pick a random element from `v`. Returns `0` if empty. |
| `shuffle` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Return a new vector with elements in random order (Fisher-Yates). Original is not modified. |
| `sample` | `(count: int, lo: int, hi: int) -> Vec[int]` | `Vec[int]` | Return a `Vec[int]` of `count` random integers in `[lo, hi]`. |

### Boolean

| Method | Signature | Returns | Description |
|---|---|---|---|
| `coin_flip` | `() -> bool` | `bool` | `true` with ~50% probability. |

### Example

```tauraro
from std.math.random import Random

mut rng = Random.new(42)            # deterministic seed for tests
print(str(rng.randint(1, 6)))       # a die roll [1..6]
print(str(rng.next_float() < 0.5))  # coin flip (bool)

mut v = Vec[int].init(5)
v.push(10); v.push(20); v.push(30)
mut shuffled = rng.shuffle(v)       # new Vec in random order
mut picked   = rng.choice(v)        # random element

mut rng2 = Random.from_time()       # seeded by clock — unpredictable
print(str(rng2.randrange(100)))     # [0, 99]
```

---

## std.math.stats — Descriptive statistics

**When**: You need mean, median, mode, standard deviation, or percentiles of an integer dataset.
**Why**: `Stats` provides a complete set of statistical operations over `Vec[int]` without external dependencies.

All methods are static — call them as `Stats.method_name(v)`.

### Central tendency

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Stats.sum` | `(v: Vec[int]) -> int` | `int` | Sum of all elements. |
| `Stats.mean_int` | `(v: Vec[int]) -> int` | `int` | Arithmetic mean, integer-truncated. |
| `Stats.mean` | `(v: Vec[int]) -> float` | `float` | Arithmetic mean as float. |
| `Stats.median` | `(v: Vec[int]) -> float` | `float` | Middle value of sorted data; lower-middle for even-length. |
| `Stats.mode` | `(v: Vec[int]) -> int` | `int` | Most frequently occurring value (first encountered on tie). |

### Dispersion

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Stats.min` | `(v: Vec[int]) -> int` | `int` | Minimum element. |
| `Stats.max` | `(v: Vec[int]) -> int` | `int` | Maximum element. |
| `Stats.data_range` | `(v: Vec[int]) -> int` | `int` | `max - min`. |
| `Stats.variance` | `(v: Vec[int]) -> float` | `float` | Population variance (sum of squared deviations / n). |
| `Stats.sample_variance` | `(v: Vec[int]) -> float` | `float` | Sample variance (Bessel-corrected, / (n-1)). |
| `Stats.stdev` | `(v: Vec[int]) -> float` | `float` | Population standard deviation (√variance). |
| `Stats.sample_stdev` | `(v: Vec[int]) -> float` | `float` | Sample standard deviation. |

### Percentile

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Stats.percentile` | `(v: Vec[int], p: int) -> float` | `float` | `p`-th percentile (0–100) using nearest-rank method. |

### Counting

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Stats.count_eq` | `(v: Vec[int], target: int) -> int` | `int` | Number of elements equal to `target`. |

### Example

```tauraro
from std.math.stats import Stats
from std.core.vec   import Vec

mut data = Vec[int].init(8)
data.push(4); data.push(7); data.push(2); data.push(9); data.push(4)

print(str(Stats.sum(data)))        # 26
print(str(Stats.mean_int(data)))   # 5  (truncated)
print(str(Stats.median(data)))     # 4.0
print(str(Stats.mode(data)))       # 4  (appears twice)
print(str(Stats.min(data)))        # 2
print(str(Stats.max(data)))        # 9
print(str(Stats.data_range(data))) # 7
print(str(Stats.percentile(data, 75)))  # 7.0 (nearest-rank)
```

---

## BigInt — `std.math.bigint`

**When**: Numbers that exceed a native 64-bit `int` — large factorials, combinatorics, cryptography-adjacent arithmetic, exact decimal computation.
**Why**: `BigInt` is a sign-magnitude arbitrary-precision integer, stored as base-10^9 "limbs" in a `Vec[int]` (least-significant limb first). Base 10^9 is chosen so a limb×limb product plus carry never overflows a 64-bit `int` (max product ≈ 9.99×10^17, safely under ~9.22×10^18), while keeping decimal string conversion trivial — each limb is exactly 9 decimal digits, zero-padded. All arithmetic is pure Tauraro compiled straight to C, so there is no interpreter overhead.

### Construction

| Method | Signature | Returns | Description |
|---|---|---|---|
| `BigInt.zero` | `() -> BigInt` | `BigInt` | The value `0`. |
| `BigInt.from_int` | `(n: int) -> BigInt` | `BigInt` | Construct from a native `int`. |
| `BigInt.from_str` | `(s: str) -> BigInt` | `BigInt` | Parse a decimal string, with an optional leading `-`/`+`. Malformed input (empty, bare sign, or a non-digit character) returns `BigInt.zero()` rather than aborting — pre-validate the string yourself if you need strict error detection. |

### Conversion

| Method | Signature | Returns | Description |
|---|---|---|---|
| `.to_str` | `(self) -> str` | `str` | Decimal string, correct sign, no leading zeros (`0` itself is `"0"`). |
| `.to_int` | `(self) -> int` | `int` | Convert back to a native `int`. When the value doesn't fit in 64 bits this is a **documented lossy truncation** (only the low-order limbs are combined, then re-signed) — not a saturating clamp. Use `.to_str()`, or check the magnitude yourself, whenever you must detect overflow. **Known issue:** a pre-existing compiler bug (see `bug2.txt` at the repo root) intercepts any call site literally named `.to_int()` before user-class dispatch, for every class, not just `BigInt`. Until that's fixed, call `.to_int_checked()` — identical behavior, unaffected call-site name. |
| `.to_int_checked` | `(self) -> int` | `int` | Same conversion as `.to_int`, exposed under a name the compiler bug above doesn't intercept. Prefer this today; switch back to `.to_int` once the compiler fix lands. |

### Arithmetic

| Method | Operator | Signature | Returns | Description |
|---|---|---|---|---|
| `.add` | `+` | `(self, other: BigInt) -> BigInt` | `BigInt` | Addition. |
| `.sub` | `-` | `(self, other: BigInt) -> BigInt` | `BigInt` | Subtraction (may go negative). |
| `.mul` | `*` | `(self, other: BigInt) -> BigInt` | `BigInt` | Multiplication. |
| `.div` | `/` | `(self, other: BigInt) -> BigInt` | `BigInt` | Integer division, **rounding toward zero** (same convention as native `int / int`): `-7 / 2 == -3`, not the floor value `-4`. Division by zero returns `BigInt.zero()` rather than aborting. |
| `.rem` | `%` | `(self, other: BigInt) -> BigInt` | `BigInt` | Remainder, sign follows the dividend (same convention as native `int % int`): `-7 % 2 == -1`. Always satisfies `self == other * (self / other) + self % other`. |
| `.mod_` | — | `(self, other: BigInt) -> BigInt` | `BigInt` | Alias for `.rem`. |
| `.pow` | — | `(self, exp: int) -> BigInt` | `BigInt` | `self^exp` via exponentiation by squaring. `exp` must be non-negative; a negative `exp` returns `BigInt.zero()` (documented — a `BigInt` can't hold a fraction). |
| `.negate` / `-x` | `-` (unary) | `(self) -> BigInt` | `BigInt` | Negation. |
| `.abs` | — | `(self) -> BigInt` | `BigInt` | Absolute value. |

### Comparison

| Method | Operator | Signature | Returns | Description |
|---|---|---|---|---|
| `.cmp` | — | `(self, other: BigInt) -> int` | `int` | `-1`, `0`, or `1`. |
| `.eq` | `==` | `(self, other: BigInt) -> bool` | `bool` | Equality. |
| `.lt` / `.le` / `.gt` / `.ge` | `<` `<=` `>` `>=` | `(self, other: BigInt) -> bool` | `bool` | Ordering. |
| `.is_zero` | — | `(self) -> bool` | `bool` | `true` if the value is `0`. |
| `.is_negative` | — | `(self) -> bool` | `bool` | `true` if the value is `< 0`. |

`BigInt` overloads `+ - * / % == != < <= > >=` and unary `-` (see [Operator Overloading](../lang/21_operator_overloading.md)), so it reads like native arithmetic.

### Example

```tauraro
from std.math.bigint import BigInt

# 30! exceeds a 64-bit int by a wide margin — BigInt computes it exactly.
mut fact = BigInt.from_int(1)
mut k = 2
while k <= 30:
    fact = fact * BigInt.from_int(k)
    k = k + 1
print(fact.to_str())    # 265252859812191058636308480000000

mut a = BigInt.from_str("123456789123456789123456789")
mut b = BigInt.from_int(2)
print((a * b).to_str())    # 246913578246913578246913578

mut neg = BigInt.from_int(-7).div(BigInt.from_int(2))
print(neg.to_str())        # -3 (rounds toward zero, not floor -4)
```
