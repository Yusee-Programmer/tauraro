#!/usr/bin/env python3
# Test basic functionality of builtin modules

print("=== Testing Module Functionality ===\n")

# Test subprocess module
print("1. Testing subprocess module:")
try:
    import subprocess

    # Test simple command
    result = subprocess.run("echo Hello from subprocess")
    print(f"   subprocess.run() executed successfully")
    print(f"   Return code: {result['returncode']}")

    # Test call
    returncode = subprocess.call("echo Testing call")
    print(f"   subprocess.call() returned: {returncode}")

    print("   ✓ subprocess module works!\n")
except Exception as e:
    print(f"   ✗ subprocess module failed: {e}\n")

# Test multiprocessing module
print("2. Testing multiprocessing module:")
try:
    import multiprocessing

    # Test cpu_count
    cpu_count = multiprocessing.cpu_count()
    print(f"   CPU count: {cpu_count}")

    print("   ✓ multiprocessing module works!\n")
except Exception as e:
    print(f"   ✗ multiprocessing module failed: {e}\n")

# Test json module
print("3. Testing json module:")
try:
    import json

    # Test json.dumps
    data = {"name": "Tauraro", "version": "0.2.0", "features": ["fast", "pythonic"]}
    json_str = json.dumps(data)
    print(f"   JSON encoded: {json_str}")

    # Test json.loads
    decoded = json.loads(json_str)
    print(f"   JSON decoded: {decoded}")

    print("   ✓ json module works!\n")
except Exception as e:
    print(f"   ✗ json module failed: {e}\n")

# Test datetime module
print("4. Testing datetime module:")
try:
    import datetime

    # Test creating a date
    d = datetime.date(2025, 1, 1)
    print(f"   Created date: {d}")

    # Test creating a time
    t = datetime.time(12, 30, 45)
    print(f"   Created time: {t}")

    print("   ✓ datetime module works!\n")
except Exception as e:
    print(f"   ✗ datetime module failed: {e}\n")

# Test os module
print("5. Testing os module:")
try:
    import os

    # Test os.getcwd
    cwd = os.getcwd()
    print(f"   Current directory: {cwd}")

    # Test os.environ
    env = os.environ
    print(f"   Environment variables loaded: {type(env)}")

    print("   ✓ os module works!\n")
except Exception as e:
    print(f"   ✗ os module failed: {e}\n")

# Test httptools module
print("6. Testing httptools module:")
try:
    import httptools

    # Test URL parsing
    url_parts = httptools.parse_url("https://example.com:8080/path?query=value")
    print(f"   URL parsed successfully: {type(url_parts)}")

    # Test URL encoding
    encoded = httptools.quote("hello world")
    print(f"   URL encoded 'hello world': {encoded}")

    print("   ✓ httptools module works!\n")
except Exception as e:
    print(f"   ✗ httptools module failed: {e}\n")

print("=== All functionality tests completed! ===")
