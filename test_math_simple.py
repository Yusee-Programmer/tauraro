#!/usr/bin/env tauraro
# Simple Math Module Test

print("Testing math module...")
print()

import math

print("Math constants:")
print("  pi = " + str(math.pi))
print()

print("Math functions:")
result = math.sqrt(16)
print("  sqrt(16) = " + str(result))

result = math.floor(3.7)
print("  floor(3.7) = " + str(result))

result = math.ceil(3.2)
print("  ceil(3.2) = " + str(result))

result = math.abs(-5)
print("  abs(-5) = " + str(result))

print()
print("Math module test complete!")
