# Direct test for collection type hints

def test_list_param(numbers: list[int]) -> int:
    """Test list parameter"""
    return 0

def test_dict_param(data: dict[str, int]) -> int:
    """Test dict parameter"""
    return 0

def test_tuple_return(x: int, y: int) -> tuple[int, int]:
    """Test tuple return"""
    return (x, y)

def main() -> int:
    # Call functions with collection type signatures
    result1: int = test_list_param([])
    result2: int = test_dict_param({})
    coords: tuple[int, int] = test_tuple_return(10, 20)

    print("Collection types compiled!")
    return 0

main()
