def main():
    print("Testing complex Python features...")
    
    # Test arithmetic operations
    print("Testing arithmetic operations:")
    x = 100
    y = 7
    print("100 + 7 =", x + y)
    print("100 - 7 =", x - y)
    print("100 * 7 =", x * y)
    print("100 / 7 =", x / y)
    print("100 % 7 =", x % y)
    
    # Test comparison operations
    print("Testing comparison operations:")
    print("100 == 7:", x == y)
    print("100 != 7:", x != y)
    print("100 > 7:", x > y)
    print("100 < 7:", x < y)
    print("100 >= 7:", x >= y)
    print("100 <= 7:", x <= y)
    
    # Test logical operations
    print("Testing logical operations:")
    a = True
    b = False
    c = True
    print("True and False =", a and b)
    print("True or False =", a or b)
    print("not True =", not a)
    print("(True and False) or (not False and True) =", (a and b) or (not b and c))
    
    # Test control flow
    print("Testing control flow:")
    print("For loop:")
    for i in range(3):
        print("Iteration", i)
    
    print("While loop:")
    counter = 0
    while counter < 3:
        print("While iteration", counter)
        counter = counter + 1
    
    # Test conditionals
    print("Testing conditionals:")
    if x > y:
        print("100 is greater than 7")
    elif x == y:
        print("100 is equal to 7")
    else:
        print("100 is less than 7")
    
    print("Complex Python features test completed!")

if __name__ == "__main__":
    main()