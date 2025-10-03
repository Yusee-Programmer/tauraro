# Built-in Exceptions

In Python, all exceptions must be instances of a class that derives from `BaseException`. In addition to the built-in exception classes, a large number of other exceptions are defined in the standard library.

## Exception Hierarchy

The class hierarchy for built-in exceptions is:

```
BaseException
 +-- SystemExit
 +-- KeyboardInterrupt
 +-- GeneratorExit
 +-- Exception
      +-- StopIteration
      +-- StopAsyncIteration
      +-- ArithmeticError
      |    +-- FloatingPointError
      |    +-- OverflowError
      |    +-- ZeroDivisionError
      +-- AssertionError
      +-- AttributeError
      +-- BufferError
      +-- EOFError
      +-- ImportError
      |    +-- ModuleNotFoundError
      +-- LookupError
      |    +-- IndexError
      |    +-- KeyError
      +-- MemoryError
      +-- NameError
      |    +-- UnboundLocalError
      +-- OSError
      |    +-- BlockingIOError
      |    +-- ChildProcessError
      |    +-- ConnectionError
      |    |    +-- BrokenPipeError
      |    |    +-- ConnectionAbortedError
      |    |    +-- ConnectionRefusedError
      |    |    +-- ConnectionResetError
      |    +-- FileExistsError
      |    +-- FileNotFoundError
      |    +-- InterruptedError
      |    +-- IsADirectoryError
      |    +-- NotADirectoryError
      |    +-- PermissionError
      |    +-- ProcessLookupError
      |    +-- TimeoutError
      +-- ReferenceError
      +-- RuntimeError
      |    +-- NotImplementedError
      |    +-- RecursionError
      +-- SyntaxError
      |    +-- IndentationError
      |         +-- TabError
      +-- SystemError
      +-- TypeError
      +-- ValueError
      |    +-- UnicodeError
      |         +-- UnicodeDecodeError
      |         +-- UnicodeEncodeError
      |         +-- UnicodeTranslateError
      +-- Warning
           +-- DeprecationWarning
           +-- PendingDeprecationWarning
           +-- RuntimeWarning
           +-- SyntaxWarning
           +-- UserWarning
           +-- FutureWarning
           +-- ImportWarning
           +-- UnicodeWarning
           +-- BytesWarning
           +-- ResourceWarning
```

## Base Classes

### `BaseException`

The base class for all built-in exceptions. It is not meant to be directly inherited by user-defined classes (for that, use `Exception`).

#### `args`

The tuple of arguments given to the exception constructor.

```tauraro
>>> try:
...     raise ValueError("Invalid value", 42)
... except ValueError as e:
...     print(e.args)
...
('Invalid value', 42)
```

#### `__traceback__`

This attribute holds the traceback object associated with the exception.

#### `with_traceback(tb)`

This method sets tb as the new traceback for the exception and returns the exception object.

```tauraro
>>> try:
...     1/0
... except ZeroDivisionError as e:
...     raise ValueError("Cannot divide by zero").with_traceback(e.__traceback__)
```

### `Exception`

All built-in, non-system-exiting exceptions are derived from this class. All user-defined exceptions should also be derived from this class.

### `ArithmeticError`

The base class for those built-in exceptions that are raised for various arithmetic errors.

#### `FloatingPointError`

Raised when a floating point operation fails.

#### `OverflowError`

Raised when the result of an arithmetic operation is too large to be represented.

```tauraro
>>> import math
>>> math.exp(1000)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
OverflowError: math range error
```

#### `ZeroDivisionError`

Raised when the second argument of a division or modulo operation is zero.

```tauraro
>>> 1 / 0
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ZeroDivisionError: division by zero
```

### `LookupError`

The base class for the exceptions that are raised when a key or index used on a mapping or sequence is invalid.

#### `IndexError`

Raised when a sequence subscript is out of range.

```tauraro
>>> l = [1, 2, 3]
>>> l[5]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: list index out of range
```

#### `KeyError`

Raised when a mapping (dictionary) key is not found in the set of existing keys.

```tauraro
>>> d = {"a": 1, "b": 2}
>>> d["c"]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: 'c'
```

## Concrete Exceptions

### `AssertionError`

Raised when an `assert` statement fails.

