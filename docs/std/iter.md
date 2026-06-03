# std.iter — Ranges and Vector Transforms

```tauraro
from std.iter.range            import Range
from std.iter.transform        import Transform
from std.iter.float_transform  import FloatTransform
```

All methods are **static** — called as `Range.method(...)`, `Transform.method(...)`, or `FloatTransform.method(...)`.  
All transform functions return **new** vectors; originals are never modified.

---

## std.iter.range — Range class

**When**: You need a sequence of integers — loop indices, graph node IDs, test data.
**Why**: Lazy range definition with `.step()` modifier; `.to_vec()` materialises only when needed.

### Construction

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Range.init` | `(start: int, end: int) -> Range` | `Range` | Create a range `[start, end)` with step 1. |
| `step` | `(n: int) -> Range` | `Range` | Return a new `Range` with step size `n`. |
| `to_vec` | `() -> Vec[int]` | `Vec[int]` | Materialise the range into a `Vec[int]`. |

### Static convenience helpers

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Range.iota` | `(n: int) -> Vec[int]` | `Vec[int]` | Produce `[0, 1, 2, …, n-1]`. |
| `Range.stepped` | `(start: int, end: int, step: int) -> Vec[int]` | `Vec[int]` | Produce elements `start, start+step, …` while `< end`. |

### Example

```tauraro
from std.iter.range import Range

mut v1 = Range.iota(5)                     # [0, 1, 2, 3, 4]
mut v2 = Range.stepped(0, 10, 2)           # [0, 2, 4, 6, 8]
mut v3 = Range.init(3, 8).to_vec()         # [3, 4, 5, 6, 7]
mut v4 = Range.init(10, 0).step(-2).to_vec()  # [10, 8, 6, 4, 2]
```

---

## std.iter.transform — Transform class

**When**: You need to filter, map, reduce, sort, zip, or slice `Vec[int]` values without writing manual loops.
**Why**: A rich set of composable operations — each returns a new vector, so calls chain naturally.

### Filtering

Keep elements that satisfy a condition.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.filter_gt` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | Keep elements `> n`. |
| `Transform.filter_lt` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | Keep elements `< n`. |
| `Transform.filter_ge` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | Keep elements `>= n`. |
| `Transform.filter_le` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | Keep elements `<= n`. |
| `Transform.filter_eq` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | Keep elements `== n`. |
| `Transform.filter_ne` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | Keep elements `!= n`. |

### Slicing

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.take` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | First `n` elements. |
| `Transform.drop` | `(v: Vec[int], n: int) -> Vec[int]` | `Vec[int]` | All elements after skipping the first `n`. |
| `Transform.take_while_lt` | `(v: Vec[int], threshold: int) -> Vec[int]` | `Vec[int]` | Elements from the front while `< threshold`. |
| `Transform.take_while_gt` | `(v: Vec[int], threshold: int) -> Vec[int]` | `Vec[int]` | Elements from the front while `> threshold`. |

### Mapping

Apply a function to every element.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.map_add` | `(v: Vec[int], delta: int) -> Vec[int]` | `Vec[int]` | Add `delta` to every element. |
| `Transform.map_sub` | `(v: Vec[int], delta: int) -> Vec[int]` | `Vec[int]` | Subtract `delta` from every element. |
| `Transform.map_mul` | `(v: Vec[int], factor: int) -> Vec[int]` | `Vec[int]` | Multiply every element by `factor`. |
| `Transform.map_div` | `(v: Vec[int], divisor: int) -> Vec[int]` | `Vec[int]` | Integer-divide every element by `divisor`. |
| `Transform.map_abs` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Absolute value of every element. |
| `Transform.clamp_vec` | `(v: Vec[int], lo: int, hi: int) -> Vec[int]` | `Vec[int]` | Clamp every element to `[lo, hi]`. |

### Reductions

Collapse a vector to a single value.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.sum` | `(v: Vec[int]) -> int` | `int` | Sum of all elements. |
| `Transform.max` | `(v: Vec[int]) -> int` | `int` | Maximum element (returns `0` if empty). |
| `Transform.min` | `(v: Vec[int]) -> int` | `int` | Minimum element (returns `0` if empty). |
| `Transform.product` | `(v: Vec[int]) -> int` | `int` | Product of all elements. |
| `Transform.count` | `(v: Vec[int]) -> int` | `int` | Number of elements (alias for `.len`). |
| `Transform.mean` | `(v: Vec[int]) -> int` | `int` | Integer mean (truncated). |
| `Transform.count_eq` | `(v: Vec[int], n: int) -> int` | `int` | Number of elements equal to `n`. |
| `Transform.any_eq` | `(v: Vec[int], n: int) -> bool` | `bool` | `true` if any element equals `n`. |
| `Transform.all_eq` | `(v: Vec[int], n: int) -> bool` | `bool` | `true` if all elements equal `n`. |

### Combine

