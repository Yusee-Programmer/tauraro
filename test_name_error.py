# Test NameError with traceback

def level3():
    return undefined_variable + 5

def level2():
    return level3()

def level1():
    return level2()

result = level1()
print(result)
