#!/usr/bin/env tauraro
# Minimal Module Test - Tests functionality directly

print("=" * 80)
print("MINIMAL BUILTIN MODULE TEST")
print("=" * 80)
print()

# Math Module
print("Testing math module...")
import math
result = math.sqrt(16)
print("  math.sqrt(16) = " + str(result))
print("  math.pi = " + str(math.pi))
print("✓ math")
print()

# JSON Module
print("Testing json module...")
import json
data = {"test": 123, "hello": "world"}
json_str = json.dumps(data)
print("  json.dumps: " + json_str)
parsed = json.loads(json_str)
print("  json.loads: " + str(parsed))
print("✓ json")
print()

# Random Module
print("Testing random module...")
import random
r = random.random()
print("  random.random(): " + str(r))
i = random.randint(1, 10)
print("  random.randint(1, 10): " + str(i))
print("✓ random")
print()

# Time Module
print("Testing time module...")
import time
t = time.time()
print("  time.time(): " + str(t))
print("✓ time")
print()

# OS Module
print("Testing os module...")
import os
cwd = os.getcwd()
print("  os.getcwd(): " + cwd)
print("✓ os")
print()

# Sys Module
print("Testing sys module...")
import sys
print("  sys.version: " + str(sys.version))
print("  sys.platform: " + str(sys.platform))
print("✓ sys")
print()

# Collections Module
print("Testing collections module...")
import collections
counter = collections.Counter([1, 2, 2, 3, 3, 3])
print("  Counter([1,2,2,3,3,3]): " + str(counter))
print("✓ collections")
print()

# Functools Module
print("Testing functools module...")
import functools
print("  functools module loaded")
print("✓ functools")
print()

# Itertools Module
print("Testing itertools module...")
import itertools
chained = list(itertools.chain([1, 2], [3, 4]))
print("  chain([1,2], [3,4]): " + str(chained))
print("✓ itertools")
print()

# Datetime Module
print("Testing datetime module...")
import datetime
now = datetime.datetime.now()
print("  datetime.now(): " + str(now))
print("✓ datetime")
print()

# RE Module
print("Testing re module...")
import re
match = re.search("test", "this is a test")
if match:
    print("  re.search('test', 'this is a test'): Found match")
else:
    print("  re.search: No match")
print("✓ re")
print()

# Copy Module
print("Testing copy module...")
import copy
original = [1, 2, [3, 4]]
shallow = copy.copy(original)
deep = copy.deepcopy(original)
print("  copy.copy and deepcopy: OK")
print("✓ copy")
print()

# Hashlib Module
print("Testing hashlib module...")
import hashlib
hash_obj = hashlib.md5("hello")
print("  hashlib.md5('hello'): Created hash")
print("✓ hashlib")
print()

# Base64 Module
print("Testing base64 module...")
import base64
encoded = base64.b64encode("hello")
print("  base64.b64encode('hello'): " + str(encoded))
print("✓ base64")
print()

# GC Module
print("Testing gc module...")
import gc
gc.collect()
print("  gc.collect(): OK")
print("✓ gc")
print()

# Logging Module
print("Testing logging module...")
import logging
logging.info("Test message")
print("  logging.info(): OK")
print("✓ logging")
print()

# Threading Module
print("Testing threading module...")
import threading
thread_count = threading.active_count()
print("  threading.active_count(): " + str(thread_count))
print("✓ threading")
print()

# Pickle Module
print("Testing pickle module...")
import pickle
data_to_pickle = {"key": "value"}
pickled = pickle.dumps(data_to_pickle)
print("  pickle.dumps(): OK")
print("✓ pickle")
print()

# CSV Module
print("Testing csv module...")
import csv
print("  csv module loaded")
print("✓ csv")
print()

# ABC Module
print("Testing abc module...")
import abc
print("  abc module loaded")
print("✓ abc")
print()

# Unittest Module
print("Testing unittest module...")
import unittest
print("  unittest module loaded")
print("✓ unittest")
print()

# Asyncio Module
print("Testing asyncio module...")
import asyncio
print("  asyncio module loaded")
print("✓ asyncio")
print()

# HTTPTools Module
print("Testing httptools module...")
import httptools
print("  httptools module loaded")
print("✓ httptools")
print()

# HTTPX Module
print("Testing httpx module...")
import httpx
print("  httpx module loaded")
print("✓ httpx")
print()

# Websockets Module
print("Testing websockets module...")
import websockets
print("  websockets module loaded")
print("✓ websockets")
print()

# Urllib Module
print("Testing urllib module...")
import urllib
print("  urllib module loaded")
print("✓ urllib")
print()

# IO Module
print("Testing io module...")
import io
print("  io module loaded")
print("✓ io")
print()

# Memory Module
print("Testing memory module...")
import memory
print("  memory module loaded")
print("✓ memory")
print()

# Exceptions Module
print("Testing exceptions module...")
import exceptions
print("  exceptions module loaded")
print("✓ exceptions")
print()

# Socket Module
print("Testing socket module...")
import socket
print("  socket module loaded")
print("✓ socket")
print()

print("=" * 80)
print("ALL 30 MODULES TESTED SUCCESSFULLY!")
print("=" * 80)
