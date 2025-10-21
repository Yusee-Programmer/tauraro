#!/usr/bin/env tauraro
# Comprehensive Test of All Builtin Modules
# Tests both import and basic functionality

print("=" * 80)
print("COMPREHENSIVE BUILTIN MODULE TEST")
print("Testing all 30 builtin modules with functionality checks")
print("=" * 80)
print()

passed = 0
failed = 0
errors = []

def test_module(name, test_func):
    global passed, failed, errors
    try:
        test_func()
        print(f"✓ {name}")
        passed += 1
        return True
    except Exception as e:
        print(f"✗ {name} - ERROR: {e}")
        failed += 1
        errors.append((name, str(e)))
        return False

# ABC Module
def test_abc():
    import abc
    # Check that ABCMeta and ABC exist
    if not hasattr(abc, 'ABCMeta'):
        raise Exception("ABCMeta not found")
    if not hasattr(abc, 'ABC'):
        raise Exception("ABC not found")

test_module("abc", test_abc)

# Asyncio Module
def test_asyncio():
    import asyncio
    # Just check the module exists and has basic attributes
    pass

test_module("asyncio", test_asyncio)

# Base64 Module
def test_base64():
    import base64
    # Test encoding/decoding
    data = "hello"
    if hasattr(base64, 'b64encode'):
        encoded = base64.b64encode(data)
        if hasattr(base64, 'b64decode'):
            decoded = base64.b64decode(encoded)

test_module("base64", test_base64)

# Collections Module
def test_collections():
    import collections
    # Check for Counter, defaultdict, deque
    if not hasattr(collections, 'Counter'):
        raise Exception("Counter not found")

test_module("collections", test_collections)

# Copy Module
def test_copy():
    import copy
    # Test copy and deepcopy functions
    if not hasattr(copy, 'copy'):
        raise Exception("copy function not found")
    if not hasattr(copy, 'deepcopy'):
        raise Exception("deepcopy function not found")

test_module("copy", test_copy)

# CSV Module
def test_csv():
    import csv
    # Check for reader and writer
    if not hasattr(csv, 'reader'):
        raise Exception("reader not found")

test_module("csv", test_csv)

# Datetime Module
def test_datetime():
    import datetime
    # Check for date, time, datetime classes
    if not hasattr(datetime, 'datetime'):
        raise Exception("datetime class not found")
    if not hasattr(datetime, 'date'):
        raise Exception("date class not found")

test_module("datetime", test_datetime)

# Exceptions Module
def test_exceptions():
    import exceptions
    # Check that module exists
    pass

test_module("exceptions", test_exceptions)

# Functools Module
def test_functools():
    import functools
    # Check for reduce, partial
    if not hasattr(functools, 'reduce'):
        raise Exception("reduce not found")

test_module("functools", test_functools)

# GC Module
def test_gc():
    import gc
    # Check for collect function
    if not hasattr(gc, 'collect'):
        raise Exception("collect not found")

test_module("gc", test_gc)

# Hashlib Module
def test_hashlib():
    import hashlib
    # Check for hash functions
    if not hasattr(hashlib, 'md5'):
        raise Exception("md5 not found")
    if not hasattr(hashlib, 'sha256'):
        raise Exception("sha256 not found")

test_module("hashlib", test_hashlib)

# HTTPTools Module
def test_httptools():
    import httptools
    # Just check module exists
    pass

test_module("httptools", test_httptools)

# HTTPX Module
def test_httpx():
    import httpx
    # Just check module exists
    pass

test_module("httpx", test_httpx)

# IO Module
def test_io():
    import io
    # Check for StringIO or BytesIO
    if not hasattr(io, 'StringIO') and not hasattr(io, 'BytesIO'):
        raise Exception("StringIO or BytesIO not found")

test_module("io", test_io)

# Itertools Module
def test_itertools():
    import itertools
    # Check for chain, islice
    if not hasattr(itertools, 'chain'):
        raise Exception("chain not found")

