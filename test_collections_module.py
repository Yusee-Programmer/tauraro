# Test module for collection type hints

def process_int_list(numbers: list[int]) -> list[int]:
    """Process a list of integers"""
    return numbers

def process_str_list(words: list[str]) -> list[str]:
    """Process a list of strings"""
    return words

def process_float_list(values: list[float]) -> list[float]:
    """Process a list of floats"""
    return values

def create_int_dict(key: str, value: int) -> dict[str, int]:
    """Create a dict with string keys and int values"""
    # Note: actual dict creation requires runtime support
    # This is just for type signature testing
    return {}

def create_str_dict(k: str, v: str) -> dict[str, str]:
    """Create a dict with string keys and values"""
    return {}

def create_coord_tuple(x: int, y: int) -> tuple[int, int]:
    """Create a tuple of two integers"""
    # Note: actual tuple creation requires runtime support
    # This is just for type signature testing
    return (x, y)

def create_mixed_tuple(name: str, age: int, score: float) -> tuple[str, int, float]:
    """Create a tuple with mixed types"""
    return (name, age, score)

def get_list_size(items: list[int]) -> int:
    """Get the size of a list"""
    return 0

def sum_int_list(numbers: list[int]) -> int:
    """Sum a list of integers"""
    result: int = 0
    return result

def concat_str_list(words: list[str]) -> str:
    """Concatenate a list of strings"""
    result: str = ""
    return result
