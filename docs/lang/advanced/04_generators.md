# Advanced — Generators

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

Tauraro has two related but distinct constructs for working with sequences:

- **List comprehensions** — eager, produce a full `List[T]` immediately
- **Generator expressions** — lazy, produce elements on demand without allocating the full sequence

Both use similar syntax. The difference is `[]` (list) vs. `()` (generator).

---

## When You Need This

- You are transforming or filtering a sequence and only need to iterate over it once — a generator avoids allocating the full list
- You are working with sequences so large they would not fit comfortably in memory
- You want to express pipeline-style data processing clearly
- You are iterating a sequence and stopping early — a generator computes only the elements you actually consume

If you need random access, multiple passes, or to keep the result, use a list comprehension or collect the generator into a list.

---

## Syntax Reference

### List Comprehension (eager)

```python
[expression for variable in iterable]
[expression for variable in iterable if condition]
```

Produces a `List[T]` immediately. All elements are computed at construction time.

### Generator Expression (lazy)

```python
(expression for variable in iterable)
(expression for variable in iterable if condition)
```

Produces a generator. Elements are computed only when consumed by a `for` loop or collecting function. No memory is allocated for the full sequence upfront.

### Nested Comprehension

```python
[inner_expr for inner_var in inner_iter for outer_var in outer_iter]
```

Equivalent to a nested `for` loop, with the outer loop on the right.

---

## Examples

### List comprehension — eager, produces a List

```python
# All squares from 0 to 9
squares = [x * x for x in range(10)]
print(squares)    # [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

# Only positive values
mut data = [3, -1, 4, -1, 5, -9, 2, -6]
mut positives = [x for x in data if x > 0]
print(positives)    # [3, 4, 5, 2]

# Transform strings
mut names = ["alice", "bob", "charlie"]
mut upper = [name.upper() for name in names]
```

### Generator expression — lazy, no upfront allocation

```python
# Squares of a million numbers — no List[int] created
gen = (x * x for x in range(1000000))

# Elements computed only as the loop runs
mut total = 0
for val in gen:
    total = total + val
    if total > 1000000: break    # we consumed only what we needed
```

### Filtered generator

```python
mut lines = read_file_lines("data.txt")    # returns List[str]

# Only lines starting with "#" — no intermediate list allocated
comment_gen = (line for line in lines if line.starts_with("#"))

for comment in comment_gen:
    print(comment)
```

### Nested comprehension — matrix

```python
# 5x5 multiplication table as a list of lists
matrix = [[i * j for j in range(1, 6)] for i in range(1, 6)]

# matrix[2][3] == 12  (3 * 4)
for row in matrix:
    print(row)
```

### Collecting a generator into a List

When you need random access or multiple passes over a lazily-generated sequence, collect it:

```python
gen = (x * x for x in range(100) if x % 2 == 0)
mut squares_of_evens: List[int] = list(gen)    # collect into List
print(squares_of_evens[5])    # random access now possible
```

---

## Key Differences: List Comprehension vs Generator

| Property | `[expr for x in it]` | `(expr for x in it)` |
|----------|----------------------|----------------------|
| Evaluates | Immediately | On demand |
| Memory | Full List allocated | One element at a time |
| Iterations | Unlimited | Once only |
| Random access | Yes (`[i]`) | No |
| Type | `List[T]` | generator |

**Generators can only be iterated once.** After the `for` loop exhausts the generator, it is empty. Create a new generator expression to iterate again.

```python
gen = (x * x for x in range(5))
for v in gen: print(v)    # prints 0, 1, 4, 9, 16
for v in gen: print(v)    # prints nothing — generator exhausted
```

---

## Async Generators

For async contexts, generator expressions work inside `async def` functions and can yield values from async operations:

```python
async def fetch_pages(urls: List[str]):
    gen = (await fetch(url) for url in urls)
    for page in gen:
        process(page)
```

Each `fetch(url)` is awaited in sequence. The generator is still lazy — it awaits the next URL only when the `for` loop advances.

---

## Common Mistakes

**Iterating a generator twice.** A generator is single-use. If you need to iterate the same sequence twice, either use a list comprehension or collect the generator into a list first.

**Using a generator where a list is expected.** A generator cannot be indexed (`gen[5]` is an error). If you need random access, collect it.

**Very heavy work in generator expressions.** A generator expression is syntactic sugar for a class implementing `__iter__` and `__next__`. Heavy computation inside a generator doesn't parallelize automatically — for parallel element-wise work, use `gpu:` with a list (see chapter 18).

**Nested generators with side effects.** The order and timing of element computation in a nested generator depends on how it is iterated. Avoid side effects inside generator expressions.

---

## Best Practices

- **Default to list comprehensions** for clarity when the sequence fits in memory. Use generators only when you have measured a memory benefit or need lazy evaluation.
- **Use generators for streaming data.** Reading large files line by line, processing API responses in chunks, or pipelining transformations are natural fits for generators.
- **Give generators descriptive names** when you assign them to variables — `even_squares` is clearer than `gen`.
- **Collect immediately when you need more than one pass.** Don't iterate a generator, realize you need to iterate again, then add a `list()` call as an afterthought — decide upfront.

---

See also:
- [07 — Collections](../07_collections.md)
- [04 — Control Flow](../04_control_flow.md) (for loops)
- [21 — Operator Overloading](../21_operator_overloading.md) (iterator protocol: `__iter__`, `__next__`)
