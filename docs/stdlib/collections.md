# Collections Module

Specialized container datatypes.

## Counter

Count hashable objects:

```python
from collections import Counter

# Count elements
counter = Counter([1, 2, 2, 3, 3, 3])
print(counter)  # Counter({3: 3, 2: 2, 1: 1})

# Most common
counter.most_common(2)  # [(3, 3), (2, 2)]

# Count words
words = "the quick brown fox jumps over the lazy dog".split()
Counter(words)
```

## defaultdict

Dictionary with default values:

```python
from collections import defaultdict

# Default to int (0)
counts = defaultdict(int)
counts['a'] += 1  # No KeyError!

# Default to list
groups = defaultdict(list)
groups['animals'].append('dog')
```

## deque

Double-ended queue:

```python
from collections import deque

# Create deque
d = deque([1, 2, 3])

# Add to left
d.appendleft(0)  # deque([0, 1, 2, 3])

# Add to right
d.append(4)  # deque([0, 1, 2, 3, 4])

# Remove from left
d.popleft()  # 0

# Remove from right
d.pop()  # 4

# Rotate
d.rotate(1)  # Rotate right
d.rotate(-1)  # Rotate left
```

## namedtuple

Tuple with named fields:

```python
from collections import namedtuple

# Define Point
Point = namedtuple('Point', ['x', 'y'])

# Create instance
p = Point(3, 4)

# Access by name
print(p.x, p.y)  # 3 4

# Access by index
print(p[0], p[1])  # 3 4
```

## OrderedDict

Dictionary that remembers insertion order:

```python
from collections import OrderedDict

# Create ordered dict
od = OrderedDict()
od['c'] = 3
od['a'] = 1
od['b'] = 2

# Order preserved
list(od.keys())  # ['c', 'a', 'b']
```

## ChainMap

Combine multiple dictionaries:

```python
from collections import ChainMap

# Combine dicts
dict1 = {'a': 1, 'b': 2}
dict2 = {'b': 3, 'c': 4}

chain = ChainMap(dict1, dict2)
print(chain['a'])  # 1 (from dict1)
print(chain['b'])  # 2 (from dict1, first dict wins)
print(chain['c'])  # 4 (from dict2)
```

## Next Steps

- [Data Types](../language/data-types.md) - Core types
- [Itertools](itertools.md) - Iterator functions
