# Comprehensive test module for built-in functions with native types

def test_print_int(value: int) -> int:
    """Test print with int"""
    print(value)
    return value

def test_print_float(value: float) -> float:
    """Test print with float"""
    print(value)
    return value

def test_print_string(message: str) -> str:
    """Test print with string"""
    print(message)
    return message

def test_print_bool(flag: bool) -> bool:
    """Test print with bool"""
    print(flag)
    return flag

def test_str_conversion_int(num: int) -> str:
    """Test str() with int"""
    result: str = str(num)
    return result

def test_str_conversion_float(num: float) -> str:
    """Test str() with float"""
    result: str = str(num)
    return result

def test_str_conversion_bool(flag: bool) -> str:
    """Test str() with bool"""
    result: str = str(flag)
    return result

def test_int_conversion_str(text: str) -> int:
    """Test int() with str"""
    result: int = int(text)
    return result

def test_int_conversion_float(num: float) -> int:
    """Test int() with float"""
    result: int = int(num)
    return result

def test_float_conversion_str(text: str) -> float:
    """Test float() with str"""
    result: float = float(text)
    return result

def test_float_conversion_int(num: int) -> float:
    """Test float() with int"""
    result: float = float(num)
    return result

def test_len_string(text: str) -> int:
    """Test len() with string"""
    length: int = len(text)
    return length

def test_mixed_operations(x: int, y: float, text: str) -> str:
    """Test mixed built-in function operations"""
    # Convert int to string
    x_str: str = str(x)

    # Convert float to int
    y_int: int = int(y)

    # Get string length
    text_len: int = len(text)

    # Print values
    print(x)
    print(y)
    print(text)

    # Return concatenated result
    result: str = x_str + " " + str(y_int) + " " + text
    return result

def test_arithmetic_with_conversions(a: int, b: int) -> float:
    """Test arithmetic with type conversions"""
    # Convert to float for division
    a_float: float = float(a)
    b_float: float = float(b)

    result: float = a_float / b_float
    return result

def test_string_operations(s1: str, s2: str) -> str:
    """Test string operations with len"""
    len1: int = len(s1)
    len2: int = len(s2)

    print(len1)
    print(len2)

    result: str = s1 + s2
    return result
