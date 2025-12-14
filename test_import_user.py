import myutils

def main() -> int:
    result: int = myutils.add(10, 20)
    product: int = myutils.multiply(5, 6)
    greeting: str = myutils.greet("World")

    print("Result:", result)
    print("Product:", product)
    print(greeting)

    return 0

main()
