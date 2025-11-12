#!/usr/bin/env python3
"""Test sum() with list comprehension to isolate the issue"""

transactions = [
    {"amount": 100.0, "status": "completed"},
    {"amount": 50.5, "status": "completed"},
    {"amount": 75.25, "status": "pending"},
]

# Test 1: Simple list comprehension
print("Test 1: Extract amounts")
amounts = [t["amount"] for t in transactions]
print(f"amounts = {amounts}")

# Test 2: List comprehension with filter
print("\nTest 2: Extract amounts with filter")
completed_amounts = [t["amount"] for t in transactions if t["status"] == "completed"]
print(f"completed_amounts = {completed_amounts}")

# Test 3: Sum directly
print("\nTest 3: Sum the amounts")
total = sum(completed_amounts)
print(f"total = {total}")

# Test 4: Sum with inline comprehension (the problematic line)
print("\nTest 4: Sum with inline list comprehension")
total_direct = sum([t["amount"] for t in transactions if t["status"] == "completed"])
print(f"total_direct = {total_direct}")
