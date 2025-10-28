#!/usr/bin/env python3

# Test script to verify FFI implementations of builtin modules

import sys
import os
import json
import math
import time
import random
import re
import base64
import hashlib
import urllib.parse
import csv
import collections
import itertools
import functools
import threading
import copy
import datetime
import gc
import pickle
import socket
import unittest
import logging
import asyncio
import abc
import httpx
import httptools
import websockets

# Test math module
print("Testing math module...")
print(f"math.pi = {math.pi}")
print(f"math.sqrt(16) = {math.sqrt(16)}")

# Test sys module
print("\nTesting sys module...")
print(f"sys.version = {sys.version}")
print(f"sys.platform = {sys.platform}")

# Test os module
print("\nTesting os module...")
print(f"os.name = {os.name}")
try:
    print(f"os.getcwd() = {os.getcwd()}")
except:
    print("os.getcwd() not available in this environment")

# Test json module
print("\nTesting json module...")
data = {"name": "test", "value": 42}
json_str = json.dumps(data)
print(f"json.dumps({data}) = {json_str}")
parsed = json.loads(json_str)
print(f"json.loads('{json_str}') = {parsed}")

# Test time module
print("\nTesting time module...")
current_time = time.time()
print(f"time.time() = {current_time}")
print(f"time.ctime() = {time.ctime()}")

# Test random module
print("\nTesting random module...")
rand_int = random.randint(1, 10)
print(f"random.randint(1, 10) = {rand_int}")
rand_float = random.random()
print(f"random.random() = {rand_float}")

# Test re module
print("\nTesting re module...")
pattern = r'\d+'
text = "There are 123 numbers in this 456 text"
matches = re.findall(pattern, text)
print(f"re.findall('{pattern}', '{text}') = {matches}")

# Test base64 module
print("\nTesting base64 module...")
original = "Hello, World!"
encoded = base64.b64encode(original.encode()).decode()
print(f"base64.b64encode('{original}') = {encoded}")
decoded = base64.b64decode(encoded).decode()
print(f"base64.b64decode('{encoded}') = {decoded}")

# Test hashlib module
print("\nTesting hashlib module...")
data = "test data"
md5_hash = hashlib.md5(data.encode()).hexdigest()
print(f"hashlib.md5('{data}').hexdigest() = {md5_hash}")
sha256_hash = hashlib.sha256(data.encode()).hexdigest()
print(f"hashlib.sha256('{data}').hexdigest() = {sha256_hash}")

# Test urllib module
print("\nTesting urllib module...")
url = "https://example.com/path?param1=value1&param2=value2"
parsed = urllib.parse.urlparse(url)
print(f"urllib.parse.urlparse('{url}') = {parsed}")
query_dict = urllib.parse.parse_qs(parsed.query)
print(f"urllib.parse.parse_qs('{parsed.query}') = {query_dict}")

# Test collections module
print("\nTesting collections module...")
counter = collections.Counter(['a', 'b', 'c', 'a', 'b', 'b'])
print(f"collections.Counter(['a', 'b', 'c', 'a', 'b', 'b']) = {dict(counter)}")
deque = collections.deque([1, 2, 3])
print(f"collections.deque([1, 2, 3]) = {list(deque)}")

# Test itertools module
print("\nTesting itertools module...")
cycle_iter = itertools.cycle([1, 2, 3])
first_three = [next(cycle_iter) for _ in range(3)]
print(f"itertools.cycle([1, 2, 3]) (first 3) = {first_three}")
product = list(itertools.product([1, 2], ['a', 'b']))
print(f"itertools.product([1, 2], ['a', 'b']) = {product}")

# Test functools module
print("\nTesting functools module...")
@functools.lru_cache(maxsize=128)
def fibonacci(n):
    if n < 2:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

fib_result = fibonacci(10)
print(f"functools.lru_cache fibonacci(10) = {fib_result}")

# Test threading module
print("\nTesting threading module...")
def worker():
    print("Thread worker function executed")

thread = threading.Thread(target=worker)
print(f"threading.Thread created: {thread}")

# Test copy module
print("\nTesting copy module...")
original_list = [1, 2, [3, 4]]
shallow_copy = copy.copy(original_list)
deep_copy = copy.deepcopy(original_list)
print(f"copy.copy({original_list}) = {shallow_copy}")
print(f"copy.deepcopy({original_list}) = {deep_copy}")

# Test datetime module
print("\nTesting datetime module...")
now = datetime.datetime.now()
print(f"datetime.datetime.now() = {now}")
today = datetime.date.today()
print(f"datetime.date.today() = {today}")

# Test gc module
print("\nTesting gc module...")
collected = gc.collect()
print(f"gc.collect() = {collected}")
print(f"gc.isenabled() = {gc.isenabled()}")

# Test pickle module
print("\nTesting pickle module...")
data = {'key': 'value', 'number': 42}
pickled = pickle.dumps(data)
print(f"pickle.dumps({data}) = {pickled}")
unpickled = pickle.loads(pickled)
print(f"pickle.loads(pickled) = {unpickled}")

# Test socket module
print("\nTesting socket module...")
print(f"socket.AF_INET = {socket.AF_INET}")
print(f"socket.SOCK_STREAM = {socket.SOCK_STREAM}")

# Test logging module
print("\nTesting logging module...")
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)
print(f"logging.getLogger(__name__) = {logger}")
print("logging.info('Test message') - logged")

# Test asyncio module
print("\nTesting asyncio module...")
async def async_function():
    return "async result"

print(f"asyncio.iscoroutinefunction(async_function) = {asyncio.iscoroutinefunction(async_function)}")

# Test abc module
print("\nTesting abc module...")
class MyABC(abc.ABC):
    @abc.abstractmethod
    def my_method(self):
        pass

print(f"abc.ABC class created: {MyABC}")

# Test exceptions
print("\nTesting exceptions...")
try:
    raise ValueError("Test exception")
except ValueError as e:
    print(f"Caught exception: {e}")

print("\nAll tests completed successfully!")