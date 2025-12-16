#!/usr/bin/env python3
"""
Benchmark: JSON Parsing and Serialization
Tests: JSON encode/decode, string processing, memory allocation
"""
import time
import sys
import json

def create_test_data(size):
    """Create nested JSON-like data structure"""
    users = []
    for i in range(size):
        user = {
            "id": i,
            "name": f"User {i}",
            "email": f"user{i}@example.com",
            "active": i % 2 == 0,
            "score": i * 1.5,
            "tags": ["tag1", "tag2", "tag3"],
            "metadata": {
                "created": "2024-01-01",
                "updated": "2024-12-01",
                "count": i
            }
        }
        users.append(user)
    return {"users": users}

def json_encode_test(data):
    """Test JSON encoding"""
    return json.dumps(data)

def json_decode_test(json_str):
    """Test JSON decoding"""
    return json.loads(json_str)

def json_process_test(data):
    """Test processing JSON data"""
    total_score = 0
    active_count = 0
    for user in data["users"]:
        total_score += user["score"]
        if user["active"]:
            active_count += 1
    return total_score, active_count

def main():
    size = 1000 if len(sys.argv) < 2 else int(sys.argv[1])

    # Test 1: Create data
    print(f"Test 1: Creating test data ({size} records)")
    start = time.time()
    data = create_test_data(size)
    elapsed1 = time.time() - start
    print(f"  Records created: {len(data['users'])}")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: JSON encoding
    print(f"\nTest 2: JSON encoding")
    start = time.time()
    json_str = json_encode_test(data)
    elapsed2 = time.time() - start
    print(f"  JSON size: {len(json_str)} bytes")
    print(f"  Time: {elapsed2:.4f} seconds")

    # Test 3: JSON decoding
    print(f"\nTest 3: JSON decoding")
    start = time.time()
    decoded_data = json_decode_test(json_str)
    elapsed3 = time.time() - start
    print(f"  Records decoded: {len(decoded_data['users'])}")
    print(f"  Time: {elapsed3:.4f} seconds")

    # Test 4: Process JSON data
    print(f"\nTest 4: Processing JSON data")
    start = time.time()
    total_score, active_count = json_process_test(decoded_data)
    elapsed4 = time.time() - start
    print(f"  Total score: {total_score}")
    print(f"  Active users: {active_count}")
    print(f"  Time: {elapsed4:.4f} seconds")

    total_time = elapsed1 + elapsed2 + elapsed3 + elapsed4
    print(f"\nTotal time: {total_time:.4f} seconds")
    return total_time

if __name__ == "__main__":
    main()
