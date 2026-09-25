# std.collections — Data Structures

```tauraro
from std.collections.vec     import Vec
from std.collections.dict    import MultiDict
from std.collections.stack   import Stack
from std.collections.queue   import Queue
from std.collections.deque   import Deque
from std.collections.set     import Set
from std.collections.counter import Counter
from std.collections.tuple   import Pair, StrPair, Triple
from std.collections.heap    import MinHeap, MaxHeap
from std.collections.list    import LinkedList, ListNode
from std.collections.graph   import Graph, GraphEdge
from std.collections.lru     import LruCache
from std.collections.btree   import BTreeMap
from std.collections.trie      import Trie
from std.collections.bloom     import BloomFilter
from std.collections.unionfind import UnionFind
```

---

## Vec[T]

**When**: You need a generic, growable, random-access array of any element type — the building block most other collections in this module are implemented on top of.
**Why**: Re-exported from `std.core.vec`; backed by a raw malloc'd buffer with doubling growth, generic over `T`.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(cap: int) -> Vec[T]` | `Vec[T]` | Create an empty vec with the given initial capacity (minimum 4). |
| `push` | `(val: T)` | `void` | Append an element, doubling capacity if full. |
| `append` | `(val: T)` | `void` | Alias for `push` (matches the builtin `List` API). |
| `pop` | `() -> T` | `T` | Remove and return the last element. |
| `get` | `(i: int) -> T` | `T` | Element at index `i`. |
| `set` | `(i: int, val: T)` | `void` | Overwrite the element at index `i`. |
| `len` | `int` field | `int` | Current number of elements. |
| `capacity` | `int` field | `int` | Current backing-buffer capacity. |
| `extend` | `(other: Vec[T])` | `void` | Append all elements from `other`. |
| `contains` | `(val: T) -> bool` | `bool` | `true` if any element equals `val` (uses `==`). |
| `remove` | `(i: int)` | `void` | Remove the element at index `i`, shifting later elements left. |
| `swap` | `(a: int, b: int)` | `void` | Swap the elements at indices `a` and `b`. |
| `is_empty` | `() -> bool` | `bool` | |
| `clear` | `()` | `void` | Reset `len` to 0 (keeps the backing buffer). |
| `first` | `() -> T` | `T` | First element. Caller must ensure the vec is non-empty. |
| `last` | `() -> T` | `T` | Last element. Caller must ensure the vec is non-empty. |
| `index_of` | `(val: T) -> int` | `int` | First index of `val`, or `-1` if absent. |
| `last_index_of` | `(val: T) -> int` | `int` | Last index of `val`, or `-1` if absent. |
| `count` | `(val: T) -> int` | `int` | Number of elements equal to `val`. |
| `reverse` | `()` | `void` | Reverse the elements in place. |
| `reversed` | `() -> Vec[T]` | `Vec[T]` | New vec with elements in reverse order (original unchanged). |
| `clone` | `() -> Vec[T]` | `Vec[T]` | Shallow copy with its own backing buffer. |
| `copy` | `() -> Vec[T]` | `Vec[T]` | Alias for `clone`. |
| `reserve` | `(min_cap: int)` | `void` | Grow capacity (by doubling) to at least `min_cap`. Never shrinks. |
| `truncate` | `(n: int)` | `void` | Shorten to `n` elements (no-op if `n >= len`). |
| `sum` | `() -> T` | `T` | Sum of all elements (`T` must support `+`). Vec must be non-empty. |
| `min_val` | `() -> T` | `T` | Smallest element (`T` must support `<`). Vec must be non-empty. |
| `max_val` | `() -> T` | `T` | Largest element (`T` must support `<`). Vec must be non-empty. |
| `sort` | `()` | `void` | In-place ascending insertion sort (`T` must support `<`). |
| `sort_desc` | `()` | `void` | In-place descending sort. |
| `free` | `()` | `void` | Free the backing buffer and reset to empty. |

### Example

```tauraro
from std.collections.vec import Vec

mut v = Vec[int].init(4)
v.push(3)
v.push(1)
v.push(2)
v.sort()                    # [1, 2, 3]
print(str(v.get(0)))        # 1
print(str(v.sum()))         # 6
print(str(v.contains(2)))   # true
v.free()
```

---

## Stack

**When**: You need last-in, first-out (LIFO) semantics — undo history, expression evaluation, DFS.
**Why**: O(1) push/pop; simpler than a raw `Vec` for stack algorithms.
**Backed by**: `Vec[int]`.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> Stack` | `Stack` | Create an empty stack. |
| `push` | `(v: int)` | `void` | Push a value onto the top. |
| `pop` | `() -> int` | `int` | Remove and return the top value. Returns `0` if empty. |
| `peek` | `() -> int` | `int` | Return the top value without removing. Returns `0` if empty. |
| `is_empty` | `() -> bool` | `bool` | `true` when the stack has no elements. |
| `len` | `() -> int` | `int` | Number of elements. |

### Example

```tauraro
from std.collections.stack import Stack

