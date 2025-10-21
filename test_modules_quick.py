#!/usr/bin/env tauraro
# Quick Module Test

print("=" * 80)
print("QUICK BUILTIN MODULE TEST")
print("=" * 80)
print()

# Math Module
import math
print("1. math - sqrt(16) = " + str(math.sqrt(16)))

# JSON Module
import json
print("2. json - module loaded")

# Random Module
import random
print("3. random - random() = " + str(random.random()))

# Time Module
import time
print("4. time - time() = " + str(time.time()))

# OS Module
import os
print("5. os - getcwd() = " + os.getcwd())

# Sys Module
import sys
print("6. sys - platform = " + str(sys.platform))

# Collections Module
import collections
print("7. collections - module loaded")

# Functools Module
import functools
print("8. functools - module loaded")

# Itertools Module
import itertools
print("9. itertools - module loaded")

# Datetime Module
import datetime
print("10. datetime - now() = " + str(datetime.datetime.now()))

# RE Module
import re
print("11. re - module loaded")

# Copy Module
import copy
print("12. copy - module loaded")

# Hashlib Module
import hashlib
print("13. hashlib - module loaded")

# Base64 Module
import base64
print("14. base64 - module loaded")

# GC Module
import gc
print("15. gc - module loaded")

# Logging Module
import logging
print("16. logging - module loaded")

# Threading Module
import threading
print("17. threading - module loaded")

# Pickle Module
import pickle
print("18. pickle - module loaded")

# CSV Module
import csv
print("19. csv - module loaded")

# ABC Module
import abc
print("20. abc - module loaded")

# Unittest Module
import unittest
print("21. unittest - module loaded")

# Asyncio Module
import asyncio
print("22. asyncio - module loaded")

# HTTPTools Module
import httptools
print("23. httptools - module loaded")

# HTTPX Module
import httpx
print("24. httpx - module loaded")

# Websockets Module
import websockets
print("25. websockets - module loaded")

# Urllib Module
import urllib
print("26. urllib - module loaded")

# IO Module
import io
print("27. io - module loaded")

# Memory Module
import memory
print("28. memory - module loaded")

# Exceptions Module
import exceptions
print("29. exceptions - module loaded")

# Socket Module
import socket
print("30. socket - module loaded")

print()
print("=" * 80)
print("ALL 30 MODULES LOADED SUCCESSFULLY!")
print("=" * 80)
