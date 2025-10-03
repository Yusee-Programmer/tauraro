# String Operations Benchmark for Python
# Tests string manipulation performance

print("Starting Python String Benchmark...")

# String operations benchmark
iterations = 10000

# String concatenation benchmark
i = 0
result = ""
while i < iterations:
    result = "Hello" + " World"
    i = i + 1

print("String concatenation completed")

# String comparison benchmark
i = 0
while i < iterations:
    result = "Hello" == "World"
    i = i + 1

print("String comparison completed")

# String length benchmark
i = 0
while i < iterations:
    result = len("Hello World")
    i = i + 1

print("String length completed")

# String uppercase benchmark
i = 0
while i < iterations:
    result = "hello".upper()
    i = i + 1

print("String uppercase completed")

# String substring benchmark
i = 0
while i < iterations:
    result = "Hello World"[0:5]
    i = i + 1

print("String substring completed")

print("String benchmark completed. Final result:", result)
print("Python String Benchmark finished!")