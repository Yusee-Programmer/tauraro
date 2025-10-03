# Lexical Analysis

This chapter describes how Tauraro parses a program into tokens.

## Line Structure

### Logical Lines

The parsing process of a Tauraro program is split into two steps:

1. The file input is broken up into tokens, using the lexical analysis rules.
2. The parser generates the structure of the program from the tokens.

A logical line is constructed from one or more physical lines by following the explicit or implicit line joining rules.

### Physical Lines

A physical line is a sequence of characters terminated by an end-of-line sequence. In source files, any of the standard platform line termination sequences can be used - the Unix form using ASCII LF (linefeed), the Windows form using the ASCII sequence CR LF (return followed by linefeed), or the old Macintosh form using the ASCII CR (return) character.

### Comments

A comment starts with a hash character (`#`) that is not part of a string literal, and ends at the end of the physical line. A comment signifies the end of the logical line unless the implicit line joining rules are invoked. Comments are ignored by the syntax.

```tauraro
# This is a comment
print("Hello")  # This is also a comment
```

### Encoding Declarations

Tauraro source files are interpreted as encoded in UTF-8 by default. To declare a different encoding, a magic comment must be placed into the source file either as first or second line in the file:

```tauraro
# -*- coding: latin-1 -*-
# This file uses latin-1 encoding
```

## Explicit Line Joining

Two or more physical lines may be joined into logical lines using backslash characters (`\`), as follows: when a physical line ends in a backslash that is not part of a string literal or comment, it is joined with the following forming a single logical line, deleting the backslash and the following end-of-line character.

```tauraro
if 1900 < year < 2100 and 1 <= month <= 12 \
   and 1 <= day <= 31 and 0 <= hour < 24 \
   and 0 <= minute < 60 and 0 <= second < 60:
    print("Valid date")
```

## Implicit Line Joining

Expressions in parentheses, square brackets or curly braces can be split over more than one physical line without using backslashes.

```tauraro
months = ['January', 'February', 'March', 'April',
          'May', 'June', 'July', 'August',
          'September', 'October', 'November', 'December']

# Dictionary example
person = {
    "name": "Alice",
    "age": 30,
    "city": "New York"
}

# Function call
result = some_function(
    arg1,
    arg2,
    arg3
)
```

## Blank Lines

A logical line that contains only whitespace, possibly preceded by a comment, is ignored (i.e., no NEWLINE token is generated).

## Indentation

Leading whitespace (spaces and tabs) at the beginning of a logical line is used to compute the indentation level of the line, which in turn is used to determine the grouping of statements.

### Indentation Rules

1. When a line is indented more than the previous line, an INDENT token is generated.
2. When a line is dedented, a DEDENT token is generated.
3. All lines at the same indentation level are grouped together.

```tauraro
def example():
    # This block is indented
    print("First line")
    print("Second line")
    
    if True:
        # This is a nested block
        print("Nested line")
    
    # Back to the outer block
    print("Back to outer block")
```

### Tab Indentation

Tabs are replaced by spaces according to the following rules:
- First, tabs are replaced with 1 to 8 spaces such that the total number of characters up to and including the replacement is a multiple of 8.
- If a file happens to use both tabs and spaces for indentation, and the resulting indentation is inconsistent, a TabError is raised.

## Whitespace Between Tokens

Except at the beginning of a logical line or in string literals, whitespace characters (spaces and tabs) can be used freely between tokens.

## Other Tokens

Besides NEWLINE, INDENT and DEDENT, the following categories of tokens exist: identifiers, keywords, literals, operators, and delimiters.

## Identifiers and Keywords

### Identifiers

Identifiers (also referred to as names) are used to identify variables, functions, classes, modules or other objects. An identifier starts with a letter (A-Z or a-z) or an underscore (_), followed by zero or more letters, underscores and digits (0-9).

Valid identifiers:
```tauraro
name
_name
Name123
MAX_SIZE
__private
```

Invalid identifiers:
```tauraro
123name    # Cannot start with digit
my-name    # Hyphen not allowed
my name    # Space not allowed
```

### Keywords

The following identifiers are used as reserved words, or keywords, and cannot be used as ordinary identifiers:

```tauraro
False      class      finally    is         return
None       continue   for        lambda     try
True       def        from       nonlocal   while
and        del        global     not        with
as         elif       if         or         yield
assert     else       import     pass
break      except     in         raise
```

## Literals

Literals are notations for constant values of built-in types.

### String and Bytes Literals

String literals are written in a variety of ways:

```tauraro
# Single quotes
'allows embedded "double" quotes'

# Double quotes
"allows embedded 'single' quotes"

# Triple quoted (multiline)
'''This is a
multiline string'''

"""This is also a
multiline string"""

# Raw strings (prefix with r)
r"C:\Users\name"

# Bytes literals (prefix with b)
b"Hello"
```

### String Literal Concatenation

Multiple adjacent string or bytes literals (delimited by whitespace) are concatenated:

```tauraro
# These are equivalent
title = "Hello" "World"
title = "HelloWorld"

# Useful for breaking long strings
message = ("This is a very long string that "
           "spans multiple lines for readability")
```

### Formatted String Literals

Formatted string literals (f-strings) are prefixed with 'f' and are expressions surrounded by curly braces replaced with their values:

```tauraro
name = "Alice"
age = 30
message = f"Hello, {name}! You are {age} years old."

# Expressions in f-strings
price = 19.99
tax = 0.08
total = f"Total: ${price * (1 + tax):.2f}"
```

### Numeric Literals

```tauraro
# Integers
decimal = 42
binary = 0b101010    # Binary (42)
octal = 0o52         # Octal (42)
hexadecimal = 0x2A   # Hexadecimal (42)

# Floating point
pi = 3.14159
scientific = 1.5e-3  # Scientific notation

# Complex numbers
complex_num = 3 + 4j
```

### Boolean Literals

```tauraro
is_true = True
is_false = False
```

### The None Literal

```tauraro
nothing = None
```

## Operators

The following tokens are operators:

```
+       -       *       **      /       //      %      @
<<      >>      &       |       ^       ~       :=
<       >       <=      >=      ==      !=
```

## Delimiters

The following tokens serve as delimiters in the grammar:

```
(       )       [       ]       {       }
,       :       .       ;       @       =       ->
+=      -=      *=      /=      //=     %=
@=      &=      |=      ^=
>>=     <<=     **=
```

## Special Tokens

### Ellipsis

The ellipsis literal `...` has a specific meaning in Tauraro:

```tauraro
# In function signatures
def example(args...):
    pass

# As a placeholder
def todo():
    ...  # To be implemented
```