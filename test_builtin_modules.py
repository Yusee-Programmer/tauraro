# Test script for builtin modules
import math
import sys
import os
import json
import re
import datetime
import collections
import itertools
import functools
import base64
import hashlib
import urllib.parse
import csv
import copy
import pickle
import gc
import random
import time

# Math module test
print("Math module test:")
print("Pi:", math.pi)
print("Square root of 16:", math.sqrt(16))
print("Sin of pi/2:", math.sin(math.pi / 2))

# Sys module test
print("\nSys module test:")
print("Platform:", sys.platform)
print("Version:", sys.version)

# OS module test
print("\nOS module test:")
print("Current directory:", os.getcwd())

# JSON module test
print("\nJSON module test:")
data = {"name": "Tauraro", "version": "0.2.0"}
json_str = json.dumps(data)
print("JSON string:", json_str)
parsed_data = json.loads(json_str)
print("Parsed data:", parsed_data)

# Regex module test
print("\nRegex module test:")
text = "Hello, Tauraro!"
match = re.search(r"Tauraro", text)
if match:
    print("Found match:", match.group())

# Datetime module test
print("\nDatetime module test:")
now = datetime.datetime.now()
print("Current time:", now)

# Collections module test
print("\nCollections module test:")
counter = collections.Counter(['a', 'b', 'c', 'a', 'b', 'b'])
print("Counter:", counter)

# Itertools module test
print("\nItertools module test:")
perms = list(itertools.permutations([1, 2, 3], 2))
print("Permutations:", perms)

# Functools module test
print("\nFunctools module test:")
@functools.lru_cache(maxsize=128)
def fibonacci(n):
    if n < 2:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

print("Fibonacci(10):", fibonacci(10))

# Base64 module test
print("\nBase64 module test:")
encoded = base64.b64encode(b"Hello, Tauraro!")
print("Base64 encoded:", encoded)
decoded = base64.b64decode(encoded)
print("Base64 decoded:", decoded.decode())

# Hashlib module test
print("\nHashlib module test:")
hasher = hashlib.md5()
hasher.update(b"Hello, Tauraro!")
print("MD5 hash:", hasher.hexdigest())

# Urllib module test
print("\nUrllib module test:")
quoted = urllib.parse.quote("Hello Tauraro!")
print("URL quoted:", quoted)
unquoted = urllib.parse.unquote(quoted)
print("URL unquoted:", unquoted)

# Copy module test
print("\nCopy module test:")
original = [1, 2, [3, 4]]
shallow = copy.copy(original)
deep = copy.deepcopy(original)
print("Original:", original)
print("Shallow copy:", shallow)
print("Deep copy:", deep)

# Pickle module test
print("\nPickle module test:")
pickled = pickle.dumps(original)
print("Pickle data:", pickled)
unpickled = pickle.loads(pickled)
print("Unpickled data:", unpickled)

# GC module test
print("\nGC module test:")
collected = gc.collect()
print("Objects collected:", collected)

# Random module test
print("\nRandom module test:")
print("Random number:", random.random())
print("Random integer:", random.randint(1, 10))

# Time module test
print("\nTime module test:")
start = time.time()
time.sleep(0.1)  # Sleep for 100ms
end = time.time()
print("Time elapsed:", end - start)

print("\nAll tests completed successfully!")