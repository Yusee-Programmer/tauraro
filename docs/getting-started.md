# Getting Started with TauraroLang

Welcome to TauraroLang! This guide will walk you through everything you need to know to start programming in Tauraro, from installation to writing your first programs. TauraroLang uses 100% Python syntax with .tr file extensions.

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

```python
# hello.tr - Your first Tauraro program
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

```python
>>> print("Welcome to Tauraro!")
Welcome to Tauraro!

>>> x = 42
>>> print(x)
42

>>> name = "Developer"
>>> print("Hello, " + name + "!")
Hello, Developer!

>>> exit()
```

## Basic Concepts

### Variables and Data Types

```python
# Variables
name = "Alice"          # String
age = 25               # Integer
height = 5.8           # Float
is_student = True      # Boolean

# Lists
numbers = [1, 2, 3, 4, 5]
mixed = [1, "hello", True, 3.14]

# Dictionaries
person = {
    "name": "Bob",
    "age": 30,
    "city": "New York"
}
```

### Basic Operations

```python
# Arithmetic
sum = 10 + 5           # 15
difference = 10 - 3    # 7
product = 4 * 6        # 24
quotient = 15 / 3      # 5
remainder = 17 % 5     # 2

# String operations
greeting = "Hello" + " " + "World"  # "Hello World"
length = len("Tauraro")             # 7

# Comparisons
is_equal = (5 == 5)        # True
is_greater = (10 > 5)      # True
is_not_equal = (3 != 7)    # True
```

## Step-by-Step Tutorials

### Tutorial 1: Calculator Program

Let's build a simple calculator that performs basic arithmetic operations.

**Step 1:** Create `calculator.tr`

```python
# calculator.tr - A simple calculator

# Function to add two numbers
def add(a, b):
    return a + b

# Function to subtract two numbers
def subtract(a, b):
    return a - b

# Function to multiply two numbers
def multiply(a, b):
    return a * b

# Function to divide two numbers
def divide(a, b):
    if b == 0:
        print("Error: Division by zero!")
        return 0
    return a / b

# Main calculator logic
def main():
    print("=== Tauraro Calculator ===")
    
    num1 = 10
    num2 = 5
    
    print("Number 1: " + str(num1))
    print("Number 2: " + str(num2))
    print("")
    
    print("Addition: " + str(num1) + " + " + str(num2) + " = " + str(add(num1, num2)))
    print("Subtraction: " + str(num1) + " - " + str(num2) + " = " + str(subtract(num1, num2)))
    print("Multiplication: " + str(num1) + " * " + str(num2) + " = " + str(multiply(num1, num2)))
    print("Division: " + str(num1) + " / " + str(num2) + " = " + str(divide(num1, num2)))

if __name__ == "__main__":
    main()
```

### Running the Calculator

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
Division: 10 / 5 = 2.0
```

### Tutorial 2: Working with Classes

Let's create a simple class-based program to manage a library.

**Step 1:** Create `library.tr`

```python
# library.tr - A simple library management system

class Book:
    def __init__(self, title, author, isbn):
        self.title = title
        self.author = author
        self.isbn = isbn
        self.is_borrowed = False
    
    def borrow(self):
        if not self.is_borrowed:
            self.is_borrowed = True
            return f"Book '{self.title}' borrowed successfully"
        else:
            return f"Book '{self.title}' is already borrowed"
    
    def return_book(self):
        if self.is_borrowed:
            self.is_borrowed = False
            return f"Book '{self.title}' returned successfully"
        else:
            return f"Book '{self.title}' was not borrowed"
    
    def __str__(self):
        status = "Borrowed" if self.is_borrowed else "Available"
        return f"'{self.title}' by {self.author} (ISBN: {self.isbn}) - {status}"

class Library:
    def __init__(self):
        self.books = []
    
    def add_book(self, book):
        self.books.append(book)
        return f"Book '{book.title}' added to library"
    
    def find_book(self, title):
        for book in self.books:
            if book.title.lower() == title.lower():
                return book
        return None
    
    def list_books(self):
        if not self.books:
            print("No books in library")
            return
        
        print("Library Books:")
        for i, book in enumerate(self.books, 1):
            print(f"{i}. {book}")

# Main program
def main():
    print("=== Library Management System ===")
    
    # Create library
    library = Library()
    
    # Add some books
    book1 = Book("The Python Guide", "Guido van Rossum", "978-0134853987")
    book2 = Book("Clean Code", "Robert Martin", "978-0132350884")
    book3 = Book("Design Patterns", "Gang of Four", "978-0201633612")
    
    print(library.add_book(book1))
    print(library.add_book(book2))
    print(library.add_book(book3))
    
    print("\n--- All Books ---")
    library.list_books()
    
    print("\n--- Borrowing a Book ---")
    book = library.find_book("The Python Guide")
    if book:
        print(book.borrow())
    
    print("\n--- After Borrowing ---")
    library.list_books()
    
    print("\n--- Returning a Book ---")
    if book:
        print(book.return_book())
    
    print("\n--- After Returning ---")
    library.list_books()

if __name__ == "__main__":
    main()
```

### Running the Library Program

```bash
tauraro run library.tr
```

This will demonstrate:
- Class definition and instantiation
- Method calls
- Object state management
- String representation methods

## Working with Files

Tauraro supports standard file operations:

```python
# file_operations.tr - Working with files

# Writing to a file
with open("example.txt", "w") as file:
    file.write("Hello, Tauraro!\n")
    file.write("This is a test file.\n")

# Reading from a file
with open("example.txt", "r") as file:
    content = file.read()
    print("File content:")
    print(content)

# Reading lines
with open("example.txt", "r") as file:
    lines = file.readlines()
    for i, line in enumerate(lines, 1):
        print(f"Line {i}: {line.strip()}")
```

## Compilation Backends

Tauraro supports multiple compilation backends:

### Interpreter Mode (Default)
```bash
tauraro run program.tr
```

### C Transpilation
```bash
tauraro compile program.tr --backend c
./program
```

### LLVM Compilation
```bash
tauraro compile program.tr --backend llvm
./program
```

### WebAssembly
```bash
tauraro compile program.tr --backend wasm
```

## Next Steps

Now that you've completed the getting started guide, here are some recommended next steps:

1. **Explore Examples**: Check the `examples/` directory for more comprehensive examples
2. **Read Documentation**: Dive into the [Language Reference](language-reference.md) for detailed syntax information
3. **Practice**: Try implementing your own programs using the concepts learned
4. **Join Community**: Connect with other Tauraro developers for support and collaboration

### Recommended Learning Path

1. **Basic Syntax**: Variables, control flow, functions
2. **Data Structures**: Lists, dictionaries, sets, tuples
3. **Object-Oriented Programming**: Classes, inheritance, polymorphism
4. **Advanced Features**: Error handling, file I/O, modules
5. **Performance Optimization**: Profiling and optimization techniques

### Resources

- **[Language Reference](language-reference.md)**: Complete syntax and feature documentation
- **[API Reference](api-reference.md)**: Built-in functions and standard library
- **[Examples Directory](../examples/)**: Practical code samples
- **[Community Forum](https://github.com/yourusername/tauraro/discussions)**: Ask questions and share knowledge

Happy coding with TauraroLang! 🚀