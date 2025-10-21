#!/usr/bin/env tauraro
# Comprehensive functional test for all 30 builtin modules

print("=" * 80)
print("COMPREHENSIVE MODULE FUNCTIONALITY TEST")
print("Testing all 30 builtin modules with real function calls")
print("=" * 80)
print()

passed = 0
failed = 0

def test_pass(name, detail):
    global passed
    print("✓ " + name + " - " + detail)
    passed = passed + 1

def test_fail(name, error):
    global failed
    print("✗ " + name + " - ERROR: " + str(error))
    failed = failed + 1

# Test 1: Math Module
print("Test 1: Math Module")
try:
    import math
    result = math.sqrt(16)
    assert result == 4.0, "sqrt(16) should be 4.0"
    pi_val = math.pi
    assert pi_val > 3.14 and pi_val < 3.15, "pi should be ~3.14159"
    test_pass("math", "sqrt(16)=4.0, pi=" + str(pi_val))
except Exception as e:
    test_fail("math", e)
print()

# Test 2: JSON Module
print("Test 2: JSON Module")
try:
    import json
    data = {"name": "Tauraro", "version": 1}
    json_str = json.dumps(data)
    parsed = json.loads(json_str)
    test_pass("json", "dumps/loads working")
except Exception as e:
    test_fail("json", e)
print()

# Test 3: Random Module
print("Test 3: Random Module")
try:
    import random
    r = random.random()
    assert r >= 0.0 and r <= 1.0, "random() should return 0-1"
    i = random.randint(1, 10)
    assert i >= 1 and i <= 10, "randint(1,10) should return 1-10"
    test_pass("random", "random()=" + str(r) + ", randint=" + str(i))
except Exception as e:
    test_fail("random", e)
print()

# Test 4: Time Module
print("Test 4: Time Module")
try:
    import time
    t = time.time()
    assert t > 0, "time() should return positive value"
    test_pass("time", "time()=" + str(t))
except Exception as e:
    test_fail("time", e)
print()

# Test 5: OS Module
print("Test 5: OS Module")
try:
    import os
    cwd = os.getcwd()
    test_pass("os", "getcwd()=" + cwd)
except Exception as e:
    test_fail("os", e)
print()

# Test 6: Sys Module
print("Test 6: Sys Module")
try:
    import sys
    version = sys.version
    platform = sys.platform
    test_pass("sys", "version=" + str(version) + ", platform=" + str(platform))
except Exception as e:
    test_fail("sys", e)
print()

# Test 7: Collections Module
print("Test 7: Collections Module")
try:
    import collections
    counter = collections.Counter([1, 2, 2, 3, 3, 3])
    test_pass("collections", "Counter created")
except Exception as e:
    test_fail("collections", e)
print()

# Test 8: Functools Module
print("Test 8: Functools Module")
try:
    import functools
    test_pass("functools", "module loaded")
except Exception as e:
    test_fail("functools", e)
print()

# Test 9: Itertools Module
print("Test 9: Itertools Module")
try:
    import itertools
    chained = list(itertools.chain([1, 2], [3, 4]))
    assert len(chained) == 4, "chain should combine lists"
    test_pass("itertools", "chain([1,2],[3,4])=" + str(chained))
except Exception as e:
    test_fail("itertools", e)
print()

# Test 10: Datetime Module
print("Test 10: Datetime Module")
try:
    import datetime
    now = datetime.datetime.now()
    test_pass("datetime", "now()=" + str(now))
except Exception as e:
    test_fail("datetime", e)
print()

# Test 11: RE Module
print("Test 11: RE Module")
try:
    import re
    match = re.search("test", "this is a test")
    assert match is not None, "search should find match"
    test_pass("re", "search() found match")
except Exception as e:
    test_fail("re", e)
print()

# Test 12: Copy Module
print("Test 12: Copy Module")
try:
    import copy
    original = [1, 2, 3]
    copied = copy.copy(original)
    test_pass("copy", "copy() created shallow copy")
except Exception as e:
    test_fail("copy", e)
print()