```tauraro
>>> assert 1 == 2, "One is not equal to two"
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AssertionError: One is not equal to two
```

### `AttributeError`

Raised when an attribute reference or assignment fails.

```tauraro
>>> class MyClass:
...     pass
...
>>> obj = MyClass()
>>> obj.nonexistent
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'MyClass' object has no attribute 'nonexistent'
```

### `EOFError`

Raised when the `input()` function hits an end-of-file condition (EOF) without reading any data.

```tauraro
>>> # This would occur when input() reaches EOF
>>> # input()
>>> # EOFError: EOF when reading a line
```

### `ImportError`

Raised when an `import` statement cannot find the module definition or when a `from ... import` fails to find a name that is to be imported.

#### `ModuleNotFoundError`

A subclass of `ImportError` which is raised by `import` when a module could not be located.

```tauraro
>>> import nonexistent_module
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ModuleNotFoundError: No module named 'nonexistent_module'
```

### `MemoryError`

Raised when an operation runs out of memory but the situation may still be rescued.

```tauraro
>>> # This would occur when memory is exhausted
>>> # MemoryError
```

### `NameError`

Raised when a local or global name is not found.

#### `UnboundLocalError`

A subclass of `NameError` raised when a local variable is referenced before assignment.

```tauraro
>>> print(undefined_variable)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'undefined_variable' is not defined
```

### `OSError`

This exception is raised when a system function returns a system-related error, including I/O failures such as "file not found" or "disk full".

#### `BlockingIOError`

Raised when an operation would block on an object (e.g. socket) set for non-blocking operation.

#### `ChildProcessError`

Raised when an operation on a child process failed.

#### `ConnectionError`

A base class for connection-related issues.

##### `BrokenPipeError`

A subclass of `ConnectionError`, raised when trying to write on a pipe while the other end has been closed, or trying to write on a socket which has been shut down for writing.

##### `ConnectionAbortedError`

A subclass of `ConnectionError`, raised when a connection attempt is aborted by the peer.

##### `ConnectionRefusedError`

A subclass of `ConnectionError`, raised when a connection attempt is refused by the peer.

##### `ConnectionResetError`

A subclass of `ConnectionError`, raised when a connection is reset by the peer.

#### `FileExistsError`

Raised when trying to create a file or directory which already exists.

```tauraro
>>> import os
>>> os.mkdir("/existing_directory")
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
FileExistsError: [Errno 17] File exists: '/existing_directory'
```

#### `FileNotFoundError`

Raised when a file or directory is requested but doesn't exist.

```tauraro
>>> open("nonexistent_file.txt")
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
FileNotFoundError: [Errno 2] No such file or directory: 'nonexistent_file.txt'
```

#### `InterruptedError`

Raised when a system call is interrupted by an incoming signal.

#### `IsADirectoryError`

Raised when a file operation (such as `os.remove()`) is requested on a directory.

#### `NotADirectoryError`

Raised when a directory operation (such as `os.listdir()`) is requested on a non-directory.

#### `PermissionError`

Raised when trying to run an operation without the adequate access rights.

```tauraro
>>> open("/root/secret_file", "r")
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
PermissionError: [Errno 13] Permission denied: '/root/secret_file'
```

#### `ProcessLookupError`

Raised when a given process doesn't exist.

#### `TimeoutError`

Raised when a system function timed out at the system level.

### `ReferenceError`

Raised when a weak reference proxy is used to access an attribute of the referent after it has been garbage collected.

### `RuntimeError`

Raised when an error is detected that doesn't fall in any of the other categories.

#### `NotImplementedError`

Raised when an abstract method that needs to be implemented in a subclass is not actually implemented.

```tauraro
class BaseClass:
    def method(self):
        raise NotImplementedError("Subclass must implement this method")

class DerivedClass(BaseClass):
    pass

>>> obj = DerivedClass()
>>> obj.method()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NotImplementedError: Subclass must implement this method
```

#### `RecursionError`

Raised when the maximum recursion depth is exceeded.

```tauraro
>>> def infinite_recursion():
...     return infinite_recursion()
...
>>> infinite_recursion()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<stdin>", line 2, in infinite_recursion
  File "<stdin>", line 2, in infinite_recursion
  # ... many more lines ...
RecursionError: maximum recursion depth exceeded
```

### `StopIteration`

