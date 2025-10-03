# Text Processing Services

The modules described in this chapter provide a wide range of string manipulation operations and other text processing services.

## `string` — Common string operations

The `string` module contains a number of useful constants and classes, as well as some deprecated legacy functions that are also available as methods on strings.

### String constants

The constants defined in this module are:

```tauraro
import string

# ASCII letters
>>> string.ascii_letters
'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'

# ASCII lowercase letters
>>> string.ascii_lowercase
'abcdefghijklmnopqrstuvwxyz'

# ASCII uppercase letters
>>> string.ascii_uppercase
'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

# Digits
>>> string.digits
'0123456789'

# Hexadecimal digits
>>> string.hexdigits
'0123456789abcdefABCDEF'

# Octal digits
>>> string.octdigits
'01234567'

# Punctuation characters
>>> string.punctuation
'!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'

# Whitespace characters
>>> string.whitespace
' \t\n\r\x0b\x0c'

# Printable characters
>>> string.printable
'0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \t\n\r\x0b\x0c'
```

### String formatting

The `string` module also contains a `Formatter` class that can be used to create custom string formatting behaviors.

```tauraro
import string

# Using Formatter class
formatter = string.Formatter()
result = formatter.format("Hello, {name}!", name="World")
print(result)  # Hello, World!

# Custom formatter
class MyFormatter(string.Formatter):
    def format_field(self, value, format_spec):
        if format_spec == 'u':
            return str(value).upper()
        elif format_spec == 'l':
            return str(value).lower()
        return super().format_field(value, format_spec)

formatter = MyFormatter()
result = formatter.format("{0:u}", "hello")
print(result)  # HELLO
```

### Template strings

The `Template` class provides a simpler string substitution mechanism.

```tauraro
from string import Template

# Basic template
template = Template("Hello, $name!")
result = template.substitute(name="World")
print(result)  # Hello, World!

# Safe substitution (doesn't raise KeyError)
template = Template("Hello, $name!")
result = template.safe_substitute()
print(result)  # Hello, $name!

# Template with braces
template = Template("Hello, ${name}!")
result = template.substitute(name="World")
print(result)  # Hello, World!

# Template with multiple placeholders
template = Template("$greeting, $name! You have $count messages.")
result = template.substitute(greeting="Hello", name="Alice", count=5)
print(result)  # Hello, Alice! You have 5 messages.
```

### Helper functions

```tauraro
import string

# capwords - Capitalize words
>>> string.capwords("hello world")
'Hello World'

# capwords with custom separator
>>> string.capwords("hello-world", "-")
'Hello-World'
```

## `re` — Regular expression operations

This module provides regular expression matching operations similar to those found in Perl.

### Regular Expression Syntax

Regular expressions use the same syntax as Perl, with a few Python-specific extensions.

```tauraro
import re

# Simple pattern matching
>>> re.match(r'hello', 'hello world')
<_sre.SRE_Match object; span=(0, 5), match='hello'>

>>> re.match(r'world', 'hello world')  # No match at beginning
None

# Search anywhere in string
>>> re.search(r'world', 'hello world')
<_sre.SRE_Match object; span=(6, 11), match='world'>

# Find all matches
>>> re.findall(r'\d+', 'There are 123 apples and 456 oranges')
['123', '456']

# Find all matches with positions
>>> re.finditer(r'\d+', 'There are 123 apples and 456 oranges')
<callable_iterator object at 0x...>
>>> list(re.finditer(r'\d+', 'There are 123 apples and 456 oranges'))
[<_sre.SRE_Match object; span=(10, 13), match='123'>, <_sre.SRE_Match object; span=(25, 28), match='456'>]
```

### Compilation flags

```tauraro
import re

# Case insensitive
>>> re.search(r'hello', 'HELLO', re.IGNORECASE)
<_sre.SRE_Match object; span=(0, 5), match='HELLO'>

# Multiline mode
>>> text = "first line\nsecond line\nthird line"
>>> re.findall(r'^\w+', text, re.MULTILINE)
['first', 'second', 'third']

# Dot matches newline
>>> re.search(r'first.*third', 'first line\nsecond line\nthird line', re.DOTALL)
<_sre.SRE_Match object; span=(0, 34), match='first line\nsecond line\nthird line'>
```

### Raw string notation

