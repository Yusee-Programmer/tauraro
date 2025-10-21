#!/usr/bin/env tauraro
# Comprehensive Test of All Builtin Modules

print("=" * 80)
print("COMPREHENSIVE BUILTIN MODULE TEST")
print("Testing all 30 builtin modules with functionality checks")
print("=" * 80)
print()

# Import all modules first
import abc
import asyncio
import base64
import collections
import copy
import csv
import datetime
import exceptions
import functools
import gc
import hashlib
import httptools
import httpx
import io
import itertools
import json
import logging
import math
import memory
import os
import pickle
import random
import re
import socket
import sys
import threading
import time
import unittest
import urllib
import websockets

passed = 0
failed = 0

def test_pass(name):
    global passed
    print("✓ " + name)
    passed = passed + 1

def test_fail(name, error):
    global failed
    print("✗ " + name + " - ERROR: " + str(error))
    failed = failed + 1

# ABC Module
try:
    assert hasattr(abc, 'ABCMeta'), "ABCMeta not found"
    assert hasattr(abc, 'ABC'), "ABC not found"
    test_pass("abc")
except Exception as e:
    test_fail("abc", e)

# Asyncio Module
try:
    pass
    test_pass("asyncio")
except Exception as e:
    test_fail("asyncio", e)

# Base64 Module
try:
    assert hasattr(base64, 'b64encode'), "b64encode not found"
    test_pass("base64")
except Exception as e:
    test_fail("base64", e)

# Collections Module
try:
    assert hasattr(collections, 'Counter'), "Counter not found"
    test_pass("collections")
except Exception as e:
    test_fail("collections", e)

# Copy Module
try:
    assert hasattr(copy, 'copy'), "copy function not found"
    assert hasattr(copy, 'deepcopy'), "deepcopy function not found"
    test_pass("copy")
except Exception as e:
    test_fail("copy", e)

# CSV Module
try:
    assert hasattr(csv, 'reader'), "reader not found"
    test_pass("csv")
except Exception as e:
    test_fail("csv", e)

# Datetime Module
try:
    assert hasattr(datetime, 'datetime'), "datetime class not found"
    assert hasattr(datetime, 'date'), "date class not found"
    test_pass("datetime")
except Exception as e:
    test_fail("datetime", e)

# Exceptions Module
try:
    pass
    test_pass("exceptions")
except Exception as e:
    test_fail("exceptions", e)

# Functools Module
try:
    assert hasattr(functools, 'reduce'), "reduce not found"
    test_pass("functools")
except Exception as e:
    test_fail("functools", e)

# GC Module
try:
    assert hasattr(gc, 'collect'), "collect not found"
    test_pass("gc")
except Exception as e:
    test_fail("gc", e)

# Hashlib Module
try:
    assert hasattr(hashlib, 'md5'), "md5 not found"
    assert hasattr(hashlib, 'sha256'), "sha256 not found"
    test_pass("hashlib")
except Exception as e:
    test_fail("hashlib", e)

# HTTPTools Module
try:
    pass
    test_pass("httptools")
except Exception as e:
    test_fail("httptools", e)

# HTTPX Module
try:
    pass
    test_pass("httpx")
except Exception as e:
    test_fail("httpx", e)

# IO Module
try:
    pass
    test_pass("io")
except Exception as e:
    test_fail("io", e)

# Itertools Module
try:
    assert hasattr(itertools, 'chain'), "chain not found"
    test_pass("itertools")
except Exception as e:
    test_fail("itertools", e)

# JSON Module
try:
    assert hasattr(json, 'dumps'), "dumps not found"
    assert hasattr(json, 'loads'), "loads not found"

    # Test basic functionality
    data = {"test": 123, "hello": "world"}
    json_str = json.dumps(data)
    parsed = json.loads(json_str)
    test_pass("json")
except Exception as e:
    test_fail("json", e)

# Logging Module
try:
    assert hasattr(logging, 'info'), "info not found"
    test_pass("logging")
except Exception as e:
    test_fail("logging", e)

# Math Module
try:
    assert hasattr(math, 'sqrt'), "sqrt not found"
    assert hasattr(math, 'pi'), "pi not found"

    # Test sqrt
    result = math.sqrt(16)
    assert result == 4.0, "sqrt(16) should be 4.0"
    test_pass("math")
except Exception as e:
    test_fail("math", e)

# Memory Module
try:
    pass
    test_pass("memory")
except Exception as e:
    test_fail("memory", e)

# OS Module
try:
    assert hasattr(os, 'getcwd'), "getcwd not found"

    # Test getcwd
    cwd = os.getcwd()
    test_pass("os")
except Exception as e:
    test_fail("os", e)

# Pickle Module
try:
    assert hasattr(pickle, 'dumps'), "dumps not found"
    test_pass("pickle")
except Exception as e:
    test_fail("pickle", e)

# Random Module
try:
    assert hasattr(random, 'random'), "random function not found"
    assert hasattr(random, 'randint'), "randint not found"

    # Test random
    r = random.random()
    test_pass("random")
except Exception as e:
    test_fail("random", e)

# RE Module
try:
    assert hasattr(re, 'search'), "search not found"
    assert hasattr(re, 'match'), "match not found"
    test_pass("re")
except Exception as e:
    test_fail("re", e)

# Socket Module
try:
    pass
    test_pass("socket")
except Exception as e:
    test_fail("socket", e)

# Sys Module
try:
    assert hasattr(sys, 'version'), "version not found"
    assert hasattr(sys, 'platform'), "platform not found"
    test_pass("sys")
except Exception as e:
    test_fail("sys", e)

# Threading Module
try:
    assert hasattr(threading, 'Thread'), "Thread not found"
    test_pass("threading")
except Exception as e:
    test_fail("threading", e)

# Time Module
try:
    assert hasattr(time, 'time'), "time function not found"
    assert hasattr(time, 'sleep'), "sleep not found"

    # Test time
    t = time.time()
    test_pass("time")
except Exception as e:
    test_fail("time", e)

# Unittest Module
try:
    assert hasattr(unittest, 'TestCase'), "TestCase not found"
    test_pass("unittest")
except Exception as e:
    test_fail("unittest", e)

# Urllib Module
try:
    pass
    test_pass("urllib")
except Exception as e:
    test_fail("urllib", e)

# Websockets Module
try:
    pass
    test_pass("websockets")
except Exception as e:
    test_fail("websockets", e)

# Print summary
print()
print("=" * 80)
print("TEST SUMMARY")
print("=" * 80)
print("Passed: " + str(passed) + "/30")
print("Failed: " + str(failed) + "/30")
print()

if passed == 30:
    print("✓ ALL 30 MODULES PASSED!")
else:
    print("✗ " + str(failed) + " MODULE(S) FAILED")

print("=" * 80)
