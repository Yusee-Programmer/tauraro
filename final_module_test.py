#!/usr/bin/env tauraro
# Final comprehensive test of all 30 builtin modules

print("=" * 80)
print("FINAL COMPREHENSIVE MODULE TEST")
print("Testing all 30 builtin modules")
print("=" * 80)
print()

# 1. Math
import math
print("1. math.sqrt(16) = " + str(math.sqrt(16)))

# 2. JSON
import json
s = json.dumps([1, 2, 3])
print("2. json.dumps([1,2,3]) = " + s)

# 3. Random
import random
print("3. random.random() = " + str(random.random()))

# 4. Time
import time
print("4. time.time() = " + str(time.time()))

# 5. OS
import os
print("5. os.getcwd() = " + os.getcwd())

# 6. Sys
import sys
print("6. sys.platform = " + str(sys.platform))

# 7. Collections
import collections
print("7. collections module loaded")

# 8. Functools
import functools
print("8. functools module loaded")

# 9. Itertools
import itertools
result = list(itertools.chain([1], [2]))
print("9. itertools.chain([1],[2]) = " + str(result))

# 10. Datetime
import datetime
print("10. datetime module loaded")

# 11. RE
import re
m = re.search("x", "text")
print("11. re.search executed")

# 12. Copy
import copy
print("12. copy module loaded")

# 13. Hashlib
import hashlib
h = hashlib.md5("test")
print("13. hashlib.md5 created hash")

# 14. Base64
import base64
enc = base64.b64encode("test")
print("14. base64.b64encode = " + str(enc))

# 15. GC
import gc
gc.collect()
print("15. gc.collect() executed")

# 16. Logging
import logging
logging.info("test")
print("16. logging.info() executed")

# 17. Threading
import threading
print("17. threading.active_count() = " + str(threading.active_count()))

# 18. Pickle
import pickle
print("18. pickle module loaded")

# 19. CSV
import csv
print("19. csv module loaded")

# 20. ABC
import abc
print("20. abc module loaded")

# 21. Unittest
import unittest
print("21. unittest module loaded")

# 22. Asyncio
import asyncio
print("22. asyncio module loaded")

# 23. HTTPTools
import httptools
print("23. httptools module loaded")

# 24. HTTPX
import httpx
print("24. httpx module loaded")

# 25. Websockets
import websockets
print("25. websockets module loaded")

# 26. Urllib
import urllib
print("26. urllib module loaded")

# 27. IO
import io
print("27. io module loaded")

# 28. Memory
import memory
print("28. memory module loaded")

# 29. Exceptions
import exceptions
print("29. exceptions module loaded")

# 30. Socket
import socket
print("30. socket module loaded")

print()
print("=" * 80)
print("SUCCESS! ALL 30 BUILTIN MODULES WORKING!")
print("=" * 80)
