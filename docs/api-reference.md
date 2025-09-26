# TauraroLang API Reference

This document provides a comprehensive reference for all built-in functions, VM operations, and standard library features available in TauraroLang.

## Table of Contents

1. [Built-in Functions](#built-in-functions)
2. [Data Type Operations](#data-type-operations)
3. [String Operations](#string-operations)
4. [Array Operations](#array-operations)
5. [Object Operations](#object-operations)
6. [Mathematical Functions](#mathematical-functions)
7. [I/O Operations](#io-operations)
8. [Type System](#type-system)
9. [Memory Management](#memory-management)
10. [VM Operations](#vm-operations)
11. [Error Handling](#error-handling)

## Built-in Functions

### Core Functions

#### `print(value)`
Outputs a value to the console.

**Parameters:**
- `value` (any): The value to print

**Returns:** `null`

**Examples:**
```tauraro
print("Hello, World!")          // Output: Hello, World!
print(42)                       // Output: 42
print([1, 2, 3])               // Output: [1, 2, 3]
print({name: "Alice", age: 25}) // Output: {name: "Alice", age: 25}
```

#### `len(collection)`
Returns the length of a collection (string, array, or object).

**Parameters:**
- `collection` (string|array|object): The collection to measure

**Returns:** `integer` - The number of elements/characters

**Examples:**
```tauraro
len("Hello")        // Returns: 5
len([1, 2, 3, 4])   // Returns: 4
len({a: 1, b: 2})   // Returns: 2
len("")             // Returns: 0
len([])             // Returns: 0
```

#### `type(value)`
Returns the type of a value as a string.

**Parameters:**
- `value` (any): The value to check

**Returns:** `string` - The type name

**Possible return values:**
- `"integer"` - For integer numbers
- `"float"` - For floating-point numbers
- `"string"` - For text strings
- `"boolean"` - For true/false values
- `"array"` - For arrays/lists
- `"object"` - For objects/dictionaries
- `"function"` - For functions
- `"null"` - For null values

**Examples:**
```tauraro
type(42)           // Returns: "integer"
type(3.14)         // Returns: "float"
type("hello")      // Returns: "string"
type(true)         // Returns: "boolean"
type([1, 2, 3])    // Returns: "array"
type({a: 1})       // Returns: "object"
type(null)         // Returns: "null"
```

## Data Type Operations

### Type Conversion Functions

#### `str(value)`
Converts a value to its string representation.

**Parameters:**
- `value` (any): The value to convert

**Returns:** `string` - String representation of the value

**Examples:**
```tauraro
str(42)           // Returns: "42"
str(3.14)         // Returns: "3.14"
str(true)         // Returns: "true"
str([1, 2, 3])    // Returns: "[1, 2, 3]"
str({a: 1})       // Returns: "{a: 1}"
str(null)         // Returns: "null"
```

#### `int(value)`
Converts a value to an integer.

**Parameters:**
- `value` (string|float|boolean): The value to convert

**Returns:** `integer` - Integer representation of the value

**Conversion rules:**
- String: Parses numeric strings, returns 0 for non-numeric
- Float: Truncates decimal part
- Boolean: `true` → 1, `false` → 0
- Other types: Returns 0

**Examples:**
```tauraro
int("42")         // Returns: 42
int("3.14")       // Returns: 3
int(3.14)         // Returns: 3
int(true)         // Returns: 1
int(false)        // Returns: 0
int("hello")      // Returns: 0
```

#### `float(value)`
Converts a value to a floating-point number.

**Parameters:**
- `value` (string|integer|boolean): The value to convert

**Returns:** `float` - Float representation of the value

**Examples:**
```tauraro
float("3.14")     // Returns: 3.14
float(42)         // Returns: 42.0
float(true)       // Returns: 1.0
float(false)      // Returns: 0.0
float("hello")    // Returns: 0.0
```

#### `bool(value)`
Converts a value to a boolean.

**Parameters:**
- `value` (any): The value to convert

**Returns:** `boolean` - Boolean representation of the value

**Truthiness rules:**
- Numbers: 0 and 0.0 are `false`, all others are `true`
- Strings: Empty string `""` is `false`, all others are `true`
- Arrays: Empty array `[]` is `false`, all others are `true`
- Objects: Empty object `{}` is `false`, all others are `true`
- `null`: Always `false`

**Examples:**
```tauraro
bool(1)           // Returns: true
bool(0)           // Returns: false
bool("hello")     // Returns: true
bool("")          // Returns: false
bool([1, 2])      // Returns: true
bool([])          // Returns: false
bool({a: 1})      // Returns: true
bool({})          // Returns: false
bool(null)        // Returns: false
```

### Range Function

#### `range(start, stop, step?)`
Generates a sequence of numbers.

**Parameters:**
- `start` (integer): Starting value (inclusive)
- `stop` (integer): Ending value (exclusive)
- `step` (integer, optional): Step size (default: 1)

**Returns:** `array` - Array of integers in the specified range

**Examples:**
```tauraro
range(0, 5)       // Returns: [0, 1, 2, 3, 4]
range(1, 6)       // Returns: [1, 2, 3, 4, 5]
range(0, 10, 2)   // Returns: [0, 2, 4, 6, 8]
range(5, 0, -1)   // Returns: [5, 4, 3, 2, 1]
range(0, 0)       // Returns: []
```

## String Operations

### String Methods

Strings in TauraroLang support various operations through built-in functions and operators.

#### String Concatenation
```tauraro
let greeting = "Hello" + " " + "World"  // "Hello World"
let name = "Alice"
let message = "Hi, " + name + "!"       // "Hi, Alice!"
```

#### String Indexing
```tauraro
let text = "Hello"
let first_char = text[0]    // "H"
let last_char = text[4]     // "o"
```

#### String Slicing (Conceptual)
```tauraro
// Note: Actual slicing syntax may vary in implementation
let text = "Hello World"
let substring = text.slice(0, 5)  // "Hello"
```

### String Utility Functions

#### `split(string, delimiter)`
Splits a string into an array of substrings.

**Parameters:**
- `string` (string): The string to split
- `delimiter` (string): The delimiter to split on

**Returns:** `array` - Array of string parts

**Examples:**
```tauraro
split("a,b,c", ",")           // Returns: ["a", "b", "c"]
split("hello world", " ")     // Returns: ["hello", "world"]
split("one-two-three", "-")   // Returns: ["one", "two", "three"]
```

#### `join(array, separator)`
Joins an array of strings into a single string.

**Parameters:**
- `array` (array): Array of strings to join
- `separator` (string): String to use as separator

**Returns:** `string` - Joined string

**Examples:**
```tauraro
join(["a", "b", "c"], ",")        // Returns: "a,b,c"
join(["hello", "world"], " ")     // Returns: "hello world"
join(["one", "two", "three"], "-") // Returns: "one-two-three"
```

## Array Operations

### Array Creation and Access

```tauraro
// Creating arrays
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "hello", true, 3.14]
let empty = []

// Accessing elements
let first = numbers[0]      // 1
let last = numbers[4]       // 5

// Getting length
let count = len(numbers)    // 5
```

### Array Methods

#### Array Concatenation
```tauraro
let arr1 = [1, 2, 3]
let arr2 = [4, 5, 6]
let combined = arr1 + arr2  // [1, 2, 3, 4, 5, 6]
```

#### Array Appending
```tauraro
let numbers = [1, 2, 3]
numbers = numbers + [4]     // [1, 2, 3, 4]
numbers = numbers + [5, 6]  // [1, 2, 3, 4, 5, 6]
```

### Array Utility Functions

#### `push(array, element)`
Adds an element to the end of an array.

**Parameters:**
- `array` (array): The array to modify
- `element` (any): The element to add

**Returns:** `array` - New array with element added

**Examples:**
```tauraro
let arr = [1, 2, 3]
arr = push(arr, 4)          // [1, 2, 3, 4]
arr = push(arr, "hello")    // [1, 2, 3, 4, "hello"]
```

#### `pop(array)`
Removes and returns the last element from an array.

**Parameters:**
- `array` (array): The array to modify

**Returns:** `any` - The removed element

**Examples:**
```tauraro
let arr = [1, 2, 3, 4]
let last = pop(arr)         // Returns: 4, arr becomes [1, 2, 3]
```

#### `slice(array, start, end?)`
Returns a portion of an array.

**Parameters:**
- `array` (array): The source array
- `start` (integer): Starting index (inclusive)
- `end` (integer, optional): Ending index (exclusive)

**Returns:** `array` - New array containing the slice

**Examples:**
```tauraro
let arr = [1, 2, 3, 4, 5]
slice(arr, 1, 4)    // Returns: [2, 3, 4]
slice(arr, 2)       // Returns: [3, 4, 5]
slice(arr, 0, 3)    // Returns: [1, 2, 3]
```

## Object Operations

### Object Creation and Access

```tauraro
// Creating objects
let person = {
    name: "Alice",
    age: 25,
    city: "New York"
}

// Accessing properties
let name = person.name      // "Alice"
let age = person["age"]     // 25

// Adding properties
person.email = "alice@example.com"
person["phone"] = "123-456-7890"
```

### Object Methods

#### `keys(object)`
Returns an array of all property names in an object.

**Parameters:**
- `object` (object): The object to get keys from

**Returns:** `array` - Array of property names

**Examples:**
```tauraro
let obj = {a: 1, b: 2, c: 3}
keys(obj)           // Returns: ["a", "b", "c"]

let person = {name: "Alice", age: 25}
keys(person)        // Returns: ["name", "age"]
```

#### `values(object)`
Returns an array of all property values in an object.

**Parameters:**
- `object` (object): The object to get values from

**Returns:** `array` - Array of property values

**Examples:**
```tauraro
let obj = {a: 1, b: 2, c: 3}
values(obj)         // Returns: [1, 2, 3]

let person = {name: "Alice", age: 25}
values(person)      // Returns: ["Alice", 25]
```

#### `has_key(object, key)`
Checks if an object has a specific property.

**Parameters:**
- `object` (object): The object to check
- `key` (string): The property name to look for

**Returns:** `boolean` - True if property exists

**Examples:**
```tauraro
let person = {name: "Alice", age: 25}
has_key(person, "name")     // Returns: true
has_key(person, "email")    // Returns: false
```

## Mathematical Functions

### Basic Math Operations

```tauraro
// Arithmetic operators
let sum = 10 + 5        // 15
let diff = 10 - 3       // 7
let product = 4 * 6     // 24
let quotient = 15 / 3   // 5
let remainder = 17 % 5  // 2
let power = 2 ** 3      // 8 (if supported)
```

### Advanced Math Functions

#### `abs(number)`
Returns the absolute value of a number.

**Parameters:**
- `number` (integer|float): The number

**Returns:** `integer|float` - Absolute value

**Examples:**
```tauraro
abs(-5)         // Returns: 5
abs(3.14)       // Returns: 3.14
abs(-2.5)       // Returns: 2.5
```

#### `min(a, b, ...)`
Returns the smallest of the given numbers.

**Parameters:**
- `a, b, ...` (integer|float): Numbers to compare

**Returns:** `integer|float` - The minimum value

**Examples:**
```tauraro
min(5, 3, 8, 1)     // Returns: 1
min(2.5, 1.8)       // Returns: 1.8
```

#### `max(a, b, ...)`
Returns the largest of the given numbers.

**Parameters:**
- `a, b, ...` (integer|float): Numbers to compare

**Returns:** `integer|float` - The maximum value

**Examples:**
```tauraro
max(5, 3, 8, 1)     // Returns: 8
max(2.5, 1.8)       // Returns: 2.5
```

#### `round(number, digits?)`
Rounds a number to a specified number of decimal places.

**Parameters:**
- `number` (float): The number to round
- `digits` (integer, optional): Number of decimal places (default: 0)

**Returns:** `float` - Rounded number

**Examples:**
```tauraro
round(3.14159)      // Returns: 3.0
round(3.14159, 2)   // Returns: 3.14
round(3.14159, 4)   // Returns: 3.1416
```

## I/O Operations

### Console I/O

#### `print(value)`
Already documented above in Built-in Functions.

#### `input(prompt?)`
Reads a line of input from the user.

**Parameters:**
- `prompt` (string, optional): Text to display as prompt

**Returns:** `string` - User input as string

**Examples:**
```tauraro
let name = input("Enter your name: ")
let age = int(input("Enter your age: "))
```

### File I/O (Conceptual)

#### `read_file(filename)`
Reads the contents of a file.

**Parameters:**
- `filename` (string): Path to the file

**Returns:** `string` - File contents

**Examples:**
```tauraro
let content = read_file("data.txt")
print(content)
```

#### `write_file(filename, content)`
Writes content to a file.

**Parameters:**
- `filename` (string): Path to the file
- `content` (string): Content to write

**Returns:** `boolean` - Success status

**Examples:**
```tauraro
let success = write_file("output.txt", "Hello, World!")
if success {
    print("File written successfully")
}
```

## Type System

### Type Checking

#### `is_type(value, type_name)`
Checks if a value is of a specific type.

**Parameters:**
- `value` (any): The value to check
- `type_name` (string): The type name to check against

**Returns:** `boolean` - True if value is of the specified type

**Examples:**
```tauraro
is_type(42, "integer")      // Returns: true
is_type("hello", "string")  // Returns: true
is_type([1, 2], "array")    // Returns: true
is_type({}, "object")       // Returns: true
is_type(null, "null")       // Returns: true
```

### Type Conversion Validation

#### `can_convert(value, target_type)`
Checks if a value can be converted to a target type.

**Parameters:**
- `value` (any): The value to check
- `target_type` (string): The target type

**Returns:** `boolean` - True if conversion is possible

**Examples:**
```tauraro
can_convert("42", "integer")    // Returns: true
can_convert("hello", "integer") // Returns: false
can_convert(3.14, "integer")    // Returns: true
```

## Memory Management

### Garbage Collection

TauraroLang uses automatic memory management. Objects are automatically freed when they're no longer referenced.

#### `gc()`
Manually triggers garbage collection (if supported).

**Returns:** `null`

**Examples:**
```tauraro
// Create many objects
for i in range(0, 1000) {
    let obj = {data: range(0, 100)}
}

// Manually trigger cleanup
gc()
```

### Memory Information

#### `memory_usage()`
Returns information about current memory usage.

**Returns:** `object` - Memory usage statistics

**Examples:**
```tauraro
let stats = memory_usage()
print("Used memory: " + str(stats.used))
print("Total memory: " + str(stats.total))
```

## VM Operations

### Execution Control

#### `exit(code?)`
Terminates the program with an optional exit code.

**Parameters:**
- `code` (integer, optional): Exit code (default: 0)

**Returns:** Does not return

**Examples:**
```tauraro
if error_occurred {
    print("An error occurred!")
    exit(1)
}

print("Program completed successfully")
exit(0)  // or just exit()
```

#### `sleep(milliseconds)`
Pauses execution for a specified time.

**Parameters:**
- `milliseconds` (integer): Time to sleep in milliseconds

**Returns:** `null`

**Examples:**
```tauraro
print("Starting...")
sleep(1000)  // Wait 1 second
print("Done!")
```

### Runtime Information

#### `version()`
Returns the TauraroLang version information.

**Returns:** `object` - Version information

**Examples:**
```tauraro
let ver = version()
print("TauraroLang version: " + ver.version)
print("Build date: " + ver.build_date)
```

#### `platform()`
Returns information about the current platform.

**Returns:** `object` - Platform information

**Examples:**
```tauraro
let plat = platform()
print("OS: " + plat.os)
print("Architecture: " + plat.arch)
```

## Error Handling

### Exception Types

TauraroLang uses a result-based error handling approach rather than exceptions.

#### Error Result Pattern
```tauraro
fn operation_that_might_fail() {
    if some_condition {
        return {
            success: true,
            value: result_value
        }
    } else {
        return {
            success: false,
            error: "Error message",
            code: error_code
        }
    }
}

// Usage
let result = operation_that_might_fail()
if result.success {
    print("Success: " + str(result.value))
} else {
    print("Error: " + result.error)
}
```

### Error Utilities

#### `is_error(result)`
Checks if a result object represents an error.

**Parameters:**
- `result` (object): Result object to check

**Returns:** `boolean` - True if result represents an error

**Examples:**
```tauraro
let result = some_operation()
if is_error(result) {
    print("Operation failed: " + result.error)
} else {
    print("Operation succeeded: " + str(result.value))
}
```

#### `unwrap(result)`
Extracts the value from a successful result or terminates on error.

**Parameters:**
- `result` (object): Result object

**Returns:** `any` - The value if successful

**Examples:**
```tauraro
let result = safe_operation()
let value = unwrap(result)  // Terminates if result is an error
print("Value: " + str(value))
```

## Standard Library Extensions

### Collections

#### `map(array, function)`
Applies a function to each element of an array.

**Parameters:**
- `array` (array): Source array
- `function` (function): Function to apply

**Returns:** `array` - New array with transformed elements

**Examples:**
```tauraro
fn double(x) { return x * 2 }
let numbers = [1, 2, 3, 4]
let doubled = map(numbers, double)  // [2, 4, 6, 8]
```

#### `filter(array, predicate)`
Filters an array based on a predicate function.

**Parameters:**
- `array` (array): Source array
- `predicate` (function): Function that returns boolean

**Returns:** `array` - New array with filtered elements

**Examples:**
```tauraro
fn is_even(x) { return x % 2 == 0 }
let numbers = [1, 2, 3, 4, 5, 6]
let evens = filter(numbers, is_even)  // [2, 4, 6]
```

#### `reduce(array, function, initial?)`
Reduces an array to a single value using a function.

**Parameters:**
- `array` (array): Source array
- `function` (function): Reducer function
- `initial` (any, optional): Initial value

**Returns:** `any` - Reduced value

**Examples:**
```tauraro
fn add(a, b) { return a + b }
let numbers = [1, 2, 3, 4, 5]
let sum = reduce(numbers, add, 0)  // 15
```

### Utility Functions

#### `clone(value)`
Creates a deep copy of a value.

**Parameters:**
- `value` (any): Value to clone

**Returns:** `any` - Cloned value

**Examples:**
```tauraro
let original = {a: 1, b: [2, 3, 4]}
let copy = clone(original)
copy.a = 10
// original.a is still 1
```

#### `equals(a, b)`
Performs deep equality comparison.

**Parameters:**
- `a` (any): First value
- `b` (any): Second value

**Returns:** `boolean` - True if values are deeply equal

**Examples:**
```tauraro
equals([1, 2, 3], [1, 2, 3])           // true
equals({a: 1}, {a: 1})                 // true
equals({a: 1, b: 2}, {b: 2, a: 1})     // true
```

---

This API reference covers all the core functionality available in TauraroLang. For more advanced features and examples, see the other documentation files in this directory.