#!/usr/bin/env python3
"""
Benchmark: Memory Allocation and GC
Tests: Object creation, list/dict allocation, memory pressure
"""
import time
import sys

class Point:
    """Simple class for object allocation testing"""
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance(self):
        return (self.x ** 2 + self.y ** 2) ** 0.5

def object_allocation_test(count):
    """Test object allocation"""
    points = []
    for i in range(count):
        points.append(Point(i, i * 2))
    return points

def list_allocation_test(size):
    """Test list allocation and manipulation"""
    lists = []
    for i in range(size):
        lst = list(range(100))
        lists.append(lst)
    return len(lists)

def dict_allocation_test(size):
    """Test dictionary allocation"""
    dicts = []
    for i in range(size):
        d = {f"key_{j}": j for j in range(50)}
        dicts.append(d)
    return len(dicts)

def nested_structure_test(depth, width):
    """Test nested data structures"""
    def create_tree(level):
        if level == 0:
            return {"value": level, "leaf": True}
        return {
            "value": level,
            "children": [create_tree(level - 1) for _ in range(width)]
        }
    return create_tree(depth)

def main():
    count = 100000 if len(sys.argv) < 2 else int(sys.argv[1])

    # Test 1: Object allocation
    print(f"Test 1: Object allocation ({count} objects)")
    start = time.time()
    points = object_allocation_test(count)
    elapsed1 = time.time() - start
    print(f"  Objects created: {len(points)}")
    print(f"  Sample distance: {points[0].distance():.2f}")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: List allocation
    list_count = int(count / 10)
    print(f"\nTest 2: List allocation ({list_count} lists)")
    start = time.time()
    num_lists = list_allocation_test(list_count)
    elapsed2 = time.time() - start
    print(f"  Lists created: {num_lists}")
    print(f"  Time: {elapsed2:.4f} seconds")

    # Test 3: Dict allocation
    dict_count = int(count / 10)
    print(f"\nTest 3: Dict allocation ({dict_count} dicts)")
    start = time.time()
    num_dicts = dict_allocation_test(dict_count)
    elapsed3 = time.time() - start
    print(f"  Dicts created: {num_dicts}")
    print(f"  Time: {elapsed3:.4f} seconds")

    # Test 4: Nested structures
    depth = 8
    width = 3
    print(f"\nTest 4: Nested structures (depth={depth}, width={width})")
    start = time.time()
    tree = nested_structure_test(depth, width)
    elapsed4 = time.time() - start
    print(f"  Tree root value: {tree['value']}")
    print(f"  Children count: {len(tree['children'])}")
    print(f"  Time: {elapsed4:.4f} seconds")

    total_time = elapsed1 + elapsed2 + elapsed3 + elapsed4
    print(f"\nTotal time: {total_time:.4f} seconds")
    return total_time

if __name__ == "__main__":
    main()
