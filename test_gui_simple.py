# Simple test to check if gui module loads
print("Attempting to import gui...")

try:
    from gui import show_info
    print("GUI module imported successfully!")
    show_info("Hello from Tauraro!", "Test")
except Exception as e:
    print(f"Error: {e}")