mut st = Stack.init()
st.push(10)
st.push(20)
print(str(st.peek()))   # 20  — look without removing
print(str(st.pop()))    # 20  — remove top
print(str(st.len()))    # 1
```

---

## Queue

**When**: You need first-in, first-out (FIFO) ordering — task scheduling, BFS, producer-consumer.
**Why**: Ring-buffer internals give O(1) enqueue and dequeue without shifting elements.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(cap: int) -> Queue` | `Queue` | Create a queue with given ring-buffer capacity. |
| `enqueue` | `(v: int)` | `void` | Add a value at the back. |
| `dequeue` | `() -> int` | `int` | Remove and return the front value. Returns `0` if empty. |
| `peek` | `() -> int` | `int` | Return the front value without removing. |
| `is_empty` | `() -> bool` | `bool` | |
| `is_full` | `() -> bool` | `bool` | `true` when the ring buffer is at capacity. |
| `len` | `() -> int` | `int` | |
| `drain` | `()` | `void` | Remove all items. |

### Example

```tauraro
from std.collections.queue import Queue

mut q = Queue.init(8)
q.enqueue(1)
q.enqueue(2)
q.enqueue(3)
print(str(q.dequeue()))  # 1  — FIFO order
print(str(q.len()))      # 2
```

---

## Deque

**When**: You need O(1) insertion and removal at **both** ends — sliding windows, palindrome checks, BFS variants.
**Why**: A double-ended queue avoids the cost of prepending to a plain array.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(cap: int) -> Deque` | `Deque` | Create a deque with given capacity. |
| `push_back` | `(v: int)` | `void` | Add to the back. |
| `push_front` | `(v: int)` | `void` | Add to the front. |
| `pop_back` | `() -> int` | `int` | Remove and return the back element. Returns `0` if empty. |
| `pop_front` | `() -> int` | `int` | Remove and return the front element. Returns `0` if empty. |
| `get` | `(i: int) -> int` | `int` | Element at index `i` (0 = front). |
| `is_empty` | `() -> bool` | `bool` | |
| `len` | `() -> int` | `int` | |

### Example

```tauraro
from std.collections.deque import Deque

mut dq = Deque.init(8)
dq.push_back(2)
dq.push_front(1)   # [1, 2]
print(str(dq.pop_front()))  # 1
print(str(dq.pop_back()))   # 2
```

---

## Set

**When**: You need fast membership testing and automatic deduplication — tag lists, visited nodes, unique IDs.
**Why**: Hash-table internals give O(1) average insert/lookup; built-in set algebra (union, intersection, difference).

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(cap: int) -> Set` | `Set` | Create a set with initial hash-table capacity. |
| `add` | `(key: str)` | `void` | Insert a key. Duplicates are silently ignored. |
| `remove` | `(key: str)` | `void` | Remove a key. No-op if absent. |
| `contains` | `(key: str) -> bool` | `bool` | `true` if `key` is present. |
| `is_empty` | `() -> bool` | `bool` | |
| `len` | `() -> int` | `int` | Number of unique keys. |
| `clear` | `()` | `void` | Remove all elements and reset the hash table. |
| `to_vec` | `() -> Vec[str]` | `Vec[str]` | All live keys as a vector (insertion order). |
| `union` | `(other: Set) -> Set` | `Set` | Keys present in either set (no duplicates). |
| `intersection` | `(other: Set) -> Set` | `Set` | Keys present in both sets. |
| `difference` | `(other: Set) -> Set` | `Set` | Keys in `self` but not in `other`. |
| `symmetric_difference` | `(other: Set) -> Set` | `Set` | Keys in exactly one of the two sets. |
| `is_subset` | `(other: Set) -> bool` | `bool` | `true` if every key of `self` is in `other`. |
| `is_superset` | `(other: Set) -> bool` | `bool` | `true` if every key of `other` is in `self`. |
| `equals` | `(other: Set) -> bool` | `bool` | `true` if both sets contain exactly the same keys. |

### Example

```tauraro
from std.collections.set import Set

mut s = Set.init(16)
s.add("apple")
s.add("banana")
s.add("apple")               # duplicate — no effect
print(str(s.len()))          # 2
print(str(s.contains("banana")))  # true

mut s2 = Set.init(8)
s2.add("banana")
s2.add("cherry")
mut inter  = s.intersection(s2)
mut sym    = s.symmetric_difference(s2)
print(str(inter.len()))           # 1  ("banana")
print(str(sym.len()))             # 2  ("apple", "cherry")
print(str(s.is_subset(s2)))       # false
print(str(s2.is_superset(s)))     # false
print(str(s.equals(s)))           # true
```

