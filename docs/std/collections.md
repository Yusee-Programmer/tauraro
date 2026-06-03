# std.collections — Data Structures

```tauraro
from std.collections.stack   import Stack
from std.collections.queue   import Queue
from std.collections.deque   import Deque
from std.collections.set     import Set
from std.collections.counter import Counter
from std.collections.tuple   import Pair, StrPair, Triple
from std.collections.heap    import MinHeap, MaxHeap
from std.collections.list    import LinkedList, ListNode
from std.collections.graph   import Graph, GraphEdge
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

## Counter

**When**: You need to count string occurrences — word frequency, vote tallies, histogram building.
**Why**: Cleaner than a `Map` with manual increment; provides `most_common`, `merge`, and `total`.

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> Counter` | `Counter` | Create an empty counter. |
| `add` | `(key: str)` | `void` | Increment the count for `key` by 1. |
| `add_n` | `(key: str, n: int)` | `void` | Increment the count for `key` by `n`. |
| `count` | `(key: str) -> int` | `int` | Current count (0 if unseen). |
| `total` | `() -> int` | `int` | Sum of all counts. |
| `has` | `(key: str) -> bool` | `bool` | `true` if the key has been counted at least once. |
| `reset` | `(key: str)` | `void` | Set the count for `key` back to 0. |
| `keys` | `() -> Vec[str]` | `Vec[str]` | All keys with a non-zero count. |
| `merge` | `(other: Counter)` | `void` | Add all counts from `other` into `self`. |
| `most_common` | `(n: int) -> Vec[str]` | `Vec[str]` | Top `n` keys by count, descending. |

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
mut top = cnt.most_common(1)
print(top.get(0))               # "cat"
```

---

## Pair / StrPair / Triple

**When**: You need a lightweight, fixed-size group of two or three values — coordinate points, key-value pairs, RGB triples.
**Why**: No heap overhead for a `Vec`; expressive field names (`first`, `second`, `a`, `b`, `c`).

### Pair — two integers

| Method / Field | Signature | Returns | Description |
|---|---|---|---|
| `first` | `int` field | `int` | First element. |
| `second` | `int` field | `int` | Second element. |
| `init` | `(a: int, b: int) -> Pair` | `Pair` | |
| `swap` | `() -> Pair` | `Pair` | New `Pair` with elements swapped. |
| `sum` | `() -> int` | `int` | `first + second` |
| `max` | `() -> int` | `int` | Larger of the two values. |
| `min` | `() -> int` | `int` | Smaller of the two values. |

### StrPair — two strings

| Method / Field | Signature | Returns | Description |
|---|---|---|---|
| `first` | `str` field | `str` | |
| `second` | `str` field | `str` | |
| `init` | `(a: str, b: str) -> StrPair` | `StrPair` | |
| `swap` | `() -> StrPair` | `StrPair` | New `StrPair` with elements swapped. |
| `concat` | `() -> str` | `str` | `first + second` |

### Triple — three integers

| Method / Field | Signature | Returns | Description |
|---|---|---|---|
| `a`, `b`, `c` | `int` fields | `int` | Elements. |
| `init` | `(a: int, b: int, c: int) -> Triple` | `Triple` | |
| `sum` | `() -> int` | `int` | `a + b + c` |
| `max` | `() -> int` | `int` | Largest of the three. |
| `min` | `() -> int` | `int` | Smallest of the three. |

### Example

```tauraro
from std.collections.tuple import Pair, StrPair, Triple

mut p = Pair.init(3, 7)
print(str(p.max()))      # 7
mut sw = p.swap()
print(str(sw.first))     # 7

mut kv = StrPair.init("name", "Alice")
print(kv.concat())       # "nameAlice"

mut t = Triple.init(1, 5, 3)
print(str(t.sum()))      # 9
print(str(t.max()))      # 5
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

