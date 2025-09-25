# String Operations Benchmark for Python
# Tests string manipulation performance

print("Starting Python String Benchmark...")

# String operations benchmark
iterations = 5000

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

print("String benchmark completed")
print("Python String Benchmark finished!")