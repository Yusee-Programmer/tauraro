#!/usr/bin/env python3
# Test that all builtin modules can be imported

print("Testing module imports...")

# Test async and HTTP modules
try:
    import asyncio
    print("✓ asyncio imported successfully")
except Exception as e:
    print(f"✗ asyncio import failed: {e}")

try:
    import httpx
    print("✓ httpx imported successfully")
except Exception as e:
    print(f"✗ httpx import failed: {e}")

try:
    import httptools
    print("✓ httptools imported successfully")
except Exception as e:
    print(f"✗ httptools import failed: {e}")

try:
    import websockets
    print("✓ websockets imported successfully")
except Exception as e:
    print(f"✗ websockets import failed: {e}")

# Test new modules
try:
    import subprocess
    print("✓ subprocess imported successfully")
except Exception as e:
    print(f"✗ subprocess import failed: {e}")

try:
    import multiprocessing
    print("✓ multiprocessing imported successfully")
except Exception as e:
    print(f"✗ multiprocessing import failed: {e}")

# Test other core modules
try:
    import json
    print("✓ json imported successfully")
except Exception as e:
    print(f"✗ json import failed: {e}")

try:
    import os
    print("✓ os imported successfully")
except Exception as e:
    print(f"✗ os import failed: {e}")

try:
    import sys
    print("✓ sys imported successfully")
except Exception as e:
    print(f"✗ sys import failed: {e}")

try:
    import datetime
    print("✓ datetime imported successfully")
except Exception as e:
    print(f"✗ datetime import failed: {e}")

print("\nAll module import tests completed!")