Produce new vectors from multiple inputs.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.zip_sum` | `(a: Vec[int], b: Vec[int]) -> Vec[int]` | `Vec[int]` | Element-wise sum of two equal-length vectors. |
| `Transform.zip` | `(a: Vec[int], b: Vec[int]) -> Vec[Pair]` | `Vec[Pair]` | Pair up elements: `[(a[0],b[0]), …]`. Uses shortest length. |
| `Transform.enumerate` | `(v: Vec[int]) -> Vec[Pair]` | `Vec[Pair]` | Pair each element with its index: `[(0,v[0]), (1,v[1]), …]`. |
| `Transform.chain` | `(a: Vec[int], b: Vec[int]) -> Vec[int]` | `Vec[int]` | Concatenate two vectors. |

### Ordering

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.sort_asc` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Return a new vector sorted ascending. |
| `Transform.sort_desc` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Return a new vector sorted descending. |
| `Transform.reverse` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Reverse the vector. |
| `Transform.unique` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Remove duplicates, preserving first occurrence order. |

### Window / chunk helpers

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.windows` | `(v: Vec[int], w: int) -> Vec[int]` | `Vec[int]` | Starting indices of each sliding window of size `w`. |
| `Transform.chunks` | `(v: Vec[int], w: int) -> Vec[int]` | `Vec[int]` | Starting indices of each non-overlapping chunk of size `w`. |

> **Note** — `windows` and `chunks` return index vectors, not sub-vectors. Use the indices with `v.get(i)` to `v.get(i+w-1)` to access each window or chunk.

### Folds

Reduce a vector using a custom starting value.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.fold_add` | `(v: Vec[int], init: int) -> int` | `int` | Fold with `+`, starting from `init`. |
| `Transform.fold_mul` | `(v: Vec[int], init: int) -> int` | `int` | Fold with `*`, starting from `init`. |

### Element access

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.first` | `(v: Vec[int]) -> int` | `int` | First element, or `0` if empty. |
| `Transform.last` | `(v: Vec[int]) -> int` | `int` | Last element, or `0` if empty. |
| `Transform.find_first` | `(v: Vec[int], target: int) -> int` | `int` | Index of the first element equal to `target`, or `-1` if not found. |
| `Transform.position` | `(v: Vec[int], target: int) -> int` | `int` | Alias for `find_first`. |

### Drop-while

Skip leading elements satisfying a condition.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.drop_while_lt` | `(v: Vec[int], threshold: int) -> Vec[int]` | `Vec[int]` | Skip leading elements `< threshold`; return the rest. |
| `Transform.drop_while_gt` | `(v: Vec[int], threshold: int) -> Vec[int]` | `Vec[int]` | Skip leading elements `> threshold`; return the rest. |

### Prefix sums

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.prefix_sums` | `(v: Vec[int]) -> Vec[int]` | `Vec[int]` | Cumulative sum: `out[i] = v[0] + … + v[i]`. |

### Positivity predicates

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Transform.all_positive` | `(v: Vec[int]) -> bool` | `bool` | `true` when every element is `> 0`. |
| `Transform.any_positive` | `(v: Vec[int]) -> bool` | `bool` | `true` when at least one element is `> 0`. |

### Example

```tauraro
from std.iter.range     import Range
from std.iter.transform import Transform

mut v = Range.iota(10)                    # [0..9]
mut big   = Transform.filter_gt(v, 5)    # [6,7,8,9]
mut scaled = Transform.map_mul(big, 2)   # [12,14,16,18]
mut total  = Transform.sum(scaled)       # 60
mut sorted = Transform.sort_desc(v)      # [9,8,7,6,5,4,3,2,1,0]
mut pairs  = Transform.enumerate(Transform.take(v, 3))
# pairs[0] = Pair(0, 0), pairs[1] = Pair(1, 1), pairs[2] = Pair(2, 2)

print(str(total))                        # 60
print(str(Transform.mean(v)))            # 4  (truncated)
print(str(Transform.any_eq(v, 7)))       # true
print(str(Transform.count(big)))         # 4

mut a = Range.stepped(0, 6, 1)           # [0,1,2,3,4,5]
mut b = Range.stepped(10, 16, 1)         # [10,11,12,13,14,15]
mut zipped = Transform.zip_sum(a, b)     # [10,12,14,16,18,20]

# Folds, element access, prefix sums
mut nums = Range.iota(5)                        # [0,1,2,3,4]
print(str(Transform.fold_add(nums, 100)))       # 110
print(str(Transform.fold_mul(nums, 1)))         # 0  (0*1*2*3*4)
print(str(Transform.first(nums)))               # 0
print(str(Transform.last(nums)))                # 4
print(str(Transform.find_first(nums, 3)))       # 3  (index)
mut ps = Transform.prefix_sums(nums)            # [0,1,3,6,10]
print(str(ps.get(4)))                           # 10

# Drop-while, positivity
mut d = Range.iota(8)                           # [0..7]
mut dropped = Transform.drop_while_lt(d, 5)    # [5,6,7]
print(str(Transform.all_positive(Transform.map_add(d, 1))))   # true
print(str(Transform.any_positive(d)))                         # true  (1..7)
```

---

## std.iter.float_transform — FloatTransform class

