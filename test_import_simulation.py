import os
import sys

# Add the tauraro_packages directory to Python path
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "tauraro_packages"))

print("Testing DUITK import simulation...")
print(f"Python path: {sys.path[:3]}")  # Show first 3 paths

try:
    print("Attempting to import duitk package...")
    import duitk
    print("✓ SUCCESS: DUITK package imported without encoding errors!")
    print(f"  Package version: {getattr(duitk, '__version__', 'Unknown')}")
    print(f"  Package name: {getattr(duitk, '__name__', 'Unknown')}")
    
    # Check if key classes are available
    if hasattr(duitk, 'Application'):
        print("  ✓ Application class available")
    if hasattr(duitk, 'Window'):
        print("  ✓ Window class available")
    if hasattr(duitk, 'Button'):
        print("  ✓ Button class available")
        
except UnicodeDecodeError as e:
    print(f"✗ FAILED: Unicode decode error (encoding issue): {e}")
except ImportError as e:
    print(f"✗ FAILED: Import error: {e}")
except Exception as e:
    print(f"✗ FAILED: Other error: {e}")
    import traceback
    traceback.print_exc()