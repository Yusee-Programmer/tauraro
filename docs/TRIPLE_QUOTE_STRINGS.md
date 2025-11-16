# Triple-Quote Strings in Tauraro

## Overview
Tauraro supports Python-style triple-quoted strings (`"""` or `'''`) for multi-line string literals. This feature is particularly useful for:
- Embedding HTML content
- Writing multi-line JavaScript code
- Creating formatted text blocks
- Docstrings

## Syntax

### Double Quotes
```python
html = """
<div class="container">
    <h1>Hello World</h1>
    <p>Multi-line content</p>
</div>
"""
```

### Single Quotes
```python
text = '''
Line 1
Line 2
Line 3
'''
```

## Use Cases

### 1. HTML Templates
```python
from webviewtk import Window, Text, mount_and_run

html_content = """
<div class="flex flex-col p-6">
    <h1 class="text-2xl font-bold">Dashboard</h1>
    <p class="text-gray-600">Welcome to the app</p>
</div>
"""

window = Window(title="HTML Example", width=800, height=600)
ui = Text(text=html_content, raw_html=True)
mount_and_run(window, ui)
```

### 2. JavaScript Code
```python
from webviewtk import add_custom_js

js_code = """
function handleClick(event) {
    console.log('Button clicked!');
    alert('Hello from JavaScript!');
}

document.addEventListener('DOMContentLoaded', function() {
    console.log('Page loaded');
});
"""

add_custom_js(window, js_code)
```

### 3. SQL Queries
```python
query = """
SELECT users.name, orders.total
FROM users
INNER JOIN orders ON users.id = orders.user_id
WHERE orders.status = 'completed'
ORDER BY orders.total DESC
LIMIT 10
"""
```

### 4. Configuration Files
```python
config_yaml = """
server:
  host: localhost
  port: 8080
  
database:
  driver: postgresql
  host: db.example.com
  name: myapp_db
"""
```

## Features

### Preserves Formatting
Triple-quote strings preserve all whitespace, indentation, and newlines exactly as written:

```python
poem = """
    Roses are red,
    Violets are blue,
    Tauraro is great,
    And so are you!
"""
print(poem)  # Outputs with leading spaces preserved
```

### Embedded Quotes
You can include single or double quotes without escaping:

```python
html = """
<button onclick="alert('Hello!')">Click Me</button>
<p class="text">She said, "Hello world!"</p>
"""
```

### Newline Handling
The string includes all newlines in the source:

```python
text = """Line 1
Line 2
Line 3"""
# Result: "Line 1\nLine 2\nLine 3"
```

## Implementation Details

- Triple-quote strings are processed during lexical analysis
- Both `"""` and `'''` are supported
- Closing quotes must match opening quotes (can't mix `"""` and `'''`)
- Content between triple-quotes is captured exactly, including:
  - Leading/trailing whitespace
  - Blank lines
  - Special characters
  - Nested quotes

## Examples in WebViewTK

### Modern Desktop App
```python
from webviewtk import Window, Column, Text, CustomTitleBar, mount_and_run

# Large HTML template
app_html = """
<div class="h-screen flex flex-col bg-gray-50">
    <header class="bg-white shadow-sm p-4">
        <h1 class="text-2xl font-bold">My App</h1>
    </header>
    <main class="flex-1 p-6">
        <div class="max-w-4xl mx-auto">
            <!-- Content here -->
        </div>
    </main>
</div>
"""

# JavaScript for interactivity
app_js = """
function initApp() {
    document.querySelectorAll('.button').forEach(btn => {
        btn.addEventListener('click', handleButtonClick);
    });
}

function handleButtonClick(event) {
    console.log('Button clicked:', event.target.textContent);
}

window.addEventListener('DOMContentLoaded', initApp);
"""

window = Window(title="Modern App", width=1200, height=800)
add_custom_js(window, app_js)

ui = Column(children=[
    CustomTitleBar(title="Modern App"),
    Text(text=app_html, raw_html=True)
])

mount_and_run(window, ui)
```

## Best Practices

1. **Use for Multi-Line Content**: Reserve triple-quotes for content that spans multiple lines
2. **Consistent Indentation**: While not required, maintaining consistent indentation improves readability
3. **Choose Quote Style**: Use `"""` for content with single quotes, `'''` for content with double quotes
4. **Avoid Mixing**: Don't mix `"""` and `'''` for opening/closing the same string

## Common Pitfalls

### ❌ Mismatched Quotes
```python
# This will fail - mismatched quote types
text = """This is wrong'''
```

### ✅ Correct Usage
```python
# Opening and closing quotes must match
text = """This is correct"""
# OR
text = '''This is also correct'''
```

### ❌ Accidental Closing
```python
# Be careful with nested triple-quotes in content
html = """
<div data-content='"""'>  # This will prematurely close the string!
</div>
"""
```

### ✅ Workaround
```python
# Use the other quote style or escape
html = '''
<div data-content='"""'>  # Safe because using ''' wrapper
</div>
'''
```

## Summary

Triple-quote strings in Tauraro provide a clean, readable way to embed multi-line content in your code. They're essential for modern desktop app development with WebViewTK, where HTML, CSS, and JavaScript often need to be embedded directly in Tauraro files.
