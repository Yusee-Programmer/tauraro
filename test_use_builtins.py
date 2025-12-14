import test_builtins_module

def main() -> int:
    print("Testing built-in functions in user modules...")

    # Test print functions
    test_builtins_module.test_print_int(42)
    test_builtins_module.test_print_float(3.14)
    test_builtins_module.test_print_string("Hello from module!")
    test_builtins_module.test_print_bool(True)

    # Test str conversions
    str_result: str = test_builtins_module.test_str_conversion_int(100)
    print(str_result)

    # Test int conversions
    int_result: int = test_builtins_module.test_int_conversion_float(9.99)
    print(int_result)

    # Test float conversions
    float_result: float = test_builtins_module.test_float_conversion_int(42)
    print(float_result)

    # Test len function
    length: int = test_builtins_module.test_len_string("Hello World")
    print(length)

    # Test mixed operations
    mixed_result: str = test_builtins_module.test_mixed_operations(10, 3.14, "Test")
    print(mixed_result)

    # Test arithmetic with conversions
    division_result: float = test_builtins_module.test_arithmetic_with_conversions(10, 3)
    print(division_result)

    # Test string operations
    concat_result: str = test_builtins_module.test_string_operations("Hello", "World")
    print(concat_result)

    print("All built-in function tests completed!")
    return 0

main()
