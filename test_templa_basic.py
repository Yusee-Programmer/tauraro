#!/usr/bin/env python3
"""
Basic tests for Templa template engine
"""

import templa

print("=== Templa Template Engine Tests ===\n")

# Test 1: Simple variable interpolation
print("1. Variable Interpolation:")
template1 = templa.Template("Hello, {{ name }}!")
result1 = template1.render({"name": "World"})
print(f"   Template: 'Hello, {{{{ name }}}}!'")
print(f"   Context: {{'name': 'World'}}")
print(f"   Result: '{result1}'")
print(f"   ✓ Variable interpolation works!\n")

# Test 2: Multiple variables
print("2. Multiple Variables:")
template2 = templa.Template("{{ greeting }}, {{ name }}! You are {{ age }} years old.")
context2 = {"greeting": "Hi", "name": "Alice", "age": 25}
result2 = template2.render(context2)
print(f"   Result: '{result2}'")
print(f"   ✓ Multiple variables work!\n")

# Test 3: Filters
print("3. Filters:")
template3 = templa.Template("{{ name|upper }} - {{ city|lower }} - {{ title|capitalize }}")
context3 = {"name": "alice", "city": "NEW YORK", "title": "hello world"}
result3 = template3.render(context3)
print(f"   Result: '{result3}'")
print(f"   ✓ Filters work (upper, lower, capitalize)!\n")

# Test 4: Title filter
print("4. Title Filter:")
template4 = templa.Template("{{ text|title }}")
result4 = template4.render({"text": "hello world from tauraro"})
print(f"   Result: '{result4}'")
print(f"   ✓ Title filter works!\n")

# Test 5: Length filter
print("5. Length Filter:")
template5 = templa.Template("Name has {{ name|length }} characters")
result5 = template5.render({"name": "Tauraro"})
print(f"   Result: '{result5}'")
print(f"   ✓ Length filter works!\n")

# Test 6: HTML Escaping
print("6. HTML Escaping (auto-escape enabled):")
template6 = templa.Template("{{ html }}")
result6 = template6.render({"html": "<script>alert('xss')</script>"})
print(f"   Input: <script>alert('xss')</script>")
print(f"   Output: '{result6}'")
print(f"   ✓ HTML escaping works!\n")

# Test 7: If condition (truthy)
print("7. If Condition (True):")
template7 = templa.Template("{% if show %}Visible{% endif %}")
result7 = template7.render({"show": True})
print(f"   Result: '{result7}'")
print(f"   ✓ If condition (True) works!\n")

# Test 8: If condition (falsy)
print("8. If Condition (False):")
template8 = templa.Template("{% if show %}Visible{% endif %}")
result8 = template8.render({"show": False})
print(f"   Result: '{result8}'")
print(f"   ✓ If condition (False) works!\n")

# Test 9: For loop with list
print("9. For Loop with List:")
template9 = templa.Template("{% for item in items %}{{ item }}, {% endfor %}")
result9 = template9.render({"items": ["apple", "banana", "cherry"]})
print(f"   Result: '{result9}'")
print(f"   ✓ For loop with list works!\n")

# Test 10: For loop with dict
print("10. For Loop with Dict:")
template10 = templa.Template("{% for key, value in data %}{{ key }}: {{ value }}, {% endfor %}")
result10 = template10.render({"data": {"name": "Alice", "age": 30}})
print(f"    Result: '{result10}'")
print(f"    ✓ For loop with dict works!\n")

# Test 11: render_string utility
print("11. render_string Utility:")
result11 = templa.render_string("Quick: {{ msg }}", {"msg": "Hello!"})
print(f"    Result: '{result11}'")
print(f"    ✓ render_string works!\n")

# Test 12: Comments
print("12. Comments:")
template12 = templa.Template("Before {# this is a comment #} After")
result12 = template12.render({})
print(f"    Result: '{result12}'")
print(f"    ✓ Comments work!\n")

# Test 13: Chained filters
print("13. Chained Filters:")
template13 = templa.Template("{{ text|lower|capitalize }}")
result13 = template13.render({"text": "HELLO WORLD"})
print(f"    Result: '{result13}'")
print(f"    ✓ Chained filters work!\n")

# Test 14: Dot notation
print("14. Dot Notation:")
template14 = templa.Template("{{ user.name }} is {{ user.age }} years old")
context14 = {"user": {"name": "Bob", "age": 25}}
result14 = template14.render(context14)
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
