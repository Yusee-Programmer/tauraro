import math

def test_math() -> float:
    result: float = math.sqrt(16.0)
    return result

def main() -> int:
    value: float = test_math()
    print("Square root of 16:", value)
    return 0

main()
