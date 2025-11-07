# Test multi-line strings (docstrings)

# Test 1: Simple multi-line string
text = """This is a
multi-line
string"""
print("Test 1:", text)

# Test 2: Multi-line string with quotes
html = """<h1 style="color: blue;">Hello</h1>
<p>This is a "quoted" word</p>"""
print("Test 2:", html)

# Test 3: Multi-line with single quotes
text2 = '''This is also
a multi-line
string'''
print("Test 3:", text2)

# Test 4: Empty lines in string
text3 = """Line 1

Line 3"""
print("Test 4:", text3)

# Test 5: HTML doc with styles
html_doc = """<!DOCTYPE html>
<html>
<head>
    <title>Test</title>
</head>
<body>
    <h1 style="color: red;">Header</h1>
</body>
</html>"""
print("Test 5:", html_doc)

print("\nAll docstring tests passed!")
