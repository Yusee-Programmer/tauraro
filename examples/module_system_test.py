# Tauraro Module System Test

# Test basic import functionality
import sys
print("sys.version:", sys.version)
print("sys.path:", sys.path)

# Test built-in module import (should work only when explicitly imported)
import os
print("os module imported successfully")

# Test from import
from math import pi, sin
print("pi =", pi)
print("sin(pi/2) =", sin(1.5708))

# Test package functionality (if available)
try:
    import datetime
    now = datetime.datetime.now()
    print("Current time:", now)
except ImportError as e:
    print("datetime module not available:", e)

# Test module aliasing
import threading as thread_mod
print("threading module aliased as thread_mod")

# Test Python interop (if available)
try:
    # This would load from tauraro_packages/pysites if available
    import requests
    print("Python requests module available")
except ImportError:
    print("Python requests module not available (install with: install_package('requests'))")

print("Module system test completed successfully!")