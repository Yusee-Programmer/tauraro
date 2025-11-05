# Test eval separately
print("Testing eval")
try:
    result = eval("2 + 3")
    print("Result:", result)
except Exception as e:
    print("Error:", e)