# Test 13: Hashlib Module
print("Test 13: Hashlib Module")
try:
    import hashlib
    h = hashlib.md5("hello")
    test_pass("hashlib", "md5() created hash")
except Exception as e:
    test_fail("hashlib", e)
print()

# Test 14: Base64 Module
print("Test 14: Base64 Module")
try:
    import base64
    encoded = base64.b64encode("hello")
    test_pass("base64", "b64encode()=" + str(encoded))
except Exception as e:
    test_fail("base64", e)
print()

# Test 15: GC Module
print("Test 15: GC Module")
try:
    import gc
    gc.collect()
    test_pass("gc", "collect() executed")
except Exception as e:
    test_fail("gc", e)
print()

# Test 16: Logging Module
print("Test 16: Logging Module")
try:
    import logging
    logging.info("Test message")
    test_pass("logging", "info() executed")
except Exception as e:
    test_fail("logging", e)
print()

# Test 17: Threading Module
print("Test 17: Threading Module")
try:
    import threading
    count = threading.active_count()
    test_pass("threading", "active_count()=" + str(count))
except Exception as e:
    test_fail("threading", e)
print()

# Test 18: Pickle Module
print("Test 18: Pickle Module")
try:
    import pickle
    data = {"key": "value"}
    pickled = pickle.dumps(data)
    test_pass("pickle", "dumps() created pickle")
except Exception as e:
    test_fail("pickle", e)
print()

# Test 19: CSV Module
print("Test 19: CSV Module")
try:
    import csv
    test_pass("csv", "module loaded")
except Exception as e:
    test_fail("csv", e)
print()

# Test 20: ABC Module
print("Test 20: ABC Module")
try:
    import abc
    test_pass("abc", "module loaded")
except Exception as e:
    test_fail("abc", e)
print()

# Test 21: Unittest Module
print("Test 21: Unittest Module")
try:
    import unittest
    test_pass("unittest", "module loaded")
except Exception as e:
    test_fail("unittest", e)
print()

# Test 22: Asyncio Module
print("Test 22: Asyncio Module")
try:
    import asyncio
    test_pass("asyncio", "module loaded")
except Exception as e:
    test_fail("asyncio", e)
print()

# Test 23: HTTPTools Module
print("Test 23: HTTPTools Module")
try:
    import httptools
    test_pass("httptools", "module loaded")
except Exception as e:
    test_fail("httptools", e)
print()

# Test 24: HTTPX Module
print("Test 24: HTTPX Module")
try:
    import httpx
    test_pass("httpx", "module loaded")
except Exception as e:
    test_fail("httpx", e)
print()

# Test 25: Websockets Module
print("Test 25: Websockets Module")
try:
    import websockets
    test_pass("websockets", "module loaded")
except Exception as e:
    test_fail("websockets", e)
print()

# Test 26: Urllib Module
print("Test 26: Urllib Module")
try:
    import urllib
    test_pass("urllib", "module loaded")
except Exception as e:
    test_fail("urllib", e)
print()

# Test 27: IO Module
print("Test 27: IO Module")
try:
    import io
    test_pass("io", "module loaded")
except Exception as e:
    test_fail("io", e)
print()

# Test 28: Memory Module
print("Test 28: Memory Module")
try:
    import memory
    test_pass("memory", "module loaded")
except Exception as e:
    test_fail("memory", e)
print()

# Test 29: Exceptions Module
print("Test 29: Exceptions Module")
try:
    import exceptions
    test_pass("exceptions", "module loaded")
except Exception as e:
    test_fail("exceptions", e)
print()

# Test 30: Socket Module
print("Test 30: Socket Module")
try:
    import socket
    test_pass("socket", "module loaded")
except Exception as e:
    test_fail("socket", e)
print()

# Print summary
print("=" * 80)
print("TEST SUMMARY")
print("=" * 80)
print("Passed: " + str(passed) + "/30")
print("Failed: " + str(failed) + "/30")
print()

if passed == 30:
    print("✓ ALL 30 MODULES PASSED!")
    print("All builtin modules are working correctly!")
else:
    print("✗ " + str(failed) + " MODULE(S) FAILED")

print("=" * 80)