---

## MultiDict

**When**: You need a simple string-to-string map with a tracked element count — config maps, header tables, simple lookups.
**Why**: Thin convenience wrapper over `Map[str]` with `Dict`-like naming (`set`/`get`/`has`/`remove`).
**Backed by**: `Map[str]`.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> MultiDict` | `MultiDict` | Create an empty map. |
| `set` | `(key: str, value: str)` | `void` | Insert or overwrite the value for `key`. |
| `get` | `(key: str) -> str` | `str` | Value for `key`, or `""` if absent. |
| `has` | `(key: str) -> bool` | `bool` | `true` if `key` is present. |
| `remove` | `(key: str)` | `void` | Remove `key`. No-op if absent. |
| `len` | `() -> int` | `int` | Number of entries. |

### Example

```tauraro
from std.collections.dict import MultiDict

mut d = MultiDict.init()
d.set("name", "Alice")
d.set("role", "admin")
print(d.get("name"))        # "Alice"
print(str(d.has("role")))   # true
d.remove("role")
print(str(d.len()))          # 1
print(d.get("role"))         # ""  — absent key
```

---

## Counter

**When**: You need to count string occurrences — word frequency, vote tallies, histogram building.
**Why**: Cleaner than a `Map` with manual increment; tracks both per-key and overall totals.
**Backed by**: `Map[int]`.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> Counter` | `Counter` | Create an empty counter. |
| `add` | `(key: str)` | `void` | Increment the count for `key` by 1 (inserts with count 1 if absent). Also increments the running total. |
| `count` | `(key: str) -> int` | `int` | Current count for `key` (0 if never added). |
| `total` | `() -> int` | `int` | Sum of all `add()` calls made on this counter. |
| `has` | `(key: str) -> bool` | `bool` | `true` if the key has been added at least once. |
| `reset` | `(key: str)` | `void` | Remove `key` entirely and subtract its count from the running total. |

### Example

```tauraro
from std.collections.counter import Counter

mut cnt = Counter.init()
cnt.add("cat")
cnt.add("dog")
cnt.add("cat")
cnt.add("cat")
print(str(cnt.count("cat")))    # 3
print(str(cnt.total()))         # 4
print(str(cnt.has("dog")))      # true
cnt.reset("dog")
print(str(cnt.has("dog")))      # false
print(str(cnt.total()))         # 3
```

---

## Pair / StrPair / Triple

**When**: You need a lightweight, fixed-size group of two or three values — coordinate points, key-value pairs, RGB triples.
**Why**: No heap overhead for a `Vec`; expressive field names (`first`, `second`, `third`).

### Pair — two integers

| Method / Field | Signature | Returns | Description |
|---|---|---|---|
| `first` | `int` field | `int` | First element. |
| `second` | `int` field | `int` | Second element. |
| `init` | `(a: int, b: int) -> Pair` | `Pair` | |
| `swap` | `() -> Pair` | `Pair` | New `Pair` with elements swapped. |
| `eq` | `(other: Pair) -> bool` | `bool` | `true` if both elements are equal. |
| `free` | `(self)` | `void` | Free the heap allocation backing this `Pair`. |

### StrPair — two strings

| Method / Field | Signature | Returns | Description |
|---|---|---|---|
| `first` | `str` field | `str` | |
| `second` | `str` field | `str` | |
| `init` | `(a: str, b: str) -> StrPair` | `StrPair` | |
| `eq` | `(other: StrPair) -> bool` | `bool` | `true` if both elements are equal. |
| `free` | `(self)` | `void` | Free the heap allocation backing this `StrPair`. |

### Triple — three integers

| Method / Field | Signature | Returns | Description |
|---|---|---|---|
| `first`, `second`, `third` | `int` fields | `int` | Elements. |
| `init` | `(a: int, b: int, c: int) -> Triple` | `Triple` | |
| `free` | `(self)` | `void` | Free the heap allocation backing this `Triple`. |

### Example

```tauraro
from std.collections.tuple import Pair, StrPair, Triple

mut p = Pair.init(3, 7)
mut sw = p.swap()
print(str(sw.first))     # 7
print(str(p.eq(sw)))     # false

mut kv = StrPair.init("name", "Alice")
print(kv.first + kv.second)  # "nameAlice"

mut t = Triple.init(1, 5, 3)
print(str(t.first + t.second + t.third))  # 9
```

---

## MinHeap

