# Test advanced features: type conversions, string operations, arithmetic
def test_type_conversions() -> int:
    # Test int to float
    x: int = 42
    xf: float = float(x)
    print(xf)

    # Test float to int
    y: float = 3.14
    yi: int = int(y)
    print(yi)

    # Test int to string
    z: int = 100
    zs: str = str(z)
    print(zs)

    # Test string length
    msg: str = "Hello World"
    length: int = len(msg)
    print(length)

    return 0

def test_arithmetic() -> int:
    a: int = 10
    b: int = 3

    # All arithmetic operations
    add: int = a + b
    sub: int = a - b
    mul: int = a * b
    div: int = a / b

    print(add)
    print(sub)
    print(mul)
    print(div)

    # Float arithmetic
    x: float = 10.5
    y: float = 2.5

    fadd: float = x + y
    fsub: float = x - y
    fmul: float = x * y
    fdiv: float = x / y

    print(fadd)
    print(fsub)
    print(fmul)
    print(fdiv)

    return 0

def test_string_ops() -> int:
    # String concatenation
    first: str = "Hello"
    second: str = "World"
    combined: str = first + " " + second
    print(combined)

    # Mixed string and int
    num: int = 42
    numstr: str = str(num)
    result: str = "The answer is " + numstr
    print(result)

    return 0

def main() -> int:
    print("Testing type conversions...")
    test_type_conversions()

    print("Testing arithmetic...")
    test_arithmetic()

    print("Testing string operations...")
    test_string_ops()

    print("All tests completed!")
    return 0

main()
