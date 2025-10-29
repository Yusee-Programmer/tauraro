import gui

# Print all available functions in the gui module
print("Available functions in gui module:")
functions = [attr for attr in dir(gui) if callable(getattr(gui, attr))]
for func in functions:
    print(f"  {func}")

print("\nAll attributes in gui module:")
for attr in dir(gui):
    print(f"  {attr}")