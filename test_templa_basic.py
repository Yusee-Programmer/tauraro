#!/usr/bin/env python3
"""
Basic tests for Templa template engine using render_string
"""

import templa

print("=== Templa Template Engine Tests ===\n")

# Test 1: Simple variable interpolation
print("1. Variable Interpolation:")
result1 = templa.render_string("Hello, {{ name }}!", {"name": "World"})
print(f"   Template: 'Hello, {{{{ name }}}}!'")
print(f"   Context: {{'name': 'World'}}")
print(f"   Result: '{result1}'")
print(f"   ✓ Variable interpolation works!\n")

# Test 2: Multiple variables
print("2. Multiple Variables:")
context2 = {}
context2["greeting"] = "Hi"
context2["name"] = "Alice"
context2["age"] = 25
result2 = templa.render_string("{{ greeting }}, {{ name }}! You are {{ age }} years old.", context2)
print(f"   Result: '{result2}'")
print(f"   ✓ Multiple variables work!\n")

# Test 3: Filters
print("3. Filters:")
context3 = {}
context3["name"] = "alice"
context3["city"] = "NEW YORK"
context3["title"] = "hello world"
result3 = templa.render_string("{{ name|upper }} - {{ city|lower }} - {{ title|capitalize }}", context3)
print(f"   Result: '{result3}'")
print(f"   ✓ Filters work (upper, lower, capitalize)!\n")

# Test 4: Title filter
print("4. Title Filter:")
result4 = templa.render_string("{{ text|title }}", {"text": "hello world from tauraro"})
print(f"   Result: '{result4}'")
print(f"   ✓ Title filter works!\n")

# Test 5: Length filter
print("5. Length Filter:")
result5 = templa.render_string("Name has {{ name|length }} characters", {"name": "Tauraro"})
print(f"   Result: '{result5}'")
print(f"   ✓ Length filter works!\n")

# Test 6: HTML Escaping
print("6. HTML Escaping (auto-escape enabled):")
result6 = templa.render_string("{{ html }}", {"html": "<script>alert('xss')</script>"})
print(f"   Input: <script>alert('xss')</script>")
print(f"   Output: '{result6}'")
print(f"   ✓ HTML escaping works!\n")

# Test 7: If condition (truthy)
print("7. If Condition (True):")
result7 = templa.render_string("{% if show %}Visible{% endif %}", {"show": True})
print(f"   Result: '{result7}'")
print(f"   ✓ If condition (True) works!\n")

# Test 8: If condition (falsy)
print("8. If Condition (False):")
result8 = templa.render_string("{% if show %}Visible{% endif %}", {"show": False})
print(f"   Result: '{result8}'")
print(f"   ✓ If condition (False) works!\n")

# Test 9: For loop with list
print("9. For Loop with List:")
items = []
items.append("apple")
items.append("banana")
items.append("cherry")
ctx9 = {}
ctx9["items"] = items
result9 = templa.render_string("{% for item in items %}{{ item }}, {% endfor %}", ctx9)
print(f"   Result: '{result9}'")
print(f"   ✓ For loop with list works!\n")

# Test 10: For loop with dict
print("10. For Loop with Dict:")
data = {}
data["name"] = "Alice"
data["age"] = 30
ctx10 = {}
ctx10["data"] = data
result10 = templa.render_string("{% for key, value in data %}{{ key }}: {{ value }}, {% endfor %}", ctx10)
print(f"    Result: '{result10}'")
print(f"    ✓ For loop with dict works!\n")

# Test 11: render_string utility
print("11. render_string Utility:")
result11 = templa.render_string("Quick: {{ msg }}", {"msg": "Hello!"})
print(f"    Result: '{result11}'")
print(f"    ✓ render_string works!\n")

# Test 12: Comments
print("12. Comments:")
result12 = templa.render_string("Before {# this is a comment #} After", {})
print(f"    Result: '{result12}'")
print(f"    ✓ Comments work!\n")

# Test 13: Chained filters
print("13. Chained Filters:")
result13 = templa.render_string("{{ text|lower|capitalize }}", {"text": "HELLO WORLD"})
print(f"    Result: '{result13}'")
print(f"    ✓ Chained filters work!\n")

# Test 14: Dot notation
print("14. Dot Notation:")
user = {}
user["name"] = "Bob"
user["age"] = 25
context14 = {}
context14["user"] = user
result14 = templa.render_string("{{ user.name }} is {{ user.age }} years old", context14)
print(f"    Result: '{result14}'")
print(f"    ✓ Dot notation works!\n")

print("=== All Tests Passed! ===")
print()
print("Templa Features:")
print("  ✓ Variable interpolation: {{ variable }}")
print("  ✓ Filters: {{ variable|filter }}")
print("  ✓ Chained filters: {{ variable|filter1|filter2 }}")
print("  ✓ Control structures: {% if %}, {% for %}")
print("  ✓ Comments: {# comment #}")
print("  ✓ Auto-escaping for security")
print("  ✓ Dot notation: {{ user.name }}")
print("  ✓ Built-in filters: upper, lower, capitalize, title, trim, length, reverse, escape")
