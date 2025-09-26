# Getting Started with TauraroLang

Welcome to TauraroLang! This guide will walk you through everything you need to know to start programming in Tauraro, from installation to writing your first programs.

## Table of Contents

1. [Installation](#installation)
2. [Your First Program](#your-first-program)
3. [Interactive REPL](#interactive-repl)
4. [Basic Concepts](#basic-concepts)
5. [Step-by-Step Tutorials](#step-by-step-tutorials)
6. [Working with Files](#working-with-files)
7. [Compilation Backends](#compilation-backends)
8. [Next Steps](#next-steps)

## Installation

### Prerequisites

Before installing TauraroLang, ensure you have:

- **Rust** (1.70 or later) - [Install from rustup.rs](https://rustup.rs/)
- **Git** - For cloning the repository
- **C Compiler** (optional) - For C transpilation backend
  - Windows: Visual Studio Build Tools or MinGW
  - Linux: GCC
  - macOS: Xcode Command Line Tools

### Building from Source

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/tauraro.git
   cd tauraro
   ```

2. **Build the project:**
   ```bash
   cargo build --release
   ```

3. **Install globally (optional):**
   ```bash
   cargo install --path .
   ```

4. **Verify installation:**
   ```bash
   ./target/release/tauraro --version
   ```

## Your First Program

Let's start with the traditional "Hello, World!" program.

### Creating Your First File

Create a file named `hello.tr`:

```tauraro
// hello.tr - Your first Tauraro program
print("Hello, World!")
```

### Running Your Program

```bash
# Using the interpreter (fastest for development)
tauraro run hello.tr

# Or compile to C and run
tauraro compile hello.tr --backend c
./hello
```

**Expected Output:**
```
Hello, World!
```

Congratulations! You've just written and executed your first Tauraro program.

## Interactive REPL

The REPL (Read-Eval-Print Loop) is perfect for experimenting and learning:

```bash
tauraro repl
```

Try these commands in the REPL:

```tauraro
>>> print("Welcome to Tauraro!")
Welcome to Tauraro!

>>> let x = 42
>>> print(x)
42

>>> let name = "Developer"
>>> print("Hello, " + name + "!")
Hello, Developer!

>>> exit()
```

## Basic Concepts

### Variables and Data Types

```tauraro
// Variables
let name = "Alice"          // String
let age = 25               // Integer
let height = 5.8           // Float
let is_student = true      // Boolean

// Arrays
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "hello", true, 3.14]

// Objects
let person = {
    name: "Bob",
    age: 30,
    city: "New York"
}
```

### Basic Operations

```tauraro
// Arithmetic
let sum = 10 + 5           // 15
let difference = 10 - 3    // 7
let product = 4 * 6        // 24
let quotient = 15 / 3      // 5
let remainder = 17 % 5     // 2

// String operations
let greeting = "Hello" + " " + "World"  // "Hello World"
let length = len("Tauraro")             // 7

// Comparisons
let is_equal = (5 == 5)        // true
let is_greater = (10 > 5)      // true
let is_not_equal = (3 != 7)    // true
```

## Step-by-Step Tutorials

### Tutorial 1: Calculator Program

Let's build a simple calculator that performs basic arithmetic operations.

**Step 1:** Create `calculator.tr`

```tauraro
// calculator.tr - A simple calculator

// Function to add two numbers
fn add(a, b) {
    return a + b
}

// Function to subtract two numbers
fn subtract(a, b) {
    return a - b
}

// Function to multiply two numbers
fn multiply(a, b) {
    return a * b
}

// Function to divide two numbers
fn divide(a, b) {
    if b == 0 {
        print("Error: Division by zero!")
        return 0
    }
    return a / b
}

// Main calculator logic
fn main() {
    print("=== Tauraro Calculator ===")
    
    let num1 = 10
    let num2 = 5
    
    print("Number 1: " + str(num1))
    print("Number 2: " + str(num2))
    print("")
    
    print("Addition: " + str(num1) + " + " + str(num2) + " = " + str(add(num1, num2)))
    print("Subtraction: " + str(num1) + " - " + str(num2) + " = " + str(subtract(num1, num2)))
    print("Multiplication: " + str(num1) + " * " + str(num2) + " = " + str(multiply(num1, num2)))
    print("Division: " + str(num1) + " / " + str(num2) + " = " + str(divide(num1, num2)))
}

// Call main function
main()
```

**Step 2:** Run the calculator

```bash
tauraro run calculator.tr
```

**Expected Output:**
```
=== Tauraro Calculator ===
Number 1: 10
Number 2: 5

Addition: 10 + 5 = 15
Subtraction: 10 - 5 = 5
Multiplication: 10 * 5 = 50
Division: 10 / 5 = 2
```

### Tutorial 2: Working with Arrays and Loops

Let's create a program that processes arrays of data.

**Step 1:** Create `array_demo.tr`

```tauraro
// array_demo.tr - Working with arrays and loops

fn main() {
    print("=== Array Processing Demo ===")
    
    // Create an array of numbers
    let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    
    print("Original array: " + str(numbers))
    print("Array length: " + str(len(numbers)))
    
    // Calculate sum
    let sum = 0
    let i = 0
    while i < len(numbers) {
        sum = sum + numbers[i]
        i = i + 1
    }
    
    print("Sum of all elements: " + str(sum))
    
    // Find maximum
    let max = numbers[0]
    i = 1
    while i < len(numbers) {
        if numbers[i] > max {
            max = numbers[i]
        }
        i = i + 1
    }
    
    print("Maximum element: " + str(max))
    
    // Create array of even numbers
    let evens = []
    i = 0
    while i < len(numbers) {
        if numbers[i] % 2 == 0 {
            evens = evens + [numbers[i]]  // Append to array
        }
        i = i + 1
    }
    
    print("Even numbers: " + str(evens))
}

main()
```

**Step 2:** Run the program

```bash
tauraro run array_demo.tr
```

### Tutorial 3: Object-Oriented Programming

Let's create a simple class-based program.

**Step 1:** Create `person_demo.tr`

```tauraro
// person_demo.tr - Object-oriented programming demo

class Person {
    // Constructor
    fn init(name, age) {
        self.name = name
        self.age = age
    }
    
    // Method to introduce the person
    fn introduce() {
        print("Hi, I'm " + self.name + " and I'm " + str(self.age) + " years old.")
    }
    
    // Method to have a birthday
    fn birthday() {
        self.age = self.age + 1
        print(self.name + " is now " + str(self.age) + " years old!")
    }
    
    // Method to check if person is adult
    fn is_adult() {
        return self.age >= 18
    }
}

fn main() {
    print("=== Person Demo ===")
    
    // Create person objects
    let alice = Person("Alice", 25)
    let bob = Person("Bob", 17)
    
    // Introduce both people
    alice.introduce()
    bob.introduce()
    
    print("")
    
    // Check if they're adults
    if alice.is_adult() {
        print(alice.name + " is an adult.")
    } else {
        print(alice.name + " is not an adult.")
    }
    
    if bob.is_adult() {
        print(bob.name + " is an adult.")
    } else {
        print(bob.name + " is not an adult.")
    }
    
    print("")
    
    // Bob has a birthday
    print("It's Bob's birthday!")
    bob.birthday()
    
    // Check again
    if bob.is_adult() {
        print("Now Bob is an adult!")
    }
}

main()
```

### Tutorial 4: Error Handling

Learn how to handle errors gracefully in your programs.

**Step 1:** Create `error_handling.tr`

```tauraro
// error_handling.tr - Error handling demonstration

fn safe_divide(a, b) {
    if b == 0 {
        return {
            success: false,
            error: "Division by zero",
            result: 0
        }
    }
    
    return {
        success: true,
        error: "",
        result: a / b
    }
}

fn safe_array_access(arr, index) {
    if index < 0 || index >= len(arr) {
        return {
            success: false,
            error: "Index out of bounds",
            result: null
        }
    }
    
    return {
        success: true,
        error: "",
        result: arr[index]
    }
}

fn main() {
    print("=== Error Handling Demo ===")
    
    // Test safe division
    let result1 = safe_divide(10, 2)
    if result1.success {
        print("10 / 2 = " + str(result1.result))
    } else {
        print("Error: " + result1.error)
    }
    
    let result2 = safe_divide(10, 0)
    if result2.success {
        print("10 / 0 = " + str(result2.result))
    } else {
        print("Error: " + result2.error)
    }
    
    print("")
    
    // Test safe array access
    let numbers = [1, 2, 3, 4, 5]
    
    let access1 = safe_array_access(numbers, 2)
    if access1.success {
        print("numbers[2] = " + str(access1.result))
    } else {
        print("Error: " + access1.error)
    }
    
    let access2 = safe_array_access(numbers, 10)
    if access2.success {
        print("numbers[10] = " + str(access2.result))
    } else {
        print("Error: " + access2.error)
    }
}

main()
```

## Working with Files

### Creating a Project Structure

For larger projects, organize your code into multiple files:

```
my_project/
├── main.tr          # Entry point
├── utils.tr         # Utility functions
├── models.tr        # Data models/classes
└── config.tr        # Configuration
```

### Example Multi-File Project

**main.tr:**
```tauraro
// Import other modules (conceptual - actual import syntax may vary)
// include "utils.tr"
// include "models.tr"

fn main() {
    print("=== Multi-File Project Demo ===")
    
    // Use functions from other files
    let result = calculate_area(5, 10)
    print("Area: " + str(result))
    
    let user = User("John", "john@example.com")
    user.display_info()
}

main()
```

## Compilation Backends

TauraroLang supports multiple compilation backends for different use cases:

### 1. Interpreter (Default)
Best for development and testing:
```bash
tauraro run program.tr
```

### 2. C Transpilation
Generates C code for maximum performance:
```bash
# Compile to C
tauraro compile program.tr --backend c

# This generates program.c and compiles to executable
./program
```

### 3. WebAssembly
For web applications:
```bash
# Compile to WebAssembly
tauraro compile program.tr --backend wasm

# Generates program.wasm
```

### 4. LLVM IR
For advanced optimization:
```bash
# Generate LLVM IR
tauraro compile program.tr --backend llvm

# Generates program.ll
```

## Next Steps

Now that you've learned the basics, here are some suggestions for continuing your TauraroLang journey:

### 1. Explore Advanced Features
- Read the [Language Reference](language-reference.md)
- Learn about [FFI and Interoperability](ffi-guide.md)
- Study [Advanced Features](advanced-features.md)

### 2. Build Real Projects
- Create a command-line tool
- Build a web server
- Write a game or simulation
- Develop a data processing pipeline

### 3. Learn Best Practices
- Read the [Best Practices Guide](best-practices.md)
- Study the [API Documentation](api-reference.md)
- Check out [Example Projects](../examples/)

### 4. Get Help
- Join the community discussions
- Report bugs and request features
- Contribute to the project

### 5. Practice Exercises

Try these exercises to reinforce your learning:

1. **Number Guessing Game**: Create a game where the computer picks a random number and the user guesses it.

2. **Text Processor**: Write a program that counts words, lines, and characters in a text file.

3. **Simple Web Server**: Use FFI to create a basic HTTP server.

4. **Data Analyzer**: Process CSV data and generate statistics.

5. **Mini Database**: Implement a simple key-value store with file persistence.

## Common Patterns

### Pattern 1: Configuration Object
```tauraro
let config = {
    debug: true,
    max_connections: 100,
    timeout: 30
}

fn setup_server(cfg) {
    if cfg.debug {
        print("Debug mode enabled")
    }
    // ... setup logic
}
```

### Pattern 2: Result Type
```tauraro
fn operation_that_might_fail() {
    // ... some logic
    if success {
        return { ok: true, value: result }
    } else {
        return { ok: false, error: "Something went wrong" }
    }
}
```

### Pattern 3: Builder Pattern
```tauraro
class QueryBuilder {
    fn init() {
        self.query = ""
        self.conditions = []
    }
    
    fn select(fields) {
        self.query = "SELECT " + fields
        return self
    }
    
    fn from(table) {
        self.query = self.query + " FROM " + table
        return self
    }
    
    fn where(condition) {
        self.conditions = self.conditions + [condition]
        return self
    }
    
    fn build() {
        let result = self.query
        if len(self.conditions) > 0 {
            result = result + " WHERE " + join(self.conditions, " AND ")
        }
        return result
    }
}
```

## Troubleshooting

### Common Issues

1. **Syntax Errors**: Check for missing semicolons, unmatched brackets, or typos
2. **Type Errors**: Ensure you're using the correct data types
3. **Runtime Errors**: Use error handling patterns to catch and handle errors gracefully

### Getting Help

- Check the [Troubleshooting Guide](troubleshooting.md)
- Look at [Common Error Messages](error-reference.md)
- Search existing issues on GitHub
- Ask questions in community forums

---

**Congratulations!** You now have a solid foundation in TauraroLang. Keep practicing, building projects, and exploring the language's features. Happy coding!