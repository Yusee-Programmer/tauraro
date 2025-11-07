# Test multi-line collections

# Test 1: Multi-line list
myList = [
    1,
    2,
    3
]
print("List:", myList)

# Test 2: Multi-line dict
config = {
    "log_level": "info",
    "reload": False,
    "workers": 1
}
print("Dict:", config)

# Test 3: Multi-line tuple
myTuple = (
    "a",
    "b",
    "c"
)
print("Tuple:", myTuple)

# Test 4: Multi-line set
mySet = {
    1,
    2,
    3
}
print("Set:", mySet)

# Test 5: Multi-line function call with dict
def test_func(name, port, options):
    print("Function called:", name, port, options)

test_func("server", 8080, {
    "log_level": "info",
    "reload": False,
    "workers": 1
})

print("All tests completed!")