**When**: You need to efficiently get the **smallest** element repeatedly — Dijkstra's algorithm, merge K sorted lists, job scheduling by priority.
**Why**: O(log n) push/pop; always returns the minimum in O(1) via `peek`.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `new` | `() -> MinHeap` | `MinHeap` | Create an empty min-heap. |
| `push` | `(val: int)` | `void` | Insert a value; maintains heap property (bubble-up). |
| `pop` | `() -> int` | `int` | Remove and return the minimum. Returns `0` if empty. |
| `peek` | `() -> int` | `int` | Return the minimum without removing. Returns `0` if empty. |
| `is_empty` | `() -> bool` | `bool` | |
| `len` | `() -> int` | `int` | Number of elements currently in the heap. |
| `to_sorted` | `() -> Vec[int]` | `Vec[int]` | Drain into a new sorted ascending `Vec[int]` (non-destructive copy). |

### Example

```tauraro
from std.collections.heap import MinHeap

mut h = MinHeap.new()
h.push(5)
h.push(1)
h.push(3)
print(str(h.peek()))     # 1  — minimum
print(str(h.pop()))      # 1
print(str(h.pop()))      # 3
print(str(h.pop()))      # 5

mut h2 = MinHeap.new()
h2.push(9); h2.push(2); h2.push(7)
mut sorted = h2.to_sorted()   # [2, 7, 9]
print(str(sorted.get(0)))     # 2
```

---

## MaxHeap

**When**: You need to efficiently get the **largest** element repeatedly — top-K queries, priority queues for max-weight tasks.
**Why**: Mirror of `MinHeap` with reversed comparisons; O(log n) push/pop.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `new` | `() -> MaxHeap` | `MaxHeap` | Create an empty max-heap. |
| `push` | `(val: int)` | `void` | Insert a value; maintains heap property (bubble-up). |
| `pop` | `() -> int` | `int` | Remove and return the maximum. Returns `0` if empty. |
| `peek` | `() -> int` | `int` | Return the maximum without removing. Returns `0` if empty. |
| `is_empty` | `() -> bool` | `bool` | |
| `len` | `() -> int` | `int` | Number of elements. |
| `to_sorted_desc` | `() -> Vec[int]` | `Vec[int]` | Drain into a new descending `Vec[int]` (non-destructive copy). |

### Example

```tauraro
from std.collections.heap import MaxHeap

mut h = MaxHeap.new()
h.push(5)
h.push(1)
h.push(9)
print(str(h.peek()))     # 9  — maximum
print(str(h.pop()))      # 9
print(str(h.pop()))      # 5

mut sorted_desc = h.to_sorted_desc()   # [1]
print(str(sorted_desc.get(0)))         # 1
```

---

## Graph / GraphEdge

**When**: You need to model relationships between nodes — routing, dependency resolution, network topology, social graphs.
**Why**: Adjacency-list storage scales well to sparse graphs; includes BFS, DFS, path detection, and degree queries out of the box.

### GraphEdge

Represents a directed weighted edge.

| Field | Type | Description |
|---|---|---|
| `to` | `int` | Destination node ID. |
| `weight` | `int` | Edge weight. |

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(to: int, weight: int) -> GraphEdge` | `GraphEdge` | Create an edge record. |

### Graph

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(max_nodes: int, directed: bool) -> Graph` | `Graph` | Create a graph for up to `max_nodes` nodes. Pass `directed=false` for undirected (edges added both ways). |
| `add_edge` | `(src: int, to: int, weight: int)` | `void` | Add a weighted edge `src → to`. Undirected graphs also add `to → src`. |
| `bfs` | `(start: int) -> Vec[int]` | `Vec[int]` | Node IDs reachable from `start` in level order. |
| `dfs` | `(start: int) -> Vec[int]` | `Vec[int]` | Node IDs reachable from `start` in DFS pre-order. |
| `has_path` | `(src: int, dst: int) -> bool` | `bool` | `true` when any path exists from `src` to `dst`. |
| `neighbors` | `(node: int) -> Vec[int]` | `Vec[int]` | All direct neighbors of `node`. |
| `edge_weight` | `(src: int, to: int) -> int` | `int` | Weight of `src → to`, or `-1` if no such edge. |
| `in_degree` | `(node: int) -> int` | `int` | Number of edges arriving at `node`. |
| `out_degree` | `(node: int) -> int` | `int` | Number of edges leaving `node`. |
| `degree` | `(node: int) -> int` | `int` | Total degree: `in_degree + out_degree` for directed; `out_degree` for undirected. |
| `all_nodes` | `() -> Vec[int]` | `Vec[int]` | All node IDs from `0` to `node_count - 1`. |
| `has_cycle` | `() -> bool` | `bool` | `true` when the directed graph contains at least one cycle (DFS back-edge). |
| `topological_sort` | `() -> Vec[int]` | `Vec[int]` | Nodes in topological order (valid on DAGs only; DFS post-order reversed). |

