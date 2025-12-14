import test_types_module

def main() -> int:
    # Test int function
    int_result: int = test_types_module.test_int(10, 20)
    print("Integer test:", int_result)

    # Test float function
    float_result: float = test_types_module.test_float(3.5, 2.0)
    print("Float test:", float_result)

    # Test bool function
    bool_result: bool = test_types_module.test_bool(True)
    print("Bool test:", bool_result)

    # Test string function
    str_result: str = test_types_module.test_string("Hello")
    print(str_result)

    # Test mixed types
    mixed_result: float = test_types_module.test_mixed(5, 3.14, "test")
    print("Mixed test:", mixed_result)

    # Test area calculation
    area: int = test_types_module.calculate_area(10, 5)
    print("Area:", area)

    # Test positive check
    is_pos: bool = test_types_module.is_positive(42)
    print("Is positive:", is_pos)

    return 0

main()
