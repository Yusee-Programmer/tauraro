import time

def add_numbers(x, y):
    return x + y

def main():
    print("Starting comprehensive Python test...")
    
    # Test arithmetic operations
    print("Testing arithmetic operations...")
    a = 10
    b = 3
    print("Addition:", a + b)
    print("Subtraction:", a - b)
    print("Multiplication:", a * b)
    print("Division:", a / b)
    print("Modulo:", a % b)
    
    # Test comparison operations
    print("Testing comparison operations...")
    print("Equal:", a == b)
    print("Not equal:", a != b)
    print("Less than:", a < b)
    print("Greater than:", a > b)
    print("Less or equal:", a <= b)
    print("Greater or equal:", a >= b)
    
    # Test logical operations
    print("Testing logical operations...")
    x = True
    y = False
    print("And:", x and y)
    print("Or:", x or y)
    print("Not:", not x)
    
    # Test control flow
    print("Testing control flow...")
    print("For loop iterations:")
    for i in range(5):
        print("For loop iteration:", i)
    
    print("While loop iterations:")
    counter = 0
    while counter < 3:
        print("While loop iteration:", counter)
        counter = counter + 1
    
    # Test conditional statements
    print("Conditional statements:")
    if a > b:
        print("a is greater than b")
    elif a == b:
        print("a is equal to b")
    else:
        print("a is less than b")
    
    # Test function calls
    print("Testing function calls...")
    result = add_numbers(5, 7)
    print("Function call result:", result)
    
    print("Comprehensive Python test completed!")

if __name__ == "__main__":
    main()