### Fields

| Field | Type | Description |
|---|---|---|
| `node_count` | `int` | Highest node ID seen + 1. |
| `edge_count` | `int` | Total edge records stored. |
| `directed` | `bool` | `true` for directed, `false` for undirected. |

### Example

```tauraro
from std.collections.graph import Graph

# Build a directed graph: 0→1 (w=5), 1→2 (w=3), 0→2 (w=10)
mut g = Graph.init(5, true)
g.add_edge(0, 1, 5)
g.add_edge(1, 2, 3)
g.add_edge(0, 2, 10)

print(str(g.has_path(0, 2)))       # true
print(str(g.edge_weight(1, 2)))    # 3
print(str(g.out_degree(0)))        # 2

mut order = g.bfs(0)               # [0, 1, 2]
print(str(order.len()))            # 3

mut nb = g.neighbors(0)            # [1, 2]
print(str(nb.get(0)))              # 1

# Degree, all_nodes, cycle detection, topological sort
print(str(g.out_degree(0)))        # 2
print(str(g.degree(0)))            # 2 (in_degree=0, so same)
mut all = g.all_nodes()            # [0, 1, 2]
print(str(g.has_cycle()))          # false  (acyclic DAG)
mut topo = g.topological_sort()    # e.g. [0, 1, 2]
print(str(topo.len()))             # 3

# Add back-edge to create a cycle
g.add_edge(2, 0, 1)
print(str(g.has_cycle()))          # true
```

---

## LinkedList

**When**: You need O(1) prepend, cheap removal from front, or want to build algorithms that work pointer-by-pointer — e.g. merge sort, LRU cache eviction, undo chains.
**Why**: Heap-allocated nodes with a `next` pointer; `prepend` and `pop_front` are O(1). Use `Vec` for random access instead.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> LinkedList` | `LinkedList` | Create an empty list. |
| `prepend` | `(value: int)` | `void` | Insert at the head in O(1). |
| `append` | `(value: int)` | `void` | Insert at the tail in O(n). |
| `pop_front` | `() -> int` | `int` | Remove and return the head value. Returns `0` if empty. |
| `remove` | `(v: int)` | `void` | Remove the first node whose value equals `v`. No-op if absent. |
| `insert_at` | `(index: int, value: int)` | `void` | Insert before the node at `index` (0 = new head). |
| `get` | `(index: int) -> int` | `int` | Value at `index`. Returns `0` if out of range. |
| `contains` | `(value: int) -> bool` | `bool` | `true` if any node holds `value`. |
| `is_empty` | `() -> bool` | `bool` | `true` when the list has no nodes. |
| `len` | `int` field | `int` | Current node count. |
| `to_vec` | `() -> Vec[int]` | `Vec[int]` | Copy all elements head→tail into a `Vec[int]`. |
| `reverse` | `()` | `void` | Reverse the list in place in O(n). |
| `clear` | `()` | `void` | Free all nodes and reset to empty. |

### Example

```tauraro
from std.collections.list import LinkedList

mut ll = LinkedList.init()
ll.append(1)
ll.append(2)
ll.append(3)
ll.prepend(0)               # [0, 1, 2, 3]
print(str(ll.len))          # 4
print(str(ll.get(2)))       # 2

ll.remove(2)                # [0, 1, 3]
ll.insert_at(1, 99)         # [0, 99, 1, 3]
print(str(ll.get(1)))       # 99

ll.reverse()                # [3, 1, 99, 0]
mut v = ll.to_vec()
print(str(v.get(0)))        # 3

