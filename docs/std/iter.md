# std.iter — Ranges and Vector Transforms

```tauraro
from std.iter.range     import Range
from std.iter.transform import Transform
```

All methods are **static** — called as `Range.method(...)` or `Transform.method(...)`.  
All transform functions operate on `Vec[int]` and return **new** vectors; originals are never modified.

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
```
