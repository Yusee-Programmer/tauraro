#!/usr/bin/env python3
"""
Simple test for Templa template engine using render_string
"""

import templa

print("=== Templa Template Engine Tests ===\n")

# Test 1: Simple variable
print("1. Variable Interpolation:")
result1 = templa.render_string("Hello, {{ name }}!", {"name": "World"})
print(f"   Result: {result1}")

# Test 2: Multiple variables
print("\n2. Multiple Variables:")
result2 = templa.render_string("{{ greeting }}, {{ name }}!", {"greeting": "Hi", "name": "Alice"})
print(f"   Result: {result2}")

# Test 3: Upper filter
print("\n3. Upper Filter:")
result3 = templa.render_string("{{ name|upper }}", {"name": "alice"})
print(f"   Result: {result3}")

# Test 4: Lower filter
print("\n4. Lower Filter:")
result4 = templa.render_string("{{ name|lower }}", {"name": "ALICE"})
print(f"   Result: {result4}")

# Test 5: Capitalize filter
print("\n5. Capitalize Filter:")
result5 = templa.render_string("{{ text|capitalize }}", {"text": "hello"})
print(f"   Result: {result5}")

# Test 6: Title filter
print("\n6. Title Filter:")
result6 = templa.render_string("{{ text|title }}", {"text": "hello world"})
print(f"   Result: {result6}")

# Test 7: Length filter
print("\n7. Length Filter:")
result7 = templa.render_string("Length: {{ name|length }}", {"name": "Tauraro"})
print(f"   Result: {result7}")

# Test 8: If condition
print("\n8. If Condition (True):")
result8 = templa.render_string("{% if show %}Visible{% endif %}", {"show": True})
print(f"   Result: {result8}")

# Test 9: If condition (False)
print("\n9. If Condition (False):")
result9 = templa.render_string("{% if show %}Hidden{% endif %}", {"show": False})
print(f"   Result: {result9}")

# Test 10: For loop with list
print("\n10. For Loop:")
result10 = templa.render_string("{% for item in items %}{{ item }} {% endfor %}", {"items": ["a", "b", "c"]})
print(f"    Result: {result10}")

# Test 11: For loop with dict
print("\n11. For Loop (Dict):")
result11 = templa.render_string("{% for k, v in data %}{{ k }}={{ v }} {% endfor %}", {"data": {"x": 1, "y": 2}})
print(f"    Result: {result11}")

# Test 12: Comments
print("\n12. Comments:")
result12 = templa.render_string("Before{# comment #}After", {})
print(f"    Result: {result12}")

# Test 13: HTML Escaping
print("\n13. HTML Escaping:")
result13 = templa.render_string("{{ html }}", {"html": "<script>alert('xss')</script>"})
print(f"    Result: {result13}")

# Test 14: Chained filters
print("\n14. Chained Filters:")
result14 = templa.render_string("{{ text|upper|reverse }}", {"text": "hello"})
print(f"    Result: {result14}")

# Test 15: Dot notation
print("\n15. Dot Notation:")
result15 = templa.render_string("{{ user.name }}", {"user": {"name": "Bob"}})
print(f"    Result: {result15}")

print("\n=== All Tests Complete! ===")