**When**: You need `Transform`-style operations on `Vec[float]` — data science, statistics, signal processing.
**Why**: Mirrors `Transform` exactly for floating-point data, plus `variance`, `normalize`, and `all_finite`.

### Filtering

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatTransform.filter_gt` | `(v: Vec[float], t: float) -> Vec[float]` | `Vec[float]` | Keep elements `> t`. |
| `FloatTransform.filter_lt` | `(v: Vec[float], t: float) -> Vec[float]` | `Vec[float]` | Keep elements `< t`. |
| `FloatTransform.filter_ge` | `(v: Vec[float], t: float) -> Vec[float]` | `Vec[float]` | Keep elements `>= t`. |
| `FloatTransform.filter_le` | `(v: Vec[float], t: float) -> Vec[float]` | `Vec[float]` | Keep elements `<= t`. |

### Mapping

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatTransform.map_add` | `(v, delta: float) -> Vec[float]` | `Vec[float]` | Add `delta` to every element. |
| `FloatTransform.map_sub` | `(v, delta: float) -> Vec[float]` | `Vec[float]` | Subtract `delta`. |
| `FloatTransform.map_mul` | `(v, factor: float) -> Vec[float]` | `Vec[float]` | Multiply by `factor`. |
| `FloatTransform.map_div` | `(v, divisor: float) -> Vec[float]` | `Vec[float]` | Divide (returns `0.0` when divisor is `0.0`). |
| `FloatTransform.map_abs` | `(v: Vec[float]) -> Vec[float]` | `Vec[float]` | Absolute value of every element. |
| `FloatTransform.clamp_vec` | `(v, lo, hi: float) -> Vec[float]` | `Vec[float]` | Clamp every element to `[lo, hi]`. |

### Reductions

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatTransform.sum` | `(v: Vec[float]) -> float` | `float` | Sum. |
| `FloatTransform.max` | `(v: Vec[float]) -> float` | `float` | Maximum (`0.0` if empty). |
| `FloatTransform.min` | `(v: Vec[float]) -> float` | `float` | Minimum (`0.0` if empty). |
| `FloatTransform.product` | `(v: Vec[float]) -> float` | `float` | Product. |
| `FloatTransform.mean` | `(v: Vec[float]) -> float` | `float` | Arithmetic mean. |
| `FloatTransform.variance` | `(v: Vec[float]) -> float` | `float` | Population variance. |

### Element access

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatTransform.first` | `(v: Vec[float]) -> float` | `float` | First element, or `0.0`. |
| `FloatTransform.last` | `(v: Vec[float]) -> float` | `float` | Last element, or `0.0`. |
| `FloatTransform.find_first_gt` | `(v, threshold: float) -> float` | `float` | First element `> threshold`, or `0.0`. |
| `FloatTransform.position` | `(v: Vec[float], val: float) -> int` | `int` | Index of first element `== val`, or `-1`. |

### Normalization and sorting

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatTransform.normalize` | `(v: Vec[float]) -> Vec[float]` | `Vec[float]` | Rescale all values to `[0.0, 1.0]` relative to min/max. |
| `FloatTransform.sort_asc` | `(v: Vec[float]) -> Vec[float]` | `Vec[float]` | Sort ascending (new vector). |
| `FloatTransform.sort_desc` | `(v: Vec[float]) -> Vec[float]` | `Vec[float]` | Sort descending (new vector). |
| `FloatTransform.prefix_sums` | `(v: Vec[float]) -> Vec[float]` | `Vec[float]` | Cumulative sum. |
| `FloatTransform.all_positive` | `(v: Vec[float]) -> bool` | `bool` | `true` when every element is `> 0.0`. |
| `FloatTransform.any_positive` | `(v: Vec[float]) -> bool` | `bool` | `true` when at least one element is `> 0.0`. |
| `FloatTransform.all_finite` | `(v: Vec[float]) -> bool` | `bool` | `true` when no element is NaN. |

### Folds

| Method | Signature | Returns | Description |
|---|---|---|---|
| `FloatTransform.fold_add` | `(v: Vec[float], init: float) -> float` | `float` | Fold with `+`. |
| `FloatTransform.fold_mul` | `(v: Vec[float], init: float) -> float` | `float` | Fold with `*`. |

### Example

```tauraro
from std.iter.float_transform import FloatTransform
from std.core.vec import Vec

mut data = Vec[float].init(5)
data.push(1.0)
data.push(3.0)
data.push(5.0)
data.push(2.0)
data.push(4.0)

print(str(FloatTransform.mean(data)))      # 3.0
print(str(FloatTransform.variance(data)))  # 2.0

mut norm = FloatTransform.normalize(data)  # [0.0, 0.5, 1.0, 0.25, 0.75]
mut big  = FloatTransform.filter_gt(data, 2.5)  # [3.0, 5.0, 4.0]
mut sc   = FloatTransform.map_mul(big, 10.0)    # [30.0, 50.0, 40.0]
print(str(FloatTransform.sum(sc)))              # 120.0

mut sorted = FloatTransform.sort_asc(data)      # [1.0, 2.0, 3.0, 4.0, 5.0]
print(str(FloatTransform.last(sorted)))         # 5.0
```