Raised by built-in function `next()` and an iterator's `__next__()` method to signal that there are no further items produced by the iterator.

```tauraro
>>> it = iter([1, 2, 3])
>>> next(it)
1
>>> next(it)
2
>>> next(it)
3
>>> next(it)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```

### `StopAsyncIteration`

Must be raised by an asynchronous iterator object's `__anext__()` method to stop the iteration.

### `SyntaxError`

Raised when the parser encounters a syntax error.

#### `IndentationError`

Base class for exceptions raised when a syntax error related to incorrect indentation is detected.

##### `TabError`

Raised when indentation contains an inconsistent use of tabs and spaces.

```tauraro
>>> # This would raise IndentationError
>>> def bad_function():
... print("Hello")  # Incorrect indentation
...
  File "<stdin>", line 2
    print("Hello")
    ^
IndentationError: expected an indented block
```

### `SystemError`

Raised when the interpreter finds an internal error, but the situation does not look so serious to cause it to abandon all hope.

### `SystemExit`

This exception is raised by the `sys.exit()` function. It inherits from `BaseException` instead of `Exception` so that it is not accidentally caught by code that catches `Exception`.

```tauraro
>>> import sys
>>> sys.exit()
# Exits the program
```

### `TypeError`

Raised when an operation or function is applied to an object of inappropriate type.

```tauraro
>>> len(42)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: object of type 'int' has no len()
```

### `ValueError`

Raised when an operation or function receives an argument that has the right type but an inappropriate value.

```tauraro
>>> int("hello")
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: invalid literal for int() with base 10: 'hello'
```

#### `UnicodeError`

Raised when a Unicode-related encoding or decoding error occurs.

##### `UnicodeDecodeError`

Raised when a Unicode-related error occurs during decoding.

##### `UnicodeEncodeError`

Raised when a Unicode-related error occurs during encoding.

##### `UnicodeTranslateError`

Raised when a Unicode-related error occurs during translating.

### `Warning`

This is the base class of all warning category classes. It is a subclass of `Exception`.

#### `DeprecationWarning`

Base category for warnings about deprecated features when those warnings are intended for other Python developers.

#### `PendingDeprecationWarning`

Base category for warnings about features that will be deprecated in the future.

#### `RuntimeWarning`

Base category for warnings about dubious runtime behavior.

#### `SyntaxWarning`

Base category for warnings about dubious syntax.

#### `UserWarning`

The default category for warn().

#### `FutureWarning`

Base category for warnings about constructs that will change semantically in the future.

#### `ImportWarning`

Base category for warnings about probable mistakes in module imports.

#### `UnicodeWarning`

Base category for warnings related to Unicode.

#### `BytesWarning`

Base category for warnings related to bytes and bytearray.

#### `ResourceWarning`

Base category for warnings about resource usage.

## Exception Context

Exception objects have two additional attributes that provide context information about the exception:

### `__context__`

This attribute is the exception that was being handled when the exception was raised.

```tauraro
>>> try:
...     try:
...         1/0
...     except ZeroDivisionError:
...         raise ValueError("Invalid operation")
... except ValueError as e:
...     print(f"Context: {type(e.__context__).__name__}")
...
Context: ZeroDivisionError
```

### `__cause__`

This attribute is the direct cause of the exception, if any.

```tauraro
>>> try:
...     try:
...         1/0
...     except ZeroDivisionError as e:
...         raise ValueError("Invalid operation") from e
... except ValueError as e:
...     print(f"Cause: {type(e.__cause__).__name__}")
...
Cause: ZeroDivisionError
```

## Exception Chaining

Exception chaining allows you to raise a new exception while preserving the original exception.

### `raise ... from ...`

The `from` clause explicitly sets the `__cause__` attribute.

```tauraro
>>> try:
...     1/0
... except ZeroDivisionError as e:
...     raise ValueError("Cannot divide by zero") from e
...
Traceback (most recent call last):
  File "<stdin>", line 2, in <module>
ZeroDivisionError: division by zero

The above exception was the direct cause of the following exception:

Traceback (most recent call last):
  File "<stdin>", line 4, in <module>
ValueError: Cannot divide by zero
```

### Implicit Exception Chaining

When an exception is raised during the handling of another exception, the `__context__` attribute is automatically set.

