#!/usr/bin/env tauraro

print("Testing import inside function...")

def test_function():
    print("Inside function, importing abc...")
    import abc
    print("abc imported:", abc)
    print("Type of abc:", type(abc))

    # Test hasattr
    result = hasattr(abc, 'ABCMeta')
    print("hasattr(abc, 'ABCMeta'):", result)

    if not result:
        raise Exception("ABCMeta not found")

    print("Success!")
    return True

# Call the function
result = test_function()
print("Function returned:", result)
