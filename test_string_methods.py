# Test new string methods for Tauraro
print("Testing String Methods\n" + "="*50)

# Test startswith()
print("\n=== startswith() ===")
s = "/api/user/123"
print(f"'{s}'.startswith('/api/') = {s.startswith('/api/')}")
print(f"'{s}'.startswith('/admin/') = {s.startswith('/admin/')}")
assert s.startswith("/api/") == True
assert s.startswith("/admin/") == False

# Test endswith()
print("\n=== endswith() ===")
s = "hello.py"
print(f"'{s}'.endswith('.py') = {s.endswith('.py')}")
print(f"'{s}'.endswith('.js') = {s.endswith('.js')}")
assert s.endswith(".py") == True
assert s.endswith(".js") == False

# Test split()
print("\n=== split() ===")
path = "/api/user/123"
parts = path.split("/")
print(f"'{path}'.split('/') = {parts}")
assert len(parts) == 4
assert parts[0] == ""
assert parts[1] == "api"
assert parts[2] == "user"
assert parts[3] == "123"

# Test split with whitespace
words = "hello world foo bar"
result = words.split()
print(f"'{words}'.split() = {result}")
assert len(result) == 4

# Test join()
print("\n=== join() ===")
items = ["a", "b", "c"]
joined = ", ".join(items)
print(f"', '.join(['a', 'b', 'c']) = '{joined}'")
assert joined == "a, b, c"

# Test replace()
print("\n=== replace() ===")
s = "hello world"
result = s.replace("world", "Python")
print(f"'{s}'.replace('world', 'Python') = '{result}'")
assert result == "hello Python"

# Test find()
print("\n=== find() ===")
s = "hello world"
pos = s.find("world")
print(f"'{s}'.find('world') = {pos}")
assert pos == 6

notfound = s.find("xyz")
print(f"'{s}'.find('xyz') = {notfound}")
assert notfound == -1

# Test count()
print("\n=== count() ===")
s = "hello hello world"
count = s.count("hello")
print(f"'{s}'.count('hello') = {count}")
assert count == 2

print("\n" + "="*50)
print("✅ All string method tests passed!")