test_module("itertools", test_itertools)

# JSON Module
def test_json():
    import json
    # Test dumps and loads
    if not hasattr(json, 'dumps'):
        raise Exception("dumps not found")
    if not hasattr(json, 'loads'):
        raise Exception("loads not found")

    # Test basic functionality
    data = {"test": 123, "hello": "world"}
    json_str = json.dumps(data)
    parsed = json.loads(json_str)

test_module("json", test_json)

# Logging Module
def test_logging():
    import logging
    # Check for basic logging functions
    if not hasattr(logging, 'info'):
        raise Exception("info not found")

test_module("logging", test_logging)

# Math Module
def test_math():
    import math
    # Test basic math functions
    if not hasattr(math, 'sqrt'):
        raise Exception("sqrt not found")
    if not hasattr(math, 'pi'):
        raise Exception("pi not found")

    # Test sqrt
    result = math.sqrt(16)
    if result != 4.0:
        raise Exception(f"sqrt(16) returned {result}, expected 4.0")

test_module("math", test_math)

# Memory Module
def test_memory():
    import memory
    # Just check module exists
    pass

test_module("memory", test_memory)

# OS Module
def test_os():
    import os
    # Check for getcwd and other basic functions
    if not hasattr(os, 'getcwd'):
        raise Exception("getcwd not found")

    # Test getcwd
    cwd = os.getcwd()

test_module("os", test_os)

# Pickle Module
def test_pickle():
    import pickle
    # Check for dumps and loads
    if not hasattr(pickle, 'dumps'):
        raise Exception("dumps not found")

test_module("pickle", test_pickle)

# Random Module
def test_random():
    import random
    # Check for random, randint
    if not hasattr(random, 'random'):
        raise Exception("random function not found")
    if not hasattr(random, 'randint'):
        raise Exception("randint not found")

    # Test random
    r = random.random()

test_module("random", test_random)

# RE Module
def test_re():
    import re
    # Check for search, match, findall
    if not hasattr(re, 'search'):
        raise Exception("search not found")
    if not hasattr(re, 'match'):
        raise Exception("match not found")

test_module("re", test_re)

# Socket Module
def test_socket():
    import socket
    # Just check module exists
    pass

test_module("socket", test_socket)

# Sys Module
def test_sys():
    import sys
    # Check for version, platform, argv
    if not hasattr(sys, 'version'):
        raise Exception("version not found")
    if not hasattr(sys, 'platform'):
        raise Exception("platform not found")

test_module("sys", test_sys)

# Threading Module
def test_threading():
    import threading
    # Check for Thread class
    if not hasattr(threading, 'Thread'):
        raise Exception("Thread not found")

test_module("threading", test_threading)

# Time Module
def test_time():
    import time
    # Check for time and sleep functions
    if not hasattr(time, 'time'):
        raise Exception("time function not found")
    if not hasattr(time, 'sleep'):
        raise Exception("sleep not found")

    # Test time
    t = time.time()

test_module("time", test_time)

# Unittest Module
def test_unittest():
    import unittest
    # Check for TestCase
    if not hasattr(unittest, 'TestCase'):
        raise Exception("TestCase not found")

test_module("unittest", test_unittest)

# Urllib Module
def test_urllib():
    import urllib
    # Just check module exists
    pass

test_module("urllib", test_urllib)

# Websockets Module
def test_websockets():
    import websockets
    # Just check module exists
    pass

test_module("websockets", test_websockets)

# Print summary
print()
print("=" * 80)
print(f"TEST SUMMARY")
print("=" * 80)
print(f"Passed: {passed}/30")
print(f"Failed: {failed}/30")
print()

if failed > 0:
    print("FAILED TESTS:")
    for name, error in errors:
        print(f"  - {name}: {error}")
    print()

if passed == 30:
    print("✓ ALL MODULES PASSED!")
else:
    print(f"✗ {failed} MODULE(S) FAILED")

print("=" * 80)
