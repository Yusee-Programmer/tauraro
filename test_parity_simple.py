# Test 1: Extended Unpacking
print("Test 1: Extended Unpacking")
a, *b, c = [1, 2, 3, 4, 5]
print("a =", a)
print("b =", b)
print("c =", c)

# Test 2: eval()
print("\nTest 2: eval()")
result = eval("2 + 3")
print("eval('2 + 3') =", result)

# Test 3: exec()
print("\nTest 3: exec()")
exec("x = 10")
print("After exec('x = 10'), x =", x)

# Test 4: compile()
print("\nTest 4: compile()")
code_obj = compile("5 * 5", "<test>", "eval")
result = eval(code_obj)
print("eval(compile('5 * 5')) =", result)

# Test 5: Property
print("\nTest 5: Property")
class Temp:
    def __init__(self, val):
        self._value = val

    def get_value(self):
        return self._value

    def set_value(self, val):
        self._value = val

    value = property(get_value, set_value)

t = Temp(100)
print("t.value =", t.value)
t.value = 200
print("After t.value = 200, t.value =", t.value)

print("\n✓ All tests completed!")