print(str(ll.pop_front()))  # 3  → removes head
ll.clear()
print(str(ll.is_empty()))   # true
```

## LruCache[K, V]

**When**: You need a fixed-capacity cache that automatically evicts the least-recently-used entry once it's full — memoizing an expensive lookup, capping an in-memory response cache, bounding a connection/resource pool by recency.
**Why**: O(1) average `put`/`fetch`/`remove` — a hash table for key→slot lookup plus an intrusive doubly-linked list for O(1) recency tracking and eviction. Not a `List`-backed linear scan.
**Backed by**: a builtin `List[LruNode[K, V]]` slab (recycled via a free-list) and a builtin `List[LruIndexEntry[K]]` open-addressing hash index.

> **Naming note**: lookups are `fetch`/`fetch_or`, not `get`/`get_or`. A compiler bug mis-infers the return type of a method literally named `get`/`get_or`/`pop` on a generic class with 2+ type parameters (it returns the class's *first* type argument — here `K`, the key type — instead of the method's actual declared return type). Renaming the accessors sidesteps it; see the implementation comments in `std/collections/lru.tr` for the full analysis.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(capacity: int) -> LruCache[K, V]` | `LruCache[K, V]` | Create an empty cache holding at most `capacity` entries (minimum 1). |
| `has` | `(key: K) -> bool` | `bool` | `true` if `key` is currently present. Does not affect recency. |
| `fetch` | `(key: K) -> V` | `V` | Look up `key`, refreshing its recency on a hit. Call `has(key)` first (or use `fetch_or`) — the return value on a miss is an arbitrary default, not a sentinel. |
| `fetch_or` | `(key: K, default_val: V) -> V` | `V` | Look up `key`, returning `default_val` if absent. A hit still refreshes recency exactly like `fetch`. |
| `put` | `(key: K, value: V)` | `void` | Insert or update `key` → `value`. An existing key is updated and moved to most-recently-used **without evicting anything**. A new key that would exceed capacity evicts the least-recently-used entry first. |
| `remove` | `(key: K)` | `void` | Explicitly remove `key`. No-op if absent. Does not count as a "use" of any other entry. |
| `clear` | `()` | `void` | Remove every entry. Keeps the allocated slab and hash table for reuse. |
| `len` | `() -> int` | `int` | Current number of entries. |
| `capacity` | `() -> int` | `int` | Maximum number of entries. |
| `is_empty` | `() -> bool` | `bool` | |
| `is_full` | `() -> bool` | `bool` | `true` when `len() >= capacity()`. |

### Example

```tauraro
from std.collections.lru import LruCache

mut cache = LruCache[str, int].init(2)
cache.put("a", 1)
cache.put("b", 2)              # cache is now full: {a, b}, MRU -> LRU: b, a

print(str(cache.fetch("a")))   # 1  -- reading "a" makes it most-recently-used
cache.put("c", 3)               # over capacity -> evicts LRU, which is now "b" (not "a")

print(str(cache.has("b")))     # false -- evicted
print(str(cache.has("a")))     # true  -- spared because it was just read
print(str(cache.has("c")))     # true  -- newly inserted

cache.put("a", 99)              # update an existing key -- no eviction
print(str(cache.fetch("a")))   # 99

print(str(cache.fetch_or("missing", -1)))  # -1  -- absent key, explicit default
```

## BTreeMap[K, V]

**When**: You need keys kept in **sorted order** with efficient ordered iteration and range queries — a task queue by priority timestamp, a leaderboard, an interval index, anything where `Dict[K, V]`'s hash-based (unordered) iteration isn't good enough.
**Why**: `Dict[K, V]` and `Set[T]` in this language are hash tables — fast lookup, but no ordering guarantee at all. `BTreeMap` fills that gap: `.keys()` / `.values()` always come back in ascending key order, and `.range(lo, hi)` answers "give me every key between these two bounds" in `O(log n + k)` instead of a full scan.

**Implementation note (read this before assuming B-tree internals):** despite the name — chosen to match the familiar `BTreeMap` API from Rust's `std::collections`, Java's `TreeMap`, etc. — this is **not** a multi-way B-tree internally. It is a height-balanced **AVL binary search tree**: each node holds exactly one key/value pair plus a cached subtree height, and every `insert`/`remove` rebalances via rotations. This gives every user-facing guarantee a literal B-tree would (`O(log n)` insert/get/remove, `O(log n)` min/max, sorted-order iteration, `O(log n + k)` range queries) with a substantially simpler, easier-to-verify rebalancing path than true multi-key node splitting/merging. If you specifically need the on-disk or cache-line-optimized node-fanout properties of a literal B-tree, this module is not that — it's a sorted-map ADT with B-tree-shaped ergonomics.

Key comparisons use plain `<` / `>` / `==`, so any key type supporting those operators works out of the box — verified here for both `int` and `str` keys.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> BTreeMap[K, V]` | `BTreeMap[K, V]` | Create an empty sorted map. |
| `insert` | `(key: K, val: V)` | `void` | Insert a new key, or overwrite the value if `key` already exists (count only grows on a genuinely new key). |
| `set` | `(key: K, val: V)` | `void` | Alias for `insert` (matches the builtin `Dict`-style API). |
| `get` | `(key: K) -> V` | `V` | Value for `key`. Returns the type's default (`0` / `""` / etc.) if absent. |
| `has` | `(key: K) -> bool` | `bool` | `true` if `key` is present. |
| `remove` | `(key: K)` | `void` | Delete `key` if present (no-op otherwise). Real structural deletion — handles the leaf, one-child, and two-child (in-order-successor splice) cases, then rebalances back up to the root. |
| `len` | `() -> int` | `int` | Number of key/value pairs. |
| `is_empty` | `() -> bool` | `bool` | |
| `min` | `() -> K` | `K` | Smallest key. `O(log n)` — walks left-spine, not a scan. Undefined on an empty map. |
| `max` | `() -> K` | `K` | Largest key. `O(log n)` — walks right-spine, not a scan. Undefined on an empty map. |
| `keys` | `() -> Vec[K]` | `Vec[K]` | All keys, **ascending sorted order**, via an in-order traversal. |
| `values` | `() -> Vec[V]` | `Vec[V]` | All values, ordered to align index-for-index with `keys()` (i.e. sorted by key, not by value). |
| `range` | `(lo: K, hi: K) -> Vec[K]` | `Vec[K]` | Keys `k` with `lo <= k < hi` (half-open — `hi` is **exclusive**), ascending sorted order. Empty `Vec` if `lo >= hi` or nothing falls in range. |

### Example

```tauraro
from std.collections.btree import BTreeMap

