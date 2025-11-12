"""
Launch All WebViewTK Examples Simultaneously
This demonstrates that multiple Tauraro programs can run at the same time!
"""

import subprocess
import sys
import time
import os

# Get the project root directory (parent of examples)
project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Path to tauraro executable
tauraro_exe = os.path.join(project_root, "target", "debug", "tauraro.exe")

# List of examples to launch
examples = [
    ("Dashboard Pro", "./examples/webviewtk_dashboard.py", "Analytics dashboard with animations"),
    ("TechStore", "./examples/webviewtk_ecommerce.py", "E-commerce store with shopping cart"),
    ("SocialHub", "./examples/webviewtk_social_media.py", "Social media feed with interactive posts"),
    ("Portfolio", "./examples/webviewtk_portfolio.py", "Modern portfolio/landing page"),
]

print("\n" + "=" * 70)
print("  Launch All WebViewTK Examples Simultaneously")
print("=" * 70)
print()
print("This will launch all 4 comprehensive examples at once!")
print("Each example runs as an independent Tauraro program.")
print()
print("Examples to launch:")
for i, (name, path, description) in enumerate(examples, 1):
    print(f"  {i}. {name:15s} - {description}")
print()
print("=" * 70)
print()

# Ask for confirmation
response = input("Launch all examples? (y/n): ").strip().lower()

if response != 'y' and response != 'yes':
    print("\nCancelled. No examples launched.")
    sys.exit(0)

print()
print("=" * 70)
print("  Launching Examples...")
print("=" * 70)
print()

# Launch all examples
processes = []
for name, path, description in examples:
    print(f"Launching {name}...")

    try:
        # Launch in background (don't wait)
        process = subprocess.Popen(
            [tauraro_exe, "run", path],
            cwd=project_root,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            creationflags=subprocess.CREATE_NEW_CONSOLE if sys.platform == "win32" else 0
        )
        processes.append((name, process))
        print(f"  ✓ {name} started (PID: {process.pid})")
        time.sleep(0.5)  # Small delay between launches

    except Exception as e:
        print(f"  ✗ Failed to launch {name}: {e}")

print()
print("=" * 70)
print(f"  SUCCESS! Launched {len(processes)} examples!")
print("=" * 70)
print()
print("Instructions:")
print("  • All 4 examples are now running as independent programs")
print("  • Each window can be moved/resized/closed independently")
print("  • Close any window - others will keep running")
print("  • Each program has its own console window")
print()
print("Example windows you should see:")
print("  1. Dashboard Pro - Analytics with charts and cards")
print("  2. TechStore - Product gallery with shopping cart")
print("  3. SocialHub - Social media feed with posts")
print("  4. Portfolio - Landing page with smooth scrolling")
print()
print("This demonstrates Tauraro's multi-process capability!")
print("Before the fix, you could only run one program at a time.")
print()
print("=" * 70)
print()
print("Press Enter to close all examples and exit...")

try:
    input()
except KeyboardInterrupt:
    print()

print()
print("Closing all examples...")
for name, process in processes:
    try:
        process.terminate()
        print(f"  ✓ Closed {name}")
    except:
        print(f"  ✗ Failed to close {name}")

print()
print("All examples closed. Goodbye!")
print()
