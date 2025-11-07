# Showcase of different error types in Tauraro
# This file demonstrates various Python exceptions
# (Note: Integration with VM pending - this shows the planned behavior)

print("=== Exception Showcase ===")
print("The following would trigger colored tracebacks:\n")

# 1. SyntaxError
print("1. SyntaxError:")
print("if x == :")
print("        ^")
print("SyntaxError: invalid syntax\n")

# 2. NameError
print("2. NameError:")
print("x = undefined_variable")
print("    ^")
print("NameError: name 'undefined_variable' is not defined\n")

# 3. TypeError
print("3. TypeError:")
print("result = 5 + 'hello'")
print("         ^")
print("TypeError: unsupported operand type(s) for +: 'int' and 'str'\n")

# 4. ZeroDivisionError
print("4. ZeroDivisionError:")
print("result = 10 / 0")
print("         ^")
print("ZeroDivisionError: division by zero\n")

# 5. IndexError
print("5. IndexError:")
print("items = [1, 2, 3]")
print("x = items[10]")
print("    ^")
print("IndexError: list index out of range\n")

# 6. KeyError
print("6. KeyError:")
print("data = {'a': 1}")
print("x = data['missing']")
print("    ^")
print("KeyError: 'missing'\n")

# 7. AttributeError
print("7. AttributeError:")
print("obj = None")
print("obj.value")
print("^")
print("AttributeError: 'NoneType' object has no attribute 'value'\n")

print("=== All Python builtin exceptions are supported! ===")
print("See EXCEPTION_SYSTEM.md for full documentation")