mut m = BTreeMap[int, str].init()
m.insert(5, "five")
m.insert(1, "one")
m.insert(8, "eight")
m.insert(3, "three")

mut v: str = m.get(5)        # explicit type annotation -- see note below
print(v)                     # "five"
print(str(m.has(99)))        # false

# Ordered iteration — the entire point of this data structure:
mut ks = m.keys()
print(str(ks.get(0)))        # 1  (smallest first, regardless of insertion order)
print(str(ks.get(3)))        # 8  (largest last)

print(str(m.min()))          # 1
print(str(m.max()))          # 8

# Range query: all keys in [3, 8)
mut r = m.range(3, 8)
print(str(r.len))            # 2   -> [3, 5]

m.remove(5)                  # real deletion, rebalances the tree
print(str(m.len()))          # 3
print(str(m.has(5)))         # false

# str keys work identically (any type supporting <, >, == does):
mut sm = BTreeMap[str, int].init()
sm.insert("banana", 2)
sm.insert("apple", 1)
sm.insert("cherry", 3)
mut sk = sm.keys()
print(sk.get(0))              # "apple"  -- lexicographic order
```

> **Known compiler gotcha with `get()`.** Calling `.get()` on a `BTreeMap[K, V]`
> where the call's result type is inferred (no explicit type annotation on the
> receiving `mut` binding, or passed directly as a call argument) can resolve
> to the wrong monomorphized type when `K` and `V` differ (a pre-existing
> compiler inference gap around generic classes with two type parameters,
> not specific to this module — see `bug2.txt`). Always give the `mut`
> binding an explicit type, as in `mut v: str = m.get(5)` above, when working
> with a `BTreeMap[K, V]` whose `K` and `V` are different types.

---

## Trie[V]

**When**: You need prefix-based lookups over string keys — autocomplete suggestions, spell-check dictionaries, IP-routing-style longest-prefix structures, or any "give me every key starting with X" query that a hash-based `Dict` can't answer without a full scan.
**Why**: Node-based prefix tree with a 256-entry child-pointer table per node (indexed directly by byte value — no hashing, no per-edge string allocation). `insert`/`get`/`has_prefix`/`remove` are all `O(len(key))`; `starts_with` is `O(len(prefix) + number of matching keys)`.

Keys are treated as raw byte sequences, so lookups walk one byte at a time — correct for ASCII keys, and for UTF-8 keys where only whole-key equality matters (not per-codepoint semantics).

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> Trie[V]` | `Trie[V]` | Create an empty trie. |
| `insert` | `(key: str, value: V)` | `void` | Insert or overwrite the value for `key`. |
| `get` | `(key: str) -> Option[V]` | `Option[V]` | `Some(value)` if `key` is present, `None` otherwise. |
| `has` | `(key: str) -> bool` | `bool` | `true` if `key` itself was inserted (not just a prefix of some other key). |
| `has_prefix` | `(prefix: str) -> bool` | `bool` | `true` if any stored key begins with `prefix` (including `prefix` being empty, or equal to a stored key). |
| `starts_with` | `(prefix: str) -> Vec[str]` | `Vec[str]` | All stored keys beginning with `prefix`, ascending byte order. Empty `Vec` if none match. |
| `remove` | `(key: str) -> bool` | `bool` | Remove `key` if present; returns `true` if a key was actually removed. Prunes now-empty nodes back up the path, but never a node still shared by another stored key — removing one key never disturbs sibling keys that share a prefix with it. |
| `len` | `() -> int` | `int` | Number of distinct keys currently stored. |
| `is_empty` | `() -> bool` | `bool` | |

### Example