```tauraro
>>> try:
...     try:
...         1/0
...     except ZeroDivisionError:
...         undefined_variable  # This will raise NameError
... except NameError:
...     print("NameError occurred")
...
NameError occurred
```

## Defining Clean-up Actions

The `try` statement has another optional clause which is intended to define clean-up actions that must be executed under all circumstances.

```tauraro
>>> try:
...     raise KeyboardInterrupt
... finally:
...     print('Goodbye, world!')
...
Goodbye, world!
Traceback (most recent call last):
  File "<stdin>", line 2, in <module>
KeyboardInterrupt
```

## Predefined Clean-up Actions

Some objects define standard clean-up actions to be undertaken when the object is no longer needed, regardless of whether or not the operation using the object succeeded or failed.

```tauraro
# File objects are automatically closed
with open("myfile.txt") as f:
    for line in f:
        print(line, end="")
# File is automatically closed even if an exception occurs
```

## Raising Exceptions

The `raise` statement allows the programmer to force a specified exception to occur.

```tauraro
>>> raise NameError('HiThere')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: HiThere
```

## Exception Handling

The `try` statement works as follows:

1. First, the try clause is executed.
2. If no exception occurs, the except clause is skipped and execution of the try statement is finished.
3. If an exception occurs during execution of the try clause, the rest of the clause is skipped. Then if its type matches the exception named after the except keyword, the except clause is executed, and then execution continues after the try statement.
4. If an exception occurs which does not match the exception named in the except clause, it is passed on to outer try statements; if no handler is found, it is an unhandled exception and execution stops with a message.

```tauraro
>>> try:
...     x = int(input("Please enter a number: "))
...     break
... except ValueError:
...     print("Oops!  That was no valid number.  Try again...")
```

## User-defined Exceptions

Programs may name their own exceptions by creating a new exception class. Exceptions should typically be derived from the `Exception` class, either directly or indirectly.

```tauraro
class MyError(Exception):
    def __init__(self, value):
        self.value = value
    
    def __str__(self):
        return repr(self.value)

>>> try:
...     raise MyError(2*2)
... except MyError as e:
...     print('My exception occurred, value:', e.value)
...
My exception occurred, value: 4
```

## Best Practices for Exception Handling

### Be Specific with Exceptions

Catch specific exceptions rather than using a bare `except:` clause.

```tauraro
# Good
try:
    f = open('myfile.txt')
    s = f.readline()
    i = int(s.strip())
except OSError as err:
    print("OS error: {0}".format(err))
except ValueError:
    print("Could not convert data to an integer.")
except Exception:
    print("Unexpected error:", sys.exc_info()[0])
    raise

# Avoid
try:
    # some code
    pass
except:
    # This catches all exceptions, including SystemExit and KeyboardInterrupt
    pass
```

### Use Exceptions for Exceptional Cases

Exceptions should be used for exceptional circumstances, not for normal control flow.

```tauraro
# Good
try:
    value = int(user_input)
except ValueError:
    print("Invalid input")

# Avoid using exceptions for normal control flow
try:
    item = my_dict[key]
except KeyError:
    # This is not the best approach for checking key existence
    pass
# Better approach
if key in my_dict:
    item = my_dict[key]
```

### Clean Up Resources Properly

Use context managers or finally blocks to ensure resources are properly cleaned up.

```tauraro
# Good - using context manager
with open('myfile.txt') as f:
    for line in f:
        print(line)

# Good - using finally
f = None
try:
    f = open('myfile.txt')
    # process file
finally:
    if f:
        f.close()
```

### Don't Ignore Exceptions

Don't catch exceptions and ignore them silently unless you have a very good reason.

```tauraro
# Good
try:
    result = risky_operation()
except SpecificException as e:
    logger.error(f"Operation failed: {e}")
    # Handle the error appropriately

# Avoid
try:
    result = risky_operation()
except:
    pass  # Silent failure - very bad!
```

### Preserve Stack Traces

When re-raising exceptions, preserve the original stack trace.

```tauraro
# Good
try:
    something()
except Exception:
    logger.error("Something failed")
    raise  # Re-raises the original exception with its stack trace

# Also good
try:
    something()
except Exception as e:
    logger.error("Something failed")
    raise RuntimeError("Operation failed") from e  # Chains exceptions
```