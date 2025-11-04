print("Testing new string methods:")

# Test isidentifier()
valid_id = "my_variable"
print("'my_variable'.isidentifier() =", valid_id.isidentifier())

# Test isascii()
ascii_str = "Hello"
print("'Hello'.isascii() =", ascii_str.isascii())

# Test partition()
text = "hello-world-python"
parts = text.partition("-")
print("'hello-world-python'.partition('-') =", parts)

# Test rpartition()
parts2 = text.rpartition("-")
print("'hello-world-python'.rpartition('-') =", parts2)

# Test expandtabs()
text2 = "a\tb"
expanded = text2.expandtabs(4)
print("'a\\tb'.expandtabs(4) =", expanded)