```tauraro
from std.collections.trie import Trie

mut t = Trie[int].init()
t.insert("cat", 1)
t.insert("car", 2)
t.insert("card", 3)
t.insert("dog", 4)

mut g = t.get("cat")
print(str(g.is_some()))       # true
print(str(g.unwrap()))        # 1

print(str(t.has_prefix("ca")))  # true
print(str(t.has_prefix("xz")))  # false

mut matches = t.starts_with("ca")   # ["car", "card", "cat"]
print(str(matches.len()))           # 3

t.remove("car")
print(str(t.has("car")))    # false -- removed
print(str(t.has("card")))   # true  -- sibling key untouched
print(str(t.has("cat")))    # true  -- sibling key untouched
print(str(t.len()))         # 3
```

---

## BloomFilter

**When**: You need fast, memory-cheap "have I probably seen this before?" checks over a large or unbounded set of items where occasional false positives are acceptable but false negatives are not — deduplicating a huge stream, a first-pass filter before an expensive database lookup, cache-miss avoidance.
**Why**: A Bloom filter trades exactness for space: it never false-negatives (if `might_contain` returns `false`, the item was definitely never added) but can false-positive (occasionally reports "might be present" for something never added). There is no way to remove an item once added, and no way to enumerate members — it only answers membership queries.

`BloomFilter.init` computes the bit-array size `m` and hash-function count `k` from the standard formulas given `expected_items` (`n`) and `false_positive_rate` (`p`):

```
m = -(n * ln(p)) / (ln(2)^2)
k = (m / n) * ln(2)
```

Internally it uses double hashing (Kirsch-Mitzenmacher): two independent base hashes (FNV-1a and a djb2-style hash) are combined as `h1 + i*h2` for the `k` probe indices, giving the statistical effect of `k` independent hash functions from only two real hash computations per operation.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(expected_items: int, false_positive_rate: float) -> BloomFilter` | `BloomFilter` | Create a filter sized for `expected_items` entries at approximately `false_positive_rate` (e.g. `0.01` for 1%). |
| `add` | `(item: str)` | `void` | Add an item. |
| `might_contain` | `(item: str) -> bool` | `bool` | `true` = "possibly present" (may be a false positive). `false` = "definitely absent" — **never** a false negative for anything actually `add`ed. |
| `len` | `() -> int` | `int` | Number of `add()` calls made so far (not distinct items — a Bloom filter can't tell a repeat add from a new one). |
| `bit_size` | `() -> int` | `int` | The computed bit-array size `m`. |
| `hash_count` | `() -> int` | `int` | The computed number of hash probes `k` performed per `add`/`might_contain`. |

### Example

```tauraro
from std.collections.bloom import BloomFilter

mut f = BloomFilter.init(1000, 0.01)   # ~1000 items, ~1% false-positive rate

mut i = 0
while i < 500:
    f.add("item-" + str(i))
    i = i + 1

# Zero false negatives: every added item is always reported present.
print(str(f.might_contain("item-42")))    # true

# Never-added items are usually (but not guaranteed) reported absent.
print(str(f.might_contain("never-added-xyz")))   # false, most of the time
```

---

## UnionFind

**When**: You need to track and merge groups of related elements and answer "are these two in the same group?" — Kruskal's minimum spanning tree, detecting cycles in an undirected graph, image-processing connected-component labeling, grouping friends/accounts by connectivity.
**Why**: Union-by-rank plus full path compression gives amortized-near-`O(1)` (inverse-Ackermann) `find`/`union`/`connected` — far better than re-scanning group membership on every query.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(n: int) -> UnionFind` | `UnionFind` | Create `n` singleton components (indices `0..n-1`, each its own component). |
| `find` | `(x: int) -> int` | `int` | Representative (root) of `x`'s component. Compresses the path to the root as a side effect. |
| `union` | `(a: int, b: int)` | `void` | Merge the components containing `a` and `b`. No-op if already the same component. |
| `connected` | `(a: int, b: int) -> bool` | `bool` | `true` if `a` and `b` are currently in the same component. |
| `component_count` | `() -> int` | `int` | Number of distinct connected components remaining. |
| `size` | `() -> int` | `int` | Total number of elements this `UnionFind` was initialized with. |

### Example

```tauraro
from std.collections.unionfind import UnionFind

mut uf = UnionFind.init(10)
print(str(uf.component_count()))   # 10 -- all singletons

uf.union(0, 1)
uf.union(1, 2)
uf.union(3, 4)
print(str(uf.connected(0, 2)))     # true  -- 0-1-2 merged
print(str(uf.connected(0, 3)))     # false -- different component
print(str(uf.component_count()))   # 7

uf.union(2, 3)
print(str(uf.connected(0, 4)))     # true -- {0,1,2} and {3,4} now joined
print(str(uf.component_count()))   # 6
```
