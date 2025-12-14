# Comprehensive type hint test module

def test_int(x: int, y: int) -> int:
    """Test integer types"""
    return x + y

def test_float(x: float, y: float) -> float:
    """Test floating point types"""
    return x * y

def test_bool(flag: bool) -> bool:
    """Test boolean types"""
    return not flag

def test_string(text: str) -> str:
    """Test string types"""
    return "Result: " + text

def test_mixed(a: int, b: float, c: str) -> float:
    """Test mixed parameter types"""
    result: float = float(a) + b
    return result

def test_no_annotation(x, y):
    """Test function without type annotations"""
    return x + y

def calculate_area(width: int, height: int) -> int:
    """Calculate rectangle area"""
    area: int = width * height
    return area

def is_positive(num: int) -> bool:
    """Check if number is positive"""
    return num > 0

def format_message(name: str, age: int) -> str:
    """Format a message with name and age"""
    return name + " is " + str(age) + " years old"

def compute_average(total: float, count: int) -> float:
    """Compute average"""
    if count == 0:
        return 0.0
    avg: float = total / float(count)
    return avg