Regular expressions use the backslash character ('\') to indicate special forms or to allow special characters to be used without invoking their special meaning. This collides with Python's usage of the same character for the same purpose in string literals.

```tauraro
import re

# Without raw strings (need double backslashes)
>>> re.match("\\\\d+", "123")
<_sre.SRE_Match object; span=(0, 3), match='123'>

# With raw strings (recommended)
>>> re.match(r"\d+", "123")
<_sre.SRE_Match object; span=(0, 3), match='123'>
```

### Common patterns

```tauraro
import re

# Email validation (simplified)
email_pattern = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
>>> re.match(email_pattern, "user@example.com")
<_sre.SRE_Match object; span=(0, 16), match='user@example.com'>

# Phone number validation
phone_pattern = r'^\+?1?-?\.?\s?\(?(\d{3})\)?[\s.-]?(\d{3})[\s.-]?(\d{4})$'
>>> re.match(phone_pattern, "(123) 456-7890")
<_sre.SRE_Match object; span=(0, 14), match='(123) 456-7890'>

# URL validation
url_pattern = r'^https?://(?:[-\w.])+(?:\:[0-9]+)?(?:/(?:[\w/_.])*(?:\?(?:[\w&=%.])*)?(?:\#(?:[\w.])*)?)?$'
>>> re.match(url_pattern, "https://www.example.com/path?query=value")
<_sre.SRE_Match object; span=(0, 43), match='https://www.example.com/path?query=value'>
```

### Substitution

```tauraro
import re

# Simple substitution
>>> re.sub(r'def', 'function', 'def my_function():')
'function my_function():'

# Substitution with count
>>> re.sub(r'a', 'o', 'banana', count=2)
'bonona'

# Substitution with function
def uppercase_match(match):
    return match.group(0).upper()

>>> re.sub(r'\b\w+\b', uppercase_match, 'hello world')
'HELLO WORLD'

# Subn returns count of substitutions
>>> re.subn(r'a', 'o', 'banana')
('bonono', 3)
```

### Splitting

```tauraro
import re

# Simple split
>>> re.split(r'\s+', 'one two   three')
['one', 'two', 'three']

# Split with maxsplit
>>> re.split(r'\s+', 'one two three four', maxsplit=2)
['one', 'two', 'three four']

# Split with capturing groups
>>> re.split(r'(\s+)', 'one two three')
['one', ' ', 'two', '   ', 'three']
```

### Match objects

```tauraro
import re

match = re.search(r'(\d+)-(\d+)-(\d+)', 'Date: 2023-12-25')
if match:
    # Group methods
    >>> match.group(0)  # Entire match
    '2023-12-25'
    >>> match.group(1)  # First group
    '2023'
    >>> match.group(2, 3)  # Multiple groups
    ('12', '25')
    >>> match.groups()  # All groups
    ('2023', '12', '25')
    
    # Named groups
    match = re.search(r'(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})', 'Date: 2023-12-25')
    >>> match.group('year')
    '2023'
    >>> match.groupdict()
    {'year': '2023', 'month': '12', 'day': '25'}
    
    # Position methods
    >>> match.start()
    6
    >>> match.end()
    16
    >>> match.span()
    (6, 16)
```

### Compilation

For better performance when using the same pattern multiple times, compile the regular expression.

```tauraro
import re

# Compile pattern
pattern = re.compile(r'\b\w+@\w+\.\w+\b')

# Use compiled pattern
>>> pattern.search('Contact: user@example.com')
<_sre.SRE_Match object; span=(9, 25), match='user@example.com'>
>>> pattern.findall('Emails: user1@example.com, user2@test.org')
['user1@example.com', 'user2@test.org']

# Compile with flags
pattern = re.compile(r'\b\w+\b', re.IGNORECASE)
>>> pattern.findall('Hello WORLD')
['Hello', 'WORLD']
```

## `difflib` — Helpers for computing deltas

This module provides classes and functions for comparing sequences.

### SequenceMatcher

```tauraro
import difflib

# Basic usage
s = difflib.SequenceMatcher(None, 'abcd', 'bcde')
>>> s.ratio()
0.75

# Find longest matching block
s = difflib.SequenceMatcher(None, 'abxcd', 'abcd')
>>> s.find_longest_match(0, 5, 0, 4)
Match(a=0, b=0, size=2)

# Get matching blocks
>>> list(s.get_matching_blocks())
[Match(a=0, b=0, size=2), Match(a=3, b=2, size=2), Match(a=5, b=4, size=0)]

# Get opcodes
>>> list(s.get_opcodes())
[('equal', 0, 2, 0, 2), ('delete', 2, 3, 2, 2), ('equal', 3, 5, 2, 4)]
```

### Differ

```tauraro
import difflib

# Compare text lines
text1 = ['one\n', 'two\n', 'three\n']
text2 = ['one\n', 'three\n', 'four\n']

d = difflib.Differ()
diff = d.compare(text1, text2)
>>> list(diff)
['  one\n', '- two\n', '  three\n', '+ four\n']
```

### HtmlDiff

```tauraro
import difflib

# Generate HTML diff
d = difflib.HtmlDiff()
html_diff = d.make_file(['line1\n', 'line2\n'], ['line1\n', 'line3\n'])
# Returns HTML string with side-by-side comparison
```

### get_close_matches

```tauraro
import difflib

# Find close matches
>>> difflib.get_close_matches('appel', ['ape', 'apple', 'peach', 'puppy'])
['apple', 'ape']

# With cutoff
>>> difflib.get_close_matches('appel', ['ape', 'apple', 'peach', 'puppy'], n=3, cutoff=0.6)
['apple', 'ape', 'puppy']
```

## `textwrap` — Text wrapping and filling

The textwrap module provides two convenience functions, `wrap()` and `fill()`, as well as `TextWrapper`, the class that does all the work.

```tauraro
import textwrap

# Sample text
text = "The textwrap module provides two convenience functions, wrap() and fill(), as well as TextWrapper, the class that does all the work."

# Wrap text
>>> textwrap.wrap(text, width=40)
['The textwrap module provides two', 'convenience functions, wrap() and', 'fill(), as well as TextWrapper, the', 'class that does all the work.']

# Fill text
>>> print(textwrap.fill(text, width=40))
The textwrap module provides two
convenience functions, wrap() and
fill(), as well as TextWrapper, the
class that does all the work.

# Indent text
>>> textwrap.indent("line1\nline2\nline3", "  ")
'  line1\n  line2\n  line3'

# Shorten text
>>> textwrap.shorten("Hello world!", width=12)
'Hello world!'
>>> textwrap.shorten("Hello world!", width=10)
'Hello [...]'
```

## `unicodedata` — Unicode Database

This module provides access to the Unicode Character Database which defines character properties for all Unicode characters.

```tauraro
import unicodedata

# Get character name
>>> unicodedata.name('A')
'LATIN CAPITAL LETTER A'

# Get character from name
>>> unicodedata.lookup('LATIN CAPITAL LETTER A')
'A'

# Get category
>>> unicodedata.category('A')
'Lu'  # Letter, uppercase

# Get bidirectional class
>>> unicodedata.bidirectional('A')
'L'  # Left-to-right

# Get combining class
>>> unicodedata.combining('A')
0

# Get east asian width
>>> unicodedata.east_asian_width('A')
'Na'  # Narrow

# Normalize text
>>> unicodedata.normalize('NFD', 'café')
'café'  # Decomposed form
>>> unicodedata.normalize('NFC', 'café')
'café'  # Composed form
```

## `stringprep` — Internet String Preparation

This module provides APIs for various string preparation algorithms used in the context of internationalized domain names (IDNs) and other Internet protocols.

```tauraro
import stringprep

# Check if character is in table
>>> stringprep.in_table_a1(' ')  # Space character
True
>>> stringprep.in_table_b1('A')  # ASCII uppercase
True

# Map characters
>>> stringprep.map_table_b2('A')
'a'  # Maps to lowercase
```

## Best Practices

### Working with Strings

1. **Use raw strings for regex patterns** to avoid escaping backslashes
2. **Compile regex patterns** that are used multiple times for better performance
3. **Use string methods** like `startswith()`, `endswith()`, `strip()` when possible instead of regex for simple operations
4. **Use f-strings** for string formatting in Python 3.6+

```tauraro
# Good practices
import re

# Use raw strings for regex
pattern = re.compile(r'\b\w+@\w+\.\w+\b')

# Use string methods for simple operations
if filename.endswith('.txt'):
    process_file(filename)

# Use f-strings for formatting
name = "Alice"
age = 30
message = f"Hello, {name}! You are {age} years old."
```

### Regular Expressions

1. **Compile patterns** that are used multiple times
2. **Use raw strings** to avoid escaping issues
3. **Use named groups** for better readability
4. **Be careful with greedy vs non-greedy matching**

```tauraro
import re

# Good regex practices
# Use named groups
pattern = re.compile(r'(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})')

# Non-greedy matching
html = "<p>Hello</p><p>World</p>"
pattern = re.compile(r'<p>.*?</p>')  # Non-greedy
matches = pattern.findall(html)

# Compile for performance
email_pattern = re.compile(r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$')
```

### Text Processing

1. **Use appropriate string methods** for simple operations
2. **Consider using difflib** for comparing text
3. **Use textwrap** for formatting text output
4. **Handle Unicode properly** when working with international text

```tauraro
import textwrap
import difflib

# Text wrapping for console output
long_text = "This is a very long line that needs to be wrapped for better readability."
wrapped = textwrap.fill(long_text, width=50)

# Text comparison
text1 = ["line1", "line2", "line3"]
text2 = ["line1", "line2 modified", "line3"]
diff = difflib.unified_diff(text1, text2)

# Unicode handling
import unicodedata
normalized = unicodedata.normalize('NFC', text_with_accents)
```